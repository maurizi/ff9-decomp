#include "ff9.h"

u8 fade_setActive(u8 a) {
    u8 old = g_fade.active;
    g_fade.active = a;
    return old;
}

u8 fade_isActive(void) {
    return g_fade.active;
}

u8 fade_isBusy(void) {
    if (g_fade.active != 0) {
        return 1;
    }
    return g_fade.covered != 0;
}

void fade_cancel(void) {
    g_fade.active = 0;
    g_fade.covered = 0;
    g_fade.type = 0;
}

s32 fade_getType(void) {
    return g_fade.type;
}

s32 fade_isSubtractive(void) {
    return g_fade.subtract;
}

s32 fade_getFrame(void) {
    return g_fade.frame;
}

void fade_clear(void) {
    g_fade.covered = 0;
    g_fade.active = 0;
    g_fade.frame = 0;
    g_fade.frames = 0;
    g_fade.type = 0;
    g_fade.onDone = 0;
}

s32 fade_setOnDone(void (*a)(void)) {
    s32 old = g_fade.onDone;
    g_fade.onDone = (s32)a;
    return old;
}

u8 fade_clearEase() {
    u8 old = g_fade.eased;
    g_fade.eased = 0;
    return old;
}

u8 fade_isEased(void) {
    return g_fade.eased;
}

s32 fade_setOtz(s32 a) {
    s32 old = g_fade.otz;
    if (a == 0) {
        a = 1;
    }
    g_fade.otz = a;
    return old;
}

s32 fade_getOtz(void) {
    return g_fade.otz;
}

u8 fade_setCovered(u8 a) {
    u8 old = g_fade.covered;
    g_fade.covered = a;
    return old;
}

u8 fade_isCovered(void) {
    return g_fade.covered;
}

u16 fade_setAngle(u16 a) {
    u16 old = g_fade.angle;
    g_fade.angle = a;
    return old;
}

u16 fade_getAngle(void) {
    return g_fade.angle;
}

void fade_setEndColor(u8 a, u8 b, u8 c) {
    FadeState *p = &g_fade;
    FadeState *q;
    s32 r = 0;
    p->endR = a;
    p->endG = b;
    p->endB = c;
    q = p;
    if (a != 0 || b != 0 || c != 0) {
        r = 1;
    }
    q->covered = r;
}

Unk4 fade_getColor(void) {
    return g_fade.color;
}

void fade_inDefault(void) {
    fade_in(0x10);
}

void fade_outDefault(void) {
    fade_out(0xC);
}

void fade_inAdditiveDefault(void) {
    fade_inAdditive(0x10);
}

void fade_outAdditiveDefault(void) {
    fade_outAdditive(0xC);
}

void fade_in(s32 a) {
    fade_setAngle(0);
    fade_clearEase(1);
    fade_start(1, 1, 0, 0xFFFFFF);
    fade_start(1, 1, a, 0);
}

void fade_out(s32 a) {
    fade_setAngle(0);
    fade_clearEase(1);
    fade_start(1, 1, 0, 0);
    fade_start(1, 1, a, 0xFFFFFF);
}

void fade_inAdditive(s32 a) {
    fade_setAngle(0);
    fade_clearEase(1);
    fade_start(1, 0, 0, 0xFFFFFF);
    fade_start(1, 0, a, 0);
}

void fade_outAdditive(s32 a) {
    fade_setAngle(0);
    fade_clearEase(1);
    fade_start(1, 0, 0, 0);
    fade_start(1, 0, a, 0xFFFFFF);
}

u32 *fade_getOt(s32 i) {
    if (g_gameState->mode != 2) {
        return &g_gameState->gameData->ot[i][g_fade.otz];
    }
    return &g_fadeBattleOt[i];
}

void fade_getWipeColors(void *a) {
    fadeWipe_getColors(a);
}

void func_8002FC30(void) {
}

void fade_init(void) {
    g_fade.otz = 1;
    g_fade.angle = -1;
    g_fade.active = 0;
    g_fade.onDone = 0;
    g_fade.eased = 0;
    g_fade.covered = 0;
    g_fade.prevType = 0;
    g_fade.type = 0;
    fade_init_clear3(g_fade.color.unk0);
    fade_init_clear3(&g_fade.endR);
}

void fade_start(s32 a, s32 b, s32 c, s32 col) {
    u8 old;
    void (*fn)(FadeState *);

    if (c == 0) {
        g_fade.color = *(Unk4 *)&col;
    } else {
        g_fade.startColor = g_fade.color;
        old = g_fade.type;
        g_fade.type = a;
        g_fade.subtract = b;
        g_fade.frame = 0;
        g_fade.frames = c;
        g_fade.prevType = old;
        *(Unk4 *)&g_fade.endR = *(Unk4 *)&col;
        g_fade.active = 1;
        g_fade.onDone = 0;
        g_fade.covered = (*(s32 *)&g_fade.endR & 0xFFFFFF) != 0;
        fn = g_fadeTypes[a].unk0;
        if (fn != NULL) {
            fn(&g_fade);
        }
    }
}

