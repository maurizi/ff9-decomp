#include "ff9.h"

u8 func_8002F7E8(u8 a) {
    u8 old = D_80077C78.unk2;
    D_80077C78.unk2 = a;
    return old;
}

u8 func_8002F800(void) {
    return D_80077C78.unk2;
}

u8 func_8002F810(void) {
    if (D_80077C78.unk2 != 0) {
        return 1;
    }
    return D_80077C78.unk3 != 0;
}

void func_8002F83C(void) {
    D_80077C78.unk2 = 0;
    D_80077C78.unk3 = 0;
    D_80077C78.unk0 = 0;
}

u8 func_8002F854(void) {
    return D_80077C78.unk0;
}

u8 func_8002F864(void) {
    return D_80077C78.unk1;
}

s32 func_8002F874(void) {
    return D_80077C78.unkC;
}

void func_8002F884(void) {
    D_80077C78.unk3 = 0;
    D_80077C78.unk2 = 0;
    D_80077C78.unkC = 0;
    D_80077C78.unk8 = 0;
    D_80077C78.unk0 = 0;
    D_80077C78.unk10 = 0;
}

s32 func_8002F8A8(void (*a)(void)) {
    s32 old = D_80077C78.unk10;
    D_80077C78.unk10 = (s32)a;
    return old;
}

u8 func_8002F8BC() {
    u8 old = D_80077C78.unk4;
    D_80077C78.unk4 = 0;
    return old;
}

u8 func_8002F8D4(void) {
    return D_80077C78.unk4;
}

s32 func_8002F8E4(s32 a) {
    s32 old = D_80077C78.unk1C;
    if (a == 0) {
        a = 1;
    }
    D_80077C78.unk1C = a;
    return old;
}

s32 func_8002F904(void) {
    return D_80077C78.unk1C;
}

u8 func_8002F914(u8 a) {
    u8 old = D_80077C78.unk3;
    D_80077C78.unk3 = a;
    return old;
}

u8 func_8002F92C(void) {
    return D_80077C78.unk3;
}

u16 func_8002F93C(u16 a) {
    u16 old = D_80077C78.unkA;
    D_80077C78.unkA = a;
    return old;
}

u16 func_8002F954(void) {
    return D_80077C78.unkA;
}

void func_8002F964(u8 a, u8 b, u8 c) {
    Unk80077C78 *p = &D_80077C78;
    Unk80077C78 *q;
    s32 r = 0;
    p->unk24 = a;
    p->unk25 = b;
    p->unk26 = c;
    q = p;
    if (a != 0 || b != 0 || c != 0) {
        r = 1;
    }
    q->unk3 = r;
}

Unk4 func_8002F9A8(void) {
    return D_80077C78.unk28;
}

void func_8002F9D0(void) {
    func_8002FA50(0x10);
}

void func_8002F9F0(void) {
    func_8002FAAC(0xC);
}

void func_8002FA10(void) {
    func_8002FB08(0x10);
}

void func_8002FA30(void) {
    func_8002FB64(0xC);
}

void func_8002FA50(s32 a) {
    func_8002F93C(0);
    func_8002F8BC(1);
    func_8002FC8C(1, 1, 0, 0xFFFFFF);
    func_8002FC8C(1, 1, a, 0);
}

void func_8002FAAC(s32 a) {
    func_8002F93C(0);
    func_8002F8BC(1);
    func_8002FC8C(1, 1, 0, 0);
    func_8002FC8C(1, 1, a, 0xFFFFFF);
}

void func_8002FB08(s32 a) {
    func_8002F93C(0);
    func_8002F8BC(1);
    func_8002FC8C(1, 0, 0, 0xFFFFFF);
    func_8002FC8C(1, 0, a, 0);
}

void func_8002FB64(s32 a) {
    func_8002F93C(0);
    func_8002F8BC(1);
    func_8002FC8C(1, 0, 0, 0);
    func_8002FC8C(1, 0, a, 0xFFFFFF);
}

u32 *func_8002FBC0(s32 i) {
    if (g_gameState->mode != 2) {
        return &g_gameState->unk1C->unk10[i][D_80077C78.unk1C];
    }
    return &D_80077CA4[i];
}

