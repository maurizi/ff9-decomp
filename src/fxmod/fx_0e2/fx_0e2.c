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
    s32 blend;

    n = (size - 4) / 8;
    pt = trail->pts4;
    trail->lastSlot0 = trail->count2 = n - 1;
    trail->head1 = 0;
    blend = t;
    for (i = 0; i < n; i++, pt++) {
        fxm_rotPosPathGet(trackA, i, &rotA, &posA);
        fxm_rotPosPathGet(trackB, i, &rotB, &posB);
        ex->btlfx_lerpVec((s16 *)&posA, (s16 *)&posB, blend, (s16 *)&pos);
        ex->btlfx_lerpVec((s16 *)&rotA, (s16 *)&rotB, blend, (s16 *)&rot);
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
    s32 step;
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
        step = i + 1;
        fxm_posPathGet(track, count - step + base, &pos);
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
    SVECTOR ofs;
    SVECTOR *pt;
    s32 n;
    s32 i;
    s32 key;

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
        key = count - i + base;
        fxm_rotPosPathGet(trackA, key, NULL, &a);
        fxm_rotPosPathGet(trackB, key, NULL, &b);
        ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, t, (s16 *)&ofs);
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = rotY;
        ofs.vx *= 6;
        ofs.vy *= 3;
        ofs.vz *= 6;
        ex->math_rotMatrixYXZ(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&ofs);
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

void fxm_projectEdgesBlend(void *trackA, void *trackB, s32 unused, s32 unusedLength, SVECTOR *scale, s32 width, s32 t, s32 rotY, s32 twist) {
    Unk800F78A4 *env = fx_0e2_env;
    Fx0e2Edge *edge = fx_0e2_edges;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR world;
    SVECTOR ends[2];
    SVECTOR rot;
    SVECTOR posA;
    SVECTOR posB;
    SVECTOR pos;
    s32 i;
    s16 roll;

    ends[0].vx = 0;
    ends[0].vy = -width / 2;
    ends[0].vz = 0;
    ends[1].vx = 0;
    ends[1].vy = width / 2;
    ends[1].vz = 0;
    for (i = 0; i < FX_FENRIR_EDGES; i++, edge++) {
        fxm_posPathGet(trackA, i, &posA);
        fxm_posPathGet(trackB, i, &posB);
        ex->btlfx_lerpVec((s16 *)&posA, (s16 *)&posB, t, (s16 *)&pos);
        pos.vx = pos.vx * scale->vx / 4096;
        pos.vy = pos.vy * scale->vy / 4096;
        pos.vz = pos.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = rotY;
        ex->math_rotMatrixYXZ(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&pos);
        gte_rtv0tr();
        gte_stlvnl(&world);
        pos.vx = world.vx;
        pos.vy = world.vy;
        pos.vz = world.vz + 0x4B0;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&pos);
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
                roll = rot.vz;
                if (roll > 0x400) {
                    roll = 0x400;
                }
                rot.vz = roll;
                if (pos.vz > 0) {
                    rot.vz = -roll;
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
        gte_stsxy(&edge->xy0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&edge->xy4);
        gte_stszotz(&edge->otz8);
    }
}

void fxm_projectEdges(void *track, s32 unused, s32 unusedLength, SVECTOR *scale, s32 width, s32 rotY) {
    Unk800F78A4 *env = fx_0e2_env;
    Fx0e2Edge *edge = fx_0e2_edges;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR world;
    SVECTOR ends[2];
    SVECTOR rot;
    SVECTOR pos;
    s32 i;

    ends[0].vx = 0;
    ends[0].vy = -width / 2;
    ends[0].vz = 0;
    ends[1].vx = 0;
    ends[1].vy = width / 2;
    ends[1].vz = 0;
    for (i = 0; i < FX_FENRIR_EDGES; i++, edge++) {
        fxm_posPathGet(track, i, &pos);
        pos.vx = pos.vx * scale->vx / 4096;
        pos.vy = pos.vy * scale->vy / 4096;
        pos.vz = pos.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vy = rotY;
        rot.vz = 0;
        ex->math_rotMatrixYXZ(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&pos);
        gte_rtv0tr();
        gte_stlvnl(&world);
        pos.vx = world.vx;
        pos.vy = world.vy;
        pos.vz = world.vz + 0x4B0;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&pos);
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
        gte_stsxy(&edge->xy0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&edge->xy4);
        gte_stszotz(&edge->otz8);
    }
}

void fxm_drawEdgeQuads(s32 n, s32 bright, s32 tpage, s32 clut, u16 v0, u16 v1) {
    Fx0e2Edge *edge = fx_0e2_edges;
    POLY_GT4 *poly;
    BtlfxExports *ex;
    u16 tp;
    u16 cl;
    s32 i;
    s32 tile;
    s32 ua;
    s32 ub;
    s32 ca;
    s32 cb;
    s32 unused;

    tp = tpage | TPAGE_ABR_ADD;
    poly = (POLY_GT4 *)fx_0e2_env->prim;
    fx_0e2_env->prim = (u8 *)(poly + n);
    cl = clut;
    ex = BTLFX_EXPORTS(fx_0e2_env);
    for (i = 0; i < n - 1; i++, poly++, edge++) {
        poly->tpage = tp;
        poly->clut = cl;
        poly->code = GPU_POLY_GT4 | GPU_ABE;
        poly->x0 = edge[0].xy0.vx;
        poly->y0 = edge[0].xy0.vy;
        poly->x1 = edge[0].xy4.vx;
        poly->y1 = edge[0].xy4.vy;
        poly->x2 = edge[1].xy0.vx;
        poly->y2 = edge[1].xy0.vy;
        poly->x3 = edge[1].xy4.vx;
        poly->y3 = edge[1].xy4.vy;
        poly->v0 = poly->v2 = v0;
        poly->v1 = poly->v3 = v1;
        tile = i % 8;
        ua = tile << 5;
        ub = (tile + 1) << 5;
        if (ua == 0x100) ua = 0xFF;
        if (ub == 0x100) ub = 0xFF;
        poly->u2 = poly->u3 = ub;
        poly->u0 = poly->u1 = ua;
        if (i < 8) {
            ca = i * 16;
            cb = (i + 1) * 16;
            if (bright < ca) {
                ca = bright;
            }
            if (bright < cb) {
                cb = bright;
            }
            setRGB0(poly, ca, ca, ca);
            setRGB1(poly, ca, ca, ca);
            setRGB2(poly, cb, cb, cb);
            setRGB3(poly, cb, cb, cb);
        } else if (i >= 0x35) {
            cb = 0x80 - (i - 0x34) * 16;
            ca = 0x80 - (i - 0x33) * 16;
            if (bright < cb) {
                cb = bright;
            }
            if (bright < ca) {
                ca = bright;
            }
            setRGB0(poly, cb, cb, cb);
            setRGB1(poly, cb, cb, cb);
            setRGB2(poly, ca, ca, ca);
            setRGB3(poly, ca, ca, ca);
        } else {
            setRGB0(poly, bright, bright, bright);
            setRGB1(poly, bright, bright, bright);
            setRGB2(poly, bright, bright, bright);
            setRGB3(poly, bright, bright, bright);
        }
        if (edge[0].xy0.vx < edge[1].xy0.vx) {
            ex->btlfx_addPrim(PRIM_TAG(POLY_GT4), 0, poly);
        } else {
            ex->btlfx_addPrim(PRIM_TAG(POLY_GT4), (edge[0].otz8 + edge[1].otz8) >> 1, poly);
        }
    }
}

