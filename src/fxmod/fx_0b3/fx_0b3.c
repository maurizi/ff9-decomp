#include "fxmod/fx_0b3.h"

static void fxm_keyGetVec(Fx0b3Track *track, s32 t, SVECTOR *out) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    track = (Fx0b3Track *)&track->keys4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

static void fxm_keyGetVec2(Fx0b3Track *track, s32 t, SVECTOR *out0, SVECTOR *out1) {
    s16 *key;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    key = &track->keys4[t * 6];
    if (out0 != NULL) {
        out0->vx = key[0];
        out0->vy = key[1];
        out0->vz = key[2];
    }
    if (out1 != NULL) {
        out1->vx = key[3];
        out1->vy = key[4];
        out1->vz = key[5];
    }
}

static void fxm_keyGetVec3(Fx0b3Track *track, s32 t, SVECTOR *out0, SVECTOR *out1, SVECTOR *out2) {
    s16 *key;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    key = &track->keys4[t * 9];
    if (out0 != NULL) {
        out0->vx = key[0];
        out0->vy = key[1];
        out0->vz = key[2];
    }
    if (out1 != NULL) {
        out1->vx = key[3];
        out1->vy = key[4];
        out1->vz = key[5];
    }
    if (out2 != NULL) {
        out2->vx = key[6];
        out2->vy = key[7];
        out2->vz = key[8];
    }
}

static s32 fxm_keyGetValue(Fx0b3Track *track, s32 t) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    return (track->keys4 + t)[0];
}

static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 wrapW = x % w;
    Unk800F78A4 *env = fx_0b3_env;
    u32 *move = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 restW;
    RECT rectRest;
    RECT rectWrap;

    env->prim = (u8 *)(move + 12);
    restW = w - wrapW;
    if (wrapW != 0) {
        rectWrap.x = sx;
        rectWrap.y = sy;
        rectWrap.w = wrapW;
        rectWrap.h = h;
        ex->SetDrawMove(move, &rectWrap, dx + restW, dy);
        addPrim(env->ot + OT_LAST, move);
    }
    move += 6;
    if (restW != 0) {
        rectRest.x = sx + wrapW;
        rectRest.y = sy;
        rectRest.w = restW;
        rectRest.h = h;
        ex->SetDrawMove(move, &rectRest, dx, dy);
        addPrim(env->ot + OT_LAST, move);
    }
}

s32 fx_0b3_op80(s32 mode, Fx0b3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR ang;
    s32 frame;
    s32 bright;
    s32 i;
    s32 nextFrame;
    s32 t;
    s32 tint;
    s32 rgb;

    if (mode == 0) {
        *args = sizeof(Fx0b3Work80);
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0b3_env = env;
        ex->btlfx_initEffModels(fx_0b3_effModels, 0x10);
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 8), 1);
        work->morphScale34.vx = 0x1800;
        work->morphScale34.vy = 0x1800;
        work->morphScale34.vz = 0x1800;
        work->modelScale54.vx = 0x2800;
        work->modelScale54.vy = 0x2800;
        work->modelScale54.vz = 0x2800;
        work->rot40.vx = 0;
        work->rot40.vy = 0;
        work->rot40.vz = 0;
        work->morphPos28.vx = 0;
        work->morphPos28.vy = 0;
        work->morphPos28.vz = 0;
        work->rot60.vx = 0;
        work->rot60.vy = 0;
        work->rot60.vz = 0;
        work->modelPos48.vx = 0;
        work->modelPos48.vy = -0x100;
        work->modelPos48.vz = 0;
        work->emit68 = NULL;
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            t = frame << 12;
            /* Dead store: only its sign word (t >> 31) survives, shared with t / 20 below. */
            bright = t / 30;
            if (frame < 20) {
                bright = t / 20;
            } else {
                bright = 0x1000;
            }
            ex->Hi_DrawEffModel(&work->rot60, (VECTOR *)&work->modelPos48, (s32)&work->modelScale54, work->model6C);
            ex->Hi_ModifyEffModelRGB(work->model6C, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
            if (frame == 20) {
                for (i = 0; i < 6; i++) {
                    fx_0b3_morphModels[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0b3_morphBlocks[i]), 0);
                }
                work->morphWeight74 = 0x1000;
                work->morphFrame70 = 0;
                work->emit68 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
                if (work->emit68 != NULL) {
                    work->emit68->otZ = 0x80;
                }
            }
            if (frame >= 20) {
                fxm_keyGetVec((Fx0b3Track *)ex->btlfx_getFileData(entry->file, 0xB), frame, &pos);
                work->morphPos28.vx = pos.vx;
                work->morphPos28.vy = pos.vy;
                work->morphPos28.vz = pos.vz;
                BTLFX_COPY_SVEC(&work->pos10, &pos);
                work->morphWeight74 -= 0x800;
                if (work->morphWeight74 <= 0) {
                    work->morphWeight74 = 0x1000;
                    if (++work->morphFrame70 >= 6) {
                        work->morphFrame70 = 0;
                    }
                }
                ang.vx = 0;
                ang.vy = 0x800;
                ang.vz = 0;
                if (work->emit68 != NULL) {
                    if (work->emit68->state == BTLFX_EMIT_DONE) {
                        work->emit68 = NULL;
                    }
                    if (work->emit68 != NULL) {
                        work->emit68->rot.vx = ang.vx;
                        work->emit68->rot.vy = ang.vy;
                        work->emit68->rot.vz = ang.vz;
                    }
                }
                nextFrame = work->morphFrame70 + 1;
                if (nextFrame >= 6) {
                    nextFrame = 0;
                }
                ex->Hi_DrawMorphEffModel(&work->rot40, (VECTOR *)&work->morphPos28, (s32)&work->morphScale34,
                                         fx_0b3_morphModels[work->morphFrame70], fx_0b3_morphModels[nextFrame], work->morphWeight74);
                if (frame < 8) {
                    rgb = ((frame << 12) >> 8) - 0x80;
                    ex->Hi_ModifyEffModelRGB(fx_0b3_morphModels[work->morphFrame70], rgb, rgb, rgb);
                }
            }
            if (frame >= 0x33) {
                tint = (frame - 0x33) * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, tint, tint, tint);
            }
            if (frame >= 0x3B) {
                ex->Hi_FreeEffModel(work->model6C);
                ex->btlfx_stopEmitter(work->emit68);
                return 1;
            }
        }
        ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
        work->frame4++;
    }
    return 0;
}

