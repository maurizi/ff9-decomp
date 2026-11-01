#include "ovl_0b.h"

s32 btlfx_align4(s32 value) {
    s32 v = value & ~3;
    if (value & 3) {
        v += 4;
    }
    return v;
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
    RotMatrix33 m;

    math_rotMatrixYXZ(rot, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    btlfx_transformPoint(dist, 0, out);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_pointAhead(s16 *origin, s16 *rot, s32 dist, s16 *out) {
    MATRIX m;

    math_rotMatrixYXZ(rot, &m);
    gte_SetRotMatrix(&m);
    m.t[0] = origin[0];
    m.t[1] = origin[1];
    m.t[2] = origin[2];
    gte_SetTransMatrix(&m);
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
    s32 s;
    s32 d;
    s32 ad;
    s32 neg;
    target &= 0xFFF;
    angle &= 0xFFF;
    s = -1;
    if (angle < target) {
        s = 1;
    }
    d = target - angle;
    neg = d < 0;
    ad = d;
    if (neg) {
        ad = -ad;
    }
    if (ad > 0x800) {
        s = -s;
        ad = 0x1000 - ad;
    }
    if (outDiff != NULL) {
        *outDiff = ad;
    }
    return s;
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
    MATRIX m;

    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    gte_rtv0tr();
    if (mat == NULL) {
        mat = &m;
    }
    gte_stlvnl(mat->t);
    gte_SetTransMatrix(mat);
}

void btlfx_setRotScale(SVECTOR *rot, VECTOR *scale, MATRIX *mat, s32 useLight) {
    MATRIX m;
    RotMatrix33 l;

    if (mat == NULL) {
        mat = &m;
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
        gte_MulMatrix0(&l, mat, &l);
        gte_SetLightMatrix(&l);
    }
    gte_SetRotMatrix(mat);
}

void btlfx_offsetPoint(SVECTOR *origin, s32 out, s32 axial, XY32 *axis, s32 radius, s32 spin) {
    struct {
        XY32 xy;
        s16 z;
    } r;
    MATRIX m;

    r.xy = *axis;
    r.z = spin;
    math_rotMatrixYXZ(&r, &m);
    gte_SetRotMatrix(&m);
    m.t[0] = origin->vx;
    m.t[1] = origin->vy;
    m.t[2] = origin->vz;
    gte_SetTransMatrix(&m);
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
    } r;
    MATRIX m;

    r.xy = *axis;
    r.z = spin;
    math_rotMatrixYXZ(&r, &m);
    gte_SetRotMatrix(&m);
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
    } r;
    RotMatrix33 m;

    rel.vx = pos->vx - center->vx;
    rel.vy = pos->vy - center->vy;
    rel.vz = pos->vz - center->vz;
    r.xy.w = (-axis->vx & 0xFFFF) | (-axis->vy << 16);
    r.z = 0;
    math_rotMatrixXYZ(&r.xy, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    *outAngle = btlfx_transformPolar(&rel, outAxial, outRadius);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

/* adds prim to ot with the given code/len word; tpage != 0xFF also adds a DR_TPAGE (semi-trans mode) */
void btlfx_linkPrimAbr(u32 code, u32 *ot, void *prim, s32 tpage) {
    DR_TPAGE *p;

    addPrimLenIn(ot, prim, code);
    if (tpage != 0xFF) {
        p = (DR_TPAGE *)g_btlfxEnv->prim;
        g_btlfxEnv->prim = (u8 *)(p + 1);
        p->code[0] = ((tpage & 3) << 5) | 0xE1000200;
        addPrimLenIn(ot, p, 0x1000000);
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
    DR_TPAGE *p = (DR_TPAGE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(p + 1);
    p->code[0] = tpage | ((abr & 3) << 5) | 0xE1000200;
    return p;
}

void btlfx_resetMaskBit(void) {
    s32 n;
    Unk800D0940 *p;
    u32 *ot;
    if (g_btlfxEnv->prim != NULL) {
        g_btlfxMaskResetBusy = n = 1;
        p = (Unk800D0940 *)g_btlfxEnv->prim;
        ot = &p->ot;
        g_btlfxEnv->prim = (u8 *)(p + 1);
        setlen(p, 2);
        p->stp = GPU_STP(0, 0);
        p->nop = 0;
        ClearOTag(ot, n);
        addPrim(ot, p);
        EnterCriticalSection();
        DrawOTag(ot);
        ExitCriticalSection();
        g_btlfxMaskResetBusy = 0;
    }
}

void btlfx_drawScreenTint(u32 *ot, s32 abr, s32 red, s32 green, s32 blue) {
    TILE *t;
    s32 i;
    t = (TILE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(t + 8);
    for (i = 0; i < 8; i++, t++) {
        t->code = abr == 0xFF ? 0x60 : 0x62;
        t->w = 0x50;
        t->h = 0x6E;
        t->x0 = (i & 3) * 0x50;
        t->r0 = red;
        t->g0 = green;
        t->b0 = blue;
        t->y0 = (i >> 2) * 0x6E;
        btlfx_linkPrimAbr(0x03000000, ot, t, abr);
    }
}

/* Draws a grid of gouraud quads whose rows ripple with a sine wave. */
void btlfx_drawRippleGrid(s32 ot, s32 top, s32 cols, s32 rows, s32 rowH, s32 abr, CVECTOR *colors, s32 amp, s32 phase,
                   s32 colStep, s32 rowStep) {
    POLY_G4 *p;
    CVECTOR *rowColor;
    s32 row;
    s32 col;
    s32 y;
    s32 curPhase;

    rowColor = colors;
    row = 0;
    p = (POLY_G4 *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(p + cols * rows);
    y = top;
    if (rows > 0) {
        do {
            curPhase = phase + row * rowStep;
            rowColor[1].cd = (abr == 0xFF) ? 0x38 : 0x3A;
            rowColor[0].cd = rowColor[1].cd;
            for (col = 0; col < cols; col++, p++, curPhase += colStep) {
                p->x0 = p->x2 = (col * 0x140) / cols;
                p->x1 = p->x3 = ((col + 1) * 0x140) / cols;
                if (row == 0) {
                    p->y0 = p->y1 = y;
                } else {
                    p->y0 = y + ((math_sin(curPhase) * amp) >> 12);
                    p->y1 = y + ((math_sin(curPhase + colStep) * amp) >> 12);
                }
                curPhase += rowStep;
                p->y2 = (y + rowH) + ((math_sin(curPhase) * amp) >> 12);
                p->y3 = (y + rowH) + ((math_sin(curPhase + colStep) * amp) >> 12);
                curPhase -= rowStep;
                *(s32 *)&p->r0 = *(s32 *)&p->r1 = *(s32 *)rowColor;
                *(s32 *)&p->r2 = *(s32 *)&p->r3 = *(s32 *)&rowColor[1];
                btlfx_linkPrimAbr(0x08000000, ot, p, abr);
            }
            row++;
            y += rowH;
            rowColor++;
        } while (row < rows);
    }
}

void btlfx_drawTrail3D(s32 ring, s32 *anchor, s32 n, s32 texV, s32 texU, s32 texH, s32 texW, s32 tpage, s32 clut, s32 alpha,
                   u32 *c1, u32 *c2, s32 tp) {
    Unk800D50A0Ctx c;
    u8 *prim;
    s32 inv;

    if (alpha < 0) {
        alpha = 0x1000;
    }
    gte_DpqColorFar_nst(c1, alpha);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + n * 0x34;
    gte_strgb(&c.color);
    gte_DpqColorFar_nst(c2, alpha);
    c.u1 = (texU + texW) << 8;
    c.u0 = texU << 8;
    gte_strgb(&c.farColor);
    c.tpage = (((tp & 3) << 5) | tpage) << 16;
    c.clut = clut << 16;
    c.color.b[3] = (tp == 0xFF) ? 0x3C : 0x3E;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    if (n != 0) {
        inv = 0x1000 / n;
        c.fadeStep = inv;
        c.fade = inv;
        c.anchorXY = anchor[0];
        inv = anchor[1];
        c.ring = ring;
        c.v0 = texV;
        c.v1 = texH;
        c.otZ = g_btlfxEnv->otZ;
        c.anchorZ = inv;
        btlfx_emitTrailRibbon3D(n, &c, prim, g_btlfxEnv->ot);
    }
}

void btlfx_drawTrailFlat(s32 ring, s32 n, s32 texV, s32 texU, s32 texH, s32 texW, s32 tpage, s32 clut, s32 alpha, u32 *c1,
                   u32 *c2, s32 tp) {
    Unk800D4D6CCtx c;
    u8 *prim;
    s32 inv;
    s32 sinYaw;

    if (alpha < 0) {
        alpha = 0x1000;
    }
    gte_DpqColorFar_nst(c1, alpha);
    gte_strgb(&c.color);
    gte_DpqColorFar_nst(c2, alpha);
    if (tpage >= 0) {
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + n * 0x34;
        c.u1 = (texU + texW) << 8;
        c.u0 = texU << 8;
        sinYaw = math_sin(-g_btlfxEnv->camera->yaw + 0x400);
        c.mode.sinCos = (sinYaw & 0xFFFF) | (math_cos(-g_btlfxEnv->camera->yaw + 0x400) << 16);
        c.color.b[3] = (tp != 0xFF) ? 0x3E : 0x3C;
        c.tpage = (((tp & 3) << 5) | tpage) << 16;
        c.clut = clut << 16;
        c.v0 = texV;
        c.v1 = texH;
    } else {
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + n * 0x1C;
        c.mode.abr = (tp & 3) << 5;
        c.color.b[3] = (tp == 0xFF) ? 0x50 : 0x52;
    }
    gte_strgb(&c.farColor);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    if (n != 0) {
        inv = 0x1000 / n;
        c.ring = ring;
        c.fadeStep = inv;
        c.fade = inv;
        c.otZ = g_btlfxEnv->otZ;
        if (tpage >= 0) {
            btlfx_emitTrailRibbonFlat(n, &c, prim, g_btlfxEnv->ot);
        } else {
            btlfx_emitTrailLines(n, &c, prim, g_btlfxEnv->ot);
        }
    }
}

void btlfx_drawTrailScreen(s32 ring, s32 n, s32 texV, s32 texU, s32 texH, s32 texW, s32 tpage, s32 clut, s32 alpha, u32 *c1,
                   u32 *c2, s32 tp) {
    Unk800D5C30Ctx c;
    u8 *prim;
    s32 inv;

    if (alpha < 0) {
        alpha = 0x1000;
    }
    gte_DpqColorFar_nst(c1, alpha);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + n * 0x34;
    gte_strgb(&c.color);
    gte_DpqColorFar_nst(c2, alpha);
    c.u1 = (texU + texW) << 8;
    c.u0 = texU << 8;
    c.tpage = (((tp & 3) << 5) | tpage) << 16;
    c.clut = clut << 16;
    c.color.b[3] = (tp == 0xFF) ? 0x3C : 0x3E;
    gte_strgb(&c.farColor);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    if (n != 0) {
        inv = 0x1000 / n;
        c.ring = ring;
        c.v0 = texV;
        c.v1 = texH;
        c.fadeStep = inv;
        c.fade = inv;
        c.otZ = g_btlfxEnv->otZ;
        btlfx_emitTrailRibbonScreen(n, &c, prim, g_btlfxEnv->ot);
    }
}

void btlfx_drawTrail(u8 *p, s32 newPoint, s32 *anchor, s32 frame, s32 scale, s32 alpha, s32 tp, u8 *out, s32 mode) {
    s32 n;
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
    n = ((Unk800D1494 *)p)->length;
    k = ((Unk800D1494 *)p)->frameOfs;
    cnt = ((Unk800D1494 *)p)->frameCount;
    tpage = ((Unk800D1494 *)p)->tpage;
    clut = ((Unk800D1494 *)p)->clut;
    texH = ((Unk800D1494 *)p)->h - 1;
    texW = ((Unk800D1494 *)p)->w - 1;
    p += sizeof(Unk800D1494);
    if (frame == -1) {
        out[1] = n;
        out[0] = n;
        out[2] = 0;
        return;
    }
    data = (u32 *)p;
    if (frame >= cnt) {
        frame = cnt - 1;
    }
    p = (u8 *)data + (k * 8 + frame * 4);
    width = p[2];
    width = (width * scale) >> 12;
    texV = p[0];
    texU = p[1];
    k = p[3];
    data += k * 2;
    if (newPoint != 0) {
        n = btlfx_pushTrailPoint(out, n, width, newPoint);
    }
    if (mode == 0) {
        btlfx_drawTrailScreen((s32)out, n, texV, texU, texH, texW, tpage, clut, alpha, data, data + 1, tp);
    } else if (mode == 1) {
        btlfx_drawTrail3D((s32)out, anchor, n, texV, texU, texH, texW, tpage, clut, alpha, data, data + 1, tp);
    } else {
        btlfx_drawTrailFlat((s32)out, n, texV, texU, texH, texW, tpage, clut, alpha, data, data + 1, tp);
    }
}

void btlfx_drawParticles(u8 *data, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha, s32 offX, s32 offY) {
    VECTOR s;
    union {
        SVECTOR v;
        s32 w[2];
    } zero;
    MATRIX m;
    Unk800D1680Ctx c;
    u8 *prim;
    u8 *d;
    u8 *e;
    s32 off;
    u16 *o;
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
    o = &h->frameOfs[k];
    c.tpage = h->tpage;
    off = *o;
    c.frame = *(u16 *)(data + off);
    off += 2;
    if (c.frame & BTLFX_SPRF_COLOR) {
        d = data + off;
        off += 4;
        c.color = *(u16 *)d | (d[2] << 16);
        if (alpha >= 0) {
            gte_DpqColorFar(&c.color, alpha);
        }
    } else {
        if (alpha < 0) {
            alpha = 0x1000;
        }
        alpha = (alpha >> 5) & 0xFF;
        c.color = alpha | (alpha << 8) | (alpha << 16);
    }
    s.vz = 0x1000;
    if (c.frame & BTLFX_SPRF_SCALE) {
        e = data + off;
        s.vx = *(u16 *)e;
        s.vy = *(u16 *)(e + 2);
        if (scale >= 0) {
            gte_lddp(scale);
            gte_ldir(&s);
            gte_gpf12();
            gte_stir(&s);
        }
        off += 4;
    } else {
        if (scale < 0) {
            scale = 0x1000;
        }
        s.vx = s.vy = scale;
    }
    data += off;
    c.frame &= BTLFX_SPRF_COUNT;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + c.frame * 40;
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(rot, &m);
    ScaleMatrix(&m, &s);
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.color |= (abr == 0xFF) ? GPU_CODE(GPU_POLY_FT4) : GPU_CODE(GPU_POLY_FT4 | GPU_ABE);
    c.abr = (abr & 3) << 5;
    c.x = offX;
    c.y = offY;
    c.otZ = g_btlfxEnv->otZ;
    btlfx_emitParticles(data, &c, prim, g_btlfxEnv->ot);
}

void btlfx_drawSprite(u8 *data, SVECTOR *pos, s32 frame, s32 abr, s32 alpha) {
    DVECTOR sxy;
    Unk800D1BC4Ctx c;
    s32 z;
    u8 *prim;
    u8 *d;
    s32 off;
    u16 *o;
    Unk800D1BC4 *h = (Unk800D1BC4 *)data;

    off = h->frameCount & 0x7FFF;
    if (frame >= off) {
        frame = off - 1;
    }
    o = &h->frameOfs[frame];
    c.tpage = h->tpage;
    off = *o;
    c.frame = *(u16 *)(data + off);
    off += 2;
    if (c.frame & BTLFX_SPRF_COLOR) {
        d = data + off;
        off += 4;
        c.color = *(u16 *)d | (d[2] << 16);
        if (alpha >= 0) {
            gte_DpqColorFar(&c.color, alpha);
        }
    } else {
        if (alpha < 0) {
            alpha = 0x1000;
        }
        alpha = (alpha >> 5) & 0xFF;
        c.color = alpha | (alpha << 8) | (alpha << 16);
    }
    if (c.frame & BTLFX_SPRF_SCALE) {
        off += 4;
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    data += off;
    c.frame &= BTLFX_SPRF_COUNT;
    gte_rtps_nn();
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + c.frame * 20;
    gte_stsxy(&sxy);
    gte_stszotz(&z);
    c.color |= (abr == 0xFF) ? GPU_CODE(GPU_SPRT) : GPU_CODE(GPU_SPRT | GPU_ABE);
    c.abr = (abr & 3) << 5;
    c.x = sxy.vx;
    c.y = sxy.vy;
    c.prim = g_btlfxEnv->prim;
    z = (z - g_btlfxEnv->otZ) >> 2;
    if ((u32)z < 0x1000) {
        g_btlfxEnv->prim = btlfx_emitScreenSprites(data, &c, prim, &g_btlfxEnv->ot[z]);
    }
}

void btlfx_drawSpriteAtOt(u32 otIndex, u8 *data, s32 x, s32 y, s32 frame, s32 abr, s32 alpha) {
    Unk800D1BC4Ctx c;
    u8 *prim;
    u8 *d;
    s32 off;
    s32 n;
    u16 *o;
    Unk800D1BC4 *h = (Unk800D1BC4 *)data;

    s32 k = frame;

    n = h->frameCount & 0x7FFF;
    if (k >= n) {
        k = n - 1;
    }
    o = &h->frameOfs[k];
    c.tpage = h->tpage;
    off = *o;
    c.frame = *(u16 *)(data + off);
    off += 2;
    if (c.frame & BTLFX_SPRF_COLOR) {
        d = data + off;
        off += 4;
        c.color = *(u16 *)d | (d[2] << 16);
        if (alpha >= 0) {
            gte_DpqColorFar(&c.color, alpha);
        }
    } else {
        if (alpha < 0) {
            alpha = 0x1000;
        }
        alpha = (alpha >> 5) & 0xFF;
        c.color = alpha | (alpha << 8) | (alpha << 16);
    }
    if (c.frame & BTLFX_SPRF_SCALE) {
        off += 4;
    }
    data += off;
    c.frame &= BTLFX_SPRF_COUNT;
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + c.frame * 20;
    c.color |= (abr == 0xFF) ? GPU_CODE(GPU_SPRT) : GPU_CODE(GPU_SPRT | GPU_ABE);
    c.abr = (abr & 3) << 5;
    c.x = x;
    c.y = y;
    c.prim = g_btlfxEnv->prim;
    if (otIndex < 0x1000) {
        g_btlfxEnv->prim = btlfx_emitScreenSprites(data, &c, prim, &g_btlfxEnv->ot[otIndex]);
    }
}

void btlfx_drawAnimQuads(u8 *data, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha) {
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)data;
    VECTOR s;
    SVECTOR r;
    MATRIX m;
    Unk800D3ED4Ctx c;
    s32 n;
    u16 *p;
    u8 *prim;
    u8 *pal;
    s32 i;
    s32 off;

    if (rot == NULL) {
        rot = &r;
        *(s32 *)&r.vx = 0;
        *(s32 *)&r.vz = 0;
    }
    n = h->frameCount & 0x7FFF;
    i = frame;
    if (i >= n) {
        i %= n;
    }
    n = n * 2 + 8;
    off = i * 2;
    pal = data + n;
    p = (u16 *)pal;
    i = *p++;
    c.uvTab = p;
    c.colTab = p + i + 1;
    c.tpage = h->tpage | ((abr & 3) << 5);
    c.clut = h->clut;
    c.halfH = (h->h - 1) * 2;
    c.halfW = (h->w - 1) * 2;
    n = *(u16 *)((u8 *)h->frameOfs + off);
    c.count = data[n++];
    data += n;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + c.count * 40;
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(rot, &m);
    if (scale >= 0) {
        s.vz = scale;
        s.vy = scale;
        s.vx = scale;
        ScaleMatrix(&m, &s);
    }
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.code = (abr == 0xFF) ? 0x2C : 0x2E;
    c.fade = alpha;
    c.otZ = g_btlfxEnv->otZ;
    if (c.count != 0) {
        btlfx_emitAnimQuads(data, &c, prim, g_btlfxEnv->ot);
    }
}

void btlfx_drawSprites(u8 *data, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha) {
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)data;
    VECTOR s;
    SVECTOR r;
    MATRIX m;
    Unk800D3ED4Ctx c;
    s32 z;
    s32 n;
    u16 *p;
    s32 off;
    u8 *prim;
    u8 *pal;
    s32 i;
    s32 mode;
    s32 size;
    DR_TPAGE *tp;

    if (rot == NULL) {
        rot = &r;
        *(s32 *)&r.vx = 0;
        *(s32 *)&r.vz = 0;
    }
    n = h->frameCount & 0x7FFF;
    i = frame;
    if (i >= n) {
        i %= n;
    }
    n = n * 2 + 8;
    off = i * 2;
    pal = data + n;
    p = (u16 *)pal;
    i = *p++;
    c.uvTab = p;
    c.colTab = p + i + 1;
    mode = h->tpage;
    c.clut = h->clut;
    c.halfH = h->h - 1;
    c.halfW = h->w - 1;
    z = *(u16 *)&h->h;
    n = *(u16 *)((u8 *)h->frameOfs + off);
    c.count = data[n++];
    data += n;
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
    g_btlfxEnv->prim = prim + size * c.count;
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(rot, &m);
    if (scale >= 0) {
        s.vz = scale;
        s.vy = scale;
        s.vx = scale;
        ScaleMatrix(&m, &s);
    }
    gte_stlvnl(m.t);
    if (abr != 0xFF) {
        z |= 2;
    }
    gte_rtps_nn();
    c.code = z;
    tp = btlfx_allocDrMode(mode, abr);
    gte_stszotz(&z);
    if (rot->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.fade = alpha;
    z = (z - g_btlfxEnv->otZ) >> 2;
    if ((u32)z < 0x1000 && c.count != 0) {
        if (size == 0x14) {
            btlfx_emitSprites(data, &c, prim, &g_btlfxEnv->ot[z]);
        } else if (size == 0xC) {
            btlfx_emitDots(data, &c, prim, &g_btlfxEnv->ot[z]);
        } else {
            btlfx_emitFixedSprites(data, &c, prim, &g_btlfxEnv->ot[z]);
        }
        addPrimLenIn(&g_btlfxEnv->ot[z], tp, 0x1000000);
    }
}

void btlfx_drawRings(u8 *data, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha) {
    VECTOR s;
    union {
        SVECTOR v;
        s32 w[2];
    } zero;
    MATRIX m;
    Unk800D4768Ctx c;
    Unk800D4A30Ctx d;
    Unk800D23D8Hdr *h = (Unk800D23D8Hdr *)data;
    Unk800D23D8Scale *e;
    Unk800D23D8Scale *sc;
    u16 *s1;
    s32 f;
    s32 type;
    s32 cnt;
    s32 w;
    s32 nf;
    s32 n;
    u16 *q;
    u16 *t;
    u8 *prim;
    s32 k;
    u32 *col;
    s32 m2;

    if (rot == NULL) {
        rot = &zero.v;
        zero.w[0] = 0;
        zero.w[1] = 0;
    }
    n = h->frameCount;
    if (frame >= n) {
        frame %= n;
    }
    type = h->type;
    cnt = h->count;
    w = h->w;
    e = (Unk800D23D8Scale *)(h + 1);
    s.vz = 0x1000;
    if (((Unk800D23D8Hdr *)e)[-1].flags & BTLFX_RINGS_SCALED) {
        sc = &e[frame];
        s.vx = sc->x;
        s.vy = sc->y;
        if (scale >= 0) {
            gte_lddp(scale);
            gte_ldir(&s);
            gte_gpf12();
            gte_stir(&s);
        }
        s1 = (u16 *)&e[n];
    } else {
        s1 = (u16 *)e;
        if (scale < 0) {
            scale = 0x1000;
        }
        s.vx = s.vy = scale;
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    nf = s1[1];
    f = 0;
    if (n >= 2) {
        f = frame * (nf - 1) / (n - 1);
    }
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(rot, &m);
    ScaleMatrix(&m, &s);
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    gte_zero_vz012();
    switch (type) {
    case 1:
        c.frame = (u16 *)((u32 *)s1 + (nf + 1));
        col = (u32 *)s1 + (f + 1);
        if (alpha >= 0) {
            gte_DpqColorFar_nst(col, alpha);
        } else {
            gte_DpqColorFar_nst(col, 0x1000);
        }
        gte_strgb(&c.color);
        q = c.frame;
        k = q[0];
        c.tpage = (q[2] | ((abr & 3) << 5)) << 16;
        c.clut = q[3] << 16;
        c.frame = q = q + 4;
        t = q + (cnt + 1) * (k + 2);
        c.scale = t + n;
        c.sizes = c.scale + 2;
        c.dirs = c.sizes + cnt + 1;
        c.uvAdd = t[frame];
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + w * cnt * 40;
        ((u8 *)&c.color)[3] = (abr != 0xFF) ? 0x2E : 0x2C;
        c.count = cnt;
        c.last = w;
        c.otZ = g_btlfxEnv->otZ;
        btlfx_emitRingQuads(k, &c, prim, g_btlfxEnv->ot);
        break;
    case 2:
        m2 = s1[0] ? 2 : 1;
        d.colors2 = (u32 *)s1 + (f * m2 * (cnt + 1) + 1);
        if (s1[0]) {
            d.colors = d.colors2 + 1;
        } else {
            d.colors = d.colors2;
        }
        if (alpha < 0) {
            alpha = 0x1000;
        }
        d.scale = (u32 *)s1 + (nf * m2 * (cnt + 1) + 1);
        d.sizes = d.scale + 1;
        d.dirs = (u16 *)d.sizes + cnt + 1;
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + w * cnt * 44;
        d.code = (abr != 0xFF) ? GPU_CODE(GPU_POLY_G4 | GPU_ABE) : GPU_CODE(GPU_POLY_G4);
        d.fade = alpha;
        d.count = cnt;
        d.abr = (abr & 3) << 5;
        d.last = w;
        d.otZ = g_btlfxEnv->otZ;
        btlfx_emitRingGouraud(m2, &d, prim, g_btlfxEnv->ot);
        break;
    }
}

void btlfx_drawFan(Unk800D2938 *fan, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha) {
    VECTOR s;
    union {
        SVECTOR v;
        s32 w[2];
    } zero;
    MATRIX m;
    Unk800D57D0Ctx c1;
    Unk800D59D0Ctx c2;
    s32 k;
    s32 n;
    s32 t;
    s32 z;
    s32 z2;
    u16 *base;
    u16 *e;
    u8 *prim;

    if (rot == NULL) {
        rot = &zero.v;
        zero.w[0] = 0;
        zero.w[1] = 0;
    }
    n = fan->frameCount - 1;
    if (n < frame) {
        frame = n;
    }
    k = fan->pointCount;
    if (n == 0) {
        t = 0;
    } else {
        t = frame * (k - 1) / n;
    }
    n++;
    z = fan->otZ * 4;
    if (fan->gouraud == 0) {
        k *= 4;
    } else {
        k *= 8;
    }
    base = (u16 *)((u8 *)fan + (k + 0x14));
    k = frame * 4;
    e = (u16 *)((u8 *)base + k);
    s.vy = 0x1000;
    s.vz = e[0];
    s.vx = e[1];
    if (scale >= 0) {
        gte_lddp(scale);
        gte_ldir(&s);
        gte_gpf12();
        gte_stir(&s);
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    gte_rtv0tr();
    math_rotMatrixYXZ(rot, &m);
    ScaleMatrix(&m, &s);
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    fan++;
    if (fan[-1].gouraud == 0) {
        c1.uv = base + n * 2 + frame;
        c1.abr = (abr & 3) << 5;
        c1.count = t;
        c1.z = z;
        c1.code = (abr != 0xFF) ? GPU_CODE(GPU_POLY_FT4 | GPU_ABE) : GPU_CODE(GPU_POLY_FT4);
        c1.fade = alpha;
        c1.otZ = g_btlfxEnv->otZ;
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + 0x28;
        btlfx_emitFrameQuad((u8 *)fan, &c1, prim, g_btlfxEnv->ot);
    } else {
        c2.tpage = (abr & 3) << 5;
        c2.count = t;
        c2.z = z;
        c2.code = (abr != 0xFF) ? GPU_CODE(GPU_POLY_G3 | GPU_ABE) : GPU_CODE(GPU_POLY_G3);
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
    POLY_FT4 *p;
    s32 c;
    u16 tp;

    p = (POLY_FT4 *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(p + 1);
    p->code = (spr->abr != 0xFF) ? 0x2E : 0x2C;
    p->clut = spr->clut;
    if (spr->abr != 0xFF) {
        spr->tpage |= (spr->abr & 3) << 5;
    }
    tp = spr->tpage;
    p->x0 = x0;
    c = u + w + padW;
    p->x1 = x0 + w + padW;
    p->x3 = x2 + w + padW;
    p->y2 = y0 + h + padH;
    p->y3 = y1 + h + padH;
    p->x2 = x2;
    p->y0 = y0;
    p->y1 = y1;
    p->u2 = u;
    p->u0 = u;
    p->tpage = tp;
    if (c >= 0x100) {
        c--;
    }
    p->u3 = c;
    p->u1 = c;
    c = v + h - padH;
    p->v1 = v;
    p->v0 = v;
    if (c >= 0x100) {
        c--;
    }
    p->v3 = c;
    p->v2 = c;
    *(u16 *)&p->r0 = *(u16 *)&color->r;
    p->b0 = color->b;
    btlfx_addPrim(0x09000000, 0xFFE, p);
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
    s32 n;
    u16 *q;
    u16 *pal;
    s32 off;
    s32 wv;
    s32 hv;
    s32 num;
    u8 *prim;
    s32 i;
    u8 *c;
    s32 b;
    s32 tpage;
    s32 f;
    s32 clut;

    n = h->frameCount & 0x7FFF;
    f = frame;
    if (f >= n) {
        f %= n;
    }
    n = n * 2 + 8;
    off = f * 2;
    q = (u16 *)(data + n);
    n = *(u16 *)((u8 *)h->frameOfs + off);
    pal = q + 1;
    f = *q;
    tpage = h->tpage;
    clut = h->clut << 16;
    hv = h->h - 1;
    wv = h->w - 1;
    num = data[n++];
    data += n;
    prim = g_btlfxEnv->prim;
    mode = (u32 *)(prim + num * 20);
    g_btlfxEnv->prim = (u8 *)(mode + 2);
    mode[1] = tpage | 0xE1000000 | ((tp & 3) << 5);
    ctab = (u8 *)(pal + f + 1);
    col.code = (tp != 0xFF) ? 0x66 : 0x64;
    for (i = 0; i < num; i++, prim += 20) {
        ((SPRT *)prim)->x0 = (s8)data[0] + x - (wv >> 1);
        ((SPRT *)prim)->y0 = (s8)data[1] + y - (hv >> 1);
        b = data[2];
        f = (b & 0xF) * 2;
        *(u32 *)&((SPRT *)prim)->u0 = *(u16 *)((u8 *)pal + f) | clut;
        c = ctab + ((b >> 2) & 0x3C);
        data += 3;
        col.rg = *(u16 *)c;
        col.b = c[2];
        gte_DpqColorFar_nst(&col, alpha);
        ((SPRT *)prim)->w = wv;
        ((SPRT *)prim)->h = hv;
        gte_strgb(&((SPRT *)prim)->r0);
        btlfx_addPrim(0x04000000, 0xFFE, prim);
    }
    btlfx_addPrim(0x01000000, 0xFFE, mode);
}

void btlfx_drawBackdrop(BtlBgLayers *bg, s32 frame) {
    RECT r;
    s16 ofs[2];
    CVECTOR col;
    DR_TPAGE *tp;
    DR_AREA *da;
    DR_OFFSET *dofs;
    s32 *offs;
    s32 i;
    s32 n;
    s32 w;
    s32 h;
    s32 step;
    s32 x, y;
    BtlBgLayer *L;
    s32 u, v;
    s32 a;
    s32 k;
    s32 j;
    s32 tw;
    s32 cnt;
    s32 wt;
    TILE *t;
    struct BtlBgScrollP *sc;
    struct BtlBgWaveP *wv;
    struct BtlBgWaveP *wv3;
    struct BtlBgFadeP *fd;

    da = (DR_AREA *)g_btlfxEnv->prim;
    n = bg->count;
    w = bg->w;
    h = bg->h;
    x = bg->x;
    y = bg->y;
    g_btlfxEnv->prim = (u8 *)(da + 2);
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SetDrawArea(da, &r);
    ((XY32 *)&r)[0].w = ((XY32 *)&g_btlfxEnv->drawEnv->clip)[0].w;
    ((XY32 *)&r)[1].w = ((XY32 *)&g_btlfxEnv->drawEnv->clip)[1].w;
    SetDrawArea(&da[1], &r);
    i = 0;
    dofs = (DR_OFFSET *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(dofs + 2);
    ofs[0] = x;
    ofs[1] = y;
    SetDrawOffset(dofs, ofs);
    SetDrawOffset(&dofs[1], g_btlfxEnv->drawEnv->ofs);
    tp = (DR_TPAGE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(tp + 2);
    tp[0].code[0] = 0xE1000000;
    tp[1].code[0] = 0xE1000200;
    btlfx_addPrim(0x01000000, 0xFFE, &tp[1]);
    btlfx_addPrim(0x02000000, 0xFFE, &da[1]);
    btlfx_addPrim(0x02000000, 0xFFE, &dofs[1]);
    offs = bg->layers;
    for (; i < n; i++) {
        L = (BtlBgLayer *)((u8 *)bg + offs[i]);
        if (L->gradOfs != 0) {
            btlfx_sampleGradient((u8 *)bg + L->gradOfs, &col, frame % L->gradFrames);
        } else {
            *(s32 *)&col = 0x808080;
        }
        switch (L->kind) {
        case 0:
            sc = &L->params.scroll;
            if (sc->sineX == 0) {
                wt = u = frame * sc->speedX;
            } else {
                wt = u = (math_sin(sc->speedX * frame) * sc->ampX) >> 12;
            }
            BG_WRAP(u, w);
            if (sc->sineY == 0) {
                wt = v = frame * sc->speedY;
            } else {
                wt = v = (math_sin(sc->speedY * frame) * sc->ampY) >> 12;
            }
            BG_WRAP(v, h);
            btlfx_addSpriteQuad((SprInfo *)L, u, u, v, v, L->u, L->v, w, h, &col, 1, 1);
            if (u != 0) {
                btlfx_addSpriteQuad((SprInfo *)L, u - w, u - w, v, v, L->u, L->v, w, h, &col, 0, 1);
            }
            if (v != 0) {
                btlfx_addSpriteQuad((SprInfo *)L, u, u, v - h, v - h, L->u, L->v, w, h, &col, 1, 0);
            }
            if (u != 0 && v != 0) {
                btlfx_addSpriteQuad((SprInfo *)L, u - w, u - w, v - h, v - h, L->u, L->v, w, h, &col, 0, 0);
            }
            break;
        case 1:
            wv = &L->params.wave;
            wt = v = frame * wv->scrollSpeed;
            BG_WRAP(v, h);
            cnt = 1;
            if (v != 0) {
                cnt = 2;
            }
            step = wv->phaseStep;
            tw = wv->stripSize;
            a = frame * wv->phaseSpeed + v * step / tw;
            do {
                k = 0;
                for (j = 0; j < wv->stripCount; j++) {
                    u = ((math_sin(a) * wv->amplitude) >> 12) - (w >> 1);
                    a += step;
                    btlfx_addSpriteQuad((SprInfo *)L, u, ((math_sin(a) * wv->amplitude) >> 12) - (w >> 1), v, v, L->u, L->v + k, w * 2, tw, &col, 0, 0);
                    v += tw;
                    k += tw;
                }
                cnt--;
                v -= h * 2;
                a -= step * wv->stripCount * 2;
            } while (cnt != 0);
            break;
        case 2:
            fd = &L->params.fade;
            if (frame >= fd->delay) {
                s32 e, f;
                e = frame - fd->delay;
                if (e < fd->duration) {
                    f = (e << 12) / fd->duration;
                } else {
                    f = 0x1000;
                }
                if (fd->dataOfs != 0) {
                    btlfx_drawBackdropSprite((u8 *)bg + fd->dataOfs + 4, fd->x, fd->y, e, L->abr, f);
                }
            }
            break;
        case 3:
            wv3 = &L->params.wave;
            wt = v = frame * wv3->scrollSpeed;
            BG_WRAP(v, w);
            cnt = 1;
            if (v != 0) {
                cnt = 2;
            }
            step = wv3->phaseStep;
            tw = wv3->stripSize;
            a = frame * wv3->phaseSpeed + v * step / tw;
            do {
                k = 0;
                for (j = 0; j < wv3->stripCount; j++) {
                    u = ((math_sin(a) * wv3->amplitude) >> 12) - (h >> 1);
                    a += step;
                    btlfx_addSpriteQuad((SprInfo *)L, v, v, u, ((math_sin(a) * wv3->amplitude) >> 12) - (h >> 1), L->u + k, L->v, tw, h * 2, &col, 0, 0);
                    v += tw;
                    k += tw;
                }
                cnt--;
                v -= w * 2;
                a -= step * wv3->stripCount * 2;
            } while (cnt != 0);
            break;
        }
    }
    t = (TILE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(t + 1);
    *(s32 *)&t->r0 = 0;
    t->code = GPU_TILE;
    t->y0 = 0;
    t->x0 = 0;
    t->w = w;
    t->h = h;
    btlfx_addPrim(0x03000000, 0xFFE, t);
    btlfx_addPrim(0x01000000, 0xFFE, tp);
    btlfx_addPrim(0x02000000, 0xFFE, da);
    btlfx_addPrim(0x02000000, 0xFFE, dofs);
}

s32 btlfx_ease(s32 t, s32 duration, s32 mode) {
    s32 r = 0;
    switch (mode) {
    case 0:
        r = (t << 12) / duration;
        break;
    case 1:
        r = 0x1000 - math_cos((t << 11) / duration);
        break;
    case 2:
        r = math_sin((t << 11) / duration);
        break;
    }
    return r;
}

void btlfx_drawSpriteLine(u8 *data, SVECTOR *pos, s32 frame, s32 alpha) {
    s16 p[2];
    s16 q[2];
    s16 sxy[2];
    s32 progress;
    s32 i;
    s32 elapsed;
    s32 entryAlpha;
    s32 angle;
    s32 dist;
    Unk800D3B6CEntry *e;
    Unk800D3B6C *h;

    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(pos);
    h = (Unk800D3B6C *)data;
    e = h->entries;
    gte_rtps_b();
    progress = (frame << 12) / h->duration;
    gte_stsxy(sxy);
    if (h->polar == 0) {
        btlfx_lerpVec(h->from, h->to, progress, p);
        for (i = 0; i < h->count; i++, e++) {
            btlfx_lerpVec(sxy, p, e->along * 2, q);
            if (frame >= e->start) {
                elapsed = frame - e->start;
                if (elapsed < e->fadeIn) {
                    entryAlpha = elapsed * alpha / e->fadeIn;
                } else {
                    entryAlpha = alpha;
                }
                btlfx_drawSpriteAtOt(8, (u8 *)h + e->spriteOfs, q[0], q[1], frame, 1, entryAlpha);
            }
        }
    } else {
        angle = h->angle0 + (((h->angle1 - h->angle0) * btlfx_ease(frame, h->duration, h->angleEase)) >> 12);
        dist = h->dist0 + (((h->dist1 - h->dist0) * btlfx_ease(frame, h->duration, h->distEase)) >> 12);
        p[0] = sxy[0] + ((math_cos(angle) * dist) >> 12);
        p[1] = sxy[1] + ((math_sin(angle) * dist) >> 12);
        for (i = 0; i < h->count; i++, e++) {
            btlfx_lerpVec(sxy, p, e->along * 2, q);
            if (frame >= e->start) {
                elapsed = frame - e->start;
                if (elapsed < e->fadeIn) {
                    entryAlpha = elapsed * alpha / e->fadeIn;
                } else {
                    entryAlpha = alpha;
                }
                btlfx_drawSpriteAtOt(8, (u8 *)h + e->spriteOfs, q[0], q[1], frame, 1, entryAlpha);
            }
        }
    }
}
