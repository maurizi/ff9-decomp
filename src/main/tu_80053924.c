#include "ff9.h"

void func_80053924(void) {
}

void func_8005392C(void) {
    GameData *d = g_gameState->unk1C;
    d->unk568 = 0;
    d->unk56A = 0;
    d->unk56C = 0;
    d->unk56E = 0;
    d->unk56F = 0;
    d->unk570 = 0;
    d->unk571 = 0;
}

s32 func_80053960(Unk800539FC *a, s32 b) {
    if (a->status & b) {
        a->status &= ~b;
        return 2;
    }
    return 1;
}

static inline s32 func_80053960_inline(Unk800539FC *a, s32 b) {
    if (a->status & b) {
        a->status &= ~b;
        return 2;
    }
    return 1;
}

s32 func_8005398C(Character *a, s32 mask) {
    s32 r = 1;
    u32 i;

    for (i = 0; i < 8; i++) {
        u8 m = mask & (1 << i);
        if (m) {
            if (func_80053960_inline(a, 1 << i) == 2) {
                r = 2;
            }
        }
    }
    return r;
}

u8 func_800539FC(Character *a, s32 b) {
    return (a->status & b) != 0;
}

s32 func_80053A10(s16 arg0, s16 arg1) {
    s16 y;
    s16 x;
    s32 c;
    u8 i;
    s16 a, b;
    GameState *gs;

    x = arg0;
    y = arg1;
    D_8007BD08 = D_8007BD2C ? &D_8006F2E0[1] : &D_8006F2E0[0];
    D_8007BD0C = D_8007BD10 = D_8007BD30 + (g_gameState->unk1C->unk8 << 10);
    ClearOTag(D_8007BD0C, 5);
    D_8007BD10 += 5;
    if (D_8007BD28 < 0x79) {
        if (D_8007BD28 == 0) {
            gs = g_gameState;
            gs->unk1C->unk0 &= ~0x20;
            *(u32 *)&gs->unk10[0].isbg = *(u32 *)&gs->unk10[1].isbg = 1;
            D_8007BD1E = 0;
            D_8007BD1C = D_8007BD08->unkA;
            D_8007BD20 = a = D_8007BD08->unk0;
            D_8007BD24 = D_8007BD08->unk6;
            D_8007BD22 = b = D_8007BD08->unk2;
            if ((arg0 ^ arg1) & 1) {
                D_8007BD20 = -a;
                D_8007BD22 = -b;
            }
            fade_clear();
        }
        if (D_8007BD28 & 1) {
            x += x < 160 ? 1 : -1;
        }
        if (D_8007BD28 & 2) {
            y += y < g_gameState->unk10->clip.h / 2 ? 1 : -1;
        }
        if (D_8007BD2C) {
            c = math_sin(D_8007BD28 * 8);
            x += c * (160 - x) / 4096;
            y += c * (g_gameState->unk10->clip.h / 2 - y) / 4096;
        }
        D_8007BD10 = func_80054294(D_8007BD10, x, y);
        i = g_gameState->unk1C->unk8;
        SetDrawMove(D_8007BD10, &g_gameState->unkC[i].disp, g_gameState->unk10[i].clip.x, g_gameState->unk10[i].clip.y);
        addPrim(D_8007BD0C, D_8007BD10);
        D_8007BD10 += 8;
        func_80054650();
        if (D_8007BD28 >= D_8007BD08->unkC) {
            D_8007BD1C -= D_8007BD08->unkE;
            if ((s16)D_8007BD1C < 0) {
                D_8007BD1C = 0;
            }
        }
        if (D_8007BD28 >= D_8007BD08->unk10) {
            func_800540E8(x, y);
            D_8007BD1E += D_8007BD08->unk12;
            if ((s16)D_8007BD1E >= 0x1000) {
                D_8007BD1E = 0xFFF;
            }
        }
        D_8007BD20 += D_8007BD22;
        D_8007BD24 += D_8007BD08->unk8;
        if (D_8007BD24 > 0x1400) {
            D_8007BD24 = 0x1400;
        }
        setlen(D_8007BD10, 2);
        D_8007BD10[1] = 0xE6000001;
        D_8007BD10[2] = 0;
        addPrim(D_8007BD0C, D_8007BD10);
        D_8007BD10 += 4;
    }
    SetDrawEnv((DR_ENV *)D_8007BD10, &g_gameState->unk10[g_gameState->unk1C->unk8]);
    addPrim(D_8007BD0C, D_8007BD10);
    D_8007BD10 += 16;
    setlen(D_8007BD10, 2);
    D_8007BD10[1] = 0xE6000000;
    D_8007BD10[2] = 0;
    addPrim(&D_8007BD0C[3], D_8007BD10);
    D_8007BD10 += 4;
    setaddr(&D_8007BD0C[4], getaddr(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8]));
    setaddr(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], D_8007BD0C);
    if (D_8007BD28 == 0) {
        DrawSync(0);
        setlen(D_8007BD10, 2);
        D_8007BD10[1] = 0xE6000001;
        D_8007BD10[2] = 0;
        DrawPrim(D_8007BD10);
        D_8007BD10 += 3;
        setlen(D_8007BD10, 1);
        D_8007BD10[1] = get_mode(0, 0, 0x120);
        DrawPrim(D_8007BD10);
        D_8007BD10 += 2;
        D_8007BD10[0] = 0x03000000;
        D_8007BD10[1] = 0x62000000;
        D_8007BD10[2] = 0;
        D_8007BD10[3] = (g_gameState->unk10->clip.h << 16) | 0x140;
        DrawPrim(D_8007BD10);
        D_8007BD10 += 4;
        setlen(D_8007BD10, 2);
        D_8007BD10[1] = 0xE6000000;
        D_8007BD10[2] = 0;
        DrawPrim(D_8007BD10);
    }
    return D_8007BD28++ > 0x78;
}

