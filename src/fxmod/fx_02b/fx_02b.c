#include "fxmod/fx_02b.h"

s32 fx_02b_op80(s32 mode, Fx02bWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
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
        *arg4 = 0x50;
    } else if (mode == 1) {
        D_801E7D34 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk14);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk1C);
        work->unk34.vx = work->unk14.vx;
        work->unk34.vy = work->unk14.vy;
        work->unk34.vz = work->unk14.vz;
        ex->btlfx_pointAhead((s16 *)&work->unk14, (s16 *)&work->unk1C, 0x7D0, (s16 *)&work->unk34);
        work->unk34.vy = -0xC8;
        ex->func_800DC8AC(0x10, 0x1F, (BtlBone *)&u.m);
        ex->btlfx_matrixPointAhead(&u.m, 0x0, (s32)&work->unk24);
        ex->func_800DC8AC(0x10, 0x2D, (BtlBone *)&m2);
        ex->btlfx_matrixPointAhead(&m2, 0x0, (s32)&work->unk2C);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk24);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk2C);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x100);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x100);
        }
        ex->btlfx_aimAngles((s16 *)&work->unk24, (s16 *)&work->unk34, (s16 *)&work->unk3C);
        ex->btlfx_aimAngles((s16 *)&work->unk2C, (s16 *)&work->unk34, (s16 *)&work->unk44);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 11:
            work->unk4C = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->unk4C, work->unk4C, 0x80);
            break;
        case 10:
            work->unk4C = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->unk4C, work->unk4C, 0x80);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk34);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk34);
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk34);
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
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &u.d.rot, &u.d.scale, &u.d.col, 1, NULL);
        }
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk3C.vx;
                work->unk0->unk5C.vy = work->unk3C.vy;
                work->unk0->unk5C.vz = work->unk3C.vz;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk44.vx;
                work->unk4->unk5C.vy = work->unk44.vy;
                work->unk4->unk5C.vz = work->unk44.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_02b_op81(s32 mode, Fx02bWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        s32 n = *arg4;

        work->unkC = n;
        D_801E7D34 = env;
        if (n >= env->targets->targetCount) {
            func_801E7CD8(env, 7, 1);
            return 1;
        }
        func_801E7CD8(env, 7, 0);
        ex->btlfx_getUnitPos(work->unkC, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(work->unkC, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (frame == 1) {
            func_801E7CD8(env, 7, 1);
        }
        return frame >= 0x28;
    }
    return 0;
}

void func_801E7CD8(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->unk0 + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_02b/nonmatchings/fx_02b", fx_02b_headerPtr);
