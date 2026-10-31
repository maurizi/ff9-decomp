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

static void fxm_keyGetVec2(Fx0b3Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
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

static void fxm_keyGetVec3(Fx0b3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
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

static s32 fxm_keyGetValue(Fx0b3Track *track, s32 t) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    return (track->keys4 + t)[0];
}

static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = fx_0b3_env;
    u32 *p = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->prim = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->ot + OT_LAST, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->ot + OT_LAST, p);
    }
}

s32 fx_0b3_op80(s32 mode, Fx0b3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR ang;
    s32 frame;
    s32 c;
    s32 i;
    s32 n;
    s32 t;
    s32 u;
    s32 x;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work80);
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
            c = t / 30;
            if (frame < 20) {
                c = t / 20;
            } else {
                c = 0x1000;
            }
            ex->Hi_DrawEffModel(&work->rot60, (VECTOR *)&work->modelPos48, (s32)&work->modelScale54, work->model6C);
            ex->Hi_ModifyEffModelRGB(work->model6C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
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
                fxm_keyGetVec((Fx0b3Track *)ex->btlfx_getFileData(entry->file, 0xB), frame, &v);
                work->morphPos28.vx = v.vx;
                work->morphPos28.vy = v.vy;
                work->morphPos28.vz = v.vz;
                BTLFX_COPY_SVEC(&work->pos10, &v);
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
                n = work->morphFrame70 + 1;
                if (n >= 6) {
                    n = 0;
                }
                ex->Hi_DrawMorphEffModel(&work->rot40, (VECTOR *)&work->morphPos28, (s32)&work->morphScale34,
                                         fx_0b3_morphModels[work->morphFrame70], fx_0b3_morphModels[n], work->morphWeight74);
                if (frame < 8) {
                    x = ((frame << 12) >> 8) - 0x80;
                    ex->Hi_ModifyEffModelRGB(fx_0b3_morphModels[work->morphFrame70], x, x, x);
                }
            }
            if (frame >= 0x33) {
                u = (frame - 0x33) * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, u, u, u);
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

