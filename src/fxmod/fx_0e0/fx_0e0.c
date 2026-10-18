#include "fxmod/fx_0e0.h"

void func_801E7718(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr) {
    POLY_FT4 *p = (POLY_FT4 *)D_801EA918->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA918);
    u8 col[4];
    s32 u1;
    s32 v1;
    s32 y1;

    D_801EA918->unk24 = (u8 *)(p + 2);
    col[0] = col[1] = col[2] = c;
    if (abr != 0xFF) {
        col[3] = 0x2E;
    } else {
        col[3] = 0x2C;
    }
    tpage |= (abr & 3) << 5;
    v1 = v + h - 1;
    p->u0 = p->u2 = u;
    p->u1 = p->u3 = u1 = u + w - 1;
    p->v0 = p->v1 = v;
    p->v2 = p->v3 = v1;
    p->x0 = p->x2 = x - w;
    p->x1 = p->x3 = x;
    p->y0 = p->y1 = y;
    p->clut = clut;
    p->tpage = tpage;
    *(u32 *)&p->r0 = *(u32 *)col;
    y1 = y + h * 7 / 6;
    p->y2 = p->y3 = y1;
    ex->btlfx_addPrim(0x09000000, ot, p);
    p++;
    p->x0 = p->x2 = x;
    p->u0 = p->u2 = u1;
    p->u1 = p->u3 = u;
    p->v0 = p->v1 = v;
    p->v2 = p->v3 = v1;
    p->x1 = p->x3 = x + w;
    p->y0 = p->y1 = y;
    p->y2 = p->y3 = y1;
    p->clut = clut;
    p->tpage = tpage;
    *(u32 *)&p->r0 = *(u32 *)col;
    ex->btlfx_addPrim(0x09000000, ot, p);
}

INCLUDE_ASM("asm/us/fx_0e0/nonmatchings/fx_0e0", func_801E7910);

void func_801E7B08(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA918);
    MATRIX m;
    VECTOR sc;
    SVECTOR zero;
    SVECTOR vtx[4];
    u8 col[4];
    s32 otz;
    POLY_FT4 *p;
    s32 code;
    s32 a;
    s32 b;

    if (rot == NULL) {
        rot = &zero;
        ((s32 *)&zero)[1] = 0;
        ((s32 *)&zero)[0] = 0;
    }
    gte_SetRotMatrix(D_801EA918->unk14);
    gte_SetTransMatrix(D_801EA918->unk14);
    gte_ldv0(pos);
    p = (POLY_FT4 *)D_801EA918->unk24;
    D_801EA918->unk24 = (u8 *)(p + 1);
    col[0] = col[1] = col[2] = c >> 5;
    col[3] = (abr != 0xFF) ? 0x2E : 0x2C;
    tpage |= (abr & 3) << 5;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    sc.vx = sc.vy = sc.vz = scale;
    ex->ScaleMatrix(&m, &sc);
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        ex->MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    a = w << 6;
    b = -a;
    vtx[1].vx = vtx[3].vx = a;
    vtx[0].vx = vtx[2].vx = b;
    a = h << 6;
    b = -a;
    vtx[2].vy = vtx[3].vy = a;
    vtx[0].vz = vtx[1].vz = vtx[2].vz = vtx[3].vz = 0;
    vtx[0].vy = vtx[1].vy = b;
    gte_ldv3c(vtx);
    gte_rtpt();
    gte_nclip();
    p->tpage = tpage;
    p->clut = clut;
    gte_stopz(&otz);
    if (otz != 0) {
        gte_stsxy3(&p->x0, &p->x1, &p->x2);
        gte_avsz3();
        gte_stszotz(&otz);
        gte_ldv0(&vtx[3]);
        gte_rtps();
        p->u1 = p->u3 = u + w - 1;
        p->u0 = p->u2 = u;
        p->v0 = p->v1 = v;
        p->v2 = p->v3 = v + h - 1;
        *(u32 *)&p->r0 = *(u32 *)col;
        gte_stsxy(&p->x3);
        ex->btlfx_addPrim(0x09000000, otz, p);
    }
}

INCLUDE_ASM("asm/us/fx_0e0/nonmatchings/fx_0e0", func_801E7E18);

void func_801E8430(Unk80100C78Entry *entry, Fx0e0Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 pad) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA918);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 0x18;
    bolt->unk0 = 0x18;
    bolt->unk1 = 0;
    for (i = 0; i < 25; i++) {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0xA0);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = pad;
        ang += dang + ex->btlfx_randCentered(0x80);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

INCLUDE_RODATA("asm/us/fx_0e0/nonmatchings/fx_0e0", fx_0e0_headerPtr);

INCLUDE_ASM("asm/us/fx_0e0/nonmatchings/fx_0e0", func_801E8628);

INCLUDE_ASM("asm/us/fx_0e0/nonmatchings/fx_0e0", fx_0e0_op80);

s32 fx_0e0_op84(s32 mode, Fx0e0Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 k;
    s32 t;
    s32 d;
    u16 r;

    if (mode == 0) {
        *arg4 = 0x10;
    } else {
        if (mode == 1) {
            D_801EA918 = env;
            work->unk0 = 0;
            work->unk4 = 0;
        } else {
            r = c = b = a = 0;
            frame = entry->unk0;
            ex->func_800E7D3C(0x10, 1, (s16 *)&work->unk8);
            switch (work->unk0) {
            case 0:
                r = 0x200;
                a = (frame << 12) / 3;
                b = ex->math_sin(a >> 2) >> 1;
                c = ((0x1000 - a) >> 1) + 0x800;
                if (frame >= 3) {
                    work->unk0 = 1;
                    entry->unk0 = -1;
                }
                break;
            case 1:
                k = (frame & 1) << 8;
                c = k + 0x1000;
                d = (frame << 12) / 14;
                a = (k + 0x800) + (d >> 1);
                r = (d >> 3) + 0x200;
                b = c;
                if (frame >= 0xE) {
                    work->unk0 = 2;
                    entry->unk0 = -1;
                }
                break;
            case 2:
                d = (frame << 12) / 14;
                t = d >> 2;
                r = 0x400 - t;
                a = ex->math_cos(t);
                b = ex->math_sin(t) + 0x1000;
                c = ex->math_sin(t) * 2 + 0x1000;
                if (frame >= 0xE) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = r;
            rot.pad = 0;
            env->unk28 = 0x18;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unk8, NULL, b * 3 / 4, 0, 1, a / 2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x22), &work->unk8, &rot, c, 0, 1, a * 2 / 3);
            env->unk28 = 0;
            work->unk4++;
        }
    }
    return 0;
}
