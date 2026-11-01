#include "ff9.h"

void math_rotMatrixXYZ(void *rot, void *out) {
    math_buildRotMatrixXYZ(rot, out, 0);
}

void math_rotMatrixXYZ_gte(void *rot, void *out) {
    math_buildRotMatrixXYZ(rot, out, 1);
}

MATRIX *math_buildRotMatrixXYZ(SVECTOR *rot, MATRIX *mtx, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 tx, ty, tz;
    s16 t1, t2;

    x = rot->vx & 0xFFF;
    y = rot->vy & 0xFFF;
    z = rot->vz & 0xFFF;
    ISIN(sx, rot->vx, x);
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
        mtx->m[0][0] = (cy * cz) >> 12;
        mtx->m[0][1] = (-cy * sz) >> 12;
        mtx->m[0][2] = sy;
        mtx->m[1][0] = (t1 * cz + cx * sz) >> 12;
        mtx->m[1][1] = (-t1 * sz + cx * cz) >> 12;
        mtx->m[1][2] = (-sx * cy) >> 12;
        mtx->m[2][0] = (-t2 * cz + sx * sz) >> 12;
        mtx->m[2][1] = (t2 * sz + sx * cz) >> 12;
        mtx->m[2][2] = (cx * cy) >> 12;
    } else {
        t1 = (sx * sy) >> 12;
        t2 = (cx * sy) >> 12;
        mtx->m[0][0] = (cy * cz) >> 12;
        mtx->m[0][1] = (-cy * sz) >> 12;
        mtx->m[0][2] = sy;
        mtx->m[1][0] = (t1 * cz + cx * sz) >> 12;
        mtx->m[1][1] = (-t1 * sz + cx * cz) >> 12;
        mtx->m[1][2] = (-sx * cy) >> 12;
        mtx->m[2][0] = (-t2 * cz + sx * sz) >> 12;
        mtx->m[2][1] = (t2 * sz + sx * cz) >> 12;
        mtx->m[2][2] = (cx * cy) >> 12;
    }
    return mtx;
}

void math_rotMatrixZXY(void *rot, void *out) {
    math_buildRotMatrixZXY(rot, out, 0);
}

void math_rotMatrixZXY_gte(void *rot, void *out) {
    math_buildRotMatrixZXY(rot, out, 1);
}

MATRIX *math_buildRotMatrixZXY(SVECTOR *rot, MATRIX *mtx, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 tx, ty, tz;
    s16 t1, t2;

    x = rot->vx & 0xFFF;
    y = rot->vy & 0xFFF;
    z = rot->vz & 0xFFF;
    ISIN(sx, rot->vx, x);
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
        mtx->m[0][0] = (cz * cy - t1 * sy) >> 12;
        mtx->m[0][1] = (-cx * sz) >> 12;
        mtx->m[0][2] = (cz * sy + t1 * cy) >> 12;
        mtx->m[1][0] = (sz * cy + t2 * sy) >> 12;
        mtx->m[1][1] = (cx * cz) >> 12;
        mtx->m[1][2] = (sz * sy - t2 * cy) >> 12;
        mtx->m[2][0] = (-cx * sy) >> 12;
        mtx->m[2][1] = sx;
        mtx->m[2][2] = (cx * cy) >> 12;
    } else {
        t1 = (sx * sz) >> 12;
        t2 = (sx * cz) >> 12;
        mtx->m[0][0] = (cz * cy - t1 * sy) >> 12;
        mtx->m[0][1] = (-cx * sz) >> 12;
        mtx->m[0][2] = (cz * sy + t1 * cy) >> 12;
        mtx->m[1][0] = (sz * cy + t2 * sy) >> 12;
        mtx->m[1][1] = (cx * cz) >> 12;
        mtx->m[1][2] = (sz * sy - t2 * cy) >> 12;
        mtx->m[2][0] = (-cx * sy) >> 12;
        mtx->m[2][1] = sx;
        mtx->m[2][2] = (cx * cy) >> 12;
    }
    return mtx;
}

void math_rotMatrixZYX(void *rot, void *out) {
    math_buildRotMatrixZYX(rot, out, 0);
}

void math_rotMatrixZYX_gte(void *rot, void *out) {
    math_buildRotMatrixZYX(rot, out, 1);
}

