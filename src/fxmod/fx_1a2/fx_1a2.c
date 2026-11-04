#include "fxmod/fx_1a2.h"

static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx1a2Seg *buf, s32 abr) {
    Unk800F78A4 *env = fx_1a2_env;
    CVECTOR colA;
    CVECTOR colB;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx1a2Seg *seg;
    u32 i;
    s32 fadeNear;
    s32 fadeFar;

    if (fade < 0) {
        fade = 0x1000;
    }
    if (n >= 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].pos0.pad = 0;
        }
        return;
    }
    seg = &buf[(s32)(n - 1)];
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&seg[1].pos0, &seg[0].pos0);
        BTLFX_COPY_SVEC(&seg[1].pos8, &seg[0].pos8);
        seg--;
    }
    seg = buf;
    for (i = 0; i < n; i++, seg++) {
        if (!(seg[0].pos0.pad & seg[1].pos0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->pos0, posA);
    buf->pos0.pad = 1;
    BTLFX_COPY_SVEC(&buf->pos8, posB);
    n = i;
    prim = (POLY_G4 *)env->prim;
    env->prim = (s32)(prim + n);
    if (colorA == NULL) {
        colA.r = colA.g = colA.b = 0;
    } else {
        gte_DpqColorFar_nst(colorA, fade);
        gte_strgb(&colA);
    }
    if (colorB == NULL) {
        colB.r = colB.g = colB.b = 0;
    } else {
        gte_DpqColorFar_nst(colorB, fade);
        gte_strgb(&colB);
    }
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    colA.cd = abr != 0xFF ? 0x3A : 0x38;
    colB.cd = abr != 0xFF ? 0x3A : 0x38;
    for (i = 0; i < n; i++, buf++, prim++) {
        gte_ldv3c(buf);
        fadeNear = 0x1000 - (i << 12) / n;
        gte_rtpt_b();
        fadeFar = 0x1000 - ((i + 1) << 12) / n;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].pos8);
            gte_rtps();
            ex->btlfx_addPrimAbr(0x8000000, otz, prim, abr);
            gte_stsxy(&prim->x3);
            gte_DpqColorFar_nst(&colA, fadeNear);
            gte_strgb(&prim->r0);
            gte_DpqColorFar_nst(&colB, fadeNear);
            gte_strgb(&prim->r1);
            gte_DpqColorFar_nst(&colA, fadeFar);
            gte_strgb(&prim->r2);
            gte_DpqColorFar_nst(&colB, fadeFar);
            gte_strgb(&prim->r3);
        }
    }
}

static void fxm_shatterModel(s32 model, SVECTOR *center) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a2_env);
    SVECTOR dir;
    Fx1a2Tri *tri;
    Fx1a2Piece *piece;
    s32 i;

    ex->Hi_SplitMdlVertex(model, -1, (s32)fx_1a2_shatterTris);
    tri = fx_1a2_shatterTris;
    piece = fx_1a2_shatterPieces;
    for (i = 0; i < 130; i++, tri++, piece++) {
        piece->frame18 = 0;
        piece->pos0 = ((tri->verts0[0].vx + tri->verts0[1].vx + tri->verts0[2].vx) / 3) << 12;
        piece->pos4 = ((tri->verts0[0].vy + tri->verts0[1].vy + tri->verts0[2].vy) / 3) << 12;
        piece->pos8 = ((tri->verts0[0].vz + tri->verts0[1].vz + tri->verts0[2].vz) / 3) << 12;
        piece->delay1A = 0;
        piece->unk12 = 0x1000;
        piece->rot14 = ex->btlfx_randCentered(0x100);
        piece->rot16 = ex->btlfx_randCentered(0x200);
        dir.vx = (piece->pos0 >> 12) - center->vx;
        dir.vy = (piece->pos4 >> 12) - center->vy;
        dir.vz = (piece->pos8 >> 12) - center->vz;
        piece->velC = dir.vx >> 2;
        piece->velE = dir.vy >> 3;
        piece->vel10 = dir.vz >> 2;
    }
}

