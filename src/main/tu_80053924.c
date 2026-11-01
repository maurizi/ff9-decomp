#include "ff9.h"

void game_onReset(void) {
}

void gamedata_resetEventFlags(void) {
    GameData *gd = g_gameState->gameData;
    gd->frogCount = 0;
    gd->stealCount = 0;
    gd->dragonKills = 0;
    gd->battleResult = 0;
    gd->battleFlags = 0;
    gd->rainCount = 0;
    gd->darkBattle = 0;
}

s32 chr_cureStatusBit(Unk800539FC *chr, s32 bit) {
    if (chr->status & bit) {
        chr->status &= ~bit;
        return 2;
    }
    return 1;
}

static inline s32 chr_cureStatusBit_inline(Unk800539FC *chr, s32 bit) {
    if (chr->status & bit) {
        chr->status &= ~bit;
        return 2;
    }
    return 1;
}

s32 chr_cureStatusMask(Character *chr, s32 mask) {
    s32 result = 1;
    u32 i;

    for (i = 0; i < 8; i++) {
        u8 hit = mask & (1 << i);
        if (hit) {
            if (chr_cureStatusBit_inline(chr, 1 << i) == 2) {
                result = 2;
            }
        }
    }
    return result;
}

u8 chr_hasStatus(Character *chr, s32 bits) {
    return (chr->status & bits) != 0;
}

s32 swirl_update(s16 startX, s16 startY) {
    s16 y;
    s16 x;
    s32 wobble;
    u8 i;
    s16 startAngle, startStep;
    GameState *gs;

    x = startX;
    y = startY;
    g_swirlParams = g_swirlVariant ? &g_swirlParamSets[1] : &g_swirlParamSets[0];
    g_swirlOt = g_swirlPkt = g_swirlBuf + (g_gameState->gameData->drawBuf << 10);
    ClearOTag(g_swirlOt, 5);
    g_swirlPkt += 5;
    if (g_swirlFrame < 0x79) {
        if (g_swirlFrame == 0) {
            gs = g_gameState;
            gs->gameData->flags &= ~GD_NO_FLIP;
            *(u32 *)&gs->drawEnv[0].isbg = *(u32 *)&gs->drawEnv[1].isbg = 1;
            g_swirlFlash = 0;
            g_swirlFade = g_swirlParams->fade;
            g_swirlAngle = startAngle = g_swirlParams->angle;
            g_swirlZoom = g_swirlParams->zoom;
            g_swirlAngleStep = startStep = g_swirlParams->angleStep;
            if ((startX ^ startY) & 1) {
                g_swirlAngle = -startAngle;
                g_swirlAngleStep = -startStep;
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
            wobble = math_sin(g_swirlFrame * 8);
            x += wobble * (160 - x) / 4096;
            y += wobble * (g_gameState->drawEnv->clip.h / 2 - y) / 4096;
        }
        g_swirlPkt = swirl_buildMesh(g_swirlPkt, x, y);
        i = g_gameState->gameData->drawBuf;
        SetDrawMove(g_swirlPkt, &g_gameState->dispEnv[i].disp, g_gameState->drawEnv[i].clip.x, g_gameState->drawEnv[i].clip.y);
        addPrim(g_swirlOt, g_swirlPkt);
        g_swirlPkt += 8;
        swirl_drawMesh();
        if (g_swirlFrame >= g_swirlParams->fadeStart) {
            g_swirlFade -= g_swirlParams->fadeStep;
            if ((s16)g_swirlFade < 0) {
                g_swirlFade = 0;
            }
        }
        if (g_swirlFrame >= g_swirlParams->flashStart) {
            swirl_drawFlash(x, y);
            g_swirlFlash += g_swirlParams->flashStep;
            if ((s16)g_swirlFlash >= 0x1000) {
                g_swirlFlash = 0xFFF;
            }
        }
        g_swirlAngle += g_swirlAngleStep;
        g_swirlZoom += g_swirlParams->zoomStep;
        if (g_swirlZoom > 0x1400) {
            g_swirlZoom = 0x1400;
        }
        setlen(g_swirlPkt, 2);
        g_swirlPkt[1] = _get_mask(1, 0);
        g_swirlPkt[2] = 0;
        addPrim(g_swirlOt, g_swirlPkt);
        g_swirlPkt += 4;
    }
    SetDrawEnv((DR_ENV *)g_swirlPkt, &g_gameState->drawEnv[g_gameState->gameData->drawBuf]);
    addPrim(g_swirlOt, g_swirlPkt);
    g_swirlPkt += 16;
    setlen(g_swirlPkt, 2);
    g_swirlPkt[1] = _get_mask(0, 0);
    g_swirlPkt[2] = 0;
    addPrim(&g_swirlOt[3], g_swirlPkt);
    g_swirlPkt += 4;
    setaddr(&g_swirlOt[4], getaddr(g_gameState->gameData->ot[g_gameState->gameData->drawBuf]));
    setaddr(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], g_swirlOt);
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
    u32 flash;
    u8 c1, c2;
    u32 xy;
    u32 g2;

    p = g_swirlPkt;
    flash = g_swirlFlash << 16;
    c1 = flash >> 20;
    c2 = flash >> 23;
    g2 = (c2 << 16) | (c2 << 8) | c2;
    setlen(p, 1);
    p[1] = get_mode(0, 1, 0x140);
    p += 7;
    for (i = 0; i < 4; i++) {
        p[0] = 0x06000000;
        p[1] = ((c1 << 16) | (c1 << 8) | c1) | GPU_CODE(GPU_POLY_G3 | GPU_ABE);
        p[2] = ((g_swirlFlashY[i] * g_gameState->drawEnv->clip.h) << 16) | ((g_swirlFlashX[i] * 320) & 0xFFFF);
        p[3] = (c1 << 16) | (c1 << 8) | c1;
        p[4] = ((g_swirlFlashY[i + 1] * g_gameState->drawEnv->clip.h) << 16) | ((g_swirlFlashX[i + 1] * 320) & 0xFFFF);
        p[5] = g2;
        p[6] = (y << 16) | (x & 0xFFFF);
        setaddr(p - 7, p);
        p += 7;
    }
    setaddr(p - 7, getaddr(&g_swirlOt[2]));
    setaddr(&g_swirlOt[2], g_swirlPkt);
    g_swirlPkt = p;
}

