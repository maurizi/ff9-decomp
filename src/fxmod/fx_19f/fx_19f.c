#include "fxmod/fx_19f.h"

static void fxm_drawCenterBand(u32 *ot, s32 tpage, s32 width, s32 r, s32 g, s32 b) {
    Unk800F78A4 *env = fx_19f_env;
    POLY_G4 *prim;
    BtlfxExports *ex;
    s32 i;

    width >>= 1;
    prim = (POLY_G4 *)env->prim;
    ex = BTLFX_EXPORTS(env);
    env->prim = (u8 *)(prim + 4);
    for (i = 0; i < 4; i++, prim++) {
        setlen(prim, 8);
        prim->code = GPU_POLY_G4 | GPU_ABE;
        if (!(i & 1)) {
            prim->r0 = prim->r2 = 0;
            prim->g0 = prim->g2 = 0;
            prim->b0 = prim->b2 = b;
            prim->r1 = prim->r3 = r;
            prim->g1 = prim->g3 = g;
            prim->b1 = prim->b3 = b;
            prim->x0 = prim->x2 = 0xA0 - width;
            prim->x1 = prim->x3 = 0xA0;
        } else {
            prim->r1 = prim->r3 = 0;
            prim->g1 = prim->g3 = 0;
            prim->b1 = prim->b3 = b;
            prim->r0 = prim->r2 = r;
            prim->g0 = prim->g2 = g;
            prim->b0 = prim->b2 = b;
            prim->x0 = prim->x2 = 0xA0;
            prim->x1 = prim->x3 = width + 0xA0;
        }
        prim->y0 = prim->y1 = (i >> 1) * 0x78;
        prim->y2 = prim->y3 = prim->y0 + 0x78;
        ex->btlfx_linkPrimAbr(PRIM_TAG(POLY_G4), ot, prim, tpage);
    }
}

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx19fTrail *trail, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang, s32 dang,
                   s32 frame, s32 fade, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_19f_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;
    s32 halfT;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    pt = trail->points4;
    trail->lastSlot0 = trail->count2 = 0x18;
    trail->head1 = 0;
    do {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x140);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = ex->btlfx_randCentered(0x80);
        halfT = t >> 1;
        offset = (ex->math_sin(halfT) * offset) >> 12;
        i++;
        offset += 0x20 + ((ex->math_sin(halfT) * amp) >> 12);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = lineWidth;
        ang += dang + ex->btlfx_randCentered(0x100);
        pt++;
    } while (i < 25);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_ORANGE_LIGHTNING), 0, NULL, frame, 0x1000, fade, BTLFX_ABR_ADD, (u8 *)trail, 0);
}

