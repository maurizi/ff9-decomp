#include "fxmod/fx_043.h"

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 t, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7BC8);
    s32 r = t << 10;

    if (r > 0x1000) {
        r = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&D_801E7BD0, (s16 *)&D_801E7BD8, r, (s16 *)pos);
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (t >= n) {
        return -1;
    }
    return 0;
}

s32 fx_043_op80(s32 mode, Fx043Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E7BC8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        work->unk14.vy -= 0x64;
        D_801E7BD8.vx = work->unk14.vx;
        D_801E7BD8.vy = work->unk14.vy;
        D_801E7BD8.vz = work->unk14.vz;
        ex->func_800DC858(0x10, 0, (u16 *)&work->unkC);
        D_801E7BD0.vx = work->unkC.vx;
        D_801E7BD0.vy = work->unkC.vy;
        D_801E7BD0.vz = work->unkC.vz;
        env->unk2C->unk0[0].vx = work->unkC.vx;
        env->unk2C->unk0[0].vy = work->unkC.vy;
        env->unk2C->unk0[0].vz = work->unkC.vz;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&D_801E7BE0);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7BE8;
            work->unk0->unk12 = 0xB;
            work->unk0->unkC = 0x20;
        }
        if (work->unk0 != NULL) {
            ((s32 *)work->unk0->unk0)[2] = (s32)func_801E7704;
        }
    } else {
        return entry->unk0 >= 0x1E;
    }
    return 0;
}

s32 fx_043_op81(s32 mode, Fx043Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E7BC8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&D_801E7BD0, (s16 *)&work->unk20);
        ex->func_800CFD78((s16 *)&work->unk8, (s16 *)&work->unk20, 0xC8, (s16 *)&work->unk18);
        work->unk18.vy -= 0x64;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk18);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7D48;
            work->unk0->unk12 = 0x34;
            work->unk0->unkC = 0x20;
        }
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk10);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk20.vx;
                work->unk0->unk5C.vy = work->unk20.vy;
                work->unk0->unk5C.vz = work->unk20.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_043/nonmatchings/fx_043", fx_043_headerPtr);
