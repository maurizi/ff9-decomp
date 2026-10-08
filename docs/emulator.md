# Investigating the running game (PCSX-Redux)

`tools/emu.py` drives a PCSX-Redux instance playing FF9, so you can see what a function or a
global does at runtime before naming it. It runs on the **host**, not in Docker.

## Setup (once per machine)

```
tools/emu.py setup --bios /path/to/SCPH1001.BIN
```

This downloads a pinned PCSX-Redux build into `tools/bin/pcsx-redux/`, copies the BIOS to
`tools/bin/bios/`, and writes `disks/us/raw/FF9 CDn.cue` next to each `.img` (Redux's CloneCD
parser reports a zero-length track and the disc never boots, but a cue works). Symbol names come from
`build/us/*.map`, so build once (`tools/docker.sh make verify`). All of this is gitignored.

Redux's config, memory cards, save states, screenshots and RAM snapshots live in
`tools/bin/redux-home/` (portable mode). `redux.log` there has the emulator's output.

## Running

```
tools/emu.py start [--disc 2] [--state NAME] [--paused]
tools/emu.py status          # running/paused, frame, pc, resident overlays
tools/emu.py stop
```

A window opens on the X display (`:1` is found automatically). Only run one instance per port
(`FF9_EMU_PORT`, default 8080). Check `tools/emu.py status` before starting another.

**Keep it paused.** Most commands run a fixed number of frames and pause again, so results are
reproducible: `frames N`, `press`, `trace`, `watch`, `until`. `resume` free-runs it for a human.
The frame counter is the bridge's own vsync count. It keeps counting across state loads, and
"paused" usually means stopped inside the vsync interrupt (`pc 80000080`).

**Save states are the main tool.** Get to a scene once, `save field_alexandria`, and then
`load` it before each experiment. Names are `[A-Za-z0-9_-]`. `states` lists them.

### Memory cards

Redux uses `tools/bin/redux-home/memcard{1,2}.mcd`. `tools/emu.py memcard list [--card 2]` shows the saves on a card.
`tools/emu.py memcard import FILE... [--card N] [--replace]` adds downloaded saves to it, with the emulator stopped.
Supported formats: DexDrive `.gme`, raw `.mcr`/`.mcd`, `.vgs`/`.mem` card images, and single-save `.mcs`/`.psx`.
Saves are appended to free blocks; `--replace` starts from a blank card, and the old card is kept as `.mcd.bak`.
To use a save, load it in-game once (title screen → Continue; the cursor already starts on Continue), then `save` a Redux state
so later runs skip the menus.

Downloaded saves go in `tools/bin/saves/`. `final-fantasy-ix.3455.gme` (GameFAQs save 3455, now on card 1) has
six Lv 99 saves: FILE01 Burmecia (disc 1), FILE02 Lucid Plains (disc 2), FILE03 Hadris Basin (disc 3),
FILE04 Pandemonium (disc 3), FILE05 Treno (disc 4), FILE06 Crystal World (disc 4). A save only loads with its own
disc inserted (`start --disc N`).

Card 2 holds GameFAQs saves 504, 505, 506, 507 and 3091 (one save each):
FILE01 Pandemonium Lv 57 (disc 3; save 506, renamed from FILE05 to avoid a name clash), FILE02 Dali inn Lv 50 (disc 1; 3091),
FILE05 Burmecia vault Lv 12 (disc 1; 504), FILE06 Iifa Tree trunk Lv 32 (disc 2; 505), FILE12 Crystal World Lv 60 (disc 4; 507).
If a game's file name is already on the card, `memcard import` moves the save to the next free number. FF9 accepts that.

Existing Redux states. Disc 1: `title_screen`, `field_burmecia_vault` (Lv 99), `field_burmecia_vault_lv12`, `field_dali_inn`,
`menu_main`, `menu_items`, `intro_test`. Disc 2 (`start --disc 2`): `disc2_load_menu`, `field_iifa_tree`.
There is no battle state yet. Get into a fight from one of the field states, then `save battle_<place>`.

