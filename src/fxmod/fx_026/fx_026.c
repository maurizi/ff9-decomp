#include "fxmod/fx_026.h"

void func_801E7704(Fx026Track *track, s32 t, SVECTOR *out) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    track = (Fx026Track *)&track->unk4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

void func_801E7754(Fx026Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 6];
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

void func_801E77DC(Fx026Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 9];
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

s32 func_801E7890(Fx026Track *track, s32 t) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    return (track->unk4 + t)[0];
}

void func_801E78BC(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801EACCC;
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
        addPrim(env->ot + 0xFFF, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->ot + 0xFFF, p);
    }
}

s32 func_801E7A5C(s32 mode, Fx026Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx026Work80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR p;
    s32 n;
    s32 t;

    if (mode == 1) {
        it->unk0 = 0;
        it->unk8 = NULL;
    } else {
        if (it->unk0 != 0) {
            return 0;
        }
        n = ((Fx026Track *)ex->btlfx_getFileData(entry->file, D_801EA774[it->unk4]))->unk2;
        if (n == 0) {
            n = 1;
        }
        t = (frame << 12) / n;
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->file, D_801EA774[it->unk4]), frame, &p);
        pos.vx = p.vx;
        pos.vy = p.vy;
        pos.vz = p.vz;
        scale.vx = scale.vy = 0x1400 - ex->math_cos(t >> 2);
        scale.vz = t;
        rot.vx = -0xEE;
        rot.vy = 0;
        rot.vz = it->unk2;
        it->unk2 += 0x20;
        if (frame == 4) {
            it->unkC.vx = pos.vx;
            it->unkC.vy = pos.vy;
            it->unkC.vz = pos.vz;
            it->unk8 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x86), (s32)&it->unkC);
            if (it->unk8 != NULL) {
                it->unk8->unk22 = 0x80;
            }
        }
        if (it->unk8 != NULL) {
            if (it->unk8->unk30 == -1) {
                it->unk8 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)it->unk8, (s32)&p);
        }
        if (frame >= 4) {
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk68);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk6C);
        }
        if (frame >= n) {
            return 1;
        }
    }
    return 0;
}

