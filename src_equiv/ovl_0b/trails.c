/* ovl_0b trail emitters (0x800D4D6C-0x800D57D0, 0x800D5C30-0x800D616C), hand-written asm in the original:
 * btlfx_emitTrailRibbonFlat, btlfx_emitTrailRibbon3D, btlfx_emitTrailLines, btlfx_emitTrailRibbonScreen.
 *
 * Register convention: standard C arguments (segment count, context, packet buffer, OT). GTE: they set FC
 * from the context's far color (unk10: r << 4, g << 4, b << 4) and leave V0-V2, IR, MAC and the RTPT/DPCS
 * outputs as the last segment did. btlfx_emitTrailRibbon3D and btlfx_emitTrailRibbonScreen overwrite parts of
 * RT/TR while working and restore them from copies they keep in the context (RT 0-4 at unk30/unk34/unk38 for
 * the 3D ribbon; all of RT/TR at unk28-unk44 for the screen ribbon).
 *
 * The trail is a ring buffer (context unk4) filled by btlfx_pushTrailPoint: {u8 last index, u8 head, -, -}
 * then 8-byte entries {s16 x, y, z; u16 width << 1 (in the upper half of the z word)}; a segment joins entry
 * i and the next one (wrapping after `last`). Each segment fades the color (unk0C, code in the top byte)
 * toward FC by the running fade (unk8, + unk20 per segment, 0x1000 on the last segment) with DPCS; the
 * previous segment's faded color is the segment's start color. The v coordinate runs from unk14 by
 * (fade * unk16) >> 12; u comes from unk24 / unk26. OT entry ((SZ >> 2) - unk0) >> 2 when below 0x1000.
 *
 * Checked with tools/equiv.py against the original (src_equiv/ovl_0b/check.py).
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define SH(p, o) (*(s16 *)((u8 *)(p) + (o)))

static void trailLink(u32 *entry, u8 *p, u32 len) {
    u32 e = *entry;

    W(p, 0) = (e & 0xFFFFFF) | (len << 24);
    *entry = (e & 0xFF000000) | ((u32)p & 0xFFFFFF);
}

/* OT entry index for a depth: ((sz >> 2) - zbase) >> 2 (>= 0x1000: not linked). */
static u32 trailZ(u32 sz, s32 zbase) {
    return (((s32)sz >> 2) - zbase) >> 2;
}

static void trailFarColor(u32 far) {
    GTE_CTC2(21, (far & 0xFF) << 4);
    GTE_CTC2(22, (far >> 4) & 0xFF0);
    GTE_CTC2(23, (far >> 12) & 0xFF0);
}

#define TRAIL_NEXT(i, size) ((i) + 1 == (size) ? 0 : (i) + 1)

/* GT4 ribbon along a flat (y-fixed) direction: each point is offset by +-(direction * width >> 12), the
 * direction being the packed s16 pair (x, z) in ctx unk28. The first segment projects both edges of its
 * start point; later ones reuse the previous end's projected edges. */
