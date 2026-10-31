# Round 33 runtime questions, fxmod slice B (fx_078-fx_0b4)

Names left as placeholders because the code alone does not settle them. One bullet per symbol: module, symbol,
hypothesis, and what to watch in the emulator.

- fx_079 / fx_07a `Fx079Pt.unk6`, `Fx07aPt.unk6`: the 4th halfword of each btlfx_drawTrail point, set to timer * 4.
  Hypothesis: strip width (as the pad of fx_003's strip points). Watch the strip thickness while the timer counts down
  (Demi Shock).
- fx_07a `D_801E9504`: zeroed in fx_07a_op80 mode 1 next to the disc radii, never read by the module C. Hypothesis:
  unused radius slot. Watch for reads (breakpoint on read) during Demi Shock (Elan).