s32 fx_026_op80(s32 mode, Fx026Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    Pool6CEntry *q;
    Fx026Item *it;
    s32 frame;
    s32 k;
    s32 s;
    s32 t;

    if (mode == 0) {
        init->workSize = 0x70;
        init->elemSize = 0x14;
        init->elemFn = (s32)func_801E7A5C;
        init->poolCount = 6;
    } else {
    if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EACCC = env;
        ex->btlfx_initEffModels(D_801EACDC, 0x20);
        work->unk58.vy = 0x800;
        work->unk4C.vx = 0x1000;
        work->unk4C.vy = 0x1000;
        work->unk4C.vz = 0x1000;
        work->unk58.vx = 0;
        work->unk58.vz = 0;
        work->unk40.vx = 0;
        work->unk40.vy = 0x2000;
        work->unk40.vz = 0;
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->file, 0xB), 2, &p);
        p.vy = 0;
        work->unk10.vx = p.vx;
        work->unk10.vy = p.vy;
        work->unk10.vz = p.vz;
        work->unk64 = 0;
    } else {
    k = 0;
    frame = entry->frame;
    s = 0;
    switch (work->unk0) {
    case 0:
        if (frame == 0) {
            q = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&work->unk10);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
            q = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->unk10);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
            q = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk10);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame >= 40) {
            work->unk0 = 1;
            entry->frame = -1;
            ex->Hi_SetSummonMotion(((Fx026Summon *)env->loader)->unk54->unk180, ((Fx026Summon *)env->loader)->unk60);
            ex->Hi_SetSummonMotFrame(((Fx026Summon *)env->loader)->unk60, 0);
            ex->Hi_StartSummonTexAnim(((Fx026Summon *)env->loader)->unk60, 0, 0);
        }
        break;
    case 1:
        work->unk40.vx = env->camera->points[2].vx;
        work->unk40.vy = env->camera->points[2].vy;
        work->unk40.vz = env->camera->points[2].vz;
        if (frame >= 2) {
            ex->Hi_DrawSummonModel(&work->unk58, (VECTOR *)&work->unk40, (s32)&work->unk4C,
                                   ((Fx026Summon *)env->loader)->unk60, 0);
            ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->unk60, 0, (u16 *)&work->unk28);
        }
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->file, 0xB), work->unk4, &p);
        work->unk40.vx = p.vx;
        work->unk40.vy = p.vy;
        work->unk40.vz = p.vz;
        env->camera->points[2].vx = p.vx;
        env->camera->points[2].vy = p.vy;
        env->camera->points[2].vz = p.vz;
        if (frame == 38) {
            q = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA2), (s32)&work->unk28);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame == 42) {
            q = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA3), (s32)&work->unk28);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame >= 66) {
            work->unk0 = 2;
            entry->frame = -1;
            D_801EC5DC = work->unk68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x85), 0);
            work->unk6C = D_801EC5E0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x87), 1);
        }
        break;
    case 2:
        work->unk40.vx = env->camera->points[2].vx;
        work->unk40.vy = env->camera->points[2].vy;
        work->unk40.vz = env->camera->points[2].vz;
        t = (frame << 12) / 50;
        ex->Hi_DrawSummonModel(&work->unk58, (VECTOR *)&work->unk40, (s32)&work->unk4C,
                               ((Fx026Summon *)env->loader)->unk60, 0);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->unk60, 0, (u16 *)&work->unk28);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->unk60, 0x19, (u16 *)&work->unk30);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->unk60, 0x12, (u16 *)&work->unk38);
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->file, 0xE), frame, &p);
        work->unk40.vx = p.vx;
        work->unk40.vy = p.vy;
        work->unk40.vz = p.vz;
        env->camera->points[2].vx = p.vx;
        env->camera->points[2].vy = p.vy;
        env->camera->points[2].vz = p.vz;
        if (frame == 0) {
            q = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA1), (s32)&work->unk28);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame == 29) {
            q = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA6), (s32)&work->unk30);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame == 32) {
            ex->Hi_StartSummonTexAnim(((Fx026Summon *)env->loader)->unk60, 1, 0);
        }
        k = t;
        s = (k >> 1) + 0x800;
        if (frame >= 50) {
            work->unk0 = 3;
            entry->frame = -1;
            ex->btlfx_initParticleSystem();
            ex->btlfx_poolClear(entry->pool);
            q = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA4), (s32)&work->unk28);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        break;
    case 3:
        if (frame == 0) {
            q = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA5), (s32)&work->unk30);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame >= 4 && frame < 10) {
            it = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (it != NULL) {
                it->unk4 = frame - 4;
                it->unk2 = ex->btlfx_rand();
            }
        }
        work->unk40.vx = env->camera->points[2].vx;
        work->unk40.vy = env->camera->points[2].vy;
        work->unk40.vz = env->camera->points[2].vz;
        ex->Hi_DrawSummonModel(&work->unk58, (VECTOR *)&work->unk40, (s32)&work->unk4C,
                               ((Fx026Summon *)env->loader)->unk60, 0);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->unk60, 0, (u16 *)&work->unk28);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->unk60, 0x19, (u16 *)&work->unk30);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->unk60, 0x12, (u16 *)&work->unk38);
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->file, 0x11), frame, &p);
        work->unk40.vx = p.vx;
        work->unk40.vy = p.vy;
        work->unk40.vz = p.vz;
        env->camera->points[2].vx = p.vx;
        k = 0x1000;
        env->camera->points[2].vy = p.vy;
        env->camera->points[2].vz = p.vz;
        s = k;
        if (frame >= 29) {
            ex->btlfx_poolClear(entry->pool);
            return 1;
        }
        break;
    }
    if (k != 0) {
        s = s * 2 / 3;
        if (work->unk4 & 1) {
            s = s * 9 >> 3;
        }
        env->otZ = 0x40;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA0), &work->unk30, 0, s, 0, 1, k);
        env->otZ = 0;
    }
    work->unk4++;
    }
    }
    return 0;
}