s32 fx_0b3_op81(s32 mode, Fx0b3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR keyPos;
    SVECTOR keyScale;
    SVECTOR summonScale;
    SVECTOR summonPos;
    SVECTOR rot;
    Fx0b3Vec3 scale;
    Fx0b3Vec3 pos;
    SVECTOR sc;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 nextFrame;
    s32 tint;

    if (mode == 0) {
        *args = sizeof(Fx0b3Work81);
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0b3_env = env;
        fx_0b3_seaModel = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x89), 1);
        work->model88[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x86), 1);
        work->model88[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x87), 0);
        work->model88[2] = work->model88[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x88), 1);
        work->rot58.vy = 0x800;
        work->rot58.vx = 0;
        work->rot58.vz = 0;
        work->scale4C.vx = 0x200;
        work->scale4C.vy = 0x200;
        work->scale4C.vz = 0x200;
        work->pos40.vx = 0;
        work->pos40.vy = 0;
        work->pos40.vz = 0;
        ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->loader)->summon54->motion190, ((Fx0b3Summon *)env->loader)->model60);
        ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->loader)->model60, 0x28);
        work->rot78.vx = 0;
        work->rot78.vy = 0;
        work->rot78.vz = 0;
        work->modelScale6C.vx = 0x2800;
        work->modelScale6C.vy = 0x2800;
        work->modelScale6C.vz = 0x2800;
        work->modelPos60.vx = 0;
        work->modelPos60.vy = 0;
        work->modelPos60.vz = 0;
        work->emit80 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
        if (work->emit80 != NULL) {
            work->emit80->otZ = 0x80;
        }
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            ex->Hi_DrawEffModel(&work->rot78, (VECTOR *)&work->modelPos60, (s32)&work->modelScale6C, fx_0b3_seaModel);
            if (frame >= 0x2B) {
                fxm_keyGetVec3((u8 *)ex->btlfx_getFileData(entry->file, 3), frame, &summonPos, &work->rot58, &summonScale);
                work->pos40.vx = summonPos.vx;
                work->pos40.vy = summonPos.vy;
                work->pos40.vz = summonPos.vz;
                work->scale4C.vx = summonScale.vx;
                work->scale4C.vy = summonScale.vy;
                work->scale4C.vz = summonScale.vz;
                ex->Hi_DrawSummonModel(&work->rot58, (VECTOR *)&work->pos40, (s32)&work->scale4C,
                                       ((Fx0b3Summon *)env->loader)->model60, 1);
            }
            if (frame < 8) {
                tint = 0xFF - frame * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, tint, tint, tint);
            }
            if (frame == 0) {
                work->morphWeight9C = 0x1000;
                work->morphFrame98 = 0;
            }
            fxm_keyGetVec2((u8 *)ex->btlfx_getFileData(entry->file, 0x20), frame, &keyPos, &keyScale);
            work->pos40.vx = keyPos.vx;
            work->pos40.vy = keyPos.vy;
            work->pos40.vz = keyPos.vz;
            BTLFX_COPY_SVEC(&work->pos10, &keyPos);
            work->scale4C.vx = keyScale.vx;
            work->scale4C.vy = keyScale.vy;
            work->scale4C.vz = keyScale.vz;
            work->morphWeight9C -= 0x800;
            if (work->morphWeight9C <= 0) {
                work->morphWeight9C = 0x1000;
                if (++work->morphFrame98 >= 6) {
                    work->morphFrame98 = 0;
                }
            }
            nextFrame = work->morphFrame98 + 1;
            if (nextFrame >= 6) {
                nextFrame = 0;
            }
            ex->Hi_DrawMorphEffModel(&work->rot58, (VECTOR *)&work->pos40, (s32)&work->scale4C, fx_0b3_morphModels[work->morphFrame98],
                                     fx_0b3_morphModels[nextFrame], work->morphWeight9C);
            rot.vx = 0;
            rot.vy = 0x800;
            rot.vz = 0;
            if (work->emit80 != NULL) {
                if (work->emit80->state == BTLFX_EMIT_DONE) {
                    work->emit80 = NULL;
                }
                if (work->emit80 != NULL) {
                    work->emit80->rot.vx = rot.vx;
                    work->emit80->rot.vy = rot.vy;
                    work->emit80->rot.vz = rot.vz;
                }
            }
            if (frame >= 0x25) {
                for (i = 0; i < 4; i++) {
                    if (frame >= fx_0b3_partStart[i]) {
                        rot.vz = 0;
                        rot.vy = 0;
                        rot.vx = 0;
                        fxm_keyGetVec2((u8 *)ex->btlfx_getFileData(entry->file, fx_0b3_partKeyBlocks[i]), frame, &keyPos, &sc);
                        pos.vx = keyPos.vx;
                        pos.vy = keyPos.vy;
                        pos.vz = keyPos.vz;
                        scale.vx = sc.vx;
                        scale.vy = sc.vy;
                        scale.vz = sc.vz;
                        ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model88[i]);
                    }
                }
                ex->btlfx_drawScrollMove(frame * 14, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            }
            if (frame == 0x21) {
                work->pos28.vx = 0xD5C;
                work->pos28.vy = 0;
                work->pos28.vz = 0x3797;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame == 0x26) {
                work->pos30.vy = -0x3000;
                work->pos30.vx = 0;
                work->pos30.vz = 0x483;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->otZ = 0x200;
                }
            }
            fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x2E), frame, &work->pos38);
            work->pos38.vy = -0x100;
            if (frame == 0) {
                work->emit84 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->pos38);
                if (work->emit84 != NULL) {
                    work->emit84->otZ = 0x100;
                }
            }
            ex->btlfx_drawScrollMove(frame * 16, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
            if (frame >= 0x36) {
                ex->btlfx_stopEmitter(work->emit80);
                ex->btlfx_stopEmitter(work->emit84);
                return 1;
            }
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_0b3_op83(s32 mode, Fx0b3Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR keyPos;
    Fx0b3Vec3 scale;
    Fx0b3Vec3 pos;
    SVECTOR rot;
    SVECTOR sc;
    Fx0b3Vec3 pos2;
    Fx0b3Vec3 scale2;
    SVECTOR rot2;
    SVECTOR keyScale;
    Pool6CEntry *emitter;
    s32 frame;
    s32 morphFade;
    s32 morphT;
    s32 morphIdx;
    s32 i;
    s32 bright;
    s32 modelFade;
    s32 x;
    s32 y;

    if (mode == 0) {
        *args = sizeof(Fx0b3Work83);
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0b3_env = env;
        work->model78[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8B), 1);
        work->model78[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8A), 1);
        work->model78[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8C), 1);
        work->model78[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8D), 1);
        work->summonScale44.vx = 0x200;
        work->summonScale44.vy = 0x200;
        work->summonScale44.vz = 0x200;
        work->summonPos38.vy = 0x1C;
        work->summonPos38.vz = -0x37;
        work->rot50.vx = 0;
        work->rot50.vy = 0;
        work->rot50.vz = 0;
        work->summonPos38.vx = 0;
        work->rot70.vx = 0;
        work->rot70.vy = 0;
        work->rot70.vz = 0;
        work->modelScale64.vx = 0x2800;
        work->modelScale64.vy = 0x2800;
        work->modelScale64.vz = 0x2800;
        work->modelPos58.vx = 0;
        work->modelPos58.vy = 0;
        work->modelPos58.vz = 0;
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            ex->Hi_DrawEffModel(&work->rot70, (VECTOR *)&work->modelPos58, (s32)&work->modelScale64, fx_0b3_seaModel);
            if (frame == 0) {
                ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->loader)->summon54->motion184, ((Fx0b3Summon *)env->loader)->model60);
                ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->loader)->model60, 6);
            } else if (frame == 0x42) {
                ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->loader)->summon54->motion188, ((Fx0b3Summon *)env->loader)->model60);
                ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->loader)->model60, 0);
            }
            fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x13), frame, &keyPos);
            work->summonPos38.vx = keyPos.vx;
            work->summonPos38.vy = keyPos.vy;
            work->summonPos38.vz = keyPos.vz;
            ex->Hi_DrawSummonModel(&work->rot50, (VECTOR *)&work->summonPos38, (s32)&work->summonScale44,
                                   ((Fx0b3Summon *)env->loader)->model60, 1);
            if ((u32)(frame - 0xF) < 0x10) {
                bright = ex->math_cos(((frame - 0xF) << 10) >> 4);
                ex->Hi_ModifySummonModelRGB(((Fx0b3Summon *)env->loader)->model60, bright >> 6, bright >> 5, bright >> 5);
            }
            if (frame < 0x28) {
                if (frame >= 0x1D) {
                    morphFade = 0x1000 - ((frame - 0x1D) << 12) / 10;
                } else {
                    morphFade = 0x1000;
                }
                if (frame < 0x19) {
                    morphIdx = 0;
                    morphT = (frame << 12) / 24;
                } else {
                    morphIdx = 1;
                    morphT = ((frame - 0x19) << 12) / 45;
                }
                for (i = 0; i < 2; i++) {
                    fxm_keyGetVec3((u8 *)ex->btlfx_getFileData(entry->file, fx_0b3_morphKeyBlocks[i]), frame, &keyPos, &rot, &sc);
                    pos.vx = keyPos.vx;
                    pos.vy = keyPos.vy;
                    pos.vz = keyPos.vz;
                    scale.vx = sc.vx;
                    scale.vy = sc.vy;
                    scale.vz = sc.vz;
                    ex->Hi_DrawMorphEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model78[morphIdx], work->model78[morphIdx + 1],
                                             0x1000 - morphT);
                    if (morphFade != 0x1000) {
                        ex->Hi_ModifyEffModelRGB(work->model78[morphIdx], (morphFade >> 5) - 0x80, (morphFade >> 5) - 0x80, (morphFade >> 5) - 0x80);
                    }
                }
            }
            if (frame < 0x14) {
                if (frame >= 0xA) {
                    modelFade = 0x1000 - ((frame - 0xA) << 12) / 10;
                } else {
                    modelFade = 0x1000;
                }
                pos2.vx = 0;
                pos2.vy = 0;
                pos2.vz = -0x2F;
                rot2.vx = 0;
                rot2.vy = 0;
                rot2.vz = 0;
                fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x2D), frame, &keyScale);
                scale2.vx = keyScale.vx;
                scale2.vy = keyScale.vy;
                scale2.vz = keyScale.vz;
                ex->Hi_DrawEffModel(&rot2, (VECTOR *)&pos2, (s32)&scale2, work->model78[3]);
                if (modelFade != 0x1000) {
                    ex->Hi_ModifyEffModelRGB(work->model78[3], (modelFade >> 5) - 0x80, (modelFade >> 5) - 0x80, (modelFade >> 5) - 0x80);
                }
            }
            if (frame == 9) {
                work->pos18.vx = 0;
                work->pos18.vy = -0x1DB;
                work->pos18.vz = 0;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos18);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos18);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == 0x19) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == 0x2C) {
                work->pos20.vx = 0;
                work->pos20.vy = -0x258;
                work->pos20.vz = 0;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2F), (s32)&work->pos20);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0b3_trails;
                    emitter->trailSlots = 0xF0;
                    emitter->trailStride = 0x1C;
                    emitter->otZ = 0x40;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x30), (s32)&work->pos18);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
            ex->btlfx_drawScrollMove(frame * 14, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x77) {
                work->state0 = 1;
                return 1;
            }
        }
        work->frame4++;
    }
    return 0;
}