s32 fx_19f_op81(s32 mode, Fx19fWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 alpha;
    s32 headKey;
    s32 stripT;
    s32 stripScale;
    s32 glowGrowY;
    s32 arcAlpha;
    s32 arcAmp;
    Fx19fTrail17 *stripA;
    Fx19fTrail17 *stripB;
    SVECTOR *stripBPt;
    s16 *stripFrom;
    s16 *stripTo;
    s32 pad;
    s16 *waveFrom;
    s32 wave;
    VECTOR corePos;
    VECTOR coreScale;
    SVECTOR coreRot;
    s32 waveAlpha[3];
    s32 waveHead[3];
    s32 waveT[3];
    s32 waveScale[3];
    Fx19fKey12 *key;
    Fx19fTrail13 *waveTrail;
    Fx19fKeys *keys;
    Pool6CEntry *emitter;
    SVECTOR *pt;
    s32 frame;
    s32 k;
    s32 gridCols;
    s32 bandBlue;
    s32 emitShrink;
    s32 emitFadeT;
    s32 gridRows;
    s32 coreT;
    s32 waveFade;
    s32 arcAng;
    s32 seg;
    s32 waveKey;
    SVECTOR *wavePt;
    s16 *waveTo;
    s32 flashTint;
    s32 blueTint;
    s32 fadeTint;
    s32 blueFade;
    s32 coreRgb;
    s32 ringRgb;
    s32 keyFrame;
    s32 ringT;
    s32 stripFade;
    s32 waveSet;
    s32 yellowTint;
    s32 whiteout;
    s32 emitFade;
    s32 glowRgb;
    s32 flashRgb;
    s32 j;
    s32 step;
    s32 stripKey;
    s32 t;
    s32 coreSize;
    s32 burstSize;
    s32 waveFrame;
    s32 coreBright;
    s32 glowBright;
    s32 glowGrow;
    s32 introT;

    if (mode == 0) {
        *args = sizeof(Fx19fWork81);
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_19f_env = env;
        ex->btlfx_initEffModels(fx_19f_effModels, FX_RAMUH_BOLT_MODEL_COUNT);
        work->model70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_ELECTRIC_BOWL), 1);
        work->model74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_GREEN_DOME), 0);
        work->model78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_THIN_ROD), 0);
        work->model80 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_STAFF), 0);
        work->model7C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_BLUE_AURA_COLUMN), 1);
        work->pos8.vx = 0;
        work->pos8.vy = 0;
        work->pos8.vz = 0;
        FX19F_W(work->camPoint18, 0) = FX19F_W(work->pos8, 0);
        FX19F_W(work->camPoint18, 1) = FX19F_W(work->pos8, 1);
        work->camPoint18.vy -= 0x1EA;
        env->camera->points[3].vx = work->camPoint18.vx;
        env->camera->points[3].vy = work->camPoint18.vy;
        env->camera->points[3].vz = work->camPoint18.vz;
    } else {
        alpha = 0;
        headKey = 0;
        stripT = 0;
        stripScale = 0;
        glowGrowY = 0;
        frame = entry->frame;
        glowGrow = glowBright = coreBright = 0;
        arcAlpha = 0;
        waveAlpha[0] = waveAlpha[1] = waveAlpha[2] = 0;
        arcAmp = 0;
        switch (work->state0) {
        case FX_RAMUH_STATE_GATHER: {
            key = (Fx19fKey12 *)((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_MOTION_KEYS) + (frame * 12 + 4));
            work->pos48[0] = key->x + work->pos8.vx;
            work->pos48[1] = key->y + work->pos8.vy;
            work->pos48[2] = key->z + work->pos8.vz;
            work->rot60.vx = key->rx;
            work->rot60.vy = key->ry;
            work->rot60.vz = key->rz;
            work->scale54[0] = 0x2000;
            work->scale54[1] = 0x2000;
            work->scale54[2] = 0x2000;
            if (frame >= FX_RAMUH_GLOW_FRAME) {
                SVECTOR rot;

                ex->btlfx_rangeProgress(frame, 7, 8, 0, 0x1000, &introT);
                env->otZ = -0x80;
                rot.vx = 0x400;
                rot.vy = 0;
                rot.vz = frame * 0x18;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_GREY_RING), &work->pos8, &rot,
                                  ex->math_sin(introT >> 2), 0, BTLFX_ABR_ADD, ex->math_cos(introT >> 2));
                env->otZ = 0;
                glowBright = ex->math_cos(introT >> 2) >> 1;
                glowGrow = ex->math_sin(introT >> 2);
                glowGrowY = ex->math_sin(introT >> 2) >> 2;
            } else {
                glowBright = (ex->math_cos((frame << 10) / FX_RAMUH_GLOW_FRAME) >> 1) + 0x800;
            }
            if (frame >= FX_RAMUH_GATHER_FRAMES) {
                work->state0 = FX_RAMUH_STATE_CHARGE;
                entry->frame = -1;
            }
        } break;
        case FX_RAMUH_STATE_CHARGE:
            alpha = 0x1000;
            t = (frame << 12) / FX_RAMUH_CHARGE_FRAMES;
            headKey = ((0x1000 - ex->math_cos(t >> 2)) << 5) >> 12;
            stripScale = (t >> 3) + 0xE00;
            if (frame < 4) {
                flashTint = (ex->math_cos(frame << 8) * 0x46) >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_SUB, flashTint, flashTint, flashTint);
            }
            if (frame >= 5) {
                blueTint = ex->math_sin((frame - 5) << 8) << 7;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0, blueTint >> 14, blueTint >> 12);
            }
            if (frame == 3) {
                ex->btlfx_startFadeBackground(1, 6);
            }
            if (frame >= FX_RAMUH_CHARGE_FRAMES) {
                work->state0 = FX_RAMUH_STATE_STRIKE;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_SUB, FX_RAMUH_CHARGE_END_RGB);
                ex->btlfx_startFadeBackground(-1, 2);
                work->emit6C = NULL;
            }
            break;
        case FX_RAMUH_STATE_STRIKE:
            t = (frame << 12) / FX_RAMUH_STRIKE_FRAMES;
            if (frame == 0) {
                ex->btlfx_startFadeBackground(-1, 1);
            }
            if (frame == 2) {
                ex->btlfx_startFadeBackground(0, 3);
            }
            if (frame < 4) {
                fadeTint = 0xFF - frame * 0xFF / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, fadeTint >> 2, fadeTint, fadeTint);
            }
            if (frame >= FX_RAMUH_FLASH_FRAME && frame < FX_RAMUH_FLASH_FRAME + FX_RAMUH_FLASH_FRAMES) {
                if (frame == FX_RAMUH_FLASH_FRAME) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, FX_RAMUH_FLASH_RGB);
                }
                blueFade = 0xFF - (frame - FX_RAMUH_FLASH_FRAME) * 0xFF / FX_RAMUH_FLASH_FRAMES;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0, blueFade >> 2, blueFade);
            }
            arcAlpha = 0x1000;
            if (frame < 16) {
                arcAmp = (ex->math_cos(frame << 6) >> 3) + 0x12C;
            } else {
                arcAmp = 0x12C;
            }
            if (frame < 20) {
                coreBright = ex->math_cos((frame << 10) / 20);
            } else {
                coreBright = ((frame - 20) << 11) / 23;
                if (frame & 1) {
                    coreBright += 0x800;
                } else {
                    coreBright += 0x400;
                }
                if (ex->btlfx_rand() & 1) {
                    coreBright += 0x400;
                }
            }
            if (frame < 24) {
                coreT = (frame << 12) / 24;
                if (frame < 4) {
                    coreSize = ex->math_sin((frame << 10) >> 2) * 5 / 2;
                } else {
                    coreSize = (frame << 7) + 0x2600;
                }
                coreScale.vx = coreScale.vy = coreScale.vz = coreSize;
                coreRot.vz = 0;
                coreRot.vx = 0;
                coreRot.vy = frame * 16;
                corePos.vx = work->pos8.vx;
                corePos.vy = work->pos8.vy;
                corePos.vz = work->pos8.vz;
                ex->Hi_DrawEffModel(&coreRot, &corePos, (s32)&coreScale, work->model70);
                coreRgb = ((0x1000 - coreT) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model70, coreRgb, coreRgb, coreRgb);
                if (frame < 16) {
                    ex->Hi_DrawEffModel(&coreRot, &corePos, (s32)&coreScale, work->model74);
                    ringRgb = ((0xC00 - (frame << 8)) >> 5) - 0x80;
                    ex->Hi_ModifyEffModelRGB(work->model74, ringRgb, ringRgb, ringRgb);
                }
            }
            if (frame > 0) {
                SVECTOR rot;

                keys = (Fx19fKeys *)ex->btlfx_getFileData(entry->file, FX_RAMUH_LENS_FLASH_KEYS);
                keyFrame = frame + 9;
                if (keyFrame >= 36) {
                    burstSize = *(s16 *)((u8 *)keys + (36 * 6 + 4)) * 2 / 3 + ((frame - 27) << 6);
                } else {
                    keys = (Fx19fKeys *)((u8 *)keys + (keyFrame * 6 + 4));
                    burstSize = keys->unk0 * 2 / 3;
                }
                FX19F_W(work->pos10, 0) = FX19F_W(work->pos8, 0);
                FX19F_W(work->pos10, 1) = FX19F_W(work->pos8, 1);
                if (frame < 16) {
                    work->pos10.vy -= (ex->math_sin((frame << 10) >> 4) * 0x56E) >> 12;
                } else {
                    work->pos10.vy -= 0x56E;
                }
                FX19F_W(fx_19f_boltCenter, 0) = FX19F_W(work->pos10, 0);
                FX19F_W(fx_19f_boltCenter, 1) = FX19F_W(work->pos10, 1);
                if (frame < 24) {
                    fx_19f_boltLength = 0x1000;
                } else if (frame < 32) {
                    fx_19f_boltLength = (burstSize * 0x44C) >> 12;
                } else {
                    fx_19f_boltLength = 0xB86;
                }
                env->otZ = 0x140;
                if (frame >= 15) {
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_LENS_FLASH), &work->pos10, NULL, burstSize * 5 / 2,
                                            frame & 1, BTLFX_ABR_ADD, ex->math_cos(t >> 2), 0, 0);
                }
                rot.vx = rot.vy = 0x200;
                rot.vz = frame * 0x2C;
                rot.pad = 0;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CYAN_RING), &work->pos10, &rot,
                                  ex->btlfx_randCentered(0x1000) + 0x4000, 0, BTLFX_ABR_ADD, 0x1000);
                env->otZ = 0;
            }
            if (frame >= 20) {
                ringT = ((frame - 20) << 12) / 23;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_BLUE_RING), &work->pos10, NULL, (ringT >> 2) + 0x2800, 0, BTLFX_ABR_ADD,
                                  ex->math_sin(ringT >> 1));
            }
            if (frame == FX_RAMUH_GLOW_RINGS_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_RAMUH_BLUE_GLOW_RINGS), (s32)&work->pos10);
                work->emit6C = emitter;
                if (emitter != NULL) {
                    emitter->otZ = 0x96;
                }
            }
            if (frame == FX_RAMUH_SPARK_BURST_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_RAMUH_BOLT_SPARK_BURST), (s32)&work->pos8);
                work->emit68 = emitter;
                if (emitter != NULL) {
                    emitter->otZ = -0x20;
                }
            }
            if (frame < 8) {
                stripFade = frame << 9;
                alpha = 0x1000 - stripFade;
                headKey = 0x24;
                stripFade >>= 2;
                stripT = ex->math_sin(stripFade);
                stripScale = 0x1000 - stripFade;
            }
            {
                for (waveSet = 0; waveSet < 3; waveSet++) {
                    waveFrame = frame - waveSet * 3;
                    if (frame >= waveSet * 3 && waveFrame < 22) {
                        if (waveFrame < 6) {
                            waveAlpha[waveSet] = 0x1000;
                            waveHead[waveSet] = waveFrame * 2;
                            waveT[waveSet] = (waveFrame << 12) / 22;
                            waveScale[waveSet] = 0x1000;
                        } else {
                            waveFade = 0x1000 - ((waveFrame - 6) << 8);
                            waveAlpha[waveSet] = waveFade;
                            waveHead[waveSet] = 11;
                            waveT[waveSet] = (waveFrame << 12) / 22;
                            waveScale[waveSet] = waveFade;
                        }
                    }
                }
                gridRows = 2;
                gridCols = 4;
            }
            if (frame == FX_RAMUH_DARKEN_FRAME) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, FX_RAMUH_DARKEN_RGB);
            }
            if (frame >= FX_RAMUH_YELLOW_FRAME && frame < FX_RAMUH_YELLOW_FRAME + FX_RAMUH_YELLOW_FRAMES) {
                yellowTint = 0x80 - ((frame - FX_RAMUH_YELLOW_FRAME) << 6);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, yellowTint, yellowTint, yellowTint >> 3);
            }
            if (frame == 24) {
                ex->btlfx_startFadeBackground(-1, 0);
            }
            if (frame == 25) {
                ex->btlfx_startFadeBackground(0, 5);
            }
            if (frame == FX_RAMUH_BLUE_FRAME) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_RAMUH_BLUE_RGB);
            }
            if (frame == 36) {
                ex->btlfx_startFadeBackground(1, 7);
            }
            if (frame == FX_RAMUH_DARKEN_2_FRAME) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, FX_RAMUH_DARKEN_2_RGB);
            }
            if (frame == FX_RAMUH_GREY_FRAME) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_RAMUH_GREY_RGB);
            }
            if (frame >= FX_RAMUH_SKY_FRAME) {
                whiteout = (frame - FX_RAMUH_SKY_FRAME) * 0xFF / FX_RAMUH_SKY_FRAMES;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, whiteout >> 2, whiteout >> 1, whiteout);
            }
            if (frame >= 35) {
                CVECTOR col;
                SVECTOR rot;
                VECTOR sc;

                sc.vy = sc.vx = 0x1020;
                rot.vz = rot.vy = rot.vx = 0;
                sc.vz = 0x1000;
                col.r = col.g = col.b = ((frame - 31) << 5) / 12;
                ex->btlfx_drawScreenGrid(0x200, 0xA0, 0x78, 0x50, 0x3C, gridCols, gridCols, gridRows, gridRows, &rot, &sc, &col, 1, 0);
            }
            if (work->emit6C != NULL) {
                work->emit6C->scale = (t >> 1) + 0x1000;
                if (work->emit6C != NULL) {
                    work->emit6C->alpha = (0x1000 - (t >> 2)) * 3 / 4;
                }
            }
            if (frame >= FX_RAMUH_STRIKE_FRAMES) {
                work->state0 = FX_RAMUH_STATE_TAIL;
                entry->frame = -1;
                ex->btlfx_startFadeBackground(-1, 6);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_RAMUH_BOLT_SPARKS), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x96;
                }
            }
            break;
        case FX_RAMUH_STATE_TAIL: {
            t = (frame << 12) / FX_RAMUH_TAIL_FRAMES;
            if (frame < 12) {
                bandBlue = 0xFF - (frame << 5) / 12;
                fxm_drawCenterBand(env->ot + 2, 1, fx_19f_bandWidths[frame], 0xC8 - frame * 0xC8 / 12, 0xC8 - frame * 0xC8 / 12,
                              bandBlue);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_RAMUH_TAIL_RGB);
            }
            if (work->emit68 != NULL && frame < 10) {
                emitFadeT = (frame << 12) / 10;
                if (work->emit68->state == BTLFX_EMIT_DONE) {
                    work->emit68 = NULL;
                }
                if (work->emit68 != NULL) {
                    emitFade = 0x1000 - emitFadeT;
                    work->emit68->alpha = emitFade;
                    if (work->emit68 != NULL) {
                        work->emit68->scale = emitFade;
                    }
                }
                if (frame == 9) {
                    ex->btlfx_stopEmitter(work->emit68);
                }
            }
            if (work->emit6C != NULL && frame < 8) {
                emitShrink = frame << 9;
                if (work->emit6C->state == BTLFX_EMIT_DONE) {
                    work->emit6C = NULL;
                }
                if (work->emit6C != NULL) {
                    work->emit6C->scale = 0x1000 - emitShrink;
                }
                if (frame == 7) {
                    ex->btlfx_stopEmitter(work->emit6C);
                }
            }
            if (frame < 14) {
                CVECTOR col;
                SVECTOR rot;
                VECTOR sc;

                col.r = col.g = col.b = 0x89;
                sc.vz = sc.vy = sc.vx = 0x1000;
                rot.vz = rot.vy = rot.vx = 0;
                ex->btlfx_drawScreenGrid(2, 0xA0, 0x78, 0x50, 0x3C, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
            }
            key = (Fx19fKey12 *)((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_MOTION_KEYS_2) + (frame * 12 + 4));
            work->pos48[0] = key->x + work->pos8.vx;
            work->pos48[1] = key->y + work->pos8.vy - 0x2EE;
            work->pos48[2] = key->z + work->pos8.vz;
            work->scale54[0] = key->rx * 2;
            work->scale54[1] = key->ry * 2;
            work->scale54[2] = key->rz * 2;
            work->rot60.vx = work->rot60.vy = work->rot60.vz = 0;
            if (frame >= FX_RAMUH_TAIL_FRAMES) {
                work->state0 = FX_RAMUH_STATE_TAIL;
                entry->frame = -1;
                ex->btlfx_clearEffModels();
                return 1;
            }
        } break;
        }
        {
            VECTOR pos;
            VECTOR sc;
            SVECTOR arcFrom;
            SVECTOR arcTo;

            pos.vx = work->pos48[0];
            pos.vy = work->pos48[1];
            pos.vz = work->pos48[2];
            pos.vy += ex->math_cos(work->frame4 * 0x70) >> 6;
            pos.vy += ex->math_sin(work->frame4 << 7) >> 6;
            ex->Hi_DrawEffModel(&work->rot60, &pos, (s32)work->scale54, work->model78);
            if (glowBright != 0) {
                sc.vx = work->scale54[0];
                sc.vy = work->scale54[1];
                sc.vz = work->scale54[2];
                sc.vx += glowGrow;
                sc.vy += glowGrowY;
                sc.vz += glowGrow;
                ex->Hi_DrawEffModel(&work->rot60, &pos, (s32)&sc, work->model7C);
                glowRgb = (glowBright >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model7C, glowRgb, glowRgb, glowRgb);
                ex->btlfx_drawScrollMove(work->frame4 * 5, FX_RAMUH_COLUMN_VRAM_X, FX_RAMUH_COLUMN_VRAM_Y, FX_RAMUH_COLUMN_VRAM_W, FX_RAMUH_COLUMN_VRAM_H, FX_RAMUH_COLUMN_SRC_X, FX_RAMUH_COLUMN_SRC_Y);
            }
            if (coreBright != 0) {
                ex->Hi_DrawEffModel(&work->rot60, &pos, (s32)work->scale54, work->model80);
                flashRgb = (((coreBright * 2) - 0x1000) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model80, flashRgb, flashRgb, flashRgb);
            }
            if (arcAlpha != 0) {
                arcAng = work->frame4 * 0x29E;
                arcFrom.vx = pos.vx;
                arcFrom.vy = pos.vy;
                arcFrom.vz = pos.vz;
                FX19F_W(arcTo, 0) = FX19F_W(arcFrom, 0);
                FX19F_W(arcTo, 1) = FX19F_W(arcFrom, 1);
                work->rot60.vx -= 0x400;
                ex->btlfx_pointAhead((s16 *)&arcFrom, (s16 *)&work->rot60, 0x5DC, (s16 *)&arcFrom);
                work->rot60.vx += 0x400;
                fxm_drawOffsetTrail(entry, &fx_19f_offsetTrail, &arcFrom, &arcTo, arcAmp, arcAng, 0xF0, work->frame4 & 7, arcAlpha, 0x40);
            }
        }
        env->otZ = 0x1EA;
        if (alpha != 0) {
            for (k = 0; k < 3; k++) {
                pad = 0x168;
                stripFrom = (s16 *)ex->btlfx_getFileData(entry->file, fx_19f_stripFromFileIds0[k]) + 2;
                stripTo = (s16 *)ex->btlfx_getFileData(entry->file, fx_19f_stripToFileIds0[k]) + 2;
                if (k != 0) {
                    pad = 0x80;
                    if (k == 1) {
                        pad = 0x58;
                    }
                }
                stripA = &fx_19f_stripTrails0[k * 2];
                pt = stripA->points4;
                stripA->lastSlot0 = stripA->count2 = 0x10;
                stripA->head1 = 0;
                stripB = &fx_19f_stripTrails0[k * 2 + 1];
                stripB->lastSlot0 = stripB->count2 = 0x10;
                stripB->head1 = 0;
                stripBPt = stripB->points4;
                for (j = step = 0; j < 34; j++, step++) {
                    if (j == 17) {
                        pt = stripBPt;
                        step--;
                    }
                    stripKey = headKey - step;
                    if (stripKey < 0) {
                        stripKey = 0;
                    }
                    stripKey *= 3;
                    pt->vx = work->pos8.vx + ex->btlfx_lerp(stripFrom[stripKey], stripTo[stripKey], stripT);
                    pt->vy = work->pos8.vy + ex->btlfx_lerp(stripFrom[stripKey + 1], stripTo[stripKey + 1], stripT);
                    pt->vz = work->pos8.vz + ex->btlfx_lerp(stripFrom[stripKey + 2], stripTo[stripKey + 2], stripT);
                    pt->pad = pad;
                    pt++;
                }
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_LIGHTNING_RIBBON), 0, NULL, (work->frame4 + k) & 7, stripScale, alpha,
                                    BTLFX_ABR_ADD, (u8 *)stripA, 2);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_LIGHTNING_RIBBON_2), 0, NULL, (work->frame4 + k) & 7, stripScale,
                                    alpha, BTLFX_ABR_ADD, (u8 *)stripB, 2);
            }
        }
        for (wave = 0; wave < 3; wave++) {
            if (waveAlpha[wave] != 0) {
                waveFrom = (s16 *)ex->btlfx_getFileData(entry->file, fx_19f_stripFromFileIds1[wave]) + 8;
                waveTo = (s16 *)ex->btlfx_getFileData(entry->file, fx_19f_stripToFileIds1[wave]);
                waveTo += 8;
                waveTrail = &fx_19f_stripTrails1[wave];
                waveTrail->lastSlot0 = waveTrail->count2 = 0xC;
                waveTrail->head1 = 0;
                wavePt = waveTrail->points4;
                for (seg = 0; seg < 13; seg++) {
                    waveKey = waveHead[wave] - seg;
                    if (waveKey < 0) {
                        waveKey = 0;
                    }
                    waveKey *= 3;
                    wavePt->vx = work->pos8.vx + ex->btlfx_lerp(waveFrom[waveKey], waveTo[waveKey], waveT[wave]) * 2;
                    wavePt->vy = work->pos8.vy + ex->btlfx_lerp(waveFrom[waveKey + 1], waveTo[waveKey + 1], waveT[wave]) * 3 / 2;
                    wavePt->vz = work->pos8.vz + ex->btlfx_lerp(waveFrom[waveKey + 2], waveTo[waveKey + 2], waveT[wave]) * 2;
                    wavePt->pad = 0xC8;
                    wavePt++;
                }
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_LIGHTNING_BOLT), 0, (s32 *)&work->pos8,
                                    (work->frame4 + wave) & 7, waveScale[wave], waveAlpha[wave], BTLFX_ABR_ADD, (u8 *)waveTrail, 1);
            }
        }
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

