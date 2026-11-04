#include "ovl_0c.h"

void wm_projectMesh(ModelObj *mdl, u8 groupIdx) {
    MATRIX boneView;
    WmModelGroup *grp;
    MATRIX *bone;
    SVECTOR *vtx;
    s32 *sxy;
    s32 *sz;
    s32 k;
    u32 i;
    s32 vtxCount;
    u32 cnt;
    MATRIX *boneMat;
    SVECTOR *vtxCur;
    s16 *c1;
    s16 *c2;

    grp = &mdl->model->groups[groupIdx];
    gte_SetGeomScreen(g_gameState->gameData->projDist);
    sxy = g_vtxScreenXY;
    sz = g_vtxScreenZ;
    k = 0;
    cnt = mdl->model->boneCount;
    bone = (MATRIX *)mdl->boneMatrices;
    for (i = 0; i < cnt; bone++) {
        vtxCount = ((u16 *)grp->boneVertexCounts)[i++];
        if (vtxCount != 0) {
            gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
            gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
            boneMat = bone;
            gte_ldclmv(boneMat);
            c1 = &boneMat->m[0][1];
            c2 = &boneMat->m[0][2];
            gte_rtir_b();
            sxy = &g_vtxScreenXY[k];
            gte_stclmv(&boneView);
            gte_ldclmv(c1);
            gte_rtir();
            sz = &g_vtxScreenZ[k];
            gte_stclmv(&boneView.m[0][1]);
            gte_ldclmv(c2);
            c1 = (s16 *)boneMat->t;
            gte_rtir_n1();
            vtx = &((SVECTOR *)grp->vertices)[k];
            gte_stclmv(&boneView.m[0][2]);
            gte_ldlv0(c1);
            k += vtxCount;
            gte_rtv0tr_n1();
            gte_ldv3c(vtx);
            gte_stlvnl(boneView.t);
            gte_SetRotMatrix(&boneView);
            gte_SetTransMatrix(&boneView);
            vtxCur = vtx;
            do {
                vtxCur += 3;
                vtxCount -= 3;
                gte_rtpt_b();
                gte_ldv3c(vtxCur);
                gte_stsxy3c(sxy);
                gte_stsz3c(sz);
                sxy += 3;
                sz += 3;
            } while (vtxCount > 0);
        }
    }
}

/* attach a model file (relocating it once) and lay out its work buffers at work */
void wm_initModel(ModelObj *mdl, u8 *work) {
    WmModel *model;
    WmModelGroup *groups;
    u32 i;
    u32 groupCount;
    u32 k;
    s32 size;
    u8 *pp;
    WmModelGroup *grp;
    WmModelGroup *g2;

    model = mdl->model;
    if (!(model->flags & WMMODEL_RELOCATED)) {
        i = 0;
        model->flags |= WMMODEL_RELOCATED;
        model->bones = (s32)model + model->bones;
        groups = (WmModelGroup *)((u8 *)model + (s32)model->groups);
        model->groups = groups;
        for (grp = groups; i < model->groupCount; i++, grp++) {
            grp->boneVertexCounts = (u8 *)model + (s32)grp->boneVertexCounts;
            grp->vertices = (u8 *)model + (s32)grp->vertices;
            grp->prims = (u8 *)model + (s32)grp->prims;
            grp->uvs = (void *)((u8 *)model + (s32)grp->uvs);
            grp->colors = (void *)((u8 *)model + (s32)grp->colors);
        }
    }
    g2 = mdl->model->groups;
    size = 0;
    if (IS_OFFSET(g2)) {
        g2 = (WmModelGroup *)((u8 *)g2 + (s32)mdl->model);
    }
    groupCount = mdl->model->groupCount;
    for (k = 0; k < groupCount; k++) {
        size += g2->packetSize;
        g2++;
    }
    mdl->scaleX = 0x1000;
    mdl->scaleY = 0x1000;
    mdl->scaleZ = 0x1000;
    mdl->parent = NULL;
    mdl->mirrorSrc = NULL;
    mdl->unk5C = 0;
    mdl->hiddenGroups = 0;
    mdl->attachX = 0;
    mdl->attachY = 0;
    mdl->attachZ = 0;
    mdl->packets = work;
    mdl->texAnims = NULL;
    pp = work + size * 2;
    mdl->boneMatrices = pp;
    mdl->flags |= MDL_SCALED;
    *(volatile u16 *)&mdl->flags = 0;
    mdl->groupOut = (WmModelOut *)(pp + model->boneCount * 32);
}

