#include "fxmod/fx_195.h"

void fx_195_buildOffsetTrail(Unk80100C78Entry *entry, Fx195Trail *trail, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_195_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    pt = trail->points4;
    trail->lastSlot0 = trail->count2 = 0xC;
    trail->head1 = 0;
    do {
        t = 0x1000 - (i << 12) / 12;
        t += ex->btlfx_randCentered(0x180);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = ex->btlfx_randCentered(0x80);
        i++;
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = 0x1A;
        ang += dang + ex->btlfx_randCentered(0x100);
        pt++;
    } while (i < 13);
}

static void fxm_drawOffsetTrail0(Unk80100C78Entry *entry, Fx195Trail57 *trail, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_195_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->points4;
    trail->lastSlot0 = trail->count2 = 0x38;
    trail->head1 = 0;
    do {
        t = 0x1000 - (i << 12) / 56;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0xA0);
        i++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)p, 0, (XY32 *)&rot, h, ang);
        p->pad = lineWidth;
        ang += dang + ex->btlfx_randCentered(0x80);
        p++;
    } while (i < 57);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x18), 0, NULL, frame, 0x1000, fade, 1, (u8 *)trail, 0);
}

static void fxm_drawOffsetTrail1(Unk80100C78Entry *entry, Fx195Trail *trail, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_195_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->points4;
    trail->lastSlot0 = trail->count2 = 0xC;
    trail->head1 = 0;
    do {
        t = 0x1000 - (i << 12) / 12;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0xA0);
        i++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)p, 0, (XY32 *)&rot, h, ang);
        p->pad = lineWidth;
        ang += dang + ex->btlfx_randCentered(0x80);
        p++;
    } while (i < 13);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x19), 0, NULL, frame, 0x1000, fade, 1, (u8 *)trail, 0);
}

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_195_env);
    Fx195Rgbc c0;
    Fx195Rgbc c1;
    Fx195Rgbc c2;
    MATRIX m;
    SVECTOR vv[4];
    s32 z;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 k;
    s32 j;
    s32 row;
    s32 h;
    s32 d;
    s32 y0;
    s32 y1;
    s32 s0;
    s32 s1;
    s32 ra;
    s32 rb;
    s32 a;
    s32 ft;
    s32 t;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 jn;
    s32 hd;
    s32 q;

    gte_SetRotMatrix(fx_195_env->viewMatrix);
    gte_SetTransMatrix(fx_195_env->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)fx_195_env->prim;
    *(s32 *)&c2 = 0;
    ft4 = (POLY_FT4 *)(gt4 + 2);
    fx_195_env->prim = (u8 *)(ft4 + 46);
    c0.r = c0.g = c0.b = c;
    c1.r = c1.g = c1.b = c;
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
    k = 0;
    h = 0;
    w0 >>= 1;
    w1 >>= 1;
    d = dy * 3;
    for (row = 0; row < 3; row++, h += dy) {
        for (j = 0; j < 16; j++, k++, ang += dang << 8) {
            jn = (j + 1) << 8;
            y0 = h + (((j << 8) * dy) >> 12);
            y1 = h + ((jn * dy) >> 12);
            /* one variable for d / 2 and the second quotient: the original recomputes d >> 1 every iteration */
            hd = d >> 1;
            q = ((y0 + hd) << 12) / d;
            hd = ((y1 + hd) << 12) / d;
            s0 = ex->math_sin(q >> 1);
            s1 = ex->math_sin(hd >> 1);
            ra = ex->btlfx_lerp(r0, r1, s0);
            rb = ex->btlfx_lerp(r0, r1, s1);
            vv[0].vx = vv[1].vx = (ex->math_cos(ang) * ra) >> 12;
            vv[0].vy = vv[1].vy = (ex->math_sin(ang) * ra) >> 12;
            vv[0].vz = y0 - ex->btlfx_lerp(w0, w1, s0);
            vv[1].vz = y0 + ex->btlfx_lerp(w0, w1, s1);
            a = ang + (dang << 8);
            vv[2].vx = vv[3].vx = (ex->math_cos(a) * rb) >> 12;
            vv[2].vy = vv[3].vy = (ex->math_sin(a) * rb) >> 12;
            vv[2].vz = y1 - ex->btlfx_lerp(w0, w1, s0);
            vv[3].vz = y1 + ex->btlfx_lerp(w0, w1, s1);
            gte_ldv3c(vv);
            gte_rtpt();
            t = k & 7;
            u0 = u + (t << 4);
            u1 = u0 + 0x10;
            if (t == 7) {
                u1 = u0 + 0xF;
            }
            v0 = v;
            v1 = v0 + 0x3F;
            gte_nclip();
            if (k == 0) {
                ft = 0;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c2;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c1;
            } else if (k >= 47) {
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c2;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c1;
            } else {
                ft = 1;
                *(s32 *)&ft4->r0 = *(s32 *)&c0;
            }
            gte_stopz(&z);
            if (z != 0) {
                if (ft) {
                    ft4->u0 = ft4->u1 = u0;
                    ft4->u2 = ft4->u3 = u1;
                    ft4->v0 = ft4->v2 = v0;
                    ft4->v1 = ft4->v3 = v1;
                    ft4->tpage = tpage;
                    ft4->clut = clut;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                } else {
                    gt4->u0 = gt4->u1 = u0;
                    gt4->u2 = gt4->u3 = u1;
                    gt4->v0 = gt4->v2 = v0;
                    gt4->v1 = gt4->v3 = v1;
                    gt4->tpage = tpage;
                    gt4->clut = clut;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                }
                gte_avsz3();
                gte_stszotz(&z);
                gte_ldv0(&vv[3]);
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
        }
    }
}

