# Round 33 runtime questions, fxmod slice D (fx_0e1_0-fx_0ff)

Placeholders left after the code-only pass, with what to watch in the emulator to settle them.

- fx_0e1_1 `Fx0e11BattleState.unk22` (byte 0x22 of env->state): op80 state 1 starts the fade-out once it is 0 after frame 0x35. Hypothesis: a "summon camera / script still running" flag; watch who writes state+0x22 during Rebirth Flame.
- fx_0e1_1 `Fx0e11Work.unk10`: only cleared in mode 1. Hypothesis: unused; watch for reads of work+0x10.
- fx_0e7 `Fx0e7Work81.unk68`: set to -1 and only tested (with the fade countdown) before Hi_ModifyEffModelRGB. Hypothesis: a fade-in countdown that this op never starts; check during Thievery whether it ever leaves -1.
- fx_0f3 `Fx0f3Work.unk20`: never written by the C. Hypothesis: unused padding; confirm in Scoop Art.
- fx_0fa `Fx0faWork.unk0[10]`: mixed countdowns/flags (0 rise, 1 drop, 2 model drawn, 4/5 scroll flashes, 6 aim emitter); slots 3 and 7-9 are never touched. Left as an array.
- fx_0fc identity: the effect label is the undecoded enemy ability name `<FAD5><FC>=`; run the enemy sequence that plays dir 13 file 0xFC and read the ability name on screen.
- fx_0e1_0 `Fx0e10Work80.unk30`: only cleared in mode 1; `Fx0e10Summon.unk54` (pointer whose +0x180 is the summon motion). Hypothesis: unk54 is the loader's summon data block; watch env->loader during Rebirth Flame.
- fx_0e1_0 `fx_0e1_0_scaleXY` (0x801E88F0): no caller and no data reference in the module. Hypothesis: dead vertex callback; a breakpoint during Rebirth Flame should never hit.
- fx_0ec `Fx0ecShard.unk1A`: a start delay in fxm_shatterModelUpdate (age waits until it), a triangle index in fxm_splitRelease / fxm_splitUpdate. Left unnamed; the shard struct serves both pools.
- fx_0ec `Fx0ecVertex.unk6`: cleared per triangle by fxm_splitModelAdd; it is the pad of the first vertex, which fxm_splitRelease uses as "already released". Hypothesis: released flag; confirm with a watch during Doom.
- fx_0ed `Fx0edWork80.unk20`: gets ribbon point a each frame but nothing reads it in the module. Hypothesis: unused; watch reads of work+0x20 during Roulette.
- fx_0e3_0 `Fx0e30Work80.unk68/unk70/unk7C`: a rotation, position (fx_0e3_0_origin) and scale (0x1000) set in mode 1 that no state reads; `unk48` only cleared. Hypothesis: a model transform the Bahamut code no longer uses.
- fx_0e3_0 `Fx0e30BonePair.unk0/unk2`: entries 0-3 are bone pairs, entries 4-7 distances for btlfx_matrixPointAhead, so the members stay unnamed.
- fx_0e3_1 `Fx0e31Work80.unk48` (a point ahead of a hand bone, the base of the camera target swing) and `unk6C` (never touched by the C). Hypothesis: unk48 = swing base; confirm with a watch during Bahamut's Mega Flare.
- fx_0fb_0 `Fx0fb0Rock.unk6` (bob height in state 0, a phase from btlfx_rand in state 1) and `Fx0fb0Work80.unkD8/unkF8` are named only where one role holds; unk6 stays. `D_801EB35C` (declared, unused in C) sits inside fx_0fb_0_ribbonRgb.
- fx_0fb_1 `Fx0fb1Rock.unk4/unk6/unk8`: a velocity added to the position for one rock kind, a particle size for kind 4. Left unnamed; watch the pool during Madeen. `D_801F0D3C` (BtlModel[], declared but unused in C) left as is.
