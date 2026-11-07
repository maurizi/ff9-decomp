# Effect-module choreography literals: slice 1 (fx_060 .. fx_0b0)

Follows the scheme in [fxdata.md](fxdata.md), "Choreography literals (pilot)": per module a `/* fx_XXX
choreography */` block after the effect-data names in `include/fxmod/fx_XXX.h`, with the module's game-name prefix
and a comment saying what the code does with each value. Counts are `tools/fxlits.py fx_XXX` (actionable
literals: compare / mask / case / arg / assign); "defines" is the number of `#define`s in the module's
choreography block. Every step was checked with `make verify` (20 binaries + 380 effect modules byte-identical) and
`tools/decl_check.py` (0).

## Counts

| module | prefix | before | after | defines |
|---|---|---|---|---|
| fx_060 | `FX_CONFUSE_` | 10 | 3 | 6 |
| fx_061 | `FX_CONFUSE_` | 10 | 4 | 6 |
| fx_062 | `FX_DEMI_` | 74 | 27 | 40 |
| fx_063 | `FX_DEMI_` | 79 | 30 | 42 |
| fx_078 | `FX_BIO_` | 13 | 5 | 8 |
| fx_079 | `FX_DEMI_SHOCK_` | 74 | 19 | 36 |
| fx_07a | `_` | 133 | 133 | 0 |
| fx_07b | `FX_PROTECT_` | 7 | 1 | 5 |
| fx_07c | `FX_MINI_FRAMES_` | 2 | 0 | 1 |
| fx_07d | `_` | 155 | 155 | 0 |
| fx_07e | `FX_SLOW_` | 63 | 39 | 27 |
| fx_07f | `FX_SILENCE_` | 13 | 3 | 12 |
| fx_080 | `FX_DISPEL_` | 7 | 4 | 3 |
| fx_081 | `FX_LV4_HOLY_` | 67 | 47 | 32 |
| fx_082 | `FX_MAGIC_HAMMER_` | 27 | 13 | 14 |
| fx_083 | `FX_SCAN_` | 73 | 20 | 47 |
| fx_084 | `FX_DRAIN_` | 14 | 9 | 7 |
| fx_086 | `_` | 225 | 225 | 0 |
| fx_087 | `FX_SPARE_CHANGE_` | 9 | 4 | 4 |
| fx_088 | `FX_SPARE_CHANGE_` | 14 | 7 | 5 |
| fx_089 | `FX_REVIVE_F_` | 5 | 2 | 2 |
| fx_08a | `FX_AUTO_LIFE_` | 9 | 4 | 5 |
| fx_08b | `FX_AERIAL_SLASH_` | 32 | 17 | 10 |
| fx_08c | `FX_BERSERK_` | 12 | 8 | 6 |
| fx_08d | `FX_CURA_` | 14 | 2 | 9 |
| fx_08e | `FX_CURAGA_` | 34 | 6 | 21 |
| fx_08f | `FX_METEOR_` | 96 | 72 | 25 |
| fx_090 | `FX_CURAGA_` | 35 | 8 | 20 |
| fx_091 | `FX_ENTICE_FRAMES_` | 4 | 2 | 1 |
| fx_092 | `FX_HYPNOTIZE_` | 9 | 5 | 5 |
| fx_093 | `FX_CANNON_` | 12 | 8 | 3 |
| fx_094 | `FX_DELTA_ATTACK_` | 10 | 4 | 5 |
| fx_095 | `FX_STOP_` | 54 | 26 | 30 |
| fx_096 | `FX_FROST_` | 41 | 18 | 13 |
| fx_099 | `FX_BREAK_` | 18 | 11 | 11 |
| fx_09a | `FX_LV5_DEATH_` | 67 | 41 | 23 |
| fx_09b | `FX_` | 23 | 11 | 11 |
| fx_09c | `FX_GNOLL_ATTACK_` | 26 | 21 | 5 |
| fx_09d | `FX_BLIND_` | 5 | 2 | 3 |
| fx_09e | `FX_BLIND_` | 5 | 2 | 2 |
| fx_09f | `FX_PYRO_` | 8 | 5 | 3 |
| fx_0a0 | `FX_WEB_` | 19 | 10 | 8 |
| fx_0a1 | `FX_BLASTER_` | 25 | 15 | 11 |
| fx_0a2 | `FX_CURA_` | 5 | 0 | 4 |
| fx_0a3 | `FX_ELECTROCUTE_` | 12 | 4 | 7 |
| fx_0a4 | `FX_BUZZ_` | 4 | 1 | 2 |
| fx_0a5 | `FX_SPORE_` | 11 | 4 | 6 |
| fx_0a6 | `FX_SWEEP_` | 5 | 0 | 5 |
| fx_0a7 | `FX_WATER_GUN_` | 20 | 7 | 11 |
| fx_0a8 | `FX_REIS_WIND_` | 13 | 6 | 5 |
| fx_0a9 | `FX_LIGHTNING_` | 33 | 19 | 8 |
| fx_0aa | `FX_SNOWSTORM_` | 17 | 4 | 9 |
| fx_0ab | `FX_MAELSTROM_` | 6 | 0 | 7 |
| fx_0ac | `FX_SLEEPING_JUICE_` | 9 | 3 | 7 |
| fx_0ad | `FX_AERO_BREATH_` | 8 | 3 | 5 |
| fx_0ae | `FX_VENOM_BREATH_` | 9 | 4 | 5 |
| fx_0af | `FX_ZOMBIE_BREATH_` | 11 | 4 | 5 |
| fx_0b0 | `FX_LEAF_SWIRL_` | 31 | 13 | 11 |
| total | | 1826 | 1120 | 624 |