void wm_buildPackets(ModelObj *mdl, WmTexInfo *texInfo, s16 (*uvOffsets)[2]) {
    WmModel *model;
    WmModelGroup *grp;
    WmModelOut *out;
    WmModelOut *outs;
    u8 *prim;
    u8 *src;
    u8 (*col)[4];
    u8 *rgb;
    u32 k;
    u32 j;
    u32 i;
    s32 du;
    s32 dv;
    u16 tp;
    POLY_FT4 *pFT4;
    WmSrcFT4 *sFT4;
    POLY_FT3 *pFT3;
    WmSrcFT3 *sFT3;
    POLY_GT4 *pGT4;
    WmSrcGT4 *sGT4;
    POLY_GT3 *pGT3;
    WmSrcGT3 *sGT3;
    POLY_G4 *pG4;
    WmSrcG4 *sG4;
    POLY_G3 *pG3;
    WmSrcG3 *sG3;
    POLY_F4 *pF4;
    WmSrcF4 *sF4;
    POLY_F3 *pF3;
    WmSrcF3 *sF3;

    prim = mdl->packets;
    model = mdl->model;
    mdl->texInfo = texInfo;
    outs = mdl->groupOut;
    for (k = 0; k < model->groupCount; k++) {
        out = &outs[k];
        grp = &model->groups[k];
        col = grp->colors;
        out->prims = prim;
        out->unk0 = 0;
        out->unk8 = 0;
        for (j = 0; j < 2; j++) {
            src = grp->prims;
            pFT4 = (POLY_FT4 *)prim;
            sFT4 = (WmSrcFT4 *)src;
            for (i = 0; i < grp->ft4Count; i++) {
                setPolyFT4(pFT4);
                pFT4->r0 = sFT4->rgb[0];
                pFT4->g0 = sFT4->rgb[1];
                pFT4->b0 = sFT4->rgb[2];
                pFT4->u0 = grp->uvs[sFT4->uvs[0]][0];
                pFT4->v0 = grp->uvs[sFT4->uvs[0]][1];
                pFT4->u1 = grp->uvs[sFT4->uvs[1]][0];
                pFT4->v1 = grp->uvs[sFT4->uvs[1]][1];
                pFT4->u2 = grp->uvs[sFT4->uvs[2]][0];
                pFT4->v2 = grp->uvs[sFT4->uvs[2]][1];
                pFT4->u3 = grp->uvs[sFT4->uvs[3]][0];
                pFT4->v3 = grp->uvs[sFT4->uvs[3]][1];
                pFT4->clut = mdl->texInfo[sFT4->texture].clut;
                pFT4->tpage = tp = mdl->texInfo[sFT4->texture].tpage & ~TPAGE_ABR_ADD;
                if (sFT4->flags & WMSRC_SEMITRANS) {
                    pFT4->tpage = tp | (sFT4->flags & WMSRC_TPAGE_BITS);
                    setSemiTrans(pFT4, 1);
                }
                prim += sizeof(POLY_FT4);
                du = uvOffsets[sFT4->texture][0];
                dv = uvOffsets[sFT4->texture][1];
                pFT4->u0 += du;
                pFT4->v0 += dv;
                pFT4->u1 += du;
                pFT4->v1 += dv;
                pFT4->u2 += du;
                pFT4->v2 += dv;
                pFT4->u3 += du;
                pFT4->v3 += dv;
                pFT4++;
                sFT4++;
                src += sizeof(WmSrcFT4);
            }
            pFT3 = (POLY_FT3 *)prim;
            sFT3 = (WmSrcFT3 *)src;
            for (i = 0; i < grp->ft3Count; i++) {
                setPolyFT3(pFT3);
                pFT3->r0 = sFT3->rgb[0];
                pFT3->g0 = sFT3->rgb[1];
                pFT3->b0 = sFT3->rgb[2];
                pFT3->u0 = grp->uvs[sFT3->uvs[0]][0];
                pFT3->v0 = grp->uvs[sFT3->uvs[0]][1];
                pFT3->u1 = grp->uvs[sFT3->uvs[1]][0];
                pFT3->v1 = grp->uvs[sFT3->uvs[1]][1];
                pFT3->u2 = grp->uvs[sFT3->uvs[2]][0];
                pFT3->v2 = grp->uvs[sFT3->uvs[2]][1];
                pFT3->clut = mdl->texInfo[sFT3->texture].clut;
                pFT3->tpage = tp = mdl->texInfo[sFT3->texture].tpage & ~TPAGE_ABR_ADD;
                if (sFT3->flags & WMSRC_SEMITRANS) {
                    pFT3->tpage = tp | (sFT3->flags & WMSRC_TPAGE_BITS);
                    setSemiTrans(pFT3, 1);
                }
                prim += sizeof(POLY_FT3);
                du = uvOffsets[sFT3->texture][0];
                dv = uvOffsets[sFT3->texture][1];
                pFT3->u0 += du;
                pFT3->v0 += dv;
                pFT3->u1 += du;
                pFT3->v1 += dv;
                pFT3->u2 += du;
                pFT3->v2 += dv;
                pFT3++;
                sFT3++;
                src += sizeof(WmSrcFT3);
            }
            pGT4 = (POLY_GT4 *)prim;
            sGT4 = (WmSrcGT4 *)src;
            for (i = 0; i < grp->gt4Count; i++) {
                setPolyGT4(pGT4);
                rgb = col[sGT4->colors[0]];
                pGT4->r0 = rgb[0];
                pGT4->g0 = rgb[1];
                pGT4->b0 = rgb[2];
                rgb = col[sGT4->colors[1]];
                pGT4->r1 = rgb[0];
                pGT4->g1 = rgb[1];
                pGT4->b1 = rgb[2];
                rgb = col[sGT4->colors[2]];
                pGT4->r2 = rgb[0];
                pGT4->g2 = rgb[1];
                pGT4->b2 = rgb[2];
                rgb = col[sGT4->colors[3]];
                pGT4->r3 = rgb[0];
                pGT4->g3 = rgb[1];
                pGT4->b3 = rgb[2];
                pGT4->u0 = grp->uvs[sGT4->uvs[0]][0];
                pGT4->v0 = grp->uvs[sGT4->uvs[0]][1];
                pGT4->u1 = grp->uvs[sGT4->uvs[1]][0];
                pGT4->v1 = grp->uvs[sGT4->uvs[1]][1];
                pGT4->u2 = grp->uvs[sGT4->uvs[2]][0];
                pGT4->v2 = grp->uvs[sGT4->uvs[2]][1];
                pGT4->u3 = grp->uvs[sGT4->uvs[3]][0];
                pGT4->v3 = grp->uvs[sGT4->uvs[3]][1];
                pGT4->clut = mdl->texInfo[sGT4->texture].clut;
                pGT4->tpage = tp = mdl->texInfo[sGT4->texture].tpage & ~TPAGE_ABR_ADD;
                if (sGT4->flags & WMSRC_SEMITRANS) {
                    pGT4->tpage = tp | (sGT4->flags & WMSRC_TPAGE_BITS);
                    setSemiTrans(pGT4, 1);
                }
                prim += sizeof(POLY_GT4);
                du = uvOffsets[sGT4->texture][0];
                dv = uvOffsets[sGT4->texture][1];
                pGT4->u0 += du;
                pGT4->v0 += dv;
                pGT4->u1 += du;
                pGT4->v1 += dv;
                pGT4->u2 += du;
                pGT4->v2 += dv;
                pGT4->u3 += du;
                pGT4->v3 += dv;
                pGT4++;
                sGT4++;
                src += sizeof(WmSrcGT4);
            }
            pGT3 = (POLY_GT3 *)prim;
            sGT3 = (WmSrcGT3 *)src;
            for (i = 0; i < grp->gt3Count; i++) {
                setPolyGT3(pGT3);
                rgb = col[sGT3->colors[0]];
                pGT3->r0 = rgb[0];
                pGT3->g0 = rgb[1];
                pGT3->b0 = rgb[2];
                rgb = col[sGT3->colors[1]];
                pGT3->r1 = rgb[0];
                pGT3->g1 = rgb[1];
                pGT3->b1 = rgb[2];
                rgb = col[sGT3->colors[2]];
                pGT3->r2 = rgb[0];
                pGT3->g2 = rgb[1];
                pGT3->b2 = rgb[2];
                pGT3->u0 = grp->uvs[sGT3->uvs[0]][0];
                pGT3->v0 = grp->uvs[sGT3->uvs[0]][1];
                pGT3->u1 = grp->uvs[sGT3->uvs[1]][0];
                pGT3->v1 = grp->uvs[sGT3->uvs[1]][1];
                pGT3->u2 = grp->uvs[sGT3->uvs[2]][0];
                pGT3->v2 = grp->uvs[sGT3->uvs[2]][1];
                pGT3->clut = mdl->texInfo[sGT3->texture].clut;
                pGT3->tpage = tp = mdl->texInfo[sGT3->texture].tpage & ~TPAGE_ABR_ADD;
                if (sGT3->flags & WMSRC_SEMITRANS) {
                    pGT3->tpage = tp | (sGT3->flags & WMSRC_TPAGE_BITS);
                    setSemiTrans(pGT3, 1);
                }
                prim += sizeof(POLY_GT3);
                du = uvOffsets[sGT3->texture][0];
                dv = uvOffsets[sGT3->texture][1];
                pGT3->u0 += du;
                pGT3->v0 += dv;
                pGT3->u1 += du;
                pGT3->v1 += dv;
                pGT3->u2 += du;
                pGT3->v2 += dv;
                pGT3++;
                sGT3++;
                src += sizeof(WmSrcGT3);
            }
            pG4 = (POLY_G4 *)prim;
            sG4 = (WmSrcG4 *)src;
            for (i = 0; i < grp->g4Count; i++) {
                setPolyG4(pG4);
                rgb = col[sG4->colors[0]];
                pG4->r0 = rgb[0];
                pG4->g0 = rgb[1];
                pG4->b0 = rgb[2];
                rgb = col[sG4->colors[1]];
                pG4->r1 = rgb[0];
                pG4->g1 = rgb[1];
                pG4->b1 = rgb[2];
                rgb = col[sG4->colors[2]];
                pG4->r2 = rgb[0];
                pG4->g2 = rgb[1];
                pG4->b2 = rgb[2];
                rgb = col[sG4->colors[3]];
                pG4->r3 = rgb[0];
                pG4->g3 = rgb[1];
                pG4->b3 = rgb[2];
                prim += sizeof(POLY_G4);
                pG4++;
                sG4++;
                src += sizeof(WmSrcG4);
            }
            pG3 = (POLY_G3 *)prim;
            sG3 = (WmSrcG3 *)src;
            for (i = 0; i < grp->g3Count; i++) {
                setPolyG3(pG3);
                rgb = col[sG3->colors[0]];
                pG3->r0 = rgb[0];
                pG3->g0 = rgb[1];
                pG3->b0 = rgb[2];
                rgb = col[sG3->colors[1]];
                pG3->r1 = rgb[0];
                pG3->g1 = rgb[1];
                pG3->b1 = rgb[2];
                rgb = col[sG3->colors[2]];
                pG3->r2 = rgb[0];
                pG3->g2 = rgb[1];
                pG3->b2 = rgb[2];
                prim += sizeof(POLY_G3);
                pG3++;
                sG3++;
                src += sizeof(WmSrcG3);
            }
            pF4 = (POLY_F4 *)prim;
            sF4 = (WmSrcF4 *)src;
            for (i = 0; i < grp->f4Count; i++) {
                setPolyF4(pF4);
                pF4->r0 = sF4->rgb[0];
                pF4->g0 = sF4->rgb[1];
                pF4->b0 = sF4->rgb[2];
                prim += sizeof(POLY_F4);
                pF4++;
                sF4++;
                src += sizeof(WmSrcF4);
            }
            for (i = 0; i < grp->f3Count; i++) {
                pF3 = (POLY_F3 *)prim;
                sF3 = (WmSrcF3 *)src;
                setPolyF3(pF3);
                pF3->r0 = sF3->rgb[0];
                pF3->g0 = sF3->rgb[1];
                pF3->b0 = sF3->rgb[2];
                prim += sizeof(POLY_F3);
                src += sizeof(WmSrcF3);
            }
        }
    }
}

