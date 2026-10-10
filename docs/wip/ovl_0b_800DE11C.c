/* WIP func_800DE11C (0x1040, battle model renderer; twin of func_800C67BC, which matched in pass 12): 79 aligned diff rows (was 430), 8 with registers masked, 1040 of 1040 insns. Pass 12: the 3-vertex loops keep their third index in b (`b = s->unk0[2]; xy2 = D_800711A8[b]; ... D_80070520[b & 0xFFFF]`), i0 is inline (`a & 0xFFFF`), which fixes xy0/xy1/xy2 (t5/t4/a3), a (a3), b (t1), i1 (a0) and the constant (t0). The first four loops (FT4..GT3) then match except the FT3/GT3 tail (`sra a0,t3,4` vs our in-place otz >>= 4). Two invented variables are still in here: zg (otz for the G4/G3/F4/F3 loops; one shared otz has priority 6*124/455 = 1.64 and takes t2 before the prim givs, while the original keeps otz in t3 = i3's register in every loop) and i2 as the second colour/uv word of GT4/G4 (the original loads it into a0, not b's t1). Left: the second half (G4..F3): zg (60 refs / 152 insns = 1.97) is allocated before the prim giv and i3*4, so it gets a2 instead of t3. Tails: 4-vertex `addPrim(&ot[otz >> 6], p)`, 3-vertex `otz /= 3; ... otz >>= 4; addPrim(&ot[otz], p)` scored best here, but the original's non-in-place sra suggests `ot[otz >> 4]` once the allocation is right. Searched and rejected: per-loop/inline scoping of a/b/i0..i3/xy/otz/t/cp (build/dsp3-dsp11), types of i0..i3/xy/otz/a/b, per-loop otz partitions (best 66 rows, needs 3 otz variables). */
#include "ovl_0b.h"

