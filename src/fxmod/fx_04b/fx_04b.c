#include "fxmod/fx_04b.h"

s32 fx_04b_op80(s32 mode, Fx04bWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk8.vx = env->targets->centerX;
        work->unk8.vy = env->targets->centerY;
        D_801E7B94 = env;
        work->unk8.vz = env->targets->centerZ;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 10:
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk8);
            if (work->unk0 != NULL) {
                work->unk0->unk14 = (s32)D_801E7BA4;
                work->unk0->unk12 = 0x82;
                work->unk0->unkC = 0x18;
            }
            break;
        case 40:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk10);
            D_801E7B9C.vx = work->unk10.vx;
            D_801E7B9C.vy = work->unk10.vy;
            D_801E7B9C.vz = work->unk10.vz;
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk18);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E87D4;
                work->unk4->unk12 = 0xA0;
                work->unk4->unkC = 0x18;
            }
            ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk8, (s16 *)&work->unk20);
            break;
        }
        if (frame >= 10) {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk18);
            if (work->unk0 != NULL) {
                if (work->unk0->state == -1) {
                    work->unk0 = NULL;
                }
                if (work->unk0 != NULL) {
                    work->unk0->unk2C = (s32)&work->unk18;
                }
            }
        }
        if (frame >= 40) {
            if (work->unk4 != NULL) {
                if (work->unk4->state == -1) {
                    work->unk4 = NULL;
                }
                if (work->unk4 != NULL) {
                    work->unk4->unk5C.vx = work->unk20.vx;
                    work->unk4->unk5C.vy = work->unk20.vy;
                    work->unk4->unk5C.vz = work->unk20.vz;
                }
            }
        }
        return frame >= 100;
    }
    return 0;
}

s32 fx_04b_op81(s32 mode, Fx04bWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        s32 n = *arg4;

        work->unk14 = n;
        D_801E7B94 = env;
        if (n >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitPos(n, (s16 *)&work->unk4);
        work->unk4.vy -= 0xC8;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(work->unk14, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&D_801E7B9C, (s16 *)&work->unk4, (s16 *)&work->unkC);
    } else {
        frame = entry->frame;
        if (work->unk0 != NULL) {
            if (work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unkC.vx;
                work->unk0->unk5C.vy = work->unkC.vy;
                work->unk0->unk5C.vz = work->unkC.vz;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04b/nonmatchings/fx_04b", fx_04b_headerPtr);