s32 func_801E8670(s32 mode, Fx026Item82 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work82 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR v2;
    SVECTOR prev;
    s32 w;
    s32 k;
    u8 *data;
    u8 *track;
    Fx026Strip32 *strip;
    Fx026Pt *pt;
    s32 i;
    s32 t;
    s32 fade;

    if (mode == 1) {
        it->unk0 = 0;
        it->unk8 = 0;
    } else {
    switch (it->unk4) {
    case 0:
        track = (u8 *)ex->btlfx_getFileData(entry->file, 2);
        w = 0x46;
        strip = &D_801EC5EC[it->unk2];
        t = frame * 2;
        track += ((s32 *)track)[it->unk2];
        strip->unk2 = 0x1F;
        strip->unk0 = 0x1F;
        strip->unk1 = 0;
        pt = strip->unk4;
        for (i = 0; i < 32; i++, pt++, t--) {
            if (t >= 0) {
                func_801E7704((Fx026Track *)track, t, &v);
                if (i == 0) {
                    prev.vx = D_801EC5E4.vx + v.vx;
                    prev.vy = D_801EC5E4.vy + v.vy;
                    prev.vz = D_801EC5E4.vz + v.vz;
                }
                pt->vx = D_801EC5E4.vx + v.vx;
                pt->vy = D_801EC5E4.vy + v.vy;
                pt->vz = D_801EC5E4.vz + v.vz;
                pt->unk6 = w;
                BTLFX_COPY_SVEC(&prev, pt);
            } else {
                BTLFX_COPY_SVEC(pt, &prev);
                pt->unk6 = 0;
            }
        }
        fade = 0x1000;
        if (frame >= 16) {
            fade = ex->math_cos(((frame - 16) << 10) >> 4);
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x36), 0, 0, 0, 0x1000, fade, 1, (u8 *)strip, 0);
        if (frame >= 32) {
            return 1;
        }
        break;
    case 1:
        track = (u8 *)ex->btlfx_getFileData(entry->file, 0x9D);
        w = 0x46;
        strip = &D_801ECC0C[it->unk2];
        t = frame * 4;
        track += ((s32 *)track)[it->unk2];
        strip->unk2 = 0x1F;
        strip->unk0 = 0x1F;
        strip->unk1 = 0;
        pt = strip->unk4;
        if (t > 32) {
            t = 32;
        }
        for (i = 0; i < 32; i++, pt++, t--) {
            if (t >= 0) {
                func_801E7704((Fx026Track *)track, t, &v);
                if (i == 0) {
                    prev.vx = D_801EC5E4.vx + v.vx;
                    prev.vy = D_801EC5E4.vy + v.vy;
                    prev.vz = D_801EC5E4.vz + v.vz;
                }
                pt->vx = D_801EC5E4.vx + v.vx;
                pt->vy = D_801EC5E4.vy + v.vy;
                pt->vz = D_801EC5E4.vz + v.vz;
                pt->unk6 = w;
                BTLFX_COPY_SVEC(&prev, pt);
            } else {
                BTLFX_COPY_SVEC(pt, &prev);
                pt->unk6 = 0;
            }
        }
        fade = 0x1000;
        if (frame >= 8) {
            fade = ex->math_cos(((frame - 8) << 10) >> 4);
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x35), 0, 0, 0, 0x1000, fade, 1, (u8 *)strip, 0);
        if (frame >= 24) {
            return 1;
        }
        break;
    case 2:
        data = (u8 *)ex->btlfx_getFileData(entry->file, 0x9D);
        w = 0x58;
        track = data + ((s32 *)data)[it->unk2 + 6];
        data += ((s32 *)data)[it->unk2 + 12];
        if (frame < 10) {
            k = 0;
        } else {
            k = 0x1000 - ex->math_cos(((frame - 10) << 10) / 30);
        }
        t = frame * 4;
        strip = (Fx026Strip32 *)&D_801ED22C[it->unk2];
        strip->unk2 = 0x27;
        strip->unk0 = 0x27;
        strip->unk1 = 0;
        pt = strip->unk4;
        if (t > 40) {
            t = 40;
        }
        for (i = 0; i < 40; i++, pt++, t--) {
            if (t >= 0) {
                func_801E7704((Fx026Track *)track, t, &v);
                func_801E7704((Fx026Track *)data, t, &v2);
                ex->btlfx_lerpVec((s16 *)&v, (s16 *)&v2, k, (s16 *)&v);
                if (i == 0) {
                    prev.vx = D_801EC5E4.vx + v.vx;
                    prev.vy = D_801EC5E4.vy + v.vy;
                    prev.vz = D_801EC5E4.vz + v.vz;
                }
                pt->vx = D_801EC5E4.vx + v.vx;
                pt->vy = D_801EC5E4.vy + v.vy;
                pt->vz = D_801EC5E4.vz + v.vz;
                pt->unk6 = w;
                BTLFX_COPY_SVEC(&prev, pt);
            } else {
                BTLFX_COPY_SVEC(pt, &prev);
                pt->unk6 = 0;
            }
        }
        fade = 0x1000;
        if (frame >= 24) {
            fade = ex->math_cos(((frame - 24) << 10) >> 4);
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x34), 0, 0, 0, 0x1000, fade, 1, (u8 *)strip, 0);
        if (frame >= 40) {
            return 1;
        }
        break;
    }
    }
    return 0;
}

