/* Draft of fx_1f3 fx_1f3_op80. Status: 27 aligned mismatches; equivalent by construction. Fixed since the 55-row
 * draft: mode 1 stores unk5C.vz before vy, and case 0x22 tests work->unk40[0] itself (its NULL path then threads
 * past the first post-switch check, as in the original). Open, all in the ring loop: the original's second pointer
 * giv is based at the point's pad (offsets -4/-2/0), its l giv starts from the D_801EC4F0 register (+0x20) instead
 * of its own lui, and it reloads unk12C for the func_801E7794 argument. */
#include "fxmod/fx_1f3.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1f3Vec3;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[33];
} Fx1f3Line; /* 0x10C */

typedef struct {
    MATRIX unk0; /* bone matrix */
    Fx1f3Line unk20;
    s32 unk12C; /* timer */
    s32 unk130; /* bone */
    s32 unk134; /* angle */
    s32 unk138; /* angle speed */
    s32 unk13C; /* radius */
    s32 unk140; /* angle step */
} Fx1f3Ring; /* 0x144 */

typedef struct {
    s32 unk0; /* model */
    s32 unk4; /* spin */
    SVECTOR unk8; /* rotation */
    Fx1f3Vec3 unk10; /* scale */
    u8 pad1C[4];
    Fx1f3Vec3 unk20; /* position */
    u8 pad2C[4];
} Fx1f3Rock; /* 0x30 */

typedef struct {
    s32 unk0; /* bone */
    s32 unk4; /* radius */
    s32 unk8;
} Fx1f3RingDef;

typedef struct {
    s32 unk0[16]; /* timers */
    Pool6CEntry *unk40[7];
    SVECTOR unk5C; /* target position */
    SVECTOR unk64; /* bone position */
    s32 unk6C; /* model */
    Fx1f3Vec3 unk70; /* model position */
    u8 pad7C[4];
    Fx1f3Vec3 unk80; /* model scale */
    u8 pad8C[4];
    SVECTOR unk90; /* model rotation */
    s32 unk98; /* models */
    s32 unk9C;
    SVECTOR unkA0;
    SVECTOR unkA8;
    Fx1f3Vec3 unkB0;
    u8 padBC[4];
    s32 unkC0;
    s32 unkC4;
    SVECTOR unkC8;
    SVECTOR unkD0;
    Fx1f3Vec3 unkD8;
    u8 padE4[4];
    s32 unkE8;
    s32 unkEC;
    SVECTOR unkF0;
    Fx1f3Vec3 unkF8;
    u8 pad104[4];
    SVECTOR unk108; /* camera */
    SVECTOR unk110;
    SVECTOR unk118;
} Fx1f3Work80; /* 0x120 */

extern Fx1f3RingDef D_801E8CA8[16];
extern BtlModel D_801E8DC0[];
extern s32 D_801EA6C0[];
extern Fx1f3Rock D_801EC370[8];
extern Fx1f3Ring D_801EC4F0[16];
extern Fx1f3Line D_801EC510;
void func_801E7704(s32 t, s32 n, s32 from, s32 to, s32 arg4, s32 ot);
s32 func_801E7794(s32 curve, s32 n, s32 t, s32 from, s32 to);

