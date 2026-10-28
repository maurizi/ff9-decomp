# Investigating the running game (PCSX-Redux)

`tools/emu.py` drives a PCSX-Redux instance playing FF9, so you can see what a function or a
global does at runtime before naming it. It runs on the **host**, not in Docker.

## Ground rules

- One emulator per port (`FF9_EMU_PORT`, default 8080). It also owns a window on your display, so run one
  investigation at a time.
- The emulator, BIOS, saves and states live in the gitignored `tools/bin/` of the checkout you ran `setup` in.
  A git worktree doesn't have them unless you symlink `tools/bin` across.

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
States are stored per disc (`redux-home/SLUS01251/` is disc 1, `SLUS01295/` disc 2, ...), so `states` and `load` only
see the states for the disc the emulator was started with (`start --disc N`).

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
The field/menu campaign (2026-10-24) saved once by accident: card 2 block 6 "FF9/FILE03/99:56" (a copy of the Dali inn
game) is new. Saving to an empty block asks no confirmation, so don't press Cross on an empty FILE in the save screen.

Existing Redux states. Disc 1: `title_screen`, `field_burmecia_vault` (Lv 99), `field_burmecia_vault_lv12`, `field_dali_inn`,
`menu_main`, `menu_items`, `intro_test`. Disc 2 (`start --disc 2`): `disc2_load_menu`, `field_iifa_tree`,
`field_iifa_outer_root` (random encounters), `battle_iifa_start` (battle HUD up, `ovl_0b` resident),
`battle_iifa_command` (Vivi's command menu open). Steps to reproduce the battle are in `docs/runtime-findings.md`.

Disc 1 menu states from the field/menu campaign (2026-10-24; boot with `frames 1800` before loading, as for disc 3):
- From `menu_main` (Burmecia vault, Lv 99) with the pad: `menu_status` (Zidane), `menu_equip`, `menu_ability`,
  `menu_card`, `menu_config` (each screen just opened, its overlay resident).
- From `field_dali_inn` (Lv 50) by forcing `menu_open(type, arg)` (see "Field/menu campaign" in
  `docs/runtime-findings.md`): `menu_shop_weapons` (ovl_3c, shop 0 "Weapons": Buy/Sell/Exit), `menu_shop_synth`
  (shop 0x20, synthesis), `menu_name_entry` (ovl_3a, Zidane), `menu_party_select` (ovl_3b; bogus request, four
  Zidanes), `menu_save` (ovl_39 save, slot select), `menu_chocograph` (ovl_32, nothing owned), `menu_chocograph2`
  (three chocographs and three chocobo abilities poked into the event variables), `menu_disc_change` (ovl_3e,
  "Change to DISC"). These menus were opened outside their story context; closing them returns to the Dali inn.

Disc 3 (`start --disc 3`; boot first with `frames 1800`, a state loaded straight after a cold start can hang in the
BIOS): `disc3_pandemonium_lv99_field` (= `disc3_scenario_field_ready`: card 1 FILE04 loaded, Pandemonium field 2714),
`disc3_malboro_lv99_start`, `disc3_malboro_lv99_calm` (Zidane's command menu, party Zidane/Vivi/Dagger/Eiko Lv 99),
`disc3_malboro_lv99_target`, `_ablist`, `_itemlist`, `_help`, `disc3_malboro_dmgnum` ("3233" over the Malboro),
`disc3_malboro_fire_banner`, `disc3_malboro_fire_hit`, `disc3_malboro_rewards` (EXP/AP screen), `disc3_ark_lv99_cmd`,
`disc3_ark_lv99_calm` (Ark: magic is disabled in that fight). The `_calm` and later Malboro states expect the
Malboro's ATB to be held at 0 (see below), otherwise it acts within ~250 frames. The ff9-ds fixture states
(`disc3_ark_combat_fixture`, ...) use synthetic parties; `ds_zidane_trance_entry` has Zidane alone, already in
Trance (Dyne in his menu). Campaign 3 states: `disc3_malboro_gameover` (GAME OVER screen after "Annihilated...", ovl_65
resident), `disc3_malboro_trance_pre` (despite the name: in the middle of Eiko's trance transformation effect 0x101),
`disc3_malboro_eiko_trance` (Eiko in Trance, Zidane's command menu up), `disc3_malboro_entry_1100` (1100 frames after
the ovl_0b entry of a fresh Malboro battle; the Malboro is mid-action). The Malboro states need the calm pokes below.

### Entering any battle (disc 3)

`tools/rt/enter_battle.py ENC [--field F] [--party 0,1,2,6] [--base-state S] [--frames N]` (adapted from
ff9-ds `tools/psx_battle_fixture.py`; needs that checkout at `../ff9-ds` or `$FF9DS` for `scenario_catalog.py`): from a paused, loaded field it forces the field exit (GameState.mode 2, flag 2),
breaks at main's dispatcher 0x80012834, writes the encounter's CD sector/length pair (read from `FF9.IMG`'s encounter
directory) into GameData.unk7FC/unk804, GameData.fieldId/unkE = field/encounter and optionally the party pointers,
then lets the normal battle loader run. Unlike the ff9-ds tool it keeps the save's own characters (no blob import).
Example: `enter_battle.py 899 --field 2714 --base-state disc3_pandemonium_lv99_field --party 0,1,2,6 --frames 1200`
reaches the Malboro's first command menu. Encounter ids: `python3 ../ff9-ds/tools/scenario_catalog.py --disc 3
disks/us/disc3/FF9.IMG [--find NAME]`. Any encounter whose data is on the inserted disc works; the battle
returns to `--field` afterwards. Fields can't be entered this way (the helper only swaps the battle data).

**Entering any field.** The DS port's `../ff9-ds/tools/psx_field_fixture.py` does it on the original PS1 (verified
by the port, 2026-10-28): field cleanup to main's dispatcher, then `GameData.fieldId` (+0xC), the area/field CD
extents from FF9.IMG dirs 4/2 into `sceneSector`/`sceneSectors` (+0x7FC/+0x804), `GameData.unk3A` = area, and mode 1.
It also measures VBlanks per drawn frame with a breakpoint-free vsync listener. Disc-4 state from that work:
`ds_perf_disc4_treno` (card 1 FILE05, Treno, loaded through Continue).

Battle experiment helpers in `tools/rt/` (outputs go to `build/rt/`; `tools/rt/init.sh [STATE]` starts Redux on disc 3,
loads a state and installs coverage + freeze): `freeze.lua` (per-vsync memory
pokes, e.g. `Freeze.list={{0x80109268,"0000"}}` keeps the Malboro's ATB at 0), `cov.py`/`cov.lua` (exec breakpoints on
every game function with ONE shared LuaJIT callback: a callback per breakpoint runs out of LuaJIT callback slots at
~900 and Redux reports "too many callbacks"), `act.py` (load state, play inputs, coverage/trace/watch, screenshots),
`matrix.py`/`cmp.py` (compare coverage runs), `ablate.py` (stub functions with `jr ra` and screenshot what vanishes),
`patch.lua` (rewrite the next btl_setCmd Attack into any command/ability, e.g. a summon). Note: removing the last
bridge breakpoint switches Redux's debugger off, so re-enable it (`PCSX.settings.emulator.Debug.Debug = true`)
when other breakpoints were added outside the bridge. A Lua error inside a vsync listener opens a modal
"unknown exception" dialog that freezes the web server; then the emulator has to be killed. `tools/emu.py stop` does not
help in that state (the API looks down, so it only deletes `redux.pid`); note the pid first (`pgrep -af AppRun`).
This also happened during plain `Bridge.addBp` log breakpoints plus `patch.lua` (campaign 2, t_regen run):
"Lua Breakpoint invoker threw an exception, deleting breakpoint" in `redux.log`, then the hang.
Campaign 3 hit the same hang twice during long summons (`redux.log`: a Lua stack dump "1: (Table)" ... "37: (Table)",
then "Unknown error in event listener"). What helped: switch the LuaJIT compiler off once per session
(`tools/emu.py lua 'jit.off(); jit.flush(); return 1'`); after that, ~45 runs and ~60k frames with breakpoints and
coverage had no hang. Also: wrap every callback body in pcall and always return true, create breakpoint callbacks once
and never free them, don't add or remove breakpoints inside callbacks, and keep per-vsync listeners off when not needed.
Redux passes its own cause string ("Lua Breakpoint") to callbacks, so a shared callback has to dispatch on the address.
`build/rt3/r3.lua` and `run3.py` (scratch, campaign 3) follow these rules.

Instead of the per-vsync Freeze, the Malboro stays passive with two one-time pokes after loading a state: ATB speed
(BtlUnit.unk2E) `write 0x8010926A 0 --size 1` and HP 60000 (`write 0x80109264 0xEA60 --size 2`).

Coverage blind spot: `cov.py` skips every function in `config/library_funcs.us.txt`, including the 46 ovl_0b
"hand-written asm" functions (effect emitters, particle integrators, btl_drawMesh, ...). They do run, so drop that
exclusion when they matter. Ablation screenshots only change after about 20 frames (4 frames showed nothing).
To cover an overlay that loads where battle effect modules live (ovl_65 at 0x801EDF00), start coverage after main's
`jal` into its entry (0x8001285C for battleEnd_main), and stop before the next overlay loads there.

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

## Investigation workflow

- Start with `tools/emu.py status`. If it isn't running, `tools/emu.py start` (add `--state NAME` when a fitting save
  state exists; `tools/emu.py states` lists them).
- Read the function's asm (`asm/us/<bin>/nonmatchings/...`) or its C in `src/` first, form a hypothesis, then test it:
  `trace` for callers, args and frequency; `watch` for who reads or writes a global; `snap`/`press`/`diff` for what an
  action changes; `find` for values shown on screen; `shot` to see the screen.
- Keep the emulator paused between commands and work from save states, so experiments are repeatable. When you reach a
  useful scene, save it under a descriptive name (`field_alexandria_start`, `battle_first_goblin`, `menu_items_open`).
- Overlay code (0x800A7000, 0x801EDF00, 0x80198700) is only valid while its overlay is resident. Check `tools/emu.py ovl`
  before setting breakpoints there, and get into the right game mode first: field `ovl_0a`, battle `ovl_0b`, world map
  `ovl_0c`, menus `ovl_3x`.
- Leave no breakpoints behind (`bp list` / `bp clear`): they halve emulation speed.
- Audio is muted by default; `audio record` captures sound when it matters (e.g. identifying a sound-effect function).

## Recording findings

Append what you learn to `docs/runtime-findings.md`, one entry per symbol, with the evidence
(the commands and what they showed, at which save state) and a proposed name. An investigation
only records findings. Names are applied separately through `config/symbols*.txt`, `make resplit`
and `make verify`, and the entry is then marked **applied**.

## DuckStation + MCP (alternative)
DuckStation's GDB stub, driven through the `duckstation` MCP server, gives Claude Code direct
`mcp__duckstation__*` tools: `read_memory`, `write_memory`, `get_registers`, `set_breakpoint`, `step`,
`pause`/`resume`, `dump_region`, `tail_log_lines`. It's handy for quick memory reads and breakpoints.
`tools/emu.py` (PCSX-Redux) is still the tool for scripted traces, save states and pad input. Don't run
both emulators against one investigation at once.

Setup (once per machine):
1. `tools/bin/duckstation/DuckStation-x64.AppImage` (github.com/stenzek/duckstation releases), with an empty
   `portable.txt` beside it so its config, BIOS dir and memory cards stay in that folder. `settings.ini` there
   sets `[Debug] EnableGDBServer = true`, `GDBServerPort = 19000`, `[BIOS] SearchDirectory` (a copy of
   `SCPH1001.BIN` in `tools/bin/duckstation/bios/`), the Null audio backend and `LogToFile`.
2. MCP server: `git clone https://github.com/snowyegret23/Duckstation_MCP ~/src/ext/Duckstation_MCP`, apply
   `tools/duckstation_mcp-linux.patch` (upstream is Windows-only; the patch skips the Win32 window-capture and
   keyboard modules and adds the XDG / `DUCKSTATION_LOG` log path), then `uv venv .venv && uv pip install -e .`.
3. `claude mcp add --scope local duckstation -e DUCKSTATION_LOG=$PWD/tools/bin/duckstation/duckstation.log
   -- ~/src/ext/Duckstation_MCP/.venv/bin/python -m duckstation_mcp`, then restart Claude Code.

Start the game (same X display lookup as `emu.py`):
`DISPLAY=:1 XAUTHORITY=/run/user/$UID/gdm/Xauthority tools/bin/duckstation/DuckStation-x64.AppImage -- "disks/us/raw/FF9 CD1.cue" &`,
then call the `connect` tool (127.0.0.1:19000). Window capture and pad input tools don't work on Linux.
Addresses are the usual KSEG0 ones (`0x8006794C`).
