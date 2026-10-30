/* ovl_0b battle-effect sprite emitters (0x800D3ED4-0x800D4768), hand-written asm in the original:
 * btlfx_emitAnimQuads, btlfx_emitSprites, btlfx_emitFixedSprites, btlfx_emitDots (btlfx_drawAnimQuads / btlfx_drawSprites),
 * btlfx_emitParticles (btlfx_drawParticles) and btlfx_emitScreenSprites (screen-space sprites).
 *
 * Register convention: standard C arguments. btlfx_emitFixedSprites and btlfx_emitDots keep the $s registers they use
 * in $t registers instead of on the stack; btlfx_emitSprites, btlfx_emitFixedSprites and btlfx_emitDots have no count check
 * up front (count 0 runs 2^32 times). GTE (all but btlfx_emitScreenSprites): VZ0 (VZ0-VZ2 for the quads) = 0 and
 * FC = 0 on entry, then V0-V2, RTPS/RTPT outputs, and IR0/RGBC/RGB2 of the DPCS fade, which runs when the
 * context's fade (unk18) is >= 0, toward FC = black by (0x1000 - fade) / 4096.
 *
 * Points are 3-byte records (s8 x, s8 y in units of 4, then a byte: low nibble = uv entry, high nibble =
 * color entry) projected through the current GTE matrices. The textured quads leave the whole batch when an
 * RTPT's MAC0 is 0 (no NCLIP is issued: MAC0 is RTPT's depth-cue product), and take their OT entry from
 * ((SZ3 >> 2) - zbase) >> 2 when it is below 0x1000 (otherwise the packet is built but not linked); the
 * sprites and dots all go into the single OT entry passed in.
 *
 * Checked with tools/equiv.py against the original (src_equiv/ovl_0b/check.py).
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
/* A signed byte shifted left, as the asm does it (no undefined signed shifts). */
#define S8SHL(b, n) ((u32)(s32)(s8)(b) << (n))

/* addPrim with the length: tag = len << 24 | next, entry = packet. */
static void fxLink(u32 *entry, u8 *p, u32 len) {
    u32 e = *entry;

    W(p, 0) = (e & 0xFFFFFF) | (len << 24);
    *entry = (e & 0xFF000000) | ((u32)p & 0xFFFFFF);
}

/* Depth-sorted link: OT entry ((sz >> 2) - zbase) >> 2 when below 0x1000. */
static void fxLinkZ(s32 *ot, u32 sz, s32 zbase, u8 *p, u32 len) {
    u32 z = (((s32)sz >> 2) - zbase) >> 2;

    if (z < 0x1000) {
        fxLink((u32 *)ot + z, p, len);
    }
}

/* Color table entry: rgb (u16 + u8) | code. */
#define FX_COLOR(tab, k, code) (H(tab, (k) * 4) | (*((u8 *)(tab) + (k) * 4 + 2) << 16) | (code))

/* DPCS fade of a color (IR0 = 0x1000 - fade). */
static u32 fxFade(u32 c, s32 fade) {
    GTE_MTC2(8, 0x1000 - fade);
    GTE_MTC2(6, c);
    GTE_CMD(0x4A780010);
    return GTE_MFC2(22);
}

/* POLY_FT4 per point: a w x h quad (ctx unk8/unkC = half sizes) around it, uvs from the uv table (u, v byte
 * pair) spanning half the size. */
void btlfx_emitAnimQuads(u8 *pts, Unk800D3ED4Ctx *ctx, u8 *out, s32 *ot) {
    s32 hw = ctx->halfW, hh = ctx->halfH;
    u16 *uvTab = ctx->uvTab;
    u16 *colTab = ctx->colTab;
    u32 tpage = ctx->tpage << 16, clut = ctx->clut << 16;
    s32 fade = ctx->fade;
    u32 n = ctx->count;
    s32 zbase = ctx->otZ;
    u32 code = ctx->code << 24;
    s32 x, y;
    u32 x0, x1, y0, y1, b, uv, u, v, u1, v1, c, sz;

    GTE_MTC2(1, 0);
    GTE_MTC2(3, 0);
    GTE_MTC2(5, 0);
    GTE_CTC2(21, 0);
    GTE_CTC2(22, 0);
    GTE_CTC2(23, 0);
    if (n == 0) {
        return;
    }
    do {
        x = S8SHL(pts[0], 2);
        y = S8SHL(pts[1], 2);
        x0 = (x - hw) & 0xFFFF;
        x1 = (x + hw) & 0xFFFF;
        y0 = (u32)(y - hh) << 16;
        y1 = (u32)(y + hh) << 16;
        GTE_MTC2(0, x0 | y0);
        GTE_MTC2(2, x1 | y0);
        GTE_MTC2(4, x0 | y1);
        GTE_CMD(0x4A280030);
        b = pts[2];
        pts += 3;
        uv = uvTab[b & 0xF];
        u = uv & 0xFF;
        v = uv >> 8;
        u1 = u + ((u32)hw >> 1);
        v1 = (v + ((u32)hh >> 1)) << 8;
        v <<= 8;
        W(out, 0xC) = u | v | clut;
        W(out, 0x14) = u1 | v | tpage;
        W(out, 0x1C) = u | v1;
        W(out, 0x8) = GTE_MFC2(12);
        W(out, 0x10) = GTE_MFC2(13);
        W(out, 0x18) = GTE_MFC2(14);
        W(out, 0x24) = u1 | v1;
        if (GTE_MFC2(24) == 0) {
            return;
        }
        sz = GTE_MFC2(19);
        GTE_MTC2(0, x1 | y1);
        GTE_CMD(0x4A180001);
        c = FX_COLOR(colTab, b >> 4, code);
        W(out, 0x20) = GTE_MFC2(14);
        if (fade >= 0) {
            c = fxFade(c, fade);
        }
        n--;
        W(out, 0x4) = c;
        fxLinkZ(ot, sz, zbase, out, 9);
        out += 0x28;
    } while (n != 0);
}

