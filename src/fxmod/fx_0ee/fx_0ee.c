#include "fxmod/fx_0ee.h"

s32 fx_0ee_op80(s32 mode, Fx0eeWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E792C = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk8);
        work->unk0.vx += ex->math_sin(work->unk8.vy) >> 4;
        work->unk0.vz += ex->math_cos(work->unk8.vy) >> 4;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk0);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk0);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 8:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk0);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0x10:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk0);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        }
        return frame >= 0x44;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ee/nonmatchings/fx_0ee", fx_0ee_headerPtr);
