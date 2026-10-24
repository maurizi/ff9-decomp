# Library audit: asm-only PsyQ internals and the movie overlay

Answers ff9-ds `docs/decomp-requests.md` R25 (items 3 and 4), 2026-10-24.

## Method

Every reference to each routine was collected two ways:

- **Symbolic:** every `.s`, `.c` and `.h` under `asm/`, `src/`, `include/` and `src_equiv/`, which covers main, the 19 code
  overlays and the 380 effect modules.
- **Raw:** a scan of every binary image (`SLUS_012.51`, all 21 files in `disks/us/ovl/`, all 380 `disks/us/fxmod/*.bin`)
  for `jal`/`j` encodings, 32-bit data words, and `lui`+`addiu/ori/load/store` pairs that form the address. This
  also finds function-pointer tables and address constants that splat didn't symbolize.

The two methods agree. No overlay and no effect module contains a reference to any of the routines below.

## Verdict: main-exe library internals (R25 item 3)

**None is reachable from game code except through PsyQ's public API.** Each routine is referenced only from (a) another
library function, or (b) a library dispatch table in `.data`, and those tables are read only by PsyQ functions
(signature-matched in `config/symbols.psyq.us.txt` or listed in `config/library_funcs.us.txt`). Game code calls
only the public entry points (`DrawSync`, `LoadImage`, `ResetCallback`, `PadStartCom`, …), which a port replaces
together with these internals.

The table covers the 24 main routines in the port's `build/ds/stubs.txt` (R25 counts 26) and the 7 other asm-only
library routines in the same address ranges (marked †), so nothing in the ranges is left out.

### libgpu (`sys.c`): the `_gpu` dispatch table

`D_80066614` is a 16-word table whose first word points to the rodata string `$Id: sys.c,v 1.140 1998/01/12 …`.
`D_80066654` points to it and is read only by libgpu (`ResetGraph`, `DrawSync`, `ClearOTagR`, `LoadImage[2]`,
`StoreImage[2]`, `MoveImage[2]`, `ClearImage[2]`, `DrawOTag[2]`, `DrawOTagEnv`, `DrawPrim`, `PutDrawEnv`,
`PutDispEnv`, `SetDispMask`, `SetGraphQueue`, `GetODE`). The slot roles come from which public function loads which offset.

| routine | table slot | role (from its users) | references | game caller? |
|---|---|---|---|---|
| func_80014DFC | +0x04 | queue entry, wraps func_80014E20 | table only; **no function reads slot +0x04** (dead in this exe) | no |
| func_80014E20 | +0x08 | GPU command queue: enqueue a (handler, args) command, installs func_800150D0 as the DMA callback | table; `jal` from func_80014DFC | no (its stale `ff9.h` prototype had no user and was removed) |
| func_8001463C | +0x0C | ClearImage handler (fill rectangle) | table; `ClearImage`, `ClearImage2` | no |
| func_80014D28 | +0x10 | GP1 control-port write | table; `SetDispMask`, `PutDispEnv` | no |
| func_80014D44 | +0x14 | write one primitive to GP0 | table; `DrawPrim` | no |
| func_80014D84 | +0x18 | send a linked list / OT by DMA | table; `DrawOTag[2]`, `DrawOTagEnv`, `PutDrawEnv`, `MoveImage[2]`; `jal` from func_8001463C | no |
| func_80014AA8 | +0x1C | StoreImage handler (VRAM→RAM) | table; `StoreImage[2]` | no |
| func_8001486C | +0x20 | LoadImage handler (RAM→VRAM) | table; `LoadImage[2]` | no |
| func_800150D0 | +0x24 | queue executor / DMA-end callback | table; `jal` from func_80014E20, func_8001546C; address taken in func_80014E20 and `_GPU_ResetCallback` | no |
| func_8001455C | +0x2C | ClearOTagR body | table; `ClearOTagR` | no |
| func_80014DCC | +0x30 | GPU info query (GP1 0x10xxxxxx, then reads GPUREAD) | table; `jal` ×3 from func_8001463C | no |
| func_80015330 | +0x34 | GPU reset | table; `SetGraphQueue`; `jal` from `ResetGraph` | no |
| func_8001546C | +0x3C | DrawSync body (wait for queue/GPU idle) | table; `DrawSync`, `DrawPrim` | no |
| func_800155A8 † | n/a | internal helper | `jal` from func_8001455C, func_8001486C, func_80014AA8, func_80014E20, func_8001546C | no |
| func_800155DC † | n/a | GPU timeout handler (prints "GPU timeout:que=%d,stat=%08x,…", resets the queue) | `jal` from the same handlers plus `LoadImage2`, `StoreImage2`, `MoveImage2`, `DrawOTag2` | no |
| func_80015720 † | n/a | internal helper | `jal` from func_80015330 | no |
| func_80015C00 † | n/a | internal helper | `jal` from `ResetGraph` ×3, `SetDispMask`, func_80015330 | no |

