# Effect-module choreography literals, slice 0 (fx_001 .. fx_05f)

Follows the scheme of docs/fxdata.md, "Choreography literals (pilot)": per module a `/* fx_XXX choreography */`
block after the effect-data names in `include/fxmod/fx_XXX.h`, with the module's game-name prefix (the one its
effect-data names already use) and a comment saying what the code does with each value. All 70 modules of the
slice (the pilot modules excluded) are done; byte-identical (`make verify`: 20 OK + 380 effect modules OK),
`decl_check` 0.

## Counts (actionable literals, `tools/fxlits.py`)

| module | effect | before | after | choreography defines |
|---|---|---|---|---|
| fx_001 | Dagger's magic (FX_DAGGERS_MAGIC) | 34 | 6 | 21 |
| fx_00a | Venom Powder | 3 | 1 | 1 |
| fx_00b | Esuna | 3 | 1 | 1 |
| fx_00c | Panacea | 3 | 1 | 1 |
| fx_00d | Stona | 7 | 2 | 4 |
| fx_00e | Berserk | 12 | 8 | 5 |
| fx_00f | Full-Life | 9 | 4 | 4 |
| fx_011 | Life | 8 | 3 | 4 |
| fx_015 | Poison | 7 | 2 | 5 |
| fx_016 | Water | 55 | 18 | 18 |
| fx_017 | Devil's Candle | 5 | 2 | 2 |
| fx_019 | Mini | 2 | 0 | 1 |
| fx_01a | Sleep | 10 | 0 | 7 |
| fx_01b | Float | 3 | 1 | 1 |
| fx_01c | Aqua Breath | 13 | 6 | 5 |
| fx_01d | Aerial Slash | 33 | 19 | 8 |
| fx_01e | 1000 Needles | 7 | 2 | 4 |
| fx_01f | FX_01F (unlabelled) | 8 | 5 | 3 |
| fx_020 | Regen | 6 | 0 | 4 |
| fx_021 | Open Book | 4 | 2 | 1 |
| fx_022 | Tentacle Gone | 4 | 2 | 1 |
| fx_023 | Psychokinesis | 17 | 2 | 8 |
| fx_024 | Aerial Slash | 33 | 19 | 8 |
| fx_026 | Shiva | 194 | 75 | 91 |
| fx_028 | Elixir | 10 | 6 | 3 |
| fx_029 | White Wind | 3 | 1 | 2 |
| fx_02a | Silence | 7 | 3 | 2 |
| fx_02b | Aerial Slash | 33 | 19 | 8 |
| fx_02c | Bad Breath | 5 | 2 | 3 |
| fx_02d | Angel's Snack | 2 | 0 | 2 |
| fx_02e | Magic Break | 11 | 7 | 2 |
| fx_02f | Flame | 14 | 9 | 4 |
| fx_030 | Pollen | 4 | 1 | 3 |
| fx_031 | Gradual Petrify | 10 | 6 | 3 |
| fx_032 | Glowing Eyes | 14 | 9 | 3 |
| fx_033 | Snort | 11 | 4 | 9 |
| fx_034 | Cold Breath | 51 | 25 | 22 |
| fx_035 | Devil's Candle | 6 | 3 | 2 |
| fx_036 | Blowup | 10 | 5 | 6 |
| fx_037 | Darkness | 11 | 2 | 8 |
| fx_038 | Comet | 49 | 27 | 19 |
| fx_039 | Osmose | 16 | 6 | 9 |
| fx_03a | Earthquake | 31 | 14 | 16 |
| fx_03b | Trouble Juice | 10 | 2 | 8 |
| fx_03c | Wind | 6 | 2 | 3 |
| fx_03d | Luna | 46 | 20 | 12 |
| fx_03f | Shell | 10 | 5 | 5 |
| fx_040 | Bubbles | 5 | 1 | 4 |
| fx_041 | Whirl Slash | 20 | 10 | 9 |
| fx_043 | Ink | 13 | 4 | 8 |
| fx_044 | Ultra Sound Wave | 7 | 1 | 5 |
| fx_045 | Aerial Slash | 31 | 19 | 9 |
| fx_04a | Goblin Punch | 3 | 1 | 1 |
| fx_04b | Sandstorm | 15 | 3 | 9 |
| fx_04c | Float (all targets) | 8 | 2 | 6 |
| fx_04e | Sleep (all targets) | 10 | 0 | 7 |
| fx_04f | Reflect | 34 | 21 | 14 |
| fx_051 | Mighty Guard | 9 | 4 | 3 |
| fx_052 | Limit Glove | 38 | 10 | 17 |
| fx_053 | White Draw | 18 | 8 | 7 |
| fx_055 | Chakra | 7 | 2 | 4 |
| fx_056 | Chakra (all targets) | 10 | 4 | 5 |
| fx_057 | Holy | 73 | 44 | 39 |
| fx_058 | Matra Magic | 30 | 17 | 10 |
| fx_059 | Haste | 20 | 7 | 9 |
| fx_05a | Might | 20 | 1 | 14 |
| fx_05c | Armor Break | 9 | 6 | 2 |
| fx_05d | Bio | 32 | 23 | 8 |
| fx_05e | Death | 59 | 31 | 22 |
| fx_05f | Water (all targets) | 58 | 21 | 19 |
| total |  | 1379 | 599 | 593 |

1,379 → 599 (780 named, 57%; 593 defines over the 70 headers, twins counted separately). The pilot's rate was
42%. This slice has many small one-emitter modules, where `*args = sizeof(...)`, the keyframes and the length
are most of the literals.

## What was named