static void fxm_drawWaveWall(SVECTOR *pos, SVECTOR *rot, Fx0b3Vec3 *scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 az,
                   s32 ay, s32 ph, s32 dph, s32 ax, s32 phx, s32 dphx) {
    Unk800F78A4 *env = fx_0b3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR colFlat;
    CVECTOR colLit;
    CVECTOR colDark;
    CVECTOR colFoam;
    CVECTOR colFoamDark;
    MATRIX m;
    MATRIX worldMat;
    SVECTOR quad[4];
    VECTOR lv;
    s32 otz;
    POLY_FT4 *ft4;
    POLY_GT4 *gt4;
    POLY_G4 *poly;
    s32 i;
    s32 zz;
    s32 j;
    s32 y;
    s32 tileU;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 ft;
    s32 tileV;
    s32 startX;

    /* startX is a variable: reload rematerializes the constant into a register for startX + zz */
    startX = -10000;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    ft4 = (POLY_FT4 *)env->prim;
    *(s32 *)&colDark = 0;
    *(s32 *)&colFoamDark = 0;
    gt4 = (POLY_GT4 *)(ft4 + 100);
    tpage |= 0x20;
    poly = (POLY_G4 *)(gt4 + 20);
    env->prim = (u8 *)(poly + 20);
    colFlat.cd = 0x2C;
    colDark.cd = 0x3E;
    colLit.cd = 0x3E;
    colFoam.r = colFoam.g = colFoam.b = 0xFF;
    colFlat.r = colFlat.g = colFlat.b = bright;
    colLit.r = colLit.g = colLit.b = bright;
    colFoamDark.cd = 0x3A;
    colFoam.cd = 0x3A;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, (VECTOR *)scale);
    worldMat = m;
    worldMat.t[0] = pos->vx;
    worldMat.t[1] = pos->vy;
    worldMat.t[2] = pos->vz;
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    zz = 0;
    for (j = 0; j < 20; j++, zz += 1000) {
        y = 0;
        ph += dph;
        for (i = 0; i < 6; i++, y -= 1000) {
            quad[0].vx = (startX + zz) + ((ex->math_sin(phx + i * dphx) * ax) >> 12);
            quad[1].vx = quad[0].vx + 1000;
            quad[2].vx = (startX + zz) + ((ex->math_sin(phx + (i + 1) * dphx) * ax) >> 12);
            quad[3].vx = quad[2].vx + 1000;
            if (i != 0) {
                quad[0].vy = y + ((ex->math_sin(ph) * ay) >> 12);
                quad[2].vy = quad[0].vy - 1000;
                quad[1].vy = y + ((ex->math_sin(ph + dph) * ay) >> 12);
                quad[3].vy = quad[1].vy - 1000;
            } else {
                /* drop gives (sin * ay >> 12) - 1000 its own register, so it is added last */
                quad[0].vy = y;
                {
                    s32 drop = ((ex->math_sin(ph) * ay) >> 12) - 1000;
                    quad[2].vy = quad[0].vy + drop;
                }
                quad[1].vy = y;
                {
                    s32 drop = ((ex->math_sin(ph + dph) * ay) >> 12) - 1000;
                    quad[3].vy = quad[1].vy + drop;
                }
            }
            quad[0].vz = quad[1].vz = (ex->math_sin((i << 11) / 6) * az) >> 12;
            quad[2].vz = quad[3].vz = (ex->math_sin(((i + 1) << 11) / 6) * az) >> 12;
            gte_ldv3c(quad);
            gte_rtpt();
            tileU = j & 1;
            u0 = u + (tileU << 6);
            u1 = u0 + 0x40;
            if (tileU != 0) {
                u1 = u0 + 0x3F;
            }
            /* tileV is local to this block (tileU lives in several), so local-alloc gives it $v0 */
            tileV = i & 3;
            v0 = v + (tileV << 5);
            v1 = v0 + 0x20;
            if (tileV == 3) {
                v1 = v0 + 0x1F;
            }
            gte_stopz(&otz);
            if (otz != 0) {
                gte_avsz3();
                gte_stszotz(&otz);
                ft = 1;
                if (i >= 5) {
                    *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colLit;
                    *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colDark;
                    *(s32 *)&poly->r0 = *(s32 *)&poly->r1 = *(s32 *)&colFoam;
                    ft = 0;
                    *(s32 *)&poly->r2 = *(s32 *)&poly->r3 = *(s32 *)&colFoamDark;
                    gte_SetRotMatrix(&worldMat);
                    gte_SetTransMatrix(&worldMat);
                    gte_ldv0(&quad[2]);
                    gte_rtv0tr();
                    gte_stlvnl(&lv);
                    fx_0b3_crestPts[j].vx = lv.vx;
                    fx_0b3_crestPts[j].vy = lv.vy;
                    fx_0b3_crestPts[j].vz = lv.vz;
                    gte_SetRotMatrix(&m);
                    gte_SetTransMatrix(&m);
                } else {
                    *(s32 *)&ft4->r0 = *(s32 *)&colFlat;
                }
                if (ft) {
                    ft4->u0 = ft4->u2 = u0;
                    ft4->u1 = ft4->u3 = u1;
                    ft4->v0 = ft4->v1 = v0;
                    ft4->v2 = ft4->v3 = v1;
                    ft4->tpage = tpage;
                    ft4->clut = clut;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                } else {
                    gt4->u0 = gt4->u2 = u0;
                    gt4->u1 = gt4->u3 = u1;
                    gt4->v0 = gt4->v1 = v0;
                    gt4->v2 = gt4->v3 = v1;
                    gt4->tpage = tpage;
                    gt4->clut = clut;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                    *(s32 *)&poly->x0 = *(s32 *)&gt4->x0;
                    *(s32 *)&poly->x1 = *(s32 *)&gt4->x1;
                    *(s32 *)&poly->x2 = *(s32 *)&gt4->x2;
                }
                gte_ldv0(&quad[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, otz, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, otz, gt4);
                    ex->btlfx_addPrimAbr(0x08000000, otz, poly, 2);
                    gte_stsxy(&gt4->x3);
                    *(s32 *)&poly->x3 = *(s32 *)&gt4->x3;
                    gt4++;
                    poly++;
                }
            }
        }
    }
}

