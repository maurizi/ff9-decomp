#include "fxmod/fx_197.h"

static void fxm_keyGetVec(Fx197Track *track, s32 t, SVECTOR *out) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    track = (Fx197Track *)&track->keys4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

static void fxm_keyGetVec2(Fx197Track *track, s32 t, SVECTOR *out0, SVECTOR *out1) {
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

static void fxm_keyGetVec3(Fx197Track *track, s32 t, SVECTOR *out0, SVECTOR *out1, SVECTOR *out2) {
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

static s32 fxm_keyGetValue(Fx197Track *track, s32 t) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    return (track->keys4 + t)[0];
}

static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 wrapW = x % w;
    Unk800F78A4 *env = fx_197_env;
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

static void fxm_shatterInit(s32 slot, s32 life, Fx197Shatter *buf, Fx197Tri *tris) {
    Fx197Shatter *set;

    fx_197_shatters[slot] = buf;
    fx_197_shards[slot] = (Fx197Shard *)(buf + 1);
    set = fx_197_shatters[slot];
    set->duration0 = life;
    set->count2 = 0;
    set->nextTriC = tris;
    set->tris8 = tris;
}

static s32 fxm_shatterAddModel(s32 slot, s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_197_env);
    Fx197Shatter *set = fx_197_shatters[slot];
    Fx197Tri *tri = set->nextTriC;
    Fx197Shard *shard;
    s32 n;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)tri);
    set->modelTris10 = tri;
    set->count4 = n;
    set->nextTriC = tri + n;
    set->count2 += n;
    for (k = 0; k < n; k++, tri++) {
        tri->vert0.pad = 0;
    }
    shard = fx_197_shards[slot];
    for (k = 0; k < n; k++, shard++) {
        shard->frame18 = -1;
    }
    return set->count2;
}

static Fx197Shard *fxm_shatterFreeShard(s32 slot) {
    Fx197Shatter *set = fx_197_shatters[slot];
    Fx197Shard *shard = fx_197_shards[slot];
    s32 k;

    for (k = 0; k < set->count2; k++, shard++) {
        if (shard->frame18 < 0) {
            return shard;
        }
    }
    return NULL;
}

static s32 fxm_shatterRelease(s32 slot) {
    s32 idx = 0;
    s32 k = 0;
    Fx197Shatter *set = fx_197_shatters[slot];
    Fx197Tri *tri = set->tris8;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_197_env);
    s32 n = 0;
    s32 lineY = fx_197_shatterHeight;
    Fx197Shard *shard;
    SVECTOR center;

    for (; k < set->count2; k++, tri++, idx++) {
        if (lineY < tri->vert8.vy && tri->vert0.pad == 0) {
            shard = fxm_shatterFreeShard(slot);
            if (shard != NULL) {
                tri->vert0.pad = 1;
                shard->pos0 = ((tri->vert0.vx + tri->vert8.vx + tri->vert10.vx) / 3) << 12;
                shard->pos4 = ((tri->vert0.vy + tri->vert8.vy + tri->vert10.vy) / 3) << 12;
                shard->pos8 = ((tri->vert0.vz + tri->vert8.vz + tri->vert10.vz) / 3) << 12;
                shard->frame18 = 0;
                shard->index1A = idx;
                shard->unk12 = 0x1000;
                shard->spin14 = ex->btlfx_randCentered(0x100);
                shard->spin16 = ex->btlfx_randCentered(0x200);
                center.vx = shard->pos0 >> 12;
                center.vy = shard->pos4 >> 12;
                center.vz = shard->pos8 >> 12;
                shard->velC = (center.vx >> 5) + ex->btlfx_randCentered(0x18);
                n++;
                shard->velE = (center.vy >> 5) + (ex->btlfx_rand() & 0x1F);
                shard->vel10 = (center.vz >> 5) + ex->btlfx_randCentered(0x18);
            }
        }
    }
    return n;
}

