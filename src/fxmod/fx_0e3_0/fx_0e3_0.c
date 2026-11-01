#include "fxmod/fx_0e3_0.h"

static void fxm_drawTexRibbonTrail(SVECTOR *posA, SVECTOR *posB, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 bright,
                   Fx0e30Seg *buf, s32 abr, s32 phase) {
    Unk800F78A4 *env = fx_0e3_0_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    s32 otz;
    POLY_GT4 *prim;
    Fx0e30Seg *seg;
    s32 i;
    s32 t;
    s32 tileV;

    if (bright < 0) {
        bright = 0x1000;
    }
    if (n > 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].a0.pad = 0;
        }
        return;
    }
    seg = &buf[(u32)(n - 1)]; /* (n - 1) * 16, not n * 16 - 16 */
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&seg[1].a0, &seg[0].a0);
        BTLFX_COPY_SVEC(&seg[1].b8, &seg[0].b8);
        seg--;
    }
    seg = buf;
    for (i = 0; i < n; i++, seg++) {
        if (!(seg[0].a0.pad & seg[1].a0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->a0, posA);
    buf->a0.pad = 1;
    BTLFX_COPY_SVEC(&buf->b8, posB);
    n = i;
    prim = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(prim + n);
    col.cd = abr == 0xFF ? 0x3C : 0x3E;
    w--;
    h--;
    tpage |= (abr & 3) << 5;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    /* the last uv store (u1) is the address giv loop.c bases the others on, as in the original's s0 = &prim->u1 */
    for (i = 0; i < n; i++, buf++, prim++, phase++) {
        gte_ldv3c(buf);
        t = (i << 12) / n;
        gte_rtpt_b();
        if (i != 0) {
            col.r = col.g = col.b = ((0x1000 - t) * bright) >> 12;
        } else {
            col.r = col.g = col.b = 0;
        }
        *(s32 *)&prim->r0 = *(s32 *)&prim->r1 = *(s32 *)&col;
        col.r = col.g = col.b = ((0x1000 - t) * bright) >> 12;
        *(s32 *)&prim->r2 = *(s32 *)&prim->r3 = *(s32 *)&col;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].b8);
            gte_rtps();
            ex->btlfx_addPrim(0x0C000000, otz, prim);
            gte_stsxy(&prim->x3);
            prim->clut = clut;
            prim->tpage = tpage;
            prim->u0 = prim->u2 = u;
            tileV = phase & 0x1F;
            prim->v0 = prim->v1 = v + ((tileV * h) >> 5);
            prim->v2 = prim->v3 = v + (((tileV + 1) * h) >> 5);
            prim->u1 = prim->u3 = u + w;
        }
    }
}

static void fxm_drawOffsetTrail0(Unk80100C78Entry *entry, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e3_0_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->pts4;
    bolt->count2 = 0xC;
    bolt->lastSlot0 = 0xC;
    bolt->head1 = 0;
    for (i = 0; i < 13; i++) {
        t = 0x1000 - (i << 12) / 12;
        t += ex->btlfx_randCentered(fx_0e3_0_alongJitter);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = ex->btlfx_randCentered(fx_0e3_0_ampJitter);
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = 0x19;
        pt++;
        ang += dang + ex->btlfx_randCentered(fx_0e3_0_angleJitter);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 8), 0, 0, frame, 0x1000, fade, 1, (u8 *)bolt, 0);
}

static void fxm_drawOffsetTrail1(Unk80100C78Entry *entry, s32 anchor, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang,
                   s32 dang, s32 frame, s32 fade) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e3_0_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->pts4;
    bolt->count2 = 0x30;
    bolt->lastSlot0 = 0x30;
    bolt->head1 = 0;
    for (i = 0; i < 0x31; i++) {
        t = 0x1000 - (i << 12) / 48;
        t += ex->btlfx_randCentered(0x97);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = ex->btlfx_randCentered(0x73);
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += (ex->math_sin(t >> 1) * amp) >> 12;
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = 0x33;
        ang += dang + ex->btlfx_randCentered(0x100);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 2), 0, (s32 *)anchor, frame, 0x1000, fade, 1, (u8 *)bolt, 1);
}

