#!/usr/bin/env python3
"""Drive a running PCSX-Redux instance playing FF9, for investigating what code and data do.

Runs on the host (not in Docker). See docs/emulator.md for the workflow. The emulator is
normally kept paused between commands so results are reproducible; commands that need time
to pass (frames, press, trace, watch, until) run a fixed number of frames and pause again.

Addresses accept hex (0x8006794C or 8006794C), symbol names from build/us/*.map and
config/symbols*.txt (func_80012345, D_8006794C, DrawSync), and an offset (D_8006794C+0x1C).
"""

import argparse
import json
import os
import re
import signal
import struct
import subprocess
import sys
import time
import urllib.error
import urllib.parse
import urllib.request
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
BIN = REPO / "tools/bin"
HOME = BIN / "redux-home"  # Redux portable dir: config, memcards, save states, logs
APPRUN = BIN / "pcsx-redux/squashfs-root/AppRun"
BIOS = BIN / "bios/SCPH1001.BIN"
BRIDGE = REPO / "tools/emu/bridge.lua"
DISCS = REPO / "disks/us/raw"
OVL_DIR = REPO / "disks/us/ovl"
MAP_DIR = REPO / "build/us"
SHOTS = HOME / "shots"
SNAPS = HOME / "snaps"
PIDFILE = HOME / "redux.pid"
LOGFILE = HOME / "redux.log"
PORT = int(os.environ.get("FF9_EMU_PORT", "8080"))
API = f"http://127.0.0.1:{PORT}/api/v1"

RAM_BASE = 0x80000000
RAM_SIZE = 0x200000
BUTTONS = ["SELECT", "START", "UP", "RIGHT", "DOWN", "LEFT", "L2", "R2", "L1", "R1",
           "TRIANGLE", "CIRCLE", "CROSS", "SQUARE"]
BUTTON_ALIASES = {"X": "CROSS", "O": "CIRCLE", "TRI": "TRIANGLE", "SQ": "SQUARE"}


class EmuError(Exception):
    pass


# ---------------------------------------------------------------- HTTP / Lua

def http(path, method="GET", data=None, timeout=10):
    req = urllib.request.Request(API + path, data=data, method=method)
    try:
        with urllib.request.urlopen(req, timeout=timeout) as r:
            return r.read()
    except urllib.error.HTTPError as e:
        raise EmuError(f"{method} {path}: HTTP {e.code} {e.read().decode(errors='replace').strip()}")
    except (urllib.error.URLError, ConnectionError, TimeoutError) as e:
        raise EmuError(f"emulator not reachable on port {PORT} ({e}); start it with `tools/emu.py start`")


def lua(code, timeout=30):
    # Redux rejects request lines over ~250 bytes, so send long code in pieces (see bridge.lua).
    pieces, piece = [], ""
    for ch in code:
        if len(urllib.parse.quote_plus(piece + ch)) > 180:
            pieces.append(piece)
            piece = ""
        piece += ch
    pieces.append(piece)
    for p in pieces[:-1]:
        http("/lua/eval?" + urllib.parse.urlencode({"append": p}), timeout=timeout)
    body = http("/lua/eval?" + urllib.parse.urlencode({"code": pieces[-1]}), timeout=timeout)
    try:
        resp = json.loads(body)
    except ValueError:
        raise EmuError(f"bad bridge response: {body[:200]!r}")
    if not resp.get("ok"):
        raise EmuError(f"lua: {resp.get('error')}")
    return resp.get("result")


def lua_lit(v):
    if isinstance(v, bool):
        return "true" if v else "false"
    if isinstance(v, int):
        return str(v)
    if isinstance(v, str):
        return json.dumps(v)  # JSON string escapes are valid Lua for our inputs
    if isinstance(v, (list, tuple)):
        return "{" + ",".join(lua_lit(x) for x in v) + "}"
    if v is None:
        return "nil"
    raise TypeError(v)


def bridge(fn, *args, timeout=30):
    return lua(f"return Bridge.{fn}({','.join(lua_lit(a) for a in args)})", timeout=timeout)


def flow():
    return json.loads(http("/execution-flow"))


def is_running():
    return flow()["running"]


def wait_paused(timeout):
    deadline = time.time() + timeout
    while time.time() < deadline:
        if not is_running():
            return True
        time.sleep(0.05)
    return False


def run_frames(n):
    """Run n frames from the current point and pause. Returns the frame counter."""
    if n <= 0:
        http("/execution-flow?function=pause", "POST")
        return bridge("status")["frame"]
    bridge("runFrames", n)
    # The interpreter runs FF9 at roughly real time or better; allow a generous margin.
    if not wait_paused(max(15.0, n / 15.0)):
        http("/execution-flow?function=pause", "POST")
        raise EmuError(f"timed out waiting for {n} frames; paused")
    return bridge("status")["frame"]


def ram():
    return http("/cpu/ram/raw", timeout=30)


def read_mem(addr, length):
    if length <= 0:
        return b""
    phys = addr & 0x1FFFFFFF
    if phys < 0x800000 and length > 0x1000:
        off = phys % RAM_SIZE
        if off + length > RAM_SIZE:
            raise EmuError("read runs past the end of RAM")
        return ram()[off:off + length]
    return bytes.fromhex(bridge("read", addr, length))


def write_mem(addr, data):
    return bridge("write", addr, data.hex())


# ---------------------------------------------------------------- symbols and overlays

def load_overlays():
    """{id: (vram, text_start, data_start)} from config/overlays.us.txt."""
    out = {}
    for line in (REPO / "config/overlays.us.txt").read_text().splitlines():
        line = line.split("#", 1)[0].split()
        if len(line) >= 4:
            out[line[0]] = tuple(int(x, 16) for x in line[1:4])
    return out


