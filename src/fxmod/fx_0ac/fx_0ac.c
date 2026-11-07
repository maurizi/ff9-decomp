#include "fxmod/fx_0ac.h"

s32 fx_0ac_op80(s32 mode, Fx0acWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0acWork);
    } else if (mode == 1) {
        fx_0ac_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos18);
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_SLEEPING_JUICE_SPLASH_FRAME:
            ex->btlfx_getUnitArgBoneMatrix(BTLFX_UNIT_CASTER, 0, &bone);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone, FX_SLEEPING_JUICE_SPLASH_REACH, (s32)&work->pos20);
            work->pos20.vy -= FX_SLEEPING_JUICE_SPLASH_HEIGHT;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SLEEPING_JUICE_SPLASH_TRAILS), (s32)&work->pos20);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)fx_0ac_trails;
                work->emit0->trailSlots = FX_SLEEPING_JUICE_SPLASH_TRAILS_TRAIL_SLOTS;
                work->emit0->trailStride = FX_SLEEPING_JUICE_SPLASH_TRAILS_TRAIL_STRIDE;
            }
            ex->btlfx_aimAngles((s16 *)&work->pos20, (s16 *)&work->pos10, (s16 *)&work->aim28);
            break;
        case FX_SLEEPING_JUICE_BUBBLES_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SLEEPING_JUICE_BUBBLES), (s32)&work->pos18);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SLEEPING_JUICE_GLOW), (s32)&work->pos18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SLEEPING_JUICE_RIPPLE), (s32)&work->pos18);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            break;
        }
        if (frame >= FX_SLEEPING_JUICE_SPLASH_FRAME) {
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->rot.vx = work->aim28.vx;
                    work->emit0->rot.vy = work->aim28.vy;
                    work->emit0->rot.vz = work->aim28.vz;
                }
            }
        }
        return frame >= FX_SLEEPING_JUICE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ac/nonmatchings/fx_0ac", fx_0ac_headerPtr);