static s32 fx_0e3_0_op80PoolTask(s32 mode, Fx0e30Puff *p, Unk800F78A4 *env, Unk80100C78Entry *entry) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos;
    s32 phase;
    s32 ang;
    s32 fade;
    s32 size;

    if (mode == 1) {
        p->phase8 = 0;
        p->speedE = (ex->btlfx_rand() & 0x1F) + 0x100;
        p->sizeC = (ex->btlfx_rand() & 0x1FF) + 0x1000;
    } else {
        phase = p->phase8 + p->speedE;
        ang = phase >> 2;
        p->phase8 = phase;
        fade = 0x1000 - ex->math_cos(ang);
        size = (p->sizeC * ((phase >> 1) + 0x800)) >> 12;
        rot.pad = 0;
        rot.vz = 0;
        rot.vy = 0;
        rot.vx = 0;
        pos.vx = p->pos0.vx;
        pos.vy = p->pos0.vy - (0x1000 - ex->math_cos(ang)) * 3;
        pos.vz = p->pos0.vz;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 3), &pos, &rot, size * 2, 0, 1, fade, 0, 0);
        if (p->phase8 > 0x1000) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e3_0/nonmatchings/fx_0e3_0", fx_0e3_0_headerPtr);

/* Matching levers (pass 3): variables split by role so that ex/work/frame get s2/s3/s5 like the original (several
 * short-lived block values otherwise outrank them in global-alloc); call results passed on in single-set variables
 * (sched1's birthing boost); `a - K - (call >> n)` through temps (fold reassociates otherwise); amp4/amp5 = 0x37
 * spilled constants for the ring loops; case 0's loop tests the first ph >= 0x1000 outside the inner loop; case 1's
 * unk50 = 1 between the unk10 stores forces the unk10.vy reload. Stack layout: a block variable whose address is
 * taken in a statement directly in its block moves to the parent temp level (preserve_temp_slots) and outlives the
 * block; case 4 needs the switch (frame) { default: } wrapper and case 1's colour the 8-byte union (combine_temp_slots
 * adds size, not full_size). */
