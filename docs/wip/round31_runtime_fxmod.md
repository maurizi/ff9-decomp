# Round 31: effect-module questions for a runtime pass

Round 31 named the effect modules from code only (docs/wip/names_round31_fxmod.txt). These need the emulator
(docs/emulator.md; "Generating states" has forced enemy commands and battle fixtures). Each line: module, symbol,
hypothesis, what to trigger.

## Confirm the code-only names
1. **VRAM_BTLFX_SCRATCH_X/Y (include/res_ids.h)**: fx_07e (Slow) fxm_drawRadialScreenGrid with src (0x1C0, 0x180),
   fx_095 (Stop) op81 MoveImage to (0x1C0, 0x100), fx_18b op80 (0x1C0, 0x100, 256x256), fx_112 fxm_captureScreen.
   Hypothesis: the draw/display buffer around the target is copied there each frame and drawn back as a texture.
   Trigger Slow and Stop on an enemy (file ids 0x7E, 0x95), dump VRAM rect (0x1C0, 0x100, 0x100, 0x100) during the
   clock, compare with the framebuffer. Also find which effect loads fx_18b and fx_112.
2. **fxm_setSignal(env, 7, v)** (17 modules: fx_01d, fx_024, fx_02b, fx_045, fx_051, fx_053, ...): writes
   g_btlfxState byte 0x17 (waitDone). Check that the unit ops 0x79-0x7B wait on it while those effects play.
3. **fxm_startMotion2IfActive / fxm_startMotion3IfActive** (fx_041, fx_09b, fx_0c2, fx_100, fx_180, fx_1a6, fx_1b2, ...):
   which motions BtlUnit.motions[2] and [3] are (hit reaction? knock-back?). Trigger fx_041's effect on a party unit
   and watch model->motion.
4. **fxm_camChan* / fxm_camKey*** (Fenrir fx_0d2, Ifrit fx_114 / fx_1bd, fx_0e2, fx_1fc, fx_1fd; Carbuncle fx_0b1,
   Odin fx_105, fx_1ed-1ef): key data written into g_btlCamera.points[] (nibble = point + 1, 10+ = a module array).
   Watch g_btlCamera.points during a Fenrir summon.

## Identify the effects (module-level comments)
5. Jagged-strip modules (fxm_drawJaggedStrip16/15, branches): fx_003, fx_005, fx_042, fx_049, fx_0d7, fx_0d8 and
   fx_018, fx_085, fx_0d9. Hypothesis: the Thunder spells (bolt + branches). Trigger Thunder, Thundara, Thundaga
   and log btlfx_loadFile.
6. fxm_drawTexelQuads modules (fx_007, fx_048, fx_097, fx_098, fx_0d3, fx_0d6, fx_0e1_0, fx_1cc, fx_1fe; variant
   fx_114, fx_1bd): which effect samples a VRAM cell as a coloured 3D grid.
7. fxm_drawRibbonTrail modules (fx_05e, fx_09a, fx_0ed, fx_0fb_0, fx_105, fx_1a2) and fxm_drawStreakField (fx_086,
   fx_08f, fx_17d_0): which effects.
8. fx_0b3 (Leviathan) drawScrollMove at (0x1C0, 0x100/0x180) source 0x1E0: confirm those are scrolling water
   textures (not the screen capture of item 1).
9. Effect names for the remaining unnamed modules in docs/effect-modules.md: a sweep of the spell list with a
   btlfx_loadFile trace (one battle fixture per caster) would fill most of the effect column.
