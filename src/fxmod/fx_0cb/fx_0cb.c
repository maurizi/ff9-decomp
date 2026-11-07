#include "fxmod/fx_0cb.h"

static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *)) {
    DVECTOR pts[2][18];
    DVECTOR inPos;
    DVECTOR outPos;
    DVECTOR inPrev;
    DVECTOR outPrev;
    DVECTOR inUv;
    DVECTOR outUv;
    DVECTOR inUvPrev;
    DVECTOR outUvPrev;
    CVECTOR colIn;
    CVECTOR colOut;
    Unk800F78A4 *env = fx_0cb_env;
    u32 *move = NULL;
    s32 step;
    s32 cx;
    s32 cy;
    s32 tpLo;
    s32 tpHi;
    s32 hiPage = 0;
    BtlfxExports *ex;
    POLY_GT4 *prim;
    s32 level;
    s32 ang;
    s32 vyOff;
    DVECTOR *uvRing;
    s32 radius;
    s32 i;
    s32 j;
    s32 sx;
    s32 sy;

    prim = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(prim + segs * rings);
    level = BTLFX_ABR_OPAQUE;
    ex = BTLFX_EXPORTS(env);
    if (src == NULL) {
        level = BTLFX_ABR_ADD;
    }
    colIn.cd = level != BTLFX_ABR_OPAQUE ? GPU_POLY_GT4 | GPU_ABE : GPU_POLY_GT4;
    if (src != NULL) {
        tpLo = getTPage(2, 0, src->x, src->y);
        tpHi = getTPage(2, 0, src->x + 0x80, src->y);
        sx = src->x;
        sy = src->y;
        src->x = env->drawEnv->clip.x + (x - (src->w >> 1));
        src->y = env->drawEnv->clip.y + (y - (src->h >> 1));
        move = (u32 *)env->prim;
        env->prim = (u8 *)move + 0x18;
        ex->SetDrawMove(move, src, sx, sy);
        cx = (src->w >> 1) + (sx & 0x3F);
        cy = (src->h >> 1) + (sy & 0xFF);
    } else {
        cx = x;
        cy = y;
        tpLo = getTPage(2, 1, env->dispEnv->disp.x, env->dispEnv->disp.y);
        tpHi = getTPage(2, 1, env->dispEnv->disp.x + 0x80, env->dispEnv->disp.y);
    }
    uvRing = pts[1];
    tpLo <<= 16;
    tpHi <<= 16;
    for (i = 0; i < 17; i++) {
        *(s32 *)&pts[0][i] = 0;
        uvRing[i].vx = cx;
        vyOff = i * 4 + 2;
        *(s16 *)((u8 *)uvRing + vyOff) = cy;
    }
    step = 0x1000 / segs;
    radius = 0;
    for (j = 1; j <= rings; j++) {
        level = ex->btlfx_lerpRatio(from, to, j - 1, rings);
        i = 0;
        ang = 0;
        colIn.r = colIn.g = colIn.b = level;
        level = ex->btlfx_lerpRatio(from, to, j, rings);
        colOut.r = colOut.g = colOut.b = level;
        radius += dr;
        for (; i <= segs; i++, ang += step) {
            *(s32 *)&inPos = *(s32 *)&pts[0][i];
            *(s32 *)&inUv = *(s32 *)&pts[1][i];
            if (i < segs) {
                outPos.vx = (ex->math_cos(ang) * radius) >> 12;
                outPos.vy = (ex->math_sin(ang) * radius) >> 12;
                outUv.vx = outPos.vx + cx;
                outUv.vy = outPos.vy + cy;
            } else {
                *(s32 *)&outPos = *(s32 *)&pts[0][0];
                *(s32 *)&outUv = *(s32 *)&pts[1][0];
            }
            if (outUv.vx < 0) {
                outUv.vx = 0;
            }
            if (outUv.vx >= SCREEN_W) {
                outUv.vx = SCREEN_W - 1;
            }
            if (outUv.vy < 0) {
                outUv.vy = 0;
            }
            if (outUv.vy >= 0xF0) {
                outUv.vy = 0xEF;
            }
            *(s32 *)&pts[1][i] = *(s32 *)&outUv;
            if (i != 0) {
                if (inUv.vx < 0x100 && outUv.vx < 0x100 && inUvPrev.vx < 0x100 && outUvPrev.vx < 0x100) {
                    level = tpLo;
                    hiPage = 0;
                } else {
                    level = tpHi;
                    hiPage = 1;
                    inUv.vx -= 0x80;
                    outUv.vx -= 0x80;
                    inUvPrev.vx -= 0x80;
                    outUvPrev.vx -= 0x80;
                }
            }
            if (i < segs && fn != NULL) {
                fn(radius, j, ang, i, &outPos);
            }
            *(s32 *)&pts[0][i] = *(s32 *)&outPos;
            inPos.vx += x;
            inPos.vy += y;
            outPos.vx += x;
            outPos.vy += y;
            if (i != 0) {
                *(s32 *)&prim->u0 = inUv.vx | (inUv.vy << 8);
                *(s32 *)&prim->u1 = outUv.vx | (outUv.vy << 8) | level;
                *(s32 *)&prim->u2 = inUvPrev.vx | (inUvPrev.vy << 8);
                *(s32 *)&prim->u3 = outUvPrev.vx | (outUvPrev.vy << 8);
                *(s32 *)&prim->r2 = *(s32 *)&colIn;
                *(s32 *)&prim->r0 = *(s32 *)&colIn;
                *(s32 *)&prim->r3 = *(s32 *)&colOut;
                *(s32 *)&prim->r1 = *(s32 *)&colOut;
                *(s32 *)&prim->x0 = *(s32 *)&inPos;
                *(s32 *)&prim->x1 = *(s32 *)&outPos;
                *(s32 *)&prim->x2 = *(s32 *)&inPrev;
                *(s32 *)&prim->x3 = *(s32 *)&outPrev;
                if (ot < OT_LEN) {
                    addPrimLenIn(&env->ot[ot], prim, PRIM_TAG(POLY_GT4));
                }
                prim++;
            }
            if (hiPage) {
                inUv.vx += 0x80;
                outUv.vx += 0x80;
                inUvPrev.vx += 0x80;
                outUvPrev.vx += 0x80;
            }
            *(s32 *)&inPrev = *(s32 *)&inPos;
            *(s32 *)&outPrev = *(s32 *)&outPos;
            *(s32 *)&inUvPrev = *(s32 *)&inUv;
            *(s32 *)&outUvPrev = *(s32 *)&outUv;
        }
    }
    if (src != NULL && ot < OT_LEN) {
        addPrimLenIn(&env->ot[ot], move, 0x5000000);
    }
}