/* SPRT per point (w x h from ctx unk8/unkC, centered), all linked at *ot. */
void btlfx_emitSprites(u8 *pts, Unk800D3ED4Ctx *ctx, u8 *out, s32 *ot) {
    s32 w = ctx->halfW, h = ctx->halfH;
    u16 *uvTab = ctx->uvTab;
    u16 *colTab = ctx->colTab;
    u32 clut = ctx->clut << 16;
    s32 fade = ctx->fade;
    u32 code = ctx->code << 24;
    u32 n = ctx->count;
    u32 b, c, xy;

    GTE_MTC2(1, 0);
    GTE_CTC2(21, 0);
    GTE_CTC2(22, 0);
    GTE_CTC2(23, 0);
    do {
        GTE_MTC2(0, (S8SHL(pts[0], 2) & 0xFFFF) | S8SHL(pts[1], 18));
        H(out, 0x10) = w;
        H(out, 0x12) = h;
        GTE_CMD(0x4A180001);
        b = pts[2];
        pts += 3;
        W(out, 0xC) = uvTab[b & 0xF] | clut;
        xy = GTE_MFC2(14);
        c = FX_COLOR(colTab, b >> 4, code);
        H(out, 0x8) = (s16)xy - ((u32)w >> 1);
        H(out, 0xA) = ((s32)xy >> 16) - ((u32)h >> 1);
        if (fade >= 0) {
            c = fxFade(c, fade);
        }
        n--;
        W(out, 0x4) = c;
        fxLink((u32 *)ot, out, 4);
        out += 0x14;
    } while (n != 0);
}

/* Fixed-size SPRT (8x8 / 16x16 by the code) per point, centered by ctx unk8 / 2, linked at *ot. */
void btlfx_emitFixedSprites(u8 *pts, Unk800D3ED4Ctx *ctx, u8 *out, s32 *ot) {
    u16 *colTab = ctx->colTab;
    u16 *uvTab = ctx->uvTab;
    u32 half = (u32)ctx->halfW >> 1;
    u32 clut = ctx->clut << 16;
    s32 fade = ctx->fade;
    u32 code = ctx->code << 24;
    u32 n = ctx->count;
    u32 b, c, xy;

    GTE_MTC2(1, 0);
    GTE_CTC2(21, 0);
    GTE_CTC2(22, 0);
    GTE_CTC2(23, 0);
    do {
        GTE_MTC2(0, (S8SHL(pts[0], 2) & 0xFFFF) | S8SHL(pts[1], 18));
        b = pts[2];
        pts += 3;
        GTE_CMD(0x4A180001);
        W(out, 0xC) = uvTab[b & 0xF] | clut;
        xy = GTE_MFC2(14);
        c = FX_COLOR(colTab, b >> 4, code);
        W(out, 0x8) = (((s16)xy - half) & 0xFFFF) | ((((s32)xy >> 16) - half) << 16); /* u32 arithmetic */
        if (fade >= 0) {
            c = fxFade(c, fade);
        }
        n--;
        W(out, 0x4) = c;
        fxLink((u32 *)ot, out, 3);
        out += 0x10;
    } while (n != 0);
}

/* TILE_1 dot per point, linked at *ot. */
void btlfx_emitDots(u8 *pts, Unk800D3ED4Ctx *ctx, u8 *out, s32 *ot) {
    u16 *colTab = ctx->colTab;
    s32 fade = ctx->fade;
    u32 code = ctx->code << 24;
    u32 n = ctx->count;
    u32 b, c;

    GTE_MTC2(1, 0);
    GTE_CTC2(21, 0);
    GTE_CTC2(22, 0);
    GTE_CTC2(23, 0);
    do {
        GTE_MTC2(0, (S8SHL(pts[0], 2) & 0xFFFF) | S8SHL(pts[1], 18));
        b = pts[2];
        pts += 3;
        GTE_CMD(0x4A180001);
        c = FX_COLOR(colTab, b >> 4, code);
        W(out, 0x8) = GTE_MFC2(14);
        if (fade >= 0) {
            c = fxFade(c, fade);
        }
        n--;
        W(out, 0x4) = c;
        fxLink((u32 *)ot, out, 2);
        out += 0xC;
    } while (n != 0);
}

