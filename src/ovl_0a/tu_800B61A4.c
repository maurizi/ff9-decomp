#include "ovl_0a.h"

void fldmdl_projectMesh(ModelObj *arg0, u8 arg1) {
    MATRIX m;
    WmModelGroup *g;
    MATRIX *bone;
    SVECTOR *vtx;
    s32 *sxy;
    s32 *sz;
    s32 k;
    u32 i;
    s32 n;
    u32 cnt;
    MATRIX *b;
    SVECTOR *v;
    s16 *c1;
    s16 *c2;

    g = &arg0->model->groups[arg1];
    gte_SetGeomScreen(g_gameState->gameData->projDist);
    sxy = g_vtxScreenXY;
    sz = g_vtxScreenZ;
    k = 0;
    cnt = arg0->model->boneCount;
    bone = (MATRIX *)arg0->boneMatrices;
    for (i = 0; i < cnt; bone++) {
        n = ((u16 *)g->boneVertexCounts)[i++];
        if (n != 0) {
            gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
            gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
            b = bone;
            gte_ldclmv(b);
            c1 = &b->m[0][1];
            c2 = &b->m[0][2];
            gte_rtir_b();
            sxy = &g_vtxScreenXY[k];
            gte_stclmv(&m);
            gte_ldclmv(c1);
            gte_rtir();
            sz = &g_vtxScreenZ[k];
            gte_stclmv(&m.m[0][1]);
            gte_ldclmv(c2);
            c1 = (s16 *)b->t;
            gte_rtir_n1();
            vtx = &((SVECTOR *)g->vertices)[k];
            gte_stclmv(&m.m[0][2]);
            gte_ldlv0(c1);
            k += n;
            gte_rtv0tr_n1();
            gte_ldv3c(vtx);
            gte_stlvnl(m.t);
            gte_SetRotMatrix(&m);
            gte_SetTransMatrix(&m);
            v = vtx;
            do {
                v += 3;
                n -= 3;
                gte_rtpt_b();
                gte_ldv3c(v);
                gte_stsxy3c(sxy);
                gte_stsz3c(sz);
                sxy += 3;
                sz += 3;
            } while (n > 0);
        }
    }
}

/* attach a model file (relocating it once) and lay out its work buffers at arg1 */
void fldmdl_initModel(ModelObj *arg0, u8 *arg1) {
    WmModel *m;
    WmModelGroup *g;
    u32 i;
    u32 n;
    u32 k;
    s32 size;
    u8 *pp;
    WmModelGroup *p;
    WmModelGroup *g2;

    m = arg0->model;
    if (!(m->flags & 1)) {
        i = 0;
        m->flags |= 1;
        m->bones = (s32)m + m->bones;
        g = (WmModelGroup *)((u8 *)m + (s32)m->groups);
        m->groups = g;
        for (p = g; i < m->groupCount; i++, p++) {
            p->boneVertexCounts = (u8 *)m + (s32)p->boneVertexCounts;
            p->vertices = (u8 *)m + (s32)p->vertices;
            p->prims = (u8 *)m + (s32)p->prims;
            p->uvs = (void *)((u8 *)m + (s32)p->uvs);
            p->colors = (void *)((u8 *)m + (s32)p->colors);
        }
    }
    g2 = arg0->model->groups;
    size = 0;
    if (IS_OFFSET(g2)) {
        g2 = (WmModelGroup *)((u8 *)g2 + (s32)arg0->model);
    }
    n = arg0->model->groupCount;
    for (k = 0; k < n; k++) {
        size += g2->packetSize;
        g2++;
    }
    arg0->scaleX = 0x1000;
    arg0->scaleY = 0x1000;
    arg0->scaleZ = 0x1000;
    arg0->parent = NULL;
    arg0->mirrorSrc = NULL;
    arg0->unk5C = 0;
    arg0->hiddenGroups = 0;
    arg0->attachX = 0;
    arg0->attachY = 0;
    arg0->attachZ = 0;
    arg0->packets = arg1;
    arg0->texAnims = NULL;
    pp = arg1 + size * 2;
    arg0->boneMatrices = pp;
    arg0->flags |= MDL_SCALED;
    *(volatile u16 *)&arg0->flags = 0;
    arg0->groupOut = (WmModelOut *)(pp + m->boneCount * 32);
}