void wm_drawMeshC(ModelObj *mdl, u8 groupIdx) {
    WmModelGroup *grp;
    u32 *ot;
    s32 opz;
    s32 otz;
    u32 primCount;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 idx01, idx23;
    u32 i0, i1, i2, i3;
    u8 *prim;
    WmSrcFT4 *sFT4;
    POLY_FT4 *pFT4;
    WmSrcFT3 *sFT3;
    POLY_FT3 *pFT3;
    WmSrcGT4 *sGT4;
    POLY_GT4 *pGT4;
    WmSrcGT3 *sGT3;
    POLY_GT3 *pGT3;
    WmSrcG4 *sG4;
    POLY_G4 *pG4;
    WmSrcG3 *sG3;
    POLY_G3 *pG3;
    WmSrcF4 *sF4;
    POLY_F4 *pF4;
    WmSrcF3 *sF3;
    POLY_F3 *pF3;

    ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf];
    grp = &mdl->model->groups[groupIdx];
    if (g_gameState->gameData->drawBuf != 0) {
        prim = (u8 *)mdl->groupOut[groupIdx].prims + grp->packetSize;
    } else {
        prim = mdl->groupOut[groupIdx].prims;
    }

    primCount = grp->ft4Count;
    pFT4 = (POLY_FT4 *)prim;
    ot -= grp->otBias;
    sFT4 = (WmSrcFT4 *)(void *)grp->prims;
    for (i = 0; i < primCount; sFT4++, pFT4++) {
        idx01 = *(u32 *)&sFT4->vertices[0];
        idx23 = *(u32 *)&sFT4->vertices[2];
        i1 = idx01 >> 16;
        i3 = idx23 >> 16;
        i0 = idx01 & 0xFFFF;
        i2 = idx23 & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pFT4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = (g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3]) >> 5;
        *(s32 *)&pFT4->x0 = xy0;
        *(s32 *)&pFT4->x1 = xy1;
        *(s32 *)&pFT4->x2 = xy2;
        *(s32 *)&pFT4->x3 = g_vtxScreenXY[i3];
        addPrim(&ot[otz], pFT4);
    }

    primCount = grp->ft3Count;
    pFT3 = (POLY_FT3 *)pFT4;
    sFT3 = (WmSrcFT3 *)sFT4;
    for (i = 0; i < primCount; sFT3++, pFT3++) {
        idx01 = *(u32 *)&sFT3->vertices[0];
        i2 = sFT3->vertices[2];
        i1 = idx01 >> 16;
        i0 = idx01 & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pFT3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pFT3->x2 = xy2;
        otz >>= 1; /* shifts the previous value: overwritten below */
        gte_stotz(&otz);
        addPrim(&ot[otz], pFT3);
    }

    primCount = grp->gt4Count;
    pGT4 = (POLY_GT4 *)pFT3;
    sGT4 = (WmSrcGT4 *)sFT3;
    for (i = 0; i < primCount; sGT4++, pGT4++) {
        idx01 = *(u32 *)&sGT4->vertices[0];
        idx23 = *(u32 *)&sGT4->vertices[2];
        i1 = idx01 >> 16;
        i3 = idx23 >> 16;
        i0 = idx01 & 0xFFFF;
        i2 = idx23 & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pGT4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3];
        *(s32 *)&pGT4->x0 = xy0;
        *(s32 *)&pGT4->x1 = xy1;
        *(s32 *)&pGT4->x2 = xy2;
        *(s32 *)&pGT4->x3 = g_vtxScreenXY[i3];
        otz >>= 5;
        addPrim(&ot[otz], pGT4);
    }

    primCount = grp->gt3Count;
    pGT3 = (POLY_GT3 *)pGT4;
    sGT3 = (WmSrcGT3 *)sGT4;
    for (i = 0; i < primCount; sGT3++, pGT3++) {
        idx01 = *(u32 *)&sGT3->vertices[0];
        i2 = sGT3->vertices[2];
        i1 = idx01 >> 16;
        i0 = idx01 & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pGT3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(s32 *)&pGT3->x0 = xy0;
        *(s32 *)&pGT3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pGT3->x2 = xy2;
        gte_stotz(&otz);
        otz >>= 1;
        addPrim(&ot[otz], pGT3);
    }

    primCount = grp->g4Count;
    pG4 = (POLY_G4 *)pGT3;
    sG4 = (WmSrcG4 *)sGT3;
    for (i = 0; i < primCount; sG4++, pG4++) {
        idx01 = *(u32 *)&sG4->vertices[0];
        idx23 = *(u32 *)&sG4->vertices[2];
        i1 = idx01 >> 16;
        i3 = idx23 >> 16;
        i0 = idx01 & 0xFFFF;
        i2 = idx23 & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pG4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = (g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3]) >> 5;
        *(s32 *)&pG4->x0 = xy0;
        *(s32 *)&pG4->x1 = xy1;
        *(s32 *)&pG4->x2 = xy2;
        *(s32 *)&pG4->x3 = g_vtxScreenXY[i3];
        addPrim(&ot[otz], pG4);
    }

    primCount = grp->g3Count;
    pG3 = (POLY_G3 *)pG4;
    sG3 = (WmSrcG3 *)sG4;
    for (i = 0; i < primCount; sG3++, pG3++) {
        idx01 = *(u32 *)&sG3->vertices[0];
        i2 = sG3->vertices[2];
        i1 = idx01 >> 16;
        i0 = idx01 & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pG3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pG3->x2 = xy2;
        gte_stotz(&otz);
        otz >>= 1;
        addPrim(&ot[otz], pG3);
    }

    primCount = grp->f4Count;
    pF4 = (POLY_F4 *)pG3;
    sF4 = (WmSrcF4 *)sG3;
    for (i = 0; i < primCount; sF4++, pF4++) {
        idx01 = *(u32 *)&sF4->vertices[0];
        idx23 = *(u32 *)&sF4->vertices[2];
        i1 = idx01 >> 16;
        i3 = idx23 >> 16;
        i0 = idx01 & 0xFFFF;
        i2 = idx23 & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pF4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3];
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        *(s32 *)&pF4->x3 = g_vtxScreenXY[i3];
        otz >>= 5;
        addPrim(&ot[otz], pF4);
    }

    primCount = grp->f3Count;
    pF3 = (POLY_F3 *)pF4;
    sF3 = (WmSrcF3 *)sF4;
    for (i = 0; i < primCount; sF3++, pF3++) {
        idx01 = *(u32 *)&sF3->vertices[0];
        i2 = sF3->vertices[2];
        i1 = idx01 >> 16;
        i0 = idx01 & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pF3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pF3->x2 = xy2;
        gte_stotz(&otz);
        otz >>= 1;
        addPrim(&ot[otz], pF3);
    }
}

