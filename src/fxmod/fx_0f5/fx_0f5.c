#include "fxmod/fx_0f5.h"

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 bright, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 dr, s32 ang, s32 dang, s32 dseg, s32 abr, s32 flicker) {
    Unk800F78A4 *env = fx_0f5_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR colFlat;
    CVECTOR colLit;
    CVECTOR colDark;
    MATRIX m;
    SVECTOR quad[4];
    s32 otz;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 rad0;
    s32 rad1;
    s32 j;
    s32 ang1;
    s32 rin;
    s32 iNext;
    s32 z1;
    s32 z0;
    s32 i;
    s32 seg;
    s32 rout;
    s32 cx;
    s32 cy;
    s32 tileV;
    s32 tileU;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 ft;
    s32 sinv;

    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)env->prim;
    ft4 = (POLY_FT4 *)(gt4 + 2 * FX_STELLAR_CIRCLE_5_CYL_SEGMENTS);
    env->prim = (u8 *)(ft4 + (FX_STELLAR_CIRCLE_5_CYL_RINGS - 2) * FX_STELLAR_CIRCLE_5_CYL_SEGMENTS);
    if (flicker == 0) {
        colFlat.r = colFlat.g = colFlat.b = bright;
        colLit.r = colLit.g = colLit.b = bright;
    } else {
        fx_0f5_flickerPhase &= 0x1F;
        ex->btlfx_sampleGradient(fx_0f5_flickerGradient, &colFlat, fx_0f5_flickerPhase);
        colFlat.r = (colFlat.r * bright) >> 7;
        colFlat.g = (colFlat.g * bright) >> 7;
        colFlat.b = (colFlat.b * bright) >> 7;
        ex->btlfx_sampleGradient(fx_0f5_flickerGradient, &colLit, fx_0f5_flickerPhase);
        fx_0f5_flickerPhase++;
        colLit.r = (colLit.r * bright) >> 7;
        colLit.g = (colLit.g * bright) >> 7;
        colLit.b = (colLit.b * bright) >> 7;
    }
    *(s32 *)&colDark = 0;
    colFlat.cd = abr != BTLFX_ABR_OPAQUE ? GPU_POLY_FT4 | GPU_ABE : GPU_POLY_FT4;
    colDark.cd = abr != BTLFX_ABR_OPAQUE ? GPU_POLY_GT4 | GPU_ABE : GPU_POLY_GT4;
    tpage |= TPAGE_ABR(abr);
    colLit.cd = colDark.cd;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    seg = 0;
    i = 0;
    rout = dr;
    rin = 0;
    iNext = 1;
    z1 = FX_STELLAR_CIRCLE_5_CYL_RING_DEPTH;
    z0 = 0;
    for (; i < FX_STELLAR_CIRCLE_5_CYL_RINGS; i++, rin += dr, rout += dr, iNext++, z1 += FX_STELLAR_CIRCLE_5_CYL_RING_DEPTH, z0 += FX_STELLAR_CIRCLE_5_CYL_RING_DEPTH) {
        seg += dseg;
        ang += dang;
        rad0 = ex->btlfx_lerp(r0, r1, i << 9);
        rad1 = ex->btlfx_lerp(r0, r1, iNext << 9);
        ang1 = ang + dang;
        for (j = 0; j < FX_STELLAR_CIRCLE_5_CYL_SEGMENTS; j++) {
            seg += dseg;
            cx = (ex->math_cos(ang1) * (rout / 8)) >> 12;
            sinv = ex->math_sin(ang1);
            /* The loop notes stop cse1 from reusing the division above; cse2 merges the second one later, which
             * leaves the original's dead reload of rin in the inner ring below. */
            do {
            } while (0);
            cy = (sinv * (rout / 8)) >> 12;
            quad[0].vx = cx + ((ex->math_cos(seg) * rad1) >> 12);
            quad[0].vy = cy + ((ex->math_sin(seg) * rad1) >> 12);
            quad[1].vx = cx + ((ex->math_cos(seg + 0x100) * rad1) >> 12);
            quad[1].vy = cy + ((ex->math_sin(seg + 0x100) * rad1) >> 12);
            quad[0].vz = quad[1].vz = z1;
            seg -= dseg;
            cx = (ex->math_cos(ang) * (rin / 8)) >> 12;
            sinv = ex->math_sin(ang);
            do {
            } while (0);
            cy = (sinv * (rin / 8)) >> 12;
            quad[2].vx = cx + ((ex->math_cos(seg) * rad0) >> 12);
            quad[2].vy = cy + ((ex->math_sin(seg) * rad0) >> 12);
            quad[3].vx = cx + ((ex->math_cos(seg + 0x100) * rad0) >> 12);
            quad[3].vy = cy + ((ex->math_sin(seg + 0x100) * rad0) >> 12);
            quad[2].vz = quad[3].vz = z0;
            gte_ldv3c(quad);
            gte_rtpt();
            tileV = i & 3;
            tileU = j & 3;
            u0 = u + (tileU << 5);
            v0 = v + (tileV << 5);
            u1 = u0 + FX_STELLAR_CIRCLE_5_CYL_TILE;
            if (tileU == 3) {
                u1 = u0 + FX_STELLAR_CIRCLE_5_CYL_TILE - 1;
            }
            v1 = v0 + FX_STELLAR_CIRCLE_5_CYL_TILE;
            if (tileV == 3) {
                v1 = v0 + FX_STELLAR_CIRCLE_5_CYL_TILE - 1;
            }
            gte_nclip();
            if (i == 0) {
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colLit;
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colDark;
            } else {
                ft = 1;
                if (i >= FX_STELLAR_CIRCLE_5_CYL_RINGS - 1) {
                    *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colLit;
                    ft = 0;
                    *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colDark;
                } else {
                    *(s32 *)&ft4->r0 = *(s32 *)&colFlat;
                }
            }
            gte_stopz(&otz);
            if (otz != 0) {
                if (ft) {
                    ft4->u0 = ft4->u2 = u0;
                    ft4->u1 = ft4->u3 = u1;
                    ft4->v2 = ft4->v3 = v0;
                    ft4->v0 = ft4->v1 = v1;
                    ft4->tpage = tpage;
                    ft4->clut = clut;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                } else {
                    gt4->u0 = gt4->u2 = u0;
                    gt4->u1 = gt4->u3 = u1;
                    gt4->v2 = gt4->v3 = v0;
                    gt4->v0 = gt4->v1 = v1;
                    gt4->tpage = tpage;
                    gt4->clut = clut;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                }
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&quad[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(PRIM_TAG(POLY_FT4), otz, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
                } else {
                    ex->btlfx_addPrim(PRIM_TAG(POLY_GT4), otz, gt4);
                    gte_stsxy(&gt4->x3);
                    gt4++;
                }
            }
            seg += 0x100;
        }
    }
    /* Dead test, removed after reload: one more reference ranks rout between i and rin in global-alloc (rout
     * gets $fp, rin is spilled). */
    if (rout != 0) {
        tileV = 0;
    }
}

