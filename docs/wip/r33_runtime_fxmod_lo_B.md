# Round 33 runtime questions, fxmod slice B (fx_078-fx_0b4)

Names left as placeholders because the code alone does not settle them. One bullet per symbol: module, symbol,
hypothesis, and what to watch in the emulator.

- fx_079 / fx_07a `Fx079Pt.unk6`, `Fx07aPt.unk6`: the 4th halfword of each btlfx_drawTrail point, set to timer * 4.
  Hypothesis: strip width (as the pad of fx_003's strip points). Watch the strip thickness while the timer counts down
  (Demi Shock).
- fx_07a `D_801E9504`: zeroed in fx_07a_op80 mode 1 next to the disc radii, never read by the module C. Hypothesis:
  unused radius slot. Watch for reads (breakpoint on read) during Demi Shock (Elan).
- fx_0a9 `Fx0a9Part.unk12`: set to 0xE00 +- 0x20 in fx_0a9_op83PoolTask mode 1, then only read by
  btlfx_stepModuleParticle (asm). Hypothesis: per-particle drag / speed factor. Watch the bolt points scatter with the
  value forced to 0x1000 (enemy Lightning).
- fx_0b3 `fx_0b3_sharedModel` (D_801EE270): effect model block 0x89 drawn at scale 0x2800 by fx_0b3_op81, op83 and op84.
  Hypothesis: the sea surface / backdrop model of Leviathan. Watch which mesh disappears when its Hi_DrawEffModel is
  skipped.
- fx_0b3 `Fx0b3Track.unk0`, fx_0b4 `Fx0b4Path.unk0`: the first halfword of an effect-file key list, skipped by the
  key readers. Hypothesis: key format / stride (1, 3, 6 or 9 halfwords). Dump a few key blocks from the loaded file.
- fx_0b1 / fx_0b3 / fx_09a `Fx0b1Summon.unk54` and friends: pointer in the summon slot behind env->loader whose 0x180+
  words are the summon motions (Hi_SetSummonMotion). Hypothesis: the summon's model/motion data block. Compare with the
  loader slot during Carbuncle.
- fx_097 / fx_098 `Fx097Work80.unk70`, `Fx098Work80.unk70`: target position raised by 0x80, only written. Hypothesis:
  unused leftover. Watch for reads.
