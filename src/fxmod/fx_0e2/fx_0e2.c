#include "fxmod/fx_0e2.h"

void fx_0e2_stripFromTrackBlend(void *trackA, void *trackB, s32 t, Fx0e2Trail *trail, s32 size, s32 pad) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e2_env);
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
    pt = trail->pts4;
    trail->lastSlot0 = trail->count2 = n - 1;
    trail->head1 = 0;
    r = t;
    for (i = 0; i < n; i++, pt++) {
        fxm_rotPosPathGet(trackA, i, &rotA, &posA);
        fxm_rotPosPathGet(trackB, i, &rotB, &posB);
        ex->btlfx_lerpVec((s16 *)&posA, (s16 *)&posB, r, (s16 *)&pos);
        ex->btlfx_lerpVec((s16 *)&rotA, (s16 *)&rotB, r, (s16 *)&rot);
        pos.vx /= 2;
        pos.vy /= 2;
        pos.vz /= 2;
        pos.vx += fx_0e2_stripOrigin.vx;
        pos.vy += fx_0e2_stripOrigin.vy;
        pos.vz += fx_0e2_stripOrigin.vz;
        pos.vx += 0x50;
        pos.vy -= 0x109;
        pos.vz += 0xE6;
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        if (i == n - 1) {
            fx_0e2_headPos.vx = pos.vx;
            fx_0e2_headPos.vy = pos.vy;
            fx_0e2_headPos.vz = pos.vz;
            fx_0e2_headRot.vx = rot.vx;
            fx_0e2_headRot.vy = rot.vy;
            fx_0e2_headRot.vz = rot.vz;
            fx_0e2_headRot.vy += 0x800;
        }
        pt->pad = pad;
    }
}

void fx_0e2_stripFromTrack(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 pad) {
    SVECTOR rot;
    SVECTOR pos;
    SVECTOR *pt;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    pt = trail->pts4;
    trail->lastSlot0 = trail->count2 = n - 1;
    trail->head1 = 0;
    for (i = 0; i < n; i++, pt++) {
        fxm_rotPosPathGet(track, i + base, &rot, &pos);
        pos.vx /= 2;
        pos.vy /= 2;
        pos.vz /= 2;
        pos.vx += fx_0e2_stripOrigin.vx;
        pos.vy += fx_0e2_stripOrigin.vy;
        pos.vz += fx_0e2_stripOrigin.vz;
        pos.vx += 0x50;
        pos.vy -= 0x109;
        pos.vz += 0xE6;
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        if (i == n - 1) {
            fx_0e2_headPos.vx = pos.vx;
            fx_0e2_headPos.vy = pos.vy;
            fx_0e2_headPos.vz = pos.vz;
            fx_0e2_headRot.vx = rot.vx;
            fx_0e2_headRot.vy = rot.vy;
            fx_0e2_headRot.vz = rot.vz;
            fx_0e2_headRot.vy += 0x800;
        }
        pt->pad = pad;
    }
}

void fx_0e2_stripFromPosPath(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad) {
    s32 k;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e2_env);
    SVECTOR pos;
    SVECTOR *pt;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    pt = trail->pts4;
    trail->lastSlot0 = trail->count2 = n - 1;
    trail->head1 = 0;
    if (count < n) {
        for (i = 0; i < n - count; i++, pt++) {
            pt->pad = 0;
        }
    } else {
        count = n;
    }
    for (i = 0; i < count; i++, pt++) {
        k = i + 1;
        fxm_posPathGet(track, count - k + base, &pos);
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        if (i == 0) {
            fx_0e2_headPos.vx = pos.vx;
            fx_0e2_headPos.vy = pos.vy;
            fx_0e2_headPos.vz = pos.vz;
            ex->btlfx_aimAngles((s16 *)&fx_0e2_headPos, (s16 *)&fx_0e2_aimPos, (s16 *)&fx_0e2_headRot);
        }
        pt->pad = pad;
    }
}

void fx_0e2_stripFromTrackBlendRot(void *trackA, void *trackB, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad, s32 t,
                   s32 rotY) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e2_env);
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
    pt = trail->pts4;
    trail->lastSlot0 = trail->count2 = n - 1;
    trail->head1 = 0;
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
        fxm_rotPosPathGet(trackA, k, NULL, &a);
        fxm_rotPosPathGet(trackB, k, NULL, &b);
        ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, t, (s16 *)&p);
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = rotY;
        p.vx *= 6;
        p.vy *= 3;
        p.vz *= 6;
        ex->math_rotMatrixYXZ(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(&out);
        out.vx += fx_0e2_aimPos.vx;
        out.vy += fx_0e2_aimPos.vy;
        out.vz += fx_0e2_aimPos.vz;
        pt->vx = out.vx;
        pt->vy = out.vy;
        pt->vz = out.vz;
        pt->pad = pad;
        pt++;
    }
}

void fxm_projectEdgesBlend(void *trackA, void *trackB, s32 unused, s32 unusedLength, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist) {
    Unk800F78A4 *env = fx_0e2_env;
    Fx0e2Edge *e = fx_0e2_edges;
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
        fxm_posPathGet(trackA, i, &a);
        fxm_posPathGet(trackB, i, &b);
        ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, t, (s16 *)&p);
        p.vx = p.vx * scale->vx / 4096;
        p.vy = p.vy * scale->vy / 4096;
        p.vz = p.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = rotY;
        ex->math_rotMatrixYXZ(&rot, &m);
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
        ex->math_rotMatrixYXZ(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&ends[0]);
        gte_rtps();
        gte_stsxy(&e->xy0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&e->xy4);
        gte_stszotz(&e->otz8);
    }
}

void fxm_projectEdges(void *track, s32 unused, s32 unusedLength, SVECTOR *scale, s32 w, s32 rotY) {
    Unk800F78A4 *env = fx_0e2_env;
    Fx0e2Edge *e = fx_0e2_edges;
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
        fxm_posPathGet(track, i, &p);
        p.vx = p.vx * scale->vx / 4096;
        p.vy = p.vy * scale->vy / 4096;
        p.vz = p.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vy = rotY;
        rot.vz = 0;
        ex->math_rotMatrixYXZ(&rot, &m);
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
        ex->math_rotMatrixYXZ(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&ends[0]);
        gte_rtps();
        gte_stsxy(&e->xy0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&e->xy4);
        gte_stszotz(&e->otz8);
    }
}

void fxm_drawEdgeQuads(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1) {
    Fx0e2Edge *e = fx_0e2_edges;
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
    p = (POLY_GT4 *)fx_0e2_env->prim;
    fx_0e2_env->prim = (u8 *)(p + n);
    cl = clut;
    ex = BTLFX_EXPORTS(fx_0e2_env);
    for (i = 0; i < n - 1; i++, p++, e++) {
        p->tpage = tp;
        p->clut = cl;
        p->code = GPU_POLY_GT4 | GPU_ABE;
        p->x0 = e[0].xy0.vx;
        p->y0 = e[0].xy0.vy;
        p->x1 = e[0].xy4.vx;
        p->y1 = e[0].xy4.vy;
        p->x2 = e[1].xy0.vx;
        p->y2 = e[1].xy0.vy;
        p->x3 = e[1].xy4.vx;
        p->y3 = e[1].xy4.vy;
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
        if (e[0].xy0.vx < e[1].xy0.vx) {
            ex->btlfx_addPrim(0x0C000000, 0, p);
        } else {
            ex->btlfx_addPrim(0x0C000000, (e[0].otz8 + e[1].otz8) >> 1, p);
        }
    }
}

