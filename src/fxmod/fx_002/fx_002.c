#include "fxmod/fx_002.h"

s32 fx_002_op80(s32 mode, Fx002Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 s;
    SVECTOR v;
    s32 data;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7AF0 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unkC);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            if (frame < 0x2C) {
                t = (frame << 12) / 44;
                if (frame < 0x17) {
                    s = ex->math_sin(frame << 6) / 4 + 0x800;
                } else {
                    s = ex->math_sin(((frame - 0x17) << 10) / 21) * 2 / 3 + 0xC00;
                }
                v.vx = 0x400;
                v.vz = 0;
                v.vy = 0;
                v.pad = 1;
                data = ex->btlfx_getFileData(entry->unk8, 0xD);
                ex->func_800D23D8((u8 *)data, &work->unk4, &v, s * 2 / 3, 0xC, 1, ex->math_sin(t >> 1));
            }
            if (frame == 5) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame == 0x12) {
                Pool6CEntry *q;

                q = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk4);
                if (q != NULL) {
                    q->unk22 = 0x80;
                    q->unk14 = (s32)D_801E7B00;
                    q->unk12 = 0x15;
                    q->unkC = 0xAC;
                }
                q = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unkC);
                if (q != NULL) {
                    q->unk22 = 0x80;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk4);
            }
            if (frame == 0x17) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame >= 0x32) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_002/nonmatchings/fx_002", fx_002_headerPtr);
