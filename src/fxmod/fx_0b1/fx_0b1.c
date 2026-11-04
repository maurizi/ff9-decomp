#include "fxmod/fx_0b1.h"

static void fxm_camKeyReset(void) {
    s32 i;
    s32 freeMark;

    fx_0b1_camKeyFrame = 0;
    freeMark = -1;
    for (i = 15; i >= 0; i--) {
        fx_0b1_camKeys[i].framesLeft4 = freeMark;
    }
}

static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (fx_0b1_camKeys[i].framesLeft4 == -1) {
            fx_0b1_camKeys[i].framesLeft4 = 0;
            fx_0b1_camKeys[i].data10 = data + 1;
            fx_0b1_camKeys[i].start0 = start;
            fx_0b1_camKeys[i].dst8 = dst;
            fx_0b1_camKeys[i].srcC = src;
            return;
        }
    }
}

static void fxm_camKeyUpdate(void) {
    SVECTOR *ext = fx_0b1_camKeyPoints;
    s32 i;
    Unk800F78A4 *env = fx_0b1_env;
    s32 frame = fx_0b1_camKeyFrame;
    s32 k;
    s32 dstSlot;
    s32 srcSlot;
    s32 x, y, z;
    s32 extIdx;

    for (i = 0; i < 16; i++) {
        if (fx_0b1_camKeys[i].framesLeft4 == 0 && fx_0b1_camKeys[i].start0 == frame) {
            fx_0b1_camKeys[i].framesLeft4 = *fx_0b1_camKeys[i].data10;
            fx_0b1_camKeys[i].data10++;
        }
        if (fx_0b1_camKeys[i].framesLeft4 > 0) {
            fx_0b1_camKeys[i].framesLeft4--;
            for (k = 0; k < 3; k++) {
                dstSlot = BTLFX_CAM_SLOT(fx_0b1_camKeys[i].dst8, k);
                srcSlot = BTLFX_CAM_SLOT(fx_0b1_camKeys[i].srcC, k);
                if (dstSlot >= 0) {
                    x = *fx_0b1_camKeys[i].data10;
                    fx_0b1_camKeys[i].data10++;
                    y = *fx_0b1_camKeys[i].data10;
                    fx_0b1_camKeys[i].data10++;
                    z = *fx_0b1_camKeys[i].data10;
                    fx_0b1_camKeys[i].data10++;
                    if (srcSlot >= 0) {
                        /* each branch adds its own source; jump2's cross-jumping merges the identical tails */
                        if (srcSlot >= 10) {
                            extIdx = srcSlot - 10;
                            x += ext[extIdx].vx;
                            y += ext[extIdx].vy;
                            z += ext[extIdx].vz;
                        } else {
                            x += ((SVECTOR *)env->camera)[srcSlot].vx;
                            y += ((SVECTOR *)env->camera)[srcSlot].vy;
                            z += ((SVECTOR *)env->camera)[srcSlot].vz;
                        }
                    }
                    if (dstSlot >= 10) {
                        extIdx = dstSlot - 10;
                        ext[extIdx].vx = x;
                        ext[extIdx].vy = y;
                        ext[extIdx].vz = z;
                    } else {
                        ((SVECTOR *)env->camera)[dstSlot].vx = x;
                        ((SVECTOR *)env->camera)[dstSlot].vy = y;
                        ((SVECTOR *)env->camera)[dstSlot].vz = z;
                    }
                }
            }
        }
    }
    fx_0b1_camKeyFrame++;
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0b1_env);
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

