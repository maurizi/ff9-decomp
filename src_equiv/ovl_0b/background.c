/* ovl_0b battle background drawing (0x800CEED4-0x800CF5A4), hand-written asm in the original.
 *
 * g_btlbgBrightness (0x800CF588) and g_btlbgOtShift (0x800CF5A0) are data words inside .text, between the
 * functions that set them (DS port F33); a port gives them ordinary storage.
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

/* Standard C ABI. */
void btlbg_setBrightness(s32 v) {
    g_btlbgBrightness = v;
}

s32 btlbg_getBrightness(void) {
    return g_btlbgBrightness;
}

void btlbg_setOtShift(s32 v) {
    g_btlbgOtShift = v;
}

/* Scratchpad layout btlbg_projectVertices leaves for btlbg_drawRecord: SXY of vertex i at 4 * i, SZ >> 4 at
 * 0x200 + 4 * i. Vertex indices in the records are these byte offsets. */
#define BG_SXY(off) (*(u32 *)(SCRATCHPAD + (off)))
#define BG_SZH(off) (*(u16 *)(SCRATCHPAD + 0x200 + (off)))
#define BG_SZW(off) (*(u32 *)(SCRATCHPAD + 0x200 + (off)))

/* RTPT over `count` packed vertices (6 bytes each: x, y, z s16; count must be a non-zero multiple of 6, or
 * the loop runs on), two RTPTs per round, stores SXY0-2 to the scratchpad and SZ1-3 >> 4 at +0x200. Reads
 * the words of six vertices plus the first word of the next group (one word past the end). GTE: V0-V2 (whole
 * words, so a VZ register gets the next vertex's x in its upper half where the packing puts it there),
 * RTPT outputs; the last RTPT's results stay in the GTE. */
static void btlbg_project(u32 *w, s32 count) {
    u32 *sxy = (u32 *)SCRATCHPAD;
    u32 *sz = (u32 *)(SCRATCHPAD + 0x200);
    s32 n = 0;
    u32 w0, w1, w2, w3, w4, w5, w6, w7, w8;

    w0 = w[0];
    for (;;) {
        w1 = w[1];
        w2 = w[2];
        w3 = w[3];
        w4 = w[4];
        if (n != 0) {
            sxy[3] = GTE_MFC2(12);
            sxy[4] = GTE_MFC2(13);
            sxy[5] = GTE_MFC2(14);
            sz[3] = GTE_MFC2(17) >> 4;
            sz[4] = GTE_MFC2(18) >> 4;
            sz[5] = GTE_MFC2(19) >> 4;
            if (n == count) {
                return;
            }
            sxy += 6;
            sz += 6;
        }
        GTE_MTC2(0, w0);
        GTE_MTC2(1, w1);
        GTE_MTC2(2, (w2 << 16) + (w1 >> 16));
        GTE_MTC2(3, w2 >> 16);
        GTE_MTC2(4, w3);
        GTE_MTC2(5, w4);
        GTE_CMD(0x4A280030);
        w5 = w[5];
        w6 = w[6];
        w7 = w[7];
        w8 = w[8];
        sxy[0] = GTE_MFC2(12);
        sxy[1] = GTE_MFC2(13);
        sxy[2] = GTE_MFC2(14);
        sz[0] = GTE_MFC2(17) >> 4;
        sz[1] = GTE_MFC2(18) >> 4;
        sz[2] = GTE_MFC2(19) >> 4;
        GTE_MTC2(0, (w5 << 16) + (w4 >> 16));
        GTE_MTC2(1, w5 >> 16);
        GTE_MTC2(2, w6);
        GTE_MTC2(3, w7);
        GTE_MTC2(4, (w8 << 16) + (w7 >> 16));
        GTE_MTC2(5, w8 >> 16);
        w += 9;
        n += 6;
        GTE_CMD(0x4A280030);
        w0 = w[0];
    }
}