void fldmdl_buildPackets(ModelObj *arg0, WmTexInfo *arg1, s16 (*arg2)[2]) {
    WmModel *m;
    WmModelGroup *g;
    WmModelOut *out;
    WmModelOut *o;
    u8 *prim;
    u8 *src;
    u8 (*col)[4];
    u8 *c;
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

    prim = arg0->packets;
    m = arg0->model;
    arg0->texInfo = arg1;
    o = arg0->groupOut;
    for (k = 0; k < m->groupCount; k++) {
        out = &o[k];
        g = &m->groups[k];
        col = g->colors;
        out->prims = prim;
        out->unk0 = 0;
        out->unk8 = 0;
        for (j = 0; j < 2; j++) {
            src = g->prims;
            pFT4 = (POLY_FT4 *)prim;
            sFT4 = (WmSrcFT4 *)src;
            for (i = 0; i < g->ft4Count; i++) {
                setPolyFT4(pFT4);
                pFT4->r0 = sFT4->rgb[0];
                pFT4->g0 = sFT4->rgb[1];
                pFT4->b0 = sFT4->rgb[2];
                pFT4->u0 = g->uvs[sFT4->uvs[0]][0];
                pFT4->v0 = g->uvs[sFT4->uvs[0]][1];
                pFT4->u1 = g->uvs[sFT4->uvs[1]][0];
                pFT4->v1 = g->uvs[sFT4->uvs[1]][1];
                pFT4->u2 = g->uvs[sFT4->uvs[2]][0];
                pFT4->v2 = g->uvs[sFT4->uvs[2]][1];
                pFT4->u3 = g->uvs[sFT4->uvs[3]][0];
                pFT4->v3 = g->uvs[sFT4->uvs[3]][1];
                pFT4->clut = arg0->texInfo[sFT4->texture].clut;
                pFT4->tpage = tp = arg0->texInfo[sFT4->texture].tpage & ~0x20;
                if (sFT4->flags & 1) {
                    pFT4->tpage = tp | (sFT4->flags & 0xE0);
                    setSemiTrans(pFT4, 1);
                }
                prim += sizeof(POLY_FT4);
                du = arg2[sFT4->texture][0];
                dv = arg2[sFT4->texture][1];
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
            for (i = 0; i < g->ft3Count; i++) {
                setPolyFT3(pFT3);
                pFT3->r0 = sFT3->rgb[0];
                pFT3->g0 = sFT3->rgb[1];
                pFT3->b0 = sFT3->rgb[2];
                pFT3->u0 = g->uvs[sFT3->uvs[0]][0];
                pFT3->v0 = g->uvs[sFT3->uvs[0]][1];
                pFT3->u1 = g->uvs[sFT3->uvs[1]][0];
                pFT3->v1 = g->uvs[sFT3->uvs[1]][1];
                pFT3->u2 = g->uvs[sFT3->uvs[2]][0];
                pFT3->v2 = g->uvs[sFT3->uvs[2]][1];
                pFT3->clut = arg0->texInfo[sFT3->texture].clut;
                pFT3->tpage = tp = arg0->texInfo[sFT3->texture].tpage & ~0x20;
                if (sFT3->flags & 1) {
                    pFT3->tpage = tp | (sFT3->flags & 0xE0);
                    setSemiTrans(pFT3, 1);
                }
                prim += sizeof(POLY_FT3);
                du = arg2[sFT3->texture][0];
                dv = arg2[sFT3->texture][1];
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
            for (i = 0; i < g->gt4Count; i++) {
                setPolyGT4(pGT4);
                c = col[sGT4->colors[0]];
                pGT4->r0 = c[0];
                pGT4->g0 = c[1];
                pGT4->b0 = c[2];
                c = col[sGT4->colors[1]];
                pGT4->r1 = c[0];
                pGT4->g1 = c[1];
                pGT4->b1 = c[2];
                c = col[sGT4->colors[2]];
                pGT4->r2 = c[0];
                pGT4->g2 = c[1];
                pGT4->b2 = c[2];
                c = col[sGT4->colors[3]];
                pGT4->r3 = c[0];
                pGT4->g3 = c[1];
                pGT4->b3 = c[2];
                pGT4->u0 = g->uvs[sGT4->uvs[0]][0];
                pGT4->v0 = g->uvs[sGT4->uvs[0]][1];
                pGT4->u1 = g->uvs[sGT4->uvs[1]][0];
                pGT4->v1 = g->uvs[sGT4->uvs[1]][1];
                pGT4->u2 = g->uvs[sGT4->uvs[2]][0];
                pGT4->v2 = g->uvs[sGT4->uvs[2]][1];
                pGT4->u3 = g->uvs[sGT4->uvs[3]][0];
                pGT4->v3 = g->uvs[sGT4->uvs[3]][1];
                pGT4->clut = arg0->texInfo[sGT4->texture].clut;
                pGT4->tpage = tp = arg0->texInfo[sGT4->texture].tpage & ~0x20;
                if (sGT4->flags & 1) {
                    pGT4->tpage = tp | (sGT4->flags & 0xE0);
                    setSemiTrans(pGT4, 1);
                }
                prim += sizeof(POLY_GT4);
                du = arg2[sGT4->texture][0];
                dv = arg2[sGT4->texture][1];
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
            for (i = 0; i < g->gt3Count; i++) {
                setPolyGT3(pGT3);
                c = col[sGT3->colors[0]];
                pGT3->r0 = c[0];
                pGT3->g0 = c[1];
                pGT3->b0 = c[2];
                c = col[sGT3->colors[1]];
                pGT3->r1 = c[0];
                pGT3->g1 = c[1];
                pGT3->b1 = c[2];
                c = col[sGT3->colors[2]];
                pGT3->r2 = c[0];
                pGT3->g2 = c[1];
                pGT3->b2 = c[2];
                pGT3->u0 = g->uvs[sGT3->uvs[0]][0];
                pGT3->v0 = g->uvs[sGT3->uvs[0]][1];
                pGT3->u1 = g->uvs[sGT3->uvs[1]][0];
                pGT3->v1 = g->uvs[sGT3->uvs[1]][1];
                pGT3->u2 = g->uvs[sGT3->uvs[2]][0];
                pGT3->v2 = g->uvs[sGT3->uvs[2]][1];
                pGT3->clut = arg0->texInfo[sGT3->texture].clut;
                pGT3->tpage = tp = arg0->texInfo[sGT3->texture].tpage & ~0x20;
                if (sGT3->flags & 1) {
                    pGT3->tpage = tp | (sGT3->flags & 0xE0);
                    setSemiTrans(pGT3, 1);
                }
                prim += sizeof(POLY_GT3);
                du = arg2[sGT3->texture][0];
                dv = arg2[sGT3->texture][1];
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
            for (i = 0; i < g->g4Count; i++) {
                setPolyG4(pG4);
                c = col[sG4->colors[0]];
                pG4->r0 = c[0];
                pG4->g0 = c[1];
                pG4->b0 = c[2];
                c = col[sG4->colors[1]];
                pG4->r1 = c[0];
                pG4->g1 = c[1];
                pG4->b1 = c[2];
                c = col[sG4->colors[2]];
                pG4->r2 = c[0];
                pG4->g2 = c[1];
                pG4->b2 = c[2];
                c = col[sG4->colors[3]];
                pG4->r3 = c[0];
                pG4->g3 = c[1];
                pG4->b3 = c[2];
                prim += sizeof(POLY_G4);
                pG4++;
                sG4++;
                src += sizeof(WmSrcG4);
            }
            pG3 = (POLY_G3 *)prim;
            sG3 = (WmSrcG3 *)src;
            for (i = 0; i < g->g3Count; i++) {
                setPolyG3(pG3);
                c = col[sG3->colors[0]];
                pG3->r0 = c[0];
                pG3->g0 = c[1];
                pG3->b0 = c[2];
                c = col[sG3->colors[1]];
                pG3->r1 = c[0];
                pG3->g1 = c[1];
                pG3->b1 = c[2];
                c = col[sG3->colors[2]];
                pG3->r2 = c[0];
                pG3->g2 = c[1];
                pG3->b2 = c[2];
                prim += sizeof(POLY_G3);
                pG3++;
                sG3++;
                src += sizeof(WmSrcG3);
            }
            pF4 = (POLY_F4 *)prim;
            sF4 = (WmSrcF4 *)src;
            for (i = 0; i < g->f4Count; i++) {
                setPolyF4(pF4);
                pF4->r0 = sF4->rgb[0];
                pF4->g0 = sF4->rgb[1];
                pF4->b0 = sF4->rgb[2];
                prim += sizeof(POLY_F4);
                pF4++;
                sF4++;
                src += sizeof(WmSrcF4);
            }
            for (i = 0; i < g->f3Count; i++) {
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

void fldmdl_drawMeshC(ModelObj *arg0, u8 arg1) {
    WmModelGroup *g;
    u32 *ot;
    s32 opz;
    s32 otz;
    u32 n;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 a, b;
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
    g = &arg0->model->groups[arg1];
    if (g_gameState->gameData->drawBuf != 0) {
        prim = (u8 *)arg0->groupOut[arg1].prims + g->packetSize;
    } else {
        prim = arg0->groupOut[arg1].prims;
    }

    n = g->ft4Count;
    pFT4 = (POLY_FT4 *)prim;
    ot -= g->otBias;
    sFT4 = (WmSrcFT4 *)(void *)g->prims;
    for (i = 0; i < n; sFT4++, pFT4++) {
        a = *(u32 *)&sFT4->vertices[0];
        b = *(u32 *)&sFT4->vertices[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT4->flags & 2)) {
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

    n = g->ft3Count;
    pFT3 = (POLY_FT3 *)pFT4;
    sFT3 = (WmSrcFT3 *)sFT4;
    for (i = 0; i < n; sFT3++, pFT3++) {
        a = *(u32 *)&sFT3->vertices[0];
        i2 = sFT3->vertices[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT3->flags & 2)) {
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

    n = g->gt4Count;
    pGT4 = (POLY_GT4 *)pFT3;
    sGT4 = (WmSrcGT4 *)sFT3;
    for (i = 0; i < n; sGT4++, pGT4++) {
        a = *(u32 *)&sGT4->vertices[0];
        b = *(u32 *)&sGT4->vertices[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT4->flags & 2)) {
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

    n = g->gt3Count;
    pGT3 = (POLY_GT3 *)pGT4;
    sGT3 = (WmSrcGT3 *)sGT4;
    for (i = 0; i < n; sGT3++, pGT3++) {
        a = *(u32 *)&sGT3->vertices[0];
        i2 = sGT3->vertices[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT3->flags & 2)) {
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

    n = g->g4Count;
    pG4 = (POLY_G4 *)pGT3;
    sG4 = (WmSrcG4 *)sGT3;
    for (i = 0; i < n; sG4++, pG4++) {
        a = *(u32 *)&sG4->vertices[0];
        b = *(u32 *)&sG4->vertices[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG4->flags & 2)) {
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

    n = g->g3Count;
    pG3 = (POLY_G3 *)pG4;
    sG3 = (WmSrcG3 *)sG4;
    for (i = 0; i < n; sG3++, pG3++) {
        a = *(u32 *)&sG3->vertices[0];
        i2 = sG3->vertices[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG3->flags & 2)) {
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

    n = g->f4Count;
    pF4 = (POLY_F4 *)pG3;
    sF4 = (WmSrcF4 *)sG3;
    for (i = 0; i < n; sF4++, pF4++) {
        a = *(u32 *)&sF4->vertices[0];
        b = *(u32 *)&sF4->vertices[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF4->flags & 2)) {
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

    n = g->f3Count;
    pF3 = (POLY_F3 *)pF4;
    sF3 = (WmSrcF3 *)sF4;
    for (i = 0; i < n; sF3++, pF3++) {
        a = *(u32 *)&sF3->vertices[0];
        i2 = sF3->vertices[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF3->flags & 2)) {
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

void fldmdl_applyScale(ModelObj *arg0) {
    MATRIX m;

    if (arg0->flags & MDL_SCALED) {
        *(s32 *)&m.m[0][0] = 0x1000;
        *(s32 *)&m.m[0][2] = 0;
        *(s32 *)&m.m[1][1] = 0x1000;
        *(s32 *)&m.m[2][0] = 0;
        *(s32 *)&m.m[2][2] = 0x1000;
        m.m[0][0] = arg0->scaleX;
        gte_SetRotMatrix(&arg0->worldMatrix);
        gte_ldclmv(&m);
        gte_rtir();
        m.m[1][1] = arg0->scaleY;
        gte_stclmv(&arg0->worldMatrix);
        gte_ldclmv(&m.m[0][1]);
        gte_rtir();
        m.m[2][2] = arg0->scaleZ;
        gte_stclmv(&arg0->worldMatrix.m[0][1]);
        gte_ldclmv(&m.m[0][2]);
        gte_rtir();
        gte_stclmv(&arg0->worldMatrix.m[0][2]);
    }
}

void fldmdl_cullBehindCamera(ModelObj *arg0) {
    SVECTOR v;
    s32 sz;

    arg0->flags &= ~MDL_BEHIND_CAMERA;
    if (arg0->parent != NULL) {
        arg0->flags |= arg0->parent->flags & MDL_BEHIND_CAMERA;
    } else {
        v.vx = arg0->worldMatrix.t[0];
        v.vy = arg0->worldMatrix.t[1];
        v.vz = arg0->worldMatrix.t[2];
        gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
        gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
        gte_ldv0(&v);
        gte_rtps();
        gte_stsz(&sz);
        if (sz == 0) {
            arg0->flags |= MDL_BEHIND_CAMERA;
        }
    }
}

void fldmdl_calcBoneMatrices(ModelObj *arg0, u16 arg1) {
    SVECTOR sv;
    u16 nb;
    VECTOR *vec;
    WmAnim *anim;
    MATRIX *bones;
    MATRIX *m;
    MATRIX *end;
    MATRIX *p;
    WmAnimKey *key;
    WmAnimKey *key2;
    WmBone *b;
    SVECTOR *rot;
    s16 *c1;
    s16 *c2;
    s32 w0, w1, w2, w3;

    anim = (WmAnim *)arg0->anim;
    if (anim == NULL) {
        if (arg0->parent != NULL) {
            /* attached to a bone of the parent model */
            w0 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[0];
            w1 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[1];
            w2 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[2];
            w3 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[3];
            ((s32 *)arg0->boneMatrices)[0] = w0;
            ((s32 *)arg0->boneMatrices)[1] = w1;
            ((s32 *)arg0->boneMatrices)[2] = w2;
            ((s32 *)arg0->boneMatrices)[3] = w3;
            w0 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[4];
            w1 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[5];
            w2 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[6];
            w3 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[7];
            ((s32 *)arg0->boneMatrices)[4] = w0;
            ((s32 *)arg0->boneMatrices)[5] = w1;
            ((s32 *)arg0->boneMatrices)[6] = w2;
            ((s32 *)arg0->boneMatrices)[7] = w3;
            gte_SetRotMatrix(arg0->boneMatrices);
            gte_ldv0(&arg0->attachX);
            gte_rtv0();
            gte_stsv(&sv);
            ((MATRIX *)arg0->boneMatrices)->t[0] += sv.vx;
            ((MATRIX *)arg0->boneMatrices)->t[1] += sv.vy;
            ((MATRIX *)arg0->boneMatrices)->t[2] += sv.vz;
        } else {
            ((MATRIX *)arg0->boneMatrices)->m[0][0] = arg0->worldMatrix.m[0][0];
            ((MATRIX *)arg0->boneMatrices)->m[0][1] = -arg0->worldMatrix.m[0][1];
            ((MATRIX *)arg0->boneMatrices)->m[0][2] = -arg0->worldMatrix.m[0][2];
            ((MATRIX *)arg0->boneMatrices)->m[1][0] = arg0->worldMatrix.m[1][0];
            ((MATRIX *)arg0->boneMatrices)->m[1][1] = -arg0->worldMatrix.m[1][1];
            ((MATRIX *)arg0->boneMatrices)->m[1][2] = -arg0->worldMatrix.m[1][2];
            ((MATRIX *)arg0->boneMatrices)->m[2][0] = arg0->worldMatrix.m[2][0];
            ((MATRIX *)arg0->boneMatrices)->m[2][1] = -arg0->worldMatrix.m[2][1];
            ((MATRIX *)arg0->boneMatrices)->m[2][2] = -arg0->worldMatrix.m[2][2];
            ((MATRIX *)arg0->boneMatrices)->t[0] = arg0->worldMatrix.t[0];
            ((MATRIX *)arg0->boneMatrices)->t[1] = arg0->worldMatrix.t[1];
            ((MATRIX *)arg0->boneMatrices)->t[2] = arg0->worldMatrix.t[2];
        }
        return;
    }
    nb = arg0->model->boneCount;
    bones = (MATRIX *)0x1F800018;
    if (nb >= 31) {
        bones = (MATRIX *)arg0->boneMatrices;
    }
    m = bones;
    vec = (VECTOR *)0x1F800008;
    vec->vx = 0;
    vec->vy = 0;
    key = anim->rotKeys;
    key2 = anim->rotKeysLo;
    rot = (SVECTOR *)0x1F800000;
    if (anim->constMask & 1) {
        bones->t[0] = anim->rootPos[0];
    } else {
        bones->t[0] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[0]))[arg1];
    }
    if (anim->constMask & 2) {
        m->t[1] = anim->rootPos[1];
    } else {
        m->t[1] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[1]))[arg1];
    }
    if (anim->constMask & 4) {
        m->t[2] = anim->rootPos[2];
    } else {
        m->t[2] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[2]))[arg1];
    }
    end = &bones[nb];
    do {
        gte_ldrotkey(key, anim, arg1, rot);
        if (key2 != NULL) {
            gte_ldrotkey_lo(key2, anim, arg1, rot);
            key2++;
        }
        key++;
        math_rotMatrixZYX_gte(rot, m);
        m++;
    } while (m < end);
    if (arg0->parent != NULL) {
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[0];
            x1 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[1];
            x2 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[2];
            x3 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[3];
            ((s32 *)bones)[0] = x0;
            ((s32 *)bones)[1] = x1;
            ((s32 *)bones)[2] = x2;
            ((s32 *)bones)[3] = x3;
        }
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[4];
            x1 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[5];
            x2 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[6];
            x3 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[7];
            ((s32 *)bones)[4] = x0;
            ((s32 *)bones)[5] = x1;
            ((s32 *)bones)[6] = x2;
            ((s32 *)bones)[7] = x3;
        }
        gte_SetRotMatrix(bones);
        gte_ldv0(&arg0->attachX);
        gte_rtv0();
        gte_stsv(&sv);
        bones->t[0] += sv.vx;
        bones->t[1] += sv.vy;
        bones->t[2] += sv.vz;
    } else {
        gte_SetRotMatrix(&arg0->worldMatrix);
        gte_SetTransMatrix(&arg0->worldMatrix);
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
    b = (WmBone *)arg0->model->bones + 1;
    m = bones + 1;
    p = &bones[b->parent];
    do {
        gte_SetRotMatrix(p);
        gte_ldclmv(m);
        gte_rtir();
        gte_SetTransMatrix(p);
        gte_stclmv(m);
        gte_ldclmv(&m->m[0][1]);
        gte_rtir();
        vec->vz = b->length;
        gte_stclmv(&m->m[0][1]);
        gte_ldclmv(&m->m[0][2]);
        gte_rtir();
        b++;
        gte_stclmv(&m->m[0][2]);
        gte_ldlv0(vec);
        gte_rtv0tr();
        p = &bones[b->parent];
        gte_stlvnl(m->t);
        m++;
    } while (m < end);
    if (nb < 31) {
        m = (MATRIX *)arg0->boneMatrices;
        do {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)bones)[0];
            x1 = ((s32 *)bones)[1];
            x2 = ((s32 *)bones)[2];
            x3 = ((s32 *)bones)[3];
            ((s32 *)m)[0] = x0;
            ((s32 *)m)[1] = x1;
            ((s32 *)m)[2] = x2;
            ((s32 *)m)[3] = x3;
            x0 = ((s32 *)bones)[4];
            x1 = ((s32 *)bones)[5];
            x2 = ((s32 *)bones)[6];
            x3 = ((s32 *)bones)[7];
            ((s32 *)m)[4] = x0;
            ((s32 *)m)[5] = x1;
            ((s32 *)m)[6] = x2;
            ((s32 *)m)[7] = x3;
            m++;
            bones++;
        } while (bones < end);
    }
}

void fldmdl_addGroupRgb(ModelObj *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    WmModelGroup *g;
    u8 *p;
    u32 j;
    u32 k;

    if (arg0->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    g = &arg0->model->groups[arg1];
    for (k = 0; k < 2; k++) {
        p = arg0->groupOut[arg1].prims;
        if (k != 0) {
            p += g->packetSize;
        }
        for (j = 0; j < g->ft4Count; j++) {
            *(u32 *)&((POLY_FT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT4 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_FT4);
        }
        for (j = 0; j < g->ft3Count; j++) {
            *(u32 *)&((POLY_FT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT3 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_FT3);
        }
        for (j = 0; j < g->gt4Count; j++) {
            *(u32 *)&((POLY_GT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT4 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT4 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r2, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT4 *)p)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r3, arg2, arg3, arg4);
            p += sizeof(POLY_GT4);
        }
        for (j = 0; j < g->gt3Count; j++) {
            *(u32 *)&((POLY_GT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT3 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT3 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r2, arg2, arg3, arg4);
            p += sizeof(POLY_GT3);
        }
        for (j = 0; j < g->g4Count; j++) {
            *(u32 *)&((POLY_G4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_G4 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_G4 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r2, arg2, arg3, arg4);
            *(u32 *)&((POLY_G4 *)p)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r3, arg2, arg3, arg4);
            p += sizeof(POLY_G4);
        }
        for (j = 0; j < g->g3Count; j++) {
            *(u32 *)&((POLY_G3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_G3 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_G3 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r2, arg2, arg3, arg4);
            p += sizeof(POLY_G3);
        }
        for (j = 0; j < g->f4Count; j++) {
            *(u32 *)&((POLY_F4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F4 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_F4);
        }
        for (j = 0; j < g->f3Count; j++) {
            *(u32 *)&((POLY_F3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F3 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_F3);
        }
    }
}

void fldmdl_addGroupRgbCur(ModelObj *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    WmModelGroup *g;
    u8 *p;
    u32 j;

    if (arg0->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    g = &arg0->model->groups[arg1];
    p = arg0->groupOut[arg1].prims;
    if (g_gameState->gameData->drawBuf != 0) {
        p += g->packetSize;
    }
    for (j = 0; j < g->ft4Count; j++) {
        *(u32 *)&((POLY_FT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT4 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_FT4);
    }
    for (j = 0; j < g->ft3Count; j++) {
        *(u32 *)&((POLY_FT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT3 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_FT3);
    }
    for (j = 0; j < g->gt4Count; j++) {
        *(u32 *)&((POLY_GT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT4 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT4 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r2, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT4 *)p)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r3, arg2, arg3, arg4);
        p += sizeof(POLY_GT4);
    }
    for (j = 0; j < g->gt3Count; j++) {
        *(u32 *)&((POLY_GT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT3 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT3 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r2, arg2, arg3, arg4);
        p += sizeof(POLY_GT3);
    }
    for (j = 0; j < g->g4Count; j++) {
        *(u32 *)&((POLY_G4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_G4 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_G4 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r2, arg2, arg3, arg4);
        *(u32 *)&((POLY_G4 *)p)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r3, arg2, arg3, arg4);
        p += sizeof(POLY_G4);
    }
    for (j = 0; j < g->g3Count; j++) {
        *(u32 *)&((POLY_G3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_G3 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_G3 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r2, arg2, arg3, arg4);
        p += sizeof(POLY_G3);
    }
    for (j = 0; j < g->f4Count; j++) {
        *(u32 *)&((POLY_F4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F4 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_F4);
    }
    for (j = 0; j < g->f3Count; j++) {
        *(u32 *)&((POLY_F3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F3 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_F3);
    }
}

void fldmdl_tintGroup(ModelObj *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    u32 *col;
    WmModelGroup *g;
    u8 *p;
    u8 *s;
    u32 j;
    u32 k;
    u8 c;

    if (arg0->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    g = &arg0->model->groups[arg1];
    for (k = 0; k < 2; k++) {
        s = g->prims;
        p = arg0->groupOut[arg1].prims;
        if (k != 0) {
            p += g->packetSize;
        }
        for (j = 0; j < g->ft4Count; j++) {
            c = ((POLY_FT4 *)p)->code;
            *(u32 *)&((POLY_FT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT4 *)s)->rgb, arg2, arg3, arg4);
            ((POLY_FT4 *)p)->code = c;
            p += sizeof(POLY_FT4);
            s += sizeof(WmSrcFT4);
        }

        for (j = 0; j < g->ft3Count; j++) {
            c = ((POLY_FT3 *)p)->code;
            *(u32 *)&((POLY_FT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT3 *)s)->rgb, arg2, arg3, arg4);
            ((POLY_FT3 *)p)->code = c;
            p += sizeof(POLY_FT3);
            s += sizeof(WmSrcFT3);
        }

        for (j = 0; j < g->gt4Count; j++) {
            c = ((POLY_GT4 *)p)->code;
            col = (u32 *)g->colors[((WmSrcGT4 *)s)->colors[0]];
            *(u32 *)&((POLY_GT4 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcGT4 *)s)->colors[1]];
            *(u32 *)&((POLY_GT4 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcGT4 *)s)->colors[2]];
            *(u32 *)&((POLY_GT4 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcGT4 *)s)->colors[3]];
            *(u32 *)&((POLY_GT4 *)p)->r3 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_GT4 *)p)->code = c;
            p += sizeof(POLY_GT4);
            s += sizeof(WmSrcGT4);
        }

        for (j = 0; j < g->gt3Count; j++) {
            c = ((POLY_GT3 *)p)->code;
            col = (u32 *)g->colors[((WmSrcGT3 *)s)->colors[0]];
            *(u32 *)&((POLY_GT3 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcGT3 *)s)->colors[1]];
            *(u32 *)&((POLY_GT3 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcGT3 *)s)->colors[2]];
            *(u32 *)&((POLY_GT3 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_GT3 *)p)->code = c;
            p += sizeof(POLY_GT3);
            s += sizeof(WmSrcGT3);
        }

        for (j = 0; j < g->g4Count; j++) {
            c = ((POLY_G4 *)p)->code;
            col = (u32 *)g->colors[((WmSrcG4 *)s)->colors[0]];
            *(u32 *)&((POLY_G4 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcG4 *)s)->colors[1]];
            *(u32 *)&((POLY_G4 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcG4 *)s)->colors[2]];
            *(u32 *)&((POLY_G4 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcG4 *)s)->colors[3]];
            *(u32 *)&((POLY_G4 *)p)->r3 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_G4 *)p)->code = c;
            p += sizeof(POLY_G4);
            s += sizeof(WmSrcG4);
        }

        for (j = 0; j < g->g3Count; j++) {
            c = ((POLY_G3 *)p)->code;
            col = (u32 *)g->colors[((WmSrcG3 *)s)->colors[0]];
            *(u32 *)&((POLY_G3 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcG3 *)s)->colors[1]];
            *(u32 *)&((POLY_G3 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcG3 *)s)->colors[2]];
            *(u32 *)&((POLY_G3 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_G3 *)p)->code = c;
            p += sizeof(POLY_G3);
            s += sizeof(WmSrcG3);
        }

        for (j = 0; j < g->f4Count; j++) {
            c = ((POLY_F4 *)p)->code;
            *(u32 *)&((POLY_F4 *)p)->r0 |= fldmdl_addRgbClamped(*(u32 *)((WmSrcF4 *)s)->rgb, arg2, arg3, arg4);
            ((POLY_F4 *)p)->code = c;
            p += sizeof(POLY_F4);
            s += sizeof(WmSrcF4);
        }

        for (j = 0; j < g->f3Count; j++) {
            c = ((POLY_F3 *)p)->code;
            *(u32 *)&((POLY_F3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcF3 *)s)->rgb, arg2, arg3, arg4);
            ((POLY_F3 *)p)->code = c;
            p += sizeof(POLY_F3);
            s += sizeof(WmSrcF3);
        }
    }
}

/* recolour a model group in the current draw buffer (linked prims only) */
void fldmdl_tintGroupCur(ModelObj *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    u32 *col;
    WmModelGroup *g;
    u8 *p;
    u8 *s;
    u32 j;
    u8 c;

    if (arg0->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    g = &arg0->model->groups[arg1];
    p = arg0->groupOut[arg1].prims;
    s = g->prims;
    if (g_gameState->gameData->drawBuf != 0) {
        p += g->packetSize;
    }
    for (j = 0; j < g->ft4Count; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_FT4 *)p)->code;
            *(u32 *)&((POLY_FT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT4 *)s)->rgb, arg2, arg3, arg4);
            ((POLY_FT4 *)p)->code = c;
        }
        p += sizeof(POLY_FT4);
        s += sizeof(WmSrcFT4);
    }

    for (j = 0; j < g->ft3Count; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_FT3 *)p)->code;
            *(u32 *)&((POLY_FT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT3 *)s)->rgb, arg2, arg3, arg4);
            ((POLY_FT3 *)p)->code = c;
        }
        p += sizeof(POLY_FT3);
        s += sizeof(WmSrcFT3);
    }

    for (j = 0; j < g->gt4Count; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_GT4 *)p)->code;
            col = (u32 *)g->colors[((WmSrcGT4 *)s)->colors[0]];
            *(u32 *)&((POLY_GT4 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcGT4 *)s)->colors[1]];
            *(u32 *)&((POLY_GT4 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcGT4 *)s)->colors[2]];
            *(u32 *)&((POLY_GT4 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcGT4 *)s)->colors[3]];
            *(u32 *)&((POLY_GT4 *)p)->r3 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_GT4 *)p)->code = c;
        }
        p += sizeof(POLY_GT4);
        s += sizeof(WmSrcGT4);
    }

    for (j = 0; j < g->gt3Count; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_GT3 *)p)->code;
            col = (u32 *)g->colors[((WmSrcGT3 *)s)->colors[0]];
            *(u32 *)&((POLY_GT3 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcGT3 *)s)->colors[1]];
            *(u32 *)&((POLY_GT3 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcGT3 *)s)->colors[2]];
            *(u32 *)&((POLY_GT3 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_GT3 *)p)->code = c;
        }
        p += sizeof(POLY_GT3);
        s += sizeof(WmSrcGT3);
    }

    for (j = 0; j < g->g4Count; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_G4 *)p)->code;
            col = (u32 *)g->colors[((WmSrcG4 *)s)->colors[0]];
            *(u32 *)&((POLY_G4 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcG4 *)s)->colors[1]];
            *(u32 *)&((POLY_G4 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcG4 *)s)->colors[2]];
            *(u32 *)&((POLY_G4 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcG4 *)s)->colors[3]];
            *(u32 *)&((POLY_G4 *)p)->r3 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_G4 *)p)->code = c;
        }
        p += sizeof(POLY_G4);
        s += sizeof(WmSrcG4);
    }

    for (j = 0; j < g->g3Count; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_G3 *)p)->code;
            col = (u32 *)g->colors[((WmSrcG3 *)s)->colors[0]];
            *(u32 *)&((POLY_G3 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcG3 *)s)->colors[1]];
            *(u32 *)&((POLY_G3 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->colors[((WmSrcG3 *)s)->colors[2]];
            *(u32 *)&((POLY_G3 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_G3 *)p)->code = c;
        }
        p += sizeof(POLY_G3);
        s += sizeof(WmSrcG3);
    }

    for (j = 0; j < g->f4Count; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_F4 *)p)->code;
            *(u32 *)&((POLY_F4 *)p)->r0 |= fldmdl_addRgbClamped(*(u32 *)((WmSrcF4 *)s)->rgb, arg2, arg3, arg4);
            ((POLY_F4 *)p)->code = c;
        }
        p += sizeof(POLY_F4);
        s += sizeof(WmSrcF4);
    }

    for (j = 0; j < g->f3Count; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_F3 *)p)->code;
            *(u32 *)&((POLY_F3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcF3 *)s)->rgb, arg2, arg3, arg4);
            ((POLY_F3 *)p)->code = c;
        }
        p += sizeof(POLY_F3);
        s += sizeof(WmSrcF3);
    }
}

