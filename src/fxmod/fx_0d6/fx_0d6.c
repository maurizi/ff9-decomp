#include "fxmod/fx_0d6.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_0d6_env;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR quad[4];
    CVECTOR colTR;
    CVECTOR colBR;
    CVECTOR colTL;
    CVECTOR colBL;
    MATRIX m;
    s32 otz;
    s32 nclip;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *src;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 rows;
    s32 left;
    s32 texel;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        rows = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * rows);
        tpage = ex->btlfx_allocDrMode(0, abr);
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(pos);
        if (rot == NULL) {
            rot = &zero;
        }
        gte_rtv0tr_b();
        ex->math_rotMatrixYXZ(rot, &m);
        if (scale >= 0) {
            sv.vx = sv.vy = sv.vz = scale;
            ex->ScaleMatrix(&m, &sv);
        }
        gte_stlvnl(m.t);
        gte_rtps();
        code = GPU_POLY_G4;
        if (abr != BTLFX_ABR_OPAQUE) {
            code = GPU_POLY_G4 | GPU_ABE;
        }
        gte_stszotz(&otz);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        left = -(w * size) >> 1;
        h = rows;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = left;
            src = row;
            *(s32 *)&colTL = 0;
            *(s32 *)&colBL = 0;
            for (i = 0; i < w; i++, prim++, px += size, src++) {
                quad[0].vx = quad[2].vx = px;
                quad[0].vy = quad[1].vy = py;
                quad[1].vx = quad[3].vx = px + size;
                quad[2].vy = quad[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&colTR = 0;
                    *(s32 *)&colBR = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&colTR = 0;
                    } else {
                        texel = src[0];
                        colTR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colTR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colTR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&colBR = 0;
                    } else {
                        texel = src[w - 1];
                        colBR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colBR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colBR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&quad[0], &quad[1], &quad[2]);
                colTL.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&colTL;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&colTR;
                *(s32 *)&prim->r2 = *(s32 *)&colBL;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&nclip);
                gte_ldv0xy(&quad[3]);
                *(s32 *)&prim->r3 = *(s32 *)&colBR;
                gte_rtps_b();
                *(s32 *)&colBL = *(s32 *)&colBR;
                *(s32 *)&colTL = *(s32 *)&colTR;
                gte_stsxy(&prim->x3);
                if (nclip != 0) {
                    ex->btlfx_addPrim(PRIM_TAG(POLY_G4), otz, prim);
                }
            }
        }
        ex->btlfx_addPrim(PRIM_TAG(DR_TPAGE), otz, tpage);
    }
    w--;
    h--;
    left = (tp & TPAGE_X_MASK) * 64 + x + (idx % div) * w;
    py = (tp & TPAGE_Y_BIT) * 16 + y + (idx / div) * h;
    rect.x = left;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