u8 *func_800DE11C(BtlUnit9C *arg0, u8 arg1, u8 *prim) {
    s32 opz;
    s32 otz;
    s32 zg;
    BtlMesh *g;
    u8 (*uv)[2];
    u8 (*col)[4];
    u32 *ot;
    u32 *tex;
    u32 n;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 a, b;
    u32 *cp;

    u32 i1, i2, i3;
    u32 t;
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

    if (arg0->unk0 & 0x10) {
        return prim;
    }
    g = &arg0->unk8->unk10[arg1];
    src = (u8 *)g->unk1C;
    uv = g->unk20;
    col = g->unk24;
    if (arg0->unk10 == 0) {
        arg0->unk10 = (s32)prim;
    }
    tex = (u32 *)arg0->unk18;
    ot = D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8];

    n = g->unk2;
    for (i = 0; i < n; src += sizeof(WmSrcFT4)) {
        sFT4 = (WmSrcFT4 *)src;
        pFT4 = (POLY_FT4 *)prim;
        a = *(u32 *)&sFT4->unk0[0];
        b = *(u32 *)&sFT4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = D_800711A8[(a & 0xFFFF)];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[b & 0xFFFF];
        otz = D_80070520[(a & 0xFFFF)] + D_80070520[i1] + D_80070520[b & 0xFFFF] + D_80070520[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
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
        a = *(u32 *)&sFT4->unk8[0];
        b = *(u32 *)&sFT4->unk8[2];
        t = tex[sFT4->unk13];
        hi = t & 0xFFFF0000;
        t <<= 16;
        *(u32 *)&pFT4->u0 = *(u16 *)uv[a & 0xFFFF] | hi;
        *(u32 *)&pFT4->u1 = *(u16 *)uv[a >> 16] | t;
        *(u16 *)&pFT4->u2 = *(u16 *)uv[b & 0xFFFF];
        *(u16 *)&pFT4->u3 = *(u16 *)uv[b >> 16];
        if (sFT4->unk15 & 1) {
            pFT4->tpage |= sFT4->unk15 & 0xE0;
            setSemiTrans(pFT4, 1);
        }
        otz &= 0xFFFFF;
        addPrim(&ot[otz >> 6], pFT4);
        prim += sizeof(POLY_FT4);
    }

    n = g->unk4;
    for (i = 0; i < n; src += sizeof(WmSrcFT3)) {
        sFT3 = (WmSrcFT3 *)src;
        pFT3 = (POLY_FT3 *)prim;
        a = *(u32 *)&sFT3->unk0[0];
        b = sFT3->unk0[2];
        i1 = a >> 16;
        xy0 = D_800711A8[(a & 0xFFFF)];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[b];
        otz = D_80070520[(a & 0xFFFF)] + D_80070520[i1] + D_80070520[b & 0xFFFF];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sFT3->unk12 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        *(u32 *)&pFT3->r0 = *(u32 *)sFT3->unk8;
        setlen(pFT3, 7);
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        *(s32 *)&pFT3->x2 = xy2;
        setcode(pFT3, 0x24);
        t = tex[sFT3->unk6];
        a = *(u32 *)&sFT3->unkC[0];
        hi = t & 0xFFFF0000;
        t <<= 16;
        *(u32 *)&pFT3->u0 = *(u16 *)uv[a & 0xFFFF] | hi;
        *(u32 *)&pFT3->u1 = *(u16 *)uv[a >> 16] | t;
        *(u16 *)&pFT3->u2 = *(u16 *)uv[sFT3->unkC[2]];
        otz /= 3;
        if (sFT3->unk12 & 1) {
            pFT3->tpage |= sFT3->unk12 & 0xE0;
            setSemiTrans(pFT3, 1);
        }
        otz >>= 4;
        addPrim(&ot[otz], pFT3);
        prim += sizeof(POLY_FT3);
    }

    n = g->unk6;
    for (i = 0; i < n; src += sizeof(WmSrcGT4)) {
        sGT4 = (WmSrcGT4 *)src;
        pGT4 = (POLY_GT4 *)prim;
        a = *(u32 *)&sGT4->unk0[0];
        b = *(u32 *)&sGT4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = D_800711A8[(a & 0xFFFF)];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[b & 0xFFFF];
        otz = D_80070520[(a & 0xFFFF)] + D_80070520[i1] + D_80070520[b & 0xFFFF] + D_80070520[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
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
        a = *(u32 *)&sGT4->unk10[0];
        i2 = *(u32 *)&sGT4->unk10[2];
        t = tex[sGT4->unk1D];
        cp = (u32 *)((u8 *)col + ((a & 0xFFFF) << 2));
        *(u32 *)&pGT4->r0 = *cp;
        cp = (u32 *)((u8 *)col + ((a >> 16) << 2));
        *(u32 *)&pGT4->r1 = *cp;
        cp = (u32 *)((u8 *)col + ((i2 & 0xFFFF) << 2));
        *(u32 *)&pGT4->r2 = *cp;
        cp = (u32 *)((u8 *)col + ((i2 >> 16) << 2));
        *(u32 *)&pGT4->r3 = *cp;
        setlen(pGT4, 12);
        setcode(pGT4, 0x3C);
        a = *(u32 *)&sGT4->unk8[0];
        i2 = *(u32 *)&sGT4->unk8[2];
        hi = t & 0xFFFF0000;
        t <<= 16;
        *(u32 *)&pGT4->u0 = *(u16 *)uv[a & 0xFFFF] | hi;
        *(u32 *)&pGT4->u1 = *(u16 *)uv[a >> 16] | t;
        *(u16 *)&pGT4->u2 = *(u16 *)uv[i2 & 0xFFFF];
        *(u16 *)&pGT4->u3 = *(u16 *)uv[i2 >> 16];
        if (sGT4->unk1C & 1) {
            pGT4->tpage |= sGT4->unk1C & 0xE0;
            setSemiTrans(pGT4, 1);
        }
        otz &= 0xFFFFF;
        addPrim(&ot[otz >> 6], pGT4);
        prim += sizeof(POLY_GT4);
    }

    n = g->unk8;
    for (i = 0; i < n; src += sizeof(WmSrcGT3)) {
        sGT3 = (WmSrcGT3 *)src;
        pGT3 = (POLY_GT3 *)prim;
        a = *(u32 *)&sGT3->unk0[0];
        b = sGT3->unk0[2];
        i1 = a >> 16;
        xy0 = D_800711A8[(a & 0xFFFF)];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[b];
        otz = D_80070520[(a & 0xFFFF)] + D_80070520[i1] + D_80070520[b & 0xFFFF];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sGT3->unkF & 2)) {
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
        a = *(u32 *)&sGT3->unk10[0];
        t = tex[sGT3->unk16];
        cp = (u32 *)((u8 *)col + ((a & 0xFFFF) << 2));
        *(u32 *)&pGT3->r0 = *cp;
        cp = (u32 *)((u8 *)col + ((a >> 16) << 2));
        *(u32 *)&pGT3->r1 = *cp;
        cp = (u32 *)((u8 *)col + ((sGT3->unk10[2]) << 2));
        *(u32 *)&pGT3->r2 = *cp;
        setlen(pGT3, 9);
        setcode(pGT3, 0x34);
        hi = t & 0xFFFF0000;
        t <<= 16;
        *(u32 *)&pGT3->u0 = *(u16 *)uv[sGT3->unk6[0]] | hi;
        a = *(u32 *)&sGT3->unk6[1];
        *(u32 *)&pGT3->u1 = *(u16 *)uv[a & 0xFFFF] | t;
        *(u16 *)&pGT3->u2 = *(u16 *)uv[a >> 16];
        otz /= 3;
        if (sGT3->unkF & 1) {
            pGT3->tpage |= sGT3->unkF & 0xE0;
            setSemiTrans(pGT3, 1);
        }
        otz >>= 4;
        addPrim(&ot[otz], pGT3);
        prim += sizeof(POLY_GT3);
    }

    n = g->unkA;
    for (i = 0; i < n; src += sizeof(WmSrcG4)) {
        sG4 = (WmSrcG4 *)src;
        pG4 = (POLY_G4 *)prim;
        a = *(u32 *)&sG4->unk0[0];
        b = *(u32 *)&sG4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = D_800711A8[(a & 0xFFFF)];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[b & 0xFFFF];
        zg = D_80070520[(a & 0xFFFF)] + D_80070520[i1] + D_80070520[b & 0xFFFF] + D_80070520[i3];
        i++;
        if (zg > 0x3FFFFFFF) {
            continue;
        }
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
        zg &= 0xFFFFF;
        a = *(u32 *)&sG4->unk8[0];
        i2 = *(u32 *)&sG4->unk8[2];
        cp = (u32 *)((u8 *)col + ((a & 0xFFFF) << 2));
        *(u32 *)&pG4->r0 = *cp;
        cp = (u32 *)((u8 *)col + ((a >> 16) << 2));
        *(u32 *)&pG4->r1 = *cp;
        cp = (u32 *)((u8 *)col + ((i2 & 0xFFFF) << 2));
        *(u32 *)&pG4->r2 = *cp;
        cp = (u32 *)((u8 *)col + ((i2 >> 16) << 2));
        *(u32 *)&pG4->r3 = *cp;
        setlen(pG4, 8);
        setcode(pG4, 0x38);
        addPrim(&ot[zg >> 6], pG4);
        prim += sizeof(POLY_G4);
    }

    n = g->unkC;
    for (i = 0; i < n; src += sizeof(WmSrcG3)) {
        sG3 = (WmSrcG3 *)src;
        pG3 = (POLY_G3 *)prim;
        a = *(u32 *)&sG3->unk0[0];
        b = sG3->unk0[2];
        i1 = a >> 16;
        xy0 = D_800711A8[(a & 0xFFFF)];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[b];
        zg = D_80070520[(a & 0xFFFF)] + D_80070520[i1] + D_80070520[b & 0xFFFF];
        i++;
        if (zg > 0x2FFFFFFF) {
            continue;
        }
        if (!(sG3->unk13 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        zg &= 0xFFFFF;
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        *(s32 *)&pG3->x2 = xy2;
        a = *(u32 *)&sG3->unk8[0];
        cp = (u32 *)((u8 *)col + ((a & 0xFFFF) << 2));
        *(u32 *)&pG3->r0 = *cp;
        cp = (u32 *)((u8 *)col + ((a >> 16) << 2));
        *(u32 *)&pG3->r1 = *cp;
        cp = (u32 *)((u8 *)col + ((sG3->unk8[2]) << 2));
        *(u32 *)&pG3->r2 = *cp;
        setlen(pG3, 6);
        setcode(pG3, 0x30);
        zg /= 3;
        zg >>= 4;
        addPrim(&ot[zg], pG3);
        prim += sizeof(POLY_G3);
    }

    n = g->unkE;
    for (i = 0; i < n; src += sizeof(WmSrcF4)) {
        sF4 = (WmSrcF4 *)src;
        pF4 = (POLY_F4 *)prim;
        a = *(u32 *)&sF4->unk0[0];
        b = *(u32 *)&sF4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = D_800711A8[(a & 0xFFFF)];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[b & 0xFFFF];
        zg = D_80070520[(a & 0xFFFF)] + D_80070520[i1] + D_80070520[b & 0xFFFF] + D_80070520[i3];
        i++;
        if (zg > 0x3FFFFFFF) {
            continue;
        }
        if (!(sF4->unkD & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(u32 *)&pF4->r0 = *(u32 *)sF4->unk8;
        setlen(pF4, 5);
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        setcode(pF4, 0x28);
        zg &= 0xFFFFF;
        *(s32 *)&pF4->x3 = D_800711A8[i3];
        addPrim(&ot[zg >> 6], pF4);
        prim += sizeof(POLY_F4);
    }

    n = g->unk10;
    for (i = 0; i < n; src += sizeof(WmSrcF3)) {
        sF3 = (WmSrcF3 *)src;
        pF3 = (POLY_F3 *)prim;
        a = *(u32 *)&sF3->unk0[0];
        b = sF3->unk0[2];
        i1 = a >> 16;
        xy0 = D_800711A8[(a & 0xFFFF)];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[b];
        zg = D_80070520[(a & 0xFFFF)] + D_80070520[i1] + D_80070520[b & 0xFFFF];
        i++;
        if (zg > 0x2FFFFFFF) {
            continue;
        }
        if (!(sF3->unkB & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        zg &= 0xFFFFF;
        *(u32 *)&pF3->r0 = *(u32 *)sF3->unk8;
        setlen(pF3, 4);
        setcode(pF3, 0x20);
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        *(s32 *)&pF3->x2 = xy2;
        zg /= 3;
        zg >>= 4;
        addPrim(&ot[zg], pF3);
        prim += sizeof(POLY_F3);
    }
    setcode((P_TAG *)prim, 0xFF);
    return prim + sizeof(P_TAG);
}
