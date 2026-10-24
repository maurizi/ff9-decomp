# Source files (translation units)

Square compiled the boot exe and every overlay from many C files (and a few hand-written asm
files), linked with PsyQ's `psylink`. This page records the translation-unit (TU) boundaries
recovered from the binaries and how `src/` is split along them. Done so far: `main`, `ovl_0a`,
`ovl_0b`, `ovl_0c`. The other overlays are still one C file each.

## How the boundaries were found

None of these leave padding in `.text`: psylink packed every object's sections at 4 bytes and
gcc 2.8.1 aligns functions to 4, so there is no zero word at any function boundary (checked for
all four binaries). The evidence instead comes from:

1. **Library placement (main only, strong).** PsyQ library objects (signature-identified,
   `config/symbols.psyq.us.txt`) are interleaved with game code because psylink pulls a library
   module in right after the first object that references it. Every run of library code is
   therefore its own object, and the object ending just before it is the TU that contains the
   first caller. Example: `GetClut` (libgpu P01.OBJ) sits at 0x80028D90 and its first caller in
   address order is 0x80028204, so the TU before it runs at least from 0x80028204 to 0x80028D90 —
   it can't be cut in between. This both places boundaries and forbids them (`NOCUT` ranges).
2. **Hand-written asm modules (strong).** Functions listed as hand-written asm in
   `config/library_funcs.us.txt` (no ABI, global registers, data words in `.text`) are separate
   objects; their edges are TU edges.
3. **Jump-table parity (strong for "a boundary exists here").** gcc emits `.align 3` before
   each jump table, relative to its object's `.rodata`, but psylink placed objects at 4. So all
   jump tables of one TU share the same address mod 8, and two consecutive tables with different
   parity have a TU boundary between their functions. splat's own "file split suggestions" (main
   rodata 0x1110, 0x14F4, 0x28C0, 0x2E8C) are this signal; all four coincide with TUs below.
4. **`.rodata` ownership (strong for rodata placement).** Each TU's rodata is contiguous and in
   TU order. Within a TU it is not always in function order (e.g. main 0x80010BD0 belongs to
   `func_80028DB0` but follows the tables of 0x80029FE4/0x8002A730; ovl_0c's w_frame strings
   are 114, 121, 115, 122, 123, 118), which ties those functions into one TU.
5. **`.data`/`.bss` order (medium).** Each object's initialized data and its static bss are
   contiguous and in link order. For a candidate cut, every data item used only by functions
   below it must sit below every item used only by functions above it (items used across more
   than 40 functions, and outliers, are ignored). Cuts that violate this are excluded; cuts
   that satisfy it with data on both sides are candidates. main: `.data` 0x800665E0-0x8006FE18,
   `.bss` 0x80071E30-0x80083F40 (static bss; the later zero areas are common symbols).
6. **Call-graph cuts (weak).** Few or no calls crossing a candidate cut.
7. **Names (weak to medium).** Prefix families applied by the naming work (`akao_`, `win_`,
   `chr_`, `btlfx_`, `btlui_`…) and Square's own names from strings (`Hi_*`, `w_frame*`,
   `w_memoryman*`, `w_readtx*`, `w_frmbuf*`, `w_music*`, `w_movement*`).
8. **Compiler flags.** ovl_0b 0x800C2DF4-0x800C58E4 is `-O1` (`btlcam_o1.c`).

**Check that the split is right:** with the TU split, the binaries now build without the
`.align 3 -> .align 2` rewrite and without the 16 hand-placed `.word 0` / `.align 3` rodata
padding statements the merged files needed (Makefile `TU_SPLIT`). gcc's own jump-table
alignment inside each object, with objects linked at 4 (`subalign: 4`), reproduces every
original padding word in main, ovl_0a, ovl_0b and ovl_0c. A TU that started at the wrong
rodata parity would misplace its jump tables and fail `make verify`.

Strength in the tables: **strong** = library/asm object edge, `NOCUT` constraint, or
jump-table parity plus rodata ownership; **medium** = a data-order cut in a narrow window,
usually with a call-graph cut or a name change; **weak** = the boundary exists (parity flip or
data cut) but its exact function is a guess from names/call graph, or only names support it.
Files named `tu_XXXXXXXX.c` (start address) have no evidence-backed subsystem name yet. Library
runs are `psyq_XXXXXXXX.c`, hand-written asm `*_asm.c`; both stay `INCLUDE_ASM` and are not
decompiled.