s32 fade_stepClamp(s32 a, s32 b, s32 lim) {
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

s32 fade_divRoundOut(s32 a, s32 b) {
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

s32 fade_ease(s32 x, s32 a, s32 b) {
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
    fade_reverse(0);
}

void fadeWipe_start(FadeState *p) {
    s32 half;
    s32 n;
    FadeWipe *d;

    if (p->frames == 0) {
        p->frames = 0x10;
    }
    g_fadeWipe.startR = p->color.unk0[0];
    g_fadeWipe.color2.r = g_fadeWipe.startR;
    g_fadeWipe.color.r = g_fadeWipe.startR;
    g_fadeWipe.startG = p->color.unk0[1];
    g_fadeWipe.color2.g = g_fadeWipe.startG;
    g_fadeWipe.color.g = g_fadeWipe.startG;
    g_fadeWipe.startB = p->color.unk0[2];
    g_fadeWipe.color2.b = g_fadeWipe.startB;
    g_fadeWipe.color.b = g_fadeWipe.startB;
    g_fadeWipe.endR = p->endR;
    g_fadeWipe.endG = p->endG;
    g_fadeWipe.endB = p->endB;
    g_fadeWipe.deltaR = g_fadeWipe.endR - g_fadeWipe.startR;
    g_fadeWipe.deltaG = g_fadeWipe.endG - g_fadeWipe.startG;
    g_fadeWipe.deltaB = g_fadeWipe.endB - g_fadeWipe.startB;
    half = p->frames >> 1;
    n = 1;
    if (half > 0) {
        n = half;
    }
    g_fadeWipe.frames = n;
    d = &g_fadeWipe;
    d->frames2 = p->frames - n;
    d->stepR = fade_divRoundOut(d->deltaR, d->frames);
    d->stepG = fade_divRoundOut(d->deltaG, d->frames);
    d->stepB = fade_divRoundOut(d->deltaB, d->frames);
    if (d->frames2 != 0) {
        d->step2R = fade_divRoundOut(d->deltaR, d->frames2);
        d->step2G = fade_divRoundOut(d->deltaG, d->frames2);
        d->step2B = fade_divRoundOut(d->deltaB, d->frames2);
    } else {
        do {
        } while (0); /* empty macro in the original; needed for the s0/s1 split */
        d->step2B = 0;
        d->step2G = 0;
        d->step2R = 0;
    }
}

void fadeWipe_update(FadeState *p) {
    if ((u32)p->frame < g_fadeWipe.frames) {
        g_fadeWipe.color.r = fade_stepClamp(g_fadeWipe.color.r, g_fadeWipe.stepR, g_fadeWipe.endR);
        g_fadeWipe.color.g = fade_stepClamp(g_fadeWipe.color.g, g_fadeWipe.stepG, g_fadeWipe.endG);
        g_fadeWipe.color.b = fade_stepClamp(g_fadeWipe.color.b, g_fadeWipe.stepB, g_fadeWipe.endB);
        g_fadeWipe.out = g_fadeWipe.color;
    } else {
        g_fadeWipe.color2.r = fade_stepClamp(g_fadeWipe.color2.r, g_fadeWipe.step2R, g_fadeWipe.endR);
        g_fadeWipe.color2.g = fade_stepClamp(g_fadeWipe.color2.g, g_fadeWipe.step2G, g_fadeWipe.endG);
        g_fadeWipe.color2.b = fade_stepClamp(g_fadeWipe.color2.b, g_fadeWipe.step2B, g_fadeWipe.endB);
        g_fadeWipe.out = g_fadeWipe.color2;
    }
    p->color.unk0[0] = g_fadeWipe.out.r;
    p->color.unk0[1] = g_fadeWipe.out.g;
    p->color.unk0[2] = g_fadeWipe.out.b;
}

void fadeWipe_seek(FadeState *p, s32 n) {
    if (n < (s32)g_fadeWipe.frames) {
        g_fadeWipe.color.r = fade_stepClamp(g_fadeWipe.color.r, g_fadeWipe.stepR * n, g_fadeWipe.endR);
        g_fadeWipe.color.g = fade_stepClamp(g_fadeWipe.color.g, g_fadeWipe.stepG * n, g_fadeWipe.endG);
        g_fadeWipe.color.b = fade_stepClamp(g_fadeWipe.color.b, g_fadeWipe.stepB * n, g_fadeWipe.endB);
        g_fadeWipe.out = g_fadeWipe.color;
    } else {
        n -= g_fadeWipe.frames;
        g_fadeWipe.color2.r = fade_stepClamp(g_fadeWipe.color2.r, g_fadeWipe.step2R * n, g_fadeWipe.endR);
        g_fadeWipe.color2.g = fade_stepClamp(g_fadeWipe.color2.g, g_fadeWipe.step2G * n, g_fadeWipe.endG);
        g_fadeWipe.color2.b = fade_stepClamp(g_fadeWipe.color2.b, g_fadeWipe.step2B * n, g_fadeWipe.endB);
        g_fadeWipe.out = g_fadeWipe.color2;
    }
    p->color.unk0[0] = g_fadeWipe.out.r;
    p->color.unk0[1] = g_fadeWipe.out.g;
    p->color.unk0[2] = g_fadeWipe.out.b;
}

void fadeWipe_draw(FadeState *arg0) {
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
    ot = fade_getOt(g_gameState->gameData->drawBuf);
    if (arg0->subtract == 0) {
        tpage = 32;
    }
    ReadGeomOffset(&ofx, &ofy);
    SetGeomOffset(g_gameState->dispEnv->disp.w >> 1, g_gameState->dispEnv->disp.h >> 1);
    gte_ReadGeomScreen_r(h);
    hw = g_gameState->dispEnv->disp.w >> 1;
    hh = g_gameState->dispEnv->disp.h >> 1;
    PushMatrix();
    g_fadeWipeRot.vx = 0;
    g_fadeWipeRot.vy = 0;
    g_fadeWipeRot.vz = arg0->angle;
    math_rotMatrixXYZ(&g_fadeWipeRot, &m);
    m.t[2] = 0;
    m.t[1] = 0;
    m.t[0] = 0;
    SetRotMatrix(&m);
    SetTransMatrix(&m);
    r = SquareRoot0(hw * hw + hh * hh);
    setVector(&g_fadeWipeQuads[0][0], -r, -r, h);
    setVector(&g_fadeWipeQuads[0][1], 0, -r, h);
    setVector(&g_fadeWipeQuads[0][2], -r, r, h);
    setVector(&g_fadeWipeQuads[0][3], 0, r, h);
    setVector(&g_fadeWipeQuads[1][0], 0, -r, h);
    setVector(&g_fadeWipeQuads[1][1], r, -r, h);
    setVector(&g_fadeWipeQuads[1][2], 0, r, h);
    setVector(&g_fadeWipeQuads[1][3], r, r, h);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            RotTransPers(&g_fadeWipeQuads[i][j], (long *)&xy[j], &p, &flag);
        }
        poly = (POLY_G4 *)g_fade.pkt;
        SetPolyG4(poly);
        if ((u32)arg0->frame < g_fadeWipe.frames) {
            setRGB0(poly, g_fadeWipe.out.r, g_fadeWipe.out.g, g_fadeWipe.out.b);
            setRGB1(poly, g_fadeWipe.out.r, g_fadeWipe.out.g, g_fadeWipe.out.b);
            setRGB2(poly, g_fadeWipe.startR, g_fadeWipe.startG, g_fadeWipe.startB);
            setRGB3(poly, g_fadeWipe.startR, g_fadeWipe.startG, g_fadeWipe.startB);
        } else {
            setRGB0(poly, g_fadeWipe.endR, g_fadeWipe.endG, g_fadeWipe.endB);
            setRGB1(poly, g_fadeWipe.endR, g_fadeWipe.endG, g_fadeWipe.endB);
            setRGB2(poly, g_fadeWipe.out.r, g_fadeWipe.out.g, g_fadeWipe.out.b);
            setRGB3(poly, g_fadeWipe.out.r, g_fadeWipe.out.g, g_fadeWipe.out.b);
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
        g_fade.pkt += sizeof(POLY_G4);
    }
    tp = (DR_TPAGE *)g_fade.pkt;
    SetDrawTPage(tp, 0, 1, tpage);
    addPrim(ot, tp);
    g_fade.pkt += sizeof(DR_TPAGE);
    PopMatrix();
    SetGeomOffset(ofx, ofy);
}

void fadeWipe_getColors(u8 *arg0) {
    if ((u32)g_fade.frame < g_fadeWipe.frames) {
        arg0[0] = g_fadeWipe.out.r;
        arg0[1] = g_fadeWipe.out.g;
        arg0[2] = g_fadeWipe.out.b;
        arg0[4] = g_fadeWipe.startR;
        arg0[5] = g_fadeWipe.startG;
        arg0[6] = g_fadeWipe.startB;
        return;
    }
    arg0[0] = g_fadeWipe.endR;
    arg0[1] = g_fadeWipe.endG;
    arg0[2] = g_fadeWipe.endB;
    arg0[4] = g_fadeWipe.out.r;
    arg0[5] = g_fadeWipe.out.g;
    arg0[6] = g_fadeWipe.out.b;
}