void func_8002FC10(void *a) {
    func_80030770(a);
}

void func_8002FC30(void) {
}

void func_8002FC38(void) {
    D_80077C78.unk1C = 1;
    D_80077C78.unkA = -1;
    D_80077C78.unk2 = 0;
    D_80077C78.unk10 = 0;
    D_80077C78.unk4 = 0;
    D_80077C78.unk3 = 0;
    D_80077C78.unk5 = 0;
    D_80077C78.unk0 = 0;
    func_8002FC38_clear3(D_80077C78.unk28.unk0);
    func_8002FC38_clear3(&D_80077C78.unk24);
}

void func_8002FC8C(s32 a, s32 b, s32 c, s32 col) {
    u8 old;
    void (*fn)(Unk80077C78 *);

    if (c == 0) {
        D_80077C78.unk28 = *(Unk4 *)&col;
    } else {
        D_80077C78.unk20 = D_80077C78.unk28;
        old = D_80077C78.unk0;
        D_80077C78.unk0 = a;
        D_80077C78.unk1 = b;
        D_80077C78.unkC = 0;
        D_80077C78.unk8 = c;
        D_80077C78.unk5 = old;
        *(Unk4 *)&D_80077C78.unk24 = *(Unk4 *)&col;
        D_80077C78.unk2 = 1;
        D_80077C78.unk10 = 0;
        D_80077C78.unk3 = (*(s32 *)&D_80077C78.unk24 & 0xFFFFFF) != 0;
        fn = D_8006BD04[a].unk0;
        if (fn != NULL) {
            fn(&D_80077C78);
        }
    }
}

s32 func_8002FD70(s32 a, s32 b, s32 lim) {
    a += b;
    if (b < 0) {
        if (a < lim) {
            a = lim;
        }
    } else {
        if (a > lim) {
            a = lim;
        }
    }
    return a;
}

s32 func_8002FD98(s32 a, s32 b) {
    s32 t;

    if (a >= 0) {
        t = a - 1;
        t += b;
        b = t / b;
    } else {
        t = a + 1;
        t -= b;
        b = t / b;
    }
    return b;
}

s32 func_8002FDE8(s32 x, s32 a, s32 b) {
    s32 t;

    if (b != 0) {
        if (b < 0) {
            t = x - a - b;
            x = a + (b - t * t / b);
        } else {
            t = x - a - b;
            x = a + (b - t * t / b);
        }
    }
    return x;
}

void func_8002FEA4(void) {
}

void func_8002FEAC(void) {
}

void func_8002FEB4(void) {
    func_8002F5A0(0);
}

void func_8002FED4(Unk80077C78 *p) {
    s32 half;
    s32 n;
    Unk80077D00 *d;

    if (p->unk8 == 0) {
        p->unk8 = 0x10;
    }
    D_80077D00.unk0 = p->unk28.unk0[0];
    D_80077D00.unk3C.unk0 = D_80077D00.unk0;
    D_80077D00.unk30.unk0 = D_80077D00.unk0;
    D_80077D00.unk4 = p->unk28.unk0[1];
    D_80077D00.unk3C.unk4 = D_80077D00.unk4;
    D_80077D00.unk30.unk4 = D_80077D00.unk4;
    D_80077D00.unk8 = p->unk28.unk0[2];
    D_80077D00.unk3C.unk8 = D_80077D00.unk8;
    D_80077D00.unk30.unk8 = D_80077D00.unk8;
    D_80077D00.unkC = p->unk24;
    D_80077D00.unk10 = p->unk25;
    D_80077D00.unk14 = p->unk26;
    D_80077D00.unk54 = D_80077D00.unkC - D_80077D00.unk0;
    D_80077D00.unk58 = D_80077D00.unk10 - D_80077D00.unk4;
    D_80077D00.unk5C = D_80077D00.unk14 - D_80077D00.unk8;
    half = p->unk8 >> 1;
    n = 1;
    if (half > 0) {
        n = half;
    }
    D_80077D00.unk60 = n;
    d = &D_80077D00;
    d->unk64 = p->unk8 - n;
    d->unk18 = func_8002FD98(d->unk54, d->unk60);
    d->unk1C = func_8002FD98(d->unk58, d->unk60);
    d->unk20 = func_8002FD98(d->unk5C, d->unk60);
    if (d->unk64 != 0) {
        d->unk24 = func_8002FD98(d->unk54, d->unk64);
        d->unk28 = func_8002FD98(d->unk58, d->unk64);
        d->unk2C = func_8002FD98(d->unk5C, d->unk64);
    } else {
        do {
        } while (0); /* empty macro in the original; needed for the s0/s1 split */
        d->unk2C = 0;
        d->unk28 = 0;
        d->unk24 = 0;
    }
}

