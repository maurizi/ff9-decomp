# Choreography literals, slice 3 (fx_0d9 .. fx_0f2)

The effect modules fx_0d9 .. fx_0f2 (Sword Magic, Rebirth Flame, Fenrir 2nd, Bahamut, Doom, Roulette and the
enemy / party abilities between them), named per docs/fxdata.md "Choreography literals (pilot)". Each header
has a `/* fx_XXX choreography */` block after its effect-data names. Counts: `python3 tools/fxlits.py <module>`
(actionable literals: compare / mask / case / arg / assign).

## Counts

| module | effect | before | after | defines | state |
|---|---|---|---|---|---|
| fx_0d9 | Sword Magic Thundaga | 115 | 62 | 57 | done |
| fx_0da | Sword Magic Blizzard | 50 | 20 | 40 | done |
| fx_0db | Sword Magic Blizzara | 63 | 26 | 46 | done |
| fx_0dc | Sword Magic Blizzaga | 112 | 53 | 55 | done |
| fx_0dd | Sword Magic Bio | 44 | 20 | 23 | done |
| fx_0de | Sword Magic Water | 73 | 32 | 26 | done |
| fx_0df | Sword Magic Flare | 140 | 70 | 60 | done |
| fx_0e0 | Sword Magic Doomsday | 189 | 117 | 60 | done |
| fx_0e1_0 | Rebirth Flame (group 0) | 193 | 120 | 55 | done |
| fx_0e1_1 | Rebirth Flame (group 1) | 23 | 11 | 13 | done |
| fx_0e2 | Fenrir (2nd) | 414 | 396 | 9 | partial: sizes, trails, counts, GPU words |
| fx_0e3_0 | Bahamut (group 0) | 205 | 181 | 13 | partial: sizes, strips, bones, ribbon texture, GPU words |
| fx_0e3_1 | Bahamut (group 1) | 279 | 260 | 7 | partial: size, strips, bones, GPU words |
| fx_0e4 | Frog Drop | 30 | 11 | 12 | done |
| fx_0e5 | Soul Blade | 16 | 5 | 6 | done |
| fx_0e6 | enemy Stone | 19 | 8 | 9 | done |
| fx_0e7 | Thievery | 39 | 11 | 19 | done |
| fx_0e8 | enemy Escape | 18 | 9 | 7 | done |
| fx_0e9 | Annoy | 14 | 5 | 9 | done |
| fx_0ea | Sacrifice | 27 | 14 | 10 | done |
| fx_0eb | enemy Psychokinesis | 15 | 2 | 7 | done |
| fx_0ec | Doom | 100 | 56 | 50 | done |
| fx_0ed | Roulette | 78 | 45 | 29 | done |
| fx_0ee | enemy Mirror | 14 | 10 | 3 | done |
| fx_0ef | enemy Red Clipper | 8 | 3 | 3 | done |
| fx_0f0 | enemy Zombie Powder | 25 | 12 | 8 | done |
| fx_0f1 | Free Energy | 3 | 1 | 1 | done |
| fx_0f2 | Tidal Flame | 23 | 8 | 11 | done |
| **total** | | **2,329** | **1,568** | **~650** | 25 done, 3 partial |

761 literals named (33%; 55% in the 25 finished modules). The three summon modules got only the mechanical names
(they hold 898 of the slice's literals): their op80 state machines (fx_0e2 four ops, about 30 frame keys each;
fx_0e3_0 / fx_0e3_1 states 2..10 with keyed bolts, rings and camera tracks) still need a reading pass.

## Notes

- **Prefixes**: the effect-data prefixes (FX_SWDMAG_THUNDAGA, FX_REBIRTH_FLAME, FX_BAHAMUT, FX_FENRIR, FX_DOOM,
  FX_ROULETTE, FX_FROG_DROP, ...). fx_0e1_0 / fx_0e1_1 and fx_0e3_0 / fx_0e3_1 share their prefix (load groups of
  one summon); where a name exists in both groups it has the group's own value (FX_BAHAMUT_ARC_WIDTH 0x19 / 0x1C).
- **Sword Magic twins**: fx_0d9 / fx_0da / fx_0db / fx_0dc are copies of Thundaga fx_018, Blizzard fx_006,
  Blizzara fx_004 and Blizzaga fx_097 and reuse the pilot's names with the FX_SWDMAG_ prefix. Their op80 starts in
  a later state (fx_0d9 in STRIKE, fx_0da in GROW, fx_0dc in CRYSTAL: no charge, swirl or ground bulge). All eight
  Sword Magic modules share op84, the sword flash: `SWORD_STATE_APPEAR / FLICKER / FADE` and their lengths (3, 9, 9;
  Doomsday 3, 14, 14).
