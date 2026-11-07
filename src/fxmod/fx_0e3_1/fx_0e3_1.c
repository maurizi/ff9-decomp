#include "fxmod/fx_0e3_1.h"

static void fxm_drawOffsetTrail0(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e3_1_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->pts4;
    bolt->count2 = FX_BAHAMUT_ARC_POINTS - 1;
    bolt->lastSlot0 = FX_BAHAMUT_ARC_POINTS - 1;
    bolt->head1 = 0;
    for (i = 0; i < FX_BAHAMUT_ARC_POINTS; i++) {
        t = 0x1000 - (i << 12) / (FX_BAHAMUT_ARC_POINTS - 1);
        t += ex->btlfx_randCentered(0x180);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = ex->btlfx_randCentered(0x80);
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = FX_BAHAMUT_ARC_WIDTH;
        ang += dang + ex->btlfx_randCentered(0x100);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_WHITE_BOLT), 0, 0, frame, 0x1000, fade, BTLFX_ABR_ADD, (u8 *)bolt, 0);
}

static void fxm_drawOffsetTrail1(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang, s32 dang,
                   s32 frame, s32 fade) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e3_1_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->pts4;
    bolt->count2 = FX_BAHAMUT_SPARK_POINTS - 1;
    bolt->lastSlot0 = FX_BAHAMUT_SPARK_POINTS - 1;
    bolt->head1 = 0;
    for (i = 0; i < FX_BAHAMUT_SPARK_POINTS; i++) {
        t = 0x1000 - (i << 10);
        t += ex->btlfx_randCentered(0x80);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = ex->btlfx_randCentered(0x20);
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += (ex->math_sin(t >> 1) * amp) >> 12;
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = FX_BAHAMUT_SPARK_WIDTH;
        ang += dang + ex->btlfx_randCentered(0xC0);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_WHITE_BOLT_2), 0, 0, frame, 0x1000, fade, BTLFX_ABR_ADD, (u8 *)bolt, 0);
}

static void fxm_drawOffsetTrail2(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e3_1_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->pts4;
    bolt->count2 = FX_BAHAMUT_BOLT_POINTS - 1;
    bolt->lastSlot0 = FX_BAHAMUT_BOLT_POINTS - 1;
    bolt->head1 = 0;
    for (i = 0; i < FX_BAHAMUT_BOLT_POINTS; i++) {
        t = 0x1000 - (i << 12) / (FX_BAHAMUT_BOLT_POINTS - 1);
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
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_PURPLE_BOLT), 0, 0, frame, 0x1000, fade, BTLFX_ABR_ADD, (u8 *)bolt, 0);
}

void fxm_drawOffsetBranch(Unk80100C78Entry *entry, Fx0e31Bolt *src, Fx0e31Bolt *dst, s32 fade) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e3_1_env);
    SVECTOR mid;
    SVECTOR *base = src->pts4;

    base += ex->btlfx_rand() % 56;

    ex->btlfx_lerpVec((s16 *)base, (s16 *)(base + 1), 0x4000, (s16 *)&mid);
    fxm_drawOffsetTrail1(entry, dst, base, &mid, 0x80, ex->btlfx_rand(), 0x37, 0, fade);
}

static void fxm_drawOffsetTrail3(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e3_1_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->pts4;
    bolt->count2 = FX_BAHAMUT_LIGHTNING_POINTS - 1;
    bolt->lastSlot0 = FX_BAHAMUT_LIGHTNING_POINTS - 1;
    bolt->head1 = 0;
    for (i = 0; i < FX_BAHAMUT_LIGHTNING_POINTS; i++) {
        t = 0x1000 - (i << 12) / (FX_BAHAMUT_LIGHTNING_POINTS - 1);
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
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_LIGHTNING_BOLT), 0, 0, frame, 0x1000, fade, BTLFX_ABR_ADD, (u8 *)bolt, 0);
}

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e3_1_env);
    Fx0e31Rgbc colFlat;
    Fx0e31Rgbc colLit;
    Fx0e31Rgbc colDark;
    MATRIX m;
    SVECTOR quad[4];
    s32 otz;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 seg;
    s32 j;
    s32 row;
    s32 rowY;
    s32 span;
    s32 y0;
    s32 y1;
    s32 bulge0;
    s32 bulge1;
    s32 rad0;
    s32 rad1;
    s32 ang1;
    s32 ft;
    s32 tile;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 nextJ;
    s32 ratio1;
    s32 ratio0;

    gte_SetRotMatrix(fx_0e3_1_env->viewMatrix);
    gte_SetTransMatrix(fx_0e3_1_env->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)fx_0e3_1_env->prim;
    *(s32 *)&colDark = 0;
    ft4 = (POLY_FT4 *)(gt4 + 2);
    fx_0e3_1_env->prim = (u8 *)(ft4 + 46);
    colFlat.r = colFlat.g = colFlat.b = bright;
    colLit.r = colLit.g = colLit.b = bright;
    colFlat.code = abr != BTLFX_ABR_OPAQUE ? GPU_POLY_FT4 | GPU_ABE : GPU_POLY_FT4;
    colDark.code = abr != BTLFX_ABR_OPAQUE ? GPU_POLY_GT4 | GPU_ABE : GPU_POLY_GT4;
    tpage |= TPAGE_ABR(abr);
    colLit.code = colDark.code;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    seg = 0;
    rowY = 0;
    w0 >>= 1;
    w1 >>= 1;
    span = dy * 3;
    for (row = 0; row < 3; row++, rowY += dy) {
        for (j = 0; j < 16; j++, seg++, ang += dang << 8) {
            nextJ = (j + 1) << 8;
            y0 = rowY + (((j << 8) * dy) >> 12);
            y1 = rowY + ((nextJ * dy) >> 12);
            /* one variable for span / 2 and the second ratio: the original recomputes span >> 1 every iteration */
            ratio1 = span >> 1;
            ratio0 = ((y0 + ratio1) << 12) / span;
            ratio1 = ((y1 + ratio1) << 12) / span;
            bulge0 = ex->math_sin(ratio0 >> 1);
            bulge1 = ex->math_sin(ratio1 >> 1);
            rad0 = ex->btlfx_lerp(r0, r1, bulge0);
            rad1 = ex->btlfx_lerp(r0, r1, bulge1);
            quad[0].vx = quad[1].vx = (ex->math_cos(ang) * rad0) >> 12;
            quad[0].vy = quad[1].vy = (ex->math_sin(ang) * rad0) >> 12;
            quad[0].vz = y0 - ex->btlfx_lerp(w0, w1, bulge0);
            quad[1].vz = y0 + ex->btlfx_lerp(w0, w1, bulge1);
            ang1 = ang + (dang << 8);
            quad[2].vx = quad[3].vx = (ex->math_cos(ang1) * rad1) >> 12;
            quad[2].vy = quad[3].vy = (ex->math_sin(ang1) * rad1) >> 12;
            quad[2].vz = y1 - ex->btlfx_lerp(w0, w1, bulge0);
            quad[3].vz = y1 + ex->btlfx_lerp(w0, w1, bulge1);
            gte_ldv3c(quad);
            gte_rtpt();
            tile = seg & 7;
            u0 = u + (tile << 4);
            u1 = u0 + 0x10;
            if (tile == 7) {
                u1 = u0 + 0xF;
            }
            v0 = v;
            v1 = v0 + 0x3F;
            gte_nclip();
            if (seg == 0) {
                ft = 0;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colDark;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colLit;
            } else if (seg >= 47) {
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colDark;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colLit;
            } else {
                ft = 1;
                *(s32 *)&ft4->r0 = *(s32 *)&colFlat;
            }
            gte_stopz(&otz);
            if (otz != 0) {
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
        }
    }
}

