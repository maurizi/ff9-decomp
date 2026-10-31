#include "fxmod/fx_131.h"

s32 fx_131_op80(s32 mode, Fx131Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        fx_131_env = env;
        ex->btlfx_getUnitBonePos(0x10, 0xC, (u16 *)&work->pos8);
        work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
        if (work->emit18 != NULL) {
            work->emit18->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (work->emit18 != NULL && work->emit18->state == BTLFX_EMIT_DONE) {
            work->emit18 = NULL;
        }
        ex->btlfx_getUnitBonePos(0x10, 0xC, (u16 *)&work->unk10);
        ex->btlfx_moveEmitterTo((s32)work->emit18, (s32)&work->unk10);
        if (frame == 8) {
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos0);
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos0);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
        }
        return frame >= 0x58;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_131/nonmatchings/fx_131", fx_131_headerPtr);
