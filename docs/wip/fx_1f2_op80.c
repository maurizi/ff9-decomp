/* Draft of fx_1f2 fx_1f2_op80 (first draft from m2c; summon with many effect models, spark trails and a frame
 * switch). Status: 2422 aligned mismatches, 824 ignoring registers (of ~2640 instructions). Types: Fx1f2Model
 * (0x30 model records), Fx1f2Ring (0x38), Fx1f2Trail (0x150 trail + 16 points + 16 velocities). */
#include "fxmod/fx_1f2.h"

typedef struct {
    s32 unk0; /* models drawn */
    s32 unk4; /* flash model timer */
    s32 unk8; /* twin model timer */
    s32 unkC; /* tint timers */
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20; /* screen ripple timer */
    s32 unk24;
    s32 unk28;
    s32 unk2C; /* spin speed */
    s32 unk30;
    s32 unk34; /* sparks timer */
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 pad5C[(0x80 - 0x5C) / 4];
    Pool6CEntry *unk80;
    Pool6CEntry *unk84;
    Pool6CEntry *unk88;
    Pool6CEntry *unk8C;
    s32 pad90[(0x100 - 0x90) / 4];
    SVECTOR unk100;
    SVECTOR unk108;
    SVECTOR unk110; /* bone 5 position */
    SVECTOR unk118; /* bone 9 position */
    SVECTOR unk120; /* midpoint */
} Fx1f2Work; /* 0x128 */

typedef struct {
    s32 unk0; /* model */
    s32 unk4; /* radius */
    SVECTOR unk8; /* rotation */
    VECTOR unk10; /* position */
    VECTOR unk20; /* scale */
} Fx1f2Model; /* 0x30 */

typedef struct {
    s32 unk0; /* models */
    s32 unk4;
    s32 unk8;
    s32 unkC; /* radius */
    SVECTOR unk10; /* rotation */
    VECTOR unk18; /* position */
    VECTOR unk28; /* scale */
} Fx1f2Ring; /* 0x38 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[9];
    s32 unk4C; /* timer */
    SVECTOR unk50[16]; /* points (pad = next) */
    SVECTOR unkD0[16]; /* velocities */
} Fx1f2Trail; /* 0x150 */

extern Fx1f2Model D_801EA270;
extern Fx1f2Model D_801EA2A0;
extern Fx1f2Model D_801EA2D0;
extern Fx1f2Ring D_801EA300[4];
extern Fx1f2Model D_801EA3E0;
extern Fx1f2Model D_801EA410[2];
extern Fx1f2Model D_801EA470;
extern Fx1f2Model D_801EA4A0[2];
extern Fx1f2Model D_801EA500;
extern Fx1f2Model D_801EA530[4];
extern SVECTOR D_801EA568;
extern Fx1f2Model D_801EA5F0;
extern VECTOR D_801EA600;
extern Fx1f2Model D_801EA620;
extern SVECTOR D_801EA650; /* target */
extern SVECTOR D_801EA658; /* angles to the target */
extern SVECTOR D_801EA660;
extern SVECTOR D_801EA670[4]; /* unit positions */
extern Fx1f2Trail D_801EA690[16];
extern Fx1f2Trail D_801EBB90[4];
extern BtlModel D_801ED090[];

s32 func_801E7794(s32 curve, s32 n, s32 t, s32 from, s32 to);

#define FX1F2_DRAW(m) ex->Hi_DrawEffModel(&(m).unk8, &(m).unk10, (s32)&(m).unk20, (m).unk0)