OVERLAYS = load_overlays()


def resident_overlays(mem=None):
    """Overlay ids whose .text currently sits in RAM, with the fraction of .text bytes matching."""
    mem = mem if mem is not None else ram()
    found = {}
    for ovl, (vram, text, data) in OVERLAYS.items():
        path = OVL_DIR / f"{ovl}.bin"
        if not path.exists():
            continue
        code = path.read_bytes()[text:data]
        off = (vram & 0x1FFFFFFF) + text
        live = mem[off:off + len(code)]
        if len(live) != len(code) or not code:
            continue
        # Cheap pre-check on the first 256 bytes before the full compare.
        if live[:256] != code[:256]:
            continue
        same = sum(1 for a, b in zip(live, code) if a == b) / len(code)
        if same > 0.95:
            found[ovl] = same
    return found


MAP_LINE = re.compile(r"^\s+0x([0-9a-fA-F]{8,16})\s+([A-Za-z_][A-Za-z0-9_]*)\s*$")
SYM_LINE = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*0x([0-9a-fA-F]+)\s*;")
IGNORED_SYMS = re.compile(r"(_VRAM|_START|_END|_SIZE|_ROM_START|_ROM_END)$|^jtbl_|^\.L")


class Symbols:
    """Name <-> address, keyed by binary ('main' or an overlay id like '0b')."""

    def __init__(self):
        self.by_bin = {}  # bin -> {addr: name}
        for path in sorted(MAP_DIR.glob("*.map")):
            bin_ = "main" if path.stem.startswith("slus") else path.stem.split("_", 1)[1]
            table = self.by_bin.setdefault(bin_, {})
            for line in path.read_text(errors="replace").splitlines():
                m = MAP_LINE.match(line)
                if m and not IGNORED_SYMS.search(m.group(2)):
                    table.setdefault(int(m.group(1), 16), m.group(2))
        for path in sorted((REPO / "config").glob("symbols*.txt")):
            m = re.match(r"symbols\.(?:psyq\.)?(?:ovl_([0-9a-f]{2})\.)?us\.txt", path.name)
            if not m:
                continue
            table = self.by_bin.setdefault(m.group(1) or "main", {})
            for line in path.read_text().splitlines():
                s = SYM_LINE.match(line)
                if s:
                    table[int(s.group(2), 16)] = s.group(1)  # hand-assigned names win
        self.names = {}
        for bin_, table in self.by_bin.items():
            for addr, name in table.items():
                self.names.setdefault(name, []).append((bin_, addr))
        self.sorted = {b: sorted(t) for b, t in self.by_bin.items()}

    def lookup(self, name):
        hits = self.names.get(name)
        if not hits:
            m = re.fullmatch(r"(?:func|D|jtbl)_([0-9A-Fa-f]{8})", name)
            if m:
                return int(m.group(1), 16)
            raise EmuError(f"unknown symbol {name}")
        addrs = {a for _, a in hits}
        if len(addrs) > 1:
            raise EmuError(f"{name} is ambiguous: " + ", ".join(f"{b}:{a:08X}" for b, a in hits))
        return hits[0][1]

    def describe(self, addr, resident=()):
        """'name+0xoff [bin]' for the closest symbol at or below addr in a plausible binary."""
        candidates = [o for o in resident if o in self.by_bin]
        for bin_ in candidates + ["main"]:
            if bin_ != "main":
                vram, _, _ = OVERLAYS[bin_]
                size = (OVL_DIR / f"{bin_}.bin").stat().st_size
                if not vram <= addr < vram + size:
                    continue
            elif addr >= 0x800A7000 and candidates:
                continue
            keys = self.sorted.get(bin_, [])
            lo, hi = 0, len(keys)
            while lo < hi:
                mid = (lo + hi) // 2
                if keys[mid] <= addr:
                    lo = mid + 1
                else:
                    hi = mid
            if lo:
                base = keys[lo - 1]
                if addr - base < 0x10000:
                    name = self.by_bin[bin_][base]
                    off = f"+0x{addr - base:X}" if addr != base else ""
                    tag = "" if bin_ == "main" else f" [ovl_{bin_}]"
                    return f"{name}{off}{tag}"
        return f"{addr:08X}"


_syms = None


def syms():
    global _syms
    if _syms is None:
        _syms = Symbols()
    return _syms


def parse_addr(text):
    text = text.strip()
    m = re.fullmatch(r"(.+?)\s*\+\s*(0x[0-9A-Fa-f]+|\d+)", text)
    if m:
        return parse_addr(m.group(1)) + int(m.group(2), 0)
    if re.fullmatch(r"(0x)?[0-9A-Fa-f]{8}", text):
        return int(text, 16)
    if re.fullmatch(r"0x[0-9A-Fa-f]+", text):
        return int(text, 16)
    return syms().lookup(text)


def fmt_hit(h, resident):
    s = syms()
    return (f"f{h['frame']:<6} {h['addr']:08X} a0={h['a0']:08X} a1={h['a1']:08X} "
            f"a2={h['a2']:08X} a3={h['a3']:08X} ra={s.describe(h['ra'], resident)}")


# ---------------------------------------------------------------- audio

SINK = "ff9_emu"


def pactl(*args):
    return subprocess.run(["pactl", *args], capture_output=True, text=True).stdout


def ensure_sink():
    if SINK not in pactl("list", "short", "sinks").split():
        pactl("load-module", "module-null-sink", f"sink_name={SINK}",
              "sink_properties=device.description=FF9-emulator")


APP_NAME = "ff9-emu"