s32 fx_0e3_0_op80(s32 mode, Fx0e30Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 fade;
    s32 trail;
    s32 nbolt;
    s32 done;
    s32 i;
    s32 side;
    s32 arc4;
    s32 arc5;
    s32 ring;
    s32 spark;
    s32 ph;
    s32 j;
    s32 sign;
    s32 boltAng;
    s32 cosAng;
    s32 coneAng;
    s32 coneWidth;
    s32 bang;
    s32 rgb;
    s32 c1;
    s32 ringAng;
    s32 rad;
    s32 rise;
    s32 tmp;
    s32 dang;
    s32 dang2;
    s32 boltFade;
    s32 swoopFrame;
    s32 gridT;
    s32 ringPhase;
    s32 gridBright;
    s32 dy;
    s32 halfLen;
    s32 glow;
    s32 wraps;
    s32 glowScale;
    s32 wave;
    s32 summonRgb;
    s32 pillarSin;
    s32 unused;
    Pool6CEntry *emitter;
    Fx0e30Puff *puff;

    if (mode == 0) {
        init->workSize = 0xA8;
        init->elemSize = 0x10;
        init->elemFn = (s32)fx_0e3_0_op80PoolTask;
        init->poolCount = 0xC;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0e3_0_env = env;
        ex->btlfx_initEffModels(fx_0e3_0_effModels, 0x10);
        work->model60 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 0);
        work->model64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xB), 1);
        work->basePos8.vx = 0;
        work->basePos8.vy = 0;
        work->basePos8.vz = 0;
        work->unk70.vx = fx_0e3_0_origin.vx;
        work->unk70.vy = fx_0e3_0_origin.vy;
        work->unk70.vz = fx_0e3_0_origin.vz;
        work->unk7C.vx = 0x1000;
        work->unk7C.vy = 0x1000;
        work->unk7C.vz = 0x1000;
        work->unk68.vx = 0;
        work->unk68.vy = 0;
        work->unk68.vz = 0;
        work->scale9C.vx = 0x800;
        work->scale9C.vy = 0x800;
        work->scale9C.vz = 0x800;
    } else {
    fade = 0;
    trail = 0;
    nbolt = 0;
    done = 0;
    frame = entry->frame;
    {
    switch (work->state0) {
    case 0: {
        SVECTOR rot;
        VECTOR sc;
        VECTOR pos;
        SVECTOR rot2;
        VECTOR sc2;
        VECTOR pos2;

        t = (frame << 12) / 69;
        if (frame < 0xC) {
            s32 flash = (ex->math_cos((frame << 10) / 12) * 0xFF) >> 12;

            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, flash, flash, flash);
        }
        if (frame >= 0x18) {
            if (frame == 0x18) {
                ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion180, SUMMON(env)->summonModel60);
                ex->Hi_SetSummonMotFrame(SUMMON(env)->summonModel60, 0);
            }
            swoopFrame = frame - 0x18;
            ph = (swoopFrame << 12) / 45;
            bang = (ex->math_sin(((ph >> 1) * 3) / 2) >> 3) + 0x800;
            rad = (((0x1000 - ph) * 0x4B0) >> 12) + 0x18;
            if (ph < 0xC00) {
                tmp = 0x1800;
                dy = ex->math_sin(ph / 3) * 2 - 0x1000;
            } else {
                s32 late = ph - 0xC00;
                dy = ex->math_sin(late) * 3 + 0x1000;
                tmp = 0x1800 - late * 4;
            }
            work->summonPos10.vx = work->basePos8.vx + ((ex->math_cos(bang) * rad) >> 12);
            work->summonPos10.vy = work->basePos8.vy + dy;
            work->summonPos10.vz = work->basePos8.vz + ((ex->math_sin(bang) * rad) >> 12);
            work->rot28.vx = 0x400;
            work->rot28.vy = -bang * 2 + 0xC00;
            work->summonScale1C.vx = work->summonScale1C.vy = work->summonScale1C.vz = tmp;
            ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C, SUMMON(env)->summonModel60, 1);
            nbolt = 1;
            ex->Hi_ModifySummonModelRGB(SUMMON(env)->summonModel60, (ex->math_cos(ph >> 2) >> 5) - 0x80,
                                        (ex->math_cos(ph >> 2) >> 5) - 0x80, (ex->math_cos(ph >> 2) >> 5) - 0x80);
            if (frame == 0x2C) {
                work->trailReset50 = nbolt;
            } else if (frame >= 0x2D) {
                trail = 0x1000;
            }
        }
        ringPhase = frame * 0xF5;
        ringAng = frame << 5;
        pos.vx = work->basePos8.vx;
        pos.vz = work->basePos8.vz;
        rot.vz = 0;
        rot.vx = 0;
        for (wave = 0; wave < 9; wave++, ringPhase += 0x1C7, ringAng += 0x1FF) {
            ph = ringPhase;
            if (ph >= 0x1000) {
                wraps = 2;
                while (ph >= 0x1000 && wraps > 0) {
                    ph -= 0x1000;
                    wraps--;
                }
            }
            if (ph <= 0x1000) {
                tmp = ph >> 2;
                {
                    s32 kq = (0x1000 - ex->math_cos(tmp)) * 3 - 0x2000;
                    pos.vy = work->basePos8.vy - kq;
                }
                rot.vy = ringAng;
                {
                    s32 fall = 0x1000 - ex->math_cos(tmp);
                    sc.vx = sc.vy = sc.vz = ((ph >> 1) + 0x800) / 2;
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model64);
                    tmp = ((fall + 0x200) >> 5) - 0x80;
                }
                ex->Hi_ModifyEffModelRGB(work->model64, tmp, tmp, tmp);
            }
        }
        if (((frame & 1) && frame < 0x23) || frame >= 0x23) {
            puff = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (puff != NULL) {
                puff->pos0.vx = work->basePos8.vx + ex->btlfx_randCentered(0x800);
                puff->pos0.vz = work->basePos8.vz + ex->btlfx_randCentered(0x800);
                puff->pos0.vy = work->basePos8.vy + 0x2000;
            }
        }
        pos2.vx = work->basePos8.vx;
        pos2.vy = work->basePos8.vy;
        pos2.vz = work->basePos8.vz;
        pos2.vy += 0x6000;
        if (t < 0xC00) {
            glowScale = t + 0x2000;
            sc2.vx = sc2.vy = sc2.vz = glowScale;
        } else {
            glowScale = (t - 0xC00) * 5 + 0x2C00;
            sc2.vx = sc2.vy = sc2.vz = glowScale;
        }
        rot2.vz = 0;
        rot2.vx = t >> 4;
        rot2.vy = frame * 7;
        ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&sc2, work->model60);
        {
            s32 coreRgb = (((t >> 1) + 0x800) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model60, coreRgb, coreRgb, coreRgb);
        }
        BTLFX_COPY_SVEC(&work->pos30, &work->basePos8);
        work->pos30.vx += (ex->btlfx_randCentered(0xA0) * (0x1000 - t)) >> 12;
        work->pos30.vz += (ex->btlfx_randCentered(0xA0) * (0x1000 - t)) >> 12;
        env->camera->points[2].vx = work->pos30.vx;
        env->camera->points[2].vy = work->pos30.vy;
        env->camera->points[2].vz = work->pos30.vz;
        if (frame < 0x2E) {
            SVECTOR vec;

            {
                s32 tint = 0x96 - (frame * 0x96) / 46;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 0x200), 1, tint, tint, tint);
            }
            BTLFX_COPY_SVEC(&vec, &work->basePos8);
            vec.vy -= 0x800;
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xC), &vec, NULL, 0x1800, 0, 1, ex->math_cos(t >> 2));
            env->otZ = 0;
        }
        if (frame >= 0x45) {
            work->state0 = 10;
            entry->frame = -1;
            ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion184, SUMMON(env)->summonModel60);
            ex->Hi_SetSummonMotFrame(SUMMON(env)->summonModel60, 0);
            work->pos30.vx = 0;
            work->pos30.vy = 0;
            work->pos30.vz = work->basePos8.vz + 0x78E;
            env->camera->points[3].vx = work->pos30.vx;
            env->camera->points[3].vy = work->pos30.vy;
            env->camera->points[3].vz = work->pos30.vz;
            work->pos30.vx = work->basePos8.vx;
            work->pos30.vy = work->basePos8.vy;
            work->pos30.vz = work->basePos8.vz;
            work->pos30.vy -= 0x1FF;
            work->pos30.vz -= 0x800;
            env->camera->points[1].vx = work->pos30.vx;
            env->camera->points[1].vy = work->pos30.vy;
            env->camera->points[1].vz = work->pos30.vz;
        }
        break;
    }
    case 10: {
        t = (frame << 12) / 24;
        if (frame == 0) {
            ex->btlfx_poolClear(entry->pool);
            work->trailReset50 = 1;
        }
        if (frame < 0x11) {
            rise = (frame << 5) - 0x1800;
        } else {
            rise = (frame - 0x10) * 0x1DB - 0x1600;
        }
        work->summonPos10.vy = work->basePos8.vy + rise - 0x400;
        work->summonPos10.vx = work->basePos8.vx;
        {
            s32 cz = rise / 3 - 0x400;
            work->summonPos10.vz = work->basePos8.vz - cz;
        }
        if (t < 0xC00) {
            work->rot28.vx = 0x400 - (t >> 3);
        } else {
            work->rot28.vx = 0x28C - (((t - 0xC00) * 0x1F4) >> 10);
        }
        work->rot28.vy = 0;
        work->summonScale1C.vx = work->summonScale1C.vy = work->summonScale1C.vz = (rise + 0x1800) * 2;
        ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C, SUMMON(env)->summonModel60, 1);
        ex->Hi_ModifySummonModelRGB(SUMMON(env)->summonModel60, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                    (ex->math_sin(t >> 2) >> 5) - 0x80);
        work->pos30.vx = work->summonPos10.vx;
        work->pos30.vy = work->summonPos10.vy;
        work->pos30.vz = work->summonPos10.vz;
        if (frame < 0x10) {
            dang = (frame << 10) >> 4;
            work->pos30.vy = -ex->math_sin(dang);
            work->pos30.vx = ex->math_sin(dang) >> 2;
        } else {
            work->pos30.vy = -0x1000;
            work->pos30.vx = 0x400;
        }
        env->camera->points[3].vx = work->pos30.vx;
        env->camera->points[3].vy = work->pos30.vy;
        env->camera->points[3].vz = work->pos30.vz;
        work->pos30.vx = work->basePos8.vx;
        work->pos30.vy = work->basePos8.vy;
        work->pos30.vz = work->basePos8.vz;
        work->pos30.vy -= 0x1FF;
        work->pos30.vz -= 0x800;
        env->camera->points[1].vx = work->pos30.vx;
        nbolt = 2;
        env->camera->points[1].vy = work->pos30.vy;
        trail = 0x1000;
        env->camera->points[1].vz = work->pos30.vz;
        if (frame >= 0x18) {
            work->state0 = 1;
            entry->frame = -1;
            ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion180, SUMMON(env)->summonModel60);
            work->summonScale1C.vx = -0x3000;
            work->rot28.vz = 0;
            work->rot28.vy = 0;
            work->rot28.vx = 0;
            work->summonScale1C.vy = 0x3000;
            work->summonScale1C.vz = 0x3000;
            work->summonPos10.vx = fx_0e3_0_origin.vx;
            work->summonPos10.vy = fx_0e3_0_origin.vy;
            work->summonPos10.vz = fx_0e3_0_origin.vz;
            work->summonPos10.vx += 0x800;
            work->summonPos10.vy -= 0x1000;
            work->summonPos10.vz = 0x7000;
            work->pos30.vx = fx_0e3_0_origin.vx;
            work->pos30.vy = fx_0e3_0_origin.vy;
            work->pos30.vz = fx_0e3_0_origin.vz;
            trail = 0;
            work->pos30.vy -= 0x400;
            env->camera->points[2].vx = work->pos30.vx;
            env->camera->points[2].vy = work->pos30.vy;
            env->camera->points[2].vz = work->pos30.vz;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF, 0xFF, 0xFF);
            work->trailReset50 = 1;
            work->model54 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x82), 1);
            work->model58 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x86), 1);
            work->model5C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x85), 0);
        }
        break;
    }
    case 1: {
        fade = 0;
        work->pos30.vz -= 0x10;
        trail = 0x1000;
        env->camera->points[2].vx = work->pos30.vx;
        nbolt = 1;
        env->camera->points[2].vy = work->pos30.vy;
        env->camera->points[2].vz = work->pos30.vz;
        if (work->summonPos10.vz > -0xC000) {
            work->summonPos10.vz -= 0x1300;
        }
        ex->Hi_SetSummonMotFrame(SUMMON(env)->summonModel60, 0xA);
        ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C, SUMMON(env)->summonModel60, nbolt);
        ex->Hi_ModifySummonModelRGB(SUMMON(env)->summonModel60, -0x20, -0x20, -0x20);
        if (frame >= 0xE) {
            SVECTOR rot;
            VECTOR sc;
            VECTOR pos;

            tmp = ((frame - 0xE) << 12) / 10;
            pillarSin = ex->math_sin(tmp >> 1);
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vz = 0x2800;
            sc.vx = 0x2800;
            if (tmp < 0x400) {
                sc.vy = tmp * 0xC;
            } else {
                sc.vy = 0x3000 - ((tmp - 0x400) * 2) / 3;
            }
            pos.vx = 0;
            pos.vy = 0;
            pos.vz = ex->math_cos(tmp >> 2) / 3;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model54);
            {
                s32 pillarRgb = (pillarSin >> 6) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model54, pillarRgb, pillarRgb, pillarRgb);
            }
        }
        if (frame >= 0xE) {
            gridT = frame - 0xE;
            if (gridT < 0x10) {
                union { CVECTOR c1; s32 w[2]; } cu;
                SVECTOR rot3;
                VECTOR sc3;

                cu.c1.r = cu.c1.g = cu.c1.b = 0x46;
                rot3.vx = 0;
                rot3.vy = 0;
                rot3.vz = 0;
                sc3.vz = 0x1000;
                gridT = (gridT << 12) / 14;
                sc3.vx = gridT + 0x1000;
                sc3.vy = gridT / 2 + 0x1000;
                ex->btlfx_drawScreenGrid(3, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot3, &sc3, &cu.c1, 1, 0);
            }
        }
        if (frame >= 0x18) {
            work->state0 = 2;
            entry->frame = -1;
            work->summonPos10.vx = fx_0e3_0_origin.vx;
            work->summonPos10.vy = fx_0e3_0_origin.vy;
            work->summonPos10.vz = fx_0e3_0_origin.vz + 0x5000;
            work->trailReset50 = 1;
            work->summonPos10.vy -= 0x2000;
            ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion188, SUMMON(env)->summonModel60);
            ex->Hi_SetSummonMotFrame(SUMMON(env)->summonModel60, 0);
            work->summonScale1C.vx = 0x1000;
            work->summonScale1C.vy = 0x1000;
            work->summonScale1C.vz = 0x1000;
            env->camera->points[2].vx = work->summonPos10.vx;
            env->camera->points[2].vy = work->summonPos10.vy;
            trail = 0;
            env->camera->points[2].vz = work->summonPos10.vz;
        }
        break;
    }
    case 2: {
        nbolt = 2;
        t = (frame << 12) / 26;
        if (frame == 0) {
            work->rot88.vx = 0x400;
            work->scale9C.vx = 0x1000;
            work->scale9C.vy = 0x1000;
            work->rot88.vz = 0;
            work->rot88.vy = 0;
            work->scale9C.vz = 0x3000;
            work->pos90.vx = work->summonPos10.vx;
            work->pos90.vy = work->summonPos10.vy;
            work->pos90.vz = work->summonPos10.vz;
            work->pos90.vy -= 0x3000;
            work->pos90.vz -= 0x1000;
        }
        work->rot88.vz += 8;
        work->pos90.vy += 0x100;
        work->summonPos10.vy -= 0x180;
        {
            s32 cz = ex->math_cos(t >> 2) >> 5;
            s32 vz = work->summonPos10.vz - 0x80;
            work->summonPos10.vz = vz - cz;
        }
        trail = 0x1000;
        env->camera->points[2].vx = work->summonPos10.vx;
        fade = 0x1000;
        env->camera->points[2].vy = work->summonPos10.vy;
        env->camera->points[2].vz = work->summonPos10.vz;
        ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C, SUMMON(env)->summonModel60, 0);
        summonRgb = ((((t * 3) >> 2) + 0x400) >> 5) - 0x80;
        ex->Hi_ModifySummonModelRGB(SUMMON(env)->summonModel60, summonRgb, summonRgb, summonRgb);
        if (frame >= 0x1A) {
            work->state0 = 4;
            entry->frame = -1;
            work->unk48 = 0;
            ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion18C, SUMMON(env)->summonModel60);
            ex->Hi_SetSummonMotFrame(SUMMON(env)->summonModel60, 0xA);
            work->pos30.vy = -0x4966;
            work->pos30.vx = 0;
            work->pos30.vz = 0x3DAF;
            env->camera->points[3].vx = work->pos30.vx;
            env->camera->points[3].vy = work->pos30.vy;
            env->camera->points[3].vz = work->pos30.vz;
        }
        break;
    }
    case 4:
        switch (frame) {
        default: {
        SVECTOR vec;
        SVECTOR ahead;
        MATRIX boneMat;
        s32 ang;
        s32 amp4;

        t = (frame << 12) / 30;
        nbolt = 3;
        fade = 0x1000;
        work->pos90.vx = work->summonPos10.vx;
        work->pos90.vy = work->summonPos10.vy;
        work->pos90.vz = work->summonPos10.vz;
        work->pos90.vx += 0x2000;
        tmp = t >> 2;
        {
            s32 cz = ex->math_cos(tmp);
            s16 vz = work->rot88.vz - 8;
            work->rot88.vz = vz - ((cz * 0x3E) >> 12);
        }
        work->summonPos10.vy -= (ex->math_cos(tmp) << 8) >> 12;
        ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C, SUMMON(env)->summonModel60, 0);
        ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, 0, (u16 *)&work->pos30);
        work->pos30.vy -= 0x80;
        env->camera->points[3].vx = work->pos30.vx;
        fx_0e3_0_alongJitter = 0xA0;
        fx_0e3_0_ampJitter = 0x4D;
        env->camera->points[3].vy = work->pos30.vy;
        env->camera->points[3].vz = work->pos30.vz;
        fx_0e3_0_angleJitter = 0x180;
        ang = work->frame4 * 0x180;
        amp4 = 0x37;
        for (arc4 = 0; arc4 < 2; arc4++) {
            ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, fx_0e3_0_trailBones[arc4], (u16 *)&vec);
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, fx_0e3_0_trailBones[arc4], (BtlBone *)&boneMat);
            ex->btlfx_matrixPointAhead(&boneMat, 0x398, (s32)&ahead);
            fxm_drawOffsetTrail0(entry, (Fx0e30Bolt *)&fx_0e3_0_arcs[arc4], &vec, &ahead, amp4, amp4, ang, 0x200, 0,
                          ex->btlfx_randCentered(0x400) + 0xC00);
        }
        if (frame >= 0x1E) {
            work->state0 = 5;
            entry->frame = -1;
            ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion194, SUMMON(env)->summonModel60);
            ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, 0, (u16 *)&work->pos40);
            work->emit4C = NULL;
            work->pos40.vy -= 0x80;
        }
        break;
        }
        }
        break;
    case 5: {
        SVECTOR vec;
        SVECTOR ba;
        SVECTOR bb;
        MATRIX bm;
        SVECTOR ca;
        SVECTOR cb;
        SVECTOR rot2;
        SVECTOR from;
        VECTOR gap;
        VECTOR sc2;
        VECTOR pos2;
        SVECTOR to;
        SVECTOR mid;
        CVECTOR col;
        SVECTOR rot3;
        VECTOR sc3;

        t = (frame << 12) / 65;
        if (frame == 0) {
            work->rot88.vx = 0x400;
            work->scale9C.vx = 0x1000;
            work->scale9C.vy = 0x1000;
            work->rot88.vy = 0;
            work->rot88.vz = 0;
            work->scale9C.vz = 0x2000;
            work->pos90.vy = work->summonPos10.vy;
            work->pos90.vx = work->summonPos10.vx;
            work->pos90.vz = work->summonPos10.vz;
            work->pos90.vy -= 0x1000;
        }
        if (frame < 2) {
            ex->Hi_SetSummonMotFrame(SUMMON(env)->summonModel60, 0);
        }
        if (frame < 0x14) {
            fade = 0x1000;
        } else {
            fade = 0x1000 - ((frame - 0x14) << 11) / 45;
        }
        work->rot88.vz += 2;
        if (frame == 7) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x83), (s32)&work->pos40);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        if (frame == 0xC) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos40);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&ba);
        ex->btlfx_matrixPointAhead((MATRIX *)&ba, 0x18, (s32)&work->pos38);
        ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&rot2);
        ex->btlfx_matrixAngles((MATRIX *)&rot2, (s16 *)&vec);
        if (frame == 0xC) {
            work->emit4C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos38);
            if (work->emit4C != NULL) {
                work->emit4C->otZ = 0x80;
            }
        }
        if (work->emit4C != NULL) {
            work->emit4C->rot.vx = vec.vx;
            work->emit4C->rot.vy = vec.vy;
            work->emit4C->rot.vz = vec.vz;
        }
        ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, 0, (u16 *)&work->pos30);
        work->pos30.vy -= 0x200;
        if (frame >= 5) {
            work->pos30.vy += 0x80 + ((ex->math_sin((((frame - 4) << 12) / 61) >> 2) * 0x180) >> 12);
        } else {
            work->pos30.vy += frame << 5;
        }
        BTLFX_COPY_SVEC(&vec, &work->pos30);
        if ((u32)(frame - 0x1C) < 7) {
            vec.vx += (ex->math_cos(((frame - 0x1C) << 12) >> 5) >> 7) * ((frame & 1) * 2 - 1);
        }
        env->camera->points[2].vx = vec.vx;
        env->camera->points[2].vy = vec.vy;
        env->camera->points[2].vz = vec.vz;
        ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C, SUMMON(env)->summonModel60, 0);
        if (frame < 0xC) {
            s32 ang;
            s32 amp5;

            fx_0e3_0_alongJitter = 0xA0;
            fx_0e3_0_ampJitter = 0x4D;
            fx_0e3_0_angleJitter = 0x180;
            ang = work->frame4 * 0x180;
            amp5 = 0x37;
            for (arc5 = 0; arc5 < 2; arc5++) {
                ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, fx_0e3_0_trailBones[arc5], (u16 *)&ba);
                ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, fx_0e3_0_trailBones[arc5], (BtlBone *)&bm);
                ex->btlfx_matrixPointAhead(&bm, 0x398, (s32)&bb);
                fxm_drawOffsetTrail0(entry, (Fx0e30Bolt *)&fx_0e3_0_arcs[arc5], &ba, &bb, amp5, amp5, ang, 0x200, 0,
                              ex->btlfx_randCentered(0x400) + 0xC00);
            }
        }
        if (frame >= 0xC) {
            s32 ang;
            s32 cnt;
            s32 amp;

            cnt = 4;
            amp = ((t * 0x180) >> 12) + 0x20;
            ang = work->frame4 << 8;
            if (frame >= 0x19) {
                cnt = 6;
            }
            fx_0e3_0_alongJitter = 0x180;
            fx_0e3_0_ampJitter = 0x80;
            fx_0e3_0_angleJitter = 0xA0;
            for (spark = 0; spark < cnt; spark++) {
                ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, ex->btlfx_rand() % 92, (u16 *)&ca);
                ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, ex->btlfx_rand() % 92, (u16 *)&cb);
                fxm_drawOffsetTrail0(entry, (Fx0e30Bolt *)&fx_0e3_0_arcs[spark], &ca, &cb, amp, amp, ang, 0x100, 0,
                              ex->btlfx_randCentered(0x400) + 0xC00);
            }
        }
        if (frame == 0x16) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x5C, 0x5C, 0x5C);
        }
        if (frame >= 0x14) {
            coneAng = (((frame - 0x14) << 12) / 45) >> 2;
            glow = ex->math_cos(coneAng) / 3 + 0x200;
            rot2.vx = 0x800;
            rot2.vy = 0;
            rot2.vz = 0;
            pos2.vx = work->summonPos10.vx;
            pos2.vy = work->summonPos10.vy;
            pos2.vz = work->summonPos10.vz;
            pos2.vy += 0x5A5;
            rot2.vy = (frame << 5) + 0x800;
            sc2.vy = ex->math_cos(coneAng) >> 2;
            coneWidth = ex->math_sin(coneAng) * 4 + 0x800;
            tmp = (glow >> 5) - 0x80;
            for (ring = 0; ring < 9; ring++) {
                sc2.vx = sc2.vz = ((ex->math_sin((ring << 11) >> 3) + 0x800) * coneWidth) >> 12;
                pos2.vy -= 0x11F;
                ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&sc2, work->model58);
                ex->Hi_ModifyEffModelRGB(work->model58, tmp, tmp, tmp);
                rot2.vy += 0x366;
            }
        }
        if (frame >= 0x18) {
            s32 amp;
            s32 boltPhase;

            boltPhase = (((frame - 0x18) << 12) / 41) >> 2;
            halfLen = ex->math_sin(boltPhase) * 2;
            from.vx = work->summonPos10.vx;
            from.vy = work->summonPos10.vy;
            from.vz = work->summonPos10.vz;
            BTLFX_COPY_SVEC(&mid, &from);
            BTLFX_COPY_SVEC(&to, &from);
            halfLen = halfLen / 3 + 0x400;
            from.vy -= halfLen;
            to.vy += halfLen;
            amp = (ex->math_sin(boltPhase) >> 1) + 0x180;
            cosAng = boltPhase;
            for (side = 0, sign = -1, boltAng = work->frame4 * 0x9B - 0x400; side < 2; sign += 2, side++, boltAng += 0x400) {
                boltFade = ex->math_cos(cosAng) + ex->btlfx_randCentered(0x400);
                fxm_drawOffsetTrail1(entry, (s32)&mid, &fx_0e3_0_bolts[side], &from, &to, amp, boltAng * sign,
                              ((ex->math_cos(cosAng) >> 5) + 0x100) >> 1, 0, boltFade);
            }
        }
        if (frame >= 0x1C) {
            s32 xq = frame - 0x1C;
            xq = (xq << 12) / 37;
            gridBright = (ex->math_cos(xq >> 2) * 0x57) >> 12;
            col.r = col.g = col.b = gridBright;
            sc3.vy = sc3.vx = 0x1200 - (xq >> 3);
            sc3.vz = 0x1000;
            rot3.vx = 0;
            rot3.vy = 0;
            rot3.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot3, &sc3, &col, 1, 0);
        }
        if (frame >= 0x41) {
            done = 1;
        }
        break;
    }
    }
    }
    {
    SVECTOR vec;
    SVECTOR ahead;
    MATRIX boneMat;
    MATRIX boneMatB;
    MATRIX trailMat;
    SVECTOR ba;
    SVECTOR bb;

    if (trail != 0) {
        env->otZ = -0x300;
        trail >>= 1;
        if (work->trailReset50 != 0) {
            work->trailReset50 = 0;
            for (j = 0; j < 4; j++) {
                ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, fx_0e3_0_ribbonBones[j].edgeA0, (u16 *)&vec);
                ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, fx_0e3_0_ribbonBones[j].edgeB2, (u16 *)&ahead);
                fxm_drawTexRibbonTrail(&vec, &ahead, 0x3F6, 0, 0, 0, 0, 0, 0, 0, fx_0e3_0_ribbons[j], 1, 0);
            }
            for (j = 0; j < 4; j++) {
                ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, fx_0e3_0_trailBones[j], (u16 *)&vec);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 1), (s32)&vec, 0, -1, 0x1000, trail, 1,
                                    fx_0e3_0_strips[j], 0);
            }
        }
        for (j = 0; j < 4; j++) {
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, fx_0e3_0_ribbonBones[j].edgeA0, (BtlBone *)&boneMat);
            ex->btlfx_matrixPointAhead(&boneMat, (fx_0e3_0_ribbonBones[j + 4].edgeA0 * work->summonScale1C.vy) >> 12, (s32)&vec);
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, fx_0e3_0_ribbonBones[j].edgeB2, (BtlBone *)&boneMatB);
            ex->btlfx_matrixPointAhead(&boneMatB, (fx_0e3_0_ribbonBones[j + 4].edgeB2 * work->summonScale1C.vy) >> 12, (s32)&ahead);
            fxm_drawTexRibbonTrail(&vec, &ahead, 0xE, 0x9D, 0x3D40, 0, 0x80, 0x40, 0x80, trail >> 5, fx_0e3_0_ribbons[j], 1,
                          10000 - work->frame4);
        }
        for (j = 0; j < 4; j++) {
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, fx_0e3_0_trailBones[j], (BtlBone *)&trailMat);
            ex->btlfx_matrixPointAhead(&trailMat, (fx_0e3_0_trailBones[j + 4] * work->summonScale1C.vy) >> 12, (s32)&vec);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 1), (s32)&vec, 0, 0, 0x2000, trail, 1,
                                fx_0e3_0_strips[j], 0);
        }
        env->otZ = 0;
    }
    if (fade != 0) {
        ex->Hi_DrawEffModel(&work->rot88, (VECTOR *)&work->pos90, (s32)&work->scale9C, work->model5C);
        rgb = (fade >> 6) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->model5C, rgb, rgb, rgb);
    }
    if (nbolt != 0) {
        fx_0e3_0_alongJitter = 0x180;
        fx_0e3_0_ampJitter = 0x80;
        fx_0e3_0_angleJitter = 0xA0;
        dang2 = work->frame4 << 8;
        for (i = 0; i < nbolt; i++) {
            ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, ex->btlfx_rand() % 92, (u16 *)&ba);
            ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, ex->btlfx_rand() % 92, (u16 *)&bb);
            fxm_drawOffsetTrail0(entry, (Fx0e30Bolt *)&fx_0e3_0_arcs[i], &ba, &bb, 0x20, 0x40, dang2, 0x100, 0,
                          ex->btlfx_randCentered(0x400) + 0x800);
        }
    }
    }
        if (done != 0) {
            return 1;
        }
        work->frame4++;
    }
    return 0;
}
