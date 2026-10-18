#include "fxmod/fx_102.h"

s32 fx_102_op80(s32 mode, Fx102Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x70;
    } else if (mode == 1) {
        D_801E797C = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk28);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk28);
        for (i = 0; i < 8; i++) {
            (work->unk30 + i)->vx = work->unk28.vx;
            (work->unk30 + i)->vy = work->unk28.vy;
            (work->unk30 + i)->vz = work->unk28.vz;
            work->unk30[i].vx += ex->math_sin(i << 9) * 800 / 4096;
            work->unk30[i].vz += ex->math_cos(i << 9) * 800 / 4096;
            work->unk4[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801E7924[i]), (s32)&work->unk30[i]);
        }
    } else {
        frame = entry->unk0;
        if (frame == 8) {
            work->unk24 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk28);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_102/nonmatchings/fx_102", fx_102_headerPtr);