s32 fx_0e2_op80(s32 mode, Fx0e2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    Pool6CEntry *trailEmitter;
    Pool6CEntry *headEmitter;
    s32 summonFrame;
    s32 modelFrame;
    Fx0e2Drop *dr;
    Fx0e2Drop *dm;
    s32 frame;
    s32 drop;
    s32 slot;
    s32 blendStrip;
    s32 holdStrip;
    s32 pathStrip;
    s32 point;
    s32 j;
    s32 k;
    s32 keyA;
    s32 clamped;
    s32 keyB;
    s32 t;
    s32 blend;
    s32 stripFrame;

    if (mode == 0) {
        *args = sizeof(Fx0e2Work80);
    } else if (mode == 1) {
        fx_0e2_env = env;
        ex->btlfx_initEffModels(fx_0e2_effModels, FX_FENRIR_MODEL_COUNT);
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
        work->model150[0] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_BEAST), fx_0e2_texPages, fx_0e2_texPos);
        work->model150[1] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_SPINDLE), fx_0e2_texPages, fx_0e2_texPos);
        work->model150[2] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_SERPENT), fx_0e2_texPages, fx_0e2_texPos);
        work->model150[3] = ((Fx0e2RegTexList)ex->Hi_RegisterTexListModel)((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_STREAK), fx_0e2_texPages, fx_0e2_texPos);
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
        work->model150[4] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_BALL), 1);
        work->model150[5] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SPIKE_BURST), 1);
        work->model150[6] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_SHELL), 1);
        work->model150[7] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_VORTEX), 1);
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
        work->model150[8] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_ROCK_SHARD), 1);
        work->model150[9] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_ROCK_SPIKES), 1);
        work->model150[10] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_ROCK_SHARD_2), 1);
        work->rotE0.vx = 0;
        work->rotE0.vy = 0;
        work->rotE0.vz = 0;
        work->posE8.vx = 0;
        work->posE8.vy = 0;
        work->posE8.vz = 0;
        work->scaleF8.vx = 0x1000;
        work->scaleF8.vy = 0x1000;
        work->scaleF8.vz = 0x1000;
        for (drop = 0, dm = fx_0e2_drops; drop < FX_FENRIR_DROPS; drop++, dm++) {
            dm->start20[2] = dm->vel30[2] + FX_FENRIR_DROP_DEPTH;
        }
        for (drop = 0; drop < FX_FENRIR_DROPS; drop++) {
            work->model150[drop + FX_FENRIR_SUMMON_MODELS] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0e2_dropBlocks[drop]), 1);
        }
        work->scrollStep14C = 0;
        work->scrollSpeed14E = 4;
        fxm_camChanReset();
        fxm_camChanAdd(0, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_1), 0));
        fxm_camChanAdd(0, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_2), 0));
        fxm_camChanAdd(0, 3, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_START), 0));
        fxm_camChanAdd(0, 4, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_START), 1));
        fxm_camChanAdd(0, 9, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_START), 2));
        fxm_camChanAdd(0, 0xA, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_START), 3));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_2_FRAME, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_1), 1));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_2_FRAME, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_2), 1));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_2_FRAME, 3, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_3), 1));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_2_FRAME, 4, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_4), 1));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_2_FRAME, 9, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_9), 1));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_3_FRAME, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_1), 2));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_3_FRAME, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_2), 2));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_3_FRAME, 3, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_3), 2));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_3_FRAME, 4, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_4), 2));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_3_FRAME, 9, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_9), 2));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_4_FRAME, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_1), 4));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_4_FRAME, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_2), 4));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_4_FRAME, 3, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_3), 4));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_4_FRAME, 4, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_4), 4));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_4_FRAME, 9, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_9), 4));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_5_FRAME, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_1), 5));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_5_FRAME, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_2), 5));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_5_FRAME, 3, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_3), 5));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_5_FRAME, 4, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_4), 5));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_5_FRAME, 9, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_9), 5));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_6_FRAME, 1, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_1), 6));
        fxm_camChanAdd(FX_FENRIR_CAM_SHOT_6_FRAME, 2, 0, (s16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_2), 6));
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_FENRIR_SUMMON_FRAME:
            work->unk128 = 0;
            work->loopMotion12C = 0;
            ex->Hi_SetSummonMotion(((Fx0e2Summon *)env->loader)->summon54->motion180[0], ((Fx0e2Summon *)env->loader)->summonModel60);
            break;
        case FX_FENRIR_MOTION_1_FRAME:
            work->rot3C.vx = 0x181;
            work->unk128 = 1;
            work->loopMotion12C = 0;
            ex->Hi_SetSummonMotion(((Fx0e2Summon *)env->loader)->summon54->motion180[1], ((Fx0e2Summon *)env->loader)->summonModel60);
            break;
        }
        switch (frame) {
        case FX_FENRIR_SUMMON_FRAME:
            work->emit0 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_RED_MOON_GLOW), (s32)&fx_0e2_emitPos0);
            if (emitter != NULL) {
                emitter->otZ = -0x100;
            }
            break;
        case FX_FENRIR_RED_MOON_FRAME:
            work->emit4 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_RED_MOON), (s32)&fx_0e2_emitPos0);
            if (emitter != NULL) {
                emitter->otZ = -0x100;
            }
            break;
        case FX_FENRIR_RED_MOON_FRAME + 1:
            ex->btlfx_stopEmitter(work->emit0);
            break;
        case FX_FENRIR_RED_MOON_STOP_FRAME:
            ex->btlfx_stopEmitter(work->emit4);
            break;
        case FX_FENRIR_WIND_TRAIL_FRAME:
            work->emit1C = trailEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_TRAIL), (s32)&fx_0e2_emitPos2);
            if (trailEmitter != NULL) {
                trailEmitter->trails = (s32)fx_0e2_op80Trails1;
                work->emit1C->trailSlots = FX_FENRIR_WIND_TRAIL_TRAIL_SLOTS;
                work->emit1C->trailStride = FX_FENRIR_WIND_TRAIL_TRAIL_STRIDE;
                if (work->emit1C != NULL) {
                    work->emit1C->otZ = 0x100;
                }
            }
            break;
        case FX_FENRIR_RING_BURST_FRAME:
            work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_GREEN_RING_BURST_2), (s32)&fx_0e2_emitPos1);
            work->emit24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_GREEN_RING_BURST), (s32)&fx_0e2_emitPos1);
            break;
        case FX_FENRIR_HEAD_EMIT_FRAME:
            for (slot = 0; slot < FX_FENRIR_HEADS; slot++) {
                work->emit28[slot] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_0e2_emitBlocks1[slot]), (s32)&fx_0e2_emitPos4[slot]);
            }
            break;
        case FX_FENRIR_STRIPS_PATH_FRAME:
            for (slot = 0; slot < FX_FENRIR_HEADS; slot++) {
                ex->btlfx_stopEmitter(work->emit28[slot]);
            }
            break;
        case FX_FENRIR_HEAD_TRAILS_FRAME:
            for (slot = 0; slot < FX_FENRIR_HEADS; slot++) {
                work->emit8[slot] = headEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_0e2_emitBlocks0[slot]), (s32)&fx_0e2_emitPos3[slot]);
                if (headEmitter != NULL) {
                    headEmitter->trails = (s32)&fx_0e2_op80Trails0[slot];
                    work->emit8[slot]->trailSlots = FX_FENRIR_HEAD_TRAIL_SLOTS;
                    work->emit8[slot]->trailStride = FX_FENRIR_HEAD_TRAIL_STRIDE;
                }
            }
            break;
        case FX_FENRIR_MORPH_FRAME:
            work->fadeTimer140 = FX_FENRIR_SUMMON_FADE_FRAMES;
            work->fadeInTimer144 = FX_FENRIR_WIND_FADE_IN_FRAMES;
            break;
        case FX_FENRIR_STRIPS_FRAME:
            work->fadeOutTimer148 = FX_FENRIR_WIND_FADE_OUT_FRAMES;
            break;
        case FX_FENRIR_CAM_SHOT_6_FRAME:
            break;
        }
        switch (frame) {
        case FX_FENRIR_MOTION_1_FRAME:
            ex->Hi_FreeEffModel(work->model150[8]);
            ex->Hi_FreeEffModel(work->model150[9]);
            ex->Hi_FreeEffModel(work->model150[10]);
            break;
        case FX_FENRIR_MORPH_END_FRAME:
            ex->Hi_FreeEffModel(work->model150[0]);
            ex->Hi_FreeEffModel(work->model150[1]);
            ex->Hi_FreeEffModel(work->model150[2]);
            ex->Hi_FreeEffModel(work->model150[3]);
            break;
        case FX_FENRIR_STRIPS_PATH_FRAME:
            ex->Hi_FreeEffModel(work->model150[11]);
            ex->Hi_FreeEffModel(work->model150[12]);
            ex->Hi_FreeEffModel(work->model150[13]);
            break;
        }
        if (frame < FX_FENRIR_CAM_SHOT_6_FRAME) {
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
        summonFrame = frame - FX_FENRIR_SUMMON_FRAME;
        if (summonFrame >= 0 && summonFrame < FX_FENRIR_SUMMON_FRAMES) {
            ex->Hi_DrawSummonModel(&work->rot3C, &work->pos44, (s32)&work->scale64, ((Fx0e2Summon *)env->loader)->summonModel60,
                                   work->loopMotion12C);
            if (summonFrame >= 0 && summonFrame < FX_FENRIR_SUMMON_DIM_FRAMES) {
                ex->Hi_ModifySummonModelRGB(((Fx0e2Summon *)env->loader)->summonModel60, -0x20, -0x20, -0x20);
            }
            if (summonFrame >= FX_FENRIR_SUMMON_ADD_FRAME) {
                ex->Hi_ModifySummonModelAbr(((Fx0e2Summon *)env->loader)->summonModel60, BTLFX_ABR_ADD);
            }
            if (work->fadeTimer140 >= 0) {
                work->summonRgb74 = fxm_lerpRatio(0, -0x80, FX_FENRIR_SUMMON_FADE_FRAMES - work->fadeTimer140, FX_FENRIR_SUMMON_FADE_FRAMES);
                work->fadeTimer140--;
                ex->Hi_ModifySummonModelRGB(((Fx0e2Summon *)env->loader)->summonModel60, work->summonRgb74, work->summonRgb74, work->summonRgb74);
            }
        }
        if (work->fadeInTimer144 >= 0) {
            work->rgb78 = fxm_lerpRatio(-0x80, 0, FX_FENRIR_WIND_FADE_IN_FRAMES - work->fadeInTimer144, FX_FENRIR_WIND_FADE_IN_FRAMES);
            work->fadeInTimer144--;
        }
        if (work->fadeOutTimer148 >= 0) {
            work->rgb78 = fxm_lerpRatio(0, -0x80, FX_FENRIR_WIND_FADE_OUT_FRAMES - work->fadeOutTimer148, FX_FENRIR_WIND_FADE_OUT_FRAMES);
            work->fadeOutTimer148--;
        }
        if (frame >= FX_FENRIR_MORPH_FRAME && frame < FX_FENRIR_MORPH_END_FRAME) {
            if (frame - FX_FENRIR_MORPH_FRAME >= FX_FENRIR_BEAST_MORPH_FRAME + FX_FENRIR_WIND_MORPH_FRAMES) {
                t = 0;
            } else if (frame - FX_FENRIR_MORPH_FRAME >= FX_FENRIR_BEAST_MORPH_FRAME) {
                t = 0x1000 - (work->morphStep138++ << 12) / FX_FENRIR_WIND_MORPH_FRAMES;
            } else {
                t = 0x1000;
            }
            ex->Hi_DrawMorphEffModel(&work->rot3C, (VECTOR *)&work->pos54, (s32)&work->scale64, work->model150[0],
                                     work->model150[1], t);
            ex->Hi_ModifyEffModelAbr(work->model150[0], BTLFX_ABR_ADD);
            if (work->fadeOutTimer148 >= 0 || work->fadeInTimer144 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->model150[0], work->rgb78, work->rgb78, work->rgb78);
            }
        }
        if (frame >= FX_FENRIR_MORPH_FRAME && frame < FX_FENRIR_MORPH_END_FRAME) {
            if (frame - FX_FENRIR_MORPH_FRAME >= FX_FENRIR_SERPENT_MORPH_FRAME + FX_FENRIR_WIND_MORPH_FRAMES) {
                t = 0;
            } else if (frame - FX_FENRIR_MORPH_FRAME >= FX_FENRIR_SERPENT_MORPH_FRAME) {
                t = 0x1000 - (work->morphStep13C++ << 12) / FX_FENRIR_WIND_MORPH_FRAMES;
            } else {
                t = 0x1000;
            }
            ex->Hi_DrawMorphEffModel(&work->rot3C, (VECTOR *)&work->pos54, (s32)&work->scale64, work->model150[2],
                                     work->model150[3], t);
            ex->Hi_ModifyEffModelAbr(work->model150[2], BTLFX_ABR_ADD);
            if (work->fadeOutTimer148 >= 0 || work->fadeInTimer144 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->model150[2], work->rgb78, work->rgb78, work->rgb78);
            }
        }
        if (frame >= FX_FENRIR_STRIPS_FRAME && frame < FX_FENRIR_STRIPS_HOLD_FRAME) {
            stripFrame = frame - FX_FENRIR_STRIPS_FRAME;
            keyA = 0;
            if (stripFrame >= FX_FENRIR_STRIP_KEYS_FRAME) {
                keyA = (frame - (FX_FENRIR_STRIPS_FRAME + FX_FENRIR_STRIP_KEYS_FRAME)) / 2;
                keyB = keyA + 1;
                blend = ((frame - (FX_FENRIR_STRIPS_FRAME + FX_FENRIR_STRIP_KEYS_FRAME)) & 1) << 11;
            } else {
                keyB = 0;
                blend = 0;
            }
            if (keyA < FX_FENRIR_STRIP_KEYS) {
                clamped = keyA;
            } else {
                clamped = FX_FENRIR_STRIP_KEYS - 1;
            }
            keyA = clamped;
            if (keyB < FX_FENRIR_STRIP_KEYS) {
                clamped = keyB;
            } else {
                clamped = FX_FENRIR_STRIP_KEYS - 1;
            }
            keyB = clamped;
            if (stripFrame < FX_FENRIR_STRIP_FADE_IN_FRAMES) {
                fx_0e2_stripBlendFade = stripFrame << 8;
            } else {
                fx_0e2_stripBlendFade = 0x1000;
            }
            for (blendStrip = 0; blendStrip < FX_FENRIR_HEADS; blendStrip++) {
                fxm_rotPosPathInit(&fx_0e2_stripBlend.pathA0,
                              (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, fx_0e2_stripTrackBlocks[keyA]), (u16)blendStrip));
                fxm_rotPosPathInit(&fx_0e2_stripBlendPathB,
                              (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, fx_0e2_stripTrackBlocks[keyB]), (u16)blendStrip));
                fx_0e2_stripFromTrackBlend(&fx_0e2_stripBlend.pathA0, &fx_0e2_stripBlendPathB, blend, (Fx0e2Trail *)&fx_0e2_strips[blendStrip], sizeof(Fx0e2Trail98), fx_0e2_stripBlend.width16);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_GREEN_WIND_STRIP), 0, NULL, 0, fx_0e2_stripBlend.scale14,
                                    fx_0e2_stripBlend.fade12, BTLFX_ABR_ADD, (u8 *)&fx_0e2_strips[blendStrip], 0);
                fx_0e2_headRot.vx = 0x113;
                fx_0e2_headRot.vy = 0x800;
                fx_0e2_headRot.vz = 0;
                work->pos84.vx = fx_0e2_headPos.vx;
                work->pos84.vy = fx_0e2_headPos.vy;
                work->pos84.vz = fx_0e2_headPos.vz;
                fx_0e2_stripHeads[blendStrip].vx = fx_0e2_headPos.vx;
                fx_0e2_stripHeads[blendStrip].vy = fx_0e2_headPos.vy;
                fx_0e2_stripHeads[blendStrip].vz = fx_0e2_headPos.vz;
                work->rot7C.vx = fx_0e2_headRot.vx;
                work->rot7C.vy = fx_0e2_headRot.vy;
                work->rot7C.vz = fx_0e2_headRot.vz;
                work->scale94.vz = work->scale94.vy = work->scale94.vx = stripFrame * 0xD7;
                if (stripFrame < FX_FENRIR_BALL_MORPH_FRAME) {
                    t = 0x1000;
                } else if (stripFrame >= FX_FENRIR_BALL_MORPH_FRAME + FX_FENRIR_BALL_MORPH_FRAMES) {
                    t = 0;
                } else {
                    t = 0x1000 - ((stripFrame - FX_FENRIR_BALL_MORPH_FRAME) << 9);
                }
                ex->Hi_DrawMorphEffModel(&work->rot7C, &work->pos84, (s32)&work->scale94, work->model150[4], work->model150[5],
                                         t);
                work->rgbB4 = -((0x13 - stripFrame) * 6);
                ex->Hi_ModifyEffModelRGB(work->model150[4], work->rgbB4, work->rgbB4, work->rgbB4);
                if (frame >= FX_FENRIR_HEAD_EMIT_FRAME) {
                    if (work->emit28[blendStrip] != NULL && work->emit28[blendStrip]->state == BTLFX_EMIT_DONE) {
                        work->emit28[blendStrip] = NULL;
                    }
                    ex->btlfx_moveEmitterTo((s32)work->emit28[blendStrip], (s32)&fx_0e2_stripHeads[blendStrip]);
                }
            }
        }
        if (frame >= FX_FENRIR_STRIPS_HOLD_FRAME && frame < FX_FENRIR_STRIPS_PATH_FRAME) {
            k = frame - FX_FENRIR_STRIPS_HOLD_FRAME;
            fx_0e2_stripPath0.base8 = k;
            fx_0e2_stripPath0.base8 = fx_0e2_stripPath0.base8 >= 0x23 ? 0x22 : fx_0e2_stripPath0.base8;
            for (holdStrip = 0; holdStrip < FX_FENRIR_HEADS; holdStrip++) {
                fxm_posPathInit(&fx_0e2_stripPath0.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_STRIP_PATHS), (u16)holdStrip));
                fxm_posPathGet(&fx_0e2_stripPath0.path0, k + 0x37, &fx_0e2_headPos);
                fx_0e2_headRot.vx = 0x113;
                fx_0e2_headRot.vy = 0x800;
                fx_0e2_headRot.vz = 0;
                work->pos84.vx = fx_0e2_headPos.vx;
                work->pos84.vy = fx_0e2_headPos.vy;
                work->pos84.vz = fx_0e2_headPos.vz;
                fx_0e2_stripHeads[holdStrip].vx = fx_0e2_headPos.vx;
                fx_0e2_stripHeads[holdStrip].vy = fx_0e2_headPos.vy;
                fx_0e2_stripHeads[holdStrip].vz = fx_0e2_headPos.vz;
                work->rot7C.vx = fx_0e2_headRot.vx;
                work->rot7C.vy = fx_0e2_headRot.vy;
                work->rot7C.vz = fx_0e2_headRot.vz;
                ex->Hi_DrawMorphEffModel(&work->rot7C, &work->pos84, (s32)&work->scale94, work->model150[4], work->model150[5],
                                         0);
                if (work->emit28[holdStrip] != NULL && work->emit28[holdStrip]->state == BTLFX_EMIT_DONE) {
                    work->emit28[holdStrip] = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit28[holdStrip], (s32)&fx_0e2_stripHeads[holdStrip]);
            }
        }
        if (frame >= FX_FENRIR_STRIPS_PATH_FRAME && frame < FX_FENRIR_STRIPS_END_FRAME) {
            fx_0e2_stripPath1.countA = frame - FX_FENRIR_STRIPS_PATH_FRAME;
            if (fx_0e2_stripPath1.countA >= FX_FENRIR_STRIP_PATH_POINTS + 1) {
                fx_0e2_stripPath1.base8 = frame - (FX_FENRIR_STRIPS_PATH_FRAME + FX_FENRIR_STRIP_PATH_POINTS);
                fx_0e2_stripPath1.countA = FX_FENRIR_STRIP_PATH_POINTS;
            }
            for (pathStrip = 0; pathStrip < FX_FENRIR_HEADS; pathStrip++) {
                fxm_posPathInit(&fx_0e2_stripPath1.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_STRIP_PATHS_2), (u16)pathStrip));
                fx_0e2_stripFromPosPath(&fx_0e2_stripPath1.path0, (Fx0e2Trail *)&fx_0e2_strips[pathStrip], sizeof(Fx0e2Trail98), fx_0e2_stripPath1.base8, fx_0e2_stripPath1.countA,
                              fx_0e2_stripPath1.width10);
                fx_0e2_headRot.vx = 0x113;
                fx_0e2_headRot.vy = 0x800;
                fx_0e2_headRot.vz = 0;
                work->pos84.vx = fx_0e2_headPos.vx;
                work->pos84.vy = fx_0e2_headPos.vy;
                work->pos84.vz = fx_0e2_headPos.vz;
                fx_0e2_stripHeads[pathStrip].vx = fx_0e2_headPos.vx;
                fx_0e2_stripHeads[pathStrip].vy = fx_0e2_headPos.vy;
                fx_0e2_stripHeads[pathStrip].vz = fx_0e2_headPos.vz;
                work->rot7C.vx = fx_0e2_headRot.vx;
                work->rot7C.vy = fx_0e2_headRot.vy;
                work->rot7C.vz = fx_0e2_headRot.vz;
                ex->Hi_DrawEffModel(&work->rot7C, &work->pos84, (s32)&work->scale94, work->model150[6]);
                ex->Hi_DrawEffModel(&work->rot7C, &work->pos84, (s32)&work->scale94, work->model150[7]);
                fxm_drawScrollMoveX(work->scrollSpeed14E * FX_FENRIR_FRAMES - work->scrollStep14C * work->scrollSpeed14E, FX_FENRIR_RIBBON_VRAM_X,
                                    FX_FENRIR_RIBBON_VRAM_Y, FX_FENRIR_RIBBON_VRAM_W, FX_FENRIR_RIBBON_VRAM_H, FX_FENRIR_RIBBON_SRC_X, FX_FENRIR_RIBBON_SRC_Y);
                work->scrollStep14C++;
                if (frame >= FX_FENRIR_HEAD_TRAILS_FRAME) {
                    if (work->emit8[pathStrip] != NULL && work->emit8[pathStrip]->state == BTLFX_EMIT_DONE) {
                        work->emit8[pathStrip] = NULL;
                    }
                    ex->btlfx_moveEmitterTo((s32)work->emit8[pathStrip], (s32)&fx_0e2_stripHeads[pathStrip]);
                }
            }
        }
        modelFrame = frame - FX_FENRIR_SUMMON_FRAME;
        if (modelFrame >= 0 && modelFrame < FX_FENRIR_ROCKS_FRAMES && !(frame >= FX_FENRIR_MOTION_1_FRAME && frame < FX_FENRIR_STRIPS_PATH_FRAME)) {
            if (modelFrame < FX_FENRIR_SPIKES_FRAMES) {
                ex->Hi_DrawEffModel(&work->rotB8, &work->posC0, (s32)&work->scaleD0, work->model150[8]);
                ex->Hi_DrawEffModel(&work->rotB8, &work->posC0, (s32)&work->scaleD0, work->model150[9]);
            } else {
                ex->Hi_DrawEffModel(&work->rotB8, &work->posC0, (s32)&work->scaleD0, work->model150[10]);
            }
        }
        if (frame >= FX_FENRIR_MOTION_1_FRAME && frame < FX_FENRIR_STRIPS_PATH_FRAME) {
            for (dr = fx_0e2_drops, j = 0; j < FX_FENRIR_DROPS; j++, dr++) {
                for (k = 0; k < FX_FENRIR_DROP_COPIES; k++) {
                    work->posE8.vx = dr->pos0[k][0];
                    work->posE8.vy = dr->pos0[k][1];
                    work->posE8.vz = dr->pos0[k][2];
                    work->posE8.vx += work->pos44.vx;
                    if (j == 2) {
                        work->posE8.vz -= 0x7530;
                    } else {
                        work->posE8.vy -= 0xC18;
                        work->posE8.vz -= FX_FENRIR_DROP_DEPTH;
                    }
                    ex->Hi_DrawEffModel(&work->rotE0, &work->posE8, (s32)&work->scaleF8, work->model150[j + FX_FENRIR_SUMMON_MODELS]);
                    dr->pos0[k][0] += dr->vel30[0];
                    dr->pos0[k][1] += dr->vel30[1];
                    dr->pos0[k][2] += dr->vel30[2];
                    if (dr->pos0[k][2] < -FX_FENRIR_DROP_DEPTH) {
                        dr->pos0[k][0] = dr->start20[0];
                        dr->pos0[k][1] = dr->start20[1];
                        dr->pos0[k][2] = dr->start20[2];
                    }
                }
            }
        }
        if (frame >= 1 && frame < FX_FENRIR_SUMMON_FRAME) {
            for (point = 1; point < FX_FENRIR_TOWER_BLOCKS; point++) {
                switch (point) {
                case 0:
                    fx_0e2_blockPos[point].vx = env->camera->points[2].vx;
                    fx_0e2_blockPos[point].vy = env->camera->points[2].vy;
                    fx_0e2_blockPos[point].vz = env->camera->points[2].vz;
                    break;
                case 1:
                    fx_0e2_blockPos[point].vx = env->camera->points[3].vx;
                    fx_0e2_blockPos[point].vy = env->camera->points[3].vy;
                    fx_0e2_blockPos[point].vz = env->camera->points[3].vz;
                    break;
                case 2:
                    fx_0e2_blockPos[point].vx = env->camera->points[8].vx;
                    fx_0e2_blockPos[point].vy = env->camera->points[8].vy;
                    fx_0e2_blockPos[point].vz = env->camera->points[8].vz;
                    break;
                case 3:
                    fx_0e2_blockPos[point].vx = env->camera->points[9].vx;
                    fx_0e2_blockPos[point].vy = env->camera->points[9].vy;
                    fx_0e2_blockPos[point].vz = env->camera->points[9].vz;
                    break;
                }
            }
        }
        if (frame >= FX_FENRIR_SUMMON_FRAME) {
            work->emitPos110.vy = -0x2C9E;
            work->emitPos110.vx = 0;
            work->emitPos110.vz = -0x729A;
            if (work->emit0 != NULL && work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit0, (s32)&work->emitPos110);
        }
        if (frame >= FX_FENRIR_RED_MOON_FRAME) {
            work->emitPos110.vy = -0x303D;
            work->emitPos110.vx = 0;
            work->emitPos110.vz = -0x7411;
            if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->emitPos110);
        }
        if (frame >= FX_FENRIR_WIND_TRAIL_FRAME) {
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
        if (frame >= FX_FENRIR_RING_BURST_FRAME) {
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
        return frame >= FX_FENRIR_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e2/nonmatchings/fx_0e2", fx_0e2_headerPtr);

s32 fx_0e2_op81(s32 mode, Fx0e2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    Pool6CEntry **pp;
    SVECTOR piecePos;
    s32 frame;
    s32 key;
    s32 towerModel;
    s32 i;
    s32 slot;
    s32 piece;
    s32 block;

    if (mode == 0) {
        *args = sizeof(Fx0e2Work81);
    } else if (mode == 1) {
        fx_0e2_env = env;
        work->pos1C.vx = FX_FENRIR_TOWER_X;
        work->pos1C.vz = FX_FENRIR_TOWER_Z;
        work->rot14.vx = 0;
        work->rot14.vy = 0;
        work->rot14.vz = 0;
        work->pos1C.vy = 0;
        work->scale2C.vx = 0x1000;
        work->scale2C.vy = 0x1000;
        work->scale2C.vz = 0x1000;
        for (i = 0; i < FX_FENRIR_TOWER_STAGES; i++) {
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
        for (i = 0; i < FX_FENRIR_TOWER_PIECES; i++) {
            fxm_rotPosPathInit(&fx_0e2_pieceTracks[i], (u16 *)fxm_offsetTableEntry(ex->btlfx_getFileData(entry->file, FX_FENRIR_PIECE_TRACKS), (u16)i));
            work->model124[i + FX_FENRIR_TOWER_STAGES] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0e2_pieceBlocks[i]), 1);
        }
        for (i = 0; i < FX_FENRIR_TOWER_BLOCKS; i++) {
            setVector(&work->blockRot64[i], 0, 0, 0);
            setVector(&work->blockPos84[i], 0, 0, 0);
            setVector(&work->blockScaleC4[i], 0x1000, 0x1000, 0x1000);
            work->model124[i + FX_FENRIR_TOWER_STAGES + FX_FENRIR_TOWER_PIECES] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_ROCK_SPIKE), 1);
        }
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(FX_FENRIR_BG_VRAM_X, FX_FENRIR_BG_VRAM_Y, FX_FENRIR_BG_VRAM_W, FX_FENRIR_BG_VRAM_H, fx_0e2_bgBuf, -0x18D, -0x300, 0x25E);
            break;
        case FX_FENRIR_SMOKE_FRAME:
            work->emitPos104[0].vx = work->pos44.vx;
            work->emitPos104[0].vy = work->pos44.vy;
            work->emitPos104[0].vz = work->pos44.vz;
            work->emit0[0] = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SMOKE), (s32)&work->emitPos104[0]);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            break;
        case FX_FENRIR_SMOKE_2_FRAME:
            work->emit0[1] = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SMOKE), (s32)&work->emitPos104[0]);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            break;
        case FX_FENRIR_SMOKE_3_FRAME:
            work->emitPos104[1].vx = 0x20C;
            work->emitPos104[1].vy = 0;
            work->emitPos104[1].vz = -0x3B6;
            work->emit0[2] = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SMOKE), (s32)&work->emitPos104[1]);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            break;
        case FX_FENRIR_SMOKE_4_FRAME:
            work->emitPos104[2].vx = -0x205;
            work->emitPos104[2].vy = 0;
            work->emitPos104[2].vz = -0x5D9;
            work->emit0[3] = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SMOKE), (s32)&work->emitPos104[2]);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            break;
        case FX_FENRIR_SMOKE_5_FRAME:
            work->emitPos104[3].vx = 0x110;
            work->emitPos104[3].vy = 0;
            work->emitPos104[3].vz = -0x703;
            work->emit0[4] = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SMOKE), (s32)&work->emitPos104[3]);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            break;
        case FX_FENRIR_SMOKE_STOP_FRAME:
            ex->btlfx_stopEmitter(work->emit0[0]);
            for (slot = 0, pp = work->emit0; slot < FX_FENRIR_SMOKES - 1; pp++, slot++) {
                ex->btlfx_stopEmitter(pp[1]);
            }
            break;
        }
        if ((u32)(frame - 1) < FX_FENRIR_TOWER_FRAMES) {
            if (frame - 1 >= FX_FENRIR_TOWER_STAGE_3_FRAME) {
                towerModel = work->model124[2];
            } else if (frame - 1 >= FX_FENRIR_TOWER_STAGE_2_FRAME) {
                towerModel = work->model124[1];
            } else {
                towerModel = work->model124[0];
            }
            ex->Hi_SetEffModelSlice(towerModel, work->pos1C.vy);
            ex->Hi_DrawSliceEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, towerModel);
            ex->Hi_ModifyEffModelRGB(towerModel, 6, 6, 6);
        }
        if ((u32)(frame - 1) < FX_FENRIR_TOWER_FRAMES) {
            key = frame - 1;
            for (piece = 0; piece < FX_FENRIR_TOWER_PIECES; piece++) {
                fxm_rotPosPathGet(&fx_0e2_pieceTracks[piece], key, &work->rot3C, &piecePos);
                work->pos44.vx = piecePos.vx;
                work->pos44.vy = piecePos.vy;
                work->pos44.vz = piecePos.vz;
                work->pos44.vx += FX_FENRIR_TOWER_X;
                work->pos44.vz += FX_FENRIR_TOWER_Z;
                ex->Hi_SetEffModelSlice(work->model124[piece + FX_FENRIR_TOWER_STAGES], work->pos44.vy);
                ex->Hi_DrawSliceEffModel(&work->rot3C, &work->pos44, (s32)&work->scale54, work->model124[piece + FX_FENRIR_TOWER_STAGES]);
            }
        }
        if ((u32)(frame - 1) < FX_FENRIR_TOWER_FRAMES) {
            for (block = 1; block < FX_FENRIR_TOWER_BLOCKS; block++) {
                setVector(&work->blockPos84[block], fx_0e2_blockPos[block].vx, fx_0e2_blockPos[block].vy, fx_0e2_blockPos[block].vz);
                ex->Hi_SetEffModelSlice(work->model124[block + FX_FENRIR_TOWER_STAGES + FX_FENRIR_TOWER_PIECES], work->blockPos84[block].vy);
                ex->Hi_DrawSliceEffModel(&work->blockRot64[block], &work->blockPos84[block], (s32)&work->blockScaleC4[block], work->model124[block + FX_FENRIR_TOWER_STAGES + FX_FENRIR_TOWER_PIECES]);
            }
        }
        return frame >= FX_FENRIR_TOWER_OP_FRAMES;
    }
    return 0;
}