void wm_applyScale(ModelObj *mdl) {
    MATRIX scaleMat;

    if (mdl->flags & MDL_SCALED) {
        *(s32 *)&scaleMat.m[0][0] = ONE;
        *(s32 *)&scaleMat.m[0][2] = 0;
        *(s32 *)&scaleMat.m[1][1] = ONE;
        *(s32 *)&scaleMat.m[2][0] = 0;
        *(s32 *)&scaleMat.m[2][2] = ONE;
        scaleMat.m[0][0] = mdl->scaleX;
        gte_SetRotMatrix(&mdl->worldMatrix);
        gte_ldclmv(&scaleMat);
        gte_rtir();
        scaleMat.m[1][1] = mdl->scaleY;
        gte_stclmv(&mdl->worldMatrix);
        gte_ldclmv(&scaleMat.m[0][1]);
        gte_rtir();
        scaleMat.m[2][2] = mdl->scaleZ;
        gte_stclmv(&mdl->worldMatrix.m[0][1]);
        gte_ldclmv(&scaleMat.m[0][2]);
        gte_rtir();
        gte_stclmv(&mdl->worldMatrix.m[0][2]);
    }
}

void wm_cullBehindCamera(ModelObj *mdl) {
    SVECTOR pos;
    s32 sz;

    mdl->flags &= ~MDL_BEHIND_CAMERA;
    if (mdl->parent != NULL) {
        mdl->flags |= mdl->parent->flags & MDL_BEHIND_CAMERA;
    } else {
        pos.vx = mdl->worldMatrix.t[0];
        pos.vy = mdl->worldMatrix.t[1];
        pos.vz = mdl->worldMatrix.t[2];
        gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
        gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
        gte_ldv0(&pos);
        gte_rtps();
        gte_stsz(&sz);
        if (sz == 0) {
            mdl->flags |= MDL_BEHIND_CAMERA;
        }
    }
}