s32 fx_19f_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR camPt;
    Fx19fKeys *keys;
    s32 frame;
    s32 key;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        fx_19f_env = env;
    } else {
        frame = entry->frame;
        keys = (Fx19fKeys *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CAMERA_KEYS);
        key = frame;
        if (key >= keys->unk2) {
            key = keys->unk2 - 1;
        }
        keys = (Fx19fKeys *)((u8 *)keys + (key * 6 + 4));
        camPt.vx = keys->unk0;
        camPt.vy = keys->unk2 - 100;
        camPt.vz = ((s16 *)keys)[2];
        env->camera->points[0].vx = camPt.vx;
        env->camera->points[0].vy = camPt.vy;
        env->camera->points[0].vz = camPt.vz;
        keys = (Fx19fKeys *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CAMERA_KEYS_2);
        key = frame;
        if (key >= keys->unk2) {
            key = keys->unk2 - 1;
        }
        keys = (Fx19fKeys *)((u8 *)keys + (key * 6 + 4));
        camPt.vx = keys->unk0;
        camPt.vy = keys->unk2 - 100;
        camPt.vz = ((s16 *)keys)[2];
        env->camera->points[1].vx = camPt.vx;
        env->camera->points[1].vy = camPt.vy;
        env->camera->points[1].vz = camPt.vz;
        if (frame >= 0x5A) {
            return 1;
        }
    }
    return 0;
}

