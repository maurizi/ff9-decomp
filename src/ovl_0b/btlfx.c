#include "ovl_0b.h"

s32 btlfx_align4(s32 value) {
    s32 aligned = value & ~3;
    if (value & 3) {
        aligned += 4;
    }
    return aligned;
}

s32 btlfx_rangeProgress(s32 frame, s32 start, s32 length, s32 dir, s32 range, s32 *out) {
    if (frame >= start && start + length >= frame) {
        *out = (frame - start) * range / length;
        if (dir < 0) {
            *out = range - *out;
        }
        return 1;
    }
    return 0;
}

void btlfx_rotateOffset(SVECTOR *rot, s32 dist, s32 out) {
    RotMatrix33 rotMat;

    math_rotMatrixYXZ(rot, &rotMat);
    gte_SetRotMatrix(&rotMat);
    gte_ldtr0();
    btlfx_transformPoint(dist, 0, out);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_pointAhead(s16 *origin, s16 *rot, s32 dist, s16 *out) {
    MATRIX mat;

    math_rotMatrixYXZ(rot, &mat);
    gte_SetRotMatrix(&mat);
    mat.t[0] = origin[0];
    mat.t[1] = origin[1];
    mat.t[2] = origin[2];
    gte_SetTransMatrix(&mat);
    btlfx_transformPoint(dist, 0, (s32)out);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_matrixPointAhead(MATRIX *mat, s32 dist, s32 out) {
    gte_SetRotMatrix(mat);
    gte_SetTransMatrix(mat);
    btlfx_transformPoint(dist, 0, out);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_matrixAngles(MATRIX *mat, s16 *outRot) {
    s16 dir[4];
    s32 origin[2];

    gte_SetRotMatrix(mat);
    gte_SetTransMatrix(mat);
    origin[1] = 0;
    origin[0] = 0;
    btlfx_transformPoint(0x1000, 0, (s32)dir);
    btlfx_aimAngles((s16 *)origin, dir, outRot);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

s32 btlfx_angleDiff(s32 target, s32 angle, s32 *outDiff) {
    s32 sign;
    s32 diff;
    s32 ad;
    s32 neg;
    target &= 0xFFF;
    angle &= 0xFFF;
    sign = -1;
    if (angle < target) {
        sign = 1;
    }
    diff = target - angle;
    neg = diff < 0;
    ad = diff;
    if (neg) {
        ad = -ad;
    }
    if (ad > 0x800) {
        sign = -sign;
        ad = 0x1000 - ad;
    }
    if (outDiff != NULL) {
        *outDiff = ad;
    }
    return sign;
}

void btlfx_turnToward(s16 *target, s16 *rot, s32 rate) {
    s32 diff;
    s32 sign;
    s32 step;
    sign = btlfx_angleDiff(target[0], rot[0], &diff);
    step = (diff * rate) >> 12;
    rot[0] += step * sign;
    diff = step;
    sign = btlfx_angleDiff(target[1], rot[1], &diff);
    step = (diff * rate) >> 12;
    rot[1] += step * sign;
    diff = step;
}

void btlfx_setWorldTrans(SVECTOR *pos, MATRIX *mat) {
    MATRIX localMat;

    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    gte_rtv0tr();
    if (mat == NULL) {
        mat = &localMat;
    }
    gte_stlvnl(mat->t);
    gte_SetTransMatrix(mat);
}

void btlfx_setRotScale(SVECTOR *rot, VECTOR *scale, MATRIX *mat, s32 useLight) {
    MATRIX localMat;
    RotMatrix33 lightMat;

    if (mat == NULL) {
        mat = &localMat;
    }
    math_rotMatrixYXZ(rot, mat);
    if (scale != NULL) {
        ScaleMatrix(mat, scale);
    }
    if (rot->pad != 0) {
        gte_ldclmv(mat);
        gte_rtir();
        gte_stclmv(mat);
        gte_ldclmv((s16 *)mat + 1);
        gte_rtir();
        gte_stclmv((s16 *)mat + 1);
        gte_ldclmv((s16 *)mat + 2);
        gte_rtir();
        gte_stclmv((s16 *)mat + 2);
    }
    if (useLight != 0) {
        gte_MulMatrix0(&lightMat, mat, &lightMat);
        gte_SetLightMatrix(&lightMat);
    }
    gte_SetRotMatrix(mat);
}

void btlfx_offsetPoint(SVECTOR *origin, s32 out, s32 axial, XY32 *axis, s32 radius, s32 spin) {
    struct {
        XY32 xy;
        s16 z;
    } angles;
    MATRIX mat;

    angles.xy = *axis;
    angles.z = spin;
    math_rotMatrixYXZ(&angles, &mat);
    gte_SetRotMatrix(&mat);
    mat.t[0] = origin->vx;
    mat.t[1] = origin->vy;
    mat.t[2] = origin->vz;
    gte_SetTransMatrix(&mat);
    btlfx_transformPoint(axial, radius, out);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_forwardAngles(s16 *outRot, XY32 *axis, s32 spin) {
    s16 dir[4];
    s32 origin[2];
    struct {
        XY32 xy;
        s16 z;
    } angles;
    MATRIX mat;

    angles.xy = *axis;
    angles.z = spin;
    math_rotMatrixYXZ(&angles, &mat);
    gte_SetRotMatrix(&mat);
    gte_ldtr0();
    origin[1] = 0;
    origin[0] = 0;
    btlfx_transformPoint(0, 0x1000, (s32)dir);
    btlfx_aimAngles((s16 *)origin, dir, outRot);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_toLocalPolar(SVECTOR *pos, SVECTOR *center, SVECTOR *axis, s32 outAxial, s32 outRadius, s32 *outAngle) {
    SVECTOR rel;
    struct {
        XY32 xy;
        s16 z;
    } angles;
    RotMatrix33 mat;

    rel.vx = pos->vx - center->vx;
    rel.vy = pos->vy - center->vy;
    rel.vz = pos->vz - center->vz;
    angles.xy.w = (-axis->vx & 0xFFFF) | (-axis->vy << 16);
    angles.z = 0;
    math_rotMatrixXYZ(&angles.xy, &mat);
    gte_SetRotMatrix(&mat);
    gte_ldtr0();
    *outAngle = btlfx_transformPolar(&rel, outAxial, outRadius);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

/* adds prim to ot with the given code/len word; tpage != BTLFX_ABR_OPAQUE also adds a DR_TPAGE (semi-trans mode) */
void btlfx_linkPrimAbr(u32 code, u32 *ot, void *prim, s32 tpage) {
    DR_TPAGE *drMode;

    addPrimLenIn(ot, prim, code);
    if (tpage != BTLFX_ABR_OPAQUE) {
        drMode = (DR_TPAGE *)g_btlfxEnv->prim;
        g_btlfxEnv->prim = (u8 *)(drMode + 1);
        drMode->code[0] = ((tpage & 3) << 5) | 0xE1000200;
        addPrimLenIn(ot, drMode, PRIM_TAG(DR_TPAGE));
    }
}

void btlfx_addPrim(u32 code, s32 depth, void *prim) {
    s32 z = (depth - g_btlfxEnv->otZ) >> 2;
    if ((u32)z < 0x1000) {
        addPrimLen(&g_btlfxEnv->ot[z], prim, code);
    }
}

void btlfx_addPrimAbr(s32 code, s32 depth, void *prim, s32 abr) {
    u32 i = (depth - g_btlfxEnv->otZ) >> 2;
    if (i < 0x1000) {
        ((void (*)())btlfx_linkPrimAbr)(code, &g_btlfxEnv->ot[i], prim, abr);
    }
}

DR_TPAGE *btlfx_allocDrMode(s32 tpage, s32 abr) {
    DR_TPAGE *drMode = (DR_TPAGE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(drMode + 1);
    drMode->code[0] = tpage | ((abr & 3) << 5) | 0xE1000200;
    return drMode;
}

void btlfx_resetMaskBit(void) {
    s32 otLen;
    Unk800D0940 *pkt;
    u32 *ot;
    if (g_btlfxEnv->prim != NULL) {
        g_btlfxMaskResetBusy = otLen = 1;
        pkt = (Unk800D0940 *)g_btlfxEnv->prim;
        ot = &pkt->ot;
        g_btlfxEnv->prim = (u8 *)(pkt + 1);
        setlen(pkt, PRIM_LEN(DR_STP));
        pkt->stp = GPU_STP(0, 0);
        pkt->nop = 0;
        ClearOTag(ot, otLen);
        addPrim(ot, pkt);
        EnterCriticalSection();
        DrawOTag(ot);
        ExitCriticalSection();
        g_btlfxMaskResetBusy = 0;
    }
}

void btlfx_drawScreenTint(u32 *ot, s32 abr, s32 red, s32 green, s32 blue) {
    TILE *tile;
    s32 i;
    tile = (TILE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(tile + 8);
    for (i = 0; i < 8; i++, tile++) {
        tile->code = abr == BTLFX_ABR_OPAQUE ? GPU_TILE : GPU_TILE | GPU_ABE;
        tile->w = 0x50;
        tile->h = 0x6E;
        tile->x0 = (i & 3) * 0x50;
        tile->r0 = red;
        tile->g0 = green;
        tile->b0 = blue;
        tile->y0 = (i >> 2) * 0x6E;
        btlfx_linkPrimAbr(0x03000000, ot, tile, abr);
    }
}

/* Draws a grid of gouraud quads whose rows ripple with a sine wave. */
void btlfx_drawRippleGrid(s32 ot, s32 top, s32 cols, s32 rows, s32 rowH, s32 abr, CVECTOR *colors, s32 amp, s32 phase,
                   s32 colStep, s32 rowStep) {
    POLY_G4 *poly;
    CVECTOR *rowColor;
    s32 row;
    s32 col;
    s32 y;
    s32 curPhase;

    rowColor = colors;
    row = 0;
    poly = (POLY_G4 *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(poly + cols * rows);
    y = top;
    if (rows > 0) {
        do {
            curPhase = phase + row * rowStep;
            rowColor[1].cd = (abr == BTLFX_ABR_OPAQUE) ? GPU_POLY_G4 : GPU_POLY_G4 | GPU_ABE;
            rowColor[0].cd = rowColor[1].cd;
            for (col = 0; col < cols; col++, poly++, curPhase += colStep) {
                poly->x0 = poly->x2 = (col * 0x140) / cols;
                poly->x1 = poly->x3 = ((col + 1) * 0x140) / cols;
                if (row == 0) {
                    poly->y0 = poly->y1 = y;
                } else {
                    poly->y0 = y + ((math_sin(curPhase) * amp) >> 12);
                    poly->y1 = y + ((math_sin(curPhase + colStep) * amp) >> 12);
                }
                curPhase += rowStep;
                poly->y2 = (y + rowH) + ((math_sin(curPhase) * amp) >> 12);
                poly->y3 = (y + rowH) + ((math_sin(curPhase + colStep) * amp) >> 12);
                curPhase -= rowStep;
                *(s32 *)&poly->r0 = *(s32 *)&poly->r1 = *(s32 *)rowColor;
                *(s32 *)&poly->r2 = *(s32 *)&poly->r3 = *(s32 *)&rowColor[1];
                btlfx_linkPrimAbr(0x08000000, ot, poly, abr);
            }
            row++;
            y += rowH;
            rowColor++;
        } while (row < rows);
    }
}

void btlfx_drawTrail3D(s32 ring, s32 *anchor, s32 pointCount, s32 texV, s32 texU, s32 texH, s32 texW, s32 tpage, s32 clut, s32 alpha,
                   u32 *c1, u32 *c2, s32 tp) {
    Unk800D50A0Ctx ctx;
    u8 *prim;
    s32 inv;

    if (alpha < 0) {
        alpha = 0x1000;
    }
    gte_DpqColorFar_nst(c1, alpha);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + pointCount * 0x34;
    gte_strgb(&ctx.color);
    gte_DpqColorFar_nst(c2, alpha);
    ctx.u1 = (texU + texW) << 8;
    ctx.u0 = texU << 8;
    gte_strgb(&ctx.farColor);
    ctx.tpage = (((tp & 3) << 5) | tpage) << 16;
    ctx.clut = clut << 16;
    ctx.color.b[3] = (tp == BTLFX_ABR_OPAQUE) ? GPU_POLY_GT4 : GPU_POLY_GT4 | GPU_ABE;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    if (pointCount != 0) {
        inv = 0x1000 / pointCount;
        ctx.fadeStep = inv;
        ctx.fade = inv;
        ctx.anchorXY = anchor[0];
        inv = anchor[1];
        ctx.ring = ring;
        ctx.v0 = texV;
        ctx.v1 = texH;
        ctx.otZ = g_btlfxEnv->otZ;
        ctx.anchorZ = inv;
        btlfx_emitTrailRibbon3D(pointCount, &ctx, prim, g_btlfxEnv->ot);
    }
}

void btlfx_drawTrailFlat(s32 ring, s32 pointCount, s32 texV, s32 texU, s32 texH, s32 texW, s32 tpage, s32 clut, s32 alpha, u32 *c1,
                   u32 *c2, s32 tp) {
    Unk800D4D6CCtx ctx;
    u8 *prim;
    s32 inv;
    s32 sinYaw;

    if (alpha < 0) {
        alpha = 0x1000;
    }
    gte_DpqColorFar_nst(c1, alpha);
    gte_strgb(&ctx.color);
    gte_DpqColorFar_nst(c2, alpha);
    if (tpage >= 0) {
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + pointCount * 0x34;
        ctx.u1 = (texU + texW) << 8;
        ctx.u0 = texU << 8;
        sinYaw = math_sin(-g_btlfxEnv->camera->yaw + 0x400);
        ctx.mode.sinCos = (sinYaw & 0xFFFF) | (math_cos(-g_btlfxEnv->camera->yaw + 0x400) << 16);
        ctx.color.b[3] = (tp != BTLFX_ABR_OPAQUE) ? GPU_POLY_GT4 | GPU_ABE : GPU_POLY_GT4;
        ctx.tpage = (((tp & 3) << 5) | tpage) << 16;
        ctx.clut = clut << 16;
        ctx.v0 = texV;
        ctx.v1 = texH;
    } else {
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + pointCount * 0x1C;
        ctx.mode.abr = (tp & 3) << 5;
        ctx.color.b[3] = (tp == BTLFX_ABR_OPAQUE) ? GPU_LINE_G2 : GPU_LINE_G2 | GPU_ABE;
    }
    gte_strgb(&ctx.farColor);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    if (pointCount != 0) {
        inv = 0x1000 / pointCount;
        ctx.ring = ring;
        ctx.fadeStep = inv;
        ctx.fade = inv;
        ctx.otZ = g_btlfxEnv->otZ;
        if (tpage >= 0) {
            btlfx_emitTrailRibbonFlat(pointCount, &ctx, prim, g_btlfxEnv->ot);
        } else {
            btlfx_emitTrailLines(pointCount, &ctx, prim, g_btlfxEnv->ot);
        }
    }
}

void btlfx_drawTrailScreen(s32 ring, s32 pointCount, s32 texV, s32 texU, s32 texH, s32 texW, s32 tpage, s32 clut, s32 alpha, u32 *c1,
                   u32 *c2, s32 tp) {
    Unk800D5C30Ctx ctx;
    u8 *prim;
    s32 inv;

    if (alpha < 0) {
        alpha = 0x1000;
    }
    gte_DpqColorFar_nst(c1, alpha);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + pointCount * 0x34;
    gte_strgb(&ctx.color);
    gte_DpqColorFar_nst(c2, alpha);
    ctx.u1 = (texU + texW) << 8;
    ctx.u0 = texU << 8;
    ctx.tpage = (((tp & 3) << 5) | tpage) << 16;
    ctx.clut = clut << 16;
    ctx.color.b[3] = (tp == BTLFX_ABR_OPAQUE) ? GPU_POLY_GT4 : GPU_POLY_GT4 | GPU_ABE;
    gte_strgb(&ctx.farColor);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    if (pointCount != 0) {
        inv = 0x1000 / pointCount;
        ctx.ring = ring;
        ctx.v0 = texV;
        ctx.v1 = texH;
        ctx.fadeStep = inv;
        ctx.fade = inv;
        ctx.otZ = g_btlfxEnv->otZ;
        btlfx_emitTrailRibbonScreen(pointCount, &ctx, prim, g_btlfxEnv->ot);
    }
}

void btlfx_drawTrail(u8 *cursor, s32 newPoint, s32 *anchor, s32 frame, s32 scale, s32 alpha, s32 tp, u8 *out, s32 mode) {
    s32 pointCount;
    s32 k;
    s32 cnt;
    s32 texV;
    s32 texU;
    s32 texH;
    s32 tpage;
    s32 texW;
    s32 clut;
    s32 width;
    u32 *data;

    if (out == NULL) {
        return;
    }
    pointCount = ((Unk800D1494 *)cursor)->length;
    k = ((Unk800D1494 *)cursor)->frameOfs;
    cnt = ((Unk800D1494 *)cursor)->frameCount;
    tpage = ((Unk800D1494 *)cursor)->tpage;
    clut = ((Unk800D1494 *)cursor)->clut;
    texH = ((Unk800D1494 *)cursor)->h - 1;
    texW = ((Unk800D1494 *)cursor)->w - 1;
    cursor += sizeof(Unk800D1494);
    if (frame == -1) {
        out[1] = pointCount;
        out[0] = pointCount;
        out[2] = 0;
        return;
    }
    data = (u32 *)cursor;
    if (frame >= cnt) {
        frame = cnt - 1;
    }
    cursor = (u8 *)data + (k * 8 + frame * 4);
    width = cursor[2];
    width = (width * scale) >> 12;
    texV = cursor[0];
    texU = cursor[1];
    k = cursor[3];
    data += k * 2;
    if (newPoint != 0) {
        pointCount = btlfx_pushTrailPoint(out, pointCount, width, newPoint);
    }
    if (mode == 0) {
        btlfx_drawTrailScreen((s32)out, pointCount, texV, texU, texH, texW, tpage, clut, alpha, data, data + 1, tp);
    } else if (mode == 1) {
        btlfx_drawTrail3D((s32)out, anchor, pointCount, texV, texU, texH, texW, tpage, clut, alpha, data, data + 1, tp);
    } else {
        btlfx_drawTrailFlat((s32)out, pointCount, texV, texU, texH, texW, tpage, clut, alpha, data, data + 1, tp);
    }
}

void btlfx_drawParticles(u8 *data, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha, s32 offX, s32 offY) {
    VECTOR scaleVec;
    union {
        SVECTOR v;
        s32 w[2];
    } zero;
    MATRIX mat;
    Unk800D1680Ctx ctx;
    u8 *prim;
    u8 *colorData;
    u8 *scaleData;
    s32 off;
    u16 *frameOfs;
    Unk800D1BC4 *h = (Unk800D1BC4 *)data;
    s32 k;

    if (rot == NULL) {
        rot = &zero.v;
        zero.w[0] = 0;
        zero.w[1] = 0;
    }
    k = frame;
    off = h->frameCount & 0x7FFF;
    if (k >= off) {
        k = off - 1;
    }
    frameOfs = &h->frameOfs[k];
    ctx.tpage = h->tpage;
    off = *frameOfs;
    ctx.frame = *(u16 *)(data + off);
    off += 2;
    if (ctx.frame & BTLFX_SPRF_COLOR) {
        colorData = data + off;
        off += 4;
        ctx.color = *(u16 *)colorData | (colorData[2] << 16);
        if (alpha >= 0) {
            gte_DpqColorFar(&ctx.color, alpha);
        }
    } else {
        if (alpha < 0) {
            alpha = 0x1000;
        }
        alpha = (alpha >> 5) & 0xFF;
        ctx.color = alpha | (alpha << 8) | (alpha << 16);
    }
    scaleVec.vz = 0x1000;
    if (ctx.frame & BTLFX_SPRF_SCALE) {
        scaleData = data + off;
        scaleVec.vx = *(u16 *)scaleData;
        scaleVec.vy = *(u16 *)(scaleData + 2);
        if (scale >= 0) {
            gte_lddp(scale);
            gte_ldir(&scaleVec);
            gte_gpf12();
            gte_stir(&scaleVec);
        }
        off += 4;
    } else {
        if (scale < 0) {
            scale = 0x1000;
        }
        scaleVec.vx = scaleVec.vy = scale;
    }
    data += off;
    ctx.frame &= BTLFX_SPRF_COUNT;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + ctx.frame * 40;
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(rot, &mat);
    ScaleMatrix(&mat, &scaleVec);
    gte_stlvnl(mat.t);
    if (rot->pad != 0) {
        MulRotMatrix(&mat);
    }
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);
    ctx.color |= (abr == BTLFX_ABR_OPAQUE) ? GPU_CODE(GPU_POLY_FT4) : GPU_CODE(GPU_POLY_FT4 | GPU_ABE);
    ctx.abr = (abr & 3) << 5;
    ctx.x = offX;
    ctx.y = offY;
    ctx.otZ = g_btlfxEnv->otZ;
    btlfx_emitParticles(data, &ctx, prim, g_btlfxEnv->ot);
}

void btlfx_drawSprite(u8 *data, SVECTOR *pos, s32 frame, s32 abr, s32 alpha) {
    DVECTOR sxy;
    Unk800D1BC4Ctx ctx;
    s32 z;
    u8 *prim;
    u8 *colorData;
    s32 off;
    u16 *frameOfs;
    Unk800D1BC4 *h = (Unk800D1BC4 *)data;

    off = h->frameCount & 0x7FFF;
    if (frame >= off) {
        frame = off - 1;
    }
    frameOfs = &h->frameOfs[frame];
    ctx.tpage = h->tpage;
    off = *frameOfs;
    ctx.frame = *(u16 *)(data + off);
    off += 2;
    if (ctx.frame & BTLFX_SPRF_COLOR) {
        colorData = data + off;
        off += 4;
        ctx.color = *(u16 *)colorData | (colorData[2] << 16);
        if (alpha >= 0) {
            gte_DpqColorFar(&ctx.color, alpha);
        }
    } else {
        if (alpha < 0) {
            alpha = 0x1000;
        }
        alpha = (alpha >> 5) & 0xFF;
        ctx.color = alpha | (alpha << 8) | (alpha << 16);
    }
    if (ctx.frame & BTLFX_SPRF_SCALE) {
        off += 4;
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    data += off;
    ctx.frame &= BTLFX_SPRF_COUNT;
    gte_rtps_nn();
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + ctx.frame * 20;
    gte_stsxy(&sxy);
    gte_stszotz(&z);
    ctx.color |= (abr == BTLFX_ABR_OPAQUE) ? GPU_CODE(GPU_SPRT) : GPU_CODE(GPU_SPRT | GPU_ABE);
    ctx.abr = (abr & 3) << 5;
    ctx.x = sxy.vx;
    ctx.y = sxy.vy;
    ctx.prim = g_btlfxEnv->prim;
    z = (z - g_btlfxEnv->otZ) >> 2;
    if ((u32)z < 0x1000) {
        g_btlfxEnv->prim = btlfx_emitScreenSprites(data, &ctx, prim, &g_btlfxEnv->ot[z]);
    }
}

void btlfx_drawSpriteAtOt(u32 otIndex, u8 *data, s32 x, s32 y, s32 frame, s32 abr, s32 alpha) {
    Unk800D1BC4Ctx ctx;
    u8 *prim;
    u8 *colorData;
    s32 off;
    s32 frameCount;
    u16 *frameOfs;
    Unk800D1BC4 *h = (Unk800D1BC4 *)data;

    s32 k = frame;

    frameCount = h->frameCount & 0x7FFF;
    if (k >= frameCount) {
        k = frameCount - 1;
    }
    frameOfs = &h->frameOfs[k];
    ctx.tpage = h->tpage;
    off = *frameOfs;
    ctx.frame = *(u16 *)(data + off);
    off += 2;
    if (ctx.frame & BTLFX_SPRF_COLOR) {
        colorData = data + off;
        off += 4;
        ctx.color = *(u16 *)colorData | (colorData[2] << 16);
        if (alpha >= 0) {
            gte_DpqColorFar(&ctx.color, alpha);
        }
    } else {
        if (alpha < 0) {
            alpha = 0x1000;
        }
        alpha = (alpha >> 5) & 0xFF;
        ctx.color = alpha | (alpha << 8) | (alpha << 16);
    }
    if (ctx.frame & BTLFX_SPRF_SCALE) {
        off += 4;
    }
    data += off;
    ctx.frame &= BTLFX_SPRF_COUNT;
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + ctx.frame * 20;
    ctx.color |= (abr == BTLFX_ABR_OPAQUE) ? GPU_CODE(GPU_SPRT) : GPU_CODE(GPU_SPRT | GPU_ABE);
    ctx.abr = (abr & 3) << 5;
    ctx.x = x;
    ctx.y = y;
    ctx.prim = g_btlfxEnv->prim;
    if (otIndex < 0x1000) {
        g_btlfxEnv->prim = btlfx_emitScreenSprites(data, &ctx, prim, &g_btlfxEnv->ot[otIndex]);
    }
}

void btlfx_drawAnimQuads(u8 *data, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha) {
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)data;
    VECTOR scaleVec;
    SVECTOR zeroRot;
    MATRIX mat;
    Unk800D3ED4Ctx ctx;
    s32 val; /* frame count, then the palette table offset, then the frame data offset (separate locals don't match) */
    u16 *palTab;
    u8 *prim;
    u8 *pal;
    s32 i;
    s32 off;

    if (rot == NULL) {
        rot = &zeroRot;
        *(s32 *)&zeroRot.vx = 0;
        *(s32 *)&zeroRot.vz = 0;
    }
    val = h->frameCount & 0x7FFF;
    i = frame;
    if (i >= val) {
        i %= val;
    }
    val = val * 2 + 8;
    off = i * 2;
    pal = data + val;
    palTab = (u16 *)pal;
    i = *palTab++;
    ctx.uvTab = palTab;
    ctx.colTab = palTab + i + 1;
    ctx.tpage = h->tpage | ((abr & 3) << 5);
    ctx.clut = h->clut;
    ctx.halfH = (h->h - 1) * 2;
    ctx.halfW = (h->w - 1) * 2;
    val = *(u16 *)((u8 *)h->frameOfs + off);
    ctx.count = data[val++];
    data += val;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + ctx.count * 40;
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(rot, &mat);
    if (scale >= 0) {
        scaleVec.vz = scale;
        scaleVec.vy = scale;
        scaleVec.vx = scale;
        ScaleMatrix(&mat, &scaleVec);
    }
    gte_stlvnl(mat.t);
    if (rot->pad != 0) {
        MulRotMatrix(&mat);
    }
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);
    ctx.code = (abr == BTLFX_ABR_OPAQUE) ? GPU_POLY_FT4 : GPU_POLY_FT4 | GPU_ABE;
    ctx.fade = alpha;
    ctx.otZ = g_btlfxEnv->otZ;
    if (ctx.count != 0) {
        btlfx_emitAnimQuads(data, &ctx, prim, g_btlfxEnv->ot);
    }
}

void btlfx_drawSprites(u8 *data, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha) {
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)data;
    VECTOR scaleVec;
    SVECTOR zeroRot;
    MATRIX mat;
    Unk800D3ED4Ctx ctx;
    s32 z;
    s32 val; /* frame count, then the palette table offset, then the frame data offset (separate locals don't match) */
    u16 *palTab;
    s32 off;
    u8 *prim;
    u8 *pal;
    s32 i;
    s32 mode;
    s32 size;
    DR_TPAGE *tp;

    if (rot == NULL) {
        rot = &zeroRot;
        *(s32 *)&zeroRot.vx = 0;
        *(s32 *)&zeroRot.vz = 0;
    }
    val = h->frameCount & 0x7FFF;
    i = frame;
    if (i >= val) {
        i %= val;
    }
    val = val * 2 + 8;
    off = i * 2;
    pal = data + val;
    palTab = (u16 *)pal;
    i = *palTab++;
    ctx.uvTab = palTab;
    ctx.colTab = palTab + i + 1;
    mode = h->tpage;
    ctx.clut = h->clut;
    ctx.halfH = h->h - 1;
    ctx.halfW = h->w - 1;
    z = *(u16 *)&h->h;
    val = *(u16 *)((u8 *)h->frameOfs + off);
    ctx.count = data[val++];
    data += val;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    if (mode == 0) {
        size = 0xC;
        z = 0x68;
    } else if (z == 0x808) {
        size = 0x10;
        z = 0x74;
    } else if (z == 0x1010) {
        size = 0x10;
        z = 0x7C;
    } else {
        size = 0x14;
        z = 0x64;
    }
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + size * ctx.count;
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(rot, &mat);
    if (scale >= 0) {
        scaleVec.vz = scale;
        scaleVec.vy = scale;
        scaleVec.vx = scale;
        ScaleMatrix(&mat, &scaleVec);
    }
    gte_stlvnl(mat.t);
    if (abr != BTLFX_ABR_OPAQUE) {
        z |= 2;
    }
    gte_rtps_nn();
    ctx.code = z;
    tp = btlfx_allocDrMode(mode, abr);
    gte_stszotz(&z);
    if (rot->pad != 0) {
        MulRotMatrix(&mat);
    }
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);
    ctx.fade = alpha;
    z = (z - g_btlfxEnv->otZ) >> 2;
    if ((u32)z < 0x1000 && ctx.count != 0) {
        if (size == 0x14) {
            btlfx_emitSprites(data, &ctx, prim, &g_btlfxEnv->ot[z]);
        } else if (size == 0xC) {
            btlfx_emitDots(data, &ctx, prim, &g_btlfxEnv->ot[z]);
        } else {
            btlfx_emitFixedSprites(data, &ctx, prim, &g_btlfxEnv->ot[z]);
        }
        addPrimLenIn(&g_btlfxEnv->ot[z], tp, PRIM_TAG(DR_TPAGE));
    }
}

void btlfx_drawRings(u8 *data, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha) {
    VECTOR scaleVec;
    union {
        SVECTOR v;
        s32 w[2];
    } zero;
    MATRIX mat;
    Unk800D4768Ctx quadCtx;
    Unk800D4A30Ctx gouraudCtx;
    Unk800D23D8Hdr *h = (Unk800D23D8Hdr *)data;
    Unk800D23D8Scale *scales;
    Unk800D23D8Scale *sc;
    u16 *s1;
    s32 colorFrame;
    s32 type;
    s32 cnt;
    s32 w;
    s32 nf;
    s32 frameCount;
    u16 *frameData;
    u16 *uvAddTab;
    u8 *prim;
    s32 k;
    u32 *col;
    s32 m2;

    if (rot == NULL) {
        rot = &zero.v;
        zero.w[0] = 0;
        zero.w[1] = 0;
    }
    frameCount = h->frameCount;
    if (frame >= frameCount) {
        frame %= frameCount;
    }
    type = h->type;
    cnt = h->count;
    w = h->w;
    scales = (Unk800D23D8Scale *)(h + 1);
    scaleVec.vz = 0x1000;
    if (((Unk800D23D8Hdr *)scales)[-1].flags & BTLFX_RINGS_SCALED) {
        sc = &scales[frame];
        scaleVec.vx = sc->x;
        scaleVec.vy = sc->y;
        if (scale >= 0) {
            gte_lddp(scale);
            gte_ldir(&scaleVec);
            gte_gpf12();
            gte_stir(&scaleVec);
        }
        s1 = (u16 *)&scales[frameCount];
    } else {
        s1 = (u16 *)scales;
        if (scale < 0) {
            scale = 0x1000;
        }
        scaleVec.vx = scaleVec.vy = scale;
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    nf = s1[1];
    colorFrame = 0;
    if (frameCount >= 2) {
        colorFrame = frame * (nf - 1) / (frameCount - 1);
    }
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(rot, &mat);
    ScaleMatrix(&mat, &scaleVec);
    gte_stlvnl(mat.t);
    if (rot->pad != 0) {
        MulRotMatrix(&mat);
    }
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);
    gte_zero_vz012();
    switch (type) {
    case 1:
        quadCtx.frame = (u16 *)((u32 *)s1 + (nf + 1));
        col = (u32 *)s1 + (colorFrame + 1);
        if (alpha >= 0) {
            gte_DpqColorFar_nst(col, alpha);
        } else {
            gte_DpqColorFar_nst(col, 0x1000);
        }
        gte_strgb(&quadCtx.color);
        frameData = quadCtx.frame;
        k = frameData[0];
        quadCtx.tpage = (frameData[2] | ((abr & 3) << 5)) << 16;
        quadCtx.clut = frameData[3] << 16;
        quadCtx.frame = frameData = frameData + 4;
        uvAddTab = frameData + (cnt + 1) * (k + 2);
        quadCtx.scale = uvAddTab + frameCount;
        quadCtx.sizes = quadCtx.scale + 2;
        quadCtx.dirs = quadCtx.sizes + cnt + 1;
        quadCtx.uvAdd = uvAddTab[frame];
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + w * cnt * 40;
        ((u8 *)&quadCtx.color)[3] = (abr != BTLFX_ABR_OPAQUE) ? GPU_POLY_FT4 | GPU_ABE : GPU_POLY_FT4;
        quadCtx.count = cnt;
        quadCtx.last = w;
        quadCtx.otZ = g_btlfxEnv->otZ;
        btlfx_emitRingQuads(k, &quadCtx, prim, g_btlfxEnv->ot);
        break;
    case 2:
        m2 = s1[0] ? 2 : 1;
        gouraudCtx.colors2 = (u32 *)s1 + (colorFrame * m2 * (cnt + 1) + 1);
        if (s1[0]) {
            gouraudCtx.colors = gouraudCtx.colors2 + 1;
        } else {
            gouraudCtx.colors = gouraudCtx.colors2;
        }
        if (alpha < 0) {
            alpha = 0x1000;
        }
        gouraudCtx.scale = (u32 *)s1 + (nf * m2 * (cnt + 1) + 1);
        gouraudCtx.sizes = gouraudCtx.scale + 1;
        gouraudCtx.dirs = (u16 *)gouraudCtx.sizes + cnt + 1;
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + w * cnt * 44;
        gouraudCtx.code = (abr != BTLFX_ABR_OPAQUE) ? GPU_CODE(GPU_POLY_G4 | GPU_ABE) : GPU_CODE(GPU_POLY_G4);
        gouraudCtx.fade = alpha;
        gouraudCtx.count = cnt;
        gouraudCtx.abr = (abr & 3) << 5;
        gouraudCtx.last = w;
        gouraudCtx.otZ = g_btlfxEnv->otZ;
        btlfx_emitRingGouraud(m2, &gouraudCtx, prim, g_btlfxEnv->ot);
        break;
    }
}

void btlfx_drawFan(Unk800D2938 *fan, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha) {
    VECTOR scaleVec;
    union {
        SVECTOR v;
        s32 w[2];
    } zero;
    MATRIX mat;
    Unk800D57D0Ctx c1;
    Unk800D59D0Ctx c2;
    s32 k;
    s32 lastFrame;
    s32 pointIdx;
    s32 z;
    s32 z2;
    u16 *base;
    u16 *frameScale;
    u8 *prim;

    if (rot == NULL) {
        rot = &zero.v;
        zero.w[0] = 0;
        zero.w[1] = 0;
    }
    lastFrame = fan->frameCount - 1;
    if (lastFrame < frame) {
        frame = lastFrame;
    }
    k = fan->pointCount;
    if (lastFrame == 0) {
        pointIdx = 0;
    } else {
        pointIdx = frame * (k - 1) / lastFrame;
    }
    lastFrame++;
    z = fan->otZ * 4;
    if (fan->gouraud == 0) {
        k *= 4;
    } else {
        k *= 8;
    }
    base = (u16 *)((u8 *)fan + (k + 0x14));
    k = frame * 4;
    frameScale = (u16 *)((u8 *)base + k);
    scaleVec.vy = 0x1000;
    scaleVec.vz = frameScale[0];
    scaleVec.vx = frameScale[1];
    if (scale >= 0) {
        gte_lddp(scale);
        gte_ldir(&scaleVec);
        gte_gpf12();
        gte_stir(&scaleVec);
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    gte_rtv0tr();
    math_rotMatrixYXZ(rot, &mat);
    ScaleMatrix(&mat, &scaleVec);
    gte_stlvnl(mat.t);
    if (rot->pad != 0) {
        MulRotMatrix(&mat);
    }
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);
    fan++;
    if (fan[-1].gouraud == 0) {
        c1.uv = base + lastFrame * 2 + frame;
        c1.abr = (abr & 3) << 5;
        c1.count = pointIdx;
        c1.z = z;
        c1.code = (abr != BTLFX_ABR_OPAQUE) ? GPU_CODE(GPU_POLY_FT4 | GPU_ABE) : GPU_CODE(GPU_POLY_FT4);
        c1.fade = alpha;
        c1.otZ = g_btlfxEnv->otZ;
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + 0x28;
        btlfx_emitFrameQuad((u8 *)fan, &c1, prim, g_btlfxEnv->ot);
    } else {
        c2.tpage = (abr & 3) << 5;
        c2.count = pointIdx;
        c2.z = z;
        c2.code = (abr != BTLFX_ABR_OPAQUE) ? GPU_CODE(GPU_POLY_G3 | GPU_ABE) : GPU_CODE(GPU_POLY_G3);
        c2.fade = alpha;
        c2.otZ = g_btlfxEnv->otZ;
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + 0x5C;
        btlfx_emitFrameFan((u8 *)fan, &c2, prim, g_btlfxEnv->ot);
    }
}

/* old-style definition: callers pass the coordinates unpromoted (no prototype) */
void btlfx_addSpriteQuad(spr, x0, x2, y0, y1, u, v, w, h, color, padW, padH)
SprInfo *spr;
s16 x0;
s16 x2;
s16 y0;
s32 y1, u, v, w, h;
CVECTOR *color;
s32 padW, padH;
{
    POLY_FT4 *poly;
    s32 texEdge;
    u16 tp;

    poly = (POLY_FT4 *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(poly + 1);
    poly->code = (spr->abr != BTLFX_ABR_OPAQUE) ? GPU_POLY_FT4 | GPU_ABE : GPU_POLY_FT4;
    poly->clut = spr->clut;
    if (spr->abr != BTLFX_ABR_OPAQUE) {
        spr->tpage |= (spr->abr & 3) << 5;
    }
    tp = spr->tpage;
    poly->x0 = x0;
    texEdge = u + w + padW;
    poly->x1 = x0 + w + padW;
    poly->x3 = x2 + w + padW;
    poly->y2 = y0 + h + padH;
    poly->y3 = y1 + h + padH;
    poly->x2 = x2;
    poly->y0 = y0;
    poly->y1 = y1;
    poly->u2 = u;
    poly->u0 = u;
    poly->tpage = tp;
    if (texEdge >= 0x100) {
        texEdge--;
    }
    poly->u3 = texEdge;
    poly->u1 = texEdge;
    texEdge = v + h - padH;
    poly->v1 = v;
    poly->v0 = v;
    if (texEdge >= 0x100) {
        texEdge--;
    }
    poly->v3 = texEdge;
    poly->v2 = texEdge;
    *(u16 *)&poly->r0 = *(u16 *)&color->r;
    poly->b0 = color->b;
    btlfx_addPrim(PRIM_TAG(POLY_FT4), 0xFFE, poly);
}

void btlfx_drawBackdropSprite(u8 *data, s32 x, s32 y, s32 frame, s32 tp, s32 alpha) {
    struct {
        u16 rg;
        u8 b;
        u8 code;
        s32 pad4;
    } col;
    u32 *mode;
    u8 *ctab;
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)data;
    s32 val; /* frame count, then the palette table offset, then the frame data offset (separate locals don't match) */
    u16 *palTab;
    u16 *pal;
    s32 off;
    s32 wv;
    s32 hv;
    s32 num;
    u8 *prim;
    s32 i;
    u8 *colEntry;
    s32 attr;
    s32 tpage;
    s32 tmp; /* frame index, then the palette entry count, then the uv table offset (separate locals don't match) */
    s32 clut;

    val = h->frameCount & 0x7FFF;
    tmp = frame;
    if (tmp >= val) {
        tmp %= val;
    }
    val = val * 2 + 8;
    off = tmp * 2;
    palTab = (u16 *)(data + val);
    val = *(u16 *)((u8 *)h->frameOfs + off);
    pal = palTab + 1;
    tmp = *palTab;
    tpage = h->tpage;
    clut = h->clut << 16;
    hv = h->h - 1;
    wv = h->w - 1;
    num = data[val++];
    data += val;
    prim = g_btlfxEnv->prim;
    mode = (u32 *)(prim + num * 20);
    g_btlfxEnv->prim = (u8 *)(mode + 2);
    mode[1] = tpage | 0xE1000000 | ((tp & 3) << 5);
    ctab = (u8 *)(pal + tmp + 1);
    col.code = (tp != BTLFX_ABR_OPAQUE) ? GPU_SPRT | GPU_ABE : GPU_SPRT;
    for (i = 0; i < num; i++, prim += 20) {
        ((SPRT *)prim)->x0 = (s8)data[0] + x - (wv >> 1);
        ((SPRT *)prim)->y0 = (s8)data[1] + y - (hv >> 1);
        attr = data[2];
        tmp = (attr & 0xF) * 2;
        *(u32 *)&((SPRT *)prim)->u0 = *(u16 *)((u8 *)pal + tmp) | clut;
        colEntry = ctab + ((attr >> 2) & 0x3C);
        data += 3;
        col.rg = *(u16 *)colEntry;
        col.b = colEntry[2];
        gte_DpqColorFar_nst(&col, alpha);
        ((SPRT *)prim)->w = wv;
        ((SPRT *)prim)->h = hv;
        gte_strgb(&((SPRT *)prim)->r0);
        btlfx_addPrim(PRIM_TAG(SPRT), 0xFFE, prim);
    }
    btlfx_addPrim(PRIM_TAG(DR_TPAGE), 0xFFE, mode);
}

void btlfx_drawBackdrop(BtlBgLayers *bg, s32 frame) {
    RECT rect;
    s16 ofs[2];
    CVECTOR col;
    DR_TPAGE *tp;
    DR_AREA *da;
    DR_OFFSET *dofs;
    s32 *offs;
    s32 i;
    s32 layerCount;
    s32 w;
    s32 h;
    s32 step;
    s32 x, y;
    BtlBgLayer *layer;
    s32 shift, scroll;
    s32 phase;
    s32 k;
    s32 j;
    s32 tw;
    s32 cnt;
    s32 wt;
    TILE *tile;
    struct BtlBgScrollP *sc;
    struct BtlBgWaveP *wv;
    struct BtlBgWaveP *wv3;
    struct BtlBgFadeP *fd;

    da = (DR_AREA *)g_btlfxEnv->prim;
    layerCount = bg->count;
    w = bg->w;
    h = bg->h;
    x = bg->x;
    y = bg->y;
    g_btlfxEnv->prim = (u8 *)(da + 2);
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SetDrawArea(da, &rect);
    ((XY32 *)&rect)[0].w = ((XY32 *)&g_btlfxEnv->drawEnv->clip)[0].w;
    ((XY32 *)&rect)[1].w = ((XY32 *)&g_btlfxEnv->drawEnv->clip)[1].w;
    SetDrawArea(&da[1], &rect);
    i = 0;
    dofs = (DR_OFFSET *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(dofs + 2);
    ofs[0] = x;
    ofs[1] = y;
    SetDrawOffset(dofs, ofs);
    SetDrawOffset(&dofs[1], g_btlfxEnv->drawEnv->ofs);
    tp = (DR_TPAGE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(tp + 2);
    tp[0].code[0] = get_mode(0, 0, 0);
    tp[1].code[0] = get_mode(0, 1, 0);
    btlfx_addPrim(PRIM_TAG(DR_TPAGE), 0xFFE, &tp[1]);
    btlfx_addPrim(PRIM_TAG(DR_AREA), 0xFFE, &da[1]);
    btlfx_addPrim(PRIM_TAG(DR_OFFSET), 0xFFE, &dofs[1]);
    offs = bg->layers;
    for (; i < layerCount; i++) {
        layer = (BtlBgLayer *)((u8 *)bg + offs[i]);
        if (layer->gradOfs != 0) {
            btlfx_sampleGradient((u8 *)bg + layer->gradOfs, &col, frame % layer->gradFrames);
        } else {
            *(s32 *)&col = GPU_RGB_NEUTRAL;
        }
        switch (layer->kind) {
        case 0:
            sc = &layer->params.scroll;
            if (sc->sineX == 0) {
                wt = shift = frame * sc->speedX;
            } else {
                wt = shift = (math_sin(sc->speedX * frame) * sc->ampX) >> 12;
            }
            BG_WRAP(shift, w);
            if (sc->sineY == 0) {
                wt = scroll = frame * sc->speedY;
            } else {
                wt = scroll = (math_sin(sc->speedY * frame) * sc->ampY) >> 12;
            }
            BG_WRAP(scroll, h);
            btlfx_addSpriteQuad((SprInfo *)layer, shift, shift, scroll, scroll, layer->u, layer->v, w, h, &col, 1, 1);
            if (shift != 0) {
                btlfx_addSpriteQuad((SprInfo *)layer, shift - w, shift - w, scroll, scroll, layer->u, layer->v, w, h, &col, 0, 1);
            }
            if (scroll != 0) {
                btlfx_addSpriteQuad((SprInfo *)layer, shift, shift, scroll - h, scroll - h, layer->u, layer->v, w, h, &col, 1, 0);
            }
            if (shift != 0 && scroll != 0) {
                btlfx_addSpriteQuad((SprInfo *)layer, shift - w, shift - w, scroll - h, scroll - h, layer->u, layer->v, w, h, &col, 0, 0);
            }
            break;
        case 1:
            wv = &layer->params.wave;
            wt = scroll = frame * wv->scrollSpeed;
            BG_WRAP(scroll, h);
            cnt = 1;
            if (scroll != 0) {
                cnt = 2;
            }
            step = wv->phaseStep;
            tw = wv->stripSize;
            phase = frame * wv->phaseSpeed + scroll * step / tw;
            do {
                k = 0;
                for (j = 0; j < wv->stripCount; j++) {
                    shift = ((math_sin(phase) * wv->amplitude) >> 12) - (w >> 1);
                    phase += step;
                    btlfx_addSpriteQuad((SprInfo *)layer, shift, ((math_sin(phase) * wv->amplitude) >> 12) - (w >> 1), scroll, scroll, layer->u, layer->v + k, w * 2, tw, &col, 0, 0);
                    scroll += tw;
                    k += tw;
                }
                cnt--;
                scroll -= h * 2;
                phase -= step * wv->stripCount * 2;
            } while (cnt != 0);
            break;
        case 2:
            fd = &layer->params.fade;
            if (frame >= fd->delay) {
                s32 elapsed, fade;
                elapsed = frame - fd->delay;
                if (elapsed < fd->duration) {
                    fade = (elapsed << 12) / fd->duration;
                } else {
                    fade = 0x1000;
                }
                if (fd->dataOfs != 0) {
                    btlfx_drawBackdropSprite((u8 *)bg + fd->dataOfs + 4, fd->x, fd->y, elapsed, layer->abr, fade);
                }
            }
            break;
        case 3:
            wv3 = &layer->params.wave;
            wt = scroll = frame * wv3->scrollSpeed;
            BG_WRAP(scroll, w);
            cnt = 1;
            if (scroll != 0) {
                cnt = 2;
            }
            step = wv3->phaseStep;
            tw = wv3->stripSize;
            phase = frame * wv3->phaseSpeed + scroll * step / tw;
            do {
                k = 0;
                for (j = 0; j < wv3->stripCount; j++) {
                    shift = ((math_sin(phase) * wv3->amplitude) >> 12) - (h >> 1);
                    phase += step;
                    btlfx_addSpriteQuad((SprInfo *)layer, scroll, scroll, shift, ((math_sin(phase) * wv3->amplitude) >> 12) - (h >> 1), layer->u + k, layer->v, tw, h * 2, &col, 0, 0);
                    scroll += tw;
                    k += tw;
                }
                cnt--;
                scroll -= w * 2;
                phase -= step * wv3->stripCount * 2;
            } while (cnt != 0);
            break;
        }
    }
    tile = (TILE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(tile + 1);
    *(s32 *)&tile->r0 = 0;
    tile->code = GPU_TILE;
    tile->y0 = 0;
    tile->x0 = 0;
    tile->w = w;
    tile->h = h;
    btlfx_addPrim(PRIM_TAG(TILE), 0xFFE, tile);
    btlfx_addPrim(PRIM_TAG(DR_TPAGE), 0xFFE, tp);
    btlfx_addPrim(PRIM_TAG(DR_AREA), 0xFFE, da);
    btlfx_addPrim(PRIM_TAG(DR_OFFSET), 0xFFE, dofs);
}

s32 btlfx_ease(s32 elapsed, s32 duration, s32 mode) {
    s32 ratio = 0;
    switch (mode) {
    case 0:
        ratio = (elapsed << 12) / duration;
        break;
    case 1:
        ratio = 0x1000 - math_cos((elapsed << 11) / duration);
        break;
    case 2:
        ratio = math_sin((elapsed << 11) / duration);
        break;
    }
    return ratio;
}

void btlfx_drawSpriteLine(u8 *data, SVECTOR *pos, s32 frame, s32 alpha) {
    s16 endPt[2];
    s16 spritePt[2];
    s16 sxy[2];
    s32 progress;
    s32 i;
    s32 elapsed;
    s32 entryAlpha;
    s32 angle;
    s32 dist;
    Unk800D3B6CEntry *entry;
    Unk800D3B6C *h;

    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    h = (Unk800D3B6C *)data;
    entry = h->entries;
    gte_rtps_b();
    progress = (frame << 12) / h->duration;
    gte_stsxy(sxy);
    if (h->polar == 0) {
        btlfx_lerpVec(h->from, h->to, progress, endPt);
        for (i = 0; i < h->count; i++, entry++) {
            btlfx_lerpVec(sxy, endPt, entry->along * 2, spritePt);
            if (frame >= entry->start) {
                elapsed = frame - entry->start;
                if (elapsed < entry->fadeIn) {
                    entryAlpha = elapsed * alpha / entry->fadeIn;
                } else {
                    entryAlpha = alpha;
                }
                btlfx_drawSpriteAtOt(8, (u8 *)h + entry->spriteOfs, spritePt[0], spritePt[1], frame, 1, entryAlpha);
            }
        }
    } else {
        angle = h->angle0 + (((h->angle1 - h->angle0) * btlfx_ease(frame, h->duration, h->angleEase)) >> 12);
        dist = h->dist0 + (((h->dist1 - h->dist0) * btlfx_ease(frame, h->duration, h->distEase)) >> 12);
        endPt[0] = sxy[0] + ((math_cos(angle) * dist) >> 12);
        endPt[1] = sxy[1] + ((math_sin(angle) * dist) >> 12);
        for (i = 0; i < h->count; i++, entry++) {
            btlfx_lerpVec(sxy, endPt, entry->along * 2, spritePt);
            if (frame >= entry->start) {
                elapsed = frame - entry->start;
                if (elapsed < entry->fadeIn) {
                    entryAlpha = elapsed * alpha / entry->fadeIn;
                } else {
                    entryAlpha = alpha;
                }
                btlfx_drawSpriteAtOt(8, (u8 *)h + entry->spriteOfs, spritePt[0], spritePt[1], frame, 1, entryAlpha);
            }
        }
    }
}
