#include "fxmod/fx_0b2.h"

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0b2Trail *trail, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0b2_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    pt = trail->pts4;
    trail->lastSlot0 = trail->count2 = 0x18;
    trail->head1 = 0;
    do {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = ex->btlfx_randCentered(0xA0);
        i++;
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = lineWidth;
        ang += dang + ex->btlfx_randCentered(0x80);
        pt++;
    } while (i < 25);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 4), 0, NULL, frame, 0x1000, fade, 1, (u8 *)trail, 0);
}

void fx_0b2_drawFunnel(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 radius0, s32 radius1, s32 bright, s32 tpage, s32 clut, s32 texU,
                   s32 texV, s32 drift, s32 driftAng, s32 driftStep, s32 twist, s32 abr, s32 segs) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0b2_env);
    Fx0b2Rgbc c0;
    Fx0b2Rgbc c1;
    Fx0b2Rgbc c2;
    MATRIX m;
    SVECTOR v[4];
    s32 z;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 r0;
    s32 r1;
    s32 j;
    s32 row;
    s32 ang;
    s32 h0;
    s32 h1;
    s32 a;
    s32 x;
    s32 y;
    s32 d;
    s32 e;
    s32 ft;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 ju;
    s32 rv;

    gte_SetRotMatrix(fx_0b2_env->viewMatrix);
    gte_SetTransMatrix(fx_0b2_env->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)fx_0b2_env->prim;
    *(s32 *)&c2 = 0;
    ft4 = (POLY_FT4 *)((u8 *)gt4 + segs * 0x68);
    fx_0b2_env->prim = (u8 *)ft4 + segs * 0xC8;
    c0.r = c0.g = c0.b = bright;
    c1.r = c1.g = c1.b = bright;
    c0.code = abr != 0xFF ? 0x2E : 0x2C;
    c2.code = abr != 0xFF ? 0x3E : 0x3C;
    tpage |= (abr & 3) << 5;
    c1.code = c2.code;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    ang = 0;
    /* row * drift, (row + 1) * drift, row + 1 and the two heights are loop-reduced into five stack temporaries;
     * h0/h1 are set here (not at their use) so their lifetime makes them worth reducing. */
    for (row = 0; row < 7; row++) {
        h1 = row * 0x90;
        h0 = (row + 1) * 0x90;
        ang += twist;
        driftAng += driftStep;
        r0 = ex->btlfx_lerp(radius0, radius1, (row << 12) / 7);
        r1 = ex->btlfx_lerp(radius0, radius1, ((row + 1) << 12) / 7);
        for (j = 0; j < segs; j++) {
            ang += twist;
            x = (ex->math_cos(driftAng + driftStep) * ((row + 1) * drift / 7)) >> 12;
            y = (ex->math_sin(driftAng + driftStep) * ((row + 1) * drift / 7)) >> 12;
            v[0].vx = x + ((ex->math_cos(ang) * r1) >> 12);
            v[0].vy = y + ((ex->math_sin(ang) * r1) >> 12);
            d = 0x1000 / segs;
            a = ang + d;
            v[1].vx = x + ((ex->math_cos(a) * r1) >> 12);
            v[1].vy = y + ((ex->math_sin(a) * r1) >> 12);
            v[0].vz = v[1].vz = h0;
            ang -= twist;
            e = ang + d; /* a new variable keeps (plus ang d) in that operand order */
            x = (ex->math_cos(driftAng) * (row * drift / 7)) >> 12;
            y = (ex->math_sin(driftAng) * (row * drift / 7)) >> 12;
            v[2].vx = x + ((ex->math_cos(ang) * r0) >> 12);
            v[2].vy = y + ((ex->math_sin(ang) * r0) >> 12);
            v[3].vx = x + ((ex->math_cos(e) * r0) >> 12);
            v[3].vy = y + ((ex->math_sin(e) * r0) >> 12);
            v[2].vz = v[3].vz = h1;
            gte_ldv3c(v);
            gte_rtpt();
            /* j & 3 and row & 3 in variables: the original computes each once, across the first ternary's label */
            ju = j & 3;
            u0 = texU + (ju * 7 * 16 >> 2);
            rv = row & 3;
            v0 = texV + (rv * 7 * 16 >> 2);
            /* a default and an override, not a ternary: the order global-alloc sees puts v1 before u1 */
            u1 = u0 + 0x1C;
            if (ju == 3) {
                u1 = u0 + 0x1B;
            }
            v1 = v0 + 0x1C;
            if (rv == 3) {
                v1 = v0 + 0x1B;
            }
            gte_nclip();
            if (row == 0) {
                ft = 0;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c1;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c2;
            } else if (row >= 6) {
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c1;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c2;
            } else {
                ft = 1;
                *(s32 *)&ft4->r0 = *(s32 *)&c0;
            }
            gte_stopz(&z);
            if (z != 0) {
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
                gte_stszotz(&z);
                gte_ldv0(&v[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, z, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, z, gt4);
                    gte_stsxy(&gt4->x3);
                    gt4++;
                }
            }
            ang += 0x1000 / segs;
        }
    }
}