void btlfx_emitTrailRibbonFlat(s32 count, Unk800D4D6CCtx *ctx, u8 *out, s32 *ot) {
    s32 zbase = ctx->otZ;
    u32 dir = ctx->unk28;
    u8 *ring = (u8 *)ctx->ring;
    s32 fade = ctx->fade;
    u32 color = ctx->color.w;
    u32 vBase = (u16)ctx->v0;
    u32 prevColor = color, v = vBase;
    u32 size, idx, n, w0, yhi, ox, oy, sxy0 = 0, sxy1 = 0, edgeP = 0, edgeM = 0, t;
    s32 x, z, zP = 0, zM = 0, k;
    u8 *ent, *e;

    trailFarColor(ctx->farColor);
    size = ring[0] + 1;
    idx = ring[1];
    ent = ring + 4;
    k = 0;
    do {
        e = ent + idx * 8;
        GTE_MTC2(9, (s16)dir);
        GTE_MTC2(10, (s32)dir >> 16);
        GTE_MTC2(8, H(e, 6));
        GTE_CMD(0x4B98003D);
        if (k == 0) {
            w0 = W(e, 0);
            z = SH(e, 4);
            ox = GTE_MFC2(9);
            oy = GTE_MFC2(10);
            yhi = w0 & 0xFFFF0000;
            x = (s16)w0;
            GTE_MTC2(0, ((x + ox) & 0xFFFF) | yhi);
            GTE_MTC2(1, z + oy);
            GTE_MTC2(2, ((x - ox) & 0xFFFF) | yhi);
            GTE_MTC2(3, z - oy);
            n = TRAIL_NEXT(idx, size);
            GTE_CMD(0x4A280030);
            e = ent + n * 8;
            w0 = W(e, 0);
            z = SH(e, 4);
            yhi = w0 & 0xFFFF0000;
            x = (s16)w0;
            edgeM = ((x - ox) & 0xFFFF) | yhi;
            zM = z - oy;
            edgeP = ((x + ox) & 0xFFFF) | yhi;
            zP = z + oy;
            W(out, 0x8) = GTE_MFC2(12);
            W(out, 0x14) = GTE_MFC2(13);
        } else {
            W(out, 0x8) = sxy0;
            W(out, 0x14) = sxy1;
            n = TRAIL_NEXT(idx, size);
            e = ent + n * 8;
            w0 = W(e, 0);
            z = SH(e, 4);
            ox = GTE_MFC2(9);
            oy = GTE_MFC2(10);
            yhi = w0 & 0xFFFF0000;
            x = (s16)w0;
            edgeM = ((x - ox) & 0xFFFF) | yhi;
            zM = z - oy;
            edgeP = ((x + ox) & 0xFFFF) | yhi;
            zP = z + oy;
        }
        if (!(k + 1 < count)) {
            fade = 0x1000;
        }
        GTE_MTC2(8, fade);
        GTE_MTC2(6, color);
        GTE_CMD(0x4A780010);
        W(out, 0x4) = prevColor;
        W(out, 0x10) = prevColor;
        W(out, 0xC) = v | H(ctx, 0x24) | ctx->clut;
        W(out, 0x18) = H(ctx, 0x26) | v | ctx->tpage;
        v = vBase + (((u32)fade * H(ctx, 0x16)) >> 12);
        prevColor = GTE_MFC2(22);
        GTE_MTC2(0, edgeP);
        GTE_MTC2(1, zP);
        GTE_MTC2(2, edgeM);
        GTE_MTC2(3, zM);
        e = ent + idx * 8;
        GTE_MTC2(4, W(e, 0));
        GTE_MTC2(5, W(e, 4));
        GTE_CMD(0x4A280030);
        W(out, 0x1C) = prevColor;
        W(out, 0x28) = prevColor;
        H(out, 0x24) = H(ctx, 0x24) | v;
        H(out, 0x30) = H(ctx, 0x26) | v;
        sxy0 = GTE_MFC2(12);
        sxy1 = GTE_MFC2(13);
        W(out, 0x20) = sxy0;
        W(out, 0x2C) = sxy1;
        idx = TRAIL_NEXT(idx, size);
        t = GTE_MFC2(19);
        k++;
        if (GTE_MFC2(24) != 0) {
            t = trailZ(t, zbase);
            if (t < 0x1000) {
                trailLink((u32 *)ot + t, out, 0xC);
            }
        }
        fade += ctx->fadeStep;
        out += 0x34;
    } while (k < count);
}

/* GT4 ribbon in 3D: the width direction of segment a->b is the normalized cross product of (b - a) * 4
 * (loaded into the RT diagonal for OP) and (reference point ctx unk28 - a), normalized with LZCS and the
 * reciprocal-square-root table g_btlfxRsqrtTable, scaled by the signed width with GPF. a's coordinates go to
 * unk44-unk4C, b's to unk50-unk58, the projected end edges to unk3C/unk40 for the next segment. */