void fxm_updateShatterPieces(void) {
    SVECTOR out;
    SVECTOR rot;
    SVECTOR center;
    SVECTOR loc[3];
    SVECTOR bias;
    VECTOR world;
    VECTOR scale;
    MATRIX m;
    BtlfxExports *ex;
    Fx1a2Tri *tri;
    Fx1a2Piece *piece;
    s32 i;
    s32 k;

    tri = fx_1a2_shatterTris;
    piece = fx_1a2_shatterPieces;
    ((s32 *)&bias)[0] = 0;
    ((s32 *)&bias)[1] = 0;
    ex = BTLFX_EXPORTS(fx_1a2_env);
    scale.vx = scale.vy = scale.vz = 0xF80;
    for (i = 0; i < 130; i++, piece++, tri++) {
        if (piece->frame18 >= 0) {
            if (piece->frame18 < piece->delay1A) {
                piece->frame18++;
            } else {
                center.vx = piece->pos0 >> 12;
                center.vy = piece->pos4 >> 12;
                center.vz = piece->pos8 >> 12;
                for (k = 0; k < 3; k++) {
                    loc[k].vx = tri->verts0[k].vx - center.vx;
                    loc[k].vy = tri->verts0[k].vy - center.vy;
                    loc[k].vz = tri->verts0[k].vz - center.vz;
                }
                if (piece->frame18 >= 3) {
                    ex->btlfx_stepModuleParticle((Pool6CEntry *)piece, -3, &bias, &out);
                    if (piece->velE < 0 && piece->pos4 < 0) {
                        piece->velE = -(piece->velE >> 1);
                    }
                }
                rot.vx = piece->rot14;
                rot.vy = piece->rot16;
                rot.vz = 0;
                ex->math_rotMatrixYXZ(&rot, &m);
                ex->ScaleMatrix(&m, &scale);
                m.t[0] = piece->pos0 >> 12;
                m.t[1] = piece->pos4 >> 12;
                m.t[2] = piece->pos8 >> 12;
                ex->SetRotMatrix(&m);
                ex->SetTransMatrix(&m);
                for (k = 0; k < 3; k++) {
                    gte_ldv0(&loc[k]);
                    gte_rtv0tr();
                    gte_stlvnl(&world);
                    tri->verts0[k].vx = world.vx;
                    tri->verts0[k].vy = world.vy;
                    tri->verts0[k].vz = world.vz;
                }
                if (++piece->frame18 >= 0x40) {
                    piece->frame18 = -1;
                    tri->verts0[2].vz = 0;
                    *(s32 *)&tri->verts0[2] = 0;
                    tri->verts0[1].vz = 0;
                    *(s32 *)&tri->verts0[1] = 0;
                    tri->verts0[0].vz = 0;
                    *(s32 *)&tri->verts0[0] = 0;
                }
            }
        }
    }
}

