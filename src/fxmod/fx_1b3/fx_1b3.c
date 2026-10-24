#include "fxmod/fx_1b3.h"

void func_801E773C(s32 t, s32 n, s32 from, s32 to, s32 arg4, s32 ot) {
    Unk800F78A4 *env = D_801EB074;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 c = ex->btlfx_lerpRatio(from, to, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + ot), arg4, c, c, c);
}

s32 func_801E77CC(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EB074);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

/* Hi_RegisterTexListModel called with three arguments (a3 is left as it is) */
#define REGTEX(id) \
    ((s32(*)(BtlUnit9C8 *, BtlTexInfo *, s16 *))ex->Hi_RegisterTexListModel)( \
        (BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, id), (BtlTexInfo *)v, ((BtlTexInfo *)w)->unk0[0])

#define SET_ROT(p, r)              \
    if ((p) != NULL) {             \
        (p)->unk5C.vx = (r).vx;    \
        (p)->unk5C.vy = (r).vy;    \
        (p)->unk5C.vz = (r).vz;    \
    }

/* Matching notes: the tex/clut lists, trail points and flash locals share three function-scope buffers (v, w, sc),
 * as the original stack offsets show; the trail file buffers are carved with one incremented local (buf); the trail
 * tables are read twice instead of through a selected pointer (cross-jumped calls); the draw loop writes the drawn
 * points through a pointer biv (pt) and bounds the loop with a local (n), with the drawTrail alpha precomputed. */
