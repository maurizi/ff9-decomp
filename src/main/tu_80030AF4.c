#include "ff9.h"

void math_rotMatrixXYZ(void *a, void *b) {
    math_buildRotMatrixXYZ(a, b, 0);
}

void math_rotMatrixXYZ_gte(void *a, void *b) {
    math_buildRotMatrixXYZ(a, b, 1);
}

MATRIX *math_buildRotMatrixXYZ(SVECTOR *r, MATRIX *m, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 tx, ty, tz;
    s16 t1, t2;

    x = r->vx & 0xFFF;
    y = r->vy & 0xFFF;
    z = r->vz & 0xFFF;
    ISIN(sx, r->vx, x);
    tx = 0x400 - x;
    x = tx & 0xFFF;
    ISIN(cx, tx, x);
    ISIN(sy, y, y);
    ty = 0x400 - y;
    y = ty & 0xFFF;
    ISIN(cy, ty, y);
    ISIN(sz, z, z);
    tz = 0x400 - z;
    z = tz & 0xFFF;
    ISIN(cz, tz, z);
    /* Both branches are identical; jump2 merges them, leaving only the dead copy of mode. */
    if (mode) {
        t1 = (sx * sy) >> 12;
        t2 = (cx * sy) >> 12;
        m->m[0][0] = (cy * cz) >> 12;
        m->m[0][1] = (-cy * sz) >> 12;
        m->m[0][2] = sy;
        m->m[1][0] = (t1 * cz + cx * sz) >> 12;
        m->m[1][1] = (-t1 * sz + cx * cz) >> 12;
        m->m[1][2] = (-sx * cy) >> 12;
        m->m[2][0] = (-t2 * cz + sx * sz) >> 12;
        m->m[2][1] = (t2 * sz + sx * cz) >> 12;
        m->m[2][2] = (cx * cy) >> 12;
    } else {
        t1 = (sx * sy) >> 12;
        t2 = (cx * sy) >> 12;
        m->m[0][0] = (cy * cz) >> 12;
        m->m[0][1] = (-cy * sz) >> 12;
        m->m[0][2] = sy;
        m->m[1][0] = (t1 * cz + cx * sz) >> 12;
        m->m[1][1] = (-t1 * sz + cx * cz) >> 12;
        m->m[1][2] = (-sx * cy) >> 12;
        m->m[2][0] = (-t2 * cz + sx * sz) >> 12;
        m->m[2][1] = (t2 * sz + sx * cz) >> 12;
        m->m[2][2] = (cx * cy) >> 12;
    }
    return m;
}

void math_rotMatrixZXY(void *a, void *b) {
    math_buildRotMatrixZXY(a, b, 0);
}

void math_rotMatrixZXY_gte(void *a, void *b) {
    math_buildRotMatrixZXY(a, b, 1);
}

MATRIX *math_buildRotMatrixZXY(SVECTOR *r, MATRIX *m, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 tx, ty, tz;
    s16 t1, t2;

    x = r->vx & 0xFFF;
    y = r->vy & 0xFFF;
    z = r->vz & 0xFFF;
    ISIN(sx, r->vx, x);
    tx = 0x400 - x;
    x = tx & 0xFFF;
    ISIN(cx, tx, x);
    ISIN(sy, y, y);
    ty = 0x400 - y;
    y = ty & 0xFFF;
    ISIN(cy, ty, y);
    ISIN(sz, z, z);
    tz = 0x400 - z;
    z = tz & 0xFFF;
    ISIN(cz, tz, z);
    /* Both branches are identical; jump2 merges them, leaving only the dead copy of mode. */
    if (mode) {
        t1 = (sx * sz) >> 12;
        t2 = (sx * cz) >> 12;
        m->m[0][0] = (cz * cy - t1 * sy) >> 12;
        m->m[0][1] = (-cx * sz) >> 12;
        m->m[0][2] = (cz * sy + t1 * cy) >> 12;
        m->m[1][0] = (sz * cy + t2 * sy) >> 12;
        m->m[1][1] = (cx * cz) >> 12;
        m->m[1][2] = (sz * sy - t2 * cy) >> 12;
        m->m[2][0] = (-cx * sy) >> 12;
        m->m[2][1] = sx;
        m->m[2][2] = (cx * cy) >> 12;
    } else {
        t1 = (sx * sz) >> 12;
        t2 = (sx * cz) >> 12;
        m->m[0][0] = (cz * cy - t1 * sy) >> 12;
        m->m[0][1] = (-cx * sz) >> 12;
        m->m[0][2] = (cz * sy + t1 * cy) >> 12;
        m->m[1][0] = (sz * cy + t2 * sy) >> 12;
        m->m[1][1] = (cx * cz) >> 12;
        m->m[1][2] = (sz * sy - t2 * cy) >> 12;
        m->m[2][0] = (-cx * sy) >> 12;
        m->m[2][1] = sx;
        m->m[2][2] = (cx * cy) >> 12;
    }
    return m;
}

