#include "fxmod/fx_179.h"

static void fxm_shatterModel(s32 model, SVECTOR *center) {
    Fx179Tri *tri = fx_179_shatterTris;
    Fx179Tri *triCopy = fx_179_shatterTris;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_179_env);
    Fx179Shard *shard;
    SVECTOR dir;
    s32 i;

    ex->Hi_SplitMdlVertex(model, -1, (s32)fx_179_shatterTris);
    shard = fx_179_shatterPieces;
    for (i = 0; i < 160; i++, tri++, triCopy++, shard++) {
        shard->frame18 = 0;
        shard->pos0 = (tri->verts0[0].vx + triCopy->verts0[1].vx + triCopy->verts0[2].vx) / 3 << 12;
        shard->pos4 = (triCopy->verts0[0].vy + triCopy->verts0[1].vy + triCopy->verts0[2].vy) / 3 << 12;
        shard->pos8 = (triCopy->verts0[0].vz + triCopy->verts0[1].vz + triCopy->verts0[2].vz) / 3 << 12;
        shard->delay1A = 0;
        shard->unk12 = 0x1000;
        shard->rot14 = ex->btlfx_randCentered(0x100);
        shard->rot16 = ex->btlfx_randCentered(0x200);
        dir.vx = (shard->pos0 >> 12) - center->vx;
        dir.vy = (shard->pos4 >> 12) - center->vy;
        dir.vz = (shard->pos8 >> 12) - center->vz;
        shard->velC = dir.vx >> 4;
        shard->velE = (dir.vy >> 4) + 0x80;
        shard->vel10 = dir.vz >> 4;
    }
}

