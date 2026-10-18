#!/usr/bin/env python3
"""Battle effect modules: the MIPS code that FF9.IMG dir 13 (id 16) carries next to effect data.

Format and loader: docs/ff9img.md ("Battle effect files"). Catalogue: docs/effect-modules.md.

    python3 tools/effect_modules.py list                 # one line per module chunk, all discs
    python3 tools/effect_modules.py catalogue            # write config/fxmods.us.txt + docs/effect-modules.md
    python3 tools/effect_modules.py extract disks/us/fxmod   # unique module images as <name>.bin

--disks DIR points at the directory holding disc1..disc4/FF9.IMG (default disks/us). Only the
discs that exist are read; extract needs disc 1. stdlib only, runs on the host.
"""
import argparse
import hashlib
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ff9img import Img, SECTOR  # noqa: E402

FX_DIR = 13            # directory index (id 16, type 3) that btlfx_loadFile reads via file_find(0xD, id)
SLOT0 = 0x801E7700     # module area: 0x801E7700-0x801F1700 (0xA000)
SLOT1 = 0x801EC700     # second half, used by odd groups of kind 1 (summons)
FILL = 0xFF            # the chunk is padded to its sector count with 0xFF

CHUNK_TYPES = {
    0: "VRAM images (staged through the module area)",
    1: "VRAM pages (64x128, staged)",
    2: "effect data (0x801F1700 ring; flag: common data first)",
    3: "module code",
    4: "summon model header",
    5: "summon model data",
    6: "summon work data",
    7: "sound data",
    8: "sound data (2)",
    9: "VRAM pages by mask",
    10: "effect data (appended)",
}


def fx_files(img):
    """[(file_id, first_sector, sectors)] of dir 13. Its table is count+1 u16 sector offsets relative
    to the directory's first sector (0xFFFF = no file); a file ends where the next present one starts."""
    d = img.dirs[FX_DIR]
    assert d["type"] == 3
    n = d["count"]
    t = struct.unpack_from(f"<{n + 1}H", img.read(d["table"] * SECTOR, (n + 1) * 2))
    present = [(i, s) for i, s in enumerate(t) if s != 0xFFFF]
    out = []
    for (i, s), (_, nxt) in zip(present, present[1:]):
        out.append((i, d["sector"] + s, nxt - s))
    return out


def parse_file(img, sector):
    """Groups of chunks as the loader (btlfx_updateLoader) walks them.
    Sector 0: u16 ngroups, then per group {u16 kind, u16 nchunks} followed by its chunk records
    {u8 type, u8 flag, u16 sectors} (+ u16 extra sectors when type 2 has a flag). Chunk data follows
    from sector 1 in the same order."""
    h = img.read(sector * SECTOR, SECTOR)
    (ngroups,) = struct.unpack_from("<H", h, 0)
    p = 2
    cur = sector + 1
    groups = []
    for _ in range(ngroups):
        kind, nch = struct.unpack_from("<2H", h, p)
        p += 4
        chunks = []
        for _ in range(nch):
            ty, fl, ns = struct.unpack_from("<BBH", h, p)
            p += 4
            extra = 0
            if ty == 2 and fl:
                (extra,) = struct.unpack_from("<H", h, p)
                p += 2
            chunks.append({"type": ty, "flag": fl, "sectors": ns + extra, "sector": cur})
            cur += ns + extra
        groups.append({"kind": kind, "chunks": chunks})
    return groups, cur - sector


def trim(data):
    e = len(data)
    while e > 0 and data[e - 1] == FILL:
        e -= 1
    return data[: (e + 3) & ~3]