static void fxm_drawJaggedTrail(Unk80100C78Entry *entry, s32 file, s32 mode, Fx19fTrail *trail, s32 size, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 amp0, s32 amp1, s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 dip) {
    Unk800F78A4 *env = fx_19f_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 count;
    s32 t;
    s32 offset;
    SVECTOR *pt;
    s32 halfT;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    count = (size - 4) / 8;
    trail->lastSlot0 = trail->count2 = count - 1;
    trail->head1 = 0;
    pt = trail->points4;
    for (i = 0; i < count; i++, pt++) {
        t = 0x1000 - (i << 12) / (count - 1);
        t += *noise++ * 2 / 3;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = *noise++;
        halfT = t >> 1;
        offset = (ex->math_sin(halfT) * offset) >> 12;
        offset += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        if (dip != 0) {
            pt->vy -= (ex->math_sin(halfT) * dip) >> 12;
        }
        pt->pad = lineWidth;
        ang += dang + *noise++;
    }
    env->otZ = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, file), 0, NULL, frame, 0x1000, fade, BTLFX_ABR_ADD, (u8 *)trail, mode);
    env->otZ = 0;
}

static void fxm_drawJaggedTrailBranch(Unk80100C78Entry *entry, s32 file, s32 mode, Fx19fTrail *path, Fx19fTrail *trail, s32 size, s32 *noise,
                   s32 branchAt, s32 frame, s32 fade, s32 lineWidth) {
    SVECTOR *pt = path->points4;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_19f_env);
    SVECTOR mid;

    pt += branchAt;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    fxm_drawJaggedTrail(entry, file, mode, trail, size, noise, pt, &mid, 0, 0x20, ex->btlfx_rand(), 0x33, frame, fade, lineWidth, 0);
}

