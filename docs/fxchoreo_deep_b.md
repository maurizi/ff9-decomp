# Effect-module choreography: deep pass B (Bahamut, Ifrit, Madeen)

Follows the scheme in docs/fxdata.md ("Choreography literals"). The slice passes (docs/fxchoreo_s3.md,
docs/fxchoreo_s4.md) gave these five summon modules their mechanical names; this pass reads each op's state machine
state by state. Each module's header has a new `deep pass` block after the existing choreography names.

## Deep pass

Actionable literals (`tools/fxlits.py`): the "slice" column is the count before the slice pass, the "before" and
"after" columns are this pass.

| module | spell | slice | before | after | named here |
|---|---|---|---|---|---|
| fx_0e3_0 | Bahamut (group 0) | 205 | 181 | 127 | 54 |
| fx_0e3_1 | Bahamut (group 1) | 279 | 260 | 177 | 83 |
| fx_114 | Ifrit | 307 | 259 | 111 | 148 |
| fx_0fb_0 | Madeen (group 0) | 242 | 206 | 131 | 75 |
| fx_0fb_1 | Madeen (group 1) | 273 | 244 | 166 | 78 |
| **total** | | **1,306** | **1,150** | **712** | **438** (38%) |

All 20 binaries and the 380 effect modules still verify. decl_check reports 0. The fxdata.py tables still resolve
the renamed texture arguments (LEAF_MOUND on the rock blocks).

### fx_114 (Ifrit)

- **op80 keys inside the shots.** The fireball (FIREBALL_FRAMES 0x2C, which also stops EMBERS and starts
  EMBER_TRAIL). The bowl (BOWL_FRAME 0x28, BOWL_FADE_FRAMES 6, `BOWL_END_FRAME = FRAME + FADE`). The pillars
  (PILLAR_FRAME 0x29, PILLAR_FADE_FRAME 0x36 + PILLAR_FADE_FRAMES 0xD = PILLAR_END_FRAME 0x43, where CANOPY is freed).
  The white flash (FLASH_FRAME 0x2A, FLASH_FRAMES 6, `FLASH_OUT_FRAME = FLASH_FRAME + FLASH_FRAMES` = 0x30). The heat
  tint (HEAT_FRAME 0x30, a 16-frame pulse, HEAT_R / G / DARKEN, fade at 0xE3 over 16, `END = FADE + FRAMES` = 0xF3;
  the pulse stops at SHOT4_END_FRAME). BODY_FRAMES 0xA8 bounds Ifrit's draw, his shade (BODY_SHADE 0x20), the swirl
  stop and the zoom (ZOOM_FRAME 0xA2). The four FLAME_SWIRLs (SWIRLS 4, SWIRL_FRAME 0x7B, SWIRL_PIN_END_FRAME 0xB7).
  The glow grid under Ifrit (GLOW_TPAGE 0x11A / X / Y / COLS / CELL / GRID, 8 frames on a 16-frame pulse, as named
  for Firaga's floor glow).
- **STAND_Z** (-0x4B0) is Ifrit's spot for every op80 piece and camera point 0 of shot 0. **TARGET_Z** (0x320) is the
  impact point in ops 82 and 83. **TRACK_LEAD** (0x10): op80 reads every summon-data path and track at frame + 0x10.
- **ops 81 .. 85.** op81's bowl at bone 0x1B (OP81_FRAME 1, OP81_FADE_FRAMES 0xC, end = sum). op82: HAZE_FRAMES 0x1D,
  ORANGE_RING_2 at 9, and 13 FIRE_TRAILs of 0x50 bytes (`op82Trails[TRAIL_COUNT][TRAIL_BYTES]`), drawn from frame 8
  for 10 frames with fade-in 4 / fade-out 6. The fade-out is set at frame 2 but counts down only inside the trail
  window, so it overrides the fade-in from frame 8 on. op82 also has the grid zoom at 0xE for 4 frames and the
  drawBgOffscreen rect of WHITE_HAZE's page (HAZE_VRAM_*, matching CYLINDER_TPAGE 0x117 = (0x1C0, 0x100)). op83:
  ROCK_PIECES 23 (`model118[1 + ROCK_PIECES]`), RINGS_FRAME 3, the dome window 4 .. 0x25 (fade-ins 3 / 6, fade-out at
  0x15 for 0x10, end = sum), the crater pieces 6 .. 0x14, ROCK_DELAY 2, SPIN_FRAMES 2, FLY_FRAMES 30, FLOOR_Y 0x32.
  op83's scroll reuses FLAME_VRAM_*. op85: WHITE_FADE_FRAMES 0x10, and OP85_FRAMES is defined as it.
- GPU words: `GPU_STP(1, 0)` for the two DR_STP words (0xE6000001), and `GPU_POLY_G4 | GPU_ABE` / `BTLFX_ABR_OPAQUE`
  in the texel-quad copy. `BTLFX_ABR_ADD` for the module's own abr arguments (fxm_tintScreenLerp,
  fxm_drawTexelQuadsB). The unused camera-key helpers get CAM_KEY_SLOTS 64 / CAM_CHANNELS 4, as in fx_0d2.

