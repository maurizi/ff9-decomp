#include "fxmod/fx_0e0.h"

static void fxm_drawMirrorSprite(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 abr) {
    POLY_FT4 *poly = (POLY_FT4 *)fx_0e0_env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e0_env);
    u8 col[4];
    s32 u1;
    s32 v1;
    s32 y1;

    fx_0e0_env->prim = (u8 *)(poly + 2);
    col[0] = col[1] = col[2] = bright;
    if (abr != 0xFF) {
        col[3] = 0x2E;
    } else {
        col[3] = 0x2C;
    }
    tpage |= TPAGE_ABR(abr);
    v1 = v + h - 1;
    poly->u0 = poly->u2 = u;
    poly->u1 = poly->u3 = u1 = u + w - 1;
    poly->v0 = poly->v1 = v;
    poly->v2 = poly->v3 = v1;
    poly->x0 = poly->x2 = x - w;
    poly->x1 = poly->x3 = x;
    poly->y0 = poly->y1 = y;
    poly->clut = clut;
    poly->tpage = tpage;
    *(u32 *)&poly->r0 = *(u32 *)col;
    y1 = y + h * 7 / 6;
    poly->y2 = poly->y3 = y1;
    ex->btlfx_addPrim(0x09000000, ot, poly);
    poly++;
    poly->x0 = poly->x2 = x;
    poly->u0 = poly->u2 = u1;
    poly->u1 = poly->u3 = u;
    poly->v0 = poly->v1 = v;
    poly->v2 = poly->v3 = v1;
    poly->x1 = poly->x3 = x + w;
    poly->y0 = poly->y1 = y;
    poly->y2 = poly->y3 = y1;
    poly->clut = clut;
    poly->tpage = tpage;
    *(u32 *)&poly->r0 = *(u32 *)col;
    ex->btlfx_addPrim(0x09000000, ot, poly);
}

static void fxm_drawTiledStrip(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 abr, s32 flip) {
    s32 dx;
    s32 tileW;
    POLY_FT4 *poly;
    BtlfxExports *ex;
    u8 col[4];
    s32 n;
    s32 i;
    s32 unused1;
    s32 unused2;
    s32 unused3;

    n = 2;
    ex = BTLFX_EXPORTS(fx_0e0_env);
    if (flip != 0) {
        n = 4;
    }
    poly = (POLY_FT4 *)fx_0e0_env->prim;
    fx_0e0_env->prim = (u8 *)(poly + n);
    col[0] = col[1] = col[2] = bright;
    if (abr != 0xFF) {
        col[3] = 0x2E;
    } else {
        col[3] = 0x2C;
    }
    tpage |= TPAGE_ABR(abr);
    dx = -0x140;
    if (flip != 0) {
        dx = 0xA0;
    }
    tileW = 0x140;
    if (flip != 0) {
        tileW = 0xA0;
    }
    for (i = 0; i < n; i++) {
        poly->x0 = poly->x2 = x;
        poly->x1 = poly->x3 = x + tileW;
        poly->y0 = poly->y1 = y;
        poly->y2 = poly->y3 = y + h;
        if (flip != 0) {
            poly->u0 = poly->u2 = u;
            poly->u1 = poly->u3 = u + w - 1;
        } else {
            poly->u0 = poly->u2 = u + w - 1;
            poly->u1 = poly->u3 = u;
        }
        poly->v0 = poly->v1 = v;
        poly->v2 = poly->v3 = v + h - 1;
        poly->clut = clut;
        poly->tpage = tpage;
        *(u32 *)&poly->r0 = *(u32 *)col;
        ex->btlfx_addPrim(0x09000000, ot, poly);
        poly++;
        x += dx;
    }
}

