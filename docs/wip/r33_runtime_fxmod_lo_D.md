# Round 33 runtime questions, fxmod slice D (fx_0e1_0-fx_0ff)

Placeholders left after the code-only pass, with what to watch in the emulator to settle them.

- fx_0e1_1 `Fx0e11BattleState.unk22` (byte 0x22 of env->state): op80 state 1 starts the fade-out once it is 0 after frame 0x35. Hypothesis: a "summon camera / script still running" flag; watch who writes state+0x22 during Rebirth Flame.
- fx_0e1_1 `Fx0e11Work.unk10`: only cleared in mode 1. Hypothesis: unused; watch for reads of work+0x10.
- fx_0e7 `Fx0e7Work81.unk68`: set to -1 and only tested (with the fade countdown) before Hi_ModifyEffModelRGB. Hypothesis: a fade-in countdown that this op never starts; check during Thievery whether it ever leaves -1.
- fx_0f3 `Fx0f3Work.unk20`: never written by the C. Hypothesis: unused padding; confirm in Scoop Art.
- fx_0fa `Fx0faWork.unk0[10]`: mixed countdowns/flags (0 rise, 1 drop, 2 model drawn, 4/5 scroll flashes, 6 aim emitter); slots 3 and 7-9 are never touched. Left as an array.
- fx_0fc identity: the effect label is the undecoded enemy ability name `<FAD5><FC>=`; run the enemy sequence that plays dir 13 file 0xFC and read the ability name on screen.