### fx_0fb_0 (Madeen, group 0)

- **Rock tasks.** SMALL_ROCKS 25 (the rest are big), with `ROCK_MODELS = SMALL_ROCKS + 2` (the model6C /
  rockBlocks arrays). Task states RISE / FLOAT, RISE_FRAMES 40. The rock blocks register on LEAF_MOUND's texture.
- **Keys inside the states.** RIPPLE: LIGHT_BAND_FRAME 0x18. ROCKS: SHEET_FADE_FRAMES 8, RING_OUT_FRAMES 0x19 with
  `/ (N - 1)`, FLASH_FRAMES 0xC with `/ (N - 1)`, the spawn window 0x1E (two a frame before 0xD), ROCKS_SPAWNED
  0x2F, BACKDROP_SWAP_FRAME 0x10. MOUND: MOUND_SHOW_FRAME 0x20. APPEAR: BOB_FRAMES 4, SHAKE_END_FRAME 0xB,
  DUST_FRAME 3. RELEASE: STREAKS_FRAME 4, RELEASE_POSE_FRAME 5 / RELEASE_MOT_FRAME 0x31, RING_2_FRAME 7,
  BLUE_FLASH_FRAME 0x11. FINALE: FADE_IN_FRAMES 2, FINALE_MOT_FRAMES 0xD, RAINBOW_FRAME 4,
  RAINBOW_FLASH_END_FRAME 8, RAINBOW_OPEN_FRAMES 8. The fade-out step 0x33 is
  `0xFF / (FINALE_FRAMES - FINALE_FADE_FRAME + 1)`.
- **Helpers.** Ribbons: RIBBONS 4, RIBBON_POINTS 12. RIBBON_CLEAR 999: fxm_drawRibbonTrail with n > 999 clears
  n - 999 points, so 0x3F3 = `RIBBON_CLEAR + RIBBON_POINTS` and 0xB = `RIBBON_POINTS - 1`. Trails: TRAILS 10,
  WING_TRAILS 5 (bones 0..4 and 5..9, `bonePts1[bone * 5]`), TRAIL_POINTS 16 (`Fx0fb0Trail16` = 4 + 16 * 8 bytes),
  TRAIL_REACH 0x46. The wave sheet is SHEET_ROWS 32 x SHEET_COLS 16 (`2 * ROWS` gouraud edge quads,
  `(COLS - 2) * ROWS` flat ones). The curtain has 44 segments. Both use one texture, WAVE_TPAGE 0x118 / CLUT
  0xFFFF / U 0x10 / V 0x90. GRID_BRIGHT 0x78.
- **op81.** It had `state0 = FX_MADEEN_STATE_RIPPLE`, op80's name. It now has its own LIFT_STATE_FLOAT / SPIN. The
  lift ramp `/ 46` is BLESS_FRAMES, and `*args` is `sizeof(Fx0fb0Work81)`.

### fx_0fb_1 (Madeen, group 1)