def layout(img, vram):
    """rodata / text / data boundaries (offsets) of a trimmed module image.
    Word 0 points at the header {0, init, op 0x80, op 0x81, ...}, which lives in .data. .rodata is word 0
    plus jump tables / strings; .text runs to the last function's jr ra; the rest is .data."""
    words = struct.unpack_from(f"<{len(img) // 4}I", img)
    hdr = words[0] - vram
    assert 4 <= hdr < len(img), "header pointer outside the image"
    jrs = [i * 4 for i in range(1, hdr // 4) if words[i] == 0x03E00008]
    text_end = jrs[-1] + 8
    i = 1
    while i * 4 < text_end:
        w = words[i]
        b = img[i * 4: i * 4 + 4]
        if vram + 4 <= w < vram + text_end or w == 0:
            i += 1  # jump table entry / zero
        elif all(32 <= c < 127 or c in (0, 10) for c in b) and 32 <= b[0] < 127:
            i += 1  # string
        else:
            break
    text = i * 4
    # Every function the header or a jal names must be in .text.
    refs = [w - vram for w in words[hdr // 4 + 2: hdr // 4 + 18] if vram + 4 <= w < vram + text_end]
    for k in range(text // 4, text_end // 4):
        w = words[k]
        if w >> 26 == 3:
            t = ((w & 0x3FFFFFF) << 2) | 0x80000000
            if vram <= t < vram + text_end:
                refs.append(t - vram)
    assert not refs or min(refs) >= text, "function reference inside the guessed .rodata"
    ops = []
    for k in range(16):
        j = hdr // 4 + 2 + k
        if j >= len(words) or not (words[j] == 0 or vram + text <= words[j] < vram + text_end):
            break
        ops.append(words[j])
    while ops and ops[-1] == 0:
        ops.pop()
    return {"text": text, "data": text_end, "size": len(img), "header": hdr, "ops": ops}


def scan_disc(path):
    """{(file_id, group): module dict} for one FF9.IMG."""
    img = Img(path)
    mods = {}
    for fid, sec, nsec in fx_files(img):
        groups, used = parse_file(img, sec)
        assert used == nsec, f"file {fid:#x}: header covers {used} of {nsec} sectors"
        for gi, g in enumerate(groups):
            for c in g["chunks"]:
                if c["type"] != 3:
                    continue
                vram = SLOT1 if (g["kind"] == 1 and gi % 2 == 1) else SLOT0
                raw = img.read(c["sector"] * SECTOR, c["sectors"] * SECTOR)
                image = trim(raw)
                mods[(fid, gi)] = {
                    "fid": fid, "group": gi, "ngroups": len(groups), "vram": vram, "chunk": len(raw),
                    "image": image, "sha1": hashlib.sha1(image).hexdigest(),
                    "raw_sha1": hashlib.sha1(raw).hexdigest(),
                }
    return mods


def mod_name(m):
    return f"fx_{m['fid']:03x}" + (f"_{m['group']}" if m["ngroups"] > 1 else "")


def load_all(disks):
    discs = {}
    for n in (1, 2, 3, 4):
        p = os.path.join(disks, f"disc{n}", "FF9.IMG")
        if os.path.exists(p):
            discs[n] = scan_disc(p)
    if not discs:
        sys.exit(f"no FF9.IMG under {disks}/discN (make extract-img)")
    return discs


def unique(discs):
    """Distinct images in file-id order: [(name, module, [aliases], {disc: same?})]."""
    first = discs[min(discs)]
    by_hash = {}
    order = []
    for key in sorted(first):
        m = first[key]
        if m["sha1"] in by_hash:
            by_hash[m["sha1"]][2].append(mod_name(m))
            continue
        entry = [mod_name(m), m, [], {}]
        by_hash[m["sha1"]] = entry
        order.append(entry)
    for entry in order:
        m = entry[1]
        key = (m["fid"], m["group"])
        entry[3] = {n: (key in d and d[key]["sha1"] == m["sha1"]) for n, d in discs.items()}
    return order


# Effects whose file id is known from runtime traces (docs/runtime-findings.md, btlfx_loadFile) or the DS port.
KNOWN = {
    0x4E: "Sleep", 0x7E: "Slow", 0x95: "Stop", 0xCA: "Poison", 0x7F: "Silence", 0x7C: "Mini",
    0x61: "Confuse", 0x0E: "Berserk", 0x9D: "Blind", 0xEC: "Doom", 0x99: "Break", 0x59: "Haste",
    0x7B: "Protect", 0x3F: "Shell", 0x4F: "Reflect", 0x1B: "Float", 0xB2: "Ark (summon, enemy)",
}


def cmd_list(args):
    discs = load_all(args.disks)
    first = discs[min(discs)]
    for key in sorted(first):
        m = first[key]
        L = layout(m["image"], m["vram"])
        same = "".join(str(n) if n in discs and key in discs[n] and discs[n][key]["sha1"] == m["sha1"] else "-"
                       for n in (1, 2, 3, 4))
        print(f"{mod_name(m):10} file {m['fid']:#05x} group {m['group']} vram {m['vram']:08X} "
              f"size {L['size']:#7x} rodata {L['text']:#6x} text {L['data'] - L['text']:#7x} "
              f"data {L['size'] - L['data']:#7x} discs {same} sha1 {m['sha1'][:12]}")


def cmd_catalogue(args):
    discs = load_all(args.disks)
    mods = unique(discs)
    first = discs[min(discs)]
    nfiles = len({k[0] for k in first})
    lines = [
        "# FF9.IMG dir 13 (id 16) battle effect modules: one line per distinct module image.",
        "# Written by tools/effect_modules.py catalogue (do not edit; boundaries are guessed, see its layout()).",
        "# <name> <vram> <.text start> <.data start> <size> <sha1>   # file ids sharing the image",
    ]
    rows = []
    tot_text = tot_size = 0
    all_text = all_size = 0
    for key in sorted(first):
        L = layout(first[key]["image"], first[key]["vram"])
        all_text += L["data"] - L["text"]
        all_size += L["size"]
    for name, m, aliases, same in mods:
        L = layout(m["image"], m["vram"])
        tot_text += L["data"] - L["text"]
        tot_size += L["size"]
        alias = (" = " + " ".join(aliases)) if aliases else ""
        lines.append(f"{name:10} {m['vram']:08X} {L['text']:5X} {L['data']:5X} {L['size']:5X} {m['sha1']}  #{alias}")
        rows.append((name, m, aliases, same, L))
    with open("config/fxmods.us.txt", "w") as f:
        f.write("\n".join(lines) + "\n")

    discs_read = sorted(discs)
    identical = all(all(r[3].values()) for r in rows)
    raw_identical = all(
        all(k in discs[n] and discs[n][k]["raw_sha1"] == first[k]["raw_sha1"] for n in discs) for k in first)
    md = [
        "# Battle effect modules",
        "",
        "Generated by `python3 tools/effect_modules.py catalogue` (format: `docs/ff9img.md`, \"Battle effect files\").",
        "Each module is one binary (`build/us/<name>.bin`) linked at its load address; `make extract-fxmod` writes",
        "the images to `disks/us/fxmod/` and `make verify` rebuilds every one of them.",
        "",
        "## Summary",
        "",
        f"- Discs read: {', '.join(map(str, discs_read))}. Dir 13 holds {nfiles} effect files on each; every file has",
        f"  at least one code chunk, {len(first)} chunks in all (summon files carry one per group).",
        f"- {len(rows)} distinct module images ({len(first) - len(rows)} chunks are byte-identical copies of another"
        " file's module).",
        f"- Size: {all_size:,} bytes in all chunks ({all_text:,} of them .text); the {len(rows)} distinct images hold"
        f" {tot_size:,} bytes, {tot_text:,} of them .text.",
        "- Per-disc differences: " + (
            "none. Every effect file's module is byte-identical on all four discs"
            + (" (whole chunks, padding included)." if raw_identical else " (padding aside).")
            if identical and len(discs) == 4 else "see the discs column."),
        "",
        "Columns: sizes in bytes (hex); `.rodata` = word 0 (the header pointer) plus jump tables/strings before the",
        "first function; ops = the header's op handlers (script opcodes 0x80+). Names in the effect column come",
        "from runtime traces of `btlfx_loadFile` (docs/runtime-findings.md) or the DS port.",
        "",
        "| module | file ids | vram | size | .rodata | .text | .data | ops | sha1 | effect |",
        "|---|---|---|---|---|---|---|---|---|---|",
    ]
    for name, m, aliases, same, L in rows:
        ids = [f"{m['fid']:#05x}"] + [a.replace("fx_", "0x") for a in aliases]
        md.append(f"| {name} | {' '.join(ids)} | {m['vram']:08X} | {L['size']:X} | {L['text']:X} | "
                  f"{L['data'] - L['text']:X} | {L['size'] - L['data']:X} | {len(L['ops'])} | {m['sha1'][:12]} | "
                  f"{KNOWN.get(m['fid'], '') if m['group'] == 0 else ''} |")
    with open("docs/effect-modules.md", "w") as f:
        f.write("\n".join(md) + "\n")
    print(f"{len(rows)} distinct modules ({len(first)} chunks in {nfiles} files), .text {tot_text:#x} "
          f"of {tot_size:#x} bytes; identical on discs {discs_read}: {identical}")


def cmd_extract(args):
    p = os.path.join(args.disks, "disc1", "FF9.IMG")
    if not os.path.exists(p):
        sys.exit(f"{p} missing (make extract-img)")
    mods = scan_disc(p)
    want = {}
    for line in open("config/fxmods.us.txt"):
        line = line.split("#")[0].split()
        if line:
            want[line[0]] = line[5]
    os.makedirs(args.out, exist_ok=True)
    n = 0
    for key in sorted(mods):
        m = mods[key]
        name = mod_name(m)
        if name in want:
            if want[name] != m["sha1"]:
                sys.exit(f"{name}: sha1 {m['sha1']} != config/fxmods.us.txt {want[name]}")
            with open(os.path.join(args.out, name + ".bin"), "wb") as f:
                f.write(m["image"])
            n += 1
    if n != len(want):
        sys.exit(f"wrote {n} of {len(want)} modules")
    print(f"wrote {n} modules to {args.out}")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--disks", default="disks/us")
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("list")
    sub.add_parser("catalogue")
    e = sub.add_parser("extract")
    e.add_argument("out")
    args = ap.parse_args()
    {"list": cmd_list, "catalogue": cmd_catalogue, "extract": cmd_extract}[args.cmd](args)


if __name__ == "__main__":
    main()
