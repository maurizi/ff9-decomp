#include "fxmod/fx_01c.h"

s32 fx_01c_op80(s32 mode, Fx01cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x2C;
    } else if (mode == 1) {
        D_801E7B38 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk14);
        if (env->targets->targetCount == 1) {
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk1C);
        } else {
            work->unk1C.vx = env->targets->centerX;
            work->unk1C.vy = env->targets->centerY;
            work->unk1C.vz = env->targets->centerZ;
        }
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk14);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk14);
        if (work->unk0 != NULL) {
            work->unk0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&work->unk14, (s16 *)&work->unk1C, (s16 *)&work->unk24);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 30:
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk14);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk14);
            if (work->unkC != NULL) {
                work->unkC->unk14 = (s32)D_801E7B48;
                work->unkC->unk12 = 8;
                work->unkC->unkC = 0x90;
            }
            break;
        case 50:
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk1C);
            if (work->unk10 != NULL) {
                work->unk10->otZ = 0x80;
            }
            break;
        }
        if (work->unk0 != NULL) {
            if (work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->rot.vx = work->unk24.vx;
                work->unk0->rot.vy = work->unk24.vy;
                work->unk0->rot.vz = work->unk24.vz;
            }
        }
        if (frame >= 0x1E) {
            if (work->unk8 != NULL) {
                if (work->unk8->state == -1) {
                    work->unk8 = NULL;
                }
                if (work->unk8 != NULL) {
                    work->unk8->rot.vx = work->unk24.vx;
                    work->unk8->rot.vy = work->unk24.vy;
                    work->unk8->rot.vz = work->unk24.vz;
                }
            }
            if (work->unkC != NULL) {
                if (work->unkC->state == -1) {
                    work->unkC = NULL;
                }
                if (work->unkC != NULL) {
                    work->unkC->rot.vx = work->unk24.vx;
                    work->unkC->rot.vy = work->unk24.vy;
                    work->unkC->rot.vz = work->unk24.vz;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01c/nonmatchings/fx_01c", fx_01c_headerPtr);