void fxm_rollTowardEye(VECTOR *pos, SVECTOR *rot) {
    Unk800F78A4 *env = fx_195_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 tmp;
    s32 a;

    v.vx = pos->vx;
    v.vy = pos->vy;
    v.vz = pos->vz;
    ex->btlfx_toLocalPolar(&env->camera->eye, &v, rot, (s32)&tmp, (s32)&tmp, &a);
    rot->vz = -a + 0x400;
}

s32 fx_195_op80(s32 mode, Fx195Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    CVECTOR col;
    SVECTOR rot2;
    VECTOR sc2;
    SVECTOR trailFrom;
    SVECTOR trailTo;
    SVECTOR flatRot;
    s32 frame;
    s32 t;
    s32 tint1;
    s32 tint2;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = 0x98;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_195_env = env;
        ex->btlfx_initEffModels(fx_195_effModels, 7);
        work->model80 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x11), 1);
        work->model84 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x14), 1);
        work->model5C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x1C), 0);
        work->model88 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x16), 1);
        work->model8C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x15), 0);
        work->model90 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x1A), 0);
        work->model94 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x13), 1);
        BTLFX_COPY_SVEC(&work->unk8, &fx_195_basePos);
        BTLFX_COPY_SVEC(&work->camPos20, &fx_195_basePos);
        work->camPos20.vy -= 0x980;
        work->pos10.vx = fx_195_basePos.vx;
        work->pos10.vy = fx_195_basePos.vy;
        work->pos10.vz = fx_195_basePos.vz;
        work->rot78.vx = -0x400;
        work->rot78.vy = 0;
        work->rot78.vz = 0;
    } else {
        tint1 = 0;
        tint2 = 0;
        frame = entry->frame;
        {
            /* Dead struct initializer: it only emits a CLOBBER of q, whose spill slot is the original's otherwise
             * unused word at 0x140 (it shifts the case 5 spill slots up by one). */
            struct {
                s32 x;
            } q = { frame };
        }
        switch (work->state0) {
        case 0:
            env->camera->points[3].vx = work->camPos20.vx;
            env->camera->points[3].vy = work->camPos20.vy;
            env->camera->points[3].vz = work->camPos20.vz;
            if (frame >= 2) {
                work->state0 = 4;
                entry->frame = -1;
                work->unk44 = 0;
            }
            break;
        case 4: {
            s32 ringFrame;
            s32 shrinkT;
            s32 flareSize;
            s32 ringAlpha;
            s32 haloSize;
            s32 ringSize;
            s32 haloAlpha;
            s32 shakeT;
            s32 burstT;
            s32 burstBright;
            s32 a;
            s32 i;
            s32 i_1;
            s32 ang;
            s32 amp;

            t = (frame << 12) / 14;
            tint2 = ex->math_cos(t >> 2);
            if (frame == 0) {
                work->pos10.vx = fx_195_basePos.vx;
                work->pos10.vy = fx_195_basePos.vy;
                work->pos10.vz = fx_195_basePos.vz;
                work->pos60.vx = fx_195_basePos.vx;
                work->pos60.vy = fx_195_basePos.vy;
                work->pos60.vz = fx_195_basePos.vz;
            }
            if (frame < 4) {
                tint1 = 0x1000 - ((frame << 12) >> 2);
                work->scale6C.vx = work->scale6C.vy = 0x1200;
                work->scale6C.vz = 0x1555;
            } else {
                shrinkT = frame - 4;
                if (frame == 4) {
                    work->scale6C.vx = work->scale6C.vy = 0x4000;
                } else if (shrinkT < 11) {
                    shrinkT = (shrinkT << 12) / 10;
                    flareSize = ((0x1000 - shrinkT) * 3) >> 1;
                    work->scale6C.vx = work->scale6C.vy = flareSize;
                }
            }
            if (frame < 5) {
                work->camPos20.vy = work->pos10.vy;
                work->camPos20.vx = work->pos10.vx;
                work->camPos20.vz = work->pos10.vz;
                work->camPos20.vy += -0x180 - ((0x1000 - ((frame << 12) >> 2)) >> 1);
                env->camera->points[3].vx = work->camPos20.vx;
                env->camera->points[3].vy = work->camPos20.vy;
                env->camera->points[3].vz = work->camPos20.vz;
            }
            if (frame == 4) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x20, 0x80, 0xFF);
            } else if ((u32)(frame - 5) < 7) {
                shakeT = 0x1000 - ((frame - 5) << 12) / 7;
                amp = shakeT >> 5;
                work->camPos20.vx = fx_195_basePos.vx + ex->btlfx_randCentered(amp);
                work->camPos20.vy = fx_195_basePos.vy + ex->btlfx_randCentered(amp);
                work->camPos20.vy = fx_195_basePos.vy - 0x200 + (shakeT >> 6) * ((frame & 1) * 2 - 1);
                env->camera->points[3].vx = work->camPos20.vx;
                env->camera->points[3].vy = work->camPos20.vy;
                env->camera->points[3].vz = work->camPos20.vz;
            }
            if (work->scale6C.vx != 0) {
                fxm_rollTowardEye((VECTOR *)&work->pos60, &work->rot78);
                ex->Hi_DrawEffModel(&work->rot78, (VECTOR *)&work->pos60, (s32)&work->scale6C, work->model5C);
            }
            if (frame >= 4) {
                burstT = ((frame - 4) << 12) / 10;
                rot.vx = 0;
                rot.vz = 0;
                rot.vy = burstT >> 3;
                sc.vx = sc.vz = (burstT >> 2) + 0x555;
                sc.vy = (burstT >> 1) + 0x400;
                burstBright = ex->math_cos(burstT >> 2);
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->pos60, (s32)&sc, work->model80);
                ex->Hi_ModifyEffModelRGB(work->model80, (burstBright >> 5) - 0x80, (burstBright >> 5) - 0x80, (burstBright >> 5) - 0x80);
                sc.vx = sc.vz = sc.vx * 2;
                sc.vy >>= 2;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->pos60, (s32)&sc, work->model80);
                ex->Hi_ModifyEffModelRGB(work->model80, (burstBright >> 6) - 0x80, (burstBright >> 6) - 0x80, (burstBright >> 6) - 0x80);
            }
            if (frame == 0) {
                work->emit54 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos10);
                work->emit58 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos10);
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos10);
            }
            col.r = col.g = col.b = 0x37;
            sc2.vx = 0x1400;
            sc2.vy = 0x1400;
            sc2.vz = 0x1000;
            rot2.vx = 0;
            rot2.vy = 0;
            rot2.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot2, &sc2, &col, 1, NULL);
            BTLFX_COPY_SVEC(&trailFrom, &work->pos10);
            ex->btlfx_pointAhead((s16 *)&trailFrom, (s16 *)&work->rot78, t - 0x2800, (s16 *)&trailTo);
            amp = (t >> 2) + 0x400;
            for (i = 0, ang = work->frame4 << 8; i < 3; i++, ang += 0x554) {
                fxm_drawOffsetTrail0(entry, &fx_195_longTrails[i], &trailFrom, &trailTo, amp, amp, ang, 0xC0, 0, ex->btlfx_randCentered(0x400) + 0xC00,
                              0x84);
            }
            env->otZ = 0x200;
            haloSize = (ex->math_sin(t >> 2) << 1) + 0x1000;
            haloAlpha = (ex->math_cos(t >> 2) >> 1) + 0x800;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->pos10, NULL, haloSize, 0, 1, haloAlpha);
            ringSize = ex->math_sin(t >> 2) * 3 + 0x1000;
            ringAlpha = ex->math_cos(t >> 2);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 8), &work->pos10, NULL, ringSize, 0, 1, ringAlpha);
            if (frame >= 4 && (ringFrame = frame - 4) < 10) {
                flatRot.vx = 0x400;
                flatRot.vy = 0;
                flatRot.vz = 0;
                flatRot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &work->pos10, &flatRot, 0x4000, ringFrame, 1, 0x1000);
            }
            env->otZ = 0;
            if (frame >= 14) {
                work->state0 = 5;
                entry->frame = -1;
                ex->btlfx_stopEmitter(work->emit54);
                ex->btlfx_stopEmitter(work->emit58);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x80, 0xFF);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x300;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x300;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x400;
                }
                work->sound30 = ex->btlfx_playFileSound(0x22, entry->file, -1);
                work->sound32 = ex->btlfx_playFileSound(0x23, entry->file, -1);
                ex->btlfx_playFileSound(0x20, entry->file, -1);
                ex->btlfx_playFileSound(0x21, entry->file, -1);
                env->camera->points[2].vx = 0;
                env->camera->points[2].vy = -0xC8;
                env->camera->points[2].vz = 0x300;
            }
            break;
        }
        case 5: {
            VECTOR sc;
            SVECTOR rotA;
            VECTOR scA;
            VECTOR posA;
            SVECTOR rotC;
            VECTOR scC;
            VECTOR posC;
            SVECTOR rotF;
            VECTOR scF;
            VECTOR posF;
            SVECTOR trailFrom;
            SVECTOR trailTo;
            SVECTOR rotG;
            s32 level;
            s32 flashTint;
            s32 columnBright;
            s32 burstBright;
            s32 grow;
            s32 cylRadius;
            s32 sizeMax;
            s32 size;
            s32 amp;
            s32 ringAng;
            s32 fadeT;
            s32 cylAng;
            s32 burstAng;
            s32 cylLen;
            s32 cylWidth;
            s32 cylBright;
            s32 burstFrame;
            s32 columnT;
            s32 i;
            s32 layer;
            s32 layerFrame;
            s32 layerStart;
            s32 ang;
            s32 trailAlpha;
            s32 layerRadius;
            s32 ringSize;
            s32 ringAlpha;

            t = (frame << 12) / 77;
            if (frame == 0) {
                work->pos10.vx = fx_195_basePos.vx;
                work->pos10.vy = fx_195_basePos.vy;
                work->pos10.vz = fx_195_basePos.vz;
                work->pos60.vx = fx_195_basePos.vx;
                work->pos60.vy = fx_195_basePos.vy;
                work->pos60.vz = fx_195_basePos.vz;
                work->rot78.vx = -0x400;
                work->rot78.vy = 0;
                work->rot78.vz = 0;
                work->scale6C.vx = 0x4000;
                work->scale6C.vy = 0x4000;
                work->scale6C.vz = 0x4000;
            }
            work->camPos20.vx = work->pos10.vx;
            work->camPos20.vy = work->pos10.vy;
            work->camPos20.vz = work->pos10.vz;
            work->camPos20.vy = -0x2D4 - ((t * 0x76C) >> 12);
            env->camera->points[3].vx = work->camPos20.vx;
            env->camera->points[3].vy = work->camPos20.vy;
            env->camera->points[3].vz = work->camPos20.vz;
            if (frame < 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0xFF - (frame << 7), 0xFF - (frame << 7), 0xFF);
            } else if (frame == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF, 0xFF, 0xFF);
            } else if (frame < 0x13) {
                level = 0x1000 - (((frame - 3) << 12) >> 4);
                level = (level * 0xA0) >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, level, level, level);
            }
            if (frame == 0x14) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xD7, 0x64, 0x64);
            }
            if (frame == 0x1D) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x97, 0x4D, 0x4D);
            }
            if (frame == 0x20) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x80, 0x40, 0x40);
            }
            if (frame < 0x49) {
                if (frame < 7) {
                    grow = ex->math_sin((frame << 10) / 7);
                    sizeMax = 0x2000;
                } else {
                    grow = (frame - 7) * 0x1124 / 65 * 7;
                    /* Scheduling barriers (LOOP notes): sizeMax is set after the multiply chain (so it gets v1), and size is
                     * computed before frame's reload below. */
                    do {
                    } while (0);
                    sizeMax = 0x7800;
                }
                size = sizeMax - grow;
                do {
                } while (0);
                if ((u32)(frame - 5) < 7) {
                    flashTint = ex->math_sin(((frame - 5) << 11) >> 12) * 0xF5;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 0x400), 1, 0, flashTint >> 14, flashTint >> 12);
                }
                work->scale6C.vx = work->scale6C.vy = size;
                fxm_rollTowardEye((VECTOR *)&work->pos60, &work->rot78);
                work->rot78.vz -= 0x400;
                ex->Hi_DrawEffModel(&work->rot78, (VECTOR *)&work->pos60, (s32)&work->scale6C, work->model88);
                ex->Hi_DrawEffModel(&work->rot78, (VECTOR *)&work->pos60, (s32)&work->scale6C, work->model8C);
            }
            if (frame >= 0x3B) {
                fadeT = ((frame - 0x3B) << 12) / 18;
                work->scale6C.vx = work->scale6C.vy = 0x1000;
                fxm_rollTowardEye((VECTOR *)&work->pos60, &work->rot78);
                work->rot78.vz += 0x400;
                ex->Hi_DrawEffModel(&work->rot78, (VECTOR *)&work->pos60, (s32)&work->scale6C, work->model90);
                ex->Hi_ModifyEffModelRGB(work->model90, (ex->math_cos(fadeT >> 2) >> 5) - 0x80,
                                         (ex->math_cos(fadeT >> 2) >> 5) - 0x80, (ex->math_cos(fadeT >> 2) >> 5) - 0x80);
            }
            cylAng = t >> 2;
            cylRadius = (t >> 1) + 0xE00;
            level = ex->math_cos(cylAng);
            sc.vx = 0x1000;
            sc.vy = 0x1000;
            sc.vz = 0x1000;
            cylWidth = (level >> 2) + 0x100;
            flatRot.vx = 0x400;
            flatRot.vy = 0;
            flatRot.vz = 0;
            cylLen = (((t >> 3) + 0x500) * 4) / 3;
            cylBright = ex->math_cos(cylAng) >> 5;
            fxm_drawTexCylinder(&work->pos10, &flatRot, &sc, cylRadius, cylAng + 0xA00, cylWidth >> 1, cylWidth, cylBright, 0x18, 0x3E81, 0, 0xC0, -frame << 7, cylLen, 1,
                          1);
            fxm_drawTexCylinder(&work->pos10, &flatRot, &sc, cylRadius, cylAng + 0xA00, cylWidth >> 1, cylWidth, cylBright, 0x18, 0x3E81, 0, 0xC0, frame * 0xA0, cylLen, 1,
                          -1);
            if (frame < 0x41) {
                columnT = frame << 6;
                amp = columnT >> 2;
                columnBright = ex->math_cos(amp) - 0x5DC;
                rotA.vx = 0;
                rotA.vz = 0;
                rotA.vy = frame << 5;
                scA.vx = scA.vz = ((ex->math_sin(amp) * 3) >> 2) + 0xA00;
                scA.vy = (columnT + 0x300) * 2;
                posA.vx = work->pos10.vx;
                posA.vy = work->pos10.vy;
                posA.vz = work->pos10.vz;
                posA.vy -= (ex->math_sin(amp) * 0xFA0) >> 12;
                ex->Hi_DrawEffModel(&rotA, &posA, (s32)&scA, work->model84);
                ex->Hi_ModifyEffModelRGB(work->model84, (columnBright >> 5) - 0x80, (columnBright >> 5) - 0x80, (columnBright >> 5) - 0x80);
            }
            rotC.vx = 0;
            rotC.vz = 0;
            rotC.vy = frame << 6;
            layerRadius = 0x4000;
            posC.vx = work->pos10.vx;
            posC.vy = work->pos10.vy;
            posC.vz = work->pos10.vz;
            for (layer = 0, layerStart = 7; layer < 6; layerStart += 7, layer++) {
                layerFrame = frame - layerStart;
                if ((u32)layerFrame < 0x20) {
                    ringAlpha = ex->math_cos((layerFrame << 12) >> 7);
                    scC.vx = scC.vz = layerRadius;
                    if (layerFrame < 4) {
                        scC.vy = ex->math_sin((layerFrame << 10) >> 2) * 6;
                    } else {
                        scC.vy = 0x6000 - ((layerFrame - 4) << 8);
                    }
                    scC.vy = (scC.vy * (8 - layer)) >> 3;
                    if (layer < 2) {
                        scC.vy = (scC.vy * 5) >> 2;
                    }
                    ex->Hi_DrawEffModel(&rotC, &posC, (s32)&scC, work->model94);
                    ex->Hi_ModifyEffModelRGB(work->model94, (ringAlpha >> 5) - 0x80, (ringAlpha >> 5) - 0x80, (ringAlpha >> 5) - 0x80);
                }
                layerRadius = (layerRadius * 9) >> 3;
                rotC.vy = (rotC.vy * 3) >> 2;
            }
            if (frame < 0x10) {
                burstFrame = frame << 12;
                rotF.vy = frame * 0x37;
                rotF.vx = 0;
                rotF.vz = 0;
                scF.vx = scF.vz = (burstFrame >> 5) + 0x1200;
                burstAng = burstFrame >> 6;
                scF.vy = ex->math_sin(burstAng) + 0x1800;
                burstBright = ex->math_cos(burstAng);
                posF.vx = work->pos10.vx;
                posF.vy = work->pos10.vy;
                posF.vz = work->pos10.vz;
                ex->Hi_DrawEffModel(&rotF, &posF, (s32)&scF, work->model80);
                ex->Hi_ModifyEffModelRGB(work->model80, (burstBright >> 5) - 0x80, (burstBright >> 5) - 0x80, (burstBright >> 5) - 0x80);
            }
            if (frame == 0x3C) {
                ex->btlfx_replaceSound(0x24, entry->file, -1, work->sound30);
                ex->btlfx_replaceSound(0x25, entry->file, -1, work->sound32);
            }
            BTLFX_COPY_SVEC(&trailFrom, &work->pos10);
            BTLFX_COPY_SVEC(&trailTo, &trailFrom);
            trailTo.vy -= 0x1800;
            amp = 0x1000 - ((t * 0xDAC) >> 12);
            ang = (work->frame4 << 9) + (ex->btlfx_rand() & 0x1FF);
            trailAlpha = (ex->math_cos(t >> 2) * 0xF5) >> 12;
            for (i = 0; i < 2; i++, ang += 0x800) {
                fxm_drawOffsetTrail0(entry, &fx_195_longTrails[i], &trailFrom, &trailTo, (amp * 3) >> 2, amp, ang, 0xC0, 0, 0x1000, trailAlpha);
            }
            if (frame < 0x26) {
                amp = (amp * 3) >> 1;
                trailAlpha = (ex->math_cos(t >> 1) * 0xF5) >> 12;
                fxm_drawOffsetTrail0(entry, &fx_195_longTrail2, &trailFrom, &trailTo, (amp * 3) >> 2, amp, -ang, 0xC0, 0, 0x1000, trailAlpha);
            }
            size = 0x1000 - t;
            env->otZ = 0x300;
            ang = work->frame4 << 7;
            trailAlpha = ((ex->math_cos(t >> 2) * 0xEB) >> 12) + 0x20;
            for (i = 0; i < 5; i++, ang += ex->btlfx_randCentered(0x20) + 0x333) {
                trailFrom.vx = work->pos10.vx;
                trailFrom.vy = work->pos10.vy;
                trailFrom.vz = work->pos10.vz;
                trailFrom.vx += (ex->math_sin(ang) * size) >> 12;
                trailFrom.vz += (ex->math_cos(ang) * size) >> 12;
                trailTo.vx = work->pos10.vx;
                trailTo.vy = work->pos10.vy;
                trailTo.vz = work->pos10.vz;
                trailTo.vx += ex->math_sin(ang);
                trailTo.vz += ex->math_cos(ang);
                trailTo.vy -= 0xF00;
                fxm_drawOffsetTrail1(entry, &fx_195_shortTrails[i], &trailTo, &trailFrom, 0x80, 0x200, ang, 0xFF, 0, ex->math_cos(t >> 2), trailAlpha);
            }
            env->otZ = 0;
            if (frame == 0x31) {
                BTLFX_COPY_SVEC(&work->pos18, &work->pos10);
                work->pos18.vy -= 0x400;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos18);
                if (emitter != NULL) {
                    emitter->otZ = 0x180;
                }
            }
            env->otZ = 0x300;
            rotG.vx = 0x400;
            rotG.vy = 0;
            rotG.vz = 0;
            rotG.pad = 1;
            ringAng = t >> 2;
            ringSize = ex->math_cos(ringAng) + 0x1800;
            ringAlpha = ((ex->math_cos(ringAng) * 3) >> 2) + 0x400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->pos10, NULL, ringSize, 0, 1, ringAlpha);
            ringSize = 0x5000 - (t << 1);
            ringAlpha = ex->math_cos(ringAng);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 2), &work->pos10, &rotG, ringSize, 0, 1, ringAlpha);
            env->otZ = 0;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 7), frame);
            ex->btlfx_drawScrollMove(work->frame4 * 5, 0x200, 0x100, 0x20, 0xC0, 0x220, 0x100);
            if (frame >= 0x4D) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        if (tint1 != 0) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 0xFFE), 0xFF, 0, 0, (tint1 * 0x4D) >> 12);
        }
        if (tint2 != 0) {
            s32 level = tint2 << 7;
            s32 blue = level >> 12;

            if (work->frame4 % 6 == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0, (blue * 2) / 3, blue);
            }
            if (((work->frame4 - 3) & 7) == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, level >> 13, level >> 13, 0);
            }
        }
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_195/nonmatchings/fx_195", fx_195_headerPtr);
