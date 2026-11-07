# Effect-module choreography: deep pass D (short summons, Moonlight Slash, Death1)

Modules: fx_195 (Mega Flare, short Bahamut), fx_1bd (Flames of Hell, short Ifrit), fx_1be (Worm Hole, short Atomos),
fx_1a2 (Moonlight Slash) and fx_1b3 (Death1). The slice pass (docs/fxchoreo_s6.md) gave them only their mechanical
names. This pass reads each op's state machine one state at a time. The scheme is docs/fxdata.md, "Choreography
literals". Each module header has a new `/* fx_XXX choreography (deep pass) */` block.

## Deep pass

### Counts (actionable literals, `tools/fxlits.py`)

| module | spell | before | after | named |
|---|---|---|---|---|
| fx_195 | Mega Flare (short Bahamut) | 173 | 109 | 64 |
| fx_1bd | Flames of Hell (short Ifrit) | 158 | 74 | 84 |
| fx_1be | Worm Hole (short Atomos) | 118 | 44 | 74 |
| fx_1a2 | Moonlight Slash | 132 | 72 | 60 |
| fx_1b3 | Death1 | 330 | 198 | 132 |
| **total** | | **911** | **497** | **414** (45%) |

All 20 binaries and the 380 effect modules still verify. decl_check reports 0. There is one commit per module.

### fx_195 (Mega Flare)

- **The short form is the long form's tail.** op80 holds the camera for HOLD_FRAMES 2 (STATE_HOLD 0). It then runs
  fx_0e3_1's IMPACT (4) and EXPLODE (5) states with the same state values and the same code. So every key carries
  fx_0e3_1's name: IMPACT_FRAMES 14, IMPACT_FLASH_FRAME 4, BURST_FRAMES 10 (`< BURST_FRAMES + 1`), QUAKE_FRAME 5 /
  QUAKE_FRAMES 7, BEAM_BOLTS 3, EXPLODE_FRAMES 77, WHITE_FRAME 2 + WHITE_FADE_FRAMES 16 (`< WHITE + 1 + FADE`), the
  three red tints, CORE_FRAMES 0x49 / CORE_IN_FRAMES 7, CORE_FLASH 5 / 7, AFTERGLOW_FRAME 0x3B
  (`/ (EXPLODE_FRAMES - AFTERGLOW_FRAME)` = 18), COLUMN_FRAMES 0x41, PILLARS 6 / PILLAR_STEP 7 / PILLAR_LIFE 0x20 /
  PILLAR_RISE_FRAMES 4, DOME_FLASH_FRAMES 0x10, COLUMN_BOLTS 2 (`0x1000 / COLUMN_BOLTS`), THIRD_BOLT_FRAMES 0x26,
  LIGHTNINGS 5 (`0x1000 / LIGHTNINGS` = 0x333), EMBERS_FRAME 0x31. All values match the long form's.
- New in this module: SOUND_TAIL_FRAME 0x3C (the two looping sounds are replaced). The long form plays its own sounds
  elsewhere.
- Helpers: BOLT_POINTS 57 (`Fx195Trail57`), LIGHTNING_POINTS 13 (`Fx195Trail`), and the uncalled
  fx_195_buildOffsetTrail's ARC_POINTS 13 / ARC_WIDTH 0x1A (the long form's arc is 13 points wide 0x1C).
  fxm_drawTexCylinder is CYL_ROWS 3 x CYL_COLS 16 with GPU_POLY_FT4 / GT4 codes and PRIM_TAG. Its texture is CYL_TPAGE
  0x18, a 4-bit page at (0x200, 0x100), where the long form uses 0x1D. CLUT 0x3E81, U 0, V 0xC0. abr is BTLFX_ABR_ADD.
- Open: the scroll rect (0x200, 0x100, 0x20, 0xC0) from (0x220, 0x100) is in the cylinder's page. It covers
  v 0 .. 0xBF, but the cylinder samples from v 0xC0. This is the same question as fx_0e3_1's rect at 0x340.

### fx_1bd (Flames of Hell)

- **ops 82 / 83 are fx_114's.** They now use fx_114's names: TARGET_Z 0x320, the HAZE_VRAM_* drawBgOffscreen rect,
  TRAIL_COUNT 13 x TRAIL_BYTES 0x50 (`fx_1bd_op82Trails[COUNT][BYTES]`), the trail fades, the GPU_STP(1, 0) DR_STP
  word, PROJ_DIST 0x208 and the OP82_ZOOM. op83 has exactly the long form's values: RINGS 3, DOME 4, the fade-ins 3 / 6,
  DOME_FADE_FRAME 21 + 0x10 = DOME_END_FRAME 37, ROCKS 6 .. 20, ROCK_PIECES 23 (`model118[1 + ROCK_PIECES]`),
  ROCK_DELAY / SPIN 2, FLY_FRAMES 30, FLOOR_Y 50, OP83_FRAMES 0x3C, and the FLAME_VRAM_* scroll.