s32 fx_0b2_op80(s32 mode, Fx0b2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    s32 frame;
    s32 alpha;
    s32 r;
    s32 a;
    s32 i;
    s32 t; /* one variable for cases 1-3 (fp in the original) */
    s32 state;
    s32 one = 1; /* a variable, not a literal: the original reloads it for `mode == 1` and `return 1` */

    if (mode == 0) {
        *args = 0x20;
    } else if (mode == one) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0b2_env = env;
        ((s32 *)&work->center8)[0] = ((s32 *)&env->targets->centerX)[0];
        ((s32 *)&work->center8)[1] = ((s32 *)&env->targets->centerX)[1];
        work->rot18.vx = 0x400;
        work->rot18.vy = 0x20;
        work->rot18.vz = 0;
    } else {
        alpha = 0x1000;
        r = work->frame4 * 0x300 / 72;
        frame = entry->frame;
        a = alpha - ex->math_cos(work->frame4 * 1500 / 72);
        work->pos10.vx = work->center8.vx + ((ex->math_cos(a) * r) >> 12);
        work->pos10.vz = work->center8.vz + ((ex->math_sin(a) * r) >> 12);
        work->pos10.vy = work->center8.vy;
        state = work->state0;
        /* Dead stores only (flow deletes them, jump2 the branch): the labels end cse's block here, so case 3
         * computes 0x1000 - t from the constant instead of reusing alpha (which cse would know is 0x1000). */
        if (a) {
            r = 0;
            a = 0;
        } else {
            r = 1;
            a = 1;
        }
        switch (state) {
        case 0: {
            SVECTOR rot;
            s32 w;
            s32 t;

            scale.vz = 0x2000;
            alpha = (frame << 12) / 12;
            scale.vy = alpha;
            scale.vx = alpha;
            w = alpha >> 6;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, 0xC8, 0x180, 0x80, 0x11A, 0xFFFF, 0x10, 0x90, w,
                          work->frame4 * 0xC8, 0x190, 0xC8, 1, 0x10);
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = work->frame4 * 90;
            rot.pad = 1;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos10, &rot, 0x6000, 0, 1, alpha / 2, 0,
                              0);
            scale.vy = 0x2000;
            scale.vx = 0x2000;
            t = alpha >> 2;
            scale.vz = ex->math_sin(t) * 2;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, (ex->math_cos(t) >> 4) + 0x1C8, 0x280, alpha >> 5, 0x1A,
                          0x3D01, 0, 0, w, work->frame4 * 0xC8, 0x190, 0x80, 1, 8);
            scale.vx = scale.vy = scale.vx * 3 / 2;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, (ex->math_cos(t) >> 4) + 0x1C8, 0x280, w, 0x17, 0x3D03, 0,
                          0x70, w, work->frame4 * 0xC8, 0x190, 0x40, 1, 8);
            if (frame >= 12) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        }
        case 1: {
            SVECTOR rot;
            s32 y;

            scale.vz = 0x2000;
            t = (frame << 12) / 12;
            scale.vx = scale.vy = t + 0x1000;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, 0xC8, 0x180, 0x80, 0x11A, 0xFFFF, 0x10, 0x90, (t >> 5) + 0x40,
                          work->frame4 * 0xC8, 0x190, 0xC8, 1, 0x10);
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = work->frame4 * 90;
            rot.pad = 1;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos10, &rot, 0x6000, 0, 1,
                              t / 2 + 0x800, 0, 0);
            scale.vz = scale.vx = scale.vy = 0x2000;
            y = 0x1C8 - (t >> 5);
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, y, 0x280, 0x80, 0x1A, 0x3D01, 0, 0, (t >> 5) + 0x40, work->frame4 * 0xC8,
                          0x190, 0x80, 1, 8);
            scale.vx = scale.vy = scale.vx * 3 / 2;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, y, 0x280, 0x40, 0x17, 0x3D03, 0, 0x70, (t >> 5) + 0x40,
                          work->frame4 * 0xC8, 0x190, 0x40, 1, 8);
            if (frame >= 12) {
                Pool6CEntry *p;

                work->state0 = 2;
                entry->frame = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos10);
                if (p != NULL) {
                    p->otZ = 0x58;
                }
            }
            break;
        }
        case 2: {
            SVECTOR rot;
            s32 s;
            s32 w;

            t = frame << 7;
            s = (t >> 1) + 0x2000;
            scale.vx = scale.vy = scale.vz = s;
            w = (t >> 5) + 0xC0;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, 0xC8 - (t >> 6), 0x180, 0x80, 0x11A, 0xFFFF, 0x10, 0x90, w,
                          work->frame4 * 0xC8, 0x190, (t >> 5) + 0xA0, 1, 0x10);
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = work->frame4 * 90;
            rot.pad = 1;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos10, &rot, 0x6000, 0, 1, alpha, 0, 0);
            scale.vx = scale.vy = scale.vz = s;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, 0x148, 0x280, 0x80, 0x1A, 0x3D01, 0, 0, w,
                          work->frame4 * 0xC8, 0x190, 0x80, 1, 8);
            scale.vx = scale.vy = scale.vx * 3 / 2;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, 0x148, 0x280, 0x40, 0x17, 0x3D03, 0, 0x70, w,
                          work->frame4 * 0xC8, 0x190, 0x40, 1, 8);
            if (frame >= 0x20) {
                work->state0 = 3;
                entry->frame = -1;
            }
            break;
        }
        case 3: {
            SVECTOR rot;
            s32 u;

            scale.vx = scale.vy = scale.vz = 0x2800;
            t = frame << 8;
            u = t >> 2;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, 0x88, 0x180, ex->math_cos(u) >> 5, 0x11A, 0xFFFF, 0x10, 0x90,
                          0x140, work->frame4 * 0xC8, 0x190, 0x120, 1, 0x10);
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = work->frame4 * 90;
            rot.pad = 1;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos10, &rot, 0x6000, 0, 1, 0x1000 - t, 0,
                              0);
            scale.vx = scale.vy = scale.vz = 0x2800;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, 0x148, 0x280, ex->math_cos(u) >> 5, 0x1A, 0x3D01, 0, 0, 0x140,
                          work->frame4 * 0xC8, 0x190, 0x80, 1, 8);
            alpha -= t;
            scale.vx = scale.vy = scale.vx * 3 / 2;
            fx_0b2_drawFunnel(&work->pos10, &work->rot18, &scale, 0x148, 0x280, ex->math_cos(u) >> 6, 0x17, 0x3D03, 0, 0x70,
                          0x140, work->frame4 * 0xC8, 0x190, 0x40, 1, 8);
            if (frame >= 0x10) {
                return one;
            }
            break;
        }
        }
        work->rot18.vz += -0x10 - work->frame4 * 2;
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 2), work->frame4);
        ex->btlfx_drawScrollMove(10000 - work->frame4 * 8, 0x1C0, 0x170, 0x1C, 0x70, 0x1DC, 0x170);
        ex->btlfx_drawScrollMove(10000 - work->frame4 * 12, 0x280, 0x100, 0x1C, 0x70, 0x29C, 0x100);
        i = 0;
        if (alpha != 0) {
            SVECTOR from;
            SVECTOR to;
            Fx0b2Trail *p;
            s32 ang;
            s32 h;
            s32 d;
            s32 e;

            from.vx = work->pos10.vx;
            from.vy = work->pos10.vy;
            from.vz = work->pos10.vz;
            from.vy -= 0x640;
            ang = work->frame4 * 0x180 + (ex->btlfx_rand() & 0xFF);
            p = fx_0b2_trails;
            do {
                i++;
                h = ((ex->btlfx_rand() & 0xFFF) * alpha) >> 12;
                d = (ex->btlfx_rand() & 0x1FF) + 0x400;
                BTLFX_COPY_SVEC(&to, &work->pos10);
                to.vx += (ex->math_cos(ang) * d) >> 12;
                to.vz += (ex->math_sin(ang) * d) >> 12;
                e = ang + ex->btlfx_rand();
                fxm_drawOffsetTrail(entry, p, &from, &to, 0x100, 0x180, e, 0x100, 0, h, (ex->btlfx_rand() & 0xF) + 0xF);
                p++;
                ang += 0x300 + (ex->btlfx_rand() & 0x3FF);
            } while (i < 3);
        }
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b2/nonmatchings/fx_0b2", fx_0b2_headerPtr);