static void fxm_anglesFromEye(s16 *target, s16 *outAng) {
    BTLFX_EXPORTS(fx_0cb_env)->btlfx_aimAngles((s16 *)&fx_0cb_env->camera->eye, target, outAng);
}

static s32 fx_0cb_op80PoolTask(s32 mode, Fx0cbPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx0cbWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 bright;
    s32 s;
    s32 ang;
    s32 pulse;

    if (mode == 1) {
        p->unk0 = 0;
        p->spin14 = ex->btlfx_rand();
        p->spinSpeed16 = (ex->btlfx_rand() & 7) + 8;
    } else {
        ang = frame << 9;
        p->spin14 += p->spinSpeed16;
        fxm_anglesFromEye((s16 *)&p->pos4, (s16 *)&rot);
        rot.vz = p->spin14;
        pos.vx = p->pos4.vx;
        pos.vy = p->pos4.vy;
        pos.vz = p->pos4.vz;
        pulse = ex->math_sin(ang >> 2);
        pulse *= 3;
        s = p->scale2;
        scale.vx = scale.vy = scale.vz = ((s * pulse) >> 13) + (s >> 1);
        if (frame < FX_LIGHT_FLARE_BURST_IN_FRAMES) {
            bright = ((frame + 1) << 12) / FX_LIGHT_FLARE_BURST_IN_FRAMES;
        } else {
            bright = ex->math_cos(((frame - FX_LIGHT_FLARE_BURST_IN_FRAMES) << 10) / (FX_LIGHT_FLARE_BURST_FRAMES - FX_LIGHT_FLARE_BURST_IN_FRAMES));
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model20);
        s = bright >> 5;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model20, s, s, s);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
        s = bright >> 6;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model24, s, s, s);
        if (frame >= FX_LIGHT_FLARE_BURST_FRAMES) {
            return 1;
        }
    }
    return 0;
}