s32 fx_1f3_op80(s32 mode, Fx1f3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    VECTOR out;
    SVECTOR cam;
    SVECTOR cam2;
    s32 flag;
    Pool6CEntry *p;
    Fx1f3Ring *g;
    Fx1f3RingDef *def;
    SVECTOR *dst;
    Fx1f3Line *l;
    s32 n1;
    s32 n2;
    s32 frame;
    s32 i;
    s32 j;
    s32 c;
    s32 a;
    s32 b;
    s32 *q;

    if (mode == 0) {
        *arg4 = 0x120;
    } else if (mode == 1) {
        work->unk40[0] = NULL;
        work->unk40[1] = NULL;
        work->unk40[2] = NULL;
        work->unk40[3] = NULL;
        work->unk40[4] = NULL;
        work->unk40[5] = NULL;
        work->unk40[6] = NULL;
        D_801E8DB0 = env;
        ex->btlfx_initEffModels(D_801E8DC0, 0x20);
        for (i = 15, q = &work->unk0[15]; i >= 0; i--, q--) {
            *q = 0;
        }
        work->unk5C.vx = env->unkC->unk38;
        work->unk5C.vz = env->unkC->unk3C;
        work->unk5C.vy = -0x200;
        for (i = 0; i < 16; i++) {
            D_801EC4F0[i].unk12C = 0;
            D_801EC4F0[i].unk130 = D_801E8CA8[i].unk0;
            D_801EC4F0[i].unk13C = D_801E8CA8[i].unk4;
            D_801EC4F0[i].unk134 = ex->btlfx_rand() & 0xFFF;
            D_801EC4F0[i].unk138 = (ex->btlfx_rand() & 0xFF) + 0x40;
            D_801EC4F0[i].unk140 = 0x40;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&D_801EC4F0[i].unk20, 2);
        }
        for (i = 0, b = 0x600, a = 0x400; i < 8; i++, b += 0x200, a += 0x200) {
            D_801EC370[i].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xD), 1);
            D_801EC370[i].unk10.vy = D_801EC370[i].unk10.vx = D_801EC370[i].unk10.vz = ex->btlfx_randRange(a, b);
            D_801EC370[i].unk20.vx = work->unk5C.vx;
            D_801EC370[i].unk20.vz = work->unk5C.vz;
            D_801EC370[i].unk20.vy = work->unk5C.vy + ex->btlfx_randRange(-0x200, 0x200);
            D_801EC370[i].unk8.vy = (i << 10) + (ex->btlfx_rand() & 0x3FF);
            D_801EC370[i].unk8.vz = 0;
            D_801EC370[i].unk8.vx = -0x400;
            D_801EC370[i].unk4 = (ex->btlfx_rand() & 0x1FF) + 0x100;
        }
        work->unk6C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 1);
        work->unk70.vx = env->unkC->unk38;
        work->unk70.vz = env->unkC->unk3C;
        work->unk70.vy = -0x200;
        work->unk90.vx = 0;
        work->unk90.vy = 0;
        work->unk90.vz = 0;
        work->unk80.vz = work->unk80.vy = work->unk80.vx = 0x3000;
        work->unk98 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 9), 1);
        work->unk9C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 9), 1);
        work->unkA0.vx = 0;
        work->unkA0.vy = 0;
        work->unkA0.vz = 0;
        work->unkA8.vx = 0x800;
        work->unkA8.vy = 0;
        work->unkA8.vz = 0;
        work->unkB0.vz = work->unkB0.vy = work->unkB0.vx = 0x400;
        work->unkC0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 8), 1);
        work->unkC4 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 8), 1);
        work->unkC8.vx = 0;
        work->unkC8.vy = 0;
        work->unkC8.vz = 0;
        work->unkD0.vx = 0;
        work->unkD0.vy = 0;
        work->unkD0.vz = 0;
        work->unkD8.vz = work->unkD8.vy = work->unkD8.vx = 0x400;
        work->unkE8 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x13), 1);
        work->unkEC = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x14), 1);
        work->unkF0.vx = 0;
        work->unkF0.vy = 0;
        work->unkF0.vz = 0;
        work->unkF8.vz = work->unkF8.vy = work->unkF8.vx = 0x400;
        ex->btlfx_getUnitHomePos(0x10, (s16 *)&work->unk108);
        ex->btlfx_getUnitHomePos(0x10, (s16 *)&work->unk110);
        ex->btlfx_getUnitHomePos(0x10, (s16 *)&work->unk118);
        work->unk110.vy -= 0x400;
        work->unk118.vy -= 0x800;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0[7] = 0x4A;
            work->unk0[9] = 0x20;
            break;
        case 0x22:
            ex->func_800DC818(0x10, 0x2E, (u16 *)&work->unk64);
            work->unk40[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk64);
            if (work->unk40[0] != NULL) {
                work->unk40[0]->unk22 = 0x200;
            }
            break;
        case 0x2A:
            p = work->unk40[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk64);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x52:
            p = work->unk40[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk5C);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x62:
            work->unk40[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk5C);
            if (work->unk40[2] != NULL) {
                work->unk40[2]->unk14 = (s32)D_801EA6C0;
                work->unk40[2]->unk12 = 0x24;
                work->unk40[2]->unkC = 0xCC;
                if (work->unk40[2] != NULL) {
                    work->unk40[2]->unk22 = 0x200;
                }
            }
            p = work->unk40[4] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk5C);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x72:
            work->unk0[0] = 0x80;
            break;
        case 0x7E:
            p = work->unk40[6] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk5C);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x92:
            ex->func_800E3D54(work->unk40[6]);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk5C);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            work->unk0[1] = 0x10;
            break;
        case 0x9A:
            work->unk0[2] = 4;
            break;
        case 0x9D:
            work->unk0[0] = 0;
            ex->func_800E3D54(work->unk40[2]);
            ex->func_800E3D54(work->unk40[3]);
            ex->func_800E3D54(work->unk40[4]);
            ex->func_800E3D54(work->unk40[6]);
            break;
        case 0x9E:
            work->unk0[3] = 8;
            work->unk0[5] = 0x20;
            work->unk0[6] = 0x20;
            work->unk0[8] = 0x20;
            work->unk0[4] = 0x60;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk5C);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0xBE:
            work->unk0[10] = 0x20;
            break;
        }
        if (work->unk40[0] != NULL && work->unk40[0]->unk30 == -1) {
            work->unk40[0] = NULL;
        }
        if (work->unk40[1] != NULL && work->unk40[1]->unk30 == -1) {
            work->unk40[1] = NULL;
        }
        if (work->unk40[2] != NULL && work->unk40[2]->unk30 == -1) {
            work->unk40[2] = NULL;
        }
        if (work->unk40[3] != NULL && work->unk40[3]->unk30 == -1) {
            work->unk40[3] = NULL;
        }
        if (work->unk40[4] != NULL && work->unk40[4]->unk30 == -1) {
            work->unk40[4] = NULL;
        }
        if (work->unk40[5] != NULL && work->unk40[5]->unk30 == -1) {
            work->unk40[5] = NULL;
        }
        if (work->unk40[6] != NULL && work->unk40[6]->unk30 == -1) {
            work->unk40[6] = NULL;
        }
        if (work->unk40[1] != NULL) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x10), frame);
        }
        if (work->unk0[1] != 0) {
            work->unk0[1]--;
            work->unk80.vx = work->unk80.vy = work->unk80.vz = func_801E7794(2, 0x10, work->unk0[1], 0x3000, 0);
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            ex->btlfx_aimAngles((s16 *)&env->unk2C->unk54, (s16 *)&work->unk5C, (s16 *)&work->unk90);
            ex->Hi_DrawEffModel(&work->unk90, (VECTOR *)&work->unk70, (s32)&work->unk80, work->unk6C);
            n1 = 0x80 - work->unk0[0];
            if (n1 < 8) {
                c = (n1 << 5) - 0x100;
                ex->Hi_ModifyEffModelRGB(work->unk6C, c, c, c);
            }
            ex->func_800E6624(0x2710 - frame * 8, 0x240, 0x100, 0x40, 0x80, 0x240, 0x180);
        }
        if (work->unk0[5] != 0) {
            work->unk0[5]--;
            work->unkB0.vx = work->unkB0.vy = work->unkB0.vz = func_801E7794(2, 0x20, work->unk0[5], 0x100, 0x400);
            ex->Hi_DrawEffModel(&work->unkA0, (VECTOR *)&work->unk70, (s32)&work->unkB0, work->unk98);
            ex->Hi_DrawEffModel(&work->unkA8, (VECTOR *)&work->unk70, (s32)&work->unkB0, work->unk9C);
            if (work->unk0[5] < 0x10) {
                c = -((0x10 - work->unk0[5]) * 0x10);
                ex->Hi_ModifyEffModelRGB(work->unk98, c, c, c);
                ex->Hi_ModifyEffModelRGB(work->unk9C, c, c, c);
            }
            ex->func_800E6624(0x2710 - frame * 8, 0x240, 0x100, 0x40, 0x80, 0x240, 0x180);
        }
        if (work->unk0[8] != 0) {
            work->unk0[8]--;
            work->unkF8.vx = work->unkF8.vy = work->unkF8.vz = func_801E7794(2, 0x20, work->unk0[8], 0x400, 0x1000);
            ex->Hi_DrawEffModel(&work->unkF0, (VECTOR *)&work->unk70, (s32)&work->unkF8, work->unkE8);
            ex->Hi_DrawEffModel(&work->unkF0, (VECTOR *)&work->unk70, (s32)&work->unkF8, work->unkEC);
            if (work->unk0[8] < 0x10) {
                c = -((0x10 - work->unk0[8]) * 0x10);
                ex->Hi_ModifyEffModelRGB(work->unkE8, c, c, c);
                ex->Hi_ModifyEffModelRGB(work->unkEC, c, c, c);
            }
            ex->func_800E6624(0x2710 - frame * 8, 0x200, 0x100, 0x40, 0x80, 0x200, 0x180);
        }
        if (work->unk0[6] != 0) {
            work->unk0[6]--;
            work->unkD8.vx = work->unkD8.vy = func_801E7794(2, 0x20, work->unk0[6], 0x400, 0xC00);
            work->unkD8.vz = func_801E7794(2, 0x20, work->unk0[6], 0x400, 0x800);
            ex->Hi_DrawEffModel(&work->unkC8, (VECTOR *)&work->unk70, (s32)&work->unkD8, work->unkC0);
            ex->Hi_DrawEffModel(&work->unkD0, (VECTOR *)&work->unk70, (s32)&work->unkD8, work->unkC4);
            if (work->unk0[6] < 0x10) {
                c = -((0x10 - work->unk0[6]) * 0x10);
                ex->Hi_ModifyEffModelRGB(work->unkC0, c, c, c);
                ex->Hi_ModifyEffModelRGB(work->unkC4, c, c, c);
            }
            work->unkC8.vy = 0x400;
            work->unkC8.vx = 0x200;
            work->unkD0.vy = -0x400;
            work->unkD0.vx = 0x200;
            work->unkC8.vz += 0x40;
            work->unkD0.vz += 0x40;
            ex->func_800E6624(0x2710 - frame * 8, 0x2C0, 0x100, 0x20, 0x80, 0x2E0, 0x100);
        }
        if (work->unk0[4] != 0) {
            work->unk0[4]--;
            for (i = 0; i < 8; i++) {
                D_801EC370[i].unk8.vz += func_801E7794(2, 0x60, work->unk0[4], D_801EC370[i].unk4, 0);
                n2 = func_801E7794(2, 0x60, work->unk0[4], 0x40, 0);
                D_801EC370[i].unk10.vz = 0x800;
                D_801EC370[i].unk10.vy += n2;
                D_801EC370[i].unk10.vx += n2;
                ex->Hi_DrawEffModel(&D_801EC370[i].unk8, (VECTOR *)&D_801EC370[i].unk20, (s32)&D_801EC370[i].unk10,
                                    D_801EC370[i].unk0);
                if (work->unk0[4] < 0x40) {
                    c = -((0x40 - work->unk0[4]) * 4);
                    ex->Hi_ModifyEffModelRGB(D_801EC370[i].unk0, c, c, c);
                }
            }
        }
        if (work->unk0[7] != 0) {
            work->unk0[7]--;
            env->unk28 = 0x80;
            if (work->unk0[7] >= 9) {
                for (i = 0; i < 8; i++) {
                    if (D_801EC4F0[i].unk12C == 0) {
                        ex->func_800DC8AC(0x10, D_801EC4F0[i].unk130, (BtlBone *)&D_801EC4F0[i]);
                        D_801EC4F0[i].unk12C = 8;
                        break;
                    }
                }
                for (i = 8; i < 16; i++) {
                    if (D_801EC4F0[i].unk12C == 0) {
                        ex->func_800DC8AC(0x10, D_801EC4F0[i].unk130, (BtlBone *)&D_801EC4F0[i]);
                        D_801EC4F0[i].unk12C = 8;
                        break;
                    }
                }
            }
            for (i = 0; i < 16; i++) {
                if (D_801EC4F0[i].unk12C != 0) {
                    D_801EC4F0[i].unk12C--;
                    ex->func_800DC818(0x10, D_801EC4F0[i].unk130, (u16 *)&v);
                    D_801EC4F0[i].unk0.t[0] = v.vx;
                    D_801EC4F0[i].unk0.t[1] = v.vy;
                    D_801EC4F0[i].unk0.t[2] = v.vz;
                    gte_SetRotMatrix(&D_801EC4F0[i].unk0);
                    gte_SetTransMatrix(&D_801EC4F0[i].unk0);
                    l = &D_801EC4F0[i].unk20;
                    l->unk0 = l->unk2 = 0x20;
                    l->unk1 = 0;
                    dst = l->unk4;
                    for (j = 0; j < 0x21; j++, dst++) {
                        v.vx = (ex->math_sin(j * D_801EC4F0[i].unk140 + D_801EC4F0[i].unk134) * D_801EC4F0[i].unk13C) >> 12;
                        v.vy = 0;
                        v.vz = (ex->math_cos(j * D_801EC4F0[i].unk140 + D_801EC4F0[i].unk134) * D_801EC4F0[i].unk13C) >> 12;
                        gte_ldv0(&v);
                        gte_rtv0tr();
                        gte_stlvnl(&out);
                        gte_stflg(&flag);
                        dst->vx = out.vx;
                        l->unk4[j].vy = out.vy;
                        l->unk4[j].vz = out.vz;
                        l->unk4[j].pad = 0xC0;
                    }
                    if (D_801EC4F0[i].unk12C < 4) {
                        c = D_801EC4F0[i].unk12C << 10;
                        D_801EC4F0[i].unk134 += func_801E7794(2, 8, D_801EC4F0[i].unk12C, D_801EC4F0[i].unk138, 0);
                    } else {
                        c = (8 - D_801EC4F0[i].unk12C) << 10;
                        D_801EC4F0[i].unk134 += D_801EC4F0[i].unk138;
                    }
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0, NULL, 0, 0x1000, c, 1,
                                        (u8 *)&D_801EC4F0[i].unk20, 2);
                }
            }
            env->unk28 = 0;
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            func_801E7704(work->unk0[2], 4, 0xFF, 0, 1, 0);
        }
        if (work->unk0[3] != 0) {
            work->unk0[3]--;
            func_801E7704(work->unk0[3], 8, 0, 0xFF, 1, 0);
        }
        if (work->unk0[9] != 0) {
            work->unk0[9]--;
            cam.vx = work->unk108.vx;
            cam.vz = work->unk108.vz;
            cam.vy = func_801E7794(1, 0x20, work->unk0[9], work->unk108.vy, work->unk118.vy);
            ex->btlfx_setUnitPos(0x10, (s16 *)&cam);
        }
        if (work->unk0[10] != 0) {
            work->unk0[10]--;
            if (work->unk0[10] != 0) {
                cam2.vx = work->unk108.vx;
                cam2.vz = work->unk108.vz;
                cam2.vy = func_801E7794(2, 0x20, work->unk0[10], work->unk118.vy, work->unk108.vy);
            } else {
                ex->btlfx_getUnitHomePos(0x10, (s16 *)&cam2);
            }
            ex->btlfx_setUnitPos(0x10, (s16 *)&cam2);
        }
        return frame >= 0xFE;
    }
    return 0;
}
