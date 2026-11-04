#include "ovl_0b.h"

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/btl_mesh", btl_drawMesh);

void btl_projectMesh(BtlModel *model, u8 meshIndex) {
    MATRIX boneView;
    BtlMesh *mesh;
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

    mesh = &model->header->meshes[meshIndex];
    gte_SetGeomScreen(g_gameState->gameData->projDist);
    sxy = g_vtxScreenXY;
    sz = g_vtxScreenZ;
    k = 0;
    cnt = model->header->boneCount;
    bone = (MATRIX *)model->bones;
    for (i = 0; i < cnt; bone++) {
        vtxLeft = mesh->vertexCounts[i++];
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
            vtx = &mesh->vertices[k];
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

u8 *btl_buildMeshPrims(BtlModel *model, u8 meshIndex, u8 *prim) {
    s32 opz;
    s32 otz;
    BtlMesh *mesh;
    u8 (*uv)[2];
    u8 (*col)[4];
    u32 *ot;
    u32 *tex;
    u32 primCount;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 *cp;
    u32 i0, i1, i2, i3;
    u32 texWord;
    u32 hi;
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

    if (model->flags & BTL_MODEL_OFFSCREEN) {
        return prim;
    }
    mesh = &model->header->meshes[meshIndex];
    sFT4 = (WmSrcFT4 *)mesh->prims;
    uv = mesh->uvs;
    col = mesh->colors;
    if (model->prims == 0) {
        model->prims = (s32)prim;
    }
    tex = (u32 *)model->texPages;
    ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf];
    ot -= mesh->otBias;

    primCount = mesh->nFT4;
    for (i = 0; i < primCount; sFT4++) {
        pFT4 = (POLY_FT4 *)prim;
        i0 = *(u32 *)&sFT4->vtx[0];
        i2 = *(u32 *)&sFT4->vtx[2];
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
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(u32 *)&pFT4->r0 = *(u32 *)sFT4->rgb;
        setlen(pFT4, PRIM_LEN(POLY_FT4));
        *(s32 *)&pFT4->x0 = xy0;
        *(s32 *)&pFT4->x1 = xy1;
        *(s32 *)&pFT4->x2 = xy2;
        setcode(pFT4, GPU_POLY_FT4);
        *(s32 *)&pFT4->x3 = g_vtxScreenXY[i3];
        otz = g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3];
        i0 = *(u32 *)&sFT4->uv[0];
        i2 = *(u32 *)&sFT4->uv[2];
        texWord = tex[sFT4->tex];
        hi = texWord & 0xFFFF0000;
        texWord <<= 16;
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        *(u32 *)&pFT4->u0 = *(u16 *)uv[i0] | hi;
        *(u32 *)&pFT4->u1 = *(u16 *)uv[i1] | texWord;
        *(u16 *)&pFT4->u2 = *(u16 *)uv[i2];
        *(u16 *)&pFT4->u3 = *(u16 *)uv[i3];
        if (sFT4->flags & WMSRC_SEMITRANS) {
            pFT4->tpage |= sFT4->flags & WMSRC_TPAGE_BITS;
            setSemiTrans(pFT4, 1);
        }
        otz >>= 6;
        addPrim(&ot[otz], pFT4);
        prim += sizeof(POLY_FT4);
    }

    primCount = mesh->nFT3;
    sFT3 = (WmSrcFT3 *)sFT4;
    for (i = 0; i < primCount; sFT3++) {
        pFT3 = (POLY_FT3 *)prim;
        i0 = *(u32 *)&sFT3->vtx[0];
        i2 = *(u32 *)&sFT3->vtx[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(u32 *)&pFT3->r0 = *(u32 *)sFT3->rgb;
        setlen(pFT3, PRIM_LEN(POLY_FT3));
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        *(s32 *)&pFT3->x2 = xy2;
        setcode(pFT3, GPU_POLY_FT3);
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        texWord = tex[sFT3->tex];
        hi = texWord & 0xFFFF0000;
        texWord <<= 16;
        gte_avsz3_b();
        i0 = *(u32 *)&sFT3->uv[0];
        i2 = *(u32 *)&sFT3->uv[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        *(u32 *)&pFT3->u0 = *(u16 *)uv[i0] | hi;
        *(u32 *)&pFT3->u1 = *(u16 *)uv[i1] | texWord;
        *(u16 *)&pFT3->u2 = *(u16 *)uv[i2];
        gte_stotz(&otz);
        if (sFT3->flags & WMSRC_SEMITRANS) {
            pFT3->tpage |= sFT3->flags & WMSRC_TPAGE_BITS;
            setSemiTrans(pFT3, 1);
        }
        otz >>= 2;
        addPrim(&ot[otz], pFT3);
        prim += sizeof(POLY_FT3);
    }

    primCount = mesh->nGT4;
    sGT4 = (WmSrcGT4 *)sFT3;
    for (i = 0; i < primCount; sGT4++) {
        pGT4 = (POLY_GT4 *)prim;
        i0 = *(u32 *)&sGT4->vtx[0];
        i2 = *(u32 *)&sGT4->vtx[2];
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
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(s32 *)&pGT4->x0 = xy0;
        *(s32 *)&pGT4->x1 = xy1;
        *(s32 *)&pGT4->x2 = xy2;
        *(s32 *)&pGT4->x3 = g_vtxScreenXY[i3];
        otz = g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3];
        i0 = *(u32 *)&sGT4->col[0];
        i2 = *(u32 *)&sGT4->col[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        texWord = tex[sGT4->tex];
        hi = texWord & 0xFFFF0000;
        texWord <<= 16;
        cp = (u32 *)col[i0];
        *(u32 *)&pGT4->r0 = *cp;
        cp = (u32 *)col[i1];
        *(u32 *)&pGT4->r1 = *cp;
        cp = (u32 *)col[i2];
        *(u32 *)&pGT4->r2 = *cp;
        cp = (u32 *)col[i3];
        *(u32 *)&pGT4->r3 = *cp;
        setPolyGT4(pGT4);
        i0 = *(u32 *)&sGT4->uv[0];
        i2 = *(u32 *)&sGT4->uv[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        *(u32 *)&pGT4->u0 = *(u16 *)uv[i0] | hi;
        *(u32 *)&pGT4->u1 = *(u16 *)uv[i1] | texWord;
        *(u16 *)&pGT4->u2 = *(u16 *)uv[i2];
        *(u16 *)&pGT4->u3 = *(u16 *)uv[i3];
        if (sGT4->flags & WMSRC_SEMITRANS) {
            pGT4->tpage |= sGT4->flags & WMSRC_TPAGE_BITS;
            setSemiTrans(pGT4, 1);
        }
        otz >>= 6;
        addPrim(&ot[otz], pGT4);
        prim += sizeof(POLY_GT4);
    }

    primCount = mesh->nGT3;
    sGT3 = (WmSrcGT3 *)sGT4;
    for (i = 0; i < primCount; sGT3++) {
        pGT3 = (POLY_GT3 *)prim;
        i0 = *(u32 *)&sGT3->vtx[0];
        i2 = *(u32 *)&sGT3->vtx[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(s32 *)&pGT3->x0 = xy0;
        gte_ldsz3(g_vtxScreenZ[(u16)i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(s32 *)&pGT3->x1 = xy1;
        *(s32 *)&pGT3->x2 = xy2;
        gte_avsz3_b();
        texWord = tex[sGT3->tex];
        hi = texWord & 0xFFFF0000;
        texWord <<= 16;
        gte_stotz(&otz);
        i0 = *(u32 *)&sGT3->col[0];
        i2 = *(u32 *)&sGT3->col[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        cp = (u32 *)col[i0];
        *(u32 *)&pGT3->r0 = *cp;
        cp = (u32 *)col[i1];
        *(u32 *)&pGT3->r1 = *cp;
        cp = (u32 *)col[i2];
        *(u32 *)&pGT3->r2 = *cp;
        setPolyGT3(pGT3);
        i1 = *(u32 *)&sGT3->uv[1];
        i0 = sGT3->uv[0];
        i2 = i1 >> 16;
        i1 &= 0xFFFF;
        *(u32 *)&pGT3->u0 = *(u16 *)uv[i0] | hi;
        *(u32 *)&pGT3->u1 = *(u16 *)uv[i1] | texWord;
        *(u16 *)&pGT3->u2 = *(u16 *)uv[i2];
        if (sGT3->flags & WMSRC_SEMITRANS) {
            pGT3->tpage |= sGT3->flags & WMSRC_TPAGE_BITS;
            setSemiTrans(pGT3, 1);
        }
        otz >>= 2;
        addPrim(&ot[otz], pGT3);
        prim += sizeof(POLY_GT3);
    }

    primCount = mesh->nG4;
    sG4 = (WmSrcG4 *)sGT3;
    for (i = 0; i < primCount; sG4++) {
        pG4 = (POLY_G4 *)prim;
        i0 = *(u32 *)&sG4->vtx[0];
        i2 = *(u32 *)&sG4->vtx[2];
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
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(s32 *)&pG4->x0 = xy0;
        *(s32 *)&pG4->x1 = xy1;
        *(s32 *)&pG4->x2 = xy2;
        *(s32 *)&pG4->x3 = g_vtxScreenXY[i3];
        otz = g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3];
        i0 = *(u32 *)&sG4->col[0];
        i2 = *(u32 *)&sG4->col[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        cp = (u32 *)col[i0];
        *(u32 *)&pG4->r0 = *cp;
        cp = (u32 *)col[i1];
        *(u32 *)&pG4->r1 = *cp;
        cp = (u32 *)col[i2];
        *(u32 *)&pG4->r2 = *cp;
        cp = (u32 *)col[i3];
        *(u32 *)&pG4->r3 = *cp;
        setPolyG4(pG4);
        otz >>= 6;
        addPrim(&ot[otz], pG4);
        prim += sizeof(POLY_G4);
    }

    primCount = mesh->nG3;
    sG3 = (WmSrcG3 *)sG4;
    for (i = 0; i < primCount; sG3++) {
        pG3 = (POLY_G3 *)prim;
        i0 = *(u32 *)&sG3->vtx[0];
        i2 = *(u32 *)&sG3->vtx[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[(u16)i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        *(s32 *)&pG3->x2 = xy2;
        gte_avsz3_b();
        i0 = *(u32 *)&sG3->col[0];
        i2 = *(u32 *)&sG3->col[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        cp = (u32 *)col[i0];
        *(u32 *)&pG3->r0 = *cp;
        cp = (u32 *)col[i1];
        *(u32 *)&pG3->r1 = *cp;
        cp = (u32 *)col[i2];
        *(u32 *)&pG3->r2 = *cp;
        gte_stotz(&otz);
        setPolyG3(pG3);
        otz >>= 2;
        addPrim(&ot[otz], pG3);
        prim += sizeof(POLY_G3);
    }

    primCount = mesh->nF4;
    sF4 = (WmSrcF4 *)sG3;
    for (i = 0; i < primCount; sF4++) {
        pF4 = (POLY_F4 *)prim;
        i0 = *(u32 *)&sF4->vtx[0];
        i2 = *(u32 *)&sF4->vtx[2];
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
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3];
        *(u32 *)&pF4->r0 = *(u32 *)sF4->rgb;
        setlen(pF4, PRIM_LEN(POLY_F4));
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        setcode(pF4, GPU_POLY_F4);
        *(s32 *)&pF4->x3 = g_vtxScreenXY[i3];
        otz >>= 6;
        addPrim(&ot[otz], pF4);
        prim += sizeof(POLY_F4);
    }

    primCount = mesh->nF3;
    sF3 = (WmSrcF3 *)sF4;
    for (i = 0; i < primCount; sF3++) {
        pF3 = (POLY_F3 *)prim;
        i0 = *(u32 *)&sF3->vtx[0];
        i2 = *(u32 *)&sF3->vtx[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(u32 *)&pF3->r0 = *(u32 *)sF3->rgb;
        gte_avsz3_b();
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        *(s32 *)&pF3->x2 = xy2;
        gte_stotz(&otz);
        setPolyF3(pF3);
        otz >>= 2;
        addPrim(&ot[otz], pF3);
        prim += sizeof(POLY_F3);
    }
    setcode((P_TAG *)prim, BTL_PRIM_MESH_END);
    return prim + sizeof(P_TAG);
}

void btl_applyModelTextures(BtlModel *model, s16 *texPages, s16 *uvOffsets) {
    BtlModelHeader *h;
    BtlMesh *mesh;
    u32 i;
    u32 j;
    u32 k;
    u8 *src;
    WmSrcFT4 *ft4;
    WmSrcFT3 *ft3;
    WmSrcGT4 *gt4;
    WmSrcGT3 *gt3;
    u8 *mark;
    s32 du;
    s32 dv;

    model->scaleX = 0x1000;
    model->scaleY = 0x1000;
    model->scaleZ = 0x1000;
    model->flags |= BTL_MODEL_SCALED;
    h = model->header;
    model->texPages = (BtlTexWord *)texPages;
    model->parent = NULL;
    model->unk60 = NULL;
    model->morph = NULL;
    model->unk44 = 0;
    model->bones = NULL;
    model->prims = 0;
    model->texAnims = NULL;
    model->attachX = 0;
    model->attachY = 0;
    model->attachZ = 0;
    model->hiddenMeshes = 0;
    model->flags = 0;
    if (!(h->flags & BTL_MODELHDR_RELOCATED)) {
        h->flags |= BTL_MODELHDR_RELOCATED;
        mesh = h->meshes = (BtlMesh *)((u8 *)h + (s32)h->meshes);
        h->bones = (s32)h + h->bones;
        for (i = 0; i < h->meshCount; i++, mesh++) {
            mesh->vertexCounts = (u16 *)((u8 *)h + (s32)mesh->vertexCounts);
            mesh->vertices = (SVECTOR *)((u8 *)h + (s32)mesh->vertices);
            mesh->prims = (u32 *)((u8 *)h + (s32)mesh->prims);
            mesh->uvs = (void *)((u8 *)h + (s32)mesh->uvs);
            mesh->colors = (void *)((u8 *)h + (s32)mesh->colors);
        }
        if (uvOffsets != NULL) {
            for (i = 0; i < h->meshCount; i++) {
                mark = (u8 *)g_vtxScreenZ;
                for (k = 0; k < 0x1908; k++) {
                    mark[k] = 0;
                }
                mesh = &h->meshes[i];
                src = (u8 *)mesh->prims;
                for (k = 0; k < mesh->nFT4; k++) {
                    ft4 = (WmSrcFT4 *)src;
                    du = uvOffsets[ft4->tex * 2];
                    dv = uvOffsets[ft4->tex * 2 + 1];
                    for (j = 0; j < 4; j++) {
                        if (mark[ft4->uv[j]] == 0) {
                            mesh->uvs[ft4->uv[j]][0] += du;
                            mesh->uvs[ft4->uv[j]][1] += dv;
                            mark[ft4->uv[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcFT4);
                }
                for (k = 0; k < mesh->nFT3; k++) {
                    ft3 = (WmSrcFT3 *)src;
                    du = uvOffsets[ft3->tex * 2];
                    dv = uvOffsets[ft3->tex * 2 + 1];
                    for (j = 0; j < 3; j++) {
                        if (mark[ft3->uv[j]] == 0) {
                            mesh->uvs[ft3->uv[j]][0] += du;
                            mesh->uvs[ft3->uv[j]][1] += dv;
                            mark[ft3->uv[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcFT3);
                }
                for (k = 0; k < mesh->nGT4; k++) {
                    gt4 = (WmSrcGT4 *)src;
                    du = uvOffsets[gt4->tex * 2];
                    dv = uvOffsets[gt4->tex * 2 + 1];
                    for (j = 0; j < 4; j++) {
                        if (mark[gt4->uv[j]] == 0) {
                            mesh->uvs[gt4->uv[j]][0] += du;
                            mesh->uvs[gt4->uv[j]][1] += dv;
                            mark[gt4->uv[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcGT4);
                }
                for (k = 0; k < mesh->nGT3; k++) {
                    gt3 = (WmSrcGT3 *)src;
                    du = uvOffsets[gt3->tex * 2];
                    dv = uvOffsets[gt3->tex * 2 + 1];
                    for (j = 0; j < 3; j++) {
                        if (mark[gt3->uv[j]] == 0) {
                            mesh->uvs[gt3->uv[j]][0] += du;
                            mesh->uvs[gt3->uv[j]][1] += dv;
                            mark[gt3->uv[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcGT3);
                }
            }
        }
    }
}

void btl_applyModelScale(BtlModel *model) {
    union {
        MATRIX m;
        s32 w[8];
    } scaleMat;
    s16 x;

    if (model->flags & BTL_MODEL_SCALED) {
        x = model->scaleX;
        scaleMat.w[0] = 0x1000;
        scaleMat.w[1] = 0;
        scaleMat.w[2] = 0x1000;
        scaleMat.w[3] = 0;
        scaleMat.w[4] = 0x1000;
        scaleMat.m.m[0][0] = x;
        gte_SetRotMatrix(&model->matrix);
        gte_ldclmv(&scaleMat.m);
        gte_rtir();
        scaleMat.m.m[1][1] = model->scaleY;
        gte_stclmv(&model->matrix);
        gte_ldclmv((s16 *)&scaleMat.m + 1);
        gte_rtir();
        scaleMat.m.m[2][2] = model->scaleZ;
        gte_stclmv((s16 *)&model->matrix + 1);
        gte_ldclmv((s16 *)&scaleMat.m + 2);
        gte_rtir();
        gte_stclmv((s16 *)&model->matrix + 2);
    }
}

void btl_updateModelVisibility(BtlModel *model) {
    SVECTOR pos;
    s32 z;

    model->flags &= ~BTL_MODEL_OFFSCREEN;
    if (model->parent != NULL) {
        model->flags |= model->parent->flags & BTL_MODEL_OFFSCREEN;
        return;
    }
    pos.vx = model->matrix.t[0];
    pos.vy = model->matrix.t[1];
    pos.vz = model->matrix.t[2];
    gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    gte_ldv0(&pos);
    gte_rtps();
    gte_stsz(&z);
    if (z == 0) {
        model->flags |= BTL_MODEL_OFFSCREEN;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/btl_mesh", btl_calcBoneMatrices);

void btl_setModelRgb(BtlModel *model, s32 r, s32 g, s32 b) {
    u32 i;
    BtlPrim *prim;
    u8 code;

    if (!(model->flags & BTL_MODEL_OFFSCREEN)) {
    i = 0;
    prim = (BtlPrim *)model->prims;
    if (model->header->meshCount != 0) {
    do {
        if (!(model->hiddenMeshes & (1 << i))) {
        do {
            code = prim->tag.code;
            if (code == BTL_PRIM_MESH_END || code == GPU_DRAW_MODE) {
                prim = (BtlPrim *)&prim->w[2];
                continue;
            }
            switch (code & GPU_POLY_KIND) {
            case GPU_POLY_FT4:
                prim->w[1] = btl_addRgbClamped(prim->w[1], (s16)r, (s16)g, (s16)b);
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[10];
                break;
            case GPU_POLY_FT3:
                prim->w[1] = btl_addRgbClamped(prim->w[1], (s16)r, (s16)g, (s16)b);
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[8];
                break;
            case GPU_POLY_GT4:
                prim->w[1] = btl_addRgbClamped(prim->w[1], (s16)r, (s16)g, (s16)b);
                prim->w[4] = btl_addRgbClamped(prim->w[4], (s16)r, (s16)g, (s16)b);
                prim->w[7] = btl_addRgbClamped(prim->w[7], (s16)r, (s16)g, (s16)b);
                prim->w[10] = btl_addRgbClamped(prim->w[10], (s16)r, (s16)g, (s16)b);
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[13];
                break;
            case GPU_POLY_GT3:
                prim->w[1] = btl_addRgbClamped(prim->w[1], (s16)r, (s16)g, (s16)b);
                prim->w[4] = btl_addRgbClamped(prim->w[4], (s16)r, (s16)g, (s16)b);
                prim->w[7] = btl_addRgbClamped(prim->w[7], (s16)r, (s16)g, (s16)b);
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[10];
                break;
            case GPU_POLY_G4:
                prim->w[1] = btl_addRgbClamped(prim->w[1], (s16)r, (s16)g, (s16)b);
                prim->w[3] = btl_addRgbClamped(prim->w[3], (s16)r, (s16)g, (s16)b);
                prim->w[5] = btl_addRgbClamped(prim->w[5], (s16)r, (s16)g, (s16)b);
                prim->w[7] = btl_addRgbClamped(prim->w[7], (s16)r, (s16)g, (s16)b);
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[9];
                break;
            case GPU_POLY_G3:
                prim->w[1] = btl_addRgbClamped(prim->w[1], (s16)r, (s16)g, (s16)b);
                prim->w[3] = btl_addRgbClamped(prim->w[3], (s16)r, (s16)g, (s16)b);
                prim->w[5] = btl_addRgbClamped(prim->w[5], (s16)r, (s16)g, (s16)b);
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[7];
                break;
            case GPU_POLY_F4:
                prim->w[1] = btl_addRgbClamped(prim->w[1], (s16)r, (s16)g, (s16)b);
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[6];
                break;
            case GPU_POLY_F3:
                prim->w[1] = btl_addRgbClamped(prim->w[1], (s16)r, (s16)g, (s16)b);
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[5];
                break;
            }
        } while (code != BTL_PRIM_MESH_END);
        }
    } while (++i < model->header->meshCount);
    }
    }
}

void btl_fillModelRgb(BtlModel *model, s32 r, s32 g, s32 b) {
    u32 i;
    BtlPrim *prim;
    u32 col;
    u8 code;

    i = 0;
    if (!(model->flags & BTL_MODEL_OFFSCREEN)) {
    col = (b << 16) | ((g << 16) >> 8) | (s16)r;
    prim = (BtlPrim *)model->prims;
    if (model->header->meshCount != 0) {
    do {
        if (!(model->hiddenMeshes & (1 << i))) {
        do {
            code = prim->tag.code;
            if (code == BTL_PRIM_MESH_END || code == GPU_DRAW_MODE) {
                prim = (BtlPrim *)&prim->w[2];
                continue;
            }
            switch (code & GPU_POLY_KIND) {
            case GPU_POLY_FT4:
                prim->w[1] = col;
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[10];
                break;
            case GPU_POLY_FT3:
                prim->w[1] = col;
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[8];
                break;
            case GPU_POLY_GT4:
                prim->w[1] = col;
                prim->w[4] = col;
                prim->w[7] = col;
                prim->w[10] = col;
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[13];
                break;
            case GPU_POLY_GT3:
                prim->w[1] = col;
                prim->w[4] = col;
                prim->w[7] = col;
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[10];
                break;
            case GPU_POLY_G4:
                prim->w[1] = col;
                prim->w[3] = col;
                prim->w[5] = col;
                prim->w[7] = col;
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[9];
                break;
            case GPU_POLY_G3:
                prim->w[1] = col;
                prim->w[3] = col;
                prim->w[5] = col;
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[7];
                break;
            case GPU_POLY_F4:
                prim->w[1] = col;
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[6];
                break;
            case GPU_POLY_F3:
                prim->w[1] = col;
                prim->tag.code = code;
                prim = (BtlPrim *)&prim->w[5];
                break;
            }
        } while (code != BTL_PRIM_MESH_END);
        }
    } while (++i < model->header->meshCount);
    }
    }
}
