#include "fxmod/fx_12b.h"

static s32 fx_12b_op80ParticleMotion0(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    s32 i = p->unk1F - 1;
    Fx12bShard *s = &D_801E97BC[i];
    Fx12bPath *path = &D_801E9B44[i];
    SVECTOR *ofs = &D_801EF7F4[i];

    if (s->unk20 >= 2 && s->unk20 < 7) {
        return func_801E7994(p, pos, s, path, ofs, frame, n);
    }
    if ((u16)(s->unk20 - 7) < 0xE) {
        return func_801E7B10(p, pos, s, frame, n);
    }
    return 0;
}

static s32 fx_12b_op80ParticleMotion1(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    s32 i = p->unk1F - 1;
    Fx12bShard *s = &D_801E9894[i];
    Fx12bPath *path = &D_801E9B74[i];
    SVECTOR *ofs = &D_801EF824[i];

    if (s->unk20 >= 2 && s->unk20 < 7) {
        return func_801E7994(p, pos, s, path, ofs, frame, n);
    }
    if ((u16)(s->unk20 - 7) < 0xE) {
        return func_801E7CA8(p, pos, s, frame, n);
    }
    return 0;
}

static s32 fx_12b_op81ParticleMotion0(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    s32 i = p->unk1F - 1;
    Fx12bShard *s = &D_801E996C[i];
    Fx12bPath *path = &D_801E9BA4;
    SVECTOR *ofs = &D_801EF7F4[i];

    if (s->unk20 >= 2 && s->unk20 < 7) {
        return func_801E7994(p, pos, s, path, ofs, frame, n);
    }
    return func_801E7B10(p, pos, s, frame, n);
}

static s32 fx_12b_op81ParticleMotion1(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    s32 i = p->unk1F - 1;
    Fx12bShard *s = &D_801E9A20[i];
    Fx12bPath *path = &D_801E9BAC;
    SVECTOR *ofs = &D_801EF824[i];

    if (s->unk20 >= 2 && s->unk20 < 7) {
        return func_801E7994(p, pos, s, path, ofs, frame, n);
    }
    return func_801E7CA8(p, pos, s, frame, n);
}

s32 func_801E7994(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, Fx12bPath *path, SVECTOR *ofs, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_12b_env);
    SVECTOR v;

    if (frame == 0) {
        ex->btlfx_playFileSound(0x39, 0, -1);
        ex->btlfx_playFileSound(0x3A, 0, -1);
    }
    fxm_posPathGet(path, s->unk20 - 2, &v);
    v.vx += ofs->vx;
    v.vy += ofs->vy;
    v.vz += ofs->vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    s->unk8.vx = v.vx;
    s->unk8.vy = v.vy;
    s->unk8.vz = v.vz;
    p->unk0.vx = pos->vx << 12;
    p->unk0.vy = pos->vy << 12;
    p->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 func_801E7B10(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_12b_env);
    SVECTOR v;
    s32 t = ((s->unk20 - 6) << 12) / 14;

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&s->unk8, (s16 *)&s->unk0, t, (s16 *)&v);
    t /= 2;
    v.vx += s->unk10 * ex->math_sin(t) / 4096;
    v.vy -= s->unk12 * ex->math_sin(t) / 4096;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    p->unk0.vx = pos->vx << 12;
    p->unk0.vy = pos->vy << 12;
    p->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 func_801E7CA8(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_12b_env);
    SVECTOR v;
    s32 t = ((s->unk20 - 6) << 12) / 14;

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&s->unk8, (s16 *)&s->unk0, t, (s16 *)&v);
    t /= 2;
    v.vx -= s->unk10 * ex->math_sin(t) / 4096;
    v.vy -= s->unk12 * ex->math_sin(t) / 4096;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    p->unk0.vx = pos->vx << 12;
    p->unk0.vy = pos->vy << 12;
    p->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