s32 fx_0e2_op82(s32 mode, Fx0e2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    s32 cycle;
    s32 keyA0;
    s32 keyB0;
    s32 phase;
    s32 tick;
    s32 ribbonFrame;
    s32 keyA1;
    s32 keyB1;
    s32 keyA2;
    s32 keyB2;
    s32 phaseCmp;
    s16 len0;
    s16 len;

    if (mode == 0) {
        *args = sizeof(Fx0e2Work82);
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
        fx_0e2_ribbon2.length1A = FX_FENRIR_SPIRAL_LENGTH;
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
        setVector(&work->pos1C, 0, 0, FX_FENRIR_WHIRL_Z);
        setVector(&work->scale2C, 0x1CCC, 0x1000, 0x1CCC);
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_RING), mode);
        work->model6A = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_RING_2), mode);
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
            work->pos40.vz = FX_FENRIR_WHIRL_Z;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_GREEN_RINGS_2), (s32)&work->pos40);
            break;
        case FX_FENRIR_GLINTS_FRAME:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_CYAN_GLINTS), (s32)&work->pos40);
            break;
        case FX_FENRIR_GREEN_RINGS_FRAME:
            work->pos48.vx = work->pos40.vx;
            work->pos48.vy = work->pos40.vy;
            work->pos48.vz = work->pos40.vz;
            work->pos48.vy -= 0xC80;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_GREEN_RINGS), (s32)&work->pos48);
            break;
        case FX_FENRIR_WHIRL_FADE_FRAME:
            work->fadeTimer58 = FX_FENRIR_WHIRL_FADE_FRAMES;
            work->fadeTimer5C = FX_FENRIR_WHIRL_FADE_FRAMES;
            work->fadeTimer60 = FX_FENRIR_WHIRL_FADE_FRAMES;
            break;
        }
        if (frame >= 0 && frame < FX_FENRIR_WHIRL_FRAMES) {
            fx_0e2_ribbon0.length1A += 6;
            len0 = fx_0e2_ribbon0.length1A;
            if (len0 > FX_FENRIR_EDGES) {
                len0 = FX_FENRIR_EDGES;
            }
            fx_0e2_ribbon0.length1A = len0;
            fx_0e2_ribbon0.blend20 = (frame % FX_FENRIR_RIBBON_0_CYCLE % FX_FENRIR_RIBBON_KEY_FRAMES) * 0x199;
            if (frame % FX_FENRIR_RIBBON_0_CYCLE < FX_FENRIR_RIBBON_KEY_FRAMES) {
                keyA0 = 0;
                keyB0 = 6;
            } else if (frame % FX_FENRIR_RIBBON_0_CYCLE < 2 * FX_FENRIR_RIBBON_KEY_FRAMES) {
                keyA0 = 6;
                keyB0 = 12;
            } else if (frame % FX_FENRIR_RIBBON_0_CYCLE < 3 * FX_FENRIR_RIBBON_KEY_FRAMES) {
                keyA0 = 12;
                keyB0 = 18;
            } else if (frame % FX_FENRIR_RIBBON_0_CYCLE < 4 * FX_FENRIR_RIBBON_KEY_FRAMES) {
                keyA0 = 18;
                keyB0 = 24;
            } else if (frame % FX_FENRIR_RIBBON_0_CYCLE < 5 * FX_FENRIR_RIBBON_KEY_FRAMES) {
                keyA0 = 24;
                keyB0 = 30;
            } else if (frame % FX_FENRIR_RIBBON_0_CYCLE < 6 * FX_FENRIR_RIBBON_KEY_FRAMES) {
                keyA0 = 30;
                keyB0 = 0;
            } else {
                keyA0 = 0;
                keyB0 = 0;
            }
            if (fx_0e2_ribbon0.length1A >= FX_FENRIR_EDGES) {
                fx_0e2_ribbon0.scale10.vy = 0x1000;
                fx_0e2_ribbon0.scale10.vx += 0x80;
                fx_0e2_ribbon0.scale10.vz += 0x80;
            } else {
                fx_0e2_ribbon0.scale10.vx = fx_0e2_ribbon0.scale10.vy = fx_0e2_ribbon0.scale10.vz =
                    (s16)(fx_0e2_ribbon0.length1A / FX_FENRIR_RIBBON_KEY_FRAMES) * 0x199 + 0x800;
            }
            if (work->fadeTimer58 >= 0) {
                fx_0e2_ribbon0.bright1C = fxm_lerpRatio(0x80, 0, FX_FENRIR_WHIRL_FADE_FRAMES - work->fadeTimer58, FX_FENRIR_WHIRL_FADE_FRAMES);
                work->fadeTimer58--;
            }
            for (i = 0; i < FX_FENRIR_RIBBON_0_STRANDS; i++) {
                fxm_posPathInit(&fx_0e2_ribbon0.pathA0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_A), keyA0 + i));
                fxm_posPathInit(&fx_0e2_ribbon0.pathB8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_A), keyB0 + i));
                fxm_projectEdgesBlend(&fx_0e2_ribbon0.pathA0, &fx_0e2_ribbon0.pathB8, fx_0e2_ribbon0.unk18, fx_0e2_ribbon0.length1A, &fx_0e2_ribbon0.scale10,
                              fx_0e2_ribbon0.width1E, fx_0e2_ribbon0.blend20, fx_0e2_ribbon0.rotY22, 1);
                fxm_drawEdgeQuads(fx_0e2_ribbon0.length1A, fx_0e2_ribbon0.bright1C, FX_FENRIR_RIBBON_TPAGE, FX_FENRIR_RIBBON_CLUT, FX_FENRIR_RIBBON_V0, FX_FENRIR_RIBBON_V1);
            }
        }
        if (frame >= 0 && frame < FX_FENRIR_WHIRL_FRAMES) {
            tick = frame;
            fx_0e2_ribbon1.length1A += 6;
            len = fx_0e2_ribbon1.length1A;
            if (len > FX_FENRIR_EDGES) {
                len = FX_FENRIR_EDGES;
            }
            fx_0e2_ribbon1.length1A = len;
            if (fx_0e2_ribbon1.length1A >= FX_FENRIR_EDGES) {
                fx_0e2_ribbon1.scale10.vy = 0x1000;
                fx_0e2_ribbon1.scale10.vx += 0x80;
                fx_0e2_ribbon1.scale10.vz += 0x80;
            } else {
                fx_0e2_ribbon1.scale10.vx = fx_0e2_ribbon1.scale10.vy = fx_0e2_ribbon1.scale10.vz =
                    (s16)(fx_0e2_ribbon1.length1A / FX_FENRIR_RIBBON_KEY_FRAMES) * 0x199 + 0x800;
            }
            if (work->fadeTimer5C >= 0) {
                fx_0e2_ribbon1.bright1C = fxm_lerpRatio(0x80, 0, FX_FENRIR_WHIRL_FADE_FRAMES - work->fadeTimer5C, FX_FENRIR_WHIRL_FADE_FRAMES);
                work->fadeTimer5C--;
            }
            cycle = tick;
            cycle %= FX_FENRIR_RIBBON_1_CYCLE;
            fx_0e2_ribbon1.blend20 = (cycle % FX_FENRIR_RIBBON_KEY_FRAMES) * 0x199;
            if (cycle < FX_FENRIR_RIBBON_KEY_FRAMES) {
                keyA1 = 0;
                keyB1 = 3;
            } else if (cycle < 2 * FX_FENRIR_RIBBON_KEY_FRAMES) {
                keyA1 = 3;
                keyB1 = 6;
            } else if (cycle < 3 * FX_FENRIR_RIBBON_KEY_FRAMES) {
                keyA1 = 6;
                keyB1 = 0;
            } else {
                keyA1 = 0;
                keyB1 = 0;
            }
            for (i = 0; i < FX_FENRIR_RIBBON_1_STRANDS; i++) {
                fxm_posPathInit(&fx_0e2_ribbon1.pathA0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_B), keyA1 + i));
                fxm_posPathInit(&fx_0e2_ribbon1.pathB8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_B), keyB1 + i));
                fxm_projectEdgesBlend(&fx_0e2_ribbon1.pathA0, &fx_0e2_ribbon1.pathB8, fx_0e2_ribbon1.unk18, fx_0e2_ribbon1.length1A, &fx_0e2_ribbon1.scale10,
                              fx_0e2_ribbon1.width1E, fx_0e2_ribbon1.blend20, fx_0e2_ribbon1.rotY22, 1);
                fxm_drawEdgeQuads(fx_0e2_ribbon1.length1A, fx_0e2_ribbon1.bright1C, FX_FENRIR_RIBBON_1_TPAGE, FX_FENRIR_RIBBON_1_CLUT, FX_FENRIR_RIBBON_V0, FX_FENRIR_RIBBON_1_V1);
            }
        }
        if (frame >= 0 && frame < FX_FENRIR_SPIRAL_FRAMES) {
            fx_0e2_ribbon3.length12 += 2;
            len = fx_0e2_ribbon3.length12;
            if (len > FX_FENRIR_SPIRAL_LENGTH) {
                len = FX_FENRIR_SPIRAL_LENGTH;
            }
            fx_0e2_ribbon3.length12 = len;
            for (i = 0; i < FX_FENRIR_RIBBON_COPIES; i++) {
                if (i & 1) {
                    fxm_posPathInit(&fx_0e2_ribbon3.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_C), 0));
                } else {
                    fxm_posPathInit(&fx_0e2_ribbon3.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_D), 0));
                }
                fx_0e2_ribbon3.rotY18 = i * 0x333;
                fxm_projectEdges(&fx_0e2_ribbon3.path0, fx_0e2_ribbon3.unk10, fx_0e2_ribbon3.length12, &fx_0e2_ribbon3.scale8, fx_0e2_ribbon3.width16,
                              fx_0e2_ribbon3.rotY18);
                fxm_drawEdgeQuads(fx_0e2_ribbon3.length12, fx_0e2_ribbon3.bright14, FX_FENRIR_RIBBON_TPAGE, FX_FENRIR_RIBBON_CLUT, FX_FENRIR_RIBBON_V0, FX_FENRIR_RIBBON_V1);
            }
        }
        if (frame >= FX_FENRIR_SPIRAL_FRAMES && frame < FX_FENRIR_WHIRL_FRAMES) {
            ribbonFrame = frame - FX_FENRIR_SPIRAL_FRAMES;
            /* separate fresh variables: the compares use phaseCmp, the % 8 uses phase */
            phase = ribbonFrame % FX_FENRIR_RIBBON_2_CYCLE;
            phaseCmp = phase;
            fx_0e2_ribbon2.blend20 = (phase % FX_FENRIR_RIBBON_2_KEY_FRAMES) << 9;
            if (phaseCmp < FX_FENRIR_RIBBON_2_KEY_FRAMES) {
                keyA2 = 0;
                keyB2 = 1;
            } else if (phaseCmp < 2 * FX_FENRIR_RIBBON_2_KEY_FRAMES) {
                keyA2 = 1;
                keyB2 = 2;
            } else if (phaseCmp < 3 * FX_FENRIR_RIBBON_2_KEY_FRAMES) {
                keyA2 = 2;
                keyB2 = 0;
            } else {
                keyA2 = 0;
                keyB2 = 0;
            }
            if (ribbonFrame >= 0) {
                fx_0e2_ribbon2.scale10.vx += 0x80;
                fx_0e2_ribbon2.scale10.vz += 0x80;
            }
            if (work->fadeTimer58 >= 0) {
                fx_0e2_ribbon2.bright1C = fxm_lerpRatio(0x80, 0, FX_FENRIR_WHIRL_FADE_FRAMES - work->fadeTimer58, FX_FENRIR_WHIRL_FADE_FRAMES);
                work->fadeTimer58--;
            }
            for (j = 0; j < FX_FENRIR_RIBBON_COPIES; j++) {
                if (j & 1) {
                    fxm_posPathInit(&fx_0e2_ribbon2.pathA0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_C), keyA2));
                    fxm_posPathInit(&fx_0e2_ribbon2.pathB8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_C), keyB2));
                } else {
                    fxm_posPathInit(&fx_0e2_ribbon2.pathA0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_D), keyA2));
                    fxm_posPathInit(&fx_0e2_ribbon2.pathB8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_D), keyB2));
                }
                fx_0e2_ribbon2.rotY22 = j * 0x333;
                fxm_projectEdgesBlend(&fx_0e2_ribbon2.pathA0, &fx_0e2_ribbon2.pathB8, fx_0e2_ribbon2.unk18, fx_0e2_ribbon2.length1A, &fx_0e2_ribbon2.scale10,
                              fx_0e2_ribbon2.width1E, fx_0e2_ribbon2.blend20, fx_0e2_ribbon2.rotY22, 0);
                fxm_drawEdgeQuads(fx_0e2_ribbon2.length1A, fx_0e2_ribbon2.bright1C, FX_FENRIR_RIBBON_TPAGE, FX_FENRIR_RIBBON_CLUT, FX_FENRIR_RIBBON_V0, FX_FENRIR_RIBBON_V1);
            }
        }
        if (frame >= FX_FENRIR_WIND_RING_FRAME && frame < FX_FENRIR_WHIRL_FRAMES) {
            ex->Hi_DrawEffModel(&work->rotC, &work->pos1C, (s32)&work->scale2C, work->model68);
            ex->Hi_DrawEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, work->model6A);
            if (work->fadeTimer60 >= 0) {
                work->rgb3C = fxm_lerpRatio(0, -0x80, FX_FENRIR_WHIRL_FADE_FRAMES - work->fadeTimer60, FX_FENRIR_WHIRL_FADE_FRAMES);
                work->fadeTimer60--;
                ex->Hi_ModifyEffModelRGB(work->model68, work->rgb3C, work->rgb3C, work->rgb3C);
                ex->Hi_ModifyEffModelRGB(work->model6A, work->rgb3C, work->rgb3C, work->rgb3C);
            }
        }
        fxm_drawScrollMoveX(work->scrollSpeed66 * FX_FENRIR_WHIRL_OP_FRAMES - work->scrollStep64 * work->scrollSpeed66, FX_FENRIR_RIBBON_VRAM_X,
                            FX_FENRIR_RIBBON_VRAM_Y, FX_FENRIR_RIBBON_VRAM_W, FX_FENRIR_RIBBON_VRAM_H, FX_FENRIR_RIBBON_SRC_X, FX_FENRIR_RIBBON_SRC_Y);
        fxm_drawScrollMoveX(work->scrollSpeed66 * FX_FENRIR_WHIRL_OP_FRAMES - work->scrollStep64 * work->scrollSpeed66, FX_FENRIR_RIBBON_1_VRAM_X,
                            FX_FENRIR_RIBBON_VRAM_Y, FX_FENRIR_RIBBON_VRAM_W, FX_FENRIR_RIBBON_VRAM_H, FX_FENRIR_RIBBON_1_VRAM_X, FX_FENRIR_RIBBON_1_SRC_Y);
        work->scrollStep64++;
        if (frame >= FX_FENRIR_WHIRL_OP_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_0e2_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR aim;
    SVECTOR delta;
    Fx0e2Orbit *orbit;
    s32 i;
    s32 j;
    Fx0e2Orbit *orb;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(SVECTOR);
    } else if (mode == 1) {
        orbit = fx_0e2_orbits;
        work->vx = 0;
        work->vy = 0;
        work->vz = FX_FENRIR_WHIRL_Z;
        fx_0e2_env = env;
        for (i = 0; i < env->targets->targetCount; i++, orbit++) {
            ex->btlfx_getUnitPos(i, (s16 *)&orbit->pos0);
            ex->btlfx_getUnitRot(i, (s16 *)&orbit->rot8);
            ex->btlfx_aimAngles((s16 *)work, (s16 *)&orbit->pos0, (s16 *)&aim);
            orbit->angle12 = aim.vy;
            delta.vx = work->vx - orbit->pos0.vx;
            delta.vy = work->vy - orbit->pos0.vy;
            delta.vz = work->vz - orbit->pos0.vz;
            orbit->dist10 = ex->btlfx_sqrt(delta.vx * delta.vx + delta.vz * delta.vz, 0x14);
        }
    } else {
        frame = entry->frame;
        if (frame >= 0 && frame <= FX_FENRIR_LIFT_FRAMES) {
            for (j = 0, orb = fx_0e2_orbits; j < env->targets->targetCount; j++, orb++) {
                aim.vx = work->vx + orb->dist10 * ex->math_sin(orb->angle12) / 4096;
                aim.vy = orb->pos0.vy - frame * FX_FENRIR_LIFT_SPEED;
                aim.vz = work->vz + orb->dist10 * ex->math_cos(orb->angle12) / 4096;
                ex->btlfx_setUnitPos(j, (s16 *)&aim);
                ex->btlfx_aimAngles((s16 *)work, (s16 *)&aim, (s16 *)&delta);
                delta.vx = 0;
                delta.vz = 0;
                ex->btlfx_setUnitRot(j, (s16 *)&delta);
                orb->angle12 += 0x100;
            }
        }
        return frame >= FX_FENRIR_LIFT_OP_FRAMES;
    }
    return 0;
}

