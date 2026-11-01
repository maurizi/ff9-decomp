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

static void fxm_keyGetVec2(Fx197Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
    s16 *k;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    k = &track->keys4[t * 6];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
}

static void fxm_keyGetVec3(Fx197Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    s16 *k;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    k = &track->keys4[t * 9];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
    if (c != NULL) {
        c->vx = k[6];
        c->vy = k[7];
        c->vz = k[8];
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

static void fxm_shatterInit(s32 i, s32 n, Fx197Shatter *buf, Fx197Tri *tris) {
    Fx197Shatter *h;

    fx_197_shatters[i] = buf;
    fx_197_shards[i] = (Fx197Shard *)(buf + 1);
    h = fx_197_shatters[i];
    h->duration0 = n;
    h->count2 = 0;
    h->nextTriC = tris;
    h->tris8 = tris;
}

static s32 fxm_shatterAddModel(s32 i, s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_197_env);
    Fx197Shatter *h = fx_197_shatters[i];
    Fx197Tri *t = h->nextTriC;
    Fx197Shard *p;
    s32 n;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)t);
    h->modelTris10 = t;
    h->count4 = n;
    h->nextTriC = t + n;
    h->count2 += n;
    for (k = 0; k < n; k++, t++) {
        t->vert0.pad = 0;
    }
    p = fx_197_shards[i];
    for (k = 0; k < n; k++, p++) {
        p->frame18 = -1;
    }
    return h->count2;
}

static Fx197Shard *fxm_shatterFreeShard(s32 i) {
    Fx197Shatter *h = fx_197_shatters[i];
    Fx197Shard *p = fx_197_shards[i];
    s32 k;

    for (k = 0; k < h->count2; k++, p++) {
        if (p->frame18 < 0) {
            return p;
        }
    }
    return NULL;
}

static s32 fxm_shatterRelease(s32 i) {
    s32 idx = 0;
    s32 k = 0;
    Fx197Shatter *h = fx_197_shatters[i];
    Fx197Tri *t = h->tris8;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_197_env);
    s32 n = 0;
    s32 y = fx_197_shatterHeight;
    Fx197Shard *p;
    SVECTOR v;

    for (; k < h->count2; k++, t++, idx++) {
        if (y < t->vert8.vy && t->vert0.pad == 0) {
            p = fxm_shatterFreeShard(i);
            if (p != NULL) {
                t->vert0.pad = 1;
                p->pos0 = ((t->vert0.vx + t->vert8.vx + t->vert10.vx) / 3) << 12;
                p->pos4 = ((t->vert0.vy + t->vert8.vy + t->vert10.vy) / 3) << 12;
                p->pos8 = ((t->vert0.vz + t->vert8.vz + t->vert10.vz) / 3) << 12;
                p->frame18 = 0;
                p->index1A = idx;
                p->unk12 = 0x1000;
                p->spin14 = ex->btlfx_randCentered(0x100);
                p->spin16 = ex->btlfx_randCentered(0x200);
                v.vx = p->pos0 >> 12;
                v.vy = p->pos4 >> 12;
                v.vz = p->pos8 >> 12;
                p->velC = (v.vx >> 5) + ex->btlfx_randCentered(0x18);
                n++;
                p->velE = (v.vy >> 5) + (ex->btlfx_rand() & 0x1F);
                p->vel10 = (v.vz >> 5) + ex->btlfx_randCentered(0x18);
            }
        }
    }
    return n;
}

