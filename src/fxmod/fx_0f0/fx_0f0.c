#include "fxmod/fx_0f0.h"

s32 fx_0f0_op80(s32 mode, Fx0f0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x2C;
    } else if (mode == 1) {
        D_801E7C20 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk1C);
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk1C);
            break;
        case 0x26:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk24);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk24);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk24);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk24);
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk24);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->unk10 != NULL) {
                work->unk10->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E7C30;
                work->unk4->unk12 = 0x1C;
                work->unk4->unkC = 0x9C;
            }
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E8D40;
                work->unk8->unk12 = 0x1C;
                work->unk8->unkC = 0x9C;
            }
            if (work->unk10 != NULL) {
                work->unk10->unk14 = (s32)D_801E9E50;
                work->unk10->unk12 = 0x3A;
                work->unk10->unkC = 0x70;
            }
            break;
        }
        if (frame >= 0x26) {
            if (work->unk10 != NULL) {
                if (work->unk10->state == -1) {
                    work->unk10 = NULL;
                }
                if (work->unk10 != NULL) {
                    work->unk10->unk2C = (s32)&work->unk14;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_0f0_op81(s32 mode, Fx0f0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E7C20 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unkC);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unkC);
        work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->unk8 != NULL) {
            work->unk8->unk22 = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 4) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->unkC);
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f0/nonmatchings/fx_0f0", fx_0f0_headerPtr);
