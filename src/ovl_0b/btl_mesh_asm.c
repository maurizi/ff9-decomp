#include "ovl_0b.h"

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/btl_mesh_asm", btl_drawMeshOffset);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/btl_mesh_asm", btlfx_projectSliceMesh);

u8 *btlfx_drawSliceMesh(BtlModel *model, u8 mesh, u8 *prim) {
    s32 opz;
    s32 otz;
    BtlMesh *meshDef;
    u8 (*uv)[2];
    u8 (*col)[4];
    u32 *ot;
    u32 *tex;
    u32 primCount;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 idx01, idx23;
    u32 *cp;
    u32 i1, i3;
    u32 texInfo;
    u32 hi;
    u8 *src;
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
    meshDef = &model->header->meshes[mesh];
    src = (u8 *)meshDef->prims;
    uv = meshDef->uvs;
    col = meshDef->colors;
    if (model->prims == 0) {
        model->prims = (s32)prim;
    }
    tex = (u32 *)model->texPages;
    ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf];

    primCount = meshDef->nFT4;
    for (i = 0; i < primCount; src += sizeof(WmSrcFT4)) {
        sFT4 = (WmSrcFT4 *)src;
        pFT4 = (POLY_FT4 *)prim;
        idx01 = *(u32 *)&sFT4->vtx[0];
        idx23 = *(u32 *)&sFT4->vtx[2];
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
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(u32 *)&pFT4->r0 = *(u32 *)sFT4->rgb;
        setlen(pFT4, 9);
        *(s32 *)&pFT4->x0 = xy0;
        *(s32 *)&pFT4->x1 = xy1;
        *(s32 *)&pFT4->x2 = xy2;
        setcode(pFT4, GPU_POLY_FT4);
        *(s32 *)&pFT4->x3 = g_vtxScreenXY[i3];
        idx01 = *(u32 *)&sFT4->uv[0];
        idx23 = *(u32 *)&sFT4->uv[2];
        texInfo = tex[sFT4->tex];
        hi = texInfo & 0xFFFF0000;
        texInfo <<= 16;
        *(u32 *)&pFT4->u0 = *(u16 *)uv[idx01 & 0xFFFF] | hi;
        *(u32 *)&pFT4->u1 = *(u16 *)uv[idx01 >> 16] | texInfo;
        *(u16 *)&pFT4->u2 = *(u16 *)uv[idx23 & 0xFFFF];
        *(u16 *)&pFT4->u3 = *(u16 *)uv[idx23 >> 16];
        if (sFT4->flags & WMSRC_SEMITRANS) {
            pFT4->tpage |= sFT4->flags & WMSRC_TPAGE_BITS;
            setSemiTrans(pFT4, 1);
        }
        otz &= 0xFFFFF;
        addPrim(&ot[otz >> 6], pFT4);
        prim += sizeof(POLY_FT4);
    }

    primCount = meshDef->nFT3;
    for (i = 0; i < primCount; src += sizeof(WmSrcFT3)) {
        sFT3 = (WmSrcFT3 *)src;
        pFT3 = (POLY_FT3 *)prim;
        idx01 = *(u32 *)&sFT3->vtx[0];
        idx23 = sFT3->vtx[2];
        i1 = idx01 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[idx23 & 0xFFFF];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sFT3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        *(u32 *)&pFT3->r0 = *(u32 *)sFT3->rgb;
        setlen(pFT3, 7);
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        *(s32 *)&pFT3->x2 = xy2;
        setcode(pFT3, GPU_POLY_FT3);
        texInfo = tex[sFT3->tex];
        idx01 = *(u32 *)&sFT3->uv[0];
        hi = texInfo & 0xFFFF0000;
        texInfo <<= 16;
        *(u32 *)&pFT3->u0 = *(u16 *)uv[idx01 & 0xFFFF] | hi;
        *(u32 *)&pFT3->u1 = *(u16 *)uv[idx01 >> 16] | texInfo;
        *(u16 *)&pFT3->u2 = *(u16 *)uv[sFT3->uv[2]];
        otz /= 3;
        if (sFT3->flags & WMSRC_SEMITRANS) {
            pFT3->tpage |= sFT3->flags & WMSRC_TPAGE_BITS;
            setSemiTrans(pFT3, 1);
        }
        addPrim(&ot[otz >> 4], pFT3);
        prim += sizeof(POLY_FT3);
    }

    primCount = meshDef->nGT4;
    for (i = 0; i < primCount; src += sizeof(WmSrcGT4)) {
        sGT4 = (WmSrcGT4 *)src;
        pGT4 = (POLY_GT4 *)prim;
        idx01 = *(u32 *)&sGT4->vtx[0];
        idx23 = *(u32 *)&sGT4->vtx[2];
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
        idx01 = *(u32 *)&sGT4->col[0];
        i1 = *(u32 *)&sGT4->col[2];
        texInfo = tex[sGT4->tex];
        cp = (u32 *)col[idx01 & 0xFFFF];
        *(u32 *)&pGT4->r0 = *cp;
        cp = (u32 *)col[idx01 >> 16];
        *(u32 *)&pGT4->r1 = *cp;
        cp = (u32 *)col[i1 & 0xFFFF];
        *(u32 *)&pGT4->r2 = *cp;
        cp = (u32 *)col[i1 >> 16];
        *(u32 *)&pGT4->r3 = *cp;
        setlen(pGT4, 12);
        setcode(pGT4, GPU_POLY_GT4);
        idx01 = *(u32 *)&sGT4->uv[0];
        i1 = *(u32 *)&sGT4->uv[2];
        hi = texInfo & 0xFFFF0000;
        texInfo <<= 16;
        *(u32 *)&pGT4->u0 = *(u16 *)uv[idx01 & 0xFFFF] | hi;
        *(u32 *)&pGT4->u1 = *(u16 *)uv[idx01 >> 16] | texInfo;
        *(u16 *)&pGT4->u2 = *(u16 *)uv[i1 & 0xFFFF];
        *(u16 *)&pGT4->u3 = *(u16 *)uv[i1 >> 16];
        if (sGT4->flags & WMSRC_SEMITRANS) {
            pGT4->tpage |= sGT4->flags & WMSRC_TPAGE_BITS;
            setSemiTrans(pGT4, 1);
        }
        otz &= 0xFFFFF;
        addPrim(&ot[otz >> 6], pGT4);
        prim += sizeof(POLY_GT4);
    }

    primCount = meshDef->nGT3;
    for (i = 0; i < primCount; src += sizeof(WmSrcGT3)) {
        sGT3 = (WmSrcGT3 *)src;
        pGT3 = (POLY_GT3 *)prim;
        idx01 = *(u32 *)&sGT3->vtx[0];
        idx23 = sGT3->vtx[2];
        i1 = idx01 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[idx23 & 0xFFFF];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sGT3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        *(s32 *)&pGT3->x0 = xy0;
        *(s32 *)&pGT3->x1 = xy1;
        *(s32 *)&pGT3->x2 = xy2;
        idx01 = *(u32 *)&sGT3->col[0];
        texInfo = tex[sGT3->tex];
        cp = (u32 *)col[idx01 & 0xFFFF];
        *(u32 *)&pGT3->r0 = *cp;
        cp = (u32 *)col[idx01 >> 16];
        *(u32 *)&pGT3->r1 = *cp;
        cp = (u32 *)col[sGT3->col[2]];
        *(u32 *)&pGT3->r2 = *cp;
        setlen(pGT3, 9);
        setcode(pGT3, GPU_POLY_GT3);
        hi = texInfo & 0xFFFF0000;
        texInfo <<= 16;
        *(u32 *)&pGT3->u0 = *(u16 *)uv[sGT3->uv[0]] | hi;
        idx01 = *(u32 *)&sGT3->uv[1];
        *(u32 *)&pGT3->u1 = *(u16 *)uv[idx01 & 0xFFFF] | texInfo;
        *(u16 *)&pGT3->u2 = *(u16 *)uv[idx01 >> 16];
        otz /= 3;
        if (sGT3->flags & WMSRC_SEMITRANS) {
            pGT3->tpage |= sGT3->flags & WMSRC_TPAGE_BITS;
            setSemiTrans(pGT3, 1);
        }
        addPrim(&ot[otz >> 4], pGT3);
        prim += sizeof(POLY_GT3);
    }

    primCount = meshDef->nG4;
    for (i = 0; i < primCount; src += sizeof(WmSrcG4)) {
        sG4 = (WmSrcG4 *)src;
        pG4 = (POLY_G4 *)prim;
        idx01 = *(u32 *)&sG4->vtx[0];
        idx23 = *(u32 *)&sG4->vtx[2];
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
        otz &= 0xFFFFF;
        idx01 = *(u32 *)&sG4->col[0];
        i1 = *(u32 *)&sG4->col[2];
        cp = (u32 *)col[idx01 & 0xFFFF];
        *(u32 *)&pG4->r0 = *cp;
        cp = (u32 *)col[idx01 >> 16];
        *(u32 *)&pG4->r1 = *cp;
        cp = (u32 *)col[i1 & 0xFFFF];
        *(u32 *)&pG4->r2 = *cp;
        cp = (u32 *)col[i1 >> 16];
        *(u32 *)&pG4->r3 = *cp;
        setlen(pG4, 8);
        setcode(pG4, GPU_POLY_G4);
        addPrim(&ot[otz >> 6], pG4);
        prim += sizeof(POLY_G4);
    }

    primCount = meshDef->nG3;
    for (i = 0; i < primCount; src += sizeof(WmSrcG3)) {
        sG3 = (WmSrcG3 *)src;
        pG3 = (POLY_G3 *)prim;
        idx01 = *(u32 *)&sG3->vtx[0];
        idx23 = sG3->vtx[2];
        i1 = idx01 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[idx23 & 0xFFFF];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sG3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        *(s32 *)&pG3->x2 = xy2;
        idx01 = *(u32 *)&sG3->col[0];
        cp = (u32 *)col[idx01 & 0xFFFF];
        *(u32 *)&pG3->r0 = *cp;
        cp = (u32 *)col[idx01 >> 16];
        *(u32 *)&pG3->r1 = *cp;
        cp = (u32 *)col[sG3->col[2]];
        *(u32 *)&pG3->r2 = *cp;
        setlen(pG3, 6);
        setcode(pG3, GPU_POLY_G3);
        otz /= 3;
        addPrim(&ot[otz >> 4], pG3);
        prim += sizeof(POLY_G3);
    }

    primCount = meshDef->nF4;
    for (i = 0; i < primCount; src += sizeof(WmSrcF4)) {
        sF4 = (WmSrcF4 *)src;
        pF4 = (POLY_F4 *)prim;
        idx01 = *(u32 *)&sF4->vtx[0];
        idx23 = *(u32 *)&sF4->vtx[2];
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
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(u32 *)&pF4->r0 = *(u32 *)sF4->rgb;
        setlen(pF4, 5);
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        setcode(pF4, GPU_POLY_F4);
        otz &= 0xFFFFF;
        *(s32 *)&pF4->x3 = g_vtxScreenXY[i3];
        addPrim(&ot[otz >> 6], pF4);
        prim += sizeof(POLY_F4);
    }

    primCount = meshDef->nF3;
    for (i = 0; i < primCount; src += sizeof(WmSrcF3)) {
        sF3 = (WmSrcF3 *)src;
        pF3 = (POLY_F3 *)prim;
        idx01 = *(u32 *)&sF3->vtx[0];
        idx23 = sF3->vtx[2];
        i1 = idx01 >> 16;
        xy0 = g_vtxScreenXY[idx01 & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[idx23];
        otz = g_vtxScreenZ[idx01 & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[idx23 & 0xFFFF];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sF3->flags & WMSRC_DOUBLE_SIDED)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        *(u32 *)&pF3->r0 = *(u32 *)sF3->rgb;
        setlen(pF3, 4);
        setcode(pF3, GPU_POLY_F3);
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        *(s32 *)&pF3->x2 = xy2;
        otz /= 3;
        addPrim(&ot[otz >> 4], pF3);
        prim += sizeof(POLY_F3);
    }
    setcode((P_TAG *)prim, 0xFF);
    return prim + sizeof(P_TAG);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/btl_mesh_asm", btlfx_projectMorphMesh);