void math_rotMatrixZYX(void *a, void *b) {
    math_buildRotMatrixZYX(a, b, 0);
}

void math_rotMatrixZYX_gte(void *a, void *b) {
    math_buildRotMatrixZYX(a, b, 1);
}

MATRIX *math_buildRotMatrixZYX(SVECTOR *r, MATRIX *m, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 t;
    s32 tx, ty, tz;
    s16 t1, t2;
    s32 a, b, c, d, e, f;

    x = r->vx & 0xFFF;
    y = r->vy & 0xFFF;
    z = r->vz & 0xFFF;
    ISIN(sx, r->vx, x);
    tx = 0x400 - x;
    x = tx & 0xFFF;
    ISIN(cx, tx, x);
    if (y & 0x400) {
        sy = g_sinTable[0x400 - (y & 0x3FF)];
    } else {
        sy = g_sinTable[y & 0x3FF];
    }
    if (y & 0x800) {
        /* identical branches (jump2 merges them), as in math_buildRotMatrixZXY */
        if (mode) {
            sy = -sy;
        } else {
            sy = -sy;
        }
    }
    ty = 0x400 - y;
    y = ty & 0xFFF;
    ISIN(cy, ty, y);
    ISIN(sz, z, z);
    tz = 0x400 - z;
    z = tz & 0xFFF;
    ISIN(cz, tz, z);
    if (mode == 0) {
        t1 = (sy * cz) >> 12;
        t2 = (sy * sz) >> 12;
        m->m[0][0] = (cy * cz) >> 12;
        m->m[0][1] = (-sz * cx + t1 * sx) >> 12;
        m->m[0][2] = (sz * sx + t1 * cx) >> 12;
        m->m[1][0] = (cy * sz) >> 12;
        m->m[1][1] = (cz * cx + t2 * sx) >> 12;
        m->m[1][2] = (-cz * sx + t2 * cx) >> 12;
        m->m[2][0] = -sy;
        m->m[2][1] = (cy * sx) >> 12;
        m->m[2][2] = (cy * cx) >> 12;
    } else {
        gte_ldir0123(cy, cz, sz, sx);
        m->m[2][0] = -sy;
        gte_gpf12_n1();
        a = cz * sx;
        gte_stmac123(d, e, f);
        gte_ldir0123(cx, cy, sz, cz);
        do {
            b = sz * sx;
            m->m[0][0] = d;
            gte_gpf12_b();
            a >>= 12;
            b >>= 12;
            m->m[1][0] = e;
            m->m[2][1] = f;
        } while (0);
        gte_stmac123(d, c, t);
        gte_ldir0123(sy, a, b, t);
        m->m[2][2] = d;
        gte_gpf12_n1();
        gte_stmac123(d, e, f);
        gte_ldir12(t, c);
        d -= c;
        m->m[0][1] = d;
        gte_gpf12_b();
        e += t;
        m->m[1][1] = e;
        gte_stmac12(d, e);
        d += b;
        e -= a;
        m->m[0][2] = d;
        m->m[1][2] = e;
    }
    return m;
}

void math_rotMatrixYXZ(void *a, void *b) {
    math_buildRotMatrixYXZ(a, b, 0);
}

