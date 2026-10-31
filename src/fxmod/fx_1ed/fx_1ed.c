#include "fxmod/fx_1ed.h"

static void fxm_camKeyReset(void) {
    s32 i;
    s32 m;

    fx_1ed_camFrame = 0;
    m = -1;
    for (i = 15; i >= 0; i--) {
        fx_1ed_camKeys[i].timer4 = m;
    }
}

static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (fx_1ed_camKeys[i].timer4 == -1) {
            fx_1ed_camKeys[i].timer4 = 0;
            fx_1ed_camKeys[i].keys10 = data + 1;
            fx_1ed_camKeys[i].frame0 = start;
            fx_1ed_camKeys[i].slot8 = dst;
            fx_1ed_camKeys[i].slotC = src;
            return;
        }
    }
}

static void fxm_camKeyUpdate(void) {
    SVECTOR *ext = fx_1ed_camKeyPoints;
    s32 i;
    Unk800F78A4 *env = fx_1ed_env;
    s32 frame = fx_1ed_camFrame;
    s32 k;
    s32 d;
    s32 s;
    s32 x, y, z;
    s32 j;

    for (i = 0; i < 16; i++) {
        if (fx_1ed_camKeys[i].timer4 == 0 && fx_1ed_camKeys[i].frame0 == frame) {
            fx_1ed_camKeys[i].timer4 = *fx_1ed_camKeys[i].keys10;
            fx_1ed_camKeys[i].keys10++;
        }
        if (fx_1ed_camKeys[i].timer4 > 0) {
            fx_1ed_camKeys[i].timer4--;
            for (k = 0; k < 3; k++) {
                d = ((fx_1ed_camKeys[i].slot8 >> (k * 4)) & 0xF) - 1;
                s = ((fx_1ed_camKeys[i].slotC >> (k * 4)) & 0xF) - 1;
                if (d >= 0) {
                    x = *fx_1ed_camKeys[i].keys10;
                    fx_1ed_camKeys[i].keys10++;
                    y = *fx_1ed_camKeys[i].keys10;
                    fx_1ed_camKeys[i].keys10++;
                    z = *fx_1ed_camKeys[i].keys10;
                    fx_1ed_camKeys[i].keys10++;
                    if (s >= 0) {
                        /* each branch adds its own source; jump2's cross-jumping merges the identical tails */
                        if (s >= 10) {
                            j = s - 10;
                            x += ext[j].vx;
                            y += ext[j].vy;
                            z += ext[j].vz;
                        } else {
                            x += ((SVECTOR *)env->camera)[s].vx;
                            y += ((SVECTOR *)env->camera)[s].vy;
                            z += ((SVECTOR *)env->camera)[s].vz;
                        }
                    }
                    if (d >= 10) {
                        j = d - 10;
                        ext[j].vx = x;
                        ext[j].vy = y;
                        ext[j].vz = z;
                    } else {
                        ((SVECTOR *)env->camera)[d].vx = x;
                        ((SVECTOR *)env->camera)[d].vy = y;
                        ((SVECTOR *)env->camera)[d].vz = z;
                    }
                }
            }
        }
    }
    fx_1ed_camFrame++;
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1ed_env);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