/* Register convention: $a2 = vertices, $a3 = count; $a0/$a1 are ignored (btlbg_drawRecord's record and
 * packet pointer happen to be there). Clobbers only $v1, $a2, $t0-$t9: btlbg_drawRecord relies on $v0 (its
 * packet pointer) surviving the call. */
void btlbg_projectVertices(BtlBgRec *rec, u8 *out, u32 *verts, s32 count) {
    btlbg_project(verts, count);
}

/* The primitive's color scaled by the brightness (GPF sf=0, then each component >> 7), or the color as is
 * at 0x80. GTE: IR0 = brightness always, IR1-IR3 and GPF only when it isn't 0x80. The quad path adds this to
 * the command word and the triangle path ors it (the same on hardware, where the bits don't overlap). */
static u32 btlbg_shade(u32 c, u32 bright) {
    GTE_MTC2(8, bright);
    if (bright == 0x80) {
        return c & 0xFFFFFF;
    }
    GTE_MTC2(9, c & 0xFF);
    GTE_MTC2(10, (c >> 8) & 0xFF);
    GTE_MTC2(11, (c >> 16) & 0xFF);
    GTE_CMD(0x4B90003D);
    return (GTE_MFC2(9) >> 7) + ((GTE_MFC2(10) >> 7) << 8) + ((GTE_MFC2(11) >> 7) << 16);
}

/* The texture word for a color: the table entry (clut << 16 | tpage) the color's bits 24-28 pick, with the
 * color's bits 30-31 as the tpage's blend mode (bits 5-6). */
static u32 btlbg_texture(BtlBgGroup *grp, u32 c) {
    return ((u32 *)((u8 *)grp + grp->unkA))[(c >> 24) & 0x1F] | ((c >> 25) & 0x60);
}

/* Draws one background record: projects its vertices (btlbg_projectVertices), then emits a POLY_FT4 per
 * front-facing quad and a POLY_FT3 per front-facing triangle (NCLIP < 0 culls), colors scaled by
 * g_btlbgBrightness, linked into `ot` at an index from the vertex depths and the record's kind (BtlBgRec.unk0).
 * Only the low halfwords of g_btlbgBrightness and g_btlbgOtShift are read. Standard C ABI (returns the
 * advanced packet pointer). GTE: see btlbg_project and btlbg_shade, plus SXY0-SXY2 and NCLIP's MAC0. */
