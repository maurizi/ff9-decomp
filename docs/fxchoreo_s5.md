# Choreography literals, slice 5 (fx_129 .. fx_187)

The effect modules fx_129 .. fx_187 (Pumpkin Head, Jet Fire, Photon, Earthquake, Death, Doomsday, Lucky Seven,
Medeo, Madeen, Ark's nine load groups, Ultima, Earth Shake and the enemy / party abilities between them), named per
docs/fxdata.md "Choreography literals (pilot)". Each header has a `/* fx_XXX choreography */` block after its
effect-data names. Counts: `python3 tools/fxlits.py <module>` (actionable literals: compare / mask / case / arg /
assign); "before" is the count at the start of the slice, before the abr and caster-unit passes.

## Counts

| module | effect | before | after | defines | state |
|---|---|---|---|---|---|
| fx_129 | Pumpkin Head | 18 | 1 | 13 | done |
| fx_12a | Jet Fire | 20 | 5 | 5 | done |
| fx_12b | Photon | 43 | 12 | 10 | done |
| fx_12c | Earthquake | 35 | 11 | 24 | done |
| fx_12d | Death (Antlion) | 23 | 3 | 11 | done |
| fx_12f | Paper Storm | 8 | 2 | 3 | done |
| fx_130 | Propeller Wind | 34 | 13 | 12 | done |
| fx_131 | Golem Revival | 12 | 2 | 3 | done |
| fx_132 | Appearance | 5 | 3 | 1 | done |
| fx_133 | Disappear | 4 | 2 | 1 | done |
| fx_134 | Doomsday | 396 | 290 | 76 | partial: states, sizes, textures, scrolls, helpers; orb phases and sprite layouts not read |
| fx_135 | Bad Breath | 5 | 1 | 2 | done |
| fx_136 | Absorb | 11 | 1 | 4 | done |
| fx_137 | Flame | 12 | 4 | 2 | done |
| fx_138 | Lucky Seven | 47 | 14 | 29 | done |
| fx_179 | Medeo | 79 | 37 | 22 | done |
| fx_17a | Madeen | 150 | 86 | 58 | done (STATE_ARCH's inner ramps numeric) |
| fx_17d_0 | Ark (group 0) | 99 | 71 | 21 | partial: states, keyframes, counts, GPU words |
| fx_17d_1 | Ark (group 1) | 72 | 42 | 18 | done |
| fx_17d_2 | Ark (group 2) | 56 | 37 | 17 | done |
| fx_17d_3 | Ark (group 3) | 235 | 89 | 40 | partial: shots, lengths, emitter keys, trails, scrolls |
| fx_17d_4 | Ark (group 4) | 137 | 94 | 23 | partial: states and lengths, trail, scroll |
| fx_17d_5 | Ark (group 5) | 116 | 88 | 27 | partial: states, pool states, keyframes, scroll |
| fx_17d_6 | Ark (group 6) | 118 | 84 | 25 | partial: states, keyframes, trail, bolt pool |
| fx_17d_7 | Ark (group 7) | 193 | 138 | 46 | partial: states, rock pool, scrolls, bolt kinds |
| fx_17d_8 | Ark (group 8) | 116 | 89 | 25 | partial: states, pool, scrolls |
| fx_17e | Chestnut | 23 | 7 | 17 | done |
| fx_17f | Melt | 33 | 11 | 8 | done |
| fx_180 | Ultima | 85 | 7 | 54 | done |
| fx_181 | Countdown | 6 | 2 | 2 | done |
| fx_182 | Countdown (targets' centre) | 6 | 2 | 2 | done |
| fx_183 | Cherry Blossom | 47 | 15 | 17 | done |
| fx_184 | Spear | 23 | 7 | 10 | done |
| fx_185 | Spear (16 spears) | 13 | 7 | 3 | done |
| fx_186 | Earth Shake | 100 | 46 | 45 | done |
| fx_187 | Fire Blades | 32 | 13 | 12 | done |
| **total** | | **2,412** | **1,336** | **688** | 28 done, 8 partial |

1,076 literals named (45%). The rest is mostly what the scheme keeps numeric (otZ biases, random spreads, ramps,
easing, one-off scales and offsets, screen-grid arguments, sprite positions) plus the partial modules' inner timings.

## Notes

- **Passes over the whole slice first** (one commit each): every literal abr of btlfx_drawScreenTint /
  drawParticles / drawRings / drawTrail / addPrimAbr / linkPrimAbr / Hi_ModifyEffModelAbr / Hi_ModifySummonModelAbr
  is a `BTLFX_ABR_*` name (`build/abrpass.py`); the caster / partner unit ids (0x10 / 0x11) passed to the
  btlfx unit functions (getUnit*, setUnit*, addRadiusSixth, ...; `btl_getUnit(id)` underneath) are
  `BTLFX_UNIT_CASTER` / `BTLFX_UNIT_PARTNER` (ovl_0b.h), as are fx_184's setWeaponVisible / fxm_moveUnitArc and
  fx_180's fxm_moveUnitArc unit args. The module-local helpers' abr parameters (fxm_tintScreenLerp,
  fxm_drawTexCylinder, fxm_drawTrailLine, drawTexGrid) got the names at their call sites.
- **Countdown slots**: fx_12c, fx_130 and fx_17e keep their phases in `work->timer*[N]`; the slots got index names
  (`FX_EARTHQUAKE_T_ROCKS / _RISE / _FALL / _SHAKE`, `FX_PROPELLER_WIND_T_TORNADOES / _CRESCENTS`,
  `FX_CHESTNUT_T_HOLD / _LIFT / _THROW / _SHOW`) and the lengths written into them, and the arrays their bound.
- **Sword Magic Doomsday twin**: fx_0e0 is a copy of fx_134's states 6..0xA and its primitive helpers (identical
  line for line). fx_134's helpers were taken over from fx_0e0's named text (GPU words, SCREEN_W tiles,
  BULGE_GRID, TRAIL_POINTS) and states 7..0xA use fx_0e0's names with the FX_DOOMSDAY_ prefix (IMPACT, DEBRIS,
  FADE; DEBRIS_WHITE_RGB, ...). Where the values differ (GATHER is 0x38 here, 0x42 in fx_0e0) each keeps its own.
- **Ark**: the nine load groups share the FX_ARK_ prefix; names that recur (STATE_*, MODEL_COUNT, SCROLL_*,
  CLOUD_TRAIL_TRAIL_*) carry each group's own value. The groups' op80s are shot sequences named by what each state
  draws (fx_17d_3's CLOUDS / VOLLEY_n / DUST, fx_17d_4's CHARGE / HOLD / SWIRL / BOWL / BEAM / BLAST, ...); their
  inner ramps and per-frame camera / sprite numbers stay numeric. fx_17d_6 / fx_17d_7's op83 lightning: bolt pools,
  kinds (STRIKE / RING) and lives, the trail sizes as `sizeof(Fx17d6Trail17)` / `sizeof(Fx17d6Trail5)`.
- **Model textures**: every literal `Hi_RegisterTexEffModel` texture is named (`FX_<SPELL>_<MODEL>_TPAGE / _CLUT /
  _U / _V`, the comment decodes the page and CLUT); `build/texcheck.py` confirms fxdata.py's lit() resolves all of
  them (fx_134, fx_138, fx_179, fx_17a). fx_129's and fx_12d's Hi_RegisterTexListModel lists are local arrays /
  copies of the caster's pages (fx_129's entries named, fx_12d's page count).
- **VRAM rects** checked against the sampling model's tpage + u/v: Earthquake / Earth Shake background copies
  (page 0x117 -> (0x1C0, 0x100)), Lucky Seven's rainbow (RAINBOW_STRIPS 0x9A v 0x80 -> (0x280, 0x180)), Madeen's
  cylinder (0x17 -> (0x1C0, 0x100)) and wings strip (WINGS 0x3A v 0x80), Doomsday's tunnel and stars rects
  (STAR_TUNNEL 0x37, v 0x80 / u 0x80). Not checked (marked in the headers): Earth Shake's smoke rect and all
  Ark scroll rects (their models are 'so' models without a literal tpage).
- **Sizes**: all `*args = N` / workSize / elemSize are sizeof except fx_17d_0's elemSize 0x10 (Fx17d0Part is
  0x14 with its unused tail; kept numeric). Two structs gained pads so sizeof matches: Fx179Work (`unused1C[4]`,
  0x20) and Fx186Rock (`unused24[4]`, 0x28).
- **Kept numeric** (per the scheme): otZ biases, random spreads, angle / fixed-point math, easing, one-off scales
  and offsets, screen-grid arguments, sprite screen positions, sound entry indices, getCasterTexInfo's size arg
  (fx_12d), the swapModelData slot (fx_17f), fx_186's `(frame - 8) << 9` ring ramp.
- **Open**: fx_134's orb sub-phases and STATE_METEOR spawn timing; the Ark groups' inner keyframes (fx_17d_0's
  pieces 16..18 hidden after 0x28, fx_17d_5's striker timings, fx_17d_7's STATE_BLAST keys 35 / 41 / 58); the models
  that sample the Ark scroll rects.