### Speed and the debugger

Breakpoints need the interpreter with Redux's debugger on, and the debugger halves the speed
(~30 fps). The bridge only enables it while a breakpoint exists, so normal play runs at
60 fps. If the game crawls, `tools/emu.py bp list` shows a leftover breakpoint, and `bp clear`
removes it. `--dynarec` is faster still, but breakpoints never fire under it.

### Audio

The emulator always plays into a silent PipeWire sink, `ff9_emu`. Commands:
- `tools/emu.py audio on` / `audio off`: loop that sink to the speakers, or stop the loop. This is the mute button.
- `tools/emu.py audio record --frames 240 --seconds 4 --out x.wav`: capture what the game plays.
  The capture runs a little short because `parecord` takes a moment to start.

## Commands for investigating

| Question | Command |
|---|---|
| What's on screen? | `shot [path]`: writes a 320x224-ish PNG; open it with Read |
| Which code overlay is loaded? | `ovl` (compares `disks/us/ovl/*.bin` .text with RAM) |
| Who calls `func_X`, with what args, how often? | `trace func_X [func_Y ...] --frames 120` |
| Who writes / reads this variable? | `watch D_X [--size 2] [--read] --frames 60` |
| Stop when code runs / memory is touched | `until func_X` / `until D_X --write`, then `regs`, `read` |
| What changes when I do something? | `snap before`, `press cross`, `diff before [--range A-B] [--exclude-ovl]` |
| Where is the value shown on screen (HP, gil, ...)? | `find 9999 --size 2`, then later `find 9998 --was 9999 --snap last` |
| Raw memory | `read D_X 0x40`, `read D_X 8 --as u32` (pointers get symbolised), `write D_X 5 --size 2` |
| Input | `press cross`, `press up+cross --hold 10 --wait 30 --shot` (buttons: cross/x circle/o triangle square l1 l2 r1 r2 start select up down left right) |
| Anything else | `lua 'return Bridge.regs()'` or `lua -f script.lua` (see below) |

Addresses can be hex (`8006794C`), any name from the maps or `config/symbols*.txt`
(`func_80016620`, `D_8006794C`, `VSync`), or `name+0x1C`. Output resolves addresses back to
`name+off`. Overlay addresses get an `[ovl_XX]` tag, but only for overlays that are actually
resident. The 0x800A7000 and 0x801EDF00 slots are shared, so an address there means nothing
without `ovl`.

Watch hits report the PC of the load/store instruction itself, so grep `asm/` for it.
Trace hits record `a0`-`a3` and `ra` at function entry.

### Lua

`lua` evaluates code inside the emulator and prints the returned value as JSON.
`tools/emu/bridge.lua` defines `Bridge.*`: `read(addr,len)` (hex), `write(addr,hex)`, `r32`, `regs()`,
`setReg`, `runFrames(n)`, `hold({'CROSS'},n)`, `addBp(addr,'Exec'|'Read'|'Write',width,'log'|'break',max,label)`,
`takeLog(id)`, `removeBp`, `clearBps`, `status()`. Redux's own API (`PCSX.*`) is documented at
https://pcsx-redux.consoledev.net/Lua/introduction/. After editing `bridge.lua`, run
`tools/emu.py reload`. Its state (frame count, breakpoints) survives the reload.

Transport quirk: Redux only passes the URL to Lua web handlers, and it rejects URLs over about
250 bytes, so `emu.py` sends longer code in pieces. Large memory reads use
`/api/v1/cpu/ram/raw` directly.

## Recording findings

Append what you learn to `docs/runtime-findings.md`, one entry per symbol, with the evidence
(the commands and what they showed) and a proposed name. Don't edit `config/symbols*.txt`,
`include/` or `src/` from an investigation. The decomp agent or the coordinator applies the
names, since renames have to go through `make resplit` and `make verify`.