s32 fx_0e2_op80(s32 mode, Fx0e2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
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
        *args = 0x16C;
    } else if (mode == 1) {
        fx_0e2_env = env;
        ex->btlfx_initEffModels(fx_0e2_effModels, 0x20);
        fx_0e2_aimPos.vx = env->targets->centerX;
        fx_0e2_aimPos.vy = env->targets->centerY;
        fx_0e2_aimPos.vz = env->targets->centerZ;
        work->rot3C.vx = 0;
        work->rot3C.vy = 0x800;
        work->rot3C.vz = 0;
        work->pos44.vx = 0;
        work->pos44.vy = 0;
        work->pos44.vz = 0;
        work->pos54.vx = 0;
        work->pos54.vy = 0;
        work->pos54.vz = 0;
        work->scale64.vx = 0x800;
        work->scale64.vy = 0x800;
        work->scale64.vz = 0x800;
        work->unk130 = 0;
        work->unk134 = 0;
        work->model150[0] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x84), fx_0e2_texPages, fx_0e2_texPos);
        work->model150[1] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x85), fx_0e2_texPages, fx_0e2_texPos);
        work->model150[2] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x83), fx_0e2_texPages, fx_0e2_texPos);
        work->model150[3] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x82), fx_0e2_texPages, fx_0e2_texPos);
        work->morphStep138 = 0;
        work->morphStep13C = 0;
        work->fadeTimer140 = -1;
        work->fadeInTimer144 = -1;
        work->fadeOutTimer148 = -1;
        work->rot7C.vx = 0;
        work->rot7C.vy = 0;
        work->rot7C.vz = 0;
        work->pos84.vx = 0;
        work->pos84.vy = 0;
        work->pos84.vz = 0;
        work->scale94.vx = 0x1000;
        work->scale94.vy = 0x1000;
        work->scale94.vz = 0x1000;
        work->unkA4.vx = 0x1000;
        work->unkA4.vy = 0x1000;
        work->unkA4.vz = 0x1000;
        work->model150[4] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x95), 1);
        work->model150[5] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x94), 1);
        work->model150[6] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x21), 1);
        work->model150[7] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x1F), 1);
        fx_0e2_stripBlend.unk10 = 0;
        fx_0e2_stripBlend.fade12 = 0x1000;
        fx_0e2_stripBlend.scale14 = 0x1000;
        fx_0e2_stripBlend.width16 = 0x40;
        fx_0e2_stripPath0.base8 = 0;
        fx_0e2_stripPath0.countA = 0x11;
        fx_0e2_stripPath0.unkC = 0x1000;
        fx_0e2_stripPath0.unkE = 0x1000;
        fx_0e2_stripPath0.width10 = 0x40;
        fx_0e2_stripPath1.base8 = 0;
        fx_0e2_stripPath1.countA = 1;
        fx_0e2_stripPath1.unkC = 0x1000;
        fx_0e2_stripPath1.unkE = 0x1000;
        fx_0e2_stripPath1.width10 = 0x40;
        work->rotB8.vx = 0;
        work->rotB8.vy = 0;
        work->rotB8.vz = 0;
        work->posC0.vx = 0;
        work->posC0.vy = 0;
        work->posC0.vz = 0;
        work->scaleD0.vx = 0x800;
        work->scaleD0.vy = 0x800;
        work->scaleD0.vz = 0x800;
        work->model150[8] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x81), 1);
        work->model150[9] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x87), 1);
        work->model150[10] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x93), 1);
        work->rotE0.vx = 0;
        work->rotE0.vy = 0;
        work->rotE0.vz = 0;
        work->posE8.vx = 0;
        work->posE8.vy = 0;
        work->posE8.vz = 0;
        work->scaleF8.vx = 0x1000;
        work->scaleF8.vy = 0x1000;
        work->scaleF8.vz = 0x1000;
        for (i0 = 0, dm = fx_0e2_drops; i0 < 3; i0++, dm++) {
            dm->start20[2] = dm->vel30[2] + 0x4E20;
        }
        for (i0 = 0; i0 < 3; i0++) {
            work->model150[i0 + 11] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0e2_dropBlocks[i0]), 1);
        }
        work->scrollStep14C = 0;
        work->scrollSpeed14E = 4;
        fxm_camChanReset();
        fxm_camChanAdd(0, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xA), 0));
        fxm_camChanAdd(0, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xB), 0));
        fxm_camChanAdd(0, 3, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x9B), 0));
        fxm_camChanAdd(0, 4, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x9B), 1));
        fxm_camChanAdd(0, 9, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x9B), 2));
        fxm_camChanAdd(0, 0xA, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x9B), 3));
        fxm_camChanAdd(0x2D, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xA), 1));
        fxm_camChanAdd(0x2D, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xB), 1));
        fxm_camChanAdd(0x2D, 3, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x9F), 1));
        fxm_camChanAdd(0x2D, 4, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x96), 1));
        fxm_camChanAdd(0x2D, 9, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x92), 1));
        fxm_camChanAdd(0x41, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xA), 2));
        fxm_camChanAdd(0x41, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xB), 2));
        fxm_camChanAdd(0x41, 3, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x9F), 2));
        fxm_camChanAdd(0x41, 4, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x96), 2));
        fxm_camChanAdd(0x41, 9, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x92), 2));
        fxm_camChanAdd(0x96, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xA), 4));
        fxm_camChanAdd(0x96, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xB), 4));
        fxm_camChanAdd(0x96, 3, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x9F), 4));
        fxm_camChanAdd(0x96, 4, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x96), 4));
        fxm_camChanAdd(0x96, 9, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x92), 4));
        fxm_camChanAdd(0xEF, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xA), 5));
        fxm_camChanAdd(0xEF, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xB), 5));
        fxm_camChanAdd(0xEF, 3, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x9F), 5));
        fxm_camChanAdd(0xEF, 4, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x96), 5));
        fxm_camChanAdd(0xEF, 9, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x92), 5));
        fxm_camChanAdd(0x10E, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xA), 6));
        fxm_camChanAdd(0x10E, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xB), 6));
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x2E:
            work->unk128 = 0;
            work->loopMotion12C = 0;
            ex->Hi_SetSummonMotion(((Fx0e2Summon *)env->loader)->summon54->motion180[0], ((Fx0e2Summon *)env->loader)->summonModel60);
            break;
        case 0x97:
            work->rot3C.vx = 0x181;
            work->unk128 = 1;
            work->loopMotion12C = 0;
            ex->Hi_SetSummonMotion(((Fx0e2Summon *)env->loader)->summon54->motion180[1], ((Fx0e2Summon *)env->loader)->summonModel60);
            break;
        }
        switch (frame) {
        case 0x2E:
            work->emit0 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA6), (s32)&fx_0e2_emitPos0);
            if (p != NULL) {
                p->otZ = -0x100;
            }
            break;
        case 0x41:
            work->emit4 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA5), (s32)&fx_0e2_emitPos0);
            if (p != NULL) {
                p->otZ = -0x100;
            }
            break;
        case 0x42:
            ex->btlfx_stopEmitter(work->emit0);
            break;
        case 0x9B:
            ex->btlfx_stopEmitter(work->emit4);
            break;
        case 0xA3:
            work->emit1C = p2 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA4), (s32)&fx_0e2_emitPos2);
            if (p2 != NULL) {
                p2->trails = (s32)fx_0e2_op80Trails1;
                work->emit1C->trailSlots = 0xC;
                work->emit1C->trailStride = 0x50;
                if (work->emit1C != NULL) {
                    work->emit1C->otZ = 0x100;
                }
            }
            break;
        case 0xB3:
            work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA3), (s32)&fx_0e2_emitPos1);
            work->emit24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA2), (s32)&fx_0e2_emitPos1);
            break;
        case 0xB5:
            for (i2 = 0; i2 < 5; i2++) {
                work->emit28[i2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_0e2_emitBlocks1[i2]), (s32)&fx_0e2_emitPos4[i2]);
            }
            break;
        case 0xF0:
            for (i2 = 0; i2 < 5; i2++) {
                ex->btlfx_stopEmitter(work->emit28[i2]);
            }
            break;
        case 0xF8:
            for (i2 = 0; i2 < 5; i2++) {
                work->emit8[i2] = p3 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_0e2_emitBlocks0[i2]), (s32)&fx_0e2_emitPos3[i2]);
                if (p3 != NULL) {
                    p3->trails = (s32)&fx_0e2_op80Trails0[i2];
                    work->emit8[i2]->trailSlots = 6;
                    work->emit8[i2]->trailStride = 0x50;
                }
            }
            break;
        case 0xA1:
            work->fadeTimer140 = 0x10;
            work->fadeInTimer144 = 0x10;
            break;
        case 0xB1:
            work->fadeOutTimer148 = 0xC;
            break;
        case 0x10E:
            break;
        }
        switch (frame) {
        case 0x97:
            ex->Hi_FreeEffModel(work->model150[8]);
            ex->Hi_FreeEffModel(work->model150[9]);
            ex->Hi_FreeEffModel(work->model150[10]);
            break;
        case 0xBD:
            ex->Hi_FreeEffModel(work->model150[0]);
            ex->Hi_FreeEffModel(work->model150[1]);
            ex->Hi_FreeEffModel(work->model150[2]);
            ex->Hi_FreeEffModel(work->model150[3]);
            break;
        case 0xF0:
            ex->Hi_FreeEffModel(work->model150[11]);
            ex->Hi_FreeEffModel(work->model150[12]);
            ex->Hi_FreeEffModel(work->model150[13]);
            break;
        }
        if (frame < 0x10E) {
            work->pos44.vx = env->camera->points[2].vx;
            work->pos44.vy = env->camera->points[2].vy;
            work->pos44.vz = env->camera->points[2].vz;
            work->pos54.vx = env->camera->points[8].vx;
            work->pos54.vy = env->camera->points[8].vy;
            work->pos54.vz = env->camera->points[8].vz;
            fx_0e2_stripOrigin.vx = work->pos54.vx;
            fx_0e2_stripOrigin.vy = work->pos54.vy;
            fx_0e2_stripOrigin.vz = work->pos54.vz;
            work->posC0.vx = env->camera->points[3].vx;
            work->posC0.vy = env->camera->points[3].vy;
            work->posC0.vz = env->camera->points[3].vz;
        }
        n = frame - 0x2E;
        if (n >= 0 && n < 0x83) {
            ex->Hi_DrawSummonModel(&work->rot3C, &work->pos44, (s32)&work->scale64, ((Fx0e2Summon *)env->loader)->summonModel60,
                                   work->loopMotion12C);
            if (n >= 0 && n < 0x14) {
                ex->Hi_ModifySummonModelRGB(((Fx0e2Summon *)env->loader)->summonModel60, -0x20, -0x20, -0x20);
            }
            if (n >= 0x69) {
                ex->Hi_ModifySummonModelAbr(((Fx0e2Summon *)env->loader)->summonModel60, 1);
            }
            if (work->fadeTimer140 >= 0) {
                work->summonRgb74 = fxm_lerpRatio(0, -0x80, 0x10 - work->fadeTimer140, 0x10);
                work->fadeTimer140--;
                ex->Hi_ModifySummonModelRGB(((Fx0e2Summon *)env->loader)->summonModel60, work->summonRgb74, work->summonRgb74, work->summonRgb74);
            }
        }
        if (work->fadeInTimer144 >= 0) {
            work->rgb78 = fxm_lerpRatio(-0x80, 0, 0x10 - work->fadeInTimer144, 0x10);
            work->fadeInTimer144--;
        }
        if (work->fadeOutTimer148 >= 0) {
            work->rgb78 = fxm_lerpRatio(0, -0x80, 0xC - work->fadeOutTimer148, 0xC);
            work->fadeOutTimer148--;
        }
        if (frame >= 0xA1 && frame < 0xBD) {
            if (frame - 0xA1 >= 0x14) {
                t = 0;
            } else if (frame - 0xA1 >= 8) {
                t = 0x1000 - (work->morphStep138++ << 12) / 12;
            } else {
                t = 0x1000;
            }
            ex->Hi_DrawMorphEffModel(&work->rot3C, (VECTOR *)&work->pos54, (s32)&work->scale64, work->model150[0],
                                     work->model150[1], t);
            ex->Hi_ModifyEffModelAbr(work->model150[0], 1);
            if (work->fadeOutTimer148 >= 0 || work->fadeInTimer144 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->model150[0], work->rgb78, work->rgb78, work->rgb78);
            }
        }
        if (frame >= 0xA1 && frame < 0xBD) {
            if (frame - 0xA1 >= 0x18) {
                t = 0;
            } else if (frame - 0xA1 >= 0xC) {
                t = 0x1000 - (work->morphStep13C++ << 12) / 12;
            } else {
                t = 0x1000;
            }
            ex->Hi_DrawMorphEffModel(&work->rot3C, (VECTOR *)&work->pos54, (s32)&work->scale64, work->model150[2],
                                     work->model150[3], t);
            ex->Hi_ModifyEffModelAbr(work->model150[2], 1);
            if (work->fadeOutTimer148 >= 0 || work->fadeInTimer144 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->model150[2], work->rgb78, work->rgb78, work->rgb78);
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
                fx_0e2_stripBlendFade = f << 8;
            } else {
                fx_0e2_stripBlendFade = 0x1000;
            }
            for (i5 = 0; i5 < 5; i5++) {
                fxm_rotPosPathInit(&fx_0e2_stripBlend.pathA0,
                              (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, fx_0e2_stripTrackBlocks[a]), (u16)i5));
                fxm_rotPosPathInit(&fx_0e2_stripBlendPathB,
                              (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, fx_0e2_stripTrackBlocks[b]), (u16)i5));
                fx_0e2_stripFromTrackBlend(&fx_0e2_stripBlend.pathA0, &fx_0e2_stripBlendPathB, c, (Fx0e2Trail *)&fx_0e2_strips[i5], 0x98, fx_0e2_stripBlend.width16);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x17), 0, NULL, 0, fx_0e2_stripBlend.scale14,
                                    fx_0e2_stripBlend.fade12, 1, (u8 *)&fx_0e2_strips[i5], 0);
                fx_0e2_headRot.vx = 0x113;
                fx_0e2_headRot.vy = 0x800;
                fx_0e2_headRot.vz = 0;
                work->pos84.vx = fx_0e2_headPos.vx;
                work->pos84.vy = fx_0e2_headPos.vy;
                work->pos84.vz = fx_0e2_headPos.vz;
                fx_0e2_stripHeads[i5].vx = fx_0e2_headPos.vx;
                fx_0e2_stripHeads[i5].vy = fx_0e2_headPos.vy;
                fx_0e2_stripHeads[i5].vz = fx_0e2_headPos.vz;
                work->rot7C.vx = fx_0e2_headRot.vx;
                work->rot7C.vy = fx_0e2_headRot.vy;
                work->rot7C.vz = fx_0e2_headRot.vz;
                work->scale94.vz = work->scale94.vy = work->scale94.vx = f * 0xD7;
                if (f < 0xC) {
                    t = 0x1000;
                } else if (f >= 0x14) {
                    t = 0;
                } else {
                    t = 0x1000 - ((f - 0xC) << 9);
                }
                ex->Hi_DrawMorphEffModel(&work->rot7C, &work->pos84, (s32)&work->scale94, work->model150[4], work->model150[5],
                                         t);
                work->rgbB4 = -((0x13 - f) * 6);
                ex->Hi_ModifyEffModelRGB(work->model150[4], work->rgbB4, work->rgbB4, work->rgbB4);
                if (frame >= 0xB5) {
                    if (work->emit28[i5] != NULL && work->emit28[i5]->state == BTLFX_EMIT_DONE) {
                        work->emit28[i5] = NULL;
                    }
                    ex->btlfx_moveEmitterTo((s32)work->emit28[i5], (s32)&fx_0e2_stripHeads[i5]);
                }
            }
        }
        if (frame >= 0xCD && frame < 0xF0) {
            k = frame - 0xCD;
            fx_0e2_stripPath0.base8 = k;
            fx_0e2_stripPath0.base8 = fx_0e2_stripPath0.base8 >= 0x23 ? 0x22 : fx_0e2_stripPath0.base8;
            for (i6 = 0; i6 < 5; i6++) {
                fxm_posPathInit(&fx_0e2_stripPath0.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x9E), (u16)i6));
                fxm_posPathGet(&fx_0e2_stripPath0.path0, k + 0x37, &fx_0e2_headPos);
                fx_0e2_headRot.vx = 0x113;
                fx_0e2_headRot.vy = 0x800;
                fx_0e2_headRot.vz = 0;
                work->pos84.vx = fx_0e2_headPos.vx;
                work->pos84.vy = fx_0e2_headPos.vy;
                work->pos84.vz = fx_0e2_headPos.vz;
                fx_0e2_stripHeads[i6].vx = fx_0e2_headPos.vx;
                fx_0e2_stripHeads[i6].vy = fx_0e2_headPos.vy;
                fx_0e2_stripHeads[i6].vz = fx_0e2_headPos.vz;
                work->rot7C.vx = fx_0e2_headRot.vx;
                work->rot7C.vy = fx_0e2_headRot.vy;
                work->rot7C.vz = fx_0e2_headRot.vz;
                ex->Hi_DrawMorphEffModel(&work->rot7C, &work->pos84, (s32)&work->scale94, work->model150[4], work->model150[5],
                                         0);
                if (work->emit28[i6] != NULL && work->emit28[i6]->state == BTLFX_EMIT_DONE) {
                    work->emit28[i6] = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit28[i6], (s32)&fx_0e2_stripHeads[i6]);
            }
        }
        if (frame >= 0xF0 && frame < 0x10F) {
            fx_0e2_stripPath1.countA = frame - 0xF0;
            if (fx_0e2_stripPath1.countA >= 0x12) {
                fx_0e2_stripPath1.base8 = frame - 0x101;
                fx_0e2_stripPath1.countA = 0x11;
            }
            for (i7 = 0; i7 < 5; i7++) {
                fxm_posPathInit(&fx_0e2_stripPath1.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x9D), (u16)i7));
                fx_0e2_stripFromPosPath(&fx_0e2_stripPath1.path0, (Fx0e2Trail *)&fx_0e2_strips[i7], 0x98, fx_0e2_stripPath1.base8, fx_0e2_stripPath1.countA,
                              fx_0e2_stripPath1.width10);
                fx_0e2_headRot.vx = 0x113;
                fx_0e2_headRot.vy = 0x800;
                fx_0e2_headRot.vz = 0;
                work->pos84.vx = fx_0e2_headPos.vx;
                work->pos84.vy = fx_0e2_headPos.vy;
                work->pos84.vz = fx_0e2_headPos.vz;
                fx_0e2_stripHeads[i7].vx = fx_0e2_headPos.vx;
                fx_0e2_stripHeads[i7].vy = fx_0e2_headPos.vy;
                fx_0e2_stripHeads[i7].vz = fx_0e2_headPos.vz;
                work->rot7C.vx = fx_0e2_headRot.vx;
                work->rot7C.vy = fx_0e2_headRot.vy;
                work->rot7C.vz = fx_0e2_headRot.vz;
                ex->Hi_DrawEffModel(&work->rot7C, &work->pos84, (s32)&work->scale94, work->model150[6]);
                ex->Hi_DrawEffModel(&work->rot7C, &work->pos84, (s32)&work->scale94, work->model150[7]);
                fxm_drawScrollMoveX(work->scrollSpeed14E * 0x14A - work->scrollStep14C * work->scrollSpeed14E, 0x1C0, 0x180, 0x80, 0x40, 0x1C0, 0x1C0);
                work->scrollStep14C++;
                if (frame >= 0xF8) {
                    if (work->emit8[i7] != NULL && work->emit8[i7]->state == BTLFX_EMIT_DONE) {
                        work->emit8[i7] = NULL;
                    }
                    ex->btlfx_moveEmitterTo((s32)work->emit8[i7], (s32)&fx_0e2_stripHeads[i7]);
                }
            }
        }
        m = frame - 0x2E;
        if (m >= 0 && m < 0x69 && !(frame >= 0x97 && frame < 0xF0)) {
            if (m < 0x14) {
                ex->Hi_DrawEffModel(&work->rotB8, &work->posC0, (s32)&work->scaleD0, work->model150[8]);
                ex->Hi_DrawEffModel(&work->rotB8, &work->posC0, (s32)&work->scaleD0, work->model150[9]);
            } else {
                ex->Hi_DrawEffModel(&work->rotB8, &work->posC0, (s32)&work->scaleD0, work->model150[10]);
            }
        }
        if (frame >= 0x97 && frame < 0xF0) {
            for (dr = fx_0e2_drops, j = 0; j < 3; j++, dr++) {
                for (k = 0; k < 2; k++) {
                    work->posE8.vx = dr->pos0[k][0];
                    work->posE8.vy = dr->pos0[k][1];
                    work->posE8.vz = dr->pos0[k][2];
                    work->posE8.vx += work->pos44.vx;
                    if (j == 2) {
                        work->posE8.vz -= 0x7530;
                    } else {
                        work->posE8.vy -= 0xC18;
                        work->posE8.vz -= 0x4E20;
                    }
                    ex->Hi_DrawEffModel(&work->rotE0, &work->posE8, (s32)&work->scaleF8, work->model150[j + 11]);
                    dr->pos0[k][0] += dr->vel30[0];
                    dr->pos0[k][1] += dr->vel30[1];
                    dr->pos0[k][2] += dr->vel30[2];
                    if (dr->pos0[k][2] < -0x4E20) {
                        dr->pos0[k][0] = dr->start20[0];
                        dr->pos0[k][1] = dr->start20[1];
                        dr->pos0[k][2] = dr->start20[2];
                    }
                }
            }
        }
        if (frame >= 1 && frame < 0x2E) {
            for (i8 = 1; i8 < 4; i8++) {
                switch (i8) {
                case 0:
                    fx_0e2_blockPos[i8].vx = env->camera->points[2].vx;
                    fx_0e2_blockPos[i8].vy = env->camera->points[2].vy;
                    fx_0e2_blockPos[i8].vz = env->camera->points[2].vz;
                    break;
                case 1:
                    fx_0e2_blockPos[i8].vx = env->camera->points[3].vx;
                    fx_0e2_blockPos[i8].vy = env->camera->points[3].vy;
                    fx_0e2_blockPos[i8].vz = env->camera->points[3].vz;
                    break;
                case 2:
                    fx_0e2_blockPos[i8].vx = env->camera->points[8].vx;
                    fx_0e2_blockPos[i8].vy = env->camera->points[8].vy;
                    fx_0e2_blockPos[i8].vz = env->camera->points[8].vz;
                    break;
                case 3:
                    fx_0e2_blockPos[i8].vx = env->camera->points[9].vx;
                    fx_0e2_blockPos[i8].vy = env->camera->points[9].vy;
                    fx_0e2_blockPos[i8].vz = env->camera->points[9].vz;
                    break;
                }
            }
        }
        if (frame >= 0x2E) {
            work->emitPos110.vy = -0x2C9E;
            work->emitPos110.vx = 0;
            work->emitPos110.vz = -0x729A;
            if (work->emit0 != NULL && work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit0, (s32)&work->emitPos110);
        }
        if (frame >= 0x41) {
            work->emitPos110.vy = -0x303D;
            work->emitPos110.vx = 0;
            work->emitPos110.vz = -0x7411;
            if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->emitPos110);
        }
        if (frame >= 0xA3) {
            work->emitPos118.vy = 0x128;
            work->emitPos118.vx = 0;
            work->emitPos118.vz = 0x3EB;
            work->emitPos118.vx += work->pos54.vx;
            work->emitPos118.vy += work->pos54.vy;
            work->emitPos118.vz += work->pos54.vz;
            if (work->emit1C != NULL && work->emit1C->state == BTLFX_EMIT_DONE) {
                work->emit1C = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit1C, (s32)&work->emitPos118);
        }
        if (frame >= 0xB3) {
            work->emitPos120.vx = fx_0e2_stripHeads[0].vx;
            work->emitPos120.vy = fx_0e2_stripHeads[0].vy;
            work->emitPos120.vz = fx_0e2_stripHeads[0].vz;
            if (work->emit20 != NULL && work->emit20->state == BTLFX_EMIT_DONE) {
                work->emit20 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit20, (s32)&work->emitPos120);
            if (work->emit24 != NULL && work->emit24->state == BTLFX_EMIT_DONE) {
                work->emit24 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit24, (s32)&work->emitPos120);
        }
        fxm_camChanUpdate(0);
        return frame >= 0x14A;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_headerPtr);