s32 fx_026_op82(s32 mode, Fx026Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Fx026Item82 *it;
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    s32 t;
    s32 c;

    if (mode == 0) {
        init->workSize = 0x14;
        init->elemSize = 0xC;
        init->elemFn = (s32)func_801E8670;
        init->poolCount = 0x18;
    } else if (mode == 1) {
        work->unk8.vx = 0;
        work->unk8.vz = -2500;
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk8.vy = 0;
        D_801EC5E4.vx = work->unk8.vx;
        D_801EC5E4.vy = work->unk8.vy;
        D_801EACCC = env;
        D_801EC5E4.vz = work->unk8.vz;
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            if (frame == 13) {
                for (i = 0; i < 6; i++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        it->unk4 = 0;
                        it->unk2 = i;
                    }
                }
            }
            if (frame == 13) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame >= 39 && frame < 92) {
                if (frame == 39) {
                    work->unk10 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xAB), 1);
                }
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = frame * 16;
                t = ((frame - 39) << 12) / 53 >> 2;
                scale.vx = scale.vz = 0x2000 - ex->math_cos(t);
                scale.vy = 0x1800;
                c = ex->math_cos(t);
                pos.vx = work->unk8.vx;
                pos.vy = work->unk8.vy;
                pos.vz = work->unk8.vz;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk10);
                ex->Hi_ModifyEffModelRGB(work->unk10, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
                func_801E78BC(1000 - frame * 2, 0x200, 0x1C0, 0x80, 0x40, 0x200, 0x180);
            }
            if (frame == 39) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x9F), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x9E), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
                for (i = 0; i < 6; i++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        it->unk4 = 1;
                        it->unk2 = i;
                    }
                }
            }
            if (frame == 39) {
                for (i = 0; i < 6; i++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        it->unk4 = 2;
                        it->unk2 = i;
                    }
                }
            }
            if (frame >= 100) {
                return 1;
            }
        }
        work->unk4++;
    }
    return 0;
}

void func_801E912C(s32 i, s32 n, Fx026Shatter *buf, Fx026Tri *tris) {
    Fx026Shatter *h;

    D_801ED9CC[i] = buf;
    D_801ED9EC[i] = (Fx026Shard *)(buf + 1);
    h = D_801ED9CC[i];
    h->unk0 = n;
    h->unk2 = 0;
    h->unkC = tris;
    h->unk8 = tris;
}

s32 func_801E9170(s32 i, s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EACCC);
    Fx026Shatter *h = D_801ED9CC[i];
    Fx026Tri *t = h->unkC;
    Fx026Shard *p;
    s32 n;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)t);
    h->unk10 = t;
    h->unk4 = n;
    h->unkC = t + n;
    h->unk2 += n;
    for (k = 0; k < n; k++, t++) {
        t->unk0.pad = 0;
    }
    p = D_801ED9EC[i];
    for (k = 0; k < n; k++, p++) {
        p->unk18 = -1;
    }
    return h->unk2;
}

Fx026Shard *func_801E928C(s32 i) {
    Fx026Shatter *h = D_801ED9CC[i];
    Fx026Shard *p = D_801ED9EC[i];
    s32 k;

    for (k = 0; k < h->unk2; k++, p++) {
        if (p->unk18 < 0) {
            return p;
        }
    }
    return NULL;
}