- **op82's own timings** in the short form: TRAILS_FRAME 21 (long form 8), TRAIL_FADE_OUT_FRAME 15 (the same
  "set early, counts only inside the window" pattern), OP82_RING_FRAME 22, OP82_ZOOM_FRAME 26, HAZE_FRAMES 0x2E,
  CLUSTER_FREE_FRAME 33, OP82_FRAMES 0x32.
- op80 is only the shot-0 camera keys: MODEL_SLOTS 0x20, FRAMES 0x5A, CAM_KEY_SLOTS 64 / CAM_CHANNELS 4 (the helpers
  are used here, unlike fx_114's). fxm_drawTexelQuadsB gets GPU_POLY_G4 | GPU_ABE, PRIM_TAG(POLY_G4 / DR_TPAGE).

### fx_1be (Worm Hole)

- op80 is a frame switch that loads countdowns into `counters0[FX_ATOMOS_T_*]`: 17 slots (GATE, GATE_DROP, SHRINK,
  ZOOM, VORTEX, FUNNEL, VORTEX_IN, GRIDS, GRID_IN, GRID_OUT, PULL, DOME, STREAKS_OUT, STREAKS_MOVE, LAND, SPEED_UP,
  DOME_SCROLL), each a countdown or flag that drives one piece. Keys: STREAKS_FRAME 0x18, GRID_OUT_FRAME 0x2C,
  STREAKS_OUT_FRAME 0x30, FUNNEL_FRAME 0x40, RINGS_FRAME 0x50, SHRINK_FRAME 0x54 + SHRINK_FRAMES 8 = CLOSE_FRAME 0x5C,
  LAND_FRAME 0x7C, FRAMES 0xAC. T_SPEED_UP is loaded with `frame`, i.e. FUNNEL_FRAME, and its ease uses that n.
- fxm_easeLerp curves are EASE_LINEAR / SQUARED / INV_SQUARED, as in fx_033. The positions are HOLE_Z 0x418,
  HOLE_Y -0x800, GATE_START_Y -0x2000 and GATE_END_Y -0x1000. VORTEX_SCALE is 0x555. UNIT_PULL_FRAMES 0x28 is the
  Fx1beUnit pull, and DEBRIS_ENDS 64 is the particle ends (mask `DEBRIS_ENDS - 1`).
- **Scroll rects identified.** RING_VRAM_* (0x2C0, 0x100, 0x38, 0x70) and DOME_VRAM_* (0x280, 0x100, 0x20, 0x80) are
  exactly fx_0b8's rects. fxdata shows they are ORANGE_RING's page 0x9B and VORTEX_DOME / VORTEX_TORUS's page 0x1A.
  The rect the slice called "a lava rect with other coordinates", (0x240, 0x140, 0x18, 0x60), is not lava. It is the
  gradient grid's own texels: GRID_TPAGE 0x19 = 4-bit (0x240, 0x100), u 0 .. 0x5F, v 0x40 .. 0x9F. It is now
  GRID_VRAM_*. The grid helper is GRID_ROWS 8 x GRID_COLS 4 cells of 0x1400 x 0xA00, centred by GRID_HALF 0x2800, with
  CLUT 0x3D03 and GPU_POLY_GT4 codes.
- Open: T_GRID_OUT is loaded at 0x2C, but it only counts while the grids are drawn (from 0x40). So the grids fade
  *out* over their first 16 frames, overriding the fade-in, and then show at full brightness. This may be a bug in
  the original. The grid offsets ±0xA00 about HOLE_Y and `HOLE_Z + 0x800` stay numeric.

### fx_1a2 (Moonlight Slash)

- op80 states: CHARGE 93 (ribbons from bone 0x10's blade), SLASH 0xB (MOON_FRAME 4, SLASH_FADE_FRAME 7, BLADE_MODELS 3,
  AFTERIMAGES 6 with `bright / (AFTERIMAGES - 1)`), PAUSE 0xC, PILLAR 29 (BAND_GROW_FRAMES 4, SHATTER_FRAME 10 with its
  tint, ROCK_SPAWN_FRAMES 4 x ROCKS_PER_FRAME 6), DEBRIS 0x64 (ARMOR_2 PLATE_FRAMES 0x30, fade 0x20 over 0x10,
  CUE_FRAME 0x4B). op81: OP81_FADE_FRAME 0x24, OP81_FRAMES 0x2C, `*args = sizeof(SVECTOR)`.
- Pool task: ROCK_STATE_HOP / REST, ROCK_HOP_FRAMES 8, ROCK_HOPS 10, BIG_ROCK_SCALE 0x1800 (the one rock spawned
  resting, which does not turn). Ribbons: RIBBON_CLEAR 999, RIBBON_POINTS 0x20 (`CLEAR + POINTS` = 0x407,
  `POINTS - 1` = 0x1F), GPU_POLY_G4 codes. Shatter: SHATTER_PIECES 130, SHATTER_HOLD_FRAMES 3, SHATTER_FRAMES 0x40.
- Scroll rects: BLADE_VRAM_* (0x280, 0x1E0, 0x40, 0x20) is the BLADEs' texels (BLADE_TPAGE 0xBA, v 0xE0). It is fed
  from an 8-frame strip at (0x2C0, 0x100 + 0x20 k). BAND_VRAM_* (0x240, 0x100, 0x30, 0x80) is the bands' texels
  (BAND_TPAGE 0xB8, u 0x80).

### fx_1b3 (Death1)

- op80 is a frame switch over 0x29C frames with 14 countdown slots (`timers0[FX_DEATH1_T_*]`): FLASH, GLOW, ARCS /
  ARC_KIND, SWEEP / SWEEP_FIRST, LEVEL_A, LEVEL_B, SLICE / SLICE_RUN, BURST, ZOOM, WHITE, CRACKS. Every case label is a
  named key. Keys one frame apart are written as offsets: `LEVEL_A_FRAME = BLAST_1_FRAME + 1`,
  `BLAST_3_FRAME = SLICE_FRAME + 1`, and FINALE / BURST / HIDE = BURST_SPLIT_FRAME + 1 .. 3.
- **The split models.** Summon data 0x80 .. 0x92, on the caster's textures, break where fx_1b3_splitTrigger passes.
  Its mode is now BREAK_SWEEP_X / SWEEP_Z / LEVEL / COLUMN / SLICE / OUTSIDE (the switch in
  fx_1b3_updateSplitPieces). Pieces fly for PIECE_FRAMES 0x40, PIECE_LONG_FRAMES 0x80, or SLICE_FRAMES 0x22 (the same
  value as T_SLICE_RUN). There are three sweeps (SWEEPS 3 x SWEEP_MODELS 3 = `fx_1b3_splits0[9]`, SWEEP_FIRST
  0 / 3 / 6) round a circle of SWEEP_RADIUS 0x1A08 (the x term is `RADIUS >> 3` >> 9). Then the levels
  (LEVEL_TOP_Y 0x990, LEVEL_STEP 0x80, LEVEL_SIDE_X ±0x3CF, LEVEL_FRAMES 0x80), the slice and the burst
  (BURST_FRAMES 0xE0, radius closed at BURST_CLOSED_LEFT 0x80, centre CORE_HEIGHT 0x514 / CORE_Z 0x800, the same
  point as emitPos0). SPLIT_GROUPS is 8.
- Lightning arcs: ARCS 16 (`buf += ARCS * sizeof(Fx1b3Trail)`; the next two carves are the two trail buffers'
  SLOTS * STRIDE), ARC_POINTS 9, ARC_WIDTH 0x80, ARC_FADE_FRAMES 4, ARC_TAIL_FRAMES 0xD. The four kinds are
  ARCS_SCATTER / LIMBS / LIMBS_2 / RINGS (LIMB_ARCS 5 x LIMB_LINKS 4, RING_ARCS 6).
- **Floor cracks.** T_CRACKS (0x20, loaded at each sweep) copies four 0x50 x 0x50 frames into FLOOR_VRAM_*
  (0x380, 0, 0x28, 0x50) at CRACK_1..4_LEFT. That rect is texels of page 0x8E, which is in the split models' texture
  list.
- Open: the texture lists built in mode 1 (tpages 0x8A / 0x8C / 0x8E / 0x8F, CLUT rows 0x3800 ..) and the caster
  pages (getCasterTexInfo 0x2A0) are left numeric. The models' looks were never rendered. The four crack source
  positions also stay numeric.

### Kept numeric

Bone ids, otZ biases, random spreads, sound entries, the screen-grid arguments, the 0x1000 ramps and their shifts
(`frame << 9`, `<< 10`, `>> 2`), the one-off scales, offsets and camera heights, and the split pieces' velocities.
