#include "fxmod/fx_07a.h"

static s32 fxm_easeLerpSvec(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_07a_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = ((0x10000000 / n) * t) >> 16;
        break;
    case 1:
        ratio = 0x1000 - ((invSq * (n - t) * (n - t)) >> 17);
        break;
    case 2:
        ratio = (invSq * t * t) >> 17;
        break;
    }
    return ex->btlfx_lerpVec((s16 *)from, (s16 *)to, 0x1000 - ratio, (s16 *)out);
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_07a_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

INCLUDE_RODATA("asm/us/fx_07a/nonmatchings/fx_07a", fx_07a_headerPtr);

s32 fx_07a_op80(s32 mode, Fx07aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR out;
    VECTOR cur;
    VECTOR step;
    Fx07aStrip *tr;
    Fx07aStrip *strip;
    Fx07aPt *pt;
    s32 frame;
    s32 i;
    s32 j;
    s32 n = 11;
    s32 spread;
    s32 rise;
    s32 pitch;
    s32 yaw;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = 0x108;
    } else if (mode == 1) {
        fx_07a_env = env;
        ex->btlfx_initEffModels(fx_07a_effModels, 3);
        for (i = 15; i >= 0; i--) {
            work->timers0[i] = 0;
        }
        for (i = 0; i < 5; i++) {
            fx_07a_strips[i].timer10C = -i;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xC), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&fx_07a_strips[i], 0);
        }
        work->pos94.vx = fx_07a_centerPos.vx = env->targets->centerX;
        work->pos94.vy = fx_07a_centerPos.vz = env->targets->centerZ;
        work->pos94.vz = fx_07a_centerPos.vy = -0x500;
        work->models40[0].model0 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xB), 0x137,
                                                         0xFFFF, 0, 0);
        work->models40[0].rot4.vx = 0x800;
        work->models40[0].rot4.vy = 0;
        work->models40[0].rot4.vz = 0;
        work->models40[0].scaleC.vx = 0x5C;
        work->models40[0].scaleC.vy = 0x5C;
        work->models40[0].scaleC.vz = 0x5C;
        work->models40[1].model0 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xD));
        work->models40[1].rot4.vx = 0;
        work->models40[1].rot4.vy = 0;
        work->models40[1].rot4.vz = 0;
        work->models40[1].scaleC.vx = 0x50;
        work->models40[1].scaleC.vy = 0x50;
        work->models40[1].scaleC.vz = 0x50;
        work->models40[2].model0 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xE));
        work->models40[2].rot4.vx = 0;
        work->models40[2].rot4.vy = 0;
        work->models40[2].rot4.vz = 0;
        work->models40[2].scaleC.vx = 0x66;
        work->models40[2].scaleC.vy = 0x66;
        work->models40[2].scaleC.vz = 0x66;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->homePosA4[i]);
            work->delayE4[i] = ex->btlfx_rand() & 7;
        }
        fx_07a_discRadius = 0;
        fx_07a_uvRadiusOut = 0;
        fx_07a_uvRadiusIn = 0;
        fx_07a_discRadiusIn = 0;
        fx_07a_ringTwist[4] = 0;
        fx_07a_ringTwist[3] = 0;
        fx_07a_ringTwist[2] = 0;
        fx_07a_ringTwist[1] = 0;
        fx_07a_ringTwist[0] = 0;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&fx_07a_bonePos);
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x17E, 0x70, 0x70, fx_07a_bgOt, fx_07a_centerPos.vx, fx_07a_centerPos.vz, 0x1208);
            break;
        case 1:
            work->timers0[9] = 1;
            work->timers0[1] = 1;
            work->timers0[0] = 0x24;
            work->timers0[2] = 8;
            work->timers0[4] = 0x10;
            break;
        case 8:
            work->timers0[10] = 1;
            work->timers0[11] = 8;
            work->timers0[12] = 0x20;
            work->timers0[6] = 0x1C;
            break;
        case 12:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&fx_07a_centerPos);
            break;
        case 24:
            work->timers0[13] = 0x10;
            work->timers0[5] = 0x10;
            break;
        case 36:
            work->timers0[7] = 4;
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&fx_07a_centerPos);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 40:
            work->timers0[10] = 0;
            ex->btlfx_drawScreenTint((u32 *)env->ot, 0xFF, 0xFF, 0xFF, 0xFF);
            work->timers0[1] = 0;
            break;
        }
        pos.vx = fx_07a_centerPos.vx;
        pos.vy = 0;
        pos.vz = fx_07a_centerPos.vz;
        if (work->timers0[11] != 0) {
            work->timers0[11]--;
            fx_07a_outerBandRadius = fxm_easeLerp(0, 8, work->timers0[11], 0, 0x4A00);
            fx_07a_discRadius = fxm_easeLerp(0, 8, work->timers0[11], 0, 0x5A00);
            fx_07a_innerBandRadius = fxm_easeLerp(0, 8, work->timers0[11], 0, 0xB20);
            fx_07a_uvRadiusOut = fxm_easeLerp(0, 8, work->timers0[11], 0, 0x340);
        }
        if (work->timers0[12] != 0) {
            work->timers0[12]--;
            fx_07a_ringTwist[1] = fxm_easeLerp(1, 0x20, work->timers0[12], 0, 0x1000);
            fx_07a_ringTwist[2] = fxm_easeLerp(1, 0x20, work->timers0[12], 0, 0xEAB);
            fx_07a_ringTwist[3] = fxm_easeLerp(1, 0x20, work->timers0[12], 0, 0xD56);
            fx_07a_ringTwist[4] = fxm_easeLerp(1, 0x20, work->timers0[12], 0, 0xC00);
            fx_07a_uvRadiusIn = fxm_easeLerp(1, 0x20, work->timers0[12], 0, 0x340);
        }
        if (work->timers0[13] != 0) {
            work->timers0[13]--;
            fx_07a_outerBandRadius = fxm_easeLerp(0, 0x10, work->timers0[12], 0x4A00, 0);
            fx_07a_innerBandRadius = fxm_easeLerp(0, 0x10, work->timers0[13], 0xB20, 0);
            fx_07a_discRadius = fxm_easeLerp(0, 0x10, work->timers0[12], 0x5A00, 0);
        }
        if (work->timers0[10] != 0) {
            fx_07a_drawTwistDisc(&pos, 0xFFF, 0x117, -1, 0xFF);
        }
        if (work->timers0[2] != 0) {
            work->timers0[2]--;
            fxm_easeLerpSvec(0, 8, work->timers0[2], &fx_07a_bonePos, &fx_07a_centerPos, &out);
            work->pos94.vx = out.vx;
            work->pos94.vy = out.vy;
            work->pos94.vz = out.vz;
        }
        if (work->timers0[4] != 0) {
            work->timers0[4]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[4], 0x5C, 0x2A2);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[4], 0x50, 0x243);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vy = work->models40[2].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[4], 0x66, 0x299);
        }
        if (work->timers0[5] != 0) {
            work->timers0[5]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[5], 0x2A2, 0);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[5], 0x243, 0);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vy = work->models40[2].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[5], 0x299, 0);
        }
        if (work->timers0[6] != 0) {
            work->timers0[6]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                if (0x1C - work->timers0[6] >= work->delayE4[i]) {
                    pos.vx = work->homePosA4[i].vx;
                    pos.vz = work->homePosA4[i].vz;
                    pos.vy = fxm_easeLerp(2, 0x1C - work->delayE4[i], work->timers0[6], work->homePosA4[i].vy,
                                           work->homePosA4[i].vy - 0x200);
                    ex->btlfx_setUnitPos(i, (s16 *)&pos);
                }
            }
        }
        if (work->timers0[7] != 0) {
            work->timers0[7]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                if (work->timers0[7] != 0) {
                    pos.vx = work->homePosA4[i].vx;
                    pos.vz = work->homePosA4[i].vz;
                    pos.vy = fxm_easeLerp(2, 4, work->timers0[7], work->homePosA4[i].vy - 0x200, work->homePosA4[i].vy);
                } else {
                    ex->btlfx_getUnitHomePos(i, (s16 *)&pos);
                }
                ex->btlfx_setUnitPos(i, (s16 *)&pos);
            }
        }
        if (work->timers0[1] != 0) {
            ex->btlfx_aimAngles((s16 *)&fx_07a_centerPos, (s16 *)&env->camera->eye, (s16 *)&work->models40[1].rot4);
            work->models40[0].rot4.vx = work->models40[1].rot4.vx + 0x800;
            work->models40[0].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vx = work->models40[1].rot4.vx + 0x400;
            work->models40[1].rot4.vx -= 0x400;
            ex->Hi_DrawEffModel(&work->models40[1].rot4, &work->pos94, (s32)&work->models40[1].scaleC, work->models40[1].model0);
            ex->Hi_DrawEffModel(&work->models40[2].rot4, &work->pos94, (s32)&work->models40[2].scaleC, work->models40[2].model0);
            ex->Hi_DrawEffModel(&work->models40[0].rot4, &work->pos94, (s32)&work->models40[0].scaleC, work->models40[0].model0);
            ex->Hi_ModifyEffModelAbr(work->models40[0].model0, 1);
        }
        if (work->timers0[0] != 0) {
            work->timers0[0]--;
            for (i = 0; i < 5; i++) {
                tr = &fx_07a_strips[i];
                if (tr->timer10C > 0) {
                    tr->timer10C--;
                    tr->lastSlot0 = tr->count2 = n - 1;
                    tr->head1 = 0;
                    cur.vx = fx_07a_bonePos.vx << 12;
                    cur.vy = fx_07a_bonePos.vy << 12;
                    cur.vz = fx_07a_bonePos.vz << 12;
                    step.vx = ((work->pos94.vx + tr->endX5C - fx_07a_bonePos.vx) << 12) / n;
                    step.vy = ((work->pos94.vy + tr->endY60 - fx_07a_bonePos.vy) << 12) / n;
                    step.vz = ((work->pos94.vz + tr->endZ64 - fx_07a_bonePos.vz) << 12) / n;
                    strip = tr;
                    for (j = 0, pt = tr->pts4; j < n; pt++, j++) {
                        pt->vx = cur.vx >> 12;
                        pt->vz = cur.vz >> 12;
                        pt->vy = tr->offsetY6C[j] + (cur.vy >> 12);
                        pt->halfWidth6 = (u16)tr->timer10C * 4;
                        cur.vx += step.vx;
                        cur.vy += step.vy;
                        cur.vz += step.vz;
                        tr->offsetY6C[j] += tr->speedYBC[j];
                    }
                    if (tr->timer10C < 4) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xC), 0, NULL, frame, 0x1000,
                                            tr->timer10C << 10, 1, (u8 *)strip, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xC), 0, NULL, frame, 0x1000,
                                            0x1000, 1, (u8 *)strip, 0);
                    }
                } else if (tr->timer10C < 0) {
                    tr->timer10C++;
                } else if (work->timers0[0] >= 5) {
                    tr->timer10C = ex->btlfx_randRange(4, 8);
                    j = 1;
                    pitch = ex->btlfx_rand() & 0xFFF;
                    yaw = ex->btlfx_rand() & 0xFFF;
                    rise = (ex->math_cos(pitch) * (work->models40[0].scaleC.vx + 0x80)) >> 12;
                    tr->endY60 = rise;
                    tr->endX5C = (ex->math_sin(yaw) * abs(rise)) >> 12;
                    tr->endZ64 = (ex->math_cos(yaw) * abs(rise)) >> 12;
                    tr->speedYBC[0] = 0;
                    tr->offsetY6C[0] = 0;
                    for (; j < n - 1; j++) {
                        spread = j * 8 + 0x10;
                        tr->offsetY6C[j] = ex->btlfx_randRange(-spread * 2, spread * 2);
                        tr->speedYBC[j] = ex->btlfx_randRange(-spread, spread);
                    }
                    tr->speedYBC[j] = 0;
                    tr->offsetY6C[j] = 0;
                }
            }
        }
        if (work->timers0[9] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 8), work->timers0[9]);
            work->timers0[9]++;
        }
        return frame >= 0x46;
    }
    return 0;
}