s32 fx_0e2_op84(s32 mode, Fx0e2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    Fx0e2Orbit *orbit;

    if (mode == 0) {
        *args = sizeof(Fx0e2Work84);
    } else if (mode == 1) {
        orbit = fx_0e2_orbits;
        fx_0e2_env = env;
        for (i = 0; i < env->targets->targetCount; i++, orbit++) {
            work->landedC0[i] = 0;
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->unitPos40[i]);
            work->unitPos40[i].vy = -FX_FENRIR_DROP_HEIGHT;
            ex->btlfx_setUnitPos(i, (s16 *)&work->unitPos40[i]);
            ex->btlfx_setUnitRot(i, (s16 *)&orbit->rot8);
        }
    } else {
        frame = entry->frame;
        for (j = 0; j < env->targets->targetCount; j++) {
            if (work->landedC0[j] == 0 && fxm_moveUnitHome(j, frame, FX_FENRIR_DROP_FRAMES, &work->unitPos40[j]) != 0) {
                work->landedC0[j] = 1;
                ex->btlfx_getUnitHomePos(j, (s16 *)&work->unitPos40[j]);
                ex->btlfx_setUnitPos(j, (s16 *)&work->unitPos40[j]);
                work->emitPos80[j].vx = work->unitPos40[j].vx;
                (work->emitPos80 + j)->vy = (work->unitPos40 + j)->vy;
                (work->emitPos80 + j)->vz = (work->unitPos40 + j)->vz;
                work->emit0[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_GREEN_WIND_TRAIL), (s32)&work->emitPos80[j]);
                work->emit20[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SAND_RINGS), (s32)&work->emitPos80[j]);
                if (work->emit0[j] != NULL) {
                    work->emit0[j]->trails = (s32)fx_0e2_op84Trails[j];
                    work->emit0[j]->trailSlots = FX_FENRIR_GREEN_WIND_TRAIL_TRAIL_SLOTS;
                    work->emit0[j]->trailStride = FX_FENRIR_GREEN_WIND_TRAIL_TRAIL_STRIDE;
                }
                if (work->emit20[j] != NULL) {
                    work->emit20[j]->otZ = ex->btlfx_addRadiusSixth(j, 0x80);
                }
            }
        }
        return frame >= FX_FENRIR_LAND_FRAMES;
    }
    return 0;
}

