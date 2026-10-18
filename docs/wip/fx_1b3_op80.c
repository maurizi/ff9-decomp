/* Draft of fx_1b3 fx_1b3_op80 (Fx1b3Split/Fx1b3Blast in include/fxmod/fx_1b3.h). Status: first draft from m2c, 716 lines, 512 ignoring registers (build/w/nd.py); frame 192 bytes vs the original 176. Hi_RegisterTexListModel is called with three arguments (a3 left over), see REGTEX. The frame switch is sparse (a compare tree, bodies in ascending case order). */
#include "fxmod/fx_1b3.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[9]; /* drawn points */
    s32 unk4C; /* frames left */
    SVECTOR unk50[9]; /* points */
    SVECTOR unk98[9]; /* point velocities */
} Fx1b3Trail; /* 0xE0 */

typedef struct {
    s32 unk0[32]; /* timers: 0 tint, 1 flash, 2 trails, 3 trail kind, 4 sweep, 5 sweep split, 6..0xD */
    Pool6CEntry *unk80[32];
    SVECTOR unk100; /* body position */
    SVECTOR unk108; /* unit position */
    SVECTOR unk110;
    SVECTOR unk118;
    SVECTOR unk120;
    SVECTOR unk128;
    SVECTOR unk130;
    SVECTOR unk138;
    SVECTOR unk140;
    SVECTOR unk148;
    SVECTOR unk150;
    s32 unk158; /* model */
    SVECTOR unk15C;
    SVECTOR unk164;
    VECTOR unk16C;
    VECTOR unk17C;
    SVECTOR unk18C;
    SVECTOR unk194;
    VECTOR unk19C;
    VECTOR unk1AC;
    VECTOR unk1BC;
    SVECTOR unk1CC;
    VECTOR unk1D4;
    VECTOR unk1E4;
    SVECTOR unk1F4;
    SVECTOR unk1FC;
    SVECTOR unk204;
    VECTOR unk20C;
    VECTOR unk21C;
    VECTOR unk22C;
    SVECTOR unk23C;
    VECTOR unk244;
    VECTOR unk254;
} Fx1b3Work80; /* 0x264 */

extern s32 D_801EAFC4[];
extern s32 D_801EB004[];
extern s32 D_801EB018[];
extern BtlTexInfo D_801EB084;
extern BtlTexInfo D_801EB0A4;
extern u8 *D_801EB0BC;
extern Fx1b3Trail *D_801EB0C4;
extern s32 D_801EB0C8;
extern s32 D_801EB0CC;
extern Fx1b3Split D_801EB0F4[9];
extern Fx1b3Split D_801EB4A4[2];
extern Fx1b3Split D_801EB574;
extern Fx1b3Split D_801EB5E4[2];
extern Fx1b3Split D_801EB6B4[3];
extern SVECTOR D_801EB7EC;
extern BtlModel D_801EB7F4[];

/* Hi_RegisterTexListModel called with three arguments (a3 is left as it is) */
#define REGTEX(id) \
    ((s32(*)(BtlUnit9C8 *, BtlTexInfo *, s16 *))ex->Hi_RegisterTexListModel)( \
        (BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, id), &tex, clut.unk0[0])

#define SET_ROT(p, r)              \
    if ((p) != NULL) {             \
        (p)->unk5C.vx = (r).vx;    \
        (p)->unk5C.vy = (r).vy;    \
        (p)->unk5C.vz = (r).vz;    \
    }