void func_800540E8(s32 x, s32 y) {
    u32 *p;
    s32 i;
    u32 v;
    u8 c1, c2;
    u32 xy;
    u32 g2;

    p = D_8007BD10;
    v = D_8007BD1E << 16;
    c1 = v >> 20;
    c2 = v >> 23;
    g2 = (c2 << 16) | (c2 << 8) | c2;
    setlen(p, 1);
    p[1] = get_mode(0, 1, 0x140);
    p += 7;
    for (i = 0; i < 4; i++) {
        p[0] = 0x06000000;
        p[1] = ((c1 << 16) | (c1 << 8) | c1) | GPU_CODE(GPU_POLY_G3 | GPU_ABE);
        p[2] = ((D_8006F310[i] * g_gameState->unk10->clip.h) << 16) | ((D_8006F308[i] * 320) & 0xFFFF);
        p[3] = (c1 << 16) | (c1 << 8) | c1;
        p[4] = ((D_8006F310[i + 1] * g_gameState->unk10->clip.h) << 16) | ((D_8006F308[i + 1] * 320) & 0xFFFF);
        p[5] = g2;
        p[6] = (y << 16) | (x & 0xFFFF);
        setaddr(p - 7, p);
        p += 7;
    }
    setaddr(p - 7, getaddr(&D_8007BD0C[2]));
    setaddr(&D_8007BD0C[2], D_8007BD10);
    D_8007BD10 = p;
}

u32 *func_80054294(u32 *buf, s32 x, s32 y) {
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

    D_8007BD14 = (DVECTOR *)buf;
    D_8007BD18 = (WarpVert *)((u8 *)buf + 0x24);
    buf = (u32 *)((u8 *)buf + 0x2AC);
    dx = g_gameState->unkC->disp.w - x;
    dy = g_gameState->unkC->disp.h - y;
    for (i = 0; i < 5; i++) {
        xs[4 - i] = x - (x * i) / 4;
        xs[4 + i] = x + (dx * i) / 4;
        ys[4 - i] = y - (y * i) / 4;
        ys[4 + i] = y + (dy * i) / 4;
    }
    for (i = 0; i < 5; i++) {
        t = math_cos(i << 8);
        pts = D_8007BD14;
        pts[i].vx = x - (x * t) / 4096;
        pts[8 - i].vx = x + (dx * t) / 4096;
        pts[i].vy = y - (y * t) / 4096;
        pts[8 - i].vy = y + (dy * t) / 4096;
    }
    for (i = 1; i < 8; i++) {
        ex = ((pts[i].vx - xs[i]) * D_8007BD24) / 4096;
        py = ys;
        ey = ((pts[i].vy - ys[i]) * D_8007BD24) / 4096;
        pts[i].vx = xs[i] + ex;
        pts[i].vy = ys[i] + ey;
    }
    sv.vy = 0;
    sv.vx = 0;
    sv.vz = (s16)D_8007BD20 >> 4;
    math_rotMatrixXYZ(&sv, &m);
    v.vx = v.vy = D_8007BD08->unk4;
    v.vz = 0;
    ScaleMatrix(&m, &v);
    m.t[0] = x;
    m.t[1] = y;
    m.t[2] = 0;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    p = D_8007BD18;
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

void func_80054650(void) {
    u32 *p;
    WarpVert *v;
    s32 i;
    s32 j;
    s32 c;
    u32 rgbc;
    s32 u0;
    s32 u1;
    u32 tp;

    p = D_8007BD10;
    c = (D_8007BD1C >> 4) & 0xFF;
    rgbc = (c << 16) | (c << 8) | c | GPU_CODE(GPU_POLY_FT4 | GPU_ABE);
    p += 10;
    for (i = 0; i < 8; i++) {
        v = &D_8007BD18[i * 9];
        tp = ((((D_8007BD14[i].vx + g_gameState->unkC[g_gameState->unk1C->unk8].disp.x) & 0x3FF) >> 6) | 0x100) << 16;
        u0 = D_8007BD14[i].vx & 0x3F;
        u1 = u0 + (D_8007BD14[i + 1].vx - D_8007BD14[i].vx) - 1;
        for (j = 0; j < 8; j++) {
            p[0] = 0x09000000;
            p[1] = rgbc;
            p[2] = (v->y << 16) | (u16)v->x;
            p[3] = (D_8007BD14[j].vy << 8) | (u8)u0;
            p[4] = (v[9].y << 16) | (u16)v[9].x;
            p[5] = (D_8007BD14[j].vy << 8) | (u8)u1 | tp;
            p[6] = (v[1].y << 16) | (u16)v[1].x;
            p[7] = ((D_8007BD14[j + 1].vy - 1) << 8) | (u8)u0;
            p[8] = (v[10].y << 16) | (u16)v[10].x;
            p[9] = ((D_8007BD14[j + 1].vy - 1) << 8) | (u8)u1;
            setaddr(p - 10, p);
            p += 10;
            v++;
        }
    }
    setaddr(p - 10, getaddr(&D_8007BD0C[1]));
    setaddr(&D_8007BD0C[1], D_8007BD10 + 10);
    D_8007BD10 = p;
}