/* The twin of fx_048 op80 (other files, sizes and colours; starts in state 1). */
s32 fx_0d6_op80(s32 mode, Fx0d6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos2;
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot2;
    SVECTOR pillarRot;
    VECTOR pos3;
    VECTOR sc2;
    SVECTOR sparkPos;
    Pool6CEntry *emitter;
    s32 ringAng;
    s32 frame;
    s32 bri;
    s32 scl;
    s32 t;
    s32 u;
    s32 tint;
    s32 i;
    s32 cnt;
    s32 ang;
    s32 spread;
    s32 radius;
    s32 s;
    s32 pieceAng;
    s32 riseT;

    if (mode == 0) {
        *args = sizeof(Fx0d6Work80);
    } else if (mode == 1) {
        fx_0d6_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        ex->btlfx_initEffModels(fx_0d6_effModels, FX_SWDMAG_FIRAGA_MODEL_COUNT);
        work->model14[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_FLAME_RIBBON_0), 1);
        work->model14[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_FLAME_RIBBON_1), 1);
        work->model14[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_FLAME_RIBBON_2), 1);
        work->model14[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_FLAME_RIBBON_3), 1);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_FLAME_PILLAR), FX_SWDMAG_FIRAGA_FLAME_PILLAR_TPAGE, FX_SWDMAG_FIRAGA_FLAME_PILLAR_CLUT, FX_SWDMAG_FIRAGA_FLAME_PILLAR_U, FX_SWDMAG_FIRAGA_FLAME_PILLAR_V);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_FLAME_DISC), FX_SWDMAG_FIRAGA_FLAME_PILLAR_TPAGE, FX_SWDMAG_FIRAGA_FLAME_PILLAR_CLUT, FX_SWDMAG_FIRAGA_FLAME_PILLAR_U, FX_SWDMAG_FIRAGA_FLAME_PILLAR_V);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_EMBER_TRAIL), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->trails = (s32)fx_0d6_trails;
            emitter->trailSlots = FX_SWDMAG_FIRAGA_EMBER_TRAIL_SLOTS;
            emitter->trailStride = FX_SWDMAG_FIRAGA_EMBER_TRAIL_STRIDE;
            emitter->otZ = 0x20;
        }
        work->emit2C = NULL;
        work->emit30 = NULL;
        work->frame34 = 0;
        work->state10 = FX_SWDMAG_FIRAGA_STATE_FLASH;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_FLAME_BURST), (s32)work);
        if (emitter != NULL) {
            emitter->otZ = 0x100;
        }
    } else {
        bri = 0;
        scl = 0;
        frame = entry->frame;
        switch (work->state10) {
        case FX_SWDMAG_FIRAGA_STATE_FLASH:
            t = frame << 9;
            ex->btlfx_drawScreenTint((u32 *)env->ot + 0x800, 1, (t * 70) >> 12, 0, 0);
            if (frame == FX_SWDMAG_FIRAGA_SPARKS_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_EMBERS), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_GOLD_SPARKS_2), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_GOLD_SPARKS), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            bri = ex->math_cos(t >> 2);
            env->otZ = 0x200;
            scl = t / 3 + 0x555;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_GLOW), &work->pos8, NULL, 0x1000, 0, 1,
                              ex->math_cos(t >> 2));
            env->otZ = 0;
            if (frame >= FX_SWDMAG_FIRAGA_FLASH_FRAMES) {
                work->state10 = FX_SWDMAG_FIRAGA_STATE_PILLAR;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_FIRAGA_STATE_PILLAR:
            t = (frame << 12) / FX_SWDMAG_FIRAGA_PILLAR_FRAMES;
            if (frame < FX_SWDMAG_FIRAGA_WHITE_FLASH_FRAMES) {
                tint = ex->math_cos(frame << 8) * 0xFF;
                ex->btlfx_drawScreenTint((u32 *)env->ot + 1, 1, tint >> 12, tint >> 13, tint >> 15);
            }
            if (frame < FX_SWDMAG_FIRAGA_RED_TINT_FRAMES) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + OT_LAST, 0xFF,
                                         ((ex->math_cos(frame << 5) / 2 + 0x800) * (ex->btlfx_rand() % 50 + 7)) >> 12, 0,
                                         0);
            }
            {
            s32 roll;
            s32 rising;
            s32 lo;
            s32 hi;
            s32 ext;
            s32 pieceH;
            s32 riseH;
            s32 spinStep;
            s32 radiusMax;
            s32 angStep;
            s32 fade;
            s32 wobbleAng;
            s32 wobbleAmp;

            roll = frame << 4;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = roll;
            rot.pad = 1;
            rising = frame < FX_SWDMAG_FIRAGA_PILLAR_RING_FRAMES;
            if (rising) {
                riseT = (frame << 12) / FX_SWDMAG_FIRAGA_PILLAR_RING_FRAMES;
                ringAng = riseT >> 2;
                bri = ex->math_cos(ringAng);
                pos2.vx = work->pos8.vx;
                pos2.vy = work->pos8.vy;
                scl = riseT / 2 + 0x800;
                pos2.vz = work->pos8.vz;
                pos2.vy -= 0x400;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_PILLAR_RING), &pos2, &rot, ex->math_sin(ringAng) * 3 / 2 + 0x1000,
                                  0, 1, ex->math_cos(ringAng));
            }
            env->otZ = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_GLOW), &work->pos8, NULL,
                              ex->math_cos(t >> 2) / 2 * 2 + 0x2000, 0, 1, ex->math_cos(t >> 2));
            env->otZ = 0;
            if (rising) {
                ex->btlfx_drawScrollMove(roll, FX_SWDMAG_FIRAGA_PILLAR_FLAME_VRAM_X, FX_SWDMAG_FIRAGA_PILLAR_FLAME_VRAM_Y, FX_SWDMAG_FIRAGA_PILLAR_FLAME_VRAM_W,
                                     FX_SWDMAG_FIRAGA_PILLAR_FLAME_VRAM_H, FX_SWDMAG_FIRAGA_PILLAR_FLAME_SRC_X, FX_SWDMAG_FIRAGA_PILLAR_FLAME_SRC_Y);
                s = ((frame << 12) / FX_SWDMAG_FIRAGA_PILLAR_RING_FRAMES) >> 2;
                sc.vx = sc.vz = ex->math_sin(s) / 4 + 0x80;
                sc.vy = ex->math_sin(s) * 3 / 2 + 0x1000;
                rot2.vx = 0;
                rot2.vy = frame * 24;
                rot2.vz = 0;
                pos.vx = work->pos8.vx;
                pos.vy = work->pos8.vy;
                pos.vz = work->pos8.vz;
                sc.vx = sc.vz = ex->math_sin(s) / 3 + 0x180;
                sc.vy = ex->math_cos(s) * 3 / 2 + 0x1000;
                ex->Hi_DrawEffModel(&rot2, &pos, (s32)&sc, work->model24);
                ex->Hi_ModifyEffModelRGB(work->model24, (ex->math_cos(s) >> 5) - 0x80, (ex->math_cos(s) >> 5) - 0x80,
                                         (ex->math_cos(s) >> 5) - 0x80);
            }
            if (frame == FX_SWDMAG_FIRAGA_FLAME_BURST_2_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_FLAME_BURST_2), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            if (frame == FX_SWDMAG_FIRAGA_SMOKE_BURST_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_SMOKE_BURST), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            if (frame == FX_SWDMAG_FIRAGA_RED_HAZE_FRAME) {
                emitter = work->emit2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_RED_HAZE), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                emitter = work->emit30 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_EMBER_CONE), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            pillarRot.vx = 0x400;
            pillarRot.vy = 0;
            pillarRot.vz = 0;
            if (work->emit2C != NULL) {
                if (work->emit2C->state == BTLFX_EMIT_DONE) {
                    work->emit2C = NULL;
                }
                if (work->emit2C != NULL) {
                    work->emit2C->rot.vx = pillarRot.vx;
                    work->emit2C->rot.vy = pillarRot.vy;
                    work->emit2C->rot.vz = pillarRot.vz;
                }
            }
            if (work->emit30 != NULL) {
                if (work->emit30->state == BTLFX_EMIT_DONE) {
                    work->emit30 = NULL;
                }
                if (work->emit30 != NULL) {
                    work->emit30->rot.vx = pillarRot.vx;
                    work->emit30->rot.vy = pillarRot.vy;
                    work->emit30->rot.vz = pillarRot.vz;
                }
            }
            if (frame < FX_SWDMAG_FIRAGA_COLUMN_FRAMES) {
                u = (frame << 12) / FX_SWDMAG_FIRAGA_COLUMN_FRAMES;
                ang = -frame << 7;
                wobbleAng = -frame * 0x19B;
                pillarRot.vx = 0x800;
                ext = 0;
                pillarRot.vy = frame * 24;
                pillarRot.vz = 0;
                spread = (u << 7) >> 12;
                angStep = spread + 0x80;
                radiusMax = spread + 0x100;
                if (frame < FX_SWDMAG_FIRAGA_COLUMN_GROW_FRAMES) {
                    lo = FX_SWDMAG_FIRAGA_COLUMN_RADIUS;
                    hi = (frame * FX_SWDMAG_FIRAGA_COLUMN_RADIUS) / 12;
                    wobbleAmp = 0xAA;
                } else {
                    s = ((frame - FX_SWDMAG_FIRAGA_COLUMN_GROW_FRAMES) << 10) / (FX_SWDMAG_FIRAGA_COLUMN_FRAMES - FX_SWDMAG_FIRAGA_COLUMN_GROW_FRAMES);
                    lo = ex->math_sin(s) / 4 + FX_SWDMAG_FIRAGA_COLUMN_RADIUS;
                    hi = ((ex->math_sin(s) * 3) >> 2) + FX_SWDMAG_FIRAGA_COLUMN_RADIUS;
                    wobbleAmp = s / 2 + 0xAA;
                    ext = s / 3;
                }
                s = u >> 2;
                pieceH = (u >> 5) + 0x9C0;
                spinStep = frame * 6 + 8;
                fade = ex->math_cos(s) / 2;
                if (frame < FX_SWDMAG_FIRAGA_COLUMN_RISE_FRAMES) {
                    riseH = (ex->math_sin((frame << 10) / FX_SWDMAG_FIRAGA_COLUMN_RISE_FRAMES) * 0x960) >> 12;
                } else {
                    riseH = 0x960 - s;
                }
                cnt = 0;
                for (i = 0; i < FX_SWDMAG_FIRAGA_COLUMN_SEGMENTS; i++) {
                    u = (i << 12) / FX_SWDMAG_FIRAGA_COLUMN_SEGMENTS;
                    pos3.vx = work->pos8.vx;
                    pos3.vy = work->pos8.vy;
                    pos3.vz = work->pos8.vz;
                    pos3.vy -= 0x200 + ((u * riseH) >> 12);
                    ang += angStep;
                    radius = (radiusMax * u) >> 12;
                    pos3.vx += (ex->math_sin(ang) * radius) >> 12;
                    pos3.vz += (ex->math_cos(ang) * radius) >> 12;
                    pieceAng = u >> 2;
                    wobbleAng += 0x180;
                    s = ex->btlfx_lerp(lo, hi, 0x1000 - ex->math_cos(pieceAng)) + ((ex->math_sin(wobbleAng) * wobbleAmp) >> 12);
                    sc2.vy = pieceH;
                    sc2.vx = sc2.vz = s;
                    if (u > 0x800) {
                        s = u - 0x800;
                        s >>= 1;
                        sc2.vx += ((0x1000 - ex->math_cos(s)) * ext) >> 12;
                        sc2.vz += ((0x1000 - ex->math_cos(s)) * ext) >> 12;
                    }
                    pillarRot.vy += spinStep;
                    ex->Hi_DrawEffModel(&pillarRot, &pos3, (s32)&sc2, work->model14[i & 3]);
                    ex->Hi_ModifyEffModelRGB(work->model14[i & 3], ((fade * ex->math_cos(pieceAng)) >> 17) - 0x80, ((fade * (0x1000 - u)) >> 17) - 0x80, ((fade * (0x1000 - u)) >> 17) - 0x80);
                    if (!(ex->btlfx_rand() & 3) && cnt < FX_SWDMAG_FIRAGA_COLUMN_SPARKS) {
                        env->otZ = 0x100;
                        sparkPos.vx = pos3.vx;
                        sparkPos.vy = pos3.vy;
                        sparkPos.vz = pos3.vz;
                        sparkPos.vx += ex->btlfx_randCentered(0x140);
                        sparkPos.vy += ex->btlfx_randCentered(0x100);
                        sparkPos.vz += ex->btlfx_randCentered(0x140);
                        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_COLUMN_SPARK), &sparkPos, NULL, 0x7000, 0, 1, fade);
                        cnt++;
                        env->otZ = 0;
                    }
                }
            }
            }
            if (frame >= FX_SWDMAG_FIRAGA_PILLAR_FRAMES) {
                work->state10 = FX_SWDMAG_FIRAGA_STATE_TAIL;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_FIRAGA_STATE_TAIL:
            if (frame >= FX_SWDMAG_FIRAGA_TAIL_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (bri != 0) {
            rot.vx = 0x400;
            rot.vz = frame << 8;
            rot.pad = 1;
            rot.vy = 0;
            fxm_drawTexelQuads(&work->pos8, &rot, FX_SWDMAG_FIRAGA_FLOOR_GLOW_TPAGE, FX_SWDMAG_FIRAGA_FLOOR_GLOW_X, FX_SWDMAG_FIRAGA_FLOOR_GLOW_Y,
                          FX_SWDMAG_FIRAGA_FLOOR_GLOW_COLS, work->frame34 & FX_SWDMAG_FIRAGA_FLOOR_GLOW_FRAME_MASK, FX_SWDMAG_FIRAGA_FLOOR_GLOW_CELL,
                          FX_SWDMAG_FIRAGA_FLOOR_GLOW_GRID, FX_SWDMAG_FIRAGA_FLOOR_GLOW_GRID, scl, bri, BTLFX_ABR_ADD,
                          fx_0d6_texels, 0);
        }
        work->frame34++;
    }
    return 0;
}