The analysis scripts were throwaway (build/tu/an.py in the worktree that did this). The
reasoning above is enough to redo any of it.

## main (SLUS_012.51): 81 files

Game TUs (start - end, function count, evidence for the start / end edges):

| file | range | funcs | start | end | notes |
|---|---|---|---|---|---|
| main.c | 800127A0-80012B68 | 4 | strong (.text start) | strong (SNMAIN pulled by `__main` call in `game_main`) | rodata 80010000-80010060; `func_80012AD4` (SN boot helper) stays INCLUDE_ASM here |
| init.c | 800169B8-80016BB8 | 3 | strong | strong (libgte/libspu/libcard first called from `game_initLibraries`) | |
| pad.c | 80018F20-80019798 | 8 | strong | strong (libpad first called from `pad_update`) | |
| db.c | 8001C6F8-8001D0F8 | 15 | strong | strong (memmove/SetDefDrawEnv first callers 0x8001CD04/0x8001CFB4) | rodata 80010380-800103A0 (db_lookup table, parity 0); weak internal candidate at `db_register` |
| tu_8001D268.c | 8001D268-8001D898 | 8 | strong | strong (memset first caller 0x8001D3C4) | rodata 800103A0-800103F0 |
| file.c | 8001D8A8-8001E3C0 | 18 | strong | strong (CdSearchFile first caller 0x8001DCB8) | rodata 800103F0-8001040C; `file_find`, `game_setNextScene` |
| tu_80021640.c | 80021640-80021938 | 10 | strong (libcd) | weak (names; 0x80021938.. only called from the preload code) | `evt_setNextLocation`, `evt_setNextBattle`, `mbg_resetState` |
| preload.c | 80021938-8002267C | 28 | weak | weak (`cd_*` names from 0x8002267C) | rodata 800108CC-800108F8 (jump table parity 4) |
| cd.c | 8002267C-80023634 | 34 | weak | strong (CdReset/CdSetDebug first called from `cd_init`/`cd_resetState`: NOCUT 0x80022AC4-0x80023634) | rodata 800108F8-80010910 ("Cannot find file. (%s)") |
| tu_800236C4.c | 800236C4-8002420C | 25 | strong | strong (SetDrawTPage first caller 0x80023A6C) | `game_*PlayTime`; weak internal candidate 0x800239D0-0x80023A6C |
| ui.c | 8002423C-80028D90 | 72 | strong | strong (GetClut first caller 0x80028204) | `win_build*`, `ui_draw*`, `item_*`; data interleaves all through, no internal cut |
| text.c | 80028DB0-8002BEEC | 38 | strong | medium (data cut must lie in 0x8002BEA4-0x8002C158) | rodata 80010910-80010CD0 = splat suggestion 0x1110, parity 0 |
| tu_8002BEEC.c | 8002BEEC-8002C9B0 | 29 | medium | weak (data allows any cut up to `win_init`; `win_` names from 0x8002C9B0) | `gfx_selectBuffer` |
| window.c | 8002C9B0-8002EB64 | 42 | weak | strong (SystemError first caller `win_get`) | rodata 80010CD0-80010CF4 (owned out of function order by 602 and 575: one TU) |
| menulist.c | 8002EB74-8002F7E8 | 18 | strong | medium (data cut 0x8002F6C8-0x8002F800) | `menuList_*`, `snd_playSe` |
| tu_8002F7E8.c | 8002F7E8-80030824 | 43 | medium | strong (libgpu/libgte run first called from 0x800302D0) | |
| tu_80030AF4.c | 80030AF4-80032058 | 16 | strong | strong (SetTile first caller 0x80031ED8) | |
| menu.c | 80032078-80032B08 | 29 | strong | medium (data cut 0x80032A34-0x80032B08) | `menu_open`, `menu_doOpen` |
| chr.c | 80032B08-80035108 | 52 | medium | medium (data cut 0x80033E84-0x80035108, no call crosses 0x80035108) | rodata 80010CF4-80010E48 = splat suggestion 0x14F4, parity 4; `chr_*`, `party_*` |
| tu_80035108.c | 80035108-8003575C | 19 | medium | strong (hand-written text asm) | rodata 80010E48-80011078 |
| tu_800357E0.c | 800357E0-80035D3C | 8 | strong | medium (data cut must lie in 0x800357E0-0x80035D3C) | |
| tu_80035D3C.c | 80035D3C-80036650 | 19 | medium | medium (no call crosses 0x80036650, data cut) | rodata 80011078-800112A4 holds "path=%s" and the assert path "f:/code/dms/loc_us/ff9db.h": Square's db code |
| tu_80036650.c | 80036650-80037D6C | 11 | medium | medium (data cut 0x800378C4-0x80037D6C) | rodata 800112A4-80011594 |
| event.c | 80037D6C-8004BB7C | 62 | medium | strong (math asm) | rodata 80011594-80011A6C; `evt_execInstr`, `evt_init`, `evt_getArg*` |
| tu_8004BBF0.c | 8004BBF0-8004BC90 | 2 | strong | strong (libapi COUNTER first caller here) | |
| rand.c | 8004BE00-8004BF20 | 1 | strong | strong | `rand_init`; the asm `rand_refill` follows |
| msg.c | 8004BFB0-80050CF8 | 60 | strong | strong (GetTPage first caller 0x80050BE0) | rodata 80011A6C-80012090; `msg_open`, `evt_clearGlobalVars`; weak internal candidate at 0x80050044 |
| tu_80050D38.c | 80050D38-80050F4C | 3 | strong | strong | |
| tu_80053924.c | 80053924-80054888 | 9 | strong | strong (SetDrawMove first caller 0x80053A10) | |
| tu_80054A28.c | 80054A28-800551F0 | 16 | strong | medium (bss 0x8007BD38-0x8007BD44 used only by 0x80054A28-0x800551D4; akao state from 0x80055230) | |
| akao_api.c | 800551F0-8005650C | 69 | medium | strong (CdMix first caller 0x80055E14) | small exported wrappers, `akao_*CdVolume*` |
| akao_init.c | 8005658C-80056CAC | 12 | strong | strong | `akao_initState`, `akao_init`, `akao_quit` |
| akao_voice.c | 8005715C-800592CC | 41 | strong | strong | `akao_keyOn` .. `akao_flushVoice` |
| tu_8005936C.c | 8005936C-80059504 | 3 | strong | strong | |
| akao_cmd.c | 80059548-8005CC14 | 66 | strong | strong (SpuSetReverbModeType first caller `akao_setReverbMode`) | `akao_cmd*`, `akao_command` |
| akao.c | 8005CDE4-80062304 | 147 | strong | strong (SpuSetIRQAddr first caller 0x80060ED8) | rodata 800120C0-800121C0 = splat suggestion 0x28C0, parity 0; weak internal candidates at 0x8005FCEC and 0x80060784 (no calls cross) |
| tu_80062344.c | 80062344-80062B3C | 24 | strong | strong | rodata 800121C0-80012578 |
| dep.c | 80062E3C-800630E8 | 2 | strong | strong | `dep_find` |
| tu_80063118.c, tu_800633E4.c, tu_80063580.c, tu_80063830.c, tu_800651D8.c | | 3, 4, 6, 1, 1 | strong | strong | small objects between library modules |
| tu_80064BE4.c | 80064BE4-80064D78 | 6 | strong | strong | rodata 80012658-8001266C; `func_80064C24` (INCLUDE_ASM) is game code |
| tu_800654F8.c | 800654F8-80065E60 | 20 | strong | strong | rodata 8001268C-800127A0 = splat suggestion 0x2E8C, parity 4 |

