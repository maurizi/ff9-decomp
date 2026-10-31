# Round 33 fxmod fx_100..fx_1ff: open questions

These came up while naming the fx_1xx effect modules from code only (round 33). Each one is a runtime check or a
type cleanup that the code could not settle, so the members or globals involved keep their placeholder names
(or got a cautious name, as noted). Names are the ones after round 33.

## Runtime checks
- **fx_105 (Odin):** what do the 32 `fx_105_quads` (POLY_FT4 0x400 x 0x600, tpage 0x3A, clut 0x3D00) and the 32
  `fx_105_screenSprites` look like on screen? The names describe the drawing, not the picture.
- **fx_114 (Ifrit):** `Fx114Work80.unk164` is the last argument of Hi_DrawSummonModel (1 before frame 0x30, 0 after).
  What does it mean?
- **fx_138 (Lucky Seven):** what are the 3 `Fx138Ring` entries (`D_801E8408`, values from a row of `fx_138_randSets`,
  models `model96`)? Work `unk88`/`unk8A`/`unk8C`/`unk8E` are only initialised in this module.
- **fx_17d_4 (Ark):** `D_801EB25C` (s8) is only zeroed in fx_17d_4_op80 mode 1. Does another 0x17D load group or ovl_0b
  read it?
- **fx_17d_6 / fx_17d_7 (Ark):** `Work83.unk8` comes from the op 0x83 script argument word. Which values do the effect
  scripts pass?
- **fx_186 (Earth Shake):** `Fx186Rock.unk6` is a bounce amplitude in state 0 and a y velocity in state 1 of
  fx_186_op80PoolTask. What is it?
- **fx_18a (Kuja's Transform):** who reads `fx_18a_op80Done`? Nothing in the module does.
- **fx_197:** `fx_197_overlayOn` starts as 1 in data and is only cleared, so a second cast in the same battle would
  skip the model6C overlay unless the module is reloaded. Check whether it is reloaded.
- **fx_1cc:** work `unk70` (pos58 minus 0x80 in y) is only written in op80 mode 1. Does anything read it?
- **fx_1e9:** `Fx1e9Spark.unk12` is set to 0xE00 + randCentered(0x200) in op82PoolTask mode 1 and is only read by
  btlfx_stepModuleParticle. Which Pool6CEntry field does it alias? The same applies to fx_101/fx_18a `Spark.unk12`.
- **fx_1ec:** `Fx1ecWork80.unk3C` (a copy of the caster body position) and `unk44` are never read in fx_1ec. Are there
  other readers?
- **Trail point pad (fx_1f7, fx_1f2, fx_1f3):** what does the pad of each trail point given to btlfx_drawTrail mean
  (0x80 in fx_1f7, 0x200 in fx_1f2 work `unk38`, 0xC0 in fx_1f3)? Per-point width or brightness?
- **Trail header bytes:** `unk0`..`unk2` of every `FxXXXTrail` / `Bolt` buffer (fx_134 `Fx134Bolt` sets 0x18, 0, 0x18
  through fxm_drawOffsetTrail). Which byte is which count for btlfx_drawTrail?
- **fx_1f2:** case 0x18F clears 16 `fx_1f2_unitTrails` entries, but the array is declared `[4]` (0x150 bytes each).
  What does the memory after it hold at runtime?

## Type cleanups (code, not runtime)
- **fx_101:** `Fx101BattleState` is a view of `Unk800F78A40.signals[0]`, and `Fx101PoolCfg` duplicates `EffInit`. Both
  could probably be replaced by the ovl_0b types if the match allows.
- **Summon views:** `Fx105Summon` (and its copies in about 25 other summon modules) is a view of `Unk80100DC0`
  (`summon` at 0x54, `summonModel` at 0x60). One shared view type would be worth it. fx_105/fx_114 now use
  `summon54`/`summonModel60`, the fx_17d_N modules `model60`.
- **fx_195:** `fx_195_longTrails` is indexed 0-2 in state 4, and index 2 is the same address as `fx_195_longTrail2`, so
  the two are probably one 3-element array.
- **fx_114:** `Fx114Work82` `unk2C`/`unk34`/`unk44`, `Fx114Work83` `unkC`/`unk14`/`unk1C`/`unk104` and `Fx114Work80.unk144`
  are only initialised (rot/pos/scale-like sets that nothing reads). fx_1bd has the same members.
