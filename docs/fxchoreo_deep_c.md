# Effect-module choreography: deep pass C (Ark, long form)

Modules: Ark's full-summon groups fx_17d_0, fx_17d_3, fx_17d_4, fx_17d_5, fx_17d_6, fx_17d_7, fx_17d_8. The slice pass
(docs/fxchoreo_s5.md) named their states, lengths, pools, trails and scroll rects; this pass reads each op's state
machine one state at a time and names the keys inside the states, the derived ramps, and the helper geometry. The
scheme is docs/fxdata.md, "Choreography literals"; every block is headed `/* fx_17d_N choreography, deep pass */` in
the module header.

## Deep pass

### Counts (actionable literals, `tools/fxlits.py`)

| module | group | before | after | named |
|---|---|---|---|---|
| fx_17d_0 | Ark (group 0): streak field, pieces gather and morph into the Ark | 71 | 49 | 22 |
| fx_17d_3 | Ark (group 3): clouds and smoke volleys | 89 | 50 | 39 |
| fx_17d_4 | Ark (group 4): charge, beam, strand blast | 94 | 52 | 42 |
| fx_17d_5 | Ark (group 5): homing trails and strikers | 88 | 56 | 32 |
| fx_17d_6 | Ark (group 6): glow discs, burst, op83 lightning | 84 | 67 | 17 |
| fx_17d_7 | Ark (group 7): pillar blast, rocks, op83 lightning | 138 | 92 | 46 |
| fx_17d_8 | Ark (group 8): planet, beam | 89 | 62 | 27 |
| **total** | | **653** | **428** | **225** |

All seven verify (20 OK + "effect modules: 380 OK"); decl_check 0. One commit per module.

### Renamed defines

None: every existing fx_17d_* define keeps its name and value (fx_17d_3's `FX_ARK_VOLLEY_3_FRAMES` only gained a
comment), so the short form (fx_1bf_*) can align with these names as they stand. Array extents that were literal
now use the names (`fx_17d_4_strandInfo[FX_ARK_STRANDS * 4]`, `fx_17d_6/7_boltTrails[FX_ARK_BOLT_POOL]`,
`Fx17d6Trail17.points4[FX_ARK_BOLT_POINTS]`, `fx_17d_0_partTrails[FX_ARK_PART_TRAILS][0x24]`, ...).

New effect-data names (file indices passed through fxm_drawArcTrail's `file` parameter, missed by the data pass
because the literal sits at the call, not at a getFileData): fx_17d_6 `FX_ARK_BRANCH_TRAIL` 0x7, `FX_ARK_BOLT_TRAIL`
0x8; fx_17d_7 `FX_ARK_BRANCH_TRAIL` 0xE, `FX_ARK_BOLT_TRAIL` 0xF, `FX_ARK_RING_BOLT_TRAIL` 0x13 (all 0x20-byte trail
strip tables; look pending).

### Names the short form can reuse

fx_1bf_0 shares fx_17d_4's blast (STRAND_POINTS, BEAM_LINE, RED_RING, RED_FLASH_RING are byte-identical): the strand
and particle geometry (`FX_ARK_STRANDS` 18, `FX_ARK_STRAND_LAYERS` 3, `FX_ARK_PATTERN_LISTS` / `_POINTS`,
`FX_ARK_ORBIT_ARCS` / `FX_ARK_ARC_POINTS` / `FX_ARK_ORBIT_RADIUS`, `FX_ARK_PARTICLE_KINDS`, `FX_ARK_OFFSET_PARTICLES`),
the blast keys (`FX_ARK_FLASH_RING_FRAMES`, `FX_ARK_STRANDS_FRAMES`, `FX_ARK_STRANDS_FADE_FRAME`,
`FX_ARK_PATTERN_STEP`), the line sizes (`FX_ARK_BEAM_LINE_HEIGHT`, `_WIDTH`, `FX_ARK_TIP_LINE_WIDTH`) and
`FX_ARK_RED_RING_SCALE`. fx_1bf_1 shares fx_17d_7's models / emitters, fx_1bf_2 fx_17d_8's (`FX_ARK_BEAM_WIDTH`,
`FX_ARK_BEAM_LENGTH`, the STARFIELD sprite's `_SCALE / _X / _Y`). The summon bone 0x21 is `FX_ARK_MUZZLE_BONE` in
fx_17d_4 (where the beam rings start at it) and fx_17d_6.

### Per module

- **fx_17d_0**: STATE_STREAKS' grid key (`GRID_FRAME`, the `(frame - 0x18) * 12` ramp as `GRID_FRAME - 1`); the
  streak colour (R / G / B, three states); STATE_PIECES' 10-key loop; drawPieces' scaled piece (16) and the pieces
  16..18 hidden from track frame 0x28 (`SHED_PIECES_FIRST / _END`, `SHED_FRAME`; only STATE_ARK reaches it);
  STATE_ARK's band (`BAND_OPEN_FRAMES` 7, `BAND_HEIGHT`, `BAND_RGB`) and the whitening `/ (FRAMES - WHITE_FRAME)`;
  the sphere's STATE_ARK scale; fxm_drawStreakField's sentinel `STREAK_NO_PREV` and depth range (`NEAR_Z` / `FAR_Z`,
  the spawn `% 640` = FAR - NEAR, `FADE_Z`); the trail buffers picked `& (PART_TRAILS - 1)`.
- **fx_17d_3**: the three scroll speeds; summon / cylinder / wing-plate / grid scales; VOLLEY / VOLLEY_2's summon
  motion held at frame 0 until `VOLLEY_MOTION_FRAME` and frozen at `END - MOTION_FRAME` from `VOLLEY_MOTION_END` /
  `VOLLEY_2_MOTION_END`; VOLLEY_2's camera dip; motion 5 frozen at `VOLLEY_3_FRAMES` (the frame it reached when
  VOLLEY_3 ended); STATE_DUST's motion freeze and its bone (4).
