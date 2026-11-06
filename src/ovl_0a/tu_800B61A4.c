#include "ovl_0a.h"

void fldmdl_projectMesh(ModelObj *mdl, u8 groupIdx) {
    MATRIX boneView;
    WmModelGroup *grp;
    MATRIX *bone;
    SVECTOR *vtx;
    s32 *sxy;
    s32 *sz;
    s32 k;
    u32 i;
    s32 vtxLeft;
    u32 cnt;
    MATRIX *curBone;
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
        vtxLeft = ((u16 *)grp->boneVertexCounts)[i++];
        if (vtxLeft != 0) {
            gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
            gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
            curBone = bone;
            gte_ldclmv(curBone);
            c1 = &curBone->m[0][1];
            c2 = &curBone->m[0][2];
            gte_rtir_b();
            sxy = &g_vtxScreenXY[k];
            gte_stclmv(&boneView);
            gte_ldclmv(c1);
            gte_rtir();
            sz = &g_vtxScreenZ[k];
            gte_stclmv(&boneView.m[0][1]);
            gte_ldclmv(c2);
            c1 = (s16 *)curBone->t;
            gte_rtir_n1();
            vtx = &((SVECTOR *)grp->vertices)[k];
            gte_stclmv(&boneView.m[0][2]);
            gte_ldlv0(c1);
            k += vtxLeft;
            gte_rtv0tr_n1();
            gte_ldv3c(vtx);
            gte_stlvnl(boneView.t);
            gte_SetRotMatrix(&boneView);
            gte_SetTransMatrix(&boneView);
            vtxCur = vtx;
            do {
                vtxCur += 3;
                vtxLeft -= 3;
                gte_rtpt_b();
                gte_ldv3c(vtxCur);
                gte_stsxy3c(sxy);
                gte_stsz3c(sz);
                sxy += 3;
                sz += 3;
            } while (vtxLeft > 0);
        }
    }
}

/* attach a model file (relocating it once) and lay out its work buffers at work */
void fldmdl_initModel(ModelObj *mdl, u8 *work) {
    WmModel *wmModel;
    WmModelGroup *grpBase;
    u32 i;
    u32 grpCount;
    u32 k;
    s32 size;
    u8 *pp;
    WmModelGroup *grp;
    WmModelGroup *g2;

    wmModel = mdl->model;
    if (!(wmModel->flags & WMMODEL_RELOCATED)) {
        i = 0;
        wmModel->flags |= WMMODEL_RELOCATED;
        wmModel->bones = (s32)wmModel + wmModel->bones;
        grpBase = (WmModelGroup *)((u8 *)wmModel + (s32)wmModel->groups);
        wmModel->groups = grpBase;
        for (grp = grpBase; i < wmModel->groupCount; i++, grp++) {
            grp->boneVertexCounts = (u8 *)wmModel + (s32)grp->boneVertexCounts;
            grp->vertices = (u8 *)wmModel + (s32)grp->vertices;
            grp->prims = (u8 *)wmModel + (s32)grp->prims;
            grp->uvs = (void *)((u8 *)wmModel + (s32)grp->uvs);
            grp->colors = (void *)((u8 *)wmModel + (s32)grp->colors);
        }
    }
    g2 = mdl->model->groups;
    size = 0;
    if (IS_OFFSET(g2)) {
        g2 = (WmModelGroup *)((u8 *)g2 + (s32)mdl->model);
    }
    grpCount = mdl->model->groupCount;
    for (k = 0; k < grpCount; k++) {
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
    mdl->groupOut = (WmModelOut *)(pp + wmModel->boneCount * 32);
}

void fldmdl_buildPackets(ModelObj *mdl, WmTexInfo *texInfo, s16 (*uvOffsets)[2]) {
    WmModel *wmModel;
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
    wmModel = mdl->model;
    mdl->texInfo = texInfo;
    outs = mdl->groupOut;
    for (k = 0; k < wmModel->groupCount; k++) {
        out = &outs[k];
        grp = &wmModel->groups[k];
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

void fldmdl_drawMeshC(ModelObj *mdl, u8 groupIdx) {
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
        otz = (g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3]) >> 4;
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
        otz >>= 4;
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
        otz = (g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3]) >> 4;
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
        otz >>= 4;
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
        addPrim(&ot[otz], pF3);
    }
}

void fldmdl_applyScale(ModelObj *mdl) {
    MATRIX scaleMtx;

    if (mdl->flags & MDL_SCALED) {
        *(s32 *)&scaleMtx.m[0][0] = ONE;
        *(s32 *)&scaleMtx.m[0][2] = 0;
        *(s32 *)&scaleMtx.m[1][1] = ONE;
        *(s32 *)&scaleMtx.m[2][0] = 0;
        *(s32 *)&scaleMtx.m[2][2] = ONE;
        scaleMtx.m[0][0] = mdl->scaleX;
        gte_SetRotMatrix(&mdl->worldMatrix);
        gte_ldclmv(&scaleMtx);
        gte_rtir();
        scaleMtx.m[1][1] = mdl->scaleY;
        gte_stclmv(&mdl->worldMatrix);
        gte_ldclmv(&scaleMtx.m[0][1]);
        gte_rtir();
        scaleMtx.m[2][2] = mdl->scaleZ;
        gte_stclmv(&mdl->worldMatrix.m[0][1]);
        gte_ldclmv(&scaleMtx.m[0][2]);
        gte_rtir();
        gte_stclmv(&mdl->worldMatrix.m[0][2]);
    }
}

void fldmdl_cullBehindCamera(ModelObj *mdl) {
    SVECTOR origin;
    s32 sz;

    mdl->flags &= ~MDL_BEHIND_CAMERA;
    if (mdl->parent != NULL) {
        mdl->flags |= mdl->parent->flags & MDL_BEHIND_CAMERA;
    } else {
        origin.vx = mdl->worldMatrix.t[0];
        origin.vy = mdl->worldMatrix.t[1];
        origin.vz = mdl->worldMatrix.t[2];
        gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
        gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
        gte_ldv0(&origin);
        gte_rtps();
        gte_stsz(&sz);
        if (sz == 0) {
            mdl->flags |= MDL_BEHIND_CAMERA;
        }
    }
}

void fldmdl_calcBoneMatrices(ModelObj *mdl, u16 frame) {
    SVECTOR sv;
    u16 nb;
    VECTOR *vec;
    WmAnim *anim;
    MATRIX *bones;
    MATRIX *boneMtx;
    MATRIX *end;
    MATRIX *parentMtx;
    WmAnimKey *key;
    WmAnimKey *key2;
    WmBone *bone;
    SVECTOR *rot;
    s16 *c1;
    s16 *c2;
    s32 w0, w1, w2, w3;

    anim = (WmAnim *)mdl->anim;
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
    bones = PSX_SPAD(MATRIX *, 0x18);
    if (nb >= 31) {
        bones = (MATRIX *)mdl->boneMatrices;
    }
    boneMtx = bones;
    vec = PSX_SPAD(VECTOR *, 0x08);
    vec->vx = 0;
    vec->vy = 0;
    key = anim->rotKeys;
    key2 = anim->rotKeysLo;
    rot = PSX_SPAD(SVECTOR *, 0);
    if (anim->constMask & WMANIM_ROOT_CONST(0)) {
        bones->t[0] = anim->rootPos[0];
    } else {
        bones->t[0] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[0]))[frame];
    }
    if (anim->constMask & WMANIM_ROOT_CONST(1)) {
        boneMtx->t[1] = anim->rootPos[1];
    } else {
        boneMtx->t[1] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[1]))[frame];
    }
    if (anim->constMask & WMANIM_ROOT_CONST(2)) {
        boneMtx->t[2] = anim->rootPos[2];
    } else {
        boneMtx->t[2] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[2]))[frame];
    }
    end = &bones[nb];
    do {
        gte_ldrotkey(key, anim, frame, rot);
        if (key2 != NULL) {
            gte_ldrotkey_lo(key2, anim, frame, rot);
            key2++;
        }
        key++;
        math_rotMatrixZYX_gte(rot, boneMtx);
        boneMtx++;
    } while (boneMtx < end);
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
    boneMtx = bones + 1;
    parentMtx = &bones[bone->parent];
    do {
        gte_SetRotMatrix(parentMtx);
        gte_ldclmv(boneMtx);
        gte_rtir();
        gte_SetTransMatrix(parentMtx);
        gte_stclmv(boneMtx);
        gte_ldclmv(&boneMtx->m[0][1]);
        gte_rtir();
        vec->vz = bone->length;
        gte_stclmv(&boneMtx->m[0][1]);
        gte_ldclmv(&boneMtx->m[0][2]);
        gte_rtir();
        bone++;
        gte_stclmv(&boneMtx->m[0][2]);
        gte_ldlv0(vec);
        gte_rtv0tr();
        parentMtx = &bones[bone->parent];
        gte_stlvnl(boneMtx->t);
        boneMtx++;
    } while (boneMtx < end);
    if (nb < 31) {
        boneMtx = (MATRIX *)mdl->boneMatrices;
        do {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)bones)[0];
            x1 = ((s32 *)bones)[1];
            x2 = ((s32 *)bones)[2];
            x3 = ((s32 *)bones)[3];
            ((s32 *)boneMtx)[0] = x0;
            ((s32 *)boneMtx)[1] = x1;
            ((s32 *)boneMtx)[2] = x2;
            ((s32 *)boneMtx)[3] = x3;
            x0 = ((s32 *)bones)[4];
            x1 = ((s32 *)bones)[5];
            x2 = ((s32 *)bones)[6];
            x3 = ((s32 *)bones)[7];
            ((s32 *)boneMtx)[4] = x0;
            ((s32 *)boneMtx)[5] = x1;
            ((s32 *)boneMtx)[6] = x2;
            ((s32 *)boneMtx)[7] = x3;
            boneMtx++;
            bones++;
        } while (bones < end);
    }
}

void fldmdl_addGroupRgb(ModelObj *mdl, u8 groupIdx, s16 dr, s16 dg, s16 db) {
    WmModelGroup *grp;
    u8 *prim;
    u32 j;
    u32 k;

    if (mdl->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    grp = &mdl->model->groups[groupIdx];
    for (k = 0; k < 2; k++) {
        prim = mdl->groupOut[groupIdx].prims;
        if (k != 0) {
            prim += grp->packetSize;
        }
        for (j = 0; j < grp->ft4Count; j++) {
            *(u32 *)&((POLY_FT4 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT4 *)prim)->r0, dr, dg, db);
            prim += sizeof(POLY_FT4);
        }
        for (j = 0; j < grp->ft3Count; j++) {
            *(u32 *)&((POLY_FT3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT3 *)prim)->r0, dr, dg, db);
            prim += sizeof(POLY_FT3);
        }
        for (j = 0; j < grp->gt4Count; j++) {
            *(u32 *)&((POLY_GT4 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)prim)->r0, dr, dg, db);
            *(u32 *)&((POLY_GT4 *)prim)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)prim)->r1, dr, dg, db);
            *(u32 *)&((POLY_GT4 *)prim)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)prim)->r2, dr, dg, db);
            *(u32 *)&((POLY_GT4 *)prim)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)prim)->r3, dr, dg, db);
            prim += sizeof(POLY_GT4);
        }
        for (j = 0; j < grp->gt3Count; j++) {
            *(u32 *)&((POLY_GT3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)prim)->r0, dr, dg, db);
            *(u32 *)&((POLY_GT3 *)prim)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)prim)->r1, dr, dg, db);
            *(u32 *)&((POLY_GT3 *)prim)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)prim)->r2, dr, dg, db);
            prim += sizeof(POLY_GT3);
        }
        for (j = 0; j < grp->g4Count; j++) {
            *(u32 *)&((POLY_G4 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)prim)->r0, dr, dg, db);
            *(u32 *)&((POLY_G4 *)prim)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)prim)->r1, dr, dg, db);
            *(u32 *)&((POLY_G4 *)prim)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)prim)->r2, dr, dg, db);
            *(u32 *)&((POLY_G4 *)prim)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)prim)->r3, dr, dg, db);
            prim += sizeof(POLY_G4);
        }
        for (j = 0; j < grp->g3Count; j++) {
            *(u32 *)&((POLY_G3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)prim)->r0, dr, dg, db);
            *(u32 *)&((POLY_G3 *)prim)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)prim)->r1, dr, dg, db);
            *(u32 *)&((POLY_G3 *)prim)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)prim)->r2, dr, dg, db);
            prim += sizeof(POLY_G3);
        }
        for (j = 0; j < grp->f4Count; j++) {
            *(u32 *)&((POLY_F4 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F4 *)prim)->r0, dr, dg, db);
            prim += sizeof(POLY_F4);
        }
        for (j = 0; j < grp->f3Count; j++) {
            *(u32 *)&((POLY_F3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F3 *)prim)->r0, dr, dg, db);
            prim += sizeof(POLY_F3);
        }
    }
}

void fldmdl_addGroupRgbCur(ModelObj *mdl, u8 groupIdx, s16 dr, s16 dg, s16 db) {
    WmModelGroup *grp;
    u8 *prim;
    u32 j;

    if (mdl->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    grp = &mdl->model->groups[groupIdx];
    prim = mdl->groupOut[groupIdx].prims;
    if (g_gameState->gameData->drawBuf != 0) {
        prim += grp->packetSize;
    }
    for (j = 0; j < grp->ft4Count; j++) {
        *(u32 *)&((POLY_FT4 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT4 *)prim)->r0, dr, dg, db);
        prim += sizeof(POLY_FT4);
    }
    for (j = 0; j < grp->ft3Count; j++) {
        *(u32 *)&((POLY_FT3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT3 *)prim)->r0, dr, dg, db);
        prim += sizeof(POLY_FT3);
    }
    for (j = 0; j < grp->gt4Count; j++) {
        *(u32 *)&((POLY_GT4 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)prim)->r0, dr, dg, db);
        *(u32 *)&((POLY_GT4 *)prim)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)prim)->r1, dr, dg, db);
        *(u32 *)&((POLY_GT4 *)prim)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)prim)->r2, dr, dg, db);
        *(u32 *)&((POLY_GT4 *)prim)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)prim)->r3, dr, dg, db);
        prim += sizeof(POLY_GT4);
    }
    for (j = 0; j < grp->gt3Count; j++) {
        *(u32 *)&((POLY_GT3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)prim)->r0, dr, dg, db);
        *(u32 *)&((POLY_GT3 *)prim)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)prim)->r1, dr, dg, db);
        *(u32 *)&((POLY_GT3 *)prim)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)prim)->r2, dr, dg, db);
        prim += sizeof(POLY_GT3);
    }
    for (j = 0; j < grp->g4Count; j++) {
        *(u32 *)&((POLY_G4 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)prim)->r0, dr, dg, db);
        *(u32 *)&((POLY_G4 *)prim)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)prim)->r1, dr, dg, db);
        *(u32 *)&((POLY_G4 *)prim)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)prim)->r2, dr, dg, db);
        *(u32 *)&((POLY_G4 *)prim)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)prim)->r3, dr, dg, db);
        prim += sizeof(POLY_G4);
    }
    for (j = 0; j < grp->g3Count; j++) {
        *(u32 *)&((POLY_G3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)prim)->r0, dr, dg, db);
        *(u32 *)&((POLY_G3 *)prim)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)prim)->r1, dr, dg, db);
        *(u32 *)&((POLY_G3 *)prim)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)prim)->r2, dr, dg, db);
        prim += sizeof(POLY_G3);
    }
    for (j = 0; j < grp->f4Count; j++) {
        *(u32 *)&((POLY_F4 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F4 *)prim)->r0, dr, dg, db);
        prim += sizeof(POLY_F4);
    }
    for (j = 0; j < grp->f3Count; j++) {
        *(u32 *)&((POLY_F3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F3 *)prim)->r0, dr, dg, db);
        prim += sizeof(POLY_F3);
    }
}