void math_rotMatrixYXZ_gte(void *a, void *b) {
    math_buildRotMatrixYXZ(a, b, 1);
}

MATRIX *math_buildRotMatrixYXZ(SVECTOR *r, MATRIX *m, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 t;
    s32 tx, ty, tz;
    s16 t1, t2;
    s32 a, b, c, d, e, f;

    x = r->vx & 0xFFF;
    y = r->vy & 0xFFF;
    z = r->vz & 0xFFF;
    ISIN(sx, r->vx, x);
    tx = 0x400 - x;
    x = tx & 0xFFF;
    ISIN(cx, tx, x);
    ISIN(sy, y, y);
    ty = 0x400 - y;
    y = ty & 0xFFF;
    ISIN(cy, ty, y);
    ISIN(sz, z, z);
    tz = 0x400 - z;
    z = tz & 0xFFF;
    ISIN(cz, tz, z);
    if (mode == 0) {
        t1 = (sx * sy) >> 12;
        t2 = (sx * cy) >> 12;
        m->m[0][0] = (cy * cz + t1 * sz) >> 12;
        m->m[0][1] = (-cy * sz + t1 * cz) >> 12;
        m->m[0][2] = (cx * sy) >> 12;
        m->m[1][0] = (cx * sz) >> 12;
        m->m[1][1] = (cx * cz) >> 12;
        m->m[1][2] = -sx;
        m->m[2][0] = (-sy * cz + t2 * sz) >> 12;
        m->m[2][1] = (sy * sz + t2 * cz) >> 12;
        m->m[2][2] = (cx * cy) >> 12;
    } else {
        gte_ldir0123(cx, sy, cy, cz);
        m->m[1][2] = -sx;
        gte_gpf12_n1();
        a = cy * cz;
        gte_stmac123(d, e, f);
        gte_ldir0123(sz, cx, cy, sy);
        do {
            b = sy * cz;
            m->m[0][2] = d;
            gte_gpf12_b();
            a >>= 12;
            b >>= 12;
            m->m[2][2] = e;
            m->m[1][1] = f;
        } while (0);
        gte_stmac123(d, c, t);
        gte_ldir0123(sx, b, a, t);
        m->m[1][0] = d;
        gte_gpf12_n1();
        gte_stmac123(d, e, f);
        gte_ldir12(t, c);
        d -= c;
        m->m[0][1] = d;
        gte_gpf12_b();
        e += t;
        m->m[2][1] = e;
        gte_stmac12(d, e);
        d += a;
        e -= b;
        m->m[0][0] = d;
        m->m[2][0] = e;
    }
    return m;
}

void fadeFlat_start(FadeState *p) {
    s32 n;
    if (p->frames == 0) {
        p->frames = 16;
    }
    g_fadeFlat.unk24.unk0 = g_fadeFlat.unk0 = p->color.unk0[0];
    g_fadeFlat.unk24.unk4 = g_fadeFlat.unk4 = p->color.unk0[1];
    g_fadeFlat.unk24.unk8 = g_fadeFlat.unk8 = p->color.unk0[2];
    g_fadeFlat.unkC = p->endR;
    g_fadeFlat.unk10 = p->endG;
    g_fadeFlat.unk14 = p->endB;
    g_fadeFlat.unk3C = g_fadeFlat.unkC - g_fadeFlat.unk0;
    g_fadeFlat.unk40 = g_fadeFlat.unk10 - g_fadeFlat.unk4;
    g_fadeFlat.unk44 = g_fadeFlat.unk14 - g_fadeFlat.unk8;
    n = p->frames - 1;
    if (n <= 0) {
        n = 1;
    }
    g_fadeFlat.unk18 = fade_divRoundOut(g_fadeFlat.unk3C, n);
    n = p->frames - 1;
    if (n <= 0) {
        n = 1;
    }
    g_fadeFlat.unk1C = fade_divRoundOut(g_fadeFlat.unk40, n);
    n = p->frames - 1;
    if (n <= 0) {
        n = 1;
    }
    g_fadeFlat.unk20 = fade_divRoundOut(g_fadeFlat.unk44, n);
}