MATRIX *math_buildRotMatrixZYX(SVECTOR *rot, MATRIX *mtx, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 cxCz;
    s32 tx, ty, tz;
    s16 t1, t2;
    s32 czSx, szSx, cxSz, mac1, mac2, mac3;

    x = rot->vx & 0xFFF;
    y = rot->vy & 0xFFF;
    z = rot->vz & 0xFFF;
    ISIN(sx, rot->vx, x);
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
        mtx->m[0][0] = (cy * cz) >> 12;
        mtx->m[0][1] = (-sz * cx + t1 * sx) >> 12;
        mtx->m[0][2] = (sz * sx + t1 * cx) >> 12;
        mtx->m[1][0] = (cy * sz) >> 12;
        mtx->m[1][1] = (cz * cx + t2 * sx) >> 12;
        mtx->m[1][2] = (-cz * sx + t2 * cx) >> 12;
        mtx->m[2][0] = -sy;
        mtx->m[2][1] = (cy * sx) >> 12;
        mtx->m[2][2] = (cy * cx) >> 12;
    } else {
        gte_ldir0123(cy, cz, sz, sx);
        mtx->m[2][0] = -sy;
        gte_gpf12_n1();
        czSx = cz * sx;
        gte_stmac123(mac1, mac2, mac3);
        gte_ldir0123(cx, cy, sz, cz);
        do {
            szSx = sz * sx;
            mtx->m[0][0] = mac1;
            gte_gpf12_b();
            czSx >>= 12;
            szSx >>= 12;
            mtx->m[1][0] = mac2;
            mtx->m[2][1] = mac3;
        } while (0);
        gte_stmac123(mac1, cxSz, cxCz);
        gte_ldir0123(sy, czSx, szSx, cxCz);
        mtx->m[2][2] = mac1;
        gte_gpf12_n1();
        gte_stmac123(mac1, mac2, mac3);
        gte_ldir12(cxCz, cxSz);
        mac1 -= cxSz;
        mtx->m[0][1] = mac1;
        gte_gpf12_b();
        mac2 += cxCz;
        mtx->m[1][1] = mac2;
        gte_stmac12(mac1, mac2);
        mac1 += szSx;
        mac2 -= czSx;
        mtx->m[0][2] = mac1;
        mtx->m[1][2] = mac2;
    }
    return mtx;
}

void math_rotMatrixYXZ(void *rot, void *out) {
    math_buildRotMatrixYXZ(rot, out, 0);
}

void math_rotMatrixYXZ_gte(void *rot, void *out) {
    math_buildRotMatrixYXZ(rot, out, 1);
}

MATRIX *math_buildRotMatrixYXZ(SVECTOR *rot, MATRIX *mtx, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 szSy;
    s32 tx, ty, tz;
    s16 t1, t2;
    s32 cyCz, syCz, szCy, mac1, mac2, mac3;

    x = rot->vx & 0xFFF;
    y = rot->vy & 0xFFF;
    z = rot->vz & 0xFFF;
    ISIN(sx, rot->vx, x);
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
        mtx->m[0][0] = (cy * cz + t1 * sz) >> 12;
        mtx->m[0][1] = (-cy * sz + t1 * cz) >> 12;
        mtx->m[0][2] = (cx * sy) >> 12;
        mtx->m[1][0] = (cx * sz) >> 12;
        mtx->m[1][1] = (cx * cz) >> 12;
        mtx->m[1][2] = -sx;
        mtx->m[2][0] = (-sy * cz + t2 * sz) >> 12;
        mtx->m[2][1] = (sy * sz + t2 * cz) >> 12;
        mtx->m[2][2] = (cx * cy) >> 12;
    } else {
        gte_ldir0123(cx, sy, cy, cz);
        mtx->m[1][2] = -sx;
        gte_gpf12_n1();
        cyCz = cy * cz;
        gte_stmac123(mac1, mac2, mac3);
        gte_ldir0123(sz, cx, cy, sy);
        do {
            syCz = sy * cz;
            mtx->m[0][2] = mac1;
            gte_gpf12_b();
            cyCz >>= 12;
            syCz >>= 12;
            mtx->m[2][2] = mac2;
            mtx->m[1][1] = mac3;
        } while (0);
        gte_stmac123(mac1, szCy, szSy);
        gte_ldir0123(sx, syCz, cyCz, szSy);
        mtx->m[1][0] = mac1;
        gte_gpf12_n1();
        gte_stmac123(mac1, mac2, mac3);
        gte_ldir12(szSy, szCy);
        mac1 -= szCy;
        mtx->m[0][1] = mac1;
        gte_gpf12_b();
        mac2 += szSy;
        mtx->m[2][1] = mac2;
        gte_stmac12(mac1, mac2);
        mac1 += cyCz;
        mac2 -= syCz;
        mtx->m[0][0] = mac1;
        mtx->m[2][0] = mac2;
    }
    return mtx;
}

