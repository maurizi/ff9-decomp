#include "fxmod/fx_11c.h"

s32 fx_11c_op80(s32 mode, Fx11cWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E7F9C = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        work->unk10.vx = work->unk8.vx;
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz;
        work->unk10.vy += 0x12C;
        work->unk10.vz -= 0x12C;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk10);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7FAC;
            work->unk0->unk12 = 0xD;
            work->unk0->unkC = 0x90;
        }
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E86FC;
            work->unk4->unk12 = 0xD;
            work->unk4->unkC = 0x90;
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_11c/nonmatchings/fx_11c", fx_11c_op81);

s32 fx_11c_op82(s32 mode, Fx11cWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        work->unk18.vx = env->unkC->unk38;
        work->unk18.vy = env->unkC->unk3A;
        work->unk18.vz = env->unkC->unk3C;
        work->unk20.vx = env->unkC->unk38;
        work->unk20.vy = env->unkC->unk3A;
        work->unk20.vz = env->unkC->unk3C;
        work->unk28.vx = env->unkC->unk38;
        work->unk28.vy = env->unkC->unk3A;
        work->unk28.vz = env->unkC->unk3C;
        work->unk20.vx -= 0x190;
        work->unk28.vx += 0x190;
        D_801E7F9C = env;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk18);
        work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk18);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E8E4C;
            work->unk0->unk12 = 8;
            work->unk0->unkC = 0x90;
        }
    } else {
        frame = entry->unk0;
        if (frame == 5) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk18);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk18);
        }
        if (frame == 0xD) {
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk20);
            work->unk14 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk28);
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_11c/nonmatchings/fx_11c", fx_11c_headerPtr);