void fldmdl_tintGroup(ModelObj *mdl, u8 groupIdx, s16 dr, s16 dg, s16 db) {
    u32 *col;
    WmModelGroup *grp;
    u8 *prim;
    u8 *src;
    u32 j;
    u32 k;
    u8 primCode;

    if (mdl->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    grp = &mdl->model->groups[groupIdx];
    for (k = 0; k < 2; k++) {
        src = grp->prims;
        prim = mdl->groupOut[groupIdx].prims;
        if (k != 0) {
            prim += grp->packetSize;
        }
        for (j = 0; j < grp->ft4Count; j++) {
            primCode = ((POLY_FT4 *)prim)->code;
            *(u32 *)&((POLY_FT4 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT4 *)src)->rgb, dr, dg, db);
            ((POLY_FT4 *)prim)->code = primCode;
            prim += sizeof(POLY_FT4);
            src += sizeof(WmSrcFT4);
        }

        for (j = 0; j < grp->ft3Count; j++) {
            primCode = ((POLY_FT3 *)prim)->code;
            *(u32 *)&((POLY_FT3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT3 *)src)->rgb, dr, dg, db);
            ((POLY_FT3 *)prim)->code = primCode;
            prim += sizeof(POLY_FT3);
            src += sizeof(WmSrcFT3);
        }

        for (j = 0; j < grp->gt4Count; j++) {
            primCode = ((POLY_GT4 *)prim)->code;
            col = (u32 *)grp->colors[((WmSrcGT4 *)src)->colors[0]];
            *(u32 *)&((POLY_GT4 *)prim)->r0 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcGT4 *)src)->colors[1]];
            *(u32 *)&((POLY_GT4 *)prim)->r1 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcGT4 *)src)->colors[2]];
            *(u32 *)&((POLY_GT4 *)prim)->r2 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcGT4 *)src)->colors[3]];
            *(u32 *)&((POLY_GT4 *)prim)->r3 = fldmdl_addRgbClamped(*col, dr, dg, db);
            ((POLY_GT4 *)prim)->code = primCode;
            prim += sizeof(POLY_GT4);
            src += sizeof(WmSrcGT4);
        }

        for (j = 0; j < grp->gt3Count; j++) {
            primCode = ((POLY_GT3 *)prim)->code;
            col = (u32 *)grp->colors[((WmSrcGT3 *)src)->colors[0]];
            *(u32 *)&((POLY_GT3 *)prim)->r0 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcGT3 *)src)->colors[1]];
            *(u32 *)&((POLY_GT3 *)prim)->r1 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcGT3 *)src)->colors[2]];
            *(u32 *)&((POLY_GT3 *)prim)->r2 = fldmdl_addRgbClamped(*col, dr, dg, db);
            ((POLY_GT3 *)prim)->code = primCode;
            prim += sizeof(POLY_GT3);
            src += sizeof(WmSrcGT3);
        }

        for (j = 0; j < grp->g4Count; j++) {
            primCode = ((POLY_G4 *)prim)->code;
            col = (u32 *)grp->colors[((WmSrcG4 *)src)->colors[0]];
            *(u32 *)&((POLY_G4 *)prim)->r0 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcG4 *)src)->colors[1]];
            *(u32 *)&((POLY_G4 *)prim)->r1 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcG4 *)src)->colors[2]];
            *(u32 *)&((POLY_G4 *)prim)->r2 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcG4 *)src)->colors[3]];
            *(u32 *)&((POLY_G4 *)prim)->r3 = fldmdl_addRgbClamped(*col, dr, dg, db);
            ((POLY_G4 *)prim)->code = primCode;
            prim += sizeof(POLY_G4);
            src += sizeof(WmSrcG4);
        }

        for (j = 0; j < grp->g3Count; j++) {
            primCode = ((POLY_G3 *)prim)->code;
            col = (u32 *)grp->colors[((WmSrcG3 *)src)->colors[0]];
            *(u32 *)&((POLY_G3 *)prim)->r0 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcG3 *)src)->colors[1]];
            *(u32 *)&((POLY_G3 *)prim)->r1 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcG3 *)src)->colors[2]];
            *(u32 *)&((POLY_G3 *)prim)->r2 = fldmdl_addRgbClamped(*col, dr, dg, db);
            ((POLY_G3 *)prim)->code = primCode;
            prim += sizeof(POLY_G3);
            src += sizeof(WmSrcG3);
        }

        for (j = 0; j < grp->f4Count; j++) {
            primCode = ((POLY_F4 *)prim)->code;
            *(u32 *)&((POLY_F4 *)prim)->r0 |= fldmdl_addRgbClamped(*(u32 *)((WmSrcF4 *)src)->rgb, dr, dg, db);
            ((POLY_F4 *)prim)->code = primCode;
            prim += sizeof(POLY_F4);
            src += sizeof(WmSrcF4);
        }

        for (j = 0; j < grp->f3Count; j++) {
            primCode = ((POLY_F3 *)prim)->code;
            *(u32 *)&((POLY_F3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcF3 *)src)->rgb, dr, dg, db);
            ((POLY_F3 *)prim)->code = primCode;
            prim += sizeof(POLY_F3);
            src += sizeof(WmSrcF3);
        }
    }
}

/* recolour a model group in the current draw buffer (linked prims only) */
void fldmdl_tintGroupCur(ModelObj *mdl, u8 groupIdx, s16 dr, s16 dg, s16 db) {
    u32 *col;
    WmModelGroup *grp;
    u8 *prim;
    u8 *src;
    u32 j;
    u8 primCode;

    if (mdl->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    grp = &mdl->model->groups[groupIdx];
    prim = mdl->groupOut[groupIdx].prims;
    src = grp->prims;
    if (g_gameState->gameData->drawBuf != 0) {
        prim += grp->packetSize;
    }
    for (j = 0; j < grp->ft4Count; j++) {
        if (*(u32 *)prim & PRIM_ADDR_MASK) {
            primCode = ((POLY_FT4 *)prim)->code;
            *(u32 *)&((POLY_FT4 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT4 *)src)->rgb, dr, dg, db);
            ((POLY_FT4 *)prim)->code = primCode;
        }
        prim += sizeof(POLY_FT4);
        src += sizeof(WmSrcFT4);
    }

    for (j = 0; j < grp->ft3Count; j++) {
        if (*(u32 *)prim & PRIM_ADDR_MASK) {
            primCode = ((POLY_FT3 *)prim)->code;
            *(u32 *)&((POLY_FT3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT3 *)src)->rgb, dr, dg, db);
            ((POLY_FT3 *)prim)->code = primCode;
        }
        prim += sizeof(POLY_FT3);
        src += sizeof(WmSrcFT3);
    }

    for (j = 0; j < grp->gt4Count; j++) {
        if (*(u32 *)prim & PRIM_ADDR_MASK) {
            primCode = ((POLY_GT4 *)prim)->code;
            col = (u32 *)grp->colors[((WmSrcGT4 *)src)->colors[0]];
            *(u32 *)&((POLY_GT4 *)prim)->r0 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcGT4 *)src)->colors[1]];
            *(u32 *)&((POLY_GT4 *)prim)->r1 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcGT4 *)src)->colors[2]];
            *(u32 *)&((POLY_GT4 *)prim)->r2 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcGT4 *)src)->colors[3]];
            *(u32 *)&((POLY_GT4 *)prim)->r3 = fldmdl_addRgbClamped(*col, dr, dg, db);
            ((POLY_GT4 *)prim)->code = primCode;
        }
        prim += sizeof(POLY_GT4);
        src += sizeof(WmSrcGT4);
    }

    for (j = 0; j < grp->gt3Count; j++) {
        if (*(u32 *)prim & PRIM_ADDR_MASK) {
            primCode = ((POLY_GT3 *)prim)->code;
            col = (u32 *)grp->colors[((WmSrcGT3 *)src)->colors[0]];
            *(u32 *)&((POLY_GT3 *)prim)->r0 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcGT3 *)src)->colors[1]];
            *(u32 *)&((POLY_GT3 *)prim)->r1 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcGT3 *)src)->colors[2]];
            *(u32 *)&((POLY_GT3 *)prim)->r2 = fldmdl_addRgbClamped(*col, dr, dg, db);
            ((POLY_GT3 *)prim)->code = primCode;
        }
        prim += sizeof(POLY_GT3);
        src += sizeof(WmSrcGT3);
    }

    for (j = 0; j < grp->g4Count; j++) {
        if (*(u32 *)prim & PRIM_ADDR_MASK) {
            primCode = ((POLY_G4 *)prim)->code;
            col = (u32 *)grp->colors[((WmSrcG4 *)src)->colors[0]];
            *(u32 *)&((POLY_G4 *)prim)->r0 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcG4 *)src)->colors[1]];
            *(u32 *)&((POLY_G4 *)prim)->r1 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcG4 *)src)->colors[2]];
            *(u32 *)&((POLY_G4 *)prim)->r2 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcG4 *)src)->colors[3]];
            *(u32 *)&((POLY_G4 *)prim)->r3 = fldmdl_addRgbClamped(*col, dr, dg, db);
            ((POLY_G4 *)prim)->code = primCode;
        }
        prim += sizeof(POLY_G4);
        src += sizeof(WmSrcG4);
    }

    for (j = 0; j < grp->g3Count; j++) {
        if (*(u32 *)prim & PRIM_ADDR_MASK) {
            primCode = ((POLY_G3 *)prim)->code;
            col = (u32 *)grp->colors[((WmSrcG3 *)src)->colors[0]];
            *(u32 *)&((POLY_G3 *)prim)->r0 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcG3 *)src)->colors[1]];
            *(u32 *)&((POLY_G3 *)prim)->r1 = fldmdl_addRgbClamped(*col, dr, dg, db);
            col = (u32 *)grp->colors[((WmSrcG3 *)src)->colors[2]];
            *(u32 *)&((POLY_G3 *)prim)->r2 = fldmdl_addRgbClamped(*col, dr, dg, db);
            ((POLY_G3 *)prim)->code = primCode;
        }
        prim += sizeof(POLY_G3);
        src += sizeof(WmSrcG3);
    }

    for (j = 0; j < grp->f4Count; j++) {
        if (*(u32 *)prim & PRIM_ADDR_MASK) {
            primCode = ((POLY_F4 *)prim)->code;
            *(u32 *)&((POLY_F4 *)prim)->r0 |= fldmdl_addRgbClamped(*(u32 *)((WmSrcF4 *)src)->rgb, dr, dg, db);
            ((POLY_F4 *)prim)->code = primCode;
        }
        prim += sizeof(POLY_F4);
        src += sizeof(WmSrcF4);
    }

    for (j = 0; j < grp->f3Count; j++) {
        if (*(u32 *)prim & PRIM_ADDR_MASK) {
            primCode = ((POLY_F3 *)prim)->code;
            *(u32 *)&((POLY_F3 *)prim)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcF3 *)src)->rgb, dr, dg, db);
            ((POLY_F3 *)prim)->code = primCode;
        }
        prim += sizeof(POLY_F3);
        src += sizeof(WmSrcF3);
    }
}

void fldmdl_setGroupRgb(ModelObj *mdl, u8 groupIdx, s16 red, s16 green, s16 blue) {
    WmModelGroup *grp;
    u32 rgb;
    u8 *prim;
    u32 j;

    if (mdl->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    grp = &mdl->model->groups[groupIdx];
    rgb = (blue << 16) | (green << 8) | red;
    if (g_gameState->gameData->drawBuf != 0) {
        prim = mdl->groupOut[groupIdx].prims + grp->packetSize;
    } else {
        prim = mdl->groupOut[groupIdx].prims;
    }
    for (j = 0; j < grp->ft4Count; j++) {
        *(u32 *)&((POLY_FT4 *)prim)->r0 = rgb | (((POLY_FT4 *)prim)->code << 24);
        prim += sizeof(POLY_FT4);
    }
    for (j = 0; j < grp->ft3Count; j++) {
        *(u32 *)&((POLY_FT3 *)prim)->r0 = rgb | (((POLY_FT3 *)prim)->code << 24);
        prim += sizeof(POLY_FT3);
    }
    for (j = 0; j < grp->gt4Count; j++) {
        *(u32 *)&((POLY_GT4 *)prim)->r0 = rgb | (((POLY_GT4 *)prim)->code << 24);
        prim += sizeof(POLY_GT4);
    }
    for (j = 0; j < grp->gt3Count; j++) {
        *(u32 *)&((POLY_GT3 *)prim)->r0 = rgb | (((POLY_GT3 *)prim)->code << 24);
        prim += sizeof(POLY_GT3);
    }
    for (j = 0; j < grp->g4Count; j++) {
        *(u32 *)&((POLY_G4 *)prim)->r0 = rgb | (((POLY_G4 *)prim)->code << 24);
        prim += sizeof(POLY_G4);
    }
    for (j = 0; j < grp->g3Count; j++) {
        *(u32 *)&((POLY_G3 *)prim)->r0 = rgb | (((POLY_G3 *)prim)->code << 24);
        prim += sizeof(POLY_G3);
    }
    for (j = 0; j < grp->f4Count; j++) {
        *(u32 *)&((POLY_F4 *)prim)->r0 = rgb | (((POLY_F4 *)prim)->code << 24);
        prim += sizeof(POLY_F4);
    }
    for (j = 0; j < grp->f3Count; j++) {
        *(u32 *)&((POLY_F3 *)prim)->r0 = rgb | (((POLY_F3 *)prim)->code << 24);
        prim += sizeof(POLY_F3);
    }
}

u32 fldmdl_addRgbClamped(u32 rgb, s32 dr, s32 dg, s32 db) {
    s16 b = db + ((rgb >> 16) & 0xFF);
    s16 g = dg + ((rgb >> 8) & 0xFF);
    s16 r = dr + (rgb & 0xFF);
    rgb &= 0xFF000000;
    if (r >= 0x100) {
        rgb |= 0xFF;
    } else if (r > 0) {
        rgb |= r;
    }
    if (g >= 0x100) {
        rgb |= 0xFF00;
    } else if (g > 0) {
        rgb |= g << 8;
    }
    if (b >= 0x100) {
        rgb |= 0xFF0000;
    } else if (b > 0) {
        rgb |= b << 16;
    }
    return rgb;
}

void fldmdl_setSemiTrans(ModelObj *mdl, u16 force) {
    u32 i;
    u32 j;
    u8 *prim;
    WmModelGroup *grp;
    u8 *src;

    if (mdl->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    for (i = 0; i < mdl->model->groupCount; i++) {
        grp = &mdl->model->groups[i];
        src = grp->prims;
        prim = mdl->groupOut[i].prims;
        if (g_gameState->gameData->drawBuf != 0) {
            prim += grp->packetSize;
        }
        for (j = 0; j < grp->ft4Count; j++) {
            if ((((WmSrcFT4 *)src)->flags & WMSRC_SEMITRANS) || force != 0) {
                setSemiTrans((POLY_FT4 *)prim, 1);
            } else {
                setSemiTrans((POLY_FT4 *)prim, 0);
            }
            prim += sizeof(POLY_FT4);
            src += sizeof(WmSrcFT4);
        }
        for (j = 0; j < grp->ft3Count; j++) {
            if ((((WmSrcFT3 *)src)->flags & WMSRC_SEMITRANS) || force != 0) {
                setSemiTrans((POLY_FT3 *)prim, 1);
            } else {
                setSemiTrans((POLY_FT3 *)prim, 0);
            }
            src += sizeof(WmSrcFT3);
            prim += sizeof(POLY_FT3);
        }
        for (j = 0; j < grp->gt4Count; j++) {
            if ((((WmSrcGT4 *)src)->flags & WMSRC_SEMITRANS) || force != 0) {
                setSemiTrans((POLY_GT4 *)prim, 1);
            } else {
                setSemiTrans((POLY_GT4 *)prim, 0);
            }
            src += sizeof(WmSrcGT4);
            prim += sizeof(POLY_GT4);
        }
        for (j = 0; j < grp->gt3Count; j++) {
            if ((((WmSrcGT3 *)src)->flags & WMSRC_SEMITRANS) || force != 0) {
                setSemiTrans((POLY_GT3 *)prim, 1);
            } else {
                setSemiTrans((POLY_GT3 *)prim, 0);
            }
            src += sizeof(WmSrcGT3);
            prim += sizeof(POLY_GT3);
        }
    }
}

void fldmdl_setAbr(ModelObj *mdl, u16 abr) {
    u32 i;
    u32 j;
    u8 *prim;
    WmModelGroup *grp;

    if (mdl->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    for (i = 0; i < mdl->model->groupCount; i++) {
        grp = &mdl->model->groups[i];
        prim = mdl->groupOut[i].prims;
        if (g_gameState->gameData->drawBuf != 0) {
            prim += grp->packetSize;
        }
        for (j = 0; j < grp->ft4Count; j++) {
            ((POLY_FT4 *)prim)->tpage = abr | (((POLY_FT4 *)prim)->tpage & ~TPAGE_ABR_MASK);
            prim += sizeof(POLY_FT4);
        }
        for (j = 0; j < grp->ft3Count; j++) {
            ((POLY_FT3 *)prim)->tpage = abr | (((POLY_FT3 *)prim)->tpage & ~TPAGE_ABR_MASK);
            prim += sizeof(POLY_FT3);
        }
        for (j = 0; j < grp->gt4Count; j++) {
            ((POLY_GT4 *)prim)->tpage = abr | (((POLY_GT4 *)prim)->tpage & ~TPAGE_ABR_MASK);
            prim += sizeof(POLY_GT4);
        }
        for (j = 0; j < grp->gt3Count; j++) {
            ((POLY_GT3 *)prim)->tpage = abr | (((POLY_GT3 *)prim)->tpage & ~TPAGE_ABR_MASK);
            prim += sizeof(POLY_GT3);
        }
    }
}

void fldmdl_calcBoneMatricesLookAt(ModelObj *mdl, s32 frame) {
    VECTOR lookDir;
    VECTOR up;
    VECTOR r0;
    VECTOR r2;
    VECTOR r1;
    MATRIX scaleMtx;
    u32 idx;
    u32 head;
    MATRIX *end;
    u16 nb;
    VECTOR *vec;
    VECTOR *up_p;
    WmAnim *anim;
    MATRIX *bones;
    MATRIX *boneMtx;
    MATRIX *parentMtx;
    WmAnimKey *key;
    WmAnimKey *key2;
    SVECTOR *rot;
    s16 *d1;
    s16 *d2;
    s32 w0, w1, w2, w3;

    anim = (WmAnim *)mdl->anim;
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
            ((MATRIX *)mdl->boneMatrices)->t[0] += mdl->attachX;
            ((MATRIX *)mdl->boneMatrices)->t[1] += mdl->attachY;
            ((MATRIX *)mdl->boneMatrices)->t[2] += mdl->attachZ;
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
    bones = PSX_SPAD(MATRIX *, 0x18);
    if (nb >= 31) {
        bones = (MATRIX *)mdl->boneMatrices;
    }
    boneMtx = bones;
    vec = PSX_SPAD(VECTOR *, 0x08);
    vec->vx = 0;
    vec->vy = 0;
    key = anim->rotKeys;
    key2 = anim->rotKeysLo;
    rot = PSX_SPAD(SVECTOR *, 0);
    if (anim->constMask & WMANIM_ROOT_CONST(0)) {
        bones->t[0] = anim->rootPos[0];
    } else {
        bones->t[0] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[0]))[(u16)frame];
    }
    if (anim->constMask & WMANIM_ROOT_CONST(1)) {
        boneMtx->t[1] = anim->rootPos[1];
    } else {
        boneMtx->t[1] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[1]))[(u16)frame];
    }
    if (anim->constMask & WMANIM_ROOT_CONST(2)) {
        boneMtx->t[2] = anim->rootPos[2];
    } else {
        boneMtx->t[2] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[2]))[(u16)frame];
    }
    end = &bones[nb];
