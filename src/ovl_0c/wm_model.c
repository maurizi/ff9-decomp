#include "ovl_0c.h"

void wm_projectMesh(ModelObj *arg0, u8 arg1) {
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
    gte_SetGeomScreen(g_gameState->gameData->unkA);
    sxy = g_vtxScreenXY;
    sz = g_vtxScreenZ;
    k = 0;
    cnt = arg0->model->boneCount;
    bone = (MATRIX *)arg0->boneMatrices;
    for (i = 0; i < cnt; bone++) {
        n = ((u16 *)g->boneVertexCounts)[i++];
        if (n != 0) {
            gte_SetRotMatrix(&g_gameState->gameData->unk18);
            gte_SetTransMatrix(&g_gameState->gameData->unk18);
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
void wm_initModel(ModelObj *arg0, u8 *arg1) {
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
    arg0->flags |= 2;
    *(volatile u16 *)&arg0->flags = 0;
    arg0->groupOut = (WmModelOut *)(pp + m->boneCount * 32);
}

void wm_buildPackets(ModelObj *arg0, WmTexInfo *arg1, s16 (*arg2)[2]) {
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

void wm_drawMeshC(ModelObj *arg0, u8 arg1) {
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

    ot = g_gameState->gameData->unk10[g_gameState->gameData->unk8];
    g = &arg0->model->groups[arg1];
    if (g_gameState->gameData->unk8 != 0) {
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
        otz = (g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3]) >> 5;
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
        otz >>= 1; /* shifts the previous value: overwritten below */
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
        otz >>= 5;
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
        otz >>= 1;
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
        otz = (g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3]) >> 5;
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
        otz >>= 1;
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
        otz >>= 5;
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
        otz >>= 1;
        addPrim(&ot[otz], pF3);
    }
}

void wm_applyScale(ModelObj *arg0) {
    MATRIX m;

    if (arg0->flags & 2) {
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

void wm_cullBehindCamera(ModelObj *arg0) {
    SVECTOR v;
    s32 sz;

    arg0->flags &= ~MDL_BEHIND_CAMERA;
    if (arg0->parent != NULL) {
        arg0->flags |= arg0->parent->flags & MDL_BEHIND_CAMERA;
    } else {
        v.vx = arg0->worldMatrix.t[0];
        v.vy = arg0->worldMatrix.t[1];
        v.vz = arg0->worldMatrix.t[2];
        gte_SetRotMatrix(&g_gameState->gameData->unk18);
        gte_SetTransMatrix(&g_gameState->gameData->unk18);
        gte_ldv0(&v);
        gte_rtps();
        gte_stsz(&sz);
        if (sz == 0) {
            arg0->flags |= MDL_BEHIND_CAMERA;
        }
    }
}

void wm_calcBoneMatrices(ModelObj *arg0, u16 arg1) {
    SVECTOR sv;
    u16 nb;
    VECTOR *vec;
    WmAnimHdr *anim;
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

    anim = arg0->anim;
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
    key = (WmAnimKey *)anim->rotKeys;
    key2 = (WmAnimKey *)anim->rotKeysLo;
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

void wm_projectMeshClipY(ModelObj *arg0, u8 arg1) {
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

    gte_SetGeomScreen(g_gameState->gameData->unkA);
    sxy = g_vtxScreenXY;
    sz = g_vtxScreenZ;
    k = 0;
    g = &arg0->model->groups[arg1];
    bone = (MATRIX *)arg0->boneMatrices;
    cnt = arg0->model->boneCount;
    ymax = (s16)arg0->clipY;
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
            gte_SetRotMatrix(&g_gameState->gameData->unk18);
            gte_SetTransMatrix(&g_gameState->gameData->unk18);
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

void wm_drawMeshClipY(ModelObj *arg0, u8 arg1) {
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
    ot = g_gameState->gameData->unk10[g_gameState->gameData->unk8];
    if (g_gameState->gameData->unk8 != 0) {
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
        otz >>= 5;
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
        addPrim(&ot[otz >> 3], pFT3);
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
        otz >>= 5;
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
        addPrim(&ot[otz >> 3], pGT3);
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
        otz >>= 5;
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
        addPrim(&ot[otz >> 3], pG3);
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
        otz >>= 5;
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
        addPrim(&ot[otz >> 3], pF3);
    }
}

s32 wm_getSpriteFrameCount(u16 *arg0) {
    return *arg0 & 0x7FFF;
}

s32 wm_drawSprite3D(s32 arg0, WmSprHdr *h, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 arg7,
                   s32 arg8, s32 arg9, s32 argA) {
    VECTOR s;
    SVECTOR zero;
    MATRIX m;
    WmSprCtx c;
    u16 *p;
    u8 *b;
    s32 n;
    s32 f;
    u16 *o;

    if (rot == NULL) {
        rot = &zero;
        ((s32 *)&zero)[0] = 0;
        ((s32 *)&zero)[1] = 0;
    }
    if (arg9 != 0) {
        h->clut += arg9;
    }
    n = h->frameCount & 0x7FFF;
    f = frame;
    if (f >= n) {
        f = frame % n;
    }
    o = &h->frameOffsets[f];
    n = n * 2 + 8;
    b = (u8 *)h + n;
    p = (u16 *)b;
    f = *p++;
    c.uvs = p;
    c.colors = p + f + 1;
    c.tpage = h->tpage | ((abr & 3) << 5);
    c.clut = h->clut;
    c.halfH = (h->height - 1) * 2;
    c.halfW = (h->width - 1) * 2;
    n = *o;
    c.quadCount = ((u8 *)h)[n++];
    h = (WmSprHdr *)((u8 *)h + n);
    gte_SetRotMatrix(&g_gameState->gameData->unk18);
    gte_SetTransMatrix(&g_gameState->gameData->unk18);
    gte_ldv0(pos);
    gte_rtv0tr();
    math_rotMatrixYXZ(rot, &m);
    if (scale >= 0) {
        s.vx = s.vy = s.vz = scale;
        ScaleMatrix(&m, &s);
    }
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.primCode = (abr == 0xFF) ? 0x2C : 0x2E;
    c.otBias = -g_worldOtBias;
    c.fade = arg7;
    c.sizeShift = argA;
    return wm_emitSprite3D((u8 *)h, &c, arg0, arg8);
}

/* upload every image and clut of an image pack (c walks the clut RECTs in the second loop) */
void wm_loadImagePack(WmImagePack *arg0) {
    RECT r;
    WmImagePackClut *c;
    s16 *img;
    s32 i;
    s32 n;
    s32 n2;
    s32 ofs;
    s32 size;
    u8 *pix;

    ofs = arg0->clutOfs;
    c = (WmImagePackClut *)((u8 *)arg0 + ofs);
    ofs = arg0->imageOfs;
    i = 0;
    img = (s16 *)((u8 *)arg0 + ofs);
    ofs = c->dataOfs;
    pix = (u8 *)arg0 + ofs;
    n = arg0->imageCount;
    for (; i < n; i++) {
        r.x = img[0];
        r.y = img[1];
        r.w = img[2];
        r.h = img[3];
        size = r.w * r.h * 2;
        img += 4;
        LoadImage(&r, (u32 *)img);
        img = (s16 *)((u8 *)img + size);
    }
    n2 = c->count;
    c = (WmImagePackClut *)c->rects;
    for (i = 0; i < n2; i++) {
        r.x = ((RECT *)c)->x;
        r.y = ((RECT *)c)->y;
        r.w = ((RECT *)c)->w;
        r.h = ((RECT *)c)->h;
        LoadImage(&r, (u32 *)pix);
        c = (WmImagePackClut *)((RECT *)c + 1);
        pix += r.w * r.h * 2;
    }
}