### libetc (`intr.c`): the interrupt manager table

`D_8006788C` is a table whose first word points to `$Id: intr.c,v 1.75 1997/02/07 …`. `D_800678AC` points to it and is
read only by `ResetCallback` (+0x0C), `InterruptCallback` (+0x08), `DMACallback` (+0x04), `VSyncCallback`/`VSyncCallbacks`
(+0x14), `StopCallback` (+0x10), `RestartCallback` (+0x18) and func_8001603C, which fills the +0x04/+0x14 slots.

| routine | table slot | role | references | game caller? |
|---|---|---|---|---|
| func_800162E4 | +0x08 | InterruptCallback body (set a handler for one IRQ) | table only | no |
| func_8001603C | +0x0C | ResetCallback body (hooks the exception entry, starts the VSync/DMA dispatchers) | table only | no |
| func_8001642C | +0x10 | StopCallback body | table only | no |
| func_800164CC | +0x18 | RestartCallback body | table only | no |
| func_80016114 † | n/a | interrupt dispatcher | `jal` from func_8001603C | no |
| func_80015DD0 † | n/a | VSync timeout handler | `jal` ×2 from `VSync` | no |

### libpad (`PDRESRES.OBJ` and the SIO state machine)

`D_800681EC` is a BIOS interrupt-queue record `{next, _padIntPad, _padIsVsync, 0}`. `PadStartCom` registers it with
`SysEnqIntRP(2, …)` and `PadStopCom` removes it with `SysDeqIntRP`. Those two are the only code that refers to it.
`_padIntPad` calls func_8001ADB4, which advances a step counter and calls through `D_80068214[step]`. That table
holds the five step functions, and func_8001ADB4 is the only code that reads it.

| routine | role | references | game caller? |
|---|---|---|---|
| _padIsVsync | BIOS IRQ-queue verifier (is this a VBlank interrupt?) | `D_800681EC` word only | no |
| _padIntPad | BIOS IRQ-queue handler (one pad transfer per VBlank) | `D_800681EC` word only | no |
| func_8001BE14 | SIO transfer step 0 | `D_80068214[0]` only | no |
| func_8001BE5C | SIO transfer step 1 | `D_80068214[1]` only | no |
| func_8001BF34 | SIO transfer step 2 | `D_80068214[2]` only | no |
| func_8001BFE0 | SIO transfer step 3 | `D_80068214[3]` only | no |
| func_8001C0FC | SIO transfer step 4 | `D_80068214[4]` only | no |

Game code reaches libpad only through `PadInitDirect` and `PadStartCom` (main.c, boot).

The descriptive roles above come from the code. The routines keep their placeholder names: their PsyQ names are
`static` names that the libraries don't export, and this project doesn't take names from outside sources.

## ovl_64: the movie player, not a debug tool (R25 item 4)

Overlay file 0x64 is FF9's FMV/MBG movie player: the `mbg_*` stream/MDEC code plus PsyQ libpress. It is not a mode
overlay: `game_loadModeOverlay` never loads it. Three retail code paths read it from `FF9.IMG` dir 0 to
`D_80067944 + 0xF1700` = 0x80198700 (`file_find(0, 0x64)` + `func_8002177C`) and call into it:

- **Field (ovl_0a):** event-script opcode 0x28 → `fld_movieCommand` (mode 0) → `fld_movieStart` stores list/entry/rgb24.
  `fld_movieUpdate`, run every frame by `fld_main`, loads the overlay, then calls `MBG_init`, `mbg_setRgb24`,
  `mbg_setEndCallback`, `func_8019AB98`, `mbg_play`, `mbg_update`, `mbg_getLastFrame`, `mbg_getStartCamera` and
  `mbg_shutdown`. Other field code calls `mbg_getFrame` (script variable 14) and `mbg_getGeomOffset`.
- **Opening (ovl_14):** `func_800A9220` loads it and plays list 1 entry 0 (`\OPENING\FMVD001.STR`). It also calls
  `mbg_stop` and `mbg_streamSetBaseSector`.