static s32 fx_19f_op83PoolTask(s32 mode, Fx19fBolt *bolt, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 alpha;
    s32 noiseStep;
    s32 width;
    s32 kind;
    Fx19fTrail *trail;
    Fx19fTrail *branch;

    if (mode != 1) {
        kind = bolt->kind11;
        if (kind < 2) {
            if (kind < 0) {
                return 0;
            }
            t = frame << 9;
            alpha = ex->math_cos(t >> 2);
            trail = (Fx19fTrail *)&fx_19f_boltTrails[bolt->index10];
            branch = (Fx19fTrail *)&fx_19f_branchTrails[bolt->index10];
            if (bolt->kind11 == 0) {
                bolt->pos8.vx = fx_19f_boltCenter.vx;
                bolt->pos8.vy = fx_19f_boltCenter.vy;
                bolt->pos8.vz = fx_19f_boltCenter.vz;
                fxm_drawJaggedTrail(entry, 0x1E, 0, trail, 0x8C, bolt->noise18, &bolt->pos0, &bolt->pos8, 0, 0x180, frame * 0x97, 0x140, frame,
                              alpha, ((0x1000 - t) * bolt->alpha15) >> 12, ((t * 3 * 128) >> 12) + 0x80);
            } else {
                if (frame == 0) {
                    noiseStep = 2;
                } else if (frame < 4) {
                    noiseStep = 3;
                } else {
                    noiseStep = 0;
                }
                width = ((0x1000 - t) * bolt->alpha15) >> 12;
                if (frame == 0) {
                    width = (width * 3) >> 1;
                }
                fxm_drawJaggedTrail(entry, 2, 0, trail, 0x8C, bolt->noise18 + frame * noiseStep, &bolt->pos0, &bolt->pos8, 0, 0x180, frame * 0xC0,
                              0x168, frame, alpha, width, 0);
                fxm_drawJaggedTrailBranch(entry, 3, 0, trail, branch, 0x24, bolt->noise18, bolt->index12, frame, alpha, width >> 2);
            }
            if (frame >= 8) {
                return 1;
            }
        }
    }
    return 0;
}