s32 fx_0cb_op80(s32 mode, Fx0cbWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR off;
    Fx0cbPart *part;
    s32 frame;
    s32 t;
    s32 grow;
    s32 alpha;
    u8 *data;
    s32 tint;
    s32 radius;
    s32 sc;
    s32 ang;

    if (mode == 0) {
        ((EffInit *)args)->workSize = sizeof(Fx0cbWork);
        ((EffInit *)args)->elemSize = sizeof(Fx0cbPart);
        ((EffInit *)args)->elemFn = (s32)fx_0cb_op80PoolTask;
        ((EffInit *)args)->poolCount = FX_LIGHT_FLARE_SLOTS;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0cb_env = env;
        work->otZ8 = ex->btlfx_addRadiusSixth(0, 0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        work->pos14.vx = work->posC.vx;
        work->pos14.vy = work->posC.vy;
        work->pos14.vz = work->posC.vz;
        ex->btlfx_initEffModels(fx_0cb_effModels, FX_LIGHT_FLARE_MODEL_COUNT);
        work->model20 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_LIGHT_FLARE_FIREBALL), 1);
        work->model24 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_LIGHT_FLARE_RED_HALO), 1);
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            t = (frame << 12) / FX_LIGHT_FLARE_FRAMES;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, FX_LIGHT_FLARE_DARKEN_RGB);
            } else if (frame < FX_LIGHT_FLARE_FLASH_FRAMES) {
                tint = 0x78 - (frame - 1) * 0x14;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, tint, tint, tint >> 1);
            }
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_LIGHT_FLARE_BACKDROP), work->frame4);
            ex->btlfx_drawScrollMove(frame * 8, FX_LIGHT_FLARE_FIREBALL_VRAM_X, FX_LIGHT_FLARE_FIREBALL_VRAM_Y, FX_LIGHT_FLARE_FIREBALL_VRAM_W,
                                     FX_LIGHT_FLARE_FIREBALL_VRAM_H, FX_LIGHT_FLARE_FIREBALL_SRC_X, FX_LIGHT_FLARE_FIREBALL_SRC_Y);
            if (frame < FX_LIGHT_FLARE_SPAWN_FRAMES) {
                if (frame == frame / FX_LIGHT_FLARE_SPAWN_GAP * FX_LIGHT_FLARE_SPAWN_GAP) {
                    grow = frame << 7;
                    radius = (grow >> 4) + 0x100;
                    sc = 0xAAA - (grow >> 1);
                    part = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (part != NULL) {
                        ex->btlfx_randRotation(&rot);
                        if (frame != 0) {
                            ex->btlfx_rotateOffset(&rot, radius, (s32)&off);
                        } else {
                            off.vz = 0;
                            off.vy = 0;
                            off.vx = 0;
                        }
                        part->pos4.vx = work->posC.vx + off.vx;
                        part->pos4.vz = work->posC.vz + off.vy;
                        part->pos4.vy = work->posC.vy + off.vz;
                        part->scale2 = sc;
                        ex->btlfx_randRotation(&part->rotC);
                    }
                }
            }
            rot.vy = 0;
            rot.vx = 0;
            rot.vz = 0;
            rot.pad = 0;
            ang = t >> 2;
            alpha = ex->math_cos(ang);
            env->otZ = work->otZ8 + 0x46;
            data = (u8 *)ex->btlfx_getFileData(entry->file, FX_LIGHT_FLARE_GLOW_RINGS);
            ex->btlfx_drawRings(data, &work->posC, &rot, ex->math_cos(ang) / 3 + 0x2AA, 0, BTLFX_ABR_ADD, alpha);
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_LIGHT_FLARE_BACKDROP_2), work->frame4);
            rot.vx = 0x200;
            rot.vy = 0x590;
            rot.vz = 0x80;
            rot.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LIGHT_FLARE_GLOW_RING), &work->posC, &rot, t / 2 + 0x555, 0, BTLFX_ABR_ADD, alpha);
            rot.vx = 0x600;
            rot.vy = 0x990;
            rot.vz = 0x80;
            rot.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LIGHT_FLARE_GLOW_RING), &work->posC, &rot, t / 2 + 0x555, 0, BTLFX_ABR_ADD, alpha);
            env->otZ = 0;
            if (frame >= FX_LIGHT_FLARE_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0cb/nonmatchings/fx_0cb", fx_0cb_headerPtr);
