#include "fxmod/fx_1cc.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_1cc_env;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR quad[4];
    CVECTOR colTR;
    CVECTOR colBR;
    CVECTOR colTL;
    CVECTOR colBL;
    MATRIX m;
    s32 otz;
    s32 nclip;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *src;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 rows;
    s32 left;
    s32 texel;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        rows = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * rows);
        tpage = ex->btlfx_allocDrMode(0, abr);
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(pos);
        if (rot == NULL) {
            rot = &zero;
        }
        gte_rtv0tr_b();
        ex->math_rotMatrixYXZ(rot, &m);
        if (scale >= 0) {
            sv.vx = sv.vy = sv.vz = scale;
            ex->ScaleMatrix(&m, &sv);
        }
        gte_stlvnl(m.t);
        gte_rtps();
        code = GPU_POLY_G4;
        if (abr != BTLFX_ABR_OPAQUE) {
            code = GPU_POLY_G4 | GPU_ABE;
        }
        gte_stszotz(&otz);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        left = -(w * size) >> 1;
        h = rows;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = left;
            src = row;
            *(s32 *)&colTL = 0;
            *(s32 *)&colBL = 0;
            for (i = 0; i < w; i++, prim++, px += size, src++) {
                quad[0].vx = quad[2].vx = px;
                quad[0].vy = quad[1].vy = py;
                quad[1].vx = quad[3].vx = px + size;
                quad[2].vy = quad[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&colTR = 0;
                    *(s32 *)&colBR = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&colTR = 0;
                    } else {
                        texel = src[0];
                        colTR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colTR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colTR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&colBR = 0;
                    } else {
                        texel = src[w - 1];
                        colBR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colBR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colBR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&quad[0], &quad[1], &quad[2]);
                colTL.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&colTL;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&colTR;
                *(s32 *)&prim->r2 = *(s32 *)&colBL;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&nclip);
                gte_ldv0xy(&quad[3]);
                *(s32 *)&prim->r3 = *(s32 *)&colBR;
                gte_rtps_b();
                *(s32 *)&colBL = *(s32 *)&colBR;
                *(s32 *)&colTL = *(s32 *)&colTR;
                gte_stsxy(&prim->x3);
                if (nclip != 0) {
                    ex->btlfx_addPrim(PRIM_TAG(POLY_G4), otz, prim);
                }
            }
        }
        ex->btlfx_addPrim(PRIM_TAG(DR_TPAGE), otz, tpage);
    }
    w--;
    h--;
    left = (tp & TPAGE_X_MASK) * 64 + x + (idx % div) * w;
    py = (tp & TPAGE_Y_BIT) * 16 + y + (idx / div) * h;
    rect.x = left;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