s32 fx_0e2_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    s32 frame;

    if (mode == 0) {
        *args = sizeof(s16);
    } else if (mode == 1) {
        fx_0e2_env = env;
        *work = -1;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            *work = FX_FENRIR_WHITE_FADE_FRAMES;
        }
        if (*work >= 0) {
            fxm_tintScreenLerp(0xFF, 0, FX_FENRIR_WHITE_FADE_FRAMES - *work, FX_FENRIR_WHITE_FADE_FRAMES, BTLFX_ABR_ADD, 0);
            (*work)--;
        }
        return frame >= FX_FENRIR_WHITE_FADE_FRAMES;
    }
    return 0;
}

static s32 fxm_offsetTableEntry(u8 *base, u16 idx) {
    return (s32)base + ((s32 *)base)[(u16)idx + 1];
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0e2_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
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
    Fx0e2Point *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_rotPosPathInit(Fx0e2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx0e2Point *)data;
}

static s32 fxm_rotPosPathGet(Fx0e2Path_0b4 *path, s32 t, SVECTOR *rot, SVECTOR *pos) {
    Fx0e2Key *key;
    s32 done = 1;

    if (t > path->count2 - 1) {
        t = path->count2 - 1;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    if (rot != NULL) {
        rot->vx = key->rot6[0];
        rot->vy = key->rot6[1];
        rot->vz = key->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = key->pos0[0];
        pos->vy = key->pos0[1];
        pos->vz = key->pos0[2];
    }
    return done;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 bright = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_0e2_env)->btlfx_drawScreenTint((u32 *)(fx_0e2_env->ot + ot), abr, bright, bright, bright);
}

