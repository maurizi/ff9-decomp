#include "fxmod/fx_114.h"

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801E777C);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801E77B0);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801E77E4);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801E7818);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801E784C);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801E78AC);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", fx_114_op80);

s32 fx_114_op81(s32 mode, Fx114Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x48;
    } else if (mode == 1) {
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk20.vx = 0x1000;
        work->unk20.vy = 0x1000;
        work->unk20.vz = 0x1000;
        D_801EB768 = env;
        func_801EA808(D_801ED0E8, ex->btlfx_getFileData(entry->unk8, 0x89));
        work->unk46 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x85));
        work->unk44 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 1:
            ex->Hi_GetSummonBonePos(((Fx114Summon *)env->unk4)->unk60, 0x1B, (u16 *)&work->unk34);
            work->unk10.vx = work->unk34.vx;
            work->unk10.vy = work->unk34.vy;
            work->unk10.vz = work->unk34.vz;
            work->unk3C.vx = work->unk34.vx;
            work->unk3C.vy = work->unk34.vy;
            work->unk3C.vz = work->unk34.vz;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk34);
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk3C);
            work->unk44 = 0xC;
            break;
        case 0xD:
            ex->Hi_FreeEffModel(work->unk46);
            ex->func_800E3D54(work->unk0);
            ex->func_800E3D54(work->unk4);
            break;
        }
        if (frame >= 1 && frame < 0xD) {
            func_801EA81C(D_801ED0E8, frame - 1, &work->unk20);
            ex->Hi_DrawEffModel(&work->unk8, &work->unk10, (s32)&work->unk20, work->unk46);
            ex->Hi_ModifyEffModelAbr(work->unk46, 1);
            if (work->unk44 >= 0) {
                work->unk30 = c = func_801EA604(0, -0xFF, 0xC - work->unk44, 0xC);
                ex->Hi_ModifyEffModelRGB(work->unk46, c, c, c);
                work->unk44--;
            }
        }
        return frame >= 0x14;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_114/nonmatchings/fx_114", fx_114_headerPtr);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", fx_114_op82);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", fx_114_op83);

s32 fx_114_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 2;
    } else if (mode == 1) {
        D_801EB768 = env;
        *work = -1;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            *work = 0x10;
        }
        if (*work >= 0) {
            func_801EA914(0xFF, 0, 0x10 - *work, 0x10, 1, 0);
            (*work)--;
        }
        return frame >= 0x10;
    }
    return 0;
}

void func_801EA5C0(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801EB768)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801EA604(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA65C);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA670);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA6DC);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA6F4);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA758);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA76C);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA808);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA81C);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA880);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA898);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA8FC);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA914);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA9AC);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EA9F8);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EAA54);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EABB4);

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", func_801EB154);