s32 func_801E92EC(s32 i) {
    s32 idx = 0;
    s32 k = 0;
    Fx026Shatter *h = D_801ED9CC[i];
    Fx026Tri *t = h->unk8;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EACCC);
    s32 n = 0;
    s32 y = D_801EDA08;
    Fx026Shard *p;
    SVECTOR v;

    for (; k < h->unk2; k++, t++, idx++) {
        if (y < t->unk8.vy && t->unk0.pad == 0) {
            p = func_801E928C(i);
            if (p != NULL) {
                t->unk0.pad = 1;
                p->unk0 = ((t->unk0.vx + t->unk8.vx + t->unk10.vx) / 3) << 12;
                p->unk4 = ((t->unk0.vy + t->unk8.vy + t->unk10.vy) / 3) << 12;
                p->unk8 = ((t->unk0.vz + t->unk8.vz + t->unk10.vz) / 3) << 12;
                p->unk18 = 0;
                p->unk1A = idx;
                p->unk12 = 0x1000;
                p->unk14 = ex->btlfx_randCentered(0x100);
                p->unk16 = ex->btlfx_randCentered(0x200);
                v.vx = p->unk0 >> 12;
                v.vy = p->unk4 >> 12;
                v.vz = p->unk8 >> 12;
                p->unkC = (v.vx >> 5) + ex->btlfx_randCentered(0x18);
                n++;
                p->unkE = (v.vy >> 5) + (ex->btlfx_rand() & 0x1F);
                p->unk10 = (v.vz >> 5) + ex->btlfx_randCentered(0x18);
            }
        }
    }
    return n;
}

/* move the flying shards of shatter i and write their triangles back */
void func_801E9564(s32 i) {
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
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EACCC);
    Fx026Shard *p;
    Fx026Shatter *h;
    Fx026Tri *t;
    s32 k;

    sv.vx = sv.vy = sv.vz = 0xF9C;
    p = D_801ED9EC[i];
    h = D_801ED9CC[i];
    rot.vz = 0;
    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    for (k = 0; k < h->unk2; k++, p++) {
        if (p->unk18 >= 0) {
            t = &h->unk8[p->unk1A];
            c.vx = p->unk0 >> 12;
            c.vy = p->unk4 >> 12;
            c.vz = p->unk8 >> 12;
            d0.vx = t->unk0.vx - c.vx;
            d0.vy = t->unk0.vy - c.vy;
            d0.vz = t->unk0.vz - c.vz;
            d1.vx = t->unk8.vx - c.vx;
            d1.vy = t->unk8.vy - c.vy;
            d1.vz = t->unk8.vz - c.vz;
            d2.vx = t->unk10.vx - c.vx;
            d2.vy = t->unk10.vy - c.vy;
            d2.vz = t->unk10.vz - c.vz;
            ex->btlfx_stepModuleParticle((Pool6CEntry *)p, 0, &zero, &a);
            *(s32 *)&rot = *(s32 *)&p->unk14;
            ex->math_rotMatrixYXZ(&rot, &m);
            ex->ScaleMatrix(&m, &sv);
            m.t[0] = p->unk0 >> 12;
            m.t[1] = p->unk4 >> 12;
            m.t[2] = p->unk8 >> 12;
            ex->SetRotMatrix(&m);
            ex->SetTransMatrix(&m);
            gte_ldv0(&d0);
            gte_rtv0tr();
            gte_stlvnl(&out);
            gte_ldv0(&d1);
            gte_rtv0tr();
            t->unk0.vx = out.vx;
            t->unk0.vy = out.vy;
            t->unk0.vz = out.vz;
            gte_stlvnl(&out);
            gte_ldv0(&d2);
            gte_rtv0tr();
            t->unk8.vx = out.vx;
            t->unk8.vy = out.vy;
            t->unk8.vz = out.vz;
            gte_stlvnl(&out);
            t->unk10.vx = out.vx;
            t->unk10.vy = out.vy;
            t->unk10.vz = out.vz;
            p->unk18++;
            if (p->unk18 >= h->unk0) {
                p->unk18 = -1;
                t->unk10.vz = 0;
                *(s32 *)&t->unk10 = 0;
                t->unk8.vz = 0;
                *(s32 *)&t->unk8 = 0;
                t->unk0.vz = 0;
                *(s32 *)&t->unk0 = 0;
            }
        }
    }
}

