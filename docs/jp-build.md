# The Japanese build as a second evidence source

The Japanese release (*Final Fantasy IX (Japan, Asia)*, 4 discs, boot exe `SLPS_020.00` .. `SLPS_020.03`) was
compiled from the same sources a few months before the US build. Comparing the two shows code the US build compiled
out and data it stopped using. Names taken from it are tagged `[jp]` (docs/naming.md).

## Files

Nothing from the JP discs is committed. Layout in the main checkout's gitignored `disks/jp/`:

| path | what |
|---|---|
| `raw/Final Fantasy IX (Japan, Asia) (Disc N).bin/.cue` | raw 2352-byte images (unzipped from the user's dumps) |
| `SLPS_020.00` .. `SLPS_020.03` | boot exe (`SYSTEM.CNF`: `BOOT=cdrom:\SLPS_020.00;1` on disc 1); the four are byte-identical (sha1 `8ba95c45…a906`) |
| `discN/FF9.IMG` | `tools/extract_disc.py IMAGE disks/jp/discN FF9.IMG` |
| `ovl/XX.bin` | `tools/ff9img.py disks/jp/disc1/FF9.IMG extract-dir 0 disks/jp/ovl`; dir 0 is identical on all four discs |
| `fxmod/` | `tools/effect_modules.py --disks disks/jp extract disks/jp/fxmod` |

The JP exe has the same size as the US one (0x1DE000 bytes of text+data, vram 0x80010000) but a different entry
point (0x80012C40) and link order (below). FF9.IMG has the same 15 directories; dir 0 holds the same 19 code
overlays at the same load addresses. JP overlays 0b, 14, 32, 33, 37, 3c and 64 are smaller than their US versions,
the rest have the same sector-rounded size.

**All 380 battle effect modules are byte-identical** to the US ones, so nothing below concerns them.

## Method (function mapping)

Scratch scripts, run in Docker, under `build/jp/` of the worktree that did this work (not committed):

1. For every US function in `config/functions.us.txt`, mask its words: `j`/`jal` targets, and the 16-bit immediate
   of `lui`, `addiu`/`ori`/`slti`/..., and every load/store (addresses and struct offsets). Branches and R-type
   instructions stay. Find the masked byte string in the masked JP binary.
2. Functions of 16+ bytes with a single hit are candidate anchors; the longest increasing subsequence of
   (US order, JP offset) keeps the ones in the same order. Functions with several hits take the one between their
   neighbouring anchors.
3. Between consecutive anchors, `difflib.SequenceMatcher` aligns the masked US and JP spans and places each
   remaining function, with a score = fraction of its words aligned.
4. Functions still unplaced (code that moved) are found by voting 5-word masked k-grams over the whole JP binary.
5. Pairing the `jal` targets and `lui`-based addresses of aligned instructions gives a JP→US address map
   (6550 addresses), used to annotate JP disassembly (rabbitizer) with US names and to find globals' JP addresses.

## Match rates

`same` = raw bytes identical; `exact` = identical with immediates/targets masked; `moved` = exact but out of
order; `close` = aligned with score ≥ 0.9; `fuzzy` = 0.3-0.9 (changed function); `missing` = no counterpart.

| binary | functions | same | exact | moved | close | fuzzy | missing | exact+moved+same |
|---|---|---|---|---|---|---|---|---|
| main | 1579 | 182 | 1006 | 124 | 248 | 19 | 0 | 83.1% |
| ovl_0a | 357 | 19 | 291 | 8 | 37 | 2 | 0 | 89.1% |
| ovl_0b | 1068 | 115 | 866 | 0 | 72 | 15 | 0 | 91.9% |
| ovl_0c | 299 | 24 | 238 | 0 | 34 | 3 | 0 | 87.6% |
| ovl_0d | 132 | 17 | 101 | 0 | 9 | 5 | 0 | 89.4% |
| ovl_14 | 87 | 6 | 66 | 6 | 5 | 4 | 0 | 89.7% |
| ovl_15 | 96 | 10 | 76 | 0 | 10 | 0 | 0 | 89.6% |
| ovl_32 | 29 | 2 | 20 | 0 | 4 | 3 | 0 | 75.9% |
| ovl_33 | 73 | 1 | 61 | 0 | 5 | 6 | 0 | 84.9% |
| ovl_34 | 53 | 0 | 34 | 0 | 12 | 7 | 0 | 64.2% |
| ovl_36 | 43 | 1 | 36 | 0 | 4 | 2 | 0 | 86.0% |
| ovl_37 | 47 | 1 | 35 | 0 | 5 | 6 | 0 | 76.6% |
| ovl_38 | 24 | 0 | 19 | 0 | 1 | 4 | 0 | 79.2% |
| ovl_39 | 187 | 4 | 156 | 0 | 19 | 7 | 1 | 85.6% |
| ovl_3a | 20 | 0 | 12 | 1 | 3 | 4 | 0 | 65.0% |
| ovl_3b | 20 | 0 | 17 | 0 | 3 | 0 | 0 | 85.0% |
| ovl_3c | 90 | 1 | 67 | 0 | 9 | 13 | 0 | 75.6% |
| ovl_3e | 21 | 0 | 16 | 0 | 5 | 0 | 0 | 76.2% |
| ovl_64 | 71 | 2 | 60 | 0 | 5 | 4 | 0 | 87.3% |
| ovl_65 | 91 | 4 | 73 | 0 | 7 | 7 | 0 | 84.6% |
| **all** | **4387** | **389** | **3250** | **139** | **497** | **111** | **1** | **86.1%** |

Every US function has a counterpart except ovl_39 `menuSave_getDiscBit` (the JP save header code around it is
0x12C bytes larger). A few low-scoring `fuzzy` placements are not real counterparts: ovl_14 `opening_bootScreen` and
`opening_bootScreenLoad` (the JP opening has no reference to their TIM 0x44FB0 at all: the boot notice screen is
US-only) and the placeholder slot of `btl_loadKanjiFont` (its real JP body is at 0x800A8FB0, below).

Where the two builds differ, the differences are almost all about text: JP draws its UI labels from halfwidth-katakana
/ ASCII strings in each overlay's rodata where US uses the FF9-encoded text banks, JP text is two-byte (kanji), and US
names and strings are longer. The menu overlays (ovl_32-3c, 65) have the lowest exact rates for that reason.

### Link order of the main exe

The JP exe starts with the `db_*` / `game_initState` .. `game_freezeScreen` / `game_initData` .. `file_find` /
`CD_*` and libcd / `cdRead_*` / `preload_*` code (US 0x8001C6F8-0x80022680) at 0x80012C40, i.e. that block of
object files was linked first in JP and after the PsyQ libgpu/libetc/libspu objects in US. 124 main functions are
`moved` for that reason; the code itself is identical.

## Stubs

Checked: every US function under 0x10 bytes, all `*Nop*` / `*_on<Phase>` names, and every name in the
"weak-name redo" and "R38 game" sections of `docs/wip/names_from_port.txt` (the BtlfxExports nop slots, the fldbg/bgi
`nopOk` functions, `btlui_perfMark`, `btlui_termHeap`, `btl_termBanner`, `help_onDraw`, `msg_onTextEnd`,
`game_onReset`, `fade_nop1/2`, `pen_nop`, the menu `on*` hooks, ...). For each, the span between its nearest exactly
matched neighbours was compared in both builds.

**All of them are empty (or the same constant return) in JP too, except two:**

### ovl_0b `btl_onInitMode` (US 0x800A8F10) → `btl_loadKanjiFont`, JP 0x800A8FB0

```
addiu a0,zero,4 ; addiu a1,zero,0x16 ; jal db_lookupRegistered ; a2 = 0
addiu a0,zero,4 ; addiu a1,zero,0x15 ; a2 = 0
lw v1,g_gameState ; lw v1,0x1C(v1)              ; ->gameData
jal db_lookupRegistered ; sw v0,0x84C(v1)       ; gameData->kanjiFont = item 4/0x16
jal file_loadTim ; a0 = item 4/0x15             ; its glyph texture
jal DrawSync(0) ; return 0
```

Same slot in `btl_initMode` (after `btl_loadCommonData`, before `btl_loadResidentMusic`). The field and the world map
do the same per area in their area loaders, inline (no stub left in US): JP `fld_findAreaItem` (0x800A8718, +0x6C
bytes) sets `kanjiFont` from the area bundle's dependency item `(9, 0x60000|area, 0xD0000)` and loads TIM
`(9, ..., 0x40000)`; JP `wm_findAreaData` (+0x6C) the same for world areas. `game_loadCommonImages` loads six more
common TIMs in JP (+0x84 bytes). In the US build `GameData.kanjiFont` (0x84C) is only ever cleared (`game_resetData`,
`fld_init`, `btl_initMode`, `wm_init`, `opening_init`), and the JP stores above are the only other writers.

### ovl_37 `menuItem_onExit` (US 0x801F2F30) → `menuItem_restoreVram`, JP 0x801F2EE8

JP `menuItem_loadImages` (0x801F2E78) loads the key-item CLUT, then `StoreImage((0,256,44,128), 0x801FB2C0)` saves
that VRAM block before `LoadImage`-ing the key-item image into it. The JP function in the `menuItem_onExit` slot
(called from the same two places: `menuItem_exit` and the `menuItem_stepKweh` exit path) is

```
LoadImage(&(RECT){0,256,44,128}, 0x801FB2C0); DrawSync(0);
```

US loads the image to (896,384), which nothing else uses, so the save/restore was dropped and only the empty call
remained.

## Globals the US build writes but never reads

For each US data symbol, JP addresses come from the address map; accesses were counted in every JP binary
(`lui`-based loads/stores; pointer-relative accesses are invisible to this, as in US).

No US symbol that has zero loads in US has a load in JP (all symbols checked, not only the commented ones). The
commented write-only globals are write-only in JP too:

| US symbol | US | JP | JP accesses |
|---|---|---|---|
| `g_msgFontGlyphCount` | 0x8007B94C | 0x8007AE0C | store in `msg_init` only |
| `g_msgPadCopyDst` | 0x8007B998 | 0x8007AE58 | store in `msg_update` only |
| `g_msgDrawBufMode`, `g_msgDrawPktStart` | 0x8007B9A0/A4 | 0x8007AE60/64 | stores only |
| `g_akaoInitDone`, `g_akaoChannelsPtr` | 0x8007F8E0, 0x800809EC | 0x8007EDA0, 0x8007FEAC | `akao_init` stores only |
| `g_worldCameraInitFlag`, `g_worldMoveInitWord`, `g_worldLowBuffer`, `g_worldSysInitFlag`, `g_worldGetoffSearching` | ovl_0c 0x800CE1E1 ... | +0x2698 | stores only |
| `g_worldEffectPos`, `g_worldEffectRot` | ovl_0c 0x800D4960/49F0 | 0x800D6FF8/7088 | `wm_initEffects` only |
| `g_theEndInitWord`, `g_messagesFrameWord` | ovl_15 0x800AE430/A8 | 0x800B0C9C/0D14 | stores only |

`g_btlfxCommonInit` (ovl_0b) and `g_fieldBgClutRows` (ovl_0a) were not located (no mapped neighbour).

One US global is *read* but never *set* in US and is set in JP: **`D_8007BBD0` → `g_penKanjiGlyphs`**. JP
`msg_init` (0x8004C1F0, +0x5C bytes):

```
if (mode == FIELD || mode == WORLD || mode == BATTLE_END) {
    Font *k = gameData->kanjiFont;
    g_penKanjiGlyphs = (u32 *)k + 3;        /* k + 0xC, like g_penGlyphs = font + 0xC */
    g_penGlyphDecode.kanjiCode = *(u32 *)k;
} else
    g_penGlyphDecode.kanjiCode = -1;        /* the only case US kept */
```

`pen_getGlyph` reads `g_penKanjiGlyphs[code - kanjiCode]` for codes at or above `kanjiCode`, which US never reaches.

## Strings

Strings (ASCII or Shift-JIS, word-aligned, outside code) present in JP and absent in US, per binary: main 3
(the `\SLPS_020.01;1` .. `.03` disc names), ovl_0a 2, ovl_0b 16, ovl_0c 3, ovl_0d 3, ovl_15 0, ovl_32 7, ovl_33 26,
ovl_34 36, ovl_36 10, ovl_37 1, ovl_38 16, ovl_39 11, ovl_3a 1, ovl_3b 3, ovl_3c 33, ovl_3e 0, ovl_64 0, ovl_65 8.

All of them are UI labels: window titles in ASCII (`"COMMAND"`, `"ABILITY"`, `"CHOCOGRAPH"`, `"CARD INFO"`,
`"GET ABILITY"`, ...) and halfwidth-katakana labels with `@` as a spacer (`"@ﾐﾂｹﾀ@ﾎﾟｲﾝﾄ   :"`, the Config option
names), referenced by the same draw functions that read text-bank strings in US (`menuChocograph_drawCounts`,
`menuCard_drawInfo`, `rewards_drawAp`, ...). The JP save header strings are `"BISLPS-02000"` and a full-width
`ＦＦ９／ファイル００／…` title (`menuSave_getFilePrefix`, `menuSave_buildCardHeader`).

**There are no debug or error strings with function names in JP that US lacks.** The reverse exists: US
`menuScreen_init` prints `"path=%s\n"` with `"f:/code/dms/loc_us/ff9db.h"` (the US localisation's db header path),
which JP doesn't have.

## Other differences

- `file_initImg` is 0x94 bytes larger in JP and reads `g_gameState` four more times; US calls `cd_getState` twice
  where JP doesn't (disc detection; the JP exe's data has `\SLPS_020.00;1` .. `.03`).
- ovl_15 `ending_movieSwap` (+0x6C): while the ending movie plays, JP sets `GameState.vsyncMode` to 4, and to 3 on
  every 50th movie frame, 0 when it stops (frame pacing). US has no such code.
- ovl_0b `btl_updateScan` (+0x2C) composes the scan message from `btl_getElementName` / `text_copy` /
  `text_getFrom` / `text_append`, `btl_updateDetect` is shorter in JP (no item suffix); `btlui_drawThrowRow` draws
  an item icon (`ui_drawItemIcon`) in JP.
- ovl_14: the US-only boot notice screen (`opening_bootScreen`, TIM 0x44FB0); the JP opening movie functions
  (`opening_movieInit` .. `opening_movieOnEnd`) sit after the attract-mode code instead of before it.
- ovl_0d (Tetra Master): JP has fewer `text_get` / `text_measureWidth` calls (fixed-width JP strings).

## Names applied

| US symbol | binary | new name | evidence |
|---|---|---|---|
| `btl_onInitMode` | ovl_0b | `btl_loadKanjiFont` | [jp] JP body loads the battle kanji font and its TIM |
| `menuItem_onExit` | ovl_37 | `menuItem_restoreVram` | [jp] JP body restores the VRAM block saved by `menuItem_loadImages` |
| `D_8007BBD0` | main | `g_penKanjiGlyphs` | [jp] JP `msg_init` points it at `kanjiFont + 0xC` |

No other placeholder or stub name gained evidence: every other stub is empty in JP as well, and the only
`func_`/`D_` placeholders left in main are library code (`exact` in JP).