def emu_streams():
    """[(sink-input id, sink id)] for the emulator's playback streams."""
    out, cur, sink = [], None, None
    for line in pactl("list", "sink-inputs").splitlines():
        line = line.strip()
        if line.startswith("Sink Input #"):
            cur, sink = line.split("#")[1], None
        elif line.startswith("Sink:"):
            sink = line.split()[1]
        elif line == f'application.name = "{APP_NAME}"' and cur:
            out.append((cur, sink))
    return out


def route_stream(settle=1.5, timeout=8.0):
    """Keep the emulator's playback on the silent sink. PULSE_SINK isn't honoured and SDL may
    reopen its stream during startup, so move it until it has stayed put for `settle` seconds."""
    sink_id = None
    for line in pactl("list", "short", "sinks").splitlines():
        cols = line.split("\t")
        if len(cols) > 1 and cols[1] == SINK:
            sink_id = cols[0]
    deadline, stable_since = time.time() + timeout, None
    while time.time() < deadline:
        streams = emu_streams()
        moved = False
        for sid, sink in streams:
            if sink != sink_id:
                pactl("move-sink-input", sid, SINK)
                moved = True
        if streams and not moved:
            stable_since = stable_since or time.time()
            if time.time() - stable_since >= settle:
                return True
        else:
            stable_since = None
        time.sleep(0.1)
    if not emu_streams():
        print("warning: emulator audio stream not found", file=sys.stderr)
    return False


def loopback_modules():
    out = []
    for line in pactl("list", "short", "modules").splitlines():
        cols = line.split("\t")
        if len(cols) > 2 and cols[1] == "module-loopback" and f"source={SINK}.monitor" in cols[2]:
            out.append(cols[0])
    return out


def cmd_audio(a):
    ensure_sink()
    route_stream(settle=0, timeout=2)
    if a.action == "on":
        if not loopback_modules():
            pactl("load-module", "module-loopback", f"source={SINK}.monitor", "latency_msec=60")
        print("audio on (emulator sink looped to the default output)")
    elif a.action == "off":
        for m in loopback_modules():
            pactl("unload-module", m)
        print("audio off (emulator still plays into the silent ff9_emu sink)")
    elif a.action == "status":
        print("on" if loopback_modules() else "off")
    elif a.action == "record":
        path = Path(a.out) if a.out else HOME / f"audio_{int(time.time())}.wav"
        if a.frames:
            bridge("runFrames", a.frames)
        rec = subprocess.Popen(["parecord", "-d", f"{SINK}.monitor", "--file-format=wav", str(path)])
        try:
            time.sleep(a.seconds)
        finally:
            rec.terminate()
            rec.wait()
        if a.frames:
            wait_paused(30)
        print(f"recorded {a.seconds}s to {path}")


# ---------------------------------------------------------------- memory cards

CARD_SIZE = 0x20000
BLOCK = 0x2000
FRAME = 0x80


def card_path(n):
    return HOME / f"memcard{n}.mcd"  # Redux's default card files in portable mode


def frame_checksum(frame):
    c = 0
    for b in frame[:0x7F]:
        c ^= b
    return c


def parse_save_file(data, name):
    """('card', 128K raw image) or ('saves', [(dir frame, blocks)]) for common save formats."""
    if len(data) == CARD_SIZE and data[:2] == b"MC":
        return "card", data                                    # .mcr / .mcd / .ddf / .mc raw
    if data[:11] == b"123-456-STD" and len(data) >= 0xF40 + CARD_SIZE:
        return "card", data[0xF40:0xF40 + CARD_SIZE]           # DexDrive .gme
    if data[:4] == b"VgsM" and len(data) >= 0x40 + CARD_SIZE:
        return "card", data[0x40:0x40 + CARD_SIZE]             # .vgs / .mem
    if len(data) > FRAME and (len(data) - FRAME) % BLOCK == 0 and data[0] == 0x51:
        return "saves", [(data[:FRAME], data[FRAME:])]          # .mcs / .psx single save
    raise EmuError(f"{name}: unrecognised save format ({len(data)} bytes); "
                   "supported: raw .mcr/.mcd, DexDrive .gme, .vgs/.mem, single-save .mcs/.psx")