- `*args` / `init->workSize` / `elemSize` → `sizeof(FxXXXWork)` / `sizeof(FxXXXItem)` everywhere. No struct
  needed a pad: every work struct already had the size the op passes. Works that are a bare position use
  `sizeof(SVECTOR)`, fx_036 op81's fade level `sizeof(s16)`, fx_04f's pool items `sizeof(s32)`. fx_026's
  `Hi_GetSplitMdlVertex() / 24` became `/ sizeof(Fx026Tri)`, and fx_059's private pool
  `btlfx_poolSize(sizeof(SVECTOR), FX_HASTE_TRAIL_POINTS)` (elemSize, count).
- Task lengths are `FX_<SPELL>_FRAMES`, with a per-op name where a module has several (`_DUST_FRAMES`,
  `_RINGS_FRAMES`, ...). Keyframes are `_FRAME`, including the `case N:` labels of the frame switches. State
  enums and their lengths cover Shiva op80/op81/pool items, Comet rocks, Reflect, Holy, Death, Haste, Matra Magic
  missiles and Float (all targets).
- Every emitter that gets a trail buffer has `_TRAIL_SLOTS` / `_TRAIL_STRIDE`. Where the header sizes the buffer
  per item, the size is now the product: `fx_039_trails[][FX_OSMOSE_DRAIN_ORB_TRAIL_SLOTS *
  FX_OSMOSE_DRAIN_ORB_TRAIL_STRIDE / 4]` (was 0x318), `fx_056_trails[][... / 4]` (was 0x168).
- Header array sizes the code loops over are named: Shiva's strips (`pts4[FX_SHIVA_STREAK_POINTS]`) and pieces
  (`pieceModels18[FX_SHIVA_PIECE_COUNT][FX_SHIVA_PIECE_STAGES]`), Cold Breath's primitive kinds, Comet's spawn
  table, Earthquake's chunks, Bubbles' streams, Float's target slots, Bio's morph models.
- Hi_RegisterTexEffModel texture args are new since the pilot (`tools/fxdata.py` now resolves them). They are
  `_TPAGE`, `_CLUT`, `_U`, `_V`, with the page / clut decoded in the comment: Dagger's magic, Berserk, Water x2,
  Cold Breath, Comet, Reflect, Holy, Might, Bio. All 85 model renders of the slice (`fxdata.py --render`) are
  byte-identical before and after (md5 of every `*model*.png`).
- VRAM rects are checked against the model's tpage + u/v: Dagger's magic (MoveImage into the light texture),
  Water (scroll into the globe page), Cold Breath (gleam into ICE_CRYSTAL_2's v 0x80 half), Shiva (vortex: tpage
  0x98 v 128; ICICLE_2: tpage 0x9A u 0). Might's star samples the AURA_TEXTURE render target (tpage 0x117 v 0xC0 =
  (0x1C0, 0x1C0)).
- Screen tints are `_RGB` (additive / subtractive), with named flash levels and fade lengths. abr arguments are
  `BTLFX_ABR_ADD` / `_SUB` / `_OPAQUE` (screen tints, Hi_ModifyEffModelAbr, fxm_tintScreenLerp). fx_05e's ribbon
  helper code byte is now `abr != BTLFX_ABR_OPAQUE ? GPU_POLY_G4 | GPU_ABE : GPU_POLY_G4`, with
  `PRIM_TAG(POLY_G4)`.
- Heights / reaches of named emitter positions are `_HEIGHT`, `_REACH`, `_DEPTH`. Also named: the Aerial Slash
  family's gust point and blade reach, Psychokinesis' lift height, Snort's push distance, Osmose's arc.
- Twins share names with their own values: Aerial Slash (fx_01d / fx_024 / fx_02b / fx_045), Water (fx_016 /
  fx_05f), Sleep (fx_01a / fx_04e), Devil's Candle (fx_017 / fx_035), Chakra (fx_055 / fx_056). fx_045 splits
  `_GUST_FRAME` 10 from `_FLASH_FRAME` 11 because its flash comes one frame later.

## What stays numeric

As in the pilot, these keep their numbers:

- otZ biases (`btlfx_addRadiusSixth(0, 0x20)`, `otZ = 0x40`)
- random spreads
- fixed-point / angle math (`0x1000` ramps, `<< 10` four-frame ramps whose compare got the name,
  `rot.vy += 0x800`)
- one-off scales (`scale = 0x800`) and the neutral `- 0x80` colour offsets
- btlfx_drawScreenGrid's screen-centre arguments (0xA0, 0x78), most of what is left in the Aerial Slash modules
- bone indices and the sound ids of btlfx_playFileSound
- the camera offsets of Death
- Bio's morph-stage chain (an if/else table of frame ranges)
- fx_03a's last-bounce countdown and Shiva's `frame + (piece - 5)` stagger

## Open questions (look pending)

| module | name | question |
|---|---|---|
| fx_026 | FX_SHIVA_BARRIER_VRAM_* | op81's second scroll copies FX_SHIVA_ICICLE's texture into the v 128 rect below it. fxdata does not analyse the barrier piece models (fx_026_pieceModelBlocks), so "the pieces sample it" is by elimination |
| fx_03a | FX_EARTHQUAKE_BG_VRAM_* | btlfx_drawBgOffscreen's target rect (0x1C0, 0x100) 0x100 x 0x100: which draw samples it (the screen grid?) |
| fx_038 | FX_COMET_SHOCK_RING_U | u = -0x40 on a 15-bit page: which texel column the ring really reads |
| fx_04f | FX_REFLECT_STAR_V / TPAGE | the star (v 0x80) and panels (v 0) against the two render targets (0x1C0, 0x1C0) / (0x1C0, 0x120): inferred from the pages, not from a capture |