static void fxm_drawSprite3D(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e0_env);
    MATRIX m;
    VECTOR sc;
    SVECTOR zero;
    SVECTOR vtx[4];
    u8 col[4];
    s32 otz;
    POLY_FT4 *poly;
    s32 code;
    s32 half;
    s32 negHalf;

    if (rot == NULL) {
        rot = &zero;
        ((s32 *)&zero)[1] = 0;
        ((s32 *)&zero)[0] = 0;
    }
    gte_SetRotMatrix(fx_0e0_env->viewMatrix);
    gte_SetTransMatrix(fx_0e0_env->viewMatrix);
    gte_ldv0(pos);
    poly = (POLY_FT4 *)fx_0e0_env->prim;
    fx_0e0_env->prim = (u8 *)(poly + 1);
    col[0] = col[1] = col[2] = bright >> 5;
    col[3] = (abr != 0xFF) ? 0x2E : 0x2C;
    tpage |= TPAGE_ABR(abr);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    sc.vx = sc.vy = sc.vz = scale;
    ex->ScaleMatrix(&m, &sc);
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        ex->MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    half = w << 6;
    negHalf = -half;
    vtx[1].vx = vtx[3].vx = half;
    vtx[0].vx = vtx[2].vx = negHalf;
    half = h << 6;
    negHalf = -half;
    vtx[2].vy = vtx[3].vy = half;
    vtx[0].vz = vtx[1].vz = vtx[2].vz = vtx[3].vz = 0;
    vtx[0].vy = vtx[1].vy = negHalf;
    gte_ldv3c(vtx);
    gte_rtpt();
    gte_nclip();
    poly->tpage = tpage;
    poly->clut = clut;
    gte_stopz(&otz);
    if (otz != 0) {
        gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
        gte_avsz3();
        gte_stszotz(&otz);
        gte_ldv0(&vtx[3]);
        gte_rtps();
        poly->u1 = poly->u3 = u + w - 1;
        poly->u0 = poly->u2 = u;
        poly->v0 = poly->v1 = v;
        poly->v2 = poly->v3 = v + h - 1;
        *(u32 *)&poly->r0 = *(u32 *)col;
        gte_stsxy(&poly->x3);
        ex->btlfx_addPrim(0x09000000, otz, poly);
    }
}

