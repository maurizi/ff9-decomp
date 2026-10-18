#include "fxmod/fx_1fd.h"

void func_801E7704(void *trackA, void *trackB, s32 arg2, s32 arg3, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist) {
    Unk800F78A4 *env = D_801E9E4C;
    Fx1fdEdge *e = D_801EB80C;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR v;
    SVECTOR ends[2];
    SVECTOR rot;
    SVECTOR a;
    SVECTOR b;
    SVECTOR p;
    s32 i;
    s16 z;

    ends[0].vx = 0;
    ends[0].vy = -w / 2;
    ends[0].vz = 0;
    ends[1].vx = 0;
    ends[1].vy = w / 2;
    ends[1].vz = 0;
    for (i = 0; i < 60; i++, e++) {
        func_801E97DC(trackA, i, &a);
        func_801E97DC(trackB, i, &b);
        ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, t, (s16 *)&p);
        p.vx = p.vx * scale->vx / 4096;
        p.vy = p.vy * scale->vy / 4096;
        p.vz = p.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = rotY;
        ex->func_800316D8(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(&v);
        p.vx = v.vx;
        p.vy = v.vy;
        p.vz = v.vz + 0x4B0;
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        if (twist != 0) {
            if (i < 0x10) {
                rot.vz = 0x400 - (i << 6);
            } else if (i >= 0x1D) {
                rot.vz = ex->math_sin(((i - 0x1C) << 7) / 4) / 4 + 0x80;
                z = rot.vz;
                if (z > 0x400) {
                    z = 0x400;
                }
                rot.vz = z;
                if (p.vz > 0) {
                    rot.vz = -z;
                }
            }
        } else {
            rot.vz = 0x400;
        }
        ex->func_800316D8(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&ends[0]);
        gte_rtps();
        gte_stsxy(&e->unk0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&e->unk4);
        gte_stszotz(&e->unk8);
    }
}

void func_801E7AF8(void *track, s32 arg1, s32 arg2, SVECTOR *scale, s32 w, s32 rotY) {
    Unk800F78A4 *env = D_801E9E4C;
    Fx1fdEdge *e = D_801EB80C;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR v;
    SVECTOR ends[2];
    SVECTOR rot;
    SVECTOR p;
    s32 i;

    ends[0].vx = 0;
    ends[0].vy = -w / 2;
    ends[0].vz = 0;
    ends[1].vx = 0;
    ends[1].vy = w / 2;
    ends[1].vz = 0;
    for (i = 0; i < 60; i++, e++) {
        func_801E97DC(track, i, &p);
        p.vx = p.vx * scale->vx / 4096;
        p.vy = p.vy * scale->vy / 4096;
        p.vz = p.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vy = rotY;
        rot.vz = 0;
        ex->func_800316D8(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(&v);
        p.vx = v.vx;
        p.vy = v.vy;
        p.vz = v.vz + 0x4B0;
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0x400;
        ex->func_800316D8(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&ends[0]);
        gte_rtps();
        gte_stsxy(&e->unk0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&e->unk4);
        gte_stszotz(&e->unk8);
    }
}

void func_801E7E18(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1) {
    Fx1fdEdge *e = D_801EB80C;
    POLY_GT4 *p;
    BtlfxExports *ex;
    u16 tp;
    u16 cl;
    s32 i;
    s32 k;
    s32 ua;
    s32 ub;
    s32 ca;
    s32 cb;
    s32 z;

    tp = tpage | 0x20;
    p = (POLY_GT4 *)D_801E9E4C->unk24;
    D_801E9E4C->unk24 = (u8 *)(p + n);
    cl = clut;
    ex = BTLFX_EXPORTS(D_801E9E4C);
    for (i = 0; i < n - 1; i++, p++, e++) {
        p->tpage = tp;
        p->clut = cl;
        p->code = 0x3E;
        p->x0 = e[0].unk0.vx;
        p->y0 = e[0].unk0.vy;
        p->x1 = e[0].unk4.vx;
        p->y1 = e[0].unk4.vy;
        p->x2 = e[1].unk0.vx;
        p->y2 = e[1].unk0.vy;
        p->x3 = e[1].unk4.vx;
        p->y3 = e[1].unk4.vy;
        p->v0 = p->v2 = v0;
        p->v1 = p->v3 = v1;
        k = i % 8;
        ua = k << 5;
        ub = (k + 1) << 5;
        if (ua == 0x100) ua = 0xFF;
        if (ub == 0x100) ub = 0xFF;
        p->u2 = p->u3 = ub;
        p->u0 = p->u1 = ua;
        if (i < 8) {
            ca = i * 16;
            cb = (i + 1) * 16;
            if (c < ca) {
                ca = c;
            }
            if (c < cb) {
                cb = c;
            }
            setRGB0(p, ca, ca, ca);
            setRGB1(p, ca, ca, ca);
            setRGB2(p, cb, cb, cb);
            setRGB3(p, cb, cb, cb);
        } else if (i >= 0x35) {
            cb = 0x80 - (i - 0x34) * 16;
            ca = 0x80 - (i - 0x33) * 16;
            if (c < cb) {
                cb = c;
            }
            if (c < ca) {
                ca = c;
            }
            setRGB0(p, cb, cb, cb);
            setRGB1(p, cb, cb, cb);
            setRGB2(p, ca, ca, ca);
            setRGB3(p, ca, ca, ca);
        } else {
            setRGB0(p, c, c, c);
            setRGB1(p, c, c, c);
            setRGB2(p, c, c, c);
            setRGB3(p, c, c, c);
        }
        if (e[0].unk0.vx < e[1].unk0.vx) {
            ex->btlfx_addPrim(0x0C000000, 0, p);
        } else {
            ex->btlfx_addPrim(0x0C000000, (e[0].unk8 + e[1].unk8) >> 1, p);
        }
    }
}

s32 fx_1fd_op80(s32 mode, Fx1fdWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 i;
    Pool6CEntry **q;
    s32 j;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        D_801E9E4C = env;
        ex->btlfx_initEffModels(D_801E9E5C, 0x20);
        for (i = 0; i < 5; i++) {
            func_801E97C4(&D_801EBB7C[i], func_801E9710((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), i));
        }
        func_801E9A50();
        func_801E9A9C(0, 1, 0, func_801E9710((u8 *)ex->btlfx_getFileData(entry->unk8, 0xE), 0));
        func_801E9A9C(0, 2, 0, func_801E9710((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0));
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            q = work->unk0;
            for (j = 0; j < 5; j++) {
                *q = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801E9DF8[j]), (s32)&D_801ED70C[j]);
                if (*q != NULL) {
                    (*q)->unk14 = (s32)D_801EBBAC[j];
                    (*q)->unk12 = 6;
                    (*q)->unkC = 0x50;
                }
                q++;
            }
        }
        if (frame >= 0) {
            t = frame;
            for (i = 0; i < 5; i++) {
                func_801E97DC(&D_801EBB7C[i], t, &work->unk14[i]);
                if (work->unk0[i] != NULL && work->unk0[i]->unk30 == -1) {
                    work->unk0[i] = NULL;
                }
                ex->func_800E3D20((s32)work->unk0[i], (s32)&work->unk14[i]);
            }
        }
        func_801E9AF8(0);
        return frame >= 0x78;
    }
    return 0;
}

