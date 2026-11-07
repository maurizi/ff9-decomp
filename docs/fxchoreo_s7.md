# Choreography literals, slice 7 (fx_1bf_0 .. fx_1fe)

The effect modules fx_1bf_0 .. fx_1fe (the short forms of Ark, Carbuncle, Fenrir and Phoenix, Carbuncle's other
forms, Dragon's Crest, Six Dragons, the Trance command and the enemy skills between them: Beatrix's Thunder Slash
and Stock Break, Sealion's appearance, Correct!!! / Wrong!!!, Kuja's Ultima, Necron's Appearance, Death3, Grand
Cross, Neutron Ring and Blue Shockwave, Vanish, Night, Aera), named per docs/fxdata.md "Choreography literals
(pilot)". Each header has a `/* fx_XXX choreography */` block after its effect-data names. Counts: `python3
tools/fxlits.py <module>` (actionable literals: compare / mask / case / arg / assign); "before" is the count at the
start of the slice (before the abr pass).

## Counts

| module | effect | before | after | defines | state |
|---|---|---|---|---|---|
| fx_1bf_0 | Ark (short), group 0 | 51 | 36 | 9 | done (fx_17d_4's BEAM_2 / BLAST) |
| fx_1bf_1 | Ark (short), group 1 | 193 | 138 | 46 | done (fx_17d_7's names) |
| fx_1bf_2 | Ark (short), group 2 | 116 | 89 | 25 | done (fx_17d_8's names) |
| fx_1c9 | enemy Thunder Slash | 7 | 2 | 4 | done (fx_0bf's op80) |
| fx_1cb | enemy Stock Break | 3 | 1 | 1 | done (fx_0cf's op80) |
| fx_1cc | enemy Sealion appears | 171 | 135 | 17 | partial: state lengths, sizes, textures, GPU words |
| fx_1e6 | enemy Correct!!! | 50 | 21 | 18 | done |
| fx_1e7 | enemy Wrong!!! | 16 | 6 | 8 | done |
| fx_1e9 | Trance command (not in Trance) | 69 | 37 | 24 | done (fx_101's names) |
| fx_1ea | Dragon's Crest | 90 | 28 | 28 | done |
| fx_1eb | Six Dragons | 36 | 9 | 15 | done |
| fx_1ec | enemy Ultima | 69 | 14 | 44 | done |
| fx_1ed | Carbuncle (3) | 69 | 31 | 35 | done (fx_0b1's names) |
| fx_1ee | Carbuncle (2) | 114 | 68 | 41 | done (fx_0b1's op80, own op81) |
| fx_1ef | Carbuncle (4) | 84 | 37 | 43 | done (fx_0b1's op80, own op81) |
| fx_1f0 | enemy Appearance | 65 | 19 | 37 | done |
| fx_1f1 | enemy Death3 | 48 | 18 | 12 | done |
| fx_1f2 | enemy Grand Cross | 266 | 234 | 13 | partial: counts, trails, emitter / phase keys, length |
| fx_1f3 | enemy Neutron Ring | 145 | 53 | 52 | done |
| fx_1f4 | Vanish | 17 | 6 | 7 | done |
| fx_1f5 | Night | 10 | 2 | 5 | done |
| fx_1f6 | enemy Aera (Griffin) | 44 | 16 | 18 | done (fx_0cd's names) |
| fx_1f7 | enemy Blue Shockwave | 41 | 15 | 16 | done |
| fx_1f8 | Carbuncle (short) | 11 | 4 | 9 | done (fx_0b1's op81) |
| fx_1f9 | Carbuncle (3) (short) | 2 | 0 | 2 | done (fx_1ed's op81) |
| fx_1fa | Carbuncle (2) (short) | 47 | 37 | 8 | done (fx_1ee's op81) |
| fx_1fb | Carbuncle (4) (short) | 17 | 6 | 10 | done (fx_1ef's op81) |
| fx_1fc | Fenrir (short) | 62 | 23 | 29 | done (fx_0d2's names) |
| fx_1fd | Fenrir (2nd) (short) | 172 | 125 | 22 | partial: lengths, sizes, counts, GPU words, scroll rects |
| fx_1fe | Phoenix (short) | 70 | 33 | 33 | done (fx_0d3's op81 states) |
| **total** | 30 modules | **2,155** | **1,243** | **631** | 26 done, 4 partial |

912 literals named (42%).

## Notes

- **abr pass first** (one script over the slice): every literal abr argument of btlfx_drawScreenTint / drawParticles /
  drawRings / drawTrail / addPrimAbr / linkPrimAbr / Hi_Modify*ModelAbr, plus the modules' own abr parameters
  (fxm_drawTrailLine, fxm_tintScreenLerp, fxm_tintScreenRatio), is a `BTLFX_ABR_*` name (108 sites); unit 0x10 is
  `BTLFX_UNIT_CASTER` (39 sites).
- **Twins via token alignment** (build/twin.py, uncommitted helper: aligns a named module's C and header tail with
  its twin's, function by function with `--byfunc`, and carries each name over with the twin's own value; names
  that would get two values are refused). Long forms whose names were reused: fx_0b1 (Carbuncle) for fx_1ed and the
  op80 of fx_1ee / fx_1ef; fx_1ed / fx_1ee / fx_1ef / fx_0b1 op81s for the short forms fx_1f9 / fx_1fa / fx_1fb /
  fx_1f8; fx_17d_4 / 7 / 8 (Ark, on main by then) for fx_1bf_0 / 1 / 2; fx_0d3's op81 for fx_1fe; fx_0d2 for fx_1fc,
  fx_0e2 for fx_1fd's head / trail counts; fx_101 for fx_1e9; fx_0bf / fx_0cf for fx_1c9 / fx_1cb; fx_0cd for fx_1f6.
  Where the twin's emitters differ the key names follow the module's own data (fx_1ed..fx_1ef: ORBIT_GLOW_* and
  GLOW_RING keys instead of fx_0b1's motes / red ring); where the short form's op is a different op the long form's
  length name is not reused (FX_CARBUNCLE_GLOW_FRAMES for the short forms' op80, FX_FENRIR_CAMERA_FRAMES,
  FX_PHOENIX_* only for the three op81 states fx_1fe has).
- **Short forms' own values**: Ark's fx_1bf_0 waits in BEAM_2 (FX_ARK_BEAM_2_FRAMES 0x3E, the orbit ramp over
  `BEAM_2_FRAMES - RING_FRAME` = 24, fx_17d_4's whole BEAM_2), fx_1bf_1's bolt ring is 5 frames (4 in fx_17d_7),
  Phoenix's white fade-in starts in BURST (FX_PHOENIX_WHITE_IN_FRAME 0x18), Fenrir's ground op lasts a frame longer
  (FX_FENRIR_GROUND_SLICE_FRAMES 0x18, MORPH_FRAMES 0x28, PIECES_FRAME 1).
- **Countdown slots** (FX_<SPELL>_T_<PHASE> plus the lengths written into them): fx_1f0 (Appearance, 8 of 16 slots,
  two never set), fx_1f3 (Neutron Ring, 11 slots), fx_1f6 (Aera, 3), fx_1ef / fx_1fb (Carbuncle (4) fade out / in);
  fx_1ed's FX_CARBUNCLE_TIMER_* are fx_0b1's.
- **Derived values** as expressions: Dragon's Crest's morph spans (`MORPH_2_FRAME - MORPH_1_FRAME` ...) and trail fade
  step (`0x1000 / (TRAILS_FRAMES - EMBLEM_FRAME)` = 0x111), Six Dragons' / Death3's / Appearance's model fades
  (`FRAMES - FADE_FRAMES (+ 1)`), Death3's `UNIT_FRAMES (2 * PHASE_FRAMES)`, Ark's strand reach
  (`STRANDS_FRAMES - 1`) and fade (`BLAST_FRAMES - STRANDS_FADE_FRAME` = 13), Blue Shockwave's point buffers
  (`LINKS * LINK_STEPS`), Neutron Ring's ring spawn bound (`RING_FRAMES + 1`), Correct!!!'s trail size
  (`4 + POINTS * 8`), Phoenix's grow ramp (`BURST_FRAMES - GOLD_FEATHERS_FRAME` = 25).
- **Sizes**: every `*args` / workSize / elemSize is `sizeof` (fx_1e9's spark, fx_1ee's spark grid stride
  `i * sizeof(row) + j * sizeof(Fx1eeSpark)`, fx_1bf_1's trail sizes). Padded: Fx1ebWork82 (+6, 0x60), Fx1ccWork80
  (+4, 0x88).
- **VRAM rects** checked against the sampling model's tpage + u/v (fxdata.py's somodel rows): Death3's rune cylinders
  (4-bit 0x17 -> (0x1C0, 0x100 / 0x180, 0x20, 0x80)), Appearance's smoke (8-bit 0x98 -> (0x200, 0x100, 0x40, 0x80);
  0x97 -> (0x1C0, 0x120, 0x38, 0x70)), Neutron Ring's fire (0x99), grey disc (0x97 at u 0x80 -> x 0x200) and torus
  (4-bit 0x1B), Phoenix short's dome / disc (fx_0d3's rects), Fenrir 2nd short's wind strips (0x97 / 0x99 rows
  0x80 .. 0xBF), fx_1e9's aura (fx_101's). Fenrir short's background rect (0x240, 0x100) is fx_0d2's at another x.
- **Model textures**: every literal Hi_RegisterTexEffModel texture is a name (fx_1e9, fx_1f8, fx_1bf_*, fx_1cc);
  `build/texcheck.py` resolves all of them through fxdata.py's lit() (fx_1cc's `tpage` variable as before), and the
  model PNGs of every module with models rendered identically before and after (md5 of build/fxdata/<file>/*model*.png,
  before vs after, all 18 modules whose files have models).
- **GPU words**: `GPU_POLY_G4 / GT4 | GPU_ABE`, `BTLFX_ABR_OPAQUE` and `PRIM_TAG(POLY_G4 / POLY_GT4 / DR_TPAGE)` in
  fx_1fe's and fx_1cc's primitive helpers, `PRIM_TAG(POLY_GT4)` in fx_1fd's edge quads; `BTL_MOTION_KO` for the
  motion after the heavy hit (fx_1e7, fx_1ec).
- **Kept numeric** (per the scheme): otZ biases, random spreads, angle / fixed-point math and easing, one-off offsets
  and scales, bone ids, unit indices, sound entries, camera-key track numbers, drawScreenGrid / drawRippleGrid
  arguments, the scroll speeds, the LOAD macros' file ids in fx_1ee / fx_1fa (effect data, not choreography).
- **Open**: fx_1f2's model countdowns (timer4 .. timer58, which model each drives, their fades) and the keys that only
  set them (0x4F, 0x8F, 0xCF, 0x13F, 0x19F, 0x1A7); fx_1cc's effect keys inside states 0..6 and its pool task;
  fx_1fd op82's ribbon path keys (the piecewise `frame % 60` tables, fx_0e2 keeps them numeric too); fx_1fa's spark
  rows are named but its LOAD file ids are not.
