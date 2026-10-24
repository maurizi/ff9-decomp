#include "ff9.h"

void func_80053924(void) {
}

void func_8005392C(void) {
    GameData *d = g_gameState->unk1C;
    d->unk568 = 0;
    d->unk56A = 0;
    d->unk56C = 0;
    d->battleResult = 0;
    d->unk56F = 0;
    d->rainCount = 0;
    d->unk571 = 0;
}

s32 chr_cureStatusBit(Unk800539FC *a, s32 b) {
    if (a->status & b) {
        a->status &= ~b;
        return 2;
    }
    return 1;
}

static inline s32 chr_cureStatusBit_inline(Unk800539FC *a, s32 b) {
    if (a->status & b) {
        a->status &= ~b;
        return 2;
    }
    return 1;
}

s32 chr_cureStatusMask(Character *a, s32 mask) {
    s32 r = 1;
    u32 i;

    for (i = 0; i < 8; i++) {
        u8 m = mask & (1 << i);
        if (m) {
            if (chr_cureStatusBit_inline(a, 1 << i) == 2) {
                r = 2;
            }
        }
    }
    return r;
}

u8 chr_hasStatus(Character *a, s32 b) {
    return (a->status & b) != 0;
}

s32 swirl_update(s16 arg0, s16 arg1) {
    s16 y;
    s16 x;
    s32 c;
    u8 i;
    s16 a, b;
    GameState *gs;

    x = arg0;
    y = arg1;
    g_swirlParams = g_swirlVariant ? &D_8006F2E0[1] : &D_8006F2E0[0];
    g_swirlOt = g_swirlPkt = g_swirlBuf + (g_gameState->unk1C->unk8 << 10);
    ClearOTag(g_swirlOt, 5);
    g_swirlPkt += 5;
    if (g_swirlFrame < 0x79) {
        if (g_swirlFrame == 0) {
            gs = g_gameState;
            gs->unk1C->unk0 &= ~0x20;
            *(u32 *)&gs->drawEnv[0].isbg = *(u32 *)&gs->drawEnv[1].isbg = 1;
            g_swirlFlash = 0;
            g_swirlFade = g_swirlParams->unkA;
            g_swirlAngle = a = g_swirlParams->unk0;
            g_swirlZoom = g_swirlParams->unk6;
            D_8007BD22 = b = g_swirlParams->unk2;
            if ((arg0 ^ arg1) & 1) {
                g_swirlAngle = -a;
                D_8007BD22 = -b;
            }
            fade_clear();
        }
        if (g_swirlFrame & 1) {
            x += x < 160 ? 1 : -1;
        }
        if (g_swirlFrame & 2) {
            y += y < g_gameState->drawEnv->clip.h / 2 ? 1 : -1;
        }
        if (g_swirlVariant) {
            c = math_sin(g_swirlFrame * 8);
            x += c * (160 - x) / 4096;
            y += c * (g_gameState->drawEnv->clip.h / 2 - y) / 4096;
        }
        g_swirlPkt = swirl_buildMesh(g_swirlPkt, x, y);
        i = g_gameState->unk1C->unk8;
        SetDrawMove(g_swirlPkt, &g_gameState->dispEnv[i].disp, g_gameState->drawEnv[i].clip.x, g_gameState->drawEnv[i].clip.y);
        addPrim(g_swirlOt, g_swirlPkt);
        g_swirlPkt += 8;
        swirl_drawMesh();
        if (g_swirlFrame >= g_swirlParams->unkC) {
            g_swirlFade -= g_swirlParams->unkE;
            if ((s16)g_swirlFade < 0) {
                g_swirlFade = 0;
            }
        }
        if (g_swirlFrame >= g_swirlParams->unk10) {
            swirl_drawFlash(x, y);
            g_swirlFlash += g_swirlParams->unk12;
            if ((s16)g_swirlFlash >= 0x1000) {
                g_swirlFlash = 0xFFF;
            }
        }
        g_swirlAngle += D_8007BD22;
        g_swirlZoom += g_swirlParams->unk8;
        if (g_swirlZoom > 0x1400) {
            g_swirlZoom = 0x1400;
        }
        setlen(g_swirlPkt, 2);
        g_swirlPkt[1] = _get_mask(1, 0);
        g_swirlPkt[2] = 0;
        addPrim(g_swirlOt, g_swirlPkt);
        g_swirlPkt += 4;
    }
    SetDrawEnv((DR_ENV *)g_swirlPkt, &g_gameState->drawEnv[g_gameState->unk1C->unk8]);
    addPrim(g_swirlOt, g_swirlPkt);
    g_swirlPkt += 16;
    setlen(g_swirlPkt, 2);
    g_swirlPkt[1] = _get_mask(0, 0);
    g_swirlPkt[2] = 0;
    addPrim(&g_swirlOt[3], g_swirlPkt);
    g_swirlPkt += 4;
    setaddr(&g_swirlOt[4], getaddr(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8]));
    setaddr(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], g_swirlOt);
    if (g_swirlFrame == 0) {
        DrawSync(0);
        setlen(g_swirlPkt, 2);
        g_swirlPkt[1] = _get_mask(1, 0);
        g_swirlPkt[2] = 0;
        DrawPrim(g_swirlPkt);
        g_swirlPkt += 3;
        setlen(g_swirlPkt, 1);
        g_swirlPkt[1] = get_mode(0, 0, 0x120);
        DrawPrim(g_swirlPkt);
        g_swirlPkt += 2;
        g_swirlPkt[0] = 0x03000000;
        g_swirlPkt[1] = GPU_CODE(GPU_TILE | GPU_ABE);
        g_swirlPkt[2] = 0;
        g_swirlPkt[3] = (g_gameState->drawEnv->clip.h << 16) | 0x140;
        DrawPrim(g_swirlPkt);
        g_swirlPkt += 4;
        setlen(g_swirlPkt, 2);
        g_swirlPkt[1] = _get_mask(0, 0);
        g_swirlPkt[2] = 0;
        DrawPrim(g_swirlPkt);
    }
    return g_swirlFrame++ > 0x78;
}

