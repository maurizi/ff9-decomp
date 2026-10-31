# Round 33 slice A (fx_000-fx_063): runtime questions

Names left as placeholders (or names worth confirming) and what to watch in the emulator to settle them.

- fx_000, fx_002, fx_00a, fx_00b, fx_00f, fx_01b, fx_01e, fx_01f `Fx0XXWork.unk0`: set to 0 in mode 1 and tested `== 0`
  before every frame's work, never written otherwise in the module. Hypothesis: a skip/abort flag some other code
  writes into the task work (watch writes to work+0 while the effect plays).
- fx_018 `Fx018Work83.kind18` (op83 init word 0 = kind 0/1/2): which battle script call passes which kind
  (trace fx_018_op83 mode 1 and the EffInit argument for the abilities that load dir 13 file 0x18).
- fx_026 `Fx026Pt.unk6` (strip point halfword, 0x46/0x58 or 0 for the unused tail): the header calls it a colour, fx_003's
  strip comment calls the same slot the width. Watch btlfx_drawTrail's read of it (width vs brightness of the strip).
- fx_026 `Fx026Shard.unk12` (set to 0x1000 by fxm_shatterRelease; the shard is passed to btlfx_stepModuleParticle as a
  Pool6CEntry): which particle field the step reads there (scale? speed factor?).
- fx_026 `Fx026Work80.unk64` (set 0 in mode 1, never read) and `Fx026Summon.unk54` (summon loader slot, +0x180 = motion):
  identify the loader structure behind env->loader for summon modules.
- fx_034 (note for the coordinator): fx_034_explodeModel / fx_034_pushPrims / fx_034_splitModel / fx_034_calcPrimCentres
  use Hi_GetMdlVertexPtr like fx_096 and fx_1b3 (other slices); if those copies are byte-identical they could share fxm_ names.
- fx_03a `Fx03aWork.unk10[4]` (zeroed with the countdowns, never read): confirm unused.