u8 *btlbg_drawRecord(BtlBgRec *rec, u8 *out, u32 *ot, BtlBgGroup *grp) {
    u32 flags = rec->unk0;
    u32 *col = (u32 *)((u8 *)rec + rec->unk6);
    u8 *idx = (u8 *)rec + rec->unk8;
    u8 *uv = (u8 *)rec + rec->unkA;
    s32 nquad = rec->unkC;
    s32 ntri = rec->unkE;
    u32 bright = *(u16 *)&g_btlbgBrightness;
    u32 shift;
    u32 xy0, xy1, xy2, xy3, z, c, t, u, code;
    u32 i01, i23;

    btlbg_project((u32 *)((u8 *)grp + grp->unkE + rec->unk4 * 6), rec->unk2);
    shift = *(u16 *)&g_btlbgOtShift & 31;

    for (; nquad != 0; nquad--, idx += 8, uv += 8, col++) {
        i01 = *(u32 *)idx;
        xy0 = BG_SXY(i01 & 0xFFFF);
        z = BG_SZH(i01 & 0xFFFF);
        xy1 = BG_SXY(i01 >> 16);
        z += BG_SZH(i01 >> 16);
        i23 = *(u32 *)(idx + 4);
        z += BG_SZH(i23 & 0xFFFF);
        xy2 = BG_SXY(i23 & 0xFFFF);
        GTE_MTC2(12, xy0);
        GTE_MTC2(13, xy1);
        GTE_MTC2(14, xy2);
        GTE_CMD(0x4B400006);
        if ((s32)GTE_MFC2(24) < 0) {
            continue;
        }
        xy3 = BG_SXY(i23 >> 16);
        if (flags & 8) {
            z = 0x3FC;
        } else {
            z += BG_SZH(i23 >> 16);
            if (flags == 0) {
                z = (z << shift) & 0xFFFC;
                if (z == 0) {
                    continue;
                }
                if (z & 0xC000) {
                    z = 0x3FFC;
                }
            } else {
                z >>= 6;
                if (flags & 2) {
                    z = 0;
                } else if (z & 0xFF00) {
                    z = 0x3FC;
                } else {
                    z <<= 2;
                    if (z == 0) {
                        continue;
                    }
                }
            }
        }
        out[3] = 9;
        *(u32 *)(out + 0x8) = xy0;
        *(u32 *)(out + 0x10) = xy1;
        *(u32 *)(out + 0x18) = xy2;
        *(u32 *)(out + 0x20) = xy3;
        c = *col;
        t = btlbg_shade(c, bright);
        code = (c & 0x20000000) ? 0x2E000000 : 0x2C000000;
        *(u32 *)(out + 0x4) = code + t;
        t = btlbg_texture(grp, c);
        u = *(u32 *)uv;
        *(u32 *)(out + 0x14) = (u >> 16) + (t << 16);
        *(u32 *)(out + 0xC) = (u & 0xFFFF) + ((t >> 16) << 16);
        u = *(u32 *)(uv + 4);
        *(u16 *)(out + 0x1C) = u;
        *(u16 *)(out + 0x24) = u >> 16;
        addPrim((u8 *)ot + z, out);
        out += 0x28;
    }

    for (; ntri != 0; ntri--, idx += 6, uv += 6, col++) {
        u32 h0 = *(u16 *)idx, h1 = *(u16 *)(idx + 2), h2;

        xy0 = BG_SXY(h0);
        z = BG_SZH(h0);
        xy1 = BG_SXY(h1);
        z = (z + BG_SZH(h1)) >> 1;
        h2 = *(u16 *)(idx + 4);
        xy2 = BG_SXY(h2);
        z = (z + BG_SZW(h2)) >> 1; /* a whole word here, halfwords above */
        GTE_MTC2(12, xy0);
        GTE_MTC2(13, xy1);
        GTE_MTC2(14, xy2);
        GTE_CMD(0x4B400006);
        if ((s32)GTE_MFC2(24) < 0) {
            continue;
        }
        if (flags == 0) {
            if (z == 0) {
                continue;
            }
            /* No 0xFFFC mask here, and only bits 14-15 are tested: a larger depth indexes past the OT. */
            z = (z << shift) << 2;
            if (z & 0xC000) {
                z = 0x3FFC;
            }
        } else if (flags & 8) {
            z = 0x3FC;
        } else {
            z >>= 4;
            if (flags & 2) {
                z = 0;
            } else if (z & 0xFF00) {
                z = 0x3FC;
            } else {
                z <<= 2;
                if (z == 0) {
                    continue;
                }
            }
        }
        out[3] = 7;
        *(u32 *)(out + 0x8) = xy0;
        *(u32 *)(out + 0x10) = xy1;
        *(u32 *)(out + 0x18) = xy2;
        c = *col;
        t = btlbg_shade(c, bright);
        code = (c & 0x20000000) ? 0x26000000 : 0x24000000;
        *(u32 *)(out + 0x4) = code | t;
        t = btlbg_texture(grp, c);
        *(u32 *)(out + 0xC) = *(u16 *)uv | ((t >> 16) << 16);
        *(u32 *)(out + 0x14) = *(u16 *)(uv + 2) | (t << 16);
        *(u16 *)(out + 0x1C) = *(u16 *)(uv + 4);
        addPrim((u8 *)ot + z, out);
        out += 0x20;
    }
    return out;
}
