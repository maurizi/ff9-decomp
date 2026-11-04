#include "fxmod/fx_18b.h"

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
    Unk800F78A4 *env = fx_18b_env;
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
    level = 0xFF;
    ex = BTLFX_EXPORTS(env);
    if (src == NULL) {
        level = 1;
    }
    colIn.cd = level != 0xFF ? 0x3E : 0x3C;
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
            if (outUv.vx >= 0x140) {
                outUv.vx = 0x13F;
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
                if (ot < 0x1000) {
                    addPrimLenIn(&env->ot[ot], prim, 0xC000000);
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
    if (src != NULL && ot < 0x1000) {
        addPrimLenIn(&env->ot[ot], move, 0x5000000);
    }
}

static void fx_18b_op80GridVertex(s32 radius, s32 ring, s32 ang, s32 spoke, s16 *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_18b_env);
    s32 wave = (ex->math_sin((ring << 11) >> 4) * fx_18b_waveAmp) >> 12;

    radius += (wave * ex->math_sin(fx_18b_wavePhase + (ring << 10))) >> 12;
    out[0] = (ex->math_cos(ang) * radius) >> 12;
    out[1] = (ex->math_sin(ang) * radius) >> 12;
}

s32 fx_18b_op80(s32 mode, Fx18bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT rect;
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        fx_18b_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos14);
        work->unk8 = ex->btlfx_addRadiusSixth(0, 0x20);
        fx_18b_wavePhase = 0;
        work->state0 = 0;
        fx_18b_waveAmp = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            t = (frame << 12) / 40;
            fx_18b_waveAmp = ex->math_sin(t >> 2) / 320;
            fx_18b_wavePhase -= 0x200;
            if (frame >= 0x28) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 40;
            fx_18b_waveAmp = ex->math_cos(t >> 2) / 320;
            fx_18b_wavePhase += 0x200;
            if (frame == 1) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SILENT_VOICE_SWIRLS), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0x28) {
                return 1;
            }
            break;
        }
        rect.x = VRAM_BTLFX_SCRATCH_X;
        rect.y = VRAM_BTLFX_SCRATCH_Y;
        rect.w = 0x100;
        rect.h = 0x100;
        fxm_drawRadialScreenGrid(1, 0xA0, 0x68, 0x10, 0x10, 8, 0x88, 0x88, &rect, fx_18b_op80GridVertex);
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18b/nonmatchings/fx_18b", fx_18b_headerPtr);