s32 func_801E98BC(s32 mode, Fx026Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work81 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR v;
    SVECTOR unused; /* never used, but it takes a stack slot */
    s32 model;
    s32 model2;
    s32 c;
    Pool6CEntry *p;
    s32 f;
    s32 g;

    if (mode == 1) {
        it->unk0 = 0;
        it->pad4[1] = 0;
    } else if (it->unk2 < 6) {
        switch (it->unk0) {
        case 0:
            func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->file, D_801EA828[it->unk2]), frame, &v);
            v.vx += work->unk8.vx;
            v.vy += work->unk8.vy;
            v.vz += work->unk8.vz;
            pos.vx = v.vx;
            pos.vy = v.vy;
            pos.vz = v.vz;
            scale.vx = 0x2000;
            scale.vy = 0x2000;
            scale.vz = 0x3000;
            rot.vx = -0x200;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, D_801EC5E0);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, D_801EC5DC);
            if (frame >= 3) {
                it->unk0 = 1;
                ((s16 *)it)[-1] = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x20, 0x46);
                it->unkC.vx = D_801EA840[it->unk2].vx;
                it->unkC.vy = D_801EA840[it->unk2].vy;
                it->unkC.vz = D_801EA840[it->unk2].vz;
                it->unkC.vx += work->unk8.vx;
                it->unkC.vy += work->unk8.vy;
                it->unkC.vz += work->unk8.vz;
                it->unkC.vy = 0;
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&it->unkC);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            return 0;
        case 1:
            if (frame + (it->unk2 - 5) <= 0) {
                model = work->unk18[it->unk2][0];
                model2 = -1;
            } else {
                if (frame < 16) {
                    model = work->unk18[it->unk2][D_801EA870[frame]];
                } else {
                    model = work->unk18[it->unk2][2];
                }
                if (frame < 16) {
                    model2 = -1;
                } else {
                    model2 = work->unk6C[it->unk2];
                }
            }
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            if (model2 >= 0) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, model2);
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, model);
            if (it->unk2 < 5) {
                g = D_801EA81C;
            } else {
                g = D_801EA824;
            }
            if (g != 0) {
                it->unk0 = 2;
                ((s16 *)it)[-1] = -1;
                func_801E912C(it->unk2, 0x14, (Fx026Shatter *)((u8 *)env->workBuf + D_801EA7EC[it->unk2].unk0),
                              (Fx026Tri *)((u8 *)env->workBuf + D_801EA7EC[it->unk2].unk4));
                func_801E9170(it->unk2, work->unk18[it->unk2][2]);
                return 0;
            }
            break;
        case 2:
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk18[it->unk2][2]);
            if (frame >= 14) {
                f = 0x1000 - ((frame - 14) << 12) / 6;
                ex->Hi_ModifyEffModelAbr(work->unk18[it->unk2][2], 1);
                c = (f / 2 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk18[it->unk2][2], c, c, c);
            } else {
                ex->Hi_ModifyEffModelAbr(work->unk18[it->unk2][2], 1);
                c = -0x40;
                ex->Hi_ModifyEffModelRGB(work->unk18[it->unk2][2], c, c, c);
            }
            func_801E92EC(it->unk2);
            func_801E9564(it->unk2);
            if (frame >= 20) {
                return 1;
            }
            break;
        }
    } else {
    switch (it->unk0) {
    case 0:
        pos.vx = work->unk8.vx;
        pos.vy = work->unk8.vy;
        pos.vz = work->unk8.vz;
        scale.vx = 0x1000;
        scale.vy = 0x1000;
        scale.vz = 0x1000;
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk6C[6]);
        if (frame < 8) {
            ex->Hi_ModifyEffModelRGB(work->unk6C[6], ((frame << 12) >> 9) - 0x80, ((frame << 12) >> 9) - 0x80,
                                     ((frame << 12) >> 9) - 0x80);
        }
        if (D_801EA820 != 0) {
            it->unk0 = 1;
            ((s16 *)it)[-1] = -1;
            func_801E912C(it->unk2, 0x57, D_801EDA0C, D_801EF00C);
            func_801E9170(it->unk2, work->unk6C[6]);
            return 0;
        }
        break;
    case 1:
        pos.vx = work->unk8.vx;
        pos.vy = work->unk8.vy;
        pos.vz = work->unk8.vz;
        scale.vx = 0x1000;
        scale.vy = 0x1000;
        scale.vz = 0x1000;
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk6C[6]);
        if (frame >= 55) {
            ex->Hi_ModifyEffModelRGB(work->unk6C[6], ((0x1000 - (((frame - 55) << 12) >> 5)) >> 5) - 0x80,
                                     ((0x1000 - (((frame - 55) << 12) >> 5)) >> 5) - 0x80,
                                     ((0x1000 - (((frame - 55) << 12) >> 5)) >> 5) - 0x80);
        }
        func_801E92EC(6);
        func_801E9564(6);
        if (frame >= 87) {
            return 1;
        }
        break;
    }
    }
    return 0;
}

