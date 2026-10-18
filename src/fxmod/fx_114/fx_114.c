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

s32 fx_114_op82(s32 mode, Fx114Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    CVECTOR col;
    DR_MODE *dr;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        work->unk4.vx = 0;
        work->unk4.vy = 0;
        work->unk4.vz = 0;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0x320;
        work->unk1C.vx = 0x1000;
        work->unk1C.vy = 0x1000;
        work->unk1C.vz = 0x1000;
        D_801EB768 = env;
        work->unk66 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x2A), 0x117, 0, 0, 0);
        ex->Hi_SetEffModelOffset(work->unk66, 0x6E);
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0x320;
        work->unk44.vx = 0x1000;
        work->unk44.vy = 0x1000;
        work->unk44.vz = 0x1000;
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0x17, 0x3D00, 0x80, 0xE0);
        D_801ED078.unk10 = 0x60;
        D_801ED078.unk8 = 0;
        D_801ED078.unkA = 0;
        D_801ED078.unkC = 0x1000;
        D_801ED078.unkE = 0x1000;
        D_801ED078.unk12 = 0x30;
        work->unk60 = -1;
        work->unk62 = -1;
        work->unk64 = 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            ex->func_800E6E1C(0x1C0, 0x100, 0x80, 0xA0, D_801ED1C8, 0, 0x168, 0xF6E);
            break;
        case 9:
            work->unk58.vx = -3;
            work->unk58.vy = 0;
            work->unk58.vz = 0x320;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk58);
            break;
        case 8:
            work->unk60 = 4;
            break;
        case 2:
            work->unk62 = 6;
            break;
        case 29:
            ex->Hi_FreeEffModel(work->unk66);
            break;
        case 18:
            ex->Hi_FreeEffModel(work->unk68);
            break;
        }
        if ((u32)frame < 0x1D) {
            ex->Hi_DrawEffModel(&work->unk4, &work->unkC, (s32)&work->unk1C, work->unk66);
            ex->Hi_ModifyEffModelRGB(work->unk66, 6, 6, 6);
        }
        if ((u32)(frame - 8) < 10) {
            D_801ED078.unkA = (frame - 8) * 2;
            D_801ED078.unkA = D_801ED078.unkA >= 10 ? 9 : D_801ED078.unkA;
            if (work->unk60 >= 0) {
                D_801ED078.unkC = func_801EA604(0, 0x1000, 4 - work->unk60, 4);
                work->unk60--;
            }
            if (work->unk62 >= 0) {
                D_801ED078.unkC = func_801EA604(0x1000, 0, 6 - work->unk62, 6);
                work->unk62--;
            }
            for (i = 0, buf = D_801EE658[0]; i < 13; i++) {
                func_801EA6DC(&D_801ED078, func_801EA8FC(ex->btlfx_getFileData(entry->unk8, 0x17), (u16)i));
                func_801E78AC(&D_801ED078, buf, 0x50, D_801ED078.unk8, D_801ED078.unkA, D_801ED078.unk10, D_801ED078.unk12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, NULL, 0, D_801ED078.unkE, D_801ED078.unkC, 1,
                                    buf, 0);
                buf += 0x50;
            }
        }
        if ((u32)(t = frame - 0xE) < 4) {
            dr = (DR_MODE *)env->unk24;
            env->unk24 = (u8 *)(dr + 1);
            setlen(dr, 2);
            dr->code[0] = 0xE6000001;
            dr->code[1] = 0;
            addPrim(&env->unk20[1], dr);
            sc.vz = sc.vy = sc.vx = (t << 7) + 0x1000;
            col.r = col.g = col.b = 0x3C;
            rot.vz = rot.vy = rot.vx = 0;
            ex->func_800E6764(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &sc, &col, 1, NULL);
            env->unk2C->unk5E = 0x208 - t * t * 4;
        }
        work->unk64++;
        return frame >= 0x1E;
    }
    return 0;
}

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