- **fx_17d_4**: each state's ramp divides by its own length; the white flash rises after `FLASH_FRAME` in SWIRL to
  `FLASH_PEAK` (step `PEAK / (SWIRL_FRAMES - FLASH_FRAME)` = 0x3C) and fades over BOWL's first `FLASH_FADE_FRAMES`
  (step `PEAK / FADE_FRAMES` = 0x14); swirl-dome ring counts, BOWL's turn and swirl lengths, the reversed scroll;
  BEAM's drift and line keys (`/ (BEAM_FRAMES - KEY)` = 10 and 14), RED_RING's fade-in; BLAST: flash ring, strand
  growth (`STRANDS_FRAMES - 1` = 0x1D), pattern spread (`(FADE_FRAME - 1 - STRANDS_FRAMES) * PATTERN_STEP` = 0x15),
  fade `/ (BLAST_FRAMES - STRANDS_FADE_FRAME)` = 13; the helpers' strand / pattern / orbit counts
  (`/ (ORBIT_ARCS * ARC_POINTS)` = 81).
- **fx_17d_5**: GATHER's lens flare window, the cream flash (8 frames from CREAM_RING_FRAME), the grid key, 3 trails a
  frame (also the `% 3` / `/ 3` fan); HOMING's 0x20 homers (bounded by SPARK_POOL); STRIKE's 0x20 strikers in 3
  lanes, the last one `FX_17D_5_HIT_FINAL` (was `2`), the shake keys (`/ (END - PEAK)` = 5), ring fade and scroll
  speed; the sparks: launch / brake / max speed, the strike fall (`0x300` a frame from
  `(SPARK_STRIKE_FRAMES + 1) * FALL_STEP` = 0x1800, so it lands on its last frame), trail widths, the final darken,
  the hit trail tail and the final flash.
- **fx_17d_6**: GLOW's two flashes and motion hold, the disc layers (4, layer 2 the swirl disc), scroll speed, grid
  scale; BURST's camera slide; each state's ramp divides by its length; `fx_17d_6_boltState` values
  (`BOLTS_ON / _END / _PAUSED`); op83's cadence (a bolt every 4 frames until frame 0x43, then every frame), bolt
  points (17, the branch leaving at `% (BOLT_POINTS - 2)`), branch points, jitter frames, width, slots `% BOLT_POOL`.
- **fx_17d_7**: SMOKE's intro flash; RINGS split at `PALE_RING_FRAME` (`/ (RINGS_FRAMES - PALE_RING_FRAME)` = 10) with
  its darken; BLAST: pillar (`/ (BLAST_FRAMES - PILLAR_FRAMES)` = 65), flicker and darken tints, gold flare window
  (`PILLAR_FRAMES + GOLD_FLARE_FRAMES` = 21), the clouds key and shake, PEACH_RING / rumble from 35 (`/ 35`), ROCK_DEBRIS
  at 41, whiteout and ORANGE_RING at 58 (`/ 12`); ROCKS: the row of 24 rocks, 3 rock models, boulders at 40, the fade
  after 41 (`/ (ROCKS_FRAMES - FADE_FRAME)` = 8); op83's widths, ring radius / shrink, bolt height, points.
- **fx_17d_8**: the STARFIELD sprite (scale, resting x / y; the slides end at `X`), puff intervals (3, BEAM 4), GOLD_SPARKS'
  frame, the beam's width / length, grid brightness / scale, the view-matrix save frame, scroll speed, GLOW / END ramps.

### Kept numeric

As in the slices: otZ biases, random spreads and noise amplitudes, the screen-grid arguments, sprite screen
positions other than fx_17d_8's starfield, fixed-point ramps (`0x1000 - cos`, `>> 12`, `- 0x80`), shifts that encode a
length already named at its compare (`frame << 9` for 8 frames), spin steps, one-off model scales and offsets,
emitter rotations, the camera offsets added to bone positions, fx_17d_0's elemSize 0x10 (Fx17d0Part is 0x14), the
`0x2710 - frame * N` reverse-scroll bases, fx_17d_5's turn cap 0x991 / 0x992, fx_17d_7's SCROLL_3 / _4
accelerating speeds (`(t >> 9) + 12`, `(t >> 8) + 8`).

### Open questions

- fx_17d_0: which pieces 16..18 are (they vanish at STATE_ARK frame 0x28) and what piece 16's scale track shapes.
- fx_17d_3: `FX_ARK_VOLLEY_3_FRAMES` as motion 5's freeze frame is read from the timing (motion set at VOLLEY_2's end,
  one motion frame per frame); an emulator trace of Hi_SetSummonMotFrame would confirm it.
- fx_17d_4 / fx_17d_6 / fx_17d_3: bone 0x21 (`FX_ARK_MUZZLE_BONE`) is named from fx_17d_4's use; which body part it is
  needs a look.
- fx_17d_6: `FX_ARK_BOLT_RUSH_FRAME` 0x43 is GLOW_FRAMES + 1; whether op83 is started in step with op80 (so the rush
  begins with STATE_BURST) needs the script timing.
- fx_17d_7: `FX_ARK_BOLT_RING_SHRINK_FRAMES` 28 vs `FX_ARK_BOLT_RING_OP_FRAMES` 0x1E: the ring reaches radius 0 two
  frames before the op stops spawning; kept separate.
- The bolt / branch trail tables (fx_17d_6 0x7 / 0x8, fx_17d_7 0xE / 0xF / 0x13): look pending (fxdata.py does not
  render them).
