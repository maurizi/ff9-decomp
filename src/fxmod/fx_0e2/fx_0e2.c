#include "fxmod/fx_0e2.h"

void func_801E77C0(void *trackA, void *trackB, s32 t, Fx0e2Trail *trail, s32 size, s32 pad) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801ECC0C);
    SVECTOR rot;
    SVECTOR pos;
    SVECTOR rotA;
    SVECTOR rotB;
    SVECTOR posA;
    SVECTOR posB;
    SVECTOR *pt;
    s32 n;
    s32 i;
    s32 r;

    n = (size - 4) / 8;
    pt = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    r = t;
    for (i = 0; i < n; i++, pt++) {
        func_801EC520(trackA, i, &rotA, &posA);
        func_801EC520(trackB, i, &rotB, &posB);
        ex->btlfx_lerpVec((s16 *)&posA, (s16 *)&posB, r, (s16 *)&pos);
        ex->btlfx_lerpVec((s16 *)&rotA, (s16 *)&rotB, r, (s16 *)&rot);
        pos.vx /= 2;
        pos.vy /= 2;
        pos.vz /= 2;
        pos.vx += D_801F0FEC.vx;
        pos.vy += D_801F0FEC.vy;
        pos.vz += D_801F0FEC.vz;
        pos.vx += 0x50;
        pos.vy -= 0x109;
        pos.vz += 0xE6;
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        if (i == n - 1) {
            D_801F0FF4.vx = pos.vx;
            D_801F0FF4.vy = pos.vy;
            D_801F0FF4.vz = pos.vz;
            D_801F0FFC.vx = rot.vx;
            D_801F0FFC.vy = rot.vy;
            D_801F0FFC.vz = rot.vz;
            D_801F0FFC.vy += 0x800;
        }
        pt->pad = pad;
    }
}

void func_801E7A0C(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 pad) {
    SVECTOR rot;
    SVECTOR pos;
    SVECTOR *pt;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    pt = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    for (i = 0; i < n; i++, pt++) {
        func_801EC520(track, i + base, &rot, &pos);
        pos.vx /= 2;
        pos.vy /= 2;
        pos.vz /= 2;
        pos.vx += D_801F0FEC.vx;
        pos.vy += D_801F0FEC.vy;
        pos.vz += D_801F0FEC.vz;
        pos.vx += 0x50;
        pos.vy -= 0x109;
        pos.vz += 0xE6;
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        if (i == n - 1) {
            D_801F0FF4.vx = pos.vx;
            D_801F0FF4.vy = pos.vy;
            D_801F0FF4.vz = pos.vz;
            D_801F0FFC.vx = rot.vx;
            D_801F0FFC.vy = rot.vy;
            D_801F0FFC.vz = rot.vz;
            D_801F0FFC.vy += 0x800;
        }
        pt->pad = pad;
    }
}

void func_801E7BE8(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad) {
    s32 k;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801ECC0C);
    SVECTOR pos;
    SVECTOR *pt;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    pt = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    if (count < n) {
        for (i = 0; i < n - count; i++, pt++) {
            pt->pad = 0;
        }
    } else {
        count = n;
    }
    for (i = 0; i < count; i++, pt++) {
        k = i + 1;
        func_801EC4A4(track, count - k + base, &pos);
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        if (i == 0) {
            D_801F0FF4.vx = pos.vx;
            D_801F0FF4.vy = pos.vy;
            D_801F0FF4.vz = pos.vz;
            ex->btlfx_aimAngles((s16 *)&D_801F0FF4, (s16 *)&D_801F1004, (s16 *)&D_801F0FFC);
        }
        pt->pad = pad;
    }
}

void func_801E7D5C(void *trackA, void *trackB, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad, s32 t,
                   s32 rotY) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801ECC0C);
    MATRIX m;
    VECTOR out;
    SVECTOR rot;
    SVECTOR a;
    SVECTOR b;
    SVECTOR p;
    SVECTOR *pt;
    s32 n;
    s32 i;
    s32 k;

    n = (size - 4) / 8;
    pt = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    if (count < n) {
        for (i = 0; i < n - count; i++, pt++) {
            pt->pad = 0;
        }
    } else {
        count = n;
    }
    for (i = 0; i < count;) {
        i++;
        k = count - i + base;
        func_801EC520(trackA, k, NULL, &a);
        func_801EC520(trackB, k, NULL, &b);
        ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, t, (s16 *)&p);
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = rotY;
        p.vx *= 6;
        p.vy *= 3;
        p.vz *= 6;
        ex->func_800316D8(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(&out);
        out.vx += D_801F1004.vx;
        out.vy += D_801F1004.vy;
        out.vz += D_801F1004.vz;
        pt->vx = out.vx;
        pt->vy = out.vy;
        pt->vz = out.vz;
        pt->pad = pad;
        pt++;
    }
}