Library and asm files (INCLUDE_ASM only, apart from a few decompiled unsigned library helpers):

| file | contents |
|---|---|
| psyq_80012B68.c | libsn SNMAIN, BIOS stubs, libgpu SYS (+ internals), libetc VSYNC/INTR/INTR_VB/INTR_DMA/VMODE, libc A63, libapi C114 (rodata 80010060-80010330 from `$Id: sys.c`) |
| psyq_80016BB8.c | libgte MSC00/REG12/PATCHGTE, libspu S_I..S_CRWA, libcard INIT/PATCH/END, libapi PAD/PATCH/CHCLRPAD (rodata 80010330-80010380) |
| psyq_80019798.c | libpad PDCMD1..PDRESRES and internals |
| psyq_8001D0F8.c, psyq_8001D898.c | memmove, libgpu E03/E04; memset |
| psyq_8001E3C0.c | libcd ISO9660 .. S_023 and the CD_* internals (rodata 8001040C-800108CC) |
| psyq_80023634.c, psyq_8002420C.c, psyq_80028D90.c, psyq_8002EB64.c, psyq_80030824.c, psyq_80032058.c | CdReset/CdSetDebug; SetDrawTPage; GetClut; SystemError; libgpu P10/P18 + libgte; SetTile |
| psyq_8004BC90.c, psyq_80050CF8.c, psyq_80053204.c, psyq_80054888.c | libapi COUNTER; GetTPage; ratan2; SetDrawMove/ScaleMatrix |
| psyq_8005650C.c, psyq_80056CAC.c, psyq_800592CC.c, psyq_80059504.c, psyq_8005CC14.c, psyq_80062304.c | libcd S_020 + libspu (rodata 80012090-800120C0 in psyq_80056CAC) |
| psyq_80062B3C.c .. psyq_80065E60.c (8 files) | the late libgpu/libgte/libc/libcd/libcard runs (sprintf rodata 80012578-80012658, CdGetDiskType 8001266C-8001268C) |
| text_asm.c, math_asm.c, rand_asm.c | `text_setBank`..`text_getFrom`; `math_cos`/`math_sin` table code; `rand_refill` |
| evt_expr_asm.c | the event-expression asm module 0x80050F4C-0x80053204 (plus the empty C stub `func_80051274`) |
| asm_80053384.c | empty C stub `func_80053384` and the asm module 0x8005338C-0x80053924 |