static s32 fxm_moveUnitHome(s32 unit, s32 t, s32 n, SVECTOR *start) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e2_env);
    SVECTOR home;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(unit, (s16 *)&home);
    fxm_lerpSvec(start, &home, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(unit, (s16 *)&out);
    return t >= n;
}

static void fxm_camChanReset(void) {
    s32 i;
    s32 freeMark = FX_0E2_KEY_FREE;

    for (i = 63; i >= 0; i--) {
        fx_0e2_camTracks[i].left6 = freeMark;
    }
    for (i = 3; i >= 0; i--) {
        fx_0e2_camChanFrame[i] = 0;
    }
}

static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 count;

    data++;
    count = *data;
    for (i = 0; i < FX_FENRIR_CAM_KEY_SLOTS; i++) {
        if (fx_0e2_camTracks[i].left6 == FX_0E2_KEY_FREE) {
            fx_0e2_camTracks[i].left6 = 0;
            fx_0e2_camTracks[i].data0 = data;
            fx_0e2_camTracks[i].start4 = start;
            fx_0e2_camTracks[i].dstA = dst;
            fx_0e2_camTracks[i].stepC = 0;
            fx_0e2_camTracks[i].stepsE = count;
            fx_0e2_camTracks[i].chan8 = ch;
            return;
        }
    }
}

