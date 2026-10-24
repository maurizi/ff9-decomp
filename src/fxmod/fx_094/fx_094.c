#include "fxmod/fx_094.h"

s32 fx_094_op80(s32 mode, Fx094Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E79AC = env;
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        work->unk20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk0);
        ex->btlfx_aimAngles((s16 *)&work->unk0, (s16 *)&work->unk8, (s16 *)&work->unk18);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x10:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk10);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 0x18:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 0x30:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 0x40:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
            break;
        }
        if (work->unk20 != NULL) {
            if (work->unk20->unk30 == -1) {
                work->unk20 = NULL;
            }
            if (work->unk20 != NULL) {
                work->unk20->unk5C.vx = work->unk18.vx;
                work->unk20->unk5C.vy = work->unk18.vy;
                work->unk20->unk5C.vz = work->unk18.vz;
            }
        }
        return frame >= 0x68;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_094/nonmatchings/fx_094", fx_094_headerPtr);