loop1:
    gte_ldrotkey(key, anim, frame, rot);
    if (key2 != NULL) {
        gte_ldrotkey_lo(key2, anim, frame, rot);
        key2++;
    }
    key++;
    math_rotMatrixZYX_gte(rot, boneMtx);
    boneMtx++;
    if (boneMtx < end) {
        goto loop1;
    }
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
        bones->t[0] += mdl->attachX;
        bones->t[1] += mdl->attachY;
        bones->t[2] += mdl->attachZ;
    } else {
        gte_SetRotMatrix(&mdl->worldMatrix);
        gte_SetTransMatrix(&mdl->worldMatrix);
        gte_ldclmv(bones);
        d1 = &bones->m[0][1];
        d2 = &bones->m[0][2];
        gte_rtir_b();
        gte_stclmv(bones);
        gte_ldclmv(d1);
        gte_rtir();
        gte_stclmv(d1);
        gte_ldclmv(d2);
        d1 = (s16 *)bones->t;
        gte_rtir_n1();
        gte_stclmv(d2);
        gte_ldlv0(d1);
        gte_rtv0tr();
        gte_stlvnl(d1);
    }
    boneMtx = bones + 1;
    idx = 1;
    up_p = &r2;
    key2 = (WmAnimKey *)((WmBone *)mdl->model->bones + 1);
    head = mdl->model->lookAtBone;
    rot = (SVECTOR *)&lookDir;
    parentMtx = &bones[((WmBone *)key2)->parent];
loop:
    gte_SetRotMatrix(parentMtx);
    gte_ldclmv(boneMtx);
    gte_rtir();
    gte_SetTransMatrix(parentMtx);
    gte_stclmv(boneMtx);
    key = (WmAnimKey *)&boneMtx->m[0][1];
    gte_ldclmv(key);
    gte_rtir();
    vec->vz = ((WmBone *)key2)->length;
    gte_stclmv(key);
    frame = (s32)&boneMtx->m[0][2];
    gte_ldclmv((s16 *)frame);
    gte_rtir();
    key2 = (WmAnimKey *)((WmBone *)key2 + 1);
    gte_stclmv((s16 *)frame);
    gte_ldlv0(vec);
    gte_rtv0tr();
    parentMtx = &bones[((WmBone *)key2)->parent];
    gte_stlvnl(boneMtx->t);
    if (head == idx) {
        up.vx = 0;
        up.vy = 0x1000;
        up.vz = 0;
        lookDir.vx = boneMtx->t[0] - mdl->lookAtTarget[0];
        lookDir.vy = boneMtx->t[1] - mdl->lookAtTarget[1];
        lookDir.vz = boneMtx->t[2] - mdl->lookAtTarget[2];
        VectorNormal(&lookDir, up_p);
        gte_ldopv1(&up);
        gte_ldopv2(up_p);
        gte_op12();
        gte_stlvnl(rot);
        VectorNormal((VECTOR *)rot, &r0);
        gte_ldopv1(&r0);
        gte_ldopv2(up_p);
        gte_op12();
        gte_stlvnl(rot);
        VectorNormal((VECTOR *)rot, &r1);
        boneMtx->m[0][0] = r0.vx;
        boneMtx->m[0][1] = r2.vx;
        boneMtx->m[0][2] = r1.vx;
        boneMtx->m[1][0] = r0.vy;
        boneMtx->m[1][1] = r2.vy;
        boneMtx->m[1][2] = r1.vy;
        boneMtx->m[2][0] = r0.vz;
        boneMtx->m[2][1] = r2.vz;
        boneMtx->m[2][2] = r1.vz;
        if (mdl->flags & MDL_SCALED) {
            *(s32 *)&scaleMtx.m[0][0] = ONE;
            *(s32 *)&scaleMtx.m[0][2] = 0;
            *(s32 *)&scaleMtx.m[1][1] = ONE;
            *(s32 *)&scaleMtx.m[2][0] = 0;
            *(s32 *)&scaleMtx.m[2][2] = ONE;
            scaleMtx.m[0][0] = mdl->scaleX;
            gte_SetRotMatrix(boneMtx);
            gte_ldclmv(&scaleMtx);
            gte_rtir();
            scaleMtx.m[1][1] = mdl->scaleY;
            gte_stclmv(boneMtx);
            gte_ldclmv(&scaleMtx.m[0][1]);
            gte_rtir();
            scaleMtx.m[2][2] = mdl->scaleZ;
            gte_stclmv(key);
            gte_ldclmv(&scaleMtx.m[0][2]);
            gte_rtir();
            gte_stclmv((s16 *)frame);
        }
    }
    boneMtx++;
    idx++;
    if (boneMtx < end) {
        goto loop;
    }
    if (nb < 31) {
        boneMtx = (MATRIX *)mdl->boneMatrices;
        do {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)bones)[0];
            x1 = ((s32 *)bones)[1];
            x2 = ((s32 *)bones)[2];
            x3 = ((s32 *)bones)[3];
            ((s32 *)boneMtx)[0] = x0;
            ((s32 *)boneMtx)[1] = x1;
            ((s32 *)boneMtx)[2] = x2;
            ((s32 *)boneMtx)[3] = x3;
            x0 = ((s32 *)bones)[4];
            x1 = ((s32 *)bones)[5];
            x2 = ((s32 *)bones)[6];
            x3 = ((s32 *)bones)[7];
            ((s32 *)boneMtx)[4] = x0;
            ((s32 *)boneMtx)[5] = x1;
            ((s32 *)boneMtx)[6] = x2;
            ((s32 *)boneMtx)[7] = x3;
            boneMtx++;
            bones++;
        } while (bones < end);
    }
}

void fldmdl_updateTexAnims(TexAnimSet *set) {
    RECT rect;
    u32 i;
    u32 j;
    TexAnim *anim;
    TexAnimRect *ar;
    s32 pos;
    s16 frame;
    s32 r8;
    s32 randDelay;
    s16 last;

    if (set == NULL) {
        return;
    }
    for (i = 0; i < set->count; i++) {
        anim = &set->anims[i];
        if (!(anim->flags & TEXANIM_ACTIVE)) {
            continue;
        }
        if (anim->frameCount != 0) {
            pos = anim->pos;
            frame = pos >> 12;
            last = anim->lastFrame;
            if (frame >= 0) {
                if (frame != last) {
                    for (j = 0; j < anim->rectCount; j++) {
                        ar = &anim->rects[j];
                        rect.x = ar->frames[frame].vx;
                        rect.y = ar->frames[frame].vy;
                        rect.w = ar->dest.w;
                        rect.h = ar->dest.h;
                        MoveImage(&rect, ar->dest.x, ar->dest.y);
                    }
                    anim->lastFrame = frame;
                }
                pos += anim->speed;
            } else {
                pos += 0x1000;
            }
            if ((pos >> 12) < anim->frameCount) {
                anim->pos = pos;
            } else if (anim->delayRange != 0) {
                /* rand16_inline() written out: calling it changes the register allocation here */
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                }
                r8 = *g_randPtr++;
                randDelay = (anim->delayRange + 1) * ((r8 << 8) | r8);
                anim->pos = -((anim->delayBase + randDelay / 0x10000) << 12);
            } else if (anim->flags & TEXANIM_ONESHOT) {
                anim->flags &= ~(TEXANIM_ACTIVE | TEXANIM_ONESHOT);
            } else {
                anim->pos = 0;
            }
        } else if (anim->flags & TEXANIM_SCROLL) {
            pos = anim->pos;
            frame = pos >> 12;
            for (j = 0; j < anim->rectCount; j++) {
                ar = &anim->rects[j];
                MoveImage(&ar->dest, ar->dest.x, ar->dest.y - frame);
                rect.x = ar->dest.x;
                rect.y = ar->dest.y - frame;
                rect.w = ar->dest.w;
                rect.h = frame;
                MoveImage(&rect, ar->dest.x, ar->dest.y + ar->dest.h - frame);
            }
            anim->pos = (pos & 0xFFF) + anim->speed;
        }
    }
}

void fldmdl_initTexAnims(TexAnimRelocSet *hdr, ModelTexOfs *ofs) {
    u32 i, j, k;
    u32 addr;
    TexAnimRelocRect *part;
    u8 idx;

    if (hdr == NULL) {
        return;
    }
    for (i = 0; i < hdr->count; i++) {
        hdr->anims[i].lastFrame = 1;
        addr = hdr->anims[i].rects;
        if (PSX_IS_RELOCATED_ABOVE(addr, 0x80000000)) {
            return;
        }
        if (addr == 0) {
            continue;
        }
        idx = hdr->anims[i].texOfsIndex;
        addr += (u32)hdr;
        hdr->anims[i].rects = addr;
        if (ofs != NULL) {
            ((TexAnimRelocRect *)addr)->destX += ofs[idx].destX;
            ((TexAnimRelocRect *)addr)->destY += ofs[idx].destY;
        }
        part = (TexAnimRelocRect *)addr;
        for (j = 0; j < hdr->anims[i].rectCount; j++) {
            addr = part[j].frames;
            addr += (u32)hdr;
            part[j].frames = addr;
            if (ofs != NULL) {
                for (k = 0; k < hdr->anims[i].frameCount; k++) {
                    ((TexAnimRelocPos *)addr)->x += ofs[idx].srcX;
                    ((TexAnimRelocPos *)addr)->y += ofs[idx].srcY;
                    addr += sizeof(TexAnimRelocPos);
                }
            }
        }
    }
}

void fldmdl_projectMeshClipY(ModelObj *mdl, u8 groupIdx) {
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
    ymax = mdl->clipY;
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

void fldmdl_drawMeshClipY(ModelObj *mdl, u8 groupIdx) {
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
        otz >>= 4;
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
        addPrim(&ot[otz >> 2], pFT3);
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
        otz >>= 4;
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
        addPrim(&ot[otz >> 2], pGT3);
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
        otz >>= 4;
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
        addPrim(&ot[otz >> 2], pG3);
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
        otz >>= 4;
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
        addPrim(&ot[otz >> 2], pF3);
    }
}

/* reflect the mirrored model across the plane (unk64 normal, unk6C point) */
void fldmdl_reflectModel(ModelObj *mdl) {
    MATRIX *reflMtx;
    s32 nx;
    s32 ny;
    s32 nz;
    s32 planeDist;

    if (mdl->mirrorSrc != NULL) {
        nx = mdl->mirrorNormal.v.vx;
        ny = mdl->mirrorNormal.v.vy;
        nz = mdl->mirrorNormal.v.vz;
        planeDist = (mdl->mirrorPoint.v.vx * nx + mdl->mirrorPoint.v.vy * ny + mdl->mirrorPoint.v.vz * nz) >> 11;
        reflMtx = PSX_SPAD(MATRIX *, 0);
        reflMtx->m[0][0] = ONE - ((nx * nx) >> 11);
        reflMtx->m[1][1] = ONE - ((ny * ny) >> 11);
        reflMtx->m[2][2] = ONE - ((nz * nz) >> 11);
        reflMtx->m[0][1] = reflMtx->m[1][0] = (nx * ny) >> 11;
        reflMtx->m[0][2] = reflMtx->m[2][0] = (nx * nz) >> 11;
        reflMtx->m[1][2] = reflMtx->m[2][1] = (ny * nz) >> 11;
        reflMtx->t[0] = (planeDist * nx) >> 12;
        reflMtx->t[1] = (planeDist * ny) >> 12;
        reflMtx->t[2] = (planeDist * nz) >> 12;
        gte_SetRotMatrix(reflMtx);
        gte_ldclmv(&mdl->mirrorSrc->worldMatrix);
        gte_rtir();
        gte_stclmv(&mdl->worldMatrix);
        gte_ldclmv(&mdl->mirrorSrc->worldMatrix.m[0][1]);
        gte_rtir();
        gte_stclmv(&mdl->worldMatrix.m[0][1]);
        gte_ldclmv(&mdl->mirrorSrc->worldMatrix.m[0][2]);
        gte_rtir();
        gte_stclmv(&mdl->worldMatrix.m[0][2]);
        gte_SetTransMatrix(reflMtx);
        gte_ldlv0(mdl->mirrorSrc->worldMatrix.t);
        gte_rtv0tr();
        gte_stlvnl(mdl->worldMatrix.t);
        mdl->anim = mdl->mirrorSrc->anim;
    }
}

