#include "ff9.h"

u8 fade_setActive(u8 active) {
    u8 old = g_fade.active;
    g_fade.active = active;
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

s32 fade_setOnDone(void (*fn)(void)) {
    s32 old = g_fade.onDone;
    g_fade.onDone = (s32)fn;
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

s32 fade_setOtz(s32 otz) {
    s32 old = g_fade.otz;
    if (otz == 0) {
        otz = 1;
    }
    g_fade.otz = otz;
    return old;
}

s32 fade_getOtz(void) {
    return g_fade.otz;
}

u8 fade_setCovered(u8 covered) {
    u8 old = g_fade.covered;
    g_fade.covered = covered;
    return old;
}

u8 fade_isCovered(void) {
    return g_fade.covered;
}

u16 fade_setAngle(u16 angle) {
    u16 old = g_fade.angle;
    g_fade.angle = angle;
    return old;
}

u16 fade_getAngle(void) {
    return g_fade.angle;
}

void fade_setEndColor(u8 r, u8 g, u8 b) {
    FadeState *fade = &g_fade;
    FadeState *fadeCopy;
    s32 covered = 0;
    fade->endR = r;
    fade->endG = g;
    fade->endB = b;
    fadeCopy = fade;
    if (r != 0 || g != 0 || b != 0) {
        covered = 1;
    }
    fadeCopy->covered = covered;
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

void fade_in(s32 frames) {
    fade_setAngle(0);
    fade_clearEase(1);
    fade_start(FADE_FLAT, FADE_SUB, 0, FADE_COLOR_FULL);
    fade_start(FADE_FLAT, FADE_SUB, frames, FADE_COLOR_NONE);
}

void fade_out(s32 frames) {
    fade_setAngle(0);
    fade_clearEase(1);
    fade_start(FADE_FLAT, FADE_SUB, 0, FADE_COLOR_NONE);
    fade_start(FADE_FLAT, FADE_SUB, frames, FADE_COLOR_FULL);
}

void fade_inAdditive(s32 frames) {
    fade_setAngle(0);
    fade_clearEase(1);
    fade_start(FADE_FLAT, FADE_ADD, 0, FADE_COLOR_FULL);
    fade_start(FADE_FLAT, FADE_ADD, frames, FADE_COLOR_NONE);
}

void fade_outAdditive(s32 frames) {
    fade_setAngle(0);
    fade_clearEase(1);
    fade_start(FADE_FLAT, FADE_ADD, 0, FADE_COLOR_NONE);
    fade_start(FADE_FLAT, FADE_ADD, frames, FADE_COLOR_FULL);
}

u32 *fade_getOt(s32 i) {
    if (g_gameState->mode != GS_MODE_BATTLE) {
        return &g_gameState->gameData->ot[i][g_fade.otz];
    }
    return &g_fadeBattleOt[i];
}

void fade_getWipeColors(void *out) {
    fadeWipe_getColors(out);
}

void fade_initField(void) {
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
    fade_init_clear3(g_fade.color.b);
    fade_init_clear3(&g_fade.endR);
}

void fade_start(s32 type, s32 subtract, s32 frames, s32 col) {
    u8 old;
    void (*fn)(FadeState *);

    if (frames == 0) {
        g_fade.color = *(Unk4 *)&col;
    } else {
        g_fade.startColor = g_fade.color;
        old = g_fade.type;
        g_fade.type = type;
        g_fade.subtract = subtract;
        g_fade.frame = 0;
        g_fade.frames = frames;
        g_fade.prevType = old;
        *(Unk4 *)&g_fade.endR = *(Unk4 *)&col;
        g_fade.active = 1;
        g_fade.onDone = 0;
        g_fade.covered = (*(s32 *)&g_fade.endR & 0xFFFFFF) != 0;
        fn = g_fadeTypes[type].start;
        if (fn != NULL) {
            fn(&g_fade);
        }
    }
}

s32 fade_stepClamp(s32 value, s32 step, s32 lim) {
    value += step;
    if (step < 0) {
        if (value < lim) {
            value = lim;
        }
    } else {
        if (value > lim) {
            value = lim;
        }
    }
    return value;
}

s32 fade_divRoundOut(s32 num, s32 den) {
    s32 biased;

    if (num >= 0) {
        biased = num - 1;
        biased += den;
        den = biased / den;
    } else {
        biased = num + 1;
        biased -= den;
        den = biased / den;
    }
    return den;
}

s32 fade_ease(s32 x, s32 from, s32 range) {
    s32 dist;

    if (range != 0) {
        if (range < 0) {
            dist = x - from - range;
            x = from + (range - dist * dist / range);
        } else {
            dist = x - from - range;
            x = from + (range - dist * dist / range);
        }
    }
    return x;
}

void fade_nop1(void) {
}

void fade_nop2(void) {
}

void fade_reverseNoCallback(void) {
    fade_reverse(0);
}

void fadeWipe_start(FadeState *fade) {
    s32 half;
    s32 firstFrames;
    FadeWipe *wipe;

    if (fade->frames == 0) {
        fade->frames = 0x10;
    }
    g_fadeWipe.startR = fade->color.b[0];
    g_fadeWipe.color2.r = g_fadeWipe.startR;
    g_fadeWipe.color.r = g_fadeWipe.startR;
    g_fadeWipe.startG = fade->color.b[1];
    g_fadeWipe.color2.g = g_fadeWipe.startG;
    g_fadeWipe.color.g = g_fadeWipe.startG;
    g_fadeWipe.startB = fade->color.b[2];
    g_fadeWipe.color2.b = g_fadeWipe.startB;
    g_fadeWipe.color.b = g_fadeWipe.startB;
    g_fadeWipe.endR = fade->endR;
    g_fadeWipe.endG = fade->endG;
    g_fadeWipe.endB = fade->endB;
    g_fadeWipe.deltaR = g_fadeWipe.endR - g_fadeWipe.startR;
    g_fadeWipe.deltaG = g_fadeWipe.endG - g_fadeWipe.startG;
    g_fadeWipe.deltaB = g_fadeWipe.endB - g_fadeWipe.startB;
    half = fade->frames >> 1;
    firstFrames = 1;
    if (half > 0) {
        firstFrames = half;
    }
    g_fadeWipe.frames = firstFrames;
    wipe = &g_fadeWipe;
    wipe->frames2 = fade->frames - firstFrames;
    wipe->stepR = fade_divRoundOut(wipe->deltaR, wipe->frames);
    wipe->stepG = fade_divRoundOut(wipe->deltaG, wipe->frames);
    wipe->stepB = fade_divRoundOut(wipe->deltaB, wipe->frames);
    if (wipe->frames2 != 0) {
        wipe->step2R = fade_divRoundOut(wipe->deltaR, wipe->frames2);
        wipe->step2G = fade_divRoundOut(wipe->deltaG, wipe->frames2);
        wipe->step2B = fade_divRoundOut(wipe->deltaB, wipe->frames2);
    } else {
        do {
        } while (0); /* empty macro in the original; needed for the s0/s1 split */
        wipe->step2B = 0;
        wipe->step2G = 0;
        wipe->step2R = 0;
    }
}

void fadeWipe_update(FadeState *fade) {
    if ((u32)fade->frame < g_fadeWipe.frames) {
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
    fade->color.b[0] = g_fadeWipe.out.r;
    fade->color.b[1] = g_fadeWipe.out.g;
    fade->color.b[2] = g_fadeWipe.out.b;
}

void fadeWipe_seek(FadeState *fade, s32 frames) {
    if (frames < (s32)g_fadeWipe.frames) {
        g_fadeWipe.color.r = fade_stepClamp(g_fadeWipe.color.r, g_fadeWipe.stepR * frames, g_fadeWipe.endR);
        g_fadeWipe.color.g = fade_stepClamp(g_fadeWipe.color.g, g_fadeWipe.stepG * frames, g_fadeWipe.endG);
        g_fadeWipe.color.b = fade_stepClamp(g_fadeWipe.color.b, g_fadeWipe.stepB * frames, g_fadeWipe.endB);
        g_fadeWipe.out = g_fadeWipe.color;
    } else {
        frames -= g_fadeWipe.frames;
        g_fadeWipe.color2.r = fade_stepClamp(g_fadeWipe.color2.r, g_fadeWipe.step2R * frames, g_fadeWipe.endR);
        g_fadeWipe.color2.g = fade_stepClamp(g_fadeWipe.color2.g, g_fadeWipe.step2G * frames, g_fadeWipe.endG);
        g_fadeWipe.color2.b = fade_stepClamp(g_fadeWipe.color2.b, g_fadeWipe.step2B * frames, g_fadeWipe.endB);
        g_fadeWipe.out = g_fadeWipe.color2;
    }
    fade->color.b[0] = g_fadeWipe.out.r;
    fade->color.b[1] = g_fadeWipe.out.g;
    fade->color.b[2] = g_fadeWipe.out.b;
}

void fadeWipe_draw(FadeState *fade) {
    DVECTOR xy[4];
    MATRIX mat;
    long ofx, ofy;
    long interp, flag;
    s32 tpage;
    u32 *ot;
    s32 h;
    s32 hw, hh;
    s32 radius;
    s32 i, j;
    POLY_G4 *poly;
    DR_TPAGE *tp;

    tpage = 64;
    ot = fade_getOt(g_gameState->gameData->drawBuf);
    if (fade->subtract == 0) {
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
    g_fadeWipeRot.vz = fade->angle;
    math_rotMatrixXYZ(&g_fadeWipeRot, &mat);
    mat.t[2] = 0;
    mat.t[1] = 0;
    mat.t[0] = 0;
    SetRotMatrix(&mat);
    SetTransMatrix(&mat);
    radius = SquareRoot0(hw * hw + hh * hh);
    setVector(&g_fadeWipeQuads[0][0], -radius, -radius, h);
    setVector(&g_fadeWipeQuads[0][1], 0, -radius, h);
    setVector(&g_fadeWipeQuads[0][2], -radius, radius, h);
    setVector(&g_fadeWipeQuads[0][3], 0, radius, h);
    setVector(&g_fadeWipeQuads[1][0], 0, -radius, h);
    setVector(&g_fadeWipeQuads[1][1], radius, -radius, h);
    setVector(&g_fadeWipeQuads[1][2], 0, radius, h);
    setVector(&g_fadeWipeQuads[1][3], radius, radius, h);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            RotTransPers(&g_fadeWipeQuads[i][j], (long *)&xy[j], &interp, &flag);
        }
        poly = (POLY_G4 *)g_fade.pkt;
        SetPolyG4(poly);
        if ((u32)fade->frame < g_fadeWipe.frames) {
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

void fadeWipe_getColors(u8 *out) {
    if ((u32)g_fade.frame < g_fadeWipe.frames) {
        out[0] = g_fadeWipe.out.r;
        out[1] = g_fadeWipe.out.g;
        out[2] = g_fadeWipe.out.b;
        out[4] = g_fadeWipe.startR;
        out[5] = g_fadeWipe.startG;
        out[6] = g_fadeWipe.startB;
        return;
    }
    out[0] = g_fadeWipe.endR;
    out[1] = g_fadeWipe.endG;
    out[2] = g_fadeWipe.endB;
    out[4] = g_fadeWipe.out.r;
    out[5] = g_fadeWipe.out.g;
    out[6] = g_fadeWipe.out.b;
}
