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
- fx_04c `((u8 *)env->state)[0x12]` (cleared by fx_04c_op81/op82 mode 1, set to 1 when op82 has visited every target):
  probably a script signal like BTLFX_SIG_WAIT; watch the effect script wait on it (ovl_0b, not this slice's header).
- fx_04f `Fx04fWork.unk8` (never touched) and fx_00b / fx_02a `unkC` (zeroed in mode 1 only): confirm unused.
- fx_062 `Fx062Strip.unk114` / fx_063 `Fx063Strip.unkFC` (random 8-0x20 / 0x40-0x80, written into every strip point's
  `Fx06xPt.unk6`): colour or width of the btlfx_drawTrail strip, same question as fx_026 `Fx026Pt.unk6`.
- fx_057 `Fx057Work80.unk0`.. done; `Fx057Work81.unk8` named otZ8 (btlfx_addRadiusSixth(0, 0), the otZ base of its
  emitters and rings); worth confirming it is the target's depth bias.
- fx_05d `Fx05dWork.unk40` (never touched) and `unk58` (set -1 in mode 1, never read): confirm unused.
- fx_059 `Fx059Work80.posZ4` (the spiral head's z in the btlfx_pushViewRotation frame, -0x20 then +0xC8 per frame):
  confirm it is depth toward the camera.