void fldmdl_drawMeshMirrored(ModelObj *mdl, u8 groupIdx) {
    WmModelGroup *grp;
    u32 *ot;
    s32 opz;
    s32 otz;
    u32 primCount;
    u32 i;
    s32 xy0, xy1, xy2;
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
        i0 = *(u32 *)&sFT4->vertices[0];
        i2 = *(u32 *)&sFT4->vertices[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pFT4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        otz = (g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3]) >> 4;
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
        i0 = *(u32 *)&sFT3->vertices[0];
        i2 = sFT3->vertices[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pFT3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[(u16)i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pFT3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pFT3);
    }

    primCount = grp->gt4Count;
    sGT4 = (WmSrcGT4 *)sFT3;
    pGT4 = (POLY_GT4 *)pFT3;
    for (i = 0; i < primCount; sGT4++, pGT4++) {
        i0 = *(u32 *)&sGT4->vertices[0];
        i2 = *(u32 *)&sGT4->vertices[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pGT4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        otz = g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3];
        *(s32 *)&pGT4->x0 = xy0;
        *(s32 *)&pGT4->x1 = xy1;
        *(s32 *)&pGT4->x2 = xy2;
        *(s32 *)&pGT4->x3 = g_vtxScreenXY[i3];
        otz >>= 4;
        addPrim(&ot[otz], pGT4);
    }

    primCount = grp->gt3Count;
    sGT3 = (WmSrcGT3 *)sGT4;
    pGT3 = (POLY_GT3 *)pGT4;
    for (i = 0; i < primCount; sGT3++, pGT3++) {
        i0 = *(u32 *)&sGT3->vertices[0];
        i2 = sGT3->vertices[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pGT3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(s32 *)&pGT3->x0 = xy0;
        *(s32 *)&pGT3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pGT3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pGT3);
    }

    primCount = grp->g4Count;
    sG4 = (WmSrcG4 *)sGT3;
    pG4 = (POLY_G4 *)pGT3;
    for (i = 0; i < primCount; sG4++, pG4++) {
        i0 = *(u32 *)&sG4->vertices[0];
        i2 = *(u32 *)&sG4->vertices[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pG4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        otz = (g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3]) >> 4;
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
        i0 = *(u32 *)&sG3->vertices[0];
        i2 = sG3->vertices[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pG3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pG3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pG3);
    }

    primCount = grp->f4Count;
    sF4 = (WmSrcF4 *)sG3;
    pF4 = (POLY_F4 *)pG3;
    for (i = 0; i < primCount; sF4++, pF4++) {
        i0 = *(u32 *)&sF4->vertices[0];
        i2 = *(u32 *)&sF4->vertices[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF4->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pF4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        otz = g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3];
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        *(s32 *)&pF4->x3 = g_vtxScreenXY[i3];
        otz >>= 4;
        addPrim(&ot[otz], pF4);
    }

    primCount = grp->f3Count;
    sF3 = (WmSrcF3 *)sF4;
    pF3 = (POLY_F3 *)pF4;
    for (i = 0; i < primCount; sF3++, pF3++) {
        i0 = *(u32 *)&sF3->vertices[0];
        i2 = sF3->vertices[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pF3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pF3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pF3);
    }
}

s32 fldbg_buildOverlaySprites(BgOverlay *ovl, BgSprt *sprites, BgSprt **spritesEnd) {
    BgPos *pos;
    BgTile *tile;
    u32 i;
    u32 bufIdx;
    s32 nTiles;
    s32 w;
    s32 x;
    u16 tpageId;
    BgFile *bg;
    s32 clutRows;
    s32 viewportMask;
    s32 posOfs;
    s32 tileOfs;

    /* Statement order and the barrier below are for the scheduler: everything above the barrier
     * forms its own group, where both global loads are ready at once and are scheduled as
     * lui/lui/lw/lbu, so %hi(g_fieldBgClutRows) gets v1. The viewport mask is set up here (and applied by
     * hand below) because its lui/ori must also sit above the barrier, and bufIdx/the two offsets
     * are read early so their insns land where the original has them. */
    viewportMask = 0xFFFF01FF;
    bg = g_fieldBgData;
    clutRows = g_fieldBgClutRows;
    posOfs = ovl->posOffset;
    tileOfs = ovl->tileOffset;
    bufIdx = 0;
    ovl->sprites = sprites;
    do { } while (0);
    pos = (BgPos *)((u8 *)bg + posOfs);
    tile = (BgTile *)((u8 *)bg + tileOfs);
    w = clutRows + 0xDF;
    nTiles = ovl->tileCount;
    ovl->screenX = ovl->x + bg->screenX;
    ovl->screenY = ovl->y + bg->screenY;
    ovl->skewVertical = 0;
    ((BgOverlayFlags *)ovl)->cameraWord &= viewportMask; /* ovl->viewport = 0 */
    for (; bufIdx < 2; bufIdx++) {
        for (i = 0; i < nTiles; i++) {
            BgSprt *sp = &sprites[i];
            setSprt16(&sp->sprt);
            sp->sprt.r0 = 0x80;
            sp->sprt.g0 = 0x80;
            sp->sprt.b0 = 0x80;
            sp->sprt.x0 = ovl->screenX + pos[i].x;
            sp->sprt.y0 = ovl->screenY + pos[i].y;
            sp->sprt.u0 = tile[i].u;
            sp->sprt.v0 = tile[i].v;
            if (tile[i].trans) {
                setSemiTrans(&sp->sprt, 1);
            } else {
                setSemiTrans(&sp->sprt, 0);
            }
            sp->sprt.clut = GetClut(tile[i].clutX << 4, tile[i].clutY);
            if (tile[i].tp == 0) {
                tpageId = GetTPage(0, tile[i].abr, tile[i].tpX << 6, tile[i].tpY << 8);
            } else {
                tpageId = GetTPage(1, tile[i].abr, tile[i].tpX << 6, tile[i].tpY << 8);
            }
            setDrawTPage(&sp->tpage, 1, 1, tpageId);
            MargePrim(&sp->tpage, &sp->sprt);
            x = tile[i].clutY;
            if (w < x) {
                w = x;
            }
        }
        sprites += nTiles;
    }
    *spritesEnd = sprites;
    g_fieldBgClutRows = w + 0x21;
    return 1;
}

s32 fldbg_initEffectList(void) {
    u32 ovlCount;
    u32 i;
    BgOverlayAnim *tbl;
    BgOverlay *trk;

    tbl = (BgOverlayAnim *)((u8 *)g_fieldBgCtx.data + g_fieldBgCtx.data->overlayAnimOffset);
    trk = (BgOverlay *)((u8 *)g_fieldBgCtx.data + g_fieldBgCtx.data->overlayOffset);
    ovlCount = g_fieldBgCtx.data->overlayCount;
    i = 0;
    g_fieldBgCtx.entryCount = 0;
    g_fieldBgCtx.entries = (BgEntry *)g_fieldBgCtx.work;
    for (; i < ovlCount; i++) {
        if (trk[i].flags & FLDBG_OVL_ANIMATED) {
            g_fieldBgCtx.entries[g_fieldBgCtx.entryCount].type = BGENT_OVERLAY_ANIM;
            g_fieldBgCtx.entries[g_fieldBgCtx.entryCount].overlay = i;
            g_fieldBgCtx.entries[g_fieldBgCtx.entryCount].dataOffset = 0;
            g_fieldBgCtx.entries[g_fieldBgCtx.entryCount].dataSize = 0;
            g_fieldBgCtx.workUsed += 0xC;
            g_fieldBgCtx.entryCount++;
            if (trk[i].flags & FLDBG_OVL_ANIM_GROUP) {
                i += tbl->stepCount;
                tbl++;
            }
        }
    }
    return 1;
}

s32 fldbg_updateOverlayAnims(void) {
    BgFile *bg = g_fieldBgData;
    u32 i;
    BgOverlayAnim *anim;
    BgOverlayAnim *paths;
    u32 cnt;
    BgOverlay *objs;
    BgAnimStep *st;
    u32 animCount;
    s32 pos;
    u32 j;
    s32 stepIdx;

    i = 0;
    paths = (BgOverlayAnim *)((u8 *)bg + bg->overlayAnimOffset);
    objs = (BgOverlay *)((u8 *)bg + bg->overlayOffset);
    animCount = bg->overlayAnimCount;
    for (; i < animCount; i++) {
        anim = &paths[i];
        pos = anim->pos;
        if ((anim->flags & FLDBG_ANIM_ACTIVE) && (anim->flags & (FLDBG_ANIM_LOOP | FLDBG_ANIM_PLAY_ONCE)) && anim->cameraId == g_fieldBgState.cameraId) {
            st = (BgAnimStep *)((u8 *)bg + anim->stepOffset);
            cnt = anim->stepCount;
            for (j = 0; j < cnt; j++) {
                objs[st[j].overlay].flags &= ~FLDBG_OVL_VISIBLE;
            }
            objs[st[pos >> 8].overlay].flags |= FLDBG_OVL_VISIBLE;
            if (st[pos >> 8].delay < 0) {
                st[pos >> 8].delay = 0;
                anim->delayCounter = 0;
                anim->flags &= ~FLDBG_ANIM_PLAY_ONCE;
                return 1;
            }
            anim->delayCounter++;
            if ((u16)anim->delayCounter < st[pos >> 8].delay) {
                continue;
            }
            anim->delayCounter = 0;
            anim->pos = pos + anim->speed;
            stepIdx = anim->pos >> 8;
            if (stepIdx >= anim->stepCount) {
                if (anim->flags & FLDBG_ANIM_PINGPONG) {
                    anim->pos = (anim->stepCount - 1) << 8;
                    fldbg_setAnimSpeed(i, -anim->speed);
                } else {
                    anim->pos = 0;
                }
            } else if (stepIdx < 0) {
                if (anim->flags & FLDBG_ANIM_PINGPONG) {
                    anim->pos = 0;
                    fldbg_setAnimSpeed(i, -anim->speed);
                } else {
                    anim->pos = (anim->stepCount - 1) << 8;
                }
            } else {
                continue;
            }
            anim->flags &= ~FLDBG_ANIM_PLAY_ONCE;
        }
    }
    return 1;
}

/* initialise the screen fade quads */
s32 fldbg_initFollowOverlays(void) {
    u16 i;
    u16 j;

    g_fieldBgFollowOverlayCount = 0;
    for (i = 0; i < 10; i++) {
        g_fieldBgFollowOverlays[i].abr = -1;
        g_fieldBgFollowOverlays[i].r = 0x80;
        g_fieldBgFollowOverlays[i].g = 0x80;
        g_fieldBgFollowOverlays[i].b = 0x80;
        g_fieldBgFollowOverlays[i].overlay = -1;
        g_fieldBgFollowOverlays[i].offsetX = 0;
        g_fieldBgFollowOverlays[i].offsetY = 0;
        for (j = 0; j < 4; j++) {
            SetPolyF4(&g_fieldBgFollowOverlays[i].quads[0][j]);
            setRGB0(&g_fieldBgFollowOverlays[i].quads[0][j], g_fieldBgFollowOverlays[i].r, g_fieldBgFollowOverlays[i].g, g_fieldBgFollowOverlays[i].b);
            SetPolyF4(&g_fieldBgFollowOverlays[i].quads[1][j]);
            setRGB0(&g_fieldBgFollowOverlays[i].quads[1][j], g_fieldBgFollowOverlays[i].r, g_fieldBgFollowOverlays[i].g, g_fieldBgFollowOverlays[i].b);
        }
    }
    return 1;
}

s32 fldbg_drawFollowMask(BgFile *bg, BgOverlay *ovl, FieldBgState *st, BgFollowOverlay *follow) {
    u32 *ot;
    POLY_F4 *quad;
    DR_TPAGE *tp;
    s16 x;
    s16 y;
    s16 edgeA;
    s16 edgeB;
    s16 w;

    ot = g_fieldBgCtx.ot[*g_fieldBgCtx.bufIndex];
    x = ovl->x + bg->posX - (st->scroll.x - FLD_SCREEN_CX);
    y = ovl->y + bg->posY - (st->scroll.y - FLD_SCREEN_CY);
    if (x > 0) {
        quad = &follow->quads[*g_fieldBgCtx.bufIndex][0];
        edgeA = y;
        if (edgeA < 0) {
            edgeA = 0;
        }
        setXY4(quad, 0, edgeA, x, edgeA, 0, SCREEN_H, x, SCREEN_H);
        addPrim(&ot[7], quad);
    }
    if (y > 0) {
        edgeA = x + ovl->width;
        quad = &follow->quads[*g_fieldBgCtx.bufIndex][1];
        if (edgeA > SCREEN_W) {
            edgeA = SCREEN_W;
        }
        edgeB = y;
        if (edgeB < 0) {
            edgeB = 0;
        }
        setXY4(quad, 0, 0, edgeA, 0, 0, edgeB, edgeA, edgeB);
        addPrim(&ot[7], quad);
    }
    if (x + ovl->width < SCREEN_W) {
        edgeA = x + ovl->width;
        edgeB = y + ovl->height;
        quad = &follow->quads[*g_fieldBgCtx.bufIndex][2];
        w = SCREEN_W - edgeA;
        if (edgeB > SCREEN_H) {
            edgeB = SCREEN_H;
        }
        setXYWH(quad, edgeA, 0, w, edgeB);
        addPrim(&ot[7], quad);
    }
    if (y + ovl->height < SCREEN_H) {
        quad = &follow->quads[*g_fieldBgCtx.bufIndex][3];
        edgeB = x;
        if (edgeB < 0) {
            edgeB = 0;
        }
        edgeA = y + ovl->height;
        if (edgeA < 0) {
            edgeA = 0;
        }
        setXY4(quad, edgeB, edgeA, SCREEN_W, edgeA, edgeB, SCREEN_H, SCREEN_W, SCREEN_H);
        addPrim(&ot[7], quad);
    }
    if (follow->abr < 4) {
        tp = &follow->tpages[*g_fieldBgCtx.bufIndex];
        SetDrawTPage(tp, 1, 1, GetTPage(2, follow->abr, 0, 0));
        addPrim(&ot[7], tp);
    }
    return 1;
}

s32 fldbg_updateFollowOverlays(void) {
    SVECTOR anchor;
    s32 sxy;
    FieldBgState *st;
    BgFile *bg;
    BgOverlay *objs;
    BgOverlay *ovl;
    EvtObject *player;
    BgFollowOverlay *follow;
    u8 i;
    u8 mode;
    u16 sx;
    u16 sy;
    s16 y;
    s16 x;

    if (g_fieldBgFollowOverlayCount == 0) {
        return 0;
    } else {
        st = &g_fieldBgState;
        bg = g_fieldBgData;
        objs = (BgOverlay *)((u8 *)bg + bg->overlayOffset);
        mode = g_gameState->mode;
        if (mode == 5) {
            player = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
        } else if (mode == GS_MODE_FIELD) {
            player = evt_getPlayer();
        } else {
            player = NULL;
        }
        if (player != NULL) {
            anchor.vx = player->pos.vx;
            anchor.vy = player->pos.vy - g_fieldBgState.followHeight;
            anchor.vz = player->pos.vz;
        } else {
            anchor.vx = 0;
            anchor.vy = 0;
            anchor.vz = 0;
        }
        fldbg_applyGeomOffset();
        gte_SetRotMatrix(g_fieldBgState.viewMatrix);
        gte_SetTransMatrix(g_fieldBgState.viewMatrix);
        gte_SetGeomScreen(*g_fieldBgState.projDist);
        gte_ldv0(&anchor);
        gte_rtps();
        gte_stsxy(&sxy);
        sx = sxy;
        sy = sxy >> 16;
        follow = g_fieldBgFollowOverlays;
        for (i = 0; i < g_fieldBgFollowOverlayCount; i++, follow++) {
            ovl = (BgOverlay *)((u16)follow->overlay * sizeof(BgOverlay) + (u32)objs);
            if ((ovl->flags & FLDBG_OVL_VISIBLE) && ovl->cameraId == st->cameraId) {
                x = st->scroll.x + (sx - bg->posX - (follow->offsetX + FLD_SCREEN_CX));
                y = follow->offsetY;
                ovl->x = x;
                ovl->y = st->scroll.y + (sy - bg->posX - (y + FLD_SCREEN_CY));
                if (follow->abr >= 0) {
                    fldbg_drawFollowMask(bg, ovl, st, follow);
                }
            }
        }
        return 1;
    }
}