void fldmdl_setGroupRgb(ModelObj *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    WmModelGroup *g;
    u32 c;
    u8 *p;
    u32 j;

    if (arg0->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    g = &arg0->model->groups[arg1];
    c = (arg4 << 16) | (arg3 << 8) | arg2;
    if (g_gameState->gameData->drawBuf != 0) {
        p = arg0->groupOut[arg1].prims + g->packetSize;
    } else {
        p = arg0->groupOut[arg1].prims;
    }
    for (j = 0; j < g->ft4Count; j++) {
        *(u32 *)&((POLY_FT4 *)p)->r0 = c | (((POLY_FT4 *)p)->code << 24);
        p += sizeof(POLY_FT4);
    }
    for (j = 0; j < g->ft3Count; j++) {
        *(u32 *)&((POLY_FT3 *)p)->r0 = c | (((POLY_FT3 *)p)->code << 24);
        p += sizeof(POLY_FT3);
    }
    for (j = 0; j < g->gt4Count; j++) {
        *(u32 *)&((POLY_GT4 *)p)->r0 = c | (((POLY_GT4 *)p)->code << 24);
        p += sizeof(POLY_GT4);
    }
    for (j = 0; j < g->gt3Count; j++) {
        *(u32 *)&((POLY_GT3 *)p)->r0 = c | (((POLY_GT3 *)p)->code << 24);
        p += sizeof(POLY_GT3);
    }
    for (j = 0; j < g->g4Count; j++) {
        *(u32 *)&((POLY_G4 *)p)->r0 = c | (((POLY_G4 *)p)->code << 24);
        p += sizeof(POLY_G4);
    }
    for (j = 0; j < g->g3Count; j++) {
        *(u32 *)&((POLY_G3 *)p)->r0 = c | (((POLY_G3 *)p)->code << 24);
        p += sizeof(POLY_G3);
    }
    for (j = 0; j < g->f4Count; j++) {
        *(u32 *)&((POLY_F4 *)p)->r0 = c | (((POLY_F4 *)p)->code << 24);
        p += sizeof(POLY_F4);
    }
    for (j = 0; j < g->f3Count; j++) {
        *(u32 *)&((POLY_F3 *)p)->r0 = c | (((POLY_F3 *)p)->code << 24);
        p += sizeof(POLY_F3);
    }
}

u32 fldmdl_addRgbClamped(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 b = arg3 + ((arg0 >> 16) & 0xFF);
    s16 g = arg2 + ((arg0 >> 8) & 0xFF);
    s16 r = arg1 + (arg0 & 0xFF);
    arg0 &= 0xFF000000;
    if (r >= 0x100) {
        arg0 |= 0xFF;
    } else if (r > 0) {
        arg0 |= r;
    }
    if (g >= 0x100) {
        arg0 |= 0xFF00;
    } else if (g > 0) {
        arg0 |= g << 8;
    }
    if (b >= 0x100) {
        arg0 |= 0xFF0000;
    } else if (b > 0) {
        arg0 |= b << 16;
    }
    return arg0;
}

void fldmdl_setSemiTrans(ModelObj *arg0, u16 arg1) {
    u32 i;
    u32 j;
    u8 *p;
    WmModelGroup *g;
    u8 *s;

    if (arg0->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    for (i = 0; i < arg0->model->groupCount; i++) {
        g = &arg0->model->groups[i];
        s = g->prims;
        p = arg0->groupOut[i].prims;
        if (g_gameState->gameData->drawBuf != 0) {
            p += g->packetSize;
        }
        for (j = 0; j < g->ft4Count; j++) {
            if ((((WmSrcFT4 *)s)->flags & WMSRC_SEMITRANS) || arg1 != 0) {
                setSemiTrans((POLY_FT4 *)p, 1);
            } else {
                setSemiTrans((POLY_FT4 *)p, 0);
            }
            p += sizeof(POLY_FT4);
            s += sizeof(WmSrcFT4);
        }
        for (j = 0; j < g->ft3Count; j++) {
            if ((((WmSrcFT3 *)s)->flags & WMSRC_SEMITRANS) || arg1 != 0) {
                setSemiTrans((POLY_FT3 *)p, 1);
            } else {
                setSemiTrans((POLY_FT3 *)p, 0);
            }
            s += sizeof(WmSrcFT3);
            p += sizeof(POLY_FT3);
        }
        for (j = 0; j < g->gt4Count; j++) {
            if ((((WmSrcGT4 *)s)->flags & WMSRC_SEMITRANS) || arg1 != 0) {
                setSemiTrans((POLY_GT4 *)p, 1);
            } else {
                setSemiTrans((POLY_GT4 *)p, 0);
            }
            s += sizeof(WmSrcGT4);
            p += sizeof(POLY_GT4);
        }
        for (j = 0; j < g->gt3Count; j++) {
            if ((((WmSrcGT3 *)s)->flags & WMSRC_SEMITRANS) || arg1 != 0) {
                setSemiTrans((POLY_GT3 *)p, 1);
            } else {
                setSemiTrans((POLY_GT3 *)p, 0);
            }
            s += sizeof(WmSrcGT3);
            p += sizeof(POLY_GT3);
        }
    }
}

void fldmdl_setAbr(ModelObj *arg0, u16 arg1) {
    u32 i;
    u32 j;
    u8 *p;
    WmModelGroup *g;

    if (arg0->flags & MDL_BEHIND_CAMERA) {
        return;
    }
    for (i = 0; i < arg0->model->groupCount; i++) {
        g = &arg0->model->groups[i];
        p = arg0->groupOut[i].prims;
        if (g_gameState->gameData->drawBuf != 0) {
            p += g->packetSize;
        }
        for (j = 0; j < g->ft4Count; j++) {
            ((POLY_FT4 *)p)->tpage = arg1 | (((POLY_FT4 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_FT4);
        }
        for (j = 0; j < g->ft3Count; j++) {
            ((POLY_FT3 *)p)->tpage = arg1 | (((POLY_FT3 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_FT3);
        }
        for (j = 0; j < g->gt4Count; j++) {
            ((POLY_GT4 *)p)->tpage = arg1 | (((POLY_GT4 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_GT4);
        }
        for (j = 0; j < g->gt3Count; j++) {
            ((POLY_GT3 *)p)->tpage = arg1 | (((POLY_GT3 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_GT3);
        }
    }
}

void fldmdl_calcBoneMatricesLookAt(ModelObj *arg0, s32 arg1) {
    VECTOR v;
    VECTOR up;
    VECTOR r0;
    VECTOR r2;
    VECTOR r1;
    MATRIX s;
    u32 idx;
    u32 head;
    MATRIX *end;
    u16 nb;
    VECTOR *vec;
    VECTOR *up_p;
    WmAnim *anim;
    MATRIX *bones;
    MATRIX *m;
    MATRIX *p;
    WmAnimKey *key;
    WmAnimKey *key2;
    SVECTOR *rot;
    s16 *d1;
    s16 *d2;
    s32 w0, w1, w2, w3;

    anim = (WmAnim *)arg0->anim;
    if (anim == NULL) {
        if (arg0->parent != NULL) {
            /* attached to a bone of the parent model */
            w0 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[0];
            w1 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[1];
            w2 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[2];
            w3 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[3];
            ((s32 *)arg0->boneMatrices)[0] = w0;
            ((s32 *)arg0->boneMatrices)[1] = w1;
            ((s32 *)arg0->boneMatrices)[2] = w2;
            ((s32 *)arg0->boneMatrices)[3] = w3;
            w0 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[4];
            w1 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[5];
            w2 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[6];
            w3 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[7];
            ((s32 *)arg0->boneMatrices)[4] = w0;
            ((s32 *)arg0->boneMatrices)[5] = w1;
            ((s32 *)arg0->boneMatrices)[6] = w2;
            ((s32 *)arg0->boneMatrices)[7] = w3;
            ((MATRIX *)arg0->boneMatrices)->t[0] += arg0->attachX;
            ((MATRIX *)arg0->boneMatrices)->t[1] += arg0->attachY;
            ((MATRIX *)arg0->boneMatrices)->t[2] += arg0->attachZ;
        } else {
            ((MATRIX *)arg0->boneMatrices)->m[0][0] = arg0->worldMatrix.m[0][0];
            ((MATRIX *)arg0->boneMatrices)->m[0][1] = -arg0->worldMatrix.m[0][1];
            ((MATRIX *)arg0->boneMatrices)->m[0][2] = -arg0->worldMatrix.m[0][2];
            ((MATRIX *)arg0->boneMatrices)->m[1][0] = arg0->worldMatrix.m[1][0];
            ((MATRIX *)arg0->boneMatrices)->m[1][1] = -arg0->worldMatrix.m[1][1];
            ((MATRIX *)arg0->boneMatrices)->m[1][2] = -arg0->worldMatrix.m[1][2];
            ((MATRIX *)arg0->boneMatrices)->m[2][0] = arg0->worldMatrix.m[2][0];
            ((MATRIX *)arg0->boneMatrices)->m[2][1] = -arg0->worldMatrix.m[2][1];
            ((MATRIX *)arg0->boneMatrices)->m[2][2] = -arg0->worldMatrix.m[2][2];
            ((MATRIX *)arg0->boneMatrices)->t[0] = arg0->worldMatrix.t[0];
            ((MATRIX *)arg0->boneMatrices)->t[1] = arg0->worldMatrix.t[1];
            ((MATRIX *)arg0->boneMatrices)->t[2] = arg0->worldMatrix.t[2];
        }
        return;
    }
    nb = arg0->model->boneCount;
    bones = (MATRIX *)0x1F800018;
    if (nb >= 31) {
        bones = (MATRIX *)arg0->boneMatrices;
    }
    m = bones;
    vec = (VECTOR *)0x1F800008;
    vec->vx = 0;
    vec->vy = 0;
    key = anim->rotKeys;
    key2 = anim->rotKeysLo;
    rot = (SVECTOR *)0x1F800000;
    if (anim->constMask & 1) {
        bones->t[0] = anim->rootPos[0];
    } else {
        bones->t[0] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[0]))[(u16)arg1];
    }
    if (anim->constMask & 2) {
        m->t[1] = anim->rootPos[1];
    } else {
        m->t[1] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[1]))[(u16)arg1];
    }
    if (anim->constMask & 4) {
        m->t[2] = anim->rootPos[2];
    } else {
        m->t[2] = ((s16 *)((u8 *)anim + (u16)anim->rootPos[2]))[(u16)arg1];
    }
    end = &bones[nb];
loop1:
    gte_ldrotkey(key, anim, arg1, rot);
    if (key2 != NULL) {
        gte_ldrotkey_lo(key2, anim, arg1, rot);
        key2++;
    }
    key++;
    math_rotMatrixZYX_gte(rot, m);
    m++;
    if (m < end) {
        goto loop1;
    }
    if (arg0->parent != NULL) {
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[0];
            x1 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[1];
            x2 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[2];
            x3 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[3];
            ((s32 *)bones)[0] = x0;
            ((s32 *)bones)[1] = x1;
            ((s32 *)bones)[2] = x2;
            ((s32 *)bones)[3] = x3;
        }
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[4];
            x1 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[5];
            x2 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[6];
            x3 = ((s32 *)&((MATRIX *)arg0->parent->boneMatrices)[arg0->parentBone])[7];
            ((s32 *)bones)[4] = x0;
            ((s32 *)bones)[5] = x1;
            ((s32 *)bones)[6] = x2;
            ((s32 *)bones)[7] = x3;
        }
        bones->t[0] += arg0->attachX;
        bones->t[1] += arg0->attachY;
        bones->t[2] += arg0->attachZ;
    } else {
        gte_SetRotMatrix(&arg0->worldMatrix);
        gte_SetTransMatrix(&arg0->worldMatrix);
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
    m = bones + 1;
    idx = 1;
    up_p = &r2;
    key2 = (WmAnimKey *)((WmBone *)arg0->model->bones + 1);
    head = arg0->model->lookAtBone;
    rot = (SVECTOR *)&v;
    p = &bones[((WmBone *)key2)->parent];
loop:
    gte_SetRotMatrix(p);
    gte_ldclmv(m);
    gte_rtir();
    gte_SetTransMatrix(p);
    gte_stclmv(m);
    key = (WmAnimKey *)&m->m[0][1];
    gte_ldclmv(key);
    gte_rtir();
    vec->vz = ((WmBone *)key2)->length;
    gte_stclmv(key);
    arg1 = (s32)&m->m[0][2];
    gte_ldclmv((s16 *)arg1);
    gte_rtir();
    key2 = (WmAnimKey *)((WmBone *)key2 + 1);
    gte_stclmv((s16 *)arg1);
    gte_ldlv0(vec);
    gte_rtv0tr();
    p = &bones[((WmBone *)key2)->parent];
    gte_stlvnl(m->t);
    if (head == idx) {
        up.vx = 0;
        up.vy = 0x1000;
        up.vz = 0;
        v.vx = m->t[0] - arg0->lookAtTarget[0];
        v.vy = m->t[1] - arg0->lookAtTarget[1];
        v.vz = m->t[2] - arg0->lookAtTarget[2];
        VectorNormal(&v, up_p);
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
        m->m[0][0] = r0.vx;
        m->m[0][1] = r2.vx;
        m->m[0][2] = r1.vx;
        m->m[1][0] = r0.vy;
        m->m[1][1] = r2.vy;
        m->m[1][2] = r1.vy;
        m->m[2][0] = r0.vz;
        m->m[2][1] = r2.vz;
        m->m[2][2] = r1.vz;
        if (arg0->flags & MDL_SCALED) {
            *(s32 *)&s.m[0][0] = 0x1000;
            *(s32 *)&s.m[0][2] = 0;
            *(s32 *)&s.m[1][1] = 0x1000;
            *(s32 *)&s.m[2][0] = 0;
            *(s32 *)&s.m[2][2] = 0x1000;
            s.m[0][0] = arg0->scaleX;
            gte_SetRotMatrix(m);
            gte_ldclmv(&s);
            gte_rtir();
            s.m[1][1] = arg0->scaleY;
            gte_stclmv(m);
            gte_ldclmv(&s.m[0][1]);
            gte_rtir();
            s.m[2][2] = arg0->scaleZ;
            gte_stclmv(key);
            gte_ldclmv(&s.m[0][2]);
            gte_rtir();
            gte_stclmv((s16 *)arg1);
        }
    }
    m++;
    idx++;
    if (m < end) {
        goto loop;
    }
    if (nb < 31) {
        m = (MATRIX *)arg0->boneMatrices;
        do {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)bones)[0];
            x1 = ((s32 *)bones)[1];
            x2 = ((s32 *)bones)[2];
            x3 = ((s32 *)bones)[3];
            ((s32 *)m)[0] = x0;
            ((s32 *)m)[1] = x1;
            ((s32 *)m)[2] = x2;
            ((s32 *)m)[3] = x3;
            x0 = ((s32 *)bones)[4];
            x1 = ((s32 *)bones)[5];
            x2 = ((s32 *)bones)[6];
            x3 = ((s32 *)bones)[7];
            ((s32 *)m)[4] = x0;
            ((s32 *)m)[5] = x1;
            ((s32 *)m)[6] = x2;
            ((s32 *)m)[7] = x3;
            m++;
            bones++;
        } while (bones < end);
    }
}

void fldmdl_updateTexAnims(TexAnimSet *arg0) {
    RECT r;
    u32 i;
    u32 j;
    TexAnim *a;
    TexAnimRect *ar;
    s32 pos;
    s16 f;
    s32 r8;
    s32 v;
    s16 last;

    if (arg0 == NULL) {
        return;
    }
    for (i = 0; i < arg0->count; i++) {
        a = &arg0->anims[i];
        if (!(a->flags & TEXANIM_ACTIVE)) {
            continue;
        }
        if (a->frameCount != 0) {
            pos = a->pos;
            f = pos >> 12;
            last = a->lastFrame;
            if (f >= 0) {
                if (f != last) {
                    for (j = 0; j < a->rectCount; j++) {
                        ar = &a->rects[j];
                        r.x = ar->frames[f].vx;
                        r.y = ar->frames[f].vy;
                        r.w = ar->dest.w;
                        r.h = ar->dest.h;
                        MoveImage(&r, ar->dest.x, ar->dest.y);
                    }
                    a->lastFrame = f;
                }
                pos += a->speed;
            } else {
                pos += 0x1000;
            }
            if ((pos >> 12) < a->frameCount) {
                a->pos = pos;
            } else if (a->delayRange != 0) {
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                }
                r8 = *g_randPtr++;
                v = (a->delayRange + 1) * ((r8 << 8) | r8);
                a->pos = -((a->delayBase + v / 0x10000) << 12);
            } else if (a->flags & TEXANIM_ONESHOT) {
                a->flags &= ~(TEXANIM_ACTIVE | TEXANIM_ONESHOT);
            } else {
                a->pos = 0;
            }
        } else if (a->flags & TEXANIM_SCROLL) {
            pos = a->pos;
            f = pos >> 12;
            for (j = 0; j < a->rectCount; j++) {
                ar = &a->rects[j];
                MoveImage(&ar->dest, ar->dest.x, ar->dest.y - f);
                r.x = ar->dest.x;
                r.y = ar->dest.y - f;
                r.w = ar->dest.w;
                r.h = f;
                MoveImage(&r, ar->dest.x, ar->dest.y + ar->dest.h - f);
            }
            a->pos = (pos & 0xFFF) + a->speed;
        }
    }
}

void fldmdl_initTexAnims(TexAnimRelocSet *hdr, ModelTexOfs *ofs) {
    u32 i, j, k;
    u32 p;
    TexAnimRelocRect *part;
    u8 idx;

    if (hdr == NULL) {
        return;
    }
    for (i = 0; i < hdr->count; i++) {
        hdr->anims[i].lastFrame = 1;
        p = hdr->anims[i].rects;
        if (p > 0x80000000) {
            return;
        }
        if (p == 0) {
            continue;
        }
        idx = hdr->anims[i].texOfsIndex;
        p += (u32)hdr;
        hdr->anims[i].rects = p;
        if (ofs != NULL) {
            ((TexAnimRelocRect *)p)->destX += ofs[idx].destX;
            ((TexAnimRelocRect *)p)->destY += ofs[idx].destY;
        }
        part = (TexAnimRelocRect *)p;
        for (j = 0; j < hdr->anims[i].rectCount; j++) {
            p = part[j].frames;
            p += (u32)hdr;
            part[j].frames = p;
            if (ofs != NULL) {
                for (k = 0; k < hdr->anims[i].frameCount; k++) {
                    ((TexAnimRelocPos *)p)->x += ofs[idx].srcX;
                    ((TexAnimRelocPos *)p)->y += ofs[idx].srcY;
                    p += sizeof(TexAnimRelocPos);
                }
            }
        }
    }
}

void fldmdl_projectMeshClipY(ModelObj *arg0, u8 arg1) {
    WmModelGroup *g;
    MATRIX *bone;
    SVECTOR *vtx;
    s32 *sxy;
    s32 *sz;
    s32 k;
    u32 i;
    u32 j;
    u32 n;
    u32 cnt;
    s32 ymax;

    gte_SetGeomScreen(g_gameState->gameData->projDist);
    sxy = g_vtxScreenXY;
    sz = g_vtxScreenZ;
    k = 0;
    g = &arg0->model->groups[arg1];
    bone = (MATRIX *)arg0->boneMatrices;
    cnt = arg0->model->boneCount;
    ymax = arg0->clipY;
    vtx = g->vertices;
    for (i = 0; i < cnt; bone++) {
        n = ((u16 *)g->boneVertexCounts)[i++];
        if (n != 0) {
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
            } while (j < n);
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
            } while (j < n);
            k += n;
        }
    }
}