static s32 fx_12b_op80ParticleMotion2(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    SVECTOR v;
    SVECTOR *o = (SVECTOR *)((u8 *)D_801EF7F4 + (p->unk1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    v.vx = o->vx;
    v.vy = o->vy;
    v.vz = o->vz;
    v.vx += 0xC8;
    return fxm_pinParticle(p, pos, &v, frame, n);
}

static s32 fx_12b_op80ParticleMotion3(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    SVECTOR v;
    SVECTOR *o = (SVECTOR *)((u8 *)D_801EF824 + (p->unk1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    v.vx = o->vx;
    v.vy = o->vy;
    v.vz = o->vz;
    v.vx -= 0xC8;
    return fxm_pinParticle(p, pos, &v, frame, n);
}

static s32 fx_12b_op81ParticleMotion2(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    SVECTOR v;
    SVECTOR *o = (SVECTOR *)((u8 *)D_801EF7F4 + (p->unk1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    v.vx = o->vx;
    v.vy = o->vy;
    v.vz = o->vz;
    v.vx += 0xC8;
    return fxm_pinParticle(p, pos, &v, frame, n);
}

static s32 fx_12b_op81ParticleMotion3(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    SVECTOR v;
    SVECTOR *o = (SVECTOR *)((u8 *)D_801EF824 + (p->unk1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    v.vx = o->vx;
    v.vy = o->vy;
    v.vz = o->vz;
    v.vx -= 0xC8;
    return fxm_pinParticle(p, pos, &v, frame, n);
}

static s32 fxm_pinParticle(Fx12bParticle *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n) {
    pos->vx = src->vx;
    pos->vy = src->vy;
    pos->vz = src->vz;
    pt->unk0.vx = pos->vx << 12;
    pt->unk0.vy = pos->vy << 12;
    pt->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_12b_op80(s32 mode, Fx12bWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m1;
    MATRIX m2;
    Fx12bShard *a;
    Fx12bShard *q;
    Fx12bShard *b;
    Fx12bShard *r;
    Pool6CEntry *e;
    s32 frame;
    s32 i;
    s32 j;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_12b_env = env;
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_getUnitBodyPos(0, 0, &work->unk10.vx);
        for (i = 0, a = D_801E97BC, b = D_801E9894, q = D_801E97BC, r = D_801E9894; i < 6; i++, a++, b++, q++, r++) {
            q->unk18 = i * 4;
            r->unk18 = i * 4 + 1;
            fxm_posPathInit(&D_801E9B44[i], (u16 *)ex->btlfx_getFileData(entry->file, D_801E9AD4[i]));
            fxm_posPathInit(&D_801E9B74[i], (u16 *)ex->btlfx_getFileData(entry->file, D_801E9AE0[i]));
            a->unk0.vx = work->unk10.vx;
            q->unk0.vy = work->unk10.vy;
            q->unk0.vz = work->unk10.vz;
            b->unk0.vx = work->unk10.vx;
            r->unk0.vy = work->unk10.vy;
            r->unk0.vz = work->unk10.vz;
            if (ex->btlfx_rand() & 1) {
                a->unk0.vx += ex->btlfx_rand() % 200;
            } else {
                a->unk0.vx -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                q->unk0.vy += ex->btlfx_rand() % 200;
            } else {
                q->unk0.vy -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                q->unk0.vz += ex->btlfx_rand() % 200;
            } else {
                q->unk0.vz -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                a->unk0.vx += ex->btlfx_rand() % 200;
            } else {
                a->unk0.vx -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                r->unk0.vy += ex->btlfx_rand() % 200;
            } else {
                r->unk0.vy -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                r->unk0.vz += ex->btlfx_rand() % 200;
            } else {
                r->unk0.vz -= ex->btlfx_rand() % 200;
            }
            q->unk10 = ex->btlfx_rand() % 1500 + 200;
            q->unk12 = ex->btlfx_rand() % 800 + 400;
            q->unk14 = 0;
            r->unk10 = ex->btlfx_rand() % 1500 + 200;
            q->unk12 = ex->btlfx_rand() % 800 + 400; /* q, not r: as in the original */
            r->unk14 = 0;
        }
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x24), (s32)&fx_12b_emitPos0);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x37), (s32)&fx_12b_emitPos2);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->trails = (s32)fx_12b_op80Trails0;
                work->emit0[0]->trailSlots = 6;
                work->emit0[0]->trailStride = 0xA4;
            }
            if (work->emit0[0] != NULL) {
                ((s32 *)work->emit0[0]->file)[2] = (s32)fx_12b_op80ParticleMotion0;
            }
            if (work->emit0[2] != NULL) {
                ((s32 *)work->emit0[2]->file)[2] = (s32)fx_12b_op80ParticleMotion2;
            }
            break;
        case 1:
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x26), (s32)&fx_12b_emitPos1);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x38), (s32)&fx_12b_emitPos2);
            if (work->emit0[1] != NULL) {
                work->emit0[1]->trails = (s32)fx_12b_op80Trails1;
                work->emit0[1]->trailSlots = 6;
                work->emit0[1]->trailStride = 0xA4;
            }
            if (work->emit0[1] != NULL) {
                ((s32 *)work->emit0[1]->file)[2] = (s32)fx_12b_op80ParticleMotion1;
            }
            if (work->emit0[3] != NULL) {
                ((s32 *)work->emit0[3]->file)[2] = (s32)fx_12b_op80ParticleMotion3;
            }
            break;
        }
        for (q = D_801E97BC, r = D_801E9894, j = 0; j < 6; j++, q++, r++) {
            if (frame == q->unk18) {
                ex->btlfx_getUnitBoneMatrix(0x10, q->unk1A, (BtlBone *)&m1);
                ex->btlfx_matrixPointAhead(&m1, q->unk1C, (s32)&D_801EF7F4[j]);
            }
            if (frame == r->unk18) {
                ex->btlfx_getUnitBoneMatrix(0x10, r->unk1A, (BtlBone *)&m2);
                ex->btlfx_matrixPointAhead(&m2, r->unk1C, (s32)&D_801EF824[j]);
            }
            if (frame >= q->unk18) {
                if (++q->unk20 == 0x13) {
                    e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)q);
                    if (e != NULL) {
                        e->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                    }
                }
            }
            if (frame >= r->unk18) {
                if (++r->unk20 == 0x13) {
                    e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)r);
                    if (e != NULL) {
                        e->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                    }
                    /* the do-while(0) adds a loop-depth-weighted ref to j, which puts it ahead of q and r in
                     * global-alloc (j in s4) */
                    do {
                        if (j == 5) {
                            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
                        }
                    } while (0);
                }
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_12b_op81(s32 mode, Fx12bWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m1;
    MATRIX m2;
    Fx12bShard *a;
    Fx12bShard *q;
    Fx12bShard *b;
    Fx12bShard *r;
    s32 frame;
    s32 i;
    s32 j;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_12b_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, &work->unk10.vx);
        fxm_posPathInit(&D_801E9BA4, (u16 *)ex->btlfx_getFileData(entry->file, 0x22));
        fxm_posPathInit(&D_801E9BAC, (u16 *)ex->btlfx_getFileData(entry->file, 0x21));
        for (i = 0, a = D_801E996C, b = D_801E9A20, q = D_801E996C, r = D_801E9A20; i < 5; i++, a++, b++, q++, r++) {
            a->unk0.vx = work->unk10.vx;
            q->unk0.vy = work->unk10.vy;
            q->unk0.vz = work->unk10.vz;
            b->unk0.vx = work->unk10.vx;
            r->unk0.vy = work->unk10.vy;
            r->unk0.vz = work->unk10.vz;
            if (ex->btlfx_rand() & 1) {
                a->unk0.vx += ex->btlfx_rand() % 200;
            } else {
                a->unk0.vx -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                q->unk0.vy += ex->btlfx_rand() % 200;
            } else {
                q->unk0.vy -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                q->unk0.vz += ex->btlfx_rand() % 200;
            } else {
                q->unk0.vz -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                a->unk0.vx += ex->btlfx_rand() % 200;
            } else {
                a->unk0.vx -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                r->unk0.vy += ex->btlfx_rand() % 200;
            } else {
                r->unk0.vy -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                r->unk0.vz += ex->btlfx_rand() % 200;
            } else {
                r->unk0.vz -= ex->btlfx_rand() % 200;
            }
            q->unk10 = ex->btlfx_rand() % 1500 + 200;
            q->unk12 = ex->btlfx_rand() % 800 + 400;
            q->unk14 = 0;
            r->unk10 = ex->btlfx_rand() % 1500 + 200;
            r->unk12 = ex->btlfx_rand() % 800 + 400;
            r->unk14 = 0;
        }
        return 0;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x25), (s32)&fx_12b_emitPos0);
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x27), (s32)&fx_12b_emitPos1);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x28), (s32)&fx_12b_emitPos2);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x29), (s32)&fx_12b_emitPos2);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->trails = (s32)fx_12b_op81Trails0;
                work->emit0[0]->trailSlots = 6;
                work->emit0[0]->trailStride = 0xA4;
            }
            if (work->emit0[1] != NULL) {
                work->emit0[1]->trails = (s32)fx_12b_op81Trails1;
                work->emit0[1]->trailSlots = 6;
                work->emit0[1]->trailStride = 0xA4;
            }
            if (work->emit0[0] != NULL) {
                ((s32 *)work->emit0[0]->file)[2] = (s32)fx_12b_op81ParticleMotion0;
            }
            if (work->emit0[1] != NULL) {
                ((s32 *)work->emit0[1]->file)[2] = (s32)fx_12b_op81ParticleMotion1;
            }
            if (work->emit0[2] != NULL) {
                ((s32 *)work->emit0[2]->file)[2] = (s32)fx_12b_op81ParticleMotion2;
            }
            if (work->emit0[3] != NULL) {
                ((s32 *)work->emit0[3]->file)[2] = (s32)fx_12b_op81ParticleMotion3;
            }
        }
        for (j = 0, r = D_801E9A20, q = D_801E996C; j < 5; j++, q++, r++) {
            if (frame == q->unk18) {
                ex->btlfx_getUnitBoneMatrix(0x10, q->unk1A, (BtlBone *)&m1);
                ex->btlfx_matrixPointAhead(&m1, q->unk1C, (s32)&D_801EF7F4[j]);
            }
            if (frame == r->unk18) {
                ex->btlfx_getUnitBoneMatrix(0x10, r->unk1A, (BtlBone *)&m2);
                ex->btlfx_matrixPointAhead(&m2, r->unk1C, (s32)&D_801EF824[j]);
            }
            if (frame >= q->unk18) {
                q->unk20++;
            }
            if (frame >= r->unk18) {
                r->unk20++;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_12b_op82(s32 mode, Fx12bWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        fx_12b_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos1C);
        work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos14);
        work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos14);
        work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos14);
        work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos14);
        work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x23), (s32)&work->pos14);
        if (work->emit0[0] != NULL) {
            work->emit0[0]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->emit0[1] != NULL) {
            work->emit0[1]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->emit0[2] != NULL) {
            work->emit0[2]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_12b_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posPathInit(Fx12bPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx12bPoint *)data;
}

static s32 fxm_posPathGet(Fx12bPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx12bPoint *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v) {
    env->state->signals[i] = v;
}

INCLUDE_RODATA("asm/us/fx_12b/nonmatchings/fx_12b", fx_12b_headerPtr);