/* POLY_FT4 per 10-byte particle record {s8 x, s8 y (units of 16), u8 w, u8 h (units of 4), u16 uv0, u16 uv1
 * (u1, v1 corners), u16 tpage}: positions offset by ctx unk4/unk8 (<< 2), one color (unk0) for all, uv0's
 * word ored with unk10 << 16, uv1's with (tpage | unkC) << 16. MAC0 == 0 ends the batch. */
void btlfx_emitParticles(u8 *rec, Unk800D1680Ctx *ctx, u8 *out, s32 *ot) {
    u32 hi10 = ctx->tpage << 16;
    u32 n = ctx->frame;
    s32 zbase = ctx->otZ;
    u32 color = ctx->color;
    u32 tpOr = ctx->abr;
    s32 ox = ctx->x << 2, oy = ctx->y << 2;
    u32 x0, x1, y0, y1, uv0, uv1, tp, xy3, sz;

    GTE_MTC2(1, 0);
    GTE_MTC2(3, 0);
    GTE_MTC2(5, 0);
    if (n == 0) {
        return;
    }
    do {
        x0 = (S8SHL(rec[0], 4) + ox) & 0xFFFF;
        x1 = (x0 + (rec[2] << 2)) & 0xFFFF;
        y0 = S8SHL(rec[1], 4) + oy;
        y1 = (y0 + (rec[3] << 2)) << 16;
        y0 <<= 16;
        GTE_MTC2(0, x0 | y0);
        GTE_MTC2(2, x1 | y0);
        GTE_MTC2(4, x0 | y1);
        xy3 = x1 | y1;
        W(out, 0x4) = color;
        GTE_CMD(0x4A280030);
        tp = (H(rec, 8) | tpOr) << 16;
        uv0 = H(rec, 4);
        uv1 = H(rec, 6);
        W(out, 0xC) = (uv0 & 0xFF) | (uv0 & 0xFF00) | hi10;
        W(out, 0x14) = (uv1 & 0xFF) | (uv0 & 0xFF00) | tp;
        W(out, 0x8) = GTE_MFC2(12);
        W(out, 0x10) = GTE_MFC2(13);
        W(out, 0x18) = GTE_MFC2(14);
        W(out, 0x1C) = (uv0 & 0xFF) | (uv1 & 0xFF00);
        if (GTE_MFC2(24) == 0) {
            return;
        }
        sz = GTE_MFC2(19);
        GTE_MTC2(0, xy3);
        W(out, 0x24) = (uv1 & 0xFF) | (uv1 & 0xFF00);
        GTE_CMD(0x4A180001);
        fxLinkZ(ot, sz, zbase, out, 9);
        rec += 10;
        W(out, 0x20) = GTE_MFC2(14);
        n--;
        out += 0x28;
    } while (n != 0);
}

/* Screen-space SPRT per 10-byte record {s8 x, s8 y (units of 4, offset by ctx unk4/unk8), u8 w, u8 h,
 * u16 uv, -, u16 tpage}, one color (unk0), uv word ored with unk10 << 16, all linked at *ot. Each run of
 * records with the same non-zero tpage gets a DR_MODE (E1000200 | tpage | unkC) from the buffer at unk18,
 * linked after the run (so it comes first in the chain). Returns the advanced DR_MODE buffer. */
u8 *btlfx_emitScreenSprites(u8 *rec, Unk800D1BC4Ctx *ctx, u8 *out, s32 *ot) {
    u32 hi10 = ctx->tpage << 16;
    u32 n = ctx->frame;
    u8 *mode = ctx->prim;
    u32 color = ctx->color;
    u32 tpOr = ctx->abr;
    s32 ox = ctx->x, oy = ctx->y;
    u32 cur = 0, tp;

    if (n != 0) {
        do {
            W(out, 0x4) = color;
            H(out, 0x8) = S8SHL(rec[0], 2) + ox;
            H(out, 0xA) = S8SHL(rec[1], 2) + oy;
            H(out, 0x10) = rec[2];
            H(out, 0x12) = rec[3];
            W(out, 0xC) = H(rec, 4) | hi10;
            tp = H(rec, 8);
            rec += 10;
            if (tp != cur) {
                if (cur != 0) {
                    W(mode, 4) = 0xE1000200 | (cur | tpOr);
                    fxLink((u32 *)ot, mode, 1);
                    mode += 8;
                }
                cur = tp;
            }
            fxLink((u32 *)ot, out, 4);
            n--;
            out += 0x14;
        } while (n != 0);
    }
    if (cur != 0) {
        W(mode, 4) = 0xE1000200 | (cur | tpOr);
        fxLink((u32 *)ot, mode, 1);
        mode += 8;
    }
    return mode;
}