void wm_calcBoneMatrices(ModelObj *mdl, u16 frame) {
    SVECTOR sv;
    u16 nb;
    VECTOR *vec;
    WmAnimHdr *anim;
    MATRIX *bones;
    MATRIX *mat;
    MATRIX *end;
    MATRIX *parentMat;
    WmAnimKey *key;
    WmAnimKey *key2;
    WmBone *bone;
    SVECTOR *rot;
    s16 *c1;
    s16 *c2;
    s32 w0, w1, w2, w3;

    anim = mdl->anim;
    if (anim == NULL) {
        if (mdl->parent != NULL) {
            /* attached to a bone of the parent model */
            w0 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[0];
            w1 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[1];
            w2 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[2];
            w3 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[3];
            ((s32 *)mdl->boneMatrices)[0] = w0;
            ((s32 *)mdl->boneMatrices)[1] = w1;
            ((s32 *)mdl->boneMatrices)[2] = w2;
            ((s32 *)mdl->boneMatrices)[3] = w3;
            w0 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[4];
            w1 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[5];
            w2 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[6];
            w3 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[7];
            ((s32 *)mdl->boneMatrices)[4] = w0;
            ((s32 *)mdl->boneMatrices)[5] = w1;
            ((s32 *)mdl->boneMatrices)[6] = w2;
            ((s32 *)mdl->boneMatrices)[7] = w3;
            gte_SetRotMatrix(mdl->boneMatrices);
            gte_ldv0(&mdl->attachX);
            gte_rtv0();
            gte_stsv(&sv);
            ((MATRIX *)mdl->boneMatrices)->t[0] += sv.vx;
            ((MATRIX *)mdl->boneMatrices)->t[1] += sv.vy;
            ((MATRIX *)mdl->boneMatrices)->t[2] += sv.vz;
        } else {
            ((MATRIX *)mdl->boneMatrices)->m[0][0] = mdl->worldMatrix.m[0][0];
            ((MATRIX *)mdl->boneMatrices)->m[0][1] = -mdl->worldMatrix.m[0][1];
            ((MATRIX *)mdl->boneMatrices)->m[0][2] = -mdl->worldMatrix.m[0][2];
            ((MATRIX *)mdl->boneMatrices)->m[1][0] = mdl->worldMatrix.m[1][0];
            ((MATRIX *)mdl->boneMatrices)->m[1][1] = -mdl->worldMatrix.m[1][1];
            ((MATRIX *)mdl->boneMatrices)->m[1][2] = -mdl->worldMatrix.m[1][2];
            ((MATRIX *)mdl->boneMatrices)->m[2][0] = mdl->worldMatrix.m[2][0];
            ((MATRIX *)mdl->boneMatrices)->m[2][1] = -mdl->worldMatrix.m[2][1];
            ((MATRIX *)mdl->boneMatrices)->m[2][2] = -mdl->worldMatrix.m[2][2];
            ((MATRIX *)mdl->boneMatrices)->t[0] = mdl->worldMatrix.t[0];
            ((MATRIX *)mdl->boneMatrices)->t[1] = mdl->worldMatrix.t[1];
            ((MATRIX *)mdl->boneMatrices)->t[2] = mdl->worldMatrix.t[2];
        }
        return;
    }
    nb = mdl->model->boneCount;
    bones = (MATRIX *)0x1F800018;
    if (nb >= 31) {
        bones = (MATRIX *)mdl->boneMatrices;
    }
    mat = bones;
    vec = (VECTOR *)0x1F800008;
    vec->vx = 0;
    vec->vy = 0;
    key = (WmAnimKey *)anim->rotKeys;
    key2 = (WmAnimKey *)anim->rotKeysLo;
    rot = (SVECTOR *)0x1F800000;
    if (anim->constMask & WMANIM_ROOT_CONST(0)) {
        bones->t[0] = anim->rootPos[0];
    } else {
        bones->t[0] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[0]))[frame];
    }
    if (anim->constMask & WMANIM_ROOT_CONST(1)) {
        mat->t[1] = anim->rootPos[1];
    } else {
        mat->t[1] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[1]))[frame];
    }
    if (anim->constMask & WMANIM_ROOT_CONST(2)) {
        mat->t[2] = anim->rootPos[2];
    } else {
        mat->t[2] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[2]))[frame];
    }
    end = &bones[nb];
    do {
        gte_ldrotkey(key, anim, frame, rot);
        if (key2 != NULL) {
            gte_ldrotkey_lo(key2, anim, frame, rot);
            key2++;
        }
        key++;
        math_rotMatrixZYX_gte(rot, mat);
        mat++;
    } while (mat < end);
    if (mdl->parent != NULL) {
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[0];
            x1 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[1];
            x2 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[2];
            x3 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[3];
            ((s32 *)bones)[0] = x0;
            ((s32 *)bones)[1] = x1;
            ((s32 *)bones)[2] = x2;
            ((s32 *)bones)[3] = x3;
        }
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[4];
            x1 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[5];
            x2 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[6];
            x3 = ((s32 *)&((MATRIX *)mdl->parent->boneMatrices)[mdl->parentBone])[7];
            ((s32 *)bones)[4] = x0;
            ((s32 *)bones)[5] = x1;
            ((s32 *)bones)[6] = x2;
            ((s32 *)bones)[7] = x3;
        }
        gte_SetRotMatrix(bones);
        gte_ldv0(&mdl->attachX);
        gte_rtv0();
        gte_stsv(&sv);
        bones->t[0] += sv.vx;
        bones->t[1] += sv.vy;
        bones->t[2] += sv.vz;
    } else {
        gte_SetRotMatrix(&mdl->worldMatrix);
        gte_SetTransMatrix(&mdl->worldMatrix);
        gte_ldclmv(bones);
        c1 = &bones->m[0][1];
        c2 = &bones->m[0][2];
        gte_rtir_b();
        gte_stclmv(bones);
        gte_ldclmv(c1);
        gte_rtir();
        gte_stclmv(c1);
        gte_ldclmv(c2);
        c1 = (s16 *)bones->t;
        gte_rtir_n1();
        gte_stclmv(c2);
        gte_ldlv0(c1);
        gte_rtv0tr();
        gte_stlvnl(c1);
    }
    bone = (WmBone *)mdl->model->bones + 1;
    mat = bones + 1;
    parentMat = &bones[bone->parent];
    do {
        gte_SetRotMatrix(parentMat);
        gte_ldclmv(mat);
        gte_rtir();
        gte_SetTransMatrix(parentMat);
        gte_stclmv(mat);
        gte_ldclmv(&mat->m[0][1]);
        gte_rtir();
        vec->vz = bone->length;
        gte_stclmv(&mat->m[0][1]);
        gte_ldclmv(&mat->m[0][2]);
        gte_rtir();
        bone++;
        gte_stclmv(&mat->m[0][2]);
        gte_ldlv0(vec);
        gte_rtv0tr();
        parentMat = &bones[bone->parent];
        gte_stlvnl(mat->t);
        mat++;
    } while (mat < end);
    if (nb < 31) {
        mat = (MATRIX *)mdl->boneMatrices;
        do {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)bones)[0];
            x1 = ((s32 *)bones)[1];
            x2 = ((s32 *)bones)[2];
            x3 = ((s32 *)bones)[3];
            ((s32 *)mat)[0] = x0;
            ((s32 *)mat)[1] = x1;
            ((s32 *)mat)[2] = x2;
            ((s32 *)mat)[3] = x3;
            x0 = ((s32 *)bones)[4];
            x1 = ((s32 *)bones)[5];
            x2 = ((s32 *)bones)[6];
            x3 = ((s32 *)bones)[7];
            ((s32 *)mat)[4] = x0;
            ((s32 *)mat)[5] = x1;
            ((s32 *)mat)[6] = x2;
            ((s32 *)mat)[7] = x3;
            mat++;
            bones++;
        } while (bones < end);
    }
}