s32 fx_0b3_op84(s32 mode, Fx0b3Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pt;
    SVECTOR rot;
    Fx0b3Vec3 scale;
    Fx0b3Vec3 pos;
    SVECTOR rot2;
    Fx0b3Vec3 scale2;
    Fx0b3Vec3 pos2;
    SVECTOR rot3;
    Fx0b3Vec3 scale3;
    Pool6CEntry *emitter;
    s32 frame;
    s32 wallFrame;
    s32 bright;
    s32 i;
    s32 t;
    s32 x;
    s32 rgb;
    s32 z;

    if (mode == 0) {
        *args = sizeof(Fx0b3Work84);
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        work->rot60.vx = 0;
        work->rot60.vy = 0;
        work->rot60.vz = 0;
        work->summonScale54.vx = 0x200;
        work->summonScale54.vy = 0x200;
        work->summonScale54.vz = 0x200;
        work->summonPos48.vx = 0;
        work->summonPos48.vy = 0;
        work->summonPos48.vz = 0;
        fx_0b3_env = env;
        work->model88 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8E), 1);
        work->model8C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8F), 1);
        work->rot80.vx = 0;
        work->rot80.vy = 0;
        work->rot80.vz = 0;
        work->modelScale74.vx = 0x2800;
        work->modelScale74.vy = 0x2800;
        work->modelScale74.vz = 0x2800;
        work->modelPos68.vx = 0;
        work->modelPos68.vy = 0;
        work->modelPos68.vz = 0;
        ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->loader)->summon54->motion18C, ((Fx0b3Summon *)env->loader)->model60);
        ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->loader)->model60, 0x1A);
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            ex->Hi_DrawEffModel(&work->rot80, (VECTOR *)&work->modelPos68, (s32)&work->modelScale74, fx_0b3_seaModel);
            fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x15), frame, &pt);
            work->summonPos48.vx = pt.vx;
            work->summonPos48.vy = pt.vy;
            work->summonPos48.vz = pt.vz;
            work->pos30.vx = pt.vx;
            work->pos30.vy = pt.vy;
            work->pos30.vz = pt.vz;
            ex->Hi_DrawSummonModel(&work->rot60, (VECTOR *)&work->summonPos48, (s32)&work->summonScale54,
                                   ((Fx0b3Summon *)env->loader)->model60, 1);
            if ((u32)(frame - 0x4B) < 5) {
                fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x3F), frame, &pt);
                rot.vx = 0;
                rot.vy = 0;
                rot.vz = 0;
                pos.vx = 0;
                pos.vy = -0x5C3;
                pos.vz = 0x61A8;
                scale.vx = pt.vx;
                scale.vz = pt.vz;
                scale.vy = pt.vy = pt.vy / 3;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model8C);
            }
            if ((u32)(frame - 0x4F) < 0x10) {
                fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0xC), frame, &pt);
                pos2.vz = 0x61A8;
                rot2.vx = 0;
                rot2.vy = 0;
                rot2.vz = 0;
                pos2.vx = 0;
                pos2.vy = 0;
                scale2.vx = pt.vx;
                scale2.vy = pt.vy;
                scale2.vz = pt.vz;
                if (frame < 0x5F) {
                    bright = ex->math_cos((frame - 0x4F) << 6);
                } else {
                    bright = 0x1000;
                }
                ex->Hi_DrawEffModel(&rot2, (VECTOR *)&pos2, (s32)&scale2, work->model88);
                rgb = bright >> 5;
                rgb -= 0x80;
                ex->Hi_ModifyEffModelRGB(work->model88, rgb, rgb, rgb);
            }
            if (frame >= 0x4E) {
                wallFrame = frame - 0x4E;
                t = (wallFrame << 12) / 26;
                pt.vz = 0x6590 - (((0x1000 - ex->math_cos(t >> 2)) * 0x8278) >> 12);
                pt.vy = -0x200;
                scale3.vz = 0x2000;
                scale3.vx = 0x2000;
                pt.vx = 0;
                rot3.vz = 0;
                rot3.vy = 0;
                rot3.vx = 0;
                if (frame < 0x53) {
                    scale3.vy = (ex->math_sin(wallFrame * 0x5E8 / 5) >> 2) + 0x200;
                } else {
                    scale3.vy = ((ex->math_sin((((frame - 0x53) << 12) / 21) >> 2) * 3) >> 2) + 0x400;
                }
                fxm_drawWaveWall(&pt, &rot3, &scale3, 0x80, 0x17, 0x3D07, 0, 0x100, 0x400, 0x2BC, t * 3 / 2,
                              0x100, 0x400, t >> 1, 0x80);
                env->otZ = 0x80;
                for (i = 0; i < 20; i++) {
                    BTLFX_COPY_SVEC(&pt, &fx_0b3_crestPts[i]);
                    ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 0x26), &pt, NULL, ((i & 1) << 12) + 0x5000,
                                      frame + i * 5, 1, 0x1000);
                    pt.vy = -0x100;
                    ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 0x24), &pt, NULL, 0x7000, frame + i * 3, 1,
                                      0xC00);
                }
                env->otZ = 0;
            }
            fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x27), frame, &work->pos28);
            work->pos28.vy = -0x100;
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x31), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0b3_trails2;
                    emitter->trailSlots = 0x30;
                    emitter->trailStride = 0x1C;
                    emitter->otZ = 0x100;
                }
            }
            ex->btlfx_drawScrollMove(frame * 16, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
            ex->btlfx_drawScrollMove(frame * 12, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x68) {
                work->state0 = 1;
                ex->btlfx_initParticleSystem();
                return 1;
            }
        }
        work->frame4++;
    }
    return 0;
}

