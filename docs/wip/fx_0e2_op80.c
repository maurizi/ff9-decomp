/* Draft of fx_0e2 op80 (Leviathan-style sequence: summon model, morph pieces, ribbons, drops and pool effects).
 * Status: first draft, ~630 aligned mismatches with registers, ~270 without, frame 0x68 against 0x60 (two extra
 * spills). Structure that already matches: the three frame switches (the second needs an extra empty case above 0xF8,
 * here case 0x10E, for the original's bound test), the B8 block's condition with the drops range negated (jump
 * threading then reproduces the original's early test), per-case pool pointers p2/p3, n = frame - 0x2E placed after
 * the copy block, mode 1's drop loop as a pointer loop on its own variable. Open: the drops loop (orig: inner goto
 * style loop without d givs, outer givs based at dr + 0x28, 0x16 + 2j reduced), the trail loop's a/b/c spills, and
 * loop counter sharing (i0 = mode 1 loops, i2 = the three case loops). */
#include "fxmod/fx_0e2.h"

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180[8]; /* summon motions */
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0e2Summon;

typedef s32 (*Fx0e2RegTexList)(BtlUnit9C8 *, BtlTexInfo *, s16 *);

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8[5];
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
    Pool6CEntry *unk24;
    Pool6CEntry *unk28[5];
    SVECTOR unk3C; /* summon rotation */
    VECTOR unk44; /* summon position */
    VECTOR unk54; /* model position */
    VECTOR unk64; /* scale */
    s32 unk74;
    s32 unk78;
    SVECTOR unk7C;
    VECTOR unk84;
    VECTOR unk94;
    VECTOR unkA4;
    s32 unkB4;
    SVECTOR unkB8;
    VECTOR unkC0;
    VECTOR unkD0;
    SVECTOR unkE0;
    VECTOR unkE8;
    VECTOR unkF8;
    u8 pad108[8];
    SVECTOR unk110;
    SVECTOR unk118;
    SVECTOR unk120;
    s32 unk128;
    s32 unk12C;
    s32 unk130;
    s32 unk134;
    s32 unk138;
    s32 unk13C;
    s32 unk140;
    s32 unk144;
    s32 unk148;
    s16 unk14C;
    s16 unk14E;
    u16 unk150[14]; /* models */
} Fx0e2Work80; /* 0x16C */

typedef struct {
    Fx0e2Path unk0;
    Fx0e2Path unk8;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
} Fx0e2Ribbon80;

typedef struct {
    Fx0e2Path unk0;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
} Fx0e2Path80;

typedef struct {
    s32 unk0[2][4]; /* positions */
    s32 unk20[3]; /* start position */
    u8 pad2C[4];
    s16 unk30[3]; /* velocity */
    s16 pad36;
} Fx0e2Drop; /* 0x38 */

typedef struct {
    u8 data[0x98];
} Fx0e2Trail98;

typedef struct {
    u8 data[0x1E0];
} Fx0e2Tex1E0;

void func_801EC508(Fx0e2Path *path, u16 *data);
s32 func_801EC4A4(Fx0e2Path *path, s32 t, SVECTOR *out);
extern Fx0e2Drop D_801ECAC0[3];
extern u16 D_801ECB7C[3];
extern u16 D_801ECB84[4];
extern u16 D_801ECB8C[5];
extern u16 D_801ECB98[5];
extern BtlTexInfo D_801ECBA4[];
extern s16 D_801ECBB4[];
extern BtlModel D_801ECC1C[];
extern Fx0e2Ribbon80 D_801EE51C;
extern Fx0e2Path80 D_801EE53C;
extern Fx0e2Path80 D_801EE55C;
extern Fx0e2Trail98 D_801EEDCC[5];
extern Fx0e2Tex1E0 D_801EF0CC[5];
extern u8 D_801EFA2C[];
extern SVECTOR D_801F100C[5];
extern SVECTOR D_801F105C;
extern SVECTOR D_801F1064;
extern SVECTOR D_801F106C;
extern SVECTOR D_801F107C[5];
extern SVECTOR D_801F10AC[5];