- **Ending (ovl_15):** `func_800A7378(4, 0x12/0x13)` plays `\ENDING\FMV059.STR` / `FMV060.STR`.

The movie table `g_mbgMovieLists` has five lists. Lists 1–4 are the movies on discs 1–4. List 0 holds `\SPARE\FMV090–094`
and `\TEST\FMV910–990`, but **no retail disc has a `\SPARE\` or `\TEST\` directory** (checked in the ISO9660
directories of all four images), so list 0 can't play. `\TEST\FMV990.STR` is only the first string in the overlay's
rodata, which is why the overlay was first thought to be a debug player.

### Which ovl_64 functions retail code reaches

Entry points come from the raw scans of every other binary. The closure follows `jal`s, taken addresses (the MDEC and
CD callbacks) and the hand-written VLC code's branches between labels. There are no function pointers in ovl_64's
`.data`, and no other binary refers to the overlay's data.

**Called from ovl_0a/14/15 (13):** MBG_init, mbg_play, mbg_update, mbg_setRgb24, mbg_shutdown, mbg_stop (ovl_14),
mbg_getFrame, func_8019AB98, mbg_setEndCallback, mbg_getLastFrame, mbg_getStartCamera, mbg_getGeomOffset,
mbg_streamSetBaseSector (ovl_14/15).

**Reached from those (39):** mbg_readFrame, mbg_mdecOutCallback, mbg_setGraphEnv, mbg_vlcBuildTable,
mbg_vlcDecode, mbg_vlcDecodeBlock, mbg_vlcDecodeLumaBlockV3 (a branch target inside mbg_vlcDecode's hand-written code,
not a separate call), mbg_applyMask, mbg_streamOnVideoSector, mbg_streamOnAudioSector, mbg_streamStart,
mbg_streamGetFrame, mbg_streamReadyCallback, mbg_streamReset, mbg_streamStop, mbg_streamSetRing,
mbg_streamSetAudioBuffer, mbg_streamPushAudio, mbg_streamSeek, mbg_streamGetPos, mbg_streamGetLastFrame,
mbg_streamUngetFrame, mbg_streamSetPaused, mbg_streamClearRing, mbg_streamFreeFrame, func_8019D3F0, and libpress
DecDCTReset, DecDCTin, DecDCTout, DecDCTinSync, DecDCToutSync, DecDCToutCallback, MDEC_reset, MDEC_in, MDEC_out,
MDEC_in_sync, MDEC_out_sync, func_8019D354, func_8019D36C.

**Dead in the retail game (19):** no reference anywhere, or only from another dead function.

| function | note |
|---|---|
| mbg_setDispEnv, mbg_putDispEnv | display helpers nobody calls |
| mbg_playToEnd | blocking play loop (calls mbg_update) |
| mbg_setPause | pause/resume (its callee mbg_streamSetPaused is live via mbg_mdecOutCallback) |
| mbg_getCameraSelect, mbg_setCameraSelect → mbg_streamGetCameraSelect, mbg_streamSetCameraSelect | camera-record select |
| mbg_setScratchVlcTable | scratchpad VLC table switch |
| mbg_getEntryCount, mbg_getEntryName | movie-list queries (a test menu would use these) |
| mbg_vlcSetSize | VLC output limit setter (the limit is read by the live mbg_vlcDecode) |
| mbg_drawStreamDebug | CD ring debug bars |
| mbg_streamGetStartPos, mbg_streamPeekFrame, mbg_streamCountFree | stream queries |
| DecDCTGetEnv, DecDCTPutEnv, DecDCTinCallback | unused libpress entries |

No separate "FMV990 test" entry function exists. The test movies are only data (list 0), and the list queries above
are the only code that looks like test tooling.

Of the 21 asm routines still in `src/ovl_64`, the ones a port must provide are mbg_vlcBuildTable, mbg_vlcDecode
(+ mbg_vlcDecodeBlock, mbg_vlcDecodeLumaBlockV3), mbg_applyMask, func_8019D36C and the live libpress/MDEC functions
(DecDCTReset, DecDCTin, DecDCTout, DecDCTinSync, DecDCToutSync, DecDCToutCallback, MDEC_*). The ff9-ds port already
replaces the player as a whole (`port/game/movie.c`). mbg_vlcSetSize, DecDCTGetEnv, DecDCTPutEnv and DecDCTinCallback
are dead.