## ovl_0b (battle): 28 files

Jump-table parities in rodata order: 21-50 → 4, 64/66 → 0, 81-118 → 4, 181 → 0, 189 → 4,
210-213 → 0, 217 → 4, 266 → 0 | 298/299 → 0, 303 → 4 | 475 → 4, 588 → 0, 663-759 → 4
(function index in address order). Every flip is a boundary; where a flip window spans several
functions the cut is at the function with fewest crossing calls (weak position).

| file | range | funcs | evidence |
|---|---|---|---|
| btl_main.c | 800A859C-800AF2E0 | 64 | start strong; end: parity flip 50→64, cut at `btl_update` (fewest crossing calls) — weak position |
| btl_update.c | 800AF2E0-800B03B0 | 5 | parity 0 (`btl_update`, `btl_victory`); end in the 66→81 flip window, cut at `btl_startBattle` — weak position |
| btl_cmd.c | 800B03B0-800BAFEC | 106 | parity 4 (`btl_setCmd`, `btl_execCmd`, `btl_calcCmd`, `btl_setPhysParams`); end in 118→181 window, cut at `btl_initParty` (data cut allowed) |
| btl_init.c | 800BAFEC-800BC0D0 | 7 | parity 0 (`btl_initWeapon`); end in 181→189 window — weak position |
| btl_unit.c | 800BC0D0-800BD414 | 9 | parity 4 (`btl_updateKo`); end in 189→210 window — weak position |
| btl_attr.c | 800BD414-800BEC64 | 23 | parity 0 (`btl_get/setUnitAttr`, `btl_get/setBattleVar`); end in 213→217 window (one function wide) — medium |
| btl_status.c | 800BEC64-800C23C8 | 49 | parity 4 (`btl_addStatus`); end in 217→266 window — weak position |
| btl_cmdfx.c | 800C23C8-800C2DF4 | 4 | parity 0 (`btl_startCmdEffect`); end strong (-O1 range) |
| btlcam_o1.c | 800C2DF4-800C58E4 | 24 | strong: compiled with -O1 (was ovl_0b_o1.c) |
| btl_mesh.c | 800C58E4-800C8BE0 | 9 | strong edges (was ovl_0b_4.c); holds asm `btl_drawMesh`, `btl_calcBoneMatrices` and 7 C functions — kept together, see below |
| btl_model.c | 800C8BE0-800CA3CC | 18 | start strong (was ovl_0b_3.c); parity 4 (`btl_setModelAbr`); end weak (names: `btlseq_*` from 0x800CA3CC) |
| btlseq.c | 800CA3CC-800CC044 | 43 | weak edges (names; calls barely cross 0x800CC044) |
| btl_popup.c | 800CC044-800CE05C | 24 | weak start; end medium (data cut exactly at `btlbg_init`) |
| btlbg.c | 800CE05C-800CEED4 | 11 | medium start; end strong (asm) |
| btlbg_asm.c | 800CEED4-800CFA80 | 20 | strong: hand-written asm (btlbg draw, btlfx math helpers, two data words); the `textbin` table 800CFA80-800CFC0C follows |
| btlfx.c | 800CFC0C-800D3ED4 | 36 | strong edges (textbin, asm) |
| btlfx_emit_asm.c | 800D3ED4-800D616C | 16 | strong: hand-written asm emitters |
| btlfx_script.c | 800D616C-800DA0FC | 101 | start strong; parity 4 (`btlfx_runScript`); end in 475→588 window, cut before the pool functions (one crossing call) — medium |
| btlfx_pool.c | 800DA0FC-800DB424 | 20 | parity 0 (`btlfx_updateLoader`); end: rodata of the `Hi_*` functions starts the next TU — medium |
| hi_model.c | 800DB424-800DD27C | 52 | Square's `Hi_*` names with their own error strings; rodata 800A7D00-800A81D0; end strong (asm) |
| btl_mesh_asm.c | 800DD27C-800DF508 | 4 | strong: asm `btl_drawMeshOffset`, `btlfx_projectSliceMesh`, `btlfx_projectMorphMesh` around the compiled-C `func_800DE11C` |
| hi_vertex.c | 800DF508-800DFE4C | 11 | strong edges (asm both sides); `Hi_SplitMdlVertex` .. |
| btlfx_particle_asm.c | 800DFE4C-800E00DC | 5 | strong: asm particle steppers |
| btlfx_heap.c | 800E00DC-800E3EEC | 23 | strong edges; parity 4 (`func_800E0168`..`func_800E2F84`); `btlfx_heap*` |
| btlfx_runframe_asm.c | 800E3EEC-800E4098 | 1 | strong: asm `btlfx_runFrame` |
| btlfx_op.c | 800E4098-800E9A74 | 118 | start strong; end medium (no call crosses `btlui_vsync`, data cut) |
| btlui.c | 800E9A74-800F4A00 | 250 | medium start; end weak (no call crosses 0x800F4A00, data cut window 0x800F3DE0-0x800F4DFC); internal candidates not taken |
| btlui_frame.c | 800F4A00-800F51E8 | 17 | `btlui_submitOt`, `btlui_beginFrame`, `btl_drawBanner`, `btl_buildBanner*` |