s32 fx_0d6_op84(s32 mode, Fx0d6Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 alpha;
    s32 scale0;
    s32 scale1;
    s32 ang;
    s32 t;
    s32 flicker;
    s32 grow;
    u16 ry;

    if (mode == 0) {
        *args = sizeof(Fx0d6Work84);
    } else if (mode == 1) {
        fx_0d6_env = env;
        work->state0 = FX_SWDMAG_FIRAGA_FLASH_STATE_IN;
        work->frame4 = 0;
    } else {
        alpha = 0;
        scale0 = 0;
        scale1 = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
        ry = 0;
        switch (work->state0) {
        case FX_SWDMAG_FIRAGA_FLASH_STATE_IN:
            ry = 0x200;
            alpha = (frame << 12) / FX_SWDMAG_FIRAGA_FLASH_IN_FRAMES;
            t = ex->math_sin(alpha >> 2);
            scale0 = t >> 1;
            scale1 = ((0x1000 - alpha) >> 1) + 0x800;
            if (frame >= FX_SWDMAG_FIRAGA_FLASH_IN_FRAMES) {
                work->state0 = FX_SWDMAG_FIRAGA_FLASH_STATE_FLICKER;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_FIRAGA_FLASH_STATE_FLICKER:
            t = (frame << 12) / FX_SWDMAG_FIRAGA_FLASH_FLICKER_FRAMES;
            flicker = (frame & 1) << 8;
            scale1 = flicker + 0x1000;
            grow = (t >> 1) + 0x800;
            alpha = flicker + grow;
            ry = (t >> 3) + 0x200;
            scale0 = scale1;
            if (frame >= FX_SWDMAG_FIRAGA_FLASH_FLICKER_FRAMES) {
                work->state0 = FX_SWDMAG_FIRAGA_FLASH_STATE_FADE;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_FIRAGA_FLASH_STATE_FADE:
            t = (frame << 12) / FX_SWDMAG_FIRAGA_FLASH_FADE_FRAMES;
            ang = t >> 2;
            ry = 0x400 - ang;
            alpha = ex->math_cos(ang);
            scale0 = ex->math_sin(ang) + 0x1000;
            scale1 = (ex->math_sin(ang) << 1) + 0x1000;
            if (frame >= FX_SWDMAG_FIRAGA_FLASH_FADE_FRAMES) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->otZ = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_SWORD_GLINT), &work->pos8, NULL, scale0 / 3, 0, 1, alpha / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRAGA_SWORD_FLASH), &work->pos8, &rot, scale1, 0, 1, alpha * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d6/nonmatchings/fx_0d6", fx_0d6_headerPtr);