s32 fx_0e2_op80(s32 mode, Fx0e2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    Pool6CEntry *p2;
    Pool6CEntry *p3;
    s32 n;
    s32 m;
    Fx0e2Drop *dr;
    Fx0e2Drop *dm;
    s32 *d;
    s32 frame;
    s32 i;
    s32 i0;
    s32 i2;
    s32 i5;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 j;
    s32 k;
    s32 a;
    s32 b;
    s32 t;
    s32 c;
    s32 f;

    if (mode == 0) {
        *arg4 = 0x16C;
    } else if (mode == 1) {
        D_801ECC0C = env;
        ex->btlfx_initEffModels(D_801ECC1C, 0x20);
        D_801F1004.vx = env->unkC->unk38;
        D_801F1004.vy = env->unkC->unk3A;
        D_801F1004.vz = env->unkC->unk3C;
        work->unk3C.vx = 0;
        work->unk3C.vy = 0x800;
        work->unk3C.vz = 0;
        work->unk44.vx = 0;
        work->unk44.vy = 0;
        work->unk44.vz = 0;
        work->unk54.vx = 0;
        work->unk54.vy = 0;
        work->unk54.vz = 0;
        work->unk64.vx = 0x800;
        work->unk64.vy = 0x800;
        work->unk64.vz = 0x800;
        work->unk130 = 0;
        work->unk134 = 0;
        work->unk150[0] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x84), D_801ECBA4, D_801ECBB4);
        work->unk150[1] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x85), D_801ECBA4, D_801ECBB4);
        work->unk150[2] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x83), D_801ECBA4, D_801ECBB4);
        work->unk150[3] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x82), D_801ECBA4, D_801ECBB4);
        work->unk138 = 0;
        work->unk13C = 0;
        work->unk140 = -1;
        work->unk144 = -1;
        work->unk148 = -1;
        work->unk7C.vx = 0;
        work->unk7C.vy = 0;
        work->unk7C.vz = 0;
        work->unk84.vx = 0;
        work->unk84.vy = 0;
        work->unk84.vz = 0;
        work->unk94.vx = 0x1000;
        work->unk94.vy = 0x1000;
        work->unk94.vz = 0x1000;
        work->unkA4.vx = 0x1000;
        work->unkA4.vy = 0x1000;
        work->unkA4.vz = 0x1000;
        work->unk150[4] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x95), 1);
        work->unk150[5] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x94), 1);
        work->unk150[6] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x21), 1);
        work->unk150[7] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 1);
        D_801EE51C.unk10 = 0;
        D_801EE51C.unk12 = 0x1000;
        D_801EE51C.unk14 = 0x1000;
        D_801EE51C.unk16 = 0x40;
        D_801EE53C.unk8 = 0;
        D_801EE53C.unkA = 0x11;
        D_801EE53C.unkC = 0x1000;
        D_801EE53C.unkE = 0x1000;
        D_801EE53C.unk10 = 0x40;
        D_801EE55C.unk8 = 0;
        D_801EE55C.unkA = 1;
        D_801EE55C.unkC = 0x1000;
        D_801EE55C.unkE = 0x1000;
        D_801EE55C.unk10 = 0x40;
        work->unkB8.vx = 0;
        work->unkB8.vy = 0;
        work->unkB8.vz = 0;
        work->unkC0.vx = 0;
        work->unkC0.vy = 0;
        work->unkC0.vz = 0;
        work->unkD0.vx = 0x800;
        work->unkD0.vy = 0x800;
        work->unkD0.vz = 0x800;
        work->unk150[8] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x81), 1);
        work->unk150[9] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x87), 1);
        work->unk150[10] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x93), 1);
        work->unkE0.vx = 0;
        work->unkE0.vy = 0;
        work->unkE0.vz = 0;
        work->unkE8.vx = 0;
        work->unkE8.vy = 0;
        work->unkE8.vz = 0;
        work->unkF8.vx = 0x1000;
        work->unkF8.vy = 0x1000;
        work->unkF8.vz = 0x1000;
        for (i0 = 0, dm = D_801ECAC0; i0 < 3; i0++, dm++) {
            dm->unk20[2] = dm->unk30[2] + 0x4E20;
        }
        for (i0 = 0; i0 < 3; i0++) {
            work->unk150[i0 + 11] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801ECB7C[i0]), 1);
        }
        work->unk14C = 0;
        work->unk14E = 4;
        func_801EC718();
        func_801EC764(0, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0));
        func_801EC764(0, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0));
        func_801EC764(0, 3, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9B), 0));
        func_801EC764(0, 4, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9B), 1));
        func_801EC764(0, 9, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9B), 2));
        func_801EC764(0, 0xA, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9B), 3));
        func_801EC764(0x2D, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 1));
        func_801EC764(0x2D, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 1));
        func_801EC764(0x2D, 3, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9F), 1));
        func_801EC764(0x2D, 4, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x96), 1));
        func_801EC764(0x2D, 9, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x92), 1));
        func_801EC764(0x41, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 2));
        func_801EC764(0x41, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 2));
        func_801EC764(0x41, 3, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9F), 2));
        func_801EC764(0x41, 4, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x96), 2));
        func_801EC764(0x41, 9, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x92), 2));
        func_801EC764(0x96, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 4));
        func_801EC764(0x96, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 4));
        func_801EC764(0x96, 3, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9F), 4));
        func_801EC764(0x96, 4, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x96), 4));
        func_801EC764(0x96, 9, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x92), 4));
        func_801EC764(0xEF, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 5));
        func_801EC764(0xEF, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 5));
        func_801EC764(0xEF, 3, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9F), 5));
        func_801EC764(0xEF, 4, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x96), 5));
        func_801EC764(0xEF, 9, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x92), 5));
        func_801EC764(0x10E, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 6));
        func_801EC764(0x10E, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 6));
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x2E:
            work->unk128 = 0;
            work->unk12C = 0;
            ex->Hi_SetSummonMotion(((Fx0e2Summon *)env->unk4)->unk54->unk180[0], ((Fx0e2Summon *)env->unk4)->unk60);
            break;
        case 0x97:
            work->unk3C.vx = 0x181;
            work->unk128 = 1;
            work->unk12C = 0;
            ex->Hi_SetSummonMotion(((Fx0e2Summon *)env->unk4)->unk54->unk180[1], ((Fx0e2Summon *)env->unk4)->unk60);
            break;
        }
        switch (frame) {
        case 0x2E:
            work->unk0 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA6), (s32)&D_801F105C);
            if (p != NULL) {
                p->unk22 = -0x100;
            }
            break;
        case 0x41:
            work->unk4 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA5), (s32)&D_801F105C);
            if (p != NULL) {
                p->unk22 = -0x100;
            }
            break;
        case 0x42:
            ex->func_800E3D54(work->unk0);
            break;
        case 0x9B:
            ex->func_800E3D54(work->unk4);
            break;
        case 0xA3:
            work->unk1C = p2 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA4), (s32)&D_801F106C);
            if (p2 != NULL) {
                p2->unk14 = (s32)D_801EFA2C;
                work->unk1C->unk12 = 0xC;
                work->unk1C->unkC = 0x50;
                if (work->unk1C != NULL) {
                    work->unk1C->unk22 = 0x100;
                }
            }
            break;
        case 0xB3:
            work->unk20 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA3), (s32)&D_801F1064);
            work->unk24 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA2), (s32)&D_801F1064);
            break;
        case 0xB5:
            for (i2 = 0; i2 < 5; i2++) {
                work->unk28[i2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801ECB98[i2]), (s32)&D_801F10AC[i2]);
            }
            break;
        case 0xF0:
            for (i2 = 0; i2 < 5; i2++) {
                ex->func_800E3D54(work->unk28[i2]);
            }
            break;
        case 0xF8:
            for (i2 = 0; i2 < 5; i2++) {
                work->unk8[i2] = p3 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801ECB8C[i2]), (s32)&D_801F107C[i2]);
                if (p3 != NULL) {
                    p3->unk14 = (s32)&D_801EF0CC[i2];
                    work->unk8[i2]->unk12 = 6;
                    work->unk8[i2]->unkC = 0x50;
                }
            }
            break;
        case 0xA1:
            work->unk140 = 0x10;
            work->unk144 = 0x10;
            break;
        case 0xB1:
            work->unk148 = 0xC;
            break;
        case 0x10E:
            break;
        }
        switch (frame) {
        case 0x97:
            ex->Hi_FreeEffModel(work->unk150[8]);
            ex->Hi_FreeEffModel(work->unk150[9]);
            ex->Hi_FreeEffModel(work->unk150[10]);
            break;
        case 0xBD:
            ex->Hi_FreeEffModel(work->unk150[0]);
            ex->Hi_FreeEffModel(work->unk150[1]);
            ex->Hi_FreeEffModel(work->unk150[2]);
            ex->Hi_FreeEffModel(work->unk150[3]);
            break;
        case 0xF0:
            ex->Hi_FreeEffModel(work->unk150[11]);
            ex->Hi_FreeEffModel(work->unk150[12]);
            ex->Hi_FreeEffModel(work->unk150[13]);
            break;
        }
        if (frame < 0x10E) {
            work->unk44.vx = env->unk2C->unk0[2].vx;
            work->unk44.vy = env->unk2C->unk0[2].vy;
            work->unk44.vz = env->unk2C->unk0[2].vz;
            work->unk54.vx = env->unk2C->unk0[8].vx;
            work->unk54.vy = env->unk2C->unk0[8].vy;
            work->unk54.vz = env->unk2C->unk0[8].vz;
            D_801F0FEC.vx = work->unk54.vx;
            D_801F0FEC.vy = work->unk54.vy;
            D_801F0FEC.vz = work->unk54.vz;
            work->unkC0.vx = env->unk2C->unk0[3].vx;
            work->unkC0.vy = env->unk2C->unk0[3].vy;
            work->unkC0.vz = env->unk2C->unk0[3].vz;
        }
        n = frame - 0x2E;
        if (n >= 0 && n < 0x83) {
            ex->Hi_DrawSummonModel(&work->unk3C, &work->unk44, (s32)&work->unk64, ((Fx0e2Summon *)env->unk4)->unk60,
                                   work->unk12C);
            if (n >= 0 && n < 0x14) {
                ex->Hi_ModifySummonModelRGB(((Fx0e2Summon *)env->unk4)->unk60, -0x20, -0x20, -0x20);
            }
            if (n >= 0x69) {
                ex->Hi_ModifySummonModelAbr(((Fx0e2Summon *)env->unk4)->unk60, 1);
            }
            if (work->unk140 >= 0) {
                work->unk74 = func_801EC434(0, -0x80, 0x10 - work->unk140, 0x10);
                work->unk140--;
                ex->Hi_ModifySummonModelRGB(((Fx0e2Summon *)env->unk4)->unk60, work->unk74, work->unk74, work->unk74);
            }
        }
        if (work->unk144 >= 0) {
            work->unk78 = func_801EC434(-0x80, 0, 0x10 - work->unk144, 0x10);
            work->unk144--;
        }
        if (work->unk148 >= 0) {
            work->unk78 = func_801EC434(0, -0x80, 0xC - work->unk148, 0xC);
            work->unk148--;
        }
        if (frame >= 0xA1 && frame < 0xBD) {
            if (frame - 0xA1 >= 0x14) {
                t = 0;
            } else if (frame - 0xA1 >= 8) {
                t = 0x1000 - (work->unk138++ << 12) / 12;
            } else {
                t = 0x1000;
            }
            ex->Hi_DrawMorphEffModel(&work->unk3C, (VECTOR *)&work->unk54, (s32)&work->unk64, work->unk150[0],
                                     work->unk150[1], t);
            ex->Hi_ModifyEffModelAbr(work->unk150[0], 1);
            if (work->unk148 >= 0 || work->unk144 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->unk150[0], work->unk78, work->unk78, work->unk78);
            }
        }
        if (frame >= 0xA1 && frame < 0xBD) {
            if (frame - 0xA1 >= 0x18) {
                t = 0;
            } else if (frame - 0xA1 >= 0xC) {
                t = 0x1000 - (work->unk13C++ << 12) / 12;
            } else {
                t = 0x1000;
            }
            ex->Hi_DrawMorphEffModel(&work->unk3C, (VECTOR *)&work->unk54, (s32)&work->unk64, work->unk150[2],
                                     work->unk150[3], t);
            ex->Hi_ModifyEffModelAbr(work->unk150[2], 1);
            if (work->unk148 >= 0 || work->unk144 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->unk150[2], work->unk78, work->unk78, work->unk78);
            }
        }
        if (frame >= 0xB1 && frame < 0xCD) {
            f = frame - 0xB1;
            a = 0;
            if (f >= 8) {
                a = (frame - 0xB9) / 2;
                b = a + 1;
                c = ((frame - 0xB9) & 1) << 11;
            } else {
                b = 0;
                c = 0;
            }
            a = a < 4 ? a : 3;
            b = b < 4 ? b : 3;
            D_801EE51C.unk12 = f < 0x10 ? f << 8 : 0x1000;
            for (i5 = 0; i5 < 5; i5++) {
                func_801EC508(&D_801EE51C.unk0,
                              (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, D_801ECB84[a]), (u16)i5));
                func_801EC508(&D_801EE51C.unk8,
                              (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, D_801ECB84[b]), (u16)i5));
                func_801E77C0(&D_801EE51C.unk0, &D_801EE51C.unk8, c, (Fx0e2Trail *)&D_801EEDCC[i5], 0x98, D_801EE51C.unk16);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x17), 0, NULL, 0, D_801EE51C.unk14,
                                    D_801EE51C.unk12, 1, (u8 *)&D_801EEDCC[i5], 0);
                D_801F0FFC.vx = 0x113;
                D_801F0FFC.vy = 0x800;
                D_801F0FFC.vz = 0;
                work->unk84.vx = D_801F0FF4.vx;
                work->unk84.vy = D_801F0FF4.vy;
                work->unk84.vz = D_801F0FF4.vz;
                D_801F100C[i5].vx = D_801F0FF4.vx;
                D_801F100C[i5].vy = D_801F0FF4.vy;
                D_801F100C[i5].vz = D_801F0FF4.vz;
                work->unk7C.vx = D_801F0FFC.vx;
                work->unk7C.vy = D_801F0FFC.vy;
                work->unk7C.vz = D_801F0FFC.vz;
                work->unk94.vx = work->unk94.vy = work->unk94.vz = f * 0xD7;
                if (f < 0xC) {
                    t = 0x1000;
                } else if (f >= 0x14) {
                    t = 0;
                } else {
                    t = 0x1000 - ((f - 0xC) << 9);
                }
                ex->Hi_DrawMorphEffModel(&work->unk7C, &work->unk84, (s32)&work->unk94, work->unk150[4], work->unk150[5],
                                         t);
                work->unkB4 = -((0x13 - f) * 6);
                ex->Hi_ModifyEffModelRGB(work->unk150[4], work->unkB4, work->unkB4, work->unkB4);
                if (frame >= 0xB5) {
                    if (work->unk28[i5] != NULL && work->unk28[i5]->unk30 == -1) {
                        work->unk28[i5] = NULL;
                    }
                    ex->func_800E3D20((s32)work->unk28[i5], (s32)&D_801F100C[i5]);
                }
            }
        }
        if (frame >= 0xCD && frame < 0xF0) {
            k = frame - 0xCD;
            D_801EE53C.unk8 = k;
            if (D_801EE53C.unk8 >= 0x23) {
                D_801EE53C.unk8 = 0x22;
            }
            for (i6 = 0; i6 < 5; i6++) {
                func_801EC48C(&D_801EE53C.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9E), (u16)i6));
                func_801EC4A4(&D_801EE53C.unk0, k + 0x37, &D_801F0FF4);
                D_801F0FFC.vx = 0x113;
                D_801F0FFC.vy = 0x800;
                D_801F0FFC.vz = 0;
                work->unk84.vx = D_801F0FF4.vx;
                work->unk84.vy = D_801F0FF4.vy;
                work->unk84.vz = D_801F0FF4.vz;
                D_801F100C[i6].vx = D_801F0FF4.vx;
                D_801F100C[i6].vy = D_801F0FF4.vy;
                D_801F100C[i6].vz = D_801F0FF4.vz;
                work->unk7C.vx = D_801F0FFC.vx;
                work->unk7C.vy = D_801F0FFC.vy;
                work->unk7C.vz = D_801F0FFC.vz;
                ex->Hi_DrawMorphEffModel(&work->unk7C, &work->unk84, (s32)&work->unk94, work->unk150[4], work->unk150[5],
                                         0);
                if (work->unk28[i6] != NULL && work->unk28[i6]->unk30 == -1) {
                    work->unk28[i6] = NULL;
                }
                ex->func_800E3D20((s32)work->unk28[i6], (s32)&D_801F100C[i6]);
            }
        }
        if (frame >= 0xF0 && frame < 0x10F) {
            D_801EE55C.unkA = frame - 0xF0;
            if (D_801EE55C.unkA >= 0x12) {
                D_801EE55C.unk8 = frame - 0x101;
                D_801EE55C.unkA = 0x11;
            }
            for (i7 = 0; i7 < 5; i7++) {
                func_801EC48C(&D_801EE55C.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9D), (u16)i7));
                func_801E7BE8(&D_801EE55C.unk0, (Fx0e2Trail *)&D_801EEDCC[i7], 0x98, D_801EE55C.unk8, D_801EE55C.unkA,
                              D_801EE55C.unk10);
                D_801F0FFC.vx = 0x113;
                D_801F0FFC.vy = 0x800;
                D_801F0FFC.vz = 0;
                work->unk84.vx = D_801F0FF4.vx;
                work->unk84.vy = D_801F0FF4.vy;
                work->unk84.vz = D_801F0FF4.vz;
                D_801F100C[i7].vx = D_801F0FF4.vx;
                D_801F100C[i7].vy = D_801F0FF4.vy;
                D_801F100C[i7].vz = D_801F0FF4.vz;
                work->unk7C.vx = D_801F0FFC.vx;
                work->unk7C.vy = D_801F0FFC.vy;
                work->unk7C.vz = D_801F0FFC.vz;
                ex->Hi_DrawEffModel(&work->unk7C, &work->unk84, (s32)&work->unk94, work->unk150[6]);
                ex->Hi_DrawEffModel(&work->unk7C, &work->unk84, (s32)&work->unk94, work->unk150[7]);
                func_801EC920(work->unk14E * 0x14A - work->unk14C * work->unk14E, 0x1C0, 0x180, 0x80, 0x40, 0x1C0, 0x1C0);
                work->unk14C++;
                if (frame >= 0xF8) {
                    if (work->unk8[i7] != NULL && work->unk8[i7]->unk30 == -1) {
                        work->unk8[i7] = NULL;
                    }
                    ex->func_800E3D20((s32)work->unk8[i7], (s32)&D_801F100C[i7]);
                }
            }
        }
        m = frame - 0x2E;
        if (m >= 0 && m < 0x69 && !(frame >= 0x97 && frame < 0xF0)) {
            if (m < 0x14) {
                ex->Hi_DrawEffModel(&work->unkB8, &work->unkC0, (s32)&work->unkD0, work->unk150[8]);
                ex->Hi_DrawEffModel(&work->unkB8, &work->unkC0, (s32)&work->unkD0, work->unk150[9]);
            } else {
                ex->Hi_DrawEffModel(&work->unkB8, &work->unkC0, (s32)&work->unkD0, work->unk150[10]);
            }
        }
        if (frame >= 0x97 && frame < 0xF0) {
            for (j = 0, dr = D_801ECAC0; j < 3; j++, dr++) {
                d = dr->unk0[0];
            drop:
                    work->unkE8.vx = d[0];
                    work->unkE8.vy = d[1];
                    work->unkE8.vz = d[2];
                    work->unkE8.vx += work->unk44.vx;
                    if (j == 2) {
                        work->unkE8.vz -= 0x7530;
                    } else {
                        work->unkE8.vy -= 0xC18;
                        work->unkE8.vz -= 0x4E20;
                    }
                    ex->Hi_DrawEffModel(&work->unkE0, &work->unkE8, (s32)&work->unkF8, work->unk150[j + 11]);
                    d[0] += dr->unk30[0];
                    d[1] += dr->unk30[1];
                    d[2] += dr->unk30[2];
                    if (d[2] < -0x4E20) {
                        d[0] = dr->unk20[0];
                        d[1] = dr->unk20[1];
                        d[2] = dr->unk20[2];
                    }
                    d += 4;
                if ((s32)d < (s32)dr->unk0[2]) {
                    goto drop;
                }
            }
        }
        if (frame >= 1 && frame < 0x2E) {
            for (i8 = 1; i8 < 4; i8++) {
                switch (i8) {
                case 0:
                    D_801F103C[0].vx = env->unk2C->unk0[2].vx;
                    D_801F103C[0].vy = env->unk2C->unk0[2].vy;
                    D_801F103C[0].vz = env->unk2C->unk0[2].vz;
                    break;
                case 1:
                    D_801F103C[1].vx = env->unk2C->unk0[3].vx;
                    D_801F103C[1].vy = env->unk2C->unk0[3].vy;
                    D_801F103C[1].vz = env->unk2C->unk0[3].vz;
                    break;
                case 2:
                    D_801F103C[2].vx = env->unk2C->unk0[8].vx;
                    D_801F103C[2].vy = env->unk2C->unk0[8].vy;
                    D_801F103C[2].vz = env->unk2C->unk0[8].vz;
                    break;
                case 3:
                    D_801F103C[3].vx = env->unk2C->unk0[9].vx;
                    D_801F103C[3].vy = env->unk2C->unk0[9].vy;
                    D_801F103C[3].vz = env->unk2C->unk0[9].vz;
                    break;
                }
            }
        }
        if (frame >= 0x2E) {
            work->unk110.vy = -0x2C9E;
            work->unk110.vx = 0;
            work->unk110.vz = -0x729A;
            if (work->unk0 != NULL && work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            ex->func_800E3D20((s32)work->unk0, (s32)&work->unk110);
        }
        if (frame >= 0x41) {
            work->unk110.vy = -0x303D;
            work->unk110.vx = 0;
            work->unk110.vz = -0x7411;
            if (work->unk4 != NULL && work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            ex->func_800E3D20((s32)work->unk4, (s32)&work->unk110);
        }
        if (frame >= 0xA3) {
            work->unk118.vy = 0x128;
            work->unk118.vx = 0;
            work->unk118.vz = 0x3EB;
            work->unk118.vx += work->unk54.vx;
            work->unk118.vy += work->unk54.vy;
            work->unk118.vz += work->unk54.vz;
            if (work->unk1C != NULL && work->unk1C->unk30 == -1) {
                work->unk1C = NULL;
            }
            ex->func_800E3D20((s32)work->unk1C, (s32)&work->unk118);
        }
        if (frame >= 0xB3) {
            work->unk120.vx = D_801F100C[0].vx;
            work->unk120.vy = D_801F100C[0].vy;
            work->unk120.vz = D_801F100C[0].vz;
            if (work->unk20 != NULL && work->unk20->unk30 == -1) {
                work->unk20 = NULL;
            }
            ex->func_800E3D20((s32)work->unk20, (s32)&work->unk120);
            if (work->unk24 != NULL && work->unk24->unk30 == -1) {
                work->unk24 = NULL;
            }
            ex->func_800E3D20((s32)work->unk24, (s32)&work->unk120);
        }
        func_801EC7C0(0);
        return frame >= 0x14A;
    }
    return 0;
}