s32 fx_0b3_op81(s32 mode, Fx0b3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    SVECTOR d;
    SVECTOR c;
    SVECTOR rot;
    Fx0b3Vec3 scale;
    Fx0b3Vec3 pos;
    SVECTOR sc;
    Pool6CEntry *e;
    s32 frame;
    s32 i;
    s32 n;
    s32 t;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work81);
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0b3_env = env;
        fx_0b3_sharedModel = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x89), 1);
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
        ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->loader)->unk54->motion190, ((Fx0b3Summon *)env->loader)->model60);
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
            ex->Hi_DrawEffModel(&work->rot78, (VECTOR *)&work->modelPos60, (s32)&work->modelScale6C, fx_0b3_sharedModel);
            if (frame >= 0x2B) {
                fxm_keyGetVec3((u8 *)ex->btlfx_getFileData(entry->file, 3), frame, &c, &work->rot58, &d);
                work->pos40.vx = c.vx;
                work->pos40.vy = c.vy;
                work->pos40.vz = c.vz;
                work->scale4C.vx = d.vx;
                work->scale4C.vy = d.vy;
                work->scale4C.vz = d.vz;
                ex->Hi_DrawSummonModel(&work->rot58, (VECTOR *)&work->pos40, (s32)&work->scale4C,
                                       ((Fx0b3Summon *)env->loader)->model60, 1);
            }
            if (frame < 8) {
                t = 0xFF - frame * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, t, t, t);
            }
            if (frame == 0) {
                work->morphWeight9C = 0x1000;
                work->morphFrame98 = 0;
            }
            fxm_keyGetVec2((u8 *)ex->btlfx_getFileData(entry->file, 0x20), frame, &a, &b);
            work->pos40.vx = a.vx;
            work->pos40.vy = a.vy;
            work->pos40.vz = a.vz;
            BTLFX_COPY_SVEC(&work->pos10, &a);
            work->scale4C.vx = b.vx;
            work->scale4C.vy = b.vy;
            work->scale4C.vz = b.vz;
            work->morphWeight9C -= 0x800;
            if (work->morphWeight9C <= 0) {
                work->morphWeight9C = 0x1000;
                if (++work->morphFrame98 >= 6) {
                    work->morphFrame98 = 0;
                }
            }
            n = work->morphFrame98 + 1;
            if (n >= 6) {
                n = 0;
            }
            ex->Hi_DrawMorphEffModel(&work->rot58, (VECTOR *)&work->pos40, (s32)&work->scale4C, fx_0b3_morphModels[work->morphFrame98],
                                     fx_0b3_morphModels[n], work->morphWeight9C);
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
                        fxm_keyGetVec2((u8 *)ex->btlfx_getFileData(entry->file, fx_0b3_partKeyBlocks[i]), frame, &a, &sc);
                        pos.vx = a.vx;
                        pos.vy = a.vy;
                        pos.vz = a.vz;
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
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos28);
                if (e != NULL) {
                    e->otZ = 0x80;
                }
            }
            if (frame == 0x26) {
                work->pos30.vy = -0x3000;
                work->pos30.vx = 0;
                work->pos30.vz = 0x483;
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos30);
                if (e != NULL) {
                    e->otZ = 0x200;
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

s32 fx_0b3_op83(s32 mode, Fx0b3Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    Fx0b3Vec3 scale;
    Fx0b3Vec3 pos;
    SVECTOR rot;
    SVECTOR sc;
    Fx0b3Vec3 pos2;
    Fx0b3Vec3 scale2;
    SVECTOR rot2;
    SVECTOR b;
    Pool6CEntry *e;
    s32 frame;
    s32 c;
    s32 w;
    s32 k;
    s32 i;
    s32 t;
    s32 c2;
    s32 x;
    s32 y;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work83);
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
            ex->Hi_DrawEffModel(&work->rot70, (VECTOR *)&work->modelPos58, (s32)&work->modelScale64, fx_0b3_sharedModel);
            if (frame == 0) {
                ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->loader)->unk54->motion184, ((Fx0b3Summon *)env->loader)->model60);
                ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->loader)->model60, 6);
            } else if (frame == 0x42) {
                ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->loader)->unk54->motion188, ((Fx0b3Summon *)env->loader)->model60);
                ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->loader)->model60, 0);
            }
            fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x13), frame, &a);
            work->summonPos38.vx = a.vx;
            work->summonPos38.vy = a.vy;
            work->summonPos38.vz = a.vz;
            ex->Hi_DrawSummonModel(&work->rot50, (VECTOR *)&work->summonPos38, (s32)&work->summonScale44,
                                   ((Fx0b3Summon *)env->loader)->model60, 1);
            if ((u32)(frame - 0xF) < 0x10) {
                t = ex->math_cos(((frame - 0xF) << 10) >> 4);
                ex->Hi_ModifySummonModelRGB(((Fx0b3Summon *)env->loader)->model60, t >> 6, t >> 5, t >> 5);
            }
            if (frame < 0x28) {
                if (frame >= 0x1D) {
                    c = 0x1000 - ((frame - 0x1D) << 12) / 10;
                } else {
                    c = 0x1000;
                }
                if (frame < 0x19) {
                    k = 0;
                    w = (frame << 12) / 24;
                } else {
                    k = 1;
                    w = ((frame - 0x19) << 12) / 45;
                }
                for (i = 0; i < 2; i++) {
                    fxm_keyGetVec3((u8 *)ex->btlfx_getFileData(entry->file, fx_0b3_morphKeyBlocks[i]), frame, &a, &rot, &sc);
                    pos.vx = a.vx;
                    pos.vy = a.vy;
                    pos.vz = a.vz;
                    scale.vx = sc.vx;
                    scale.vy = sc.vy;
                    scale.vz = sc.vz;
                    ex->Hi_DrawMorphEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model78[k], work->model78[k + 1],
                                             0x1000 - w);
                    if (c != 0x1000) {
                        ex->Hi_ModifyEffModelRGB(work->model78[k], (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
                    }
                }
            }
            if (frame < 0x14) {
                if (frame >= 0xA) {
                    c2 = 0x1000 - ((frame - 0xA) << 12) / 10;
                } else {
                    c2 = 0x1000;
                }
                pos2.vx = 0;
                pos2.vy = 0;
                pos2.vz = -0x2F;
                rot2.vx = 0;
                rot2.vy = 0;
                rot2.vz = 0;
                fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x2D), frame, &b);
                scale2.vx = b.vx;
                scale2.vy = b.vy;
                scale2.vz = b.vz;
                ex->Hi_DrawEffModel(&rot2, (VECTOR *)&pos2, (s32)&scale2, work->model78[3]);
                if (c2 != 0x1000) {
                    ex->Hi_ModifyEffModelRGB(work->model78[3], (c2 >> 5) - 0x80, (c2 >> 5) - 0x80, (c2 >> 5) - 0x80);
                }
            }
            if (frame == 9) {
                work->pos18.vx = 0;
                work->pos18.vy = -0x1DB;
                work->pos18.vz = 0;
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos18);
                if (e != NULL) {
                    e->otZ = 0x40;
                }
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos18);
                if (e != NULL) {
                    e->otZ = 0x40;
                }
            }
            if (frame == 0x19) {
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
                if (e != NULL) {
                    e->otZ = 0x40;
                }
            }
            if (frame == 0x2C) {
                work->pos20.vx = 0;
                work->pos20.vy = -0x258;
                work->pos20.vz = 0;
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2F), (s32)&work->pos20);
                if (e != NULL) {
                    e->trails = (s32)fx_0b3_trails;
                    e->trailSlots = 0xF0;
                    e->trailStride = 0x1C;
                    e->otZ = 0x40;
                }
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x30), (s32)&work->pos18);
                if (e != NULL) {
                    e->otZ = 0x40;
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

static void fxm_drawWaveWall(SVECTOR *pos, SVECTOR *rot, Fx0b3Vec3 *scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 az,
                   s32 ay, s32 ph, s32 dph, s32 ax, s32 phx, s32 dphx) {
    Unk800F78A4 *env = fx_0b3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col0;
    CVECTOR col1;
    CVECTOR col2;
    CVECTOR col3;
    CVECTOR col4;
    MATRIX m;
    MATRIX m2;
    SVECTOR p[4];
    VECTOR lv;
    s32 otz;
    POLY_FT4 *ft4;
    POLY_GT4 *gt4;
    POLY_G4 *g4;
    s32 i;
    s32 zz;
    s32 j;
    s32 y;
    s32 k;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 ft;
    s32 k2;
    s32 x0;

    /* x0 is a variable: reload rematerializes the constant into a register for x0 + zz */
    x0 = -10000;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    ft4 = (POLY_FT4 *)env->prim;
    *(s32 *)&col2 = 0;
    *(s32 *)&col4 = 0;
    gt4 = (POLY_GT4 *)(ft4 + 100);
    tpage |= 0x20;
    g4 = (POLY_G4 *)(gt4 + 20);
    env->prim = (u8 *)(g4 + 20);
    col0.cd = 0x2C;
    col2.cd = 0x3E;
    col1.cd = 0x3E;
    col3.r = col3.g = col3.b = 0xFF;
    col0.r = col0.g = col0.b = c;
    col1.r = col1.g = col1.b = c;
    col4.cd = 0x3A;
    col3.cd = 0x3A;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, (VECTOR *)scale);
    m2 = m;
    m2.t[0] = pos->vx;
    m2.t[1] = pos->vy;
    m2.t[2] = pos->vz;
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    zz = 0;
    for (j = 0; j < 20; j++, zz += 1000) {
        y = 0;
        ph += dph;
        for (i = 0; i < 6; i++, y -= 1000) {
            p[0].vx = (x0 + zz) + ((ex->math_sin(phx + i * dphx) * ax) >> 12);
            p[1].vx = p[0].vx + 1000;
            p[2].vx = (x0 + zz) + ((ex->math_sin(phx + (i + 1) * dphx) * ax) >> 12);
            p[3].vx = p[2].vx + 1000;
            if (i != 0) {
                p[0].vy = y + ((ex->math_sin(ph) * ay) >> 12);
                p[2].vy = p[0].vy - 1000;
                p[1].vy = y + ((ex->math_sin(ph + dph) * ay) >> 12);
                p[3].vy = p[1].vy - 1000;
            } else {
                /* d gives (sin * ay >> 12) - 1000 its own register, so it is added last */
                p[0].vy = y;
                {
                    s32 d = ((ex->math_sin(ph) * ay) >> 12) - 1000;
                    p[2].vy = p[0].vy + d;
                }
                p[1].vy = y;
                {
                    s32 d = ((ex->math_sin(ph + dph) * ay) >> 12) - 1000;
                    p[3].vy = p[1].vy + d;
                }
            }
            p[0].vz = p[1].vz = (ex->math_sin((i << 11) / 6) * az) >> 12;
            p[2].vz = p[3].vz = (ex->math_sin(((i + 1) << 11) / 6) * az) >> 12;
            gte_ldv3c(p);
            gte_rtpt();
            k = j & 1;
            u0 = u + (k << 6);
            u1 = u0 + 0x40;
            if (k != 0) {
                u1 = u0 + 0x3F;
            }
            /* k2 is local to this block (k lives in several), so local-alloc gives it $v0 */
            k2 = i & 3;
            v0 = v + (k2 << 5);
            v1 = v0 + 0x20;
            if (k2 == 3) {
                v1 = v0 + 0x1F;
            }
            gte_stopz(&otz);
            if (otz != 0) {
                gte_avsz3();
                gte_stszotz(&otz);
                ft = 1;
                if (i >= 5) {
                    *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&col1;
                    *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&col2;
                    *(s32 *)&g4->r0 = *(s32 *)&g4->r1 = *(s32 *)&col3;
                    ft = 0;
                    *(s32 *)&g4->r2 = *(s32 *)&g4->r3 = *(s32 *)&col4;
                    gte_SetRotMatrix(&m2);
                    gte_SetTransMatrix(&m2);
                    gte_ldv0(&p[2]);
                    gte_rtv0tr();
                    gte_stlvnl(&lv);
                    fx_0b3_crestPts[j].vx = lv.vx;
                    fx_0b3_crestPts[j].vy = lv.vy;
                    fx_0b3_crestPts[j].vz = lv.vz;
                    gte_SetRotMatrix(&m);
                    gte_SetTransMatrix(&m);
                } else {
                    *(s32 *)&ft4->r0 = *(s32 *)&col0;
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
                    *(s32 *)&g4->x0 = *(s32 *)&gt4->x0;
                    *(s32 *)&g4->x1 = *(s32 *)&gt4->x1;
                    *(s32 *)&g4->x2 = *(s32 *)&gt4->x2;
                }
                gte_ldv0(&p[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, otz, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
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

s32 fx_0b3_op84(s32 mode, Fx0b3Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR rot;
    Fx0b3Vec3 scale;
    Fx0b3Vec3 pos;
    SVECTOR rot2;
    Fx0b3Vec3 scale2;
    Fx0b3Vec3 pos2;
    SVECTOR rot3;
    Fx0b3Vec3 scale3;
    Pool6CEntry *e;
    s32 frame;
    s32 t;
    s32 c;
    s32 i;
    s32 u;
    s32 x;
    s32 y;
    s32 z;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work84);
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
        ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->loader)->unk54->motion18C, ((Fx0b3Summon *)env->loader)->model60);
        ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->loader)->model60, 0x1A);
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            ex->Hi_DrawEffModel(&work->rot80, (VECTOR *)&work->modelPos68, (s32)&work->modelScale74, fx_0b3_sharedModel);
            fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x15), frame, &a);
            work->summonPos48.vx = a.vx;
            work->summonPos48.vy = a.vy;
            work->summonPos48.vz = a.vz;
            work->pos30.vx = a.vx;
            work->pos30.vy = a.vy;
            work->pos30.vz = a.vz;
            ex->Hi_DrawSummonModel(&work->rot60, (VECTOR *)&work->summonPos48, (s32)&work->summonScale54,
                                   ((Fx0b3Summon *)env->loader)->model60, 1);
            if ((u32)(frame - 0x4B) < 5) {
                fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x3F), frame, &a);
                rot.vx = 0;
                rot.vy = 0;
                rot.vz = 0;
                pos.vx = 0;
                pos.vy = -0x5C3;
                pos.vz = 0x61A8;
                scale.vx = a.vx;
                scale.vz = a.vz;
                scale.vy = a.vy = a.vy / 3;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->model8C);
            }
            if ((u32)(frame - 0x4F) < 0x10) {
                fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0xC), frame, &a);
                pos2.vz = 0x61A8;
                rot2.vx = 0;
                rot2.vy = 0;
                rot2.vz = 0;
                pos2.vx = 0;
                pos2.vy = 0;
                scale2.vx = a.vx;
                scale2.vy = a.vy;
                scale2.vz = a.vz;
                if (frame < 0x5F) {
                    c = ex->math_cos((frame - 0x4F) << 6);
                } else {
                    c = 0x1000;
                }
                ex->Hi_DrawEffModel(&rot2, (VECTOR *)&pos2, (s32)&scale2, work->model88);
                y = c >> 5;
                y -= 0x80;
                ex->Hi_ModifyEffModelRGB(work->model88, y, y, y);
            }
            if (frame >= 0x4E) {
                t = frame - 0x4E;
                u = (t << 12) / 26;
                a.vz = 0x6590 - (((0x1000 - ex->math_cos(u >> 2)) * 0x8278) >> 12);
                a.vy = -0x200;
                scale3.vz = 0x2000;
                scale3.vx = 0x2000;
                a.vx = 0;
                rot3.vz = 0;
                rot3.vy = 0;
                rot3.vx = 0;
                if (frame < 0x53) {
                    scale3.vy = (ex->math_sin(t * 0x5E8 / 5) >> 2) + 0x200;
                } else {
                    scale3.vy = ((ex->math_sin((((frame - 0x53) << 12) / 21) >> 2) * 3) >> 2) + 0x400;
                }
                fxm_drawWaveWall(&a, &rot3, &scale3, 0x80, 0x17, 0x3D07, 0, 0x100, 0x400, 0x2BC, u * 3 / 2,
                              0x100, 0x400, u >> 1, 0x80);
                env->otZ = 0x80;
                for (i = 0; i < 20; i++) {
                    BTLFX_COPY_SVEC(&a, &fx_0b3_crestPts[i]);
                    ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 0x26), &a, NULL, ((i & 1) << 12) + 0x5000,
                                      frame + i * 5, 1, 0x1000);
                    a.vy = -0x100;
                    ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 0x24), &a, NULL, 0x7000, frame + i * 3, 1,
                                      0xC00);
                }
                env->otZ = 0;
            }
            fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, 0x27), frame, &work->pos28);
            work->pos28.vy = -0x100;
            if (frame == 0) {
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->pos28);
                if (e != NULL) {
                    e->otZ = 0x100;
                }
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x31), (s32)&work->pos30);
                if (e != NULL) {
                    e->trails = (s32)fx_0b3_trails2;
                    e->trailSlots = 0x30;
                    e->trailStride = 0x1C;
                    e->otZ = 0x100;
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

static s32 fxm_sinHalfNeg(s32 a) {
    s32 v = BTLFX_EXPORTS(fx_0b3_env)->math_sin(a);

    if (v < 0) {
        v >>= 1;
    }
    return v;
}

static void fxm_drawWaterSheet(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 c, s32 c2, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 ay, s32 ph, s32 dph, s32 az, s32 phz, s32 dphz) {
    Unk800F78A4 *env = fx_0b3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col0;
    CVECTOR col1;
    CVECTOR col2;
    CVECTOR col3;
    CVECTOR col4;
    CVECTOR col5;
    MATRIX m;
    MATRIX m2;
    SVECTOR p[4];
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
    s32 k;
    s32 k2;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 ft;
    s32 abr;
    s32 x0;
    s32 z0;

    abr = 1;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)env->prim;
    *(s32 *)&col2 = 0;
    *(s32 *)&col5 = 0;
    ft4 = (POLY_FT4 *)(gt4 + 18);
    g4 = (POLY_G4 *)(ft4 + 324);
    f4 = (POLY_F4 *)(g4 + 18);
    env->prim = (u8 *)(f4 + 324);
    col0.r = col0.g = col0.b = c;
    col1.r = col1.g = col1.b = c;
    col3.r = col3.g = col3.b = c2;
    col4.r = col4.g = col4.b = c2;
    col0.cd = abr != 0xFF ? 0x2E : 0x2C;
    col2.cd = abr != 0xFF ? 0x3E : 0x3C;
    col1.cd = col2.cd;
    col3.cd = abr != 0xFF ? 0x2A : 0x28;
    col5.cd = abr != 0xFF ? 0x3A : 0x38;
    tpage |= (abr & 3) << 5;
    col4.cd = col5.cd;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    m2 = m;
    m2.t[0] = pos->vx;
    m2.t[1] = pos->vy;
    m2.t[2] = pos->vz;
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    x0 = -0x24EA;
    z0 = -0x17CD;
    zz = 0;
    for (j = 0; j < 18; j++, zz += 0x2A5) {
        i = 0;
        ph += dph;
        y0 = z0 + zz;
        y1 = y0 + 0x2A5;
        for (xx = 0; i < 19; i++, xx += 0x41A) {
            x = x0 + xx;
            p[0].vx = x + ((ex->math_sin(ph) * ay) >> 12);
            p[1].vx = p[0].vx + 0x41A;
            x += (ex->math_sin(ph + dph) * ay) >> 12;
            p[0].vy = p[1].vy = y0;
            p[2].vy = p[3].vy = y1;
            p[2].vx = x;
            x += 0x41A;
            p[3].vx = x;
            p[0].vz = p[2].vz = (fxm_sinHalfNeg(phz + i * dphz) * az) >> 12;
            p[1].vz = p[3].vz = (fxm_sinHalfNeg(phz + (i + 1) * dphz) * az) >> 12;
            gte_ldv3c(p);
            gte_rtpt();
            k = i & 1;
            u0 = u + (k << 6);
            u1 = u0 + 0x40;
            if (k != 0) {
                u1 = u0 + 0x3F;
            }
            k2 = j & 1;
            v0 = v + (k2 << 6);
            v1 = v0 + 0x40;
            if (k2 != 0) {
                v1 = v0 + 0x3F;
            }
            gte_nclip();
            gte_stopz(&opz);
            if (opz != 0) {
                gte_avsz3();
                gte_stszotz(&otz);
                ft = 1;
                if (i == 0) {
                    *(s32 *)&gt4->r0 = *(s32 *)&gt4->r2 = *(s32 *)&col2;
                    *(s32 *)&gt4->r1 = *(s32 *)&gt4->r3 = *(s32 *)&col1;
                    *(s32 *)&g4->r0 = *(s32 *)&g4->r2 = *(s32 *)&col5;
                    ft = 0;
                    *(s32 *)&g4->r1 = *(s32 *)&g4->r3 = *(s32 *)&col4;
                } else {
                    *(s32 *)&ft4->r0 = *(s32 *)&col0;
                    *(s32 *)&f4->r0 = *(s32 *)&col3;
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
                    gte_SetRotMatrix(&m2);
                    gte_SetTransMatrix(&m2);
                    gte_ldv0(&p[0]);
                    gte_rtv0tr();
                    gte_stlvnl(&lv);
                    fx_0b3_edgePts[j].vx = lv.vx;
                    fx_0b3_edgePts[j].vy = lv.vy;
                    fx_0b3_edgePts[j].vz = lv.vz;
                    gte_SetRotMatrix(&m);
                    gte_SetTransMatrix(&m);
                    gte_ldv0(&p[0]);
                    gte_rtps();
                    gte_stsxy(&sxy);
                    if ((u16)(sxy.vx + 0x20) > 0x180 || sxy.vy < -0x20 || sxy.vy > 0x110) {
                        fx_0b3_edgePts[j].pad = 0;
                    } else {
                        fx_0b3_edgePts[j].pad = 1;
                    }
                }
                gte_ldv0(&p[3]);
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

static s32 fxm_nextRingPoint(SVECTOR *v) {
    s32 i = fx_0b3_edgePtIdx;

    v->pad = 1;
    v->vx = fx_0b3_edgePts[i].vx;
    v->vy = fx_0b3_edgePts[i].vy;
    v->vz = fx_0b3_edgePts[i].vz;
    i++;
    if (i >= 16) {
        i = 0;
    }
    fx_0b3_edgePtIdx = i;
    return 0;
}

s32 fx_0b3_op85(s32 mode, Fx0b3Work85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR q;
    SVECTOR a;
    SVECTOR b;
    VECTOR c;
    SVECTOR p1;
    SVECTOR p2;
    VECTOR scale;
    Pool6CEntry *p;
    SVECTOR a2;
    SVECTOR b2;
    VECTOR c2;
    s32 frame;
    s32 t;
    s32 f;
    s32 d;
    s32 m;
    s32 s;
    s32 s_6;
    s32 s_5;
    s32 u;
    s32 u_7;
    s32 k;
    s32 o;
    s32 k_10;
    s32 r;
    s32 g;
    s32 i;
    s32 j;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work85);
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
        ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->loader)->unk54->motion18C, ((Fx0b3Summon *)env->loader)->model60);
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
                    ((EffTailSpawn *)work->emit68->tail)->spawnFn = (s32 (*)(SVECTOR *, SVECTOR *, s32, s32))fxm_nextRingPoint;
                    if (work->emit68 != NULL) {
                        work->emit68->otZ = 0x200;
                    }
                }
            }
            if (frame >= 9) {
                if (frame < 0x3D) {
                    f = ((frame - 9) << 12) / 52;
                } else {
                    f = 0x1000;
                }
                if (frame < 0x14) {
                    a.vx = 0;
                    m = 1;
                } else {
                    a.vx = 0x1000 - (t >> 1);
                    m = 2;
                }
                s_5 = ex->math_sin(t >> 2);
                u_7 = f >> 2;
                a.vy = (s16)(((0x1000 - s_5) >> 2) - 0x400) - ((ex->math_cos(u_7) * 0x44C) >> 12);
                a.vz = ((0x1000 - ex->math_cos(u_7)) << 2) - 0x4000;
                b.vx = 0x400;
                b.vy = 0x400;
                b.vz = 0;
                c.vy = 0x1000;
                c.vx = 0x1000;
                c.vz = ex->math_cos(u_7);
                if (frame < 0x10) {
                    d = ex->math_sin((frame << 10) >> 4) + 0x800;
                } else {
                    d = 0x1800;
                }
                k = 0xC8;
                if ((u32)(frame - 0x28) < 0x20) {
                    k = 0xC8 - (((frame - 0x28) * 0x96) >> 5);
                } else if (frame >= 0x48) {
                    k = 0x32;
                }
                if (frame >= 0x3D) {
                    r = 0x80 - (((frame - 0x3D) << 5) / 20);
                } else {
                    r = 0x80;
                }
                if (frame < 0x29) {
                    env->otZ = 0;
                } else {
                    env->otZ = -0x80;
                }
                fxm_drawWaterSheet(&a, &b, &c, r, k, 0x17, 0x3D07, 0, 0x80, 0x7D0, f * m, -0x140, d,
                              ex->math_cos(f >> 2) + 0x400, 0x100);
                g = (t * 3) >> 3;
                env->otZ = 0x7C;
                for (i = 0; i < 18; i++) {
                    if (fx_0b3_edgePts[i].pad != 0) {
                        r = (i & 1) << 12; /* r reused: a global pseudo, so r + 0x3800 is not tied to it */
                        ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 0x24), &fx_0b3_edgePts[i], NULL,
                                                g + (s16)(r + 0x3800), frame + i * 2, 1, 0x1000);
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
                fxm_keyGetVec((Fx0b3Track *)ex->btlfx_getFileData(entry->file, 0x16), frame, &q);
                work->summonPos28.vx = q.vx;
                work->summonPos28.vy = q.vy;
                work->summonPos28.vz = q.vz;
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
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&work->pos10);
                if (p != NULL) {
                    p->otZ = 0x40;
                }
            }
            if (frame >= 0x2B) {
                s_6 = ((frame - 0x2B) << 10) / 38;
                ex->math_cos(s_6);
                ex->math_cos(s_6);
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
                k_10 = frame - 0x2B;
                p2.vx = 0;
                p2.vy = 0;
                p2.vz = 0;
                scale.vz = 0x1000;
                ((CVECTOR *)&p1)->r = ((CVECTOR *)&p1)->g = ((CVECTOR *)&p1)->b = -0x10 - (k_10 * 0x8D) / 38;
                scale.vy = scale.vx = 0x1400 - (k_10 << 10) / 38;
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
                ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&a2);
                if (a2.vy < -0x800) {
                    work->camPos18.vz = 0x300;
                    work->camPos18.vy -= 0x1F4;
                }
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 24;
            u = t >> 2;
            work->camPos18.vy -= (ex->math_cos(u) * 0x18) >> 12;
            env->camera->points[3].vx = work->camPos18.vx;
            env->camera->points[3].vy = work->camPos18.vy;
            env->camera->points[3].vz = work->camPos18.vz;
            a2.vx = 0;
            a2.vy = -0x200 - (ex->math_cos(u) >> 3);
            b2.vx = 0x400;
            b2.vy = 0x400;
            a2.vz = 0;
            b2.vz = 0;
            c2.vy = 0x1000;
            c2.vx = 0x1000;
            c2.vz = 0;
            s = ex->math_cos(u) >> 5;
            fxm_drawWaterSheet(&a2, &b2, &c2, s, s, 0x17, 0x3D07, 0, 0x80, (ex->math_cos(u) >> 3) + 0x300, u, 0x100, 0, 0,
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

s32 fx_0b3_op86(s32 mode, Fx0b3Work86 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *e;
    s32 i;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work86);
    } else if (mode == 1) {
        fx_0b3_env = env;
        work->unitCount40 = env->targets->targetCount;
        for (i = 0; i < work->unitCount40; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->emitPos0[i]);
                work->emitPos0[i].vz -= ex->btlfx_getUnitRadius(i);
                work->emitPos0[i].vy -= 0x200;
                e = ex->btlfx_startEmitter((s32)ex->btlfx_getFileData(entry->file, 0x23), (s32)&work->emitPos0[i]);
                if (e != NULL) {
                    e->otZ = ex->btlfx_addRadiusSixth(i, 0x200);
                }
            }
        }
    } else if (entry->frame >= 0x2E) {
        return 1;
    }
    return 0;
}

s32 fx_0b3_op87(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        t = *arg4;
        fx_0b3_env = env;
        *work = t;
        if (*work >= 10) {
            v.vx = env->targets->centerX;
            v.vy = env->targets->centerY;
            v.vz = env->targets->centerZ;
            v.vy -= 0x2BC;
            env->camera->points[2].vx = v.vx;
            env->camera->points[2].vy = v.vy;
            env->camera->points[2].vz = v.vz;
            return 1;
        }
    } else {
        frame = entry->frame;
        fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, fx_0b3_camShots[*work].point1Keys2), frame, &v);
        env->camera->points[1].vx = v.vx;
        env->camera->points[1].vy = v.vy;
        env->camera->points[1].vz = v.vz;
        fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, fx_0b3_camShots[*work].point0Keys0), frame, &v);
        env->camera->points[0].vx = v.vx;
        env->camera->points[0].vy = v.vy;
        env->camera->points[0].vz = v.vz;
        if (frame >= fx_0b3_camShots[*work].endFrame4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_headerPtr);
