#include "fxmod/fx_134.h"

static void fxm_drawMirrorSprite(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr) {
    POLY_FT4 *poly = (POLY_FT4 *)fx_134_env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_134_env);
    u8 col[4];
    s32 u1;
    s32 v1;
    s32 y1;

    fx_134_env->prim = (u8 *)(poly + 2);
    col[0] = col[1] = col[2] = c;
    if (abr != 0xFF) {
        col[3] = 0x2E;
    } else {
        col[3] = 0x2C;
    }
    tpage |= (abr & 3) << 5;
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

static void fxm_drawTiledStrip(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr, s32 flip) {
    s32 dx;
    s32 w2;
    POLY_FT4 *poly;
    BtlfxExports *ex;
    u8 col[4];
    s32 n;
    s32 i;
    s32 u1;
    s32 v1;
    s32 y1;

    n = 2;
    ex = BTLFX_EXPORTS(fx_134_env);
    if (flip != 0) {
        n = 4;
    }
    poly = (POLY_FT4 *)fx_134_env->prim;
    fx_134_env->prim = (u8 *)(poly + n);
    col[0] = col[1] = col[2] = c;
    if (abr != 0xFF) {
        col[3] = 0x2E;
    } else {
        col[3] = 0x2C;
    }
    tpage |= (abr & 3) << 5;
    dx = -0x140;
    if (flip != 0) {
        dx = 0xA0;
    }
    w2 = 0x140;
    if (flip != 0) {
        w2 = 0xA0;
    }
    for (i = 0; i < n; i++) {
        poly->x0 = poly->x2 = x;
        poly->x1 = poly->x3 = x + w2;
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

static void fxm_drawSprite3D(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_134_env);
    MATRIX m;
    VECTOR sc;
    SVECTOR zero;
    SVECTOR vtx[4];
    u8 col[4];
    s32 otz;
    POLY_FT4 *poly;
    s32 code;
    s32 a;
    s32 b;

    if (rot == NULL) {
        rot = &zero;
        ((s32 *)&zero)[1] = 0;
        ((s32 *)&zero)[0] = 0;
    }
    gte_SetRotMatrix(fx_134_env->viewMatrix);
    gte_SetTransMatrix(fx_134_env->viewMatrix);
    gte_ldv0(pos);
    poly = (POLY_FT4 *)fx_134_env->prim;
    fx_134_env->prim = (u8 *)(poly + 1);
    col[0] = col[1] = col[2] = c >> 5;
    col[3] = (abr != 0xFF) ? 0x2E : 0x2C;
    tpage |= (abr & 3) << 5;
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
    a = w << 6;
    b = -a;
    vtx[1].vx = vtx[3].vx = a;
    vtx[0].vx = vtx[2].vx = b;
    a = h << 6;
    b = -a;
    vtx[2].vy = vtx[3].vy = a;
    vtx[0].vz = vtx[1].vz = vtx[2].vz = vtx[3].vz = 0;
    vtx[0].vy = vtx[1].vy = b;
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

static void fxm_drawBulgeGrid24(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr) {
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
    s32 d0;
    s32 d1;
    s32 d2;
    s32 d3;
    s32 r;
    s32 iv;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;

    ex = BTLFX_EXPORTS(fx_134_env);
    ((s32 *)&center)[0] = 0;
    center.vz = 0;
    gte_SetRotMatrix(fx_134_env->viewMatrix);
    gte_SetTransMatrix(fx_134_env->viewMatrix);
    gte_ldv0(pos);
    poly = (POLY_GT4 *)fx_134_env->prim;
    code = 0x3C;
    fx_134_env->prim = (u8 *)(poly + 576);
    if (abr != 0xFF) {
        code = 0x3E;
    }
    tpage |= (abr & 3) << 5;
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
            d0 = ex->btlfx_distance(&vtx[0], &center);
            d1 = ex->btlfx_distance(&vtx[1], &center);
            d2 = ex->btlfx_distance(&vtx[2], &center);
            d3 = ex->btlfx_distance(&vtx[3], &center);
            if (d0 > 0xC00) {
                vtx[0].vz = 0;
            } else {
                vtx[0].vz = ((ex->math_cos((d0 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (d1 > 0xC00) {
                vtx[1].vz = 0;
            } else {
                vtx[1].vz = ((ex->math_cos((d1 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (d2 > 0xC00) {
                vtx[2].vz = 0;
            } else {
                vtx[2].vz = ((ex->math_cos((d2 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (d3 > 0xC00) {
                vtx[3].vz = 0;
            } else {
                vtx[3].vz = ((ex->math_cos((d3 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            gte_ldv3c(vtx);
            gte_rtpt();
            r = c - (vtx[0].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            poly->code = code;
            poly->r0 = poly->g0 = poly->b0 = r;
            r = c - (vtx[1].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            poly->r1 = poly->g1 = poly->b1 = r;
            r = c - (vtx[2].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            poly->r2 = poly->g2 = poly->b2 = r;
            r = c - (vtx[3].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            poly->r3 = poly->g3 = poly->b3 = r;
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
                iv = (i - 1) & 1;
                v0 = v + iv * 48;
                u1 = u0 + 0x30;
                if ((j - 1) & 1) {
                    u1 = u0 + 0x2F;
                }
                v1 = v0 + 0x30;
                if (iv != 0) {
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

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx134Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_134_env);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->pos4;
    bolt->count2 = 0x18;
    bolt->lastSlot0 = 0x18;
    bolt->head1 = 0;
    for (i = 0; i < 25; i++) {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0xA0);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = lineWidth;
        ang += dang + ex->btlfx_randCentered(0x80);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), 0, 0, frame, 0x1000, fade, 1, (u8 *)bolt, 0);
}

INCLUDE_RODATA("asm/us/fx_134/nonmatchings/fx_134", fx_134_headerPtr);

static s32 fx_134_op80PoolTask(s32 mode, Fx134Orb *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx134Work *parent) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR toCenter;
    SVECTOR old;
    SVECTOR trailPos;
    SVECTOR pad;
    Pool6CEntry *emitter;
    s32 trailAlpha;
    s32 phase;
    s32 c;
    s32 i;
    s32 sh;

    if (mode == 1) {
        it->phase2 = 0;
        it->rot14.vx = ex->btlfx_rand();
        it->rot14.vy = ex->btlfx_rand();
        it->rot14.vz = 0;
        it->spin1C = ex->btlfx_rand() % 64;
        it->spin1E = ex->btlfx_rand() % 32;
        it->emit20 = NULL;
    } else {
        switch (it->kind0) {
        case 0:
        case 1:
            trailAlpha = 0;
            switch (it->phase2) {
            case 0:
                it->rot14.vx += it->spin1C;
                it->rot14.vy += it->spin1E;
                old.vx = it->posC.vx;
                old.vy = it->posC.vy;
                old.vz = it->posC.vz;
                it->posC.vx += it->vel4.vx;
                it->posC.vy += it->vel4.vy;
                it->posC.vz += it->vel4.vz;
                pos.vx = it->posC.vx;
                pos.vy = it->posC.vy;
                pos.vz = it->posC.vz;
                scale.vx = scale.vy = scale.vz = fx_134_orbScales[it->vel4.pad];
                ex->Hi_SetEffModelOffset(parent->model5C, 0x40);
                ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, parent->model5C);
                toCenter.vx = parent->pos8.vx - it->posC.vx;
                toCenter.vy = parent->pos8.vy - it->posC.vy;
                toCenter.vz = parent->pos8.vz - it->posC.vz;
                if (frame < 8) {
                    trailAlpha = (frame << 12) >> 3;
                } else {
                    trailAlpha = 0x1000;
                }
                if (toCenter.vx * toCenter.vx + toCenter.vy * toCenter.vy + toCenter.vz * toCenter.vz < fx_134_catchRadius * fx_134_catchRadius * fx_134_catchRadius) {
                    it->phase2 = 1;
                    ((s16 *)it)[-1] = -1;
                    it->emit20 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&it->posC);
                    if (emitter != NULL) {
                        emitter->otZ = 0x20;
                    }
                }
                break;
            case 1:
                scale.vx = scale.vy = scale.vz = fx_134_orbScales[it->vel4.pad];
                pos.vx = it->posC.vx;
                pos.vy = it->posC.vy;
                pos.vz = it->posC.vz;
                ex->Hi_SetEffModelOffset(parent->model5C, 0x40);
                ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, parent->model5C);
                ex->btlfx_aimAngles((s16 *)&parent->pos8, (s16 *)&it->posC, (s16 *)&ang);
                ang.pad = 1;
                if (it->emit20 != NULL) {
                    it->emit20->rot.vx = ang.vx;
                    it->emit20->rot.vy = ang.vy;
                    it->emit20->rot.vz = ang.vz;
                }
                if (frame < 4) {
                    trailAlpha = 0x1000 - ((frame << 12) >> 2);
                }
                it->vel4.vx = it->vel4.vx * 2 / 3;
                it->vel4.vy = it->vel4.vy * 2 / 3;
                it->vel4.vz = it->vel4.vz * 2 / 3;
                env->otZ = 0x1F4;
                if (frame < 8) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &it->posC, NULL, 0x3000, frame, 1,
                                      ex->math_cos(frame << 7));
                }
                env->otZ = 0x20;
                if (frame < 8) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &it->posC, &ang, 0x2000, frame, 1,
                                      ex->math_cos(frame << 7));
                }
                env->otZ = 0;
                if (fx_134_stopOrbs != 0) {
                    return 1;
                }
                break;
            }
            if (trailAlpha != 0) {
                env->otZ = 0x180;
                BTLFX_COPY_SVEC(&trailPos, &it->posC);
                trailPos.vx -= it->vel4.vx;
                trailPos.vy -= it->vel4.vy;
                trailPos.vz -= it->vel4.vz;
                for (i = 0; i < 12; i++) {
                    trailPos.vx -= it->vel4.vx;
                    trailPos.vy -= it->vel4.vy;
                    trailPos.vz -= it->vel4.vz;
                    fxm_drawSprite3D(&trailPos, NULL, 0x600, trailAlpha, 0x1A, 0x3D06, (i & 3) << 6, (i >> 2) << 6, 0x40, 0x40, 1);
                }
                env->otZ = 0;
            }
            break;
        case 2:
            phase = (frame << 12) / 24;
            scale.vx = scale.vy = (ex->math_sin(phase >> 2) >> 1) + 0x800;
            scale.vz = it->vel4.pad + 0x200;
            it->rot14.vz -= 0x40;
            it->posC.vy -= 0x4D;
            pos.vx = it->posC.vx;
            pos.vy = it->posC.vy;
            pos.vz = it->posC.vz;
            ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, parent->model70);
            ex->Hi_ModifyEffModelRGB(parent->model70, (ex->math_sin(phase >> 1) >> 5) - 0x80, (ex->math_sin(phase >> 1) >> 5) - 0x80,
                                     (ex->math_sin(phase >> 1) >> 5) - 0x80);
            if (frame >= 24) {
                return 1;
            }
            break;
        case 3:
            it->rot14.vx += it->spin1C * 2;
            it->rot14.vy += it->spin1E * 2;
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
            it->rot14.vx += it->spin1C;
            it->rot14.vy += it->spin1E;
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

/* padded: a zero word follows fx_134_op80PoolTask's jump table (op80's table is 8-aligned in the original object) */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

/* Matching notes (same family as fx_0e0 op80): scalar temporaries are separate variables per case (_cN) and per
 * re-assignment (_pN/_wN, or a short name), since a variable reused across cases or phases spans blocks and changes
 * the allocation. Case 3's first angle a3x = t >> 2 (t = (frame << 12) / 24) is block-local: with it the case-3
 * block has eight call-crossing locals, its 0x1000 constant lands in s7 and is kicked out by work (work's
 * REG_LIVE_LENGTH is doubled for its REG_EQUIV, so ex is allocated first and gets fp, work then takes s7). The
 * outer switch sits in its own block so case 3/4's stack locals are freed before the tail's (which reuse 0x38).
 * Field copies are often written as three copies then the +=/-= adjustments (flow keeps both stores), the
 * statement orders were searched (build/s/permhc.py, bperm.py), and cases 6, 7, 8 and 10 follow fx_0e0's matched
 * forms. */
s32 fx_134_op80(s32 mode, Fx134Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    s32 ringCount;
    s32 boltAng6;
    s32 boltFade6;
    s32 spriteX;
    s32 glowAng;
    s32 sparkAlpha;
    s32 orbSpeed;
    s32 columnAng;
    s32 columnSin;
    s32 column3;
    s32 scrollX;
    s32 pulseAng5;
    s32 bulgeAng;
    s32 boltAng;
    s32 boltNum;
    Fx134Orb *orb;
    Fx134Orb *riser;
    Fx134Orb *debris;
    s32 swayAng;
    s32 remain;
    s32 bobAng;
    s32 spriteY;
    s32 spriteAlpha;
    s32 pulseAng;
    s32 spawnAng;
    s32 orbSize;
    s32 spawnRadius;
    s32 ringScale;
    s32 ringBright;
    s32 ring2;
    s32 pulseAng3;
    s32 swayCos;
    s32 ring3;
    s32 ramp;
    s32 gridAng;
    s32 flareFrame;
    s32 flareT;
    s32 spark;
    s32 cycle;
    s32 glowAng5;
    s32 dim;
    s32 ring5;
    s32 boltFade;
    s32 flareT6;
    s32 ringAng6;
    s32 boltNum6;
    s32 flareAng6;
    s32 swayAng6;
    s32 pulseAng7;
    s32 flash;
    s32 spawnNum;
    s32 tint;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;
    s32 gridBright;
    s32 gridScale;
    s32 gridX;
    s32 gridY;

    if (mode == 0) {
        init->workSize = 0x84;
        init->elemSize = 0x24;
        init->elemFn = (s32)fx_134_op80PoolTask;
        init->poolCount = 0x31;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_134_env = env;
        ex->btlfx_initEffModels(fx_134_effModels, 8);
        work->model5C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 8), 0x98, 0x3D80, 0, 0);
        work->model60 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xB), 0x118, 0xFFFF, 0x40,
                                                 0);
        work->model64 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x14), 0x118, 0xFFFF,
                                                 0x40, 0);
        work->model68 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x11), 0x37, 0x3D07, 0,
                                                 0);
        work->model6C = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x13));
        work->model70 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x12), 0x37, 0x3D07, 0,
                                                 0);
        work->model74 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x19), 0xBC, 0x3E00, 0x80,
                                                 0);
        work->pos8.vx = fx_134_origin.vx;
        work->pos8.vy = fx_134_origin.vy - 0x3000;
        work->pos8.vz = fx_134_origin.vz;
        work->emit7C = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->trails = (s32)fx_134_op80Trails;
            work->emit7C->trailSlots = 0x40;
            work->emit7C->trailStride = 0xCC;
        }
    } else {
        gridBright = 0;
        gridScale = 0;
        gridX = 0;
        gridY = 0;
        frame = entry->frame;
        {
        switch (work->state0) {
        case 0:
            switch (frame) {
            default: {
                SVECTOR emitRot;

                t = (frame << 12) / 40;
                if (frame < 0x10) {
                    BTLFX_COPY_SVEC(&work->pos18, &fx_134_origin);
                    bobAng = frame << 6;
                    work->pos18.vy -= ex->math_sin(bobAng) * 3;
                    env->camera->points[3].vx = work->pos18.vx;
                    env->camera->points[3].vy = work->pos18.vy;
                    env->camera->points[3].vz = work->pos18.vz;
                    spriteY = (ex->math_sin(bobAng) >> 3) - 0x200;
                } else {
                    spriteY = 0;
                }
                emitRot.vx = ex->btlfx_randCentered(0x44C) + 0x400;
                emitRot.vy = 0;
                emitRot.vz = 0;
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = emitRot.vx;
                    work->emit7C->rot.vy = emitRot.vy;
                    work->emit7C->rot.vz = emitRot.vz;
                }
                if (frame < 8) {
                    spriteAlpha = 0;
                } else if (frame < 0xC) {
                    spriteAlpha = (frame - 8) << 10;
                } else {
                    spriteAlpha = 0x1000;
                }
                if (spriteAlpha != 0) {
                    ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 3), spriteX = (t >> 7) + 0xA0, spriteY, 0, 1, spriteAlpha);
                    spriteAlpha += ex->btlfx_randCentered(0x18);
                    if (spriteAlpha < 0) {
                        spriteAlpha = 0;
                    }
                    ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 4), spriteX, spriteY, 0, 1, spriteAlpha);
                    ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 0x1F), spriteX, spriteY + 0x60, 0, 1, spriteAlpha);
                }
                if (frame == 0x18) {
                    work->emit80 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos8);
                }
                if (frame >= 8) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->pos8, NULL, 0x1800, frame, 2, -1);
                }
                if (frame >= 0x28) {
                    work->state0 = 1;
                    entry->frame = -1;
                    ex->btlfx_stopEmitter(work->emit7C);
                    ex->btlfx_stopEmitter(work->emit80);
                    BTLFX_COPY_SVEC(&work->pos8, &fx_134_origin);
                    work->scale30.vx = 0x1800;
                    work->scale30.vy = 0x1800;
                    work->scale30.vz = 0x1800;
                    work->rot3C.vx = 0x3C0;
                    work->pos8.vy -= 0x3000;
                    work->rot3C.vy = 0x100;
                    work->rot3C.vz = 0x180;
                    work->pos24.vx = work->pos8.vx;
                    work->pos24.vy = work->pos8.vy;
                    work->pos24.vz = work->pos8.vz;
                    ex->btlfx_startScreenFade(1, 1);
                }
                break;
            }
            }
            break;
        case 1:
            switch (frame) {
            default: {
                SVECTOR ang;

                t = frame << 6;
                BTLFX_COPY_SVEC(&work->pos18, &work->pos8);
                remain = 0x1000 - t;
                work->pos18.vy += 0x400 + (remain >> 1);
                gridScale = remain >> 4;
                env->camera->points[3].vx = work->pos18.vx;
                env->camera->points[3].vy = work->pos18.vy;
                env->camera->points[3].vz = work->pos18.vz;
                ex->Hi_DrawEffModel(&work->rot3C, (VECTOR *)&work->pos24, (s32)&work->scale30, work->model60);
                pulseAng = t >> 2;
                ex->Hi_ModifyEffModelRGB(work->model60, (((ex->math_sin(pulseAng) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_sin(pulseAng) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_sin(pulseAng) >> 1) + 0x800) >> 5) - 0x80);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xC), work->frame4);
                ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 3), (t >> 6) + 0xA0, (t >> 9) + 0x78,
                                  0, 1, -1);
                fxm_drawMirrorSprite(0xFFD, (t >> 5) + 0x78, (t >> 8) + 0xA9, 0x100, 0x40, 0x80, 0x98, 0x3DC0, 0, 0xC0, 0xFF);
                ex->btlfx_drawRippleGrid((s32)(env->ot + 8), (t >> 8) + 0x78, 1, 2, 0x78, 1, fx_134_rippleColors0, 0, 0, 0, 0);
                gridBright = 0x93;
                gridY = 0;
                gridX = 0;
                ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 0x18), (frame << 6) / 40 + 0xBE, 0x73, 0,
                                  0xFF, -1);
                ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 0x20), (t >> 6) + 0x20, 0x40, 0, 0xFF, -1);
                spawnRadius = (frame << 7) + 0x2000;
                orb = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (orb != NULL) {
                    orbSize = frame & 3;
                    spawnAng = frame * 0x140 + (orbSize << 5) - 0x20;
                    spawnAng = spawnAng % 3072 + 0x600;
                    orbSpeed = fx_134_orbSpeeds[orbSize] + 0x300;
                    orb->posC.vx = work->pos8.vx + ((ex->math_cos(spawnAng) * spawnRadius) >> 12);
                    orb->posC.vz = work->pos8.vz + ((ex->math_sin(spawnAng) * spawnRadius) >> 12);
                    orb->posC.vy = work->pos8.vy + ((((frame + 4) % 6) << 11) - 0x1400);
                    ex->btlfx_aimAngles((s16 *)&orb->posC, (s16 *)&work->pos8, (s16 *)&ang);
                    ex->btlfx_rotateOffset(&ang, orbSpeed, (s32)&orb->vel4);
                    orb->kind0 = frame & 1;
                    orb->vel4.pad = orbSize;
                }
                fx_134_catchRadius = (t >> 8) + 0x116;
                if (frame == 0x3E) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x46, 0x46, 0x46);
                }
                if (frame == 0x3F) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x80, 0x80, 0x80);
                }
                if (frame >= 0x40) {
                    work->state0 = 2;
                    entry->frame = -1;
                    fx_134_stopOrbs = 1;
                    ex->btlfx_initParticleSystem();
                    work->rot3C.vx = 0x400;
                    work->rot3C.vy = 0;
                    work->rot3C.vz = 0;
                    ex->btlfx_startScreenFade(1, 1);
                }
                break;
            }
            }
            break;
        case 2:
            switch (frame) {
            default: {
                SVECTOR rot;
                Fx134Vec3 pos;
                Fx134Vec3 scale;

                t = (frame << 12) / 6;
                if (frame == 0) {
                    ex->btlfx_poolClear(entry->pool);
                }
                gridBright = 0x97;
                gridScale = 0x10;
                gridY = 0;
                gridX = 0;
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xC0, 0xC0, 0xC0);
                }
                if (frame == 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x60, 0x60, 0x60);
                }
                BTLFX_COPY_SVEC(&work->pos18, &work->pos8);

                env->camera->points[3].vx = work->pos18.vx;
                env->camera->points[3].vy = work->pos18.vy;
                env->camera->points[3].vz = work->pos18.vz;
                work->rot3C.vy += 4;
                ex->Hi_DrawEffModel(&work->rot3C, (VECTOR *)&work->pos24, (s32)&work->scale30, work->model64);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xC), work->frame4);
                ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 3), 0x120, 0x58, 0, 1, -1);
                ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 4), 0x78, 0xA0, 0, 1, -1);
                ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 0x1F), 0x78, 0x46, 0, 1, -1);
                ringScale = (ex->math_sin(t >> 2) >> 3) + 0x1300;
                scale.vz = 0x1000;
                ringBright = (((t >> 1) + 0x800) >> 5) - 0x80;
                pos.vx = work->pos24.vx;
                pos.vy = work->pos24.vy;
                pos.vz = work->pos24.vz;
                pos.vy += 0xC00;
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = frame << 5;
                for (ring2 = 0; ring2 < 4; ring2++) {
                    scale.vx = scale.vy = (ringScale * ((ex->math_sin((ring2 << 11) / 3) >> 1) + 0x800)) >> 12;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model70);
                    ex->Hi_ModifyEffModelRGB(work->model70, ringBright, ringBright, ringBright);
                    rot.vz += 0x400;
                    pos.vy -= 0x800;
                }
                if (frame >= 6) {
                    work->state0 = 3;
                    entry->frame = -1;
                    work->rot50.vx = 0x400;
                    work->scale44.vx = 0x3000;
                    work->scale44.vy = 0x3000;
                    work->rot50.vy = 0;
                    work->rot50.vz = 0;
                    work->scale44.vz = 0x1800;
                }
                break;
            }
            }
            break;
        case 3: {
            Fx134Vec3 pos;
            Fx134Vec3 scale;
            SVECTOR rot;
            Fx134Vec3 pos2;
            Fx134Vec3 scale2;
            SVECTOR rot2;

            t = (frame << 12) / 24;
            swayAng = t >> 2;
            swayCos = ex->math_cos(swayAng);
            BTLFX_COPY_SVEC(&work->pos18, &work->pos8);

            env->camera->points[3].vx = work->pos18.vx;
            env->camera->points[3].vy = work->pos18.vy;
            gridBright = 0x97;
            env->camera->points[3].vz = work->pos18.vz;
            gridScale = (swayCos >> 4) + 0x20;
            work->rot3C.vy += 4;
            ramp = t >> 7;
            work->rot3C.vz += ramp;
            gridY = 0;
            gridX = 0;
            work->pos24.vy += (((0x1000 - ex->math_cos(swayAng)) << 9) >> 12) + (ex->math_cos(swayAng) >> 6) * ((frame & 1) * 2 - 1);
            ex->Hi_DrawEffModel(&work->rot3C, (VECTOR *)&work->pos24, (s32)&work->scale30, work->model64);
            ex->Hi_ModifyEffModelRGB(work->model64, -0x2B, -0x2B, -0x2B);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x30), work->frame4);
            work->scale44.vx = work->scale44.vy = t + 0x2000;
            work->scale44.vz = 0x1800;
            work->rot50.vz += ex->math_sin(swayAng) >> 7;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
            ex->Hi_ModifyEffModelRGB(work->model68, (ex->math_sin(swayAng) >> 6) - 0x80, (ex->math_sin(swayAng) >> 6) - 0x80,
                                     (ex->math_sin(swayAng) >> 6) - 0x80);
            work->rot50.vz += 0x200;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
            ex->Hi_ModifyEffModelRGB(work->model68, (ex->math_sin(swayAng) >> 5) - 0x80, (ex->math_sin(swayAng) >> 5) - 0x80,
                                     (ex->math_sin(swayAng) >> 5) - 0x80);
            work->rot50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model6C);
            ex->Hi_ModifyEffModelRGB(work->model6C, ramp - 0x80, ramp - 0x80, ramp - 0x80);
            scale.vx = scale.vy = ex->math_sin(swayAng) * 2 + 0x400;
            scale.vz = 0x1000;
            pos.vx = work->pos24.vx;
            pos.vy = work->pos24.vy;
            pulseAng3 = t >> 1;
            pos.vz = work->pos24.vz;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = frame << 7;
            for (ring3 = 0; ring3 < 4; ring3++) {
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model70);
                ex->Hi_ModifyEffModelRGB(work->model70, (ex->math_sin(pulseAng3) >> 5) - 0x80, (ex->math_sin(pulseAng3) >> 5) - 0x80,
                                         (ex->math_sin(pulseAng3) >> 5) - 0x80);
                scale.vx = scale.vy = scale.vx * 2 / 3;
                rot.vz += 0x200;
                pos.vy += 0x40;
            }
            column3 = 0;
            columnSin = ex->math_sin(t >> 2);
            columnAng = t >> 2;
            scale2.vz = (t >> 1) + 0x1000;
            pos2.vx = work->pos24.vx;
            pos2.vy = work->pos24.vy;
            pos2.vz = work->pos24.vz;
            pos2.vy += 0xC00;
            rot2.vx = -0x400;
            rot2.vy = 0;
            rot2.vz = 0xC0 - (frame << 6);
            for (; column3 < 4; column3++) {
                scale2.vx = scale2.vy = ((columnSin * 3 / 2 + 0x1500) * ((ex->math_sin((column3 << 10) / 3) >> 1) + 0x800)) >> 12;
                ex->Hi_DrawEffModel(&rot2, (VECTOR *)&pos2, (s32)&scale2, work->model70);
                ex->Hi_ModifyEffModelRGB(work->model70, (ex->math_cos(columnAng) >> 5) - 0x80, (ex->math_cos(columnAng) >> 5) - 0x80,
                                         (ex->math_cos(columnAng) >> 5) - 0x80);
                rot2.vz += 0x400;
                pos2.vy -= 0x800;
            }
            ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 3), 0x120, 0x58, 0, 1, -1);
            ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 4), 0x78, 0xA0, 0, 1, -1);
            ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 0x1F), 0x78, 0x46, 0, 1, -1);
            if (frame >= 0x18) {
                work->state0 = 4;
                entry->frame = -1;
                work->pos24.vx = work->pos8.vx;
                work->pos24.vz = work->pos8.vz;
                work->pos24.vy = work->pos8.vy;
                work->scale30.vx = 0x36D;
                work->scale30.vy = 0x36D;
                work->scale30.vz = 0x36D;
                work->pos24.vy -= 0x800;
                work->pos10.vx = work->pos24.vx;
                work->pos10.vy = work->pos24.vy;
                work->pos10.vz = work->pos24.vz;
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos10);
                work->emit80 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->pos10);
            }
            break;
        }
        case 4: {
            SVECTOR rot;
            Fx134Vec3 scale;
            Fx134Vec3 pos;
            SVECTOR pos3;
            SVECTOR rot3;

            t = (frame << 12) / 40;
            if (frame > 0x10) {
                gridAng = ((frame - 0x10) << 10) / 24;
                gridBright = (ex->math_sin(gridAng) >> 6) + 0x8D;
                gridScale = gridAng + 0x10;
            } else {
                gridBright = 0x8D;
                gridScale = 0x10;
            }
            BTLFX_COPY_SVEC(&work->pos18, &work->pos8);

            env->camera->points[3].vx = work->pos18.vx;
            env->camera->points[3].vy = work->pos18.vy;
            env->camera->points[3].vz = work->pos18.vz;
            gridY = 0;
            work->rot3C.vz += 0x20;
            work->pos24.vy += 0x60 + ex->btlfx_randCentered(8);
            work->pos10.vx = work->pos24.vx;
            work->pos10.vy = work->pos24.vy;
            work->pos10.vz = work->pos24.vz;
            work->pos10.vy += 0x578;
            ex->Hi_DrawEffModel(&work->rot3C, (VECTOR *)&work->pos24, (s32)&work->scale30, work->model64);
            ex->Hi_ModifyEffModelRGB(work->model64, 0, 0, 0);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x30), work->frame4);
            work->scale44.vx = work->scale44.vy = 0x492;
            work->scale44.vz = 0xAAA;
            work->pos24.vy -= 0x578;
            work->rot50.vz += 0x18;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
            ex->Hi_ModifyEffModelRGB(work->model68, -0x40, -0x40, -0x40);
            work->rot50.vz += 0x200;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
            ex->Hi_ModifyEffModelRGB(work->model68, -0x40, -0x40, -0x40);
            work->scale44.vx = work->scale44.vy = 0x924;
            work->scale44.vz = 0x1000;
            work->rot50.vz -= 0x200;
            work->pos24.vy -= 0x60E;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model6C);
            glowAng = t >> 2;
            ex->Hi_ModifyEffModelRGB(work->model6C, (((ex->math_sin(glowAng) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_sin(glowAng) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_sin(glowAng) >> 1) + 0x800) >> 5) - 0x80);
            gridX = 0;
            work->pos24.vy += 0xB86;
            if (frame > 0x1C) {
                flareFrame = frame - 0x1C;
                flareT = (flareFrame << 12) / 12;
                pos.vx = work->pos24.vx;
                pos.vy = work->pos24.vy;
                pos.vz = work->pos24.vz;
                pos.vy += 0x6A4;
                rot.vx = 0;
                rot.vy = frame << 6;
                rot.vz = 0;
                scale.vx = scale.vz = ((ex->math_sin(flareT >> 2) * 7) >> 3) + 0x578;
                scale.vy = ex->math_sin(flareT >> 2) * 2 / 3 + 0x400;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model74);
                ex->Hi_ModifyEffModelRGB(work->model74, (ex->math_sin(flareT >> 1) >> 5) - 0x80, (ex->math_sin(flareT >> 1) >> 5) - 0x80,
                                         (ex->math_sin(flareT >> 1) >> 5) - 0x80);
            }
            env->otZ = 0x100;
            sparkAlpha = ex->math_sin(glowAng);
            pos3.vx = work->pos24.vx;
            pos3.vy = work->pos24.vy;
            pos3.vz = work->pos24.vz;
            rot3.pad = 0;
            rot3.vy = 0;
            rot3.vx = 0;
            rot3.vz = frame << 5;
            for (spark = 0; spark < 0x10; spark++) {
                pos3.vy -= 0x19F;
                ex->btlfx_rand();
                fxm_drawSprite3D(&pos3, &rot3, 0x600, sparkAlpha, 0x1A, 0x3D02, (spark & 3) << 6, (spark >> 2) << 6, 0x40, 0x40, 1);
            }
            env->otZ = 0;
            cycle = frame - (frame / 5) * 5;
            if (cycle == 2) {
                riser = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (riser != NULL) {
                    riser->posC.vx = work->pos24.vx;
                    riser->posC.vz = work->pos24.vz;
                    riser->posC.vy = work->pos24.vy + 0x400;
                    riser->rot14.vx = -0x400;
                    riser->rot14.vy = 0;
                    riser->rot14.vz = frame * 0x154;
                    riser->kind0 = cycle;
                    riser->vel4.pad = t;
                }
            }
            scrollX = t >> 6;
            ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 3), 0xA0 - scrollX, 0x58, 0, 1, -1);
            ex->btlfx_drawRippleGrid((s32)(env->ot + 0x3E8), 0x28, 1, 3, 0x64, 1, fx_134_rippleColors1, 0, 0, 0, 0);
            ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 0x18), 0xDC - t / 55, 0x78, 0, 1, -1);
            ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 0x20), 0x28 - scrollX, 0x28, 0, 1, -1);
            work->scroll58 += 4 + ((t * 10) >> 12);
            fxm_drawTiledStrip(0, work->scroll58 % 320, 0xA0 - (t >> 8), 0x80, 0x80, 0x80, 0x9C, 0x3E00, 0x80, 0, 1, 0);
            fxm_drawTiledStrip(0xFFF, -((work->scroll58 >> 1) % 160), (t >> 9) + 0x8C, 0x80, 0x80, 0x80, 0x9C,
                          0x3E00, 0x80, 0, 0, 1);
            if (frame >= 0x28) {
                work->state0 = 5;
                entry->frame = -1;
                ex->btlfx_poolClear(entry->pool);
                ex->btlfx_initParticleSystem();
                work->pos24.vy = work->pos8.vy;
                work->scale30.vx = 0x400;
                work->scale30.vy = 0x400;
                work->scale30.vz = 0x400;
                work->pos24.vx = work->pos8.vx;
                work->pos24.vz = work->pos8.vz;
                work->pos24.vy -= 0x280;
                ex->btlfx_startScreenFade(0, 1);
            }
            break;
        }
        case 5:
            switch (frame) {
            default: {
                Fx134Vec3 pos;
                Fx134Vec3 scale;
                SVECTOR rot;
                SVECTOR from;
                SVECTOR to;
                SVECTOR rot2;
                Fx134Bolt *bolt;

                t = (frame << 12) / 23;
                gridBright = (t >> 7) + 0x8D;
                gridScale = (t >> 4) + 0x20;
                ex->btlfx_drawScrollMove(frame * 0x14, 0x1C0, 0x180, 0x18, 0x60, 0x320, 0x100);
                BTLFX_COPY_SVEC(&work->pos18, &work->pos8);

                env->camera->points[3].vx = work->pos18.vx;
                env->camera->points[3].vy = work->pos18.vy;
                env->camera->points[3].vz = work->pos18.vz;
                work->rot3C.vz += 0x18;
                work->pos24.vy += 0x20;
                ex->Hi_DrawEffModel(&work->rot3C, (VECTOR *)&work->pos24, (s32)&work->scale30, work->model64);
                dim = (t >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model64, dim, dim, dim);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x31), work->frame4);
                work->scale44.vx = work->scale44.vy = 0x800;
                work->scale44.vz = 0xAAA;
                work->pos24.vy -= 0x578;
                work->rot50.vz += 0x18;
                ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
                ex->Hi_ModifyEffModelRGB(work->model68, dim, dim, dim);
                work->rot50.vz -= 0x155;
                ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
                ex->Hi_ModifyEffModelRGB(work->model68, dim, dim, dim);
                work->rot50.vz -= 0x155;
                ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
                ex->Hi_ModifyEffModelRGB(work->model68, dim, dim, dim);
                work->scale44.vx = work->scale44.vy = 0x6DB;
                work->scale44.vz = 0x1000;
                work->rot50.vz += 0x2AA;
                work->pos24.vy -= 0x578;
                ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model6C);
                glowAng5 = t >> 2;
                ex->Hi_ModifyEffModelRGB(work->model6C, (ex->math_sin(glowAng5) >> 5) - 0x80, (ex->math_sin(glowAng5) >> 5) - 0x80,
                                         (ex->math_sin(glowAng5) >> 5) - 0x80);
                work->pos24.vy += 0xAF0;
                gridY = 0;
                gridX = 0;
                scale.vx = scale.vy = ex->math_sin(glowAng5) * 2 / 3 + 0x100;
                scale.vz = 0x555;
                pos.vx = work->pos24.vx;
                pulseAng5 = t >> 1;
                pos.vy = work->pos24.vy;
                pos.vz = work->pos24.vz;
                pos.vy += 0x80;
                rot.vx = 0x400;
                rot.vy = 0;
                rot.vz = frame << 7;
                for (ring5 = 0; ring5 < 3; ring5++) {
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model70);
                    ex->Hi_ModifyEffModelRGB(work->model70, (ex->math_sin(pulseAng5) >> 5) - 0x80, (ex->math_sin(pulseAng5) >> 5) - 0x80,
                                             (ex->math_sin(pulseAng5) >> 5) - 0x80);
                    scale.vx = scale.vy = (scale.vx * 3) >> 2;
                    rot.vz += 0x300;
                    pos.vy += 0x20;
                }
                from.vx = work->pos24.vx;
                from.vy = work->pos24.vy;
                from.vz = work->pos24.vz;
                from.vy += 0x400;
                boltAng = ex->btlfx_rand();
                for (boltNum = 0, bolt = fx_134_bolts; boltNum < 3; boltNum++, bolt++) {
                    boltFade = ex->btlfx_randCentered(0x400) + 0xC00;
                    BTLFX_COPY_SVEC(&to, &work->pos8);
                    to.vx += ex->math_cos(boltAng) >> 1;
                    to.vz += ex->math_sin(boltAng) >> 1;
                    fxm_drawOffsetTrail(entry, bolt, &from, &to, 0x100, 0x200, boltAng, 0xC0, 0, boltFade, (ex->btlfx_rand() & 0xF) + 0xF);
                    boltAng += 0x554 + (ex->btlfx_rand() & 0xFF);
                }
                rot2.vx = -0x400;
                rot2.vy = 0;
                bulgeAng = t >> 2;
                rot2.vz = ex->math_sin(bulgeAng) >> 3;
                fxm_drawBulgeGrid24(&work->pos8, &rot2, (ex->math_sin(bulgeAng) >> 3) + 0x1000, 0x40, 0x17, 0x3D00, 0, 0x80, bulgeAng, 1);
                if (frame >= 0x17) {
                    work->state0 = 6;
                    entry->frame = -1;
                    fx_134_origin.vz += 0x400;
                    BTLFX_COPY_SVEC(&work->pos8, &fx_134_origin);
                    fx_134_origin.vz -= 0x400;
                    work->scale30.vx = 0x2800;
                    work->scale30.vy = 0x2800;
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
                    ex->btlfx_startScreenFade(1, 1);
                    work->rot3C.vz += 0x320;
                }
                break;
            }
            }
            break;
        case 6:
            switch (frame) {
            default: {
                Fx134Vec3 pos;
                Fx134Vec3 scale;
                SVECTOR rot;
                SVECTOR from;
                SVECTOR to;
                Fx134Bolt *bolt;

                t = (frame << 12) / 56;
                gridBright = ((t << 5) >> 12) + 0x8D;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, (t * 0x58) >> 12, 0, 0);
                swayAng6 = t >> 2;
                BTLFX_COPY_SVEC(&work->pos18, &work->pos8);
                work->pos18.vy -= 0x400 + (ex->math_sin(swayAng6) >> 1);
                gridScale = 0x80;
                gridY = 0;
                gridX = 0;
                env->camera->points[3].vx = work->pos18.vx;
                env->camera->points[3].vy = work->pos18.vy;
                env->camera->points[3].vz = work->pos18.vz;
                work->rot3C.vz += 8;
                work->pos24.vy += 0x9B;
                work->pos24.vz += 0x4D;
                ex->Hi_DrawEffModel(&work->rot3C, (VECTOR *)&work->pos24, (s32)&work->scale30, work->model64);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x31), work->frame4);
                work->scale44.vx = work->scale44.vy = 0x3800;
                work->scale44.vz = 0x2000;
                work->pos24.vy -= 0x578;
                work->pos24.vz += 0x2BC;
                work->rot50.vz += 0x14;
                ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
                ex->Hi_ModifyEffModelRGB(work->model68, (((ex->math_cos(swayAng6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(swayAng6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(swayAng6) >> 1) + 0x800) >> 5) - 0x80);
                work->rot50.vz -= 0x200;
                ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model68);
                ex->Hi_ModifyEffModelRGB(work->model68, (((ex->math_cos(swayAng6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(swayAng6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(swayAng6) >> 1) + 0x800) >> 5) - 0x80);
                work->scale44.vx = work->scale44.vy = 0x2000;
                work->rot50.vz += 0x200;
                work->pos24.vy += 0x578;
                work->pos24.vz -= 0x2BC;
                work->scale44.vz = 0x3000;
                work->pos24.vy -= 0x1388;
                work->pos24.vz -= 0x9C4;
                ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model6C);
                ex->Hi_ModifyEffModelRGB(work->model6C, -0x2B, -0x2B, -0x2B);
                work->pos24.vy += 0x1388;
                work->pos24.vz += 0x9C4;
                if (frame > 0x28) {
                    ringCount = 0;
                    flareT6 = (frame - 0x28) << 8;
                    scale.vx = scale.vy = (ex->math_sin(flareT6 >> 2) * 5) / 2 + 0x400;
                    ringAng6 = flareT6 >> 2;
                    scale.vz = flareT6 + 0x1800;
                    rot.vx = -0x400;
                    rot.vy = 0;
                    pos.vx = fx_134_origin.vx;
                    rot.vz = frame * 0x8D;
                    pos.vy = fx_134_origin.vy;
                    pos.vz = fx_134_origin.vz;
                    pos.vy -= 0x100;
                    do {
                        ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model70);
                        ex->Hi_ModifyEffModelRGB(work->model70, (ex->math_sin(ringAng6) >> 5) - 0x80, (ex->math_sin(ringAng6) >> 5) - 0x80,
                                                 (ex->math_sin(ringAng6) >> 5) - 0x80);
                        ringCount++;
                        scale.vx = scale.vy = (scale.vx * 3) >> 2;
                        rot.vz += 0x400;
                        pos.vy += 0x20;
                    } while (ringCount <= 0);
                    scale.vx = scale.vz = (ex->math_sin(flareT6 >> 2) * 5) / 2 + 0x1000;
                    scale.vy = ((flareT6 * 0x300) >> 12) + 0x800;
                    rot.vx = 0;
                    rot.vy = frame * 0x8D;
                    rot.vz = 0;
                    pos.vx = fx_134_origin.vx;
                    pos.vy = fx_134_origin.vy;
                    pos.vz = fx_134_origin.vz;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model74);
                    flareAng6 = flareT6 >> 1;
                    ex->Hi_ModifyEffModelRGB(work->model74, (ex->math_sin(flareAng6) >> 5) - 0x80, (ex->math_sin(flareAng6) >> 5) - 0x80,
                                             (ex->math_sin(flareAng6) >> 5) - 0x80);
                }
                from.vx = work->pos24.vx;
                from.vy = work->pos24.vy;
                from.vz = work->pos24.vz;
                from.vy += 0x1838;
                from.vz += 0xC1C;
                for (boltNum6 = 0; boltNum6 < 2; boltNum6++) {
                    boltAng6 = ex->btlfx_rand();
                    boltFade6 = ex->btlfx_randCentered(0x400) + 0xC00;
                    ((s32 *)&to)[0] = work->pos24.vx;
                    ((s32 *)&to)[1] = work->pos24.vy;
                    ex->btlfx_offsetPoint(&from, (s32)&to, 0x1000, (XY32 *)&work->rot3C, 0x3000, boltAng6);
                    fxm_drawOffsetTrail(entry, &fx_134_bolts[boltNum6], &from, &to, 0x100, 0x200, boltAng6, 0xC0, 0, boltFade6, (ex->btlfx_rand() & 0x1F) + 0x58);
                }
                if (frame >= 0x38) {
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
            }
            }
            break;
        case 7:
            t = frame << 8;
            gridBright = 0xAD;
            gridY = 0;
            gridX = 0;
            pulseAng7 = t >> 2;
            gridScale = pulseAng7 + 0x80;
            fx_134_rippleColors2[1].r = (ex->math_sin(pulseAng7) * 0xFF) >> 12;
            fx_134_rippleColors2[1].g = (ex->math_sin(pulseAng7) * 0xFF) >> 12;
            fx_134_rippleColors2[1].b = (u32)ex->math_sin(pulseAng7) >> 6;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x58, 0, 0);
            ex->btlfx_drawRippleGrid((s32)(env->ot + 0xFF0), 0x28, 2, 2, 0xC8, 0xFF, fx_134_rippleColors2, 0, 0, 0, 0);
            if (frame > 8) {
                flash = (frame - 8) * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, flash, flash, flash);
            }
            work->pos24.vy = work->pos8.vy + (t >> 6) + ((((frame & 1) * 2 - 1) * ex->math_cos(t >> 2)) >> 6);
            ex->Hi_DrawEffModel(&work->rot3C, (VECTOR *)&work->pos24, (s32)&work->scale30, work->model64);
            ex->Hi_ModifyEffModelRGB(work->model64, ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x31), work->frame4);
            work->scale44.vx = work->scale44.vy = 0x3800;
            work->scale44.vz = 0x2000;
            work->pos24.vy -= 0x578;
            work->pos24.vz += 0x2BC;
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
            work->pos24.vy += 0x578;
            work->pos24.vz -= 0x2BC;
            work->scale44.vz = 0x3000;
            work->pos24.vy -= 0x1388;
            work->pos24.vz -= 0x9C4;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos24, (s32)&work->scale44, work->model6C);
            ex->Hi_ModifyEffModelRGB(work->model6C, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                     (ex->math_cos(t >> 2) >> 5) - 0x80);
            work->pos24.vy += 0x1388;
            work->pos24.vz += 0x9C4;
            env->otZ = 0x898;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), &fx_134_origin, NULL, t * 3 + 0x1000, frame, 1,
                              ex->math_sin(t >> 2));
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1B), &fx_134_origin, NULL, t * 2 + 0x4000, frame, 1,
                              ex->math_sin(t >> 2));
            env->otZ = 0;
            if (frame >= 0x10) {
                work->state0 = 8;
                entry->frame = -1;
                work->pos10.vx = work->pos18.vx;
                work->pos10.vy = work->pos18.vy;
                work->pos10.vz = work->pos18.vz;
                work->pos10.vx -= 0x2800;
                work->pos10.vy += 0xB86;
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->pos10);
            }
            break;
        case 8:
            t = frame << 7;
            gridBright = 0xBC;
            gridScale = 0x200;
            gridY = 0;
            gridX = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 0xFF8), 0xFF, 0xFF, 0xFF, 0xC8);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x40, 0, 0);
            if (frame >= 0x1A) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, ((frame - 0x1A) << 7) / 6, ((frame - 0x1A) << 7) / 6,
                                         ((frame - 0x1A) << 7) / 6);
            }
            if (frame >= 0xA) {
                for (spawnNum = 0; spawnNum < 3; spawnNum++) {
                    debris = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (debris != NULL) {
                        debris->posC.vx = work->pos18.vx - ((t >> 4) + 0x1800) + ex->btlfx_randCentered(0x200);
                        debris->posC.vy = 0x6A4 - ((0x1000 - t) >> 1);
                        debris->posC.vz = work->pos18.vz + ex->btlfx_randCentered(0x1C00);
                        debris->vel4.vx = ex->btlfx_randCentered(0x100);
                        debris->vel4.vy = -0x140 - (ex->btlfx_rand() & 0xFF);
                        debris->vel4.vz = ex->btlfx_randCentered(0x100);
                        debris->rot14.vx = ex->btlfx_rand();
                        debris->rot14.vy = ex->btlfx_rand();
                        debris->rot14.vz = ex->btlfx_rand();
                        debris->kind0 = 3;
                        debris->vel4.pad = t / 5 + (s16)((ex->btlfx_rand() & 0x3FF) + 0x80);
                    }
                }
            }
            if (frame == 1) {
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->pos10);
            }
            if (frame == 4) {
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->pos10);
            }
            if (frame >= 0x20) {
                work->state0 = 0xA;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF, 0xFF, 0xFF);
                ex->btlfx_poolClear(entry->pool);
                ex->btlfx_initParticleSystem();
                work->pos18.vx = 0;
                work->pos18.vy = -0x200;
                work->pos18.vz = 0;
                env->camera->points[3].vx = work->pos18.vx;
                env->camera->points[3].vy = work->pos18.vy;
                env->camera->points[3].vz = work->pos18.vz;
            }
            break;
        case 10:
            switch (frame) {
            default: {
                CVECTOR col;
                SVECTOR rot;
                Fx134Vec3 scale;

                t = frame << 7;
                tint = 0xF5 - ((t * 0xF5) >> 12);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint, tint);
                rot.vx = 0;
                rot.vy = 0;
                tint = 0xA0 - ((frame << 14) >> 12);
                rot.vz = 0x20;
                scale.vx = 0x1100;
                scale.vy = 0x1100;
                scale.vz = 0x1000;
                col.r = col.g = col.b = tint;
                ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, (VECTOR *)&scale, &col, 1, NULL);
                if (frame >= 0x20) {
                    ex->btlfx_clearEffModels();
                    return 1;
                }
                break;
            }
            }
            break;
        }
        }
        if (gridBright != 0) {
            CVECTOR col;
            SVECTOR rot;
            Fx134Vec3 scale;

            col.r = col.g = col.b = gridBright;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            scale.vy = scale.vx = gridScale + 0x1000;
            scale.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, gridX + 0xA0, gridY + 0x78, 0x50, 0x3C, 4, 4, 2, 2, &rot, (VECTOR *)&scale, &col, 0, NULL);
        }
        if ((u32)(work->state0 - 3) < 5) {
            ex->btlfx_drawScrollMove((0x1F4 - work->frame4) * 8, 0x1E0, 0x100, 0x20, 0x80, 0x300, 0x100);
        }
        work->frame4++;
    }
    return 0;
}
