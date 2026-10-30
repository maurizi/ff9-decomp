#include "fxmod/fx_08d.h"

s32 fx_08d_op80(s32 mode, Fx08dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u16 c;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E79A0 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk14);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E79B0;
            work->unk0->unk12 = 3;
            work->unk0->unkC = 0xA0;
        }
        work->unk1C = work->unk1E = 0;
    } else {
        frame = entry->frame;
        if (frame == 0x12) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->unk14);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame == 0x14) {
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unkC);
        }
        if (frame >= 0x25 && frame < 0x31) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 7), &work->unkC, work->unk1C, work->unk1E);
            if (frame - 0x25 < 6) {
                work->unk1E += 0x333;
            } else if (work->unk1E >= 0x249) {
                work->unk1E -= 0x249;
            } else {
                work->unk1E = 0;
            }
            c = ++work->unk1C;
            if (c > 5) {
                c = 5;
            }
            work->unk1C = c;
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_08d/nonmatchings/fx_08d", fx_08d_headerPtr);
