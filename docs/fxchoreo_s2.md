# Effect-module choreography literals: slice 2 (fx_0b1 .. fx_0d8)

Slice 2 of the choreography-literal naming that follows the pilot in `docs/fxdata.md` ("Choreography literals
(pilot)"): 39 modules, among them the summons Carbuncle (fx_0b1), Leviathan (fx_0b3), Atomos (fx_0b8), Ramuh
(fx_0ba), Fenrir (fx_0d2) and Phoenix (fx_0d3), and the Sword Magic family fx_0d4 .. fx_0d8. Every module's names
are in a `/* fx_XXX choreography ... */` block after its effect-data names, with the module's game-name prefix and a
comment saying what the code does with the value. Counts are `tools/fxlits.py fx_XXX` (actionable literals:
compare / mask / case / arg / assign), before and after; "defines" counts the block's `#define`s.

| module | effect | before | after | defines |
|---|---|---|---|---|
| fx_0b1 | Carbuncle | 78 | 35 | 40 |
| fx_0b2 | Twister; enemy Twister, Whirlwind (Ark, ...) | 137 | 78 | 36 |
| fx_0b3 | Leviathan | 291 | 181 | 42 |
| fx_0b4 | enemy Aero (Griffin, ...) | 15 | 0 | 12 |
| fx_0b5 | enemy Rainbow Wind (Zemzelett) | 8 | 3 | 4 |
| fx_0b6 | enemy Scorch (Grenade/Vepal) | 5 | 3 | 1 |
| fx_0b7 | enemy Heavy (Jabberwock) | 4 | 2 | 1 |
| fx_0b8 | Atomos | 337 | 261 | 53 |
| fx_0b9 | enemy Sandstorm (Core/Sand Golem, ...) | 15 | 3 | 9 |
| fx_0ba | Ramuh | 377 | 266 | 56 |
| fx_0bb | Mustard Bomb; enemy Mustard Bomb (Drakan, ...) | 8 | 3 | 3 |
| fx_0bc | Darkside | 6 | 2 | 2 |
| fx_0bd | Minus Strike | 6 | 2 | 2 |
| fx_0be | Iai Strike | 7 | 3 | 2 |
| fx_0bf | Thunder Slash | 10 | 3 | 5 |
| fx_0c0 | Shock; Shock (Seiken); enemy Shock (Beatrix) | 10 | 7 | 2 |
| fx_0c1 | No Mercy | 20 | 11 | 6 |
| fx_0c2 | No Mercy (Elan) | 43 | 24 | 10 |
| fx_0c3 | Curse | 2 | 0 | 1 |
| fx_0c4 | Curse (Elan) | 2 | 0 | 1 |
| fx_0c5 | Aura | 5 | 2 | 2 |
| fx_0c6 | Aura (Elan) | 9 | 5 | 3 |
| fx_0c7 | Focus | 5 | 3 | 1 |
| fx_0c9 | Jewel | 22 | 4 | 14 |
| fx_0ca | Poison (all targets); enemy Poison (Quale) | 6 | 1 | 5 |
| fx_0cb | enemy Light Flare (Thorn/Zorn) | 49 | 28 | 15 |
| fx_0cc | enemy Meteorite (Thorn/Zorn) | 20 | 10 | 10 |
| fx_0cd | enemy Aera (Gimme Cat, ...) | 44 | 18 | 14 |
| fx_0ce | enemy Screech, Shriek (Gimme Cat, ...) | 22 | 17 | 6 |
| fx_0cf | Stock Break | 11 | 6 | 3 |
| fx_0d0 | Power Break | 12 | 7 | 4 |
| fx_0d1 | Mental Break | 12 | 8 | 3 |
| fx_0d2 | Fenrir | 183 | 93 | 53 |
| fx_0d3 | Phoenix | 274 | 150 | 80 |
| fx_0d4 | Sword Magic Fire | 14 | 8 | 8 |
| fx_0d5 | Sword Magic Fira | 28 | 14 | 14 |
| fx_0d6 | Sword Magic Firaga | 98 | 48 | 45 |
| fx_0d7 | Sword Magic Thunder | 63 | 37 | 26 |
| fx_0d8 | Sword Magic Thundara | 72 | 45 | 30 |
| **total** | | **2330** | **1391** | **624** |

named 939 of 2330 (40%)

## Notes

- **Twins share names.** fx_0c1 / fx_0c2 (No Mercy and Elan's) share FX_NO_MERCY_BONE, _DASH_FRAME, _DUST_REACH,
  _FLASH_PATH_FRAMES, _IMPACT_FRAME with their own values; fx_0c3 / fx_0c4 (Curse), fx_0c5 / fx_0c6 (Aura) likewise.
  The Sword Magic modules mirror the pilot spells they copy: fx_0d5 = fx_002 (Fira), fx_0d6 op80 = fx_048 (Firaga,
  from state FLASH on; its column grows over `/ 12`, not over COLUMN_GROW_FRAMES, so that divisor stays numeric),
  fx_0d7 = fx_003 (Thunder), fx_0d8 = fx_005 (Thundara; fx_0d8 adds FX_0D8_BOLT_FLAT / _STRIKE / _PAIR for its spawn
  table, the pilot's FX_005_BOLT_* equivalents). All five share op84, the sword flash:
  FX_SWDMAG_<SPELL>_FLASH_STATE_IN / _FLICKER / _FADE with 3 / 9 / 9 frames.
- **Texture args.** Hi_RegisterTexEffModel args are named FX_<SPELL>_<MODEL>_TPAGE / _CLUT / _U / _V (fx_0b1 STAR_CRYSTAL,
  fx_0c9 GEM, fx_0d6 FLAME_PILLAR (also used for FLAME_DISC), fx_0b8 CLOUD_DISC / SPHERE (+ the WHITE_DROPs) /
  PURPLE_DOME (+ CLOUD_FUNNEL) / ORANGE_RING). `tools/fxdata.py fx_XXX --render` still resolves them (the entry table
  prints the names, the renders stay textured: checked fx_0b1 0x23, fx_0c9 0x1, fx_0d6 0x9, fx_0b8 0x13). The modules'
  own draw helpers' texture args are named the same way (fx_0b2's funnels CORE / INNER / OUTER, fx_0d3's ribbon,
  fx_0b3's water).
- **VRAM rects** of btlfx_drawScrollMove were checked against the tpage + v of the model that samples them (fxdata's
  model table / Hi_Register args): fx_0b2 INNER (tpage 0x1A) / OUTER (0x17 v 0x70), fx_0b3 water (0x17, v 0 and v 0x80),
  fx_0b8 CLOUD (0xB6), LAVA (0x17), DOME (0x3A), RING (0xBB), fx_0ba CLOUD (0x99, both u halves), COLUMN (0x15),
  fx_0cb FIREBALL (0x99, also seen in the vram render), fx_0d3 SWIRL (0x19), DOME (0x9B), DISC (0x1A), RIBBON (0x1C v
  0x80), fx_0d6 PILLAR_FLAME (as fx_048). fx_0b8's LAVA_2 rect (a smaller rect of the same page, from the texture
  below it) is the one whose look is not settled.
- **GPU words**: the modules' own primitive helpers now use GPU_POLY_* | GPU_ABE, BTLFX_ABR_OPAQUE and PRIM_TAG(...)
  (fx_0b2, fx_0b3, fx_0b8, fx_0ba, fx_0cb, fx_0d3, fx_0d6); fxm_drawRadialScreenGrid's screen clamp uses SCREEN_W and its
  OT bound OT_LEN. DR_MOVE has no psyq.h type, so `PRIM_TAG`-less `0x5000000` / `+ 0x18` for the SetDrawMove packet
  stay numeric. Model abr args use BTLFX_ABR_ADD / _SUB (Hi_ModifyEffModelAbr, fxm_tintScreenLerp, addPrimAbr).
- **Struct pad**: Fx0cfWork81 was 0x28 bytes but op81 asks for 0x30: it gains `u8 pad28[8]` so `sizeof` matches.
- **Motion ids**: fx_0c2's fxm_startUnitMotion(0x10, ...) args are BTL_MOTION_CAST_END / BTL_MOTION_READY (ovl_0b.h).
- **Derived values are expressions**: fx_0b4's trail buffer `[FX_AERO_PATHS][FX_AERO_PATH_TRAIL_SLOTS *
  FX_AERO_PATH_TRAIL_STRIDE / 4]`, fx_0b2's FX_TWISTER_FRAMES (the four states' sum, 72, used by the orbit), fx_0cd's
  fade thresholds `FX_AERA_<MODEL>_FRAMES - FX_AERA_MODEL_FADE_FRAMES`, fx_0d2's model arrays `1 +
  FX_FENRIR_MORPH_MODELS + FX_FENRIR_PIECE_MODELS`, the various `(frame - X_FRAME) * 0xFF / (END - X_FRAME)` fades.

### What stays numeric

As in the pilot: draw-order biases (otZ, addRadiusSixth), random spreads, angles / fixed-point math, colour
offsets, one-off scales and positions (summon placement coordinates, Fenrir's emitter positions), unit selectors
(`btlfx_getUnitPos(0x10, ...)`), sound indices (btlfx_playFileSound), the camera channel numbers of fxm_camKeyAdd /
fxm_camChanAdd, btlfx_drawScreenGrid / drawRippleGrid / drawRadialScreenGrid geometry. The summons keep more:
Atomos' op80 drives most of its timing through `work->flags0[N]` countdown slots set on keyframes (about 60
literals), which would need a per-slot reading of the draw code to name; Leviathan's, Ramuh's and Phoenix's inner
draw passes (ripples, cloud morphs, bolt pools) keep their per-pass constants; the bolt pool op83 / particle op85 of
Ramuh are named only for their pool sizes.

### Open questions

- fx_0b8 FX_ATOMOS_LAVA_2_*: which model samples the 0x18 x 0x60 rect (look pending).
- fx_0b8 `work->flags0[]` slot meanings (an emulator pass or a closer reading of op80's draw section).