static void fxm_shatterUpdate(s32 i) {
    SVECTOR a;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR d0;
    SVECTOR d1;
    SVECTOR d2;
    SVECTOR zero;
    VECTOR out;
    VECTOR sv;
    MATRIX m;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_197_env);
    Fx197Shard *p;
    Fx197Shatter *h;
    Fx197Tri *t;
    s32 k;

    sv.vx = sv.vy = sv.vz = 0xF9C;
    p = fx_197_shards[i];
    h = fx_197_shatters[i];
    rot.vz = 0;
    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    for (k = 0; k < h->count2; k++, p++) {
        if (p->frame18 >= 0) {
            t = &h->tris8[p->index1A];
            c.vx = p->pos0 >> 12;
            c.vy = p->pos4 >> 12;
            c.vz = p->pos8 >> 12;
            d0.vx = t->vert0.vx - c.vx;
            d0.vy = t->vert0.vy - c.vy;
            d0.vz = t->vert0.vz - c.vz;
            d1.vx = t->vert8.vx - c.vx;
            d1.vy = t->vert8.vy - c.vy;
            d1.vz = t->vert8.vz - c.vz;
            d2.vx = t->vert10.vx - c.vx;
            d2.vy = t->vert10.vy - c.vy;
            d2.vz = t->vert10.vz - c.vz;
            ex->btlfx_stepModuleParticle((Pool6CEntry *)p, 0, &zero, &a);
            *(s32 *)&rot = *(s32 *)&p->spin14;
            ex->math_rotMatrixYXZ(&rot, &m);
            ex->ScaleMatrix(&m, &sv);
            m.t[0] = p->pos0 >> 12;
            m.t[1] = p->pos4 >> 12;
            m.t[2] = p->pos8 >> 12;
            ex->SetRotMatrix(&m);
            ex->SetTransMatrix(&m);
            gte_ldv0(&d0);
            gte_rtv0tr();
            gte_stlvnl(&out);
            gte_ldv0(&d1);
            gte_rtv0tr();
            t->vert0.vx = out.vx;
            t->vert0.vy = out.vy;
            t->vert0.vz = out.vz;
            gte_stlvnl(&out);
            gte_ldv0(&d2);
            gte_rtv0tr();
            t->vert8.vx = out.vx;
            t->vert8.vy = out.vy;
            t->vert8.vz = out.vz;
            gte_stlvnl(&out);
            t->vert10.vx = out.vx;
            t->vert10.vy = out.vy;
            t->vert10.vz = out.vz;
            p->frame18++;
            if (p->frame18 >= h->duration0) {
                p->frame18 = -1;
                t->vert10.vz = 0;
                *(s32 *)&t->vert10 = 0;
                t->vert8.vz = 0;
                *(s32 *)&t->vert8 = 0;
                t->vert0.vz = 0;
                *(s32 *)&t->vert0 = 0;
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
    SVECTOR p;
    SVECTOR unused; /* allocated but never referenced in the original */
    s32 m;
    s32 m2;
    s32 c;
    s32 t;
    s32 on;
    Pool6CEntry *emitter;

    if (mode == 1) {
        it->state0 = 0;
        it->unk8 = 0;
    } else if (it->index2 < 6) {
        switch (it->state0) {
        case 0:
            fxm_keyGetVec((Fx197Track *)ex->btlfx_getFileData(entry->file, fx_197_piecePathFileIds[it->index2]), frame, &p);
            p.vx += work->pos8.vx;
            p.vy += work->pos8.vy;
            p.vz += work->pos8.vz;
            pos.vx = p.vx;
            pos.vy = p.vy;
            pos.vz = p.vz;
            scale.vx = 0x2000;
            scale.vy = 0x2000;
            scale.vz = 0x3000;
            rot.vx = -0x200;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, fx_197_model0);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, fx_197_model1);
            if (frame >= 3) {
                it->state0 = 1;
                ((s16 *)it)[-1] = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x20, 0x46);
                it->posC.vx = fx_197_pieceOffsets[it->index2].vx;
                it->posC.vy = fx_197_pieceOffsets[it->index2].vy;
                it->posC.vz = fx_197_pieceOffsets[it->index2].vz;
                it->posC.vx += work->pos8.vx;
                it->posC.vy += work->pos8.vy;
                it->posC.vz += work->pos8.vz;
                it->posC.vy = 0;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&it->posC);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                break;
            }
            return 0;
        case 1:
            if (frame + (it->index2 - 5) <= 0) {
                m = work->model18[it->index2][0];
                m2 = -1;
            } else {
                if (frame < 0x10) {
                    m = work->model18[it->index2][fx_197_pieceModelFrames[frame]];
                } else {
                    m = work->model18[it->index2][2];
                }
                if (frame >= 0x10) {
                    m2 = work->model6C[it->index2];
                } else {
                    m2 = -1;
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
            if (m2 >= 0 && fx_197_overlayOn != 0) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, m2);
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, m);
            if (it->index2 < 5) {
                on = fx_197_shatterFlag0;
            } else {
                on = fx_197_shatterFlag2;
            }
            if (on != 0) {
                it->state0 = 2;
                ((s16 *)it)[-1] = -1;
                fxm_shatterInit(it->index2, 0x14, (Fx197Shatter *)(env->workBuf + fx_197_shatterBufs[it->index2].shatterOfs0),
                              (Fx197Tri *)(env->workBuf + fx_197_shatterBufs[it->index2].trisOfs4));
                fxm_shatterAddModel(it->index2, work->model18[it->index2][2]);
                return 0;
            }
            break;
        case 2:
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model18[it->index2][2]);
            if (frame >= 0xE) {
                t = 0x1000 - ((frame - 0xE) << 12) / 6;
                ex->Hi_ModifyEffModelAbr(work->model18[it->index2][2], 1);
                c = (t / 2 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model18[it->index2][2], c, c, c);
            } else {
                ex->Hi_ModifyEffModelAbr(work->model18[it->index2][2], 1);
                c = -0x40;
                ex->Hi_ModifyEffModelRGB(work->model18[it->index2][2], c, c, c);
            }
            fxm_shatterRelease(it->index2);
            fxm_shatterUpdate(it->index2);
            if (frame < 0x14) {
                break;
            }
            return 1;
        default:
            return 0;
        }
    } else {
        switch (it->state0) {
        case 0:
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model6C[6]);
            if (frame < 8) {
                s32 c0 = ((frame << 12) >> 9) - 0x80;

                ex->Hi_ModifyEffModelRGB(work->model6C[6], c0, c0, c0);
            }
            if (fx_197_shatterFlag1 != 0) {
                it->state0 = 1;
                ((s16 *)it)[-1] = -1;
                fxm_shatterInit(it->index2, 0x57, &fx_197_shatterBuf6, fx_197_shatterTris6);
                fxm_shatterAddModel(it->index2, work->model6C[6]);
                return 0;
            }
            break;
        case 1:
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model6C[6]);
            if (frame >= 0x37) {
                s32 c1 = ((0x1000 - (((frame - 0x37) << 12) >> 5)) >> 5) - 0x80;

                ex->Hi_ModifyEffModelRGB(work->model6C[6], c1, c1, c1);
            }
            fxm_shatterRelease(6);
            fxm_shatterUpdate(6);
            if (frame >= 0x57) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_197_op81(s32 mode, Fx197Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx197Item81 *it;
    Fx197Item81 *it2;
    Pool6CEntry *emitter;
    s32 frame;
    s32 state;
    s32 i;
    s32 j;
    s32 c;

    if (mode == 0) {
        init->workSize = 0x88;
        init->elemSize = 0x14;
        init->elemFn = (s32)fx_197_op81PoolTask;
        init->poolCount = 7;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_197_env = env;
        ex->btlfx_initEffModels(fx_197_effModels, 0x20);
        fx_197_model1 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD8), 0);
        fx_197_model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD9), 1);
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 3; j++) {
                work->model18[i][j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_197_pieceModelFileIds[i][j]), 0);
            }
        }
        for (i = 0; i < 7; i++) {
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
        case 0:
            if (frame < 10 && (frame & 1)) {
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    it->index2 = fx_197_spawnPieces[frame >> 1];
                }
            }
            if (frame == 5) {
                it2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it2 != NULL) {
                    it2->index2 = 6;
                }
            }
            if (frame == 4) {
                it2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it2 != NULL) {
                    it2->index2 = 5;
                }
            }
            if (frame >= 16) {
                work->state0 = 1;
                entry->frame = -1;
                fx_197_shatterHeight = 0x400;
            }
            break;
        case 1:
            if (frame == 20) {
                fx_197_shatterFlag0 = 1;
                fx_197_shatterFlag1 = 1;
            } else {
                fx_197_shatterHeight = 0x400 - (frame << 7);
            }
            if (frame == 24) {
                fx_197_shatterFlag2 = 1;
            }
            if (frame == 20) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_197_op81Trails;
                    emitter->trailSlots = 0xC;
                    emitter->trailStride = 0x4C;
                    emitter->otZ = 0x40;
                }
            }
            if (frame == 18) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == 32) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == 44) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == 19) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0x80);
            }
            if (frame == 20) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x80, 0xFF);
                fx_197_overlayOn = 0;
            }
            if (frame >= 20 && frame < 36) {
                c = 0x80 - (((frame - 20) << 7) >> 4);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, c >> 2, c);
            }
            ex->btlfx_drawScrollMove(frame * 2, 0x2C0, 0x180, 0x40, 0x80, 0x2C0, 0x100);
            if (frame >= 126) {
                return 1;
            }
            break;
        }
        ex->btlfx_drawScrollMove(frame, 0x280, 0x100, 0x40, 0x80, 0x280, 0x180);
        work->frame4++;
    }
    return 0;
}

s32 fx_197_op87(s32 mode, Fx197Work87 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x14;
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