s32 fx_026_op81(s32 mode, Fx026Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx026Item81 *it;
    Fx026Item81 *it2;
    Pool6CEntry *p;
    s32 frame;
    s32 state;
    s32 i;
    s32 j;
    s32 c;

    if (mode == 0) {
        init->workSize = 0x88;
        init->elemSize = 0x14;
        init->elemFn = (s32)func_801E98BC;
        init->poolCount = 0x4A;
    } else if (mode == 1) {
        D_801EACCC = env;
        work->unk0 = 0;
        work->unk4 = 0;
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 3; j++) {
                work->unk18[i][j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, D_801EA788[i][j]), 0);
            }
        }
        for (i = 0; i < 7; i++) {
            work->unk6C[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, D_801EA7D0[i]), 1);
        }
        work->unk8.vx = D_801EA780.vx;
        work->unk8.vy = D_801EA780.vy;
        work->unk8.vz = D_801EA780.vz;
        work->unk10.vx = D_801EA780.vx;
        work->unk10.vy = D_801EA780.vy;
        work->unk10.vz = D_801EA780.vz;
    } else {
        state = work->unk0;
        frame = entry->frame;
        switch (state) {
        case 0:
            if (frame < 10 && (frame & 1)) {
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    it->unk2 = D_801EA8B0[frame >> 1];
                }
            }
            if (frame == 5) {
                it2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it2 != NULL) {
                    it2->unk2 = 6;
                }
            }
            if (frame == 4) {
                it2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it2 != NULL) {
                    it2->unk2 = 5;
                }
            }
            if (frame >= 16) {
                work->unk0 = 1;
                entry->frame = -1;
                D_801EDA08 = 0x400;
            }
            break;
        case 1:
            if (frame == 20) {
                D_801EA81C = 1;
                D_801EA820 = 1;
            } else {
                D_801EDA08 = 0x400 - (frame << 7);
            }
            if (frame == 24) {
                D_801EA824 = 1;
            }
            if (frame == 20) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x20), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk14 = (s32)D_801EA93C;
                    p->unk12 = 0xC;
                    p->unkC = 0x4C;
                    p->unk22 = 0x40;
                }
            }
            if (frame == 18) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame == 32) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame == 44) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame == 19) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0x80);
            }
            if (frame == 20) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x80, 0xFF);
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
        work->unk4++;
    }
    return 0;
}

s32 fx_026_op87(s32 mode, Fx026Work87 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        work->unk0 = *arg4;
        D_801EACCC = env;
    } else {
        frame = entry->frame;
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->file, D_801EA8C4[work->unk0].unk2), frame,
                      &work->unk4);
        env->camera->points[1].vx = work->unk4.vx;
        env->camera->points[1].vy = work->unk4.vy;
        env->camera->points[1].vz = work->unk4.vz;
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->file, D_801EA8C4[work->unk0].unk0), frame,
                      &work->unkC);
        env->camera->points[0].vx = work->unkC.vx;
        env->camera->points[0].vy = work->unkC.vy;
        env->camera->points[0].vz = work->unkC.vz;
        if (frame >= D_801EA8C4[work->unk0].unk4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_026/nonmatchings/fx_026", fx_026_headerPtr);