s32 fx_0f5_op80(s32 mode, Fx0f5Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 frame;
    s32 t;
    s32 radius;
    s32 ang;
    s32 spread;
    s32 fadeRgb;
    s32 growRgb;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = sizeof(Fx0f5Work80);
    } else if (mode == 1) {
        fx_0f5_env = env;
        ex->btlfx_initEffModels(fx_0f5_effModels, 1);
        work->model20 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_STELLAR_CIRCLE_5_CLOUD_DOME), 1);
        work->state0 = FX_STELLAR_CIRCLE_5_STATE_OPEN;
        work->frame4 = 0;
        BTLFX_COPY_SVEC(&work->center8, &env->targets->centerX);
        work->rot18.vx = 0x400;
        work->rot18.vy = 0x20;
        work->rot18.vz = 0;
    } else {
        radius = (work->frame4 << 9) / FX_STELLAR_CIRCLE_5_FRAMES;
        frame = entry->frame;
        ang = 0x1000 - ex->math_cos((work->frame4 << 10) / FX_STELLAR_CIRCLE_5_FRAMES);
        work->pos10.vx = work->center8.vx + ((ex->math_cos(ang) * radius) >> 12);
        work->pos10.vz = work->center8.vz + ((ex->math_sin(ang) * radius) >> 12);
        work->pos10.vy = work->center8.vy;
        rot.vz = 0;
        rot.vx = 0;
        rot.vy = work->frame4 * 32;
        switch (work->state0) {
        case FX_STELLAR_CIRCLE_5_STATE_OPEN:
            t = (frame << 12) / FX_STELLAR_CIRCLE_5_OPEN_FRAMES;
            scale.vz = 0x2000;
            scale.vx = scale.vy = t;
            fxm_drawTexCylinder(&work->pos10, &work->rot18, &scale, 0xC8, FX_STELLAR_CIRCLE_5_TUBE_END_RADIUS, 0x80, FX_STELLAR_CIRCLE_5_TUBE_TPAGE, FX_STELLAR_CIRCLE_5_TUBE_CLUT, FX_STELLAR_CIRCLE_5_TUBE_U, FX_STELLAR_CIRCLE_5_TUBE_V, t >> 6, work->frame4 * 40, FX_STELLAR_CIRCLE_5_CYL_TWIST,
                          0xC8, BTLFX_ABR_ADD, 0);
            scale.vx = scale.vy = 0x2000;
            scale.vz = ex->math_sin(t >> 2) * 2;
            fxm_drawTexCylinder(&work->pos10, &work->rot18, &scale, (ex->math_cos(t >> 2) >> 4) + 0x1C8, FX_STELLAR_CIRCLE_5_TUBE_2_END_RADIUS, t >> 5, FX_STELLAR_CIRCLE_5_TUBE_2_TPAGE, FX_STELLAR_CIRCLE_5_TUBE_2_CLUT, FX_STELLAR_CIRCLE_5_TUBE_2_U, FX_STELLAR_CIRCLE_5_TUBE_2_V, t >> 6,
                          work->frame4 * 40, FX_STELLAR_CIRCLE_5_CYL_TWIST, 0x80, BTLFX_ABR_ADD, 1);
            scale.vx = scale.vz = t;
            scale.vy = 0x1000 - ex->math_cos(t >> 2);
            pos.vx = work->pos10.vx;
            pos.vy = work->pos10.vy;
            pos.vz = work->pos10.vz;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model20);
            growRgb = (t >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model20, growRgb, growRgb, growRgb);
            if (frame >= FX_STELLAR_CIRCLE_5_OPEN_FRAMES) {
                work->state0 = FX_STELLAR_CIRCLE_5_STATE_WIDEN;
                entry->frame = -1;
            }
            break;
        case FX_STELLAR_CIRCLE_5_STATE_WIDEN:
            t = (frame << 12) / FX_STELLAR_CIRCLE_5_WIDEN_FRAMES;
            scale.vz = 0x2000;
            scale.vx = scale.vy = t + 0x1000;
            fxm_drawTexCylinder(&work->pos10, &work->rot18, &scale, 0xC8, FX_STELLAR_CIRCLE_5_TUBE_END_RADIUS, 0x80, FX_STELLAR_CIRCLE_5_TUBE_TPAGE, FX_STELLAR_CIRCLE_5_TUBE_CLUT, FX_STELLAR_CIRCLE_5_TUBE_U, FX_STELLAR_CIRCLE_5_TUBE_V, (t >> 5) + 0x40, work->frame4 * 40, FX_STELLAR_CIRCLE_5_CYL_TWIST,
                          0xC8, BTLFX_ABR_ADD, 0);
            scale.vx = scale.vy = 0x2000;
            fxm_drawTexCylinder(&work->pos10, &work->rot18, &scale, 0x1C8 - (t >> 5), FX_STELLAR_CIRCLE_5_TUBE_2_END_RADIUS, 0x80, FX_STELLAR_CIRCLE_5_TUBE_2_TPAGE, FX_STELLAR_CIRCLE_5_TUBE_2_CLUT, FX_STELLAR_CIRCLE_5_TUBE_2_U, FX_STELLAR_CIRCLE_5_TUBE_2_V, (t >> 5) + 0x40,
                          work->frame4 * 40, FX_STELLAR_CIRCLE_5_CYL_TWIST, 0x80, BTLFX_ABR_ADD, 1);
            pos.vx = work->pos10.vx;
            pos.vy = work->pos10.vy;
            pos.vz = work->pos10.vz;
            scale.vy = scale.vx = scale.vz = 0x1000;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model20);
            if (frame >= FX_STELLAR_CIRCLE_5_WIDEN_FRAMES) {
                work->state0 = FX_STELLAR_CIRCLE_5_STATE_SWELL;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_STELLAR_CIRCLE_5_SMOKE), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x31;
                }
            }
            break;
        case FX_STELLAR_CIRCLE_5_STATE_SWELL:
            t = frame << 7;
            scale.vx = scale.vy = scale.vz = (t >> 1) + 0x2000;
            spread = (t >> 5) + 0xC0;
            fxm_drawTexCylinder(&work->pos10, &work->rot18, &scale, 0xC8 - (t >> 6), FX_STELLAR_CIRCLE_5_TUBE_END_RADIUS, 0x80, FX_STELLAR_CIRCLE_5_TUBE_TPAGE, FX_STELLAR_CIRCLE_5_TUBE_CLUT, FX_STELLAR_CIRCLE_5_TUBE_U, FX_STELLAR_CIRCLE_5_TUBE_V, spread,
                          work->frame4 * 40, FX_STELLAR_CIRCLE_5_CYL_TWIST, (t >> 5) + 0xA0, BTLFX_ABR_ADD, 0);
            scale.vx = scale.vy = scale.vz = (t >> 1) + 0x2000;
            fxm_drawTexCylinder(&work->pos10, &work->rot18, &scale, 0x148, FX_STELLAR_CIRCLE_5_TUBE_2_END_RADIUS, 0x80, FX_STELLAR_CIRCLE_5_TUBE_2_TPAGE, FX_STELLAR_CIRCLE_5_TUBE_2_CLUT, FX_STELLAR_CIRCLE_5_TUBE_2_U, FX_STELLAR_CIRCLE_5_TUBE_2_V, spread, work->frame4 * 40,
                          FX_STELLAR_CIRCLE_5_CYL_TWIST, 0x80, BTLFX_ABR_ADD, 1);
            pos.vx = work->pos10.vx;
            pos.vy = work->pos10.vy;
            pos.vz = work->pos10.vz;
            scale.vy = scale.vx = scale.vz = 0x1000;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model20);
            if (frame >= FX_STELLAR_CIRCLE_5_SWELL_FRAMES) {
                work->state0 = FX_STELLAR_CIRCLE_5_STATE_FADE;
                entry->frame = -1;
            }
            break;
        case FX_STELLAR_CIRCLE_5_STATE_FADE:
            scale.vx = scale.vy = scale.vz = 0x2800;
            t = frame << 8;
            fxm_drawTexCylinder(&work->pos10, &work->rot18, &scale, 0x88, FX_STELLAR_CIRCLE_5_TUBE_END_RADIUS, ex->math_cos(t >> 2) >> 5, FX_STELLAR_CIRCLE_5_TUBE_TPAGE, FX_STELLAR_CIRCLE_5_TUBE_CLUT, FX_STELLAR_CIRCLE_5_TUBE_U, FX_STELLAR_CIRCLE_5_TUBE_V, 0x140,
                          work->frame4 * 40, FX_STELLAR_CIRCLE_5_CYL_TWIST, 0x120, BTLFX_ABR_ADD, 0);
            scale.vx = scale.vy = scale.vz = 0x2800;
            fxm_drawTexCylinder(&work->pos10, &work->rot18, &scale, 0x148, FX_STELLAR_CIRCLE_5_TUBE_2_END_RADIUS, ex->math_cos(t >> 2) >> 5, FX_STELLAR_CIRCLE_5_TUBE_2_TPAGE, FX_STELLAR_CIRCLE_5_TUBE_2_CLUT, FX_STELLAR_CIRCLE_5_TUBE_2_U, FX_STELLAR_CIRCLE_5_TUBE_2_V, 0x140,
                          work->frame4 * 40, FX_STELLAR_CIRCLE_5_CYL_TWIST, 0x80, BTLFX_ABR_ADD, 1);
            scale.vx = scale.vz = 0x1000;
            scale.vy = ex->math_cos(t >> 2);
            pos.vx = work->pos10.vx;
            pos.vy = work->pos10.vy;
            pos.vz = work->pos10.vz;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model20);
            fadeRgb = ((0x1000 - t) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model20, fadeRgb, fadeRgb, fadeRgb);
            if (frame >= FX_STELLAR_CIRCLE_5_FADE_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->rot18.vz -= work->frame4 * 2 + 0x10;
        ex->btlfx_drawScrollMove(10000 - work->frame4 * 12, FX_STELLAR_CIRCLE_5_TUBE_VRAM_X, FX_STELLAR_CIRCLE_5_TUBE_VRAM_Y, FX_STELLAR_CIRCLE_5_TUBE_VRAM_W, FX_STELLAR_CIRCLE_5_TUBE_VRAM_H, FX_STELLAR_CIRCLE_5_TUBE_SRC_X, FX_STELLAR_CIRCLE_5_TUBE_SRC_Y);
        ex->btlfx_drawScrollMove(10000 - work->frame4 * 6, FX_STELLAR_CIRCLE_5_TUBE_2_VRAM_X, FX_STELLAR_CIRCLE_5_TUBE_2_VRAM_Y, FX_STELLAR_CIRCLE_5_TUBE_2_VRAM_W, FX_STELLAR_CIRCLE_5_TUBE_2_VRAM_H, FX_STELLAR_CIRCLE_5_TUBE_2_SRC_X, FX_STELLAR_CIRCLE_5_TUBE_2_SRC_Y);
        ex->btlfx_drawScrollMove(work->frame4 * 8, FX_STELLAR_CIRCLE_5_DOME_VRAM_X, FX_STELLAR_CIRCLE_5_DOME_VRAM_Y, FX_STELLAR_CIRCLE_5_DOME_VRAM_W, FX_STELLAR_CIRCLE_5_DOME_VRAM_H, FX_STELLAR_CIRCLE_5_DOME_SRC_X, FX_STELLAR_CIRCLE_5_DOME_SRC_Y);
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f5/nonmatchings/fx_0f5", fx_0f5_headerPtr);
