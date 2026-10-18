#include "fxmod/fx_1a3.h"

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    s32 t = (frame - 4) << 9;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7C04);

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&D_801E93F4, (s16 *)&D_801E93FC, t, (s16 *)pos);
    pos->vy += ex->math_sin(t >> 1) * 0;
    e->unk0.vx = pos->vx << 12;
    e->unk0.vy = pos->vy << 12;
    e->unk0.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1a3_op80(s32 mode, Fx1a3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E7C04 = env;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        D_801E93FC.vx = work->unk10.vx;
        D_801E93FC.vy = work->unk10.vy;
        D_801E93FC.vz = work->unk10.vz;
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk18);
        ex->func_800CFD78((s16 *)&work->unk18, (s16 *)&work->unk8, 0x190, (s16 *)&work->unk20);
        D_801E93F4.vx = work->unk20.vx;
        D_801E93F4.vy = work->unk20.vy;
        D_801E93F4.vz = work->unk20.vz;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&D_801E9404);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk20);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7C14;
            work->unk0->unk12 = 0x23;
            work->unk0->unkC = 0x20;
        }
        if (work->unk0 != NULL) {
            ((EffMotion *)work->unk0->unk0)->unk8 = func_801E7704;
        }
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

s32 fx_1a3_op81(s32 mode, Fx1a3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E7C04 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unkC);
        work->unk14.vx = work->unkC.vx;
        work->unk14.vy = work->unkC.vy;
        work->unk14.vz = work->unkC.vz;
        work->unk14.vy -= 0x190;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk14);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E8074;
            work->unk0->unk12 = 0x33;
            work->unk0->unkC = 0x20;
        }
    } else {
        frame = entry->unk0;
        if (frame == 4) {
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unkC);
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E86D4;
                work->unk8->unk12 = 0x1E;
                work->unk8->unkC = 0x70;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a3/nonmatchings/fx_1a3", fx_1a3_headerPtr);
