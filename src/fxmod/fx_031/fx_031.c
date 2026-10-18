#include "fxmod/fx_031.h"

s32 fx_031_op80(s32 mode, Fx031Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        D_801E7AE8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        work->unk8.vy += 0x80;
        ex->func_800DC8AC(0x10, env->unkC->unk25[0], (BtlBone *)&m1);
        ex->func_800CFE88(&m1, (s16)env->unkC->unk42, (s32)&work->unk10);
        work->unk30 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
        ex->func_800DC8AC(0x10, env->unkC->unk25[1], (BtlBone *)&m2);
        ex->func_800CFE88(&m2, (s16)env->unkC->unk42, (s32)&work->unk20);
        work->unk34 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk20);
        ex->func_800DC8AC(0x10, env->unkC->unk25[1], (BtlBone *)&m3);
        ex->func_800CFE88(&m3, (s16)env->unkC->unk42, (s32)env->unk2C);
    } else {
        frame = entry->unk0;
        ex->func_800DC8AC(0x10, env->unkC->unk25[0], (BtlBone *)&m1);
        ex->func_800CFE88(&m1, (s16)env->unkC->unk42, (s32)&work->unk18);
        if (work->unk30 != NULL && work->unk30->unk30 == -1) {
            work->unk30 = NULL;
        }
        ex->func_800E3D20((s32)work->unk30, (s32)&work->unk18);
        ex->func_800DC8AC(0x10, env->unkC->unk25[1], (BtlBone *)&m2);
        ex->func_800CFE88(&m2, (s16)env->unkC->unk42, (s32)&work->unk28);
        if (work->unk34 != NULL && work->unk34->unk30 == -1) {
            work->unk34 = NULL;
        }
        ex->func_800E3D20((s32)work->unk34, (s32)&work->unk28);
        if (frame < 8) {
            s32 c = (8 - frame) * 0x10;

            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c, c);
        }
        if (frame == 8) {
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk8);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk0);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk0);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_031/nonmatchings/fx_031", fx_031_headerPtr);