- **op80's seven states.** BANDS 0x1C, MORPH 22, APPEAR 24, ARCHES 37, BURST 10, BEAM 0x40, END 0x10. Derived
  ramps are written as expressions of the state length. MORPH's morph is `/ (MORPH_FRAMES - (MORPH_FRAME - 1))`
  = 18. In ARCHES, VIOLET_RINGS (0x11 + 20), the arches and LENS_FLARES (0x14 + 17) and the white-out (0x21 + 4)
  all end exactly at ARCHES_FRAMES 37. BURST's darken step 0x55 is `0xFF / (BURST_FRAMES - DARKEN_FRAME)`.
- BEAM: the beam cylinder texture (BEAM_TPAGE 0x1B = 4-bit (0x2C0, 0x100), CLUT 0x3E80, RADIUS 0x1C00). Its
  scroll rect (BEAM_VRAM_* = (0x2C0, 0x100, 0x20, 0x80), source (0x360, 0x100)) is that page's 0x80 x 0x80 texels.
  Also SPIN_FRAMES 0x20, ORBIT_GROW_FRAMES 0x18, GLOW_RING_COUNT 0x80, RING_TRAILS 2 x RING_TRAIL_POINTS 0x40 (orbit
  step `0x1000 / RING_TRAILS`), SPARKS_FRAME 8 / SPARKS_PER_FRAME 3 and END_GLOW_FRAME 0x38.
- **Rock tasks.** FLY / BURST states, BIG_ROCK_SCALE 0x199 / SMALL_ROCK_SCALE 0xCC, SHARDS 3 (step
  `0x1000 / SHARDS` = 0x555), burst ring windows 10 / 6 / 3 and the swirl flash 8, SHARD_FRAMES 0xD, SPARK_FRAMES
  0x11, HELD_ROCKS 0x19. fxm_drawTexCylinder is CYL_ROWS 20 x CYL_COLS 16. op82's states are DROP_STATE_FALL /
  LAND, with LAND_RING_FRAMES 7.

### fx_0e3_0 (Bahamut, group 0)

- **op80's states**, in run order 0, 10, 1, 2, 4, 5: SWOOP 69, RISE 24, DIVE 0x18, HOVER 0x1A, CHARGE 0x1E, BLAST
  0x41. Every BLAST ramp ends at BLAST_FRAMES: the sphere fade and SMOKE_ARC cone from CONE_FRAME 0x14 (/ 45), the
  PURPLE_BOLTs from 0x18 (/ 41), the screen grid from IMPACT_FRAME 0x1C (/ 37) and the camera sway from 5
  (/ (65 - 4)). All of them are written as `/ (BLAST_FRAMES - X)`. SWOOP's swoop likewise runs from SWOOP_FRAME
  0x18 to SWOOP_FRAMES (/ 45).
- Other keys: INTRO_FLASH_FRAMES 0xC, TRAILS_FRAME 0x2C, CLOUD_RINGS 9, PUFF_EVERY_FRAME 0x23,
  WHITE_FADE_FRAMES 0x2E, RISE_TILT_FRAMES 0x10, RISE_CLIMB_FRAME 0x11, DIVE_MOT_FRAME / CHARGE_MOT_FRAME 0xA,
  PILLAR_FRAME 0xE / PILLAR_FRAMES 10, MOUTH_ARCS 2 / ARC_REACH 0x398, SPARKS_FRAME 7, FIRE_FRAME 0xC, FLASH_FRAME
  0x16 / FLASH_RGB, CONE_RINGS 9, BOLTS 2, ARCS_FEW 4 / ARCS_MANY 6 from 0x19, SHAKE_FRAMES 7.
- **Ribbon helper.** RIBBON_CLEAR 999 (this copy tests n > 1000), 0x3F6 = `RIBBON_CLEAR + RIBBON_SEGMENTS`,
  0xE = `RIBBON_SEGMENTS - 1`, RIBBON_W / H 0x40 / 0x80, `ribbonBones[j + RIBBONS]`. GPU_POLY_GT4 codes.

### fx_0e3_1 (Bahamut, group 1)