s32 fx_1b3_op80(s32 mode, Fx1b3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlTexInfo tex;
    BtlTexInfo clut;
    Pool6CEntry *p;
    Fx1b3Trail *tr;
    s32 frame;
    s32 i;
    s32 j;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x264;
    } else if (mode == 1) {
        D_801EB074 = env;
        ex->btlfx_initEffModels(D_801EB7F4, 0x14);
        for (i = 0; i < 32; i++) {
            work->unk0[i] = 0;
            work->unk80[i] = NULL;
        }
        D_801EB0C4 = (Fx1b3Trail *)ex->btlfx_getFileData(entry->unk8, 0x93);
        D_801EB0C8 = (s32)D_801EB0C4 + 0xE00;
        D_801EB0CC = D_801EB0C8 + 0xF30;
        D_801EB0BC = (u8 *)D_801EB0CC + 0x2448;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&D_801EB7EC);
        D_801EB7EC.vy = -0x514;
        D_801EB7EC.vz += 0x800;
        env->unk2C->unk0[1].vx = D_801EB7EC.vx;
        env->unk2C->unk0[1].vy = 0;
        env->unk2C->unk0[1].vz = 0;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk100);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk108);
        tex.unk0[0][0] = 0x8A;
        tex.unk0[0][1] = 0x3800;
        tex.unk0[1][0] = 0x8C;
        tex.unk0[1][1] = 0x3840;
        clut.unk0[0][0] = 0;
        clut.unk0[1][0] = 0;
        clut.unk0[0][1] = 0;
        clut.unk0[1][1] = 0;
        work->unk158 = REGTEX(0x81);
        tex.unk0[0][0] = 0x8E;
        tex.unk0[1][0] = 0x8F;
        tex.unk0[0][1] = 0x3900;
        tex.unk0[1][1] = 0x3980;
        tex.unk0[2][1] = 0x39C0;
        clut.unk0[2][0] = 0x30;
        tex.unk0[2][0] = 0x8C;
        clut.unk0[0][0] = 0;
        clut.unk0[1][0] = 0;
        clut.unk0[0][1] = 0;
        clut.unk0[1][1] = 0;
        clut.unk0[2][1] = 0x80;
        work->unk16C.vx = 0;
        work->unk16C.vy = 0;
        work->unk16C.vz = 0;
        work->unk164.vx = 0;
        work->unk164.vy = 0;
        work->unk164.vz = 0;
        work->unk17C.vx = 0x1000;
        work->unk17C.vy = 0x1000;
        work->unk17C.vz = 0x1000;
        work->unk15C.vx = 0;
        work->unk15C.vy = 0;
        work->unk15C.vz = 0;
        D_801EB0F4[0].unk0 = REGTEX(0x85);
        D_801EB0F4[1].unk0 = REGTEX(0x8A);
        D_801EB0F4[2].unk0 = REGTEX(0x89);
        D_801EB0F4[3].unk0 = REGTEX(0x8F);
        D_801EB0F4[4].unk0 = REGTEX(0x86);
        D_801EB0F4[5].unk0 = REGTEX(0x87);
        D_801EB0F4[6].unk0 = REGTEX(0x83);
        D_801EB0F4[7].unk0 = REGTEX(0x90);
        D_801EB0F4[8].unk0 = REGTEX(0x92);
        D_801EB0F4[5].unk4 = 0x40;
        D_801EB0F4[4].unk4 = 0x40;
        D_801EB0F4[3].unk4 = 0x40;
        D_801EB0F4[2].unk4 = 0x40;
        D_801EB0F4[1].unk4 = 0x40;
        D_801EB0F4[0].unk4 = 0x40;
        D_801EB0F4[8].unk4 = 0x64;
        D_801EB0F4[7].unk4 = 0x64;
        D_801EB0F4[6].unk4 = 0x64;
        ex->func_800DD120(&D_801EB084, &D_801EB0A4, 0x2A0);
        tex.unk0[0][0] = D_801EB084.unk0[5][0];
        tex.unk0[0][1] = D_801EB084.unk0[5][1];
        clut.unk0[0][0] = D_801EB0A4.unk0[5][0];
        clut.unk0[0][1] = D_801EB0A4.unk0[5][1];
        D_801EB4A4[0].unk0 = REGTEX(0x91);
        D_801EB4A4[1].unk0 = REGTEX(0x8D);
        D_801EB4A4[1].unk4 = 0x80;
        D_801EB4A4[0].unk4 = 0x80;
        work->unk19C.vx = 0;
        work->unk19C.vy = 0;
        work->unk19C.vz = 0;
        work->unk18C.vx = 0;
        work->unk18C.vy = 0;
        work->unk18C.vz = 0;
        work->unk1BC.vx = 0x1000;
        work->unk1BC.vy = 0x1000;
        work->unk1BC.vz = 0x1000;
        tex.unk0[0][0] = D_801EB084.unk0[0][0];
        tex.unk0[0][1] = D_801EB084.unk0[0][1];
        clut.unk0[0][0] = D_801EB0A4.unk0[0][0];
        clut.unk0[0][1] = D_801EB0A4.unk0[0][1];
        D_801EB574.unk0 = REGTEX(0x8B);
        D_801EB574.unk4 = 0x80;
        work->unk1D4.vx = 0;
        work->unk1D4.vy = 0;
        work->unk1D4.vz = 0;
        work->unk1CC.vx = 0;
        work->unk1CC.vy = 0;
        work->unk1CC.vz = 0;
        work->unk1E4.vx = 0x1000;
        work->unk1E4.vy = 0x1000;
        work->unk1E4.vz = 0x1000;
        tex.unk0[0][0] = D_801EB084.unk0[3][0];
        tex.unk0[0][1] = D_801EB084.unk0[3][1];
        clut.unk0[0][0] = D_801EB0A4.unk0[3][0];
        clut.unk0[0][1] = D_801EB0A4.unk0[3][1];
        tex.unk0[1][0] = D_801EB084.unk0[1][0];
        tex.unk0[1][1] = D_801EB084.unk0[1][1];
        clut.unk0[1][0] = D_801EB0A4.unk0[1][0];
        clut.unk0[1][1] = D_801EB0A4.unk0[1][1];
        D_801EB5E4[0].unk0 = REGTEX(0x8E);
        D_801EB5E4[1].unk0 = REGTEX(0x8C);
        work->unk1FC.vx = 0;
        work->unk1FC.vy = 0;
        work->unk1FC.vz = 0;
        work->unk204.vx = 0;
        work->unk204.vy = 0;
        work->unk204.vz = 0;
        work->unk22C.vx = 0x1000;
        work->unk22C.vy = 0x1000;
        work->unk22C.vz = 0x1000;
        tex.unk0[0][0] = D_801EB084.unk0[3][0];
        tex.unk0[0][1] = D_801EB084.unk0[3][1];
        clut.unk0[0][0] = D_801EB0A4.unk0[3][0];
        clut.unk0[0][1] = D_801EB0A4.unk0[3][1];
        tex.unk0[1][0] = D_801EB084.unk0[2][0];
        tex.unk0[1][1] = D_801EB084.unk0[2][1];
        clut.unk0[1][0] = D_801EB0A4.unk0[2][0];
        clut.unk0[1][1] = D_801EB0A4.unk0[2][1];
        tex.unk0[2][0] = D_801EB084.unk0[0][0];
        tex.unk0[2][1] = D_801EB084.unk0[0][1];
        clut.unk0[2][0] = D_801EB0A4.unk0[0][0];
        clut.unk0[2][1] = D_801EB0A4.unk0[0][1];
        D_801EB6B4[0].unk0 = REGTEX(0x88);
        D_801EB6B4[1].unk0 = REGTEX(0x82);
        work->unk23C.vx = 0;
        work->unk23C.vy = 0;
        work->unk23C.vz = 0;
        work->unk254.vx = 0x1000;
        work->unk254.vy = 0x1000;
        work->unk254.vz = 0x1000;
        work->unk244.vx = work->unk108.vx;
        work->unk244.vy = work->unk108.vy;
        work->unk244.vz = work->unk108.vz;
        tex.unk0[0][0] = D_801EB084.unk0[4][0];
        tex.unk0[0][1] = D_801EB084.unk0[4][1];
        clut.unk0[0][0] = D_801EB0A4.unk0[4][0];
        clut.unk0[0][1] = D_801EB0A4.unk0[4][1];
        tex.unk0[1][0] = D_801EB084.unk0[0][0];
        tex.unk0[1][1] = D_801EB084.unk0[0][1];
        clut.unk0[1][0] = D_801EB0A4.unk0[0][0];
        clut.unk0[1][1] = D_801EB0A4.unk0[0][1];
        D_801EB6B4[2].unk0 = REGTEX(0x80);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            ex->func_800E83EC();
            work->unk0[0] = 0x10;
            break;
        case 8:
            p = work->unk80[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk100);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            p = work->unk80[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk100);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            break;
        case 0x10:
            work->unk0[1] = 0x90;
            p = work->unk80[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk100);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            break;
        case 0x30:
            work->unk0[1] = 0;
            work->unk0[2] = 0x40;
            work->unk0[3] = 0;
            for (i = 0; i < 16; i++) {
                D_801EB0C4[i].unk4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&D_801EB0C4[i], 0);
            }
            p = work->unk80[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk100);
            if (p != NULL) {
                p->unk14 = D_801EB0C8;
                work->unk80[3]->unk12 = 0x24;
                work->unk80[3]->unkC = 0x6C;
                p = work->unk80[3];
                if (p != NULL) {
                    p->unk22 = 0x400;
                }
            }
            break;
        case 0x60:
            work->unk0[2] = 0;
            ex->func_800E3D54(work->unk80[0]);
            ex->func_800E3D54(work->unk80[1]);
            ex->func_800E3D54(work->unk80[2]);
            ex->func_800E3D54(work->unk80[3]);
            work->unk0[13] = 0x20;
            D_801EB0C0 = D_801EB0BC;
            for (i = 0; i < 3; i++) {
                func_801EA39C(&D_801EB0F4[i]);
            }
            D_801EB0D4.unk0 = 0;
            D_801EB0D4.unk10 = 0;
            D_801EB0D4.unk14 = -0xA;
            work->unk0[4] = 0x3C;
            work->unk0[5] = 0;
            p = work->unk80[16] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk110);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            p = work->unk80[17] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk110);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x9C:
            work->unk0[4] = 0;
            ex->func_800E3D54(work->unk80[16]);
            ex->func_800E3D54(work->unk80[17]);
            work->unk0[13] = 0x20;
            D_801EB0C0 = D_801EB0BC;
            for (i = 0; i < 3; i++) {
                func_801EA39C(&D_801EB0F4[3 + i]);
            }
            D_801EB0D4.unk0 = 0;
            D_801EB0D4.unk10 = -0x800;
            D_801EB0D4.unk14 = 0xA;
            work->unk0[4] = 0x3C;
            work->unk0[5] = 3;
            p = work->unk80[18] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk110);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            p = work->unk80[19] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk110);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0xD8:
            work->unk0[4] = 0;
            ex->func_800E3D54(work->unk80[18]);
            ex->func_800E3D54(work->unk80[19]);
            work->unk0[13] = 0x20;
            D_801EB0C0 = D_801EB0BC;
            for (i = 0; i < 3; i++) {
                func_801EA39C(&D_801EB0F4[6 + i]);
            }
            D_801EB0D4.unk0 = 1;
            D_801EB0D4.unk10 = 0;
            D_801EB0D4.unk14 = 0xE;
            work->unk0[4] = 0x4B;
            work->unk0[5] = 6;
            p = work->unk80[20] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk110);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            p = work->unk80[21] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk110);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0xFE:
            work->unk0[2] = 0x48;
            work->unk0[3] = 1;
            for (i = 0; i < 16; i++) {
                D_801EB0C4[i].unk4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&D_801EB0C4[i], 0);
            }
            break;
        case 0x106:
            ex->func_800DC818(0x10, 0x3A, (u16 *)&work->unk118);
            p = work->unk80[4] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk118);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            break;
        case 0x11E:
            work->unk0[4] = 0;
            ex->func_800E3D54(work->unk80[20]);
            ex->func_800E3D54(work->unk80[21]);
            p = work->unk80[5] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk118);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            p = work->unk80[6] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk118);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            break;
        case 0x11F:
            ex->func_800E3D54(work->unk80[4]);
            work->unk0[11] = 0x10;
            D_801EB0C0 = D_801EB0BC;
            func_801EA39C(&D_801EB4A4[0]);
            work->unk19C.vx = work->unk108.vx;
            work->unk19C.vy = work->unk108.vy;
            work->unk19C.vz = work->unk108.vz;
            D_801EB0D4.unk6 = 0x990;
            D_801EB0D4.unk4 = 0x3CF;
            D_801EB0D4.unk8 = 0;
            work->unk0[6] = 0x80;
            break;
        case 0x136:
            ex->func_800DC818(0x10, 0x3A, (u16 *)&work->unk118);
            p = work->unk80[7] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk118);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            work->unk80[8] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk118);
            p = work->unk80[9] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x3A), (s32)&work->unk118);
            if (p != NULL) {
                p->unk14 = D_801EB0CC;
                work->unk80[9]->unk12 = 0x36;
                work->unk80[9]->unkC = 0xAC;
            }
            work->unk130.vy = 0x800;
            work->unk128.vx = 0;
            work->unk128.vy = 0;
            work->unk128.vz = 0;
            work->unk130.vx = 0;
            work->unk130.vz = 0;
            work->unk138.vx = 0;
            work->unk138.vy = 0x400;
            work->unk138.vz = 0;
            break;
        case 0x14F:
            work->unk0[2] = 0x48;
            work->unk0[3] = 2;
            for (i = 0; i < 16; i++) {
                D_801EB0C4[i].unk4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&D_801EB0C4[i], 0);
            }
            break;
        case 0x157:
            ex->func_800DC818(0x10, 0x18, (u16 *)&work->unk120);
            p = work->unk80[10] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk120);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            break;
        case 0x16F:
            work->unk0[6] = 0;
            ex->func_800E3D54(work->unk80[4]);
            ex->func_800E3D54(work->unk80[5]);
            ex->func_800E3D54(work->unk80[6]);
            p = work->unk80[11] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk120);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            p = work->unk80[12] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk120);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            break;
        case 0x170:
            ex->func_800E3D54(work->unk80[10]);
            work->unk0[11] = 0x10;
            D_801EB0C0 = D_801EB0BC;
            func_801EA39C(&D_801EB4A4[1]);
            func_801EA39C(&D_801EB574);
            work->unk0[7] = 0x80;
            work->unk1AC.vx = work->unk108.vx;
            work->unk1AC.vy = work->unk108.vy;
            work->unk1AC.vz = work->unk108.vz;
            work->unk1D4.vx = work->unk108.vx;
            work->unk1D4.vz = work->unk108.vz;
            work->unk1D4.vy = work->unk108.vy;
            break;
        case 0x187:
            ex->func_800E3D54(work->unk80[7]);
            ex->func_800E3D54(work->unk80[8]);
            ex->func_800E3D54(work->unk80[9]);
            ex->func_800DC818(0x10, 0x18, (u16 *)&work->unk120);
            p = work->unk80[13] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk120);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            work->unk80[14] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk120);
            p = work->unk80[15] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x39), (s32)&work->unk120);
            if (p != NULL) {
                p->unk14 = D_801EB0CC;
                work->unk80[15]->unk12 = 0x36;
                work->unk80[15]->unkC = 0xAC;
            }
            work->unk148.vx = -0x600;
            work->unk148.vy = -0x400;
            work->unk140.vx = 0;
            work->unk140.vy = 0;
            work->unk140.vz = 0;
            work->unk148.vz = 0;
            work->unk150.vx = 0;
            work->unk150.vy = 0xC00;
            work->unk150.vz = 0;
            break;
        case 0x1A8:
            work->unk0[2] = 0x30;
            work->unk0[3] = 3;
            for (i = 0; i < 16; i++) {
                D_801EB0C4[i].unk4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&D_801EB0C4[i], 0);
            }
            break;
        case 0x1C0:
            work->unk0[7] = 0;
            ex->func_800DC818(0x10, 0x18, (u16 *)&work->unk120);
            ex->func_800E3D54(work->unk80[10]);
            ex->func_800E3D54(work->unk80[11]);
            ex->func_800E3D54(work->unk80[12]);
            D_801EB0C0 = D_801EB0BC;
            func_801EA39C(&D_801EB5E4[0]);
            func_801EA39C(&D_801EB5E4[1]);
            work->unk0[8] = 0x3C;
            work->unk20C.vx = work->unk108.vx;
            work->unk20C.vy = work->unk108.vy;
            work->unk20C.vz = work->unk108.vz;
            work->unk21C.vx = work->unk108.vx;
            work->unk21C.vy = work->unk108.vy;
            work->unk21C.vz = work->unk108.vz;
            break;
        case 0x1C1:
            ex->func_800DC818(0x10, 3, (u16 *)&work->unk1F4);
            work->unk1F4.vx += 0x100;
            work->unk1F4.vy -= 0x80;
            p = work->unk80[22] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk1F4);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            p = work->unk80[23] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk1F4);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            break;
        case 0x1C2:
            work->unk0[11] = 0x10;
            work->unk0[9] = 0x22;
            break;
        case 0x1E2:
            for (i = 0; i < 3; i++) {
                func_801EA39C(&D_801EB6B4[i]);
            }
            break;
        case 0x1E3:
            p = work->unk80[24] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk100);
            if (p != NULL) {
                p->unk14 = D_801EB0C8;
                work->unk80[24]->unk12 = 0x24;
                work->unk80[24]->unkC = 0x6C;
                p = work->unk80[24];
                if (p != NULL) {
                    p->unk22 = 0x400;
                }
            }
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&D_801EB7EC);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&D_801EB7EC);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&D_801EB7EC);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 0x1E4:
            work->unk0[8] = 0;
            work->unk0[10] = 0xE0;
            break;
        case 0x1E5:
            ex->func_800E806C(0x10, 0);
            break;
        case 0x264:
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&D_801EB7EC);
            break;
        case 0x27C:
            work->unk0[12] = 0x20;
            break;
        case 0x29C:
            ex->func_800E83EC();
            return 1;
        }
        for (i = 0; i < 32; i++) {
            p = work->unk80[i];
            if (p != NULL && p->unk30 == -1) {
                work->unk80[i] = NULL;
            }
        }
        if (work->unk80[8] != NULL) {
            work->unk128.vx += 0x80;
            work->unk128.vy += 0x80;
            SET_ROT(work->unk80[8], work->unk128);
        }
        SET_ROT(work->unk80[9], work->unk130);
        SET_ROT(work->unk80[7], work->unk138);
        if (work->unk80[14] != NULL) {
            work->unk140.vx += 0x80;
            work->unk140.vy += 0x80;
            SET_ROT(work->unk80[14], work->unk140);
        }
        SET_ROT(work->unk80[15], work->unk148);
        SET_ROT(work->unk80[13], work->unk150);
        if (work->unk80[9] != NULL || work->unk80[15] != NULL) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xA), frame);
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            c = ex->btlfx_lerpRatio(0x40, 0, work->unk0[0], 0x10);
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, c, c, c);
        }
        if (work->unk0[12] != 0) {
            work->unk0[12]--;
            c = ex->btlfx_lerpRatio(0xFF, 0, work->unk0[12], 0x20);
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, c, c, c);
        }
        if (work->unk0[1] != 0) {
            work->unk0[1]--;
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, 0x40, 0x40, 0x40);
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            switch (work->unk0[3]) {
            case 0:
                for (i = 0; i < 16; i++) {
                    tr = &D_801EB0C4[i];
                    if (tr->unk4C == 0 && work->unk0[2] >= 0xD) {
                        SVECTOR pos;
                        SVECTOR d;

                        ex->func_800DC818(0x10, D_801EAFC4[ex->btlfx_rand() & 0xF], (u16 *)&pos);
                        tr->unk4C = (ex->btlfx_rand() & 7) + 4;
                        d.vx = ex->btlfx_randRange(-0x100, 0x100);
                        d.vy = ex->btlfx_randRange(-0x100, 0x100);
                        d.vz = -(ex->btlfx_rand() % 255) - 0x200;
                        for (j = 0; j < 9; j++) {
                            pos.vx += d.vx;
                            pos.vy += d.vy;
                            pos.vz += d.vz;
                            tr->unk50[j].vx = pos.vx;
                            tr->unk50[j].vy = pos.vy;
                            tr->unk50[j].vz = pos.vz;
                            tr->unk98[j].vx = ex->btlfx_randRange(-0x60, 0x60);
                            tr->unk98[j].vy = ex->btlfx_randRange(-0x60, 0x60);
                            tr->unk98[j].vz = 0;
                        }
                    }
                }
                break;
            case 1:
            case 2:
                for (i = 0; i < 5; i++) {
                    tr = &D_801EB0C4[i];
                    if (tr->unk4C == 0 && work->unk0[2] >= 0xD) {
                        s32 k;

                        tr->unk4C = (ex->btlfx_rand() & 3) + 4;
                        for (k = 0; k < 4; k++) {
                            SVECTOR a;
                            SVECTOR b;
                            s32 *tbl;
                            s32 m;

                            if (work->unk0[3] == 1) {
                                ex->func_800DC818(0x10, D_801EB004[k], (u16 *)&a);
                                tbl = D_801EB004;
                            } else {
                                ex->func_800DC818(0x10, D_801EB018[k], (u16 *)&a);
                                tbl = D_801EB018;
                            }
                            ex->func_800DC818(0x10, tbl[k + 1], (u16 *)&b);
                            b.vx += ex->btlfx_randRange(-0x100, 0x100);
                            b.vy += ex->btlfx_randRange(-0x100, 0x100);
                            b.vz += ex->btlfx_randRange(-0x100, 0x100);
                            b.vx = (s16)(b.vx - a.vx) >> 2;
                            b.vy = (s16)(b.vy - a.vy) >> 2;
                            b.vz = (s16)(b.vz - a.vz) >> 2;
                            for (m = 0; m < 2; m++) {
                                tr->unk50[k * 2 + m].vx = a.vx;
                                tr->unk50[k * 2 + m].vy = a.vy;
                                tr->unk50[k * 2 + m].vz = a.vz;
                                tr->unk98[k * 2 + m].vx = ex->btlfx_randRange(-0x20, 0x20);
                                tr->unk98[k * 2 + m].vz = ex->btlfx_randRange(-0x20, 0x20);
                                tr->unk98[k * 2 + m].vy = ex->btlfx_randRange(-4, 4);
                                a.vx += b.vx;
                                a.vy += b.vy;
                                a.vz += b.vz;
                            }
                            tr->unk98[8].vx = 0;
                            tr->unk98[8].vz = 0;
                            tr->unk98[8].vy = 0;
                            tr->unk50[8].vx = tr->unk50[7].vx;
                            tr->unk50[8].vy = tr->unk50[7].vy;
                            tr->unk50[8].vz = tr->unk50[7].vz;
                        }
                    }
                }
                break;
            case 3:
                for (i = 0; i < 6; i++) {
                    tr = &D_801EB0C4[i];
                    if (tr->unk4C == 0 && work->unk0[2] >= 0xD) {
                        SVECTOR pos;
                        s32 part;
                        s32 rad;
                        s32 ang;

                        tr->unk4C = (ex->btlfx_rand() & 3) + 4;
                        if (i & 1) {
                            part = 3;
                            rad = 0x200;
                        } else {
                            part = 9;
                            rad = 0x180;
                        }
                        ex->func_800DC818(0x10, part, (u16 *)&pos);
                        ang = ex->btlfx_rand() & 0xFFF;
                        for (j = 0; j < 8; j++) {
                            tr->unk50[j].vx = pos.vx + ((ex->math_sin(ang) * (rad + (ex->btlfx_rand() & 0x7F))) >> 12);
                            tr->unk50[j].vz = pos.vz + ((ex->math_cos(ang) * (rad + (ex->btlfx_rand() & 0x7F))) >> 12);
                            tr->unk50[j].vy = pos.vy + ex->btlfx_randRange(-0x40, 0x40);
                            tr->unk98[j].vx = ex->btlfx_randRange(-0x20, 0x20);
                            tr->unk98[j].vz = ex->btlfx_randRange(-0x20, 0x20);
                            tr->unk98[j].vy = ex->btlfx_randRange(-0x20, 0x20);
                            ang += 0x100;
                        }
                        tr->unk98[8].vx = 0;
                        tr->unk98[8].vz = 0;
                        tr->unk98[8].vy = 0;
                        tr->unk50[8].vx = tr->unk50[0].vx;
                        tr->unk50[8].vy = tr->unk50[0].vy;
                        tr->unk50[8].vz = tr->unk50[0].vz;
                    }
                }
                break;
            }
            env->unk28 = 0x40;
            for (i = 0; i < 16; i++) {
                tr = &D_801EB0C4[i];
                if (tr->unk4C != 0) {
                    tr->unk4C--;
                    if (tr->unk4C < 4) {
                        tr->unk2 = 8;
                        tr->unk0 = 8;
                        tr->unk1 = 0;
                        for (j = 0; j < 9; j++) {
                            tr->unk50[j].vx += tr->unk98[j].vx;
                            tr->unk50[j].vy += tr->unk98[j].vy;
                            tr->unk50[j].vz += tr->unk98[j].vz;
                            tr->unk4[j].vx = tr->unk50[j].vx;
                            tr->unk4[j].vy = tr->unk50[j].vy;
                            tr->unk4[j].vz = tr->unk50[j].vz;
                            tr->unk4[j].pad = 0x80;
                        }
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, NULL, frame, 0x1000,
                                            tr->unk4C << 10, 1, (u8 *)tr, 0);
                    }
                }
            }
            env->unk28 = 0;
        }
        if (work->unk0[4] != 0) {
            work->unk0[4]--;
            D_801EB0D4.unkC = 0x80;
            D_801EB0D4.unk4 = (ex->math_sin(D_801EB0D4.unk10) * 0x341) >> 9;
            D_801EB0D4.unk8 = -(ex->math_cos(D_801EB0D4.unk10) * 0x1A08) >> 12;
            D_801EB0D4.unk6 = 0;
            D_801EB0D4.unk10 += D_801EB0D4.unk14;
            work->unk110.vx = D_801EB0D4.unk4;
            work->unk110.vy = D_801EB0D4.unk6;
            work->unk110.vz = -D_801EB0D4.unk8;
            for (i = work->unk0[5]; i < work->unk0[5] + 3; i++) {
                if (D_801EB0F4[i].unk4 != 0) {
                    D_801EB0F4[i].unk4--;
                    func_801EA7E4(&D_801EB0F4[i]);
                    ex->Hi_DrawEffModel(&work->unk164, &work->unk16C, (s32)&work->unk17C, D_801EB0F4[i].unk0);
                }
            }
        }
        if (work->unk0[6] != 0) {
            work->unk0[6]--;
            D_801EB0D4.unk0 = 2;
            D_801EB0D4.unkC = 0x80;
            D_801EB0D4.unk6 -= 0x80;
            if (D_801EB4A4[0].unk4 != 0) {
                D_801EB4A4[0].unk4--;
                func_801EA7E4(&D_801EB4A4[0]);
                ex->Hi_DrawEffModel(&work->unk18C, &work->unk19C, (s32)&work->unk1BC, D_801EB4A4[0].unk0);
            }
        }
        if (work->unk0[7] != 0) {
            work->unk0[7]--;
            D_801EB0D4.unk0 = 2;
            D_801EB0D4.unkC = 0x80;
            D_801EB0D4.unk4 = -0x3CF;
            D_801EB0D4.unk8 = 0;
            D_801EB0D4.unk6 = 0x990 - ((0x80 - work->unk0[7]) << 7);
            if (D_801EB4A4[1].unk4 != 0) {
                D_801EB4A4[1].unk4--;
                func_801EA7E4(&D_801EB4A4[1]);
                ex->Hi_DrawEffModel(&work->unk194, &work->unk1AC, (s32)&work->unk1BC, D_801EB4A4[1].unk0);
            }
            D_801EB0D4.unk0 = 3;
            D_801EB0D4.unkC = 0x100;
            D_801EB0D4.unk6 = 0;
            D_801EB0D4.unk8 = 0;
            D_801EB0D4.unk4 = ((0x80 - work->unk0[7]) << 7) - 0x600;
            if (D_801EB574.unk4 != 0) {
                D_801EB574.unk4--;
                func_801EA7E4(&D_801EB574);
                ex->Hi_DrawEffModel(&work->unk1CC, &work->unk1D4, (s32)&work->unk1E4, D_801EB574.unk0);
            }
        }
        if (work->unk0[8] != 0) {
            work->unk0[8]--;
            if (work->unk0[9] != 0) {
                work->unk0[9]--;
                D_801EB0D4.unk0 = 4;
                D_801EB0D4.unkC = 0x100;
                D_801EB0D4.unk6 = 0x331;
                D_801EB0D4.unk8 = 0;
                D_801EB0D4.unk4 = 0x2BC - ((0x22 - work->unk0[9]) << 8);
                func_801EA7E4(&D_801EB5E4[0]);
                func_801EA7E4(&D_801EB5E4[1]);
            }
            ex->Hi_DrawEffModel(&work->unk1FC, &work->unk20C, (s32)&work->unk22C, D_801EB5E4[0].unk0);
            ex->Hi_DrawEffModel(&work->unk204, &work->unk21C, (s32)&work->unk22C, D_801EB5E4[1].unk0);
        }
        if (work->unk0[10] != 0) {
            work->unk0[10]--;
            D_801EB0D4.unk0 = 5;
            if (work->unk0[10] < 0x80) {
                D_801EB0D4.unkC = 0;
            } else {
                D_801EB0D4.unkC = (work->unk0[10] - 0x80) * 16;
            }
            D_801EB0D4.unk6 = 0x514;
            D_801EB0D4.unk4 = 0;
            D_801EB0D4.unk8 = 0x800;
            for (i = 0; i < 3; i++) {
                func_801EA7E4(&D_801EB6B4[i]);
                ex->Hi_DrawEffModel(&work->unk23C, &work->unk244, (s32)&work->unk254, D_801EB6B4[i].unk0);
            }
        }
        if (work->unk0[13] != 0) {
            work->unk0[13]--;
            switch (work->unk0[13]) {
            case 0x1F:
                ex->func_800E6624(0, 0x380, 0, 0x28, 0x50, 0x380, 0x50);
                break;
            case 0x17:
                ex->func_800E6624(0, 0x380, 0, 0x28, 0x50, 0x380, 0xA0);
                break;
            case 0x13:
                ex->func_800E6624(0, 0x380, 0, 0x28, 0x50, 0x3A8, 0);
                break;
            case 0xF:
                ex->func_800E6624(0, 0x380, 0, 0x28, 0x50, 0x3A8, 0x50);
                break;
            }
        }
        if (work->unk0[11] != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;

            work->unk0[11]--;
            col.r = col.g = col.b = work->unk0[11] * 7;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vx = sc.vy = sc.vz = work->unk0[11] * 10 + 0x1000;
            ex->func_800E6764(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &sc, &col, 1, NULL);
        }
        if (frame >= 0x61) {
            work->unk15C.vy++;
            ex->Hi_DrawEffModel(&work->unk15C, &work->unk16C, (s32)&work->unk17C, work->unk158);
        }
    }
    return 0;
}