s32 fx_0e2_op81(s32 mode, Fx0e2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
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
        *args = 0x13C;
    } else if (mode == 1) {
        fx_0e2_env = env;
        work->pos1C.vx = -0x18D;
        work->pos1C.vz = -0x25E;
        work->rot14.vx = 0;
        work->rot14.vy = 0;
        work->rot14.vz = 0;
        work->pos1C.vy = 0;
        work->scale2C.vx = 0x1000;
        work->scale2C.vy = 0x1000;
        work->scale2C.vz = 0x1000;
        for (i = 0; i < 3; i++) {
            work->model124[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0e2_towerBlocks[i]), 1);
        }
        work->rot3C.vx = 0;
        work->rot3C.vy = 0;
        work->rot3C.vz = 0;
        work->pos44.vx = 0;
        work->pos44.vy = 0;
        work->pos44.vz = 0;
        work->scale54.vx = 0x1000;
        work->scale54.vy = 0x1000;
        work->scale54.vz = 0x1000;
        for (i = 0; i < 5; i++) {
            fxm_rotPosPathInit(&fx_0e2_pieceTracks[i], (u16 *)fxm_offsetTableEntry(ex->btlfx_getFileData(entry->file, 0x9C), (u16)i));
            work->model124[i + 3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0e2_pieceBlocks[i]), 1);
        }
        for (i = 0; i < 4; i++) {
            setVector(&work->blockRot64[i], 0, 0, 0);
            setVector(&work->blockPos84[i], 0, 0, 0);
            setVector(&work->blockScaleC4[i], 0x1000, 0x1000, 0x1000);
            work->model124[i + 8] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x80), 1);
        }
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x300, 0x180, 0x80, 0x80, fx_0e2_bgBuf, -0x18D, -0x300, 0x25E);
            break;
        case 3:
            work->emitPos104[0].vx = work->pos44.vx;
            work->emitPos104[0].vy = work->pos44.vy;
            work->emitPos104[0].vz = work->pos44.vz;
            work->emit0[0] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA0), (s32)&work->emitPos104[0]);
            if (p != NULL) {
                p->otZ = 0x100;
            }
            break;
        case 16:
            work->emit0[1] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA0), (s32)&work->emitPos104[0]);
            if (p != NULL) {
                p->otZ = 0x100;
            }
            break;
        case 30:
            work->emitPos104[1].vx = 0x20C;
            work->emitPos104[1].vy = 0;
            work->emitPos104[1].vz = -0x3B6;
            work->emit0[2] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA0), (s32)&work->emitPos104[1]);
            if (p != NULL) {
                p->otZ = 0x100;
            }
            break;
        case 35:
            work->emitPos104[2].vx = -0x205;
            work->emitPos104[2].vy = 0;
            work->emitPos104[2].vz = -0x5D9;
            work->emit0[3] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA0), (s32)&work->emitPos104[2]);
            if (p != NULL) {
                p->otZ = 0x100;
            }
            break;
        case 38:
            work->emitPos104[3].vx = 0x110;
            work->emitPos104[3].vy = 0;
            work->emitPos104[3].vz = -0x703;
            work->emit0[4] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA0), (s32)&work->emitPos104[3]);
            if (p != NULL) {
                p->otZ = 0x100;
            }
            break;
        case 46:
            ex->btlfx_stopEmitter(work->emit0[0]);
            for (i3 = 0, pp = work->emit0; i3 < 4; pp++, i3++) {
                ex->btlfx_stopEmitter(pp[1]);
            }
            break;
        }
        if ((u32)(frame - 1) < 0x2D) {
            if (frame - 1 >= 0x12) {
                m = work->model124[2];
            } else if (frame - 1 >= 5) {
                m = work->model124[1];
            } else {
                m = work->model124[0];
            }
            ex->Hi_SetEffModelSlice(m, work->pos1C.vy);
            ex->Hi_DrawSliceEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, m);
            ex->Hi_ModifyEffModelRGB(m, 6, 6, 6);
        }
        if ((u32)(frame - 1) < 0x2D) {
            n = frame - 1;
            for (i4 = 0; i4 < 5; i4++) {
                fxm_rotPosPathGet(&fx_0e2_pieceTracks[i4], n, &work->rot3C, &v);
                work->pos44.vx = v.vx;
                work->pos44.vy = v.vy;
                work->pos44.vz = v.vz;
                work->pos44.vx -= 0x18D;
                work->pos44.vz -= 0x25E;
                ex->Hi_SetEffModelSlice(work->model124[i4 + 3], work->pos44.vy);
                ex->Hi_DrawSliceEffModel(&work->rot3C, &work->pos44, (s32)&work->scale54, work->model124[i4 + 3]);
            }
        }
        if ((u32)(frame - 1) < 0x2D) {
            for (i5 = 1; i5 < 4; i5++) {
                setVector(&work->blockPos84[i5], fx_0e2_blockPos[i5].vx, fx_0e2_blockPos[i5].vy, fx_0e2_blockPos[i5].vz);
                ex->Hi_SetEffModelSlice(work->model124[i5 + 8], work->blockPos84[i5].vy);
                ex->Hi_DrawSliceEffModel(&work->blockRot64[i5], &work->blockPos84[i5], (s32)&work->blockScaleC4[i5], work->model124[i5 + 8]);
            }
        }
        return frame >= 0x30;
    }
    return 0;
}

