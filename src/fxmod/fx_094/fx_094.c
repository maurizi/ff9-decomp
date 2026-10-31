#include "fxmod/fx_094.h"

s32 fx_094_op80(s32 mode, Fx094Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        fx_094_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos0);
        ex->btlfx_aimAngles((s16 *)&work->pos0, (s16 *)&work->pos8, (s16 *)&work->aim18);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x10:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos10);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 0x18:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos8);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 0x30:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos8);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 0x40:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
            break;
        }
        if (work->emit20 != NULL) {
            if (work->emit20->state == BTLFX_EMIT_DONE) {
                work->emit20 = NULL;
            }
            if (work->emit20 != NULL) {
                work->emit20->rot.vx = work->aim18.vx;
                work->emit20->rot.vy = work->aim18.vy;
                work->emit20->rot.vz = work->aim18.vz;
            }
        }
        return frame >= 0x68;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_094/nonmatchings/fx_094", fx_094_headerPtr);
