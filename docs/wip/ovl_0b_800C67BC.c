#include "ovl_0b.h"

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
    u32 a, b;
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
    ot = D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8];
    ot -= g->unk13;

    n = g->unk2;
    for (i = 0; i < n; sFT4++) {
        pFT4 = (POLY_FT4 *)prim;
        a = *(u32 *)&sFT4->unk0[0];
        b = *(u32 *)&sFT4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
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
        *(s32 *)&pFT4->x3 = D_800711A8[i3];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
        a = *(u32 *)&sFT4->unk8[0];
        b = *(u32 *)&sFT4->unk8[2];
        t = tex[sFT4->unk13];
        hi = t & 0xFFFF0000;
        t <<= 16;
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
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
        a = *(u32 *)&sFT3->unk0[0];
        i2 = sFT3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sFT3->unk12 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        setlen(pFT3, 7);
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        *(s32 *)&pFT3->x2 = xy2;
        *(u32 *)&pFT3->r0 = *(u32 *)sFT3->unk8;
        setcode(pFT3, 0x24);
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
        t = tex[sFT3->unk6];
        hi = t & 0xFFFF0000;
        t <<= 16;
        gte_avsz3_b();
        a = *(u32 *)&sFT3->unkC[0];
        i2 = sFT3->unkC[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
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
        a = *(u32 *)&sGT4->unk0[0];
        b = *(u32 *)&sGT4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
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
        *(s32 *)&pGT4->x3 = D_800711A8[i3];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
        a = *(u32 *)&sGT4->unk10[0];
        b = *(u32 *)&sGT4->unk10[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        t = tex[sGT4->unk1D];
        hi = t & 0xFFFF0000;
        t <<= 16;
        *(u32 *)&pGT4->r0 = *(u32 *)col[i0];
        *(u32 *)&pGT4->r1 = *(u32 *)col[i1];
        *(u32 *)&pGT4->r2 = *(u32 *)col[i2];
        setlen(pGT4, 12);
        setcode(pGT4, 0x3C);
        *(u32 *)&pGT4->r3 = *(u32 *)col[i3];
        a = *(u32 *)&sGT4->unk8[0];
        b = *(u32 *)&sGT4->unk8[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
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
        a = *(u32 *)&sGT3->unk0[0];
        i2 = sGT3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
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
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
        *(s32 *)&pGT3->x1 = xy1;
        *(s32 *)&pGT3->x2 = xy2;
        gte_avsz3_b();
        t = tex[sGT3->unk16];
        hi = t & 0xFFFF0000;
        t <<= 16;
        gte_stotz(&otz);
        a = *(u32 *)&sGT3->unk10[0];
        i2 = sGT3->unk10[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        *(u32 *)&pGT3->r0 = *(u32 *)col[i0];
        *(u32 *)&pGT3->r1 = *(u32 *)col[i1];
        setlen(pGT3, 9);
        setcode(pGT3, 0x34);
        *(u32 *)&pGT3->r2 = *(u32 *)col[i2];
        a = *(u32 *)&sGT3->unk6[1];
        i0 = sGT3->unk6[0];
        i2 = a >> 16;
        i1 = a & 0xFFFF;
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
        a = *(u32 *)&sG4->unk0[0];
        b = *(u32 *)&sG4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
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
        *(s32 *)&pG4->x3 = D_800711A8[i3];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
        a = *(u32 *)&sG4->unk8[0];
        b = *(u32 *)&sG4->unk8[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        *(u32 *)&pG4->r0 = *(u32 *)col[i0];
        *(u32 *)&pG4->r1 = *(u32 *)col[i1];
        *(u32 *)&pG4->r2 = *(u32 *)col[i2];
        setlen(pG4, 8);
        setcode(pG4, 0x38);
        *(u32 *)&pG4->r3 = *(u32 *)col[i3];
        otz >>= 6;
        addPrim(&ot[otz], pG4);
        prim += sizeof(POLY_G4);
    }

    n = g->unkC;
    sG3 = (WmSrcG3 *)sG4;
    for (i = 0; i < n; sG3++) {
        pG3 = (POLY_G3 *)prim;
        a = *(u32 *)&sG3->unk0[0];
        i2 = sG3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sG3->unk13 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        *(s32 *)&pG3->x2 = xy2;
        gte_avsz3_b();
        a = *(u32 *)&sG3->unk8[0];
        i2 = sG3->unk8[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        *(u32 *)&pG3->r0 = *(u32 *)col[i0];
        *(u32 *)&pG3->r1 = *(u32 *)col[i1];
        *(u32 *)&pG3->r2 = *(u32 *)col[i2];
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
        a = *(u32 *)&sF4->unk0[0];
        b = *(u32 *)&sF4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sF4->unkD & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
        setlen(pF4, 5);
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        *(u32 *)&pF4->r0 = *(u32 *)sF4->unk8;
        setcode(pF4, 0x28);
        *(s32 *)&pF4->x3 = D_800711A8[i3];
        otz >>= 6;
        addPrim(&ot[otz], pF4);
        prim += sizeof(POLY_F4);
    }

    n = g->unk10;
    sF3 = (WmSrcF3 *)sF4;
    for (i = 0; i < n; sF3++) {
        pF3 = (POLY_F3 *)prim;
        a = *(u32 *)&sF3->unk0[0];
        i2 = sF3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sF3->unkB & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
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