s32 fldbg_initOverlays(u8 *heap, BgSprt **spritesOut) {
    BgFile *h;
    BgCamera *cam;
    s32 x;
    s32 y;
    u32 ovlCount;
    u32 size;
    u32 i;
    BgOverlay *ovl;
    BgOverlay *objs;
    BgOverlay *ovlAlias;

    h = g_fieldBgCtx.data;
    fldbg_setCamera(g_fieldBgState.cameraId);
    cam = &((BgCamera *)((u8 *)h + h->cameraOffset))[g_fieldBgState.cameraId];
    x = (cam->scrollMinX + cam->scrollMaxX) / 2;
    g_fieldBgState.scroll.x = (g_fieldBgState.parallaxOriginX = x);
    g_fieldBgState.scroll.y = (g_fieldBgState.parallaxOriginY = (cam->scrollMinY + cam->scrollMaxY) / 2);
    x -= FLD_SCREEN_CX;
    h->screenX = h->posX - x;
    ovlCount = h->overlayCount;
    objs = (BgOverlay *)((u8 *)h + h->overlayOffset);
    size = ovlCount * 3;
    y = g_fieldBgState.scroll.y - FLD_SCREEN_CY;
    h->screenY = h->posY - (s16)y;
    g_fieldBgCtx.overlayRgb = heap;
    while (size / 4 * 4 != size) {
        size++;
    }
    g_fieldBgCtx.overlayTinted = heap + size;
    size += ovlCount;
    while (size / 4 * 4 != size) {
        size++;
    }
    *spritesOut = (BgSprt *)(heap + size);
    for (i = 0; i < 4; i++) {
        g_fieldBgState.viewportPos[i][0] = 0;
        g_fieldBgState.viewportPos[i][1] = 0;
        g_fieldBgState.viewportSize[i][0] = cam->width;
        g_fieldBgState.viewportSize[i][1] = cam->height;
        g_fieldBgState.viewportRateX[i] = 0x100;
        g_fieldBgState.viewportRateY[i] = 0x100;
    }
    for (i = 0; i < ovlCount; i++) {
        ovl = &objs[i];
        g_fieldBgCtx.overlayRgb[i * 3 + 0] = 0x80;
        g_fieldBgCtx.overlayRgb[i * 3 + 1] = 0x80;
        g_fieldBgCtx.overlayRgb[i * 3 + 2] = 0x80;
        g_fieldBgCtx.overlayTinted[i] = 0;
        ovlAlias = ovl;
        if (fldbg_buildOverlaySprites(ovlAlias, *spritesOut, spritesOut) != 1) {
            return 0;
        }
        ovl->minX = -0x8000;
        ovlAlias->maxX = 0x7FFF;
        ovl->minY = -0x8000;
        ovlAlias->maxY = 0x7FFF;
    }
    g_fieldBgState.unk73 = 0;
    g_fieldBgState.unk72 = 0;
    g_fieldBgState.flags &= ~FLDBG_OVERLAY_STATE;
    return 1;
}

s32 fldbg_drawOverlay(BgOverlay *ovl, u8 *unused) {
    BgSprt *spr;
    u32 *ot;
    BgFile *h;
    BgPos *pos;
    u32 nTiles;
    s16 i;
    s16 x;
    s16 y;
    s16 z;
    s32 sx;
    s32 sy;
    u16 ox;
    u16 oy;
    u16 wx;
    u16 wy;
    u8 bufIdx;

    spr = ovl->sprites;
    bufIdx = *g_fieldBgCtx.bufIndex;
    h = g_fieldBgCtx.data;
    nTiles = ovl->tileCount;
    ot = g_fieldBgCtx.ot[bufIdx];
    if (bufIdx != 0) {
        spr += nTiles;
    }
    x = ovl->x + h->screenX;
    y = ovl->y + h->screenY;
    z = h->baseDepth + ovl->depth;
    pos = (BgPos *)((u8 *)h + ovl->posOffset);
    if (ovl->flags & FLDBG_OVL_LOOP) {
        s16 tx;
        s16 ty;
        s16 otz;
        s16 k;

        if (ovl->flags & FLDBG_OVL_SCREEN) {
            ox = g_fieldBgState.viewportPos[ovl->viewport][0];
            oy = g_fieldBgState.viewportPos[ovl->viewport][1];
        } else {
            ox = g_fieldBgState.viewportPos[ovl->viewport][0] + FLD_SCREEN_CX - g_fieldBgState.scroll.x;
            oy = g_fieldBgState.viewportPos[ovl->viewport][1] + FLD_SCREEN_CY - g_fieldBgState.scroll.y;
        }
        wx = g_fieldBgState.viewportSize[ovl->viewport][0];
        wy = g_fieldBgState.viewportSize[ovl->viewport][1];
        sx = ovl->scrollX;
        if (sx < 0) {
            k = 0x100 - sx;
            x = h->screenX + ((((ovl->x << 8) | ovl->fracX) + k) >> 8);
        }
        sy = ovl->scrollY;
        if (sy < 0) {
            k = 0x100 - sx;
            y = h->screenY + ((((ovl->y << 8) | ovl->fracY) + k) >> 8);
        }
        if (sx != 0) {
            x = (x - ((s16)wx - ovl->width)) % ovl->width + (wx - ovl->width);
        }
        if (sy != 0) {
            y = (y - ((s16)wy - ovl->height)) % ovl->height + (wy - ovl->height);
        }
        for (i = 0; i < nTiles; i++) {
            if (ovl->flags & FLDBG_OVL_SCREEN) {
                tx = x + pos[i].x;
                if (ovl->scrollX != 0) {
                    if (tx + 16 >= ovl->width) {
                        tx -= ovl->width;
                    } else if (tx < -15) {
                        tx += ovl->width;
                    }
                }
                if (tx + (s16)ox < -15 || (s16)wx < tx) {
                    continue;
                }
                ty = y + pos[i].y;
                if (ovl->scrollY != 0) {
                    if (ty + 16 >= ovl->height) {
                        ty -= ovl->height;
                    } else if (ty < -15) {
                        ty += ovl->height;
                    }
                }
                if (ty + (s16)oy < -15 || ty >= (s16)wy) {
                    continue;
                }
                otz = z + pos[i].pad;
                if (otz < 8 || otz >= 0xFF9) {
                    continue;
                }
                spr[i].sprt.x0 = tx + ox;
                spr[i].sprt.y0 = ty + oy;
            } else {
                tx = x + pos[i].x;
                if (ovl->scrollX != 0) {
                    if (tx + 16 >= ovl->width) {
                        tx -= ovl->width;
                    } else if (tx < -15) {
                        tx += ovl->width;
                    }
                    if (tx + (s16)ox < -15 || tx >= (s16)wx) {
                        continue;
                    }
                    spr[i].sprt.x0 = tx + ox;
                } else {
                    if (tx < -15 || tx >= (s16)ox + (s16)wx) {
                        continue;
                    }
                    spr[i].sprt.x0 = tx;
                }
                ty = y + pos[i].y;
                if (ovl->scrollY != 0) {
                    if (ty + 16 >= ovl->height) {
                        ty -= ovl->height;
                    } else if (ty < -15) {
                        ty += ovl->height;
                    }
                    if (ty + (s16)oy < -15 || ty >= (s16)wy) {
                        continue;
                    }
                    spr[i].sprt.y0 = ty + oy;
                } else {
                    if (ty < -15 || ty >= (s16)oy + (s16)wy) {
                        continue;
                    }
                    spr[i].sprt.y0 = ty;
                }
                otz = z + pos[i].pad;
                if (otz < 8 || otz >= 0xFF9) {
                    continue;
                }
            }
            addPrim(ot + otz, &spr[i]);
        }
    } else if (ovl->flags & FLDBG_OVL_SKEW_LOOP) {
        s16 tx;
        s16 ty;
        s16 otz;
        s16 k;

        if (ovl->flags & FLDBG_OVL_SCREEN) {
            ox = g_fieldBgState.viewportPos[ovl->viewport][0];
            oy = g_fieldBgState.viewportPos[ovl->viewport][1];
        } else {
            ox = g_fieldBgState.viewportPos[ovl->viewport][0] + FLD_SCREEN_CX - g_fieldBgState.scroll.x;
            oy = g_fieldBgState.viewportPos[ovl->viewport][1] + FLD_SCREEN_CY - g_fieldBgState.scroll.y;
        }
        wx = g_fieldBgState.viewportSize[ovl->viewport][0];
        wy = g_fieldBgState.viewportSize[ovl->viewport][1];
        if (ovl->skewVertical) {
            y = (y - ((s16)wy - ovl->height)) % ovl->height + (wy - ovl->height);
            x += y * ovl->scrollX / ovl->height % ovl->width;
        } else {
            x = (x - ((s16)wx - ovl->width)) % ovl->width + (wx - ovl->width);
            y += x * ovl->scrollY / ovl->width % ovl->height;
        }
        for (i = 0; i < nTiles; i++) {
            if (ovl->skewVertical) {
                k = 0;
                ty = y + pos[i].y;
                if (ty + 16 >= ovl->height) {
                    ty -= ovl->height;
                    k = -ovl->scrollX;
                } else if (ty < -15) {
                    ty += ovl->height;
                    k = ovl->scrollX;
                }
                if (ty + (s16)oy < -15 || ty >= (s16)wy) {
                    continue;
                }
                tx = k + (x + pos[i].x);
                if (tx < -15 || tx >= (s16)ox + (s16)wx) {
                    continue;
                }
                spr[i].sprt.x0 = tx;
                spr[i].sprt.y0 = ty + oy;
            } else {
                k = 0;
                tx = x + pos[i].x;
                if (tx + 16 >= ovl->width) {
                    tx -= ovl->width;
                    k = -ovl->scrollY;
                } else if (tx < -15) {
                    tx += ovl->width;
                    k = ovl->scrollY;
                }
                if (tx + (s16)ox < -15 || tx >= (s16)wx) {
                    continue;
                }
                ty = k + (y + pos[i].y);
                if (ty < -15 || ty >= (s16)oy + (s16)wy) {
                    continue;
                }
                spr[i].sprt.x0 = tx + ox;
                spr[i].sprt.y0 = ty;
            }
            otz = z + pos[i].pad;
            if (otz < 8 || otz >= 0xFF9) {
                continue;
            }
            addPrim(ot + otz, &spr[i]);
        }
    } else {
        s16 tx;
        s16 ty;
        s16 otz;

        for (i = 0; i < nTiles; i++) {
            tx = x + pos[i].x;
            if (tx < -15 || tx >= SCREEN_W) {
                continue;
            }
            ty = y + pos[i].y;
            if (ty < -15 || ty >= SCREEN_H) {
                continue;
            }
            otz = z + pos[i].pad;
            if (otz < 8 || otz >= 0xFF9) {
                continue;
            }
            spr[i].sprt.y0 = ty;
            spr[i].sprt.x0 = tx;
            addPrim(ot + otz, &spr[i]);
        }
    }
    ovl->screenX = x;
    ovl->screenY = y;
    return 1;
}

s32 fldbg_updateOverlayScroll(BgFile *bg) {
    s32 i;
    s32 ovlCount;
    BgOverlay *ovl;
    BgOverlay *objs;
    s32 fixPos;

    ovlCount = bg->overlayCount;
    objs = (BgOverlay *)((u8 *)bg + bg->overlayOffset);
    for (i = 0; i < ovlCount; i++) {
        ovl = &objs[i];
        if (ovl->flags & FLDBG_OVL_LOOP) {
            if (ovl->scrollX != 0 && ovl->scrollX != FLDBG_OVL_NO_SCROLL) {
                fixPos = (ovl->x << 8) | (ovl->fracX & 0xFF);
                fixPos = fixPos + ovl->scrollX;
                ovl->fracX = fixPos & 0xFF;
                ovl->x = (fixPos >> 8) % ovl->width;
            }
            if (ovl->scrollY != 0 && ovl->scrollY != FLDBG_OVL_NO_SCROLL) {
                fixPos = (ovl->y << 8) | (ovl->fracY & 0xFF);
                fixPos = fixPos + ovl->scrollY;
                ovl->fracY = fixPos & 0xFF;
                ovl->y = (fixPos >> 8) % ovl->height;
            }
        }
        if (ovl->flags & FLDBG_OVL_SKEW_LOOP) {
            if (ovl->skewVertical) {
                if (ovl->scrollY != FLDBG_OVL_NO_SCROLL) {
                    fixPos = (ovl->y << 8) | (ovl->fracY & 0xFF);
                    fixPos = fixPos + ovl->scrollY;
                    ovl->fracY = fixPos & 0xFF;
                    ovl->y = (fixPos >> 8) % ovl->height;
                }
            } else if (ovl->scrollX != FLDBG_OVL_NO_SCROLL) {
                fixPos = (ovl->x << 8) | (ovl->fracX & 0xFF);
                fixPos = fixPos + ovl->scrollX;
                ovl->fracX = fixPos & 0xFF;
                ovl->x = (fixPos >> 8) % ovl->width;
            }
        }
        if (ovl->flags & FLDBG_OVL_PARALLAX) {
            fixPos = (ovl->baseX << 8) + ((s16)g_fieldBgState.scroll.x - g_fieldBgState.parallaxOriginX) * ovl->scrollX;
            ovl->x = fixPos >> 8;
            ovl->fracX = fixPos & 0xFF;
            fixPos = (ovl->baseY << 8) + ((s16)g_fieldBgState.scroll.y - g_fieldBgState.parallaxOriginY) * ovl->scrollY;
            ovl->y = fixPos >> 8;
            ovl->fracY = fixPos & 0xFF;
        }
    }
    if (g_fieldBgState.flags & FLDBG_CAMERA_CHANGED) {
        g_fieldBgState.flags &= ~FLDBG_CAMERA_CHANGED;
        g_fieldBgState.cameraScroll.x = g_fieldBgState.scroll.x;
        g_fieldBgState.cameraScroll.y = g_fieldBgState.scroll.y;
    }
    return 1;
}