55 of the slice's 58 modules are done: 1826 → 1120 actionable literals
(706 named), 624 defines. Not done (no choreography block yet; their abr
arguments are named): fx_07a, fx_07d, fx_086.

## What was named

- **Lengths, keyframes, states**: `frame >= N` / `return frame >= N` (`_FRAMES`, or `_LAST_FRAME` for a `> N`),
  `frame == N` / `case N:` that starts an emitter (`_FRAME`), the `(frame << 12) / N` ramp of the same phase (the
  define replaces both), the op's `switch (work->state0)` labels and assignments (`_STATE_<PHASE>`, named by what
  the case draws). Pool particles' own state machines get `_<PIECE>_STATE_*` (fx_095 Stop's clock marks, fx_07e
  Slow's crescents); a second op's states `_OP81_STATE_*`.
- **Phase timer slots**: the Demi family (fx_062, fx_063, fx_079 Demi Shock) and fx_083 Scan keep per-phase
  countdowns in `work->timers0[]`, set on keyframes. The slot indices are named (`FX_DEMI_T_GROW`, `FX_SCAN_T_PILLARS`,
  array size `FX_DEMI_TIMERS`), and so are the values written into them (`FX_DEMI_GROW_FRAMES`) and the `n` of the
  eased lerps that read them. fx_062 and fx_063 share slot names where the slot does the same thing (0-6); 7 and 9
  differ (Demi lifts one target / bobs all of them).
- **Work / pool sizes**: `*args = N` → `sizeof(FxXXXWork)`, `workSize` / `elemSize` → `sizeof`, `poolCount` as
  the count it is (fx_0a9 Lightning: `FX_LIGHTNING_BOLT_SLOTS * FX_LIGHTNING_BOLT_POINTS` = 0xB0). Two structs were
  shorter than the size op80 allocates and gained an `unused` pad word: `Fx062Work` (0xD0 → 0xD4) and `Fx079Work`
  (0xA4 → 0xA8).
- **Counts / array sizes**: loop bounds, `% N`, `(i << 12) / N` angle steps and the header arrays they index
  (`fx_09b_trails[FX_09B_SHOTS][FX_LAVA_GUN_SHOT_TRAIL_SLOTS * FX_LAVA_GUN_SHOT_TRAIL_STRIDE / 4]`: the 0x10A
  words per shot are the trail buffer; `pixels8[FX_STOP_DISC_VRAM_W * FX_STOP_DISC_VRAM_H / FX_STOP_GREY_BANDS]`).