void btlfx_emitTrailRibbon3D(s32 count, Unk800D50A0Ctx *ctx, u8 *out, s32 *ot) {
    s32 zbase = ctx->otZ;
    u8 *ref = (u8 *)ctx + 0x28;
    u8 *ring = (u8 *)ctx->ring;
    s32 fade = ctx->fade;
    u32 color = ctx->color.w;
    u32 vBase = (u16)ctx->v0;
    u32 prevColor = color, v = vBase;
    u32 size, idx, n, t, sum, lz, mx, my, mz, edgeP = 0, edgeM = 0, sxy0, sxy1;
    s32 ax, ay, az, bx, by, bz, shift, norm, ox, oy, oz, zP = 0, zM = 0, k, w;
    u8 *ent, *ea, *eb;

    trailFarColor(ctx->farColor);
    size = ring[0] + 1;
    idx = ring[1];
    ent = ring + 4;
    W(ctx, 0x30) = GTE_CFC2(0);
    W(ctx, 0x34) = GTE_CFC2(2);
    W(ctx, 0x38) = GTE_CFC2(4);
    k = 0;
    do {
        ea = ent + idx * 8;
        eb = ent + TRAIL_NEXT(idx, size) * 8;
        bx = SH(eb, 0);
        ax = SH(ea, 0);
        W(ctx, 0x50) = bx;
        GTE_CTC2(0, (bx - ax) << 2);
        by = SH(eb, 2);
        ay = SH(ea, 2);
        W(ctx, 0x54) = by;
        GTE_CTC2(2, (by - ay) << 2);
        bz = SH(eb, 4);
        az = SH(ea, 4);
        W(ctx, 0x58) = bz;
        GTE_CTC2(4, (bz - az) << 2);
        W(ctx, 0x44) = ax;
        GTE_MTC2(9, SH(ref, 0) - ax);
        W(ctx, 0x48) = ay;
        GTE_MTC2(10, SH(ref, 2) - ay);
        W(ctx, 0x4C) = az;
        GTE_MTC2(11, SH(ref, 4) - az);
        GTE_CMD(0x4B78000C);
        mx = GTE_MFC2(25);
        my = GTE_MFC2(26);
        mz = GTE_MFC2(27);
        GTE_CMD(0x4AA00428);
        sum = GTE_MFC2(25) + GTE_MFC2(26) + GTE_MFC2(27);
        GTE_MTC2(30, sum);
        lz = GTE_MFC2(31);
        GTE_MTC2(9, mx);
        GTE_MTC2(10, my);
        GTE_MTC2(11, mz);
        lz &= ~1;
        shift = (s32)(31 - lz) >> 1;
        if ((s32)(lz - 24) >= 0) {
            t = sum << ((lz - 24) & 31);
        } else {
            t = (s32)sum >> ((24 - lz) & 31);
        }
        norm = g_btlfxRsqrtTable[t - 0x40];
        GTE_MTC2(8, norm);
        w = SH(ea, 6);
        GTE_CMD(0x4B90003D);
        mx = (s32)GTE_MFC2(25) >> (shift & 31);
        my = (s32)GTE_MFC2(26) >> (shift & 31);
        mz = (s32)GTE_MFC2(27) >> (shift & 31);
        GTE_MTC2(8, w);
        GTE_MTC2(9, mx);
        GTE_MTC2(10, my);
        GTE_MTC2(11, mz);
        GTE_CMD(0x4B98003D);
        if (k == 0) {
            ax = W(ctx, 0x44);
            ay = W(ctx, 0x48);
            az = W(ctx, 0x4C);
            ox = GTE_MFC2(9);
            oy = GTE_MFC2(10);
            oz = GTE_MFC2(11);
            GTE_CTC2(0, W(ctx, 0x30));
            GTE_CTC2(2, W(ctx, 0x34));
            GTE_CTC2(4, W(ctx, 0x38));
            GTE_MTC2(0, ((ax + ox) & 0xFFFF) | ((u32)(ay + oy) << 16));
            GTE_MTC2(1, az + oz);
            GTE_MTC2(2, ((ax - ox) & 0xFFFF) | ((u32)(ay - oy) << 16));
            GTE_MTC2(3, az - oz);
            bx = W(ctx, 0x50);
            GTE_CMD(0x4A280030);
            by = W(ctx, 0x54);
            bz = W(ctx, 0x58);
            W(out, 0x8) = GTE_MFC2(12);
            W(out, 0x14) = GTE_MFC2(13);
        } else {
            W(out, 0x8) = W(ctx, 0x3C);
            W(out, 0x14) = W(ctx, 0x40);
            bx = W(ctx, 0x50);
            by = W(ctx, 0x54);
            bz = W(ctx, 0x58);
            ox = GTE_MFC2(9);
            oy = GTE_MFC2(10);
            oz = GTE_MFC2(11);
        }
        edgeM = ((bx - ox) & 0xFFFF) | ((u32)(by - oy) << 16);
        zM = bz - oz;
        edgeP = ((bx + ox) & 0xFFFF) | ((u32)(by + oy) << 16);
        zP = bz + oz;
        if (!(k + 1 < count)) {
            fade = 0x1000;
        }
        GTE_MTC2(8, fade);
        GTE_MTC2(6, color);
        GTE_CMD(0x4A780010);
        W(out, 0x4) = prevColor;
        W(out, 0x10) = prevColor;
        W(out, 0xC) = v | H(ctx, 0x24) | ctx->clut;
        W(out, 0x18) = H(ctx, 0x26) | v | ctx->tpage;
        v = vBase + (((u32)fade * H(ctx, 0x16)) >> 12);
        prevColor = GTE_MFC2(22);
        GTE_CTC2(0, W(ctx, 0x30));
        GTE_CTC2(2, W(ctx, 0x34));
        GTE_CTC2(4, W(ctx, 0x38));
        GTE_MTC2(0, edgeP);
        GTE_MTC2(1, zP);
        GTE_MTC2(2, edgeM);
        GTE_MTC2(3, zM);
        ea = ent + idx * 8;
        GTE_MTC2(4, W(ea, 0));
        GTE_MTC2(5, W(ea, 4));
        GTE_CMD(0x4A280030);
        W(out, 0x1C) = prevColor;
        W(out, 0x28) = prevColor;
        H(out, 0x24) = H(ctx, 0x24) | v;
        H(out, 0x30) = H(ctx, 0x26) | v;
        sxy0 = GTE_MFC2(12);
        sxy1 = GTE_MFC2(13);
        W(out, 0x20) = sxy0;
        W(out, 0x2C) = sxy1;
        W(ctx, 0x3C) = sxy0;
        W(ctx, 0x40) = sxy1;
        idx = TRAIL_NEXT(idx, size);
        t = GTE_MFC2(19);
        k++;
        if (GTE_MFC2(24) != 0) {
            t = trailZ(t, zbase);
            if (t < 0x1000) {
                trailLink((u32 *)ot + t, out, 0xC);
            }
        }
        fade += ctx->fadeStep;
        out += 0x34;
    } while (k < count);
}

