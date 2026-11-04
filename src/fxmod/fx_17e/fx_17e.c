#include "fxmod/fx_17e.h"

s32 fx_17e_op80(s32 mode, Fx17eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    SVECTOR lerpPos;
    s32 frame;
    s32 i;
    s32 fade;

    if (mode == 0) {
        *args = 0x54;
    } else if (mode == 1) {
        fx_17e_env = env;
        for (i = 3; i >= 0; i--) {
            work->timer40[i] = 0;
        }
        ex->btlfx_initEffModels(fx_17e_effModels, 1);
        work->rot28.vx = 0;
        work->rot28.vy = 0;
        work->rot28.vz = 0;
        work->scale8.vx = 0x1000;
        work->scale8.vy = 0x1000;
        work->scale8.vz = 0x1000;
        work->model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_CHESTNUT_CHESTNUT_MODEL), 0xFF);
        work->fade4 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos38);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 1:
            work->timer40[3] = 0x1A;
            work->timer50 = 8;
            work->timer40[0] = 8;
            break;
        case 10:
            work->fade4 = 0x80;
            work->timer40[1] = 0xE;
            break;
        case 24:
            work->timer40[2] = 3;
            break;
        case 26:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHESTNUT_IMPACT_RINGS), (s32)&work->pos38);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (work->timer40[0] != 0) {
            work->timer40[0]--;
            ex->btlfx_getUnitBonePos(0x10, 5, (u16 *)&work->pos30);
            work->pos18.vx = work->pos30.vx;
            work->pos18.vy = work->pos30.vy - 0x64;
            work->pos18.vz = work->pos30.vz;
        }
        if (work->timer40[1] != 0) {
            work->timer40[1]--;
            ex->btlfx_getUnitBonePos(0x10, 0xC, (u16 *)&work->pos30);
            work->pos18.vx = work->pos30.vx;
            work->pos18.vy = work->pos30.vy;
            work->pos18.vz = work->pos30.vz;
        }
        if (work->timer40[2] != 0) {
            work->timer40[2]--;
            ex->btlfx_lerpVec((s16 *)&work->pos30, (s16 *)&work->pos38, (3 - work->timer40[2]) * 0x555, (s16 *)&lerpPos);
            work->pos18.vx = lerpPos.vx;
            work->pos18.vy = lerpPos.vy;
            work->pos18.vz = lerpPos.vz;
        }
        if (work->timer50 != 0) {
            work->timer50--;
            work->fade4 = 0x80 - work->timer50 * 0x10;
        }
        if (work->timer40[3] != 0) {
            work->timer40[3]--;
            ex->Hi_DrawEffModel(&work->rot28, &work->pos18, (s32)&work->scale8, work->model0);
            if (work->fade4 != 0x80) {
                fade = work->fade4 - 0x80;
                ex->Hi_ModifyEffModelAbr(work->model0, 1);
                ex->Hi_ModifyEffModelRGB(work->model0, fade, fade, fade);
            }
        }
        return frame >= 0x42;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17e/nonmatchings/fx_17e", fx_17e_headerPtr);