## ovl_0a (field): 11 files

Parities: 2/19 → 4, 50/62/64 → 0, 80 → 4. Asm: 0x800B5958, 0x800C1DC4-0x800C1E14, 0x800C9AB8.

| file | range | funcs | evidence |
|---|---|---|---|
| fld_main.c | 800A7A48-800AA3F4 | 38 | parity 4 (`fld_main`, `fld_exit`); `fld_load`, `fld_depFind*`; end in the 19→50 window, cut at the fewest crossing calls — weak position |
| fld_movie.c | 800AA3F4-800B0124 | 42 | parity 0 (`fld_movie*`); end in the 64→80 window — weak position |
| tu_800B0124.c | 800B0124-800B5004 | 47 | parity 4; end: data cut 0x800B3ACC-0x800B5004 and a single crossing call — medium |
| tu_800B5004.c | 800B5004-800B5958 | 28 | rodata 800A7170-800A71F4 owned out of order by 0x800B5660/0x800B54FC; end strong (asm) |
| fld_asm_800B5958.c | 800B5958-800B61A4 | 1 | strong: hand-written asm |
| tu_800B61A4.c | 800B61A4-800C1DC4 | 92 | strong edges; internal cuts possible (no calls cross 0x800BA190-0x800BD130; data cuts at 0x800BDC44, 0x800C1A4C) but not placed |
| fld_asm_800C1DC4.c | 800C1DC4-800C1E14 | 4 | strong: hand-written asm |
| tu_800C1E14.c | 800C1E14-800C4B30 | 19 | strong start; end: data cut 0x800C3E18-0x800C4B30 — weak position |
| bgi.c | 800C4B30-800C8124 | 58 | `bgi_dumpText` and its debug strings (rodata 800A71F4-800A7860); end: no call crosses, data cut — medium |
| tu_800C8124.c | 800C8124-800C9AB8 | 27 | rodata 800A7860-800A7A48; end strong (asm) |
| fld_asm_800C9AB8.c | 800C9AB8-800C9CCC | 1 | strong: hand-written asm |