void fxm_updateShatterPieces(void) {
    Fx179Tri *tri = fx_179_shatterTris;
    Fx179Shard *shard = fx_179_shatterPieces;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR center;
    SVECTOR loc[3];
    s32 bias[2];
    VECTOR world;
    VECTOR sc;
    MATRIX m;
    s32 i;
    s32 j;

    bias[0] = 0;
    bias[1] = 0;
    ex = BTLFX_EXPORTS(fx_179_env);
    sc.vx = sc.vy = sc.vz = 0xF00;
    for (i = 0; i < 160; i++, shard++, tri++) {
        if (shard->frame18 >= 0) {
            if (shard->frame18 >= shard->delay1A) {
                center.vx = shard->pos0 >> 12;
                center.vy = shard->pos4 >> 12;
                center.vz = shard->pos8 >> 12;
                for (j = 0; j < 3; j++) {
                    loc[j].vx = tri->verts0[j].vx - center.vx;
                    loc[j].vy = tri->verts0[j].vy - center.vy;
                    loc[j].vz = tri->verts0[j].vz - center.vz;
                }
                ex->btlfx_stepModuleParticle((Pool6CEntry *)shard, -8, (SVECTOR *)bias, &out);
                rot.vx = shard->rot14;
                rot.vy = shard->rot16;
                rot.vz = 0;
                ex->math_rotMatrixYXZ(&rot, &m);
                ex->ScaleMatrix(&m, &sc);
                m.t[0] = shard->pos0 >> 12;
                m.t[1] = shard->pos4 >> 12;
                m.t[2] = shard->pos8 >> 12;
                ex->SetRotMatrix(&m);
                ex->SetTransMatrix(&m);
                for (j = 0; j < 3; j++) {
                    gte_ldv0(&loc[j]);
                    gte_rtv0tr();
                    gte_stlvnl(&world);
                    tri->verts0[j].vx = world.vx;
                    tri->verts0[j].vy = world.vy;
                    tri->verts0[j].vz = world.vz;
                }
                if (++shard->frame18 >= 0x20) {
                    shard->frame18 = -1;
                }
            } else {
                shard->frame18++;
            }
        }
    }
}

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_179_env);
    Fx179Rgbc colFlat;
    Fx179Rgbc colLit;
    Fx179Rgbc colDark;
    MATRIX m;
    SVECTOR quad[4];
    s32 otz;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 seg;
    s32 j;
    s32 row;
    s32 rowY;
    s32 y0;
    s32 y1;
    s32 bulge0;
    s32 bulge1;
    s32 rad0;
    s32 rad1;
    s32 ang1;
    s32 ft;
    s32 tile;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 nextJ;
    s32 nextSeg;

    gte_SetRotMatrix(fx_179_env->viewMatrix);
    gte_SetTransMatrix(fx_179_env->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)fx_179_env->prim;
    *(s32 *)&colDark = 0;
    ft4 = (POLY_FT4 *)(gt4 + 2);
    fx_179_env->prim = (u8 *)(ft4 + 14);
    colFlat.r = colFlat.g = colFlat.b = bright;
    colLit.r = colLit.g = colLit.b = bright;
    colFlat.code = abr != 0xFF ? 0x2E : 0x2C;
    colDark.code = abr != 0xFF ? 0x3E : 0x3C;
    tpage |= TPAGE_ABR(abr);
    colLit.code = colDark.code;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    seg = 0;
    rowY = 0;
    w0 >>= 1;
    w1 >>= 1;
    for (row = 0; row < 1; row++, rowY += dy) {
        for (j = 0; j < 16; j++, seg++, ang += dang << 8) {
            nextJ = (j + 1) << 8;
            nextSeg = (seg + 1) << 8;
            y0 = rowY + (((j << 8) * dy) >> 12);
            y1 = rowY + ((nextJ * dy) >> 12);
            bulge0 = ex->math_sin((seg << 8) >> 2);
            bulge1 = ex->math_sin(nextSeg >> 2);
            rad0 = ex->btlfx_lerp(r0, r1, bulge0);
            rad1 = ex->btlfx_lerp(r0, r1, bulge1);
            quad[0].vx = quad[1].vx = (ex->math_cos(ang) * rad0) >> 12;
            quad[0].vy = quad[1].vy = (ex->math_sin(ang) * rad0) >> 12;
            quad[0].vz = y0 - ex->btlfx_lerp(w0, w1, bulge0);
            quad[1].vz = y0 + ex->btlfx_lerp(w0, w1, bulge1);
            ang1 = ang + (dang << 8);
            quad[2].vx = quad[3].vx = (ex->math_cos(ang1) * rad1) >> 12;
            quad[2].vy = quad[3].vy = (ex->math_sin(ang1) * rad1) >> 12;
            quad[2].vz = y1 - ex->btlfx_lerp(w0, w1, bulge0);
            quad[3].vz = y1 + ex->btlfx_lerp(w0, w1, bulge1);
            gte_ldv3c(quad);
            gte_rtpt();
            tile = seg & 0xF;
            u0 = u + tile * 8;
            u1 = u0 + 8;
            if (tile == 15) {
                u1 = u0 + 7;
            }
            v0 = v;
            v1 = v0 + 0x1F;
            gte_nclip();
            if (seg == 0) {
                ft = 0;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colDark;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colLit;
            } else if (seg >= 15) {
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colDark;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colLit;
            } else {
                ft = 1;
                *(s32 *)&ft4->r0 = *(s32 *)&colFlat;
            }
            gte_stopz(&otz);
            if (otz != 0) {
                if (ft) {
                    ft4->u0 = ft4->u1 = u0;
                    ft4->u2 = ft4->u3 = u1;
                    ft4->v0 = ft4->v2 = v0;
                    ft4->v1 = ft4->v3 = v1;
                    ft4->tpage = tpage;
                    ft4->clut = clut;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                } else {
                    gt4->u0 = gt4->u1 = u0;
                    gt4->u2 = gt4->u3 = u1;
                    gt4->v0 = gt4->v2 = v0;
                    gt4->v1 = gt4->v3 = v1;
                    gt4->tpage = tpage;
                    gt4->clut = clut;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                }
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&quad[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, otz, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, otz, gt4);
                    gte_stsxy(&gt4->x3);
                    gt4++;
                }
            }
        }
    }
}