/* LINE_G2 per segment, each after a DR_MODE (E1000200 | ctx unk28) linked behind it at the same depth (the
 * previous point's SZ, SZ2 after the new point's RTPS). The start point is the previous projected point
 * moved one pixel toward the new one on each axis where they differ. */
void btlfx_emitTrailLines(s32 count, Unk800D4D6CCtx *ctx, u8 *out, s32 *ot) {
    s32 zbase = ctx->otZ;
    u32 mode = ctx->unk28;
    u8 *ring = (u8 *)ctx->ring;
    s32 fade = ctx->fade;
    u32 color = ctx->color.w;
    u32 prevColor = color;
    u32 size, idx, xy, pxy, sz, z;
    s32 x, y, px, py, dx, dy, k;
    u8 *ent, *e, *m;

    trailFarColor(ctx->farColor);
    size = ring[0] + 1;
    idx = ring[1];
    ent = ring + 4;
    e = ent + idx * 8;
    GTE_MTC2(0, W(e, 0));
    GTE_MTC2(1, W(e, 4));
    GTE_CMD(0x4A180001);
    (void)GTE_MFC2(13);
    idx = TRAIL_NEXT(idx, size);
    k = 0;
    do {
        e = ent + idx * 8;
        GTE_MTC2(0, W(e, 0));
        GTE_MTC2(1, W(e, 4));
        GTE_CMD(0x4A180001);
        m = out + 0x14;
        W(m, 4) = 0xE1000200 | mode;
        if (!(k + 1 < count)) {
            fade = 0x1000;
        }
        W(out, 0x4) = prevColor;
        xy = GTE_MFC2(14);
        pxy = GTE_MFC2(13);
        W(out, 0x8) = xy;
        x = (s16)xy;
        y = (s32)xy >> 16;
        px = (s16)pxy;
        py = (s32)pxy >> 16;
        dx = x - px;
        dy = y - py;
        if (dx != 0) {
            dx = dx < 0 ? -1 : 1;
        }
        if (dy != 0) {
            dy = dy < 0 ? -1 : 1;
        }
        sz = GTE_MFC2(18);
        H(out, 0x10) = px + dx;
        H(out, 0x12) = py + dy;
        GTE_MTC2(8, fade);
        GTE_MTC2(6, color);
        k++;
        GTE_CMD(0x4A780010);
        z = trailZ(sz, zbase);
        if (z < 0x1000) {
            trailLink((u32 *)ot + z, out, 4);
        }
        if (z < 0x1000) {
            trailLink((u32 *)ot + z, m, 1);
        }
        fade += ctx->fadeStep;
        prevColor = GTE_MFC2(22);
        W(out, 0xC) = prevColor;
        idx = TRAIL_NEXT(idx, size);
        out += 0x1C;
    } while (k < count);
}