static void fxm_camChanUpdate(s32 ch) {
    Unk800F78A4 *env = fx_0e2_env;
    s32 i;
    s32 k;
    s32 dstSlot;

    for (i = 0; i < FX_FENRIR_CAM_KEY_SLOTS; i++) {
        if (fx_0e2_camTracks[i].chan8 == ch) {
            if (fx_0e2_camTracks[i].left6 == 0 && fx_0e2_camTracks[i].start4 == fx_0e2_camChanFrame[ch]) {
                fx_0e2_camTracks[i].left6 = *fx_0e2_camTracks[i].data0;
                fx_0e2_camTracks[i].data0++;
            }
            if (fx_0e2_camTracks[i].left6 > 0) {
                fx_0e2_camTracks[i].left6--;
                for (k = 0; k < FX_FENRIR_CAM_KEY_TARGETS; k++) {
                    dstSlot = BTLFX_CAM_SLOT(fx_0e2_camTracks[i].dstA, k);
                    if (dstSlot >= 0) {
                        ((SVECTOR *)env->camera)[dstSlot].vx = fx_0e2_camTracks[i].data0[0];
                        ((SVECTOR *)env->camera)[dstSlot].vy = fx_0e2_camTracks[i].data0[1];
                        ((SVECTOR *)env->camera)[dstSlot].vz = fx_0e2_camTracks[i].data0[2];
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
    s32 wrapW = x % w;
    Unk800F78A4 *env = fx_0e2_env;
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