## ovl_0c (world map): 14 files

Parities: 35 → 4, 68-169 → 0, 237-259 → 4. The `w_*` names are Square's (from strings) and
look like module prefixes; the boundaries below fall exactly where both the data order and the
call graph cut between them.

| file | range | funcs | evidence |
|---|---|---|---|
| wm_main.c | 800A8860-800AD204 | 52 | parity 4; `wm_loadFile`, `wm_depFind*`; end medium (data cut 0x800ACEEC-0x800AD204, fewest crossing calls) |
| w_movement.c | 800AD204-800B3844 | 46 | parity 0; `w_movementChange*` and their strings; end strong (asm) |
| wm_mesh_asm.c | 800B3844-800B6908 | 14 | strong: hand-written mesh vertex/face emitters |
| w_frame.c | 800B6908-800B88C0 | 32 | `w_frame*` (strings owned out of order: one TU); end medium (data cut exactly at `w_memorymanHeapAlloc`) |
| w_memoryman.c | 800B88C0-800BA528 | 20 | `w_memoryman*`; end weak (fewest crossing calls) |
| tu_800BA528.c | 800BA528-800C1DA4 | 52 | `wm_applyEncounterEvents`, `wm_getEncounterEntry`; end medium (`w_readtxRequest`, call cut) |
| w_readtx.c | 800C1DA4-800C2230 | 7 | `w_readtx*`; end medium |
| w_frmbuf.c | 800C2230-800C3EAC | 13 | `w_frmbuf*`; end: parity flip 169→237 and fewest crossing calls at `w_musicUpdate` — medium |
| w_music.c | 800C3EAC-800C4A5C | 17 | parity 4 (`w_musicGetID`, `w_musicRequestSend`); end medium (data cut 0x800C497C-0x800C4A5C) |
| tu_800C4A5C.c | 800C4A5C-800C709C | 8 | parity 4; end strong (asm) |
| wm_asm_800C709C.c | 800C709C-800C7914 | 1 | strong: hand-written asm |
| tu_800C7914.c | 800C7914-800CB1EC | 12 | strong edges |
| wm_asm_800CB1EC.c | 800CB1EC-800CB434 | 1 | strong: hand-written asm |
| tu_800CB434.c | 800CB434-800CCA10 | 24 | strong start; rodata 800A8678-800A8860 |

In each of the three overlays the first word of `.rodata` (the overlay id) is an object of its own
(`[0x0, rodata]` in the yaml).

## What was not split, and why

- **`.data` stays one asm file per binary.** No C defines data yet, so the data split would only
  move `.s` files; the data ranges above are evidence, not subsegments.
- **btl_mesh.c (ovl_0b 0x800C58E4-0x800C8BE0)** mixes two hand-written asm functions with C.
  CLAUDE.md notes 0x800C7730-0x800C8BE0 may use other compiler flags. It probably holds 3-4
  objects, but with no rodata or flag evidence for the cuts it stays one file.
- **Compiled C inside asm runs.** `func_80051274` and `func_80053384` (main, `jr ra; nop`) and
  `func_800DE11C` (ovl_0b, 0x1040 bytes) sit between hand-written asm functions. They may be
  objects of their own; they stay in the asm files.
- **Big single files with weak internal candidates** (ui.c, akao.c, event.c, msg.c, btlui.c,
  btl_cmd.c, tu_800B61A4.c): the data interleaves, or the only candidates are call-graph cuts,
  so no boundary was placed.
- **`static`.** Functions called only inside their file could be made `static`. A test
  (`func_80021938` in preload.c) still matches. It wasn't applied in general because it changes
  symbol binding, which matters for the port's link data and for overlays that reach main
  functions by address.

