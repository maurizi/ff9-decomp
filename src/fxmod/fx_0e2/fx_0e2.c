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
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
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
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
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

void func_801E86D0(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1) {
    Fx0e2Edge *e = D_801EE62C;
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
    p = (POLY_GT4 *)D_801ECC0C->prim;
    D_801ECC0C->prim = (u8 *)(p + n);
    cl = clut;
    ex = BTLFX_EXPORTS(D_801ECC0C);
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

s32 fx_0e2_op80(s32 mode, Fx0e2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    Pool6CEntry *p2;
    Pool6CEntry *p3;
    s32 n;
    s32 m;
    Fx0e2Drop *dr;
    Fx0e2Drop *dm;
    s32 frame;
    s32 i0;
    s32 i2;
    s32 i5;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 j;
    s32 k;
    s32 a;
    s32 x;
    s32 b;
    s32 t;
    s32 c;
    s32 f;

    if (mode == 0) {
        *arg4 = 0x16C;
    } else if (mode == 1) {
        D_801ECC0C = env;
        ex->btlfx_initEffModels(D_801ECC1C, 0x20);
        D_801F1004.vx = env->targets->centerX;
        D_801F1004.vy = env->targets->centerY;
        D_801F1004.vz = env->targets->centerZ;
        work->unk3C.vx = 0;
        work->unk3C.vy = 0x800;
        work->unk3C.vz = 0;
        work->unk44.vx = 0;
        work->unk44.vy = 0;
        work->unk44.vz = 0;
        work->unk54.vx = 0;
        work->unk54.vy = 0;
        work->unk54.vz = 0;
        work->unk64.vx = 0x800;
        work->unk64.vy = 0x800;
        work->unk64.vz = 0x800;
        work->unk130 = 0;
        work->unk134 = 0;
        work->unk150[0] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x84), D_801ECBA4, D_801ECBB4);
        work->unk150[1] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x85), D_801ECBA4, D_801ECBB4);
        work->unk150[2] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x83), D_801ECBA4, D_801ECBB4);
        work->unk150[3] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x82), D_801ECBA4, D_801ECBB4);
        work->unk138 = 0;
        work->unk13C = 0;
        work->unk140 = -1;
        work->unk144 = -1;
        work->unk148 = -1;
        work->unk7C.vx = 0;
        work->unk7C.vy = 0;
        work->unk7C.vz = 0;
        work->unk84.vx = 0;
        work->unk84.vy = 0;
        work->unk84.vz = 0;
        work->unk94.vx = 0x1000;
        work->unk94.vy = 0x1000;
        work->unk94.vz = 0x1000;
        work->unkA4.vx = 0x1000;
        work->unkA4.vy = 0x1000;
        work->unkA4.vz = 0x1000;
        work->unk150[4] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x95), 1);
        work->unk150[5] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x94), 1);
        work->unk150[6] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x21), 1);
        work->unk150[7] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 1);
        D_801EE51C.unk10 = 0;
        D_801EE51C.unk12 = 0x1000;
        D_801EE51C.unk14 = 0x1000;
        D_801EE51C.unk16 = 0x40;
        D_801EE53C.unk8 = 0;
        D_801EE53C.unkA = 0x11;
        D_801EE53C.unkC = 0x1000;
        D_801EE53C.unkE = 0x1000;
        D_801EE53C.unk10 = 0x40;
        D_801EE55C.unk8 = 0;
        D_801EE55C.unkA = 1;
        D_801EE55C.unkC = 0x1000;
        D_801EE55C.unkE = 0x1000;
        D_801EE55C.unk10 = 0x40;
        work->unkB8.vx = 0;
        work->unkB8.vy = 0;
        work->unkB8.vz = 0;
        work->unkC0.vx = 0;
        work->unkC0.vy = 0;
        work->unkC0.vz = 0;
        work->unkD0.vx = 0x800;
        work->unkD0.vy = 0x800;
        work->unkD0.vz = 0x800;
        work->unk150[8] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x81), 1);
        work->unk150[9] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x87), 1);
        work->unk150[10] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x93), 1);
        work->unkE0.vx = 0;
        work->unkE0.vy = 0;
        work->unkE0.vz = 0;
        work->unkE8.vx = 0;
        work->unkE8.vy = 0;
        work->unkE8.vz = 0;
        work->unkF8.vx = 0x1000;
        work->unkF8.vy = 0x1000;
        work->unkF8.vz = 0x1000;
        for (i0 = 0, dm = D_801ECAC0; i0 < 3; i0++, dm++) {
            dm->unk20[2] = dm->unk30[2] + 0x4E20;
        }
        for (i0 = 0; i0 < 3; i0++) {
            work->unk150[i0 + 11] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801ECB7C[i0]), 1);
        }
        work->unk14C = 0;
        work->unk14E = 4;
        func_801EC718();
        func_801EC764(0, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0));
        func_801EC764(0, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0));
        func_801EC764(0, 3, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9B), 0));
        func_801EC764(0, 4, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9B), 1));
        func_801EC764(0, 9, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9B), 2));
        func_801EC764(0, 0xA, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9B), 3));
        func_801EC764(0x2D, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 1));
        func_801EC764(0x2D, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 1));
        func_801EC764(0x2D, 3, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9F), 1));
        func_801EC764(0x2D, 4, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x96), 1));
        func_801EC764(0x2D, 9, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x92), 1));
        func_801EC764(0x41, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 2));
        func_801EC764(0x41, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 2));
        func_801EC764(0x41, 3, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9F), 2));
        func_801EC764(0x41, 4, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x96), 2));
        func_801EC764(0x41, 9, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x92), 2));
        func_801EC764(0x96, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 4));
        func_801EC764(0x96, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 4));
        func_801EC764(0x96, 3, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9F), 4));
        func_801EC764(0x96, 4, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x96), 4));
        func_801EC764(0x96, 9, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x92), 4));
        func_801EC764(0xEF, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 5));
        func_801EC764(0xEF, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 5));
        func_801EC764(0xEF, 3, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9F), 5));
        func_801EC764(0xEF, 4, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x96), 5));
        func_801EC764(0xEF, 9, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x92), 5));
        func_801EC764(0x10E, 1, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 6));
        func_801EC764(0x10E, 2, 0, (s16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 6));
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x2E:
            work->unk128 = 0;
            work->unk12C = 0;
            ex->Hi_SetSummonMotion(((Fx0e2Summon *)env->unk4)->unk54->unk180[0], ((Fx0e2Summon *)env->unk4)->unk60);
            break;
        case 0x97:
            work->unk3C.vx = 0x181;
            work->unk128 = 1;
            work->unk12C = 0;
            ex->Hi_SetSummonMotion(((Fx0e2Summon *)env->unk4)->unk54->unk180[1], ((Fx0e2Summon *)env->unk4)->unk60);
            break;
        }
        switch (frame) {
        case 0x2E:
            work->unk0 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA6), (s32)&D_801F105C);
            if (p != NULL) {
                p->unk22 = -0x100;
            }
            break;
        case 0x41:
            work->unk4 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA5), (s32)&D_801F105C);
            if (p != NULL) {
                p->unk22 = -0x100;
            }
            break;
        case 0x42:
            ex->func_800E3D54(work->unk0);
            break;
        case 0x9B:
            ex->func_800E3D54(work->unk4);
            break;
        case 0xA3:
            work->unk1C = p2 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA4), (s32)&D_801F106C);
            if (p2 != NULL) {
                p2->unk14 = (s32)D_801EFA2C;
                work->unk1C->unk12 = 0xC;
                work->unk1C->unkC = 0x50;
                if (work->unk1C != NULL) {
                    work->unk1C->unk22 = 0x100;
                }
            }
            break;
        case 0xB3:
            work->unk20 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA3), (s32)&D_801F1064);
            work->unk24 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA2), (s32)&D_801F1064);
            break;
        case 0xB5:
            for (i2 = 0; i2 < 5; i2++) {
                work->unk28[i2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801ECB98[i2]), (s32)&D_801F10AC[i2]);
            }
            break;
        case 0xF0:
            for (i2 = 0; i2 < 5; i2++) {
                ex->func_800E3D54(work->unk28[i2]);
            }
            break;
        case 0xF8:
            for (i2 = 0; i2 < 5; i2++) {
                work->unk8[i2] = p3 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801ECB8C[i2]), (s32)&D_801F107C[i2]);
                if (p3 != NULL) {
                    p3->unk14 = (s32)&D_801EF0CC[i2];
                    work->unk8[i2]->unk12 = 6;
                    work->unk8[i2]->unkC = 0x50;
                }
            }
            break;
        case 0xA1:
            work->unk140 = 0x10;
            work->unk144 = 0x10;
            break;
        case 0xB1:
            work->unk148 = 0xC;
            break;
        case 0x10E:
            break;
        }
        switch (frame) {
        case 0x97:
            ex->Hi_FreeEffModel(work->unk150[8]);
            ex->Hi_FreeEffModel(work->unk150[9]);
            ex->Hi_FreeEffModel(work->unk150[10]);
            break;
        case 0xBD:
            ex->Hi_FreeEffModel(work->unk150[0]);
            ex->Hi_FreeEffModel(work->unk150[1]);
            ex->Hi_FreeEffModel(work->unk150[2]);
            ex->Hi_FreeEffModel(work->unk150[3]);
            break;
        case 0xF0:
            ex->Hi_FreeEffModel(work->unk150[11]);
            ex->Hi_FreeEffModel(work->unk150[12]);
            ex->Hi_FreeEffModel(work->unk150[13]);
            break;
        }
        if (frame < 0x10E) {
            work->unk44.vx = env->camera->points[2].vx;
            work->unk44.vy = env->camera->points[2].vy;
            work->unk44.vz = env->camera->points[2].vz;
            work->unk54.vx = env->camera->points[8].vx;
            work->unk54.vy = env->camera->points[8].vy;
            work->unk54.vz = env->camera->points[8].vz;
            D_801F0FEC.vx = work->unk54.vx;
            D_801F0FEC.vy = work->unk54.vy;
            D_801F0FEC.vz = work->unk54.vz;
            work->unkC0.vx = env->camera->points[3].vx;
            work->unkC0.vy = env->camera->points[3].vy;
            work->unkC0.vz = env->camera->points[3].vz;
        }
        n = frame - 0x2E;
        if (n >= 0 && n < 0x83) {
            ex->Hi_DrawSummonModel(&work->unk3C, &work->unk44, (s32)&work->unk64, ((Fx0e2Summon *)env->unk4)->unk60,
                                   work->unk12C);
            if (n >= 0 && n < 0x14) {
                ex->Hi_ModifySummonModelRGB(((Fx0e2Summon *)env->unk4)->unk60, -0x20, -0x20, -0x20);
            }
            if (n >= 0x69) {
                ex->Hi_ModifySummonModelAbr(((Fx0e2Summon *)env->unk4)->unk60, 1);
            }
            if (work->unk140 >= 0) {
                work->unk74 = func_801EC434(0, -0x80, 0x10 - work->unk140, 0x10);
                work->unk140--;
                ex->Hi_ModifySummonModelRGB(((Fx0e2Summon *)env->unk4)->unk60, work->unk74, work->unk74, work->unk74);
            }
        }
        if (work->unk144 >= 0) {
            work->unk78 = func_801EC434(-0x80, 0, 0x10 - work->unk144, 0x10);
            work->unk144--;
        }
        if (work->unk148 >= 0) {
            work->unk78 = func_801EC434(0, -0x80, 0xC - work->unk148, 0xC);
            work->unk148--;
        }
        if (frame >= 0xA1 && frame < 0xBD) {
            if (frame - 0xA1 >= 0x14) {
                t = 0;
            } else if (frame - 0xA1 >= 8) {
                t = 0x1000 - (work->unk138++ << 12) / 12;
            } else {
                t = 0x1000;
            }
            ex->Hi_DrawMorphEffModel(&work->unk3C, (VECTOR *)&work->unk54, (s32)&work->unk64, work->unk150[0],
                                     work->unk150[1], t);
            ex->Hi_ModifyEffModelAbr(work->unk150[0], 1);
            if (work->unk148 >= 0 || work->unk144 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->unk150[0], work->unk78, work->unk78, work->unk78);
            }
        }
        if (frame >= 0xA1 && frame < 0xBD) {
            if (frame - 0xA1 >= 0x18) {
                t = 0;
            } else if (frame - 0xA1 >= 0xC) {
                t = 0x1000 - (work->unk13C++ << 12) / 12;
            } else {
                t = 0x1000;
            }
            ex->Hi_DrawMorphEffModel(&work->unk3C, (VECTOR *)&work->unk54, (s32)&work->unk64, work->unk150[2],
                                     work->unk150[3], t);
            ex->Hi_ModifyEffModelAbr(work->unk150[2], 1);
            if (work->unk148 >= 0 || work->unk144 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->unk150[2], work->unk78, work->unk78, work->unk78);
            }
        }
        if (frame >= 0xB1 && frame < 0xCD) {
            f = frame - 0xB1;
            a = 0;
            if (f >= 8) {
                a = (frame - 0xB9) / 2;
                b = a + 1;
                c = ((frame - 0xB9) & 1) << 11;
            } else {
                b = 0;
                c = 0;
            }
            if (a < 4) {
                x = a;
            } else {
                x = 3;
            }
            a = x;
            if (b < 4) {
                x = b;
            } else {
                x = 3;
            }
            b = x;
            if (f < 0x10) {
                D_801EE52E = f << 8;
            } else {
                D_801EE52E = 0x1000;
            }
            for (i5 = 0; i5 < 5; i5++) {
                func_801EC508(&D_801EE51C.unk0,
                              (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, D_801ECB84[a]), (u16)i5));
                func_801EC508(&D_801EE524,
                              (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, D_801ECB84[b]), (u16)i5));
                func_801E77C0(&D_801EE51C.unk0, &D_801EE524, c, (Fx0e2Trail *)&D_801EEDCC[i5], 0x98, D_801EE51C.unk16);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x17), 0, NULL, 0, D_801EE51C.unk14,
                                    D_801EE51C.unk12, 1, (u8 *)&D_801EEDCC[i5], 0);
                D_801F0FFC.vx = 0x113;
                D_801F0FFC.vy = 0x800;
                D_801F0FFC.vz = 0;
                work->unk84.vx = D_801F0FF4.vx;
                work->unk84.vy = D_801F0FF4.vy;
                work->unk84.vz = D_801F0FF4.vz;
                D_801F100C[i5].vx = D_801F0FF4.vx;
                D_801F100C[i5].vy = D_801F0FF4.vy;
                D_801F100C[i5].vz = D_801F0FF4.vz;
                work->unk7C.vx = D_801F0FFC.vx;
                work->unk7C.vy = D_801F0FFC.vy;
                work->unk7C.vz = D_801F0FFC.vz;
                work->unk94.vz = work->unk94.vy = work->unk94.vx = f * 0xD7;
                if (f < 0xC) {
                    t = 0x1000;
                } else if (f >= 0x14) {
                    t = 0;
                } else {
                    t = 0x1000 - ((f - 0xC) << 9);
                }
                ex->Hi_DrawMorphEffModel(&work->unk7C, &work->unk84, (s32)&work->unk94, work->unk150[4], work->unk150[5],
                                         t);
                work->unkB4 = -((0x13 - f) * 6);
                ex->Hi_ModifyEffModelRGB(work->unk150[4], work->unkB4, work->unkB4, work->unkB4);
                if (frame >= 0xB5) {
                    if (work->unk28[i5] != NULL && work->unk28[i5]->unk30 == -1) {
                        work->unk28[i5] = NULL;
                    }
                    ex->func_800E3D20((s32)work->unk28[i5], (s32)&D_801F100C[i5]);
                }
            }
        }
        if (frame >= 0xCD && frame < 0xF0) {
            k = frame - 0xCD;
            D_801EE53C.unk8 = k;
            D_801EE53C.unk8 = D_801EE53C.unk8 >= 0x23 ? 0x22 : D_801EE53C.unk8;
            for (i6 = 0; i6 < 5; i6++) {
                func_801EC48C(&D_801EE53C.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9E), (u16)i6));
                func_801EC4A4(&D_801EE53C.unk0, k + 0x37, &D_801F0FF4);
                D_801F0FFC.vx = 0x113;
                D_801F0FFC.vy = 0x800;
                D_801F0FFC.vz = 0;
                work->unk84.vx = D_801F0FF4.vx;
                work->unk84.vy = D_801F0FF4.vy;
                work->unk84.vz = D_801F0FF4.vz;
                D_801F100C[i6].vx = D_801F0FF4.vx;
                D_801F100C[i6].vy = D_801F0FF4.vy;
                D_801F100C[i6].vz = D_801F0FF4.vz;
                work->unk7C.vx = D_801F0FFC.vx;
                work->unk7C.vy = D_801F0FFC.vy;
                work->unk7C.vz = D_801F0FFC.vz;
                ex->Hi_DrawMorphEffModel(&work->unk7C, &work->unk84, (s32)&work->unk94, work->unk150[4], work->unk150[5],
                                         0);
                if (work->unk28[i6] != NULL && work->unk28[i6]->unk30 == -1) {
                    work->unk28[i6] = NULL;
                }
                ex->func_800E3D20((s32)work->unk28[i6], (s32)&D_801F100C[i6]);
            }
        }
        if (frame >= 0xF0 && frame < 0x10F) {
            D_801EE55C.unkA = frame - 0xF0;
            if (D_801EE55C.unkA >= 0x12) {
                D_801EE55C.unk8 = frame - 0x101;
                D_801EE55C.unkA = 0x11;
            }
            for (i7 = 0; i7 < 5; i7++) {
                func_801EC48C(&D_801EE55C.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x9D), (u16)i7));
                func_801E7BE8(&D_801EE55C.unk0, (Fx0e2Trail *)&D_801EEDCC[i7], 0x98, D_801EE55C.unk8, D_801EE55C.unkA,
                              D_801EE55C.unk10);
                D_801F0FFC.vx = 0x113;
                D_801F0FFC.vy = 0x800;
                D_801F0FFC.vz = 0;
                work->unk84.vx = D_801F0FF4.vx;
                work->unk84.vy = D_801F0FF4.vy;
                work->unk84.vz = D_801F0FF4.vz;
                D_801F100C[i7].vx = D_801F0FF4.vx;
                D_801F100C[i7].vy = D_801F0FF4.vy;
                D_801F100C[i7].vz = D_801F0FF4.vz;
                work->unk7C.vx = D_801F0FFC.vx;
                work->unk7C.vy = D_801F0FFC.vy;
                work->unk7C.vz = D_801F0FFC.vz;
                ex->Hi_DrawEffModel(&work->unk7C, &work->unk84, (s32)&work->unk94, work->unk150[6]);
                ex->Hi_DrawEffModel(&work->unk7C, &work->unk84, (s32)&work->unk94, work->unk150[7]);
                func_801EC920(work->unk14E * 0x14A - work->unk14C * work->unk14E, 0x1C0, 0x180, 0x80, 0x40, 0x1C0, 0x1C0);
                work->unk14C++;
                if (frame >= 0xF8) {
                    if (work->unk8[i7] != NULL && work->unk8[i7]->unk30 == -1) {
                        work->unk8[i7] = NULL;
                    }
                    ex->func_800E3D20((s32)work->unk8[i7], (s32)&D_801F100C[i7]);
                }
            }
        }
        m = frame - 0x2E;
        if (m >= 0 && m < 0x69 && !(frame >= 0x97 && frame < 0xF0)) {
            if (m < 0x14) {
                ex->Hi_DrawEffModel(&work->unkB8, &work->unkC0, (s32)&work->unkD0, work->unk150[8]);
                ex->Hi_DrawEffModel(&work->unkB8, &work->unkC0, (s32)&work->unkD0, work->unk150[9]);
            } else {
                ex->Hi_DrawEffModel(&work->unkB8, &work->unkC0, (s32)&work->unkD0, work->unk150[10]);
            }
        }
        if (frame >= 0x97 && frame < 0xF0) {
            for (dr = D_801ECAC0, j = 0; j < 3; j++, dr++) {
                for (k = 0; k < 2; k++) {
                    work->unkE8.vx = dr->unk0[k][0];
                    work->unkE8.vy = dr->unk0[k][1];
                    work->unkE8.vz = dr->unk0[k][2];
                    work->unkE8.vx += work->unk44.vx;
                    if (j == 2) {
                        work->unkE8.vz -= 0x7530;
                    } else {
                        work->unkE8.vy -= 0xC18;
                        work->unkE8.vz -= 0x4E20;
                    }
                    ex->Hi_DrawEffModel(&work->unkE0, &work->unkE8, (s32)&work->unkF8, work->unk150[j + 11]);
                    dr->unk0[k][0] += dr->unk30[0];
                    dr->unk0[k][1] += dr->unk30[1];
                    dr->unk0[k][2] += dr->unk30[2];
                    if (dr->unk0[k][2] < -0x4E20) {
                        dr->unk0[k][0] = dr->unk20[0];
                        dr->unk0[k][1] = dr->unk20[1];
                        dr->unk0[k][2] = dr->unk20[2];
                    }
                }
            }
        }
        if (frame >= 1 && frame < 0x2E) {
            for (i8 = 1; i8 < 4; i8++) {
                switch (i8) {
                case 0:
                    D_801F103C[i8].vx = env->camera->points[2].vx;
                    D_801F103C[i8].vy = env->camera->points[2].vy;
                    D_801F103C[i8].vz = env->camera->points[2].vz;
                    break;
                case 1:
                    D_801F103C[i8].vx = env->camera->points[3].vx;
                    D_801F103C[i8].vy = env->camera->points[3].vy;
                    D_801F103C[i8].vz = env->camera->points[3].vz;
                    break;
                case 2:
                    D_801F103C[i8].vx = env->camera->points[8].vx;
                    D_801F103C[i8].vy = env->camera->points[8].vy;
                    D_801F103C[i8].vz = env->camera->points[8].vz;
                    break;
                case 3:
                    D_801F103C[i8].vx = env->camera->points[9].vx;
                    D_801F103C[i8].vy = env->camera->points[9].vy;
                    D_801F103C[i8].vz = env->camera->points[9].vz;
                    break;
                }
            }
        }
        if (frame >= 0x2E) {
            work->unk110.vy = -0x2C9E;
            work->unk110.vx = 0;
            work->unk110.vz = -0x729A;
            if (work->unk0 != NULL && work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            ex->func_800E3D20((s32)work->unk0, (s32)&work->unk110);
        }
        if (frame >= 0x41) {
            work->unk110.vy = -0x303D;
            work->unk110.vx = 0;
            work->unk110.vz = -0x7411;
            if (work->unk4 != NULL && work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            ex->func_800E3D20((s32)work->unk4, (s32)&work->unk110);
        }
        if (frame >= 0xA3) {
            work->unk118.vy = 0x128;
            work->unk118.vx = 0;
            work->unk118.vz = 0x3EB;
            work->unk118.vx += work->unk54.vx;
            work->unk118.vy += work->unk54.vy;
            work->unk118.vz += work->unk54.vz;
            if (work->unk1C != NULL && work->unk1C->unk30 == -1) {
                work->unk1C = NULL;
            }
            ex->func_800E3D20((s32)work->unk1C, (s32)&work->unk118);
        }
        if (frame >= 0xB3) {
            work->unk120.vx = D_801F100C[0].vx;
            work->unk120.vy = D_801F100C[0].vy;
            work->unk120.vz = D_801F100C[0].vz;
            if (work->unk20 != NULL && work->unk20->unk30 == -1) {
                work->unk20 = NULL;
            }
            ex->func_800E3D20((s32)work->unk20, (s32)&work->unk120);
            if (work->unk24 != NULL && work->unk24->unk30 == -1) {
                work->unk24 = NULL;
            }
            ex->func_800E3D20((s32)work->unk24, (s32)&work->unk120);
        }
        func_801EC7C0(0);
        return frame >= 0x14A;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_headerPtr);

s32 fx_0e2_op81(s32 mode, Fx0e2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    Pool6CEntry **pp;
    SVECTOR v;
    s32 frame;
    s32 n;
    s32 m;
    s32 i;
    s32 i3;
    s32 i4;
    s32 i5;

    if (mode == 0) {
        *arg4 = 0x13C;
    } else if (mode == 1) {
        D_801ECC0C = env;
        work->unk1C.vx = -0x18D;
        work->unk1C.vz = -0x25E;
        work->unk14.vx = 0;
        work->unk14.vy = 0;
        work->unk14.vz = 0;
        work->unk1C.vy = 0;
        work->unk2C.vx = 0x1000;
        work->unk2C.vy = 0x1000;
        work->unk2C.vz = 0x1000;
        for (i = 0; i < 3; i++) {
            work->unk124[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801ECB68[i]), 1);
        }
        work->unk3C.vx = 0;
        work->unk3C.vy = 0;
        work->unk3C.vz = 0;
        work->unk44.vx = 0;
        work->unk44.vy = 0;
        work->unk44.vz = 0;
        work->unk54.vx = 0x1000;
        work->unk54.vy = 0x1000;
        work->unk54.vz = 0x1000;
        for (i = 0; i < 5; i++) {
            func_801EC508(&D_801EE99C[i], (u16 *)func_801EC3D8(ex->btlfx_getFileData(entry->unk8, 0x9C), (u16)i));
            work->unk124[i + 3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801ECB70[i]), 1);
        }
        for (i = 0; i < 4; i++) {
            setVector(&work->unk64[i], 0, 0, 0);
            setVector(&work->unk84[i], 0, 0, 0);
            setVector(&work->unkC4[i], 0x1000, 0x1000, 0x1000);
            work->unk124[i + 8] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x80), 1);
        }
        return 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            ex->func_800E6E1C(0x300, 0x180, 0x80, 0x80, D_801EE9CC, -0x18D, -0x300, 0x25E);
            break;
        case 3:
            work->unk104[0].vx = work->unk44.vx;
            work->unk104[0].vy = work->unk44.vy;
            work->unk104[0].vz = work->unk44.vz;
            work->unk0[0] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA0), (s32)&work->unk104[0]);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 16:
            work->unk0[1] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA0), (s32)&work->unk104[0]);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 30:
            work->unk104[1].vx = 0x20C;
            work->unk104[1].vy = 0;
            work->unk104[1].vz = -0x3B6;
            work->unk0[2] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA0), (s32)&work->unk104[1]);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 35:
            work->unk104[2].vx = -0x205;
            work->unk104[2].vy = 0;
            work->unk104[2].vz = -0x5D9;
            work->unk0[3] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA0), (s32)&work->unk104[2]);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 38:
            work->unk104[3].vx = 0x110;
            work->unk104[3].vy = 0;
            work->unk104[3].vz = -0x703;
            work->unk0[4] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA0), (s32)&work->unk104[3]);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 46:
            ex->func_800E3D54(work->unk0[0]);
            for (i3 = 0, pp = work->unk0; i3 < 4; pp++, i3++) {
                ex->func_800E3D54(pp[1]);
            }
            break;
        }
        if ((u32)(frame - 1) < 0x2D) {
            if (frame - 1 >= 0x12) {
                m = work->unk124[2];
            } else if (frame - 1 >= 5) {
                m = work->unk124[1];
            } else {
                m = work->unk124[0];
            }
            ex->Hi_SetEffModelSlice(m, work->unk1C.vy);
            ex->Hi_DrawSliceEffModel(&work->unk14, &work->unk1C, (s32)&work->unk2C, m);
            ex->Hi_ModifyEffModelRGB(m, 6, 6, 6);
        }
        if ((u32)(frame - 1) < 0x2D) {
            n = frame - 1;
            for (i4 = 0; i4 < 5; i4++) {
                func_801EC520(&D_801EE99C[i4], n, &work->unk3C, &v);
                work->unk44.vx = v.vx;
                work->unk44.vy = v.vy;
                work->unk44.vz = v.vz;
                work->unk44.vx -= 0x18D;
                work->unk44.vz -= 0x25E;
                ex->Hi_SetEffModelSlice(work->unk124[i4 + 3], work->unk44.vy);
                ex->Hi_DrawSliceEffModel(&work->unk3C, &work->unk44, (s32)&work->unk54, work->unk124[i4 + 3]);
            }
        }
        if ((u32)(frame - 1) < 0x2D) {
            for (i5 = 1; i5 < 4; i5++) {
                setVector(&work->unk84[i5], D_801F103C[i5].vx, D_801F103C[i5].vy, D_801F103C[i5].vz);
                ex->Hi_SetEffModelSlice(work->unk124[i5 + 8], work->unk84[i5].vy);
                ex->Hi_DrawSliceEffModel(&work->unk64[i5], &work->unk84[i5], (s32)&work->unkC4[i5], work->unk124[i5 + 8]);
            }
        }
        return frame >= 0x30;
    }
    return 0;
}

