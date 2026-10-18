#include "fxmod/fx_045.h"

s32 fx_045_op80(s32 mode, Fx045Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    union { /* one stack area: bone matrix in mode 1, draw arguments afterwards */
        MATRIX m;
        struct {
            VECTOR scale;
            SVECTOR rot;
            CVECTOR col;
        } d;
    } u;
    MATRIX m2;

    if (mode == 0) {
        *arg4 = 0x54;
    } else if (mode == 1) {
        D_801E7D38 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk18);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk20);
        work->unk38.vx = work->unk18.vx;
        work->unk38.vy = work->unk18.vy;
        work->unk38.vz = work->unk18.vz;
        ex->func_800CFD78((s16 *)&work->unk18, (s16 *)&work->unk20, 0x3E8, (s16 *)&work->unk38);
        work->unk38.vy = -0xC8;
        ex->func_800DC8AC(0x10, 0x1C, (BtlBone *)&u.m);
        ex->func_800CFE88(&u.m, 0x320, (s32)&work->unk28);
        ex->func_800DC8AC(0x10, 0x36, (BtlBone *)&m2);
        ex->func_800CFE88(&m2, 0x320, (s32)&work->unk30);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk28);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk30);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x100);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x100);
        }
        ex->btlfx_aimAngles((s16 *)&work->unk28, (s16 *)&work->unk38, (s16 *)&work->unk40);
        ex->btlfx_aimAngles((s16 *)&work->unk30, (s16 *)&work->unk38, (s16 *)&work->unk48);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 11:
            work->unk50 = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, work->unk50, work->unk50, 0x80);
            break;
        case 10:
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk38);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk38);
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk38);
            work->unk14 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk38);
            break;
        }
        if (frame >= 0xD) {
            s32 s = (frame << 7) + 0x980;

            u.d.scale.vx = s;
            u.d.scale.vy = s;
            u.d.scale.vz = s;
            u.d.col.b = 0x28;
            u.d.col.g = 0x28;
            u.d.col.r = 0x28;
            u.d.rot.vx = 0;
            u.d.rot.vy = 0;
            u.d.rot.vz = 0;
            ex->func_800E6764(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &u.d.rot, &u.d.scale, &u.d.col, 1, NULL);
        }
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk40.vx;
                work->unk0->unk5C.vy = work->unk40.vy;
                work->unk0->unk5C.vz = work->unk40.vz;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk48.vx;
                work->unk4->unk5C.vy = work->unk48.vy;
                work->unk4->unk5C.vz = work->unk48.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_045_op81(s32 mode, Fx045Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        s32 n = *arg4;

        work->unkC = n;
        D_801E7D38 = env;
        if (n >= env->unkC->unk24) {
            func_801E7CDC(env, 7, 1);
            return 1;
        }
        func_801E7CDC(env, 7, 0);
        ex->btlfx_getUnitPos(work->unkC, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(work->unkC, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (frame == 1) {
            func_801E7CDC(env, 7, 1);
        }
        return frame >= 0x28;
    }
    return 0;
}

void func_801E7CDC(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->unk0 + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_045/nonmatchings/fx_045", fx_045_headerPtr);