- **Model textures**: every `Hi_RegisterTexEffModel` whose args were literals got `FX_<SPELL>_<MODEL>_TPAGE / _CLUT
  / _U / _V` (the comment decodes the page, abr and CLUT position); models sharing one texture share one set
  (FX_SWDMAG_BLIZZAGA_ICE_* / _SHEEN_*, FX_SWDMAG_BIO_BUBBLE_*). `build/texcheck.py`-style check: fxdata.py's lit()
  resolves all of them; the renders stay textured (fx_0d9 GLOW_DOME checked by eye). Three registrations were
  already unresolved before and stay so: fx_0dc's ICE_CRYSTAL models (tpage through the `tp` register variable),
  fx_0ec's GLOW_SHELL (tpage from the display buffer, `tp0 | TPAGE_ABR_ADD`; its CLUT / u / v are named), fx_0dd's
  morph models (data index from a table).
- **VRAM rects** (drawScrollMove) were checked against the sampling model's tpage + u/v: Blizzard / Blizzara /
  Blizzaga reflection maps, Water (4-bit page 0x39, v 0x80 -> (0x240, 0x180)), Flare's fire dome (0x38 ->
  (0x200, 0x100)), Rebirth Flame's two wave grids (0x99 u 0x80 -> 0x280; 0x1B u 0xC0 -> 0x2F0). Not checked:
  Doomsday's STARS rect (0x1E0, 0x100: inside the star models' 4-bit page at u 0x80; source the common effect
  page) and Rebirth Flame's third rect (BAND, (0x2A0, 0x180); its model is summon data).
- **abr arguments**: every literal abr of btlfx_drawScreenTint / drawParticles / drawRings / drawTrail /
  Hi_ModifyEffModelAbr / Hi_ModifySummonModelAbr / btlfx_addPrimAbr is a `BTLFX_ABR_*` name.
- **GPU words** in the modules' primitive helpers: `GPU_POLY_FT4 / G4 / GT4 (| GPU_ABE)`, `PRIM_TAG(POLY_FT4 /
  POLY_G4 / POLY_GT4 / DR_TPAGE / DR_MOVE)`, `sizeof(DR_MOVE)`, `BTLFX_ABR_OPAQUE` (fx_0df, fx_0e0, fx_0e1_0, fx_0e2,
  fx_0e3_0, fx_0e3_1, fx_0ed). fx_0df's screen-grid clamps use SCREEN_W / SCREEN_H_NTSC, fx_0ec's other display
  buffer SCREEN_W.
- **Sizes**: all `*args = N` / `workSize` / `elemSize` are `sizeof`; no struct needed padding except fx_0d9's bolt
  (Fx0d9Bolt + `pad1C[8]`, as the pilot's Fx018Bolt); fx_0ec's halo pool items are `sizeof(SVECTOR)`. Headers gained
  `/* 0xNN */` size comments and array bounds from the names (strip points, ribbons, spawn tables, trail buffers:
  `fx_0e2_op84Trails[][SLOTS * STRIDE]`).
- **Derived values** written as expressions: `STRIP_POINTS - 1`, `FLASH_FRAMES - 1` (fx_0db, fx_0df tints),
  `DEBRIS_FRAMES - DEBRIS_FLASH_FRAME` (fx_0e0), `CLOUD_FRAMES - CLOUD_GROW_FRAMES` (fx_0df), `HIDE_FRAME + HIDE_FRAMES`
  and `GLOW_FADE_FRAME + GLOW_FADE_FRAMES` (fx_0e1_0), `LIFT_FRAMES + DROP_FRAMES` (fx_0de), `MORPH_FRAME +
  MORPH_FRAMES` (fx_0e4), `SHRINK_FRAMES - 1` (fx_0e8), `FLARES_ANIM_FRAMES - 1` (the lens-flare sprite lines).
- **Kept numeric** (per the scheme): otZ biases, random spreads, angle / fixed-point math, easing, one-off scales
  and offsets (model scales, the 0x400 rot / 0x1400 ring scales, fx_0de's caster-size clamps), the screen-grid
  arguments of btlfx_drawScreenGrid (centre / size pairs), the ribbon helper's count / colour args, sound entry
  indices of btlfx_playFileSound, unit ids (0x10 = caster). fx_0e0's pool states other than OBJ_STATE_DEBRIS are
  never spawned by this module and stay numeric; fx_0e4's state64 1 likewise.
- **Open**: fx_0e3_0 / fx_0e3_1 / fx_0e2 frame keys; whether fx_0ed's RIBBON_BONE 9 is the scythe; the model that
  samples fx_0e1_0's BAND rect.