s32 fx_0e2_op82(s32 mode, Fx0e2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 x;
    s32 a2;
    s32 b2;
    s32 a3;
    s32 b3;
    s32 r;
    s16 n;
    s16 m;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        work->unk50.vx = env->targets->centerX;
        D_801ECC0C = env;
        work->unk50.vy = env->targets->centerY;
        work->unk50.vz = env->targets->centerZ;
        work->unk50.vy -= 0x3E8;
        D_801EE57C.unk1E = 0x200;
        D_801EE57C.unk18 = 0;
        D_801EE57C.unk1A = 0;
        D_801EE57C.unk1C = 0x80;
        D_801EE57C.unk20 = 0;
        D_801EE57C.unk22 = 0;
        D_801EE5AC.unk1E = 0x180;
        D_801EE5AC.unk18 = 0;
        D_801EE5AC.unk1A = 0;
        D_801EE5AC.unk1C = 0x80;
        D_801EE5AC.unk20 = 0;
        D_801EE5AC.unk22 = 0;
        D_801EE60C.unk10 = 0;
        D_801EE60C.unk12 = 0;
        D_801EE60C.unk14 = 0x80;
        D_801EE60C.unk16 = 0x400;
        D_801EE60C.unk18 = 0;
        D_801EE5DC.unk1A = 0x20;
        D_801EE5DC.unk18 = 0;
        D_801EE5DC.unk1C = 0x80;
        D_801EE5DC.unk1E = 0x400;
        D_801EE5DC.unk20 = 0;
        D_801EE5DC.unk22 = 0;
        setVector(&D_801EE57C.unk10, 0x1000, 0x1000, 0x1000);
        setVector(&D_801EE5AC.unk10, 0x1000, 0x1000, 0x1000);
        setVector(&D_801EE60C.unk8, 0x1000, 0x1000, 0x1000);
        setVector(&D_801EE5DC.unk10, 0x1000, 0x1000, 0x1000);
        setVector(&work->unkC, 0, 0, 0);
        setVector(&work->unk14, 0, 0, 0);
        setVector(&work->unk1C, 0, 0, 0x4B0);
        setVector(&work->unk2C, 0x1CCC, 0x1000, 0x1CCC);
        work->unk68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x11), mode);
        work->unk6A = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1D), mode);
        work->unk58 = -1;
        work->unk5C = -1;
        work->unk60 = mode;
        work->unk64 = 0;
        work->unk66 = 0xC;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk40.vx = 0;
            work->unk40.vy = 0;
            work->unk40.vz = 0x4B0;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk40);
            break;
        case 1:
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk40);
            break;
        case 6:
            work->unk48.vx = work->unk40.vx;
            work->unk48.vy = work->unk40.vy;
            work->unk48.vz = work->unk40.vz;
            work->unk48.vy -= 0xC80;
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk48);
            break;
        case 0x1C:
            work->unk58 = 0x20;
            work->unk5C = 0x20;
            work->unk60 = 0x20;
            break;
        }
        if (frame >= 0 && frame < 60) {
            D_801EE57C.unk1A += 6;
            n = D_801EE57C.unk1A;
            if (n > 60) {
                n = 60;
            }
            D_801EE57C.unk1A = n;
            D_801EE57C.unk20 = (frame % 60 % 10) * 0x199;
            if (frame % 60 < 10) {
                a = 0;
                b = 6;
            } else if (frame % 60 < 20) {
                a = 6;
                b = 12;
            } else if (frame % 60 < 30) {
                a = 12;
                b = 18;
            } else if (frame % 60 < 40) {
                a = 18;
                b = 24;
            } else if (frame % 60 < 50) {
                a = 24;
                b = 30;
            } else if (frame % 60 < 60) {
                a = 30;
                b = 0;
            } else {
                a = 0;
                b = 0;
            }
            if (D_801EE57C.unk1A >= 60) {
                D_801EE57C.unk10.vy = 0x1000;
                D_801EE57C.unk10.vx += 0x80;
                D_801EE57C.unk10.vz += 0x80;
            } else {
                D_801EE57C.unk10.vx = D_801EE57C.unk10.vy = D_801EE57C.unk10.vz =
                    (s16)(D_801EE57C.unk1A / 10) * 0x199 + 0x800;
            }
            if (work->unk58 >= 0) {
                D_801EE57C.unk1C = func_801EC434(0x80, 0, 0x20 - work->unk58, 0x20);
                work->unk58--;
            }
            for (i = 0; i < 6; i++) {
                func_801EC48C(&D_801EE57C.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), a + i));
                func_801EC48C(&D_801EE57C.unk8, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), b + i));
                func_801E7FBC(&D_801EE57C.unk0, &D_801EE57C.unk8, D_801EE57C.unk18, D_801EE57C.unk1A, &D_801EE57C.unk10,
                              D_801EE57C.unk1E, D_801EE57C.unk20, D_801EE57C.unk22, 1);
                func_801E86D0(D_801EE57C.unk1A, D_801EE57C.unk1C, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 0 && frame < 60) {
            t = frame;
            D_801EE5AC.unk1A += 6;
            m = D_801EE5AC.unk1A;
            if (m > 60) {
                m = 60;
            }
            D_801EE5AC.unk1A = m;
            if (D_801EE5AC.unk1A >= 60) {
                D_801EE5AC.unk10.vy = 0x1000;
                D_801EE5AC.unk10.vx += 0x80;
                D_801EE5AC.unk10.vz += 0x80;
            } else {
                D_801EE5AC.unk10.vx = D_801EE5AC.unk10.vy = D_801EE5AC.unk10.vz =
                    (s16)(D_801EE5AC.unk1A / 10) * 0x199 + 0x800;
            }
            if (work->unk5C >= 0) {
                D_801EE5AC.unk1C = func_801EC434(0x80, 0, 0x20 - work->unk5C, 0x20);
                work->unk5C--;
            }
            k = t;
            k %= 30;
            D_801EE5AC.unk20 = (k % 10) * 0x199;
            if (k < 10) {
                a2 = 0;
                b2 = 3;
            } else if (k < 20) {
                a2 = 3;
                b2 = 6;
            } else if (k < 30) {
                a2 = 6;
                b2 = 0;
            } else {
                a2 = 0;
                b2 = 0;
            }
            for (i = 0; i < 3; i++) {
                func_801EC48C(&D_801EE5AC.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), a2 + i));
                func_801EC48C(&D_801EE5AC.unk8, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), b2 + i));
                func_801E7FBC(&D_801EE5AC.unk0, &D_801EE5AC.unk8, D_801EE5AC.unk18, D_801EE5AC.unk1A, &D_801EE5AC.unk10,
                              D_801EE5AC.unk1E, D_801EE5AC.unk20, D_801EE5AC.unk22, 1);
                func_801E86D0(D_801EE5AC.unk1A, D_801EE5AC.unk1C, 0x9C, 0x3D40, 0x80, 0xBF);
            }
        }
        if (frame >= 0 && frame < 16) {
            D_801EE60C.unk12 += 2;
            m = D_801EE60C.unk12;
            if (m > 0x20) {
                m = 0x20;
            }
            D_801EE60C.unk12 = m;
            for (i = 0; i < 5; i++) {
                if (i & 1) {
                    func_801EC48C(&D_801EE60C.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), 0));
                } else {
                    func_801EC48C(&D_801EE60C.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0));
                }
                D_801EE60C.unk18 = i * 0x333;
                func_801E83B0(&D_801EE60C.unk0, D_801EE60C.unk10, D_801EE60C.unk12, &D_801EE60C.unk8, D_801EE60C.unk16,
                              D_801EE60C.unk18);
                func_801E86D0(D_801EE60C.unk12, D_801EE60C.unk14, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 16 && frame < 60) {
            x = frame - 16;
            /* separate fresh variables: the compares use r, the % 8 uses c */
            c = x % 24;
            r = c;
            D_801EE5DC.unk20 = (c % 8) << 9;
            if (r < 8) {
                a3 = 0;
                b3 = 1;
            } else if (r < 16) {
                a3 = 1;
                b3 = 2;
            } else if (r < 24) {
                a3 = 2;
                b3 = 0;
            } else {
                a3 = 0;
                b3 = 0;
            }
            if (x >= 0) {
                D_801EE5DC.unk10.vx += 0x80;
                D_801EE5DC.unk10.vz += 0x80;
            }
            if (work->unk58 >= 0) {
                D_801EE5DC.unk1C = func_801EC434(0x80, 0, 0x20 - work->unk58, 0x20);
                work->unk58--;
            }
            for (j = 0; j < 5; j++) {
                if (j & 1) {
                    func_801EC48C(&D_801EE5DC.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), a3));
                    func_801EC48C(&D_801EE5DC.unk8, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), b3));
                } else {
                    func_801EC48C(&D_801EE5DC.unk0, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), a3));
                    func_801EC48C(&D_801EE5DC.unk8, (u16 *)func_801EC3D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), b3));
                }
                D_801EE5DC.unk22 = j * 0x333;
                func_801E7FBC(&D_801EE5DC.unk0, &D_801EE5DC.unk8, D_801EE5DC.unk18, D_801EE5DC.unk1A, &D_801EE5DC.unk10,
                              D_801EE5DC.unk1E, D_801EE5DC.unk20, D_801EE5DC.unk22, 0);
                func_801E86D0(D_801EE5DC.unk1A, D_801EE5DC.unk1C, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 8 && frame < 60) {
            ex->Hi_DrawEffModel(&work->unkC, &work->unk1C, (s32)&work->unk2C, work->unk68);
            ex->Hi_DrawEffModel(&work->unk14, &work->unk1C, (s32)&work->unk2C, work->unk6A);
            if (work->unk60 >= 0) {
                work->unk3C = func_801EC434(0, -0x80, 0x20 - work->unk60, 0x20);
                work->unk60--;
                ex->Hi_ModifyEffModelRGB(work->unk68, work->unk3C, work->unk3C, work->unk3C);
                ex->Hi_ModifyEffModelRGB(work->unk6A, work->unk3C, work->unk3C, work->unk3C);
            }
        }
        func_801EC920(work->unk66 * 0x78 - work->unk64 * work->unk66, 0x1C0, 0x180, 0x80, 0x40, 0x1C0, 0x1C0);
        func_801EC920(work->unk66 * 0x78 - work->unk64 * work->unk66, 0x300, 0x180, 0x80, 0x40, 0x300, 0x140);
        work->unk64++;
        if (frame >= 0x78) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

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
        for (i = 0; i < env->targets->targetCount; i++, o++) {
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
            for (j = 0, q = D_801EE8FC; j < env->targets->targetCount; j++, q++) {
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
        for (i = 0; i < env->targets->targetCount; i++, o++) {
            work->unkC0[i] = 0;
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk40[i]);
            work->unk40[i].vy = -0xBB8;
            ex->btlfx_setUnitPos(i, (s16 *)&work->unk40[i]);
            ex->btlfx_setUnitRot(i, (s16 *)&o->unk8);
        }
    } else {
        frame = entry->unk0;
        for (j = 0; j < env->targets->targetCount; j++) {
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

    BTLFX_EXPORTS(D_801ECC0C)->btlfx_drawScreenTint((u32 *)(D_801ECC0C->ot + ot), arg4, c, c, c);
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
                        ((SVECTOR *)env->camera)[d].vx = D_801F10DC[i].unk0[0];
                        ((SVECTOR *)env->camera)[d].vy = D_801F10DC[i].unk0[1];
                        ((SVECTOR *)env->camera)[d].vz = D_801F10DC[i].unk0[2];
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