void swirl_drawFlash(s32 x, s32 y) {
    u32 *p;
    s32 i;
    u32 v;
    u8 c1, c2;
    u32 xy;
    u32 g2;

    p = g_swirlPkt;
    v = g_swirlFlash << 16;
    c1 = v >> 20;
    c2 = v >> 23;
    g2 = (c2 << 16) | (c2 << 8) | c2;
    setlen(p, 1);
    p[1] = get_mode(0, 1, 0x140);
    p += 7;
    for (i = 0; i < 4; i++) {
        p[0] = 0x06000000;
        p[1] = ((c1 << 16) | (c1 << 8) | c1) | GPU_CODE(GPU_POLY_G3 | GPU_ABE);
        p[2] = ((D_8006F310[i] * g_gameState->drawEnv->clip.h) << 16) | ((D_8006F308[i] * 320) & 0xFFFF);
        p[3] = (c1 << 16) | (c1 << 8) | c1;
        p[4] = ((D_8006F310[i + 1] * g_gameState->drawEnv->clip.h) << 16) | ((D_8006F308[i + 1] * 320) & 0xFFFF);
        p[5] = g2;
        p[6] = (y << 16) | (x & 0xFFFF);
        setaddr(p - 7, p);
        p += 7;
    }
    setaddr(p - 7, getaddr(&g_swirlOt[2]));
    setaddr(&g_swirlOt[2], g_swirlPkt);
    g_swirlPkt = p;
}

