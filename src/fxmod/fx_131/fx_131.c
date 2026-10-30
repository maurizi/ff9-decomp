#include "fxmod/fx_131.h"

s32 fx_131_op80(s32 mode, Fx131Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E78EC = env;
        ex->btlfx_getUnitBonePos(0x10, 0xC, (u16 *)&work->unk8);
        work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk8);
        if (work->unk18 != NULL) {
            work->unk18->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (work->unk18 != NULL && work->unk18->state == -1) {
            work->unk18 = NULL;
        }
        ex->btlfx_getUnitBonePos(0x10, 0xC, (u16 *)&work->unk10);
        ex->btlfx_moveEmitterTo((s32)work->unk18, (s32)&work->unk10);
        if (frame == 8) {
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk0);
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk0);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
        }
        return frame >= 0x58;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_131/nonmatchings/fx_131", fx_131_headerPtr);