static void fxm_shatterUpdate(s32 slot) {
    SVECTOR moved;
    SVECTOR rot;
    SVECTOR center;
    SVECTOR rel0;
    SVECTOR rel1;
    SVECTOR rel2;
    SVECTOR zero;
    VECTOR out;
    VECTOR sv;
    MATRIX m;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_197_env);
    Fx197Shard *shard;
    Fx197Shatter *set;
    Fx197Tri *tri;
    s32 k;

    sv.vx = sv.vy = sv.vz = 0xF9C;
    shard = fx_197_shards[slot];
    set = fx_197_shatters[slot];
    rot.vz = 0;
    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    for (k = 0; k < set->count2; k++, shard++) {
        if (shard->frame18 >= 0) {
            tri = &set->tris8[shard->index1A];
            center.vx = shard->pos0 >> 12;
            center.vy = shard->pos4 >> 12;
            center.vz = shard->pos8 >> 12;
            rel0.vx = tri->vert0.vx - center.vx;
            rel0.vy = tri->vert0.vy - center.vy;
            rel0.vz = tri->vert0.vz - center.vz;
            rel1.vx = tri->vert8.vx - center.vx;
            rel1.vy = tri->vert8.vy - center.vy;
            rel1.vz = tri->vert8.vz - center.vz;
            rel2.vx = tri->vert10.vx - center.vx;
            rel2.vy = tri->vert10.vy - center.vy;
            rel2.vz = tri->vert10.vz - center.vz;
            ex->btlfx_stepModuleParticle((Pool6CEntry *)shard, 0, &zero, &moved);
            *(s32 *)&rot = *(s32 *)&shard->spin14;
            ex->math_rotMatrixYXZ(&rot, &m);
            ex->ScaleMatrix(&m, &sv);
            m.t[0] = shard->pos0 >> 12;
            m.t[1] = shard->pos4 >> 12;
            m.t[2] = shard->pos8 >> 12;
            ex->SetRotMatrix(&m);
            ex->SetTransMatrix(&m);
            gte_ldv0(&rel0);
            gte_rtv0tr();
            gte_stlvnl(&out);
            gte_ldv0(&rel1);
            gte_rtv0tr();
            tri->vert0.vx = out.vx;
            tri->vert0.vy = out.vy;
            tri->vert0.vz = out.vz;
            gte_stlvnl(&out);
            gte_ldv0(&rel2);
            gte_rtv0tr();
            tri->vert8.vx = out.vx;
            tri->vert8.vy = out.vy;
            tri->vert8.vz = out.vz;
            gte_stlvnl(&out);
            tri->vert10.vx = out.vx;
            tri->vert10.vy = out.vy;
            tri->vert10.vz = out.vz;
            shard->frame18++;
            if (shard->frame18 >= set->duration0) {
                shard->frame18 = -1;
                tri->vert10.vz = 0;
                *(s32 *)&tri->vert10 = 0;
                tri->vert8.vz = 0;
                *(s32 *)&tri->vert8 = 0;
                tri->vert0.vz = 0;
                *(s32 *)&tri->vert0 = 0;
            }
        }
    }
}

