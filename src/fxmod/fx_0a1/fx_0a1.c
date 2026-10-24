#include "fxmod/fx_0a1.h"

s32 fx_0a1_op80(s32 mode, Fx0a1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E7C60 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk18);
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk18, (s16 *)&work->unk28);
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 10;
            ex->func_800DC858(0x10, 0, (u16 *)&work->unk20);
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk20);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0x10, 0x40);
                }
            }
            ex->btlfx_lerpVec((s16 *)&work->unk18, (s16 *)&work->unk8, 0x1000 - ex->math_cos(t >> 2),
                              (s16 *)&pos);
            if (frame == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0, NULL, -1, 0x1000, 0, 1, D_801E7C70, 0);
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&pos, NULL, 0, 0x1000, 0x1000, 1,
                              D_801E7C70, 0);
            if (frame >= 0xA) {
                work->unk0 = 1;
                entry->unk0 = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
            }
            break;
        case 1:
            if (frame == 7) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                    p->unk14 = (s32)D_801E7CA0;
                    p->unk12 = 0x2D;
                    p->unkC = 0x4C;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
            }
            if (frame == 0xC) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
            }
            if (frame == 0x11) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                    p->unk14 = (s32)D_801E8A00;
                    p->unk12 = 0x1E;
                    p->unkC = 0x4C;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk10);
            }
            if (frame >= 0x20) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a1/nonmatchings/fx_0a1", fx_0a1_headerPtr);