s32 fx_1b3_op80(s32 mode, Fx1b3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v[3]; /* texture list, trail points */
    SVECTOR w[3]; /* clut list, trail step, flash colour and rotation */
    VECTOR sc;
    Pool6CEntry *p;
    Fx1b3Trail *tr;
    s32 frame;
    s32 i;
    s32 j;
    s32 c;
    s32 zz1;
    s32 zz2;
    s32 zz3;

    if (mode == 0) {
        *arg4 = 0x264;
    } else if (mode == 1) {
        s32 buf;

        D_801EB074 = env;
        ex->btlfx_initEffModels(D_801EB7F4, 0x14);
        for (i = 0; i < 32; i++) {
            work->unk0[i] = 0;
            work->unk80[i] = NULL;
        }
        buf = (s32)ex->btlfx_getFileData(entry->unk8, 0x93);
        D_801EB0C4 = (Fx1b3Trail *)buf;
        buf += 0xE00;
        D_801EB0C8 = buf;
        buf += 0xF30;
        D_801EB0CC = buf;
        buf += 0x2448;
        D_801EB0BC = (u8 *)buf;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&D_801EB7EC);
        D_801EB7EC.vy = -0x514;
        D_801EB7EC.vz += 0x800;
        env->unk2C->unk0[1].vx = D_801EB7EC.vx;
        env->unk2C->unk0[1].vy = 0;
        env->unk2C->unk0[1].vz = 0;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk100);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk108);
        ((BtlTexInfo *)v)->unk0[0][0] = 0x8A;
        ((BtlTexInfo *)v)->unk0[1][0] = 0x8C;
        ((BtlTexInfo *)v)->unk0[0][1] = 0x3800;
        ((BtlTexInfo *)v)->unk0[1][1] = 0x3840;
        ((BtlTexInfo *)w)->unk0[0][0] = 0;
        ((BtlTexInfo *)w)->unk0[1][0] = 0;
        ((BtlTexInfo *)w)->unk0[0][1] = 0;
        ((BtlTexInfo *)w)->unk0[1][1] = 0;
        work->unk158 = REGTEX(0x81);
        ((BtlTexInfo *)v)->unk0[0][0] = 0x8E;
        ((BtlTexInfo *)v)->unk0[1][0] = 0x8F;
        ((BtlTexInfo *)v)->unk0[0][1] = 0x3900;
        ((BtlTexInfo *)v)->unk0[1][1] = 0x3980;
        ((BtlTexInfo *)v)->unk0[2][1] = 0x39C0;
        ((BtlTexInfo *)w)->unk0[2][0] = 0x30;
        ((BtlTexInfo *)v)->unk0[2][0] = 0x8C;
        ((BtlTexInfo *)w)->unk0[0][0] = 0;
        ((BtlTexInfo *)w)->unk0[1][0] = 0;
        ((BtlTexInfo *)w)->unk0[0][1] = 0;
        ((BtlTexInfo *)w)->unk0[1][1] = 0;
        ((BtlTexInfo *)w)->unk0[2][1] = 0x80;
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
        ((BtlTexInfo *)v)->unk0[0][0] = D_801EB084.unk0[5][0];
        ((BtlTexInfo *)v)->unk0[0][1] = D_801EB084.unk0[5][1];
        ((BtlTexInfo *)w)->unk0[0][0] = D_801EB0A4.unk0[5][0];
        ((BtlTexInfo *)w)->unk0[0][1] = D_801EB0A4.unk0[5][1];
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
        ((BtlTexInfo *)v)->unk0[0][0] = D_801EB084.unk0[0][0];
        ((BtlTexInfo *)v)->unk0[0][1] = D_801EB084.unk0[0][1];
        ((BtlTexInfo *)w)->unk0[0][0] = D_801EB0A4.unk0[0][0];
        ((BtlTexInfo *)w)->unk0[0][1] = D_801EB0A4.unk0[0][1];
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
        ((BtlTexInfo *)v)->unk0[0][0] = D_801EB084.unk0[3][0];
        ((BtlTexInfo *)v)->unk0[0][1] = D_801EB084.unk0[3][1];
        ((BtlTexInfo *)w)->unk0[0][0] = D_801EB0A4.unk0[3][0];
        ((BtlTexInfo *)w)->unk0[0][1] = D_801EB0A4.unk0[3][1];
        ((BtlTexInfo *)v)->unk0[1][0] = D_801EB084.unk0[1][0];
        ((BtlTexInfo *)v)->unk0[1][1] = D_801EB084.unk0[1][1];
        ((BtlTexInfo *)w)->unk0[1][0] = D_801EB0A4.unk0[1][0];
        ((BtlTexInfo *)w)->unk0[1][1] = D_801EB0A4.unk0[1][1];
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
        ((BtlTexInfo *)v)->unk0[0][0] = D_801EB084.unk0[3][0];
        ((BtlTexInfo *)v)->unk0[0][1] = D_801EB084.unk0[3][1];
        ((BtlTexInfo *)w)->unk0[0][0] = D_801EB0A4.unk0[3][0];
        ((BtlTexInfo *)w)->unk0[0][1] = D_801EB0A4.unk0[3][1];
        ((BtlTexInfo *)v)->unk0[1][0] = D_801EB084.unk0[2][0];
        ((BtlTexInfo *)v)->unk0[1][1] = D_801EB084.unk0[2][1];
        ((BtlTexInfo *)w)->unk0[1][0] = D_801EB0A4.unk0[2][0];
        ((BtlTexInfo *)w)->unk0[1][1] = D_801EB0A4.unk0[2][1];
        ((BtlTexInfo *)v)->unk0[2][0] = D_801EB084.unk0[0][0];
        ((BtlTexInfo *)v)->unk0[2][1] = D_801EB084.unk0[0][1];
        ((BtlTexInfo *)w)->unk0[2][0] = D_801EB0A4.unk0[0][0];
        ((BtlTexInfo *)w)->unk0[2][1] = D_801EB0A4.unk0[0][1];
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
        ((BtlTexInfo *)v)->unk0[0][0] = D_801EB084.unk0[4][0];
        ((BtlTexInfo *)v)->unk0[0][1] = D_801EB084.unk0[4][1];
        ((BtlTexInfo *)w)->unk0[0][0] = D_801EB0A4.unk0[4][0];
        ((BtlTexInfo *)w)->unk0[0][1] = D_801EB0A4.unk0[4][1];
        ((BtlTexInfo *)v)->unk0[1][0] = D_801EB084.unk0[0][0];
        ((BtlTexInfo *)v)->unk0[1][1] = D_801EB084.unk0[0][1];
        ((BtlTexInfo *)w)->unk0[1][0] = D_801EB0A4.unk0[0][0];
        ((BtlTexInfo *)w)->unk0[1][1] = D_801EB0A4.unk0[0][1];
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
            if (work->unk80[i] != NULL && work->unk80[i]->unk30 == -1) {
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
            zz3 = ex->btlfx_lerpRatio(0x40, 0, work->unk0[0], 0x10);
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, zz3, zz3, zz3);
        }
        if (work->unk0[12] != 0) {
            work->unk0[12]--;
            zz3 = ex->btlfx_lerpRatio(0xFF, 0, work->unk0[12], 0x20);
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, zz3, zz3, zz3);
        }
        if (work->unk0[1] != 0) {
            work->unk0[1]--;
            zz3 = 0x40;
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, zz3, zz3, zz3);
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            switch (work->unk0[3]) {
            case 0:
                for (i = 0; i < 16; i++) {
                    if (D_801EB0C4[i].unk4C == 0 && work->unk0[2] >= 0xD) {

                        ex->func_800DC818(0x10, D_801EAFC4[ex->btlfx_rand() & 0xF], (u16 *)&v[0]);
                        D_801EB0C4[i].unk4C = (ex->btlfx_rand() & 7) + 4;
                        w[0].vx = ex->btlfx_randRange(-0x100, 0x100);
                        w[0].vy = ex->btlfx_randRange(-0x100, 0x100);
                        w[0].vz = -(ex->btlfx_rand() % 255) - 0x200;
                        for (j = 0; j < 9; j++) {
                            v[0].vx += w[0].vx;
                            v[0].vy += w[0].vy;
                            v[0].vz += w[0].vz;
                            D_801EB0C4[i].unk50[j].vx = v[0].vx;
                            D_801EB0C4[i].unk50[j].vy = v[0].vy;
                            D_801EB0C4[i].unk50[j].vz = v[0].vz;
                            D_801EB0C4[i].unk98[j].vx = ex->btlfx_randRange(-0x60, 0x60);
                            D_801EB0C4[i].unk98[j].vy = ex->btlfx_randRange(-0x60, 0x60);
                            D_801EB0C4[i].unk98[j].vz = 0;
                        }
                    }
                }
                break;
            case 1:
            case 2:
                for (i = 0; i < 5; i++) {
                    if (D_801EB0C4[i].unk4C == 0 && work->unk0[2] >= 0xD) {

                        D_801EB0C4[i].unk4C = (ex->btlfx_rand() & 3) + 4;
                        for (zz2 = 0; zz2 < 4; zz2++) {

                            if (work->unk0[3] == 1) {
                                ex->func_800DC818(0x10, D_801EB004[zz2], (u16 *)&v[1]);
                                ex->func_800DC818(0x10, D_801EB004[zz2 + 1], (u16 *)&v[2]);
                            } else {
                                ex->func_800DC818(0x10, D_801EB018[zz2], (u16 *)&v[1]);
                                ex->func_800DC818(0x10, D_801EB018[zz2 + 1], (u16 *)&v[2]);
                            }
                            v[2].vx += ex->btlfx_randRange(-0x100, 0x100);
                            v[2].vy += ex->btlfx_randRange(-0x100, 0x100);
                            v[2].vz += ex->btlfx_randRange(-0x100, 0x100);
                            v[2].vx = (s16)(v[2].vx - v[1].vx) >> 2;
                            v[2].vy = (s16)(v[2].vy - v[1].vy) >> 2;
                            v[2].vz = (s16)(v[2].vz - v[1].vz) >> 2;
                            for (c = 0; c < 2; c++) {
                                D_801EB0C4[i].unk50[zz2 * 2 + c].vx = v[1].vx;
                                D_801EB0C4[i].unk50[zz2 * 2 + c].vy = v[1].vy;
                                D_801EB0C4[i].unk50[zz2 * 2 + c].vz = v[1].vz;
                                D_801EB0C4[i].unk98[zz2 * 2 + c].vx = ex->btlfx_randRange(-0x20, 0x20);
                                D_801EB0C4[i].unk98[zz2 * 2 + c].vz = ex->btlfx_randRange(-0x20, 0x20);
                                D_801EB0C4[i].unk98[zz2 * 2 + c].vy = ex->btlfx_randRange(-4, 4);
                                v[1].vx += v[2].vx;
                                v[1].vy += v[2].vy;
                                v[1].vz += v[2].vz;
                            }
                            D_801EB0C4[i].unk98[8].vx = 0;
                            D_801EB0C4[i].unk98[8].vz = 0;
                            D_801EB0C4[i].unk98[8].vy = 0;
                            D_801EB0C4[i].unk50[8].vx = D_801EB0C4[i].unk50[7].vx;
                            D_801EB0C4[i].unk50[8].vy = D_801EB0C4[i].unk50[7].vy;
                            D_801EB0C4[i].unk50[8].vz = D_801EB0C4[i].unk50[7].vz;
                        }
                    }
                }
                break;
            case 3:
                for (i = 0; i < 6; i++) {
                    if (D_801EB0C4[i].unk4C == 0 && work->unk0[2] >= 0xD) {
                        s32 ang;

                        D_801EB0C4[i].unk4C = (ex->btlfx_rand() & 3) + 4;
                        if (i & 1) {
                            ex->func_800DC818(0x10, 3, (u16 *)&v[0]);
                            c = 0x200;
                        } else {
                            ex->func_800DC818(0x10, 9, (u16 *)&v[0]);
                            c = 0x180;
                        }
                        ang = ex->btlfx_rand() & 0xFFF;
                        for (zz2 = 0; zz2 < 8; zz2++) {
                            D_801EB0C4[i].unk50[zz2].vx = v[0].vx + ((ex->math_sin(ang) * (c + (ex->btlfx_rand() & 0x7F))) >> 12);
                            D_801EB0C4[i].unk50[zz2].vz = v[0].vz + ((ex->math_cos(ang) * (c + (ex->btlfx_rand() & 0x7F))) >> 12);
                            D_801EB0C4[i].unk50[zz2].vy = v[0].vy + ex->btlfx_randRange(-0x40, 0x40);
                            D_801EB0C4[i].unk98[zz2].vx = ex->btlfx_randRange(-0x20, 0x20);
                            D_801EB0C4[i].unk98[zz2].vz = ex->btlfx_randRange(-0x20, 0x20);
                            D_801EB0C4[i].unk98[zz2].vy = ex->btlfx_randRange(-0x20, 0x20);
                            ang += 0x100;
                        }
                        D_801EB0C4[i].unk98[8].vx = 0;
                        D_801EB0C4[i].unk98[8].vz = 0;
                        D_801EB0C4[i].unk98[8].vy = 0;
                        D_801EB0C4[i].unk50[8].vx = D_801EB0C4[i].unk50[0].vx;
                        D_801EB0C4[i].unk50[8].vy = D_801EB0C4[i].unk50[0].vy;
                        D_801EB0C4[i].unk50[8].vz = D_801EB0C4[i].unk50[0].vz;
                    }
                }
                break;
            }
            env->unk28 = 0x40;
            for (i = 0; i < 16; i++) {
                if (D_801EB0C4[i].unk4C != 0) {
                    tr = &D_801EB0C4[i];
                    tr->unk4C--;
                    if (tr->unk4C < 4) {
                        s32 n;
                        SVECTOR *pt;

                        tr->unk2 = 8;
                        tr->unk0 = 8;
                        tr->unk1 = 0;
                        n = 9;
                        for (pt = tr->unk4, j = 0; j < n; j++, pt++) {
                            D_801EB0C4[i].unk50[j].vx += D_801EB0C4[i].unk98[j].vx;
                            D_801EB0C4[i].unk50[j].vy += D_801EB0C4[i].unk98[j].vy;
                            D_801EB0C4[i].unk50[j].vz += D_801EB0C4[i].unk98[j].vz;
                            pt->vx = D_801EB0C4[i].unk50[j].vx;
                            pt->vy = D_801EB0C4[i].unk50[j].vy;
                            pt->vz = D_801EB0C4[i].unk50[j].vz;
                            pt->pad = 0x80;
                        }
                        zz1 = D_801EB0C4[i].unk4C << 10;
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, NULL, frame, 0x1000,
                                            zz1, 1, (u8 *)&D_801EB0C4[i], 0);
                    }
                }
            }
            env->unk28 = 0;
        }
        if (work->unk0[4] != 0) {
            work->unk0[4]--;
            D_801EB0D4.unkC = 0x80;
            D_801EB0D4.unk4 = (u32)(ex->math_sin(D_801EB0D4.unk10) * 0x341) >> 9;
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
            D_801EB0D4.unk6 = 0x990 - (0x80 - work->unk0[7]) * 0x80;
            D_801EB0D4.unk4 = -0x3CF;
            D_801EB0D4.unk8 = 0;
            if (D_801EB4A4[1].unk4 != 0) {
                D_801EB4A4[1].unk4--;
                func_801EA7E4(&D_801EB4A4[1]);
                ex->Hi_DrawEffModel(&work->unk194, &work->unk1AC, (s32)&work->unk1BC, D_801EB4A4[1].unk0);
            }
            D_801EB0D4.unk0 = 3;
            D_801EB0D4.unkC = 0x100;
            D_801EB0D4.unk4 = (0x80 - work->unk0[7]) * 0x80 - 0x600;
            D_801EB0D4.unk6 = 0;
            D_801EB0D4.unk8 = 0;
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
                D_801EB0D4.unk4 = 0x2BC - (0x22 - work->unk0[9]) * 0x100;
                D_801EB0D4.unk8 = 0;
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

            work->unk0[11]--;
            ((CVECTOR *)&w[0])->r = ((CVECTOR *)&w[0])->g = ((CVECTOR *)&w[0])->b = work->unk0[11] * 7;
            sc.vz = sc.vy = sc.vx = work->unk0[11] * 10 + 0x1000;
            w[1].vx = 0;
            w[1].vy = 0;
            w[1].vz = 0;
            ex->func_800E6764(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &w[1], &sc, (CVECTOR *)&w[0], 1, NULL);
        }
        if (frame >= 0x61) {
            work->unk15C.vy++;
            ex->Hi_DrawEffModel(&work->unk15C, &work->unk16C, (s32)&work->unk17C, work->unk158);
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b3/nonmatchings/fx_1b3", fx_1b3_headerPtr);

void func_801EA39C(Fx1b3Split *sp) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EB074);
    SVECTOR *q;
    Fx1b3Piece *p;
    s32 i;
    s32 j;

    ex->Hi_SplitMdlVertex(sp->unk0, -1, (s32)D_801EB0C0);
    for (i = 0; i < 8; i++) {
        sp->unk8[i] = ex->Hi_GetMdlVertexPtr(sp->unk0, 0, (s32)D_801EB0C0, (s32 *)&sp->unk28[i], i);
    }
    D_801EB0C0 += ex->Hi_GetSplitMdlVertex(sp->unk0, -1);
    for (i = 0; i < 8; i++) {
        sp->unk48[i] = (Fx1b3Piece *)D_801EB0C0;
        D_801EB0C0 += sp->unk8[i] * 0x18;
    }
    for (i = 0; i < 8; i++) {
        p = sp->unk48[i];
        q = sp->unk28[i];
        switch (i) {
        case 0:
        case 2:
        case 4:
        case 6:
            for (j = 0; j < sp->unk8[i]; j++) {
                p->unk6 = -1;
                p->unk10 = q[0].vx;
                p->unk10 += q[1].vx;
                p->unk10 += q[2].vx;
                p->unk10 = (s16)(p->unk10 + q[3].vx) >> 2;
                p->unk12 = q[0].vy;
                p->unk12 += q[1].vy;
                p->unk12 += q[2].vy;
                p->unk12 = (s16)(p->unk12 + q[3].vy) >> 2;
                p->unk14 = q[0].vz;
                p->unk14 += q[1].vz;
                p->unk14 += q[2].vz;
                p->unk14 = (s16)(p->unk14 + q[3].vz) >> 2;
                p->unk8 = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkA = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkC = (ex->btlfx_rand() & 0xFF) - 0x80;
                p++;
                q += 4;
            }
            break;
        case 1:
        case 3:
        case 5:
        case 7:
            for (j = 0; j < sp->unk8[i]; j++) {
                p->unk6 = -1;
                p->unk10 = q[0].vx;
                p->unk10 += q[1].vx;
                p->unk10 = (s16)(p->unk10 + q[2].vx) / 3;
                p->unk12 = q[0].vy;
                p->unk12 += q[1].vy;
                p->unk12 = (s16)(p->unk12 + q[2].vy) / 3;
                p->unk14 = q[0].vz;
                p->unk14 += q[1].vz;
                p->unk14 = (s16)(p->unk14 + q[2].vz) / 3;
                p->unk8 = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkA = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkC = (ex->btlfx_rand() & 0xFF) - 0x80;
                p++;
                q += 3;
            }
            break;
        }
    }
}