- **op80's six states.** RAISE 35, CHARGE 0x30, FIRE 28, STRIKE 2, IMPACT 0xE, EXPLODE 77. CHARGE's sphere fades in
  over 6 and out by 0x2C (`/ (END - IN)` = 38). The flare runs from 0x2B to the state's end (/ 5). FIRE's swing and
  blue tint run from 0xE to the end (/ 14). EXPLODE's RED_STRIP afterglow runs from 0x3B to the end (/ 18).
- Other keys: ORBS_FRAME 0x10, CHARGE_ARCS 6, PURPLE_GLOW_FRAME 0x26, SPIN_RING_FRAME 0x28, RED_FLASH_FRAME 0x2F,
  SWING_BASE_FRAME 0x12, CONE_FRAMES 0x18, BEAM_BOLTS 3 (`fx_0e3_1_bolts[3]`), BOLT_BRANCHES 4, DISC_FRAMES 0x21 x
  DISC_COPIES 3, FLASH_RING_FRAMES 0x10, IMPACT_FLASH_FRAME 4, BURST_FRAMES 10, QUAKE_FRAME 5 (also the four
  explosion sounds) / QUAKE_FRAMES 7, WHITE_FRAME 2 + WHITE_FADE_FRAMES 16, the three red flashes
  (RED_TINT_1..3_FRAME / _RGB), CORE_FRAMES 0x49 / CORE_IN_FRAMES 7, CORE_FLASH 5 / 7, COLUMN_FRAMES 0x41.
  PILLARS 6 come one every PILLAR_STEP 7, each PILLAR_LIFE 0x20 long with PILLAR_RISE_FRAMES 4. Then DOME_FLASH_FRAMES
  0x10, COLUMN_BOLTS 2 (step `0x1000 / 2`), THIRD_BOLT_FRAMES 0x26, LIGHTNINGS 5 (step `0x1000 / 5` = 0x333) and
  EMBERS_FRAME 0x31.
- **Cylinder helper.** CYL_TPAGE 0x1D / CLUT 0x3E81 / U 0 / V 0xC0, abr BTLFX_ABR_ADD. It is CYL_ROWS 3 x CYL_COLS
  16: `ft4 + ROWS * COLS - 2` flat quads, `seg >= ROWS * COLS - 1`, `span = dy * ROWS`.

### Kept numeric, and open questions

- Kept numeric: random spreads and jitters (the bolt helpers' along / amp / angle jitter values per state), otZ
  biases, the 0x1000 ramps and shifts, scales and offsets used once, bone ids (0x1B, 0x3D, 0x5B, 0x24, 7, 0x31),
  sound entries, drawScreenGrid / drawRippleGrid arguments, per-state grid greys (Madeen 0x97 / 0xAD / 0xAA / 0xF0),
  the cloud strip's 240-pixel scroll and the motion indices.
- The bolt step 0x554 in fx_0e3_1 (FIRE / STRIKE / IMPACT) stays: it is not 0x1000 / 3 (0x555).
- **Open: three scroll rects.** fx_0e3_1's (0x340, 0x100, 0x20, 0xC0) from (0x360, 0x100) is in the cylinder's
  4-bit page, but covers v 0 .. 0xBF while the cylinder samples v 0xC0 .., so the model that reads it is unknown.
  fx_0fb_1 has two: (0x360, 0x180, 0x10, 0x40) is in the page of Y_FRAME (4-bit) and RAINBOW_ARCH (8-bit) at v 0x80,
  and (0x2A0, 0x180, 0x10, 0x40), drawn in states 0 .. 1, is in BLUE_SWIRL_DISC's page at v 0x80. An fxdata `--vram`
  look would settle which texels they are.
- **Open:** the 25 held rocks of fx_0fb_1's MORPH state use `kk %= 7` rows, and their meaning is unread.
  fx_0e3_0's DIVE screen grid zooms over 14 frames but is drawn for 0x10, so both stay numeric. fx_114's
  `countA >= 10 ? 9` trail clamp is also unread.
- **Shared with the short summons** (fx_195 Bahamut, fx_1bd Ifrit, named in parallel by another agent): where the
  code is the same, the same names fit (e.g. fx_1bd's projDist zoom `0x208 - elapsed^2 * 4` is FX_IFRIT_PROJ_DIST
  and the OP82 zoom). Harmonise once both are on main.
