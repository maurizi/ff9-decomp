/* ovl_0b effect frame emitters of btlfx_drawFan (0x800D57D0-0x800D5C30), hand-written asm in the original:
 * btlfx_emitFrameQuad (one textured POLY_FT4), btlfx_emitFrameFan (a fan of three POLY_G3 and a DR_MODE) and
 * btlfx_linkFrameTri, btlfx_emitFrameFan's OT-link helper.
 *
 * Register convention: both emitters take standard C arguments and keep $ra in $t9 (btlfx_emitFrameFan calls
 * btlfx_linkFrameTri). btlfx_linkFrameTri is internal: packet in $a2, OT in $a3, SZ in $t2, z base in $t4; it links
 * the packet with length 6 and clobbers $v0, $v1, $t0, $t1. Its C form takes them as arguments.
 * GTE: FC = 0, V0-V2, RTPT/RTPS outputs, the DPCS/DPCT fade registers.
 * A frame whose first RTPT gives MAC0 == 0 emits nothing more (no NCLIP; MAC0 is RTPT's depth-cue product).
 * OT entry ((SZ3 >> 2) - unk4) >> 2 when below 0x1000.
 *
 * Checked with tools/equiv.py against the original (src_equiv/ovl_0b/check.py).
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))

static void frameLinkZ(s32 *ot, u32 sz, s32 zbase, u8 *p, u32 len) {
    u32 z = (((s32)sz >> 2) - zbase) >> 2;
    u32 e;

    if (z < 0x1000) {
        e = ((u32 *)ot)[z];
        W(p, 0) = (e & 0xFFFFFF) | (len << 24);
        ((u32 *)ot)[z] = (e & 0xFF000000) | ((u32)p & 0xFFFFFF);
    }
}

/* One POLY_FT4 for a frame record {u16 tpage, u16 clut, u16 w, u16 h, u16 tw, u16 th, colors (4 bytes each)
 * from +0xC}: corners (-+w*2, 0, -c / h*4 - c) with c = ctx unkC, uv from the u8 pair at ctx unk10 spanning
 * tw x th, tpage | unk18, color entry unk14 | code unk0, DPCS-faded when unk8 >= 0. The corner words are
 * left in ctx unk1C / unk20. */
void btlfx_emitFrameQuad(u8 *rec, Unk800D57D0Ctx *ctx, u8 *out, s32 *ot) {
    u32 code = ctx->unk0;
    s32 zbase = ctx->unk4;
    s32 fade = ctx->unk8;
    u8 *uvb = (u8 *)ctx->unk10;
    u32 hx, nx, c, z1, u0, v0, uR, vB, sz, col;

    GTE_CTC2(21, 0);
    GTE_CTC2(22, 0);
    GTE_CTC2(23, 0);
    hx = H(rec, 6) << 1;
    c = -ctx->unkC;
    GTE_MTC2(4, hx);
    ctx->unk1C = hx;
    nx = -hx & 0xFFFF;
    GTE_MTC2(0, nx);
    GTE_MTC2(2, nx);
    GTE_MTC2(1, c);
    GTE_MTC2(5, c);
    z1 = (H(rec, 4) << 2) + c;
    GTE_MTC2(3, z1);
    ctx->unk20 = z1;
    u0 = uvb[0];
    v0 = uvb[1];
    GTE_CMD(0x4A280030);
    W(out, 0xC) = (u0 | (v0 << 8)) | (H(rec, 2) << 16);
    uR = H(rec, 8) - 1 + u0;
    W(out, 0x14) = ((H(rec, 0) | ctx->unk18) << 16) | uR | (v0 << 8);
    vB = (v0 << 8) + ((H(rec, 0xA) - 1) << 8);
    H(out, 0x1C) = u0 | vB;
    H(out, 0x24) = uR | vB;
    W(out, 0x8) = GTE_MFC2(12);
    W(out, 0x10) = GTE_MFC2(13);
    W(out, 0x18) = GTE_MFC2(14);
    if (GTE_MFC2(24) == 0) {
        return;
    }
    sz = GTE_MFC2(19);
    GTE_MTC2(0, ctx->unk1C);
    GTE_MTC2(1, ctx->unk20);
    rec += 0xC + ctx->unk14 * 4;
    GTE_CMD(0x4A180001);
    frameLinkZ(ot, sz, zbase, out, 9);
    col = H(rec, 0) | (rec[2] << 16);
    W(out, 0x20) = GTE_MFC2(14);
    col |= code;
    if (fade >= 0) {
        GTE_MTC2(8, 0x1000 - fade);
        GTE_MTC2(6, col);
        GTE_CMD(0x4A780010);
        col = GTE_MFC2(22);
    }
    W(out, 0x4) = col;
}