void wm_projectMeshClipY(ModelObj *mdl, u8 groupIdx) {
    WmModelGroup *grp;
    MATRIX *bone;
    SVECTOR *vtx;
    s32 *sxy;
    s32 *sz;
    s32 k;
    u32 i;
    u32 j;
    u32 vtxCount;
    u32 cnt;
    s32 ymax;

    gte_SetGeomScreen(g_gameState->gameData->projDist);
    sxy = g_vtxScreenXY;
    sz = g_vtxScreenZ;
    k = 0;
    grp = &mdl->model->groups[groupIdx];
    bone = (MATRIX *)mdl->boneMatrices;
    cnt = mdl->model->boneCount;
    ymax = (s16)mdl->clipY;
    vtx = grp->vertices;
    for (i = 0; i < cnt; bone++) {
        vtxCount = ((u16 *)grp->boneVertexCounts)[i++];
        if (vtxCount != 0) {
            sxy = &g_vtxScreenXY[k];
            sz = &g_vtxScreenZ[k];
            gte_SetRotMatrix(bone);
            gte_SetTransMatrix(bone);
            j = 0;
            do {
                gte_ldv0(vtx);
                j++;
                vtx++;
                gte_rtv0tr_b();
                gte_stsvclamp(sxy, sz, ymax);
                sxy++;
                sz++;
            } while (j < vtxCount);
            sxy = &g_vtxScreenXY[k];
            sz = &g_vtxScreenZ[k];
            gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
            gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
            j = 0;
            do {
                gte_ldv3xyz(sxy, sz);
                j += 3;
                gte_rtpt_b();
                gte_stsxy3c(sxy);
                sxy += 3;
                gte_stsz3h(sz);
                sz += 3;
            } while (j < vtxCount);
            k += vtxCount;
        }
    }
}