static void fxm_drawBulgeGrid14(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr) {
    Unk800F78A4 *env = fx_1cc_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR sc;
    SVECTOR vtx[4];
    SVECTOR center;
    s32 otz;
    s32 row;
    s32 code;
    s32 rowX;
    s32 right;
    POLY_GT4 *prim;
    s32 col;
    s32 dist0;
    s32 dist1;
    s32 dist2;
    s32 dist3;
    s32 shade;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;

    *(s32 *)&center.vx = 0;
    center.vz = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    prim = (POLY_GT4 *)env->prim;
    code = GPU_POLY_GT4;
    env->prim = (u8 *)(prim + 14 * 14);
    if (abr != BTLFX_ABR_OPAQUE) {
        code = GPU_POLY_GT4 | GPU_ABE;
    }
    tpage |= TPAGE_ABR(abr);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    sc.vx = sc.vy = sc.vz = scale;
    ex->ScaleMatrix(&m, &sc);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    for (row = 0; row < 14; row++) {
        right = (row << 6) - 0x180;
        rowX = row << 6;
        for (col = 0; col < 14; col++) {
            vtx[0].vz = vtx[1].vz = vtx[2].vz = vtx[3].vz = 0;
            vtx[0].vx = vtx[2].vx = rowX - 0x1C0;
            vtx[1].vx = vtx[3].vx = right;
            vtx[0].vy = vtx[1].vy = (col << 6) - 0x1C0;
            vtx[2].vy = vtx[3].vy = (col << 6) - 0x180;
            dist0 = ex->btlfx_distance(&vtx[0], &center);
            dist1 = ex->btlfx_distance(&vtx[1], &center);
            dist2 = ex->btlfx_distance(&vtx[2], &center);
            dist3 = ex->btlfx_distance(&vtx[3], &center);
            if (dist0 > 0x100) {
                vtx[0].vz = 0;
            } else {
                vtx[0].vz = -(((ex->math_cos(dist0 * 8) + 0x1000) * amp) >> 13);
            }
            if (dist1 > 0x100) {
                vtx[1].vz = 0;
            } else {
                vtx[1].vz = -(((ex->math_cos(dist1 * 8) + 0x1000) * amp) >> 13);
            }
            if (dist2 > 0x100) {
                vtx[2].vz = 0;
            } else {
                vtx[2].vz = -(((ex->math_cos(dist2 * 8) + 0x1000) * amp) >> 13);
            }
            if (dist3 > 0x100) {
                vtx[3].vz = 0;
            } else {
                vtx[3].vz = -(((ex->math_cos(dist3 * 8) + 0x1000) * amp) >> 13);
            }
            gte_ldv3c(vtx);
            gte_rtpt();
            shade = bright - (-vtx[0].vz >> 3);
            if (shade >= 0x100) {
                shade = 0xFF;
            }
            if (dist0 > 0x1A0) {
                shade = 0;
            }
            prim->code = code;
            prim->r0 = prim->g0 = prim->b0 = shade;
            shade = bright - (-vtx[1].vz >> 3);
            if (shade >= 0x100) {
                shade = 0xFF;
            }
            if (dist1 > 0x1A0) {
                shade = 0;
            }
            prim->r1 = prim->g1 = prim->b1 = shade;
            shade = bright - (-vtx[2].vz >> 3);
            if (shade >= 0x100) {
                shade = 0xFF;
            }
            if (dist2 > 0x1A0) {
                shade = 0;
            }
            prim->r2 = prim->g2 = prim->b2 = shade;
            shade = bright - (-vtx[3].vz >> 3);
            if (shade >= 0x100) {
                shade = 0xFF;
            }
            if (dist3 > 0x1A0) {
                shade = 0;
            }
            prim->r3 = prim->g3 = prim->b3 = shade;
            gte_nclip();
            prim->tpage = tpage;
            prim->clut = clut;
            gte_stopz(&otz);
            if (otz != 0) {
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&vtx[3]);
                gte_rtps();
                u0 = u + ((row & 3) << 5);
                u1 = u0 + 0x20;
                v0 = v + ((col & 3) << 5);
                if ((row & 3) == 3) {
                    u1 = u0 + 0x1F;
                }
                v1 = v0 + 0x20;
                if ((col & 3) == 3) {
                    v1 = v0 + 0x1F;
                }
                prim->u0 = prim->u2 = u0;
                prim->u1 = prim->u3 = u1;
                prim->v0 = prim->v1 = v0;
                prim->v2 = prim->v3 = v1;
                ex->btlfx_addPrim(PRIM_TAG(POLY_GT4), otz, prim);
                gte_stsxy(&prim->x3);
                prim++;
            }
        }
    }
}

void fx_1cc_piecePos(s16 *from, s16 *to, s32 i, s16 *out) {
    BTLFX_EXPORTS(fx_1cc_env)->btlfx_pointAhead(from, to, -(fx_1cc_pieceDists[i] * 4), out);
}