/* Original convention: packet $a2, OT $a3, SZ $t2, z base $t4. */
void btlfx_linkFrameTri(u8 *out, s32 *ot, u32 sz, s32 zbase) {
    frameLinkZ(ot, sz, zbase, out, 6);
}

/* Three POLY_G3 fanning out from (0, 0, -c) (c = ctx unk10) over a frame record {s16 p0-p4 (units of 4),
 * -, color pairs (8 bytes each) from +0xC}, vertices (x, 0, z): A = (0, -c), B = (-p1, p2 - c),
 * C = (-p3, p4 - c), D = (p1, p0 - c); triangles A-B-C, B-D-C, C-D-A (each RTPT only reloads the vertices
 * that change). Colors: color pair unk14 (| code unk0, DPCT-faded when unk8 >= 0); the first two triangles
 * are code, 0, rim color, the last rim color, 0, center color. All three and the closing DR_MODE
 * (E1000200 | unkC) use the first triangle's SZ3 for the OT entry. */
void btlfx_emitFrameFan(u8 *pts, Unk800D59D0Ctx *ctx, u8 *out, s32 *ot) {
    u32 code = ctx->unk0;
    s32 zbase = ctx->unk4;
    s32 fade = ctx->unk8;
    u32 tpage = ctx->unkC;
    s32 c = -ctx->unk10;
    s32 a, b, d, e, f;
    u32 ny0, py0, ny1, col0, col1, sz = 0;
    u8 *cp;
    s32 k;

    GTE_CTC2(21, 0);
    GTE_CTC2(22, 0);
    GTE_CTC2(23, 0);
    a = *(s16 *)(pts + 0) << 2;
    b = *(s16 *)(pts + 2) << 2;
    d = *(s16 *)(pts + 4) << 2;
    e = *(s16 *)(pts + 6) << 2;
    f = *(s16 *)(pts + 8) << 2;
    cp = pts + 0xC + ctx->unk14 * 8;
    col0 = H(cp, 0) | (cp[2] << 16) | code;
    col1 = H(cp, 4) | (cp[6] << 16) | code;
    if (fade >= 0) {
        GTE_MTC2(8, 0x1000 - fade);
        GTE_MTC2(6, code);
        GTE_MTC2(20, col0);
        GTE_MTC2(21, col1);
        GTE_CMD(0x4AF8002A);
        col0 = GTE_MFC2(20);
        col1 = GTE_MFC2(21);
    }
    a += c;
    d += c;
    f += c;
    py0 = b & 0xFFFF;
    ny0 = -b & 0xFFFF;
    ny1 = -e & 0xFFFF;
    for (k = 0;; k++) {
        if (k == 0) {
            GTE_MTC2(0, 0);
            GTE_MTC2(2, ny0);
            GTE_MTC2(4, ny1);
            GTE_MTC2(1, c);
            GTE_MTC2(3, d);
            GTE_MTC2(5, f);
            W(out, 0x4) = code;
            W(out, 0x14) = col1;
        } else if (k == 1) {
            GTE_MTC2(0, ny0);
            GTE_MTC2(2, py0);
            GTE_MTC2(1, d);
            GTE_MTC2(3, a);
            W(out, 0x4) = code;
            W(out, 0x14) = col1;
        } else {
            GTE_MTC2(0, ny1);
            GTE_MTC2(4, 0);
            GTE_MTC2(1, f);
            GTE_MTC2(5, c);
            W(out, 0x4) = col1;
            W(out, 0x14) = col0;
        }
        GTE_CMD(0x4A280030);
        W(out, 0xC) = 0;
        if (k != 0) {
            frameLinkZ(ot, sz, zbase, out, 6);
        }
        W(out, 0x8) = GTE_MFC2(12);
        W(out, 0x10) = GTE_MFC2(13);
        W(out, 0x18) = GTE_MFC2(14);
        if (k == 0) {
            if (GTE_MFC2(24) == 0) {
                return;
            }
            sz = GTE_MFC2(19);
            frameLinkZ(ot, sz, zbase, out, 6);
        }
        if (k == 2) {
            out += 0x1C;
            W(out, 0x4) = 0xE1000200 | tpage;
            frameLinkZ(ot, sz, zbase, out, 1);
            return;
        }
        out += 0x1C;
    }
}
