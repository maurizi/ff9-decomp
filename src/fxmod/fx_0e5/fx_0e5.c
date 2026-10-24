#include "fxmod/fx_0e5.h"

s32 fx_0e5_op80(s32 mode, Fx0e5Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u16 n;

    if (mode == 0) {
        *arg4 = 0x44;
    } else if (mode == 1) {
        D_801E7A9C = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk20);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk18);
        work->unk38.vx = work->unk10.vx;
        work->unk38.vy = work->unk10.vy;
        work->unk38.vz = work->unk10.vz;
        ex->btlfx_pointAhead((s16 *)&work->unk8, (s16 *)&work->unk20, 0xC8, (s16 *)&work->unk30);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk30);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk28);
        work->unk42 = 0x1000;
        work->unk40 = 0;
    } else {
        frame = entry->frame;
        if (frame == 0xF) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk10);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (frame >= 0xF && frame < 0x1C) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->unk38, work->unk40, work->unk42);
            if (frame - 0xF >= 5) {
                work->unk42 -= 0x200;
            }
            work->unk40++;
            n = work->unk40;
            if (n >= 6) {
                n = 5;
            }
            work->unk40 = n;
        }
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk28.vx;
                work->unk0->unk5C.vy = work->unk28.vy;
                work->unk0->unk5C.vz = work->unk28.vz;
            }
        }
        if (frame >= 0xF) {
            if (work->unk4 != NULL) {
                if (work->unk4->unk30 == -1) {
                    work->unk4 = NULL;
                }
                if (work->unk4 != NULL) {
                    work->unk4->unk5C.vx = work->unk28.vx;
                    work->unk4->unk5C.vy = work->unk28.vy;
                    work->unk4->unk5C.vz = work->unk28.vz;
                }
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e5/nonmatchings/fx_0e5", fx_0e5_headerPtr);