s32 fldbg_updateScroll(void) {
    s16 dur;
    u16 ox;
    u16 oy;
    BgOverlay *ovl;
    BgFile *h;
    s16 wx;
    s16 wy;
    s32 dx;
    s32 flags;
    s32 smooth;
    s32 ax;
    s32 ay;
    s32 i;
    BgCamera *cam;
    s16 cnt;
    s32 tmp; /* overlay scroll rate, then the current BgCamera address (separate locals don't match) */
    s32 val; /* scaled overlay position, then half the screen width (separate locals don't match) */
    s32 geomY;
    s32 y;
    s32 mode;
    flags = g_fieldBgState.flags;
    if (!(flags & FLDBG_FOLLOW)) {
        return 0;
    }
    mode = flags & FLDBG_SCROLL_MODE;
    smooth = flags & FLDBG_SCROLL_SMOOTH;
    if (mode == FLDBG_SCROLL_START || mode == FLDBG_SCROLL_RUN || mode == FLDBG_SCROLL_TO_PLAYER) {
        ox = g_fieldBgState.scroll.x;
        oy = g_fieldBgState.scroll.y;
        h = g_fieldBgData;
        wx = g_fieldBgState.scrollToX - g_fieldBgState.scrollFrom.x;
        wy = g_fieldBgState.scrollToY - g_fieldBgState.scrollFrom.y;
        cnt = g_fieldBgState.scrollFrame;
        dur = g_fieldBgState.scrollFrames;
        if (!smooth) {
            g_fieldBgState.scroll.x = g_fieldBgState.scrollFrom.x + wx * cnt / dur;
            g_fieldBgState.scroll.y = g_fieldBgState.scrollFrom.y + wy * cnt / dur;
        } else {
            y = math_cos(((cnt << 16) >> 5) / dur + 0x800) + 0x1000;
            ax = wx * y / 0x2000;
            ay = wy * y / 0x2000;
            g_fieldBgState.scroll.x = g_fieldBgState.scrollFrom.x + ax;
            g_fieldBgState.scroll.y = g_fieldBgState.scrollFrom.y + ay;
        }
        ox = g_fieldBgState.scroll.x - ox;
        oy = g_fieldBgState.scroll.y - oy;
        ovl = (BgOverlay *)((u8 *)h + h->overlayOffset);
        for (i = 0; i < h->overlayCount; ovl++, i++) {
            if (ovl->flags & FLDBG_OVL_LOOP) {
                if (ovl->scrollX != 0) {
                    ovl->x += ox;
                }
                if (ovl->scrollY != 0) {
                    ovl->y += oy;
                }
                dx = (s16)ox << 16;
            tmp = g_fieldBgState.viewportRateX[ovl->viewport] << 8;
            val = dx;
            if (tmp != 0x10000) {
                if (tmp < 0) {
                    val = (ovl->x << 16) - fld_fixMul(val, -tmp);
                } else {
                    val = (ovl->x << 16) + fld_fixMul(val, tmp);
                }
                ovl->x = val >> 16;
                ovl->fracX = (val >> 8) & 0xFF;
            }
            tmp = g_fieldBgState.viewportRateY[ovl->viewport] << 8;
            val = oy << 16;
            if (tmp != 0x10000) {
                if (tmp < 0) {
                    val = (ovl->y << 16) - fld_fixMul(val, -tmp);
                } else {
                    val = (ovl->y << 16) + fld_fixMul(val, tmp);
                }
                ovl->y = val >> 16;
                ovl->fracY = (val >> 8) & 0xFF;
            }
            } else if (ovl->flags & FLDBG_OVL_SKEW_LOOP) {
                if (ovl->skewVertical) {
                    ovl->y += oy;
                } else {
                    ovl->x += ox;
                }
            tmp = g_fieldBgState.viewportRateX[ovl->viewport] << 8;
            val = (s16)ox << 16;
            if (tmp != 0x10000) {
                if (tmp < 0) {
                    val = (ovl->x << 16) - fld_fixMul(val, -tmp);
                } else {
                    val = (ovl->x << 16) + fld_fixMul(val, tmp);
                }
                ovl->x = val >> 16;
                ovl->fracX = (val >> 8) & 0xFF;
            }
            tmp = g_fieldBgState.viewportRateY[ovl->viewport] << 8;
            val = oy << 16;
            if (tmp != 0x10000) {
                if (tmp < 0) {
                    val = (ovl->y << 16) - fld_fixMul(val, -tmp);
                    ovl->y = val >> 16;
                    ovl->fracY = (val >> 8) & 0xFF;
                } else {
                    val = (ovl->y << 16) + fld_fixMul(val, tmp);
                    ovl->y = val >> 16;
                    ovl->fracY = (val >> 8) & 0xFF;
                }
            }
            }
        }
        tmp = h->cameraOffset;
        tmp = (s32)h - -tmp; /* keeps the h + off operand order (plus h tmp) */
        tmp = tmp + g_fieldBgState.cameraId * sizeof(BgCamera);
        val = FLD_SCREEN_CX;
        y = *(s16 *)&g_fieldBgState.scroll.x - (((BgCamera *)tmp)->width >> 1);
        i = val - y;
        geomY = FLD_SCREEN_CY - (*(s16 *)&g_fieldBgState.scroll.y - (((BgCamera *)tmp)->height >> 1));
        g_fieldBgState.geomOffsetX = i;
        g_fieldBgState.geomOffsetY = geomY;
        if (mode == FLDBG_SCROLL_START) {
            g_fieldBgState.flags = (g_fieldBgState.flags & ~FLDBG_SCROLL_MODE) | FLDBG_SCROLL_RUN;
        }
        if (++g_fieldBgState.scrollFrame > dur) {
            if ((g_fieldBgState.flags & FLDBG_SCROLL_MODE) == FLDBG_SCROLL_RUN) {
                g_fieldBgState.flags = (g_fieldBgState.flags & ~FLDBG_SCROLL_MODE) | FLDBG_CAMERA_CHANGED | FLDBG_SCROLL_HELD;
            } else {
                g_fieldBgState.flags = (g_fieldBgState.flags & ~FLDBG_SCROLL_MODE) | FLDBG_CAMERA_CHANGED;
            }
        }
    }
    return 1;
}

s32 fldbg_followPlayer(void) {
    SVECTOR sv;
    long ofx, ofy;
    union {
        s32 w;
        DVECTOR v;
    } sxy;
    s16 dx, dy;
    s16 *px, *py;
    BgFile *h;
    BgCamera *cam;
    EvtObject *player;
    s32 x;
    s32 y;
    s32 cx;
    s32 cy;
    s32 sx;
    s32 sy;
    s32 lim;
    s32 wx;
    s32 ox;
    s32 oy;
    u8 mode;
    s32 flags;
    s32 i;
    BgOverlay *ovl;
    s32 rate;
    s32 fixPos;
    flags = g_fieldBgState.flags;
    if (flags & FLDBG_SCROLL_MODE) {
        return 0;
    }
    if (!(flags & FLDBG_FOLLOW)) {
        return 0;
    }
    mode = g_gameState->mode;
    if (mode == 5) {
        player = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
    } else if (mode == GS_MODE_FIELD) {
        player = evt_getPlayer();
    } else {
        player = NULL;
    }
    h = g_fieldBgData;
    cam = (BgCamera *)((u8 *)h + h->cameraOffset);
    cam += g_fieldBgState.cameraId;
    if (player != NULL) {
        sv.vx = player->pos.vx;
        sv.vy = player->pos.vy - g_fieldBgState.followHeight;
        sv.vz = player->pos.vz;
    } else {
        sv.vx = 0;
        sv.vy = 0;
        sv.vz = 0;
    }
    ReadGeomOffset(&ofx, &ofy);
    gte_SetGeomOffset(FLD_SCREEN_CX, FLD_SCREEN_CY);
    gte_SetRotMatrix(g_fieldBgState.viewMatrix);
    gte_SetTransMatrix(g_fieldBgState.viewMatrix);
    gte_SetGeomScreen(*g_fieldBgState.projDist);
    gte_ldv0(&sv);
    gte_rtps();
    gte_stsxy(&sxy);
    gte_SetGeomOffset(ofx, ofy);
    x = sxy.v.vx;
    y = sxy.w >> 16;
    if (g_fieldBgState.flags & FLDBG_CAMERA_CHANGED) {
        g_fieldBgState.playerScreenX = x;
        g_fieldBgState.playerScreenY = y;
    }
    x = (g_fieldBgState.playerScreenX + x) >> 1;
    y = (g_fieldBgState.playerScreenY + y) >> 1;
    sx = SCREEN_W - x;
    sy = SCREEN_H - y;
    g_fieldBgState.playerScreenX = x;
    g_fieldBgState.playerScreenY = y;
    wx = (cam->width >> 1) + (s16)cam->centerX;
    lim = wx - FLD_SCREEN_CX;
    cx = lim + x;
    {
        s32 ofsY = (cam->height >> 1) + (s16)cam->centerY - FLD_SCREEN_CY;
        cy = ofsY + y;
    }
    if (cx < cam->scrollMinX) {
        {
            s32 limX = cam->scrollMinX - FLD_SCREEN_CX;
            sx = wx - limX;
        }
    } else if (cam->scrollMaxX < cx) {
        {
            s32 limX = cam->scrollMaxX - FLD_SCREEN_CX;
            sx = wx - limX;
        }
    }
    if (cy < cam->scrollMinY) {
        {
            s32 camMidY = ((cam->height >> 1) + (s16)cam->centerY);
            s32 limY = cam->scrollMinY - FLD_SCREEN_CY;
            sy = camMidY - limY;
        }
    } else if (cam->scrollMaxY < cy) {
        {
            s32 camMidY = ((cam->height >> 1) + (s16)cam->centerY);
            s32 limY = cam->scrollMaxY - FLD_SCREEN_CY;
            sy = camMidY - limY;
        }
    }
    g_fieldBgState.geomOffsetX = sx - cam->centerX;
    g_fieldBgState.geomOffsetY = sy - cam->centerY;
    px = &dx;
    py = &dy;
    if (g_fieldBgState.flags & FLDBG_FOLLOW) {
        ox = g_fieldBgState.scroll.x;
        oy = g_fieldBgState.scroll.y;
        if ((s16)cx < cam->scrollMinX) {
            g_fieldBgState.scroll.x = cam->scrollMinX;
        } else if (cam->scrollMaxX < (s16)cx) {
            g_fieldBgState.scroll.x = cam->scrollMaxX;
        } else {
            g_fieldBgState.scroll.x = cx;
        }
        if ((s16)cy < cam->scrollMinY) {
            g_fieldBgState.scroll.y = cam->scrollMinY;
        } else if (cam->scrollMaxY < (s16)cy) {
            g_fieldBgState.scroll.y = cam->scrollMaxY;
        } else {
            g_fieldBgState.scroll.y = cy;
        }
        *px = g_fieldBgState.scroll.x - ox;
        *py = g_fieldBgState.scroll.y - oy;
    }
    ovl = (BgOverlay *)((u8 *)h + h->overlayOffset);
    for (i = 0; i < h->overlayCount; ovl++, i++) {
        if (ovl->flags & FLDBG_OVL_LOOP) {
            if (ovl->scrollX != 0) {
                ovl->x += dx;
            }
            if (ovl->scrollY != 0) {
                ovl->y += dy;
            }
        rate = g_fieldBgState.viewportRateX[ovl->viewport] << 8;
        fixPos = dx << 16;
        if (rate != 0x10000) {
            if (rate < 0) {
                fixPos = (ovl->x << 16) - fld_fixMul(fixPos, -rate);
            } else {
                fixPos = (ovl->x << 16) + fld_fixMul(fixPos, rate);
            }
            ovl->x = fixPos >> 16;
            ovl->fracX = (fixPos >> 8) & 0xFF;
        }
        rate = g_fieldBgState.viewportRateY[ovl->viewport] << 8;
        fixPos = dy << 16;
        if (rate != 0x10000) {
            if (rate < 0) {
                fixPos = (ovl->y << 16) - fld_fixMul(fixPos, -rate);
            } else {
                fixPos = (ovl->y << 16) + fld_fixMul(fixPos, rate);
            }
            ovl->y = fixPos >> 16;
            ovl->fracY = (fixPos >> 8) & 0xFF;
        }
        } else if (ovl->flags & FLDBG_OVL_SKEW_LOOP) {
            if (ovl->skewVertical) {
                ovl->y += dy;
            } else {
                ovl->x += dx;
            }
        rate = g_fieldBgState.viewportRateX[ovl->viewport] << 8;
        fixPos = dx << 16;
        if (rate != 0x10000) {
            if (rate < 0) {
                fixPos = (ovl->x << 16) - fld_fixMul(fixPos, -rate);
            } else {
                fixPos = (ovl->x << 16) + fld_fixMul(fixPos, rate);
            }
            ovl->x = fixPos >> 16;
            ovl->fracX = (fixPos >> 8) & 0xFF;
        }
        rate = g_fieldBgState.viewportRateY[ovl->viewport] << 8;
        fixPos = dy << 16;
        if (rate != 0x10000) {
            if (rate < 0) {
                fixPos = (ovl->y << 16) - fld_fixMul(fixPos, -rate);
                ovl->y = fixPos >> 16;
                ovl->fracY = (fixPos >> 8) & 0xFF;
            } else {
                fixPos = (ovl->y << 16) + fld_fixMul(fixPos, rate);
                ovl->y = fixPos >> 16;
                ovl->fracY = (fixPos >> 8) & 0xFF;
            }
        }
        }
    }
    return 1;
}

/* reset the field background state; returns the end of its allocations */
BgSprt *fldbg_init(void) {
    SVECTOR origin;
    BgSprt *spr;
    BgFile *h;
    BgOverlayAnimView *anims;
    BgOverlay *trk;
    u32 i;
    u32 animCount;
    s32 projWord;
    s32 x;
    union {
        s32 w;
        DVECTOR v;
    } projected; /* projected origin */

    g_fieldBgState.flags = FLDBG_FOLLOW | FLDBG_SCROLL_LIMITS;
    g_fieldBgState.unk74 = 0x40;
    g_fieldBgState.followHeight = 0x144;
    g_fieldBgState.radiusSq = 0xE10;
    *(s32 *)&g_fieldBgState.scroll = 0;
    *(s32 *)&g_fieldBgState.cameraScroll = 0;
    *(s32 *)&g_fieldBgState.geomOffsetX = 0;
    *(s32 *)&g_fieldBgState.scrollFrom = 0;
    *(s32 *)&g_fieldBgState.scrollToX = 0;
    *(s32 *)&g_fieldBgState.scrollFrame = 0;
    *(s32 *)&g_fieldBgState.savedScrollMinX = 0;
    *(s32 *)&g_fieldBgState.savedScrollMinY = 0;
    *(s32 *)&g_fieldBgState.playerScreenX = 0;
    g_fieldBgState.cameraId = 0;
    g_fieldBgState.unk9C = 0;
    g_fieldBgState.clutRows = 0;
    g_fieldBgState.unkA1 = 0xFF;
    g_fieldBgState.unkA2 = 0xFF;
    g_fieldBgState.unkA3 = 0xFF;
    *(s32 *)&g_fieldBgState.parallaxOriginX = 0;
    g_fieldBgState.viewMatrix = (MATRIX *)g_fieldBgCtx.viewMatrix;
    g_fieldBgState.projDist = (u16 *)g_fieldBgCtx.projDist;
    if (fldbg_initOverlays(g_fieldBgCtx.heap, &spr) != 1) {
        return NULL;
    }
    h = g_fieldBgCtx.data;
    anims = (BgOverlayAnimView *)((u8 *)h + h->overlayAnimOffset);
    trk = (BgOverlay *)((u8 *)h + h->overlayOffset);
    animCount = h->overlayAnimCount;
    for (i = 0; i < animCount; i++) {
        anims[i].cameraPos &= 0xFF;
        anims[i].speed = 0x100;
        anims[i].delayCounter = 0;
        anims[i].flags = 1;
        trk[*((u8 *)h + anims[i].stepOffset)].flags |= FLDBG_OVL_VISIBLE;
    }
    fldbg_initEffectList();
    fldbg_initFollowOverlays();
    origin.vx = 0;
    origin.vy = 0;
    origin.vz = 0;
    gte_SetGeomOffset(FLD_SCREEN_CX, FLD_SCREEN_CY);
    gte_SetRotMatrix(g_fieldBgState.viewMatrix);
    gte_SetTransMatrix(g_fieldBgState.viewMatrix);
    gte_ldv0(&origin);
    gte_rtps();
    gte_stsxy(&projected.w);
    projWord = projected.w;
    x = projected.v.vx;
    g_fieldBgState.playerScreenX = x;
    g_fieldBgState.playerScreenY = projWord >> 16;
    return spr;
}

s32 fldbg_draw(void) {
    BgFile *h;
    BgOverlay *ovls;
    FieldBgState *st;
    u32 i;
    s32 j;

    fldbg_updateOverlayAnims();
    /* the switch results are dead; only the loads and the hoisted 0x10/0x40 survive */
    do {
        for (i = 0; i < g_fieldBgCtx.entryCount; i++) {
            switch (g_fieldBgCtx.entries[i].type) {
            case BGENT_OVERLAY_ANIM:
                j = 1;
                break;
            case 0x40:
                j = 2;
                break;
            }
        }
    } while (0);
    j = 0;
    fldbg_updateScroll();
    fldbg_followPlayer();
    fldbg_updateOverlayScroll(g_fieldBgCtx.data);
    fldbg_updateFollowOverlays();
    st = &g_fieldBgState;
    h = g_fieldBgCtx.data;
    h->screenX = h->posX - (s16)(st->scroll.x - FLD_SCREEN_CX);
    h->screenY = h->posY - (s16)(st->scroll.y - FLD_SCREEN_CY);
    ovls = (BgOverlay *)((u8 *)h + h->overlayOffset);
    for (j = 0; j < h->overlayCount; j++) {
        if ((ovls[j].flags & FLDBG_OVL_VISIBLE) && ovls[j].cameraId == st->cameraId) {
            fldbg_drawOverlay(&ovls[j], g_fieldBgCtx.overlayRgb + j * 3);
        }
    }
    return 1;
}

