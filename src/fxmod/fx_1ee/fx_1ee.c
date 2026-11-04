#include "fxmod/fx_1ee.h"

static void fxm_camKeyReset(void) {
    s32 i;
    s32 freeMark;

    fx_1ee_camFrame = 0;
    freeMark = FX_1EE_KEY_FREE;
    for (i = 15; i >= 0; i--) {
        fx_1ee_camKeys[i].timer4 = freeMark;
    }
}

static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (fx_1ee_camKeys[i].timer4 == FX_1EE_KEY_FREE) {
            fx_1ee_camKeys[i].timer4 = 0;
            fx_1ee_camKeys[i].keys10 = data + 1;
            fx_1ee_camKeys[i].frame0 = start;
            fx_1ee_camKeys[i].slot8 = dst;
            fx_1ee_camKeys[i].slotC = src;
            return;
        }
    }
}

static void fxm_camKeyUpdate(void) {
    SVECTOR *ext = fx_1ee_camKeyPoints;
    s32 i;
    Unk800F78A4 *env = fx_1ee_env;
    s32 frame = fx_1ee_camFrame;
    s32 k;
    s32 dstSlot;
    s32 srcSlot;
    s32 x, y, z;
    s32 extIdx;

    for (i = 0; i < 16; i++) {
        if (fx_1ee_camKeys[i].timer4 == 0 && fx_1ee_camKeys[i].frame0 == frame) {
            fx_1ee_camKeys[i].timer4 = *fx_1ee_camKeys[i].keys10;
            fx_1ee_camKeys[i].keys10++;
        }
        if (fx_1ee_camKeys[i].timer4 > 0) {
            fx_1ee_camKeys[i].timer4--;
            for (k = 0; k < 3; k++) {
                dstSlot = BTLFX_CAM_SLOT(fx_1ee_camKeys[i].slot8, k);
                srcSlot = BTLFX_CAM_SLOT(fx_1ee_camKeys[i].slotC, k);
                if (dstSlot >= 0) {
                    x = *fx_1ee_camKeys[i].keys10;
                    fx_1ee_camKeys[i].keys10++;
                    y = *fx_1ee_camKeys[i].keys10;
                    fx_1ee_camKeys[i].keys10++;
                    z = *fx_1ee_camKeys[i].keys10;
                    fx_1ee_camKeys[i].keys10++;
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
    fx_1ee_camFrame++;
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1ee_env);
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

s32 fx_1ee_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *pt;
    Pool6CEntry *emitter2;
    Pool6CEntry *emitter3;
    Pool6CEntry *emitter4;
    SVECTOR *pt2;
    Fx1eeTrail *tr;
    Fx1eeTrail *tr2;
    s32 frame;
    s32 alpha;
    s32 lineWidth;
    s32 topY;
    s32 i;
    u8 n = 0x15;
    s32 shade;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1ee_env = env;
        for (i = 7; i >= 0; i--) {
            fx_1ee_timers[i] = 0;
        }
        fx_1ee_emitter0 = NULL;
        fx_1ee_emitter1 = NULL;
        fx_1ee_emitter2 = NULL;
        fx_1ee_emitter3 = NULL;
        fx_1ee_emitter4 = NULL;
        fx_1ee_emitter5 = NULL;
        fx_1ee_emitter6 = NULL;
        fx_1ee_emitter7 = NULL;
        fx_1ee_emitter8 = NULL;
        fx_1ee_emitter10 = NULL;
        fx_1ee_emitter9 = NULL;
        fx_1ee_emitter9Pos.vx = 0;
        fx_1ee_emitter9Pos.vy = 0;
        fx_1ee_emitter9Pos.vz = -0x618;
        fx_1ee_modelRot.vx = 0;
        fx_1ee_modelRot.vy = 0;
        fx_1ee_modelRot.vz = 0;
        fx_1ee_modelPos.vx = 0;
        fx_1ee_modelPos.vy = 0;
        fx_1ee_modelPos.vz = 0;
        fx_1ee_modelScale.vx = 0x1000;
        fx_1ee_modelScale.vy = 0x1000;
        fx_1ee_modelScale.vz = 0x1000;
        fx_1ee_emitPos0.vx = 0;
        fx_1ee_emitPos0.vy = 0;
        fx_1ee_emitPos0.vz = -0x2EE;
        fxm_camKeyReset();
        fxm_camKeyAdd(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_CAMERA_KEYS_3));
        fxm_camKeyAdd(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_CAMERA_KEYS));
        fxm_camKeyAdd(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_CAMERA_KEYS_2));
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LIGHT_BEAM), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&fx_1ee_trails[0], 0);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LIGHT_BEAM), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&fx_1ee_trails[1], 0);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 1:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_RING_BURST), (s32)&fx_1ee_emitPos0);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_CRYSTAL_SPARKLES), (s32)&fx_1ee_emitPos0);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_FAN_EMITTER), (s32)&fx_1ee_emitPos0);
            fx_1ee_timers[5] = 0x10;
            break;
        case 9:
            ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion180, SUMMON(env)->model60);
            fx_1ee_timers[0] = 1;
            fx_1ee_emitter0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW), (s32)&fx_1ee_emitter0Pos);
            break;
        case 0x18:
            fx_1ee_emitter1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW), (s32)&fx_1ee_emitter1Pos);
            break;
        case 0x40:
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0x11, (u16 *)&fx_1ee_emitter2Pos);
            fx_1ee_emitter2 = emitter2 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LENS_FLARE), (s32)&fx_1ee_emitter2Pos);
            if (emitter2 != NULL) {
                emitter2->trails = (s32)fx_1ee_op80Trails;
                emitter2->trailSlots = 0x28;
                emitter2->trailStride = 0xAC;
                emitter2->otZ = 0x50;
            }
            break;
        case 0x48:
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0x11, (u16 *)&fx_1ee_emitter3Pos);
            fx_1ee_emitter3 = emitter3 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_GLOW_RING), (s32)&fx_1ee_emitter3Pos);
            if (emitter3 != NULL) {
                emitter3->otZ = 0x40;
            }
            break;
        case 0x67:
            ex->btlfx_stopEmitter(fx_1ee_emitter3);
            break;
        case 0x6C:
            ex->btlfx_stopEmitter(fx_1ee_emitter2);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0, (u16 *)&fx_1ee_emitter4Pos);
            fx_1ee_emitter4Pos.vy -= 0x180;
            fx_1ee_emitter4 = emitter4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_RING_BURST_2), (s32)&fx_1ee_emitter4Pos);
            if (emitter4 != NULL) {
                emitter4->otZ = -0x80;
            }
            fx_1ee_timers[1] = 0x10;
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0x11, (u16 *)&fx_1ee_emitter5Pos);
            fx_1ee_emitter5 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LENS_STREAK), (s32)&fx_1ee_emitter5Pos);
            fx_1ee_timers[2] = 8;
            fx_1ee_timers[3] = 1;
            break;
        case 0x77:
            fx_1ee_timers[0] = 0;
            ex->btlfx_stopEmitter(fx_1ee_emitter0);
            ex->btlfx_stopEmitter(fx_1ee_emitter1);
            fx_1ee_emitter6Pos.vy = fx_1ee_emitter5Pos.vy - 0x1800;
            fx_1ee_emitter6Pos.vx = fx_1ee_emitter5Pos.vx;
            fx_1ee_emitter6Pos.vz = fx_1ee_emitter5Pos.vz;
            fx_1ee_emitter6 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_FLAME_GLINT), (s32)&fx_1ee_emitter6Pos);
            break;
        case 0x97:
            ex->btlfx_stopEmitter(fx_1ee_emitter4);
            ex->btlfx_stopEmitter(fx_1ee_emitter5);
            ex->btlfx_stopEmitter(fx_1ee_emitter6);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xB0, 0xB0, 0xB0);
            fx_1ee_emitter7Pos.vy = fx_1ee_emitter6Pos.vy;
            fx_1ee_emitter7Pos.vx = fx_1ee_emitter6Pos.vx;
            fx_1ee_emitter7Pos.vz = fx_1ee_emitter6Pos.vz;
            fx_1ee_emitter7 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_RING_BURST_3), (s32)&fx_1ee_emitter7Pos);
            break;
        case 0x99:
            fx_1ee_timers[4] = 4;
            fx_1ee_emitter8Pos.vx = fx_1ee_emitter10Pos.vx = fx_1ee_emitter6Pos.vx;
            fx_1ee_emitter8Pos.vy = fx_1ee_emitter10Pos.vy = fx_1ee_emitter6Pos.vy;
            fx_1ee_emitter8Pos.vz = fx_1ee_emitter10Pos.vz = fx_1ee_emitter6Pos.vz;
            fx_1ee_emitter8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LENS_STREAK_2), (s32)&fx_1ee_emitter8Pos);
            break;
        case 0x9F:
            fx_1ee_emitter10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW_3), (s32)&fx_1ee_emitter10Pos);
            break;
        case 0xAE:
            fx_1ee_emitter9 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW_2), (s32)&fx_1ee_emitter9Pos);
            break;
        case 0xCA:
            ex->btlfx_stopEmitter(fx_1ee_emitter10);
            break;
        }
        if (fx_1ee_timers[0] != 0) {
            fx_1ee_modelPos.vx = env->camera->points[2].vx;
            fx_1ee_modelPos.vy = env->camera->points[2].vy;
            fx_1ee_modelPos.vz = env->camera->points[2].vz;
            ex->Hi_DrawSummonModel(&fx_1ee_modelRot, &fx_1ee_modelPos, (s32)&fx_1ee_modelScale, SUMMON(env)->model60, 1);
            if (fx_1ee_timers[1] != 0) {
                fx_1ee_timers[1]--;
                ex->Hi_ModifySummonModelRGB(SUMMON(env)->model60, -0x40, -0x40, -0x40);
            }
        }
        if (fx_1ee_emitter0 != NULL) {
            if (fx_1ee_emitter0->state == BTLFX_EMIT_DONE) {
                fx_1ee_emitter0 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0, (u16 *)&fx_1ee_emitter0Pos);
        }
        if (fx_1ee_emitter1 != NULL) {
            if (fx_1ee_emitter1->state == BTLFX_EMIT_DONE) {
                fx_1ee_emitter1 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0, (u16 *)&fx_1ee_emitter1Pos);
        }
        if (fx_1ee_emitter3 != NULL) {
            if (fx_1ee_emitter3->state == BTLFX_EMIT_DONE) {
                fx_1ee_emitter3 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0x11, (u16 *)&fx_1ee_emitter3Target);
            ex->btlfx_moveEmitterTo((s32)fx_1ee_emitter3, (s32)&fx_1ee_emitter3Target);
        }
        if (fx_1ee_timers[3] != 0) {
            tr = fx_1ee_trails;
            pt = tr->pos4;
            tr->lastSlot0 = tr->count2 = 0x14;
            tr->head1 = 0;
            tr2 = tr + 1;
            tr2->lastSlot0 = tr2->count2 = 0x14;
            tr2->head1 = 0;
            do {
                            } while (0);
            pt2 = tr[1].pos4;
            if (fx_1ee_timers[2] != 0) {
                fx_1ee_timers[2]--;
                topY = fx_1ee_emitter5Pos.vy - fxm_easeLerp(1, 8, fx_1ee_timers[2], 0, 0x1800);
            } else {
                topY = fx_1ee_emitter5Pos.vy - 0x1800;
            }
            if (fx_1ee_timers[4] != 0) {
                fx_1ee_timers[4]--;
                if (fx_1ee_timers[4] == 0) {
                    fx_1ee_timers[3] = 0;
                }
                lineWidth = fx_1ee_timers[4] << 4;
                alpha = fx_1ee_timers[4] << 10;
            } else {
                lineWidth = 0x40;
                alpha = 0x1000;
            }
            for (i = 0; i < n; i++) {
                pt->vy = pt2->vy = ex->btlfx_lerpRatio(fx_1ee_emitter5Pos.vy, topY, i, 0x15);
                pt->vx = pt2->vx = fx_1ee_emitter5Pos.vx;
                pt->vz = pt2->vz = fx_1ee_emitter5Pos.vz;
                pt->pad = lineWidth;
                pt2->pad = lineWidth * 2;
                pt++;
                pt2++;
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LIGHT_BEAM), 0, NULL, frame, 0x1000, alpha, 1, (u8 *)&fx_1ee_trails[0], 0);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_LIGHT_BEAM), 0, NULL, frame, 0x1000, alpha, 1, (u8 *)&fx_1ee_trails[1], 0);
        }
        if (fx_1ee_timers[5] != 0) {
            fx_1ee_timers[5]--;
            if (fx_1ee_timers[5] >= 9) {
                shade = (0x10 - fx_1ee_timers[5]) << 4;
            } else {
                shade = fx_1ee_timers[5] << 4;
            }
            col.r = col.g = col.b = shade;
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

/* row r: first spark uses file a, the other three file b */
#define LOAD(r, a, b) \
    fx_1ee_sparks[r][0].tex8 = (u8 *)ex->btlfx_getFileData(entry->file, a); \
    fx_1ee_sparks[r][1].tex8 = (u8 *)ex->btlfx_getFileData(entry->file, b); \
    fx_1ee_sparks[r][2].tex8 = (u8 *)ex->btlfx_getFileData(entry->file, b); \
    fx_1ee_sparks[r][3].tex8 = (u8 *)ex->btlfx_getFileData(entry->file, b);

s32 fx_1ee_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1eeSpark *spark;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 j;
    s32 fade;
    s32 scale;
    s32 offX;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1ee_op81Timer[0] = 0;
        fx_1ee_env = env;
        LOAD(0, 3, 0x12)
        LOAD(1, 8, 0xF)
        LOAD(2, 0x15, 0x10)
        LOAD(3, 0x16, 0xD)
        LOAD(4, 0x18, 0xE)
        LOAD(5, 0x19, 0xB)
        LOAD(6, 0x1A, 0xC)
        LOAD(7, 0x1B, 9)
        LOAD(8, 0x1C, 0xA)
        LOAD(9, 1, 6)
        LOAD(10, 4, 7)
        LOAD(11, 0x11, 5)
        for (i = 0; i < 12; i++) {
            for (j = 0; j < 4; j++) {
                fx_1ee_sparks[i][j].angleC = i * -0x2A9 + j * 0xC0;
                fx_1ee_sparks[i][j].radius10 = j * 16 + 16;
                fx_1ee_sparks[i][j].timer0 = i + 4 - j;
                fx_1ee_sparks[i][j].timer4 = 8;
            }
        }
        fx_1ee_sparkOrigin.vx = env->camera->points[3].vx;
        fx_1ee_sparkOrigin.vy = env->camera->points[3].vy;
        fx_1ee_sparkOrigin.vz = env->camera->points[3].vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            fx_1ee_op81Timer[0] = 0x20;
            break;
        case 8:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_VIOLET_RINGS), (s32)&fx_1ee_sparkOrigin);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            break;
        case 0x18:
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&fx_1ee_op81TargetPos[i]);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_GREEN_RING), (s32)&fx_1ee_op81TargetPos[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x80);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_YELLOW_RED_RINGS), (s32)&fx_1ee_op81TargetPos[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x80);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_CLOCK_FACE), (s32)&fx_1ee_op81TargetPos[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x80);
                }
            }
            break;
        }
        if (fx_1ee_op81Timer[0] != 0) {
            fx_1ee_op81Timer[0]--;
            env->otZ = 0x200;
            for (i = 0; i < 12; i++) {
                for (j = 0; j < 4; j++) {
                    spark = (Fx1eeSpark *)(i * 0x50 + j * 0x14 + (s32)fx_1ee_sparks);
                    if (spark->timer0 != 0) {
                        spark->timer0--;
                    } else if (spark->timer4 != 0) {
                        spark->angleC -= 0x40;
                        spark->radius10 -= 2;
                        spark->timer4--;
                        scale = (spark->radius10 << 8) + 0x4000;
                        fade = spark->timer4 << 9;
                        offX = (ex->math_sin(spark->angleC) * spark->radius10) >> 12;
                        ex->btlfx_drawParticles(spark->tex8, &fx_1ee_sparkOrigin, NULL, scale, 0, 1, fade, offX,
                                                (ex->math_cos(spark->angleC) * spark->radius10) >> 12);
                    }
                }
            }
            env->otZ = 0;
        }
        return frame >= 0x40;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ee/nonmatchings/fx_1ee", fx_1ee_headerPtr);
