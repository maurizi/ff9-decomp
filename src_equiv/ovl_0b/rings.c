/* ovl_0b ring-grid emitters of btlfx_drawRings (0x800D4768-0x800D4D6C), hand-written asm in the original:
 * btlfx_emitRingQuads (textured POLY_FT4 grid), btlfx_emitRingGouraud (Gouraud POLY_G4 grid with a DR_MODE per quad) and
 * their color-fade helper btlfx_fadeColor.
 *
 * Register convention: standard C arguments for the two emitters. btlfx_fadeColor is an internal helper with
 * its own convention: color in $s1 (returned in $s1), fade in $t1, clobbers $v0; its C form below takes and
 * returns them normally.
 *
 * Both walk `rows` (ctx unk24, counted down in the context) rings of ctx unk1C + 1 points: each point is a
 * u16 direction pair (ctx unk10, the same list for every row) scaled by a radius with GPF (sf=1, IR0 =
 * radius): the row's inner radius ctx unkC[r] * 4 and outer unkC[r + 1] * 4, each first scaled by the
 * factor in ctx unk8[1] when unk8[0] is non-zero (unk8 itself never advances). Odd points use the scaled
 * radii, even points the plain ones (kept at ctx unk2C/unk30 and unk34/unk38). Point pairs i-1, i give a
 * quad (prev outer, outer, prev inner, inner) through the current matrices (RTPT + RTPS, z = 0 stays
 * whatever VZ held). A quad whose RTPT gives MAC0 == 0 ends the whole call (no NCLIP). OT entry
 * ((SZ3 >> 2) - unk18) >> 2 when below 0x1000.
 *
 * Checked with tools/equiv.py against the original (src_equiv/ovl_0b/check.py).
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))

static void ringLink(u32 *entry, u8 *p, u32 len) {
    u32 e = *entry;

    W(p, 0) = (e & 0xFFFFFF) | (len << 24);
    *entry = (e & 0xFF000000) | ((u32)p & 0xFFFFFF);
}

static u32 ringZ(u32 sz, s32 zbase) {
    return (((s32)sz >> 2) - zbase) >> 2;
}

/* GPF sf=1 on IR1 only (IR0 already set): the radius scaled by the factor. */
static u32 ringScale(u32 r) {
    GTE_MTC2(9, r);
    GTE_CMD(0x4B98003D);
    return GTE_MFC2(9);
}

/* A direction pair scaled by IR0 = radius (GPF sf=1), packed as an xy word. */
static u32 ringPoint(u32 dx, u32 dy, u32 radius) {
    GTE_MTC2(9, dx);
    GTE_MTC2(10, dy);
    GTE_MTC2(8, radius);
    GTE_CMD(0x4B98003D);
    return (GTE_MFC2(9) & 0xFFFF) | (GTE_MFC2(10) << 16);
}

/* Sets up a row: the four radii in the context (unk2C/unk34 inner plain/scaled, unk30/unk38 outer). */
static void ringRadii(u8 *ctx, u16 *scale, u16 **sizes) {
    u32 on = scale[0], f = scale[1], r, s;

    r = **sizes << 2;
    if (on) {
        GTE_MTC2(8, f);
        s = ringScale(r);
    } else {
        s = r;
    }
    W(ctx, 0x2C) = r;
    W(ctx, 0x34) = s;
    (*sizes)++;
    r = **sizes << 2;
    s = on ? ringScale(r) : r;
    W(ctx, 0x30) = r;
    W(ctx, 0x38) = s;
}

/* POLY_FT4 grid: color unk0, uv pairs from unk14 (rows of stride + 2 u16 entries, each entry + unk20), uv0's
 * word ored with unk4 (clut << 16), uv1's with unk28 (tpage << 16). The uv column is (i - 1) & stride while
 * stride + 1 < points, else i - 1. */