void fx_07a_drawTwistDisc(SVECTOR *pos, s32 ot, s32 tpage, s32 clut, s32 abr) {
    Unk800F78A4 *env = fx_07a_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    u8 uv[4][8];
    SVECTOR quad[4];
    SVECTOR rot;
    MATRIX m;
    s32 radiusStep;
    s32 uvStep;
    POLY_FT4 *ft4;
    POLY_G4 *poly;
    s32 ang;
    s32 j;
    s32 radius;
    s32 uvRadius;
    s32 twistAng;

    quad[3].vy = 0;
    quad[2].vy = 0;
    quad[1].vy = 0;
    quad[0].vy = 0;
    rot.vz = 0;
    rot.vy = 0;
    rot.vx = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    ft4 = (POLY_FT4 *)env->prim;
    tpage |= TPAGE_ABR(abr);
    poly = (POLY_G4 *)(ft4 + 64);
    env->prim = (u8 *)(poly + 32);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(&rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    ang = 0;
    radiusStep = fx_07a_discRadius >> 2;
    uvStep = (fx_07a_uvRadiusOut - fx_07a_uvRadiusIn) >> 2;
    for (; ang < 0x1000; ang += 0x100) {
        radius = 0;
        uvRadius = 0;
        for (j = 0; j < 4; j++) {
            twistAng = ang + fx_07a_ringTwist[j];
            uv[0][0] = ((ex->math_cos(ang) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + 0x38;
            uv[0][1] = ((ex->math_sin(ang) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) - 0x48;
            uv[1][0] = ((ex->math_cos(ang + 0x100) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + 0x38;
            uv[1][1] = ((ex->math_sin(ang + 0x100) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) - 0x48;
            quad[0].vx = (ex->math_cos(twistAng) * radius) >> 16;
            quad[0].vz = -((ex->math_sin(twistAng) * radius) >> 16) - 0x20;
            twistAng += 0x100;
            quad[1].vx = (ex->math_cos(twistAng) * radius) >> 16;
            quad[1].vz = -((ex->math_sin(twistAng) * radius) >> 16) - 0x20;
            twistAng = ang + fx_07a_ringTwist[j + 1];
            uvRadius += uvStep;
            uv[2][0] = ((ex->math_cos(ang) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + 0x38;
            uv[2][1] = ((ex->math_sin(ang) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) - 0x48;
            uv[3][0] = ((ex->math_cos(ang + 0x100) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + 0x38;
            radius += radiusStep;
            uv[3][1] = ((ex->math_sin(ang + 0x100) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) - 0x48;
            quad[2].vx = (ex->math_cos(twistAng) * radius) >> 16;
            quad[2].vz = -((ex->math_sin(twistAng) * radius) >> 16) - 0x20;
            twistAng += 0x100;
            quad[3].vx = (ex->math_cos(twistAng) * radius) >> 16;
            quad[3].vz = -((ex->math_sin(twistAng) * radius) >> 16) - 0x20;
            gte_ldv3c(quad);
            gte_rtpt();
            ft4->b0 = 0x40;
            ft4->g0 = 0x40;
            ft4->r0 = 0x40;
            ft4->code = abr == 0xFF ? 0x2C : 0x2E;
            *(u16 *)&ft4->u0 = *(u16 *)uv[0];
            *(u16 *)&ft4->u1 = *(u16 *)uv[1];
            *(u16 *)&ft4->u2 = *(u16 *)uv[2];
            *(u16 *)&ft4->u3 = *(u16 *)uv[3];
            gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
            gte_ldv0(&quad[3]);
            gte_rtps();
            ft4->clut = clut;
            ft4->tpage = tpage;
            gte_stsxy(&ft4->x3);
            ex->btlfx_addPrim(0x09000000, ot * 4, ft4);
            ft4++;
        }
        for (j = 0; j < 2; j++) {
            if (j != 0) {
                quad[2].vx = (ex->math_cos(ang) * fx_07a_outerBandRadius) >> 16;
                quad[2].vz = -((ex->math_sin(ang) * fx_07a_outerBandRadius) >> 16) - 0x20;
                quad[3].vx = (ex->math_cos(ang + 0x100) * fx_07a_outerBandRadius) >> 16;
                quad[3].vz = -((ex->math_sin(ang + 0x100) * fx_07a_outerBandRadius) >> 16) - 0x20;
                quad[0].vx = (ex->math_cos(ang) * fx_07a_discRadius) >> 16;
                quad[0].vz = -((ex->math_sin(ang) * fx_07a_discRadius) >> 16) - 0x20;
                quad[1].vx = (ex->math_cos(ang + 0x100) * fx_07a_discRadius) >> 16;
                quad[1].vz = -((ex->math_sin(ang + 0x100) * fx_07a_discRadius) >> 16) - 0x20;
                poly->b1 = 0x80;
                poly->g1 = 0x80;
                poly->r1 = 0x80;
                poly->b0 = 0x80;
                poly->g0 = 0x80;
                poly->r0 = 0x80;
                poly->b3 = 0;
                poly->g3 = 0;
                poly->r3 = 0;
                poly->b2 = 0;
                poly->g2 = 0;
                poly->r2 = 0;
            } else {
                quad[2].vx = (ex->math_cos(ang) * fx_07a_discRadius) >> 16;
                quad[2].vz = -((ex->math_sin(ang) * fx_07a_discRadius) >> 16) - 0x20;
                quad[3].vx = (ex->math_cos(ang + 0x100) * fx_07a_discRadius) >> 16;
                quad[3].vz = -((ex->math_sin(ang + 0x100) * fx_07a_discRadius) >> 16) - 0x20;
                quad[0].vx = (ex->math_cos(ang) * fx_07a_innerBandRadius) >> 12;
                quad[0].vz = -((ex->math_sin(ang) * fx_07a_innerBandRadius) >> 12) - 0x20;
                quad[1].vx = (ex->math_cos(ang + 0x100) * fx_07a_innerBandRadius) >> 12;
                quad[1].vz = -((ex->math_sin(ang + 0x100) * fx_07a_innerBandRadius) >> 12) - 0x20;
                poly->b1 = 0;
                poly->g1 = 0;
                poly->r1 = 0;
                poly->b0 = 0;
                poly->g0 = 0;
                poly->r0 = 0;
                poly->b3 = 0xFF;
                poly->g3 = 0xFF;
                poly->r3 = 0xFF;
                poly->b2 = 0xFF;
                poly->g2 = 0xFF;
                poly->r2 = 0xFF;
            }
            gte_ldv3c(quad);
            gte_rtpt();
            poly->code = GPU_POLY_G4 | GPU_ABE;
            gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
            gte_ldv0(&quad[3]);
            gte_rtps();
            gte_stsxy(&poly->x3);
            ex->btlfx_addPrim(0x08000000, ot * 4 - 4, poly);
            poly++;
        }
    }
    ex->btlfx_addPrim(0x01000000, ot * 4 - 4, ex->btlfx_allocDrMode(0, 2));
}