void func_801EA7E4(Fx1b3Split *sp) {
    SVECTOR d[4];
    VECTOR out;
    VECTOR scale;
    MATRIX m;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EB074);
    Fx1b3Piece *p;
    SVECTOR *v;
    s32 i;
    s32 j;
    s32 dx;
    s32 dy;
    s32 dz;

    for (i = 0; i < 8; i++) {
        v = sp->unk28[i];
        p = sp->unk48[i];
        for (j = 0; j < sp->unk8[i]; j++) {
            if (p->unk6 > 0) {
                ex->math_rotMatrixYXZ(&p->unk8, &m);
                m.t[0] = p->unk10 + p->unk0;
                m.t[1] = p->unk12 + p->unk2;
                m.t[2] = p->unk14 + p->unk4;
                scale.vx = scale.vy = scale.vz = p->unk16;
                p->unk16 -= p->unkE;
                ex->ScaleMatrix(&m, &scale);
                gte_SetRotMatrix(&m);
                gte_SetTransMatrix(&m);
                d[0].vx = v[0].vx - p->unk10;
                d[0].vy = v[0].vy - p->unk12;
                d[0].vz = v[0].vz - p->unk14;
                gte_ldv0(&d[0]);
                gte_rtv0tr();
                d[1].vx = v[1].vx - p->unk10;
                d[1].vy = v[1].vy - p->unk12;
                d[1].vz = v[1].vz - p->unk14;
                gte_stlvnl(&out);
                gte_ldv0(&d[1]);
                gte_rtv0tr();
                v[0].vx = out.vx;
                v[0].vy = out.vy;
                v[0].vz = out.vz;
                d[2].vx = v[2].vx - p->unk10;
                d[2].vy = v[2].vy - p->unk12;
                d[2].vz = v[2].vz - p->unk14;
                gte_stlvnl(&out);
                gte_ldv0(&d[2]);
                gte_rtv0tr();
                v[1].vx = out.vx;
                v[1].vy = out.vy;
                v[1].vz = out.vz;
                p->unk6--;
                gte_stlvnl(&out);
                v[2].vx = out.vx;
                v[2].vy = out.vy;
                v[2].vz = out.vz;
                if (!(i & 1)) {
                    d[3].vx = v[3].vx - p->unk10;
                    d[3].vy = v[3].vy - p->unk12;
                    d[3].vz = v[3].vz - p->unk14;
                    gte_ldv0(&d[3]);
                    gte_rtv0tr();
                    gte_stlvnl(&out);
                    v[3].vx = out.vx;
                    v[3].vy = out.vy;
                    v[3].vz = out.vz;
                }
                p->unk10 += p->unk0;
                p->unk12 += p->unk2;
                p->unk14 += p->unk4;
            } else {
                dx = D_801EB0D4.unk4 - p->unk10;
                dz = D_801EB0D4.unk8 - p->unk14;
                dy = D_801EB0D4.unk6 - p->unk12;
                switch (D_801EB0D4.unk0) {
                case 0:
                    if (abs(dx) < D_801EB0D4.unkC && abs(dz) < D_801EB0D4.unkC) {
                        p->unk6 = 0x40;
                        p->unk16 = 0x1000;
                        p->unkE = 0x20;
                        p->unk0 = (ex->btlfx_rand() & 0x7F) + 0x20;
                        p->unk2 = (ex->btlfx_rand() & 0x3F) - 0x20;
                        p->unk4 = 0;
                    }
                    break;
                case 1:
                    if (abs(dx) < D_801EB0D4.unkC && abs(dz) < D_801EB0D4.unkC) {
                        p->unk6 = 0x40;
                        p->unk16 = 0x1000;
                        p->unkE = 0x20;
                        p->unk0 = (ex->btlfx_rand() & 0x1F) - 0x10;
                        p->unk2 = (ex->btlfx_rand() & 0x1F) - 0x10;
                        p->unk4 = -0x20 - (ex->btlfx_rand() & 0x7F);
                    }
                    break;
                case 2:
                    if (abs(dy) < D_801EB0D4.unkC) {
                        p->unk6 = 0x80;
                        p->unk0 = -(dx >> 3);
                        p->unk2 = -(dy >> 3);
                        p->unk4 = -(dz >> 3);
                        p->unk16 = 0x1000;
                        p->unkE = 0x10;
                    }
                    break;
                case 3:
                    if (abs(dx) < D_801EB0D4.unkC && abs(dz) < D_801EB0D4.unkC) {
                        p->unk6 = 0x80;
                        p->unk0 = -(dx >> 2) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        p->unk2 = -(ex->btlfx_rand() & 0x7F) + 0x40;
                        p->unk4 = -(dz >> 2) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        p->unk16 = 0x1000;
                        p->unkE = 0x10;
                    }
                    break;
                case 4:
                    if (abs(dx) < D_801EB0D4.unkC) {
                        p->unk6 = 0x22;
                        p->unk0 = -(dx >> 4) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        p->unk2 = -(ex->btlfx_rand() & 0x3F);
                        p->unk4 = -(dz >> 4) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        p->unk16 = 0x1000;
                        p->unkE = 0x10;
                    }
                    break;
                case 5:
                    if (D_801EB0D4.unkC < ex->btlfx_sqrt(dy * dy + dx * dx, 0xF)) {
                        p->unk6 = 0x40;
                        p->unk0 = dx >> 7;
                        p->unk2 = dy >> 7;
                        p->unk4 = -(dz >> 7);
                        p->unk16 = 0x1000;
                        p->unkE = 0x20;
                    }
                    break;
                }
            }
            if (!(i & 1)) {
                v += 4;
            } else {
                v += 3;
            }
            p++;
        }
    }
}