void fadeFlat_start(FadeState *fade) {
    s32 steps;
    if (fade->frames == 0) {
        fade->frames = 16;
    }
    g_fadeFlat.color.r = g_fadeFlat.startR = fade->color.b[0];
    g_fadeFlat.color.g = g_fadeFlat.startG = fade->color.b[1];
    g_fadeFlat.color.b = g_fadeFlat.startB = fade->color.b[2];
    g_fadeFlat.endR = fade->endR;
    g_fadeFlat.endG = fade->endG;
    g_fadeFlat.endB = fade->endB;
    g_fadeFlat.deltaR = g_fadeFlat.endR - g_fadeFlat.startR;
    g_fadeFlat.deltaG = g_fadeFlat.endG - g_fadeFlat.startG;
    g_fadeFlat.deltaB = g_fadeFlat.endB - g_fadeFlat.startB;
    steps = fade->frames - 1;
    if (steps <= 0) {
        steps = 1;
    }
    g_fadeFlat.stepR = fade_divRoundOut(g_fadeFlat.deltaR, steps);
    steps = fade->frames - 1;
    if (steps <= 0) {
        steps = 1;
    }
    g_fadeFlat.stepG = fade_divRoundOut(g_fadeFlat.deltaG, steps);
    steps = fade->frames - 1;
    if (steps <= 0) {
        steps = 1;
    }
    g_fadeFlat.stepB = fade_divRoundOut(g_fadeFlat.deltaB, steps);
}

void fadeFlat_update(FadeState *fade) {
    g_fadeFlat.color.r = fade_stepClamp(g_fadeFlat.color.r, g_fadeFlat.stepR, g_fadeFlat.endR);
    g_fadeFlat.color.g = fade_stepClamp(g_fadeFlat.color.g, g_fadeFlat.stepG, g_fadeFlat.endG);
    g_fadeFlat.color.b = fade_stepClamp(g_fadeFlat.color.b, g_fadeFlat.stepB, g_fadeFlat.endB);
    if (fade->eased != 0) {
        g_fadeFlat.out.r = fade_ease(g_fadeFlat.color.r, g_fadeFlat.startR, g_fadeFlat.deltaR);
        g_fadeFlat.out.g = fade_ease(g_fadeFlat.color.g, g_fadeFlat.startG, g_fadeFlat.deltaG);
        g_fadeFlat.out.b = fade_ease(g_fadeFlat.color.b, g_fadeFlat.startB, g_fadeFlat.deltaB);
    } else {
        g_fadeFlat.out = g_fadeFlat.color;
    }
    fade->color.b[0] = g_fadeFlat.out.r;
    fade->color.b[1] = g_fadeFlat.out.g;
    fade->color.b[2] = g_fadeFlat.out.b;
}

void fadeFlat_seek(FadeState *fade, s32 steps) {
    g_fadeFlat.color.r = fade_stepClamp(g_fadeFlat.color.r, g_fadeFlat.stepR * steps, g_fadeFlat.endR);
    g_fadeFlat.color.g = fade_stepClamp(g_fadeFlat.color.g, g_fadeFlat.stepG * steps, g_fadeFlat.endG);
    g_fadeFlat.color.b = fade_stepClamp(g_fadeFlat.color.b, g_fadeFlat.stepB * steps, g_fadeFlat.endB);
    if (fade->eased != 0) {
        g_fadeFlat.out.r = fade_ease(g_fadeFlat.color.r, g_fadeFlat.startR, g_fadeFlat.deltaR);
        g_fadeFlat.out.g = fade_ease(g_fadeFlat.color.g, g_fadeFlat.startG, g_fadeFlat.deltaG);
        g_fadeFlat.out.b = fade_ease(g_fadeFlat.color.b, g_fadeFlat.startB, g_fadeFlat.deltaB);
    } else {
        g_fadeFlat.out = g_fadeFlat.color;
    }
    fade->color.b[0] = g_fadeFlat.out.r;
    fade->color.b[1] = g_fadeFlat.out.g;
    fade->color.b[2] = g_fadeFlat.out.b;
}

void fadeFlat_draw(FadeState *fade) {
    u32 *ot = fade_getOt(g_gameState->gameData->drawBuf);
    s32 tp = 0x40;
    TILE *tile;
    u8 *dr;

    if (fade->subtract == 0) {
        tp = 0x20;
    }
    tile = (TILE *)g_fade.pkt;
    SetTile(tile);
    tile->r0 = g_fadeFlat.out.r;
    tile->g0 = g_fadeFlat.out.g;
    tile->b0 = g_fadeFlat.out.b;
    tile->y0 = 0;
    tile->x0 = 0;
    tile->w = g_gameState->dispEnv->disp.w;
    tile->h = g_gameState->dispEnv->disp.h;
    SetSemiTrans(tile, 1);
    addPrim(ot, tile);
    g_fade.pkt += sizeof(TILE);
    dr = g_fade.pkt;
    SetDrawTPage(dr, 0, 1, tp);
    addPrim(ot, dr);
    g_fade.pkt += sizeof(DR_TPAGE);
}
