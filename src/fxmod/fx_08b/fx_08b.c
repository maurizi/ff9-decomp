#include "fxmod/fx_08b.h"

INCLUDE_RODATA("asm/us/fx_08b/nonmatchings/fx_08b", fx_08b_headerPtr);

s32 fx_08b_op80(s32 mode, Fx08bWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    union {
        BtlBone bone;
        struct {
            VECTOR scale;
            SVECTOR rot;
            CVECTOR col;
        } s;
    } u;
    BtlBone bone2;
    s32 frame;
    s32 v;

    if (mode == 0) {
        *arg4 = 0x5C;
    } else if (mode == 1) {
        D_801E7D74 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk18);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk20);
        ex->btlfx_pointAhead((s16 *)&work->unk18, (s16 *)&work->unk20, 0x708, (s16 *)&work->unk38);
        ex->btlfx_pointAhead((s16 *)&work->unk18, (s16 *)&work->unk20, 0x7D0, (s16 *)&work->unk50);
        work->unk38.vy = -0xC8;
        work->unk50.vy = -0xC8;
        ex->func_800DC8AC(0x10, 5, &u.bone);
        ex->btlfx_matrixPointAhead((MATRIX *)&u.bone, 0x190, (s32)&work->unk28);
        ex->func_800DC8AC(0x10, 0x22, &bone2);
        ex->btlfx_matrixPointAhead((MATRIX *)&bone2, 0x190, (s32)&work->unk30);
        work->unk28.vy += 0x32;
        work->unk30.vy += 0x32;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk28);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk30);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x100);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x100);
        }
        ex->btlfx_aimAngles((s16 *)&work->unk28, (s16 *)&work->unk50, (s16 *)&work->unk40);
        ex->btlfx_aimAngles((s16 *)&work->unk30, (s16 *)&work->unk50, (s16 *)&work->unk48);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 11:
            ex->printf("flash\n");
            work->unk58 = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->unk58, work->unk58, 0x80);
            break;
        case 10:
            work->unk58 = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->unk58, work->unk58, 0x80);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk38);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk38);
            work->unk14 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk38);
            break;
        }
        if (frame >= 0xD) {
            v = (frame << 7) + 0x980;
            u.s.scale.vx = v;
            u.s.scale.vy = v;
            u.s.scale.vz = v;
            u.s.col.r = u.s.col.g = u.s.col.b = 0x28;
            u.s.rot.vx = 0;
            u.s.rot.vy = 0;
            u.s.rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &u.s.rot, &u.s.scale, &u.s.col, 1, NULL);
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

s32 fx_08b_op81(s32 mode, Fx08bWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        s32 n = *arg4;
        work->unkC = n;
        D_801E7D74 = env;
        if (n >= env->targets->targetCount) {
            func_801E7D18(env, 7, 1);
            return 1;
        }
        func_801E7D18(env, 7, 0);
        ex->btlfx_getUnitPos(work->unkC, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(work->unkC, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (frame == 1) {
            func_801E7D18(env, 7, 1);
        }
        return frame >= 0x28;
    }
    return 0;
}

void func_801E7D18(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->unk0 + i + 0x10) = v;
}