- **Trail buffers**: `_TRAIL_SLOTS` / `_TRAIL_STRIDE` per emitter (one name where several emitters share the
  values, e.g. Leaf Swirl's `FX_LEAF_SWIRL_LEAF_TRAIL_STRIDE`).
- **Model textures**: `Hi_RegisterTexEffModel`'s tpage / CLUT / u / v as `FX_<SPELL>_<MODEL>_TPAGE` / `_CLUT` /
  `_U` / `_V` (u / v only when nonzero; 0xFFFF = no CLUT for 15-bit pages), with the decoded page in the comment.
  `tools/fxdata.py` resolves them (renders of fx_07f, fx_08e, fx_062 checked: still textured; fx_078's models are
  table-driven and fx_08c's funnel samples a run-time texture, both untextured before as well).
- **VRAM rects checked against the textures**: fx_08e / fx_090 Curaga's MoveImage target (0x2C0, 0x100) 0x20 x 0x80
  is the fountain models' 4-bit page; fx_083 Scan's glyph rect (0x2C0, 0x180) 0x40 x 0x40 is the sphere's page
  (0x280, 0x100) + u / v 0x80 in 8-bit texels, and its scroll rect (0x260, 0x100) is the band's page (0x200, 0x100)
  + u 0xC0; fx_08c Berserk's funnel page (0x200, 0x100) + v 0x80 is the rect FX_BERSERK_FLAME_TEXTURE renders.
- **Screen tints**: `_RGB` triples (`FX_STOP_DARKEN_RGB`, `FX_DEMI_SHOCK_FLASH_RGB`), single levels where the code
  keeps the value in a variable (`FX_AERIAL_SLASH_FLASH`).
- **GPU words**: `GPU_LINE_G2 | GPU_ABE` / `BTLFX_ABR_OPAQUE` in fx_0a0's line helper, `PRIM_TAG(POLY_GT4)` /
  `PRIM_TAG(POLY_FT3)` / `PRIM_TAG(POLY_G4)` / `PRIM_TAG(POLY_F3)` / `PRIM_TAG(LINE_G2)` for the addPrim / linkPrim
  length words (fx_095, fx_07e, fx_0a0).
- **abr arguments**, the whole slice (including the modules not done): `BTLFX_ABR_HALF` / `_ADD` / `_SUB` /
  `_OPAQUE` for the literal abr argument of `Hi_ModifyEffModelAbr`, `btlfx_drawScreenTint`, `btlfx_drawTrail`,
  `btlfx_drawParticles`, `btlfx_drawSprite(s)`, `btlfx_drawSpriteAtOt`, `btlfx_drawAnimQuads`, `btlfx_drawRings`,
  `btlfx_drawFan`, `btlfx_drawLayerSprite`, `btlfx_allocDrMode` (133 sites), plus module helpers' abr arguments and
  local `abr = N` assignments where seen. They do not change the counts (1 / 2 are noise to the classifier).
- **Sizes**: heights, reaches and offsets used for one named piece (`FX_HYPNOTIZE_HALO_HEIGHT`,
  `FX_WATER_GUN_TRAIL_REACH`, `FX_SPORE_SWEEP_X`), model full scales, fixed positions (`FX_BLIND_VORTEX_Y`).

## What stays numeric

As in the pilot: OT depth biases (`otZ`, `btlfx_addRadiusSixth(.., N)`, `btlfx_drawSpriteAtOt`'s ot index), random
spreads (`randRange`, `rand() % N`), angle / fixed-point math (`0x1000`, `0x400`, `>> 12`, `- 0x80` neutral
colour offsets), easing coefficients and frame shifts (`t = frame << 9` stays a shift; its length is a separate
compare that got the name), bone indices and unit ids (0x10), `fxm_easeLerp`'s curve argument, file sound indices
(fx_09b `btlfx_playFileSound(0x13, ...)`), `btlfx_drawScreenGrid` / `btlfx_drawBgOffscreen`'s trailing mode
arguments, and values the code does not tie to one meaning (fx_062 / fx_063's `0x2E8BA` curve step, which divides
by 11 for 10 intervals; fx_081's `/ 42` swell; strands' respawn thresholds).

## Notes

- The all-targets twins share names with their own values: Curaga fx_08e / fx_090, Cura fx_08d / fx_0a2, Demi
  fx_062 / fx_063, Spare Change fx_087 / fx_088, Blind fx_09d / fx_09e, Confuse fx_060 / fx_061.
- fx_09b keeps the existing `FX_09B_SHOTS` (now also the `btlfx_initEffModels` count and the shot arrays' size).
- `build/chor.py` (not committed) applied each module's block and exact-string replacements with occurrence
  counts asserted; `build/abr.py` the abr pass.