static s32 fx_197_op81PoolTask(s32 mode, Fx197Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx197Work81 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR pathPos;
    SVECTOR unused; /* allocated but never referenced in the original */
    s32 model;
    s32 overlayModel;
    s32 rgb;
    s32 t;
    s32 on;
    Pool6CEntry *emitter;

    if (mode == 1) {
        it->state0 = FX_SHIVA_PIECE_STATE_FLY;
        it->unk8 = 0;
    } else if (it->index2 < FX_SHIVA_PIECE_COUNT) {
        switch (it->state0) {
        case FX_SHIVA_PIECE_STATE_FLY:
            fxm_keyGetVec((Fx197Track *)ex->btlfx_getFileData(entry->file, fx_197_piecePathFileIds[it->index2]), frame, &pathPos);
            pathPos.vx += work->pos8.vx;
            pathPos.vy += work->pos8.vy;
            pathPos.vz += work->pos8.vz;
            pos.vx = pathPos.vx;
            pos.vy = pathPos.vy;
            pos.vz = pathPos.vz;
            scale.vx = 0x2000;
            scale.vy = 0x2000;
            scale.vz = 0x3000;
            rot.vx = -0x200;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, fx_197_model0);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, fx_197_model1);
            if (frame >= FX_SHIVA_PIECE_FLY_FRAMES) {
                it->state0 = FX_SHIVA_PIECE_STATE_STAND;
                ((s16 *)it)[-1] = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_SHIVA_PIECE_TINT_RGB);
                it->posC.vx = fx_197_pieceOffsets[it->index2].vx;
                it->posC.vy = fx_197_pieceOffsets[it->index2].vy;
                it->posC.vz = fx_197_pieceOffsets[it->index2].vz;
                it->posC.vx += work->pos8.vx;
                it->posC.vy += work->pos8.vy;
                it->posC.vz += work->pos8.vz;
                it->posC.vy = 0;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIVA_SHATTER_RINGS), (s32)&it->posC);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                break;
            }
            return 0;
        case FX_SHIVA_PIECE_STATE_STAND:
            if (frame + (it->index2 - 5) <= 0) {
                model = work->model18[it->index2][0];
                overlayModel = -1;
            } else {
                if (frame < FX_SHIVA_PIECE_FORM_FRAMES) {
                    model = work->model18[it->index2][fx_197_pieceModelFrames[frame]];
                } else {
                    model = work->model18[it->index2][FX_SHIVA_PIECE_STAGES - 1];
                }
                if (frame >= FX_SHIVA_PIECE_FORM_FRAMES) {
                    overlayModel = work->model6C[it->index2];
                } else {
                    overlayModel = -1;
                }
            }
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            if (overlayModel >= 0 && fx_197_overlayOn != 0) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, overlayModel);
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, model);
            if (it->index2 < FX_SHIVA_PIECE_COUNT - 1) {
                on = fx_197_shatterFlag0;
            } else {
                on = fx_197_shatterFlag2;
            }
            if (on != 0) {
                it->state0 = FX_SHIVA_PIECE_STATE_SHATTER;
                ((s16 *)it)[-1] = -1;
                fxm_shatterInit(it->index2, FX_SHIVA_PIECE_SHARD_LIFE, (Fx197Shatter *)(env->workBuf + fx_197_shatterBufs[it->index2].shatterOfs0),
                              (Fx197Tri *)(env->workBuf + fx_197_shatterBufs[it->index2].trisOfs4));
                fxm_shatterAddModel(it->index2, work->model18[it->index2][FX_SHIVA_PIECE_STAGES - 1]);
                return 0;
            }
            break;
        case FX_SHIVA_PIECE_STATE_SHATTER:
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model18[it->index2][FX_SHIVA_PIECE_STAGES - 1]);
            if (frame >= FX_SHIVA_PIECE_FADE_FRAME) {
                t = 0x1000 - ((frame - FX_SHIVA_PIECE_FADE_FRAME) << 12) / FX_SHIVA_PIECE_FADE_FRAMES;
                ex->Hi_ModifyEffModelAbr(work->model18[it->index2][FX_SHIVA_PIECE_STAGES - 1], BTLFX_ABR_ADD);
                rgb = (t / 2 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model18[it->index2][FX_SHIVA_PIECE_STAGES - 1], rgb, rgb, rgb);
            } else {
                ex->Hi_ModifyEffModelAbr(work->model18[it->index2][FX_SHIVA_PIECE_STAGES - 1], BTLFX_ABR_ADD);
                rgb = -0x40;
                ex->Hi_ModifyEffModelRGB(work->model18[it->index2][FX_SHIVA_PIECE_STAGES - 1], rgb, rgb, rgb);
            }
            fxm_shatterRelease(it->index2);
            fxm_shatterUpdate(it->index2);
            if (frame < FX_SHIVA_PIECE_FADE_FRAME + FX_SHIVA_PIECE_FADE_FRAMES) {
                break;
            }
            return 1;
        default:
            return 0;
        }
    } else {
        switch (it->state0) {
        case FX_SHIVA_CORE_STATE_STAND:
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model6C[FX_SHIVA_PIECE_CORE]);
            if (frame < 8) {
                s32 fadeIn = ((frame << 12) >> 9) - 0x80;

                ex->Hi_ModifyEffModelRGB(work->model6C[FX_SHIVA_PIECE_CORE], fadeIn, fadeIn, fadeIn);
            }
            if (fx_197_shatterFlag1 != 0) {
                it->state0 = FX_SHIVA_CORE_STATE_SHATTER;
                ((s16 *)it)[-1] = -1;
                fxm_shatterInit(it->index2, FX_SHIVA_CORE_SHARD_LIFE, &fx_197_shatterBuf6, fx_197_shatterTris6);
                fxm_shatterAddModel(it->index2, work->model6C[FX_SHIVA_PIECE_CORE]);
                return 0;
            }
            break;
        case FX_SHIVA_CORE_STATE_SHATTER:
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model6C[FX_SHIVA_PIECE_CORE]);
            if (frame >= FX_SHIVA_CORE_FADE_FRAME) {
                s32 fadeOut = ((0x1000 - (((frame - FX_SHIVA_CORE_FADE_FRAME) << 12) >> 5)) >> 5) - 0x80;

                ex->Hi_ModifyEffModelRGB(work->model6C[6], fadeOut, fadeOut, fadeOut);
            }
            fxm_shatterRelease(FX_SHIVA_PIECE_CORE);
            fxm_shatterUpdate(FX_SHIVA_PIECE_CORE);
            if (frame >= FX_SHIVA_CORE_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_197_op81(s32 mode, Fx197Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx197Item81 *piece;
    Fx197Item81 *slab;
    Pool6CEntry *emitter;
    s32 frame;
    s32 state;
    s32 i;
    s32 j;
    s32 tint;

    if (mode == 0) {
        init->workSize = sizeof(Fx197Work81);
        init->elemSize = sizeof(Fx197Item81);
        init->elemFn = (s32)fx_197_op81PoolTask;
        init->poolCount = FX_SHIVA_BARRIER_POOL_COUNT;
    } else if (mode == 1) {
        work->state0 = FX_SHIVA_BARRIER_STATE_BUILD;
        work->frame4 = 0;
        fx_197_env = env;
        ex->btlfx_initEffModels(fx_197_effModels, FX_SHIVA_MODELS);
        fx_197_model1 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_SHIVA_ICICLE), 0);
        fx_197_model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_SHIVA_ICICLE_2), 1);
        for (i = 0; i < FX_SHIVA_PIECE_COUNT; i++) {
            for (j = 0; j < FX_SHIVA_PIECE_STAGES; j++) {
                work->model18[i][j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_197_pieceModelFileIds[i][j]), 0);
            }
        }
        for (i = 0; i < FX_SHIVA_PIECE_COUNT + 1; i++) {
            work->model6C[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_197_modelFileIds[i]), 1);
        }
        work->pos8.vx = fx_197_basePos.vx;
        work->pos8.vy = fx_197_basePos.vy;
        work->pos8.vz = fx_197_basePos.vz;
        work->pos10.vx = fx_197_basePos.vx;
        work->pos10.vy = fx_197_basePos.vy;
        work->pos10.vz = fx_197_basePos.vz;
    } else {
        state = work->state0;
        frame = entry->frame;
        switch (state) {
        case FX_SHIVA_BARRIER_STATE_BUILD:
            if (frame < 10 && (frame & 1)) {
                piece = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (piece != NULL) {
                    piece->index2 = fx_197_spawnPieces[frame >> 1];
                }
            }
            if (frame == 5) {
                slab = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (slab != NULL) {
                    slab->index2 = FX_SHIVA_PIECE_CORE;
                }
            }
            if (frame == 4) {
                slab = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (slab != NULL) {
                    slab->index2 = FX_SHIVA_PIECE_COUNT - 1;
                }
            }
            if (frame >= FX_SHIVA_BUILD_FRAMES) {
                work->state0 = FX_SHIVA_BARRIER_STATE_SHATTER;
                entry->frame = -1;
                fx_197_shatterHeight = FX_SHIVA_SHATTER_LINE_Y;
            }
            break;
        case FX_SHIVA_BARRIER_STATE_SHATTER:
            if (frame == FX_SHIVA_SHATTER_FRAME) {
                fx_197_shatterFlag0 = 1;
                fx_197_shatterFlag1 = 1;
            } else {
                fx_197_shatterHeight = FX_SHIVA_SHATTER_LINE_Y - (frame << 7);
            }
            if (frame == FX_SHIVA_SHATTER_5_FRAME) {
                fx_197_shatterFlag2 = 1;
            }
            if (frame == FX_SHIVA_SHATTER_FRAME) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIVA_ICE_DEBRIS_2), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIVA_ICE_DEBRIS), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIVA_BLUE_TRAIL), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_197_op81Trails;
                    emitter->trailSlots = FX_SHIVA_BLUE_TRAIL_TRAIL_SLOTS;
                    emitter->trailStride = FX_SHIVA_BLUE_TRAIL_TRAIL_STRIDE;
                    emitter->otZ = 0x40;
                }
            }
            if (frame == FX_SHIVA_CYAN_RING_FRAME) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIVA_CYAN_RING), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == FX_SHIVA_STAR_GLINTS_FRAME) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIVA_STAR_GLINTS), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == FX_SHIVA_ICE_CHUNKS_FRAME) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIVA_ICE_CHUNKS), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == FX_SHIVA_DARKEN_FRAME) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, FX_SHIVA_DARKEN_RGB);
            }
            if (frame == FX_SHIVA_SHATTER_FRAME) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_SHIVA_FLASH_RGB);
                fx_197_overlayOn = 0;
            }
            if (frame >= FX_SHIVA_SHATTER_FRAME && frame < FX_SHIVA_SHATTER_FRAME + FX_SHIVA_FLASH_FADE_FRAMES) {
                tint = FX_SHIVA_FLASH_LEVEL - (((frame - FX_SHIVA_SHATTER_FRAME) << 7) >> 4);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, 0, tint >> 2, tint);
            }
            ex->btlfx_drawScrollMove(frame * 2, FX_SHIVA_BARRIER_VRAM_X, FX_SHIVA_BARRIER_VRAM_Y, FX_SHIVA_BARRIER_VRAM_W, FX_SHIVA_BARRIER_VRAM_H, FX_SHIVA_BARRIER_SRC_X, FX_SHIVA_BARRIER_SRC_Y);
            if (frame >= FX_SHIVA_SHATTER_OP_FRAMES) {
                return 1;
            }
            break;
        }
        ex->btlfx_drawScrollMove(frame, FX_SHIVA_ICICLE_2_VRAM_X, FX_SHIVA_ICICLE_2_VRAM_Y, FX_SHIVA_ICICLE_2_VRAM_W, FX_SHIVA_ICICLE_2_VRAM_H, FX_SHIVA_ICICLE_2_SRC_X, FX_SHIVA_ICICLE_2_SRC_Y);
        work->frame4++;
    }
    return 0;
}

s32 fx_197_op87(s32 mode, Fx197Work87 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx197Work87);
    } else if (mode == 1) {
        work->index0 = *args - 3;
        fx_197_env = env;
    } else {
        frame = entry->frame;
        fxm_keyGetVec((Fx197Track *)ex->btlfx_getFileData(entry->file, fx_197_camPaths[work->index0].fileId2), frame,
                      &work->camPos4);
        env->camera->points[1].vx = work->camPos4.vx;
        env->camera->points[1].vy = work->camPos4.vy;
        env->camera->points[1].vz = work->camPos4.vz;
        fxm_keyGetVec((Fx197Track *)ex->btlfx_getFileData(entry->file, fx_197_camPaths[work->index0].fileId0), frame,
                      &work->camPosC);
        env->camera->points[0].vx = work->camPosC.vx;
        env->camera->points[0].vy = work->camPosC.vy;
        env->camera->points[0].vz = work->camPosC.vz;
        if (frame >= fx_197_camPaths[work->index0].duration4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_197/nonmatchings/fx_197", fx_197_headerPtr);