static s32 fxm_sinHalfNeg(s32 ang) {
    s32 sinv = BTLFX_EXPORTS(fx_0b3_env)->math_sin(ang);

    if (sinv < 0) {
        sinv >>= 1;
    }
    return sinv;
}

static void fxm_drawWaterSheet(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 bright, s32 brightShade, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 ay, s32 ph, s32 dph, s32 az, s32 phz, s32 dphz) {
    Unk800F78A4 *env = fx_0b3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR colFlat;
    CVECTOR colLit;
    CVECTOR colDark;
    CVECTOR colShadeFlat;
    CVECTOR colShadeLit;
    CVECTOR colShadeDark;
    MATRIX m;
    MATRIX worldMat;
    SVECTOR quad[4];
    VECTOR lv;
    DVECTOR sxy;
    s32 opz;
    s32 otz;
    POLY_FT4 *ft4;
    POLY_G4 *g4;
    POLY_F4 *f4;
    s32 xx;
    s32 zz;
    s32 j;
    s32 y0;
    s32 y1;
    POLY_GT4 *gt4;
    s32 i;
    s32 x;
    s32 tileU;
    s32 tileV;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 ft;
    s32 abr;
    s32 startX;
    s32 startY;

    abr = 1;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)env->prim;
    *(s32 *)&colDark = 0;
    *(s32 *)&colShadeDark = 0;
    ft4 = (POLY_FT4 *)(gt4 + 18);
    g4 = (POLY_G4 *)(ft4 + 324);
    f4 = (POLY_F4 *)(g4 + 18);
    env->prim = (u8 *)(f4 + 324);
    colFlat.r = colFlat.g = colFlat.b = bright;
    colLit.r = colLit.g = colLit.b = bright;
    colShadeFlat.r = colShadeFlat.g = colShadeFlat.b = brightShade;
    colShadeLit.r = colShadeLit.g = colShadeLit.b = brightShade;
    colFlat.cd = abr != 0xFF ? 0x2E : 0x2C;
    colDark.cd = abr != 0xFF ? 0x3E : 0x3C;
    colLit.cd = colDark.cd;
    colShadeFlat.cd = abr != 0xFF ? 0x2A : 0x28;
    colShadeDark.cd = abr != 0xFF ? 0x3A : 0x38;
    tpage |= (abr & 3) << 5;
    colShadeLit.cd = colShadeDark.cd;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    worldMat = m;
    worldMat.t[0] = pos->vx;
    worldMat.t[1] = pos->vy;
    worldMat.t[2] = pos->vz;
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    startX = -0x24EA;
    startY = -0x17CD;
    zz = 0;
    for (j = 0; j < 18; j++, zz += 0x2A5) {
        i = 0;
        ph += dph;
        y0 = startY + zz;
        y1 = y0 + 0x2A5;
        for (xx = 0; i < 19; i++, xx += 0x41A) {
            x = startX + xx;
            quad[0].vx = x + ((ex->math_sin(ph) * ay) >> 12);
            quad[1].vx = quad[0].vx + 0x41A;
            x += (ex->math_sin(ph + dph) * ay) >> 12;
            quad[0].vy = quad[1].vy = y0;
            quad[2].vy = quad[3].vy = y1;
            quad[2].vx = x;
            x += 0x41A;
            quad[3].vx = x;
            quad[0].vz = quad[2].vz = (fxm_sinHalfNeg(phz + i * dphz) * az) >> 12;
            quad[1].vz = quad[3].vz = (fxm_sinHalfNeg(phz + (i + 1) * dphz) * az) >> 12;
            gte_ldv3c(quad);
            gte_rtpt();
            tileU = i & 1;
            u0 = u + (tileU << 6);
            u1 = u0 + 0x40;
            if (tileU != 0) {
                u1 = u0 + 0x3F;
            }
            tileV = j & 1;
            v0 = v + (tileV << 6);
            v1 = v0 + 0x40;
            if (tileV != 0) {
                v1 = v0 + 0x3F;
            }
            gte_nclip();
            gte_stopz(&opz);
            if (opz != 0) {
                gte_avsz3();
                gte_stszotz(&otz);
                ft = 1;
                if (i == 0) {
                    *(s32 *)&gt4->r0 = *(s32 *)&gt4->r2 = *(s32 *)&colDark;
                    *(s32 *)&gt4->r1 = *(s32 *)&gt4->r3 = *(s32 *)&colLit;
                    *(s32 *)&g4->r0 = *(s32 *)&g4->r2 = *(s32 *)&colShadeDark;
                    ft = 0;
                    *(s32 *)&g4->r1 = *(s32 *)&g4->r3 = *(s32 *)&colShadeLit;
                } else {
                    *(s32 *)&ft4->r0 = *(s32 *)&colFlat;
                    *(s32 *)&f4->r0 = *(s32 *)&colShadeFlat;
                }
                if (opz > 0) {
                    if (ft) {
                        ft4->r0 >>= 1;
                        ft4->g0 >>= 1;
                    } else {
                        gt4->r0 >>= 1;
                        gt4->g0 >>= 1;
                        gt4->r1 >>= 1;
                        gt4->g1 >>= 1;
                        gt4->r2 >>= 1;
                        gt4->g2 >>= 1;
                        gt4->r3 >>= 1;
                        gt4->g3 >>= 1;
                    }
                }
                if (ft) {
                    ft4->u0 = ft4->u2 = u0;
                    ft4->u1 = ft4->u3 = u1;
                    ft4->v0 = ft4->v1 = v0;
                    ft4->v2 = ft4->v3 = v1;
                    ft4->tpage = tpage;
                    ft4->clut = clut;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                    *(s32 *)&f4->x0 = *(s32 *)&ft4->x0;
                    *(s32 *)&f4->x1 = *(s32 *)&ft4->x1;
                    *(s32 *)&f4->x2 = *(s32 *)&ft4->x2;
                } else {
                    gt4->u0 = gt4->u2 = u0;
                    gt4->u1 = gt4->u3 = u1;
                    gt4->v0 = gt4->v1 = v0;
                    gt4->v2 = gt4->v3 = v1;
                    gt4->tpage = tpage;
                    gt4->clut = clut;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                    *(s32 *)&g4->x0 = *(s32 *)&gt4->x0;
                    *(s32 *)&g4->x1 = *(s32 *)&gt4->x1;
                    *(s32 *)&g4->x2 = *(s32 *)&gt4->x2;
                    gte_SetRotMatrix(&worldMat);
                    gte_SetTransMatrix(&worldMat);
                    gte_ldv0(&quad[0]);
                    gte_rtv0tr();
                    gte_stlvnl(&lv);
                    fx_0b3_edgePts[j].vx = lv.vx;
                    fx_0b3_edgePts[j].vy = lv.vy;
                    fx_0b3_edgePts[j].vz = lv.vz;
                    gte_SetRotMatrix(&m);
                    gte_SetTransMatrix(&m);
                    gte_ldv0(&quad[0]);
                    gte_rtps();
                    gte_stsxy(&sxy);
                    if ((u16)(sxy.vx + 0x20) > 0x180 || sxy.vy < -0x20 || sxy.vy > 0x110) {
                        fx_0b3_edgePts[j].pad = 0;
                    } else {
                        fx_0b3_edgePts[j].pad = 1;
                    }
                }
                gte_ldv0(&quad[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, otz, ft4);
                    ex->btlfx_addPrimAbr(0x05000000, otz, f4, 2);
                    gte_stsxy(&ft4->x3);
                    *(s32 *)&f4->x3 = *(s32 *)&ft4->x3;
                    ft4++;
                    f4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, otz, gt4);
                    ex->btlfx_addPrimAbr(0x08000000, otz, g4, 2);
                    gte_stsxy(&gt4->x3);
                    *(s32 *)&g4->x3 = *(s32 *)&gt4->x3;
                    gt4++;
                    g4++;
                }
            }
        }
    }
}