void func_80030030(Unk80077C78 *p) {
    if ((u32)p->unkC < D_80077D00.unk60) {
        D_80077D00.unk30.unk0 = func_8002FD70(D_80077D00.unk30.unk0, D_80077D00.unk18, D_80077D00.unkC);
        D_80077D00.unk30.unk4 = func_8002FD70(D_80077D00.unk30.unk4, D_80077D00.unk1C, D_80077D00.unk10);
        D_80077D00.unk30.unk8 = func_8002FD70(D_80077D00.unk30.unk8, D_80077D00.unk20, D_80077D00.unk14);
        D_80077D00.unk48 = D_80077D00.unk30;
    } else {
        D_80077D00.unk3C.unk0 = func_8002FD70(D_80077D00.unk3C.unk0, D_80077D00.unk24, D_80077D00.unkC);
        D_80077D00.unk3C.unk4 = func_8002FD70(D_80077D00.unk3C.unk4, D_80077D00.unk28, D_80077D00.unk10);
        D_80077D00.unk3C.unk8 = func_8002FD70(D_80077D00.unk3C.unk8, D_80077D00.unk2C, D_80077D00.unk14);
        D_80077D00.unk48 = D_80077D00.unk3C;
    }
    p->unk28.unk0[0] = D_80077D00.unk48.unk0;
    p->unk28.unk0[1] = D_80077D00.unk48.unk4;
    p->unk28.unk0[2] = D_80077D00.unk48.unk8;
}

void func_8003015C(Unk80077C78 *p, s32 n) {
    if (n < (s32)D_80077D00.unk60) {
        D_80077D00.unk30.unk0 = func_8002FD70(D_80077D00.unk30.unk0, D_80077D00.unk18 * n, D_80077D00.unkC);
        D_80077D00.unk30.unk4 = func_8002FD70(D_80077D00.unk30.unk4, D_80077D00.unk1C * n, D_80077D00.unk10);
        D_80077D00.unk30.unk8 = func_8002FD70(D_80077D00.unk30.unk8, D_80077D00.unk20 * n, D_80077D00.unk14);
        D_80077D00.unk48 = D_80077D00.unk30;
    } else {
        n -= D_80077D00.unk60;
        D_80077D00.unk3C.unk0 = func_8002FD70(D_80077D00.unk3C.unk0, D_80077D00.unk24 * n, D_80077D00.unkC);
        D_80077D00.unk3C.unk4 = func_8002FD70(D_80077D00.unk3C.unk4, D_80077D00.unk28 * n, D_80077D00.unk10);
        D_80077D00.unk3C.unk8 = func_8002FD70(D_80077D00.unk3C.unk8, D_80077D00.unk2C * n, D_80077D00.unk14);
        D_80077D00.unk48 = D_80077D00.unk3C;
    }
    p->unk28.unk0[0] = D_80077D00.unk48.unk0;
    p->unk28.unk0[1] = D_80077D00.unk48.unk4;
    p->unk28.unk0[2] = D_80077D00.unk48.unk8;
}

