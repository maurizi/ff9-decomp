#include "fxmod/fx_0af.h"

s32 fx_0af_op80(s32 mode, Fx0afWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E798C = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk8);
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk18);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk8);
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E799C;
            work->unk4->unk12 = 0x1E;
            work->unk4->unkC = 0x4C;
        }
        if (work->unk4 != NULL) {
            work->unk4->otZ = 0x20;
        }
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            if (work->unk4 != NULL) {
                work->unk4->rot.vx = work->unk18.vx;
                work->unk4->rot.vy = work->unk18.vy;
                work->unk4->rot.vz = work->unk18.vz;
            }
            if (frame == 0x10) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk10);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E828C;
                    p->unk12 = 0x1E;
                    p->unkC = 0x4C;
                    p->otZ = 0x20;
                }
            }
            if (frame >= 0x3C) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0af/nonmatchings/fx_0af", fx_0af_headerPtr);
