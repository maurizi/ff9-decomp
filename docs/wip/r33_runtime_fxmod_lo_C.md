# Round 33 runtime questions, slice C (fx_0b5-fx_0e0)

Names left as placeholders (or named from a guess the code can't settle). One bullet each: module, symbol,
hypothesis, what to watch.

- fx_0df `fx_0df_recordElemDraw` (0x801E7EB8): nothing in the module references it, but fx_0df_op83 reads the
  positions it records (fx_0df_recentPos). Hypothesis: an element draw callback whose address sits in the effect
  file data (Sword Magic Flare, file 0xDF). Watch: breakpoint on 0x801E7EB8 while Steiner casts Flare Sword; note
  $ra (caller in ovl_0b) and whether fx_0df_recentNext advances.
- fx_0e0 `D_801EB1D8`: s32 compared cubed against the squared distance of a Fx0e0Obj to the target in
  fx_0e0_op80PoolTask (switches the object to phase 1). Never written by the module. Hypothesis: a capture radius
  constant. Watch: read the value during Doomsday Sword.
- fx_0e0 `D_801EB1DC`: s32 that ends fx_0e0_op80PoolTask items in phase 1 when nonzero; never written by the
  module. Hypothesis: a constant 0 (dead switch) or a flag the effect file data sets. Watch: memory watch on
  0x801EB1DC during Doomsday Sword.