s32 fx_0b1_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *pt0;
    Pool6CEntry *emit2;
    Pool6CEntry *emit3;
    Pool6CEntry *emit4;
    SVECTOR *pt1;
    Fx0b1Trail *tr;
    Fx0b1Trail *tr2;
    s32 frame;
    s32 alpha;
    s32 lineWidth;
    s32 topY;
    s32 i;
    u8 count = 0x15;
    s32 bright;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_0b1_env = env;
        for (i = 7; i >= 0; i--) {
            fx_0b1_timers[i] = 0;
        }
        fx_0b1_emitter0 = NULL;
        fx_0b1_emitter1 = NULL;
        fx_0b1_emitter2 = NULL;
        fx_0b1_emitter3 = NULL;
        fx_0b1_emitter4 = NULL;
        fx_0b1_emitter5 = NULL;
        fx_0b1_emitter6 = NULL;
        fx_0b1_emitter7 = NULL;
        fx_0b1_emitter8 = NULL;
        fx_0b1_emitter10 = NULL;
        fx_0b1_emitter9 = NULL;
        fx_0b1_emitPos9.vx = 0;
        fx_0b1_emitPos9.vy = 0;
        fx_0b1_emitPos9.vz = -0x618;
        fx_0b1_summonRot.vx = 0;
        fx_0b1_summonRot.vy = 0;
        fx_0b1_summonRot.vz = 0;
        fx_0b1_summonPos.vx = 0;
        fx_0b1_summonPos.vy = 0;
        fx_0b1_summonPos.vz = 0;
        fx_0b1_summonScale.vx = 0x1000;
        fx_0b1_summonScale.vy = 0x1000;
        fx_0b1_summonScale.vz = 0x1000;
        fx_0b1_startPos.vx = 0;
        fx_0b1_startPos.vy = 0;
        fx_0b1_startPos.vz = -0x2EE;
        fxm_camKeyReset();
        fxm_camKeyAdd(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_CAMERA_KEYS_3));
        fxm_camKeyAdd(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_CAMERA_KEYS));
        fxm_camKeyAdd(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_CAMERA_KEYS_2));
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LIGHT_BEAM), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&fx_0b1_beams[0], 0);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LIGHT_BEAM), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&fx_0b1_beams[1], 0);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 1:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_RING_BURST), (s32)&fx_0b1_startPos);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_CRYSTAL_SPARKLES), (s32)&fx_0b1_startPos);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_FAN_EMITTER), (s32)&fx_0b1_startPos);
            fx_0b1_timers[5] = 0x10;
            break;
        case 9:
            ex->Hi_SetSummonMotion(SUMMON0B1(env)->summon54->motion180, SUMMON0B1(env)->model60);
            fx_0b1_timers[0] = 1;
            fx_0b1_emitter0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW), (s32)&fx_0b1_emitPos0);
            break;
        case 0x18:
            fx_0b1_emitter1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW), (s32)&fx_0b1_emitPos1);
            break;
        case 0x40:
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->model60, 0x11, (u16 *)&fx_0b1_emitPos2);
            fx_0b1_emitter2 = emit2 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LENS_FLARE), (s32)&fx_0b1_emitPos2);
            if (emit2 != NULL) {
                emit2->trails = (s32)fx_0b1_trails;
                emit2->trailSlots = 0x28;
                emit2->trailStride = 0xAC;
                emit2->otZ = 0x50;
            }
            break;
        case 0x48:
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->model60, 0x11, (u16 *)&fx_0b1_emitStartPos3);
            fx_0b1_emitter3 = emit3 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_RED_RING), (s32)&fx_0b1_emitStartPos3);
            if (emit3 != NULL) {
                emit3->otZ = 0x40;
            }
            break;
        case 0x67:
            ex->btlfx_stopEmitter(fx_0b1_emitter3);
            break;
        case 0x6C:
            ex->btlfx_stopEmitter(fx_0b1_emitter2);
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->model60, 0, (u16 *)&fx_0b1_emitPos4);
            fx_0b1_emitPos4.vy -= 0x180;
            fx_0b1_emitter4 = emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_RING_BURST_2), (s32)&fx_0b1_emitPos4);
            if (emit4 != NULL) {
                emit4->otZ = -0x80;
            }
            fx_0b1_timers[1] = 0x10;
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->model60, 0x11, (u16 *)&fx_0b1_emitPos5);
            fx_0b1_emitter5 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LENS_STREAK), (s32)&fx_0b1_emitPos5);
            fx_0b1_timers[2] = 8;
            fx_0b1_timers[3] = 1;
            break;
        case 0x77:
            fx_0b1_timers[0] = 0;
            ex->btlfx_stopEmitter(fx_0b1_emitter0);
            ex->btlfx_stopEmitter(fx_0b1_emitter1);
            fx_0b1_emitPos6.vy = fx_0b1_emitPos5.vy - 0x1800;
            fx_0b1_emitPos6.vx = fx_0b1_emitPos5.vx;
            fx_0b1_emitPos6.vz = fx_0b1_emitPos5.vz;
            fx_0b1_emitter6 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_FLAME_GLINT), (s32)&fx_0b1_emitPos6);
            break;
        case 0x97:
            ex->btlfx_stopEmitter(fx_0b1_emitter4);
            ex->btlfx_stopEmitter(fx_0b1_emitter5);
            ex->btlfx_stopEmitter(fx_0b1_emitter6);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xB0, 0xB0, 0xB0);
            fx_0b1_emitPos7.vy = fx_0b1_emitPos6.vy;
            fx_0b1_emitPos7.vx = fx_0b1_emitPos6.vx;
            fx_0b1_emitPos7.vz = fx_0b1_emitPos6.vz;
            fx_0b1_emitter7 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_RING_BURST_3), (s32)&fx_0b1_emitPos7);
            break;
        case 0x99:
            fx_0b1_timers[4] = 4;
            fx_0b1_emitPos8.vx = fx_0b1_emitPos10.vx = fx_0b1_emitPos6.vx;
            fx_0b1_emitPos8.vy = fx_0b1_emitPos10.vy = fx_0b1_emitPos6.vy;
            fx_0b1_emitPos8.vz = fx_0b1_emitPos10.vz = fx_0b1_emitPos6.vz;
            fx_0b1_emitter8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LENS_STREAK_2), (s32)&fx_0b1_emitPos8);
            break;
        case 0x9F:
            fx_0b1_emitter10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW_3), (s32)&fx_0b1_emitPos10);
            break;
        case 0xAE:
            fx_0b1_emitter9 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW_2), (s32)&fx_0b1_emitPos9);
            break;
        case 0xCA:
            ex->btlfx_stopEmitter(fx_0b1_emitter10);
            break;
        }
        if (fx_0b1_timers[0] != 0) {
            fx_0b1_summonPos.vx = env->camera->points[2].vx;
            fx_0b1_summonPos.vy = env->camera->points[2].vy;
            fx_0b1_summonPos.vz = env->camera->points[2].vz;
            ex->Hi_DrawSummonModel(&fx_0b1_summonRot, &fx_0b1_summonPos, (s32)&fx_0b1_summonScale, SUMMON0B1(env)->model60, 1);
            if (fx_0b1_timers[1] != 0) {
                fx_0b1_timers[1]--;
                ex->Hi_ModifySummonModelRGB(SUMMON0B1(env)->model60, -0x40, -0x40, -0x40);
            }
        }
        if (fx_0b1_emitter0 != NULL) {
            if (fx_0b1_emitter0->state == BTLFX_EMIT_DONE) {
                fx_0b1_emitter0 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->model60, 0, (u16 *)&fx_0b1_emitPos0);
        }
        if (fx_0b1_emitter1 != NULL) {
            if (fx_0b1_emitter1->state == BTLFX_EMIT_DONE) {
                fx_0b1_emitter1 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->model60, 0, (u16 *)&fx_0b1_emitPos1);
        }
        if (fx_0b1_emitter3 != NULL) {
            if (fx_0b1_emitter3->state == BTLFX_EMIT_DONE) {
                fx_0b1_emitter3 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON0B1(env)->model60, 0x11, (u16 *)&fx_0b1_emitPos3);
            ex->btlfx_moveEmitterTo((s32)fx_0b1_emitter3, (s32)&fx_0b1_emitPos3);
        }
        if (fx_0b1_timers[3] != 0) {
            tr = fx_0b1_beams;
            pt0 = tr->pts4;
            tr->lastSlot0 = tr->count2 = 0x14;
            tr->head1 = 0;
            tr2 = tr + 1;
            tr2->lastSlot0 = tr2->count2 = 0x14;
            tr2->head1 = 0;
            do {
                /* empty: the original has a block boundary here (an emptied macro?) */
            } while (0);
            pt1 = tr[1].pts4;
            if (fx_0b1_timers[2] != 0) {
                fx_0b1_timers[2]--;
                topY = fx_0b1_emitPos5.vy - fxm_easeLerp(1, 8, fx_0b1_timers[2], 0, 0x1800);
            } else {
                topY = fx_0b1_emitPos5.vy - 0x1800;
            }
            if (fx_0b1_timers[4] != 0) {
                fx_0b1_timers[4]--;
                if (fx_0b1_timers[4] == 0) {
                    fx_0b1_timers[3] = 0;
                }
                lineWidth = fx_0b1_timers[4] << 4;
                alpha = fx_0b1_timers[4] << 10;
            } else {
                lineWidth = 0x40;
                alpha = 0x1000;
            }
            for (i = 0; i < count; i++) {
                pt0->vy = pt1->vy = ex->btlfx_lerpRatio(fx_0b1_emitPos5.vy, topY, i, 0x15);
                pt0->vx = pt1->vx = fx_0b1_emitPos5.vx;
                pt0->vz = pt1->vz = fx_0b1_emitPos5.vz;
                pt0->pad = lineWidth;
                pt1->pad = lineWidth * 2;
                pt0++;
                pt1++;
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LIGHT_BEAM), 0, NULL, frame, 0x1000, alpha, 1, (u8 *)&fx_0b1_beams[0], 0);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LIGHT_BEAM), 0, NULL, frame, 0x1000, alpha, 1, (u8 *)&fx_0b1_beams[1], 0);
        }
        if (fx_0b1_timers[5] != 0) {
            fx_0b1_timers[5]--;
            if (fx_0b1_timers[5] >= 9) {
                bright = (0x10 - fx_0b1_timers[5]) << 4;
            } else {
                bright = fx_0b1_timers[5] << 4;
            }
            col.r = col.g = col.b = bright;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, 0);
        }
        fxm_camKeyUpdate();
        return frame >= 0x10A;
    }
    return 0;
}

