/*
 * ovl_0a func_800BB714 (0xC28 bytes): field model renderer, depth-sorted variant of
 * func_800B71E0 (8 primitive loops FT4/FT3/GT4/GT3/G4/G3/F4/F3, Z sum + 0x3FFFFFFF /
 * 0x2FFFFFFF far-clip test before the nclip). ovl_0c func_800CA1D8 is a byte-identical twin.
 * WIP: 772 of 778 instructions. Control flow and per-loop code shape match; left:
 *  - 3-vertex loops: the original recomputes `i2 * 4` for the Z lookup (sll twice) where we
 *    CSE it (one instruction per loop, x4);
 *  - register allocation: the original uses all of s0-s7 (count n in s0, mfhi in s7) where we
 *    put n in t9 and leave s7 free, so most t/s registers are shifted by one.
 *  `otz &= 0xFFFFF; otz >>= 4;` as two statements is needed to keep and+sra (one statement
 *  folds into srl+andi).
 */
#include "ovl_0a.h"

void func_800BB714(Unk800C95C8 *arg0, u8 arg1) {
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

    g = &arg0->unk8->unk10[arg1];
    ot = D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8];
    if (D_8006794C->unk1C->unk8 != 0) {
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
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
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
        *(s32 *)&pFT4->x3 = D_800711A8[i3];
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
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2];
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
        otz &= 0xFFFFF;
        otz = otz / 3 >> 2;
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        *(s32 *)&pFT3->x2 = xy2;
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
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
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
        *(s32 *)&pGT4->x3 = D_800711A8[i3];
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
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2];
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
        otz &= 0xFFFFF;
        otz = otz / 3 >> 2;
        *(s32 *)&pGT3->x0 = xy0;
        *(s32 *)&pGT3->x1 = xy1;
        *(s32 *)&pGT3->x2 = xy2;
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
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
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
        *(s32 *)&pG4->x3 = D_800711A8[i3];
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
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2];
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
        otz &= 0xFFFFF;
        otz = otz / 3 >> 2;
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        *(s32 *)&pG3->x2 = xy2;
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
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
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
        *(s32 *)&pF4->x3 = D_800711A8[i3];
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
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2];
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
        otz &= 0xFFFFF;
        otz = otz / 3 >> 2;
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        *(s32 *)&pF3->x2 = xy2;
        addPrim(&ot[otz], pF3);
    }
}