s32 fx_179_op80(s32 mode, Fx179Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR from;
    SVECTOR to;
    SVECTOR out;
    union {
        VECTOR v;
        SVECTOR s;
    } scratch;
    s32 frame;
    s32 bobAng;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = 0x20;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        work->pos10.vx = env->targets->centerX;
        work->pos10.vy = env->targets->centerY;
        work->pos10.vz = env->targets->centerZ;
        fx_179_env = env;
        ex->btlfx_initEffModels(fx_179_effModels, 1);
        work->model18 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 2), 0x9A, 0x3D40, 0, 0);
    } else {
    frame = entry->frame;
    switch (work->state0) {
    case 0:
        bobAng = (frame << 8) >> 2;
        pos.vz = work->pos10.vz + ex->math_cos(bobAng) * 3 / 2;
        pos.vy = -(ex->math_cos(bobAng) * 3 / 2 + 0x180) + work->pos10.vy;
        rot.vx = 0x200;
        rot.vz = -frame << 6;
        pos.vx = 0;
        rot.vy = 0;
        scale.vx = 0xAAA;
        scale.vy = 0xAAA;
        scale.vz = 0xAAA;
        work->pos8.vx = pos.vx;
        work->pos8.vy = pos.vy;
        work->pos8.vz = pos.vz;
        if (frame < 0xB) {
            from.vx = pos.vx;
            from.vy = pos.vy;
            from.vz = pos.vz;
            BTLFX_COPY_SVEC(&to, &work->pos10);
            to.vy -= 0x200;
            ex->btlfx_lerpVec((s16 *)&from, (s16 *)&to, (frame << 12) / 10, (s16 *)&out);
            env->camera->points[3].vx = out.vx;
            env->camera->points[3].vy = out.vy;
            env->camera->points[3].vz = out.vz;
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model18);
        scratch.v.vx = scratch.v.vy = scratch.v.vz = 0xAAA;
        fxm_drawTexCylinder(&work->pos8, &rot, &scratch.v, 0x3E8, 0x2BC, 0x300, 0x398, 0x80, 0x17, 0x3D03, 0, 0x40,
                      -(work->frame4 * 0x180), 0x2000, 1, 1);
        fxm_drawTexCylinder(&work->pos8, &rot, &scratch.v, 0x3E8, 0x2BC, 0x300, 0x400, 0x80, 0x17, 0x3D00, 0, 0x40,
                      -(work->frame4 * 0x180) + 0x400, 0x2000, 1, 1);
        fxm_drawTexCylinder(&work->pos8, &rot, &scratch.v, 0x3E8, 0x2BC, 0x300, 0x300, 0x80, 0x17, 0x3D04, 0, 0x40,
                      -(work->frame4 * 0x180) + 0x800, 0x2000, 1, 1);
        fxm_drawTexCylinder(&work->pos8, &rot, &scratch.v, 0x3E8, 0x2BC, 0x300, 0x300, 0x80, 0x17, 0x3D06, 0, 0x40,
                      -(work->frame4 * 0x180) + 0xC00, 0x2000, 1, 1);
        if (frame >= 0x10) {
            work->state0 = 1;
            entry->frame = -1;
            scratch.s.vx = 0;
            scratch.s.vy = 0;
            scratch.s.vz = 0;
            fxm_shatterModel(work->model18, &scratch.s);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
            if (emitter != NULL) {
                emitter->otZ = 0x20;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_179_op80Trails;
                emitter->trailSlots = 0xA;
                emitter->trailStride = 0x5C;
                emitter->otZ = 0x20;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = 0x20;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        break;
    case 1:
        if (frame == 0) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 100, 100, 100);
        }
        if (frame == 1) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 70, 70, 70);
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        scale.vx = 0xAAA;
        scale.vy = 0xAAA;
        scale.vz = 0xAAA;
        pos.vx = work->pos10.vx;
        pos.vy = work->pos10.vy;
        pos.vz = work->pos10.vz;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model18);
        fxm_updateShatterPieces();
        if (frame >= 0x20) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_179/nonmatchings/fx_179", fx_179_headerPtr);