void func_801E7FBC(void *trackA, void *trackB, s32 arg2, s32 arg3, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist) {
    Unk800F78A4 *env = D_801ECC0C;
    Fx0e2Edge *e = D_801EE62C;
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
        func_801EC4A4(trackA, i, &a);
        func_801EC4A4(trackB, i, &b);
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

void func_801E83B0(void *track, s32 arg1, s32 arg2, SVECTOR *scale, s32 w, s32 rotY) {
    Unk800F78A4 *env = D_801ECC0C;
    Fx0e2Edge *e = D_801EE62C;
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
        func_801EC4A4(track, i, &p);
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

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", func_801E86D0);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_op80);

INCLUDE_RODATA("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_headerPtr);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_op81);

INCLUDE_ASM("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_op82);

s32 fx_0e2_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR d;
    Fx0e2Orbit *o;
    s32 i;
    s32 j;
    Fx0e2Orbit *q;
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        o = D_801EE8FC;
        work->vx = 0;
        work->vy = 0;
        work->vz = 0x4B0;
        D_801ECC0C = env;
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
            for (j = 0, q = D_801EE8FC; j < env->unkC->unk24; j++, q++) {
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

s32 fx_0e2_op84(s32 mode, Fx0e2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    Fx0e2Orbit *o;

    if (mode == 0) {
        *arg4 = 0xD0;
    } else if (mode == 1) {
        o = D_801EE8FC;
        D_801ECC0C = env;
        for (i = 0; i < env->unkC->unk24; i++, o++) {
            work->unkC0[i] = 0;
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk40[i]);
            work->unk40[i].vy = -0xBB8;
            ex->btlfx_setUnitPos(i, (s16 *)&work->unk40[i]);
            ex->btlfx_setUnitRot(i, (s16 *)&o->unk8);
        }
    } else {
        frame = entry->unk0;
        for (j = 0; j < env->unkC->unk24; j++) {
            if (work->unkC0[j] == 0 && func_801EC64C(j, frame, 4, &work->unk40[j]) != 0) {
                work->unkC0[j] = 1;
                ex->btlfx_getUnitHomePos(j, (s16 *)&work->unk40[j]);
                ex->btlfx_setUnitPos(j, (s16 *)&work->unk40[j]);
                work->unk80[j].vx = work->unk40[j].vx;
                (work->unk80 + j)->vy = (work->unk40 + j)->vy;
                (work->unk80 + j)->vz = (work->unk40 + j)->vz;
                work->unk0[j] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk80[j]);
                work->unk20[j] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk80[j]);
                if (work->unk0[j] != NULL) {
                    work->unk0[j]->unk14 = (s32)D_801EFDEC[j];
                    work->unk0[j]->unk12 = 0xC;
                    work->unk0[j]->unkC = 0x30;
                }
                if (work->unk20[j] != NULL) {
                    work->unk20[j]->unk22 = ex->func_800E7CC4(j, 0x80);
                }
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_0e2_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 2;
    } else if (mode == 1) {
        D_801ECC0C = env;
        *work = -1;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            *work = 0x10;
        }
        if (*work >= 0) {
            func_801EC5B4(0xFF, 0, 0x10 - *work, 0x10, 1, 0);
            (*work)--;
        }
        return frame >= 0x10;
    }
    return 0;
}

s32 func_801EC3D8(u8 *base, u16 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

void func_801EC3F0(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801ECC0C)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801EC434(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801EC48C(Fx0e2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0e2Point *)data;
}

s32 func_801EC4A4(Fx0e2Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx0e2Point *p;

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

void func_801EC508(Fx0e2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0e2Point *)data;
}

s32 func_801EC520(Fx0e2Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx0e2Key *k;
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

void func_801EC5B4(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801ECC0C)->btlfx_drawScreenTint((u32 *)(D_801ECC0C->unk20 + ot), arg4, c, c, c);
}

s32 func_801EC64C(s32 a0, s32 t, s32 n, SVECTOR *b) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801ECC0C);
    SVECTOR a;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(a0, (s16 *)&a);
    func_801EC3F0(b, &a, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(a0, (s16 *)&out);
    return t >= n;
}

void func_801EC718(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801F10DC[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801F14DC[i] = 0;
    }
}

void func_801EC764(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (D_801F10DC[i].unk6 == -1) {
            D_801F10DC[i].unk6 = 0;
            D_801F10DC[i].unk0 = data;
            D_801F10DC[i].unk4 = start;
            D_801F10DC[i].unkA = dst;
            D_801F10DC[i].unkC = 0;
            D_801F10DC[i].unkE = c;
            D_801F10DC[i].unk8 = ch;
            return;
        }
    }
}

void func_801EC7C0(s32 ch) {
    Unk800F78A4 *env = D_801ECC0C;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (D_801F10DC[i].unk8 == ch) {
            if (D_801F10DC[i].unk6 == 0 && D_801F10DC[i].unk4 == D_801F14DC[ch]) {
                D_801F10DC[i].unk6 = *D_801F10DC[i].unk0;
                D_801F10DC[i].unk0++;
            }
            if (D_801F10DC[i].unk6 > 0) {
                D_801F10DC[i].unk6--;
                for (k = 0; k < 3; k++) {
                    d = ((D_801F10DC[i].unkA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->unk2C)[d].vx = D_801F10DC[i].unk0[0];
                        ((SVECTOR *)env->unk2C)[d].vy = D_801F10DC[i].unk0[1];
                        ((SVECTOR *)env->unk2C)[d].vz = D_801F10DC[i].unk0[2];
                        if (D_801F10DC[i].unkC < D_801F10DC[i].unkE) {
                            D_801F10DC[i].unk0 += 3;
                            D_801F10DC[i].unkC++;
                        }
                    }
                }
            }
        }
    }
    D_801F14DC[ch]++;
}

void func_801EC920(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801ECC0C;
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