/* GT4 ribbon in screen space: each segment projects its points (RTPT of the next, this and the following
 * point; at the last segment the following point is the next one again), turns the screen-space direction
 * (<< 7) into a unit normal (OP against z, SQR, LZCS and g_btlfxRsqrtTable), scales it by the width (GPF) and
 * projects the two edge offsets with an identity rotation and the point's view-space position as the
 * translation. The first segment's start edge is this point's projection moved by the previous-to-this
 * screen step; later ones reuse the previous end. The quad is linked when its first RTPT's SZ2 is non-zero
 * and NCLIP of the final SXY registers is non-zero. Scratch values in ctx unk48-unk68. */
void btlfx_emitTrailRibbonScreen(s32 count, Unk800D5C30Ctx *ctx, u8 *out, s32 *ot) {
    s32 zbase = ctx->otZ;
    u8 *ring = (u8 *)ctx->ring;
    s32 fade = ctx->fade;
    u32 color = ctx->color.w;
    u32 vBase = (u16)ctx->v0;
    u32 prevColor = color, v = vBase;
    u32 size, idx, n1, n2, sum, lz, t, mx, my, ex, ey, edgeP, edgeM, sxy0, sxy1, sz;
    s32 shift, norm, w, dx, dy, k;
    u8 *ent, *e0, *e1, *e2;

    trailFarColor(ctx->farColor);
    size = ring[0] + 1;
    idx = ring[1];
    ent = ring + 4;
    W(ctx, 0x28) = GTE_CFC2(0);
    W(ctx, 0x2C) = GTE_CFC2(1);
    W(ctx, 0x30) = GTE_CFC2(2);
    W(ctx, 0x34) = GTE_CFC2(3);
    W(ctx, 0x38) = GTE_CFC2(4);
    W(ctx, 0x3C) = GTE_CFC2(5);
    W(ctx, 0x40) = GTE_CFC2(6);
    W(ctx, 0x44) = GTE_CFC2(7);
    k = 0;
    do {
        e0 = ent + idx * 8;
        n1 = TRAIL_NEXT(idx, size);
        e1 = ent + n1 * 8;
        n2 = TRAIL_NEXT(n1, size);
        e2 = ent + n2 * 8;
        GTE_MTC2(2, W(e0, 0));
        GTE_MTC2(3, W(e0, 4));
        if (k - count + 1 >= 0) {
            GTE_MTC2(0, W(e1, 0));
            GTE_MTC2(1, W(e1, 4));
        } else {
            GTE_MTC2(0, W(e2, 0));
            GTE_MTC2(1, W(e2, 4));
        }
        GTE_MTC2(4, W(e1, 0));
        GTE_MTC2(5, W(e1, 4));
        GTE_CMD(0x4A280030);
        W(ctx, 0x50) = GTE_MFC2(14);
        W(ctx, 0x54) = GTE_MFC2(13);
        W(ctx, 0x58) = GTE_MFC2(12);
        W(ctx, 0x60) = GTE_MFC2(25);
        W(ctx, 0x64) = GTE_MFC2(26);
        W(ctx, 0x68) = GTE_MFC2(27);
        dx = SH(ctx, 0x54);
        dy = SH(ctx, 0x56);
        sz = GTE_MFC2(18);
        GTE_CTC2(0, 0);
        GTE_CTC2(2, 0);
        GTE_CTC2(4, 0x1000);
        W(ctx, 0x5C) = sz;
        GTE_MTC2(9, (SH(ctx, 0x58) - dx) << 7);
        GTE_MTC2(10, (SH(ctx, 0x5A) - dy) << 7);
        GTE_MTC2(11, 0);
        GTE_CMD(0x4B78000C);
        SH(ctx, 0x54) = dx - SH(ctx, 0x50);
        SH(ctx, 0x56) = dy - SH(ctx, 0x52);
        mx = GTE_MFC2(25);
        my = GTE_MFC2(26);
        GTE_CMD(0x4AA00428);
        sum = GTE_MFC2(25) + GTE_MFC2(26);
        GTE_MTC2(30, sum);
        lz = GTE_MFC2(31);
        GTE_MTC2(9, mx);
        GTE_MTC2(10, my);
        lz &= ~1;
        shift = (s32)(31 - lz) >> 1;
        if ((s32)(lz - 24) >= 0) {
            t = sum << ((lz - 24) & 31);
        } else {
            t = (s32)sum >> ((24 - lz) & 31);
        }
        norm = g_btlfxRsqrtTable[t - 0x40];
        GTE_MTC2(8, norm);
        w = SH(e0, 6);
        GTE_CMD(0x4B90003D);
        mx = (s32)GTE_MFC2(25) >> (shift & 31);
        my = (s32)GTE_MFC2(26) >> (shift & 31);
        GTE_MTC2(8, w);
        GTE_MTC2(9, mx);
        GTE_MTC2(10, my);
        GTE_CMD(0x4B98003D);
        if (!(k + 1 < count)) {
            fade = 0x1000;
        }
        ex = GTE_MFC2(9);
        ey = GTE_MFC2(10);
        edgeP = (ex + 0xFFFF) | (ey << 16);
        edgeM = (-ex + 0xFFFF) | (-ey << 16);
        W(out, 0xC) = v | H(ctx, 0x24) | ctx->clut;
        GTE_CTC2(5, W(ctx, 0x60));
        GTE_CTC2(6, W(ctx, 0x64));
        GTE_CTC2(7, W(ctx, 0x68));
        GTE_CTC2(0, 0x1000);
        GTE_CTC2(2, 0x1000);
        GTE_CTC2(4, 0x1000);
        GTE_CTC2(1, 0);
        GTE_CTC2(3, 0);
        GTE_MTC2(0, edgeP);
        GTE_MTC2(1, 0);
        GTE_MTC2(2, edgeM);
        GTE_MTC2(3, 0);
        GTE_CMD(0x4A280030);
        W(out, 0x18) = H(ctx, 0x26) | v | ctx->tpage;
        v = vBase + (((u32)fade * H(ctx, 0x16)) >> 12);
        H(out, 0x24) = H(ctx, 0x24) | v;
        H(out, 0x30) = H(ctx, 0x26) | v;
        if (k == 0) {
            sxy0 = GTE_MFC2(12);
            dx = SH(ctx, 0x54);
            dy = SH(ctx, 0x56);
            H(out, 0x8) = (s16)sxy0 + dx;
            H(out, 0xA) = ((s32)sxy0 >> 16) + dy;
            sxy1 = GTE_MFC2(13);
            H(out, 0x14) = (s16)sxy1 + dx;
            H(out, 0x16) = ((s32)sxy1 >> 16) + dy;
            GTE_MTC2(13, sxy1);
        } else {
            t = W(ctx, 0x48);
            W(out, 0x8) = t;
            W(out, 0x14) = W(ctx, 0x4C);
            sxy0 = GTE_MFC2(12);
            sxy1 = GTE_MFC2(13);
            GTE_MTC2(14, t);
        }
        GTE_CTC2(0, W(ctx, 0x28));
        GTE_CTC2(1, W(ctx, 0x2C));
        GTE_CTC2(2, W(ctx, 0x30));
        GTE_CTC2(3, W(ctx, 0x34));
        GTE_CTC2(4, W(ctx, 0x38));
        GTE_CTC2(5, W(ctx, 0x3C));
        GTE_CTC2(6, W(ctx, 0x40));
        GTE_CTC2(7, W(ctx, 0x44));
        GTE_MTC2(8, fade);
        GTE_MTC2(6, color);
        GTE_CMD(0x4A780010);
        W(out, 0x4) = prevColor;
        W(out, 0x10) = prevColor;
        prevColor = GTE_MFC2(22);
        W(out, 0x1C) = prevColor;
        W(out, 0x28) = prevColor;
        W(out, 0x20) = sxy0;
        W(out, 0x2C) = sxy1;
        W(ctx, 0x48) = sxy0;
        W(ctx, 0x4C) = sxy1;
        sz = W(ctx, 0x5C);
        idx = TRAIL_NEXT(idx, size);
        k++;
        if (sz != 0) {
            GTE_CMD(0x4B400006);
            if (GTE_MFC2(24) != 0) {
                t = trailZ(sz, zbase);
                if (t < 0x1000) {
                    trailLink((u32 *)ot + t, out, 0xC);
                }
            }
        }
        fade += ctx->fadeStep;
        out += 0x34;
    } while (k < count);
}