s32 fx_1fd_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E9E4C = env;
    } else {
        return entry->unk0 > 0;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op82);

s32 fx_1fd_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR d;
    Fx1fdOrbit *o;
    s32 i;
    s32 j;
    Fx1fdOrbit *q;
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        o = D_801EBADC;
        work->vx = 0;
        work->vy = 0;
        work->vz = 0x4B0;
        D_801E9E4C = env;
        for (i = 0; i < env->unkC->unk24; i++, o++) {
            ex->btlfx_getUnitPos(i, (s16 *)&o->unk0);
            ex->btlfx_getUnitRot(i, (s16 *)&o->unk8);
            ex->btlfx_aimAngles((s16 *)work, (s16 *)&o->unk0, (s16 *)&a);
            o->unk12 = a.vy;
            d.vx = work->vx - o->unk0.vx;
            d.vy = work->vy - o->unk0.vy;
            d.vz = work->vz - o->unk0.vz;
            o->unk10 = ex->btlfx_sqrt(d.vx * d.vx + d.vz * d.vz, 0x14);
        }
    } else {
        frame = entry->unk0;
        if (frame >= 0 && frame <= 0x40) {
            for (j = 0, q = D_801EBADC; j < env->unkC->unk24; j++, q++) {
                a.vx = work->vx + q->unk10 * ex->math_sin(q->unk12) / 4096;
                a.vy = q->unk0.vy - frame * 120;
                a.vz = work->vz + q->unk10 * ex->math_cos(q->unk12) / 4096;
                ex->btlfx_setUnitPos(j, (s16 *)&a);
                ex->btlfx_aimAngles((s16 *)work, (s16 *)&a, (s16 *)&d);
                d.vx = 0;
                d.vz = 0;
                ex->btlfx_setUnitRot(j, (s16 *)&d);
                q->unk12 += 0x100;
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

s32 fx_1fd_op84(s32 mode, Fx1fdWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *a;
    SVECTOR *b;
    s32 frame;
    s32 i;
    s32 j;
    Fx1fdOrbit *q;

    if (mode == 0) {
        *arg4 = 0xD0;
    } else if (mode == 1) {
        q = D_801EBADC;
        D_801E9E4C = env;
        for (j = 0; j < env->unkC->unk24; j++, q++) {
            work->unkC0[j] = 0;
            ex->btlfx_getUnitHomePos(j, (s16 *)&work->unk40[j]);
            work->unk40[j].vy = -0xBB8;
            ex->btlfx_setUnitPos(j, (s16 *)&work->unk40[j]);
            ex->btlfx_setUnitRot(j, (s16 *)&q->unk8);
        }
    } else {
        frame = entry->unk0;
        for (i = 0; i < env->unkC->unk24; i++) {
            if (work->unkC0[i] == 0 && func_801E9984(i, frame, 4, &work->unk40[i]) != 0) {
                work->unkC0[i] = 1;
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk40[i]);
                ex->btlfx_setUnitPos(i, (s16 *)&work->unk40[i]);
                a = (SVECTOR *)((u8 *)work + i * 8 + 0x40);
                b = (SVECTOR *)((u8 *)work + i * 8 + 0x80);
                b->vx = a->vx;
                b->vy = a->vy;
                b->vz = a->vz;
                work->unk0[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk80[i]);
                work->unk20[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk80[i]);
                if (work->unk0[i] != NULL) {
                    work->unk0[i]->unk14 = (s32)D_801EC50C[i];
                    work->unk0[i]->unk12 = 0xC;
                    work->unk0[i]->unkC = 0x30;
                }
                if (work->unk20[i] != NULL) {
                    work->unk20[i]->unk22 = ex->func_800E7CC4(i, 0x80);
                }
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

u16 *func_801E9710(u8 *base, u16 i) {
    return (u16 *)((s32)base + ((s32 *)base)[(u16)i + 1]);
}

void func_801E9728(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E9E4C)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E976C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E97C4(Fx1fdPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fdPoint *)data;
}

s32 func_801E97DC(Fx1fdPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1fdPoint *p;

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

void func_801E9840(Fx1fdPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fdPoint *)data;
}

s32 func_801E9858(Fx1fdPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx1fdKey *k;
    s32 r = 1;

    if (i > path->unk2 - 1) {
        i = path->unk2 - 1;
    } else {
        r = 0;
    }
    k = &path->unk4[i];
    if (rot != NULL) {
        rot->vx = k->unk6[0];
        rot->vy = k->unk6[1];
        rot->vz = k->unk6[2];
    }
    if (pos != NULL) {
        pos->vx = k->unk0[0];
        pos->vy = k->unk0[1];
        pos->vz = k->unk0[2];
    }
    return r;
}

void func_801E98EC(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E9E4C)->btlfx_drawScreenTint((u32 *)(D_801E9E4C->unk20 + ot), arg4, c, c, c);
}

s32 func_801E9984(s32 a0, s32 t, s32 n, SVECTOR *b) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9E4C);
    SVECTOR a;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(a0, (s16 *)&a);
    func_801E9728(b, &a, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(a0, (s16 *)&out);
    return t >= n;
}

void func_801E9A50(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801ED73C[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801EDB3C[i] = 0;
    }
}

void func_801E9A9C(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (D_801ED73C[i].unk6 == -1) {
            D_801ED73C[i].unk6 = 0;
            D_801ED73C[i].unk0 = data;
            D_801ED73C[i].unk4 = start;
            D_801ED73C[i].unkA = dst;
            D_801ED73C[i].unkC = 0;
            D_801ED73C[i].unkE = c;
            D_801ED73C[i].unk8 = ch;
            return;
        }
    }
}

void func_801E9AF8(s32 ch) {
    Unk800F78A4 *env = D_801E9E4C;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (D_801ED73C[i].unk8 == ch) {
            if (D_801ED73C[i].unk6 == 0 && D_801ED73C[i].unk4 == D_801EDB3C[ch]) {
                D_801ED73C[i].unk6 = *D_801ED73C[i].unk0;
                D_801ED73C[i].unk0++;
            }
            if (D_801ED73C[i].unk6 > 0) {
                D_801ED73C[i].unk6--;
                for (k = 0; k < 3; k++) {
                    d = ((D_801ED73C[i].unkA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->unk2C)[d].vx = D_801ED73C[i].unk0[0];
                        ((SVECTOR *)env->unk2C)[d].vy = D_801ED73C[i].unk0[1];
                        ((SVECTOR *)env->unk2C)[d].vz = D_801ED73C[i].unk0[2];
                        if (D_801ED73C[i].unkC < D_801ED73C[i].unkE) {
                            D_801ED73C[i].unk0 += 3;
                            D_801ED73C[i].unkC++;
                        }
                    }
                }
            }
        }
    }
    D_801EDB3C[ch]++;
}

void func_801E9C58(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801E9E4C;
    u32 *p = (u32 *)env->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->unk24 = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
}

INCLUDE_RODATA("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_headerPtr);