s32 fx_0e2_op82(s32 mode, Fx0e2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
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
        *args = 0x6C;
    } else if (mode == 1) {
        work->center50.vx = env->targets->centerX;
        fx_0e2_env = env;
        work->center50.vy = env->targets->centerY;
        work->center50.vz = env->targets->centerZ;
        work->center50.vy -= 0x3E8;
        fx_0e2_ribbon0.width1E = 0x200;
        fx_0e2_ribbon0.unk18 = 0;
        fx_0e2_ribbon0.length1A = 0;
        fx_0e2_ribbon0.bright1C = 0x80;
        fx_0e2_ribbon0.blend20 = 0;
        fx_0e2_ribbon0.rotY22 = 0;
        fx_0e2_ribbon1.width1E = 0x180;
        fx_0e2_ribbon1.unk18 = 0;
        fx_0e2_ribbon1.length1A = 0;
        fx_0e2_ribbon1.bright1C = 0x80;
        fx_0e2_ribbon1.blend20 = 0;
        fx_0e2_ribbon1.rotY22 = 0;
        fx_0e2_ribbon3.unk10 = 0;
        fx_0e2_ribbon3.length12 = 0;
        fx_0e2_ribbon3.bright14 = 0x80;
        fx_0e2_ribbon3.width16 = 0x400;
        fx_0e2_ribbon3.rotY18 = 0;
        fx_0e2_ribbon2.length1A = 0x20;
        fx_0e2_ribbon2.unk18 = 0;
        fx_0e2_ribbon2.bright1C = 0x80;
        fx_0e2_ribbon2.width1E = 0x400;
        fx_0e2_ribbon2.blend20 = 0;
        fx_0e2_ribbon2.rotY22 = 0;
        setVector(&fx_0e2_ribbon0.scale10, 0x1000, 0x1000, 0x1000);
        setVector(&fx_0e2_ribbon1.scale10, 0x1000, 0x1000, 0x1000);
        setVector(&fx_0e2_ribbon3.scale8, 0x1000, 0x1000, 0x1000);
        setVector(&fx_0e2_ribbon2.scale10, 0x1000, 0x1000, 0x1000);
        setVector(&work->rotC, 0, 0, 0);
        setVector(&work->rot14, 0, 0, 0);
        setVector(&work->pos1C, 0, 0, 0x4B0);
        setVector(&work->scale2C, 0x1CCC, 0x1000, 0x1CCC);
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x11), mode);
        work->model6A = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x1D), mode);
        work->fadeTimer58 = -1;
        work->fadeTimer5C = -1;
        work->fadeTimer60 = mode;
        work->scrollStep64 = 0;
        work->scrollSpeed66 = 0xC;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->pos40.vx = 0;
            work->pos40.vy = 0;
            work->pos40.vz = 0x4B0;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos40);
            break;
        case 1:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos40);
            break;
        case 6:
            work->pos48.vx = work->pos40.vx;
            work->pos48.vy = work->pos40.vy;
            work->pos48.vz = work->pos40.vz;
            work->pos48.vy -= 0xC80;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos48);
            break;
        case 0x1C:
            work->fadeTimer58 = 0x20;
            work->fadeTimer5C = 0x20;
            work->fadeTimer60 = 0x20;
            break;
        }
        if (frame >= 0 && frame < 60) {
            fx_0e2_ribbon0.length1A += 6;
            n = fx_0e2_ribbon0.length1A;
            if (n > 60) {
                n = 60;
            }
            fx_0e2_ribbon0.length1A = n;
            fx_0e2_ribbon0.blend20 = (frame % 60 % 10) * 0x199;
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
            if (fx_0e2_ribbon0.length1A >= 60) {
                fx_0e2_ribbon0.scale10.vy = 0x1000;
                fx_0e2_ribbon0.scale10.vx += 0x80;
                fx_0e2_ribbon0.scale10.vz += 0x80;
            } else {
                fx_0e2_ribbon0.scale10.vx = fx_0e2_ribbon0.scale10.vy = fx_0e2_ribbon0.scale10.vz =
                    (s16)(fx_0e2_ribbon0.length1A / 10) * 0x199 + 0x800;
            }
            if (work->fadeTimer58 >= 0) {
                fx_0e2_ribbon0.bright1C = fxm_lerpRatio(0x80, 0, 0x20 - work->fadeTimer58, 0x20);
                work->fadeTimer58--;
            }
            for (i = 0; i < 6; i++) {
                fxm_posPathInit(&fx_0e2_ribbon0.pathA0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 9), a + i));
                fxm_posPathInit(&fx_0e2_ribbon0.pathB8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 9), b + i));
                fxm_projectEdgesBlend(&fx_0e2_ribbon0.pathA0, &fx_0e2_ribbon0.pathB8, fx_0e2_ribbon0.unk18, fx_0e2_ribbon0.length1A, &fx_0e2_ribbon0.scale10,
                              fx_0e2_ribbon0.width1E, fx_0e2_ribbon0.blend20, fx_0e2_ribbon0.rotY22, 1);
                fxm_drawEdgeQuads(fx_0e2_ribbon0.length1A, fx_0e2_ribbon0.bright1C, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 0 && frame < 60) {
            t = frame;
            fx_0e2_ribbon1.length1A += 6;
            m = fx_0e2_ribbon1.length1A;
            if (m > 60) {
                m = 60;
            }
            fx_0e2_ribbon1.length1A = m;
            if (fx_0e2_ribbon1.length1A >= 60) {
                fx_0e2_ribbon1.scale10.vy = 0x1000;
                fx_0e2_ribbon1.scale10.vx += 0x80;
                fx_0e2_ribbon1.scale10.vz += 0x80;
            } else {
                fx_0e2_ribbon1.scale10.vx = fx_0e2_ribbon1.scale10.vy = fx_0e2_ribbon1.scale10.vz =
                    (s16)(fx_0e2_ribbon1.length1A / 10) * 0x199 + 0x800;
            }
            if (work->fadeTimer5C >= 0) {
                fx_0e2_ribbon1.bright1C = fxm_lerpRatio(0x80, 0, 0x20 - work->fadeTimer5C, 0x20);
                work->fadeTimer5C--;
            }
            k = t;
            k %= 30;
            fx_0e2_ribbon1.blend20 = (k % 10) * 0x199;
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
                fxm_posPathInit(&fx_0e2_ribbon1.pathA0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xC), a2 + i));
                fxm_posPathInit(&fx_0e2_ribbon1.pathB8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xC), b2 + i));
                fxm_projectEdgesBlend(&fx_0e2_ribbon1.pathA0, &fx_0e2_ribbon1.pathB8, fx_0e2_ribbon1.unk18, fx_0e2_ribbon1.length1A, &fx_0e2_ribbon1.scale10,
                              fx_0e2_ribbon1.width1E, fx_0e2_ribbon1.blend20, fx_0e2_ribbon1.rotY22, 1);
                fxm_drawEdgeQuads(fx_0e2_ribbon1.length1A, fx_0e2_ribbon1.bright1C, 0x9C, 0x3D40, 0x80, 0xBF);
            }
        }
        if (frame >= 0 && frame < 16) {
            fx_0e2_ribbon3.length12 += 2;
            m = fx_0e2_ribbon3.length12;
            if (m > 0x20) {
                m = 0x20;
            }
            fx_0e2_ribbon3.length12 = m;
            for (i = 0; i < 5; i++) {
                if (i & 1) {
                    fxm_posPathInit(&fx_0e2_ribbon3.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 1), 0));
                } else {
                    fxm_posPathInit(&fx_0e2_ribbon3.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xF), 0));
                }
                fx_0e2_ribbon3.rotY18 = i * 0x333;
                fxm_projectEdges(&fx_0e2_ribbon3.path0, fx_0e2_ribbon3.unk10, fx_0e2_ribbon3.length12, &fx_0e2_ribbon3.scale8, fx_0e2_ribbon3.width16,
                              fx_0e2_ribbon3.rotY18);
                fxm_drawEdgeQuads(fx_0e2_ribbon3.length12, fx_0e2_ribbon3.bright14, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 16 && frame < 60) {
            x = frame - 16;
            /* separate fresh variables: the compares use r, the % 8 uses c */
            c = x % 24;
            r = c;
            fx_0e2_ribbon2.blend20 = (c % 8) << 9;
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
                fx_0e2_ribbon2.scale10.vx += 0x80;
                fx_0e2_ribbon2.scale10.vz += 0x80;
            }
            if (work->fadeTimer58 >= 0) {
                fx_0e2_ribbon2.bright1C = fxm_lerpRatio(0x80, 0, 0x20 - work->fadeTimer58, 0x20);
                work->fadeTimer58--;
            }
            for (j = 0; j < 5; j++) {
                if (j & 1) {
                    fxm_posPathInit(&fx_0e2_ribbon2.pathA0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 1), a3));
                    fxm_posPathInit(&fx_0e2_ribbon2.pathB8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 1), b3));
                } else {
                    fxm_posPathInit(&fx_0e2_ribbon2.pathA0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xF), a3));
                    fxm_posPathInit(&fx_0e2_ribbon2.pathB8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xF), b3));
                }
                fx_0e2_ribbon2.rotY22 = j * 0x333;
                fxm_projectEdgesBlend(&fx_0e2_ribbon2.pathA0, &fx_0e2_ribbon2.pathB8, fx_0e2_ribbon2.unk18, fx_0e2_ribbon2.length1A, &fx_0e2_ribbon2.scale10,
                              fx_0e2_ribbon2.width1E, fx_0e2_ribbon2.blend20, fx_0e2_ribbon2.rotY22, 0);
                fxm_drawEdgeQuads(fx_0e2_ribbon2.length1A, fx_0e2_ribbon2.bright1C, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 8 && frame < 60) {
            ex->Hi_DrawEffModel(&work->rotC, &work->pos1C, (s32)&work->scale2C, work->model68);
            ex->Hi_DrawEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, work->model6A);
            if (work->fadeTimer60 >= 0) {
                work->rgb3C = fxm_lerpRatio(0, -0x80, 0x20 - work->fadeTimer60, 0x20);
                work->fadeTimer60--;
                ex->Hi_ModifyEffModelRGB(work->model68, work->rgb3C, work->rgb3C, work->rgb3C);
                ex->Hi_ModifyEffModelRGB(work->model6A, work->rgb3C, work->rgb3C, work->rgb3C);
            }
        }
        fxm_drawScrollMoveX(work->scrollSpeed66 * 0x78 - work->scrollStep64 * work->scrollSpeed66, 0x1C0, 0x180, 0x80, 0x40, 0x1C0, 0x1C0);
        fxm_drawScrollMoveX(work->scrollSpeed66 * 0x78 - work->scrollStep64 * work->scrollSpeed66, 0x300, 0x180, 0x80, 0x40, 0x300, 0x140);
        work->scrollStep64++;
        if (frame >= 0x78) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_0e2_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR d;
    Fx0e2Orbit *o;
    s32 i;
    s32 j;
    Fx0e2Orbit *q;
    s32 frame;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        o = fx_0e2_orbits;
        work->vx = 0;
        work->vy = 0;
        work->vz = 0x4B0;
        fx_0e2_env = env;
        for (i = 0; i < env->targets->targetCount; i++, o++) {
            ex->btlfx_getUnitPos(i, (s16 *)&o->pos0);
            ex->btlfx_getUnitRot(i, (s16 *)&o->rot8);
            ex->btlfx_aimAngles((s16 *)work, (s16 *)&o->pos0, (s16 *)&a);
            o->angle12 = a.vy;
            d.vx = work->vx - o->pos0.vx;
            d.vy = work->vy - o->pos0.vy;
            d.vz = work->vz - o->pos0.vz;
            o->dist10 = ex->btlfx_sqrt(d.vx * d.vx + d.vz * d.vz, 0x14);
        }
    } else {
        frame = entry->frame;
        if (frame >= 0 && frame <= 0x40) {
            for (j = 0, q = fx_0e2_orbits; j < env->targets->targetCount; j++, q++) {
                a.vx = work->vx + q->dist10 * ex->math_sin(q->angle12) / 4096;
                a.vy = q->pos0.vy - frame * 120;
                a.vz = work->vz + q->dist10 * ex->math_cos(q->angle12) / 4096;
                ex->btlfx_setUnitPos(j, (s16 *)&a);
                ex->btlfx_aimAngles((s16 *)work, (s16 *)&a, (s16 *)&d);
                d.vx = 0;
                d.vz = 0;
                ex->btlfx_setUnitRot(j, (s16 *)&d);
                q->angle12 += 0x100;
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

s32 fx_0e2_op84(s32 mode, Fx0e2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    Fx0e2Orbit *o;

    if (mode == 0) {
        *args = 0xD0;
    } else if (mode == 1) {
        o = fx_0e2_orbits;
        fx_0e2_env = env;
        for (i = 0; i < env->targets->targetCount; i++, o++) {
            work->landedC0[i] = 0;
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->unitPos40[i]);
            work->unitPos40[i].vy = -0xBB8;
            ex->btlfx_setUnitPos(i, (s16 *)&work->unitPos40[i]);
            ex->btlfx_setUnitRot(i, (s16 *)&o->rot8);
        }
    } else {
        frame = entry->frame;
        for (j = 0; j < env->targets->targetCount; j++) {
            if (work->landedC0[j] == 0 && fxm_moveUnitHome(j, frame, 4, &work->unitPos40[j]) != 0) {
                work->landedC0[j] = 1;
                ex->btlfx_getUnitHomePos(j, (s16 *)&work->unitPos40[j]);
                ex->btlfx_setUnitPos(j, (s16 *)&work->unitPos40[j]);
                work->emitPos80[j].vx = work->unitPos40[j].vx;
                (work->emitPos80 + j)->vy = (work->unitPos40 + j)->vy;
                (work->emitPos80 + j)->vz = (work->unitPos40 + j)->vz;
                work->emit0[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->emitPos80[j]);
                work->emit20[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->emitPos80[j]);
                if (work->emit0[j] != NULL) {
                    work->emit0[j]->trails = (s32)fx_0e2_op84Trails[j];
                    work->emit0[j]->trailSlots = 0xC;
                    work->emit0[j]->trailStride = 0x30;
                }
                if (work->emit20[j] != NULL) {
                    work->emit20[j]->otZ = ex->btlfx_addRadiusSixth(j, 0x80);
                }
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_0e2_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    s32 frame;

    if (mode == 0) {
        *args = 2;
    } else if (mode == 1) {
        fx_0e2_env = env;
        *work = -1;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            *work = 0x10;
        }
        if (*work >= 0) {
            fxm_tintScreenLerp(0xFF, 0, 0x10 - *work, 0x10, 1, 0);
            (*work)--;
        }
        return frame >= 0x10;
    }
    return 0;
}

static s32 fxm_offsetTableEntry(u8 *base, u16 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0e2_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posPathInit(Fx0e2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx0e2Point *)data;
}

static s32 fxm_posPathGet(Fx0e2Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx0e2Point *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->keys4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

static void fxm_rotPosPathInit(Fx0e2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx0e2Point *)data;
}

static s32 fxm_rotPosPathGet(Fx0e2Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx0e2Key *k;
    s32 r = 1;

    if (i > path->count2 - 1) {
        i = path->count2 - 1;
    } else {
        r = 0;
    }
    k = &path->keys4[i];
    if (rot != NULL) {
        rot->vx = k->rot6[0];
        rot->vy = k->rot6[1];
        rot->vz = k->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = k->pos0[0];
        pos->vy = k->pos0[1];
        pos->vz = k->pos0[2];
    }
    return r;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_0e2_env)->btlfx_drawScreenTint((u32 *)(fx_0e2_env->ot + ot), abr, c, c, c);
}

static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e2_env);
    SVECTOR a;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(a0, (s16 *)&a);
    fxm_lerpSvec(b, &a, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(a0, (s16 *)&out);
    return t >= n;
}

static void fxm_camChanReset(void) {
    s32 i;
    s32 m = FX_0E2_KEY_FREE;

    for (i = 63; i >= 0; i--) {
        fx_0e2_camTracks[i].left6 = m;
    }
    for (i = 3; i >= 0; i--) {
        fx_0e2_camChanFrame[i] = 0;
    }
}

static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (fx_0e2_camTracks[i].left6 == FX_0E2_KEY_FREE) {
            fx_0e2_camTracks[i].left6 = 0;
            fx_0e2_camTracks[i].data0 = data;
            fx_0e2_camTracks[i].start4 = start;
            fx_0e2_camTracks[i].dstA = dst;
            fx_0e2_camTracks[i].stepC = 0;
            fx_0e2_camTracks[i].stepsE = c;
            fx_0e2_camTracks[i].chan8 = ch;
            return;
        }
    }
}

