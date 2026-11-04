#include "fxmod/fx_185.h"

s32 fx_185_op80(s32 mode, Fx185Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR spearPos;
    s32 frame;

    if (mode == 0) {
        *args = 0x40;
    } else if (mode == 1) {
        s32 spear = *args;
        fx_185_env = env;
        work->index38 = spear;
        if (spear >= FX_185_SPEARS) {
            return 1;
        }
        if (spear == 0) {
            ex->btlfx_initEffModels(fx_185_effModels, 0x10);
        }
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos0);
        work->pos0.vy = -0x7D0;
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        work->pos8.vz = env->targets->centerZ;
        work->pos8.vx += ex->btlfx_randCentered(0x7D0);
        work->pos8.vz += ex->btlfx_randCentered(0x7D0);
        fx_185_targetPos.vx = work->pos8.vx;
        fx_185_targetPos.vy = work->pos8.vy;
        fx_185_targetPos.vz = work->pos8.vz;
        ex->btlfx_aimAngles((s16 *)&work->pos0, (s16 *)&work->pos8, (s16 *)&work->rot10);
        work->rot10.vx = ex->ratan2(work->pos8.vy - work->pos0.vy, work->pos0.vz - work->pos8.vz);
        work->scale28.vx = 0x1000;
        work->scale28.vy = 0x1000;
        work->scale28.vz = 0x1000;
        work->pos18.vx = work->pos0.vx;
        work->pos18.vy = work->pos0.vy;
        work->pos18.vz = work->pos0.vz;
        work->model3C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_SPEAR_SPEAR), 1);
    } else {
        frame = entry->frame;
        if (frame < 6) {
            ex->btlfx_lerpVec((s16 *)&work->pos0, (s16 *)&work->pos8, frame << 10, (s16 *)&spearPos);
            work->pos18.vx = spearPos.vx;
            work->pos18.vy = spearPos.vy;
            work->pos18.vz = spearPos.vz;
        }
        ex->Hi_SetEffModelSlice(work->model3C, work->pos18.vy);
        ex->Hi_DrawSliceEffModel(&work->rot10, &work->pos18, (s32)&work->scale28, work->model3C);
        if (frame >= 4) {
            if (work->index38 == FX_185_LAST_SPEAR) {
                ex->btlfx_clearEffModels();
            }
            return 1;
        }
    }
    return 0;
}

s32 fx_185_op81(s32 mode, Fx185Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        work->pos4.vx = fx_185_targetPos.vx;
        work->pos4.vy = fx_185_targetPos.vy;
        work->pos4.vz = fx_185_targetPos.vz;
        fx_185_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SPEAR_WHITE_RING), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_185/nonmatchings/fx_185", fx_185_headerPtr);