static s32 fxm_nextRingPoint(SVECTOR *out) {
    s32 slot = fx_0b3_edgePtIdx;

    out->pad = 1;
    out->vx = fx_0b3_edgePts[slot].vx;
    out->vy = fx_0b3_edgePts[slot].vy;
    out->vz = fx_0b3_edgePts[slot].vz;
    slot++;
    if (slot >= 16) {
        slot = 0;
    }
    fx_0b3_edgePtIdx = slot;
    return 0;
}

s32 fx_0b3_op85(s32 mode, Fx0b3Work85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR keyPos;
    SVECTOR sheetPos;
    SVECTOR sheetRot;
    VECTOR sheetScale;
    SVECTOR p1;
    SVECTOR p2;
    VECTOR scale;
    Pool6CEntry *emitter;
    SVECTOR calmPos;
    SVECTOR calmRot;
    VECTOR calmScale;
    s32 frame;
    s32 t;
    s32 sheetT;
    s32 ampZ;
    s32 phaseMul;
    s32 bright;
    s32 fadeAng;
    s32 sinv;
    s32 ang;
    s32 sheetAng;
    s32 bright1;
    s32 o;
    s32 gridFrame;
    s32 bright0;
    s32 scaleAdd;
    s32 i;
    s32 j;

    if (mode == 0) {
        *args = sizeof(Fx0b3Work85);
    } else if (mode == 1) {
        work->rot40.vy = 0x800;
        work->state0 = 0;
        work->frame4 = 0;
        work->rot40.vx = 0;
        work->rot40.vz = 0;
        work->summonScale34.vx = 0x1000;
        work->summonScale34.vy = 0x1000;
        work->summonScale34.vz = 0x1000;
        work->summonPos28.vx = 0;
        work->summonPos28.vy = 0;
        work->summonPos28.vz = 0;
        fx_0b3_env = env;
        ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->loader)->summon54->motion18C, ((Fx0b3Summon *)env->loader)->model60);
        ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->loader)->model60, 0x5A);
        return 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            t = (frame << 12) / 81;
            if (frame == 0x12) {
                work->emit68 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x25), (s32)&work->pos10);
                if (work->emit68 != NULL) {
                    work->emit68->tail->shape = -1;
                    work->emit68->tail->s.module.spawnFn = (s32 (*)(SVECTOR *, SVECTOR *, s32, s32))fxm_nextRingPoint;
                    if (work->emit68 != NULL) {
                        work->emit68->otZ = 0x200;
                    }
                }
            }
            if (frame >= 9) {
                if (frame < 0x3D) {
                    sheetT = ((frame - 9) << 12) / 52;
                } else {
                    sheetT = 0x1000;
                }
                if (frame < 0x14) {
                    sheetPos.vx = 0;
                    phaseMul = 1;
                } else {
                    sheetPos.vx = 0x1000 - (t >> 1);
                    phaseMul = 2;
                }
                sinv = ex->math_sin(t >> 2);
                sheetAng = sheetT >> 2;
                sheetPos.vy = (s16)(((0x1000 - sinv) >> 2) - 0x400) - ((ex->math_cos(sheetAng) * 0x44C) >> 12);
                sheetPos.vz = ((0x1000 - ex->math_cos(sheetAng)) << 2) - 0x4000;
                sheetRot.vx = 0x400;
                sheetRot.vy = 0x400;
                sheetRot.vz = 0;
                sheetScale.vy = 0x1000;
                sheetScale.vx = 0x1000;
                sheetScale.vz = ex->math_cos(sheetAng);
                if (frame < 0x10) {
                    ampZ = ex->math_sin((frame << 10) >> 4) + 0x800;
                } else {
                    ampZ = 0x1800;
                }
                bright1 = 0xC8;
                if ((u32)(frame - 0x28) < 0x20) {
                    bright1 = 0xC8 - (((frame - 0x28) * 0x96) >> 5);
                } else if (frame >= 0x48) {
                    bright1 = 0x32;
                }
                if (frame >= 0x3D) {
                    bright0 = 0x80 - (((frame - 0x3D) << 5) / 20);
                } else {
                    bright0 = 0x80;
                }
                if (frame < 0x29) {
                    env->otZ = 0;
                } else {
                    env->otZ = -0x80;
                }
                fxm_drawWaterSheet(&sheetPos, &sheetRot, &sheetScale, bright0, bright1, 0x17, 0x3D07, 0, 0x80, 0x7D0, sheetT * phaseMul, -0x140, ampZ,
                              ex->math_cos(sheetT >> 2) + 0x400, 0x100);
                scaleAdd = (t * 3) >> 3;
                env->otZ = 0x7C;
                for (i = 0; i < 18; i++) {
                    if (fx_0b3_edgePts[i].pad != 0) {
                        bright0 = (i & 1) << 12; /* bright0 reused: a global pseudo, so bright0 + 0x3800 is not tied to it */
                        ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 0x24), &fx_0b3_edgePts[i], NULL,
                                                scaleAdd + (s16)(bright0 + 0x3800), frame + i * 2, 1, 0x1000);
                    }
                }
                env->otZ = 0x37;
                if (frame >= 0x1D) {
                    for (i = 0, j = frame; i < 7; j += 2, i++) {
                        if (fx_0b3_edgePts[i].pad != 0) {
                            ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 0x24), &fx_0b3_edgePts[i], NULL,
                                                    0x2400, j + 4, 1, 0x1000);
                        }
                    }
                }
                env->otZ = 0;
            }
            if (frame < 0x10) {
                fxm_keyGetVec((Fx0b3Track *)ex->btlfx_getFileData(entry->file, 0x16), frame, &keyPos);
                work->summonPos28.vx = keyPos.vx;
                work->summonPos28.vy = keyPos.vy;
                work->summonPos28.vz = keyPos.vz;
                ex->Hi_DrawSummonModel(&work->rot40, (VECTOR *)&work->summonPos28, (s32)&work->summonScale34,
                                       ((Fx0b3Summon *)env->loader)->model60, 1);
            }
            if (frame == 0x32) {
                p1.vx = env->camera->points[0].vx;
                p1.vy = env->camera->points[0].vy;
                p1.vz = env->camera->points[0].vz;
                p2.vx = env->camera->points[1].vx;
                p2.vy = env->camera->points[1].vy;
                p2.vz = env->camera->points[1].vz;
                work->pos10.vx = (p1.vx + p2.vx) / 2;
                work->pos10.vy = (p1.vy + p2.vy) / 2;
                work->pos10.vz = (p1.vz + p2.vz) / 2;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame >= 0x2B) {
                fadeAng = ((frame - 0x2B) << 10) / 38;
                ex->math_cos(fadeAng);
                ex->math_cos(fadeAng);
                ex->btlfx_drawRippleGrid((s32)(env->ot + 2), -0x20, 0x10, 3, 0x50, 1, &fx_0b3_rippleColor, 0x18, frame << 8, 0x186,
                                  0x100);
                if (frame == 0x2B) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 0, 0x20, 0xAA, 0xFF);
                }
                if (frame == 0x2D || frame == 0x32) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 0, 0, 0x64, 0xFF);
                }
            }
            if (frame >= 0x2B) {
                gridFrame = frame - 0x2B;
                p2.vx = 0;
                p2.vy = 0;
                p2.vz = 0;
                scale.vz = 0x1000;
                ((CVECTOR *)&p1)->r = ((CVECTOR *)&p1)->g = ((CVECTOR *)&p1)->b = -0x10 - (gridFrame * 0x8D) / 38;
                scale.vy = scale.vx = 0x1400 - (gridFrame << 10) / 38;
                ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &p2, &scale, (CVECTOR *)&p1, 0, NULL);
            }
            fxm_drawScrollMoveX(frame * 3, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x51) {
                work->state0 = 1;
                work->camPos18.vx = 0;
                work->camPos18.vy = -0x64;
                work->camPos18.vz = 0;
                env->camera->points[3].vx = work->camPos18.vx;
                env->camera->points[3].vy = work->camPos18.vy;
                env->camera->points[3].vz = work->camPos18.vz;
                ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&calmPos);
                if (calmPos.vy < -0x800) {
                    work->camPos18.vz = 0x300;
                    work->camPos18.vy -= 0x1F4;
                }
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 24;
            ang = t >> 2;
            work->camPos18.vy -= (ex->math_cos(ang) * 0x18) >> 12;
            env->camera->points[3].vx = work->camPos18.vx;
            env->camera->points[3].vy = work->camPos18.vy;
            env->camera->points[3].vz = work->camPos18.vz;
            calmPos.vx = 0;
            calmPos.vy = -0x200 - (ex->math_cos(ang) >> 3);
            calmRot.vx = 0x400;
            calmRot.vy = 0x400;
            calmPos.vz = 0;
            calmRot.vz = 0;
            calmScale.vy = 0x1000;
            calmScale.vx = 0x1000;
            calmScale.vz = 0;
            bright = ex->math_cos(ang) >> 5;
            fxm_drawWaterSheet(&calmPos, &calmRot, &calmScale, bright, bright, 0x17, 0x3D07, 0, 0x80, (ex->math_cos(ang) >> 3) + 0x300, ang, 0x100, 0, 0,
                          0);
            fxm_drawScrollMoveX(frame * 3, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x18) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_0b3_op86(s32 mode, Fx0b3Work86 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx0b3Work86);
    } else if (mode == 1) {
        fx_0b3_env = env;
        work->unitCount40 = env->targets->targetCount;
        for (i = 0; i < work->unitCount40; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->emitPos0[i]);
                work->emitPos0[i].vz -= ex->btlfx_getUnitRadius(i);
                work->emitPos0[i].vy -= 0x200;
                emitter = ex->btlfx_startEmitter((s32)ex->btlfx_getFileData(entry->file, 0x23), (s32)&work->emitPos0[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x200);
                }
            }
        }
    } else if (entry->frame >= 0x2E) {
        return 1;
    }
    return 0;
}

s32 fx_0b3_op87(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR camPt;
    s32 frame;
    s32 shot;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        shot = *args;
        fx_0b3_env = env;
        *work = shot;
        if (*work >= 10) {
            camPt.vx = env->targets->centerX;
            camPt.vy = env->targets->centerY;
            camPt.vz = env->targets->centerZ;
            camPt.vy -= 0x2BC;
            env->camera->points[2].vx = camPt.vx;
            env->camera->points[2].vy = camPt.vy;
            env->camera->points[2].vz = camPt.vz;
            return 1;
        }
    } else {
        frame = entry->frame;
        fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, fx_0b3_camShots[*work].point1Keys2), frame, &camPt);
        env->camera->points[1].vx = camPt.vx;
        env->camera->points[1].vy = camPt.vy;
        env->camera->points[1].vz = camPt.vz;
        fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, fx_0b3_camShots[*work].point0Keys0), frame, &camPt);
        env->camera->points[0].vx = camPt.vx;
        env->camera->points[0].vy = camPt.vy;
        env->camera->points[0].vz = camPt.vz;
        if (frame >= fx_0b3_camShots[*work].endFrame4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_headerPtr);