s32 fx_0b1_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 bright;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_0b1_fadeTimer = 0;
        fx_0b1_env = env;
        ex->btlfx_initEffModels(fx_0b1_effModels, 4);
        for (i = 0; i < env->targets->targetCount; i++) {
            fx_0b1_models[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_STAR_CRYSTAL), 0x177,
                                                       0xFFFF, 0, 0x80);
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&fx_0b1_bodyPos[i]);
            fx_0b1_modelPos[i].vx = fx_0b1_bodyPos[i].vx;
            fx_0b1_modelPos[i].vy = fx_0b1_bodyPos[i].vy;
            fx_0b1_modelPos[i].vz = fx_0b1_bodyPos[i].vz + 0x100;
            fx_0b1_bodyPos[i].vz += 0x200;
        }
        fx_0b1_modelRot.vx = 0;
        fx_0b1_modelRot.vy = 0x800;
        fx_0b1_modelRot.vz = 0;
        fx_0b1_modelScale.vx = 0x1800;
        fx_0b1_modelScale.vy = 0x1800;
        fx_0b1_modelScale.vz = 0x1800;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            do {
                fx_0b1_fadeTimer = 0x20;
            } while (0);
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_startEmitter((s32)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_PINK_RING), (s32)&fx_0b1_bodyPos[i]);
                ex->btlfx_startEmitter((s32)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_COLOR_RINGS), (s32)&fx_0b1_bodyPos[i]);
            }
        }
        if (fx_0b1_fadeTimer != 0) {
            fx_0b1_fadeTimer--;
            do {
                for (i = 0; i < env->targets->targetCount; i++) {
                    ex->Hi_DrawEffModel(&fx_0b1_modelRot, &fx_0b1_modelPos[i], (s32)&fx_0b1_modelScale, fx_0b1_models[i]);
                    if (fx_0b1_fadeTimer >= 0x19) {
                        bright = (0x18 - fx_0b1_fadeTimer) * 0x10;
                        ex->Hi_ModifyEffModelRGB(fx_0b1_models[i], bright, bright, bright);
                    }
                    if (fx_0b1_fadeTimer < 8) {
                        bright = (fx_0b1_fadeTimer - 8) * 0x10;
                        ex->Hi_ModifyEffModelRGB(fx_0b1_models[i], bright, bright, bright);
                    }
                }
            } while (0);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_BACKDROP), frame);
        }
        return frame >= 0x20;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b1/nonmatchings/fx_0b1", fx_0b1_headerPtr);