def card_saves(card):
    """[(first block, product code + name, title, blocks)] for saves on a raw card."""
    out = []
    for i in range(1, 16):
        f = card[i * FRAME:(i + 1) * FRAME]
        if struct.unpack_from("<I", f, 0)[0] != 0x51:
            continue
        size = struct.unpack_from("<I", f, 4)[0]
        fname = f[0x0A:0x1F].split(b"\0")[0].decode("ascii", "replace")
        raw_title = card[i * BLOCK + 4:i * BLOCK + 0x44].split(b"\0")[0]
        try:
            import unicodedata
            title = unicodedata.normalize("NFKC", raw_title.decode("shift_jis", "replace"))
        except LookupError:
            title = raw_title.decode("ascii", "replace")
        out.append((i, fname, title.strip(), max(1, size // BLOCK)))
    return out


def insert_save(card, frame, blocks):
    card = bytearray(card)
    # Two files can't share a name. Games number their saves in the name's last digits
    # (FF9: BASLUS-0125100000-NN), so move a clash to the next free number.
    frame = bytearray(frame)
    taken = {fname for _, fname, _, _ in card_saves(card)}
    name = frame[0x0A:0x1F].split(b"\0")[0].decode("ascii", "replace")
    m = re.fullmatch(r"(.*?)(\d{2})", name)
    if name in taken and m:
        for k in range(100):
            alt = f"{m.group(1)}{k:02d}"
            if alt not in taken:
                print(f"note: {name} already on the card; stored as {alt}")
                frame[0x0A:0x1F] = alt.encode().ljust(0x15, b"\0")
                break
    elif name in taken:
        raise EmuError(f"{name} is already on the card")
    n = len(blocks) // BLOCK
    free = [i for i in range(1, 16) if struct.unpack_from("<I", card, i * FRAME)[0] in (0xA0, 0)]
    if len(free) < n:
        raise EmuError("not enough free blocks on the card")
    slots = free[:n]
    for k, i in enumerate(slots):
        f = bytearray(FRAME)
        if k == 0:
            f[:] = frame
            struct.pack_into("<I", f, 0, 0x51)
            struct.pack_into("<I", f, 4, n * BLOCK)
        else:
            struct.pack_into("<I", f, 0, 0x53 if k == n - 1 else 0x52)
        struct.pack_into("<H", f, 8, slots[k + 1] - 1 if k + 1 < n else 0xFFFF)
        f[0x7F] = frame_checksum(f)
        card[i * FRAME:(i + 1) * FRAME] = f
        card[i * BLOCK:(i + 1) * BLOCK] = blocks[k * BLOCK:(k + 1) * BLOCK]
    return bytes(card)


def blank_card():
    card = bytearray(CARD_SIZE)
    card[0:2] = b"MC"
    card[0x7F] = frame_checksum(card[:FRAME])
    for i in range(1, 16):
        f = bytearray(FRAME)
        struct.pack_into("<IIH", f, 0, 0xA0, 0, 0xFFFF)
        f[0x7F] = frame_checksum(f)
        card[i * FRAME:(i + 1) * FRAME] = f
    return bytes(card)


def cmd_memcard(a):
    path = card_path(a.card)
    if a.action == "list":
        card = path.read_bytes() if path.exists() else blank_card()
        saves = card_saves(card)
        print(f"{path}: {len(saves)} saves")
        for i, fname, title, n in saves:
            print(f"  block {i:2}  {fname:<20} {title}" + (f"  ({n} blocks)" if n > 1 else ""))
        return
    if api_up():
        raise EmuError("stop the emulator first (`tools/emu.py stop`): Redux rewrites the card file")
    card = blank_card() if a.replace or not path.exists() else path.read_bytes()
    for f in a.files:
        kind, payload = parse_save_file(Path(f).read_bytes(), f)
        if kind == "card":
            if not card_saves(card):
                card = payload  # onto an empty card: take the image as-is
                continue
            payload = [(payload[i * FRAME:(i + 1) * FRAME], payload[i * BLOCK:(i + n) * BLOCK])
                       for i, _, _, n in card_saves(payload)]
        for frame, blocks in payload:
            card = insert_save(card, frame, blocks)
    if path.exists():
        path.with_suffix(".mcd.bak").write_bytes(path.read_bytes())
    path.write_bytes(card)
    cmd_memcard(argparse.Namespace(action="list", card=a.card))


# ---------------------------------------------------------------- setup

# Pinned PCSX-Redux dev build (distrib.app build 379, commit 8d1aaadf); newer builds should also work.
REDUX_URL = ("https://distrib.app/storage/assets/d5d/50f/834/ef2704b2349315c8acef338705810bb1d7a3db87f76858a63327e17/"
             "PCSX-Redux-8d1aaadf-linux-x86_64.zip")
REDUX_SHA1 = "756f15123bbb8fd83b8a3d881abe749463dc46d0"
BIOS_SHA1 = "10155d8d6e6e832d6ea66db9bc098321fb5e8ebf"  # SCPH-1001 v2.2


def cmd_setup(a):
    import hashlib
    import zipfile
    if not APPRUN.exists():
        dest = BIN / "pcsx-redux"
        dest.mkdir(parents=True, exist_ok=True)
        zpath = dest / "redux.zip"
        print("downloading PCSX-Redux ...")
        urllib.request.urlretrieve(REDUX_URL, zpath)
        if hashlib.sha1(zpath.read_bytes()).hexdigest() != REDUX_SHA1:
            raise EmuError("PCSX-Redux download has the wrong sha1")
        with zipfile.ZipFile(zpath) as z:
            z.extractall(dest)
        zpath.unlink()
        image = next(dest.glob("*.AppImage"))
        image.chmod(0o755)
        subprocess.run([str(image), "--appimage-extract"], cwd=dest, check=True, stdout=subprocess.DEVNULL)
    print(f"emulator: {APPRUN}")
    if a.bios:
        BIOS.parent.mkdir(parents=True, exist_ok=True)
        BIOS.write_bytes(Path(a.bios).read_bytes())
    if not BIOS.exists():
        raise EmuError(f"no BIOS; rerun with --bios /path/to/SCPH1001.BIN")
    if hashlib.sha1(BIOS.read_bytes()).hexdigest() != BIOS_SHA1:
        print("warning: BIOS is not SCPH-1001 v2.2; it will probably still work")
    print(f"bios: {BIOS}")
    # Redux's CloneCD (.ccd) parser reports a zero-length track and the BIOS never boots; use cues.
    for n in range(1, 5):
        img = DISCS / f"FF9 CD{n}.img"
        cue = DISCS / f"FF9 CD{n}.cue"
        if img.exists() and not cue.exists():
            cue.write_text(f'FILE "{img.name}" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n')
        print(f"disc {n}: {cue if cue.exists() else 'missing ' + str(img)}")
    if not MAP_DIR.exists():
        print("note: build/us/*.map missing; run `tools/docker.sh make verify` for symbol names")


# ---------------------------------------------------------------- process control

def api_up():
    try:
        flow()
        return True
    except EmuError:
        return False


def find_display():
    if os.environ.get("DISPLAY"):
        return os.environ["DISPLAY"], os.environ.get("XAUTHORITY")
    sockets = sorted(Path("/tmp/.X11-unix").glob("X*"))
    if not sockets:
        return None, None
    xauth = Path(f"/run/user/{os.getuid()}/gdm/Xauthority")
    return ":" + sockets[0].name[1:], str(xauth) if xauth.exists() else None


def cmd_start(a):
    if api_up():
        print(f"already running on port {PORT}")
        return
    for path, what in [(APPRUN, "PCSX-Redux (see docs/emulator.md)"), (BIOS, "BIOS")]:
        if not path.exists():
            raise EmuError(f"missing {what}: {path}")
    disc = DISCS / f"FF9 CD{a.disc}.cue"
    if not disc.exists():
        raise EmuError(f"missing {disc}")
    HOME.mkdir(parents=True, exist_ok=True)
    cfg = HOME / "pcsx.json"
    if not cfg.exists():
        cfg.write_text("{}\n")  # its presence puts Redux in portable mode (config lives in HOME)
    display, xauth = find_display()
    if not display:
        raise EmuError("no X display found; set DISPLAY")
    ensure_sink()
    # Audio always goes to the silent ff9_emu sink; `emu.py audio on` loops it to the speakers.
    env = dict(os.environ, DISPLAY=display, PULSE_SINK=SINK,
               SDL_AUDIO_DRIVER="pulseaudio", SDL_AUDIODRIVER="pulseaudio",
               SDL_APP_NAME=APP_NAME, SDL_AUDIO_DEVICE_APP_NAME=APP_NAME)
    if xauth:
        env["XAUTHORITY"] = xauth
    args = [str(APPRUN), "-stdout", "-lua_stdout",
            "-dynarec" if a.dynarec else "-interpreter", "-debugger",
            "-softgpu" if a.gpu == "soft" else "-openglgpu",
            "-webserver", "-webserver-port", str(PORT),
            "-bios", str(BIOS), "-iso", str(disc),
            "-dofile", str(BRIDGE)]
    if a.fastboot:
        args.append("-fastboot")
    if not a.paused:
        args.append("-run")
    log = open(LOGFILE, "w")
    proc = subprocess.Popen(args, cwd=HOME, env=env, stdout=log, stderr=subprocess.STDOUT,
                            stdin=subprocess.DEVNULL, start_new_session=True)
    PIDFILE.write_text(str(proc.pid))
    for _ in range(100):
        if proc.poll() is not None:
            raise EmuError(f"emulator exited with {proc.returncode}; see {LOGFILE}")
        try:
            bridge("status")  # the web server comes up before -dofile has registered the bridge
            break
        except EmuError:
            time.sleep(0.2)
    else:
        raise EmuError(f"bridge never came up; see {LOGFILE}")
    upload_symbols()
    route_stream()
    print(f"started pid {proc.pid}, disc {a.disc}, port {PORT}" + (" (paused)" if a.paused else ""))
    if a.state:
        cmd_load(argparse.Namespace(name=a.state))


def upload_symbols():
    """Give Redux's own debugger UI the main exe's names (overlay names are ambiguous)."""
    table = syms().by_bin.get("main", {})
    body = "".join(f"{addr:08x} {name}\n" for addr, name in sorted(table.items())).encode()
    try:
        http("/assembly/symbols?function=upload", "POST", body)
    except EmuError:
        pass


def cmd_stop(a):
    try:
        lua("PCSX.quit(0) return true", timeout=3)
    except EmuError:
        pass
    pid = int(PIDFILE.read_text()) if PIDFILE.exists() else None
    for _ in range(30):
        if not api_up():
            break
        time.sleep(0.2)
    else:
        if pid:
            os.killpg(pid, signal.SIGTERM)
    PIDFILE.unlink(missing_ok=True)
    print("stopped")


def cmd_status(a):
    st = bridge("status")
    f = flow()
    mem = ram()
    res = resident_overlays(mem)
    s = syms()
    print(f"{'running' if f['running'] else 'paused'}  frame {st['frame']}  pc {s.describe(st['pc'], res)}")
    print("overlays: " + (", ".join(f"ovl_{o} ({OVERLAYS[o][0]:08X})" for o in sorted(res)) or "none"))
    if st.get("lastBreak"):
        print("last break: " + fmt_hit(st["lastBreak"], res))


def cmd_pause(a):
    http("/execution-flow?function=pause", "POST")
    print(f"paused at frame {bridge('status')['frame']}")


def cmd_resume(a):
    http("/execution-flow?function=resume", "POST")
    print("running")


def cmd_reset(a):
    bridge("clearBps")
    http(f"/execution-flow?function=reset&type={'hard' if a.hard else 'soft'}", "POST")
    print("reset")


def cmd_frames(a):
    print(f"paused at frame {run_frames(a.n)}")


def cmd_reload(a):
    lua(f"dofile({lua_lit(str(BRIDGE))}) return true")
    print("bridge reloaded")


# ---------------------------------------------------------------- memory

def hexdump(addr, data, width=16):
    for i in range(0, len(data), width):
        chunk = data[i:i + width]
        words = " ".join(chunk[j:j + 4].hex() for j in range(0, len(chunk), 4))
        text = "".join(chr(c) if 32 <= c < 127 else "." for c in chunk)
        print(f"{addr + i:08X}  {words:<{width * 2 + width // 4}} {text}")


FORMATS = {"u8": "<B", "s8": "<b", "u16": "<H", "s16": "<h", "u32": "<I", "s32": "<i"}


def cmd_read(a):
    addr = parse_addr(a.addr)
    data = read_mem(addr, a.length)
    if a.as_ == "hex":
        hexdump(addr, data)
        return
    fmt = FORMATS[a.as_]
    size = struct.calcsize(fmt)
    for i in range(0, len(data) - size + 1, size):
        (v,) = struct.unpack_from(fmt, data, i)
        extra = ""
        if a.as_ == "u32" and RAM_BASE <= v < RAM_BASE + RAM_SIZE:
            extra = "  -> " + syms().describe(v, resident_overlays())
        print(f"{addr + i:08X}  {v:#x} ({v}){extra}")


def cmd_write(a):
    addr = parse_addr(a.addr)
    if a.hex:
        data = bytes.fromhex(a.value)
    else:
        v = int(a.value, 0)
        data = v.to_bytes(a.size, "little", signed=v < 0)
    write_mem(addr, data)
    print(f"wrote {len(data)} bytes at {addr:08X}")


def cmd_regs(a):
    r = bridge("regs")
    res = resident_overlays()
    s = syms()
    print(f"pc {s.describe(r['pc'], res)}   ra {s.describe(r['ra'], res)}")
    names = ["at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
             "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "s8",
             "lo", "hi"]
    for i in range(0, len(names), 8):
        print("  ".join(f"{n}={r[n]:08X}" for n in names[i:i + 8]))


def cmd_sym(a):
    s = syms()
    res = resident_overlays() if api_up() else {}
    for q in a.query:
        if re.fullmatch(r"(0x)?[0-9A-Fa-f]{8}", q):
            print(f"{q} = {s.describe(int(q, 16), res)}")
        else:
            hits = s.names.get(q) or []
            print(f"{q} = " + (", ".join(f"{b}:{x:08X}" for b, x in hits) or f"{parse_addr(q):08X}"))


def cmd_ovl(a):
    res = resident_overlays()
    if not res:
        print("no known overlay resident")
    for o in sorted(res):
        vram, text, data = OVERLAYS[o]
        print(f"ovl_{o}  {vram:08X}  .text {vram + text:08X}-{vram + data:08X}  match {res[o]:.1%}")


def snap_path(name):
    return SNAPS / f"{name}.bin"


def cmd_snap(a):
    SNAPS.mkdir(parents=True, exist_ok=True)
    snap_path(a.name).write_bytes(ram())
    print(f"saved RAM snapshot {a.name} (frame {bridge('status')['frame']})")


def load_snap(name):
    if name == "now":
        return ram()
    p = snap_path(name)
    if not p.exists():
        raise EmuError(f"no snapshot {name}")
    return p.read_bytes()


def parse_range(text):
    if not text:
        return 0, RAM_SIZE
    lo, hi = text.split("-")
    return parse_addr(lo) & 0x1FFFFF, parse_addr(hi) & 0x1FFFFF


def cmd_diff(a):
    """Words that differ between two snapshots (or 'now'), grouped into runs."""
    old, new = load_snap(a.a), load_snap(a.b)
    lo, hi = parse_range(a.range)
    size = a.size
    changed = [o for o in range(lo - lo % size, hi, size) if old[o:o + size] != new[o:o + size]]
    if a.exclude_ovl:
        for ovl, (vram, _, _) in OVERLAYS.items():
            base = vram & 0x1FFFFF
            end = base + (OVL_DIR / f"{ovl}.bin").stat().st_size
            changed = [o for o in changed if not base <= o < end]
    res = resident_overlays(new)
    s = syms()
    fmt = {1: "<B", 2: "<H", 4: "<I"}[size]
    print(f"{len(changed)} changed {size}-byte units")
    for o in changed[:a.limit]:
        (x,) = struct.unpack_from(fmt, old, o)
        (y,) = struct.unpack_from(fmt, new, o)
        addr = RAM_BASE + o
        print(f"{addr:08X} {s.describe(addr, res):<32} {x:#x} -> {y:#x}")
    if len(changed) > a.limit:
        print(f"... {len(changed) - a.limit} more (use --limit / --range)")


def cmd_find(a):
    """Addresses holding a value (aligned to its size), optionally only those that also held
    another value in a snapshot: e.g. find 9999 --size 4 --was 9998 --snap before."""
    mem = ram()
    lo, hi = parse_range(a.range)
    fmt = {1: "<B", 2: "<H", 4: "<I"}[a.size]
    want = struct.pack(fmt, int(a.value, 0) & ((1 << (8 * a.size)) - 1))
    hits = [o for o in range(lo - lo % a.size, hi, a.size) if mem[o:o + a.size] == want]
    if a.was is not None:
        old = load_snap(a.snap)
        was = struct.pack(fmt, int(a.was, 0) & ((1 << (8 * a.size)) - 1))
        hits = [o for o in hits if old[o:o + a.size] == was]
    res = resident_overlays(mem)
    print(f"{len(hits)} hits")
    for o in hits[:a.limit]:
        print(f"{RAM_BASE + o:08X} {syms().describe(RAM_BASE + o, res)}")


# ---------------------------------------------------------------- input, screen, states

def cmd_press(a):
    names = []
    for b in a.buttons.upper().replace("+", ",").split(","):
        b = BUTTON_ALIASES.get(b.strip(), b.strip())
        if b not in BUTTONS:
            raise EmuError(f"unknown button {b}; one of {', '.join(BUTTONS)}")
        names.append(b)
    bridge("hold", names, a.hold)
    frame = run_frames(a.hold + a.wait)
    print(f"pressed {'+'.join(names)} for {a.hold} frames; paused at frame {frame}")
    if a.shot:
        cmd_shot(argparse.Namespace(path=None, quiet=False))


def cmd_shot(a):
    png = http("/screen/still", timeout=10)
    if a.path:
        path = Path(a.path)
    else:
        SHOTS.mkdir(parents=True, exist_ok=True)
        path = SHOTS / f"frame{bridge('status')['frame']:07d}.png"
    path.write_bytes(png)
    print(f"screenshot {path}")


STATE_NAME = re.compile(r"[A-Za-z0-9_-]+")


def cmd_save(a):
    if not STATE_NAME.fullmatch(a.name):
        raise EmuError("state names are letters, digits, _ and -")
    print(http(f"/state/save?name={a.name}").decode().strip())


def cmd_load(a):
    if not STATE_NAME.fullmatch(a.name):
        raise EmuError("state names are letters, digits, _ and -")
    running = is_running()
    print(http(f"/state/load?name={a.name}").decode().strip())
    if not running:
        http("/execution-flow?function=pause", "POST")


def cmd_states(a):
    usage = json.loads(http("/state/usage"))
    for s in usage.get("named") or []:
        print(s["name"])


# ---------------------------------------------------------------- breakpoints

def cmd_trace(a):
    """Log every call to the given functions over N frames."""
    targets = [(t, parse_addr(t)) for t in a.funcs]
    ids = {bridge("addBp", addr, "Exec", 4, "log", a.max, name): (name, addr) for name, addr in targets}
    try:
        frame = run_frames(a.frames)
        res = resident_overlays()
        for bp, (name, addr) in ids.items():
            log = bridge("takeLog", bp)
            print(f"== {name} ({addr:08X}): {log['hits']} calls in {a.frames} frames (to frame {frame})")
            callers = {}
            for h in log["log"] or []:
                c = syms().describe(h["ra"], res)
                callers[c] = callers.get(c, 0) + 1
            for c, n in sorted(callers.items(), key=lambda kv: -kv[1]):
                print(f"   {n:6} from {c}")
            for h in (log["log"] or [])[:a.show]:
                print("   " + fmt_hit(h, res))
    finally:
        for bp in ids:
            bridge("removeBp", bp)


def cmd_watch(a):
    """Log the code that reads or writes an address range over N frames."""
    addr = parse_addr(a.addr)
    kind = "Read" if a.read else "Write"
    bp = bridge("addBp", addr, kind, a.size, "log", a.max, a.addr)
    try:
        frame = run_frames(a.frames)
        log = bridge("takeLog", bp)
    finally:
        bridge("removeBp", bp)
    res = resident_overlays()
    s = syms()
    print(f"== {kind.lower()}s of {a.addr} ({addr:08X}, {a.size} bytes): {log['hits']} in {a.frames} frames (to frame {frame})")
    by_pc = {}
    for h in log["log"] or []:
        by_pc.setdefault(h["pc"], []).append(h)
    for pc, hs in sorted(by_pc.items(), key=lambda kv: -len(kv[1])):
        print(f"   {len(hs):6} at {s.describe(pc, res)} ({pc:08X})  ra={s.describe(hs[0]['ra'], res)}  first f{hs[0]['frame']}")


def cmd_until(a):
    """Run until the given code address executes (or memory is accessed), then stay paused there."""
    addr = parse_addr(a.addr)
    kind = "Read" if a.read else "Write" if a.write else "Exec"
    bp = bridge("addBp", addr, kind, a.size if kind != "Exec" else 4, "break", 1, a.addr)
    try:
        run_frames(a.frames)
    except EmuError:
        pass
    finally:
        bridge("removeBp", bp)
    st = bridge("status")
    if not st.get("lastBreak"):
        print(f"not hit within {a.frames} frames (paused at frame {st['frame']})")
        return 1
    print(f"hit at frame {st['frame']}")
    lua("Bridge.pauseAt = nil return true")
    cmd_regs(a)


def cmd_bp(a):
    if a.action == "list":
        for b in bridge("listBps") or []:
            print(f"#{b['id']} {b['kind']} {b['addr']:08X} w{b['width']} {b['mode']} hits={b['hits']} {b['label']}")
    elif a.action == "clear":
        bridge("clearBps")
        print("cleared")


def cmd_lua(a):
    code = Path(a.file).read_text() if a.file else a.code
    if code is None:
        code = sys.stdin.read()
    print(json.dumps(lua(code), indent=1))


# ---------------------------------------------------------------- main

def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)

    s = sub.add_parser("setup", help="download PCSX-Redux, install the BIOS, write .cue files")
    s.add_argument("--bios", help="path to a PS1 BIOS image (SCPH1001.BIN)")
    s.set_defaults(fn=cmd_setup)
    s = sub.add_parser("start", help="launch PCSX-Redux on a disc")
    s.add_argument("--disc", type=int, default=1, choices=[1, 2, 3, 4])
    s.add_argument("--state", help="named save state to load after boot")
    s.add_argument("--paused", action="store_true", help="don't start emulation")
    s.add_argument("--gpu", choices=["soft", "gl"], default="soft",
                   help="gl has no screenshot support (default soft; same speed)")
    s.add_argument("--dynarec", action="store_true", help="breakpoints don't fire; rarely needed")
    s.add_argument("--no-fastboot", dest="fastboot", action="store_false", help="show the BIOS intro")
    s.set_defaults(fn=cmd_start)
    sub.add_parser("stop", help="quit the emulator").set_defaults(fn=cmd_stop)
    sub.add_parser("status", help="run state, frame, pc, resident overlays").set_defaults(fn=cmd_status)
    sub.add_parser("pause").set_defaults(fn=cmd_pause)
    sub.add_parser("resume", help="free-run (until pause)").set_defaults(fn=cmd_resume)
    s = sub.add_parser("reset")
    s.add_argument("--hard", action="store_true")
    s.set_defaults(fn=cmd_reset)
    s = sub.add_parser("frames", help="run N frames then pause")
    s.add_argument("n", type=int)
    s.set_defaults(fn=cmd_frames)
    sub.add_parser("reload", help="re-run tools/emu/bridge.lua in the emulator").set_defaults(fn=cmd_reload)

    s = sub.add_parser("read", help="read memory")
    s.add_argument("addr")
    s.add_argument("length", type=lambda x: int(x, 0), nargs="?", default=64)
    s.add_argument("--as", dest="as_", choices=["hex"] + list(FORMATS), default="hex")
    s.set_defaults(fn=cmd_read)
    s = sub.add_parser("write", help="write memory")
    s.add_argument("addr")
    s.add_argument("value", help="integer, or hex bytes with --hex")
    s.add_argument("--size", type=int, choices=[1, 2, 4], default=4)
    s.add_argument("--hex", action="store_true")
    s.set_defaults(fn=cmd_write)
    sub.add_parser("regs", help="CPU registers").set_defaults(fn=cmd_regs)
    s = sub.add_parser("sym", help="resolve names <-> addresses")
    s.add_argument("query", nargs="+")
    s.set_defaults(fn=cmd_sym)
    sub.add_parser("ovl", help="which code overlays are in RAM").set_defaults(fn=cmd_ovl)

    s = sub.add_parser("snap", help="save a RAM snapshot for diff/find")
    s.add_argument("name")
    s.set_defaults(fn=cmd_snap)
    s = sub.add_parser("diff", help="compare two RAM snapshots ('now' = live RAM)")
    s.add_argument("a")
    s.add_argument("b", nargs="?", default="now")
    s.add_argument("--size", type=int, choices=[1, 2, 4], default=4)
    s.add_argument("--range", help="e.g. 0x80060000-0x80070000 or D_8006794C-D_80068000")
    s.add_argument("--exclude-ovl", action="store_true", help="skip overlay load areas")
    s.add_argument("--limit", type=int, default=200)
    s.set_defaults(fn=cmd_diff)
    s = sub.add_parser("find", help="search RAM for a value")
    s.add_argument("value")
    s.add_argument("--size", type=int, choices=[1, 2, 4], default=4)
    s.add_argument("--was", help="only addresses that held this value in --snap")
    s.add_argument("--snap", default="last")
    s.add_argument("--range")
    s.add_argument("--limit", type=int, default=100)
    s.set_defaults(fn=cmd_find)

    s = sub.add_parser("press", help="hold buttons, run, pause (e.g. press cross, press up+x --hold 10)")
    s.add_argument("buttons")
    s.add_argument("--hold", type=int, default=4, help="frames to hold (default 4)")
    s.add_argument("--wait", type=int, default=20, help="frames to run after release (default 20)")
    s.add_argument("--shot", action="store_true", help="screenshot afterwards")
    s.set_defaults(fn=cmd_press)
    s = sub.add_parser("shot", help="save a PNG of the current frame")
    s.add_argument("path", nargs="?")
    s.set_defaults(fn=cmd_shot)
    s = sub.add_parser("save", help="named save state")
    s.add_argument("name")
    s.set_defaults(fn=cmd_save)
    s = sub.add_parser("load", help="load named save state")
    s.add_argument("name")
    s.set_defaults(fn=cmd_load)
    sub.add_parser("states", help="list named save states").set_defaults(fn=cmd_states)

    s = sub.add_parser("memcard", help="list saves on a memory card, or import save files into it")
    s.add_argument("action", choices=["list", "import"])
    s.add_argument("files", nargs="*", help="import: .gme/.mcr/.mcd/.vgs card images or .mcs single saves")
    s.add_argument("--card", type=int, default=1, choices=[1, 2])
    s.add_argument("--replace", action="store_true", help="import: start from a blank card")
    s.set_defaults(fn=cmd_memcard)
    s = sub.add_parser("audio", help="on/off: hear the emulator; record: capture its output to WAV")
    s.add_argument("action", choices=["on", "off", "status", "record"])
    s.add_argument("--seconds", type=float, default=5)
    s.add_argument("--frames", type=int, help="record: run this many frames while recording (else it must be running)")
    s.add_argument("--out")
    s.set_defaults(fn=cmd_audio)
    s = sub.add_parser("trace", help="log calls (args, callers) to functions over N frames")
    s.add_argument("funcs", nargs="+")
    s.add_argument("--frames", type=int, default=60)
    s.add_argument("--max", type=int, default=2000, help="hits kept per function")
    s.add_argument("--show", type=int, default=10, help="individual hits printed per function")
    s.set_defaults(fn=cmd_trace)
    s = sub.add_parser("watch", help="log code writing (or --read reading) an address over N frames")
    s.add_argument("addr")
    s.add_argument("--size", type=int, default=4)
    s.add_argument("--read", action="store_true")
    s.add_argument("--frames", type=int, default=60)
    s.add_argument("--max", type=int, default=2000)
    s.set_defaults(fn=cmd_watch)
    s = sub.add_parser("until", help="run until an address executes (or --read/--write), stay paused")
    s.add_argument("addr")
    g = s.add_mutually_exclusive_group()
    g.add_argument("--read", action="store_true")
    g.add_argument("--write", action="store_true")
    s.add_argument("--size", type=int, default=4)
    s.add_argument("--frames", type=int, default=600, help="give up after this many frames")
    s.set_defaults(fn=cmd_until)
    s = sub.add_parser("bp", help="list or clear bridge breakpoints")
    s.add_argument("action", choices=["list", "clear"])
    s.set_defaults(fn=cmd_bp)
    s = sub.add_parser("lua", help="evaluate Lua in the emulator (Bridge.* helpers available)")
    s.add_argument("code", nargs="?")
    s.add_argument("-f", "--file")
    s.set_defaults(fn=cmd_lua)

    a = p.parse_args()
    try:
        return a.fn(a) or 0
    except EmuError as e:
        print(f"error: {e}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