u32 *swirl_buildMesh(u32 *work, s32 x, s32 y) {
    s16 xs[9];
    s16 ys[9];
    SVECTOR sv;
    VECTOR vec;
    MATRIX mat;
    s32 i;
    s32 dx, dy;
    s32 t;
    s32 ex, ey;
    s16 *py;
    s16 *col;
    DVECTOR *pts;
    WarpVert *p;

    g_swirlColumns = (DVECTOR *)work;
    g_swirlMesh = (WarpVert *)((u8 *)work + 0x24);
    work = (u32 *)((u8 *)work + 0x2AC);
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
    math_rotMatrixXYZ(&sv, &mat);
    vec.vx = vec.vy = g_swirlParams->scale;
    vec.vz = 0;
    ScaleMatrix(&mat, &vec);
    mat.t[0] = x;
    mat.t[1] = y;
    mat.t[2] = 0;
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);
    p = g_swirlMesh;
    for (ex = 0; ex < 9; ex++) {
        col = py;
        sv.vx = xs[ex] - x;
        for (ey = 0; ey < 9; ey++) {
            sv.vy = col[ey] - y;
            gte_ldv0(&sv);
            gte_rtv0tr();
            gte_stlvnl(&vec);
            p->x = vec.vx;
            p->y = vec.vy;
            p++;
        }
    }
    return work;
}

void swirl_drawMesh(void) {
    u32 *p;
    WarpVert *vert;
    s32 i;
    s32 j;
    s32 shade;
    u32 rgbc;
    s32 u0;
    s32 u1;
    u32 tp;

    p = g_swirlPkt;
    shade = (g_swirlFade >> 4) & 0xFF;
    rgbc = (shade << 16) | (shade << 8) | shade | GPU_CODE(GPU_POLY_FT4 | GPU_ABE);
    p += 10;
    for (i = 0; i < 8; i++) {
        vert = &g_swirlMesh[i * 9];
        tp = ((((g_swirlColumns[i].vx + g_gameState->dispEnv[g_gameState->gameData->drawBuf].disp.x) & 0x3FF) >> 6) | 0x100) << 16;
        u0 = g_swirlColumns[i].vx & 0x3F;
        u1 = u0 + (g_swirlColumns[i + 1].vx - g_swirlColumns[i].vx) - 1;
        for (j = 0; j < 8; j++) {
            p[0] = 0x09000000;
            p[1] = rgbc;
            p[2] = (vert->y << 16) | (u16)vert->x;
            p[3] = (g_swirlColumns[j].vy << 8) | (u8)u0;
            p[4] = (vert[9].y << 16) | (u16)vert[9].x;
            p[5] = (g_swirlColumns[j].vy << 8) | (u8)u1 | tp;
            p[6] = (vert[1].y << 16) | (u16)vert[1].x;
            p[7] = ((g_swirlColumns[j + 1].vy - 1) << 8) | (u8)u0;
            p[8] = (vert[10].y << 16) | (u16)vert[10].x;
            p[9] = ((g_swirlColumns[j + 1].vy - 1) << 8) | (u8)u1;
            setaddr(p - 10, p);
            p += 10;
            vert++;
        }
    }
    setaddr(p - 10, getaddr(&g_swirlOt[1]));
    setaddr(&g_swirlOt[1], g_swirlPkt + 10);
    g_swirlPkt = p;
}
