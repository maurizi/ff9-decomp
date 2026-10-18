#include "fxmod/fx_134.h"

void func_801E7748(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr) {
    POLY_FT4 *p = (POLY_FT4 *)D_801EC8E4->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC8E4);
    u8 col[4];
    s32 u1;
    s32 v1;
    s32 y1;

    D_801EC8E4->unk24 = (u8 *)(p + 2);
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

INCLUDE_ASM("asm/us/fx_134/nonmatchings/fx_134", func_801E7940);

void func_801E7B38(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC8E4);
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
    gte_SetRotMatrix(D_801EC8E4->unk14);
    gte_SetTransMatrix(D_801EC8E4->unk14);
    gte_ldv0(pos);
    p = (POLY_FT4 *)D_801EC8E4->unk24;
    D_801EC8E4->unk24 = (u8 *)(p + 1);
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

INCLUDE_ASM("asm/us/fx_134/nonmatchings/fx_134", func_801E7E48);

void func_801E8460(Unk80100C78Entry *entry, Fx134Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 pad) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC8E4);
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
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

INCLUDE_RODATA("asm/us/fx_134/nonmatchings/fx_134", fx_134_headerPtr);

INCLUDE_ASM("asm/us/fx_134/nonmatchings/fx_134", func_801E8658);

INCLUDE_ASM("asm/us/fx_134/nonmatchings/fx_134", fx_134_op80);