void btlfx_emitRingQuads(s32 stride, Unk800D4768Ctx *ctx, u8 *out, s32 *ot) {
    u32 clut = ctx->clut;
    s32 last = ctx->last;
    s32 zbase = ctx->otZ;
    u32 color = ctx->color;
    u16 *scale = ctx->scale;
    u16 *sizes = ctx->sizes;
    u32 uvAdd = ctx->uvAdd;
    u16 *uvRow = ctx->frame;
    u16 *dir;
    u32 inner, outer, prevInner = 0, prevOuter = 0, r, sz;
    s32 i, col;
    u16 *uv;

    do {
        ringRadii((u8 *)ctx, scale, &sizes);
        dir = ctx->dirs;
        for (i = 0; i <= last; i++) {
            r = (i & 1) ? W(ctx, 0x34) : W(ctx, 0x2C);
            inner = ringPoint(dir[0], dir[1], r);
            r = (i & 1) ? W(ctx, 0x38) : W(ctx, 0x30);
            outer = ringPoint(dir[0], dir[1], r);
            dir += 2;
            if (i != 0) {
                GTE_MTC2(0, prevOuter);
                GTE_MTC2(2, outer);
                GTE_MTC2(4, prevInner);
                col = (stride + 1 < last) ? ((i - 1) & stride) : (i - 1);
                GTE_CMD(0x4A280030);
                uv = uvRow + col;
                H(out, 0x1C) = uv[0] + uvAdd;
                H(out, 0x24) = uv[1] + uvAdd;
                uv += stride + 2;
                W(out, 0x8) = GTE_MFC2(12);
                W(out, 0x10) = GTE_MFC2(13);
                W(out, 0x18) = GTE_MFC2(14);
                sz = GTE_MFC2(19);
                if (GTE_MFC2(24) == 0) {
                    return;
                }
                GTE_MTC2(0, inner);
                GTE_CMD(0x4A180001);
                W(out, 0xC) = (uv[0] + uvAdd) | clut;
                W(out, 0x14) = (uv[1] + uvAdd) | ctx->tpage;
                W(out, 0x4) = color;
                W(out, 0x20) = GTE_MFC2(14);
                r = ringZ(sz, zbase);
                if (r < 0x1000) {
                    ringLink((u32 *)ot + r, out, 9);
                }
                out += 0x28;
            }
            prevInner = inner;
            prevOuter = outer;
        }
        uvRow += stride + 2;
    } while (--ctx->count != 0);
}

/* The DPCS fade (IR0 = 0x1000 - fade) of a color. */
static u32 ringFade(u32 color, s32 fade) {
    GTE_MTC2(8, 0x1000 - fade);
    GTE_MTC2(6, color);
    GTE_CMD(0x4A780010);
    return GTE_MFC2(22);
}

/* Original convention: color in $s1 (result in $s1), fade in $t1. */
u32 btlfx_fadeColor(u32 color, s32 fade) {
    return ringFade(color, fade);
}

/* POLY_G4 grid + DR_MODE (E1000200 | unk28) per quad, both linked at the quad's depth (the mode first in the
 * chain): colors per row from two tables, unk14 and unk20 (advancing by `stride` words a row), the inner
 * edge from one and the outer from the other, swapping on odd points; all four faded by unk4 through
 * btlfx_fadeColor, the first ored with the code unk0. FC = 0. */
void btlfx_emitRingGouraud(s32 stride, Unk800D4A30Ctx *ctx, u8 *out, s32 *ot) {
    s32 fade = ctx->fade;
    s32 last = ctx->last;
    s32 zbase = ctx->otZ;
    u32 code = ctx->code;
    u16 *scale = (u16 *)ctx->scale;
    u16 *sizes = (u16 *)ctx->sizes;
    u32 *colA = ctx->colors;
    u32 *colB = ctx->colors2;
    u16 *dir;
    u8 *mode;
    u32 inner, outer, prevInner = 0, prevOuter = 0, r, sz;
    u32 *c0, *c1;
    s32 i;

    GTE_CTC2(21, 0);
    GTE_CTC2(22, 0);
    GTE_CTC2(23, 0);
    do {
        ringRadii((u8 *)ctx, scale, &sizes);
        dir = ctx->dirs;
        for (i = 0; i <= last; i++) {
            r = (i & 1) ? W(ctx, 0x34) : W(ctx, 0x2C);
            inner = ringPoint(dir[0], dir[1], r);
            r = (i & 1) ? W(ctx, 0x38) : W(ctx, 0x30);
            outer = ringPoint(dir[0], dir[1], r);
            dir += 2;
            if (i != 0) {
                GTE_MTC2(0, prevOuter);
                GTE_MTC2(2, outer);
                GTE_MTC2(4, prevInner);
                mode = out + 0x24;
                GTE_CMD(0x4A280030);
                W(mode, 4) = 0xE1000200 | ctx->abr;
                W(out, 0x8) = GTE_MFC2(12);
                W(out, 0x10) = GTE_MFC2(13);
                W(out, 0x18) = GTE_MFC2(14);
                if (GTE_MFC2(24) == 0) {
                    return;
                }
                sz = GTE_MFC2(19);
                GTE_MTC2(0, inner);
                GTE_CMD(0x4A180001);
                r = ringZ(sz, zbase);
                if (r < 0x1000) {
                    ringLink((u32 *)ot + r, out, 8);
                }
                if (r < 0x1000) {
                    ringLink((u32 *)ot + r, mode, 1);
                }
                W(out, 0x20) = GTE_MFC2(14);
                c0 = (i & 1) ? colB : colA;
                c1 = (i & 1) ? colA : colB;
                W(out, 0x14) = ringFade(*c0, fade);
                W(out, 0x1C) = ringFade(*c1, fade);
                W(out, 0x4) = ringFade(c0[stride], fade) | code;
                W(out, 0xC) = ringFade(c1[stride], fade);
                out += 0x2C;
            }
            prevInner = inner;
            prevOuter = outer;
        }
        colB += stride;
        colA += stride;
    } while (--ctx->count != 0);
}
