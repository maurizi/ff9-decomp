#include "fxmod/fx_0e8.h"

s32 fx_0e8_op80(s32 mode, Fx0e8Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x44;
    } else if (mode == 1) {
        D_801E7A70 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk14);
        work->unk3C.vx = work->unk14.vx;
        work->unk3C.vy = work->unk14.vy;
        work->unk3C.vz = work->unk14.vz;
        work->unk3C.vy -= 0x12C;
        ex->btlfx_getUnitScale(0x10, &work->unk2C);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk3C);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            break;
        case 0xC:
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk14);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk14);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk14);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E7A80;
                work->unk4->unk12 = 0x14;
                work->unk4->unkC = 0xD0;
            }
            break;
        case 0x12:
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk14);
            if (work->unk10 != NULL) {
                work->unk10->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            break;
        }
        if (frame >= 0x12 && frame < 0x17) {
            i = frame - 0x12;
            if (i == 4) {
                scale.vx = 0;
                scale.vy = 0;
                scale.vz = 0;
                ex->btlfx_fadeUnitById(0x10, 0);
            } else {
                ex->btlfx_getUnitScale(0x10, &scale);
                scale.vx = work->unk2C.vx - work->unk2C.vx / 4 * i;
                scale.vy += 0x190;
                scale.vz = work->unk2C.vz - work->unk2C.vz / 4 * i;
            }
            ex->btlfx_setUnitScale(0x10, (s32 *)&scale);
        }
        return frame >= 0x2D;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e8/nonmatchings/fx_0e8", fx_0e8_headerPtr);