static s32 fxm_randStripPoint32(SVECTOR *out, SVECTOR *dir, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_19f_env);
    SVECTOR *src = fx_19f_boltTrailPoints[ex->btlfx_rand() % 32].points0;

    src += ex->btlfx_rand() % 17;

    out->vx = src->vx;
    out->vy = src->vy;
    out->vz = src->vz;
    out->pad = 1;
    return 0;
}

s32 fx_19f_op83(s32 mode, Fx19fWork83 *work, Unk800F78A4 *env, Fx19fInst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR base;
    SVECTOR start;
    SVECTOR end;
    SVECTOR segStart;
    SVECTOR segEnd;
    s32 frame;
    s32 i;
    s32 n;
    s32 radius;
    s32 ang;
    s32 alpha;
    s32 dist;
    Fx19fBolt *bolt;
    Fx19fBolt *strand;
    s32 *noise;
    s32 jitter;

    if (mode == 0) {
        init->workSize = sizeof(Fx19fWork83);
        init->elemSize = sizeof(Fx19fBolt);
        init->elemFn = (s32)fx_19f_op83PoolTask;
        init->poolCount = FX_RAMUH_BOLT_SLOTS;
    } else if (mode == 1) {
        i = init->workSize;
        fx_19f_env = env;
        work->noise0 = fx_19f_noise[0];
        noise = fx_19f_noise[0];
        for (n = 0; n < 64; n++) {
            *noise++ = ex->btlfx_randCentered(0xD2);
            *noise++ = ex->btlfx_randCentered(0x1C3);
            *noise++ = ex->btlfx_randCentered(0xDC);
        }
        work->kind8 = i;
        work->count4 = 0;
    } else {
        frame = inst->frame0;
        switch (work->kind8) {
        case 0:
            FX19F_W(base, 0) = FX19F_W(fx_19f_boltCenter, 0);
            FX19F_W(base, 1) = FX19F_W(fx_19f_boltCenter, 1);
            if (frame >= 0x14) {
                return 1;
            }
            work->count4++;
            FX19F_W(start, 0) = FX19F_W(base, 0);
            start.vy = 0;
            FX19F_W(start, 1) = FX19F_W(base, 1);
            FX19F_W(end, 0) = FX19F_W(base, 0);
            FX19F_W(end, 1) = FX19F_W(base, 1);
            bolt = ex->btlfx_poolSpawn(inst->pool10);
            if (bolt != NULL) {
                radius = ex->btlfx_randCentered(0x200) + 0x7D0;
                jitter = ex->btlfx_randCentered(0x200) - 0x200;
                ang = ((work->count4 / 2) << 10) + jitter;
                start.vx += (ex->math_cos(ang) * radius) >> 12;
                start.vz += (ex->math_sin(ang) * radius) >> 12;
                ((s32 *)&bolt->pos0)[0] = FX19F_W(start, 0);
                ((s32 *)&bolt->pos0)[1] = FX19F_W(start, 1);
                ((s32 *)&bolt->pos8)[0] = FX19F_W(end, 0);
                ((s32 *)&bolt->pos8)[1] = FX19F_W(end, 1);
                bolt->kind11 = 0;
                bolt->index10 = work->count4 % 32;
                bolt->noise18 = work->noise0 + (ex->btlfx_rand() & 0x1F);
                bolt->index12 = ex->btlfx_rand() % 15;
                bolt->unk13 = ex->btlfx_rand() % 15;
                bolt->alpha15 = 0x3E;
                bolt->unk14 = 0;
            }
            work->count4++;
            FX19F_W(start, 0) = FX19F_W(base, 0);
            FX19F_W(end, 0) = FX19F_W(base, 0);
            start.vy = 0;
            FX19F_W(start, 1) = FX19F_W(base, 1);
            FX19F_W(end, 1) = FX19F_W(base, 1);
            if (frame >= 0xB) {
                bolt = ex->btlfx_poolSpawn(inst->pool10);
                if (bolt != NULL) {
                    radius = ex->btlfx_randCentered(0x100) + 0x3E8;
                    ang = ((work->count4 / 2) << 10) + ex->btlfx_randCentered(0x200);
                    start.vx += (ex->math_cos(ang) * radius) >> 12;
                    start.vz += (ex->math_sin(ang) * radius) >> 12;
                    ((s32 *)&bolt->pos0)[0] = FX19F_W(start, 0);
                    ((s32 *)&bolt->pos0)[1] = FX19F_W(start, 1);
                    ((s32 *)&bolt->pos8)[0] = FX19F_W(end, 0);
                    ((s32 *)&bolt->pos8)[1] = FX19F_W(end, 1);
                    bolt->kind11 = 0;
                    bolt->index10 = work->count4 % 32;
                    bolt->noise18 = work->noise0 + (ex->btlfx_rand() & 0xF) * 3;
                    bolt->index12 = ex->btlfx_rand() % 15;
                    bolt->unk13 = ex->btlfx_rand() % 15;
                    bolt->alpha15 = 0x32;
                    bolt->unk14 = 0;
                }
            }
            if (frame >= 0x14) {
                return 1;
            }
            break;
        case 1:
            FX19F_W(start, 0) = FX19F_W(fx_19f_boltCenter, 0);
            FX19F_W(start, 1) = FX19F_W(fx_19f_boltCenter, 1);
            if (frame < 0x18) {
                alpha = fx_19f_boltLength * 100 / 2048 + 0x3C;
                if (alpha >= 0x100) {
                    alpha = 0xFF;
                }
                for (i = 0; i < 2; i++) {
                    work->count4++;
                    FX19F_W(segStart, 0) = FX19F_W(start, 0);
                    FX19F_W(segEnd, 0) = FX19F_W(start, 0);
                    FX19F_W(segStart, 1) = FX19F_W(start, 1);
                    FX19F_W(segEnd, 1) = FX19F_W(start, 1);
                    strand = ex->btlfx_poolSpawn(inst->pool10);
                    if (strand != NULL) {
                        dist = fx_19f_boltLength;
                        ex->btlfx_randRotation(&end);
                        ex->btlfx_pointAhead((s16 *)&segEnd, (s16 *)&end, 0x180, (s16 *)&segStart);
                        ex->btlfx_pointAhead((s16 *)&segEnd, (s16 *)&end, dist + 0x180, (s16 *)&segEnd);
                        ((s32 *)&strand->pos0)[0] = FX19F_W(segStart, 0);
                        ((s32 *)&strand->pos0)[1] = FX19F_W(segStart, 1);
                        ((s32 *)&strand->pos8)[0] = FX19F_W(segEnd, 0);
                        ((s32 *)&strand->pos8)[1] = FX19F_W(segEnd, 1);
                        strand->kind11 = 1;
                        strand->index10 = work->count4 % 32;
                        strand->noise18 = work->noise0 + (ex->btlfx_rand() & 0x1F);
                        strand->index12 = ex->btlfx_rand() % 15;
                        strand->unk13 = ex->btlfx_rand() % 15;
                        strand->alpha15 = alpha;
                        strand->unk14 = 0;
                    }
                }
                if (frame >= 0xD) {
                    for (i = 0; i < 2; i++) {
                        work->count4++;
                        FX19F_W(segStart, 0) = FX19F_W(start, 0);
                        FX19F_W(segEnd, 0) = FX19F_W(start, 0);
                        FX19F_W(segStart, 1) = FX19F_W(start, 1);
                        FX19F_W(segEnd, 1) = FX19F_W(start, 1);
                        strand = ex->btlfx_poolSpawn(inst->pool10);
                        if (strand != NULL) {
                            dist = fx_19f_boltLength;
                            ex->btlfx_randRotation(&end);
                            ex->btlfx_pointAhead((s16 *)&segEnd, (s16 *)&end, 0x180, (s16 *)&segStart);
                            ex->btlfx_pointAhead((s16 *)&segEnd, (s16 *)&end, dist + 0x180, (s16 *)&segEnd);
                            ((s32 *)&strand->pos0)[0] = FX19F_W(segStart, 0);
                            ((s32 *)&strand->pos0)[1] = FX19F_W(segStart, 1);
                            ((s32 *)&strand->pos8)[0] = FX19F_W(segEnd, 0);
                            ((s32 *)&strand->pos8)[1] = FX19F_W(segEnd, 1);
                            strand->kind11 = 1;
                            strand->index10 = work->count4 % 32;
                            strand->noise18 = work->noise0 + (ex->btlfx_rand() & 0xF) * 3;
                            strand->index12 = ex->btlfx_rand() % 15;
                            strand->unk13 = ex->btlfx_rand() % 15;
                            strand->alpha15 = alpha - 0x32;
                            strand->unk14 = 0;
                        }
                    }
                }
            }
            if (frame >= 0x18) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_19f/nonmatchings/fx_19f", fx_19f_headerPtr);
