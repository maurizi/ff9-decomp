#include "fxmod/fx_01e.h"

s32 fx_01e_op80(s32 mode, Fx01eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E791C = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E792C;
            work->unk4->unk12 = 0x64;
            work->unk4->unkC = 0x1C;
        }
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk18);
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk18.vx;
                work->unk4->unk5C.vy = work->unk18.vy;
                work->unk4->unk5C.vz = work->unk18.vz;
            }
            if (frame == 8) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0xC);
                }
            }
            if (frame >= 0x3C) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01e/nonmatchings/fx_01e", fx_01e_headerPtr);