static void fxm_drawBulgeGrid24(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr) {
    MATRIX m;
    VECTOR sc;
    SVECTOR vtx[4];
    SVECTOR center;
    s32 otz;
    s32 j;
    s32 i;
    s32 code;
    BtlfxExports *ex;
    POLY_GT4 *poly;
    s32 dist0;
    s32 dist1;
    s32 dist2;
    s32 dist3;
    s32 shade;
    s32 tileV;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;

    ex = BTLFX_EXPORTS(fx_0e0_env);
    ((s32 *)&center)[0] = 0;
    center.vz = 0;
    gte_SetRotMatrix(fx_0e0_env->viewMatrix);
    gte_SetTransMatrix(fx_0e0_env->viewMatrix);
    gte_ldv0(pos);
    poly = (POLY_GT4 *)fx_0e0_env->prim;
    code = 0x3C;
    fx_0e0_env->prim = (u8 *)(poly + 576);
    if (abr != 0xFF) {
        code = 0x3E;
    }
    tpage |= TPAGE_ABR(abr);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    sc.vx = sc.vy = sc.vz = scale;
    ex->ScaleMatrix(&m, &sc);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    for (j = 0; j < 24; j++) {
        for (i = 0; i < 24; i++) {
            vtx[0].vx = vtx[2].vx = j * 0x180 - 0x1200;
            vtx[0].vz = vtx[1].vz = vtx[2].vz = vtx[3].vz = 0;
            vtx[0].vy = vtx[1].vy = i * 0x180 - 0x1200;
            vtx[2].vy = vtx[3].vy = i * 0x180 - 0x1080;
            vtx[1].vx = vtx[3].vx = j * 0x180 - 0x1080;
            dist0 = ex->btlfx_distance(&vtx[0], &center);
            dist1 = ex->btlfx_distance(&vtx[1], &center);
            dist2 = ex->btlfx_distance(&vtx[2], &center);
            dist3 = ex->btlfx_distance(&vtx[3], &center);
            if (dist0 > 0xC00) {
                vtx[0].vz = 0;
            } else {
                vtx[0].vz = ((ex->math_cos((dist0 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (dist1 > 0xC00) {
                vtx[1].vz = 0;
            } else {
                vtx[1].vz = ((ex->math_cos((dist1 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (dist2 > 0xC00) {
                vtx[2].vz = 0;
            } else {
                vtx[2].vz = ((ex->math_cos((dist2 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (dist3 > 0xC00) {
                vtx[3].vz = 0;
            } else {
                vtx[3].vz = ((ex->math_cos((dist3 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            gte_ldv3c(vtx);
            gte_rtpt();
            shade = bright - (vtx[0].vz >> 4);
            if (shade < 0) {
                shade = 0;
            }
            poly->code = code;
            poly->r0 = poly->g0 = poly->b0 = shade;
            shade = bright - (vtx[1].vz >> 4);
            if (shade < 0) {
                shade = 0;
            }
            poly->r1 = poly->g1 = poly->b1 = shade;
            shade = bright - (vtx[2].vz >> 4);
            if (shade < 0) {
                shade = 0;
            }
            poly->r2 = poly->g2 = poly->b2 = shade;
            shade = bright - (vtx[3].vz >> 4);
            if (shade < 0) {
                shade = 0;
            }
            poly->r3 = poly->g3 = poly->b3 = shade;
            gte_nclip();
            poly->tpage = tpage;
            poly->clut = clut;
            gte_stopz(&otz);
            if (otz != 0) {
                gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&vtx[3]);
                gte_rtps();
                u0 = u + ((j - 1) & 1) * 48;
                tileV = (i - 1) & 1;
                v0 = v + tileV * 48;
                u1 = u0 + 0x30;
                if ((j - 1) & 1) {
                    u1 = u0 + 0x2F;
                }
                v1 = v0 + 0x30;
                if (tileV != 0) {
                    v1 = v0 + 0x2F;
                }
                poly->u0 = poly->u2 = u0;
                poly->u1 = poly->u3 = u1;
                poly->v0 = poly->v1 = v0;
                poly->v2 = poly->v3 = v1;
                ex->btlfx_addPrim(0x0C000000, 2, poly);
                gte_stsxy(&poly->x3);
                poly++;
            }
        }
    }
}

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0e0Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e0_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->pts4;
    bolt->count2 = 0x18;
    bolt->lastSlot0 = 0x18;
    bolt->head1 = 0;
    for (i = 0; i < 25; i++) {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = ex->btlfx_randCentered(0xA0);
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = lineWidth;
        ang += dang + ex->btlfx_randCentered(0x80);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_PURPLE_STREAK), 0, 0, frame, 0x1000, fade, 1, (u8 *)bolt, 0);
}

INCLUDE_RODATA("asm/us/fx_0e0/nonmatchings/fx_0e0", fx_0e0_headerPtr);

static s32 fx_0e0_op80PoolTask(s32 mode, Fx0e0Obj *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool,
                  Fx0e0Inst *parent) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR toParent;
    SVECTOR old;
    SVECTOR trailPos;
    SVECTOR pad;
    Pool6CEntry *emitter;
    s32 fade;
    s32 t;
    s32 c;
    s32 i;
    s32 sh;

    if (mode == 1) {
        it->phase2 = 0;
        it->rot14.vx = ex->btlfx_rand();
        it->rot14.vy = ex->btlfx_rand();
        it->rot14.vz = 0;
        it->spinX1C = ex->btlfx_rand() % 64;
        it->spinY1E = ex->btlfx_rand() % 32;
        it->emit20 = NULL;
    } else {
        switch (it->state0) {
        case 0:
        case 1:
            fade = 0;
            switch (it->phase2) {
            case 0:
                it->rot14.vx += it->spinX1C;
                it->rot14.vy += it->spinY1E;
                old.vx = it->posC.vx;
                old.vy = it->posC.vy;
                old.vz = it->posC.vz;
                it->posC.vx += it->vel4.vx;
                it->posC.vy += it->vel4.vy;
                it->posC.vz += it->vel4.vz;
                pos.vx = it->posC.vx;
                pos.vy = it->posC.vy;
                pos.vz = it->posC.vz;
                scale.vx = scale.vy = scale.vz = fx_0e0_objScales[it->vel4.pad];
                ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, parent->model5C);
                toParent.vx = parent->pos8.vx - it->posC.vx;
                toParent.vy = parent->pos8.vy - it->posC.vy;
                toParent.vz = parent->pos8.vz - it->posC.vz;
                if (frame < 8) {
                    fade = (frame << 12) >> 3;
                } else {
                    fade = 0x1000;
                }
                if (toParent.vx * toParent.vx + toParent.vy * toParent.vy + toParent.vz * toParent.vz < fx_0e0_captureRadius * fx_0e0_captureRadius * fx_0e0_captureRadius) {
                    it->phase2 = 1;
                    ((s16 *)it)[-1] = -1;
                    it->emit20 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_EMBERS), (s32)&it->posC);
                    if (emitter != NULL) {
                        emitter->otZ = 0x20;
                    }
                }
                break;
            case 1:
                scale.vx = scale.vy = scale.vz = fx_0e0_objScales[it->vel4.pad];
                pos.vx = it->posC.vx;
                pos.vy = it->posC.vy;
                pos.vz = it->posC.vz;
                ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, parent->model5C);
                ex->btlfx_aimAngles((s16 *)&parent->pos8, (s16 *)&it->posC, (s16 *)&ang);
                ang.pad = 1;
                if (it->emit20 != NULL) {
                    it->emit20->rot.vx = ang.vx;
                    it->emit20->rot.vy = ang.vy;
                    it->emit20->rot.vz = ang.vz;
                }
                if (frame < 4) {
                    fade = 0x1000 - ((frame << 12) >> 2);
                }
                it->vel4.vx = it->vel4.vx * 2 / 3;
                it->vel4.vy = it->vel4.vy * 2 / 3;
                it->vel4.vz = it->vel4.vz * 2 / 3;
                env->otZ = 0x1F4;
                if (frame < 8) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_FIRE_RING), &it->posC, NULL, 0x3000, frame, 1,
                                      ex->math_cos(frame << 7));
                }
                env->otZ = 0x20;
                if (frame < 8) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_WHITE_RING), &it->posC, &ang, 0x2000, frame, 1,
                                      ex->math_cos(frame << 7));
                }
                env->otZ = 0;
                if (fx_0e0_endEarly != 0) {
                    return 1;
                }
                break;
            }
            if (fade != 0) {
                env->otZ = 0x180;
                BTLFX_COPY_SVEC(&trailPos, &it->posC);
                trailPos.vx -= it->vel4.vx;
                trailPos.vy -= it->vel4.vy;
                trailPos.vz -= it->vel4.vz;
                for (i = 0; i < 12; i++) {
                    trailPos.vx -= it->vel4.vx;
                    trailPos.vy -= it->vel4.vy;
                    trailPos.vz -= it->vel4.vz;
                    fxm_drawSprite3D(&trailPos, NULL, 0x600, fade, 0x1A, 0x3D06, (i & 3) << 6, (i >> 2) << 6, 0x40, 0x40, 1);
                }
                env->otZ = 0;
            }
            break;
        case 2:
            t = (frame << 12) / 24;
            scale.vx = scale.vy = (ex->math_sin(t >> 2) >> 1) + 0x800;
            scale.vz = it->vel4.pad + 0x200;
            it->rot14.vz -= 0x40;
            it->posC.vy -= 0x4D;
            pos.vx = it->posC.vx;
            pos.vy = it->posC.vy;
            pos.vz = it->posC.vz;
            ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, parent->model70);
            ex->Hi_ModifyEffModelRGB(parent->model70, (ex->math_sin(t >> 1) >> 5) - 0x80, (ex->math_sin(t >> 1) >> 5) - 0x80,
                                     (ex->math_sin(t >> 1) >> 5) - 0x80);
            if (frame >= 24) {
                return 1;
            }
            break;
        case 3:
            it->rot14.vx += it->spinX1C * 2;
            it->rot14.vy += it->spinY1E * 2;
            it->posC.vx += it->vel4.vx;
            it->posC.vy += it->vel4.vy;
            it->posC.vz += it->vel4.vz;
            pos.vx = it->posC.vx;
            pos.vy = it->posC.vy;
            pos.vz = it->posC.vz;
            scale.vz = ((it->vel4.pad * ex->math_cos((frame << 8) >> 2)) >> 12) * 2;
            scale.vx = scale.vy = scale.vz / 3;
            ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, parent->model5C);
            ex->Hi_ModifyEffModelRGB(parent->model5C, -0x4B, -0x4B, -0x4B);
            if (frame >= 16) {
                return 1;
            }
            break;
        case 4:
            it->rot14.vx += it->spinX1C;
            it->rot14.vy += it->spinY1E;
            it->posC.vx += it->vel4.vx;
            it->posC.vy += it->vel4.vy;
            it->posC.vz += it->vel4.vz;
            it->vel4.vx = (it->vel4.vx * 31) >> 5;
            it->vel4.vy = (it->vel4.vy * 31) >> 5;
            it->vel4.vz = (it->vel4.vz * 31) >> 5;
            pos.vx = it->posC.vx;
            pos.vy = it->posC.vy;
            sh = frame << 6;
            /* Scheduling barrier: frame << 6 fills the pos.vy load delay and pos.vz's stays a nop. */
            do {
            } while (0);
            pos.vz = it->posC.vz;
            scale.vz = ((it->vel4.pad * ex->math_cos(sh >> 2)) >> 12) * 2;
            scale.vx = scale.vy = scale.vz >> 1;
            ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, parent->model5C);
            ex->Hi_ModifyEffModelRGB(parent->model5C, 0, -0x4B, -0x4B);
            if (frame >= 0x40) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

/* Matching notes: case 6's two loop counters are different variables (n, then i; both s3), with i = 0 after the
 * if, and an empty do { } while (0); barrier before the if keeps the call-crossing constant set from floating up
 * in sched1; `one` is set in the mode test and passed as case 10's 13th argument (the original's s0); the frame and
 * state loads come before the col/scl/xo inits; a dead-store-only if/else after them ends cse's block so the
 * per-case xo = 0 stores survive; case 6/7's unk44/unk50/unk24 updates in searched order; from's fields copied
 * before both += adjustments (flow keeps both stores); case 7's tint in a variable and t * 3 for frame * 0x300; case
 * 8 reuses t, writes t / 5 and casts the random part to s16. */
s32 fx_0e0_op80(s32 mode, Fx0e0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0e0Tmp scratch;
    VECTOR sc;
    SVECTOR rot;
    SVECTOR from;
    SVECTOR to;
    Fx0e0Spark *sp;
    s32 pulseAng;
    s32 flareAng;
    s32 coreAng;
    s32 rippleAng;
    s32 rgb;
    s32 spark;
    s32 trailAng;
    s32 trailFade;
    s32 tc;
    s32 frame;
    s32 col;
    s32 scl;
    s32 xo;
    s32 t;
    s32 flareT;
    s32 tint;
    s32 i;
    s32 r;
    s32 state;
    s32 n;
    s32 one;

    if (mode == 0) {
        init->workSize = 0x84;
        init->elemSize = 0x24;
        init->elemFn = (s32)fx_0e0_op80PoolTask;
        init->poolCount = 0x31;
    } else if (one = 1, mode == one) {
        work->state0 = 6;
        work->frame4 = 0;
        fx_0e0_env = env;
        ex->btlfx_initEffModels(fx_0e0_effModels, 8);
        work->model5C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_METEOR), 0x98, 0x3D80, 0, 0);
        work->model60 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_BIG_METEOR), 0x118, 0xFFFF, 0x40, 0);
        work->model64 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_BOULDER_2), 0x118, 0xFFFF, 0x40, 0);
        work->model68 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_STAR_TUNNEL), 0x37, 0x3D07, 0, 0);
        work->model6C = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_RED_DOME));
        work->model70 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_STAR_ARCH), 0x37, 0x3D07, 0, 0);
        work->model74 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_LIGHT_CONE), 0xBC, 0x3E00, 0x80, 0);
        fx_0e0_centerPos.vz += 0x400;
        BTLFX_COPY_SVEC(&work->pos8, &fx_0e0_centerPos);
        fx_0e0_centerPos.vz -= 0x400;
        work->scale30.vy = work->scale30.vx = 0x2800;
        work->pos24.vx = work->pos8.vx;
        work->pos24.vy = work->pos8.vy;
        work->pos24.vz = work->pos8.vz;
        work->pos24.vy -= 0x3800;
        work->pos24.vz -= 0x1C00;
        work->scale30.vz = 0x1800;
        work->pos10.vx = work->pos24.vx;
        work->pos10.vy = work->pos24.vy;
        work->pos10.vz = work->pos24.vz;
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->rot3C);
        BTLFX_COPY_SVEC(&work->rot50, &work->rot3C);
    } else {
        state = work->state0;
        frame = entry->frame;
        col = 0;
        scl = 0;
        xo = 0;
        if (mode) {
            pulseAng = 1;
            flareAng = 1;
        } else {
            pulseAng = 2;
            flareAng = 2;
        }
        switch (state) {
        case 6:
            t = (frame << 12) / 66;
            col = ((t << 5) >> 12) + 0x8D;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, (t * 0x58) >> 12, 0, 0);
            pulseAng = t >> 2;
            BTLFX_COPY_SVEC(&work->camAt18, &work->pos8);
            work->camAt18.vy -= 0x400 + (ex->math_sin(pulseAng) >> 1);
            scl = 0x80;
            xo = 0;
            env->camera->points[3].vx = work->camAt18.vx;
            env->camera->points[3].vy = work->camAt18.vy;
            env->camera->points[3].vz = work->camAt18.vz;
            work->rot3C.vz += 8;
            work->pos24.vy += 0x90;
            work->pos24.vz += 0x46;
            ex->Hi_DrawEffModel(&work->rot3C, (VECTOR *)&work->pos24, (s32)&work->scale30, work->model64);
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_BACKDROP), work->frame4);
            work->scale44.vx = work->scale44.vy = 0x3800;
            work->scale44.vz = 0x2000;
            work->pos24.vy -= 0x5DC;
            work->pos24.vz += 0x2EE;
            work->rot50.vz += 0x14;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
            ex->Hi_ModifyEffModelRGB(work->model68, (((ex->math_cos(pulseAng) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(pulseAng) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(pulseAng) >> 1) + 0x800) >> 5) - 0x80);
            work->rot50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
            ex->Hi_ModifyEffModelRGB(work->model68, (((ex->math_cos(pulseAng) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(pulseAng) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(pulseAng) >> 1) + 0x800) >> 5) - 0x80);
            work->scale44.vx = work->scale44.vy = 0x2000;
            work->rot50.vz += 0x200;
            work->pos24.vy += 0x5DC;
            work->pos24.vz -= 0x2EE;
            work->scale44.vz = 0x3000;
            work->pos24.vy -= 0x125C;
            work->pos24.vz -= 0x8FC;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model6C);
            rgb = (((t * 2) - 0x1000) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model6C, rgb, rgb, rgb);
            work->pos24.vy += 0x125C;
            work->pos24.vz += 0x8FC;
            if (frame > 0x32) {
                n = 0;
                flareT = (frame - 0x32) << 8;
                sc.vx = sc.vy = (ex->math_sin(flareT >> 2) * 5) / 2 + 0x400;
                flareAng = flareT >> 2;
                sc.vz = flareT + 0x1800;
                rot.vx = -0x400;
                rot.vy = 0;
                scratch.v.vx = fx_0e0_centerPos.vx;
                rot.vz = frame * 0x8D;
                scratch.v.vy = fx_0e0_centerPos.vy;
                scratch.v.vz = fx_0e0_centerPos.vz;
                scratch.v.vy -= 0x100;
                do {
                    ex->Hi_DrawEffModel(&rot, &scratch.v, (s32)&sc, work->model70);
                    ex->Hi_ModifyEffModelRGB(work->model70, (ex->math_sin(flareAng) >> 5) - 0x80, (ex->math_sin(flareAng) >> 5) - 0x80,
                                             (ex->math_sin(flareAng) >> 5) - 0x80);
                    n++;
                    sc.vx = sc.vy = (sc.vx * 3) >> 2;
                    rot.vz += 0x400;
                    scratch.v.vy += 0x20;
                } while (n <= 0);
                sc.vx = sc.vz = (ex->math_sin(flareT >> 2) * 5) / 2 + 0x1000;
                sc.vy = ((flareT * 0x300) >> 12) + 0x800;
                rot.vx = 0;
                rot.vy = frame * 0x8D;
                rot.vz = 0;
                scratch.v.vx = fx_0e0_centerPos.vx;
                scratch.v.vy = fx_0e0_centerPos.vy;
                scratch.v.vz = fx_0e0_centerPos.vz;
                ex->Hi_DrawEffModel(&rot, &scratch.v, (s32)&sc, work->model74);
                coreAng = flareT >> 1;
                ex->Hi_ModifyEffModelRGB(work->model74, (ex->math_sin(coreAng) >> 5) - 0x80, (ex->math_sin(coreAng) >> 5) - 0x80,
                                         (ex->math_sin(coreAng) >> 5) - 0x80);
            }
            from.vx = work->pos24.vx;
            from.vy = work->pos24.vy;
            from.vz = work->pos24.vz;
            from.vy += 0x1838;
            from.vz += 0xC1C;
            for (i = 0; i < 1; i++) {
                trailAng = ex->btlfx_rand();
                trailFade = ex->btlfx_randCentered(0x400) + 0xC00;
                ((s32 *)&to)[0] = work->pos24.vx;
                ((s32 *)&to)[1] = work->pos24.vy;
                ex->btlfx_offsetPoint(&from, (s32)&to, 0x1000, (XY32 *)&work->rot3C, 0x3000, trailAng);
                fxm_drawOffsetTrail(entry, &fx_0e0_trailStrips[i], &from, &to, 0x100, 0x200, trailAng, 0xC0, 0, trailFade, (ex->btlfx_rand() & 0x1F) + 0x58);
            }
            if (frame >= 0x42) {
                work->state0 = 7;
                entry->frame = -1;
                work->pos8.vx = work->pos24.vx;
                work->pos8.vy = work->pos24.vy;
                work->pos8.vz = work->pos24.vz;
                work->pos10.vx = work->pos24.vx;
                work->pos10.vy = work->pos24.vy;
                work->pos10.vz = work->pos24.vz;
            }
            break;
        case 7:
            xo = 0;
            t = frame << 8;
            rippleAng = t >> 2;
            col = 0xAD;
            scl = rippleAng + 0x80;
            fx_0e0_rippleColors[1].r = (ex->math_sin(rippleAng) * 0xFF) >> 12;
            fx_0e0_rippleColors[1].g = (ex->math_sin(rippleAng) * 0xFF) >> 12;
            fx_0e0_rippleColors[1].b = (u32)ex->math_sin(rippleAng) >> 6;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x58, 0, 0);
            ex->btlfx_drawRippleGrid((s32)(env->ot + 0xFF0), 0x28, 2, 2, 0xC8, 0xFF, fx_0e0_rippleColors, 0, 0, 0, 0);
            if (frame > 8) {
                tint = ((frame - 8) * 0xFF) / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, tint, tint, tint);
            }
            work->pos24.vy = work->pos8.vy + (t >> 6) + ((((frame & 1) * 2 - 1) * ex->math_cos(t >> 2)) >> 6);
            ex->Hi_DrawEffModel(&work->rot3C, (VECTOR *)&work->pos24, (s32)&work->scale30, work->model64);
            ex->Hi_ModifyEffModelRGB(work->model64, ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80);
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_BACKDROP), work->frame4);
            work->scale44.vx = work->scale44.vy = 0x3800;
            work->scale44.vz = 0x2000;
            work->pos24.vy -= 0x5DC;
            work->pos24.vz += 0x2EE;
            work->rot50.vz += 8;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
            ex->Hi_ModifyEffModelRGB(work->model68, (ex->math_cos(t >> 2) >> 6) - 0x80, (ex->math_cos(t >> 2) >> 6) - 0x80,
                                     (ex->math_cos(t >> 2) >> 6) - 0x80);
            work->rot50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
            ex->Hi_ModifyEffModelRGB(work->model68, (ex->math_cos(t >> 2) >> 6) - 0x80, (ex->math_cos(t >> 2) >> 6) - 0x80,
                                     (ex->math_cos(t >> 2) >> 6) - 0x80);
            work->scale44.vx = work->scale44.vy = 0x2000;
            work->rot50.vz += 0x200;
            work->pos24.vy += 0x5DC;
            work->pos24.vz -= 0x2EE;
            work->scale44.vz = 0x3000;
            work->pos24.vy -= 0x1388;
            work->pos24.vz -= 0x9C4;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model6C);
            ex->Hi_ModifyEffModelRGB(work->model6C, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                     (ex->math_cos(t >> 2) >> 5) - 0x80);
            work->pos24.vy += 0x1388;
            work->pos24.vz += 0x9C4;
            env->otZ = 0x898;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_LILAC_RINGS), &fx_0e0_centerPos, NULL, t * 3 + 0x1000,
                              frame, 1, ex->math_sin(t >> 2));
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_BLUE_RINGS), &fx_0e0_centerPos, NULL, t * 2 + 0x4000, frame, 1,
                              ex->math_sin(t >> 2));
            env->otZ = 0;
            if (frame >= 0x10) {
                work->state0 = 8;
                entry->frame = -1;
                work->pos10.vx = work->camAt18.vx;
                work->pos10.vy = work->camAt18.vy;
                work->pos10.vz = work->camAt18.vz;
                work->pos10.vx -= 0x2800;
                work->pos10.vy += 0xB86;
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_PEBBLES), (s32)&work->pos10);
            }
            break;
        case 8:
            t = frame << 7;
            col = 0xBC;
            scl = 0x200;
            xo = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 0xFF8), 0xFF, 0xFF, 0xFF, 0xC8);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x40, 0, 0);
            if (frame >= 0x1A) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, ((frame - 0x1A) << 7) / 6, ((frame - 0x1A) << 7) / 6,
                                         ((frame - 0x1A) << 7) / 6);
            }
            if (frame >= 0xA) {
                for (spark = 0; spark < 3; spark++) {
                    sp = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (sp != NULL) {
                        sp->posXC = work->camAt18.vx - ((t >> 4) + 0x1800) + ex->btlfx_randCentered(0x200);
                        sp->posYE = 0x6A4 - ((0x1000 - t) >> 1);
                        sp->posZ10 = work->camAt18.vz + ex->btlfx_randCentered(0x1C00);
                        sp->velX4 = ex->btlfx_randCentered(0x100);
                        sp->velY6 = -0x140 - (ex->btlfx_rand() & 0xFF);
                        sp->velZ8 = ex->btlfx_randCentered(0x100);
                        sp->rotX14 = ex->btlfx_rand();
                        sp->rotY16 = ex->btlfx_rand();
                        sp->rotZ18 = ex->btlfx_rand();
                        sp->state0 = 3;
                        sp->unkA = t / 5 + (s16)((ex->btlfx_rand() & 0x3FF) + 0x80);
                    }
                }
            }
            if (frame == 1) {
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_ROCK_DEBRIS), (s32)&work->pos10);
            }
            if (frame == 4) {
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_ROCK_DEBRIS), (s32)&work->pos10);
            }
            if (frame >= 0x20) {
                work->state0 = 0xA;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF, 0xFF, 0xFF);
                ex->btlfx_poolClear(entry->pool);
                ex->btlfx_initParticleSystem();
                work->camAt18.vx = 0;
                work->camAt18.vy = -0x200;
                work->camAt18.vz = 0;
                env->camera->points[3].vx = work->camAt18.vx;
                env->camera->points[3].vy = work->camAt18.vy;
                env->camera->points[3].vz = work->camAt18.vz;
            }
            break;
        case 10:
            t = frame << 8;
            tc = 0xF5 - ((t * 0xF5) >> 12);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tc, tc, tc);
            scratch.s.rot.vx = 0;
            scratch.s.rot.vy = 0;
            scratch.s.rot.vz = 0x20;
            sc.vy = sc.vx = 0x1100;
            sc.vz = 0x1000;
            tc = 0xA0 - ((frame << 15) >> 12);
            scratch.s.col.r = scratch.s.col.g = scratch.s.col.b = tc;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &scratch.s.rot, &sc, &scratch.s.col, one, NULL);
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (col != 0) {
            scratch.s.col.r = scratch.s.col.g = scratch.s.col.b = col;
            scratch.s.rot.vx = 0;
            scratch.s.rot.vy = 0;
            scratch.s.rot.vz = 0;
            sc.vy = sc.vx = scl + 0x1000;
            sc.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, xo + 0xA0, 0x78, 0x50, 0x3C, 4, 4, 2, 2, &scratch.s.rot, &sc, &scratch.s.col, 0, NULL);
        }
        if ((u32)(work->state0 - 3) < 5) {
            ex->btlfx_drawScrollMove((0x1F4 - work->frame4) * 8, 0x1E0, 0x100, 0x20, 0x80, 0x300, 0x100);
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_0e0_op84(s32 mode, Fx0e0Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 alpha;
    s32 scale0;
    s32 scale1;
    s32 flicker;
    s32 ang;
    s32 t;
    u16 ry;

    if (mode == 0) {
        *args = 0x10;
    } else {
        if (mode == 1) {
            fx_0e0_env = env;
            work->state0 = 0;
            work->frame4 = 0;
        } else {
            ry = scale1 = scale0 = alpha = 0;
            frame = entry->frame;
            ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
            switch (work->state0) {
            case 0:
                ry = 0x200;
                alpha = (frame << 12) / 3;
                scale0 = ex->math_sin(alpha >> 2) >> 1;
                scale1 = ((0x1000 - alpha) >> 1) + 0x800;
                if (frame >= 3) {
                    work->state0 = 1;
                    entry->frame = -1;
                }
                break;
            case 1:
                flicker = (frame & 1) << 8;
                scale1 = flicker + 0x1000;
                t = (frame << 12) / 14;
                alpha = (flicker + 0x800) + (t >> 1);
                ry = (t >> 3) + 0x200;
                scale0 = scale1;
                if (frame >= 0xE) {
                    work->state0 = 2;
                    entry->frame = -1;
                }
                break;
            case 2:
                t = (frame << 12) / 14;
                ang = t >> 2;
                ry = 0x400 - ang;
                alpha = ex->math_cos(ang);
                scale0 = ex->math_sin(ang) + 0x1000;
                scale1 = ex->math_sin(ang) * 2 + 0x1000;
                if (frame >= 0xE) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = ry;
            rot.pad = 0;
            env->otZ = 0x18;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_SWORD_GLINT), &work->pos8, NULL, scale0 * 3 / 4, 0, 1, alpha / 2);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_DOOMSDAY_SWORD_FLASH), &work->pos8, &rot, scale1, 0, 1, alpha * 2 / 3);
            env->otZ = 0;
            work->frame4++;
        }
    }
    return 0;
}