void fadeFlat_update(FadeState *p) {
    g_fadeFlat.unk24.unk0 = fade_stepClamp(g_fadeFlat.unk24.unk0, g_fadeFlat.unk18, g_fadeFlat.unkC);
    g_fadeFlat.unk24.unk4 = fade_stepClamp(g_fadeFlat.unk24.unk4, g_fadeFlat.unk1C, g_fadeFlat.unk10);
    g_fadeFlat.unk24.unk8 = fade_stepClamp(g_fadeFlat.unk24.unk8, g_fadeFlat.unk20, g_fadeFlat.unk14);
    if (p->eased != 0) {
        g_fadeFlat.unk30.unk0 = fade_ease(g_fadeFlat.unk24.unk0, g_fadeFlat.unk0, g_fadeFlat.unk3C);
        g_fadeFlat.unk30.unk4 = fade_ease(g_fadeFlat.unk24.unk4, g_fadeFlat.unk4, g_fadeFlat.unk40);
        g_fadeFlat.unk30.unk8 = fade_ease(g_fadeFlat.unk24.unk8, g_fadeFlat.unk8, g_fadeFlat.unk44);
    } else {
        g_fadeFlat.unk30 = g_fadeFlat.unk24;
    }
    p->color.unk0[0] = g_fadeFlat.unk30.unk0;
    p->color.unk0[1] = g_fadeFlat.unk30.unk4;
    p->color.unk0[2] = g_fadeFlat.unk30.unk8;
}

void fadeFlat_seek(FadeState *p, s32 n) {
    g_fadeFlat.unk24.unk0 = fade_stepClamp(g_fadeFlat.unk24.unk0, g_fadeFlat.unk18 * n, g_fadeFlat.unkC);
    g_fadeFlat.unk24.unk4 = fade_stepClamp(g_fadeFlat.unk24.unk4, g_fadeFlat.unk1C * n, g_fadeFlat.unk10);
    g_fadeFlat.unk24.unk8 = fade_stepClamp(g_fadeFlat.unk24.unk8, g_fadeFlat.unk20 * n, g_fadeFlat.unk14);
    if (p->eased != 0) {
        g_fadeFlat.unk30.unk0 = fade_ease(g_fadeFlat.unk24.unk0, g_fadeFlat.unk0, g_fadeFlat.unk3C);
        g_fadeFlat.unk30.unk4 = fade_ease(g_fadeFlat.unk24.unk4, g_fadeFlat.unk4, g_fadeFlat.unk40);
        g_fadeFlat.unk30.unk8 = fade_ease(g_fadeFlat.unk24.unk8, g_fadeFlat.unk8, g_fadeFlat.unk44);
    } else {
        g_fadeFlat.unk30 = g_fadeFlat.unk24;
    }
    p->color.unk0[0] = g_fadeFlat.unk30.unk0;
    p->color.unk0[1] = g_fadeFlat.unk30.unk4;
    p->color.unk0[2] = g_fadeFlat.unk30.unk8;
}

void fadeFlat_draw(FadeState *p) {
    u32 *ot = fade_getOt(g_gameState->unk1C->unk8);
    s32 tp = 0x40;
    TILE *t;
    u8 *dr;

    if (p->subtract == 0) {
        tp = 0x20;
    }
    t = (TILE *)g_fade.pkt;
    SetTile(t);
    t->r0 = g_fadeFlat.unk30.unk0;
    t->g0 = g_fadeFlat.unk30.unk4;
    t->b0 = g_fadeFlat.unk30.unk8;
    t->y0 = 0;
    t->x0 = 0;
    t->w = g_gameState->unkC->disp.w;
    t->h = g_gameState->unkC->disp.h;
    SetSemiTrans(t, 1);
    addPrim(ot, t);
    g_fade.pkt += sizeof(TILE);
    dr = g_fade.pkt;
    SetDrawTPage(dr, 0, 1, tp);
    addPrim(ot, dr);
    g_fade.pkt += sizeof(DR_TPAGE);
}