static s32 fx_1cc_op80PoolTask(s32 mode, Fx1ccObj *obj, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool,
                  Fx1ccInst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 ringSize;

    if (mode == 1) {
        obj->state0 = 0;
        obj->frame8 = 0;
    } else if (obj->kind2 == 0) {
        SVECTOR dir;
        VECTOR sc;
        SVECTOR origin;
        SVECTOR ringPos;
        SVECTOR piecePos;
        SVECTOR center;
        SVECTOR launchDir;
        SVECTOR *tab;
        Fx1ccObj *ch;
        Pool6CEntry *emitter;
        u8 *ringFile;
        s32 piece;
        s32 i;
        s32 speed;
        s32 speedDrop;
        s32 growFrame;
        s32 rnd;
        s32 ang;

        tab = &fx_1cc_dirRots[obj->index4];
        switch (obj->state0) {
        case 0:
            if (obj->frame8 == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_SHARD_MIST), (s32)&obj->velX18);
                obj->emit34 = emitter;
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            sc.vx = 0;
            BTLFX_COPY_SVEC(&dir, tab);
            dir.vx -= 0x800;
            if (obj->emit34 != NULL) {
                obj->emit34->rot.vx = dir.vx;
                obj->emit34->rot.vy = dir.vy;
                obj->emit34->rot.vz = dir.vz;
            }
            if (++obj->frame8 > 0) {
                obj->state0 = 1;
                obj->frame8 = 0;
            }
            break;
        case 1:
            growFrame = obj->frame8;
            sc.vx = sc.vy = ((growFrame << 10) * 0x1000) >> 12;
            sc.vz = (ex->math_sin((growFrame << 10) >> 2) * 0x1000) >> 12;
            BTLFX_COPY_SVEC(&dir, tab);
            dir.vx -= 0x800;
            if (obj->emit34 != NULL) {
                obj->emit34->rot.vx = dir.vx;
                obj->emit34->rot.vy = dir.vy;
                obj->emit34->rot.vz = dir.vz;
            }
            if (++obj->frame8 >= 4) {
                obj->state0 = 2;
                obj->frame8 = 0;
            }
            break;
        case 2:
            sc.vx = sc.vy = sc.vz = 0x1000;
            BTLFX_COPY_SVEC(&dir, tab);
            dir.vx -= 0x800;
            if (obj->emit34 != NULL) {
                obj->emit34->rot.vx = dir.vx;
                obj->emit34->rot.vy = dir.vy;
                obj->emit34->rot.vz = dir.vz;
            }
            if (frame < 12) {
                ang = (frame << 10) / 12;
                dir.pad = 1;
                origin.vx = obj->posXC;
                origin.vy = obj->posY10;
                origin.vz = obj->posZ14;
                ex->btlfx_pointAhead((s16 *)&origin, (s16 *)&dir, 0x578, (s16 *)&ringPos);
                ringFile = (u8 *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_SNOW_RING_2);
                ringSize = (ex->math_sin(ang) * 0x2000) >> 12;
                ex->btlfx_drawRings(ringFile, &ringPos, &dir, ringSize, 0, BTLFX_ABR_ADD, ex->math_cos(ang) * 2 / 3);
            }
            if (inst->flag78 != 0) {
                piece = 6;
                obj->frame8 = 0;
                if (obj->index4 < 3) {
                    piece = 10;
                }
                BTLFX_COPY_SVEC(&center, &obj->velX18);
                for (i = 0, speedDrop = 0; i < 4; speedDrop += 0x29, i++, piece++) {
                    ch = obj;
                    if (i != 0) {
                        ch = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    }
                    if (ch != NULL) {
                        ch->kind2 = 1;
                        ch->state0 = piece;
                        ch->index4 = i;
                        ch->frame8 = 0;
                        ch->model2C = inst->model20[piece];
                        fx_1cc_piecePos((s16 *)&center, (s16 *)tab, piece, (s16 *)&piecePos);
                        BTLFX_COPY_SVEC(&ch->rot20, tab);
                        ch->posXC = piecePos.vx << 4;
                        ch->posY10 = piecePos.vy << 4;
                        ch->posZ14 = piecePos.vz << 4;
                        ch->spin28 = ex->btlfx_randCentered(0x84);
                        ch->spin2A = ex->btlfx_randCentered(0x84);
                        rnd = (ex->btlfx_rand() & 0xF) + 0x80;
                        speed = (rnd - speedDrop) << 4;
                        BTLFX_COPY_SVEC(&launchDir, tab);
                        launchDir.vx += 0x80;
                        ex->btlfx_rotateOffset(&launchDir, -speed, (s32)&ch->velX18);
                        ch->velX18 += ex->btlfx_randCentered(0x200);
                        ch->velY1A += ex->btlfx_randCentered(0x200);
                        ch->velZ1C += ex->btlfx_randCentered(0x200);
                    }
                }
            }
            break;
        }
        if (sc.vx != 0) {
            sc.vx *= 2;
            sc.vy *= 2;
            sc.vz = sc.vz * 5 / 2;
            ex->Hi_SetEffModelOffset(obj->model2C, 2);
            ex->Hi_DrawEffModel(tab, (VECTOR *)&obj->posXC, (s32)&sc, obj->model2C);
            ex->Hi_DrawEffModel(tab, (VECTOR *)&obj->posXC, (s32)&sc, obj->model30);
        }
    } else {
        VECTOR pos;
        VECTOR sc;
        s32 bright;

        t = ((obj->frame8 << 12) / 22) >> 2;
        bright = ex->math_cos(t);
        obj->posXC += obj->velX18;
        obj->posY10 += obj->velY1A;
        obj->posZ14 += obj->velZ1C;
        obj->velY1A += 0x40;
        obj->rot20.vx += obj->spin28;
        obj->rot20.vy += obj->spin2A;
        bright = (bright * 3) >> 1;
        if (obj->state0 < 6) {
            sc.vx = sc.vy = (ex->math_cos(t) * 0x3000) >> 12;
            sc.vz = (ex->math_cos(t) * 0x2000) >> 12;
        } else {
            sc.vx = sc.vy = (ex->math_cos(t) * 0x2000) >> 12;
            sc.vz = (ex->math_cos(t) * 0x2800) >> 12;
        }
        pos.vx = obj->posXC >> 4;
        pos.vy = obj->posY10 >> 4;
        pos.vz = obj->posZ14 >> 4;
        ex->Hi_DrawEffModel(&obj->rot20, &pos, (s32)&sc, obj->model2C);
        ex->Hi_ModifyEffModelAbr(obj->model2C, BTLFX_ABR_ADD);
        ex->Hi_ModifyEffModelRGB(obj->model2C, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
        if (++obj->frame8 >= 0x16) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1cc/nonmatchings/fx_1cc", fx_1cc_headerPtr);

s32 fx_1cc_op80(s32 mode, Fx1ccWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot;
    SVECTOR scratch;
    s32 ringAlpha;
    s32 frame;
    s32 i;
    s32 j;
    s32 tpage;
    s32 t;
    s32 ang;
    s32 tint;
    s32 rnd;
    s32 acc;
    s32 rad;
    u8 *ringFile;
    SVECTOR *tab;
    Fx1ccObj *ch;
    Fx1ccObj *ch3;
    Pool6CEntry *emitter;
    Pool6CEntry *emitter1;

    if (mode == 0) {
        init->workSize = sizeof(Fx1ccWork80);
        init->elemSize = sizeof(Fx1ccObj);
        init->elemFn = (s32)fx_1cc_op80PoolTask;
        init->poolCount = FX_SEALION_APPEARS_POOL;
    } else if (mode == 1) {
        fx_1cc_env = env;
        ex->btlfx_initEffModels(fx_1cc_effModels, FX_SEALION_APPEARS_MODEL_SLOTS);
        tpage = FX_SEALION_APPEARS_ICE_TPAGE;
        work->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_ICE_CRYSTAL), tpage, FX_SEALION_APPEARS_ICE_CLUT, FX_SEALION_APPEARS_ICE_U, FX_SEALION_APPEARS_ICE_V);
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_ICE_CRYSTAL_2), tpage, FX_SEALION_APPEARS_ICE_CLUT, FX_SEALION_APPEARS_ICE_U, FX_SEALION_APPEARS_ICE_V);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_ICE_CRYSTAL_3), tpage, FX_SEALION_APPEARS_ICE_CLUT, FX_SEALION_APPEARS_ICE_U, FX_SEALION_APPEARS_ICE_V);
        work->model14 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_CRYSTAL_SHEEN), FX_SEALION_APPEARS_SHEEN_TPAGE, FX_SEALION_APPEARS_SHEEN_CLUT, FX_SEALION_APPEARS_SHEEN_U, FX_SEALION_APPEARS_SHEEN_V);
        work->model18 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_CRYSTAL_SHEEN_2), FX_SEALION_APPEARS_SHEEN_TPAGE, FX_SEALION_APPEARS_SHEEN_CLUT, FX_SEALION_APPEARS_SHEEN_U, FX_SEALION_APPEARS_SHEEN_V);
        work->model1C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_CRYSTAL_SHEEN_3), FX_SEALION_APPEARS_SHEEN_TPAGE, FX_SEALION_APPEARS_SHEEN_CLUT, FX_SEALION_APPEARS_SHEEN_U, FX_SEALION_APPEARS_SHEEN_V);
        for (i = 0; i < FX_SEALION_APPEARS_PIECES; i++) {
            work->model20[i] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_1cc_pieceFileIds[i]), tpage, FX_SEALION_APPEARS_ICE_CLUT, FX_SEALION_APPEARS_ICE_U, FX_SEALION_APPEARS_ICE_V);
        }
        work->state0 = 0;
        work->frame4 = 0;
        work->index7A = 3;
        work->flag78 = 0;
        work->frame7C = 0;
        work->scroll80 = 0;
        work->pos58.vx = env->targets->centerX;
        work->pos58.vy = env->targets->centerY;
        work->pos58.vz = env->targets->centerZ;
        work->pos60.vx = work->pos58.vx;
        work->pos60.vy = work->pos58.vy;
        work->pos60.vz = work->pos58.vz;
        work->pos60.vy -= 0x555;
        BTLFX_COPY_SVEC(&work->unk70, &work->pos58);
        work->unk70.vy -= 0x80;
    } else {
        frame = entry->frame;
        pos.vx = work->pos58.vx;
        pos.vy = work->pos58.vy;
        pos.vz = work->pos58.vz;
        sc.vx = sc.vy = 0x3400;
        sc.vz = 0x2200;
        ringAlpha = 0;
        switch (work->state0) {
        case 0:
            rot.vx = 0x400;
            rot.vz = 0x200;
            rot.vy = 0;
            t = (frame << 12) / FX_SEALION_APPEARS_BULGE_FRAMES;
            fxm_drawBulgeGrid14(&work->pos58, &rot, 0x4400, t >> 5, 0x9A, 0x3D40, 0, 0x80, 0, BTLFX_ABR_ADD);
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            if (frame >= FX_SEALION_APPEARS_BULGE_FRAMES) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / FX_SEALION_APPEARS_MIST_FRAMES;
            ang = t >> 2;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = 0x200;
            fxm_drawBulgeGrid14(&work->pos58, &rot, 0x4400, 0x80, 0x9A, 0x3D40, 0, 0x80, -((ex->math_sin(ang) * 700) >> 12), BTLFX_ABR_ADD);
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            ringAlpha = t;
            rot.pad = 1;
            ringFile = (u8 *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_SNOW_RING_2);
            ex->btlfx_drawRings(ringFile, &work->pos58, &rot, ((((ex->math_sin(ang) * 0x3000) >> 12) + 0x2000) * 3) >> 2, 0, BTLFX_ABR_ADD,
                              ex->math_cos(ang));
            if (frame >= FX_SEALION_APPEARS_MIST_FRAMES) {
                work->state0 = 2;
                entry->frame = -1;
                BTLFX_COPY_SVEC(&work->pos68, &work->pos58);
                work->pos68.vy -= 0xAAA;
                emitter1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_MIST), (s32)&work->pos58);
                if (emitter1 != NULL) {
                    emitter1->otZ = 0x80;
                }
                emitter1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_MIST_HIGH), (s32)&work->pos68);
                if (emitter1 != NULL) {
                    emitter1->otZ = 0x80;
                }
                emitter1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_ICE_SPARKLES_2), (s32)&work->pos58);
                if (emitter1 != NULL) {
                    emitter1->otZ = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, 0xFF, 0xFF, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, 0, 0, 0x80);
            }
            break;
        case 2:
            ringAlpha = 0x1000;
            t = frame << 10;
            t = 0x2000 - t;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model8);
            ex->Hi_ModifyEffModelRGB(work->model8, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14);
            if (frame < 6) {
                t = 0xFF - (frame * 0xFF) / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, t >> 2, (t * 2) / 3, t);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= FX_SEALION_APPEARS_FLASH_FRAMES) {
                work->state0 = 3;
                entry->frame = -1;
            }
            break;
        case 3:
            ringAlpha = 0x1000;
            if (frame & 1) {
                ch3 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (ch3 != NULL) {
                    ch3->kind2 = 0;
                    ch3->index4 = work->index7A;
                    tab = &fx_1cc_dirRots[work->index7A];
                    if (work->index7A >= 3) {
                        ch3->model2C = work->modelC;
                        ch3->model30 = work->model18;
                    } else {
                        ch3->model2C = work->model10;
                        ch3->model30 = work->model1C;
                    }
                    ch3->posXC = work->pos58.vx;
                    ch3->posY10 = work->pos58.vy;
                    ch3->posZ14 = work->pos58.vz;
                    rad = 0x1DD;
                    ch3->posXC += (ex->math_sin(tab->vy + 0x800) * rad) >> 12;
                    ch3->posZ14 += (ex->math_cos(tab->vy + 0x800) * rad) >> 12;
                    ch3->velX18 = ch3->posXC;
                    ch3->velY1A = ch3->posY10;
                    ch3->velZ1C = ch3->posZ14;
                    work->index7A++;
                }
            }
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14);
            if (work->index7A >= 6) {
                work->state0 = 4;
                entry->frame = -1;
            }
            break;
        case 4:
            ringAlpha = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14);
            if (frame >= FX_SEALION_APPEARS_HOLD_FRAMES) {
                work->state0 = 5;
                entry->frame = -1;
            }
            break;
        case 5:
            ringAlpha = 0x1000;
            t = (frame << 12) / FX_SEALION_APPEARS_BURST_FRAMES;
            ex->btlfx_fadeUnitById(0, (t << 7) >> 12);
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14);
            rot.pad = 1;
            if (frame >= FX_SEALION_APPEARS_BURST_FRAMES) {
                work->state0 = 6;
                work->flag78 = 1;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, 0xFF, 0xFF, 0);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_ICE_FLARES), (s32)&work->pos58);
                if (emitter != NULL) {
                    emitter->otZ = 0x60;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_ICE_FLARES_2), (s32)&work->pos58);
                if (emitter != NULL) {
                    emitter->otZ = 0x60;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_FREEZE_BURST), (s32)&work->pos60);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_FROST_RING_2), (s32)&work->pos60);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = -0x400;
                for (j = 0; j < 6; j++) {
                    ch = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (ch != NULL) {
                        ch->kind2 = 1;
                        ch->state0 = j;
                        ch->index4 = j;
                        ch->frame8 = 0;
                        ch->model2C = work->model20[j];
                        fx_1cc_piecePos((s16 *)&work->pos58, (s16 *)&rot, j, (s16 *)&scratch);
                        ((s32 *)&ch->rot20)[0] = ((s32 *)&rot)[0];
                        ((s32 *)&ch->rot20)[1] = ((s32 *)&rot)[1];
                        ch->posXC = scratch.vx << 4;
                        ch->posY10 = scratch.vy << 4;
                        ch->posZ14 = scratch.vz << 4;
                        ch->spin28 = ex->btlfx_randCentered(0x5C);
                        ch->spin2A = ex->btlfx_randCentered(0x80);
                        rnd = (ex->btlfx_rand() & 0xF) + 0x5C;
                        ex->btlfx_rotateOffset(&rot, -((rnd - j * 8) << 4), (s32)&ch->velX18);
                        ch->velX18 += ex->btlfx_randCentered(0x3E0);
                        ch->velZ1C += ex->btlfx_randCentered(0x3E0);
                    }
                }
            }
            break;
        case 6:
            rot.vx = -0x400;
            rot.vz = -0x400;
            rot.vy = 0;
            if (frame < 18) {
                ringAlpha = ex->math_cos((frame << 10) / 18);
            }
            if (frame < 8) {
                tint = 0xFF - ((frame * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, tint >> 2, (tint * 2) / 3, tint);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= FX_SEALION_APPEARS_RINGS_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if ((u32)(work->state0 - 2) < 4) {
            if (++work->frame7C < 11) {
                acc = work->scroll80 + 11;
                work->scroll80 = acc - (work->frame7C * 10) / 11;
            } else {
                work->scroll80++;
            }
            ex->btlfx_drawScrollMove(work->scroll80, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        }
        if (ringAlpha != 0) {
            scratch.vx = 0x400;
            scratch.vy = 0;
            scratch.vz = work->frame4 * 41;
            scratch.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_SNOW_RING), &work->pos58, &scratch, (work->frame4 << 6) + 0x2800,
                              0, BTLFX_ABR_ADD, ringAlpha);
            scratch.vz = work->frame4 * 24 + 0x400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_SNOW_RING), &work->pos58, &scratch, (work->frame4 << 6) + 0x2800,
                              0, BTLFX_ABR_ADD, ringAlpha);
            env->otZ = 0x127;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SEALION_APPEARS_FROST_RING), &work->pos58, NULL, (work->frame4 << 5) + 0x2800,
                              0, BTLFX_ABR_ADD, ringAlpha);
            env->otZ = 0;
        }
        work->frame4++;
    }
    return 0;
}