static s32 fx_1a2_op80PoolTask(s32 mode, Fx1a2Rock *rock, Unk800F78A4 *env, s32 entry, s32 frame, s32 pool, Fx1a2Inst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    VECTOR pos;
    SVECTOR rot;
    s32 t;
    s32 tilt;
    s32 ang;

    if (mode == 1) {
        rock->count0 = 0;
        rock->height4 = (ex->btlfx_rand() & 0x7F) + 100;
    } else {
    tilt = 0;
    switch (rock->state8) {
    case 0:
        t = frame << 9;
        ex->btlfx_pointAhead((s16 *)&rock->posC, (s16 *)&rock->rot14, (rock->scale1C << 5) >> 12, (s16 *)&rock->posC);
        pos.vx = rock->posC.vx;
        pos.vy = rock->posC.vy;
        pos.vz = rock->posC.vz;
        pos.vy -= (rock->height2 * (0x1000 - t)) >> 12;
        ang = t >> 1; /* a fresh single-set temporary (sched1 birthing boost) */
        pos.vy -= (ex->math_sin(ang) * rock->height4) >> 12;
        tilt = -(ex->math_sin(ang) >> 2);
        if (frame >= 8) {
            rock->state8 = 1;
            ((s16 *)rock)[-1] = -1;
            rock->height2 = 0;
            rock->wait6 = (ex->btlfx_rand() & 3) + 1;
        }
        break;
    case 1:
        pos.vx = rock->posC.vx;
        pos.vy = rock->posC.vy;
        pos.vz = rock->posC.vz;
        pos.vy -= (ex->btlfx_rand() & 0xF) + rock->height2;
        if (frame >= rock->wait6) {
            rock->state8 = 0;
            ((s16 *)rock)[-1] = -1;
            if (rock->scale1C < 0x1800) {
                rock->rot14.vy += ex->btlfx_randCentered(0x400);
            }
            rock->height4 = ex->btlfx_rand() % 160 + 0x118;
            if (++rock->count0 >= 10) {
                return 1;
            }
        }
        break;
    }
    rot.vx = rock->rot14.vx;
    rot.vy = rock->rot14.vy;
    rot.vz = rock->rot14.vz;
    rot.vx += 0x400;
    rot.vx += tilt;
    rot.vy += 0x800;
    scale.vx = scale.vy = scale.vz = rock->scale1C;
    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, inst->model48);
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a2/nonmatchings/fx_1a2", fx_1a2_headerPtr);

s32 fx_1a2_op80(s32 mode, Fx1a2Work80 *w, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR bonePos;
    VECTOR scale;
    VECTOR pos;
    SVECTOR ribbon1;
    SVECTOR ribbon2;
    SVECTOR ribbon3;
    SVECTOR ringRot;
    MATRIX boneMtx1;
    MATRIX boneMtx2;
    MATRIX boneMtx3;
    MATRIX m;
    SVECTOR sv;
    Fx1a2Rock *rock;
    Pool6CEntry *emitter;
    SVECTOR *tbl;
    s32 frame;
    s32 t;
    s32 bright;
    s32 fade;
    s32 i;
    s32 cc;
    SVECTOR *spawnPos;

    if (mode == 0) {
        init->workSize = 0x54;
        init->elemSize = 0x20;
        init->elemFn = (s32)fx_1a2_op80PoolTask;
        init->poolCount = 0x20;
    } else if (mode == 1) {
        w->state0 = 0;
        w->frame4 = 0;
        fx_1a2_env = env;
        ex->btlfx_initEffModels(fx_1a2_effModels, 10);
        w->model3C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_LIGHT_BAND_2), 0xB8, 0x3DC0, 0x80, 0);
        w->model40 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_LIGHT_RING), 0xB8, 0x3DC0, 0x80, 0);
        w->model30[0] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_BLADE_3), 0xBA, 0x3E40, 0, 0xE0);
        w->model30[1] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_BLADE_2), 0xBA, 0x3E40, 0, 0xE0);
        w->model30[2] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_BLADE), 0xBA, 0x3E40, 0, 0xE0);
        w->model44 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_LIGHT_BAND), 0xB8, 0x3DC0, 0x80, 0);
        w->model4C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_ARMOR), 0x98, 0x3E00, 0, 0x80);
        w->model50 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_ARMOR_2), 0x98, 0x3E00, 0, 0x80);
        rot.vx = rot.vz = 0x97;
        rot.vy = rot.pad = 0x3D40;
        bonePos.vy = bonePos.vx = bonePos.vz = 0;
        bonePos.pad = 0x80;
        w->model48 = (s16)((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_SPIKED_SHAPE), &rot, &bonePos);
        ex->btlfx_getUnitPos(0, (s16 *)&w->pos8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&w->pos10);
        ex->btlfx_setPartnerByArg(0);
        w->scale2C = 0x8F8;
    } else {
    frame = entry->frame;
    switch (w->state0) {
    case 0:
        t = (frame << 12) / 93;
        ex->btlfx_getUnitBonePos(0x10, 0x10, (u16 *)&bonePos);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x10, (BtlBone *)&boneMtx1);
        ex->btlfx_matrixPointAhead(&boneMtx1, 0x10E, (s32)&ribbon1);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x10, (BtlBone *)&boneMtx2);
        ex->btlfx_matrixPointAhead(&boneMtx2, 0x147, (s32)&ribbon2);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x10, (BtlBone *)&boneMtx3);
        ex->btlfx_matrixPointAhead(&boneMtx3, 0x155, (s32)&ribbon3);
        if (frame == 0) {
            fxm_drawRibbonTrail(&bonePos, &ribbon1, 0x407, &fx_1a2_colors0, &fx_1a2_colors1, 0, fx_1a2_ribbonEdges0, 1);
            fxm_drawRibbonTrail(&ribbon1, &ribbon2, 0x407, &fx_1a2_colors1, &fx_1a2_colors2, 0, fx_1a2_ribbonEdges1, 1);
            fxm_drawRibbonTrail(&ribbon2, &ribbon3, 0x407, &fx_1a2_colors2, &fx_1a2_colors0, 0, fx_1a2_ribbonEdges2, 1);
        }
        fxm_drawRibbonTrail(&bonePos, &ribbon1, 0x1F, &fx_1a2_colors0, &fx_1a2_colors1, 0x1000, fx_1a2_ribbonEdges0, 1);
        fxm_drawRibbonTrail(&ribbon1, &ribbon2, 0x1F, &fx_1a2_colors1, &fx_1a2_colors2, 0x1000, fx_1a2_ribbonEdges1, 1);
        fxm_drawRibbonTrail(&ribbon2, &ribbon3, 0x1F, &fx_1a2_colors2, &fx_1a2_colors0, 0x1000, fx_1a2_ribbonEdges2, 1);
        env->otZ = 0x20;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_WHITE_RINGS_2), &ribbon2, NULL, 0x400, 0, 1, ex->math_sin(t >> 2) >> 1);
        ringRot.pad = 0;
        ringRot.vy = 0;
        ringRot.vx = 0;
        ringRot.vz = frame << 4;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_WHITE_RINGS), &ribbon2, &ringRot, 0x800, 0, 1, ex->math_sin(t >> 2));
        env->otZ = 0;
        ((s32 *)&w->pos18)[0] = ((s32 *)&ribbon2)[0];
        ((s32 *)&w->pos18)[1] = ((s32 *)&ribbon2)[1];
        if (frame == 0) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_CYAN_SPARKLES), (s32)&w->pos18);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
        }
        if (frame >= 0x5D) {
            w->state0 = 1;
            entry->frame = -1;
        }
        break;
    case 1:
        if (frame < 0xB) {
            tbl = (SVECTOR *)ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_ROT_KEYS);
            tbl = (SVECTOR *)((u8 *)tbl + 4);
            pos.vx = w->pos8.vx;
            pos.vy = w->pos8.vy;
            pos.vz = w->pos8.vz;
            rot.vx = 0;
            rot.vy = frame << 5;
            rot.vz = 0;
            if (frame == 4) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_MOON), (s32)&w->pos8);
                if (emitter != NULL) {
                    emitter->otZ = -0x40;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_SPRITES), (s32)&w->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame < 7) {
                bright = 0x1000;
            } else {
                bright = 0x1000 - ((frame - 7) << 10);
            }
            scale.vx = scale.vz = (w->scale2C * 13) >> 4;
            scale.vy = w->scale2C * 3;
            for (i = 0; i < 6; i++) {
                if (frame - i >= 0) {
                    rot.vy = ((s16 *)tbl)[(frame - i) * 3] + 0x800;
                    if (frame < 3) {
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model30[frame]);
                    } else {
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model30[2]);
                        ex->Hi_ModifyEffModelRGB(w->model30[2], ((bright - i * bright / 5) >> 5) - 0x80, ((bright - i * bright / 5) >> 5) - 0x80,
                                                 ((bright - i * bright / 5) >> 5) - 0x80);
                    }
                }
            }
            gte_SetRotMatrix(env->viewMatrix);
            gte_SetTransMatrix(env->viewMatrix);
            rot.vy = ((s16 *)tbl)[frame * 3] + 0x800;
            ex->math_rotMatrixYXZ(&rot, &m);
            ex->ScaleMatrix(&m, &scale);
            m.t[0] = pos.vx;
            m.t[1] = pos.vy;
            m.t[2] = pos.vz;
            gte_SetRotMatrix(&m);
            gte_SetTransMatrix(&m);
            sv.vx = 0x3E8;
            sv.vy = 0;
            sv.vz = 0;
            gte_ldv0(&sv);
            gte_rtv0tr();
            gte_stlvnl(&pos);
            sv.vx = pos.vx;
            sv.vy = pos.vy;
            sv.vz = pos.vz;
            ((s32 *)&w->pos18)[0] = ((s32 *)&sv)[0];
            ((s32 *)&w->pos18)[1] = ((s32 *)&sv)[1];
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_CYAN_SPARKLES_2), (s32)&w->pos18);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            ex->btlfx_drawScrollMove(0, 0x280, 0x1E0, 0x40, 0x20, 0x2C0, ((frame & 7) << 5) + 0x100);
            if (frame < 0xB) {
                break;
            }
        }
        w->state0 = 2;
        entry->frame = -1;
        break;
    case 2:
        if (frame >= 0xC) {
            w->state0 = 3;
            entry->frame = -1;
        }
        break;
    case 3:
        t = (frame << 12) / 29;
        if (frame == 0) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_CRESCENT_TRAIL), (s32)&w->pos8);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_1a2_op80Trails0;
                emitter->trailSlots = 0x10;
                emitter->trailStride = 0x4C;
                emitter->otZ = 0x40;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_CRESCENT_TRAIL_2), (s32)&w->pos8);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_1a2_op80Trails1;
                emitter->trailSlots = 8;
                emitter->trailStride = 0x4C;
                emitter->otZ = 0x40;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_CYAN_RINGS), (s32)(spawnPos = &w->pos8));
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_CYAN_STREAKS), (s32)spawnPos);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MOONLIGHT_SLASH_SPRITES_2), (s32)&w->pos8);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
            /* Dead load (flow deletes it): spawnPos's last use comes after the 0x17 spawn's, so cse makes spawnPos the class
             * head for the 7/0xF spawns and keeps the single-set copy (sched1 places it after the call). */
            fade = spawnPos->vx;

        }
        pos.vx = w->pos8.vx;
        pos.vy = w->pos8.vy;
        pos.vz = w->pos8.vz;
        rot.vx = 0;
        rot.vy = frame << 5;
        rot.vz = 0;
        scale.vx = scale.vz = (w->scale2C * 14) >> 4;
        scale.vy = w->scale2C + ex->math_cos(t >> 2) * 2;
        ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, w->model40, w->model3C, t);
        ex->Hi_ModifyEffModelRGB(w->model40, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                 (ex->math_cos(t >> 2) >> 5) - 0x80);
        scale.vx = scale.vz = (w->scale2C * 14) >> 4;
        if (frame < 4) {
            scale.vy = (w->scale2C * frame / 4) * 2;
        } else {
            scale.vy = ((w->scale2C * ex->math_cos(t >> 2)) >> 12) * 2;
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model44);
        ex->Hi_ModifyEffModelRGB(w->model44, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                 (ex->math_cos(t >> 2) >> 5) - 0x80);
        ex->btlfx_drawScrollMove(frame * 8, 0x240, 0x100, 0x30, 0x80, 0x240, 0x180);
        if (frame >= 10) {
            if (frame == 10) {
                ((SVECTOR *)&m)->vx = 0;
                ((SVECTOR *)&m)->vy = 0;
                ((SVECTOR *)&m)->vz = 0;
                fxm_shatterModel(w->model4C, (SVECTOR *)&m);
                w->pos22 = w->pos8.vy;
                w->vel28 = -0x28;
                w->pos20 = w->pos8.vx;
                w->pos24 = w->pos8.vz;
                w->pos22 -= 0x1AE;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x10, 0x3C, 0x46);
                ex->btlfx_fadeUnitById(0, 0);
                ex->btlfx_fadeUnitById(0x11, -2);
                ex->btlfx_setUnitPos(0x11, (s16 *)&w->pos8);
            }
            pos.vx = w->pos8.vx;
            pos.vy = w->pos8.vy;
            pos.vz = w->pos8.vz;
            ex->btlfx_getUnitRot(0, (s16 *)&rot);
            scale.vx = scale.vy = scale.vz = 0x3800;
            rot.vy += 0x800;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model4C);
            fxm_updateShatterPieces();
            rot.vx = (w->vel28 + 0x28) * 32;
            pos.vx = w->pos20;
            pos.vy = w->pos22;
            pos.vz = w->pos24;
            w->pos22 += w->vel28;
            w->pos20 += 7;
            w->pos24 += 0xC;
            w->vel28 += 4;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model50);
        }
        if ((u32)(frame - 10) < 4) {
            for (i = 0; i < 6; i++) {
                rock = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (rock != NULL) {
                    rock->posC.vx = w->pos8.vx;
                    rock->posC.vy = w->pos8.vy;
                    rock->posC.vz = w->pos8.vz;
                    rock->posC.vx += ex->btlfx_randCentered(0x100);
                    rock->posC.vy += ex->btlfx_randCentered(0x180) - 0x100;
                    rock->posC.vz += ex->btlfx_randCentered(0x100);
                    rock->height2 = w->pos8.vy - w->pos10.vy;
                    rock->posC.vy = w->pos8.vy;
                    rock->rot14.vz = 0;
                    rock->rot14.vx = 0;
                    rock->rot14.vy = ex->btlfx_randCentered(0x400);
                    rock->scale1C = ((ex->btlfx_rand() & 3) << 8) + 0xAAA;
                    rock->state8 = 0;
                }
            }
            if (frame == 10) {
                rock = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (rock != NULL) {
                    rock->posC.vx = w->pos8.vx;
                    rock->posC.vy = w->pos8.vy;
                    rock->posC.vz = w->pos8.vz - 0x20;
                    rock->height2 = 0x1A4;
                    rock->scale1C = 0x1800;
                    rock->state8 = 1;
                    rock->rot14.vz = 0;
                    rock->rot14.vx = 0;
                    rock->rot14.vy = 0;
                    rock->wait6 = 0x40;
                }
            }
        }
        if (frame >= 0x1D) {
            w->state0 = 4;
            entry->frame = -1;
        }
        break;
    case 4:
        pos.vx = w->pos8.vx;
        pos.vy = w->pos8.vy;
        pos.vz = w->pos8.vz;
        ex->btlfx_getUnitRot(0, (s16 *)&rot);
        scale.vx = scale.vy = scale.vz = 0x3800;
        rot.vy += 0x800;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model4C);
        fxm_updateShatterPieces();
        rot.vx = (w->vel28 + 0x28) * 32;
        pos.vx = w->pos20;
        pos.vy = w->pos22;
        pos.vz = w->pos24;
        w->pos22 += w->vel28;
        w->pos20 += 7;
        w->pos24 += 0xC;
        w->vel28 += 4;
        if (w->vel28 > 0 && w->pos22 > 0) {
            w->vel28 = -w->vel28 / 2;
        }
        if (frame < 0x30) {
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model50);
            if ((u32)(frame - 0x20) < 0x10) {
                fade = 0x1000 - ((frame - 0x20) << 8);
                ex->Hi_ModifyEffModelAbr(w->model50, 1);
                ex->Hi_ModifyEffModelRGB(w->model50, (fade >> 5) - 0x80, (fade >> 5) - 0x80, (fade >> 5) - 0x80);
            }
        }
        if (frame == 0x4B) {
            ex->btlfx_signalScriptCue();
        }
        if (frame >= 0x64) {
            ex->btlfx_poolClear((Unk800DA1B8 *)entry->pool);
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    w->frame4++;
    }
    return 0;
}

s32 fx_1a2_op81(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 alpha;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        fx_1a2_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)work);
        return 0;
    } else {
        frame = entry->frame;
        work->vz += 0x60;
        ex->btlfx_setUnitPos(0x10, (s16 *)work);
        if (frame < 0x24) {
            alpha = 0x80;
        } else {
            alpha = 0x80 - (frame - 0x24) * 0x10;
        }
        ex->btlfx_fadeUnitById(0x10, alpha);
        if (frame >= 0x2C) {
            return 1;
        }
    }
    return 0;
}