## Build and tool changes

- `config/slus_012.51.yaml`: `subalign: 4`, `ld_align_section_vram_end: false`,
  `ld_align_segment_vram_end: false` (as the overlays have), 81 C subsegments + 24 `.rodata`.
- `Makefile`: `TU_SPLIT := src/main src/ovl_0a src/ovl_0b src/ovl_0c` skips the `.align 3 → 2`
  rewrite for those directories. The 16 rodata padding `__asm__` statements are gone. `ccprobe`
  and `permute` still rewrite the alignment; it doesn't matter for single functions.
- `tools/resplit.py`: (1) INCLUDE lines that move to another file get that file's asm path;
  (2) a rodata symbol that a new subsegment boundary splits off an old INCLUDE_RODATA keeps
  its slot (it used to be dropped as "now emitted by C"); (3) an INCLUDE_RODATA line only
  travels with the item below it when both end up in the same file. New files get splat's
  `#include "common.h"` preamble; replace it with `ff9.h` / `ovl_XX.h`.
- `tools/integrate.py`: the default target is now `src/main` (a directory). Each function goes
  to whichever C file of the binary has its `INCLUDE_ASM`. `--into` takes a directory or a file.
- Unchanged, already glob `src/<bin>/*.c`: `progress.py`, `rename.py`, `void_returns.py`,
  `arg_check.py`, `fxmod_void_returns.py` (all still report 0 / the same totals).

## For the ff9-ds port

The port globs `src/<bin>/*.c` (`src_of` in its Makefile), so it picks up the new files. These
need updating there:

- `config/patches.txt` names files that no longer exist (`src/main/main.c`,
  `src/ovl_0a/ovl_0a.c`, `src/ovl_0b/ovl_0b{,_2,_3,_o1}.c`, `src/ovl_0c/ovl_0c.c`). Where each
  pattern is now (line numbers of patches.txt as of 2026-10-24; a pattern found in several files lists
  each with its count):