u32 *swirl_buildMesh(u32 *buf, s32 x, s32 y) {
    s16 xs[9];
    s16 ys[9];
    SVECTOR sv;
    VECTOR v;
    MATRIX m;
    s32 i;
    s32 dx, dy;
    s32 t;
    s32 ex, ey;
    s16 *py;
    s16 *q;
    DVECTOR *pts;
    WarpVert *p;

    g_swirlColumns = (DVECTOR *)buf;
    g_swirlMesh = (WarpVert *)((u8 *)buf + 0x24);
    buf = (u32 *)((u8 *)buf + 0x2AC);
    dx = g_gameState->dispEnv->disp.w - x;
    dy = g_gameState->dispEnv->disp.h - y;
    for (i = 0; i < 5; i++) {
        xs[4 - i] = x - (x * i) / 4;
        xs[4 + i] = x + (dx * i) / 4;
        ys[4 - i] = y - (y * i) / 4;
        ys[4 + i] = y + (dy * i) / 4;
    }
    for (i = 0; i < 5; i++) {
        t = math_cos(i << 8);
        pts = g_swirlColumns;
        pts[i].vx = x - (x * t) / 4096;
        pts[8 - i].vx = x + (dx * t) / 4096;
        pts[i].vy = y - (y * t) / 4096;
        pts[8 - i].vy = y + (dy * t) / 4096;
    }
    for (i = 1; i < 8; i++) {
        ex = ((pts[i].vx - xs[i]) * g_swirlZoom) / 4096;
        py = ys;
        ey = ((pts[i].vy - ys[i]) * g_swirlZoom) / 4096;
        pts[i].vx = xs[i] + ex;
        pts[i].vy = ys[i] + ey;
    }
    sv.vy = 0;
    sv.vx = 0;
    sv.vz = (s16)g_swirlAngle >> 4;
    math_rotMatrixXYZ(&sv, &m);
    v.vx = v.vy = g_swirlParams->unk4;
    v.vz = 0;
    ScaleMatrix(&m, &v);
    m.t[0] = x;
    m.t[1] = y;
    m.t[2] = 0;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    p = g_swirlMesh;
    for (ex = 0; ex < 9; ex++) {
        q = py;
        sv.vx = xs[ex] - x;
        for (ey = 0; ey < 9; ey++) {
            sv.vy = q[ey] - y;
            gte_ldv0(&sv);
            gte_rtv0tr();
            gte_stlvnl(&v);
            p->x = v.vx;
            p->y = v.vy;
            p++;
        }
    }
    return buf;
}

void swirl_drawMesh(void) {
    u32 *p;
    WarpVert *v;
    s32 i;
    s32 j;
    s32 c;
    u32 rgbc;
    s32 u0;
    s32 u1;
    u32 tp;

    p = g_swirlPkt;
    c = (g_swirlFade >> 4) & 0xFF;
    rgbc = (c << 16) | (c << 8) | c | GPU_CODE(GPU_POLY_FT4 | GPU_ABE);
    p += 10;
    for (i = 0; i < 8; i++) {
        v = &g_swirlMesh[i * 9];
        tp = ((((g_swirlColumns[i].vx + g_gameState->dispEnv[g_gameState->unk1C->unk8].disp.x) & 0x3FF) >> 6) | 0x100) << 16;
        u0 = g_swirlColumns[i].vx & 0x3F;
        u1 = u0 + (g_swirlColumns[i + 1].vx - g_swirlColumns[i].vx) - 1;
        for (j = 0; j < 8; j++) {
            p[0] = 0x09000000;
            p[1] = rgbc;
            p[2] = (v->y << 16) | (u16)v->x;
            p[3] = (g_swirlColumns[j].vy << 8) | (u8)u0;
            p[4] = (v[9].y << 16) | (u16)v[9].x;
            p[5] = (g_swirlColumns[j].vy << 8) | (u8)u1 | tp;
            p[6] = (v[1].y << 16) | (u16)v[1].x;
            p[7] = ((g_swirlColumns[j + 1].vy - 1) << 8) | (u8)u0;
            p[8] = (v[10].y << 16) | (u16)v[10].x;
            p[9] = ((g_swirlColumns[j + 1].vy - 1) << 8) | (u8)u1;
            setaddr(p - 10, p);
            p += 10;
            v++;
        }
    }
    setaddr(p - 10, getaddr(&g_swirlOt[1]));
    setaddr(&g_swirlOt[1], g_swirlPkt + 10);
    g_swirlPkt = p;
}
