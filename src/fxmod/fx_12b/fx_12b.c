#include "fxmod/fx_12b.h"

s32 func_801E7704(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
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

s32 func_801E77B0(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
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

s32 func_801E785C(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    s32 i = p->unk1F - 1;
    Fx12bShard *s = &D_801E996C[i];
    Fx12bPath *path = &D_801E9BA4;
    SVECTOR *ofs = &D_801EF7F4[i];

    if (s->unk20 >= 2 && s->unk20 < 7) {
        return func_801E7994(p, pos, s, path, ofs, frame, n);
    }
    return func_801E7B10(p, pos, s, frame, n);
}

s32 func_801E78F8(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
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
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9B34);
    SVECTOR v;

    if (frame == 0) {
        ex->func_800E9930(0x39, 0, -1);
        ex->func_800E9930(0x3A, 0, -1);
    }
    func_801E9744(path, s->unk20 - 2, &v);
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
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9B34);
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
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9B34);
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

s32 func_801E7E40(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    SVECTOR v;
    SVECTOR *o = (SVECTOR *)((u8 *)D_801EF7F4 + (p->unk1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    v.vx = o->vx;
    v.vy = o->vy;
    v.vz = o->vz;
    v.vx += 0xC8;
    return func_801E8000(p, pos, &v, frame, n);
}

s32 func_801E7EB0(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    SVECTOR v;
    SVECTOR *o = (SVECTOR *)((u8 *)D_801EF824 + (p->unk1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    v.vx = o->vx;
    v.vy = o->vy;
    v.vz = o->vz;
    v.vx -= 0xC8;
    return func_801E8000(p, pos, &v, frame, n);
}

s32 func_801E7F20(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    SVECTOR v;
    SVECTOR *o = (SVECTOR *)((u8 *)D_801EF7F4 + (p->unk1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    v.vx = o->vx;
    v.vy = o->vy;
    v.vz = o->vz;
    v.vx += 0xC8;
    return func_801E8000(p, pos, &v, frame, n);
}

s32 func_801E7F90(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    SVECTOR v;
    SVECTOR *o = (SVECTOR *)((u8 *)D_801EF824 + (p->unk1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    v.vx = o->vx;
    v.vy = o->vy;
    v.vz = o->vz;
    v.vx -= 0xC8;
    return func_801E8000(p, pos, &v, frame, n);
}

s32 func_801E8000(Fx12bParticle *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n) {
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

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", fx_12b_op80);

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
        D_801E9B34 = env;
        ex->btlfx_getUnitBodyPos(0, 0, &work->unk10.vx);
        func_801E972C(&D_801E9BA4, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x22));
        func_801E972C(&D_801E9BAC, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x21));
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
        frame = entry->unk0;
        if (frame == 0) {
            work->unk0[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x25), (s32)&D_801EF854);
            work->unk0[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x27), (s32)&D_801EF85C);
            work->unk0[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x28), (s32)&D_801EF864);
            work->unk0[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x29), (s32)&D_801EF864);
            if (work->unk0[0] != NULL) {
                work->unk0[0]->unk14 = (s32)D_801EC9D4;
                work->unk0[0]->unk12 = 6;
                work->unk0[0]->unkC = 0xA4;
            }
            if (work->unk0[1] != NULL) {
                work->unk0[1]->unk14 = (s32)D_801EE0E4;
                work->unk0[1]->unk12 = 6;
                work->unk0[1]->unkC = 0xA4;
            }
            if (work->unk0[0] != NULL) {
                ((s32 *)work->unk0[0]->unk0)[2] = (s32)func_801E785C;
            }
            if (work->unk0[1] != NULL) {
                ((s32 *)work->unk0[1]->unk0)[2] = (s32)func_801E78F8;
            }
            if (work->unk0[2] != NULL) {
                ((s32 *)work->unk0[2]->unk0)[2] = (s32)func_801E7F20;
            }
            if (work->unk0[3] != NULL) {
                ((s32 *)work->unk0[3]->unk0)[2] = (s32)func_801E7F90;
            }
        }
        for (j = 0, r = D_801E9A20, q = D_801E996C; j < 5; j++, q++, r++) {
            if (frame == q->unk18) {
                ex->func_800DC8AC(0x10, q->unk1A, (BtlBone *)&m1);
                ex->func_800CFE88(&m1, q->unk1C, (s32)&D_801EF7F4[j]);
            }
            if (frame == r->unk18) {
                ex->func_800DC8AC(0x10, r->unk1A, (BtlBone *)&m2);
                ex->func_800CFE88(&m2, r->unk1C, (s32)&D_801EF824[j]);
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
        D_801E9B34 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk1C);
        work->unk0[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk14);
        work->unk0[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk14);
        work->unk0[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk14);
        work->unk0[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk14);
        work->unk0[4] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x23), (s32)&work->unk14);
        if (work->unk0[0] != NULL) {
            work->unk0[0]->unk22 = ex->func_800E7CC4(0, 0x80);
        }
        if (work->unk0[1] != NULL) {
            work->unk0[1]->unk22 = ex->func_800E7CC4(0, 0x80);
        }
        if (work->unk0[2] != NULL) {
            work->unk0[2]->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

void func_801E9690(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E9B34)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E96D4(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E972C(Fx12bPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx12bPoint *)data;
}

s32 func_801E9744(Fx12bPath *path, s32 t, SVECTOR *out) {
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

void func_801E97A8(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->unk0 + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_12b/nonmatchings/fx_12b", fx_12b_headerPtr);