s32 fx_1f2_op80(s32 mode, Fx1f2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    CVECTOR col;
    MATRIX m;
    SVECTOR in[2];
    VECTOR out[2];
    VECTOR step;
    s32 flag;
    s32 flag2;
    s32 frame;
    Pool6CEntry *p;
    Fx1f2Trail *tr;
    Fx1f2Ring *rg;
    Fx1f2Model *md;
    s32 i;
    s32 j;
    s32 a;
    s32 r;
    s32 c;
    s32 lo;
    s32 hi;

    if (mode == 0) {
        *arg4 = 0x128;
    } else if (mode == 1) {
        env->unk2C->unk0[1].vx = 0;
        env->unk2C->unk0[1].vz = -0x4000;
        env->unk2C->unk0[1].vy = -0x6000;
        D_801EA260 = env;
        ex->btlfx_initEffModels(D_801ED090, 0x20);
        for (i = 0; i < 0x20; i++) {
            ((s32 *)work)[i] = 0;
            ((s32 *)work)[i + 0x20] = 0;
        }
        for (i = 0; i < env->unkC->unk24; i++) {
            ex->btlfx_getUnitPos(i, (s16 *)&D_801EA670[i]);
            D_801EA670[i].vy = 0;
        }
        ex->func_800DC818(0x10, 5, (u16 *)&work->unk110);
        ex->func_800DC818(0x10, 9, (u16 *)&work->unk118);
        work->unk120.vx = work->unk110.vx;
        work->unk120.vz = work->unk110.vz - 0x200;
        work->unk120.vy = (work->unk110.vy + work->unk118.vy) >> 1;
        D_801EA650.vx = 0;
        D_801EA650.vy = 0;
        D_801EA650.vz = 0;
        D_801EA660.vx = 0;
        D_801EA660.vy = 0;
        D_801EA660.vz = 0;
        for (i = 0, tr = D_801EA690; i < 0x10; i++, tr++) {
            tr->unk4C = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 6), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)tr,
                                0);
        }
        for (i = 0, tr = D_801EA690; i < 8; i++, tr++) {
            tr->unk4C = ex->btlfx_rand() & 0xF;
            for (j = 0; j < 0x10;) {
                if (i & 1) {
                    a = j << 8;
                    c = 1;
                    r = (ex->btlfx_rand() & 0xFF) + 0x17E;
                } else {
                    a = 0x1000 - (j << 8);
                    c = 0;
                    r = (ex->btlfx_rand() & 0xFF) + 0x200;
                }
                lo = (ex->math_sin(a) * r) >> 12;
                hi = ex->math_cos(a) * r;
                tr->unk50[j].vx = (&work->unk110)[c].vx - lo;
                tr->unk50[j].vz = (&work->unk110)[c].vz - (hi >> 12);
                j++;
                tr->unk50[j - 1].pad = j;
                tr->unk50[j - 1].vy = (&work->unk110)[c].vy + (ex->btlfx_rand() & 0xFF) - 0x80;
            }
            tr->unk50[15].pad = 0;
        }
        D_801EA270.unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x84), 0xFF);
        D_801EA270.unk8.vx = 0;
        D_801EA270.unk8.vy = 0;
        D_801EA270.unk8.vz = 0;
        D_801EA270.unk10.vx = 0;
        D_801EA270.unk10.vy = 0;
        D_801EA270.unk10.vz = 0;
        D_801EA270.unk20.vx = 0x1000;
        D_801EA270.unk20.vy = 0x1000;
        D_801EA270.unk20.vz = 0x1000;
        D_801EA2A0.unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x87), 0xFF);
        D_801EA2A0.unk8.vx = 0;
        D_801EA2A0.unk8.vy = 0;
        D_801EA2A0.unk8.vz = 0;
        D_801EA2A0.unk10.vx = 0;
        D_801EA2A0.unk10.vy = 0;
        D_801EA2A0.unk10.vz = 0;
        D_801EA2A0.unk20.vx = 0x1000;
        D_801EA2A0.unk20.vy = 0x1000;
        D_801EA2A0.unk20.vz = 0x1000;
        D_801EA2D0.unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x89), 1);
        D_801EA2D0.unk8.vx = 0;
        D_801EA2D0.unk8.vy = 0;
        D_801EA2D0.unk8.vz = 0;
        D_801EA2D0.unk10.vx = 0;
        D_801EA2D0.unk10.vy = 0;
        D_801EA2D0.unk10.vz = 0;
        D_801EA2D0.unk20.vx = 0xD00;
        D_801EA2D0.unk20.vy = 0xD00;
        D_801EA2D0.unk20.vz = 0xD00;
        D_801EA3E0.unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x88), 1);
        D_801EA3E0.unk8.vx = 0;
        D_801EA3E0.unk8.vy = 0;
        D_801EA3E0.unk8.vz = 0;
        D_801EA3E0.unk10.vx = 0;
        D_801EA3E0.unk10.vy = 0;
        D_801EA3E0.unk10.vz = 0;
        D_801EA3E0.unk20.vx = 0x1000;
        D_801EA3E0.unk20.vy = 0x1000;
        D_801EA3E0.unk20.vz = 0x1000;
        D_801EA410[0].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x80), 0xFF);
        D_801EA410[1].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8A), 0xFF);
        D_801EA410[0].unk4 = 0xC00;
        D_801EA410[1].unk4 = 0xC00;
        D_801EA410[0].unk8.vx = 0;
        D_801EA410[0].unk8.vy = 0;
        D_801EA410[0].unk8.vz = 0;
        D_801EA410[1].unk8.vx = 0;
        D_801EA410[1].unk8.vy = 0;
        D_801EA410[1].unk8.vz = 0x800;
        D_801EA410[0].unk10.vx = 0;
        D_801EA410[0].unk10.vy = 0;
        D_801EA410[0].unk10.vz = 0;
        D_801EA410[1].unk10.vx = 0;
        D_801EA410[1].unk10.vy = 0;
        D_801EA410[1].unk10.vz = 0;
        D_801EA410[0].unk20.vx = 0xC00;
        D_801EA410[0].unk20.vy = 0xC00;
        D_801EA410[0].unk20.vz = 0xC00;
        D_801EA410[1].unk20.vx = 0xC00;
        D_801EA410[1].unk20.vy = 0xC00;
        D_801EA410[1].unk20.vz = 0xC00;
        D_801EA470.unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8C), 1);
        D_801EA470.unk8.vx = 0;
        D_801EA470.unk8.vy = 0;
        D_801EA470.unk8.vz = 0;
        D_801EA470.unk10.vx = 0;
        D_801EA470.unk10.vy = 0;
        D_801EA470.unk10.vz = 0;
        D_801EA470.unk20.vx = 0x1000;
        D_801EA470.unk20.vy = 0x1000;
        D_801EA470.unk20.vz = 0x1000;
        D_801EA4A0[0].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8D), 1);
        D_801EA4A0[1].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8D), 1);
        D_801EA4A0[0].unk8.vx = 0;
        D_801EA4A0[0].unk8.vy = 0;
        D_801EA4A0[0].unk8.vz = 0;
        D_801EA4A0[1].unk8.vx = 0;
        D_801EA4A0[1].unk8.vy = 0;
        D_801EA4A0[1].unk8.vz = 0x800;
        D_801EA4A0[0].unk10.vx = 0;
        D_801EA4A0[0].unk10.vy = 0;
        D_801EA4A0[0].unk10.vz = 0;
        D_801EA4A0[1].unk10.vx = 0;
        D_801EA4A0[1].unk10.vy = 0;
        D_801EA4A0[1].unk10.vz = 0;
        D_801EA4A0[0].unk20.vx = 0x1000;
        D_801EA4A0[0].unk20.vy = 0x1000;
        D_801EA4A0[0].unk20.vz = 0x1000;
        D_801EA4A0[1].unk20.vx = 0x1000;
        D_801EA4A0[1].unk20.vy = 0x1000;
        D_801EA4A0[1].unk20.vz = 0x1000;
        D_801EA530[0].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 7), 1);
        D_801EA530[1].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 7), 1);
        D_801EA530[2].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 9), 1);
        D_801EA530[3].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 9), 1);
        D_801EA530[0].unk8.vx = 0;
        D_801EA530[0].unk8.vy = 0;
        D_801EA530[0].unk8.vz = 0;
        D_801EA530[1].unk8.vx = 0;
        D_801EA530[1].unk8.vy = 0;
        D_801EA530[1].unk8.vz = 0x800;
        D_801EA530[2].unk8.vx = 0;
        D_801EA530[2].unk8.vy = 0;
        D_801EA530[2].unk8.vz = 0;
        D_801EA530[3].unk8.vx = 0;
        D_801EA530[3].unk8.vy = 0;
        D_801EA530[3].unk8.vz = 0x800;
        D_801EA530[0].unk10.vx = 0;
        D_801EA530[0].unk10.vy = 0;
        D_801EA530[0].unk10.vz = 0;
        D_801EA530[1].unk10.vx = 0;
        D_801EA530[1].unk10.vy = 0;
        D_801EA530[1].unk10.vz = 0;
        D_801EA530[2].unk10.vx = 0;
        D_801EA530[2].unk10.vy = 0;
        D_801EA530[2].unk10.vz = 0;
        D_801EA530[3].unk10.vx = 0;
        D_801EA530[3].unk10.vy = 0;
        D_801EA530[3].unk10.vz = 0;
        D_801EA530[0].unk20.vx = 0x1000;
        D_801EA530[0].unk20.vy = 0x1000;
        D_801EA530[0].unk20.vz = 0x1000;
        D_801EA530[1].unk20.vx = 0x1000;
        D_801EA530[1].unk20.vy = 0x1000;
        D_801EA530[1].unk20.vz = 0x1000;
        D_801EA530[2].unk20.vx = 0x1000;
        D_801EA530[2].unk20.vy = 0x1000;
        D_801EA530[2].unk20.vz = 0x1000;
        D_801EA530[3].unk20.vx = 0x1000;
        D_801EA530[3].unk20.vy = 0x1000;
        D_801EA530[3].unk20.vz = 0x1000;
        D_801EA500.unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8E), 1);
        D_801EA500.unk8.vx = 0;
        D_801EA500.unk8.vy = 0;
        D_801EA500.unk8.vz = 0;
        D_801EA500.unk10.vx = 0;
        D_801EA500.unk10.vy = 0;
        D_801EA500.unk10.vz = 0;
        D_801EA500.unk20.vx = 0x1000;
        D_801EA500.unk20.vy = 0x1000;
        D_801EA500.unk20.vz = 0x1000;
        D_801EA5F0.unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xF), 1);
        D_801EA5F0.unk8.vx = 0;
        D_801EA5F0.unk8.vy = 0;
        D_801EA5F0.unk8.vz = 0;
        D_801EA5F0.unk10.vx = 0;
        D_801EA5F0.unk10.vy = 0;
        D_801EA5F0.unk10.vz = 0;
        D_801EA5F0.unk20.vx = 0x1000;
        D_801EA5F0.unk20.vy = 0x1000;
        D_801EA5F0.unk20.vz = 0x1000;
        D_801EA620.unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 2);
        D_801EA620.unk8.vx = 0;
        D_801EA620.unk8.vy = 0;
        D_801EA620.unk8.vz = 0;
        D_801EA620.unk10.vx = work->unk120.vx;
        D_801EA620.unk10.vy = work->unk120.vy;
        D_801EA620.unk10.vz = work->unk120.vz;
        D_801EA620.unk20.vx = 0x1000;
        D_801EA620.unk20.vy = 0x1000;
        D_801EA620.unk20.vz = 0x1000;
        D_801EA300[0].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x82), 0xFF);
        D_801EA300[1].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x81), 0xFF);
        D_801EA300[2].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8B), 0xFF);
        D_801EA300[3].unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x83), 0xFF);
        for (i = 0, a = 0x800; i < 4; i++, a += 0x400) {
            D_801EA300[i].unk4 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x86), 1);
            D_801EA300[i].unk8 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x85), 1);
            D_801EA300[i].unkC = 0xC00;
            D_801EA300[i].unk10.vx = 0;
            D_801EA300[i].unk10.vy = a;
            D_801EA300[i].unk10.vz = 0;
            D_801EA300[i].unk18.vx = 0;
            D_801EA300[i].unk18.vy = 0;
            D_801EA300[i].unk18.vz = 0;
            D_801EA300[i].unk28.vx = 0x1000;
            D_801EA300[i].unk28.vy = 0x1000;
            D_801EA300[i].unk28.vz = 0x1000;
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk2C = 0x20;
            work->unk4C = 0x5F;
            p = work->unk84 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk120);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x2D:
            work->unk54 = 0x32;
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk110);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk118);
            p = work->unk80 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk120);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x4F:
            work->unkC = 0x10;
            break;
        case 0x5F:
            work->unk4C = 0;
            for (i = 0, tr = D_801EA690; i < 0x10; i++, tr++) {
                tr->unk4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 6), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)tr,
                                    0);
            }
            work->unk20 = 0x40;
            work->unk0 = 1;
            break;
        case 0x8F:
            work->unk8 = 0x50;
            work->unk44 = 0x40;
            break;
        case 0x9F:
            work->unk3C = 0x40;
            work->unk24 = 0x50;
            work->unk100.vy = -0x4C8;
            work->unk100.vx = 0;
            work->unk100.vz = 0;
            work->unk108.vx = 0;
            work->unk108.vy = 0x4C8;
            work->unk108.vz = 0;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk100);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk108);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0xCF:
            work->unk28 = 0x80;
            work->unk34 = 0x80;
            work->unk38 = 0x200;
            break;
        case 0xFF:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&D_801EA650);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x11F:
            p = work->unk88 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&D_801EA660);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            p = work->unk8C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&D_801EA660);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            work->unk4 = 0x80;
            break;
        case 0x13F:
            work->unk14 = 0x10;
            break;
        case 0x14F:
            work->unk2C = 0x40;
            D_801EA650.vx = 0;
            D_801EA650.vy = -0x4FD;
            D_801EA650.vz = 0;
            D_801EA600.vx = 0;
            D_801EA600.vy = -0x4FD;
            D_801EA600.vz = 0;
            ex->func_800E3D20((s32)work->unk88, (s32)&D_801EA650);
            ex->func_800E3D20((s32)work->unk8C, (s32)&D_801EA650);
            work->unk18 = 0x10;
            work->unk50 = 0x68;
            work->unk24 = 0;
            work->unk0 = 0;
            work->unk8 = 0;
            work->unk44 = 0;
            work->unk3C = 0;
            work->unk4 = 0;
            work->unk20 = 0x40;
            work->unk34 = 0x50;
            break;
        case 0x18F:
            work->unk14 = 0x10;
            for (i = 0, tr = D_801EBB90; i < 0x10; i++, tr++) {
                tr->unk4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 6), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)tr,
                                    0);
            }
            break;
        case 0x19F:
            work->unk18 = 0x10;
            ex->func_800E3D54(work->unk8C);
            ex->func_800E3D54(work->unk88);
            break;
        case 0x1A7:
            work->unk58 = 0x18;
            break;
        case 0x1BF:
            return 1;
        }
        ex->btlfx_aimAngles((s16 *)&D_801EA650, (s16 *)&env->unk2C->unk54, (s16 *)&D_801EA658);
        if (work->unk0 != 0) {
            D_801EA2A0.unk8.vx = D_801EA658.vx + 0x800;
            D_801EA2A0.unk8.vy = D_801EA658.vy;
            FX1F2_DRAW(D_801EA270);
            FX1F2_DRAW(D_801EA2A0);
            FX1F2_DRAW(D_801EA2D0);
            FX1F2_DRAW(D_801EA3E0);
            for (rg = D_801EA300; rg < &D_801EA300[4]; rg++) {
                rg->unk18.vx = (ex->math_sin(rg->unk10.vy) * rg->unkC) >> 12;
                lo = ex->math_cos(rg->unk10.vy) * rg->unkC;
                rg->unk18.vy = 0;
                rg->unk18.vz = lo >> 12;
                if (work->unk48 == 0) {
                    ex->Hi_DrawEffModel(&rg->unk10, &rg->unk18, (s32)&rg->unk28, rg->unk8);
                    if (work->unk44 != 0) {
                        if (--work->unk44 == 0) {
                            work->unk48 = 1;
                        }
                        c = (work->unk44 - 0x40) * 2;
                        ex->Hi_ModifyEffModelRGB(rg->unk8, c, c, c);
                        rg->unkC -= work->unk44 >> 4;
                    }
                }
                rg->unk18.vx += ex->btlfx_randRange(-8, 8);
                rg->unk18.vy += ex->btlfx_randRange(-8, 8);
                if (work->unk40 == 0) {
                    ex->Hi_DrawEffModel(&rg->unk10, &rg->unk18, (s32)&rg->unk28, rg->unk4);
                    if (work->unk3C != 0) {
                        if (--work->unk3C == 0) {
                            work->unk40 = 1;
                        }
                        c = (work->unk3C - 0x40) * 2;
                        ex->Hi_ModifyEffModelRGB(rg->unk4, c, c, c);
                    }
                }
                ex->Hi_DrawEffModel(&rg->unk10, &rg->unk18, (s32)&rg->unk28, rg->unk0);
            }
            for (i = 0, md = D_801EA410; i < 2; i++, md++) {
                lo = ex->math_cos(md->unk8.vz) * md->unk4;
                md->unk10.vx = 0;
                md->unk10.vz = 0;
                md->unk10.vy = lo >> 12;
                ex->Hi_DrawEffModel(&md->unk8, &md->unk10, (s32)&md->unk20, md->unk0);
                md->unk4 -= 4;
            }
            D_801EA270.unk8.vy++;
            D_801EA2D0.unk8.vy--;
            ex->func_800E6624(0x7530 - frame * 3, 0x340, 0x180, 0x10, 0x80, 0x350, 0x180);
            ex->func_800E6624(0x7530 - frame * 4, 0x1C0, 0x100, 0x10, 0x80, 0x1D0, 0x100);
        }
        if (work->unk4 != 0) {
            work->unk4--;
            D_801EA470.unk20.vx += 8;
            D_801EA470.unk20.vz += 8;
            D_801EA470.unk20.vy += 8;
            FX1F2_DRAW(D_801EA470);
            if (work->unk4 > 0x60) {
                c = (0x60 - work->unk4) * 2;
                ex->Hi_ModifyEffModelRGB(D_801EA470.unk0, c, c, c);
            }
        }
        if (work->unk24 != 0) {
            work->unk24--;
            D_801EA500.unk8.vx = D_801EA658.vx + 0x800;
            D_801EA500.unk8.vy = D_801EA658.vy;
            FX1F2_DRAW(D_801EA500);
            if (work->unk24 > 0x30) {
                c = (0x30 - work->unk24) * 8;
                ex->Hi_ModifyEffModelRGB(D_801EA500.unk0, c, c, c);
            }
            if (work->unk24 < 0x20) {
                c = (work->unk24 - 0x20) * 8;
                ex->Hi_ModifyEffModelRGB(D_801EA500.unk0, c, c, c);
            }
        }
        if (work->unk8 != 0) {
            work->unk8--;
            D_801EA4A0[0].unk8.vx = 0x900;
            D_801EA4A0[1].unk8.vx = 0x700;
            D_801EA4A0[1].unk8.vy = D_801EA658.vy;
            D_801EA4A0[0].unk8.vy = D_801EA658.vy;
            for (i = 0, md = D_801EA4A0; i < 2; i++, md++) {
                ex->Hi_DrawEffModel(&md->unk8, &md->unk10, (s32)&md->unk20, md->unk0);
                if (work->unk8 > 0x30) {
                    c = (0x30 - work->unk8) * 8;
                    ex->Hi_ModifyEffModelRGB(md->unk0, c, c, c);
                }
                if (work->unk8 < 0x20) {
                    c = (work->unk8 - 0x20) * 8;
                    ex->Hi_ModifyEffModelRGB(md->unk0, c, c, c);
                }
            }
        }
        if (work->unk28 != 0) {
            work->unk28--;
            for (i = 0, md = D_801EA530; i < 4; i++, md++) {
                md->unk8.vx = D_801EA658.vx + 0x800;
                md->unk8.vy = D_801EA658.vy;
                ex->Hi_DrawEffModel(&md->unk8, &md->unk10, (s32)&md->unk20, md->unk0);
                if (work->unk28 > 0x70) {
                    c = (0x70 - work->unk28) * 0x10;
                    ex->Hi_ModifyEffModelRGB(md->unk0, c, c, c);
                }
                if (work->unk28 < 0x20) {
                    c = (work->unk28 - 0x20) * 8;
                    ex->Hi_ModifyEffModelRGB(md->unk0, c, c, c);
                }
            }
            ex->func_800E6624(frame * 8, 0x300, 0x100, 0x20, 0x80, 0x320, 0x100);
            ex->func_800E6624(frame * 0x10, 0x320, 0x180, 0x20, 0x80, 0x360, 0x180);
        }
        for (i = 0, md = D_801EA530; i < 4; i++, md++) {
            md->unk8.vz += work->unk2C;
        }
        if (work->unk50 != 0) {
            work->unk50--;
            D_801EA5F0.unk8.vx = D_801EA658.vx + 0x800;
            D_801EA5F0.unk8.vy = D_801EA658.vy;
            D_801EA5F0.unk8.vz += 0x40;
            FX1F2_DRAW(D_801EA5F0);
            if (work->unk50 > 0x70) {
                c = (0x40 - work->unk50) * 0x10;
                ex->Hi_ModifyEffModelRGB(D_801EA5F0.unk0, c, c, c);
            }
            if (work->unk50 < 0x20) {
                c = (work->unk50 - 0x20) * 8;
                ex->Hi_ModifyEffModelRGB(D_801EA5F0.unk0, c, c, c);
            }
            ex->func_800E6624(frame * 8, 0x1C0, 0x100, 0x10, 0x80, 0x1D0, 0x100);
        }
        if (work->unk54 != 0) {
            work->unk54--;
            D_801EA620.unk8.vx = D_801EA658.vx + 0x800;
            D_801EA620.unk8.vy = D_801EA658.vy;
            D_801EA620.unk8.vz += 0x40;
            FX1F2_DRAW(D_801EA620);
            if (work->unk54 > 0x2A) {
                c = (0x2A - work->unk54) << 5;
                ex->Hi_ModifyEffModelRGB(D_801EA620.unk0, c, c, c);
            }
            ex->func_800E6624(frame * 8, 0x1C0, 0x100, 0x10, 0x80, 0x1D0, 0x100);
        }
        if (work->unkC != 0) {
            work->unkC--;
            c = func_801E7794(1, 0x20, work->unkC, 0, 0xFF);
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 2, c, c, c);
        }
        if (work->unk10 != 0) {
            work->unk10--;
            c = func_801E7794(2, 0x10, work->unk10, 0xFF, 0);
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 2, c, c, c);
        }
        if (work->unk14 != 0) {
            work->unk14--;
            c = func_801E7794(1, 0x10, work->unk14, 0, 0xFF);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 8), 1, c, c, c);
        }
        if (work->unk18 != 0) {
            work->unk18--;
            c = func_801E7794(0, 0x10, work->unk18, 0xFF, 0);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 8), 1, c, c, c);
        }
        if (work->unk20 != 0) {
            work->unk20--;
            scale.vx = scale.vy = scale.vz = work->unk20 * 3 + 0x1000;
            col.r = col.g = col.b = work->unk20 * 2;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->func_800E6764(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, NULL);
        }
        if (work->unk34 != 0) {
            work->unk34--;
            for (i = 0, tr = D_801EA690; i < 0x10; i++, tr++) {
                if (tr->unk4C == 0 && work->unk34 >= 0xD) {
                    ex->func_800316D8(&D_801EA568, &m);
                    m.t[0] = D_801EA650.vx;
                    m.t[1] = D_801EA650.vy;
                    m.t[2] = D_801EA650.vz;
                    gte_SetRotMatrix(&m);
                    gte_SetTransMatrix(&m);
                    for (j = 0, lo = 0, hi = -0x1800, r = -0xC00; j < 2; j++, lo += 0x2400, hi += 0x2400, r += 0x2400) {
                        if (i & 1) {
                            in[j].vx = ex->btlfx_randRange(0xC00 - lo, 0x1800 - lo);
                        } else {
                            in[j].vx = ex->btlfx_randRange(hi, r);
                        }
                        in[j].vy = (ex->btlfx_rand() & 0x1FFF) - 0x1000;
                        in[j].vz = (ex->btlfx_rand() & 0x1FFF) - 0x1000;
                        gte_ldv0(&in[j]);
                        gte_rtv0tr();
                        gte_stlvnl(&out[j]);
                        gte_stflg(&flag);
                    }
                    tr->unk4C = (ex->btlfx_rand() & 3) + 8;
                    step.vx = (out[0].vx - out[1].vx) / 9;
                    step.vy = (out[0].vy - out[1].vy) / 9;
                    step.vz = (out[0].vz - out[1].vz) / 9;
                    for (j = 0; j < 9; j++) {
                        tr->unk50[j].vx = out[0].vx;
                        tr->unk50[j].vy = out[0].vy;
                        tr->unk50[j].vz = out[0].vz;
                        tr->unkD0[j].vx = ex->btlfx_randRange(-0x80, 0x80);
                        tr->unkD0[j].vy = ex->btlfx_randRange(-0x80, 0x80);
                        tr->unkD0[j].vz = ex->btlfx_randRange(-0x80, 0x80);
                        out[0].vx -= step.vx;
                        out[0].vy -= step.vy;
                        out[0].vz -= step.vz;
                    }
                }
            }
            env->unk28 = 0x80;
            for (tr = D_801EA690; tr < &D_801EA690[0x10]; tr++) {
                if (tr->unk4C != 0) {
                    if (--tr->unk4C < 8) {
                        tr->unk0 = tr->unk2 = 8;
                        tr->unk1 = 0;
                        for (j = 0; j < 9; j++) {
                            tr->unk50[j].vx += tr->unkD0[j].vx;
                            tr->unk50[j].vy += tr->unkD0[j].vy;
                            tr->unk50[j].vz += tr->unkD0[j].vz;
                            tr->unk4[j].vx = tr->unk50[j].vx;
                            tr->unk4[j].vy = tr->unk50[j].vy;
                            tr->unk4[j].vz = tr->unk50[j].vz;
                            tr->unk4[j].pad = work->unk38;
                        }
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 6), 0, NULL, frame, 0x1000,
                                            tr->unk4C << 9, 1, (u8 *)tr, 0);
                    }
                }
            }
            env->unk28 = 0;
        }
        if (work->unk4C != 0) {
            work->unk4C--;
            for (i = 0, tr = D_801EA690; i < 0x10; i++, tr++) {
                if (i >= 8) {
                    if (tr->unk4C != 0) {
                        if (--tr->unk4C < 8) {
                            if (i & 1) {
                                ex->func_800DC8AC(0x10, 5, (BtlBone *)&m);
                                m.t[0] = work->unk110.vx;
                                m.t[1] = work->unk110.vy;
                                m.t[2] = work->unk110.vz;
                                c = 0x200;
                            } else {
                                ex->func_800DC8AC(0x10, 9, (BtlBone *)&m);
                                m.t[0] = work->unk118.vx;
                                m.t[1] = work->unk118.vy;
                                m.t[2] = work->unk118.vz;
                                c = 0x180;
                            }
                            gte_SetRotMatrix(&m);
                            gte_SetTransMatrix(&m);
                            tr->unk0 = tr->unk2 = 8;
                            tr->unk1 = 0;
                            for (j = 0, a = 0; j < 9; j++, a += 0x60) {
                                in[0].vx = tr->unk50[j].vx + a - 0x180;
                                in[0].vz = tr->unk50[j].vz + c;
                                in[0].vy = tr->unk50[j].vy;
                                gte_ldv0(&in[0]);
                                gte_rtv0tr();
                                gte_stlvnl(&out[0]);
                                gte_stflg(&flag2);
                                tr->unk4[j].vx = out[0].vx;
                                tr->unk4[j].vy = out[0].vy;
                                tr->unk4[j].pad = 0x40;
                                tr->unk4[j].vz = out[0].vz;
                                tr->unk50[j].vx += tr->unkD0[j].vx;
                                tr->unk50[j].vy += tr->unkD0[j].vy;
                                tr->unk50[j].vz += tr->unkD0[j].vz;
                            }
                            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, (i & 1) ? 0xC : 6), 0, NULL,
                                                frame, 0x1000, 0x800, 1, (u8 *)tr, 0);
                        }
                    } else {
                        tr->unk4C = (ex->btlfx_rand() & 3) + 8;
                        for (j = 0; j < 9; j++) {
                            tr->unk50[j].vx = 0;
                            tr->unk50[j].vy = 0;
                            tr->unk50[j].vz = 0;
                            tr->unkD0[j].vx = (ex->btlfx_rand() & 0xF) - 8;
                            tr->unkD0[j].vy = (ex->btlfx_rand() & 0x1F) - 0x10;
                            tr->unkD0[j].vz = (ex->btlfx_rand() & 0x1F) - 0x10;
                        }
                    }
                } else {
                    tr->unk0 = tr->unk2 = 8;
                    tr->unk1 = 0;
                    a = tr->unk4C;
                    tr->unk4C = tr->unk50[a].pad;
                    for (j = 0; j < 9; j++) {
                        tr->unk4[j].vx = tr->unk50[a].vx;
                        tr->unk4[j].vy = tr->unk50[a].vy;
                        tr->unk4[j].pad = 0x80;
                        tr->unk4[j].vz = tr->unk50[a].vz;
                        a = tr->unk50[a].pad;
                    }
                    env->unk28 = 0x80;
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, (i & 1) ? 0xC : 6), 0, NULL, frame,
                                        0x1000, 0xFFF, 1, (u8 *)tr, 0);
                    env->unk28 = 0;
                }
            }
        }
        if (work->unk58 != 0) {
            work->unk58--;
            env->unk28 = 0x40;
            for (i = 0, tr = D_801EBB90; i < env->unkC->unk24; i++, tr++) {
                if (tr->unk4C != 0) {
                    if (--tr->unk4C < 8) {
                        tr->unk0 = tr->unk2 = 8;
                        tr->unk1 = 0;
                        for (j = 0; j < 9; j++) {
                            tr->unk50[j].vx += tr->unkD0[j].vx;
                            tr->unk50[j].vy += tr->unkD0[j].vy;
                            tr->unk50[j].vz += tr->unkD0[j].vz;
                            tr->unk4[j].vx = tr->unk50[j].vx;
                            tr->unk4[j].vy = tr->unk50[j].vy;
                            tr->unk4[j].pad = 0x80;
                            tr->unk4[j].vz = tr->unk50[j].vz;
                        }
                        c = 0x1000;
                        if (work->unk58 < 8) {
                            c = work->unk58 << 9;
                        }
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 6), 0, NULL, frame, 0x1000, c, 1,
                                            (u8 *)tr, 0);
                    }
                } else {
                    tr->unk4C = 8;
                    out[0].vz = D_801EA670[i].vx + ex->btlfx_randRange(-0x40, 0x40);
                    out[1].vx = D_801EA670[i].vz + ex->btlfx_randRange(-0x40, 0x40);
                    for (j = 0; j < 9;) {
                        a = j + 1;
                        tr->unk50[j].vx = out[0].vz + ex->btlfx_randRange(a * -2, a * 2);
                        tr->unk50[j].vz = out[1].vx + ex->btlfx_randRange(a * -2, a * 2);
                        tr->unk50[j].vy = ex->btlfx_randRange(a * -4, 0) - (j << 6);
                        tr->unkD0[j].vx = ex->btlfx_randRange(a * -2, a * 2);
                        tr->unkD0[j].vz = ex->btlfx_randRange(a * -2, a * 2);
                        j = a;
                        tr->unkD0[j - 1].vy = -ex->btlfx_rand() & 7;
                    }
                }
            }
            env->unk28 = 0;
        }
        D_801EA3E0.unk8.vy += 3;
    }
    return 0;
}