void fxm_rollTowardEye(VECTOR *pos, SVECTOR *rot) {
    Unk800F78A4 *env = fx_0e3_1_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pt;
    s32 ignored;
    s32 roll;

    pt.vx = pos->vx;
    pt.vy = pos->vy;
    pt.vz = pos->vz;
    ex->btlfx_toLocalPolar(&env->camera->eye, &pt, rot, (s32)&ignored, (s32)&ignored, &roll);
    rot->vz = -roll + 0x400;
}

INCLUDE_RODATA("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", fx_0e3_1_headerPtr);

/* Matching levers (pass 3): call results passed to a later call live in single-set variables (sched1's birthing
 * boost then puts the v0 copy right before that call); pseudos that must stay after a call don't cross one (case 2's
 * ya/yb); shared variables split by role (the original reuses registers per role); a dead struct initializer gets
 * the unused spill slot at 0x280; the frame == 0x12 matrix shares s1/r1/a1's stack (union u1); dead divisions keep
 * their magic constant where the original loads it. */
s32 fx_0e3_1_op80(s32 mode, Fx0e31Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX boneMat;
    MATRIX handMat;
    MATRIX handRotMat;
    SVECTOR from;
    SVECTOR to;
    MATRIX camMat;
    MATRIX tipMat;
    MATRIX tipRotMat;
    union {
        MATRIX m; /* the frame == 0x12 bone matrix */
        struct {
            VECTOR s1;
            SVECTOR r1;
            SVECTOR a1;
        } v;
    } u1;
    SVECTOR chargeTo;
    SVECTOR discRot;
    SVECTOR discAhead;
    VECTOR discScale;
    VECTOR discPos;
    SVECTOR strikeFrom;
    SVECTOR strikeTo;
    SVECTOR burstRot;
    VECTOR burstScale;
    CVECTOR col;
    SVECTOR gridRot;
    VECTOR gridScale;
    SVECTOR burstFrom;
    SVECTOR burstTo;
    SVECTOR ringRot;
    VECTOR ss;
    SVECTOR columnRot;
    VECTOR columnScale;
    VECTOR columnPos;
    SVECTOR pillarRot;
    VECTOR pillarScale;
    VECTOR pillarPos;
    SVECTOR flashRot;
    VECTOR flashScale;
    VECTOR flashPos;
    SVECTOR boltFrom;
    SVECTOR boltTo;
    SVECTOR flatRingRot;
    s32 frame;
    s32 t;
    s32 tC;
    s32 tB;
    s32 tA;
    s32 arcAmp;
    s32 fade;
    s32 tint;
    s32 shake;
    s32 chargeRingScale;
    s32 chargeRingFade;
    s32 flashRingScale;
    s32 kk;
    s32 k2k;
    s32 spinRingScale;
    s32 unused;
    s32 discCos;
    s32 x2k;
    s32 flareScale;
    s32 haloScale;
    s32 glowScale;
    s32 quakeAmp;
    s32 burstPhase;
    s32 burstRingScale;
    s32 burstGlowScale;
    s32 fadeLevel;
    s32 coreScale;
    s32 columnRgb;
    s32 boltAmp;
    s32 spreadRadius;
    s32 ringScale;
    s32 unused2;
    s32 shakeTint;
    s32 flareFade;
    s32 burstCos;
    s32 burstRingFade;
    s32 flashTint;
    s32 cylCos;
    s32 columnCos;
    s32 pillarCos;
    s32 flashCos;
    s32 boltWidth;
    s32 sparkWidth;
    s32 discRgb;
    s32 ringFade;
    s32 rndAng;
    s32 sparkRing;
    s32 waveAng;
    s32 unused3;
    s32 haloAng;
    s32 quake;
    s32 ringFrame;
    s32 shrink;
    s32 burstAng;
    s32 afterT;
    s32 cylPhase;
    s32 columnAng;
    s32 pillarFrame;
    s32 flashT;
    s32 ringAng;
    s32 arcAng;
    s32 shakeLevel;
    s32 chargeFade;
    s32 strikeFade;
    s32 cylWidth;
    s32 innerRadius;
    s32 chargeArcs;
    s32 cylRise;
    s32 chargeAmp;
    s32 burstAmp;
    s32 pillarWidth;
    s32 unused4;
    s32 t_s2;
    s32 i;
    s32 boltIdx;
    s32 burstBolt;
    s32 j;
    s32 strikeBranch;
    s32 branch;
    s32 ang;
    s32 ang3;
    s32 ang1;
    s32 ang4;
    s32 ang2;
    Fx0e31Bolt *bl;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = sizeof(Fx0e31Work80);
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0e3_1_env = env;
        work->modelC4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_SMOKE_ARC_2), 1);
        work->modelC8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_FIRE_DOME), 1);
        work->modelCC = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_SMOKE_RING), 1);
        work->model7C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_CLOUD_SPHERE), 0);
        work->modelA0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_BLUE_SLIVER), 0);
        work->modelD0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_SMOKE_SHELL), 1);
        work->modelD4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_FLAME_STRIP), 0);
        work->modelD8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_RED_STRIP), 0);
        work->modelDC = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_FIRE_RING), 1);
        work->modelE0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_GREY_CONE), 0);
        BTLFX_COPY_SVEC(&work->center8, &fx_0e3_1_origin);
        work->rot28.vy = 0x800;
        work->rot28.vx = 0;
        work->rot28.vz = 0;
        work->summonScale1C.vz = work->summonScale1C.vy = work->summonScale1C.vx = 0x1800;
        work->summonPos10.vx = work->center8.vx;
        work->summonPos10.vy = work->center8.vy;
        work->summonPos10.vz = work->center8.vz;
        work->summonPos10.vy -= 0x3000;
        work->summonPos10.vz -= 0x2000;
        ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion198, SUMMON(env)->summonModel60);
        ex->Hi_SetSummonMotFrame(SUMMON(env)->summonModel60, 0);
        work->scale94.vz = work->scale94.vy = work->scale94.vx = 0x1000;
        work->rot80.vx = -0x400;
        work->rot80.vy = 0;
        work->rot80.vz = 0;
        work->pos88.vx = work->summonPos10.vx;
        work->pos88.vy = work->summonPos10.vy;
        work->pos88.vz = work->summonPos10.vz;
    } else {
        {
        struct { s32 x; } dq = { frame }; /* dead: its CLOBBER takes the original's unused spill slot 0x280 */
        }
        arcAmp = 0;
        fade = 0;
        tint = 0;
        shake = 0;
        frame = entry->frame;
        switch (work->state0) {
            s32 y6_l; /* declared here: its pseudo (spill slot) comes after the function-level ones */
        case 0:
            t = (frame << 12) / 35;
            ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C, SUMMON(env)->summonModel60, 1);
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&boneMat);
            ex->btlfx_matrixPointAhead(&boneMat, 0x40, (s32)&work->camTarget40);
            work->camTarget40.vy += 0x1000 - ex->math_sin(t >> 2);
            env->camera->points[3].vx = work->camTarget40.vx;
            env->camera->points[3].vy = work->camTarget40.vy;
            arcAmp = 0x100;
            env->camera->points[3].vz = work->camTarget40.vz;
            if (frame >= 0x23) {
                work->state0 = 1;
                entry->frame = -1;
                work->emit60 = NULL;
            }
            break;
        case 1:
            rndAng = frame / 6; /* dead: its magic constant, shared with the / 6 below, stays at the top of the case */
            arcAmp = 0x80;
            if (frame < 6) {
                fade = (frame << 11) / 6;
            } else if (frame < 0x2C) {
                fade = ex->math_cos(((frame - 6) << 10) / 38) >> 1;
            }
            work->rot80.vz += 8;
            ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C, SUMMON(env)->summonModel60, 0);
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&handMat);
            ex->btlfx_matrixPointAhead(&handMat, 0x5C, (s32)&work->pos30);
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&handRotMat);
            ex->btlfx_matrixAngles(&handRotMat, &work->handRot58.vx);
            work->handRot58.vx -= 0x800;
            if (frame >= 0x10) {
                s32 frameCount = work->frame4;
                from.vx = work->pos30.vx;
                tA = (frame - 0x10) << 7;
                from.vy = work->pos30.vy;
                from.vz = work->pos30.vz;
                chargeAmp = (tA >> 7) + 0x10;
                ang1 = frameCount << 8;
                chargeArcs = (ex->math_sin(tA >> 2) * 6) >> 12;
                for (i = 0; i < chargeArcs; i++) {
                    rndAng = ex->btlfx_rand();
                    to.vx = work->pos30.vx;
                    to.vy = work->pos30.vy;
                    to.vz = work->pos30.vz;
                    to.vy += ex->math_sin(rndAng) >> 2;
                    to.vx += ex->math_cos(rndAng) >> 2;
                    fxm_drawOffsetTrail0(entry, (Fx0e31Bolt *)&fx_0e3_1_arcs[i], &from, &to, chargeAmp, chargeAmp, ang1, 0xFF, 0, tA);
                }
            }
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&camMat);
            ex->btlfx_matrixPointAhead(&camMat, 0x40, (s32)&work->camTarget40);
            env->camera->points[3].vx = work->camTarget40.vx;
            env->camera->points[3].vy = work->camTarget40.vy;
            env->camera->points[3].vz = work->camTarget40.vz;
            if (frame == 0) {
                work->emit60 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_FIRE_RING_BURST), (s32)&work->pos30);
                work->emit64 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_FIRE_SPARKS), (s32)&work->pos30);
            }
            if (work->emit60 != NULL) {
                work->emit60->rot.vx = work->handRot58.vx;
                work->emit60->rot.vy = work->handRot58.vy;
                work->emit60->rot.vz = work->handRot58.vz;
            }
            if (work->emit64 != NULL) {
                work->emit64->rot.vx = work->handRot58.vx;
                work->emit64->rot.vy = work->handRot58.vy;
                work->emit64->rot.vz = work->handRot58.vz;
            }
            env->otZ = 0x80;
            t_s2 = ((frame << 12) / 48) >> 2;
            chargeRingScale = (ex->math_sin(t_s2) >> 1) + 0x800;
            chargeRingFade = ex->math_sin(t_s2);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_CHARGE_RINGS), &work->pos30, NULL, chargeRingScale >> 1, 0, BTLFX_ABR_ADD, chargeRingFade);
            if (frame >= 0x2B) {
                tB = (((frame - 0x2B) << 12) / 5) >> 2;
                flashRingScale = ex->math_sin(tB) / 3;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_WHITE_FLASH_RING), &work->pos30, NULL, flashRingScale, 0, BTLFX_ABR_ADD, 0x1000);
                ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_LENS_FLARE), &work->pos30, 0xB, ex->math_sin(tB));
            }
            if (frame >= 0x10) {
                sparkRing = ex->math_sin(((frame - 0x10) << 7) >> 2);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_VIOLET_RINGS), &work->pos30, NULL, sparkRing, 0, BTLFX_ABR_ADD, sparkRing);
            }
            if (frame == 0x26) {
                Pool6CEntry *sparkEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_PURPLE_GLOW), (s32)&work->pos30);
                if (sparkEmitter != NULL) {
                    sparkEmitter->otZ = 0x80;
                }
            }
            env->otZ = 0;
            if (frame == 0x10) {
                work->emit68 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_ENERGY_ORBS), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame == 0x2F) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_OPAQUE, 0x80, 0, 0);
            }
            if (frame >= 0x28) {
                tC = ((frame - 0x28) << 12) >> 5;
                { s32 q4b;
                spinRingScale = (ex->math_cos(tC) * 3) >> 1;
                q4b = ex->math_sin(tC);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_SPIN_RING), &work->pos30, &work->rotBC, spinRingScale, 0, BTLFX_ABR_ADD, q4b); }
            }
            if (frame >= 0x30) {
                work->state0 = 2;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion19C, SUMMON(env)->summonModel60);
                ex->Hi_SetSummonMotFrame(SUMMON(env)->summonModel60, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, 0, 0xFF, 0xFF);
            }
            break;
        case 2:
            t = (frame << 12) / 28;
            if (frame >= 0xE) {
                tint = ((frame - 0xE) << 12) / 14;
            }
            ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C, SUMMON(env)->summonModel60, 1);
            if (frame == 1) {
                ex->Hi_ModifySummonModelRGB(SUMMON(env)->summonModel60, -0x60, -0x60, -0x60);
            }
            if (frame == 0) {
                ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&tipMat);
                ex->btlfx_matrixPointAhead(&tipMat, 0x80, (s32)&work->camTarget40);
                ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&tipRotMat);
                ex->btlfx_matrixAngles(&tipRotMat, &work->rotBC.vx);
            }
            if (frame < 0x12) {
                ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&tipMat);
                ex->btlfx_matrixPointAhead(&tipMat, 0x5C, (s32)&work->pos30);
                ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&tipRotMat);
                ex->btlfx_matrixPointAhead(&tipRotMat, 0x80, (s32)&work->camTarget40);
            } else {
                if (frame == 0x12) {
                    ex->Hi_GetSummonBoneMatrix(SUMMON(env)->summonModel60, 0x3D, (BtlBone *)&u1.m);
                    ex->btlfx_matrixPointAhead(&u1.m, 0x80, (s32)&work->swingBase48);
                }
                ex->btlfx_pointAhead(&work->swingBase48.vx, &work->rotBC.vx,
                                  -((ex->math_sin(((frame - 0xE) << 10) / 14) * 3) / 2), &work->camTarget40.vx);
            }
            env->camera->points[3].vx = work->camTarget40.vx;
            env->camera->points[3].vy = work->camTarget40.vy;
            env->camera->points[3].vz = work->camTarget40.vz;
            if (frame == 1) {
                work->scaleB0.vx = work->scaleB0.vy = 0x249;
            } else if (frame < 0x11) {
                work->scaleB0.vx = work->scaleB0.vy = ((0x1000 - ex->math_sin((frame << 8) >> 2)) * 4) + 0x400;
            } else {
                work->scaleB0.vx = work->scaleB0.vy = 0x400;
            }
            work->scaleB0.vz = 0xAAA;
            work->posA4.vx = work->pos30.vx;
            work->posA4.vy = work->pos30.vy;
            work->posA4.vz = work->pos30.vz;
            fxm_rollTowardEye((VECTOR *)&work->posA4, &work->rotBC);
            ex->Hi_DrawEffModel(&work->rotBC, (VECTOR *)&work->posA4, (s32)&work->scaleB0, work->modelA0);
            if (frame < 0x18 && frame != 1) {
                waveAng = (frame << 10) / 24;
                u1.v.s1.vx = u1.v.s1.vy = ex->math_sin(waveAng) / 2 + 0x800;
                u1.v.s1.vz = 0x1000;
                u1.v.r1.vx = work->rotBC.vx;
                u1.v.r1.vy = work->rotBC.vy;
                u1.v.r1.vz = frame << 5;
                ex->Hi_DrawEffModel(&u1.v.r1, (VECTOR *)&work->posA4, (s32)&u1.v.s1, work->modelE0);
                ex->Hi_ModifyEffModelRGB(work->modelE0, ((ex->math_cos(waveAng) - 0x800) >> 5) - 0x80,
                                         ((ex->math_cos(waveAng) - 0x800) >> 5) - 0x80, ((ex->math_cos(waveAng) - 0x800) >> 5) - 0x80);
            }
            BTLFX_COPY_SVEC(&u1.v.a1, &work->pos30);
            ex->btlfx_pointAhead(&u1.v.a1.vx, &work->rotBC.vx, -(t + 0x1000), &chargeTo.vx);
            for (kk = 0, ang3 = -work->frame4 << 7; kk < 3; kk++, ang3 += 0x554) {
                chargeFade = ex->btlfx_randCentered(0x400) + 0xC00;
                fxm_drawOffsetTrail2(entry, &fx_0e3_1_bolts[kk], &u1.v.a1, &chargeTo, 0x7C, 0x200, ang3, 0xC0, 0, chargeFade, 0x1D);
                for (branch = 0; branch < 4; branch++) {
                    fxm_drawOffsetBranch(entry, &fx_0e3_1_bolts[kk], (Fx0e31Bolt *)((Fx0e31Bolt5 *)fx_0e3_1_arcs + branch), chargeFade);
                }
            }
            if (frame < 0x21) {
                s32 ya;
                s32 yb;
                ya = frame << 12;
                yb = ya >> 5;
                discCos = ex->math_cos(ya >> 7);
                i = 0;
                discRot.vx = work->rotBC.vx;
                discRot.vy = work->rotBC.vy;
                discRot.vz = work->rotBC.vz;
                discScale.vz = 0;
                discScale.vx = discScale.vy = yb * 2 + 0x900;
                discRot.vz = (frame << 5) + 0x400;
                ex->btlfx_pointAhead(&work->pos30.vx, &work->rotBC.vx, -0x80, &discAhead.vx);
                discRgb = (discCos >> 5) - 0x80;
                discPos.vx = discAhead.vx;
                discPos.vy = discAhead.vy;
                discPos.vz = discAhead.vz;
                for (; i < 3; i++) {
                    ex->Hi_DrawEffModel(&discRot, &discPos, (s32)&discScale, work->modelC4);
                    ex->Hi_ModifyEffModelRGB(work->modelC4, discRgb, discRgb, discRgb);
                    discScale.vx = discScale.vy = (discScale.vx * 3) >> 2;
                    discRot.vz += 0x550;
                }
            }
            env->otZ = 0x190;
            if (frame < 0x10 && frame != 1) {
                flareScale = (ex->math_cos(t >> 2) >> 2) + 0x555;
                flareFade = ex->math_cos((frame << 8) >> 2);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_WHITE_FLASH_RING), &work->pos30, NULL, flareScale, 0, BTLFX_ABR_ADD, flareFade);
                work->rotBC.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_BLUE_RINGS), &work->pos30, &work->rotBC, 0x1000,
                                  frame, BTLFX_ABR_ADD, flareFade);
                ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_SPARKLE_RING), &work->pos30, &work->rotBC, 0x2000,
                                      frame, 1, flareFade);
                ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_LENS_FLARE), &work->pos30, frame - 1, flareFade);
            }
            if (frame < 0x11) {
                haloScale = (ex->math_cos(t >> 2) >> 2) + 0x800;
            } else {
                haloScale = 0x800;
            }
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_HALO_RINGS), &work->pos30, NULL, (haloScale * 3) >> 1, 0, BTLFX_ABR_ADD,
                              0x1000);
            shake = t;
            haloAng = t >> 2;
            glowScale = ex->math_cos(haloAng) + 0x1000;
            {
                s32 qa;
                qa = ex->math_cos(haloAng);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_VIOLET_RINGS), &work->pos30, NULL, glowScale, 0, BTLFX_ABR_ADD, qa);
            }
            env->otZ = 0;
            if (frame >= 0x1C) {
                work->state0 = 3;
                entry->frame = -1;
            }
            break;
        case 3:
            tint = 0x1000;
            if (frame == 0) {
                work->pos30.vx = fx_0e3_1_origin.vx;
                work->pos30.vy = fx_0e3_1_origin.vy;
                work->pos30.vz = fx_0e3_1_origin.vz;
                work->pos30.vy -= 0x1000;
                work->rotBC.vx = -0x400;
                work->rotBC.vy = 0;
                work->rotBC.vz = 0;
                work->posA4.vx = fx_0e3_1_origin.vx;
                work->posA4.vy = fx_0e3_1_origin.vy;
                work->posA4.vz = fx_0e3_1_origin.vz;
                work->posA4.vy -= 0x1000;
                work->scaleB0.vx = 0x1200;
                work->scaleB0.vy = 0x1200;
                work->scaleB0.vz = 0x1555;
            }
            work->camTarget40.vy = work->pos30.vy;
            work->camTarget40.vx = work->pos30.vx;
            work->camTarget40.vz = work->pos30.vz;
            work->camTarget40.vy -= 0x1000;
            env->camera->points[3].vx = work->camTarget40.vx;
            env->camera->points[3].vy = work->camTarget40.vy;
            env->camera->points[3].vz = work->camTarget40.vz;
            fxm_rollTowardEye((VECTOR *)&work->posA4, &work->rotBC);
            ex->Hi_DrawEffModel(&work->rotBC, (VECTOR *)&work->posA4, (s32)&work->scaleB0, work->modelA0);
            BTLFX_COPY_SVEC(&strikeFrom, &work->pos30);
            ex->btlfx_pointAhead(&strikeFrom.vx, &work->rotBC.vx, -0x2800, &strikeTo.vx);
            for (k2k = 0, ang2 = -work->frame4 << 7; k2k < 3; k2k++, ang2 += 0x554) {
                strikeFade = ex->btlfx_randCentered(0x400) + 0xC00;
                fxm_drawOffsetTrail2(entry, &fx_0e3_1_bolts[k2k], &strikeFrom, &strikeTo, 0x400, 0x528, ang2, 0xC0, 0, strikeFade, 0x84);
                for (strikeBranch = 0; strikeBranch < 4; strikeBranch++) {
                    fxm_drawOffsetBranch(entry, &fx_0e3_1_bolts[k2k], (Fx0e31Bolt *)((Fx0e31Bolt5 *)fx_0e3_1_arcs + strikeBranch), strikeFade);
                }
            }
            shake = 0x1000;
            if (frame >= 2) {
                work->state0 = 4;
                entry->frame = -1;
            }
            break;
        case 4:
            t = (frame << 12) / 14;
            if (frame == 5) {
                ex->btlfx_playFileSound(0x28, entry->file, -1);
                ex->btlfx_playFileSound(0x2A, entry->file, -1);
                ex->btlfx_playFileSound(0x2B, entry->file, -1);
                ex->btlfx_playFileSound(0x2C, entry->file, -1);
            }
            shake = ex->math_cos(t >> 2);
            if (frame == 0) {
                work->pos30.vx = fx_0e3_1_origin.vx;
                work->pos30.vy = fx_0e3_1_origin.vy;
                work->pos30.vz = fx_0e3_1_origin.vz;
                work->posA4.vx = fx_0e3_1_origin.vx;
                work->posA4.vy = fx_0e3_1_origin.vy;
                work->posA4.vz = fx_0e3_1_origin.vz;
            }
            if (frame < 4) {
                tint = 0x1000 - ((frame << 12) >> 2);
                work->scaleB0.vx = work->scaleB0.vy = 0x1200;
                work->scaleB0.vz = 0x1555;
            } else if (frame == 4) {
                work->scaleB0.vx = work->scaleB0.vy = 0x4000;
            } else if ((shrink = frame - 4) < 0xB) {
                shrink = (shrink << 12) / 10;
                work->scaleB0.vx = work->scaleB0.vy = ((0x1000 - shrink) * 3) >> 1;
            }
            if (frame < 5) {
                work->camTarget40.vx = work->pos30.vx;
                work->camTarget40.vy = work->pos30.vy;
                work->camTarget40.vz = work->pos30.vz;
                work->camTarget40.vy -= 0x180 + (((0x1000 - ((frame << 12) >> 2)) * 3) >> 1);
                env->camera->points[3].vx = work->camTarget40.vx;
                env->camera->points[3].vy = work->camTarget40.vy;
                env->camera->points[3].vz = work->camTarget40.vz;
            }
            if (frame == 4) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0x20, 0x80, 0xFF);
            } else if ((u32)(frame - 5) < 7) {
                quake = 0x1000 - (((frame - 5) << 12) / 7);
                quakeAmp = quake >> 5;
                work->camTarget40.vx = fx_0e3_1_origin.vx + ex->btlfx_randCentered(quakeAmp);
                work->camTarget40.vy = fx_0e3_1_origin.vy + ex->btlfx_randCentered(quakeAmp);
                {
                    s32 dd = (quake >> 6) * ((frame & 1) * 2 - 1) - 0x200;
                    work->camTarget40.vy = fx_0e3_1_origin.vy + dd;
                }
                env->camera->points[3].vx = work->camTarget40.vx;
                env->camera->points[3].vy = work->camTarget40.vy;
                env->camera->points[3].vz = work->camTarget40.vz;
            }
            if (work->scaleB0.vx != 0) {
                fxm_rollTowardEye((VECTOR *)&work->posA4, &work->rotBC);
                ex->Hi_DrawEffModel(&work->rotBC, (VECTOR *)&work->posA4, (s32)&work->scaleB0, work->modelA0);
            }
            if (frame >= 4) {
                s32 k7a;
                burstRot.vx = 0;
                burstRot.vz = 0;
                k7a = ((frame - 4) << 12) / 10;
                burstPhase = k7a >> 2;
                burstRot.vy = k7a >> 3;
                burstScale.vx = burstScale.vz = burstPhase + 0x555;
                burstScale.vy = (k7a >> 1) + 0x400;
                burstCos = ex->math_cos(burstPhase);
                ex->Hi_DrawEffModel(&burstRot, (VECTOR *)&work->posA4, (s32)&burstScale, work->modelC8);
                ex->Hi_ModifyEffModelRGB(work->modelC8, (burstCos >> 5) - 0x80, (burstCos >> 5) - 0x80, (burstCos >> 5) - 0x80);
                burstScale.vx = burstScale.vz = burstScale.vx * 2;
                burstScale.vy >>= 2;
                ex->Hi_DrawEffModel(&burstRot, (VECTOR *)&work->posA4, (s32)&burstScale, work->modelC8);
                ex->Hi_ModifyEffModelRGB(work->modelC8, (burstCos >> 6) - 0x80, (burstCos >> 6) - 0x80, (burstCos >> 6) - 0x80);
            }
            if (frame == 0) {
                work->emit70 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_BLUE_SPARKS), (s32)&work->pos30);
                work->emit74 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_BLUE_SPARKS_2), (s32)&work->pos30);
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_ORANGE_STREAKS), (s32)&work->pos30);
            }
            col.r = col.g = col.b = 0x37;
            gridScale.vx = 0x1400;
            gridScale.vy = 0x1400;
            gridScale.vz = 0x1000;
            gridRot.vx = 0;
            gridRot.vy = 0;
            gridRot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &gridRot, &gridScale, &col, 1, NULL);
            BTLFX_COPY_SVEC(&burstFrom, &work->pos30);
            ex->btlfx_pointAhead(&burstFrom.vx, &work->rotBC.vx, t - 0x2800, &burstTo.vx);
            burstAmp = (t >> 2) + 0x400;
            for (burstBolt = 0, ang4 = work->frame4 << 8; burstBolt < 3; burstBolt++, ang4 += 0x554) {
                fxm_drawOffsetTrail2(entry, &fx_0e3_1_bolts[burstBolt], &burstFrom, &burstTo, burstAmp, burstAmp, ang4, 0xC0, 0, ex->btlfx_randCentered(0x400) + 0xC00,
                              0x84);
            }
            env->otZ = 0x200;
            burstAng = t >> 2;
            burstRingScale = (ex->math_sin(burstAng) * 2) + 0x1000;
            burstRingFade = (ex->math_cos(burstAng) >> 1) + 0x800;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_HALO_RINGS), &work->pos30, NULL, burstRingScale, 0, BTLFX_ABR_ADD, burstRingFade);
            burstGlowScale = (ex->math_sin(burstAng) * 3) + 0x1000;
            {
                s32 qb;
                qb = ex->math_cos(burstAng);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_VIOLET_RINGS), &work->pos30, NULL, burstGlowScale, 0, BTLFX_ABR_ADD, qb);
            }
            if (frame >= 4) {
                ringFrame = frame - 4;
                if (ringFrame < 0xA) {
                    ringRot.vx = 0x400;
                    ringRot.vy = 0;
                    ringRot.vz = 0;
                    ringRot.pad = 1;
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_BLUE_RINGS), &work->pos30, &ringRot, 0x4000, ringFrame, BTLFX_ABR_ADD,
                                      0x1000);
                }
            }
            env->otZ = 0;
            if (frame >= 0xE) {
                work->state0 = 5;
                entry->frame = -1;
                ex->btlfx_stopEmitter(work->emit70);
                ex->btlfx_stopEmitter(work->emit74);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, 0, 0x80, 0xFF);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_FIRE_BURST), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->otZ = 0x300;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_EMBERS), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->otZ = 0x300;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_PURPLE_GLOW_2), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->otZ = 0x400;
                }
                work->sound78 = ex->btlfx_playFileSound(0x30, entry->file, -1);
                work->sound7A = ex->btlfx_playFileSound(0x31, entry->file, -1);
                ex->btlfx_playFileSound(0x2D, entry->file, -1);
                ex->btlfx_playFileSound(0x2E, entry->file, -1);
                env->camera->points[2].vx = 0;
                env->camera->points[2].vy = -0xC8;
                env->camera->points[2].vz = 0x300;
            }
            break;
        case 5:
            t = (frame << 12) / 77;
            if (frame == 0) {
                work->pos30.vx = fx_0e3_1_origin.vx;
                work->pos30.vy = fx_0e3_1_origin.vy;
                work->pos30.vz = fx_0e3_1_origin.vz;
                work->posA4.vx = fx_0e3_1_origin.vx;
                work->posA4.vy = fx_0e3_1_origin.vy;
                work->posA4.vz = fx_0e3_1_origin.vz;
                work->rotBC.vx = -0x400;
                work->rotBC.vy = 0;
                work->rotBC.vz = 0;
                work->scaleB0.vx = 0x4000;
                work->scaleB0.vy = 0x4000;
                work->scaleB0.vz = 0x4000;
            }
            work->camTarget40.vx = work->pos30.vx;
            work->camTarget40.vy = work->pos30.vy;
            work->camTarget40.vz = work->pos30.vz;
            work->camTarget40.vy = -0x2D4 - ((t * 0x76C) >> 12);
            env->camera->points[3].vx = work->camTarget40.vx;
            env->camera->points[3].vy = work->camTarget40.vy;
            env->camera->points[3].vz = work->camTarget40.vz;
            if (frame < 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_SUB, 0xFF - (frame << 7), 0xFF - (frame << 7), 0xFF);
            } else if (frame == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0xFF, 0xFF, 0xFF);
            } else if (frame < 0x13) {
                fadeLevel = 0x1000 - (((frame - 3) << 12) >> 4);
                fadeLevel = (fadeLevel * 0xA0) >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, fadeLevel, fadeLevel, fadeLevel);
            }
            if (frame == 0x14) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0xD7, 0x64, 0x64);
            }
            if (frame == 0x1D) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0x97, 0x4D, 0x4D);
            }
            if (frame == 0x20) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0x80, 0x40, 0x40);
            }
            if (frame < 0x49) {
                if (frame < 7) {
                    coreScale = 0x2000 - ex->math_sin((frame << 10) / 7);
                } else {
                    coreScale = 0x7800 - ((frame - 7) * 0x1124) / 65 * 7;
                }
                if ((u32)(frame - 5) < 7) {
                    flashTint = ex->math_sin(((frame - 5) << 11) >> 12) * 0xF5;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 0x400), BTLFX_ABR_ADD, 0, flashTint >> 14, flashTint >> 12);
                }
                work->scaleB0.vx = work->scaleB0.vy = coreScale;
                fxm_rollTowardEye((VECTOR *)&work->posA4, &work->rotBC);
                work->rotBC.vz -= 0x400;
                ex->Hi_DrawEffModel(&work->rotBC, (VECTOR *)&work->posA4, (s32)&work->scaleB0, work->modelD0);
                ex->Hi_DrawEffModel(&work->rotBC, (VECTOR *)&work->posA4, (s32)&work->scaleB0, work->modelD4);
            }
            if (frame >= 0x3B) {
                afterT = ((frame - 0x3B) << 12) / 18;
                work->scaleB0.vx = work->scaleB0.vy = 0x1000;
                fxm_rollTowardEye((VECTOR *)&work->posA4, &work->rotBC);
                work->rotBC.vz += 0x400;
                ex->Hi_DrawEffModel(&work->rotBC, (VECTOR *)&work->posA4, (s32)&work->scaleB0, work->modelD8);
                ex->Hi_ModifyEffModelRGB(work->modelD8, (ex->math_cos(afterT >> 2) >> 5) - 0x80, (ex->math_cos(afterT >> 2) >> 5) - 0x80,
                                         (ex->math_cos(afterT >> 2) >> 5) - 0x80);
            }
            cylPhase = t >> 2;
            innerRadius = (t >> 1) + 0xE00;
            cylCos = ex->math_cos(cylPhase);
            ss.vx = 0x1000;
            ss.vy = 0x1000;
            ss.vz = 0x1000;
            cylWidth = (cylCos >> 2) + 0x100;
            ringRot.vx = 0x400;
            ringRot.vy = 0;
            ringRot.vz = 0;
            cylRise = (((t >> 3) + 0x500) * 4) / 3;
            y6_l = ex->math_cos(cylPhase) >> 5;
            fxm_drawTexCylinder(&work->pos30, &ringRot, &ss, innerRadius, cylPhase + 0xA00, cylWidth >> 1, cylWidth, y6_l, 0x1D, 0x3E81, 0, 0xC0, -frame << 7, cylRise, 1, 1);
            fxm_drawTexCylinder(&work->pos30, &ringRot, &ss, innerRadius, cylPhase + 0xA00, cylWidth >> 1, cylWidth, y6_l, 0x1D, 0x3E81, 0, 0xC0, frame * 0xA0, cylRise, 1, -1);
            if (frame < 0x41) {
                columnAng = (frame << 6) >> 2;
                columnCos = ex->math_cos(columnAng) - 0x5DC;
                columnRot.vx = 0;
                columnRot.vz = 0;
                columnRot.vy = frame << 5;
                columnScale.vx = columnScale.vz = ((ex->math_sin(columnAng) * 3) >> 2) + 0xA00;
                columnScale.vy = ((frame << 6) + 0x300) * 2;
                columnPos.vx = work->pos30.vx;
                columnPos.vy = work->pos30.vy;
                columnPos.vz = work->pos30.vz;
                columnPos.vy -= (ex->math_sin(columnAng) * 0xFA0) >> 12;
                ex->Hi_DrawEffModel(&columnRot, &columnPos, (s32)&columnScale, work->modelCC);
                columnRgb = (columnCos >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->modelCC, columnRgb, columnRgb, columnRgb);
            }
            pillarRot.vx = 0;
            pillarRot.vz = 0;
            pillarRot.vy = frame << 6;
            pillarWidth = 0x4000;
            pillarPos.vx = work->pos30.vx;
            pillarPos.vy = work->pos30.vy;
            pillarPos.vz = work->pos30.vz;
            for (i = 0, j = 7; i < 6; j += 7, i++) {
                pillarFrame = frame - j;
                if ((u32)pillarFrame < 0x20) {
                    pillarCos = ex->math_cos((pillarFrame << 12) >> 7);
                    pillarScale.vx = pillarScale.vz = pillarWidth;
                    if (pillarFrame < 4) {
                        pillarScale.vy = ex->math_sin((pillarFrame << 10) >> 2) * 6;
                    } else {
                        pillarScale.vy = 0x6000 - ((pillarFrame - 4) << 8);
                    }
                    pillarScale.vy = (pillarScale.vy * (8 - i)) >> 3;
                    if (i < 2) {
                        pillarScale.vy = (pillarScale.vy * 5) >> 2;
                    }
                    ex->Hi_DrawEffModel(&pillarRot, &pillarPos, (s32)&pillarScale, work->modelDC);
                    ex->Hi_ModifyEffModelRGB(work->modelDC, (pillarCos >> 5) - 0x80, (pillarCos >> 5) - 0x80, (pillarCos >> 5) - 0x80);
                }
                pillarWidth = (pillarWidth * 9) >> 3;
                pillarRot.vy = (pillarRot.vy * 3) >> 2;
            }
            if (frame < 0x10) {
                flashT = frame << 12;
                flashRot.vy = frame * 0x37;
                flashRot.vx = 0;
                flashRot.vz = 0;
                flashScale.vx = flashScale.vz = (flashT >> 5) + 0x1200;
                {
                    s32 kb = flashT >> 6;
                    flashScale.vy = ex->math_sin(kb) + 0x1800;
                    flashCos = ex->math_cos(kb);
                }
                flashPos.vx = work->pos30.vx;
                flashPos.vy = work->pos30.vy;
                flashPos.vz = work->pos30.vz;
                ex->Hi_DrawEffModel(&flashRot, &flashPos, (s32)&flashScale, work->modelC8);
                ex->Hi_ModifyEffModelRGB(work->modelC8, (flashCos >> 5) - 0x80, (flashCos >> 5) - 0x80, (flashCos >> 5) - 0x80);
            }
            ((s32 *)&boltTo)[0] = ((s32 *)&boltFrom)[0] = ((s32 *)&work->pos30)[0];
            ((s32 *)&boltTo)[1] = ((s32 *)&boltFrom)[1] = ((s32 *)&work->pos30)[1];
            boltTo.vy -= 0x1800;
            boltAmp = 0x1000 - ((t * 0xDAC) >> 12);
            ang = (work->frame4 << 9) + (ex->btlfx_rand() & 0x1FF);
            boltWidth = (ex->math_cos(t >> 2) * 0xF5) >> 12;
            for (boltIdx = 0; boltIdx < 2; boltIdx++, ang += 0x800) {
                fxm_drawOffsetTrail2(entry, &fx_0e3_1_bolts[boltIdx], &boltFrom, &boltTo, (boltAmp * 3) >> 2, boltAmp, ang, 0xC0, 0, 0x1000, boltWidth);
            }
            if (frame < 0x26) {
                boltAmp = (boltAmp * 3) >> 1;
                boltWidth = (ex->math_cos(t >> 1) * 0xF5) >> 12;
                fxm_drawOffsetTrail2(entry, &fx_0e3_1_bolts[2], &boltFrom, &boltTo, (boltAmp * 3) >> 2, boltAmp, -ang, 0xC0, 0, 0x1000, boltWidth);
            }
            spreadRadius = 0x1000 - t;
            env->otZ = 0x300;
            ang = work->frame4 << 7;
            sparkWidth = ((ex->math_cos(t >> 2) * 0xEB) >> 12) + 0x20;
            for (boltIdx = 0; boltIdx < 5; boltIdx++, ang += 0x333 + ex->btlfx_randCentered(0x20)) {
                boltFrom.vx = work->pos30.vx;
                boltFrom.vy = work->pos30.vy;
                boltFrom.vz = work->pos30.vz;
                boltFrom.vx += (ex->math_sin(ang) * spreadRadius) >> 12;
                boltFrom.vz += (ex->math_cos(ang) * spreadRadius) >> 12;
                boltTo.vx = work->pos30.vx;
                boltTo.vy = work->pos30.vy;
                boltTo.vz = work->pos30.vz;
                boltTo.vx += ex->math_sin(ang);
                boltTo.vz += ex->math_cos(ang);
                boltTo.vy -= 0xF00;
                fxm_drawOffsetTrail3(entry, (Fx0e31Bolt *)&fx_0e3_1_arcs[boltIdx], &boltTo, &boltFrom, 0x80, 0x200, ang, 0xFF, 0,
                              ex->math_cos(t >> 2), sparkWidth);
            }
            env->otZ = 0;
            if (frame == 0x31) {
                BTLFX_COPY_SVEC(&work->pos38, &work->pos30);
                work->pos38.vy -= 0x400;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAHAMUT_EMBERS_2), (s32)&work->pos38);
                if (emitter != NULL) {
                    emitter->otZ = 0x180;
                }
            }
            env->otZ = 0x300;
            flatRingRot.vx = 0x400;
            flatRingRot.vy = 0;
            flatRingRot.vz = 0;
            flatRingRot.pad = 1;
            ringAng = t >> 2;
            ringScale = ex->math_cos(ringAng) + 0x1800;
            ringFade = ((ex->math_cos(ringAng) * 3) >> 2) + 0x400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_HALO_RINGS), &work->pos30, NULL, ringScale, 0, BTLFX_ABR_ADD, ringFade);
            ringScale = 0x5000 - t * 2;
            ringFade = ex->math_cos(ringAng);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_PINK_FLAT_RING), &work->pos30, &flatRingRot, ringScale, 0, BTLFX_ABR_ADD, ringFade);
            env->otZ = 0;
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_BAHAMUT_BACKDROP), frame);
            ex->btlfx_drawScrollMove(work->frame4 * 8, 0x340, 0x100, 0x20, 0xC0, 0x360, 0x100);
            if (frame >= 0x4D) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (arcAmp != 0) {
            arcAng = work->frame4 << 8;
            ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, ex->btlfx_rand() % FX_BAHAMUT_SUMMON_BONES, (u16 *)&boneMat);
            ex->Hi_GetSummonBonePos(SUMMON(env)->summonModel60, ex->btlfx_rand() % FX_BAHAMUT_SUMMON_BONES, (u16 *)((SVECTOR *)&boneMat + 1));
            fxm_drawOffsetTrail0(entry, (Fx0e31Bolt *)fx_0e3_1_arcs, (SVECTOR *)&boneMat, (SVECTOR *)&boneMat + 1, arcAmp, arcAmp, arcAng, 0x100, 0,
                          ex->btlfx_randCentered(0x400) + 0xC00);
        }
        if (fade != 0) {
            ex->Hi_DrawEffModel(&work->rot80, (VECTOR *)&work->pos88, (s32)&work->scale94, work->model7C);
            ex->Hi_ModifyEffModelRGB(work->model7C, (fade >> 5) - 0x80, (fade >> 5) - 0x80, (fade >> 5) - 0x80);
        }
        if (tint != 0) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 0xFFE), BTLFX_ABR_OPAQUE, 0, 0, (tint * 0x4D) >> 12);
        }
        if (shake != 0) {
            shakeLevel = shake << 7;
            shakeTint = shakeLevel >> 12;
            if (work->frame4 == work->frame4 / 6 * 6) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0, shakeTint * 2 / 3, shakeTint);
            }
            if (!((work->frame4 - 3) & 7)) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_SUB, shakeLevel >> 13, shakeLevel >> 13, 0);
            }
        }
        work->frame4++;
    }
    return 0;
}