void wm_drawMeshClipY(ModelObj *mdl, u8 groupIdx) {
    WmModelGroup *grp;
    u32 *ot;
    s32 opz;
    s32 otz;
    u32 primCount;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 idx01, idx23;
    u32 i1, i3;
    u8 *prim;
    WmSrcFT4 *sFT4;
    POLY_FT4 *pFT4;
    WmSrcFT3 *sFT3;
    POLY_FT3 *pFT3;
    WmSrcGT4 *sGT4;
    POLY_GT4 *pGT4;
    WmSrcGT3 *sGT3;
    POLY_GT3 *pGT3;
    WmSrcG4 *sG4;
    POLY_G4 *pG4;
    WmSrcG3 *sG3;
    POLY_G3 *pG3;
    WmSrcF4 *sF4;
    POLY_F4 *pF4;
    WmSrcF3 *sF3;
    POLY_F3 *pF3;

    grp = &mdl->model->groups[groupIdx];
    ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf];
    if (g_gameState->gameData->drawBuf != 0) {
        prim = (u8 *)mdl->groupOut[groupIdx].prims + grp->packetSize;
    } else {
        prim = mdl->groupOut[groupIdx].prims;
    }

    primCount = grp->ft4Count;
    pFT4 = (POLY_FT4 *)prim;
    sFT4 = (WmSrcFT4 *)(void *)grp->prims;
    for (i = 0; i < primCount; sFT4++, pFT4++) {
        idx01 = *(u32 *)&sFT4->vertices[0];
        idx23 = *(u32 *)&sFT4->vertices[2];
        i1 = idx01 >> 16;
        i3 = idx23 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23 & 0xFFFF];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[idx23 & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sFT4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pFT4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        otz >>= 5;
        *(s32 *)&pFT4->x0 = xy0;
        *(s32 *)&pFT4->x1 = xy1;
        *(s32 *)&pFT4->x2 = xy2;
        *(s32 *)&pFT4->x3 = g_vtxScreenXY[i3];
        addPrim(&ot[otz], pFT4);
    }

    primCount = grp->ft3Count;
    sFT3 = (WmSrcFT3 *)sFT4;
    pFT3 = (POLY_FT3 *)pFT4;
    for (i = 0; i < primCount; sFT3++, pFT3++) {
        idx01 = *(u32 *)&sFT3->vertices[0];
        idx23 = sFT3->vertices[2];
        i1 = idx01 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)idx23];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sFT3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pFT3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = (otz & 0xFFFFF) / 3;
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        *(s32 *)&pFT3->x2 = xy2;
        addPrim(&ot[otz >> 3], pFT3);
    }

    primCount = grp->gt4Count;
    sGT4 = (WmSrcGT4 *)sFT3;
    pGT4 = (POLY_GT4 *)pFT3;
    for (i = 0; i < primCount; sGT4++, pGT4++) {
        idx01 = *(u32 *)&sGT4->vertices[0];
        idx23 = *(u32 *)&sGT4->vertices[2];
        i1 = idx01 >> 16;
        i3 = idx23 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23 & 0xFFFF];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[idx23 & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sGT4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pGT4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        otz >>= 5;
        *(s32 *)&pGT4->x0 = xy0;
        *(s32 *)&pGT4->x1 = xy1;
        *(s32 *)&pGT4->x2 = xy2;
        *(s32 *)&pGT4->x3 = g_vtxScreenXY[i3];
        addPrim(&ot[otz], pGT4);
    }

    primCount = grp->gt3Count;
    sGT3 = (WmSrcGT3 *)sGT4;
    pGT3 = (POLY_GT3 *)pGT4;
    for (i = 0; i < primCount; sGT3++, pGT3++) {
        idx01 = *(u32 *)&sGT3->vertices[0];
        idx23 = sGT3->vertices[2];
        i1 = idx01 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)idx23];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sGT3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pGT3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = (otz & 0xFFFFF) / 3;
        *(s32 *)&pGT3->x0 = xy0;
        *(s32 *)&pGT3->x1 = xy1;
        *(s32 *)&pGT3->x2 = xy2;
        addPrim(&ot[otz >> 3], pGT3);
    }

    primCount = grp->g4Count;
    sG4 = (WmSrcG4 *)sGT3;
    pG4 = (POLY_G4 *)pGT3;
    for (i = 0; i < primCount; sG4++, pG4++) {
        idx01 = *(u32 *)&sG4->vertices[0];
        idx23 = *(u32 *)&sG4->vertices[2];
        i1 = idx01 >> 16;
        i3 = idx23 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23 & 0xFFFF];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[idx23 & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sG4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pG4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        otz >>= 5;
        *(s32 *)&pG4->x0 = xy0;
        *(s32 *)&pG4->x1 = xy1;
        *(s32 *)&pG4->x2 = xy2;
        *(s32 *)&pG4->x3 = g_vtxScreenXY[i3];
        addPrim(&ot[otz], pG4);
    }

    primCount = grp->g3Count;
    sG3 = (WmSrcG3 *)sG4;
    pG3 = (POLY_G3 *)pG4;
    for (i = 0; i < primCount; sG3++, pG3++) {
        idx01 = *(u32 *)&sG3->vertices[0];
        idx23 = sG3->vertices[2];
        i1 = idx01 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)idx23];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sG3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pG3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = (otz & 0xFFFFF) / 3;
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        *(s32 *)&pG3->x2 = xy2;
        addPrim(&ot[otz >> 3], pG3);
    }

    primCount = grp->f4Count;
    sF4 = (WmSrcF4 *)sG3;
    pF4 = (POLY_F4 *)pG3;
    for (i = 0; i < primCount; sF4++, pF4++) {
        idx01 = *(u32 *)&sF4->vertices[0];
        idx23 = *(u32 *)&sF4->vertices[2];
        i1 = idx01 >> 16;
        i3 = idx23 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23 & 0xFFFF];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[idx23 & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sF4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pF4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        otz >>= 5;
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        *(s32 *)&pF4->x3 = g_vtxScreenXY[i3];
        addPrim(&ot[otz], pF4);
    }

    primCount = grp->f3Count;
    sF3 = (WmSrcF3 *)sF4;
    pF3 = (POLY_F3 *)pF4;
    for (i = 0; i < primCount; sF3++, pF3++) {
        idx01 = *(u32 *)&sF3->vertices[0];
        idx23 = sF3->vertices[2];
        i1 = idx01 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)idx23];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sF3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pF3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = (otz & 0xFFFFF) / 3;
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        *(s32 *)&pF3->x2 = xy2;
        addPrim(&ot[otz >> 3], pF3);
    }
}