void fldmdl_drawMeshClipY(ModelObj *arg0, u8 arg1) {
    WmModelGroup *g;
    u32 *ot;
    s32 opz;
    s32 otz;
    u32 n;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 a, b;
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

    g = &arg0->model->groups[arg1];
    ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf];
    if (g_gameState->gameData->drawBuf != 0) {
        prim = (u8 *)arg0->groupOut[arg1].prims + g->packetSize;
    } else {
        prim = arg0->groupOut[arg1].prims;
    }

    n = g->ft4Count;
    pFT4 = (POLY_FT4 *)prim;
    sFT4 = (WmSrcFT4 *)(void *)g->prims;
    for (i = 0; i < n; sFT4++, pFT4++) {
        a = *(u32 *)&sFT4->vertices[0];
        b = *(u32 *)&sFT4->vertices[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b & 0xFFFF];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sFT4->flags & 2)) {
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

    n = g->ft3Count;
    sFT3 = (WmSrcFT3 *)sFT4;
    pFT3 = (POLY_FT3 *)pFT4;
    for (i = 0; i < n; sFT3++, pFT3++) {
        a = *(u32 *)&sFT3->vertices[0];
        b = sFT3->vertices[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)b];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sFT3->flags & 2)) {
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

    n = g->gt4Count;
    sGT4 = (WmSrcGT4 *)sFT3;
    pGT4 = (POLY_GT4 *)pFT3;
    for (i = 0; i < n; sGT4++, pGT4++) {
        a = *(u32 *)&sGT4->vertices[0];
        b = *(u32 *)&sGT4->vertices[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b & 0xFFFF];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sGT4->flags & 2)) {
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

    n = g->gt3Count;
    sGT3 = (WmSrcGT3 *)sGT4;
    pGT3 = (POLY_GT3 *)pGT4;
    for (i = 0; i < n; sGT3++, pGT3++) {
        a = *(u32 *)&sGT3->vertices[0];
        b = sGT3->vertices[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)b];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sGT3->flags & 2)) {
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

    n = g->g4Count;
    sG4 = (WmSrcG4 *)sGT3;
    pG4 = (POLY_G4 *)pGT3;
    for (i = 0; i < n; sG4++, pG4++) {
        a = *(u32 *)&sG4->vertices[0];
        b = *(u32 *)&sG4->vertices[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b & 0xFFFF];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sG4->flags & 2)) {
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

    n = g->g3Count;
    sG3 = (WmSrcG3 *)sG4;
    pG3 = (POLY_G3 *)pG4;
    for (i = 0; i < n; sG3++, pG3++) {
        a = *(u32 *)&sG3->vertices[0];
        b = sG3->vertices[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)b];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sG3->flags & 2)) {
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

    n = g->f4Count;
    sF4 = (WmSrcF4 *)sG3;
    pF4 = (POLY_F4 *)pG3;
    for (i = 0; i < n; sF4++, pF4++) {
        a = *(u32 *)&sF4->vertices[0];
        b = *(u32 *)&sF4->vertices[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b & 0xFFFF];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sF4->flags & 2)) {
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

    n = g->f3Count;
    sF3 = (WmSrcF3 *)sF4;
    pF3 = (POLY_F3 *)pF4;
    for (i = 0; i < n; sF3++, pF3++) {
        a = *(u32 *)&sF3->vertices[0];
        b = sF3->vertices[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)b];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sF3->flags & 2)) {
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
void fldmdl_reflectModel(ModelObj *arg0) {
    MATRIX *m;
    s32 nx;
    s32 ny;
    s32 nz;
    s32 d;

    if (arg0->mirrorSrc != NULL) {
        nx = arg0->mirrorNormal.v.vx;
        ny = arg0->mirrorNormal.v.vy;
        nz = arg0->mirrorNormal.v.vz;
        d = (arg0->mirrorPoint.v.vx * nx + arg0->mirrorPoint.v.vy * ny + arg0->mirrorPoint.v.vz * nz) >> 11;
        m = (MATRIX *)0x1F800000;
        m->m[0][0] = 0x1000 - ((nx * nx) >> 11);
        m->m[1][1] = 0x1000 - ((ny * ny) >> 11);
        m->m[2][2] = 0x1000 - ((nz * nz) >> 11);
        m->m[0][1] = m->m[1][0] = (nx * ny) >> 11;
        m->m[0][2] = m->m[2][0] = (nx * nz) >> 11;
        m->m[1][2] = m->m[2][1] = (ny * nz) >> 11;
        m->t[0] = (d * nx) >> 12;
        m->t[1] = (d * ny) >> 12;
        m->t[2] = (d * nz) >> 12;
        gte_SetRotMatrix(m);
        gte_ldclmv(&arg0->mirrorSrc->worldMatrix);
        gte_rtir();
        gte_stclmv(&arg0->worldMatrix);
        gte_ldclmv(&arg0->mirrorSrc->worldMatrix.m[0][1]);
        gte_rtir();
        gte_stclmv(&arg0->worldMatrix.m[0][1]);
        gte_ldclmv(&arg0->mirrorSrc->worldMatrix.m[0][2]);
        gte_rtir();
        gte_stclmv(&arg0->worldMatrix.m[0][2]);
        gte_SetTransMatrix(m);
        gte_ldlv0(arg0->mirrorSrc->worldMatrix.t);
        gte_rtv0tr();
        gte_stlvnl(arg0->worldMatrix.t);
        arg0->anim = arg0->mirrorSrc->anim;
    }
}

void fldmdl_drawMeshMirrored(ModelObj *arg0, u8 arg1) {
    WmModelGroup *g;
    u32 *ot;
    s32 opz;
    s32 otz;
    u32 n;
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
    g = &arg0->model->groups[arg1];
    if (g_gameState->gameData->drawBuf != 0) {
        prim = (u8 *)arg0->groupOut[arg1].prims + g->packetSize;
    } else {
        prim = arg0->groupOut[arg1].prims;
    }

    n = g->ft4Count;
    pFT4 = (POLY_FT4 *)prim;
    ot -= g->otBias;
    sFT4 = (WmSrcFT4 *)(void *)g->prims;
    for (i = 0; i < n; sFT4++, pFT4++) {
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
        if (!(sFT4->flags & 2)) {
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

    n = g->ft3Count;
    sFT3 = (WmSrcFT3 *)sFT4;
    pFT3 = (POLY_FT3 *)pFT4;
    for (i = 0; i < n; sFT3++, pFT3++) {
        i0 = *(u32 *)&sFT3->vertices[0];
        i2 = sFT3->vertices[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT3->flags & 2)) {
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

    n = g->gt4Count;
    sGT4 = (WmSrcGT4 *)sFT3;
    pGT4 = (POLY_GT4 *)pFT3;
    for (i = 0; i < n; sGT4++, pGT4++) {
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
        if (!(sGT4->flags & 2)) {
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

    n = g->gt3Count;
    sGT3 = (WmSrcGT3 *)sGT4;
    pGT3 = (POLY_GT3 *)pGT4;
    for (i = 0; i < n; sGT3++, pGT3++) {
        i0 = *(u32 *)&sGT3->vertices[0];
        i2 = sGT3->vertices[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT3->flags & 2)) {
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

    n = g->g4Count;
    sG4 = (WmSrcG4 *)sGT3;
    pG4 = (POLY_G4 *)pGT3;
    for (i = 0; i < n; sG4++, pG4++) {
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
        if (!(sG4->flags & 2)) {
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

    n = g->g3Count;
    sG3 = (WmSrcG3 *)sG4;
    pG3 = (POLY_G3 *)pG4;
    for (i = 0; i < n; sG3++, pG3++) {
        i0 = *(u32 *)&sG3->vertices[0];
        i2 = sG3->vertices[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG3->flags & 2)) {
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

    n = g->f4Count;
    sF4 = (WmSrcF4 *)sG3;
    pF4 = (POLY_F4 *)pG3;
    for (i = 0; i < n; sF4++, pF4++) {
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
        if (!(sF4->flags & 2)) {
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

    n = g->f3Count;
    sF3 = (WmSrcF3 *)sF4;
    pF3 = (POLY_F3 *)pF4;
    for (i = 0; i < n; sF3++, pF3++) {
        i0 = *(u32 *)&sF3->vertices[0];
        i2 = sF3->vertices[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF3->flags & 2)) {
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

s32 fldbg_buildOverlaySprites(BgOverlay *arg0, BgSprt *arg1, BgSprt **arg2) {
    BgPos *pos;
    BgTile *tile;
    u32 i;
    u32 buf;
    s32 n;
    s32 w;
    s32 x;
    u16 t;
    BgFile *bg;
    s32 u;
    s32 mask;
    s32 posOfs;
    s32 tileOfs;

    /* Statement order and the barrier below are for the scheduler: everything above the barrier
     * forms its own group, where both global loads are ready at once and are scheduled as
     * lui/lui/lw/lbu, so %hi(g_fieldBgClutRows) gets v1. The unk25_1 mask is set up here (and applied by
     * hand below) because its lui/ori must also sit above the barrier, and buf/the two offsets
     * are read early so their insns land where the original has them. */
    mask = 0xFFFF01FF;
    bg = g_fieldBgData;
    u = g_fieldBgClutRows;
    posOfs = arg0->posOffset;
    tileOfs = arg0->tileOffset;
    buf = 0;
    arg0->sprites = arg1;
    do { } while (0);
    pos = (BgPos *)((u8 *)bg + posOfs);
    tile = (BgTile *)((u8 *)bg + tileOfs);
    w = u + 0xDF;
    n = arg0->tileCount;
    arg0->screenX = arg0->x + bg->screenX;
    arg0->screenY = arg0->y + bg->screenY;
    arg0->skewVertical = 0;
    ((BgOverlayFlags *)arg0)->unk24 &= mask; /* arg0->unk25_1 = 0 */
    for (; buf < 2; buf++) {
        for (i = 0; i < n; i++) {
            BgSprt *sp = &arg1[i];
            setSprt16(&sp->sprt);
            sp->sprt.r0 = 0x80;
            sp->sprt.g0 = 0x80;
            sp->sprt.b0 = 0x80;
            sp->sprt.x0 = arg0->screenX + pos[i].x;
            sp->sprt.y0 = arg0->screenY + pos[i].y;
            sp->sprt.u0 = tile[i].u;
            sp->sprt.v0 = tile[i].v;
            if (tile[i].trans) {
                setSemiTrans(&sp->sprt, 1);
            } else {
                setSemiTrans(&sp->sprt, 0);
            }
            sp->sprt.clut = GetClut(tile[i].clutX << 4, tile[i].clutY);
            if (tile[i].tp == 0) {
                t = GetTPage(0, tile[i].abr, tile[i].tpX << 6, tile[i].tpY << 8);
            } else {
                t = GetTPage(1, tile[i].abr, tile[i].tpX << 6, tile[i].tpY << 8);
            }
            setDrawTPage(&sp->tpage, 1, 1, t);
            MargePrim(&sp->tpage, &sp->sprt);
            x = tile[i].clutY;
            if (w < x) {
                w = x;
            }
        }
        arg1 += n;
    }
    *arg2 = arg1;
    g_fieldBgClutRows = w + 0x21;
    return 1;
}

s32 fldbg_initEffectList(void) {
    u32 n;
    u32 i;
    BgOverlayAnim *tbl;
    BgOverlay *trk;

    tbl = (BgOverlayAnim *)((u8 *)g_fieldBgCtx.data + g_fieldBgCtx.data->overlayAnimOffset);
    trk = (BgOverlay *)((u8 *)g_fieldBgCtx.data + g_fieldBgCtx.data->overlayOffset);
    n = g_fieldBgCtx.data->overlayCount;
    i = 0;
    g_fieldBgCtx.entryCount = 0;
    g_fieldBgCtx.entries = (BgEntry *)g_fieldBgCtx.work;
    for (; i < n; i++) {
        if (trk[i].flags & FLDBG_OVL_ANIMATED) {
            g_fieldBgCtx.entries[g_fieldBgCtx.entryCount].type = 0x10;
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
    BgFile *d = g_fieldBgData;
    u32 i;
    BgOverlayAnim *p;
    BgOverlayAnim *paths;
    u32 cnt;
    BgOverlay *objs;
    BgAnimStep *st;
    u32 n;
    s32 pos;
    u32 j;
    s32 v;

    i = 0;
    paths = (BgOverlayAnim *)((u8 *)d + d->overlayAnimOffset);
    objs = (BgOverlay *)((u8 *)d + d->overlayOffset);
    n = d->overlayAnimCount;
    for (; i < n; i++) {
        p = &paths[i];
        pos = p->pos;
        if ((p->flags & FLDBG_ANIM_ACTIVE) && (p->flags & (FLDBG_ANIM_LOOP | FLDBG_ANIM_PLAY_ONCE)) && p->cameraId == g_fieldBgState.cameraId) {
            st = (BgAnimStep *)((u8 *)d + p->stepOffset);
            cnt = p->stepCount;
            for (j = 0; j < cnt; j++) {
                objs[st[j].overlay].flags &= ~FLDBG_OVL_VISIBLE;
            }
            objs[st[pos >> 8].overlay].flags |= FLDBG_OVL_VISIBLE;
            if (st[pos >> 8].delay < 0) {
                st[pos >> 8].delay = 0;
                p->delayCounter = 0;
                p->flags &= ~FLDBG_ANIM_PLAY_ONCE;
                return 1;
            }
            p->delayCounter++;
            if ((u16)p->delayCounter < st[pos >> 8].delay) {
                continue;
            }
            p->delayCounter = 0;
            p->pos = pos + p->speed;
            v = p->pos >> 8;
            if (v >= p->stepCount) {
                if (p->flags & FLDBG_ANIM_PINGPONG) {
                    p->pos = (p->stepCount - 1) << 8;
                    fldbg_setAnimSpeed(i, -p->speed);
                } else {
                    p->pos = 0;
                }
            } else if (v < 0) {
                if (p->flags & FLDBG_ANIM_PINGPONG) {
                    p->pos = 0;
                    fldbg_setAnimSpeed(i, -p->speed);
                } else {
                    p->pos = (p->stepCount - 1) << 8;
                }
            } else {
                continue;
            }
            p->flags &= ~FLDBG_ANIM_PLAY_ONCE;
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

s32 fldbg_drawFollowMask(BgFile *bg, BgOverlay *o, FieldBgState *st, BgFollowOverlay *f) {
    u32 *ot;
    POLY_F4 *p;
    DR_TPAGE *tp;
    s16 x;
    s16 y;
    s16 u;
    s16 v;
    s16 w;

    ot = g_fieldBgCtx.ot[*g_fieldBgCtx.bufIndex];
    x = o->x + bg->posX - (st->scroll.x - 0xA0);
    y = o->y + bg->posY - (st->scroll.y - 0x70);
    if (x > 0) {
        p = &f->quads[*g_fieldBgCtx.bufIndex][0];
        u = y;
        if (u < 0) {
            u = 0;
        }
        setXY4(p, 0, u, x, u, 0, 0xE0, x, 0xE0);
        addPrim(&ot[7], p);
    }
    if (y > 0) {
        u = x + o->width;
        p = &f->quads[*g_fieldBgCtx.bufIndex][1];
        if (u > 0x140) {
            u = 0x140;
        }
        v = y;
        if (v < 0) {
            v = 0;
        }
        setXY4(p, 0, 0, u, 0, 0, v, u, v);
        addPrim(&ot[7], p);
    }
    if (x + o->width < 0x140) {
        u = x + o->width;
        v = y + o->height;
        p = &f->quads[*g_fieldBgCtx.bufIndex][2];
        w = 0x140 - u;
        if (v > 0xE0) {
            v = 0xE0;
        }
        setXYWH(p, u, 0, w, v);
        addPrim(&ot[7], p);
    }
    if (y + o->height < 0xE0) {
        p = &f->quads[*g_fieldBgCtx.bufIndex][3];
        v = x;
        if (v < 0) {
            v = 0;
        }
        u = y + o->height;
        if (u < 0) {
            u = 0;
        }
        setXY4(p, v, u, 0x140, u, v, 0xE0, 0x140, 0xE0);
        addPrim(&ot[7], p);
    }
    if (f->abr < 4) {
        tp = &f->tpages[*g_fieldBgCtx.bufIndex];
        SetDrawTPage(tp, 1, 1, GetTPage(2, f->abr, 0, 0));
        addPrim(&ot[7], tp);
    }
    return 1;
}

s32 fldbg_updateFollowOverlays(void) {
    SVECTOR v;
    s32 sxy;
    FieldBgState *st;
    BgFile *bg;
    BgOverlay *objs;
    BgOverlay *o;
    EvtObject *a;
    BgFollowOverlay *f;
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
            a = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
        } else if (mode == 1) {
            a = evt_getPlayer();
        } else {
            a = NULL;
        }
        if (a != NULL) {
            v.vx = a->pos.vx;
            v.vy = a->pos.vy - g_fieldBgState.followHeight;
            v.vz = a->pos.vz;
        } else {
            v.vx = 0;
            v.vy = 0;
            v.vz = 0;
        }
        fldbg_applyGeomOffset();
        gte_SetRotMatrix(g_fieldBgState.viewMatrix);
        gte_SetTransMatrix(g_fieldBgState.viewMatrix);
        gte_SetGeomScreen(*g_fieldBgState.projDist);
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&sxy);
        sx = sxy;
        sy = sxy >> 16;
        f = g_fieldBgFollowOverlays;
        for (i = 0; i < g_fieldBgFollowOverlayCount; i++, f++) {
            o = (BgOverlay *)((u16)f->overlay * sizeof(BgOverlay) + (u32)objs);
            if ((o->flags & 2) && o->cameraId == st->cameraId) {
                x = st->scroll.x + (sx - bg->posX - (f->offsetX + 0xA0));
                y = f->offsetY;
                o->x = x;
                o->y = st->scroll.y + (sy - bg->posX - (y + 0x70));
                if (f->abr >= 0) {
                    fldbg_drawFollowMask(bg, o, st, f);
                }
            }
        }
        return 1;
    }
}

s32 fldbg_initOverlays(u8 *arg0, BgSprt **arg1) {
    BgFile *h;
    BgCamera *c;
    s32 x;
    s32 y;
    u32 n;
    u32 size;
    u32 i;
    BgOverlay *o;
    BgOverlay *objs;
    BgOverlay *p;

    h = g_fieldBgCtx.data;
    fldbg_setCamera(g_fieldBgState.cameraId);
    c = &((BgCamera *)((u8 *)h + h->cameraOffset))[g_fieldBgState.cameraId];
    x = (c->scrollMinX + c->scrollMaxX) / 2;
    g_fieldBgState.scroll.x = (g_fieldBgState.parallaxOriginX = x);
    g_fieldBgState.scroll.y = (g_fieldBgState.parallaxOriginY = (c->scrollMinY + c->scrollMaxY) / 2);
    x -= 160;
    h->screenX = h->posX - x;
    n = h->overlayCount;
    objs = (BgOverlay *)((u8 *)h + h->overlayOffset);
    size = n * 3;
    y = g_fieldBgState.scroll.y - 112;
    h->screenY = h->posY - (s16)y;
    g_fieldBgCtx.overlayRgb = arg0;
    while (size / 4 * 4 != size) {
        size++;
    }
    g_fieldBgCtx.overlayTinted = arg0 + size;
    size += n;
    while (size / 4 * 4 != size) {
        size++;
    }
    *arg1 = (BgSprt *)(arg0 + size);
    for (i = 0; i < 4; i++) {
        g_fieldBgState.viewportPos[i][0] = 0;
        g_fieldBgState.viewportPos[i][1] = 0;
        g_fieldBgState.viewportSize[i][0] = c->width;
        g_fieldBgState.viewportSize[i][1] = c->height;
        g_fieldBgState.viewportRateX[i] = 0x100;
        g_fieldBgState.viewportRateY[i] = 0x100;
    }
    for (i = 0; i < n; i++) {
        o = &objs[i];
        g_fieldBgCtx.overlayRgb[i * 3 + 0] = 0x80;
        g_fieldBgCtx.overlayRgb[i * 3 + 1] = 0x80;
        g_fieldBgCtx.overlayRgb[i * 3 + 2] = 0x80;
        g_fieldBgCtx.overlayTinted[i] = 0;
        p = o;
        if (fldbg_buildOverlaySprites(p, *arg1, arg1) != 1) {
            return 0;
        }
        o->minX = -0x8000;
        p->maxX = 0x7FFF;
        o->minY = -0x8000;
        p->maxY = 0x7FFF;
    }
    g_fieldBgState.unk73 = 0;
    g_fieldBgState.unk72 = 0;
    g_fieldBgState.flags &= ~0xC00;
    return 1;
}

s32 fldbg_drawOverlay(BgOverlay *o, u8 *arg1) {
    BgSprt *spr;
    u32 *ot;
    BgFile *h;
    BgPos *pos;
    u32 n;
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
    u8 buf;

    spr = o->sprites;
    buf = *g_fieldBgCtx.bufIndex;
    h = g_fieldBgCtx.data;
    n = o->tileCount;
    ot = g_fieldBgCtx.ot[buf];
    if (buf != 0) {
        spr += n;
    }
    x = o->x + h->screenX;
    y = o->y + h->screenY;
    z = h->baseDepth + o->depth;
    pos = (BgPos *)((u8 *)h + o->posOffset);
    if (o->flags & FLDBG_OVL_LOOP) {
        s16 tx;
        s16 ty;
        s16 d;
        s16 k;

        if (o->flags & FLDBG_OVL_SCREEN) {
            ox = g_fieldBgState.viewportPos[o->viewport][0];
            oy = g_fieldBgState.viewportPos[o->viewport][1];
        } else {
            ox = g_fieldBgState.viewportPos[o->viewport][0] + 0xA0 - g_fieldBgState.scroll.x;
            oy = g_fieldBgState.viewportPos[o->viewport][1] + 0x70 - g_fieldBgState.scroll.y;
        }
        wx = g_fieldBgState.viewportSize[o->viewport][0];
        wy = g_fieldBgState.viewportSize[o->viewport][1];
        sx = o->scrollX;
        if (sx < 0) {
            k = 0x100 - sx;
            x = h->screenX + ((((o->x << 8) | o->fracX) + k) >> 8);
        }
        sy = o->scrollY;
        if (sy < 0) {
            k = 0x100 - sx;
            y = h->screenY + ((((o->y << 8) | o->fracY) + k) >> 8);
        }
        if (sx != 0) {
            x = (x - ((s16)wx - o->width)) % o->width + (wx - o->width);
        }
        if (sy != 0) {
            y = (y - ((s16)wy - o->height)) % o->height + (wy - o->height);
        }
        for (i = 0; i < n; i++) {
            if (o->flags & FLDBG_OVL_SCREEN) {
                tx = x + pos[i].x;
                if (o->scrollX != 0) {
                    if (tx + 16 >= o->width) {
                        tx -= o->width;
                    } else if (tx < -15) {
                        tx += o->width;
                    }
                }
                if (tx + (s16)ox < -15 || (s16)wx < tx) {
                    continue;
                }
                ty = y + pos[i].y;
                if (o->scrollY != 0) {
                    if (ty + 16 >= o->height) {
                        ty -= o->height;
                    } else if (ty < -15) {
                        ty += o->height;
                    }
                }
                if (ty + (s16)oy < -15 || ty >= (s16)wy) {
                    continue;
                }
                d = z + pos[i].pad;
                if (d < 8 || d >= 0xFF9) {
                    continue;
                }
                spr[i].sprt.x0 = tx + ox;
                spr[i].sprt.y0 = ty + oy;
            } else {
                tx = x + pos[i].x;
                if (o->scrollX != 0) {
                    if (tx + 16 >= o->width) {
                        tx -= o->width;
                    } else if (tx < -15) {
                        tx += o->width;
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
                if (o->scrollY != 0) {
                    if (ty + 16 >= o->height) {
                        ty -= o->height;
                    } else if (ty < -15) {
                        ty += o->height;
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
                d = z + pos[i].pad;
                if (d < 8 || d >= 0xFF9) {
                    continue;
                }
            }
            addPrim(ot + d, &spr[i]);
        }
    } else if (o->flags & FLDBG_OVL_SKEW_LOOP) {
        s16 tx;
        s16 ty;
        s16 d;
        s16 k;

        if (o->flags & FLDBG_OVL_SCREEN) {
            ox = g_fieldBgState.viewportPos[o->viewport][0];
            oy = g_fieldBgState.viewportPos[o->viewport][1];
        } else {
            ox = g_fieldBgState.viewportPos[o->viewport][0] + 0xA0 - g_fieldBgState.scroll.x;
            oy = g_fieldBgState.viewportPos[o->viewport][1] + 0x70 - g_fieldBgState.scroll.y;
        }
        wx = g_fieldBgState.viewportSize[o->viewport][0];
        wy = g_fieldBgState.viewportSize[o->viewport][1];
        if (o->skewVertical) {
            y = (y - ((s16)wy - o->height)) % o->height + (wy - o->height);
            x += y * o->scrollX / o->height % o->width;
        } else {
            x = (x - ((s16)wx - o->width)) % o->width + (wx - o->width);
            y += x * o->scrollY / o->width % o->height;
        }
        for (i = 0; i < n; i++) {
            if (o->skewVertical) {
                k = 0;
                ty = y + pos[i].y;
                if (ty + 16 >= o->height) {
                    ty -= o->height;
                    k = -o->scrollX;
                } else if (ty < -15) {
                    ty += o->height;
                    k = o->scrollX;
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
                if (tx + 16 >= o->width) {
                    tx -= o->width;
                    k = -o->scrollY;
                } else if (tx < -15) {
                    tx += o->width;
                    k = o->scrollY;
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
            d = z + pos[i].pad;
            if (d < 8 || d >= 0xFF9) {
                continue;
            }
            addPrim(ot + d, &spr[i]);
        }
    } else {
        s16 tx;
        s16 ty;
        s16 d;

        for (i = 0; i < n; i++) {
            tx = x + pos[i].x;
            if (tx < -15 || tx >= 320) {
                continue;
            }
            ty = y + pos[i].y;
            if (ty < -15 || ty >= 224) {
                continue;
            }
            d = z + pos[i].pad;
            if (d < 8 || d >= 0xFF9) {
                continue;
            }
            spr[i].sprt.y0 = ty;
            spr[i].sprt.x0 = tx;
            addPrim(ot + d, &spr[i]);
        }
    }
    o->screenX = x;
    o->screenY = y;
    return 1;
}

s32 fldbg_updateOverlayScroll(BgFile *arg0) {
    s32 i;
    s32 n;
    BgOverlay *o;
    BgOverlay *objs;
    s32 v;

    n = arg0->overlayCount;
    objs = (BgOverlay *)((u8 *)arg0 + arg0->overlayOffset);
    for (i = 0; i < n; i++) {
        o = &objs[i];
        if (o->flags & FLDBG_OVL_LOOP) {
            if (o->scrollX != 0 && o->scrollX != 0x7FFF) {
                v = (o->x << 8) | (o->fracX & 0xFF);
                v = v + o->scrollX;
                o->fracX = v & 0xFF;
                o->x = (v >> 8) % o->width;
            }
            if (o->scrollY != 0 && o->scrollY != 0x7FFF) {
                v = (o->y << 8) | (o->fracY & 0xFF);
                v = v + o->scrollY;
                o->fracY = v & 0xFF;
                o->y = (v >> 8) % o->height;
            }
        }
        if (o->flags & FLDBG_OVL_SKEW_LOOP) {
            if (o->skewVertical) {
                if (o->scrollY != 0x7FFF) {
                    v = (o->y << 8) | (o->fracY & 0xFF);
                    v = v + o->scrollY;
                    o->fracY = v & 0xFF;
                    o->y = (v >> 8) % o->height;
                }
            } else if (o->scrollX != 0x7FFF) {
                v = (o->x << 8) | (o->fracX & 0xFF);
                v = v + o->scrollX;
                o->fracX = v & 0xFF;
                o->x = (v >> 8) % o->width;
            }
        }
        if (o->flags & FLDBG_OVL_PARALLAX) {
            v = (o->baseX << 8) + ((s16)g_fieldBgState.scroll.x - g_fieldBgState.parallaxOriginX) * o->scrollX;
            o->x = v >> 8;
            o->fracX = v & 0xFF;
            v = (o->baseY << 8) + ((s16)g_fieldBgState.scroll.y - g_fieldBgState.parallaxOriginY) * o->scrollY;
            o->y = v >> 8;
            o->fracY = v & 0xFF;
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
    BgOverlay *o;
    BgFile *h;
    s16 wx;
    s16 wy;
    s32 dx;
    s32 flags;
    s32 smooth;
    s32 ax;
    s32 ay;
    s32 i;
    BgCamera *c;
    s16 cnt;
    s32 s;
    s32 v;
    s32 t;
    s32 y;
    s32 mode;
    flags = g_fieldBgState.flags;
    if (!(flags & 0x40)) {
        return 0;
    }
    mode = flags & 7;
    smooth = flags & 8;
    if (mode == 1 || mode == 2 || mode == 3) {
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
        o = (BgOverlay *)((u8 *)h + h->overlayOffset);
        for (i = 0; i < h->overlayCount; o++, i++) {
            if (o->flags & FLDBG_OVL_LOOP) {
                if (o->scrollX != 0) {
                    o->x += ox;
                }
                if (o->scrollY != 0) {
                    o->y += oy;
                }
                dx = (s16)ox << 16;
            s = g_fieldBgState.viewportRateX[o->viewport] << 8;
            v = dx;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->x << 16) - fld_fixMul(v, -s);
                } else {
                    v = (o->x << 16) + fld_fixMul(v, s);
                }
                o->x = v >> 16;
                o->fracX = (v >> 8) & 0xFF;
            }
            s = g_fieldBgState.viewportRateY[o->viewport] << 8;
            v = oy << 16;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->y << 16) - fld_fixMul(v, -s);
                } else {
                    v = (o->y << 16) + fld_fixMul(v, s);
                }
                o->y = v >> 16;
                o->fracY = (v >> 8) & 0xFF;
            }
            } else if (o->flags & FLDBG_OVL_SKEW_LOOP) {
                if (o->skewVertical) {
                    o->y += oy;
                } else {
                    o->x += ox;
                }
            s = g_fieldBgState.viewportRateX[o->viewport] << 8;
            v = (s16)ox << 16;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->x << 16) - fld_fixMul(v, -s);
                } else {
                    v = (o->x << 16) + fld_fixMul(v, s);
                }
                o->x = v >> 16;
                o->fracX = (v >> 8) & 0xFF;
            }
            s = g_fieldBgState.viewportRateY[o->viewport] << 8;
            v = oy << 16;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->y << 16) - fld_fixMul(v, -s);
                    o->y = v >> 16;
                    o->fracY = (v >> 8) & 0xFF;
                } else {
                    v = (o->y << 16) + fld_fixMul(v, s);
                    o->y = v >> 16;
                    o->fracY = (v >> 8) & 0xFF;
                }
            }
            }
        }
        s = h->cameraOffset;
        s = (s32)h - -s; /* keeps the h + off operand order (plus h s) */
        s = s + g_fieldBgState.cameraId * sizeof(BgCamera);
        v = 160;
        y = *(s16 *)&g_fieldBgState.scroll.x - (((BgCamera *)s)->width >> 1);
        i = v - y;
        t = 112 - (*(s16 *)&g_fieldBgState.scroll.y - (((BgCamera *)s)->height >> 1));
        g_fieldBgState.geomOffsetX = i;
        g_fieldBgState.geomOffsetY = t;
        if (mode == 1) {
            g_fieldBgState.flags = (g_fieldBgState.flags & ~7) | 2;
        }
        if (++g_fieldBgState.scrollFrame > dur) {
            if ((g_fieldBgState.flags & 7) == 2) {
                g_fieldBgState.flags = (g_fieldBgState.flags & ~7) | 0x84;
            } else {
                g_fieldBgState.flags = (g_fieldBgState.flags & ~7) | 0x80;
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
    BgCamera *c;
    EvtObject *a;
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
    BgOverlay *o;
    s32 s;
    s32 v;
    flags = g_fieldBgState.flags;
    if (flags & 7) {
        return 0;
    }
    if (!(flags & 0x40)) {
        return 0;
    }
    mode = g_gameState->mode;
    if (mode == 5) {
        a = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
    } else if (mode == 1) {
        a = evt_getPlayer();
    } else {
        a = NULL;
    }
    h = g_fieldBgData;
    c = (BgCamera *)((u8 *)h + h->cameraOffset);
    c += g_fieldBgState.cameraId;
    if (a != NULL) {
        sv.vx = a->pos.vx;
        sv.vy = a->pos.vy - g_fieldBgState.followHeight;
        sv.vz = a->pos.vz;
    } else {
        sv.vx = 0;
        sv.vy = 0;
        sv.vz = 0;
    }
    ReadGeomOffset(&ofx, &ofy);
    gte_SetGeomOffset(0xA0, 0x70);
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
    sx = 320 - x;
    sy = 224 - y;
    g_fieldBgState.playerScreenX = x;
    g_fieldBgState.playerScreenY = y;
    wx = (c->width >> 1) + (s16)c->centerX;
    lim = wx - 160;
    cx = lim + x;
    {
        s32 t = (c->height >> 1) + (s16)c->centerY - 112;
        cy = t + y;
    }
    if (cx < c->scrollMinX) {
        {
            s32 t = c->scrollMinX - 160;
            sx = wx - t;
        }
    } else if (c->scrollMaxX < cx) {
        {
            s32 t = c->scrollMaxX - 160;
            sx = wx - t;
        }
    }
    if (cy < c->scrollMinY) {
        {
            s32 a = ((c->height >> 1) + (s16)c->centerY);
            s32 t = c->scrollMinY - 112;
            sy = a - t;
        }
    } else if (c->scrollMaxY < cy) {
        {
            s32 a = ((c->height >> 1) + (s16)c->centerY);
            s32 t = c->scrollMaxY - 112;
            sy = a - t;
        }
    }
    g_fieldBgState.geomOffsetX = sx - c->centerX;
    g_fieldBgState.geomOffsetY = sy - c->centerY;
    px = &dx;
    py = &dy;
    if (g_fieldBgState.flags & 0x40) {
        ox = g_fieldBgState.scroll.x;
        oy = g_fieldBgState.scroll.y;
        if ((s16)cx < c->scrollMinX) {
            g_fieldBgState.scroll.x = c->scrollMinX;
        } else if (c->scrollMaxX < (s16)cx) {
            g_fieldBgState.scroll.x = c->scrollMaxX;
        } else {
            g_fieldBgState.scroll.x = cx;
        }
        if ((s16)cy < c->scrollMinY) {
            g_fieldBgState.scroll.y = c->scrollMinY;
        } else if (c->scrollMaxY < (s16)cy) {
            g_fieldBgState.scroll.y = c->scrollMaxY;
        } else {
            g_fieldBgState.scroll.y = cy;
        }
        *px = g_fieldBgState.scroll.x - ox;
        *py = g_fieldBgState.scroll.y - oy;
    }
    o = (BgOverlay *)((u8 *)h + h->overlayOffset);
    for (i = 0; i < h->overlayCount; o++, i++) {
        if (o->flags & FLDBG_OVL_LOOP) {
            if (o->scrollX != 0) {
                o->x += dx;
            }
            if (o->scrollY != 0) {
                o->y += dy;
            }
        s = g_fieldBgState.viewportRateX[o->viewport] << 8;
        v = dx << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->x << 16) - fld_fixMul(v, -s);
            } else {
                v = (o->x << 16) + fld_fixMul(v, s);
            }
            o->x = v >> 16;
            o->fracX = (v >> 8) & 0xFF;
        }
        s = g_fieldBgState.viewportRateY[o->viewport] << 8;
        v = dy << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->y << 16) - fld_fixMul(v, -s);
            } else {
                v = (o->y << 16) + fld_fixMul(v, s);
            }
            o->y = v >> 16;
            o->fracY = (v >> 8) & 0xFF;
        }
        } else if (o->flags & FLDBG_OVL_SKEW_LOOP) {
            if (o->skewVertical) {
                o->y += dy;
            } else {
                o->x += dx;
            }
        s = g_fieldBgState.viewportRateX[o->viewport] << 8;
        v = dx << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->x << 16) - fld_fixMul(v, -s);
            } else {
                v = (o->x << 16) + fld_fixMul(v, s);
            }
            o->x = v >> 16;
            o->fracX = (v >> 8) & 0xFF;
        }
        s = g_fieldBgState.viewportRateY[o->viewport] << 8;
        v = dy << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->y << 16) - fld_fixMul(v, -s);
                o->y = v >> 16;
                o->fracY = (v >> 8) & 0xFF;
            } else {
                v = (o->y << 16) + fld_fixMul(v, s);
                o->y = v >> 16;
                o->fracY = (v >> 8) & 0xFF;
            }
        }
        }
    }
    return 1;
}

/* reset the field background state; returns the end of its allocations */
BgSprt *fldbg_init(void) {
    SVECTOR v;
    BgSprt *spr;
    BgFile *h;
    BgOverlayAnimView *anims;
    BgOverlay *trk;
    u32 i;
    u32 n;
    s32 t;
    s32 x;
    union {
        s32 w;
        DVECTOR v;
    } u; /* projected origin */

    g_fieldBgState.flags = 0x60;
    g_fieldBgState.unk74 = 0x40;
    g_fieldBgState.followHeight = 0x144;
    g_fieldBgState.unk78 = 0xE10;
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
    n = h->overlayAnimCount;
    for (i = 0; i < n; i++) {
        anims[i].cameraPos &= 0xFF;
        anims[i].speed = 0x100;
        anims[i].delayCounter = 0;
        anims[i].flags = 1;
        trk[*((u8 *)h + anims[i].stepOffset)].flags |= FLDBG_OVL_VISIBLE;
    }
    fldbg_initEffectList();
    fldbg_initFollowOverlays();
    v.vx = 0;
    v.vy = 0;
    v.vz = 0;
    gte_SetGeomOffset(0xA0, 0x70);
    gte_SetRotMatrix(g_fieldBgState.viewMatrix);
    gte_SetTransMatrix(g_fieldBgState.viewMatrix);
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(&u.w);
    t = u.w;
    x = u.v.vx;
    g_fieldBgState.playerScreenX = x;
    g_fieldBgState.playerScreenY = t >> 16;
    return spr;
}

s32 fldbg_draw(void) {
    BgFile *h;
    BgOverlay *o;
    FieldBgState *c;
    u32 i;
    s32 j;

    fldbg_updateOverlayAnims();
    /* the switch results are dead; only the loads and the hoisted 0x10/0x40 survive */
    do {
        for (i = 0; i < g_fieldBgCtx.entryCount; i++) {
            switch (g_fieldBgCtx.entries[i].type) {
            case 0x10:
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
    c = &g_fieldBgState;
    h = g_fieldBgCtx.data;
    h->screenX = h->posX - (s16)(c->scroll.x - 0xA0);
    h->screenY = h->posY - (s16)(c->scroll.y - 0x70);
    o = (BgOverlay *)((u8 *)h + h->overlayOffset);
    for (j = 0; j < h->overlayCount; j++) {
        if ((o[j].flags & 2) && o[j].cameraId == c->cameraId) {
            fldbg_drawOverlay(&o[j], g_fieldBgCtx.overlayRgb + j * 3);
        }
    }
    return 1;
}

s32 fldbg_scrollToPlayer(s32 arg0, s32 arg1) {
    SVECTOR v;
    s16 xy[2];
    BgCamera *c;
    EvtObject *a;
    u8 mode;
    s32 x;
    s32 y;
    s32 t;
    s32 cp;
    FieldBgState *p;
    s32 f;
    s32 ox;
    s32 oy;
    BgFile *bg;
    FieldBgState *st = NULL; /* dead store: lets cse keep the st = p copy below */

    if (!(g_fieldBgState.flags & 0x40)) {
        return 0;
    }
    mode = g_gameState->mode;
    if (mode == 5) {
        a = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
    } else {
        a = NULL;
        if (mode == 1) {
            a = evt_getPlayer();
        }
    }
    bg = g_fieldBgData;
    p = &g_fieldBgState;
    c = (BgCamera *)((u8 *)bg + bg->cameraOffset);
    cp = *(s32 *)&p->scroll;
    /* a second pointer to g_fieldBgState whose copy disappears after register allocation; it is what
       makes sched1 order the copy/vx loads and local-alloc give the copy $v0 as in the original */
    st = p;
    *(s32 *)&st->scrollFrom = cp;
    v.vx = a->pos.vx;
    v.vy = a->pos.vy - st->followHeight;
    c = &c[st->cameraId];
    v.vz = a->pos.vz;
    ox = (s16)c->centerX;
    oy = (s16)c->centerY;
    gte_SetGeomOffset(ox + 0xA0, oy + 0x70);
    {
        /* the original's "r"(st->unk1C) operand was reloaded into $t0; as a hard-register variable
           it is one less reference to st, which keeps st below the unk76 temporary in local-alloc */
        register MATRIX *m REG_ASM("$8") = st->viewMatrix;
        gte_SetRotMatrix(m);
        gte_SetTransMatrix(m);
    }
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(xy);
    /* dead test (deleted by jump2): extends st's live range to here */
    if (st != NULL) {
        st = NULL;
    }
    {
        s32 t = (c->width >> 1) - 0xA0;
        x = t + xy[0];
    }
    {
        s32 t = (c->height >> 1) - 0x70;
        y = t + xy[1];
    }
    if (x < c->scrollMinX) {
        x = c->scrollMinX;
    } else if (c->scrollMaxX < x) {
        x = c->scrollMaxX;
    }
    if (y < c->scrollMinY) {
        y = c->scrollMinY;
    } else if (c->scrollMaxY < y) {
        y = c->scrollMaxY;
    }
    g_fieldBgState.scrollToX = x;
    g_fieldBgState.scrollToY = y;
    if (arg0 == -1) {
        g_fieldBgState.scrollFrames = 0x1E;
    } else {
        g_fieldBgState.scrollFrames = arg0;
    }
    g_fieldBgState.scrollFrame = 1;
    if (arg1 == -1) {
        g_fieldBgState.flags = (g_fieldBgState.flags & ~7) | 3;
    } else {
        f = g_fieldBgState.flags & ~0xF;
        g_fieldBgState.flags = f | 3;
        if (arg1 == 8) {
            g_fieldBgState.flags = f | 8 | 3;
        } else {
            g_fieldBgState.flags = f | 3;
        }
    }
    return 1;
}

/* add a screen fade: rectangle (arg0..arg2), mode arg3, colour arg4..arg6 */
s32 fldbg_addFollowOverlay(s16 arg0, s16 arg1, s16 arg2, s8 arg3, u8 arg4, u8 arg5, u8 arg6) {
    u32 i;

    g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].overlay = arg0;
    g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].offsetX = arg1;
    g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].offsetY = arg2;
    g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].abr = arg3;
    if (arg3 >= 0) {
        g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].r = arg4;
        g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].g = arg5;
        g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].b = arg6;
        for (i = 0; i < 4; i++) {
            SetPolyF4(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[0][i]);
            setRGB0(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[0][i], arg4, arg5, arg6);
            SetPolyF4(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[0][i + 4]); /* = unkC[1][i] */
            setRGB0(&g_fieldBgFollowOverlays[g_fieldBgFollowOverlayCount].quads[1][i], arg4, arg5, arg6);
            if (arg3 >= 4) {
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
s32 fldbg_toggleOverlayColor(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    BgFile *h;
    BgOverlay *o;
    BgSprt *s;
    u8 t;
    u16 n;
    s32 off;
    u16 i;

    t = g_fieldBgCtx.overlayTinted[arg0];
    h = g_fieldBgCtx.data;
    if (t != 0) {
        o = &((BgOverlay *)((u8 *)h + h->overlayOffset))[arg0];
        n = o->tileCount;
        s = o->sprites;
        off = 0;
        if (*g_fieldBgCtx.bufIndex != 0) {
            off = n;
        }
        for (i = 0; i < n; i++) {
            setRGB0(&s[i + off].sprt, arg1, arg2, arg3);
        }
        g_fieldBgCtx.overlayTinted[arg0] = 0;
        return 0;
    }
    g_fieldBgCtx.overlayRgb[arg0 * 3 + 0] = arg1;
    g_fieldBgCtx.overlayRgb[arg0 * 3 + 1] = arg2;
    g_fieldBgCtx.overlayRgb[arg0 * 3 + 2] = arg3;
    o = &((BgOverlay *)((u8 *)h + h->overlayOffset))[arg0];
    n = o->tileCount;
    s = o->sprites;
    off = 0;
    if (*g_fieldBgCtx.bufIndex != 0) {
        off = n;
    }
    for (i = 0; i < n; i++) {
        setRGB0(&s[i + off].sprt, arg1, arg2, arg3);
    }
    g_fieldBgCtx.overlayTinted[arg0] = 1;
    return 1;
}

s32 fldbg_startClutFade(s32 arg0, u8 arg1, u32 arg2, u8 arg3, s32 arg4, u8 arg5, u8 arg6, u8 arg7) {
    s32 rgb[3];
    BgEntry *e;
    FieldPalAnim *p;
    BgFile *h;
    s32 n;
    s32 y;
    s32 size;
    u16 i;
    u8 *q;
    volatile u16 *w;
    s32 t;
    s32 k;
    register u16 v REG_ASM("$3");

    t = arg3 - 1;
    n = (u8)arg4 - t;
    size = n * 4 + 0x2C;
    e = &g_fieldBgCtx.entries[arg0];
    e->type = 0x80;
    e->dataSize = size;
    h = g_fieldBgCtx.data;
    q = (u8 *)h + h->overlayOffset;
    y = *(u32 *)((u8 *)h + ((BgOverlay *)q)[e->overlay].tileOffset) & 0x1FF;
    p = (FieldPalAnim *)(g_fieldBgCtx.work + g_fieldBgCtx.workUsed);
    /* size is reused for the new end offset (two sets keep sched1 from moving its computation down to
     * e->unk8), read through a copy so the add keeps the original operand order */
    k = size;
    size = g_fieldBgCtx.workUsed + k;
    g_fieldBgCtx.workUsed = size;
    e->dataOffset = (u8 *)p - (u8 *)g_fieldBgCtx.entries;
    p->active = 1;
    p->count = n;
    p->first = arg3;
    p->last = arg4;
    p->frames = arg2;
    p->frame = 0;
    if (arg2 >= 2) {
        rgb[0] = arg5 << 16;
        rgb[1] = arg6 << 16;
        rgb[2] = arg7 << 16;
        switch (arg1) {
        case 0:
            rgb[0] = ((u32)(arg5 << 24) / (arg2 << 16)) << 8;
            rgb[1] = ((u32)(arg6 << 24) / (arg2 << 16)) << 8;
            rgb[2] = ((u32)(arg7 << 24) / (arg2 << 16)) << 8;
            p->unk4 = 0;
            break;
        case 1:
            rgb[0] = -(((u32)(arg5 << 24) / (arg2 << 16)) << 8);
            rgb[1] = -(((u32)(arg6 << 24) / (arg2 << 16)) << 8);
            rgb[2] = -(((u32)(arg7 << 24) / (arg2 << 16)) << 8);
            p->unk4 = 0;
            break;
        case 2:
            rgb[0] = ((u32)(arg5 << 24) / (arg2 << 16)) << 8;
            rgb[1] = ((u32)(arg6 << 24) / (arg2 << 16)) << 8;
            rgb[2] = ((u32)(arg7 << 24) / (arg2 << 16)) << 8;
            p->unk4 = 1;
            break;
        }
    } else {
        switch (arg1) {
        case 0:
            rgb[0] = arg5;
            rgb[1] = arg6;
            rgb[2] = arg7;
            p->unk4 = 0;
            break;
        case 1:
            rgb[0] = -arg5;
            rgb[1] = -arg6;
            rgb[2] = -arg7;
            p->unk4 = 0;
            break;
        case 2:
            rgb[0] = arg5;
            rgb[1] = arg6;
            rgb[2] = arg7;
            p->unk4 = 1;
            break;
        }
    }
    p->stepR = rgb[0];
    p->stepG = rgb[1];
    p->stepB = rgb[2];
    p->savedOffset = 0x2C;
    p->workOffset = n * 2 + 0x2C;
    p->rect.x = arg3;
    p->rect.y = y;
    p->rect.w = n;
    p->rect.h = 1;
    StoreImage(&p->rect, (u32 *)((u8 *)p + p->savedOffset));
    w = (u16 *)((u8 *)p + p->workOffset);
    /* read the CLUT back; the original keeps the value in v1 (see REG_ASM) */
    for (i = 0; i < n; i++) {
        v = w[i];
    }
    g_fieldBgCtx.workUsed += e->dataSize;
    return 1;
}

s32 fldbg_setCamera(s32 arg0) {
    BgCamera *c;
    BgFile *h;

    h = g_fieldBgData;
    c = (BgCamera *)((u8 *)h + h->cameraOffset) + arg0;
    *g_fieldBgState.projDist = c->projDist;
    gte_SetGeomScreen(c->projDist);
    g_fieldBgState.viewMatrix->m[0][0] = c->rot[0][0];
    g_fieldBgState.viewMatrix->m[0][1] = c->rot[0][1];
    g_fieldBgState.viewMatrix->m[0][2] = c->rot[0][2];
    g_fieldBgState.viewMatrix->m[1][0] = c->rot[1][0];
    g_fieldBgState.viewMatrix->m[1][1] = c->rot[1][1];
    g_fieldBgState.viewMatrix->m[1][2] = c->rot[1][2];
    g_fieldBgState.viewMatrix->m[2][0] = c->rot[2][0];
    g_fieldBgState.viewMatrix->m[2][1] = c->rot[2][1];
    g_fieldBgState.viewMatrix->m[2][2] = c->rot[2][2];
    g_fieldBgState.viewMatrix->t[0] = c->trans[0];
    g_fieldBgState.viewMatrix->t[1] = c->trans[1];
    g_fieldBgState.viewMatrix->t[2] = c->trans[2];
    h->scrollMaxX = c->scrollMaxX;
    h->scrollMaxY = c->scrollMaxY;
    g_fieldBgState.cameraId = arg0;
    g_fieldBgState.flags |= FLDBG_CAMERA_CHANGED;
    g_gameState->gameData->ctx.field->area->scene->otBias = c->otBias;
    return 1;
}

void func_800C0D38(void) {
}

void fldbg_setRenderContext(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5) {
    g_fieldBgCtx.bufIndex = (u8 *)arg0;
    g_fieldBgCtx.projDist = arg1;
    g_fieldBgCtx.ot[0] = (u32 *)arg2;
    g_fieldBgCtx.ot[1] = (u32 *)arg3;
    g_fieldBgCtx.data = 0;
    g_fieldBgCtx.entryCount = 0;
    g_fieldBgCtx.entries = NULL;
    g_fieldBgCtx.work = 0;
    g_fieldBgCtx.workSize = 0;
    g_fieldBgCtx.workUsed = 0;
    g_fieldBgCtx.viewMatrix = arg4;
    g_fieldBgCtx.heap = arg5;
    g_fieldBgCtx.overlayRgb = arg5;
    g_fieldBgCtx.overlayTinted = arg5;
}

void fldbg_setWorkBuffer(s32 arg0, s32 arg1) {
    g_fieldBgCtx.work = arg0;
    g_fieldBgCtx.workSize = arg1;
    g_fieldBgCtx.workUsed = 0;
}

void fldbg_setData(BgFile *arg0) {
    g_fieldBgData = arg0;
}

s32 fldbg_moveOrigin(s32 arg0, s32 arg1, s32 arg2) {
    u16 x = g_fieldBgData->orgX + arg0;
    u16 y = g_fieldBgData->orgY + arg1;
    u16 z;

    g_fieldBgData->orgX = x;
    g_fieldBgData->orgY = y;
    g_fieldBgData->posX = x;
    g_fieldBgData->posY = y;
    z = g_fieldBgData->orgDepth + arg2;
    g_fieldBgData->orgDepth = z;
    g_fieldBgData->baseDepth = z;
    return 1;
}

s32 fldbg_scrollTo(s16 arg0, s16 arg1, s16 arg2, s32 arg3) {
    s32 f;

    if (!(g_fieldBgState.flags & 0x40)) {
        return 0;
    }
    g_fieldBgState.scrollFrame = 1;
    g_fieldBgState.scrollToX = arg0;
    f = g_fieldBgState.flags & ~0xF;
    g_fieldBgState.scrollToY = arg1;
    g_fieldBgState.scrollFrames = arg2;
    g_fieldBgState.flags = f;
    g_fieldBgState.scrollFrom = g_fieldBgState.scroll;
    if (arg3 == 8) {
        g_fieldBgState.flags = f | 8;
    } else {
        g_fieldBgState.flags = f;
    }
    g_fieldBgState.flags |= 1;
    return 1;
}

s32 fldbg_getScroll(u16 *arg0, u16 *arg1) {
    *arg0 = g_fieldBgState.scroll.x;
    *arg1 = g_fieldBgState.scroll.y;
    return 1;
}

s32 fldbg_setFollow(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 != 0) {
        g_fieldBgState.flags |= 0x40;
        if (arg1 > 0) {
            fldbg_scrollToPlayer(arg1, arg2);
        }
    } else {
        g_fieldBgState.flags &= ~0x40;
    }
    return 1;
}

s32 fldbg_isFollowing(void) {
    if (g_fieldBgState.flags & 0x40) {
        return 1;
    }
    return 0;
}

s32 fldbg_applyGeomOffset(void) {
    BgCamera *c = fldbg_cameraPtr(0);

    c += g_fieldBgState.cameraId;
    if (!(mbg_getStatus() & 2) || g_mbgFrame < 2) {
        gte_SetGeomOffset((s16)c->centerX + g_fieldBgState.geomOffsetX, (s16)c->centerY + g_fieldBgState.geomOffsetY);
    }
    return 1;
}

s32 fldbg_resetGeomOffset(void) {
    if (!(mbg_getStatus() & 2)) {
        gte_SetGeomOffset(0xA0, 0x70);
    }
    return 1;
}

s32 func_800C0FF4(s32 arg0) {
    if (arg0 == -1) {
        g_fieldBgState.unk78 = 3600;
    } else {
        g_fieldBgState.unk78 = arg0 * arg0;
    }
    return 1;
}

s32 fldbg_setFollowHeight(s16 arg0) {
    g_fieldBgState.followHeight = arg0;
    return 1;
}

s32 fldbg_hasScrollLimits(void) {
    if (g_fieldBgState.flags & 0x20) {
        return 1;
    }
    return 0;
}

s32 fldbg_disableScrollLimits(void) {
    BgCamera *c;

    if (!(g_fieldBgState.flags & 0x20)) {
        return 0;
    }
    if (g_fieldBgData != NULL) {
        c = fldbg_cameraPtr(g_fieldBgState.cameraId);
        g_fieldBgState.savedScrollMinX = c->scrollMinX;
        g_fieldBgState.savedScrollMaxX = c->scrollMaxX;
        g_fieldBgState.savedScrollMinY = c->scrollMinY;
        g_fieldBgState.savedScrollMaxY = c->scrollMaxY;
        c->scrollMinX = -0x8000;
        c->scrollMaxX = 0x7FFF;
        c->scrollMinY = -0x8000;
        c->scrollMaxY = 0x7FFF;
        g_fieldBgState.flags &= ~0x20;
    }
    return 1;
}

s32 fldbg_enableScrollLimits(void) {
    BgCamera *c;

    if (g_fieldBgState.flags & 0x20) {
        return 0;
    }
    if (g_fieldBgData != NULL) {
        c = fldbg_cameraPtr(g_fieldBgState.cameraId);
        c->scrollMinX = g_fieldBgState.savedScrollMinX;
        c->scrollMaxX = g_fieldBgState.savedScrollMaxX;
        c->scrollMinY = g_fieldBgState.savedScrollMinY;
        c->scrollMaxY = g_fieldBgState.savedScrollMaxY;
        g_fieldBgState.flags |= 0x20;
    }
    return 1;
}

s32 fldbg_setViewport(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    g_fieldBgState.viewportPos[arg0][0] = arg1;
    g_fieldBgState.viewportPos[arg0][1] = arg2;
    g_fieldBgState.viewportSize[arg0][0] = arg3;
    g_fieldBgState.viewportSize[arg0][1] = arg4;
    return 1;
}

s32 fldbg_setViewportScrollRate(s32 arg0, s16 arg1, s16 arg2) {
    g_fieldBgState.viewportRateX[arg0] = arg1;
    g_fieldBgState.viewportRateY[arg0] = arg2;
    return 1;
}

/* move a background object, clamped to its limits */
s32 fldbg_moveOverlay(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    BgOverlay *o = &((BgOverlay *)((u8 *)g_fieldBgData + g_fieldBgData->overlayOffset))[arg0];
    s16 x = o->baseX + arg1;
    s16 y = o->baseY + arg2;
    s16 z = arg3 + o->baseDepth;

    if (x < o->minX) {
        x = o->minX;
    } else if (x > o->maxX) {
        x = o->maxX;
    }
    if (y < o->minY) {
        y = o->minY;
    } else if (y > o->maxY) {
        y = o->maxY;
    }
    o->baseX = x;
    o->baseDepth = z;
    o->baseY = y;
    o->x = x;
    o->y = y;
    o->depth = z;
    return 1;
}

s32 fldbg_getOverlayColor(s32 arg0, u8 *arg1, u8 *arg2, u8 *arg3) {
    *arg1 = g_fieldBgCtx.overlayRgb[arg0 * 3];
    *arg2 = g_fieldBgCtx.overlayRgb[arg0 * 3 + 1];
    *arg3 = g_fieldBgCtx.overlayRgb[arg0 * 3 + 2];
    return 1;
}

s32 fldbg_setOverlayColor(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    BgFile *h;
    BgOverlay *o;
    BgSprt *s;
    u16 n;
    s32 off;
    u16 i;

    h = g_fieldBgCtx.data;
    g_fieldBgCtx.overlayRgb[arg0 * 3 + 0] = arg1;
    g_fieldBgCtx.overlayRgb[arg0 * 3 + 1] = arg2;
    g_fieldBgCtx.overlayRgb[arg0 * 3 + 2] = arg3;
    o = &((BgOverlay *)((u8 *)h + h->overlayOffset))[arg0];
    n = o->tileCount;
    s = o->sprites;
    off = 0;
    if (*g_fieldBgCtx.bufIndex != 0) {
        off = n;
    }
    for (i = 0; i < n; i++) {
        setRGB0(&s[i + off].sprt, arg1, arg2, arg3);
    }
    return 1;
}

s32 fldbg_showOverlay(s32 arg0, s32 arg1) {
    BgOverlay *o = fldbg_overlayPtr(arg0);
    u8 v;

    if (arg1 != 0) {
        v = o->flags | FLDBG_OVL_VISIBLE;
    } else {
        v = o->flags & ~FLDBG_OVL_VISIBLE;
    }
    o->flags = v;
    return 1;
}

s32 fldbg_setOverlayLoop(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    BgOverlay *o = fldbg_overlayPtr(arg0);

    if (arg1 != 0) {
        o->flags |= FLDBG_OVL_LOOP;
    } else {
        o->flags &= ~FLDBG_OVL_LOOP;
    }
    o->scrollX = arg2;
    o->fracX = 0;
    o->scrollY = arg3;
    o->fracY = 0;
    return 1;
}

s32 fldbg_setOverlayScreen(s32 arg0, s32 arg1) {
    BgOverlay *o = fldbg_overlayPtr(arg0);
    u8 v;

    if (arg1 != 0) {
        v = o->flags | FLDBG_OVL_SCREEN;
    } else {
        v = o->flags & ~FLDBG_OVL_SCREEN;
    }
    o->flags = v;
    return 1;
}

s32 fldbg_setOverlaySkewLoop(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4) {
    BgOverlay *o = fldbg_overlayPtr(arg0);

    if (arg1 != 0) {
        o->flags |= FLDBG_OVL_SKEW_LOOP;
    } else {
        o->flags &= ~FLDBG_OVL_SKEW_LOOP;
    }
    if (arg4 != 0) {
        o->scrollX = arg3;
        o->scrollY = arg2;
        o->skewVertical = 1;
    } else {
        o->scrollX = arg2;
        o->scrollY = arg3;
        o->skewVertical = 0;
    }
    o->fracX = 0;
    o->fracY = 0;
    return 1;
}

s32 fldbg_setOverlayParallax(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    BgOverlay *o = fldbg_overlayPtr(arg0);

    if (arg1 != 0) {
        o->flags |= FLDBG_OVL_PARALLAX;
    } else {
        o->flags &= ~FLDBG_OVL_PARALLAX;
    }
    o->scrollX = arg2;
    o->scrollY = arg3;
    return 1;
}

s32 fldbg_setOverlayPos(s32 arg0, s16 arg1, s16 arg2) {
    BgOverlay *o = fldbg_overlayPtr(arg0);

    o->x = arg1;
    o->baseX = arg1;
    o->y = arg2;
    o->baseY = arg2;
    g_fieldBgState.flags |= FLDBG_CAMERA_CHANGED;
    return 1;
}

s32 fldbg_setOverlayViewport(s32 arg0, s32 arg1) {
    BgOverlay *o = fldbg_overlayPtr(arg0);

    o->viewport = arg1;
    return 1;
}

s32 fldbg_setAnimFrame(s32 arg0, s32 arg1) {
    BgOverlayAnim *p = fldbg_overlayAnimPtr(arg0);

    p->delayCounter = 0;
    p->flags |= FLDBG_ANIM_ACTIVE;
    p->pos = arg1 << 8;
    return 1;
}

s32 fldbg_showAnimFrame(s32 arg0, s32 arg1) {
    s32 i;
    BgOverlayAnim *p;
    BgAnimStep *st;
    BgOverlay *objs;

    i = 0;
    p = fldbg_overlayAnimPtr(arg0);
    st = fldbg_animSteps(p);
    objs = fldbg_overlayPtr(0);
    for (; i < p->stepCount; st++, i++) {
        if (i == arg1) {
            objs[st->overlay].flags |= FLDBG_OVL_VISIBLE;
        } else {
            objs[st->overlay].flags &= ~FLDBG_OVL_VISIBLE;
        }
    }
    p->flags = 1;
    return 1;
}

s32 fldbg_playAnim(s32 arg0, s32 arg1) {
    BgOverlayAnim *p = fldbg_overlayAnimPtr(arg0);
    u8 v;

    if (arg1 != 0) {
        v = p->flags | 6;
    } else {
        v = p->flags & ~6;
    }
    p->flags = v;
    return 1;
}

s32 fldbg_setAnimSpeed(s32 arg0, s32 arg1) {
    fldbg_overlayAnimPtr(arg0)->speed = arg1;
    return 1;
}

s32 fldbg_setAnimDelayAll(s32 arg0, u8 arg1) {
    BgOverlayAnim *p = fldbg_overlayAnimPtr(arg0);
    BgAnimStep *st = fldbg_animSteps(p);
    s32 n = p->stepCount;
    s32 i;

    for (i = 0; i < n; i++) {
        st[i].delay = arg1;
    }
    return 1;
}

s32 fldbg_setAnimStepDelay(s32 arg0, s32 arg1, u8 arg2) {
    fldbg_animSteps(fldbg_overlayAnimPtr(arg0))[arg1].delay = arg2;
    return 1;
}

s32 fldbg_setAnimFlags(s32 arg0, s32 arg1) {
    fldbg_overlayAnimPtr(arg0)->flags |= arg1 & (FLDBG_ANIM_LOOP | FLDBG_ANIM_PINGPONG);
    return 1;
}

s32 fldbg_playAnimRange(s32 arg0, u32 arg1, u32 arg2) {
    BgFile *b = g_fieldBgData;
    BgOverlayAnim *p = &((BgOverlayAnim *)((u8 *)b + b->overlayAnimOffset))[arg0];
    BgAnimStep *st = (BgAnimStep *)((u8 *)b + p->stepOffset);

    p->delayCounter = 0;
    p->flags |= (FLDBG_ANIM_ACTIVE | FLDBG_ANIM_PLAY_ONCE);
    p->pos = arg1 << 8;
    st[arg2].delay = -1;
    if ((p->speed > 0 && arg2 < arg1) || (p->speed < 0 && arg1 < arg2)) {
        p->speed = -p->speed;
    }
    return 1;
}

s32 fldbg_resetAnim(s32 arg0, s32 arg1) {
    BgFile *d = g_fieldBgData;
    BgOverlayAnim *p = &((BgOverlayAnim *)((u8 *)d + d->overlayAnimOffset))[arg0];
    BgOverlay *objs;
    BgAnimStep *st;
    s32 i;

    p->speed = 0x100;
    i = 0;
    p->delayCounter = 0;
    p->flags = 1;
    p->pos = 0;
    objs = (BgOverlay *)((u8 *)d + d->overlayOffset);
    st = (BgAnimStep *)((u8 *)d + p->stepOffset);
    for (; i < p->stepCount; i++) {
        if (arg1 != 0 && i == 0) {
            objs[st[i].overlay].flags |= FLDBG_OVL_VISIBLE;
        } else {
            objs[st[i].overlay].flags &= ~FLDBG_OVL_VISIBLE;
        }
    }
    return 1;
}

s32 fldbg_getEntryType(s32 arg0, u8 *arg1) {
    *arg1 = g_fieldBgCtxEntries[arg0].type;
    return 1;
}

s32 fldbg_getWorkBlock(s32 arg0) {
    return g_fieldBgCtxWork + (arg0 << 9);
}

/* set the camera scroll limits */
s32 fldbg_setScrollLimits(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    BgCamera *c = &((BgCamera *)((u8 *)g_fieldBgData + g_fieldBgData->cameraOffset))[arg0];
    s16 v;

    c->scrollMinX = v = arg1 + 160;
    if (v > c->width - 160) {
        c->scrollMinX = c->width - 160;
    }
    c->scrollMaxX = v = arg2 - 160;
    if (v < 160) {
        c->scrollMaxX = 160;
    }
    c->scrollMinY = v = arg3 + 112;
    if (v > c->height - 112) {
        c->scrollMinY = c->height - 112;
    }
    c->scrollMaxY = v = arg4 - 112;
    if (v < 112) {
        c->scrollMaxY = 112;
    }
    return 1;
}

s32 fldbg_getCamera(u8 *arg0) {
    *arg0 = g_fieldBgState.cameraId;
    return 1;
}

s32 fldbg_setCameraCenter(s32 arg0, s16 arg1, s16 arg2) {
    BgCamera *c = fldbg_cameraPtr(arg0);

    c->centerX = arg1;
    c->centerY = arg2;
    return 1;
}

s32 fldbg_getCameraCenter(s32 arg0, u16 *arg1, u16 *arg2) {
    BgCamera *c = fldbg_cameraPtr(arg0);

    *arg1 = c->centerX;
    *arg2 = c->centerY;
    return 1;
}

s32 fldbg_getClutFadeSize(s32 arg0) {
    return arg0 * 4 + 0x2C;
}

s32 fldbg_allocWork(s32 arg0) {
    s32 r = g_fieldBgCtx.work + g_fieldBgCtx.workUsed;

    g_fieldBgCtx.workUsed += arg0;
    return r;
}

s32 fldbg_resetAllAnims(void) {
    BgFile *d = g_fieldBgData;
    u32 i;
    BgOverlayAnim *p;
    BgOverlay *objs;
    u16 n;

    i = 0;
    p = (BgOverlayAnim *)((u8 *)d + d->overlayAnimOffset);
    objs = (BgOverlay *)((u8 *)d + d->overlayOffset);
    n = d->overlayAnimCount;
    for (; i < n; i++) {
        p[i].speed = 0x100;
        p[i].delayCounter = 0;
        p[i].flags = 1;
        p[i].pos = 0;
        objs[((BgAnimStep *)((u8 *)d + p[i].stepOffset))->overlay].flags |= FLDBG_OVL_VISIBLE;
    }
    return 1;
}

s32 fldbg_scanEntries(void) {
    u32 i;
    s32 ret;
    s32 type;

    ret = 1;
    for (i = 0; i < g_fieldBgCtx.entryCount; i++) {
        type = g_fieldBgCtx.entries[i].type;
        if (type == 0x10) {
            type = 0;
        } else if (type == 0x40) {
            type = ret;
        }
    }
    return ret;
}

s32 func_800C1D08(void) {
    return 1;
}

s32 fldbg_setOverlaySpriteColor(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    BgOverlay *o = fldbg_overlayPtr(arg0);
    u16 n = o->tileCount;
    BgSprt *s = o->sprites;
    u16 i;

    for (i = 0; i < n; i++) {
        setRGB0(&s[i].sprt, arg1, arg2, arg3);
        setRGB0(&s[i + n].sprt, arg1, arg2, arg3);
    }
    return 1;
}

s32 func_800C1DAC(void) {
    return 1;
}

s32 func_800C1DB4(void) {
    return 1;
}

s32 func_800C1DBC(void) {
    return 1;
}
