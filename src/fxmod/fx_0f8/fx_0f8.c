#include "fxmod/fx_0f8.h"

s32 fx_0f8_op80(s32 mode, Fx0f8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E7BD0 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk4);
        work->unkC.vx = env->unkC->unk38;
        work->unkC.vy = env->unkC->unk3A;
        work->unkC.vz = env->unkC->unk3C;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk4);
        ex->btlfx_aimAngles((s16 *)&work->unk4, (s16 *)&work->unkC, (s16 *)&work->unk14);
        work->unk14.vy -= 0x400;
    } else {
        frame = entry->unk0;
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk14.vx;
                work->unk0->unk5C.vy = work->unk14.vy;
                work->unk0->unk5C.vz = work->unk14.vz;
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0f8/nonmatchings/fx_0f8", fx_0f8_op81);

INCLUDE_RODATA("asm/us/fx_0f8/nonmatchings/fx_0f8", fx_0f8_headerPtr);