s32 fldbg_scrollToPlayer(s32 frames, s32 scrollMode) {
    SVECTOR anchor;
    s16 xy[2];
    BgCamera *cam;
    EvtObject *player;
    u8 mode;
    s32 x;
    s32 y;
    s32 unusedOfs;
    s32 cp;
    FieldBgState *state;
    s32 baseFlags;
    s32 ox;
    s32 oy;
    BgFile *bg;
    FieldBgState *st = NULL; /* dead store: lets cse keep the st = state copy below */

    if (!(g_fieldBgState.flags & FLDBG_FOLLOW)) {
        return 0;
    }
    mode = g_gameState->mode;
    if (mode == 5) {
        player = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
    } else {
        player = NULL;
        if (mode == GS_MODE_FIELD) {
            player = evt_getPlayer();
        }
    }
    bg = g_fieldBgData;
    state = &g_fieldBgState;
    cam = (BgCamera *)((u8 *)bg + bg->cameraOffset);
    cp = *(s32 *)&state->scroll;
    /* a second pointer to g_fieldBgState whose copy disappears after register allocation; it is what
       makes sched1 order the copy/vx loads and local-alloc give the copy $v0 as in the original */
    st = state;
    *(s32 *)&st->scrollFrom = cp;
    anchor.vx = player->pos.vx;
    anchor.vy = player->pos.vy - st->followHeight;
    cam = &cam[st->cameraId];
    anchor.vz = player->pos.vz;
    ox = (s16)cam->centerX;
    oy = (s16)cam->centerY;
    gte_SetGeomOffset(ox + FLD_SCREEN_CX, oy + FLD_SCREEN_CY);
    {
        /* the original's "r"(st->unk1C) operand was reloaded into $t0; as a hard-register variable
           it is one less reference to st, which keeps st below the unk76 temporary in local-alloc */
        register MATRIX *viewMtx REG_ASM("$8") = st->viewMatrix;
        gte_SetRotMatrix(viewMtx);
        gte_SetTransMatrix(viewMtx);
    }
    gte_ldv0(&anchor);
    gte_rtps();
    gte_stsxy(xy);
    /* dead test (deleted by jump2): extends st's live range to here */
    if (st != NULL) {
        st = NULL;
    }
    {
        s32 ofsX = (cam->width >> 1) - FLD_SCREEN_CX;
        x = ofsX + xy[0];
    }
    {
        s32 ofsY = (cam->height >> 1) - FLD_SCREEN_CY;
        y = ofsY + xy[1];
    }
    if (x < cam->scrollMinX) {
        x = cam->scrollMinX;
    } else if (cam->scrollMaxX < x) {
        x = cam->scrollMaxX;
    }
    if (y < cam->scrollMinY) {
        y = cam->scrollMinY;
    } else if (cam->scrollMaxY < y) {
        y = cam->scrollMaxY;
    }
    g_fieldBgState.scrollToX = x;
    g_fieldBgState.scrollToY = y;
    if (frames == -1) {
        g_fieldBgState.scrollFrames = 0x1E;
    } else {
        g_fieldBgState.scrollFrames = frames;
    }
    g_fieldBgState.scrollFrame = 1;
    if (scrollMode == -1) {
        g_fieldBgState.flags = (g_fieldBgState.flags & ~FLDBG_SCROLL_MODE) | FLDBG_SCROLL_TO_PLAYER;
    } else {
        baseFlags = g_fieldBgState.flags & ~(FLDBG_SCROLL_MODE | FLDBG_SCROLL_SMOOTH);
        g_fieldBgState.flags = baseFlags | FLDBG_SCROLL_TO_PLAYER;
        if (scrollMode == FLDBG_SCROLL_SMOOTH) {
            g_fieldBgState.flags = baseFlags | FLDBG_SCROLL_SMOOTH | FLDBG_SCROLL_TO_PLAYER;
        } else {
            g_fieldBgState.flags = baseFlags | FLDBG_SCROLL_TO_PLAYER;
        }
    }
    return 1;
}

/* add a follow overlay for overlayId at (offsetX, offsetY); abr >= 0 also colours its quads r, g, b (semi-transparent below 4) */
s32 fldbg_addFollowOverlay(s16 overlayId, s16 offsetX, s16 offsetY, s8 abr, u8 r, u8 g, u8 b) {
    u32 i;

    g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].overlay = overlayId;
    g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].offsetX = offsetX;
    g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].offsetY = offsetY;
    g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].abr = abr;
    if (abr >= 0) {
        g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].r = r;
        g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].g = g;
        g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].b = b;
        for (i = 0; i < 4; i++) {
            SetPolyF4(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[0][i]);
            setRGB0(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[0][i], r, g, b);
            SetPolyF4(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[0][i + 4]); /* = unkC[1][i] */
            setRGB0(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[1][i], r, g, b);
            if (abr >= 4) {
                setSemiTrans(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[0][i], 0);
                setSemiTrans(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[1][i], 0);
            } else {
                setSemiTrans(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[0][i], 1);
                setSemiTrans(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[1][i], 1);
            }
        }
    }
    g_fieldBgFollowOverlayCount++;
    return 1;
}

/* toggle a background object's tint */
s32 fldbg_toggleOverlayColor(s32 overlayId, u8 r, u8 g, u8 b) {
    BgFile *h;
    BgOverlay *ovl;
    BgSprt *spr;
    u8 tinted;
    u16 nTiles;
    s32 off;
    u16 i;

    tinted = g_fieldBgCtx.overlayTinted[overlayId];
    h = g_fieldBgCtx.data;
    if (tinted != 0) {
        ovl = &((BgOverlay *)((u8 *)h + h->overlayOffset))[overlayId];
        nTiles = ovl->tileCount;
        spr = ovl->sprites;
        off = 0;
        if (*g_fieldBgCtx.bufIndex != 0) {
            off = nTiles;
        }
        for (i = 0; i < nTiles; i++) {
            setRGB0(&spr[i + off].sprt, r, g, b);
        }
        g_fieldBgCtx.overlayTinted[overlayId] = 0;
        return 0;
    }
    g_fieldBgCtx.overlayRgb[overlayId * 3 + 0] = r;
    g_fieldBgCtx.overlayRgb[overlayId * 3 + 1] = g;
    g_fieldBgCtx.overlayRgb[overlayId * 3 + 2] = b;
    ovl = &((BgOverlay *)((u8 *)h + h->overlayOffset))[overlayId];
    nTiles = ovl->tileCount;
    spr = ovl->sprites;
    off = 0;
    if (*g_fieldBgCtx.bufIndex != 0) {
        off = nTiles;
    }
    for (i = 0; i < nTiles; i++) {
        setRGB0(&spr[i + off].sprt, r, g, b);
    }
    g_fieldBgCtx.overlayTinted[overlayId] = 1;
    return 1;
}

s32 fldbg_startClutFade(s32 entryId, u8 mode, u32 frames, u8 first, s32 last, u8 r, u8 g, u8 b) {
    s32 rgb[3];
    BgEntry *entry;
    FieldPalAnim *fade;
    BgFile *h;
    s32 clutCount;
    s32 y;
    s32 size;
    u16 i;
    u8 *ovlBase;
    volatile u16 *w;
    s32 firstPrev;
    s32 k;
    register u16 clutVal REG_ASM("$3");

    firstPrev = first - 1;
    clutCount = (u8)last - firstPrev;
    size = clutCount * 4 + 0x2C;
    entry = &g_fieldBgCtx.entries[entryId];
    entry->type = BGENT_CLUT_FADE;
    entry->dataSize = size;
    h = g_fieldBgCtx.data;
    ovlBase = (u8 *)h + h->overlayOffset;
    y = *(u32 *)((u8 *)h + ((BgOverlay *)ovlBase)[entry->overlay].tileOffset) & 0x1FF;
    fade = (FieldPalAnim *)(g_fieldBgCtx.work + g_fieldBgCtx.workUsed);
    /* size is reused for the new end offset (two sets keep sched1 from moving its computation down to
     * entry->unk8), read through a copy so the add keeps the original operand order */
    k = size;
    size = g_fieldBgCtx.workUsed + k;
    g_fieldBgCtx.workUsed = size;
    entry->dataOffset = (u8 *)fade - (u8 *)g_fieldBgCtx.entries;
    fade->active = 1;
    fade->count = clutCount;
    fade->first = first;
    fade->last = last;
    fade->frames = frames;
    fade->frame = 0;
    if (frames >= 2) {
        rgb[0] = r << 16;
        rgb[1] = g << 16;
        rgb[2] = b << 16;
        switch (mode) {
        case 0:
            rgb[0] = ((u32)(r << 24) / (frames << 16)) << 8;
            rgb[1] = ((u32)(g << 24) / (frames << 16)) << 8;
            rgb[2] = ((u32)(b << 24) / (frames << 16)) << 8;
            fade->unk4 = 0;
            break;
        case 1:
            rgb[0] = -(((u32)(r << 24) / (frames << 16)) << 8);
            rgb[1] = -(((u32)(g << 24) / (frames << 16)) << 8);
            rgb[2] = -(((u32)(b << 24) / (frames << 16)) << 8);
            fade->unk4 = 0;
            break;
        case 2:
            rgb[0] = ((u32)(r << 24) / (frames << 16)) << 8;
            rgb[1] = ((u32)(g << 24) / (frames << 16)) << 8;
            rgb[2] = ((u32)(b << 24) / (frames << 16)) << 8;
            fade->unk4 = 1;
            break;
        }
    } else {
        switch (mode) {
        case 0:
            rgb[0] = r;
            rgb[1] = g;
            rgb[2] = b;
            fade->unk4 = 0;
            break;
        case 1:
            rgb[0] = -r;
            rgb[1] = -g;
            rgb[2] = -b;
            fade->unk4 = 0;
            break;
        case 2:
            rgb[0] = r;
            rgb[1] = g;
            rgb[2] = b;
            fade->unk4 = 1;
            break;
        }
    }
    fade->stepR = rgb[0];
    fade->stepG = rgb[1];
    fade->stepB = rgb[2];
    fade->savedOffset = 0x2C;
    fade->workOffset = clutCount * 2 + 0x2C;
    fade->rect.x = first;
    fade->rect.y = y;
    fade->rect.w = clutCount;
    fade->rect.h = 1;
    StoreImage(&fade->rect, (u32 *)((u8 *)fade + fade->savedOffset));
    w = (u16 *)((u8 *)fade + fade->workOffset);
    /* read the CLUT back; the original keeps the value in v1 (see REG_ASM) */
    for (i = 0; i < clutCount; i++) {
        clutVal = w[i];
    }
    g_fieldBgCtx.workUsed += entry->dataSize;
    return 1;
}

s32 fldbg_setCamera(s32 cameraId) {
    BgCamera *cam;
    BgFile *h;

    h = g_fieldBgData;
    cam = (BgCamera *)((u8 *)h + h->cameraOffset) + cameraId;
    *g_fieldBgState.projDist = cam->projDist;
    gte_SetGeomScreen(cam->projDist);
    g_fieldBgState.viewMatrix->m[0][0] = cam->rot[0][0];
    g_fieldBgState.viewMatrix->m[0][1] = cam->rot[0][1];
    g_fieldBgState.viewMatrix->m[0][2] = cam->rot[0][2];
    g_fieldBgState.viewMatrix->m[1][0] = cam->rot[1][0];
    g_fieldBgState.viewMatrix->m[1][1] = cam->rot[1][1];
    g_fieldBgState.viewMatrix->m[1][2] = cam->rot[1][2];
    g_fieldBgState.viewMatrix->m[2][0] = cam->rot[2][0];
    g_fieldBgState.viewMatrix->m[2][1] = cam->rot[2][1];
    g_fieldBgState.viewMatrix->m[2][2] = cam->rot[2][2];
    g_fieldBgState.viewMatrix->t[0] = cam->trans[0];
    g_fieldBgState.viewMatrix->t[1] = cam->trans[1];
    g_fieldBgState.viewMatrix->t[2] = cam->trans[2];
    h->scrollMaxX = cam->scrollMaxX;
    h->scrollMaxY = cam->scrollMaxY;
    g_fieldBgState.cameraId = cameraId;
    g_fieldBgState.flags |= FLDBG_CAMERA_CHANGED;
    g_gameState->gameData->ctx.field->area->scene->otBias = cam->otBias;
    return 1;
}

void fldbg_nop(void) {
}

void fldbg_setRenderContext(s32 bufIndex, s32 projDist, s32 ot0, s32 ot1, s32 viewMatrix, u8 *heap) {
    g_fieldBgCtx.bufIndex = (u8 *)bufIndex;
    g_fieldBgCtx.projDist = projDist;
    g_fieldBgCtx.ot[0] = (u32 *)ot0;
    g_fieldBgCtx.ot[1] = (u32 *)ot1;
    g_fieldBgCtx.data = 0;
    g_fieldBgCtx.entryCount = 0;
    g_fieldBgCtx.entries = NULL;
    g_fieldBgCtx.work = 0;
    g_fieldBgCtx.workSize = 0;
    g_fieldBgCtx.workUsed = 0;
    g_fieldBgCtx.viewMatrix = viewMatrix;
    g_fieldBgCtx.heap = heap;
    g_fieldBgCtx.overlayRgb = heap;
    g_fieldBgCtx.overlayTinted = heap;
}

void fldbg_setWorkBuffer(s32 work, s32 size) {
    g_fieldBgCtx.work = work;
    g_fieldBgCtx.workSize = size;
    g_fieldBgCtx.workUsed = 0;
}

void fldbg_setData(BgFile *bg) {
    g_fieldBgData = bg;
}

s32 fldbg_moveOrigin(s32 dx, s32 dy, s32 dz) {
    u16 x = g_fieldBgData->orgX + dx;
    u16 y = g_fieldBgData->orgY + dy;
    u16 z;

    g_fieldBgData->orgX = x;
    g_fieldBgData->orgY = y;
    g_fieldBgData->posX = x;
    g_fieldBgData->posY = y;
    z = g_fieldBgData->orgDepth + dz;
    g_fieldBgData->orgDepth = z;
    g_fieldBgData->baseDepth = z;
    return 1;
}

s32 fldbg_scrollTo(s16 x, s16 y, s16 frames, s32 scrollMode) {
    s32 baseFlags;

    if (!(g_fieldBgState.flags & FLDBG_FOLLOW)) {
        return 0;
    }
    g_fieldBgState.scrollFrame = 1;
    g_fieldBgState.scrollToX = x;
    baseFlags = g_fieldBgState.flags & ~(FLDBG_SCROLL_MODE | FLDBG_SCROLL_SMOOTH);
    g_fieldBgState.scrollToY = y;
    g_fieldBgState.scrollFrames = frames;
    g_fieldBgState.flags = baseFlags;
    g_fieldBgState.scrollFrom = g_fieldBgState.scroll;
    if (scrollMode == FLDBG_SCROLL_SMOOTH) {
        g_fieldBgState.flags = baseFlags | FLDBG_SCROLL_SMOOTH;
    } else {
        g_fieldBgState.flags = baseFlags;
    }
    g_fieldBgState.flags |= FLDBG_SCROLL_START;
    return 1;
}

s32 fldbg_getScroll(u16 *outX, u16 *outY) {
    *outX = g_fieldBgState.scroll.x;
    *outY = g_fieldBgState.scroll.y;
    return 1;
}

s32 fldbg_setFollow(s32 enable, s32 frames, s32 scrollMode) {
    if (enable != 0) {
        g_fieldBgState.flags |= FLDBG_FOLLOW;
        if (frames > 0) {
            fldbg_scrollToPlayer(frames, scrollMode);
        }
    } else {
        g_fieldBgState.flags &= ~FLDBG_FOLLOW;
    }
    return 1;
}

s32 fldbg_isFollowing(void) {
    if (g_fieldBgState.flags & FLDBG_FOLLOW) {
        return 1;
    }
    return 0;
}

s32 fldbg_applyGeomOffset(void) {
    BgCamera *cam = fldbg_cameraPtr(0);

    cam += g_fieldBgState.cameraId;
    if (!(mbg_getStatus() & MBG_STATUS_PLAYING) || g_mbgFrame < 2) {
        gte_SetGeomOffset((s16)cam->centerX + g_fieldBgState.geomOffsetX, (s16)cam->centerY + g_fieldBgState.geomOffsetY);
    }
    return 1;
}

s32 fldbg_resetGeomOffset(void) {
    if (!(mbg_getStatus() & MBG_STATUS_PLAYING)) {
        gte_SetGeomOffset(FLD_SCREEN_CX, FLD_SCREEN_CY);
    }
    return 1;
}

s32 fldbg_setRadius(s32 radius) {
    if (radius == -1) {
        g_fieldBgState.radiusSq = 3600;
    } else {
        g_fieldBgState.radiusSq = radius * radius;
    }
    return 1;
}

s32 fldbg_setFollowHeight(s16 height) {
    g_fieldBgState.followHeight = height;
    return 1;
}

s32 fldbg_hasScrollLimits(void) {
    if (g_fieldBgState.flags & FLDBG_SCROLL_LIMITS) {
        return 1;
    }
    return 0;
}

