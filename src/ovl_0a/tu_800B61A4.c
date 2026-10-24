#include "ovl_0a.h"

void fldmdl_projectMesh(Unk800C95C8 *arg0, u8 arg1) {
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

    g = &arg0->unk8->unk10[arg1];
    gte_SetGeomScreen(g_gameState->unk1C->unkA);
    sxy = g_vtxScreenXY;
    sz = g_vtxScreenZ;
    k = 0;
    cnt = arg0->unk8->unk2;
    bone = (MATRIX *)arg0->unk20;
    for (i = 0; i < cnt; bone++) {
        n = ((u16 *)g->unk14)[i++];
        if (n != 0) {
            gte_SetRotMatrix(&g_gameState->unk1C->unk18);
            gte_SetTransMatrix(&g_gameState->unk1C->unk18);
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
            vtx = &((SVECTOR *)g->unk18)[k];
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
void fldmdl_initModel(Unk800C95C8 *arg0, u8 *arg1) {
    WmModel *m;
    WmModelGroup *g;
    u32 i;
    u32 n;
    u32 k;
    s32 size;
    u8 *pp;
    WmModelGroup *p;
    WmModelGroup *g2;

    m = arg0->unk8;
    if (!(m->unk0 & 1)) {
        i = 0;
        m->unk0 |= 1;
        m->unkC = (s32)m + m->unkC;
        g = (WmModelGroup *)((u8 *)m + (s32)m->unk10);
        m->unk10 = g;
        for (p = g; i < m->unk3; i++, p++) {
            p->unk14 = (u8 *)m + (s32)p->unk14;
            p->unk18 = (u8 *)m + (s32)p->unk18;
            p->unk1C = (u8 *)m + (s32)p->unk1C;
            p->unk20 = (void *)((u8 *)m + (s32)p->unk20);
            p->unk24 = (void *)((u8 *)m + (s32)p->unk24);
        }
    }
    g2 = arg0->unk8->unk10;
    size = 0;
    if (IS_OFFSET(g2)) {
        g2 = (WmModelGroup *)((u8 *)g2 + (s32)arg0->unk8);
    }
    n = arg0->unk8->unk3;
    for (k = 0; k < n; k++) {
        size += g2->unk0;
        g2++;
    }
    arg0->unk54 = 0x1000;
    arg0->unk56 = 0x1000;
    arg0->unk58 = 0x1000;
    arg0->unk1C = NULL;
    arg0->unk60 = NULL;
    arg0->unk5C = 0;
    arg0->unk14 = 0;
    arg0->unk74 = 0;
    arg0->unk76 = 0;
    arg0->unk78 = 0;
    arg0->unk10 = arg1;
    arg0->unk50 = NULL;
    pp = arg1 + size * 2;
    arg0->unk20 = pp;
    arg0->unk0 |= 2;
    *(volatile u16 *)&arg0->unk0 = 0;
    arg0->unk44 = (WmModelOut *)(pp + m->unk2 * 32);
}

void fldmdl_buildPackets(Unk800C95C8 *arg0, WmTexInfo *arg1, s16 (*arg2)[2]) {
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

    prim = arg0->unk10;
    m = arg0->unk8;
    arg0->unk18 = arg1;
    o = arg0->unk44;
    for (k = 0; k < m->unk3; k++) {
        out = &o[k];
        g = &m->unk10[k];
        col = g->unk24;
        out->unk4 = prim;
        out->unk0 = 0;
        out->unk8 = 0;
        for (j = 0; j < 2; j++) {
            src = g->unk1C;
            pFT4 = (POLY_FT4 *)prim;
            sFT4 = (WmSrcFT4 *)src;
            for (i = 0; i < g->unk2; i++) {
                setPolyFT4(pFT4);
                pFT4->r0 = sFT4->unk10[0];
                pFT4->g0 = sFT4->unk10[1];
                pFT4->b0 = sFT4->unk10[2];
                pFT4->u0 = g->unk20[sFT4->unk8[0]][0];
                pFT4->v0 = g->unk20[sFT4->unk8[0]][1];
                pFT4->u1 = g->unk20[sFT4->unk8[1]][0];
                pFT4->v1 = g->unk20[sFT4->unk8[1]][1];
                pFT4->u2 = g->unk20[sFT4->unk8[2]][0];
                pFT4->v2 = g->unk20[sFT4->unk8[2]][1];
                pFT4->u3 = g->unk20[sFT4->unk8[3]][0];
                pFT4->v3 = g->unk20[sFT4->unk8[3]][1];
                pFT4->clut = arg0->unk18[sFT4->unk13].unk2;
                pFT4->tpage = tp = arg0->unk18[sFT4->unk13].unk0 & ~0x20;
                if (sFT4->unk15 & 1) {
                    pFT4->tpage = tp | (sFT4->unk15 & 0xE0);
                    setSemiTrans(pFT4, 1);
                }
                prim += sizeof(POLY_FT4);
                du = arg2[sFT4->unk13][0];
                dv = arg2[sFT4->unk13][1];
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
            for (i = 0; i < g->unk4; i++) {
                setPolyFT3(pFT3);
                pFT3->r0 = sFT3->unk8[0];
                pFT3->g0 = sFT3->unk8[1];
                pFT3->b0 = sFT3->unk8[2];
                pFT3->u0 = g->unk20[sFT3->unkC[0]][0];
                pFT3->v0 = g->unk20[sFT3->unkC[0]][1];
                pFT3->u1 = g->unk20[sFT3->unkC[1]][0];
                pFT3->v1 = g->unk20[sFT3->unkC[1]][1];
                pFT3->u2 = g->unk20[sFT3->unkC[2]][0];
                pFT3->v2 = g->unk20[sFT3->unkC[2]][1];
                pFT3->clut = arg0->unk18[sFT3->unk6].unk2;
                pFT3->tpage = tp = arg0->unk18[sFT3->unk6].unk0 & ~0x20;
                if (sFT3->unk12 & 1) {
                    pFT3->tpage = tp | (sFT3->unk12 & 0xE0);
                    setSemiTrans(pFT3, 1);
                }
                prim += sizeof(POLY_FT3);
                du = arg2[sFT3->unk6][0];
                dv = arg2[sFT3->unk6][1];
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
            for (i = 0; i < g->unk6; i++) {
                setPolyGT4(pGT4);
                c = col[sGT4->unk10[0]];
                pGT4->r0 = c[0];
                pGT4->g0 = c[1];
                pGT4->b0 = c[2];
                c = col[sGT4->unk10[1]];
                pGT4->r1 = c[0];
                pGT4->g1 = c[1];
                pGT4->b1 = c[2];
                c = col[sGT4->unk10[2]];
                pGT4->r2 = c[0];
                pGT4->g2 = c[1];
                pGT4->b2 = c[2];
                c = col[sGT4->unk10[3]];
                pGT4->r3 = c[0];
                pGT4->g3 = c[1];
                pGT4->b3 = c[2];
                pGT4->u0 = g->unk20[sGT4->unk8[0]][0];
                pGT4->v0 = g->unk20[sGT4->unk8[0]][1];
                pGT4->u1 = g->unk20[sGT4->unk8[1]][0];
                pGT4->v1 = g->unk20[sGT4->unk8[1]][1];
                pGT4->u2 = g->unk20[sGT4->unk8[2]][0];
                pGT4->v2 = g->unk20[sGT4->unk8[2]][1];
                pGT4->u3 = g->unk20[sGT4->unk8[3]][0];
                pGT4->v3 = g->unk20[sGT4->unk8[3]][1];
                pGT4->clut = arg0->unk18[sGT4->unk1D].unk2;
                pGT4->tpage = tp = arg0->unk18[sGT4->unk1D].unk0 & ~0x20;
                if (sGT4->unk1C & 1) {
                    pGT4->tpage = tp | (sGT4->unk1C & 0xE0);
                    setSemiTrans(pGT4, 1);
                }
                prim += sizeof(POLY_GT4);
                du = arg2[sGT4->unk1D][0];
                dv = arg2[sGT4->unk1D][1];
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
            for (i = 0; i < g->unk8; i++) {
                setPolyGT3(pGT3);
                c = col[sGT3->unk10[0]];
                pGT3->r0 = c[0];
                pGT3->g0 = c[1];
                pGT3->b0 = c[2];
                c = col[sGT3->unk10[1]];
                pGT3->r1 = c[0];
                pGT3->g1 = c[1];
                pGT3->b1 = c[2];
                c = col[sGT3->unk10[2]];
                pGT3->r2 = c[0];
                pGT3->g2 = c[1];
                pGT3->b2 = c[2];
                pGT3->u0 = g->unk20[sGT3->unk6[0]][0];
                pGT3->v0 = g->unk20[sGT3->unk6[0]][1];
                pGT3->u1 = g->unk20[sGT3->unk6[1]][0];
                pGT3->v1 = g->unk20[sGT3->unk6[1]][1];
                pGT3->u2 = g->unk20[sGT3->unk6[2]][0];
                pGT3->v2 = g->unk20[sGT3->unk6[2]][1];
                pGT3->clut = arg0->unk18[sGT3->unk16].unk2;
                pGT3->tpage = tp = arg0->unk18[sGT3->unk16].unk0 & ~0x20;
                if (sGT3->unkF & 1) {
                    pGT3->tpage = tp | (sGT3->unkF & 0xE0);
                    setSemiTrans(pGT3, 1);
                }
                prim += sizeof(POLY_GT3);
                du = arg2[sGT3->unk16][0];
                dv = arg2[sGT3->unk16][1];
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
            for (i = 0; i < g->unkA; i++) {
                setPolyG4(pG4);
                c = col[sG4->unk8[0]];
                pG4->r0 = c[0];
                pG4->g0 = c[1];
                pG4->b0 = c[2];
                c = col[sG4->unk8[1]];
                pG4->r1 = c[0];
                pG4->g1 = c[1];
                pG4->b1 = c[2];
                c = col[sG4->unk8[2]];
                pG4->r2 = c[0];
                pG4->g2 = c[1];
                pG4->b2 = c[2];
                c = col[sG4->unk8[3]];
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
            for (i = 0; i < g->unkC; i++) {
                setPolyG3(pG3);
                c = col[sG3->unk8[0]];
                pG3->r0 = c[0];
                pG3->g0 = c[1];
                pG3->b0 = c[2];
                c = col[sG3->unk8[1]];
                pG3->r1 = c[0];
                pG3->g1 = c[1];
                pG3->b1 = c[2];
                c = col[sG3->unk8[2]];
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
            for (i = 0; i < g->unkE; i++) {
                setPolyF4(pF4);
                pF4->r0 = sF4->unk8[0];
                pF4->g0 = sF4->unk8[1];
                pF4->b0 = sF4->unk8[2];
                prim += sizeof(POLY_F4);
                pF4++;
                sF4++;
                src += sizeof(WmSrcF4);
            }
            for (i = 0; i < g->unk10; i++) {
                pF3 = (POLY_F3 *)prim;
                sF3 = (WmSrcF3 *)src;
                setPolyF3(pF3);
                pF3->r0 = sF3->unk8[0];
                pF3->g0 = sF3->unk8[1];
                pF3->b0 = sF3->unk8[2];
                prim += sizeof(POLY_F3);
                src += sizeof(WmSrcF3);
            }
        }
    }
}

void fldmdl_drawMeshC(Unk800C95C8 *arg0, u8 arg1) {
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

    ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8];
    g = &arg0->unk8->unk10[arg1];
    if (g_gameState->unk1C->unk8 != 0) {
        prim = (u8 *)arg0->unk44[arg1].unk4 + g->unk0;
    } else {
        prim = arg0->unk44[arg1].unk4;
    }

    n = g->unk2;
    pFT4 = (POLY_FT4 *)prim;
    ot -= g->unk13;
    sFT4 = (WmSrcFT4 *)(void *)g->unk1C;
    for (i = 0; i < n; sFT4++, pFT4++) {
        a = *(u32 *)&sFT4->unk0[0];
        b = *(u32 *)&sFT4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT4->unk15 & 2)) {
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

    n = g->unk4;
    pFT3 = (POLY_FT3 *)pFT4;
    sFT3 = (WmSrcFT3 *)sFT4;
    for (i = 0; i < n; sFT3++, pFT3++) {
        a = *(u32 *)&sFT3->unk0[0];
        i2 = sFT3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT3->unk12 & 2)) {
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

    n = g->unk6;
    pGT4 = (POLY_GT4 *)pFT3;
    sGT4 = (WmSrcGT4 *)sFT3;
    for (i = 0; i < n; sGT4++, pGT4++) {
        a = *(u32 *)&sGT4->unk0[0];
        b = *(u32 *)&sGT4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT4->unk1C & 2)) {
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

    n = g->unk8;
    pGT3 = (POLY_GT3 *)pGT4;
    sGT3 = (WmSrcGT3 *)sGT4;
    for (i = 0; i < n; sGT3++, pGT3++) {
        a = *(u32 *)&sGT3->unk0[0];
        i2 = sGT3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT3->unkF & 2)) {
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

    n = g->unkA;
    pG4 = (POLY_G4 *)pGT3;
    sG4 = (WmSrcG4 *)sGT3;
    for (i = 0; i < n; sG4++, pG4++) {
        a = *(u32 *)&sG4->unk0[0];
        b = *(u32 *)&sG4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG4->unk14 & 2)) {
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

    n = g->unkC;
    pG3 = (POLY_G3 *)pG4;
    sG3 = (WmSrcG3 *)sG4;
    for (i = 0; i < n; sG3++, pG3++) {
        a = *(u32 *)&sG3->unk0[0];
        i2 = sG3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG3->unk13 & 2)) {
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

    n = g->unkE;
    pF4 = (POLY_F4 *)pG3;
    sF4 = (WmSrcF4 *)sG3;
    for (i = 0; i < n; sF4++, pF4++) {
        a = *(u32 *)&sF4->unk0[0];
        b = *(u32 *)&sF4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF4->unkD & 2)) {
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

    n = g->unk10;
    pF3 = (POLY_F3 *)pF4;
    sF3 = (WmSrcF3 *)sF4;
    for (i = 0; i < n; sF3++, pF3++) {
        a = *(u32 *)&sF3->unk0[0];
        i2 = sF3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF3->unkB & 2)) {
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

void fldmdl_applyScale(Unk800C95C8 *arg0) {
    MATRIX m;

    if (arg0->unk0 & 2) {
        *(s32 *)&m.m[0][0] = 0x1000;
        *(s32 *)&m.m[0][2] = 0;
        *(s32 *)&m.m[1][1] = 0x1000;
        *(s32 *)&m.m[2][0] = 0;
        *(s32 *)&m.m[2][2] = 0x1000;
        m.m[0][0] = arg0->unk54;
        gte_SetRotMatrix(&arg0->unk24);
        gte_ldclmv(&m);
        gte_rtir();
        m.m[1][1] = arg0->unk56;
        gte_stclmv(&arg0->unk24);
        gte_ldclmv(&m.m[0][1]);
        gte_rtir();
        m.m[2][2] = arg0->unk58;
        gte_stclmv(&arg0->unk24.m[0][1]);
        gte_ldclmv(&m.m[0][2]);
        gte_rtir();
        gte_stclmv(&arg0->unk24.m[0][2]);
    }
}

void fldmdl_cullBehindCamera(Unk800C95C8 *arg0) {
    SVECTOR v;
    s32 sz;

    arg0->unk0 &= ~0x10;
    if (arg0->unk1C != NULL) {
        arg0->unk0 |= arg0->unk1C->unk0 & 0x10;
    } else {
        v.vx = arg0->unk24.t[0];
        v.vy = arg0->unk24.t[1];
        v.vz = arg0->unk24.t[2];
        gte_SetRotMatrix(&g_gameState->unk1C->unk18);
        gte_SetTransMatrix(&g_gameState->unk1C->unk18);
        gte_ldv0(&v);
        gte_rtps();
        gte_stsz(&sz);
        if (sz == 0) {
            arg0->unk0 |= 0x10;
        }
    }
}

void fldmdl_calcBoneMatrices(Unk800C95C8 *arg0, u16 arg1) {
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

    anim = (WmAnim *)arg0->unkC;
    if (anim == NULL) {
        if (arg0->unk1C != NULL) {
            /* attached to a bone of the parent model */
            w0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[0];
            w1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[1];
            w2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[2];
            w3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[3];
            ((s32 *)arg0->unk20)[0] = w0;
            ((s32 *)arg0->unk20)[1] = w1;
            ((s32 *)arg0->unk20)[2] = w2;
            ((s32 *)arg0->unk20)[3] = w3;
            w0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[4];
            w1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[5];
            w2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[6];
            w3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[7];
            ((s32 *)arg0->unk20)[4] = w0;
            ((s32 *)arg0->unk20)[5] = w1;
            ((s32 *)arg0->unk20)[6] = w2;
            ((s32 *)arg0->unk20)[7] = w3;
            gte_SetRotMatrix(arg0->unk20);
            gte_ldv0(&arg0->unk74);
            gte_rtv0();
            gte_stsv(&sv);
            ((MATRIX *)arg0->unk20)->t[0] += sv.vx;
            ((MATRIX *)arg0->unk20)->t[1] += sv.vy;
            ((MATRIX *)arg0->unk20)->t[2] += sv.vz;
        } else {
            ((MATRIX *)arg0->unk20)->m[0][0] = arg0->unk24.m[0][0];
            ((MATRIX *)arg0->unk20)->m[0][1] = -arg0->unk24.m[0][1];
            ((MATRIX *)arg0->unk20)->m[0][2] = -arg0->unk24.m[0][2];
            ((MATRIX *)arg0->unk20)->m[1][0] = arg0->unk24.m[1][0];
            ((MATRIX *)arg0->unk20)->m[1][1] = -arg0->unk24.m[1][1];
            ((MATRIX *)arg0->unk20)->m[1][2] = -arg0->unk24.m[1][2];
            ((MATRIX *)arg0->unk20)->m[2][0] = arg0->unk24.m[2][0];
            ((MATRIX *)arg0->unk20)->m[2][1] = -arg0->unk24.m[2][1];
            ((MATRIX *)arg0->unk20)->m[2][2] = -arg0->unk24.m[2][2];
            ((MATRIX *)arg0->unk20)->t[0] = arg0->unk24.t[0];
            ((MATRIX *)arg0->unk20)->t[1] = arg0->unk24.t[1];
            ((MATRIX *)arg0->unk20)->t[2] = arg0->unk24.t[2];
        }
        return;
    }
    nb = arg0->unk8->unk2;
    bones = (MATRIX *)0x1F800018;
    if (nb >= 31) {
        bones = (MATRIX *)arg0->unk20;
    }
    m = bones;
    vec = (VECTOR *)0x1F800008;
    vec->vx = 0;
    vec->vy = 0;
    key = anim->unkC;
    key2 = anim->unk10;
    rot = (SVECTOR *)0x1F800000;
    if (anim->unkA & 1) {
        bones->t[0] = anim->unk4[0];
    } else {
        bones->t[0] = ((s16 *)((u8 *)anim + (u16)anim->unk4[0]))[arg1];
    }
    if (anim->unkA & 2) {
        m->t[1] = anim->unk4[1];
    } else {
        m->t[1] = ((s16 *)((u8 *)anim + (u16)anim->unk4[1]))[arg1];
    }
    if (anim->unkA & 4) {
        m->t[2] = anim->unk4[2];
    } else {
        m->t[2] = ((s16 *)((u8 *)anim + (u16)anim->unk4[2]))[arg1];
    }
    end = &bones[nb];
    do {
        gte_ldrotkey(key, anim, arg1, rot);
        if (key2 != NULL) {
            gte_ldrotkey_lo(key2, anim, arg1, rot);
            key2++;
        }
        key++;
        func_80031264(rot, m);
        m++;
    } while (m < end);
    if (arg0->unk1C != NULL) {
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[0];
            x1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[1];
            x2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[2];
            x3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[3];
            ((s32 *)bones)[0] = x0;
            ((s32 *)bones)[1] = x1;
            ((s32 *)bones)[2] = x2;
            ((s32 *)bones)[3] = x3;
        }
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[4];
            x1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[5];
            x2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[6];
            x3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[7];
            ((s32 *)bones)[4] = x0;
            ((s32 *)bones)[5] = x1;
            ((s32 *)bones)[6] = x2;
            ((s32 *)bones)[7] = x3;
        }
        gte_SetRotMatrix(bones);
        gte_ldv0(&arg0->unk74);
        gte_rtv0();
        gte_stsv(&sv);
        bones->t[0] += sv.vx;
        bones->t[1] += sv.vy;
        bones->t[2] += sv.vz;
    } else {
        gte_SetRotMatrix(&arg0->unk24);
        gte_SetTransMatrix(&arg0->unk24);
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
    b = (WmBone *)arg0->unk8->unkC + 1;
    m = bones + 1;
    p = &bones[b->unk3];
    do {
        gte_SetRotMatrix(p);
        gte_ldclmv(m);
        gte_rtir();
        gte_SetTransMatrix(p);
        gte_stclmv(m);
        gte_ldclmv(&m->m[0][1]);
        gte_rtir();
        vec->vz = b->unk0;
        gte_stclmv(&m->m[0][1]);
        gte_ldclmv(&m->m[0][2]);
        gte_rtir();
        b++;
        gte_stclmv(&m->m[0][2]);
        gte_ldlv0(vec);
        gte_rtv0tr();
        p = &bones[b->unk3];
        gte_stlvnl(m->t);
        m++;
    } while (m < end);
    if (nb < 31) {
        m = (MATRIX *)arg0->unk20;
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

void fldmdl_addGroupRgb(Unk800C95C8 *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    WmModelGroup *g;
    u8 *p;
    u32 j;
    u32 k;

    if (arg0->unk0 & 0x10) {
        return;
    }
    g = &arg0->unk8->unk10[arg1];
    for (k = 0; k < 2; k++) {
        p = arg0->unk44[arg1].unk4;
        if (k != 0) {
            p += g->unk0;
        }
        for (j = 0; j < g->unk2; j++) {
            *(u32 *)&((POLY_FT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT4 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_FT4);
        }
        for (j = 0; j < g->unk4; j++) {
            *(u32 *)&((POLY_FT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT3 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_FT3);
        }
        for (j = 0; j < g->unk6; j++) {
            *(u32 *)&((POLY_GT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT4 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT4 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r2, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT4 *)p)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r3, arg2, arg3, arg4);
            p += sizeof(POLY_GT4);
        }
        for (j = 0; j < g->unk8; j++) {
            *(u32 *)&((POLY_GT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT3 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT3 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r2, arg2, arg3, arg4);
            p += sizeof(POLY_GT3);
        }
        for (j = 0; j < g->unkA; j++) {
            *(u32 *)&((POLY_G4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_G4 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_G4 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r2, arg2, arg3, arg4);
            *(u32 *)&((POLY_G4 *)p)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r3, arg2, arg3, arg4);
            p += sizeof(POLY_G4);
        }
        for (j = 0; j < g->unkC; j++) {
            *(u32 *)&((POLY_G3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_G3 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_G3 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r2, arg2, arg3, arg4);
            p += sizeof(POLY_G3);
        }
        for (j = 0; j < g->unkE; j++) {
            *(u32 *)&((POLY_F4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F4 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_F4);
        }
        for (j = 0; j < g->unk10; j++) {
            *(u32 *)&((POLY_F3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F3 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_F3);
        }
    }
}

void fldmdl_addGroupRgbCur(Unk800C95C8 *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    WmModelGroup *g;
    u8 *p;
    u32 j;

    if (arg0->unk0 & 0x10) {
        return;
    }
    g = &arg0->unk8->unk10[arg1];
    p = arg0->unk44[arg1].unk4;
    if (g_gameState->unk1C->unk8 != 0) {
        p += g->unk0;
    }
    for (j = 0; j < g->unk2; j++) {
        *(u32 *)&((POLY_FT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT4 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_FT4);
    }
    for (j = 0; j < g->unk4; j++) {
        *(u32 *)&((POLY_FT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_FT3 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_FT3);
    }
    for (j = 0; j < g->unk6; j++) {
        *(u32 *)&((POLY_GT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT4 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT4 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r2, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT4 *)p)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT4 *)p)->r3, arg2, arg3, arg4);
        p += sizeof(POLY_GT4);
    }
    for (j = 0; j < g->unk8; j++) {
        *(u32 *)&((POLY_GT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT3 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT3 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_GT3 *)p)->r2, arg2, arg3, arg4);
        p += sizeof(POLY_GT3);
    }
    for (j = 0; j < g->unkA; j++) {
        *(u32 *)&((POLY_G4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_G4 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_G4 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r2, arg2, arg3, arg4);
        *(u32 *)&((POLY_G4 *)p)->r3 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G4 *)p)->r3, arg2, arg3, arg4);
        p += sizeof(POLY_G4);
    }
    for (j = 0; j < g->unkC; j++) {
        *(u32 *)&((POLY_G3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_G3 *)p)->r1 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_G3 *)p)->r2 = fldmdl_addRgbClamped(*(u32 *)&((POLY_G3 *)p)->r2, arg2, arg3, arg4);
        p += sizeof(POLY_G3);
    }
    for (j = 0; j < g->unkE; j++) {
        *(u32 *)&((POLY_F4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F4 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_F4);
    }
    for (j = 0; j < g->unk10; j++) {
        *(u32 *)&((POLY_F3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)&((POLY_F3 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_F3);
    }
}

void fldmdl_tintGroup(Unk800C95C8 *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    u32 *col;
    WmModelGroup *g;
    u8 *p;
    u8 *s;
    u32 j;
    u32 k;
    u8 c;

    if (arg0->unk0 & 0x10) {
        return;
    }
    g = &arg0->unk8->unk10[arg1];
    for (k = 0; k < 2; k++) {
        s = g->unk1C;
        p = arg0->unk44[arg1].unk4;
        if (k != 0) {
            p += g->unk0;
        }
        for (j = 0; j < g->unk2; j++) {
            c = ((POLY_FT4 *)p)->code;
            *(u32 *)&((POLY_FT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT4 *)s)->unk10, arg2, arg3, arg4);
            ((POLY_FT4 *)p)->code = c;
            p += sizeof(POLY_FT4);
            s += sizeof(WmSrcFT4);
        }

        for (j = 0; j < g->unk4; j++) {
            c = ((POLY_FT3 *)p)->code;
            *(u32 *)&((POLY_FT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT3 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_FT3 *)p)->code = c;
            p += sizeof(POLY_FT3);
            s += sizeof(WmSrcFT3);
        }

        for (j = 0; j < g->unk6; j++) {
            c = ((POLY_GT4 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[0]];
            *(u32 *)&((POLY_GT4 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[1]];
            *(u32 *)&((POLY_GT4 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[2]];
            *(u32 *)&((POLY_GT4 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[3]];
            *(u32 *)&((POLY_GT4 *)p)->r3 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_GT4 *)p)->code = c;
            p += sizeof(POLY_GT4);
            s += sizeof(WmSrcGT4);
        }

        for (j = 0; j < g->unk8; j++) {
            c = ((POLY_GT3 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[0]];
            *(u32 *)&((POLY_GT3 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[1]];
            *(u32 *)&((POLY_GT3 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[2]];
            *(u32 *)&((POLY_GT3 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_GT3 *)p)->code = c;
            p += sizeof(POLY_GT3);
            s += sizeof(WmSrcGT3);
        }

        for (j = 0; j < g->unkA; j++) {
            c = ((POLY_G4 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[0]];
            *(u32 *)&((POLY_G4 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[1]];
            *(u32 *)&((POLY_G4 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[2]];
            *(u32 *)&((POLY_G4 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[3]];
            *(u32 *)&((POLY_G4 *)p)->r3 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_G4 *)p)->code = c;
            p += sizeof(POLY_G4);
            s += sizeof(WmSrcG4);
        }

        for (j = 0; j < g->unkC; j++) {
            c = ((POLY_G3 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[0]];
            *(u32 *)&((POLY_G3 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[1]];
            *(u32 *)&((POLY_G3 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[2]];
            *(u32 *)&((POLY_G3 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_G3 *)p)->code = c;
            p += sizeof(POLY_G3);
            s += sizeof(WmSrcG3);
        }

        for (j = 0; j < g->unkE; j++) {
            c = ((POLY_F4 *)p)->code;
            *(u32 *)&((POLY_F4 *)p)->r0 |= fldmdl_addRgbClamped(*(u32 *)((WmSrcF4 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_F4 *)p)->code = c;
            p += sizeof(POLY_F4);
            s += sizeof(WmSrcF4);
        }

        for (j = 0; j < g->unk10; j++) {
            c = ((POLY_F3 *)p)->code;
            *(u32 *)&((POLY_F3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcF3 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_F3 *)p)->code = c;
            p += sizeof(POLY_F3);
            s += sizeof(WmSrcF3);
        }
    }
}

/* recolour a model group in the current draw buffer (linked prims only) */
void fldmdl_tintGroupCur(Unk800C95C8 *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    u32 *col;
    WmModelGroup *g;
    u8 *p;
    u8 *s;
    u32 j;
    u8 c;

    if (arg0->unk0 & 0x10) {
        return;
    }
    g = &arg0->unk8->unk10[arg1];
    p = arg0->unk44[arg1].unk4;
    s = g->unk1C;
    if (g_gameState->unk1C->unk8 != 0) {
        p += g->unk0;
    }
    for (j = 0; j < g->unk2; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_FT4 *)p)->code;
            *(u32 *)&((POLY_FT4 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT4 *)s)->unk10, arg2, arg3, arg4);
            ((POLY_FT4 *)p)->code = c;
        }
        p += sizeof(POLY_FT4);
        s += sizeof(WmSrcFT4);
    }

    for (j = 0; j < g->unk4; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_FT3 *)p)->code;
            *(u32 *)&((POLY_FT3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcFT3 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_FT3 *)p)->code = c;
        }
        p += sizeof(POLY_FT3);
        s += sizeof(WmSrcFT3);
    }

    for (j = 0; j < g->unk6; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_GT4 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[0]];
            *(u32 *)&((POLY_GT4 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[1]];
            *(u32 *)&((POLY_GT4 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[2]];
            *(u32 *)&((POLY_GT4 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[3]];
            *(u32 *)&((POLY_GT4 *)p)->r3 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_GT4 *)p)->code = c;
        }
        p += sizeof(POLY_GT4);
        s += sizeof(WmSrcGT4);
    }

    for (j = 0; j < g->unk8; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_GT3 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[0]];
            *(u32 *)&((POLY_GT3 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[1]];
            *(u32 *)&((POLY_GT3 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[2]];
            *(u32 *)&((POLY_GT3 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_GT3 *)p)->code = c;
        }
        p += sizeof(POLY_GT3);
        s += sizeof(WmSrcGT3);
    }

    for (j = 0; j < g->unkA; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_G4 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[0]];
            *(u32 *)&((POLY_G4 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[1]];
            *(u32 *)&((POLY_G4 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[2]];
            *(u32 *)&((POLY_G4 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[3]];
            *(u32 *)&((POLY_G4 *)p)->r3 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_G4 *)p)->code = c;
        }
        p += sizeof(POLY_G4);
        s += sizeof(WmSrcG4);
    }

    for (j = 0; j < g->unkC; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_G3 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[0]];
            *(u32 *)&((POLY_G3 *)p)->r0 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[1]];
            *(u32 *)&((POLY_G3 *)p)->r1 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[2]];
            *(u32 *)&((POLY_G3 *)p)->r2 = fldmdl_addRgbClamped(*col, arg2, arg3, arg4);
            ((POLY_G3 *)p)->code = c;
        }
        p += sizeof(POLY_G3);
        s += sizeof(WmSrcG3);
    }

    for (j = 0; j < g->unkE; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_F4 *)p)->code;
            *(u32 *)&((POLY_F4 *)p)->r0 |= fldmdl_addRgbClamped(*(u32 *)((WmSrcF4 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_F4 *)p)->code = c;
        }
        p += sizeof(POLY_F4);
        s += sizeof(WmSrcF4);
    }

    for (j = 0; j < g->unk10; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_F3 *)p)->code;
            *(u32 *)&((POLY_F3 *)p)->r0 = fldmdl_addRgbClamped(*(u32 *)((WmSrcF3 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_F3 *)p)->code = c;
        }
        p += sizeof(POLY_F3);
        s += sizeof(WmSrcF3);
    }
}

void fldmdl_setGroupRgb(Unk800C95C8 *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    WmModelGroup *g;
    u32 c;
    u8 *p;
    u32 j;

    if (arg0->unk0 & 0x10) {
        return;
    }
    g = &arg0->unk8->unk10[arg1];
    c = (arg4 << 16) | (arg3 << 8) | arg2;
    if (g_gameState->unk1C->unk8 != 0) {
        p = arg0->unk44[arg1].unk4 + g->unk0;
    } else {
        p = arg0->unk44[arg1].unk4;
    }
    for (j = 0; j < g->unk2; j++) {
        *(u32 *)&((POLY_FT4 *)p)->r0 = c | (((POLY_FT4 *)p)->code << 24);
        p += sizeof(POLY_FT4);
    }
    for (j = 0; j < g->unk4; j++) {
        *(u32 *)&((POLY_FT3 *)p)->r0 = c | (((POLY_FT3 *)p)->code << 24);
        p += sizeof(POLY_FT3);
    }
    for (j = 0; j < g->unk6; j++) {
        *(u32 *)&((POLY_GT4 *)p)->r0 = c | (((POLY_GT4 *)p)->code << 24);
        p += sizeof(POLY_GT4);
    }
    for (j = 0; j < g->unk8; j++) {
        *(u32 *)&((POLY_GT3 *)p)->r0 = c | (((POLY_GT3 *)p)->code << 24);
        p += sizeof(POLY_GT3);
    }
    for (j = 0; j < g->unkA; j++) {
        *(u32 *)&((POLY_G4 *)p)->r0 = c | (((POLY_G4 *)p)->code << 24);
        p += sizeof(POLY_G4);
    }
    for (j = 0; j < g->unkC; j++) {
        *(u32 *)&((POLY_G3 *)p)->r0 = c | (((POLY_G3 *)p)->code << 24);
        p += sizeof(POLY_G3);
    }
    for (j = 0; j < g->unkE; j++) {
        *(u32 *)&((POLY_F4 *)p)->r0 = c | (((POLY_F4 *)p)->code << 24);
        p += sizeof(POLY_F4);
    }
    for (j = 0; j < g->unk10; j++) {
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

void fldmdl_setSemiTrans(Unk800C95C8 *arg0, u16 arg1) {
    u32 i;
    u32 j;
    u8 *p;
    WmModelGroup *g;
    u8 *s;

    if (arg0->unk0 & 0x10) {
        return;
    }
    for (i = 0; i < arg0->unk8->unk3; i++) {
        g = &arg0->unk8->unk10[i];
        s = g->unk1C;
        p = arg0->unk44[i].unk4;
        if (g_gameState->unk1C->unk8 != 0) {
            p += g->unk0;
        }
        for (j = 0; j < g->unk2; j++) {
            if ((((WmSrcFT4 *)s)->unk15 & 1) || arg1 != 0) {
                ((POLY_FT4 *)p)->code |= 2;
            } else {
                ((POLY_FT4 *)p)->code &= ~2;
            }
            p += sizeof(POLY_FT4);
            s += sizeof(WmSrcFT4);
        }
        for (j = 0; j < g->unk4; j++) {
            if ((((WmSrcFT3 *)s)->unk12 & 1) || arg1 != 0) {
                ((POLY_FT3 *)p)->code |= 2;
            } else {
                ((POLY_FT3 *)p)->code &= ~2;
            }
            s += sizeof(WmSrcFT3);
            p += sizeof(POLY_FT3);
        }
        for (j = 0; j < g->unk6; j++) {
            if ((((WmSrcGT4 *)s)->unk1C & 1) || arg1 != 0) {
                ((POLY_GT4 *)p)->code |= 2;
            } else {
                ((POLY_GT4 *)p)->code &= ~2;
            }
            s += sizeof(WmSrcGT4);
            p += sizeof(POLY_GT4);
        }
        for (j = 0; j < g->unk8; j++) {
            if ((((WmSrcGT3 *)s)->unkF & 1) || arg1 != 0) {
                ((POLY_GT3 *)p)->code |= 2;
            } else {
                ((POLY_GT3 *)p)->code &= ~2;
            }
            s += sizeof(WmSrcGT3);
            p += sizeof(POLY_GT3);
        }
    }
}

void fldmdl_setAbr(Unk800C95C8 *arg0, u16 arg1) {
    u32 i;
    u32 j;
    u8 *p;
    WmModelGroup *g;

    if (arg0->unk0 & 0x10) {
        return;
    }
    for (i = 0; i < arg0->unk8->unk3; i++) {
        g = &arg0->unk8->unk10[i];
        p = arg0->unk44[i].unk4;
        if (g_gameState->unk1C->unk8 != 0) {
            p += g->unk0;
        }
        for (j = 0; j < g->unk2; j++) {
            ((POLY_FT4 *)p)->tpage = arg1 | (((POLY_FT4 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_FT4);
        }
        for (j = 0; j < g->unk4; j++) {
            ((POLY_FT3 *)p)->tpage = arg1 | (((POLY_FT3 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_FT3);
        }
        for (j = 0; j < g->unk6; j++) {
            ((POLY_GT4 *)p)->tpage = arg1 | (((POLY_GT4 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_GT4);
        }
        for (j = 0; j < g->unk8; j++) {
            ((POLY_GT3 *)p)->tpage = arg1 | (((POLY_GT3 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_GT3);
        }
    }
}

void fldmdl_calcBoneMatricesLookAt(Unk800C95C8 *arg0, s32 arg1) {
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

    anim = (WmAnim *)arg0->unkC;
    if (anim == NULL) {
        if (arg0->unk1C != NULL) {
            /* attached to a bone of the parent model */
            w0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[0];
            w1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[1];
            w2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[2];
            w3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[3];
            ((s32 *)arg0->unk20)[0] = w0;
            ((s32 *)arg0->unk20)[1] = w1;
            ((s32 *)arg0->unk20)[2] = w2;
            ((s32 *)arg0->unk20)[3] = w3;
            w0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[4];
            w1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[5];
            w2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[6];
            w3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[7];
            ((s32 *)arg0->unk20)[4] = w0;
            ((s32 *)arg0->unk20)[5] = w1;
            ((s32 *)arg0->unk20)[6] = w2;
            ((s32 *)arg0->unk20)[7] = w3;
            ((MATRIX *)arg0->unk20)->t[0] += arg0->unk74;
            ((MATRIX *)arg0->unk20)->t[1] += arg0->unk76;
            ((MATRIX *)arg0->unk20)->t[2] += arg0->unk78;
        } else {
            ((MATRIX *)arg0->unk20)->m[0][0] = arg0->unk24.m[0][0];
            ((MATRIX *)arg0->unk20)->m[0][1] = -arg0->unk24.m[0][1];
            ((MATRIX *)arg0->unk20)->m[0][2] = -arg0->unk24.m[0][2];
            ((MATRIX *)arg0->unk20)->m[1][0] = arg0->unk24.m[1][0];
            ((MATRIX *)arg0->unk20)->m[1][1] = -arg0->unk24.m[1][1];
            ((MATRIX *)arg0->unk20)->m[1][2] = -arg0->unk24.m[1][2];
            ((MATRIX *)arg0->unk20)->m[2][0] = arg0->unk24.m[2][0];
            ((MATRIX *)arg0->unk20)->m[2][1] = -arg0->unk24.m[2][1];
            ((MATRIX *)arg0->unk20)->m[2][2] = -arg0->unk24.m[2][2];
            ((MATRIX *)arg0->unk20)->t[0] = arg0->unk24.t[0];
            ((MATRIX *)arg0->unk20)->t[1] = arg0->unk24.t[1];
            ((MATRIX *)arg0->unk20)->t[2] = arg0->unk24.t[2];
        }
        return;
    }
    nb = arg0->unk8->unk2;
    bones = (MATRIX *)0x1F800018;
    if (nb >= 31) {
        bones = (MATRIX *)arg0->unk20;
    }
    m = bones;
    vec = (VECTOR *)0x1F800008;
    vec->vx = 0;
    vec->vy = 0;
    key = anim->unkC;
    key2 = anim->unk10;
    rot = (SVECTOR *)0x1F800000;
    if (anim->unkA & 1) {
        bones->t[0] = anim->unk4[0];
    } else {
        bones->t[0] = ((s16 *)((u8 *)anim + (u16)anim->unk4[0]))[(u16)arg1];
    }
    if (anim->unkA & 2) {
        m->t[1] = anim->unk4[1];
    } else {
        m->t[1] = ((s16 *)((u8 *)anim + (u16)anim->unk4[1]))[(u16)arg1];
    }
    if (anim->unkA & 4) {
        m->t[2] = anim->unk4[2];
    } else {
        m->t[2] = ((s16 *)((u8 *)anim + (u16)anim->unk4[2]))[(u16)arg1];
    }
    end = &bones[nb];
loop1:
    gte_ldrotkey(key, anim, arg1, rot);
    if (key2 != NULL) {
        gte_ldrotkey_lo(key2, anim, arg1, rot);
        key2++;
    }
    key++;
    func_80031264(rot, m);
    m++;
    if (m < end) {
        goto loop1;
    }
    if (arg0->unk1C != NULL) {
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[0];
            x1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[1];
            x2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[2];
            x3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[3];
            ((s32 *)bones)[0] = x0;
            ((s32 *)bones)[1] = x1;
            ((s32 *)bones)[2] = x2;
            ((s32 *)bones)[3] = x3;
        }
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[4];
            x1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[5];
            x2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[6];
            x3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[7];
            ((s32 *)bones)[4] = x0;
            ((s32 *)bones)[5] = x1;
            ((s32 *)bones)[6] = x2;
            ((s32 *)bones)[7] = x3;
        }
        bones->t[0] += arg0->unk74;
        bones->t[1] += arg0->unk76;
        bones->t[2] += arg0->unk78;
    } else {
        gte_SetRotMatrix(&arg0->unk24);
        gte_SetTransMatrix(&arg0->unk24);
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
    key2 = (WmAnimKey *)((WmBone *)arg0->unk8->unkC + 1);
    head = arg0->unk8->unk6;
    rot = (SVECTOR *)&v;
    p = &bones[((WmBone *)key2)->unk3];
loop:
    gte_SetRotMatrix(p);
    gte_ldclmv(m);
    gte_rtir();
    gte_SetTransMatrix(p);
    gte_stclmv(m);
    key = (WmAnimKey *)&m->m[0][1];
    gte_ldclmv(key);
    gte_rtir();
    vec->vz = ((WmBone *)key2)->unk0;
    gte_stclmv(key);
    arg1 = (s32)&m->m[0][2];
    gte_ldclmv((s16 *)arg1);
    gte_rtir();
    key2 = (WmAnimKey *)((WmBone *)key2 + 1);
    gte_stclmv((s16 *)arg1);
    gte_ldlv0(vec);
    gte_rtv0tr();
    p = &bones[((WmBone *)key2)->unk3];
    gte_stlvnl(m->t);
    if (head == idx) {
        up.vx = 0;
        up.vy = 0x1000;
        up.vz = 0;
        v.vx = m->t[0] - arg0->unk48[0];
        v.vy = m->t[1] - arg0->unk48[1];
        v.vz = m->t[2] - arg0->unk48[2];
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
        if (arg0->unk0 & 2) {
            *(s32 *)&s.m[0][0] = 0x1000;
            *(s32 *)&s.m[0][2] = 0;
            *(s32 *)&s.m[1][1] = 0x1000;
            *(s32 *)&s.m[2][0] = 0;
            *(s32 *)&s.m[2][2] = 0x1000;
            s.m[0][0] = arg0->unk54;
            gte_SetRotMatrix(m);
            gte_ldclmv(&s);
            gte_rtir();
            s.m[1][1] = arg0->unk56;
            gte_stclmv(m);
            gte_ldclmv(&s.m[0][1]);
            gte_rtir();
            s.m[2][2] = arg0->unk58;
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
        m = (MATRIX *)arg0->unk20;
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

void fldmdl_updateTexAnims(BgAnimHdr *arg0) {
    RECT r;
    u32 i;
    u32 j;
    BgAnim *a;
    BgAnimRect *ar;
    s32 pos;
    s16 f;
    s32 r8;
    s32 v;
    s16 last;

    if (arg0 == NULL) {
        return;
    }
    for (i = 0; i < arg0->unk0; i++) {
        a = &arg0->unk4[i];
        if (!(a->unk0 & 1)) {
            continue;
        }
        if (a->unk1 != 0) {
            pos = a->unk8;
            f = pos >> 12;
            last = a->unkE;
            if (f >= 0) {
                if (f != last) {
                    for (j = 0; j < a->unkC; j++) {
                        ar = &a->unk10[j];
                        r.x = ar->unk8[f].vx;
                        r.y = ar->unk8[f].vy;
                        r.w = ar->unk0.w;
                        r.h = ar->unk0.h;
                        MoveImage(&r, ar->unk0.x, ar->unk0.y);
                    }
                    a->unkE = f;
                }
                pos += a->unk2;
            } else {
                pos += 0x1000;
            }
            if ((pos >> 12) < a->unk1) {
                a->unk8 = pos;
            } else if (a->unk6 != 0) {
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                }
                r8 = *g_randPtr++;
                v = (a->unk6 + 1) * ((r8 << 8) | r8);
                a->unk8 = -((a->unk4 + v / 0x10000) << 12);
            } else if (a->unk0 & 2) {
                a->unk0 &= ~3;
            } else {
                a->unk8 = 0;
            }
        } else if (a->unk0 & 4) {
            pos = a->unk8;
            f = pos >> 12;
            for (j = 0; j < a->unkC; j++) {
                ar = &a->unk10[j];
                MoveImage(&ar->unk0, ar->unk0.x, ar->unk0.y - f);
                r.x = ar->unk0.x;
                r.y = ar->unk0.y - f;
                r.w = ar->unk0.w;
                r.h = f;
                MoveImage(&r, ar->unk0.x, ar->unk0.y + ar->unk0.h - f);
            }
            a->unk8 = (pos & 0xFFF) + a->unk2;
        }
    }
}

void fldmdl_initTexAnims(BgSprHdr *hdr, BgSprOfs *ofs) {
    u32 i, j, k;
    u32 p;
    BgSprPart *part;
    u8 idx;

    if (hdr == NULL) {
        return;
    }
    for (i = 0; i < hdr->unk0; i++) {
        hdr->unk4[i].unkE = 1;
        p = hdr->unk4[i].unk10;
        if (p > 0x80000000) {
            return;
        }
        if (p == 0) {
            continue;
        }
        idx = hdr->unk4[i].unkD;
        p += (u32)hdr;
        hdr->unk4[i].unk10 = p;
        if (ofs != NULL) {
            ((BgSprPart *)p)->unk0 += ofs[idx].unk4;
            ((BgSprPart *)p)->unk2 += ofs[idx].unk6;
        }
        part = (BgSprPart *)p;
        for (j = 0; j < hdr->unk4[i].unkC; j++) {
            p = part[j].unk8;
            p += (u32)hdr;
            part[j].unk8 = p;
            if (ofs != NULL) {
                for (k = 0; k < hdr->unk4[i].unk1; k++) {
                    ((BgSprVtx *)p)->unk0 += ofs[idx].unk0;
                    ((BgSprVtx *)p)->unk2 += ofs[idx].unk2;
                    p += sizeof(BgSprVtx);
                }
            }
        }
    }
}

void fldmdl_projectMeshClipY(Unk800C95C8 *arg0, u8 arg1) {
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

    gte_SetGeomScreen(g_gameState->unk1C->unkA);
    sxy = g_vtxScreenXY;
    sz = g_vtxScreenZ;
    k = 0;
    g = &arg0->unk8->unk10[arg1];
    bone = (MATRIX *)arg0->unk20;
    cnt = arg0->unk8->unk2;
    ymax = arg0->unk5A;
    vtx = g->unk18;
    for (i = 0; i < cnt; bone++) {
        n = ((u16 *)g->unk14)[i++];
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
            gte_SetRotMatrix(&g_gameState->unk1C->unk18);
            gte_SetTransMatrix(&g_gameState->unk1C->unk18);
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

void fldmdl_drawMeshClipY(Unk800C95C8 *arg0, u8 arg1) {
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

    g = &arg0->unk8->unk10[arg1];
    ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8];
    if (g_gameState->unk1C->unk8 != 0) {
        prim = (u8 *)arg0->unk44[arg1].unk4 + g->unk0;
    } else {
        prim = arg0->unk44[arg1].unk4;
    }

    n = g->unk2;
    pFT4 = (POLY_FT4 *)prim;
    sFT4 = (WmSrcFT4 *)(void *)g->unk1C;
    for (i = 0; i < n; sFT4++, pFT4++) {
        a = *(u32 *)&sFT4->unk0[0];
        b = *(u32 *)&sFT4->unk0[2];
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
        if (!(sFT4->unk15 & 2)) {
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

    n = g->unk4;
    sFT3 = (WmSrcFT3 *)sFT4;
    pFT3 = (POLY_FT3 *)pFT4;
    for (i = 0; i < n; sFT3++, pFT3++) {
        a = *(u32 *)&sFT3->unk0[0];
        b = sFT3->unk0[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)b];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sFT3->unk12 & 2)) {
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

    n = g->unk6;
    sGT4 = (WmSrcGT4 *)sFT3;
    pGT4 = (POLY_GT4 *)pFT3;
    for (i = 0; i < n; sGT4++, pGT4++) {
        a = *(u32 *)&sGT4->unk0[0];
        b = *(u32 *)&sGT4->unk0[2];
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
        if (!(sGT4->unk1C & 2)) {
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

    n = g->unk8;
    sGT3 = (WmSrcGT3 *)sGT4;
    pGT3 = (POLY_GT3 *)pGT4;
    for (i = 0; i < n; sGT3++, pGT3++) {
        a = *(u32 *)&sGT3->unk0[0];
        b = sGT3->unk0[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)b];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sGT3->unkF & 2)) {
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

    n = g->unkA;
    sG4 = (WmSrcG4 *)sGT3;
    pG4 = (POLY_G4 *)pGT3;
    for (i = 0; i < n; sG4++, pG4++) {
        a = *(u32 *)&sG4->unk0[0];
        b = *(u32 *)&sG4->unk0[2];
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
        if (!(sG4->unk14 & 2)) {
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

    n = g->unkC;
    sG3 = (WmSrcG3 *)sG4;
    pG3 = (POLY_G3 *)pG4;
    for (i = 0; i < n; sG3++, pG3++) {
        a = *(u32 *)&sG3->unk0[0];
        b = sG3->unk0[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)b];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sG3->unk13 & 2)) {
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

    n = g->unkE;
    sF4 = (WmSrcF4 *)sG3;
    pF4 = (POLY_F4 *)pG3;
    for (i = 0; i < n; sF4++, pF4++) {
        a = *(u32 *)&sF4->unk0[0];
        b = *(u32 *)&sF4->unk0[2];
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
        if (!(sF4->unkD & 2)) {
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

    n = g->unk10;
    sF3 = (WmSrcF3 *)sF4;
    pF3 = (POLY_F3 *)pF4;
    for (i = 0; i < n; sF3++, pF3++) {
        a = *(u32 *)&sF3->unk0[0];
        b = sF3->unk0[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[(u16)b];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sF3->unkB & 2)) {
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
void fldmdl_reflectModel(Unk800C95C8 *arg0) {
    MATRIX *m;
    s32 nx;
    s32 ny;
    s32 nz;
    s32 d;

    if (arg0->unk60 != NULL) {
        nx = arg0->unk64.v.vx;
        ny = arg0->unk64.v.vy;
        nz = arg0->unk64.v.vz;
        d = (arg0->unk6C.v.vx * nx + arg0->unk6C.v.vy * ny + arg0->unk6C.v.vz * nz) >> 11;
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
        gte_ldclmv(&arg0->unk60->unk24);
        gte_rtir();
        gte_stclmv(&arg0->unk24);
        gte_ldclmv(&arg0->unk60->unk24.m[0][1]);
        gte_rtir();
        gte_stclmv(&arg0->unk24.m[0][1]);
        gte_ldclmv(&arg0->unk60->unk24.m[0][2]);
        gte_rtir();
        gte_stclmv(&arg0->unk24.m[0][2]);
        gte_SetTransMatrix(m);
        gte_ldlv0(arg0->unk60->unk24.t);
        gte_rtv0tr();
        gte_stlvnl(arg0->unk24.t);
        arg0->unkC = arg0->unk60->unkC;
    }
}

void fldmdl_drawMeshMirrored(Unk800C95C8 *arg0, u8 arg1) {
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

    ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8];
    g = &arg0->unk8->unk10[arg1];
    if (g_gameState->unk1C->unk8 != 0) {
        prim = (u8 *)arg0->unk44[arg1].unk4 + g->unk0;
    } else {
        prim = arg0->unk44[arg1].unk4;
    }

    n = g->unk2;
    pFT4 = (POLY_FT4 *)prim;
    ot -= g->unk13;
    sFT4 = (WmSrcFT4 *)(void *)g->unk1C;
    for (i = 0; i < n; sFT4++, pFT4++) {
        i0 = *(u32 *)&sFT4->unk0[0];
        i2 = *(u32 *)&sFT4->unk0[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT4->unk15 & 2)) {
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

    n = g->unk4;
    sFT3 = (WmSrcFT3 *)sFT4;
    pFT3 = (POLY_FT3 *)pFT4;
    for (i = 0; i < n; sFT3++, pFT3++) {
        i0 = *(u32 *)&sFT3->unk0[0];
        i2 = sFT3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT3->unk12 & 2)) {
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

    n = g->unk6;
    sGT4 = (WmSrcGT4 *)sFT3;
    pGT4 = (POLY_GT4 *)pFT3;
    for (i = 0; i < n; sGT4++, pGT4++) {
        i0 = *(u32 *)&sGT4->unk0[0];
        i2 = *(u32 *)&sGT4->unk0[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT4->unk1C & 2)) {
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

    n = g->unk8;
    sGT3 = (WmSrcGT3 *)sGT4;
    pGT3 = (POLY_GT3 *)pGT4;
    for (i = 0; i < n; sGT3++, pGT3++) {
        i0 = *(u32 *)&sGT3->unk0[0];
        i2 = sGT3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT3->unkF & 2)) {
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

    n = g->unkA;
    sG4 = (WmSrcG4 *)sGT3;
    pG4 = (POLY_G4 *)pGT3;
    for (i = 0; i < n; sG4++, pG4++) {
        i0 = *(u32 *)&sG4->unk0[0];
        i2 = *(u32 *)&sG4->unk0[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG4->unk14 & 2)) {
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

    n = g->unkC;
    sG3 = (WmSrcG3 *)sG4;
    pG3 = (POLY_G3 *)pG4;
    for (i = 0; i < n; sG3++, pG3++) {
        i0 = *(u32 *)&sG3->unk0[0];
        i2 = sG3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG3->unk13 & 2)) {
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

    n = g->unkE;
    sF4 = (WmSrcF4 *)sG3;
    pF4 = (POLY_F4 *)pG3;
    for (i = 0; i < n; sF4++, pF4++) {
        i0 = *(u32 *)&sF4->unk0[0];
        i2 = *(u32 *)&sF4->unk0[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF4->unkD & 2)) {
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

    n = g->unk10;
    sF3 = (WmSrcF3 *)sF4;
    pF3 = (POLY_F3 *)pF4;
    for (i = 0; i < n; sF3++, pF3++) {
        i0 = *(u32 *)&sF3->unk0[0];
        i2 = sF3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF3->unkB & 2)) {
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

s32 fldbg_buildLayerSprites(Unk800C9DB4Obj *arg0, BgSprt *arg1, BgSprt **arg2) {
    BgPos *pos;
    BgTile *tile;
    u32 i;
    u32 buf;
    s32 n;
    s32 w;
    s32 x;
    u16 t;
    Unk800C9DB4 *bg;
    s32 u;
    s32 mask;
    s32 posOfs;
    s32 tileOfs;

    /* Statement order and the barrier below are for the scheduler: everything above the barrier
     * forms its own group, where both global loads are ready at once and are scheduled as
     * lui/lui/lw/lbu, so %hi(D_800CA108) gets v1. The unk25_1 mask is set up here (and applied by
     * hand below) because its lui/ori must also sit above the barrier, and buf/the two offsets
     * are read early so their insns land where the original has them. */
    mask = 0xFFFF01FF;
    bg = g_fieldBgData;
    u = D_800CA108;
    posOfs = arg0->unk28;
    tileOfs = arg0->unk2C;
    buf = 0;
    arg0->unk30 = arg1;
    do { } while (0);
    pos = (BgPos *)((u8 *)bg + posOfs);
    tile = (BgTile *)((u8 *)bg + tileOfs);
    w = u + 0xDF;
    n = arg0->unk26;
    arg0->unk18 = arg0->unkC + bg->unk30;
    arg0->unk1A = arg0->unkE + bg->unk32;
    arg0->unk25 = 0;
    ((Unk800C9DB4ObjFlags *)arg0)->unk24 &= mask; /* arg0->unk25_1 = 0 */
    for (; buf < 2; buf++) {
        for (i = 0; i < n; i++) {
            BgSprt *sp = &arg1[i];
            setlen(&sp->unk8, 3);
            sp->unk8.code = 0x7C;
            sp->unk8.r0 = 0x80;
            sp->unk8.g0 = 0x80;
            sp->unk8.b0 = 0x80;
            sp->unk8.x0 = arg0->unk18 + pos[i].x;
            sp->unk8.y0 = arg0->unk1A + pos[i].y;
            sp->unk8.u0 = tile[i].u;
            sp->unk8.v0 = tile[i].v;
            if (tile[i].trans) {
                sp->unk8.code |= 2;
            } else {
                sp->unk8.code &= ~2;
            }
            sp->unk8.clut = GetClut(tile[i].clutX << 4, tile[i].clutY);
            if (tile[i].tp == 0) {
                t = GetTPage(0, tile[i].abr, tile[i].tpX << 6, tile[i].tpY << 8);
            } else {
                t = GetTPage(1, tile[i].abr, tile[i].tpX << 6, tile[i].tpY << 8);
            }
            setDrawTPage(&sp->unk0, 1, 1, t);
            MargePrim(&sp->unk0, &sp->unk8);
            x = tile[i].clutY;
            if (w < x) {
                w = x;
            }
        }
        arg1 += n;
    }
    *arg2 = arg1;
    D_800CA108 = w + 0x21;
    return 1;
}

s32 fldbg_initEffectList(void) {
    u32 n;
    u32 i;
    Unk800C9DA4Tbl *tbl;
    Unk800C9DA4Trk *trk;

    tbl = (Unk800C9DA4Tbl *)((u8 *)g_fieldBgCtx.unk10 + g_fieldBgCtx.unk10->unkC);
    trk = (Unk800C9DA4Trk *)((u8 *)g_fieldBgCtx.unk10 + g_fieldBgCtx.unk10->unk10);
    n = g_fieldBgCtx.unk10->unk6;
    i = 0;
    g_fieldBgCtx.unk24 = 0;
    g_fieldBgCtx.unk28 = (Unk800C9DA4Ent *)g_fieldBgCtx.unk2C;
    for (; i < n; i++) {
        if (trk[i].unk0 & 0x40) {
            g_fieldBgCtx.unk28[g_fieldBgCtx.unk24].unk0 = 0x10;
            g_fieldBgCtx.unk28[g_fieldBgCtx.unk24].unk1 = i;
            g_fieldBgCtx.unk28[g_fieldBgCtx.unk24].unk4 = 0;
            g_fieldBgCtx.unk28[g_fieldBgCtx.unk24].unk8 = 0;
            g_fieldBgCtx.unk34 += 0xC;
            g_fieldBgCtx.unk24++;
            if (trk[i].unk0 & 0x20) {
                i += tbl->unk0 >> 8;
                tbl++;
            }
        }
    }
    return 1;
}

s32 fldbg_updateLayerAnims(void) {
    Unk800C9DB4 *d = g_fieldBgData;
    u32 i;
    Unk800C9DB4Path *p;
    Unk800C9DB4Path *paths;
    u32 cnt;
    Unk800C9DB4Obj *objs;
    Unk800C9DB4Step *st;
    u32 n;
    s32 pos;
    u32 j;
    s32 v;

    i = 0;
    paths = (Unk800C9DB4Path *)((u8 *)d + d->unkC);
    objs = (Unk800C9DB4Obj *)((u8 *)d + d->unk10);
    n = d->unk4;
    for (; i < n; i++) {
        p = &paths[i];
        pos = p->unk5;
        if ((p->unk0 & 2) && (p->unk0 & 0x14) && p->unk4 == g_fieldBgState.unk75) {
            st = (Unk800C9DB4Step *)((u8 *)d + p->unkC);
            cnt = p->unk1;
            for (j = 0; j < cnt; j++) {
                objs[st[j].unk0].unk0 &= ~2;
            }
            objs[st[pos >> 8].unk0].unk0 |= 2;
            if (st[pos >> 8].unk1 < 0) {
                st[pos >> 8].unk1 = 0;
                p->unkA = 0;
                p->unk0 &= ~4;
                return 1;
            }
            p->unkA++;
            if ((u16)p->unkA < st[pos >> 8].unk1) {
                continue;
            }
            p->unkA = 0;
            p->unk5 = pos + p->unk8;
            v = p->unk5 >> 8;
            if (v >= p->unk1) {
                if (p->unk0 & 0x20) {
                    p->unk5 = (p->unk1 - 1) << 8;
                    fldbg_setAnimSpeed(i, -p->unk8);
                } else {
                    p->unk5 = 0;
                }
            } else if (v < 0) {
                if (p->unk0 & 0x20) {
                    p->unk5 = 0;
                    fldbg_setAnimSpeed(i, -p->unk8);
                } else {
                    p->unk5 = (p->unk1 - 1) << 8;
                }
            } else {
                continue;
            }
            p->unk0 &= ~4;
        }
    }
    return 1;
}

/* initialise the screen fade quads */
s32 fldbg_initFollowLayers(void) {
    u16 i;
    u16 j;

    g_fieldBgFollowLayerCount = 0;
    for (i = 0; i < 10; i++) {
        g_fieldBgFollowLayers[i].unk0 = -1;
        g_fieldBgFollowLayers[i].unk1 = 0x80;
        g_fieldBgFollowLayers[i].unk2 = 0x80;
        g_fieldBgFollowLayers[i].unk3 = 0x80;
        g_fieldBgFollowLayers[i].unk4 = -1;
        g_fieldBgFollowLayers[i].unk6 = 0;
        g_fieldBgFollowLayers[i].unk8 = 0;
        for (j = 0; j < 4; j++) {
            SetPolyF4(&g_fieldBgFollowLayers[i].unkC[0][j]);
            setRGB0(&g_fieldBgFollowLayers[i].unkC[0][j], g_fieldBgFollowLayers[i].unk1, g_fieldBgFollowLayers[i].unk2, g_fieldBgFollowLayers[i].unk3);
            SetPolyF4(&g_fieldBgFollowLayers[i].unkC[1][j]);
            setRGB0(&g_fieldBgFollowLayers[i].unkC[1][j], g_fieldBgFollowLayers[i].unk1, g_fieldBgFollowLayers[i].unk2, g_fieldBgFollowLayers[i].unk3);
        }
    }
    return 1;
}

s32 fldbg_drawFollowMask(Unk800C9DB4 *bg, Unk800C9DB4Obj *o, Unk800CA068 *st, FieldFade *f) {
    u32 *ot;
    POLY_F4 *p;
    DR_TPAGE *tp;
    s16 x;
    s16 y;
    s16 u;
    s16 v;
    s16 w;

    ot = g_fieldBgCtx.unk8[*g_fieldBgCtx.unk0];
    x = o->unkC + bg->unk24 - (st->unk4.unk0 - 0xA0);
    y = o->unkE + bg->unk26 - (st->unk4.unk2 - 0x70);
    if (x > 0) {
        p = &f->unkC[*g_fieldBgCtx.unk0][0];
        u = y;
        if (u < 0) {
            u = 0;
        }
        setXY4(p, 0, u, x, u, 0, 0xE0, x, 0xE0);
        addPrim(&ot[7], p);
    }
    if (y > 0) {
        u = x + o->unk4;
        p = &f->unkC[*g_fieldBgCtx.unk0][1];
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
    if (x + o->unk4 < 0x140) {
        u = x + o->unk4;
        v = y + o->unk6;
        p = &f->unkC[*g_fieldBgCtx.unk0][2];
        w = 0x140 - u;
        if (v > 0xE0) {
            v = 0xE0;
        }
        setXYWH(p, u, 0, w, v);
        addPrim(&ot[7], p);
    }
    if (y + o->unk6 < 0xE0) {
        p = &f->unkC[*g_fieldBgCtx.unk0][3];
        v = x;
        if (v < 0) {
            v = 0;
        }
        u = y + o->unk6;
        if (u < 0) {
            u = 0;
        }
        setXY4(p, v, u, 0x140, u, v, 0xE0, 0x140, 0xE0);
        addPrim(&ot[7], p);
    }
    if (f->unk0 < 4) {
        tp = &f->unkCC[*g_fieldBgCtx.unk0];
        SetDrawTPage(tp, 1, 1, GetTPage(2, f->unk0, 0, 0));
        addPrim(&ot[7], tp);
    }
    return 1;
}

s32 fldbg_updateFollowLayers(void) {
    SVECTOR v;
    s32 sxy;
    Unk800CA068 *st;
    Unk800C9DB4 *bg;
    Unk800C9DB4Obj *objs;
    Unk800C9DB4Obj *o;
    Unk8004AB64 *a;
    FieldFade *f;
    u8 i;
    u8 mode;
    u16 sx;
    u16 sy;
    s16 y;
    s16 x;

    if (g_fieldBgFollowLayerCount == 0) {
        return 0;
    } else {
        st = &g_fieldBgState;
        bg = g_fieldBgData;
        objs = (Unk800C9DB4Obj *)((u8 *)bg + bg->unk10);
        mode = g_gameState->mode;
        if (mode == 5) {
            a = (Unk8004AB64 *)g_gameState->unk1C->unk8E0[g_gameState->unk1C->unk8E4.field->unk8].unkC;
        } else if (mode == 1) {
            a = func_8004AE00();
        } else {
            a = NULL;
        }
        if (a != NULL) {
            v.vx = a->pos.vx;
            v.vy = a->pos.vy - g_fieldBgState.unk76;
            v.vz = a->pos.vz;
        } else {
            v.vx = 0;
            v.vy = 0;
            v.vz = 0;
        }
        fldbg_applyGeomOffset();
        gte_SetRotMatrix(g_fieldBgState.unk1C);
        gte_SetTransMatrix(g_fieldBgState.unk1C);
        gte_SetGeomScreen(*g_fieldBgState.unk20);
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&sxy);
        sx = sxy;
        sy = sxy >> 16;
        f = g_fieldBgFollowLayers;
        for (i = 0; i < g_fieldBgFollowLayerCount; i++, f++) {
            o = (Unk800C9DB4Obj *)((u16)f->unk4 * sizeof(Unk800C9DB4Obj) + (u32)objs);
            if ((o->unk0 & 2) && o->unk24 == st->unk75) {
                x = st->unk4.unk0 + (sx - bg->unk24 - (f->unk6 + 0xA0));
                y = f->unk8;
                o->unkC = x;
                o->unkE = st->unk4.unk2 + (sy - bg->unk24 - (y + 0x70));
                if (f->unk0 >= 0) {
                    fldbg_drawFollowMask(bg, o, st, f);
                }
            }
        }
        return 1;
    }
}

s32 fldbg_initLayers(u8 *arg0, BgSprt **arg1) {
    Unk800C9DA4Hdr *h;
    Unk800C9DB4Cam *c;
    s32 x;
    s32 y;
    u32 n;
    u32 size;
    u32 i;
    Unk800C9DB4Obj *o;
    Unk800C9DB4Obj *objs;
    Unk800C9DB4Obj *p;

    h = g_fieldBgCtx.unk10;
    fldbg_setCamera(g_fieldBgState.unk75);
    c = &((Unk800C9DB4Cam *)((u8 *)h + h->unk18))[g_fieldBgState.unk75];
    x = (c->unk28 + c->unk2A) / 2;
    g_fieldBgState.unk4.unk0 = (g_fieldBgState.unkA4 = x);
    g_fieldBgState.unk4.unk2 = (g_fieldBgState.unkA6 = (c->unk2C + c->unk2E) / 2);
    x -= 160;
    h->unk30 = h->unk24 - x;
    n = h->unk6;
    objs = (Unk800C9DB4Obj *)((u8 *)h + h->unk10);
    size = n * 3;
    y = g_fieldBgState.unk4.unk2 - 112;
    h->unk32 = h->unk26 - (s16)y;
    g_fieldBgCtx.unk1C = arg0;
    while (size / 4 * 4 != size) {
        size++;
    }
    g_fieldBgCtx.unk20 = arg0 + size;
    size += n;
    while (size / 4 * 4 != size) {
        size++;
    }
    *arg1 = (BgSprt *)(arg0 + size);
    for (i = 0; i < 4; i++) {
        g_fieldBgState.unkA8[i][0] = 0;
        g_fieldBgState.unkA8[i][1] = 0;
        g_fieldBgState.unkB8[i][0] = c->unk24;
        g_fieldBgState.unkB8[i][1] = c->unk26;
        g_fieldBgState.unkC8[i] = 0x100;
        g_fieldBgState.unkD0[i] = 0x100;
    }
    for (i = 0; i < n; i++) {
        o = &objs[i];
        g_fieldBgCtx.unk1C[i * 3 + 0] = 0x80;
        g_fieldBgCtx.unk1C[i * 3 + 1] = 0x80;
        g_fieldBgCtx.unk1C[i * 3 + 2] = 0x80;
        g_fieldBgCtx.unk20[i] = 0;
        p = o;
        if (fldbg_buildLayerSprites(p, *arg1, arg1) != 1) {
            return 0;
        }
        o->unk10 = -0x8000;
        p->unk12 = 0x7FFF;
        o->unk14 = -0x8000;
        p->unk16 = 0x7FFF;
    }
    g_fieldBgState.unk73 = 0;
    g_fieldBgState.unk72 = 0;
    g_fieldBgState.unk0 &= ~0xC00;
    return 1;
}

s32 fldbg_drawLayer(Unk800C9DB4Obj *o, u8 *arg1) {
    BgSprt *spr;
    u32 *ot;
    Unk800C9DA4Hdr *h;
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

    spr = o->unk30;
    buf = *g_fieldBgCtx.unk0;
    h = g_fieldBgCtx.unk10;
    n = o->unk26;
    ot = g_fieldBgCtx.unk8[buf];
    if (buf != 0) {
        spr += n;
    }
    x = o->unkC + h->unk30;
    y = o->unkE + h->unk32;
    z = h->unk1E + o->unk1;
    pos = (BgPos *)((u8 *)h + o->unk28);
    if (o->unk0 & 4) {
        s16 tx;
        s16 ty;
        s16 d;
        s16 k;

        if (o->unk0 & 1) {
            ox = g_fieldBgState.unkA8[o->unk25_1][0];
            oy = g_fieldBgState.unkA8[o->unk25_1][1];
        } else {
            ox = g_fieldBgState.unkA8[o->unk25_1][0] + 0xA0 - g_fieldBgState.unk4.unk0;
            oy = g_fieldBgState.unkA8[o->unk25_1][1] + 0x70 - g_fieldBgState.unk4.unk2;
        }
        wx = g_fieldBgState.unkB8[o->unk25_1][0];
        wy = g_fieldBgState.unkB8[o->unk25_1][1];
        sx = o->unk1C;
        if (sx < 0) {
            k = 0x100 - sx;
            x = h->unk30 + ((((o->unkC << 8) | o->unk20) + k) >> 8);
        }
        sy = o->unk1E;
        if (sy < 0) {
            k = 0x100 - sx;
            y = h->unk32 + ((((o->unkE << 8) | o->unk22) + k) >> 8);
        }
        if (sx != 0) {
            x = (x - ((s16)wx - o->unk4)) % o->unk4 + (wx - o->unk4);
        }
        if (sy != 0) {
            y = (y - ((s16)wy - o->unk6)) % o->unk6 + (wy - o->unk6);
        }
        for (i = 0; i < n; i++) {
            if (o->unk0 & 1) {
                tx = x + pos[i].x;
                if (o->unk1C != 0) {
                    if (tx + 16 >= o->unk4) {
                        tx -= o->unk4;
                    } else if (tx < -15) {
                        tx += o->unk4;
                    }
                }
                if (tx + (s16)ox < -15 || (s16)wx < tx) {
                    continue;
                }
                ty = y + pos[i].y;
                if (o->unk1E != 0) {
                    if (ty + 16 >= o->unk6) {
                        ty -= o->unk6;
                    } else if (ty < -15) {
                        ty += o->unk6;
                    }
                }
                if (ty + (s16)oy < -15 || ty >= (s16)wy) {
                    continue;
                }
                d = z + pos[i].pad;
                if (d < 8 || d >= 0xFF9) {
                    continue;
                }
                spr[i].unk8.x0 = tx + ox;
                spr[i].unk8.y0 = ty + oy;
            } else {
                tx = x + pos[i].x;
                if (o->unk1C != 0) {
                    if (tx + 16 >= o->unk4) {
                        tx -= o->unk4;
                    } else if (tx < -15) {
                        tx += o->unk4;
                    }
                    if (tx + (s16)ox < -15 || tx >= (s16)wx) {
                        continue;
                    }
                    spr[i].unk8.x0 = tx + ox;
                } else {
                    if (tx < -15 || tx >= (s16)ox + (s16)wx) {
                        continue;
                    }
                    spr[i].unk8.x0 = tx;
                }
                ty = y + pos[i].y;
                if (o->unk1E != 0) {
                    if (ty + 16 >= o->unk6) {
                        ty -= o->unk6;
                    } else if (ty < -15) {
                        ty += o->unk6;
                    }
                    if (ty + (s16)oy < -15 || ty >= (s16)wy) {
                        continue;
                    }
                    spr[i].unk8.y0 = ty + oy;
                } else {
                    if (ty < -15 || ty >= (s16)oy + (s16)wy) {
                        continue;
                    }
                    spr[i].unk8.y0 = ty;
                }
                d = z + pos[i].pad;
                if (d < 8 || d >= 0xFF9) {
                    continue;
                }
            }
            addPrim(ot + d, &spr[i]);
        }
    } else if (o->unk0 & 0x80) {
        s16 tx;
        s16 ty;
        s16 d;
        s16 k;

        if (o->unk0 & 1) {
            ox = g_fieldBgState.unkA8[o->unk25_1][0];
            oy = g_fieldBgState.unkA8[o->unk25_1][1];
        } else {
            ox = g_fieldBgState.unkA8[o->unk25_1][0] + 0xA0 - g_fieldBgState.unk4.unk0;
            oy = g_fieldBgState.unkA8[o->unk25_1][1] + 0x70 - g_fieldBgState.unk4.unk2;
        }
        wx = g_fieldBgState.unkB8[o->unk25_1][0];
        wy = g_fieldBgState.unkB8[o->unk25_1][1];
        if (o->unk25) {
            y = (y - ((s16)wy - o->unk6)) % o->unk6 + (wy - o->unk6);
            x += y * o->unk1C / o->unk6 % o->unk4;
        } else {
            x = (x - ((s16)wx - o->unk4)) % o->unk4 + (wx - o->unk4);
            y += x * o->unk1E / o->unk4 % o->unk6;
        }
        for (i = 0; i < n; i++) {
            if (o->unk25) {
                k = 0;
                ty = y + pos[i].y;
                if (ty + 16 >= o->unk6) {
                    ty -= o->unk6;
                    k = -o->unk1C;
                } else if (ty < -15) {
                    ty += o->unk6;
                    k = o->unk1C;
                }
                if (ty + (s16)oy < -15 || ty >= (s16)wy) {
                    continue;
                }
                tx = k + (x + pos[i].x);
                if (tx < -15 || tx >= (s16)ox + (s16)wx) {
                    continue;
                }
                spr[i].unk8.x0 = tx;
                spr[i].unk8.y0 = ty + oy;
            } else {
                k = 0;
                tx = x + pos[i].x;
                if (tx + 16 >= o->unk4) {
                    tx -= o->unk4;
                    k = -o->unk1E;
                } else if (tx < -15) {
                    tx += o->unk4;
                    k = o->unk1E;
                }
                if (tx + (s16)ox < -15 || tx >= (s16)wx) {
                    continue;
                }
                ty = k + (y + pos[i].y);
                if (ty < -15 || ty >= (s16)oy + (s16)wy) {
                    continue;
                }
                spr[i].unk8.x0 = tx + ox;
                spr[i].unk8.y0 = ty;
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
            spr[i].unk8.y0 = ty;
            spr[i].unk8.x0 = tx;
            addPrim(ot + d, &spr[i]);
        }
    }
    o->unk18 = x;
    o->unk1A = y;
    return 1;
}

s32 fldbg_updateLayerScroll(Unk800C9DA4Hdr *arg0) {
    s32 i;
    s32 n;
    Unk800C9DB4Obj *o;
    Unk800C9DB4Obj *objs;
    s32 v;

    n = arg0->unk6;
    objs = (Unk800C9DB4Obj *)((u8 *)arg0 + arg0->unk10);
    for (i = 0; i < n; i++) {
        o = &objs[i];
        if (o->unk0 & 4) {
            if (o->unk1C != 0 && o->unk1C != 0x7FFF) {
                v = (o->unkC << 8) | (o->unk20 & 0xFF);
                v = v + o->unk1C;
                o->unk20 = v & 0xFF;
                o->unkC = (v >> 8) % o->unk4;
            }
            if (o->unk1E != 0 && o->unk1E != 0x7FFF) {
                v = (o->unkE << 8) | (o->unk22 & 0xFF);
                v = v + o->unk1E;
                o->unk22 = v & 0xFF;
                o->unkE = (v >> 8) % o->unk6;
            }
        }
        if (o->unk0 & 0x80) {
            if (o->unk25) {
                if (o->unk1E != 0x7FFF) {
                    v = (o->unkE << 8) | (o->unk22 & 0xFF);
                    v = v + o->unk1E;
                    o->unk22 = v & 0xFF;
                    o->unkE = (v >> 8) % o->unk6;
                }
            } else if (o->unk1C != 0x7FFF) {
                v = (o->unkC << 8) | (o->unk20 & 0xFF);
                v = v + o->unk1C;
                o->unk20 = v & 0xFF;
                o->unkC = (v >> 8) % o->unk4;
            }
        }
        if (o->unk0 & 8) {
            v = (o->unk8 << 8) + ((s16)g_fieldBgState.unk4.unk0 - g_fieldBgState.unkA4) * o->unk1C;
            o->unkC = v >> 8;
            o->unk20 = v & 0xFF;
            v = (o->unkA << 8) + ((s16)g_fieldBgState.unk4.unk2 - g_fieldBgState.unkA6) * o->unk1E;
            o->unkE = v >> 8;
            o->unk22 = v & 0xFF;
        }
    }
    if (g_fieldBgState.unk0 & 0x80) {
        g_fieldBgState.unk0 &= ~0x80;
        g_fieldBgState.unk8.unk0 = g_fieldBgState.unk4.unk0;
        g_fieldBgState.unk8.unk2 = g_fieldBgState.unk4.unk2;
    }
    return 1;
}

s32 fldbg_updateScroll(void) {
    s16 dur;
    u16 ox;
    u16 oy;
    Unk800C9DB4Obj *o;
    Unk800C9DB4 *h;
    s16 wx;
    s16 wy;
    s32 dx;
    s32 flags;
    s32 smooth;
    s32 ax;
    s32 ay;
    s32 i;
    Unk800C9DB4Cam *c;
    s16 cnt;
    s32 s;
    s32 v;
    s32 t;
    s32 y;
    s32 mode;
    flags = g_fieldBgState.unk0;
    if (!(flags & 0x40)) {
        return 0;
    }
    mode = flags & 7;
    smooth = flags & 8;
    if (mode == 1 || mode == 2 || mode == 3) {
        ox = g_fieldBgState.unk4.unk0;
        oy = g_fieldBgState.unk4.unk2;
        h = g_fieldBgData;
        wx = g_fieldBgState.unk14 - g_fieldBgState.unk10.unk0;
        wy = g_fieldBgState.unk16 - g_fieldBgState.unk10.unk2;
        cnt = g_fieldBgState.unk18;
        dur = g_fieldBgState.unk1A;
        if (!smooth) {
            g_fieldBgState.unk4.unk0 = g_fieldBgState.unk10.unk0 + wx * cnt / dur;
            g_fieldBgState.unk4.unk2 = g_fieldBgState.unk10.unk2 + wy * cnt / dur;
        } else {
            y = math_cos(((cnt << 16) >> 5) / dur + 0x800) + 0x1000;
            ax = wx * y / 0x2000;
            ay = wy * y / 0x2000;
            g_fieldBgState.unk4.unk0 = g_fieldBgState.unk10.unk0 + ax;
            g_fieldBgState.unk4.unk2 = g_fieldBgState.unk10.unk2 + ay;
        }
        ox = g_fieldBgState.unk4.unk0 - ox;
        oy = g_fieldBgState.unk4.unk2 - oy;
        o = (Unk800C9DB4Obj *)((u8 *)h + h->unk10);
        for (i = 0; i < h->unk6; o++, i++) {
            if (o->unk0 & 4) {
                if (o->unk1C != 0) {
                    o->unkC += ox;
                }
                if (o->unk1E != 0) {
                    o->unkE += oy;
                }
                dx = (s16)ox << 16;
            s = g_fieldBgState.unkC8[o->unk25_1] << 8;
            v = dx;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->unkC << 16) - func_800C1DC4(v, -s);
                } else {
                    v = (o->unkC << 16) + func_800C1DC4(v, s);
                }
                o->unkC = v >> 16;
                o->unk20 = (v >> 8) & 0xFF;
            }
            s = g_fieldBgState.unkD0[o->unk25_1] << 8;
            v = oy << 16;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->unkE << 16) - func_800C1DC4(v, -s);
                } else {
                    v = (o->unkE << 16) + func_800C1DC4(v, s);
                }
                o->unkE = v >> 16;
                o->unk22 = (v >> 8) & 0xFF;
            }
            } else if (o->unk0 & 0x80) {
                if (o->unk25) {
                    o->unkE += oy;
                } else {
                    o->unkC += ox;
                }
            s = g_fieldBgState.unkC8[o->unk25_1] << 8;
            v = (s16)ox << 16;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->unkC << 16) - func_800C1DC4(v, -s);
                } else {
                    v = (o->unkC << 16) + func_800C1DC4(v, s);
                }
                o->unkC = v >> 16;
                o->unk20 = (v >> 8) & 0xFF;
            }
            s = g_fieldBgState.unkD0[o->unk25_1] << 8;
            v = oy << 16;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->unkE << 16) - func_800C1DC4(v, -s);
                    o->unkE = v >> 16;
                    o->unk22 = (v >> 8) & 0xFF;
                } else {
                    v = (o->unkE << 16) + func_800C1DC4(v, s);
                    o->unkE = v >> 16;
                    o->unk22 = (v >> 8) & 0xFF;
                }
            }
            }
        }
        s = h->unk18;
        s = (s32)h - -s; /* keeps the h + off operand order (plus h s) */
        s = s + g_fieldBgState.unk75 * sizeof(Unk800C9DB4Cam);
        v = 160;
        y = *(s16 *)&g_fieldBgState.unk4.unk0 - (((Unk800C9DB4Cam *)s)->unk24 >> 1);
        i = v - y;
        t = 112 - (*(s16 *)&g_fieldBgState.unk4.unk2 - (((Unk800C9DB4Cam *)s)->unk26 >> 1));
        g_fieldBgState.unkC = i;
        g_fieldBgState.unkE = t;
        if (mode == 1) {
            g_fieldBgState.unk0 = (g_fieldBgState.unk0 & ~7) | 2;
        }
        if (++g_fieldBgState.unk18 > dur) {
            if ((g_fieldBgState.unk0 & 7) == 2) {
                g_fieldBgState.unk0 = (g_fieldBgState.unk0 & ~7) | 0x84;
            } else {
                g_fieldBgState.unk0 = (g_fieldBgState.unk0 & ~7) | 0x80;
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
    Unk800C9DB4 *h;
    Unk800C9DB4Cam *c;
    Unk8004AB64 *a;
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
    Unk800C9DB4Obj *o;
    s32 s;
    s32 v;
    flags = g_fieldBgState.unk0;
    if (flags & 7) {
        return 0;
    }
    if (!(flags & 0x40)) {
        return 0;
    }
    mode = g_gameState->mode;
    if (mode == 5) {
        a = (Unk8004AB64 *)g_gameState->unk1C->unk8E0[g_gameState->unk1C->unk8E4.field->unk8].unkC;
    } else if (mode == 1) {
        a = func_8004AE00();
    } else {
        a = NULL;
    }
    h = g_fieldBgData;
    c = (Unk800C9DB4Cam *)((u8 *)h + h->unk18);
    c += g_fieldBgState.unk75;
    if (a != NULL) {
        sv.vx = a->pos.vx;
        sv.vy = a->pos.vy - g_fieldBgState.unk76;
        sv.vz = a->pos.vz;
    } else {
        sv.vx = 0;
        sv.vy = 0;
        sv.vz = 0;
    }
    ReadGeomOffset(&ofx, &ofy);
    gte_SetGeomOffset(0xA0, 0x70);
    gte_SetRotMatrix(g_fieldBgState.unk1C);
    gte_SetTransMatrix(g_fieldBgState.unk1C);
    gte_SetGeomScreen(*g_fieldBgState.unk20);
    gte_ldv0(&sv);
    gte_rtps();
    gte_stsxy(&sxy);
    gte_SetGeomOffset(ofx, ofy);
    x = sxy.v.vx;
    y = sxy.w >> 16;
    if (g_fieldBgState.unk0 & 0x80) {
        g_fieldBgState.unk2C = x;
        g_fieldBgState.unk2E = y;
    }
    x = (g_fieldBgState.unk2C + x) >> 1;
    y = (g_fieldBgState.unk2E + y) >> 1;
    sx = 320 - x;
    sy = 224 - y;
    g_fieldBgState.unk2C = x;
    g_fieldBgState.unk2E = y;
    wx = (c->unk24 >> 1) + (s16)c->unk20;
    lim = wx - 160;
    cx = lim + x;
    {
        s32 t = (c->unk26 >> 1) + (s16)c->unk22 - 112;
        cy = t + y;
    }
    if (cx < c->unk28) {
        {
            s32 t = c->unk28 - 160;
            sx = wx - t;
        }
    } else if (c->unk2A < cx) {
        {
            s32 t = c->unk2A - 160;
            sx = wx - t;
        }
    }
    if (cy < c->unk2C) {
        {
            s32 a = ((c->unk26 >> 1) + (s16)c->unk22);
            s32 t = c->unk2C - 112;
            sy = a - t;
        }
    } else if (c->unk2E < cy) {
        {
            s32 a = ((c->unk26 >> 1) + (s16)c->unk22);
            s32 t = c->unk2E - 112;
            sy = a - t;
        }
    }
    g_fieldBgState.unkC = sx - c->unk20;
    g_fieldBgState.unkE = sy - c->unk22;
    px = &dx;
    py = &dy;
    if (g_fieldBgState.unk0 & 0x40) {
        ox = g_fieldBgState.unk4.unk0;
        oy = g_fieldBgState.unk4.unk2;
        if ((s16)cx < c->unk28) {
            g_fieldBgState.unk4.unk0 = c->unk28;
        } else if (c->unk2A < (s16)cx) {
            g_fieldBgState.unk4.unk0 = c->unk2A;
        } else {
            g_fieldBgState.unk4.unk0 = cx;
        }
        if ((s16)cy < c->unk2C) {
            g_fieldBgState.unk4.unk2 = c->unk2C;
        } else if (c->unk2E < (s16)cy) {
            g_fieldBgState.unk4.unk2 = c->unk2E;
        } else {
            g_fieldBgState.unk4.unk2 = cy;
        }
        *px = g_fieldBgState.unk4.unk0 - ox;
        *py = g_fieldBgState.unk4.unk2 - oy;
    }
    o = (Unk800C9DB4Obj *)((u8 *)h + h->unk10);
    for (i = 0; i < h->unk6; o++, i++) {
        if (o->unk0 & 4) {
            if (o->unk1C != 0) {
                o->unkC += dx;
            }
            if (o->unk1E != 0) {
                o->unkE += dy;
            }
        s = g_fieldBgState.unkC8[o->unk25_1] << 8;
        v = dx << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->unkC << 16) - func_800C1DC4(v, -s);
            } else {
                v = (o->unkC << 16) + func_800C1DC4(v, s);
            }
            o->unkC = v >> 16;
            o->unk20 = (v >> 8) & 0xFF;
        }
        s = g_fieldBgState.unkD0[o->unk25_1] << 8;
        v = dy << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->unkE << 16) - func_800C1DC4(v, -s);
            } else {
                v = (o->unkE << 16) + func_800C1DC4(v, s);
            }
            o->unkE = v >> 16;
            o->unk22 = (v >> 8) & 0xFF;
        }
        } else if (o->unk0 & 0x80) {
            if (o->unk25) {
                o->unkE += dy;
            } else {
                o->unkC += dx;
            }
        s = g_fieldBgState.unkC8[o->unk25_1] << 8;
        v = dx << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->unkC << 16) - func_800C1DC4(v, -s);
            } else {
                v = (o->unkC << 16) + func_800C1DC4(v, s);
            }
            o->unkC = v >> 16;
            o->unk20 = (v >> 8) & 0xFF;
        }
        s = g_fieldBgState.unkD0[o->unk25_1] << 8;
        v = dy << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->unkE << 16) - func_800C1DC4(v, -s);
                o->unkE = v >> 16;
                o->unk22 = (v >> 8) & 0xFF;
            } else {
                v = (o->unkE << 16) + func_800C1DC4(v, s);
                o->unkE = v >> 16;
                o->unk22 = (v >> 8) & 0xFF;
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
    Unk800C9DA4Hdr *h;
    Unk800C9DA4Anim *anims;
    Unk800C9DA4Trk *trk;
    u32 i;
    u32 n;
    s32 t;
    s32 x;
    union {
        s32 w;
        DVECTOR v;
    } u; /* projected origin */

    g_fieldBgState.unk0 = 0x60;
    g_fieldBgState.unk74 = 0x40;
    g_fieldBgState.unk76 = 0x144;
    g_fieldBgState.unk78 = 0xE10;
    *(s32 *)&g_fieldBgState.unk4 = 0;
    *(s32 *)&g_fieldBgState.unk8 = 0;
    *(s32 *)&g_fieldBgState.unkC = 0;
    *(s32 *)&g_fieldBgState.unk10 = 0;
    *(s32 *)&g_fieldBgState.unk14 = 0;
    *(s32 *)&g_fieldBgState.unk18 = 0;
    *(s32 *)&g_fieldBgState.unk24 = 0;
    *(s32 *)&g_fieldBgState.unk28 = 0;
    *(s32 *)&g_fieldBgState.unk2C = 0;
    g_fieldBgState.unk75 = 0;
    g_fieldBgState.unk9C = 0;
    g_fieldBgState.unkA0 = 0;
    g_fieldBgState.unkA1 = 0xFF;
    g_fieldBgState.unkA2 = 0xFF;
    g_fieldBgState.unkA3 = 0xFF;
    *(s32 *)&g_fieldBgState.unkA4 = 0;
    g_fieldBgState.unk1C = (MATRIX *)g_fieldBgCtx.unk14;
    g_fieldBgState.unk20 = (u16 *)g_fieldBgCtx.unk4;
    if (fldbg_initLayers(g_fieldBgCtx.unk18, &spr) != 1) {
        return NULL;
    }
    h = g_fieldBgCtx.unk10;
    anims = (Unk800C9DA4Anim *)((u8 *)h + h->unkC);
    trk = (Unk800C9DA4Trk *)((u8 *)h + h->unk10);
    n = h->unk4;
    for (i = 0; i < n; i++) {
        anims[i].unk4 &= 0xFF;
        anims[i].unk8 = 0x100;
        anims[i].unkA = 0;
        anims[i].unk0 = 1;
        trk[*((u8 *)h + anims[i].unkC)].unk0 |= 2;
    }
    fldbg_initEffectList();
    fldbg_initFollowLayers();
    v.vx = 0;
    v.vy = 0;
    v.vz = 0;
    gte_SetGeomOffset(0xA0, 0x70);
    gte_SetRotMatrix(g_fieldBgState.unk1C);
    gte_SetTransMatrix(g_fieldBgState.unk1C);
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(&u.w);
    t = u.w;
    x = u.v.vx;
    g_fieldBgState.unk2C = x;
    g_fieldBgState.unk2E = t >> 16;
    return spr;
}

s32 fldbg_update(void) {
    Unk800C9DA4Hdr *h;
    Unk800C9DB4Obj *o;
    Unk800CA068 *c;
    u32 i;
    s32 j;

    fldbg_updateLayerAnims();
    /* the switch results are dead; only the loads and the hoisted 0x10/0x40 survive */
    do {
        for (i = 0; i < g_fieldBgCtx.unk24; i++) {
            switch (g_fieldBgCtx.unk28[i].unk0) {
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
    fldbg_updateLayerScroll(g_fieldBgCtx.unk10);
    fldbg_updateFollowLayers();
    c = &g_fieldBgState;
    h = g_fieldBgCtx.unk10;
    h->unk30 = h->unk24 - (s16)(c->unk4.unk0 - 0xA0);
    h->unk32 = h->unk26 - (s16)(c->unk4.unk2 - 0x70);
    o = (Unk800C9DB4Obj *)((u8 *)h + h->unk10);
    for (j = 0; j < h->unk6; j++) {
        if ((o[j].unk0 & 2) && o[j].unk24 == c->unk75) {
            fldbg_drawLayer(&o[j], g_fieldBgCtx.unk1C + j * 3);
        }
    }
    return 1;
}

s32 fldbg_scrollToPlayer(s32 arg0, s32 arg1) {
    SVECTOR v;
    s16 xy[2];
    Unk800C9DB4Cam *c;
    Unk8004AB64 *a;
    u8 mode;
    s32 x;
    s32 y;
    s32 t;
    s32 cp;
    Unk800CA068 *p;
    s32 f;
    s32 ox;
    s32 oy;
    Unk800C9DB4 *bg;
    Unk800CA068 *st = NULL; /* dead store: lets cse keep the st = p copy below */

    if (!(g_fieldBgState.unk0 & 0x40)) {
        return 0;
    }
    mode = g_gameState->mode;
    if (mode == 5) {
        a = (Unk8004AB64 *)g_gameState->unk1C->unk8E0[g_gameState->unk1C->unk8E4.field->unk8].unkC;
    } else {
        a = NULL;
        if (mode == 1) {
            a = func_8004AE00();
        }
    }
    bg = g_fieldBgData;
    p = &g_fieldBgState;
    c = (Unk800C9DB4Cam *)((u8 *)bg + bg->unk18);
    cp = *(s32 *)&p->unk4;
    /* a second pointer to g_fieldBgState whose copy disappears after register allocation; it is what
       makes sched1 order the copy/vx loads and local-alloc give the copy $v0 as in the original */
    st = p;
    *(s32 *)&st->unk10 = cp;
    v.vx = a->pos.vx;
    v.vy = a->pos.vy - st->unk76;
    c = &c[st->unk75];
    v.vz = a->pos.vz;
    ox = (s16)c->unk20;
    oy = (s16)c->unk22;
    gte_SetGeomOffset(ox + 0xA0, oy + 0x70);
    {
        /* the original's "r"(st->unk1C) operand was reloaded into $t0; as a hard-register variable
           it is one less reference to st, which keeps st below the unk76 temporary in local-alloc */
        register MATRIX *m REG_ASM("$8") = st->unk1C;
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
        s32 t = (c->unk24 >> 1) - 0xA0;
        x = t + xy[0];
    }
    {
        s32 t = (c->unk26 >> 1) - 0x70;
        y = t + xy[1];
    }
    if (x < c->unk28) {
        x = c->unk28;
    } else if (c->unk2A < x) {
        x = c->unk2A;
    }
    if (y < c->unk2C) {
        y = c->unk2C;
    } else if (c->unk2E < y) {
        y = c->unk2E;
    }
    g_fieldBgState.unk14 = x;
    g_fieldBgState.unk16 = y;
    if (arg0 == -1) {
        g_fieldBgState.unk1A = 0x1E;
    } else {
        g_fieldBgState.unk1A = arg0;
    }
    g_fieldBgState.unk18 = 1;
    if (arg1 == -1) {
        g_fieldBgState.unk0 = (g_fieldBgState.unk0 & ~7) | 3;
    } else {
        f = g_fieldBgState.unk0 & ~0xF;
        g_fieldBgState.unk0 = f | 3;
        if (arg1 == 8) {
            g_fieldBgState.unk0 = f | 8 | 3;
        } else {
            g_fieldBgState.unk0 = f | 3;
        }
    }
    return 1;
}

/* add a screen fade: rectangle (arg0..arg2), mode arg3, colour arg4..arg6 */
s32 fldbg_addFollowLayer(s16 arg0, s16 arg1, s16 arg2, s8 arg3, u8 arg4, u8 arg5, u8 arg6) {
    u32 i;

    g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unk4 = arg0;
    g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unk6 = arg1;
    g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unk8 = arg2;
    g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unk0 = arg3;
    if (arg3 >= 0) {
        g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unk1 = arg4;
        g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unk2 = arg5;
        g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unk3 = arg6;
        for (i = 0; i < 4; i++) {
            SetPolyF4(&g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unkC[0][i]);
            setRGB0(&g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unkC[0][i], arg4, arg5, arg6);
            SetPolyF4(&g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unkC[0][i + 4]); /* = unkC[1][i] */
            setRGB0(&g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unkC[1][i], arg4, arg5, arg6);
            if (arg3 >= 4) {
                setSemiTrans(&g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unkC[0][i], 0);
                setSemiTrans(&g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unkC[1][i], 0);
            } else {
                setSemiTrans(&g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unkC[0][i], 1);
                setSemiTrans(&g_fieldBgFollowLayers[g_fieldBgFollowLayerCount].unkC[1][i], 1);
            }
        }
    }
    g_fieldBgFollowLayerCount++;
    return 1;
}

/* toggle a background object's tint */
s32 fldbg_toggleLayerColor(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    Unk800C9DA4Hdr *h;
    Unk800C9DB4Obj *o;
    BgSprt *s;
    u8 t;
    u16 n;
    s32 off;
    u16 i;

    t = g_fieldBgCtx.unk20[arg0];
    h = g_fieldBgCtx.unk10;
    if (t != 0) {
        o = &((Unk800C9DB4Obj *)((u8 *)h + h->unk10))[arg0];
        n = o->unk26;
        s = o->unk30;
        off = 0;
        if (*g_fieldBgCtx.unk0 != 0) {
            off = n;
        }
        for (i = 0; i < n; i++) {
            setRGB0(&s[i + off].unk8, arg1, arg2, arg3);
        }
        g_fieldBgCtx.unk20[arg0] = 0;
        return 0;
    }
    g_fieldBgCtx.unk1C[arg0 * 3 + 0] = arg1;
    g_fieldBgCtx.unk1C[arg0 * 3 + 1] = arg2;
    g_fieldBgCtx.unk1C[arg0 * 3 + 2] = arg3;
    o = &((Unk800C9DB4Obj *)((u8 *)h + h->unk10))[arg0];
    n = o->unk26;
    s = o->unk30;
    off = 0;
    if (*g_fieldBgCtx.unk0 != 0) {
        off = n;
    }
    for (i = 0; i < n; i++) {
        setRGB0(&s[i + off].unk8, arg1, arg2, arg3);
    }
    g_fieldBgCtx.unk20[arg0] = 1;
    return 1;
}

s32 fldbg_startClutFade(s32 arg0, u8 arg1, u32 arg2, u8 arg3, s32 arg4, u8 arg5, u8 arg6, u8 arg7) {
    s32 rgb[3];
    Unk800C9DA4Ent *e;
    FieldPalAnim *p;
    Unk800C9DA4Hdr *h;
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
    e = &g_fieldBgCtx.unk28[arg0];
    e->unk0 = 0x80;
    e->unk8 = size;
    h = g_fieldBgCtx.unk10;
    q = (u8 *)h + h->unk10;
    y = *(u32 *)((u8 *)h + ((Unk800C9DB4Obj *)q)[e->unk1].unk2C) & 0x1FF;
    p = (FieldPalAnim *)(g_fieldBgCtx.unk2C + g_fieldBgCtx.unk34);
    /* size is reused for the new end offset (two sets keep sched1 from moving its computation down to
     * e->unk8), read through a copy so the add keeps the original operand order */
    k = size;
    size = g_fieldBgCtx.unk34 + k;
    g_fieldBgCtx.unk34 = size;
    e->unk4 = (u8 *)p - (u8 *)g_fieldBgCtx.unk28;
    p->unk0 = 1;
    p->unk1 = n;
    p->unk2 = arg3;
    p->unk3 = arg4;
    p->unk8 = arg2;
    p->unkC = 0;
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
    p->unk10 = rgb[0];
    p->unk14 = rgb[1];
    p->unk18 = rgb[2];
    p->unk1C = 0x2C;
    p->unk20 = n * 2 + 0x2C;
    p->unk24.x = arg3;
    p->unk24.y = y;
    p->unk24.w = n;
    p->unk24.h = 1;
    StoreImage(&p->unk24, (u32 *)((u8 *)p + p->unk1C));
    w = (u16 *)((u8 *)p + p->unk20);
    /* read the CLUT back; the original keeps the value in v1 (see REG_ASM) */
    for (i = 0; i < n; i++) {
        v = w[i];
    }
    g_fieldBgCtx.unk34 += e->unk8;
    return 1;
}

s32 fldbg_setCamera(s32 arg0) {
    Unk800C9DB4Cam *c;
    Unk800C9DB4 *h;

    h = g_fieldBgData;
    c = (Unk800C9DB4Cam *)((u8 *)h + h->unk18) + arg0;
    *g_fieldBgState.unk20 = c->unk0;
    gte_SetGeomScreen(c->unk0);
    g_fieldBgState.unk1C->m[0][0] = c->unk2[0][0];
    g_fieldBgState.unk1C->m[0][1] = c->unk2[0][1];
    g_fieldBgState.unk1C->m[0][2] = c->unk2[0][2];
    g_fieldBgState.unk1C->m[1][0] = c->unk2[1][0];
    g_fieldBgState.unk1C->m[1][1] = c->unk2[1][1];
    g_fieldBgState.unk1C->m[1][2] = c->unk2[1][2];
    g_fieldBgState.unk1C->m[2][0] = c->unk2[2][0];
    g_fieldBgState.unk1C->m[2][1] = c->unk2[2][1];
    g_fieldBgState.unk1C->m[2][2] = c->unk2[2][2];
    g_fieldBgState.unk1C->t[0] = c->unk14[0];
    g_fieldBgState.unk1C->t[1] = c->unk14[1];
    g_fieldBgState.unk1C->t[2] = c->unk14[2];
    h->unk2A = c->unk2A;
    h->unk2E = c->unk2E;
    g_fieldBgState.unk75 = arg0;
    g_fieldBgState.unk0 |= 0x80;
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk30 = c->unk30;
    return 1;
}

void func_800C0D38(void) {
}

void fldbg_setRenderContext(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5) {
    g_fieldBgCtx.unk0 = (u8 *)arg0;
    g_fieldBgCtx.unk4 = arg1;
    g_fieldBgCtx.unk8[0] = (u32 *)arg2;
    g_fieldBgCtx.unk8[1] = (u32 *)arg3;
    g_fieldBgCtx.unk10 = 0;
    g_fieldBgCtx.unk24 = 0;
    g_fieldBgCtx.unk28 = NULL;
    g_fieldBgCtx.unk2C = 0;
    g_fieldBgCtx.unk30 = 0;
    g_fieldBgCtx.unk34 = 0;
    g_fieldBgCtx.unk14 = arg4;
    g_fieldBgCtx.unk18 = arg5;
    g_fieldBgCtx.unk1C = arg5;
    g_fieldBgCtx.unk20 = arg5;
}

void fldbg_setWorkBuffer(s32 arg0, s32 arg1) {
    g_fieldBgCtx.unk2C = arg0;
    g_fieldBgCtx.unk30 = arg1;
    g_fieldBgCtx.unk34 = 0;
}

void fldbg_setData(Unk800C9DB4 *arg0) {
    g_fieldBgData = arg0;
}

s32 fldbg_moveOrigin(s32 arg0, s32 arg1, s32 arg2) {
    u16 x = g_fieldBgData->unk20 + arg0;
    u16 y = g_fieldBgData->unk22 + arg1;
    u16 z;

    g_fieldBgData->unk20 = x;
    g_fieldBgData->unk22 = y;
    g_fieldBgData->unk24 = x;
    g_fieldBgData->unk26 = y;
    z = g_fieldBgData->unk1C + arg2;
    g_fieldBgData->unk1C = z;
    g_fieldBgData->unk1E = z;
    return 1;
}

s32 fldbg_scrollTo(s16 arg0, s16 arg1, s16 arg2, s32 arg3) {
    s32 f;

    if (!(g_fieldBgState.unk0 & 0x40)) {
        return 0;
    }
    g_fieldBgState.unk18 = 1;
    g_fieldBgState.unk14 = arg0;
    f = g_fieldBgState.unk0 & ~0xF;
    g_fieldBgState.unk16 = arg1;
    g_fieldBgState.unk1A = arg2;
    g_fieldBgState.unk0 = f;
    g_fieldBgState.unk10 = g_fieldBgState.unk4;
    if (arg3 == 8) {
        g_fieldBgState.unk0 = f | 8;
    } else {
        g_fieldBgState.unk0 = f;
    }
    g_fieldBgState.unk0 |= 1;
    return 1;
}

s32 fldbg_getScroll(u16 *arg0, u16 *arg1) {
    *arg0 = g_fieldBgState.unk4.unk0;
    *arg1 = g_fieldBgState.unk4.unk2;
    return 1;
}

s32 fldbg_setFollow(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 != 0) {
        g_fieldBgState.unk0 |= 0x40;
        if (arg1 > 0) {
            fldbg_scrollToPlayer(arg1, arg2);
        }
    } else {
        g_fieldBgState.unk0 &= ~0x40;
    }
    return 1;
}

s32 fldbg_isFollowing(void) {
    if (g_fieldBgState.unk0 & 0x40) {
        return 1;
    }
    return 0;
}

s32 fldbg_applyGeomOffset(void) {
    Unk800C9DB4Cam *c = func_800C9DB4_cam(0);

    c += g_fieldBgState.unk75;
    if (!(func_80012AD4() & 2) || g_mbgFrame < 2) {
        gte_SetGeomOffset((s16)c->unk20 + g_fieldBgState.unkC, (s16)c->unk22 + g_fieldBgState.unkE);
    }
    return 1;
}

s32 fldbg_resetGeomOffset(void) {
    if (!(func_80012AD4() & 2)) {
        gte_SetGeomOffset(0xA0, 0x70);
    }
    return 1;
}

s32 func_800C0FF4(s32 arg0) {
    if (arg0 == -1) {
        D_800CA0E0 = 3600;
    } else {
        D_800CA0E0 = arg0 * arg0;
    }
    return 1;
}

s32 fldbg_setFollowHeight(s16 arg0) {
    g_fieldBgState.unk76 = arg0;
    return 1;
}

s32 fldbg_hasScrollLimits(void) {
    if (g_fieldBgState.unk0 & 0x20) {
        return 1;
    }
    return 0;
}

s32 fldbg_disableScrollLimits(void) {
    Unk800C9DB4Cam *c;

    if (!(g_fieldBgState.unk0 & 0x20)) {
        return 0;
    }
    if (g_fieldBgData != NULL) {
        c = func_800C9DB4_cam(g_fieldBgState.unk75);
        g_fieldBgState.unk24 = c->unk28;
        g_fieldBgState.unk26 = c->unk2A;
        g_fieldBgState.unk28 = c->unk2C;
        g_fieldBgState.unk2A = c->unk2E;
        c->unk28 = -0x8000;
        c->unk2A = 0x7FFF;
        c->unk2C = -0x8000;
        c->unk2E = 0x7FFF;
        g_fieldBgState.unk0 &= ~0x20;
    }
    return 1;
}

s32 fldbg_enableScrollLimits(void) {
    Unk800C9DB4Cam *c;

    if (g_fieldBgState.unk0 & 0x20) {
        return 0;
    }
    if (g_fieldBgData != NULL) {
        c = func_800C9DB4_cam(g_fieldBgState.unk75);
        c->unk28 = g_fieldBgState.unk24;
        c->unk2A = g_fieldBgState.unk26;
        c->unk2C = g_fieldBgState.unk28;
        c->unk2E = g_fieldBgState.unk2A;
        g_fieldBgState.unk0 |= 0x20;
    }
    return 1;
}

s32 fldbg_setViewport(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    g_fieldBgState.unkA8[arg0][0] = arg1;
    g_fieldBgState.unkA8[arg0][1] = arg2;
    g_fieldBgState.unkB8[arg0][0] = arg3;
    g_fieldBgState.unkB8[arg0][1] = arg4;
    return 1;
}

s32 fldbg_setViewportScrollRate(s32 arg0, s16 arg1, s16 arg2) {
    g_fieldBgState.unkC8[arg0] = arg1;
    g_fieldBgState.unkD0[arg0] = arg2;
    return 1;
}

/* move a background object, clamped to its limits */
s32 fldbg_moveLayer(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    Unk800C9DB4Obj *o = &((Unk800C9DB4Obj *)((u8 *)g_fieldBgData + g_fieldBgData->unk10))[arg0];
    s16 x = o->unk8 + arg1;
    s16 y = o->unkA + arg2;
    s16 z = arg3 + o->unk2;

    if (x < o->unk10) {
        x = o->unk10;
    } else if (x > o->unk12) {
        x = o->unk12;
    }
    if (y < o->unk14) {
        y = o->unk14;
    } else if (y > o->unk16) {
        y = o->unk16;
    }
    o->unk8 = x;
    o->unk2 = z;
    o->unkA = y;
    o->unkC = x;
    o->unkE = y;
    o->unk1 = z;
    return 1;
}

s32 fldbg_getLayerColor(s32 arg0, u8 *arg1, u8 *arg2, u8 *arg3) {
    *arg1 = g_fieldBgCtx.unk1C[arg0 * 3];
    *arg2 = g_fieldBgCtx.unk1C[arg0 * 3 + 1];
    *arg3 = g_fieldBgCtx.unk1C[arg0 * 3 + 2];
    return 1;
}

s32 fldbg_setLayerColor(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    Unk800C9DA4Hdr *h;
    Unk800C9DB4Obj *o;
    BgSprt *s;
    u16 n;
    s32 off;
    u16 i;

    h = g_fieldBgCtx.unk10;
    g_fieldBgCtx.unk1C[arg0 * 3 + 0] = arg1;
    g_fieldBgCtx.unk1C[arg0 * 3 + 1] = arg2;
    g_fieldBgCtx.unk1C[arg0 * 3 + 2] = arg3;
    o = &((Unk800C9DB4Obj *)((u8 *)h + h->unk10))[arg0];
    n = o->unk26;
    s = o->unk30;
    off = 0;
    if (*g_fieldBgCtx.unk0 != 0) {
        off = n;
    }
    for (i = 0; i < n; i++) {
        setRGB0(&s[i + off].unk8, arg1, arg2, arg3);
    }
    return 1;
}

s32 fldbg_showLayer(s32 arg0, s32 arg1) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);
    u8 v;

    if (arg1 != 0) {
        v = o->unk0 | 2;
    } else {
        v = o->unk0 & ~2;
    }
    o->unk0 = v;
    return 1;
}

s32 fldbg_setLayerLoop(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);

    if (arg1 != 0) {
        o->unk0 |= 4;
    } else {
        o->unk0 &= ~4;
    }
    o->unk1C = arg2;
    o->unk20 = 0;
    o->unk1E = arg3;
    o->unk22 = 0;
    return 1;
}

s32 fldbg_setLayerFlag1(s32 arg0, s32 arg1) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);
    u8 v;

    if (arg1 != 0) {
        v = o->unk0 | 1;
    } else {
        v = o->unk0 & ~1;
    }
    o->unk0 = v;
    return 1;
}

s32 fldbg_setLayerSkewLoop(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);

    if (arg1 != 0) {
        o->unk0 |= 0x80;
    } else {
        o->unk0 &= ~0x80;
    }
    if (arg4 != 0) {
        o->unk1C = arg3;
        o->unk1E = arg2;
        o->unk25 = 1;
    } else {
        o->unk1C = arg2;
        o->unk1E = arg3;
        o->unk25 = 0;
    }
    o->unk20 = 0;
    o->unk22 = 0;
    return 1;
}

s32 fldbg_setLayerParallax(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);

    if (arg1 != 0) {
        o->unk0 |= 8;
    } else {
        o->unk0 &= ~8;
    }
    o->unk1C = arg2;
    o->unk1E = arg3;
    return 1;
}

s32 fldbg_setLayerPos(s32 arg0, s16 arg1, s16 arg2) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);

    o->unkC = arg1;
    o->unk8 = arg1;
    o->unkE = arg2;
    o->unkA = arg2;
    g_fieldBgState.unk0 |= 0x80;
    return 1;
}

s32 fldbg_setLayerViewport(s32 arg0, s32 arg1) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);

    o->unk25_1 = arg1;
    return 1;
}

s32 fldbg_setAnimFrame(s32 arg0, s32 arg1) {
    Unk800C9DB4Path *p = func_800C9DB4_path(arg0);

    p->unkA = 0;
    p->unk0 |= 2;
    p->unk5 = arg1 << 8;
    return 1;
}

s32 fldbg_showAnimFrame(s32 arg0, s32 arg1) {
    s32 i;
    Unk800C9DB4Path *p;
    Unk800C9DB4Step *st;
    Unk800C9DB4Obj *objs;

    i = 0;
    p = func_800C9DB4_path(arg0);
    st = func_800C9DB4_steps(p);
    objs = func_800C9DB4_obj(0);
    for (; i < p->unk1; st++, i++) {
        if (i == arg1) {
            objs[st->unk0].unk0 |= 2;
        } else {
            objs[st->unk0].unk0 &= ~2;
        }
    }
    p->unk0 = 1;
    return 1;
}

s32 fldbg_playAnim(s32 arg0, s32 arg1) {
    Unk800C9DB4Path *p = func_800C9DB4_path(arg0);
    u8 v;

    if (arg1 != 0) {
        v = p->unk0 | 6;
    } else {
        v = p->unk0 & ~6;
    }
    p->unk0 = v;
    return 1;
}

s32 fldbg_setAnimSpeed(s32 arg0, s16 arg1) {
    func_800C9DB4_path(arg0)->unk8 = arg1;
    return 1;
}

s32 fldbg_setAnimDelayAll(s32 arg0, u8 arg1) {
    Unk800C9DB4Path *p = func_800C9DB4_path(arg0);
    Unk800C9DB4Step *st = func_800C9DB4_steps(p);
    s32 n = p->unk1;
    s32 i;

    for (i = 0; i < n; i++) {
        st[i].unk1 = arg1;
    }
    return 1;
}

s32 fldbg_setAnimStepDelay(s32 arg0, s32 arg1, u8 arg2) {
    func_800C9DB4_steps(func_800C9DB4_path(arg0))[arg1].unk1 = arg2;
    return 1;
}

s32 fldbg_setAnimFlags(s32 arg0, s32 arg1) {
    func_800C9DB4_path(arg0)->unk0 |= arg1 & 0x30;
    return 1;
}

s32 fldbg_playAnimRange(s32 arg0, u32 arg1, u32 arg2) {
    Unk800C9DB4 *b = g_fieldBgData;
    Unk800C9DB4Path *p = &((Unk800C9DB4Path *)((u8 *)b + b->unkC))[arg0];
    Unk800C9DB4Step *st = (Unk800C9DB4Step *)((u8 *)b + p->unkC);

    p->unkA = 0;
    p->unk0 |= 6;
    p->unk5 = arg1 << 8;
    st[arg2].unk1 = -1;
    if ((p->unk8 > 0 && arg2 < arg1) || (p->unk8 < 0 && arg1 < arg2)) {
        p->unk8 = -p->unk8;
    }
    return 1;
}

s32 fldbg_resetAnim(s32 arg0, s32 arg1) {
    Unk800C9DB4 *d = g_fieldBgData;
    Unk800C9DB4Path *p = &((Unk800C9DB4Path *)((u8 *)d + d->unkC))[arg0];
    Unk800C9DB4Obj *objs;
    Unk800C9DB4Step *st;
    s32 i;

    p->unk8 = 0x100;
    i = 0;
    p->unkA = 0;
    p->unk0 = 1;
    p->unk5 = 0;
    objs = (Unk800C9DB4Obj *)((u8 *)d + d->unk10);
    st = (Unk800C9DB4Step *)((u8 *)d + p->unkC);
    for (; i < p->unk1; i++) {
        if (arg1 != 0 && i == 0) {
            objs[st[i].unk0].unk0 |= 2;
        } else {
            objs[st[i].unk0].unk0 &= ~2;
        }
    }
    return 1;
}

s32 func_800C1A4C(s32 arg0, u8 *arg1) {
    *arg1 = D_800C9DCC[arg0].unk0;
    return 1;
}

s32 func_800C1A78(s32 arg0) {
    return D_800C9DD0 + (arg0 << 9);
}

/* set the camera scroll limits */
s32 fldbg_setScrollLimits(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    Unk800C9DB4Cam *c = &((Unk800C9DB4Cam *)((u8 *)g_fieldBgData + g_fieldBgData->unk18))[arg0];
    s16 v;

    c->unk28 = v = arg1 + 160;
    if (v > c->unk24 - 160) {
        c->unk28 = c->unk24 - 160;
    }
    c->unk2A = v = arg2 - 160;
    if (v < 160) {
        c->unk2A = 160;
    }
    c->unk2C = v = arg3 + 112;
    if (v > c->unk26 - 112) {
        c->unk2C = c->unk26 - 112;
    }
    c->unk2E = v = arg4 - 112;
    if (v < 112) {
        c->unk2E = 112;
    }
    return 1;
}

s32 fldbg_getCamera(u8 *arg0) {
    *arg0 = g_fieldBgState.unk75;
    return 1;
}

s32 fldbg_setCameraCenter(s32 arg0, s16 arg1, s16 arg2) {
    Unk800C9DB4Cam *c = func_800C9DB4_cam(arg0);

    c->unk20 = arg1;
    c->unk22 = arg2;
    return 1;
}

s32 fldbg_getCameraCenter(s32 arg0, u16 *arg1, u16 *arg2) {
    Unk800C9DB4Cam *c = func_800C9DB4_cam(arg0);

    *arg1 = c->unk20;
    *arg2 = c->unk22;
    return 1;
}

s32 fldbg_getClutFadeSize(s32 arg0) {
    return arg0 * 4 + 0x2C;
}

s32 fldbg_allocWork(s32 arg0) {
    s32 r = g_fieldBgCtx.unk2C + g_fieldBgCtx.unk34;

    g_fieldBgCtx.unk34 += arg0;
    return r;
}

s32 fldbg_resetAllAnims(void) {
    Unk800C9DB4 *d = g_fieldBgData;
    u32 i;
    Unk800C9DB4Path *p;
    Unk800C9DB4Obj *objs;
    u16 n;

    i = 0;
    p = (Unk800C9DB4Path *)((u8 *)d + d->unkC);
    objs = (Unk800C9DB4Obj *)((u8 *)d + d->unk10);
    n = d->unk4;
    for (; i < n; i++) {
        p[i].unk8 = 0x100;
        p[i].unkA = 0;
        p[i].unk0 = 1;
        p[i].unk5 = 0;
        objs[((Unk800C9DB4Step *)((u8 *)d + p[i].unkC))->unk0].unk0 |= 2;
    }
    return 1;
}

s32 func_800C1CC0(void) {
    u32 i;
    s32 ret;
    s32 type;

    ret = 1;
    for (i = 0; i < g_fieldBgCtx.unk24; i++) {
        type = g_fieldBgCtx.unk28[i].unk0;
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

s32 fldbg_setLayerSpriteColor(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);
    u16 n = o->unk26;
    BgSprt *s = o->unk30;
    u16 i;

    for (i = 0; i < n; i++) {
        setRGB0(&s[i].unk8, arg1, arg2, arg3);
        setRGB0(&s[i + n].unk8, arg1, arg2, arg3);
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
