#include "ovl_0b.h"

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/btl_mesh", btl_drawMesh);

void btl_projectMesh(BtlUnit9C *arg0, u8 arg1) {
    MATRIX m;
    BtlMesh *g;
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
        n = g->unk14[i++];
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
            vtx = &g->unk18[k];
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

u8 *func_800C67BC(BtlUnit9C *arg0, u8 arg1, u8 *prim) {
    s32 opz;
    s32 otz;
    BtlMesh *g;
    u8 (*uv)[2];
    u8 (*col)[4];
    u32 *ot;
    u32 *tex;
    u32 n;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 *cp;
    u32 i0, i1, i2, i3;
    u32 t;
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

    if (arg0->unk0 & 0x10) {
        return prim;
    }
    g = &arg0->unk8->unk10[arg1];
    sFT4 = (WmSrcFT4 *)g->unk1C;
    uv = g->unk20;
    col = g->unk24;
    if (arg0->unk10 == 0) {
        arg0->unk10 = (s32)prim;
    }
    tex = (u32 *)arg0->unk18;
    ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8];
    ot -= g->unk13;

    n = g->unk2;
    for (i = 0; i < n; sFT4++) {
        pFT4 = (POLY_FT4 *)prim;
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
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(u32 *)&pFT4->r0 = *(u32 *)sFT4->unk10;
        setlen(pFT4, 9);
        *(s32 *)&pFT4->x0 = xy0;
        *(s32 *)&pFT4->x1 = xy1;
        *(s32 *)&pFT4->x2 = xy2;
        setcode(pFT4, 0x2C);
        *(s32 *)&pFT4->x3 = g_vtxScreenXY[i3];
        otz = g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3];
        i0 = *(u32 *)&sFT4->unk8[0];
        i2 = *(u32 *)&sFT4->unk8[2];
        t = tex[sFT4->unk13];
        hi = t & 0xFFFF0000;
        t <<= 16;
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        *(u32 *)&pFT4->u0 = *(u16 *)uv[i0] | hi;
        *(u32 *)&pFT4->u1 = *(u16 *)uv[i1] | t;
        *(u16 *)&pFT4->u2 = *(u16 *)uv[i2];
        *(u16 *)&pFT4->u3 = *(u16 *)uv[i3];
        if (sFT4->unk15 & 1) {
            pFT4->tpage |= sFT4->unk15 & 0xE0;
            setSemiTrans(pFT4, 1);
        }
        otz >>= 6;
        addPrim(&ot[otz], pFT4);
        prim += sizeof(POLY_FT4);
    }

    n = g->unk4;
    sFT3 = (WmSrcFT3 *)sFT4;
    for (i = 0; i < n; sFT3++) {
        pFT3 = (POLY_FT3 *)prim;
        i0 = *(u32 *)&sFT3->unk0[0];
        i2 = *(u32 *)&sFT3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sFT3->unk12 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(u32 *)&pFT3->r0 = *(u32 *)sFT3->unk8;
        setlen(pFT3, 7);
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        *(s32 *)&pFT3->x2 = xy2;
        setcode(pFT3, 0x24);
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        t = tex[sFT3->unk6];
        hi = t & 0xFFFF0000;
        t <<= 16;
        gte_avsz3_b();
        i0 = *(u32 *)&sFT3->unkC[0];
        i2 = *(u32 *)&sFT3->unkC[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        *(u32 *)&pFT3->u0 = *(u16 *)uv[i0] | hi;
        *(u32 *)&pFT3->u1 = *(u16 *)uv[i1] | t;
        *(u16 *)&pFT3->u2 = *(u16 *)uv[i2];
        gte_stotz(&otz);
        if (sFT3->unk12 & 1) {
            pFT3->tpage |= sFT3->unk12 & 0xE0;
            setSemiTrans(pFT3, 1);
        }
        otz >>= 2;
        addPrim(&ot[otz], pFT3);
        prim += sizeof(POLY_FT3);
    }

    n = g->unk6;
    sGT4 = (WmSrcGT4 *)sFT3;
    for (i = 0; i < n; sGT4++) {
        pGT4 = (POLY_GT4 *)prim;
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
        i0 = *(u32 *)&sGT4->unk10[0];
        i2 = *(u32 *)&sGT4->unk10[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        t = tex[sGT4->unk1D];
        hi = t & 0xFFFF0000;
        t <<= 16;
        cp = (u32 *)col[i0];
        *(u32 *)&pGT4->r0 = *cp;
        cp = (u32 *)col[i1];
        *(u32 *)&pGT4->r1 = *cp;
        cp = (u32 *)col[i2];
        *(u32 *)&pGT4->r2 = *cp;
        cp = (u32 *)col[i3];
        *(u32 *)&pGT4->r3 = *cp;
        setlen(pGT4, 12);
        setcode(pGT4, 0x3C);
        i0 = *(u32 *)&sGT4->unk8[0];
        i2 = *(u32 *)&sGT4->unk8[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        *(u32 *)&pGT4->u0 = *(u16 *)uv[i0] | hi;
        *(u32 *)&pGT4->u1 = *(u16 *)uv[i1] | t;
        *(u16 *)&pGT4->u2 = *(u16 *)uv[i2];
        *(u16 *)&pGT4->u3 = *(u16 *)uv[i3];
        if (sGT4->unk1C & 1) {
            pGT4->tpage |= sGT4->unk1C & 0xE0;
            setSemiTrans(pGT4, 1);
        }
        otz >>= 6;
        addPrim(&ot[otz], pGT4);
        prim += sizeof(POLY_GT4);
    }

    n = g->unk8;
    sGT3 = (WmSrcGT3 *)sGT4;
    for (i = 0; i < n; sGT3++) {
        pGT3 = (POLY_GT3 *)prim;
        i0 = *(u32 *)&sGT3->unk0[0];
        i2 = *(u32 *)&sGT3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sGT3->unkF & 2)) {
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
        t = tex[sGT3->unk16];
        hi = t & 0xFFFF0000;
        t <<= 16;
        gte_stotz(&otz);
        i0 = *(u32 *)&sGT3->unk10[0];
        i2 = *(u32 *)&sGT3->unk10[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        cp = (u32 *)col[i0];
        *(u32 *)&pGT3->r0 = *cp;
        cp = (u32 *)col[i1];
        *(u32 *)&pGT3->r1 = *cp;
        cp = (u32 *)col[i2];
        *(u32 *)&pGT3->r2 = *cp;
        setlen(pGT3, 9);
        setcode(pGT3, 0x34);
        i1 = *(u32 *)&sGT3->unk6[1];
        i0 = sGT3->unk6[0];
        i2 = i1 >> 16;
        i1 &= 0xFFFF;
        *(u32 *)&pGT3->u0 = *(u16 *)uv[i0] | hi;
        *(u32 *)&pGT3->u1 = *(u16 *)uv[i1] | t;
        *(u16 *)&pGT3->u2 = *(u16 *)uv[i2];
        if (sGT3->unkF & 1) {
            pGT3->tpage |= sGT3->unkF & 0xE0;
            setSemiTrans(pGT3, 1);
        }
        otz >>= 2;
        addPrim(&ot[otz], pGT3);
        prim += sizeof(POLY_GT3);
    }

    n = g->unkA;
    sG4 = (WmSrcG4 *)sGT3;
    for (i = 0; i < n; sG4++) {
        pG4 = (POLY_G4 *)prim;
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
        i0 = *(u32 *)&sG4->unk8[0];
        i2 = *(u32 *)&sG4->unk8[2];
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
        setlen(pG4, 8);
        setcode(pG4, 0x38);
        otz >>= 6;
        addPrim(&ot[otz], pG4);
        prim += sizeof(POLY_G4);
    }

    n = g->unkC;
    sG3 = (WmSrcG3 *)sG4;
    for (i = 0; i < n; sG3++) {
        pG3 = (POLY_G3 *)prim;
        i0 = *(u32 *)&sG3->unk0[0];
        i2 = *(u32 *)&sG3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sG3->unk13 & 2)) {
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
        i0 = *(u32 *)&sG3->unk8[0];
        i2 = *(u32 *)&sG3->unk8[2];
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
        setlen(pG3, 6);
        setcode(pG3, 0x30);
        otz >>= 2;
        addPrim(&ot[otz], pG3);
        prim += sizeof(POLY_G3);
    }

    n = g->unkE;
    sF4 = (WmSrcF4 *)sG3;
    for (i = 0; i < n; sF4++) {
        pF4 = (POLY_F4 *)prim;
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
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3];
        *(u32 *)&pF4->r0 = *(u32 *)sF4->unk8;
        setlen(pF4, 5);
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        setcode(pF4, 0x28);
        *(s32 *)&pF4->x3 = g_vtxScreenXY[i3];
        otz >>= 6;
        addPrim(&ot[otz], pF4);
        prim += sizeof(POLY_F4);
    }

    n = g->unk10;
    sF3 = (WmSrcF3 *)sF4;
    for (i = 0; i < n; sF3++) {
        pF3 = (POLY_F3 *)prim;
        i0 = *(u32 *)&sF3->unk0[0];
        i2 = *(u32 *)&sF3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = g_vtxScreenXY[i0];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[i2];
        i++;
        if (!(sF3->unkB & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
        *(u32 *)&pF3->r0 = *(u32 *)sF3->unk8;
        gte_avsz3_b();
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        *(s32 *)&pF3->x2 = xy2;
        gte_stotz(&otz);
        setlen(pF3, 4);
        setcode(pF3, 0x20);
        otz >>= 2;
        addPrim(&ot[otz], pF3);
        prim += sizeof(POLY_F3);
    }
    setcode((P_TAG *)prim, 0xFF);
    return prim + sizeof(P_TAG);
}

void func_800C7730(BtlUnit9C *arg0, s16 *arg1, s16 *arg2) {
    BtlUnit9C8 *h;
    BtlMesh *m;
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

    arg0->unk54 = 0x1000;
    arg0->unk56 = 0x1000;
    arg0->unk58 = 0x1000;
    arg0->unk0 |= 2;
    h = arg0->unk8;
    arg0->unk18 = (BtlUnit9C18 *)arg1;
    arg0->unk1C = NULL;
    arg0->unk60 = NULL;
    arg0->unk5C = NULL;
    arg0->unk44 = 0;
    arg0->unk20 = NULL;
    arg0->unk10 = 0;
    arg0->unk50 = NULL;
    arg0->unk74 = 0;
    arg0->unk76 = 0;
    arg0->unk78 = 0;
    arg0->unk14 = 0;
    arg0->unk0 = 0;
    if (!(h->unk0 & 1)) {
        h->unk0 |= 1;
        m = h->unk10 = (BtlMesh *)((u8 *)h + (s32)h->unk10);
        h->unkC = (s32)h + h->unkC;
        for (i = 0; i < h->unk3; i++, m++) {
            m->unk14 = (u16 *)((u8 *)h + (s32)m->unk14);
            m->unk18 = (SVECTOR *)((u8 *)h + (s32)m->unk18);
            m->unk1C = (u32 *)((u8 *)h + (s32)m->unk1C);
            m->unk20 = (void *)((u8 *)h + (s32)m->unk20);
            m->unk24 = (void *)((u8 *)h + (s32)m->unk24);
        }
        if (arg2 != NULL) {
            for (i = 0; i < h->unk3; i++) {
                mark = (u8 *)g_vtxScreenZ;
                for (k = 0; k < 0x1908; k++) {
                    mark[k] = 0;
                }
                m = &h->unk10[i];
                src = (u8 *)m->unk1C;
                for (k = 0; k < m->unk2; k++) {
                    ft4 = (WmSrcFT4 *)src;
                    du = arg2[ft4->unk13 * 2];
                    dv = arg2[ft4->unk13 * 2 + 1];
                    for (j = 0; j < 4; j++) {
                        if (mark[ft4->unk8[j]] == 0) {
                            m->unk20[ft4->unk8[j]][0] += du;
                            m->unk20[ft4->unk8[j]][1] += dv;
                            mark[ft4->unk8[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcFT4);
                }
                for (k = 0; k < m->unk4; k++) {
                    ft3 = (WmSrcFT3 *)src;
                    du = arg2[ft3->unk6 * 2];
                    dv = arg2[ft3->unk6 * 2 + 1];
                    for (j = 0; j < 3; j++) {
                        if (mark[ft3->unkC[j]] == 0) {
                            m->unk20[ft3->unkC[j]][0] += du;
                            m->unk20[ft3->unkC[j]][1] += dv;
                            mark[ft3->unkC[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcFT3);
                }
                for (k = 0; k < m->unk6; k++) {
                    gt4 = (WmSrcGT4 *)src;
                    du = arg2[gt4->unk1D * 2];
                    dv = arg2[gt4->unk1D * 2 + 1];
                    for (j = 0; j < 4; j++) {
                        if (mark[gt4->unk8[j]] == 0) {
                            m->unk20[gt4->unk8[j]][0] += du;
                            m->unk20[gt4->unk8[j]][1] += dv;
                            mark[gt4->unk8[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcGT4);
                }
                for (k = 0; k < m->unk8; k++) {
                    gt3 = (WmSrcGT3 *)src;
                    du = arg2[gt3->unk16 * 2];
                    dv = arg2[gt3->unk16 * 2 + 1];
                    for (j = 0; j < 3; j++) {
                        if (mark[gt3->unk6[j]] == 0) {
                            m->unk20[gt3->unk6[j]][0] += du;
                            m->unk20[gt3->unk6[j]][1] += dv;
                            mark[gt3->unk6[j]] = 1;
                        }
                    }
                    src += sizeof(WmSrcGT3);
                }
            }
        }
    }
}

void func_800C7B84(BtlUnit9C *arg0) {
    union {
        MATRIX m;
        s32 w[8];
    } s;
    s16 x;

    if (arg0->unk0 & 2) {
        x = arg0->unk54;
        s.w[0] = 0x1000;
        s.w[1] = 0;
        s.w[2] = 0x1000;
        s.w[3] = 0;
        s.w[4] = 0x1000;
        s.m.m[0][0] = x;
        gte_SetRotMatrix(&arg0->unk24);
        gte_ldclmv(&s.m);
        gte_rtir();
        s.m.m[1][1] = arg0->unk56;
        gte_stclmv(&arg0->unk24);
        gte_ldclmv((s16 *)&s.m + 1);
        gte_rtir();
        s.m.m[2][2] = arg0->unk58;
        gte_stclmv((s16 *)&arg0->unk24 + 1);
        gte_ldclmv((s16 *)&s.m + 2);
        gte_rtir();
        gte_stclmv((s16 *)&arg0->unk24 + 2);
    }
}

void func_800C7CC8(BtlUnit9C *arg0) {
    SVECTOR v;
    s32 z;

    arg0->unk0 &= ~0x10;
    if (arg0->unk1C != NULL) {
        arg0->unk0 |= arg0->unk1C->unk0 & 0x10;
        return;
    }
    v.vx = arg0->unk24.t[0];
    v.vy = arg0->unk24.t[1];
    v.vz = arg0->unk24.t[2];
    gte_SetRotMatrix(&g_gameState->unk1C->unk18);
    gte_SetTransMatrix(&g_gameState->unk1C->unk18);
    gte_ldv0(&v);
    gte_rtps();
    gte_stsz(&z);
    if (z == 0) {
        arg0->unk0 |= 0x10;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/btl_mesh", btl_calcBoneMatrices);

void btl_setModelRgb(BtlUnit9C *arg0, s32 r, s32 g, s32 b) {
    u32 i;
    BtlPrim *p;
    u8 code;

    if (!(arg0->unk0 & 0x10)) {
    i = 0;
    p = (BtlPrim *)arg0->unk10;
    if (arg0->unk8->unk3 != 0) {
    do {
        if (!(arg0->unk14 & (1 << i))) {
        do {
            code = p->tag.code;
            if (code == 0xFF || code == 0xE1) {
                p = (BtlPrim *)&p->w[2];
                continue;
            }
            switch (code & 0x3C) {
            case 0x2C:
                p->w[1] = btl_addRgbClamped(p->w[1], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[10];
                break;
            case 0x24:
                p->w[1] = btl_addRgbClamped(p->w[1], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[8];
                break;
            case 0x3C:
                p->w[1] = btl_addRgbClamped(p->w[1], (s16)r, (s16)g, (s16)b);
                p->w[4] = btl_addRgbClamped(p->w[4], (s16)r, (s16)g, (s16)b);
                p->w[7] = btl_addRgbClamped(p->w[7], (s16)r, (s16)g, (s16)b);
                p->w[10] = btl_addRgbClamped(p->w[10], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[13];
                break;
            case 0x34:
                p->w[1] = btl_addRgbClamped(p->w[1], (s16)r, (s16)g, (s16)b);
                p->w[4] = btl_addRgbClamped(p->w[4], (s16)r, (s16)g, (s16)b);
                p->w[7] = btl_addRgbClamped(p->w[7], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[10];
                break;
            case 0x38:
                p->w[1] = btl_addRgbClamped(p->w[1], (s16)r, (s16)g, (s16)b);
                p->w[3] = btl_addRgbClamped(p->w[3], (s16)r, (s16)g, (s16)b);
                p->w[5] = btl_addRgbClamped(p->w[5], (s16)r, (s16)g, (s16)b);
                p->w[7] = btl_addRgbClamped(p->w[7], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[9];
                break;
            case 0x30:
                p->w[1] = btl_addRgbClamped(p->w[1], (s16)r, (s16)g, (s16)b);
                p->w[3] = btl_addRgbClamped(p->w[3], (s16)r, (s16)g, (s16)b);
                p->w[5] = btl_addRgbClamped(p->w[5], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[7];
                break;
            case 0x28:
                p->w[1] = btl_addRgbClamped(p->w[1], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[6];
                break;
            case 0x20:
                p->w[1] = btl_addRgbClamped(p->w[1], (s16)r, (s16)g, (s16)b);
                p->tag.code = code;
                p = (BtlPrim *)&p->w[5];
                break;
            }
        } while (code != 0xFF);
        }
    } while (++i < arg0->unk8->unk3);
    }
    }
}

void func_800C8A3C(BtlUnit9C *arg0, s32 r, s32 g, s32 b) {
    u32 i;
    BtlPrim *p;
    u32 col;
    u8 code;

    i = 0;
    if (!(arg0->unk0 & 0x10)) {
    col = (b << 16) | ((g << 16) >> 8) | (s16)r;
    p = (BtlPrim *)arg0->unk10;
    if (arg0->unk8->unk3 != 0) {
    do {
        if (!(arg0->unk14 & (1 << i))) {
        do {
            code = p->tag.code;
            if (code == 0xFF || code == 0xE1) {
                p = (BtlPrim *)&p->w[2];
                continue;
            }
            switch (code & 0x3C) {
            case 0x2C:
                p->w[1] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[10];
                break;
            case 0x24:
                p->w[1] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[8];
                break;
            case 0x3C:
                p->w[1] = col;
                p->w[4] = col;
                p->w[7] = col;
                p->w[10] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[13];
                break;
            case 0x34:
                p->w[1] = col;
                p->w[4] = col;
                p->w[7] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[10];
                break;
            case 0x38:
                p->w[1] = col;
                p->w[3] = col;
                p->w[5] = col;
                p->w[7] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[9];
                break;
            case 0x30:
                p->w[1] = col;
                p->w[3] = col;
                p->w[5] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[7];
                break;
            case 0x28:
                p->w[1] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[6];
                break;
            case 0x20:
                p->w[1] = col;
                p->tag.code = code;
                p = (BtlPrim *)&p->w[5];
                break;
            }
        } while (code != 0xFF);
        }
    } while (++i < arg0->unk8->unk3);
    }
    }
}