s32 fldbg_disableScrollLimits(void) {
    BgCamera *cam;

    if (!(g_fieldBgState.flags & FLDBG_SCROLL_LIMITS)) {
        return 0;
    }
    if (g_fieldBgData != NULL) {
        cam = fldbg_cameraPtr(g_fieldBgState.cameraId);
        g_fieldBgState.savedScrollMinX = cam->scrollMinX;
        g_fieldBgState.savedScrollMaxX = cam->scrollMaxX;
        g_fieldBgState.savedScrollMinY = cam->scrollMinY;
        g_fieldBgState.savedScrollMaxY = cam->scrollMaxY;
        cam->scrollMinX = -0x8000;
        cam->scrollMaxX = 0x7FFF;
        cam->scrollMinY = -0x8000;
        cam->scrollMaxY = 0x7FFF;
        g_fieldBgState.flags &= ~FLDBG_SCROLL_LIMITS;
    }
    return 1;
}

s32 fldbg_enableScrollLimits(void) {
    BgCamera *cam;

    if (g_fieldBgState.flags & FLDBG_SCROLL_LIMITS) {
        return 0;
    }
    if (g_fieldBgData != NULL) {
        cam = fldbg_cameraPtr(g_fieldBgState.cameraId);
        cam->scrollMinX = g_fieldBgState.savedScrollMinX;
        cam->scrollMaxX = g_fieldBgState.savedScrollMaxX;
        cam->scrollMinY = g_fieldBgState.savedScrollMinY;
        cam->scrollMaxY = g_fieldBgState.savedScrollMaxY;
        g_fieldBgState.flags |= FLDBG_SCROLL_LIMITS;
    }
    return 1;
}

s32 fldbg_setViewport(s32 viewportId, s16 x, s16 y, s16 w, s16 h) {
    g_fieldBgState.viewportPos[viewportId][0] = x;
    g_fieldBgState.viewportPos[viewportId][1] = y;
    g_fieldBgState.viewportSize[viewportId][0] = w;
    g_fieldBgState.viewportSize[viewportId][1] = h;
    return 1;
}

s32 fldbg_setViewportScrollRate(s32 viewportId, s16 rateX, s16 rateY) {
    g_fieldBgState.viewportRateX[viewportId] = rateX;
    g_fieldBgState.viewportRateY[viewportId] = rateY;
    return 1;
}

/* move a background object, clamped to its limits */
s32 fldbg_moveOverlay(s32 overlayId, s16 dx, s16 dy, s16 dz) {
    BgOverlay *ovl = &((BgOverlay *)((u8 *)g_fieldBgData + g_fieldBgData->overlayOffset))[overlayId];
    s16 x = ovl->baseX + dx;
    s16 y = ovl->baseY + dy;
    s16 z = dz + ovl->baseDepth;

    if (x < ovl->minX) {
        x = ovl->minX;
    } else if (x > ovl->maxX) {
        x = ovl->maxX;
    }
    if (y < ovl->minY) {
        y = ovl->minY;
    } else if (y > ovl->maxY) {
        y = ovl->maxY;
    }
    ovl->baseX = x;
    ovl->baseDepth = z;
    ovl->baseY = y;
    ovl->x = x;
    ovl->y = y;
    ovl->depth = z;
    return 1;
}

s32 fldbg_getOverlayColor(s32 overlayId, u8 *outR, u8 *outG, u8 *outB) {
    *outR = g_fieldBgCtx.overlayRgb[overlayId * 3];
    *outG = g_fieldBgCtx.overlayRgb[overlayId * 3 + 1];
    *outB = g_fieldBgCtx.overlayRgb[overlayId * 3 + 2];
    return 1;
}

s32 fldbg_setOverlayColor(s32 overlayId, u8 r, u8 g, u8 b) {
    BgFile *h;
    BgOverlay *ovl;
    BgSprt *spr;
    u16 nTiles;
    s32 off;
    u16 i;

    h = g_fieldBgCtx.data;
    g_fieldBgCtx.overlayRgb[overlayId * 3 + 0] = r;
    g_fieldBgCtx.overlayRgb[overlayId * 3 + 1] = g;
    g_fieldBgCtx.overlayRgb[overlayId * 3 + 2] = b;
    ovl = &((BgOverlay *)((u8 *)h + h->overlayOffset))[overlayId];
    nTiles = ovl->tileCount;
    spr = ovl->sprites;
    off = 0;
    if (*g_fieldBgCtx.bufIndex != 0) {
        off = nTiles;
    }
    for (i = 0; i < nTiles; i++) {
        setRGB0(&spr[i + off].sprt, r, g, b);
    }
    return 1;
}

s32 fldbg_showOverlay(s32 overlayId, s32 visible) {
    BgOverlay *ovl = fldbg_overlayPtr(overlayId);
    u8 newFlags;

    if (visible != 0) {
        newFlags = ovl->flags | FLDBG_OVL_VISIBLE;
    } else {
        newFlags = ovl->flags & ~FLDBG_OVL_VISIBLE;
    }
    ovl->flags = newFlags;
    return 1;
}

s32 fldbg_setOverlayLoop(s32 overlayId, s32 enable, s16 scrollX, s16 scrollY) {
    BgOverlay *ovl = fldbg_overlayPtr(overlayId);

    if (enable != 0) {
        ovl->flags |= FLDBG_OVL_LOOP;
    } else {
        ovl->flags &= ~FLDBG_OVL_LOOP;
    }
    ovl->scrollX = scrollX;
    ovl->fracX = 0;
    ovl->scrollY = scrollY;
    ovl->fracY = 0;
    return 1;
}

s32 fldbg_setOverlayScreen(s32 overlayId, s32 enable) {
    BgOverlay *ovl = fldbg_overlayPtr(overlayId);
    u8 newFlags;

    if (enable != 0) {
        newFlags = ovl->flags | FLDBG_OVL_SCREEN;
    } else {
        newFlags = ovl->flags & ~FLDBG_OVL_SCREEN;
    }
    ovl->flags = newFlags;
    return 1;
}

s32 fldbg_setOverlaySkewLoop(s32 overlayId, s32 enable, s16 scrollX, s16 scrollY, s32 vertical) {
    BgOverlay *ovl = fldbg_overlayPtr(overlayId);

    if (enable != 0) {
        ovl->flags |= FLDBG_OVL_SKEW_LOOP;
    } else {
        ovl->flags &= ~FLDBG_OVL_SKEW_LOOP;
    }
    if (vertical != 0) {
        ovl->scrollX = scrollY;
        ovl->scrollY = scrollX;
        ovl->skewVertical = 1;
    } else {
        ovl->scrollX = scrollX;
        ovl->scrollY = scrollY;
        ovl->skewVertical = 0;
    }
    ovl->fracX = 0;
    ovl->fracY = 0;
    return 1;
}

s32 fldbg_setOverlayParallax(s32 overlayId, s32 enable, s16 scrollX, s16 scrollY) {
    BgOverlay *ovl = fldbg_overlayPtr(overlayId);

    if (enable != 0) {
        ovl->flags |= FLDBG_OVL_PARALLAX;
    } else {
        ovl->flags &= ~FLDBG_OVL_PARALLAX;
    }
    ovl->scrollX = scrollX;
    ovl->scrollY = scrollY;
    return 1;
}

s32 fldbg_setOverlayPos(s32 overlayId, s16 x, s16 y) {
    BgOverlay *ovl = fldbg_overlayPtr(overlayId);

    ovl->x = x;
    ovl->baseX = x;
    ovl->y = y;
    ovl->baseY = y;
    g_fieldBgState.flags |= FLDBG_CAMERA_CHANGED;
    return 1;
}

s32 fldbg_setOverlayViewport(s32 overlayId, s32 viewport) {
    BgOverlay *ovl = fldbg_overlayPtr(overlayId);

    ovl->viewport = viewport;
    return 1;
}

s32 fldbg_setAnimFrame(s32 animId, s32 frame) {
    BgOverlayAnim *anim = fldbg_overlayAnimPtr(animId);

    anim->delayCounter = 0;
    anim->flags |= FLDBG_ANIM_ACTIVE;
    anim->pos = frame << 8;
    return 1;
}

s32 fldbg_showAnimFrame(s32 animId, s32 frame) {
    s32 i;
    BgOverlayAnim *anim;
    BgAnimStep *st;
    BgOverlay *objs;

    i = 0;
    anim = fldbg_overlayAnimPtr(animId);
    st = fldbg_animSteps(anim);
    objs = fldbg_overlayPtr(0);
    for (; i < anim->stepCount; st++, i++) {
        if (i == frame) {
            objs[st->overlay].flags |= FLDBG_OVL_VISIBLE;
        } else {
            objs[st->overlay].flags &= ~FLDBG_OVL_VISIBLE;
        }
    }
    anim->flags = 1;
    return 1;
}

s32 fldbg_playAnim(s32 animId, s32 play) {
    BgOverlayAnim *anim = fldbg_overlayAnimPtr(animId);
    u8 newFlags;

    if (play != 0) {
        newFlags = anim->flags | (FLDBG_ANIM_ACTIVE | FLDBG_ANIM_PLAY_ONCE);
    } else {
        newFlags = anim->flags & ~(FLDBG_ANIM_ACTIVE | FLDBG_ANIM_PLAY_ONCE);
    }
    anim->flags = newFlags;
    return 1;
}

s32 fldbg_setAnimSpeed(s32 animId, s32 speed) {
    fldbg_overlayAnimPtr(animId)->speed = speed;
    return 1;
}

s32 fldbg_setAnimDelayAll(s32 animId, u8 delay) {
    BgOverlayAnim *anim = fldbg_overlayAnimPtr(animId);
    BgAnimStep *st = fldbg_animSteps(anim);
    s32 nSteps = anim->stepCount;
    s32 i;

    for (i = 0; i < nSteps; i++) {
        st[i].delay = delay;
    }
    return 1;
}

s32 fldbg_setAnimStepDelay(s32 animId, s32 step, u8 delay) {
    fldbg_animSteps(fldbg_overlayAnimPtr(animId))[step].delay = delay;
    return 1;
}

s32 fldbg_setAnimFlags(s32 animId, s32 flags) {
    fldbg_overlayAnimPtr(animId)->flags |= flags & (FLDBG_ANIM_LOOP | FLDBG_ANIM_PINGPONG);
    return 1;
}

s32 fldbg_playAnimRange(s32 animId, u32 first, u32 last) {
    BgFile *bg = g_fieldBgData;
    BgOverlayAnim *anim = &((BgOverlayAnim *)((u8 *)bg + bg->overlayAnimOffset))[animId];
    BgAnimStep *st = (BgAnimStep *)((u8 *)bg + anim->stepOffset);

    anim->delayCounter = 0;
    anim->flags |= (FLDBG_ANIM_ACTIVE | FLDBG_ANIM_PLAY_ONCE);
    anim->pos = first << 8;
    st[last].delay = -1;
    if ((anim->speed > 0 && last < first) || (anim->speed < 0 && first < last)) {
        anim->speed = -anim->speed;
    }
    return 1;
}

s32 fldbg_resetAnim(s32 animId, s32 showFirst) {
    BgFile *bg = g_fieldBgData;
    BgOverlayAnim *anim = &((BgOverlayAnim *)((u8 *)bg + bg->overlayAnimOffset))[animId];
    BgOverlay *objs;
    BgAnimStep *st;
    s32 i;

    anim->speed = 0x100;
    i = 0;
    anim->delayCounter = 0;
    anim->flags = 1;
    anim->pos = 0;
    objs = (BgOverlay *)((u8 *)bg + bg->overlayOffset);
    st = (BgAnimStep *)((u8 *)bg + anim->stepOffset);
    for (; i < anim->stepCount; i++) {
        if (showFirst != 0 && i == 0) {
            objs[st[i].overlay].flags |= FLDBG_OVL_VISIBLE;
        } else {
            objs[st[i].overlay].flags &= ~FLDBG_OVL_VISIBLE;
        }
    }
    return 1;
}

s32 fldbg_getEntryType(s32 entryId, u8 *outType) {
    *outType = g_fieldBgCtxEntries[entryId].type;
    return 1;
}

s32 fldbg_getWorkBlock(s32 block) {
    return g_fieldBgCtxWork + (block << 9);
}

/* set the camera scroll limits */
s32 fldbg_setScrollLimits(s32 cameraId, s16 minX, s16 maxX, s16 minY, s16 maxY) {
    BgCamera *cam = &((BgCamera *)((u8 *)g_fieldBgData + g_fieldBgData->cameraOffset))[cameraId];
    s16 limit;

    cam->scrollMinX = limit = minX + FLD_SCREEN_CX;
    if (limit > cam->width - FLD_SCREEN_CX) {
        cam->scrollMinX = cam->width - FLD_SCREEN_CX;
    }
    cam->scrollMaxX = limit = maxX - FLD_SCREEN_CX;
    if (limit < FLD_SCREEN_CX) {
        cam->scrollMaxX = FLD_SCREEN_CX;
    }
    cam->scrollMinY = limit = minY + FLD_SCREEN_CY;
    if (limit > cam->height - FLD_SCREEN_CY) {
        cam->scrollMinY = cam->height - FLD_SCREEN_CY;
    }
    cam->scrollMaxY = limit = maxY - FLD_SCREEN_CY;
    if (limit < FLD_SCREEN_CY) {
        cam->scrollMaxY = FLD_SCREEN_CY;
    }
    return 1;
}

s32 fldbg_getCamera(u8 *outCameraId) {
    *outCameraId = g_fieldBgState.cameraId;
    return 1;
}

s32 fldbg_setCameraCenter(s32 cameraId, s16 x, s16 y) {
    BgCamera *cam = fldbg_cameraPtr(cameraId);

    cam->centerX = x;
    cam->centerY = y;
    return 1;
}

s32 fldbg_getCameraCenter(s32 cameraId, u16 *outX, u16 *outY) {
    BgCamera *cam = fldbg_cameraPtr(cameraId);

    *outX = cam->centerX;
    *outY = cam->centerY;
    return 1;
}

s32 fldbg_getClutFadeSize(s32 count) {
    return count * 4 + 0x2C;
}

s32 fldbg_allocWork(s32 size) {
    s32 addr = g_fieldBgCtx.work + g_fieldBgCtx.workUsed;

    g_fieldBgCtx.workUsed += size;
    return addr;
}

s32 fldbg_resetAllAnims(void) {
    BgFile *bg = g_fieldBgData;
    u32 i;
    BgOverlayAnim *anims;
    BgOverlay *objs;
    u16 animCount;

    i = 0;
    anims = (BgOverlayAnim *)((u8 *)bg + bg->overlayAnimOffset);
    objs = (BgOverlay *)((u8 *)bg + bg->overlayOffset);
    animCount = bg->overlayAnimCount;
    for (; i < animCount; i++) {
        anims[i].speed = 0x100;
        anims[i].delayCounter = 0;
        anims[i].flags = 1;
        anims[i].pos = 0;
        objs[((BgAnimStep *)((u8 *)bg + anims[i].stepOffset))->overlay].flags |= FLDBG_OVL_VISIBLE;
    }
    return 1;
}

s32 fldbg_scanEntries(void) {
    u32 i;
    s32 ok;
    s32 type;

    ok = 1;
    for (i = 0; i < g_fieldBgCtx.entryCount; i++) {
        type = g_fieldBgCtx.entries[i].type;
        if (type == BGENT_OVERLAY_ANIM) {
            type = 0;
        } else if (type == 0x40) {
            type = ok;
        }
    }
    return ok;
}

s32 fldbg_nopOk1(void) {
    return 1;
}

s32 fldbg_setOverlaySpriteColor(s32 overlayId, u8 r, u8 g, u8 b) {
    BgOverlay *ovl = fldbg_overlayPtr(overlayId);
    u16 nTiles = ovl->tileCount;
    BgSprt *spr = ovl->sprites;
    u16 i;

    for (i = 0; i < nTiles; i++) {
        setRGB0(&spr[i].sprt, r, g, b);
        setRGB0(&spr[i + nTiles].sprt, r, g, b);
    }
    return 1;
}

s32 fldbg_nopOk2(void) {
    return 1;
}

s32 fldbg_nopOk3(void) {
    return 1;
}

s32 fldbg_nopOk4(void) {
    return 1;
}