| line | old file | now in |
|---|---|---|
| 8 | main.c | main.c (×1) |
| 9 | main.c | main.c (×1) |
| 10 | main.c | tu_8004BBF0.c (×1) |
| 12 | ovl_0a.c | fld_main.c (×1) |
| 14 | main.c | cd.c (×1) |
| 15 | main.c | preload.c (×2) |
| 17 | ovl_0b.c | btl_update.c (×1) |
| 20 | ovl_0b_2.c | btlfx_pool.c (×1) |
| 21 | ovl_0b_2.c | btlfx_pool.c (×1) |
| 22 | ovl_0b_2.c | btlfx_script.c (×1) |
| 23 | ovl_0b_2.c | btlfx_script.c (×1) |
| 24 | ovl_0b_2.c | btlfx_script.c (×1) |
| 27 | ovl_0b_2.c | btlfx_heap.c (×1) |
| 28 | ovl_0b_2.c | btlfx_heap.c (×1) |
| 31 | ovl_0b_2.c | btlfx_heap.c (×6) |
| 32 | ovl_0b_2.c | btlfx_heap.c (×1) |
| 33 | ovl_0b_2.c | btlfx_heap.c (×1) |
| 35 | ovl_0b_2.c | btlfx.c (×1) |
| 37 | ovl_0b_2.c | btlfx_heap.c (×1) |
| 38 | ovl_0b_2.c | btlfx_heap.c (×1) |
| 40 | ovl_0b_2.c | btlfx_pool.c (×1) |
| 41 | ovl_0b_2.c | btlfx_pool.c (×1) |
| 42 | ovl_0b_2.c | btlfx_heap.c (×1) |
| 45 | ovl_0b_2.c | btlfx_op.c (×1) |
| 48 | ovl_0b_2.c | hi_vertex.c (×1) |
| 49 | ovl_0b_2.c | hi_vertex.c (×1) |
| 52 | ovl_0b_2.c | hi_model.c (×1) |
| 53 | ovl_0b_2.c | hi_model.c (×1) |
| 54 | ovl_0b_2.c | hi_model.c (×1) |
| 55 | ovl_0b_2.c | hi_model.c (×1) |
| 60 | ovl_0b_2.c | btlfx_op.c (×1) |
| 70 | main.c | tu_80036650.c (×2) |
| 74 | ovl_0a.c | tu_800B61A4.c (×1) |
| 75 | ovl_0b_3.c | btl_model.c (×1) |
| 83 | ovl_0b_2.c | btlfx_heap.c (×16) |
| 84 | ovl_0b_2.c | btlfx_heap.c (×5) |
| 87 | ovl_0b_o1.c | btl_cmd.c (×1), btlcam_o1.c (×1), btlfx_op.c (×2) — check counts |
| 88 | ovl_0b_2.c | btl_cmd.c (×1), btlcam_o1.c (×1), btlfx_op.c (×2) — check counts |
| 89 | ovl_0b.c | btl_cmd.c (×1), btlcam_o1.c (×1), btlfx_op.c (×2) — check counts |
| 90 | ovl_0b.c | btl_init.c (×2), btl_popup.c (×1), btl_unit.c (×2) — check counts |
| 91 | ovl_0b_3.c | btl_init.c (×2), btl_popup.c (×1), btl_unit.c (×2) — check counts |
| 94 | ovl_0b_2.c | hi_model.c (×4) |
| 96 | main.c | akao_voice.c (×1) |
| 97 | main.c | akao_voice.c (×1) |
| 98 | main.c | akao_voice.c (×1) |
| 102 | ovl_0a.c | fld_movie.c (×3) |
| 111 | ovl_0a.c | fld_movie.c (×1) |
| 112 | ovl_0a.c | fld_movie.c (×3) |
| 115 | main.c | event.c (×7) |
| 118 | main.c | tu_80021640.c (×1) |
| 119 | main.c | tu_80021640.c (×1) |
| 127 | ovl_0a.c | tu_800B0124.c (×1) |
| 128 | ovl_0a.c | tu_800B0124.c (×1) |
| 136 | main.c | main.c (×1) |
| 140 | ovl_0b_2.c | btlui.c (×1) |
| 146 | ovl_0b.c | btl_attr.c (×11), btl_init.c (×3) — check counts |
| 147 | ovl_0b.c | btl_status.c (×2), btlfx_op.c (×7) — check counts |
| 148 | ovl_0b.c | btl_status.c (×1) |
| 149 | ovl_0b_2.c | btl_status.c (×2), btlfx_op.c (×7) — check counts |
| 150 | ovl_0b_2.c | hi_model.c (×4) |
| 151 | ovl_0b_2.c | hi_model.c (×1) |
| 152 | ovl_0b_3.c | btlseq.c (×6) |
| 153 | ovl_0b_3.c | btlseq.c (×1) |
| 157 | ovl_0b.c | btl_main.c (×1) |
| 161 | ovl_0b.c | btl_status.c (×1) |
| 162 | ovl_0b.c | btl_unit.c (×1) |
| 163 | ovl_0b.c | btl_unit.c (×1) |
| 164 | ovl_0b.c | btl_unit.c (×1) |
| 168 | ovl_0b.c | btl_cmd.c (×2) |
| 169 | ovl_0b.c | btl_cmd.c (×1) |
| 173 | ovl_0b_2.c | btlui_frame.c (×1) |
| 175 | ovl_0b_2.c | btlfx.c (×1) |
| 182 | main.c | akao.c (×3) |
| 187 | main.c | event.c (×1) |
| 191 | ovl_0c.c | w_frame.c (×1) |
| 192 | ovl_0c.c | w_frame.c (×1) |

  Lines 87-91 and 146-149 match in several files both before and after the split, so their
  per-file counts need checking.
- The port Makefile's `akao_pitch` test fixture depends on `build/portified/src/main/main.c`;
  `akao_calcPitch` and its callers are now in `src/main/akao.c`.
- `tools/test_battle_pan.py` reads `build/portified/src/ovl_0b/ovl_0b_2.c`; those functions
  are now spread over `btlfx*.c`, `hi_*.c` and `btlui*.c`.
- `portify.py` strips MIPS padding statements. There are none left in the four split binaries.
- Removed `INCLUDE_RODATA(.., D_800A7000)` in ovl_0a/0b/0c (overlay header word, now an asm
  rodata object).
