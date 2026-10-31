#include "fxmod/fx_185.h"

s32 fx_185_op80(s32 mode, Fx185Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        s32 n = *arg4;
        fx_185_env = env;
        work->unk38 = n;
        if (n >= 0x10) {
            return 1;
        }
        if (n == 0) {
            ex->btlfx_initEffModels(fx_185_effModels, 0x10);
        }
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos0);
        work->pos0.vy = -0x7D0;
        work->unk8.vx = env->targets->centerX;
        work->unk8.vy = env->targets->centerY;
        work->unk8.vz = env->targets->centerZ;
        work->unk8.vx += ex->btlfx_randCentered(0x7D0);
        work->unk8.vz += ex->btlfx_randCentered(0x7D0);
        D_801E8764.vx = work->unk8.vx;
        D_801E8764.vy = work->unk8.vy;
        D_801E8764.vz = work->unk8.vz;
        ex->btlfx_aimAngles((s16 *)&work->pos0, (s16 *)&work->unk8, (s16 *)&work->rot10);
        work->rot10.vx = ex->ratan2(work->unk8.vy - work->pos0.vy, work->pos0.vz - work->unk8.vz);
        work->scale28.vx = 0x1000;
        work->scale28.vy = 0x1000;
        work->scale28.vz = 0x1000;
        work->pos18.vx = work->pos0.vx;
        work->pos18.vy = work->pos0.vy;
        work->pos18.vz = work->pos0.vz;
        work->model3C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xB), 1);
    } else {
        frame = entry->frame;
        if (frame < 6) {
            ex->btlfx_lerpVec((s16 *)&work->pos0, (s16 *)&work->unk8, frame << 10, (s16 *)&v);
            work->pos18.vx = v.vx;
            work->pos18.vy = v.vy;
            work->pos18.vz = v.vz;
        }
        ex->Hi_SetEffModelSlice(work->model3C, work->pos18.vy);
        ex->Hi_DrawSliceEffModel(&work->rot10, &work->pos18, (s32)&work->scale28, work->model3C);
        if (frame >= 4) {
            if (work->unk38 == 0xF) {
                ex->btlfx_clearEffModels();
            }
            return 1;
        }
    }
    return 0;
}

s32 fx_185_op81(s32 mode, Fx185Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->pos4.vx = D_801E8764.vx;
        work->pos4.vy = D_801E8764.vy;
        work->pos4.vz = D_801E8764.vz;
        fx_185_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_185/nonmatchings/fx_185", fx_185_headerPtr);
