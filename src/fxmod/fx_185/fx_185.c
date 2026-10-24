#include "fxmod/fx_185.h"

s32 fx_185_op80(s32 mode, Fx185Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        s32 n = *arg4;
        D_801E7AD4 = env;
        work->unk38 = n;
        if (n >= 0x10) {
            return 1;
        }
        if (n == 0) {
            ex->btlfx_initEffModels(D_801E7AE4, 0x10);
        }
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        work->unk0.vy = -0x7D0;
        work->unk8.vx = env->targets->centerX;
        work->unk8.vy = env->targets->centerY;
        work->unk8.vz = env->targets->centerZ;
        work->unk8.vx += ex->btlfx_randCentered(0x7D0);
        work->unk8.vz += ex->btlfx_randCentered(0x7D0);
        D_801E8764.vx = work->unk8.vx;
        D_801E8764.vy = work->unk8.vy;
        D_801E8764.vz = work->unk8.vz;
        ex->btlfx_aimAngles((s16 *)&work->unk0, (s16 *)&work->unk8, (s16 *)&work->unk10);
        work->unk10.vx = ex->ratan2(work->unk8.vy - work->unk0.vy, work->unk0.vz - work->unk8.vz);
        work->unk28.vx = 0x1000;
        work->unk28.vy = 0x1000;
        work->unk28.vz = 0x1000;
        work->unk18.vx = work->unk0.vx;
        work->unk18.vy = work->unk0.vy;
        work->unk18.vz = work->unk0.vz;
        work->unk3C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xB), 1);
    } else {
        frame = entry->unk0;
        if (frame < 6) {
            ex->btlfx_lerpVec((s16 *)&work->unk0, (s16 *)&work->unk8, frame << 10, (s16 *)&v);
            work->unk18.vx = v.vx;
            work->unk18.vy = v.vy;
            work->unk18.vz = v.vz;
        }
        ex->Hi_SetEffModelSlice(work->unk3C, work->unk18.vy);
        ex->Hi_DrawSliceEffModel(&work->unk10, &work->unk18, (s32)&work->unk28, work->unk3C);
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
        work->unk4.vx = D_801E8764.vx;
        work->unk4.vy = D_801E8764.vy;
        work->unk4.vz = D_801E8764.vz;
        D_801E7AD4 = env;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_185/nonmatchings/fx_185", fx_185_headerPtr);
