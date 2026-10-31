# Round 33 slice A (fx_000-fx_063): runtime questions

Names left as placeholders (or names worth confirming) and what to watch in the emulator to settle them.

- fx_000, fx_002, fx_00a, fx_00b, fx_00f, fx_01b, fx_01e, fx_01f `Fx0XXWork.unk0`: set to 0 in mode 1 and tested `== 0`
  before every frame's work, never written otherwise in the module. Hypothesis: a skip/abort flag some other code
  writes into the task work (watch writes to work+0 while the effect plays).
- fx_018 `Fx018Work83.kind18` (op83 init word 0 = kind 0/1/2): which battle script call passes which kind
  (trace fx_018_op83 mode 1 and the EffInit argument for the abilities that load dir 13 file 0x18).