void func_800302D0(Unk80077C78 *arg0) {
    DVECTOR xy[4];
    MATRIX m;
    long ofx, ofy;
    long p, flag;
    s32 tpage;
    u32 *ot;
    s32 h;
    s32 hw, hh;
    s32 r;
    s32 i, j;
    POLY_G4 *poly;
    DR_TPAGE *tp;

    tpage = 64;
    ot = func_8002FBC0(g_gameState->unk1C->unk8);
    if (arg0->unk1 == 0) {
        tpage = 32;
    }
    ReadGeomOffset(&ofx, &ofy);
    SetGeomOffset(g_gameState->unkC->disp.w >> 1, g_gameState->unkC->disp.h >> 1);
    gte_ReadGeomScreen_r(h);
    hw = g_gameState->unkC->disp.w >> 1;
    hh = g_gameState->unkC->disp.h >> 1;
    PushMatrix();
    D_80077CB0.vx = 0;
    D_80077CB0.vy = 0;
    D_80077CB0.vz = arg0->unkA;
    func_80030AF4(&D_80077CB0, &m);
    m.t[2] = 0;
    m.t[1] = 0;
    m.t[0] = 0;
    SetRotMatrix(&m);
    SetTransMatrix(&m);
    r = SquareRoot0(hw * hw + hh * hh);
    setVector(&D_80077CC0[0][0], -r, -r, h);
    setVector(&D_80077CC0[0][1], 0, -r, h);
    setVector(&D_80077CC0[0][2], -r, r, h);
    setVector(&D_80077CC0[0][3], 0, r, h);
    setVector(&D_80077CC0[1][0], 0, -r, h);
    setVector(&D_80077CC0[1][1], r, -r, h);
    setVector(&D_80077CC0[1][2], 0, r, h);
    setVector(&D_80077CC0[1][3], r, r, h);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            RotTransPers(&D_80077CC0[i][j], (long *)&xy[j], &p, &flag);
        }
        poly = (POLY_G4 *)D_80077C78.unk18;
        SetPolyG4(poly);
        if ((u32)arg0->unkC < D_80077D00.unk60) {
            setRGB0(poly, D_80077D00.unk48.unk0, D_80077D00.unk48.unk4, D_80077D00.unk48.unk8);
            setRGB1(poly, D_80077D00.unk48.unk0, D_80077D00.unk48.unk4, D_80077D00.unk48.unk8);
            setRGB2(poly, D_80077D00.unk0, D_80077D00.unk4, D_80077D00.unk8);
            setRGB3(poly, D_80077D00.unk0, D_80077D00.unk4, D_80077D00.unk8);
        } else {
            setRGB0(poly, D_80077D00.unkC, D_80077D00.unk10, D_80077D00.unk14);
            setRGB1(poly, D_80077D00.unkC, D_80077D00.unk10, D_80077D00.unk14);
            setRGB2(poly, D_80077D00.unk48.unk0, D_80077D00.unk48.unk4, D_80077D00.unk48.unk8);
            setRGB3(poly, D_80077D00.unk48.unk0, D_80077D00.unk48.unk4, D_80077D00.unk48.unk8);
        }
        poly->x0 = xy[0].vx;
        poly->y0 = xy[0].vy;
        poly->x1 = xy[1].vx;
        poly->y1 = xy[1].vy;
        poly->x2 = xy[2].vx;
        poly->y2 = xy[2].vy;
        poly->x3 = xy[3].vx;
        poly->y3 = xy[3].vy;
        SetSemiTrans(poly, 1);
        addPrim(ot, poly);
        D_80077C78.unk18 += sizeof(POLY_G4);
    }
    tp = (DR_TPAGE *)D_80077C78.unk18;
    SetDrawTPage(tp, 0, 1, tpage);
    addPrim(ot, tp);
    D_80077C78.unk18 += sizeof(DR_TPAGE);
    PopMatrix();
    SetGeomOffset(ofx, ofy);
}

void func_80030770(u8 *arg0) {
    if ((u32)D_80077C78.unkC < D_80077D00.unk60) {
        arg0[0] = D_80077D00.unk48.unk0;
        arg0[1] = D_80077D00.unk48.unk4;
        arg0[2] = D_80077D00.unk48.unk8;
        arg0[4] = D_80077D00.unk0;
        arg0[5] = D_80077D00.unk4;
        arg0[6] = D_80077D00.unk8;
        return;
    }
    arg0[0] = D_80077D00.unkC;
    arg0[1] = D_80077D00.unk10;
    arg0[2] = D_80077D00.unk14;
    arg0[4] = D_80077D00.unk48.unk0;
    arg0[5] = D_80077D00.unk48.unk4;
    arg0[6] = D_80077D00.unk48.unk8;
}