static void fxm_camChanUpdate(s32 ch) {
    Unk800F78A4 *env = fx_0e2_env;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (fx_0e2_camTracks[i].chan8 == ch) {
            if (fx_0e2_camTracks[i].left6 == 0 && fx_0e2_camTracks[i].start4 == fx_0e2_camChanFrame[ch]) {
                fx_0e2_camTracks[i].left6 = *fx_0e2_camTracks[i].data0;
                fx_0e2_camTracks[i].data0++;
            }
            if (fx_0e2_camTracks[i].left6 > 0) {
                fx_0e2_camTracks[i].left6--;
                for (k = 0; k < 3; k++) {
                    d = ((fx_0e2_camTracks[i].dstA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->camera)[d].vx = fx_0e2_camTracks[i].data0[0];
                        ((SVECTOR *)env->camera)[d].vy = fx_0e2_camTracks[i].data0[1];
                        ((SVECTOR *)env->camera)[d].vz = fx_0e2_camTracks[i].data0[2];
                        if (fx_0e2_camTracks[i].stepC < fx_0e2_camTracks[i].stepsE) {
                            fx_0e2_camTracks[i].data0 += 3;
                            fx_0e2_camTracks[i].stepC++;
                        }
                    }
                }
            }
        }
    }
    fx_0e2_camChanFrame[ch]++;
}

static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = fx_0e2_env;
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
