#include "fxmod/fx_1f4.h"

s32 fx_1f4_op80(s32 mode, Fx1f4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E79B8 = env;
        work->unk0 = 0;
        work->unk4 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk8);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x200);
            p->unk14 = (s32)D_801E79C8;
            p->unk12 = 0x1E;
            p->unkC = 0x9C;
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 16:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x200);
                p->unk14 = (s32)D_801E8C18;
                p->unk12 = 0x1E;
                p->unkC = 0x9C;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x200);
            }
            break;
        case 24:
            work->unk0 = 0x10;
            break;
        case 40:
            work->unk4 = 0x10;
            break;
        }
        if (work->unk0 != 0) {
            work->unk0--;
            ex->btlfx_fadeUnitById(0, work->unk0 * 8);
        }
        if (work->unk4 != 0) {
            work->unk4--;
            ex->btlfx_fadeUnitById(0, 0x80 - work->unk4 * 8);
        }
        return frame >= 0x56;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f4/nonmatchings/fx_1f4", fx_1f4_headerPtr);
