#include "fxmod/fx_0d5.h"

s32 fx_0d5_op80(s32 mode, Fx0d5Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    Pool6CEntry *p;
    Pool6CEntry *q;
    s32 frame;
    s32 s;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7E30 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unkC);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (frame < 0x2C) {
                t = (frame << 12) / 44;
                if (frame < 0x17) {
                    s = ex->math_sin(frame << 6) / 4 + 0x800;
                } else {
                    s = ex->math_sin(((frame - 0x17) << 10) / 21) * 2 / 3 + 0xC00;
                }
                rot.vx = 0x400;
                rot.vz = 0;
                rot.vy = 0;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xD), &work->unk4, &rot, s * 2 / 3, 0xC, 1,
                                  ex->math_sin(t >> 1));
            }
            if (frame == 5) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame == 0x12) {
                q = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk4);
                if (q != NULL) {
                    q->unk22 = 0x80;
                    q->unk14 = (s32)D_801E7E40;
                    q->unk12 = 0x15;
                    q->unkC = 0xAC;
                }
                q = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unkC);
                if (q != NULL) {
                    q->unk22 = 0x80;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk4);
            }
            if (frame == 0x17) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame >= 0x32) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0d5/nonmatchings/fx_0d5", fx_0d5_op84);

INCLUDE_RODATA("asm/us/fx_0d5/nonmatchings/fx_0d5", fx_0d5_headerPtr);