s32 wm_getSpriteFrameCount(u16 *spr) {
    return *spr & WMSPR_COUNT_MASK;
}

s32 wm_drawSprite3D(s32 pkt, WmSprHdr *h, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 fade,
                   s32 tex, s32 clutOfs, s32 sizeShift) {
    VECTOR scaleVec;
    SVECTOR zero;
    MATRIX mat;
    WmSprCtx ctx;
    u16 *palTab;
    u8 *palBase;
    s32 val; /* frame count, then the palette table offset, then the frame data offset (separate locals don't match) */
    s32 tmp; /* frame index, then the palette entry count (separate locals don't match) */
    u16 *frameOfs;

    if (rot == NULL) {
        rot = &zero;
        ((s32 *)&zero)[0] = 0;
        ((s32 *)&zero)[1] = 0;
    }
    if (clutOfs != 0) {
        h->clut += clutOfs;
    }
    val = h->frameCount & WMSPR_COUNT_MASK;
    tmp = frame;
    if (tmp >= val) {
        tmp = frame % val;
    }
    frameOfs = &h->frameOffsets[tmp];
    val = val * 2 + 8;
    palBase = (u8 *)h + val;
    palTab = (u16 *)palBase;
    tmp = *palTab++;
    ctx.uvs = palTab;
    ctx.colors = palTab + tmp + 1;
    ctx.tpage = h->tpage | ((abr & 3) << 5);
    ctx.clut = h->clut;
    ctx.halfH = (h->height - 1) * 2;
    ctx.halfW = (h->width - 1) * 2;
    val = *frameOfs;
    ctx.quadCount = ((u8 *)h)[val++];
    h = (WmSprHdr *)((u8 *)h + val);
    gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    gte_ldv0(pos);
    gte_rtv0tr();
    math_rotMatrixYXZ(rot, &mat);
    if (scale >= 0) {
        scaleVec.vx = scaleVec.vy = scaleVec.vz = scale;
        ScaleMatrix(&mat, &scaleVec);
    }
    gte_stlvnl(mat.t);
    if (rot->pad != 0) {
        MulRotMatrix(&mat);
    }
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);
    ctx.primCode = (abr == 0xFF) ? 0x2C : 0x2E;
    ctx.otBias = -g_worldOtBias;
    ctx.fade = fade;
    ctx.sizeShift = sizeShift;
    return wm_emitSprite3D((u8 *)h, &ctx, pkt, tex);
}

/* upload every image and clut of an image pack (clut walks the clut RECTs in the second loop) */
void wm_loadImagePack(WmImagePack *pack) {
    RECT rect;
    WmImagePackClut *clut;
    s16 *img;
    s32 i;
    s32 imageCount;
    s32 n2;
    s32 ofs;
    s32 size;
    u8 *pix;

    ofs = pack->clutOfs;
    clut = (WmImagePackClut *)((u8 *)pack + ofs);
    ofs = pack->imageOfs;
    i = 0;
    img = (s16 *)((u8 *)pack + ofs);
    ofs = clut->dataOfs;
    pix = (u8 *)pack + ofs;
    imageCount = pack->imageCount;
    for (; i < imageCount; i++) {
        rect.x = img[0];
        rect.y = img[1];
        rect.w = img[2];
        rect.h = img[3];
        size = rect.w * rect.h * 2;
        img += 4;
        LoadImage(&rect, (u32 *)img);
        img = (s16 *)((u8 *)img + size);
    }
    n2 = clut->count;
    clut = (WmImagePackClut *)clut->rects;
    for (i = 0; i < n2; i++) {
        rect.x = ((RECT *)clut)->x;
        rect.y = ((RECT *)clut)->y;
        rect.w = ((RECT *)clut)->w;
        rect.h = ((RECT *)clut)->h;
        LoadImage(&rect, (u32 *)pix);
        clut = (WmImagePackClut *)((RECT *)clut + 1);
        pix += rect.w * rect.h * 2;
    }
}