s32 fx_1ed_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *p;
    Pool6CEntry *e;
    Pool6CEntry *e2;
    Pool6CEntry *e3;
    SVECTOR *q;
    Fx1edTrail *tr;
    Fx1edTrail *tr2;
    s32 frame;
    s32 a;
    s32 c;
    s32 y;
    s32 i;
    u8 n = 0x15;
    s32 t;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        fx_1ed_env = env;
        for (i = 7; i >= 0; i--) {
            fx_1ed_timers[i] = 0;
        }
        fx_1ed_emitter0 = NULL;
        fx_1ed_emitter1 = NULL;
        fx_1ed_emitter2 = NULL;
        fx_1ed_emitter3 = NULL;
        fx_1ed_emitter4 = NULL;
        fx_1ed_emitter5 = NULL;
        fx_1ed_emitter6 = NULL;
        fx_1ed_emitter7 = NULL;
        fx_1ed_emitter8 = NULL;
        fx_1ed_emitter10 = NULL;
        fx_1ed_emitter9 = NULL;
        fx_1ed_emitter9Pos.vx = 0;
        fx_1ed_emitter9Pos.vy = 0;
        fx_1ed_emitter9Pos.vz = -0x618;
        fx_1ed_modelRot.vx = 0;
        fx_1ed_modelRot.vy = 0;
        fx_1ed_modelRot.vz = 0;
        fx_1ed_modelPos.vx = 0;
        fx_1ed_modelPos.vy = 0;
        fx_1ed_modelPos.vz = 0;
        fx_1ed_modelScale.vx = 0x1000;
        fx_1ed_modelScale.vy = 0x1000;
        fx_1ed_modelScale.vz = 0x1000;
        fx_1ed_emitPos0.vx = 0;
        fx_1ed_emitPos0.vy = 0;
        fx_1ed_emitPos0.vz = -0x2EE;
        fxm_camKeyReset();
        fxm_camKeyAdd(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, 3));
        fxm_camKeyAdd(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, 4));
        fxm_camKeyAdd(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, 5));
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1D), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&fx_1ed_trails[0], 0);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1D), 0, NULL, -1, 0x1000, 0x1000, 1, (u8 *)&fx_1ed_trails[1], 0);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 1:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&fx_1ed_emitPos0);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&fx_1ed_emitPos0);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1B), (s32)&fx_1ed_emitPos0);
            fx_1ed_timers[5] = 0x10;
            break;
        case 9:
            ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion180, SUMMON(env)->model60);
            fx_1ed_timers[0] = 1;
            fx_1ed_emitter0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&fx_1ed_emitter0Pos);
            break;
        case 0x18:
            fx_1ed_emitter1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&fx_1ed_emitter1Pos);
            break;
        case 0x40:
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0x11, (u16 *)&fx_1ed_emitter2Pos);
            fx_1ed_emitter2 = e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&fx_1ed_emitter2Pos);
            if (e != NULL) {
                e->trails = (s32)fx_1ed_op80Trails;
                e->trailSlots = 0x28;
                e->trailStride = 0xAC;
                e->otZ = 0x50;
            }
            break;
        case 0x48:
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0x11, (u16 *)&fx_1ed_emitter3Pos);
            fx_1ed_emitter3 = e2 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&fx_1ed_emitter3Pos);
            if (e2 != NULL) {
                e2->otZ = 0x40;
            }
            break;
        case 0x67:
            ex->btlfx_stopEmitter(fx_1ed_emitter3);
            break;
        case 0x6C:
            ex->btlfx_stopEmitter(fx_1ed_emitter2);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0, (u16 *)&fx_1ed_emitter4Pos);
            fx_1ed_emitter4Pos.vy -= 0x180;
            fx_1ed_emitter4 = e3 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&fx_1ed_emitter4Pos);
            if (e3 != NULL) {
                e3->otZ = -0x80;
            }
            fx_1ed_timers[1] = 0x10;
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0x11, (u16 *)&fx_1ed_emitter5Pos);
            fx_1ed_emitter5 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&fx_1ed_emitter5Pos);
            fx_1ed_timers[2] = 8;
            fx_1ed_timers[3] = 1;
            break;
        case 0x77:
            fx_1ed_timers[0] = 0;
            ex->btlfx_stopEmitter(fx_1ed_emitter0);
            ex->btlfx_stopEmitter(fx_1ed_emitter1);
            fx_1ed_emitter6Pos.vy = fx_1ed_emitter5Pos.vy - 0x1800;
            fx_1ed_emitter6Pos.vx = fx_1ed_emitter5Pos.vx;
            fx_1ed_emitter6Pos.vz = fx_1ed_emitter5Pos.vz;
            fx_1ed_emitter6 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&fx_1ed_emitter6Pos);
            break;
        case 0x97:
            ex->btlfx_stopEmitter(fx_1ed_emitter4);
            ex->btlfx_stopEmitter(fx_1ed_emitter5);
            ex->btlfx_stopEmitter(fx_1ed_emitter6);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xB0, 0xB0, 0xB0);
            fx_1ed_emitter7Pos.vy = fx_1ed_emitter6Pos.vy;
            fx_1ed_emitter7Pos.vx = fx_1ed_emitter6Pos.vx;
            fx_1ed_emitter7Pos.vz = fx_1ed_emitter6Pos.vz;
            fx_1ed_emitter7 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&fx_1ed_emitter7Pos);
            break;
        case 0x99:
            fx_1ed_timers[4] = 4;
            fx_1ed_emitter8Pos.vx = fx_1ed_emitter10Pos.vx = fx_1ed_emitter6Pos.vx;
            fx_1ed_emitter8Pos.vy = fx_1ed_emitter10Pos.vy = fx_1ed_emitter6Pos.vy;
            fx_1ed_emitter8Pos.vz = fx_1ed_emitter10Pos.vz = fx_1ed_emitter6Pos.vz;
            fx_1ed_emitter8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&fx_1ed_emitter8Pos);
            break;
        case 0x9F:
            fx_1ed_emitter10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&fx_1ed_emitter10Pos);
            break;
        case 0xAE:
            fx_1ed_emitter9 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&fx_1ed_emitter9Pos);
            break;
        case 0xCA:
            ex->btlfx_stopEmitter(fx_1ed_emitter10);
            break;
        }
        if (fx_1ed_timers[0] != 0) {
            fx_1ed_modelPos.vx = env->camera->points[2].vx;
            fx_1ed_modelPos.vy = env->camera->points[2].vy;
            fx_1ed_modelPos.vz = env->camera->points[2].vz;
            ex->Hi_DrawSummonModel(&fx_1ed_modelRot, &fx_1ed_modelPos, (s32)&fx_1ed_modelScale, SUMMON(env)->model60, 1);
            if (fx_1ed_timers[1] != 0) {
                fx_1ed_timers[1]--;
                ex->Hi_ModifySummonModelRGB(SUMMON(env)->model60, -0x40, -0x40, -0x40);
            }
        }
        if (fx_1ed_emitter0 != NULL) {
            if (fx_1ed_emitter0->state == BTLFX_EMIT_DONE) {
                fx_1ed_emitter0 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0, (u16 *)&fx_1ed_emitter0Pos);
        }
        if (fx_1ed_emitter1 != NULL) {
            if (fx_1ed_emitter1->state == BTLFX_EMIT_DONE) {
                fx_1ed_emitter1 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0, (u16 *)&fx_1ed_emitter1Pos);
        }
        if (fx_1ed_emitter3 != NULL) {
            if (fx_1ed_emitter3->state == BTLFX_EMIT_DONE) {
                fx_1ed_emitter3 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0x11, (u16 *)&fx_1ed_emitter3Target);
            ex->btlfx_moveEmitterTo((s32)fx_1ed_emitter3, (s32)&fx_1ed_emitter3Target);
        }
        if (fx_1ed_timers[3] != 0) {
            tr = fx_1ed_trails;
            p = tr->pos4;
            tr->lastSlot0 = tr->count2 = 0x14;
            tr->head1 = 0;
            tr2 = tr + 1;
            tr2->lastSlot0 = tr2->count2 = 0x14;
            tr2->head1 = 0;
            do {
                /* empty: the original has a block boundary here (an emptied macro?) */
            } while (0);
            q = tr[1].pos4;
            if (fx_1ed_timers[2] != 0) {
                fx_1ed_timers[2]--;
                y = fx_1ed_emitter5Pos.vy - fxm_easeLerp(1, 8, fx_1ed_timers[2], 0, 0x1800);
            } else {
                y = fx_1ed_emitter5Pos.vy - 0x1800;
            }
            if (fx_1ed_timers[4] != 0) {
                fx_1ed_timers[4]--;
                if (fx_1ed_timers[4] == 0) {
                    fx_1ed_timers[3] = 0;
                }
                c = fx_1ed_timers[4] << 4;
                a = fx_1ed_timers[4] << 10;
            } else {
                c = 0x40;
                a = 0x1000;
            }
            for (i = 0; i < n; i++) {
                p->vy = q->vy = ex->btlfx_lerpRatio(fx_1ed_emitter5Pos.vy, y, i, 0x15);
                p->vx = q->vx = fx_1ed_emitter5Pos.vx;
                p->vz = q->vz = fx_1ed_emitter5Pos.vz;
                p->pad = c;
                q->pad = c * 2;
                p++;
                q++;
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1D), 0, NULL, frame, 0x1000, a, 1, (u8 *)&fx_1ed_trails[0], 0);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1D), 0, NULL, frame, 0x1000, a, 1, (u8 *)&fx_1ed_trails[1], 0);
        }
        if (fx_1ed_timers[5] != 0) {
            fx_1ed_timers[5]--;
            if (fx_1ed_timers[5] >= 9) {
                t = (0x10 - fx_1ed_timers[5]) << 4;
            } else {
                t = fx_1ed_timers[5] << 4;
            }
            col.r = col.g = col.b = t;
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

s32 fx_1ed_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 i;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        fx_1ed_env = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&fx_1ed_op81TargetPos[i]);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&fx_1ed_op81TargetPos[i]);
        }
    } else {
        return entry->frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ed/nonmatchings/fx_1ed", fx_1ed_headerPtr);
