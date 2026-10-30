/* ovl_0b battle model packet emitters btl_drawMesh (0x800C58E4) and btl_drawMeshOffset (0x800DD27C),
 * hand-written asm in the original.
 *
 * Register convention: standard C ABI (they save the $s registers they use), but neither keeps a frame for
 * locals. GTE: SXY0-SXY2 and NCLIP's MAC0 per front-face test, SZ1-SZ3 and AVSZ3's OTZ per triangle; nothing
 * else.
 *
 * A mesh (BtlMesh) holds eight primitive lists one after the other at unk1C, with the counts at unk2-unk10:
 *   type  count  source  packet     source layout (u16 vertex/uv/color indices)
 *   0     unk2   0x18    FT4 0x28   v[4] uv[4] rgb.w tex@13 flags@15
 *   1     unk4   0x14    FT3 0x20   v[3] tex@6 rgb.w@8 uv[3]@C flags@12
 *   2     unk6   0x20    GT4 0x34   v[4] uv[4] c[4]@10 flags@1C tex@1D
 *   3     unk8   0x18    GT3 0x28   v[3] uv[3]@6 flags@F c[3]@10 tex@16
 *   4     unkA   0x18    G4  0x24   v[4] c[4]@8 flags@14
 *   5     unkC   0x14    G3  0x1C   v[3] c[3]@8 flags@13
 *   6     unkE   0x10    F4  0x18   v[4] rgb.w@8 flags@D
 *   7     unk10  0x0C    F3  0x14   v[3] rgb.w@8 flags@B
 * Vertices index g_vtxScreenXY/g_vtxScreenZ, uvs the mesh's u16 uv pairs (unk20), colors its words (unk24),
 * `tex` the model's texture words (BtlUnit9C.unk18: clut << 16 | tpage). Flags: bit 1 = double-sided (no NCLIP
 * test), bit 0 = semi-transparent (textured and, in btl_drawMeshOffset, Gouraud types), bits 5-7 = blend mode
 * (btl_drawMesh ors them into the tpage; btl_drawMeshOffset puts them in an E1 draw-mode word instead, in front
 * of each Gouraud polygon, and uses the model's tpage as is).
 * A face is culled when NCLIP <= 0. Quads take OT index (z0 + z1 + z2 + z3) >> 6, triangles AVSZ3's OTZ & ~3;
 * the OT is the frame's (GameData.unk10[unk8]) less 4 * BtlMesh.unk13 (plus 4 * the offset argument). The tag
 * written is the OT entry ored with the length (not masked: the entry's top byte is assumed 0) and the entry
 * becomes the packet address & 0xFFFFFF.
 * Delay-slot effects kept: a culled textured or flat triangle (types 1, 3, 7, and 5 in btl_drawMesh) still
 * writes its first vertex to out + 8 of the unused packet. After the lists a terminator byte 0xFF goes to
 * out + 7 and out + 8 is returned. A model with flag 0x10 emits only that. BtlUnit9C.unk10 records the first
 * packet the first time.
 *
 * Checked with tools/equiv.py against the original (src_equiv/ovl_0b/check.py).
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))

/* NCLIP on three screen xys: > 0 = front facing. */
static s32 meshNclip(u32 xy0, u32 xy1, u32 xy2) {
    GTE_MTC2(12, xy0);
    GTE_MTC2(13, xy1);
    GTE_MTC2(14, xy2);
    GTE_CMD(0x4B400006);
    return GTE_MFC2(24);
}

static u32 meshAvsz3(u32 z0, u32 z1, u32 z2) {
    GTE_MTC2(17, z0);
    GTE_MTC2(18, z1);
    GTE_MTC2(19, z2);
    GTE_CMD(0x4B58002D);
    return GTE_MFC2(7);
}

/* Links the packet: tag = OT entry | len << 24, entry = packet. */
static void meshLink(u8 *ot, u32 z, u8 *p, u32 len) {
    u32 *e = (u32 *)(ot + z);

    W(p, 0) = *e | (len << 24);
    *e = (u32)p & 0xFFFFFF;
}

/* Gouraud packets in btl_drawMeshOffset: an E1 word in its own 2-word header that heads the OT chain
 * (tag = entry | (len + 2) << 24), then the polygon with a bare length tag. */
static void meshLinkMode(u8 *ot, u32 z, u8 *mode, u8 *p, u32 len) {
    u32 *e = (u32 *)(ot + z);
    u32 t = *e;

    W(mode, 0) = t | ((len + 2) << 24);
    W(p, 0) = len << 24;
    *e = (u32)mode & 0xFFFFFF;
}

static u8 *meshEmit(BtlUnit9C *m, u32 meshIdx, u8 *out, s32 otOfs, s32 offsetVariant) {
    u32 *xyTab = (u32 *)g_vtxScreenXY;
    u32 *zTab = (u32 *)g_vtxScreenZ;
    u32 *tex = (u32 *)m->texPages;
    BtlMesh *mesh;
    u8 *src, *uv, *col, *ot, *mode;
    s32 n;
    u32 i0, i1, i2, i3, xy0, xy1, xy2, xy3, z, f, c, page, hi, lo, w0, w1;

    if (m->flags & 0x10) {
        goto end;
    }
    if (m->prims == 0) {
        m->prims = (s32)out;
    }
    mesh = &m->header->meshes[meshIdx];
    ot = (u8 *)g_gameState->gameData->ot[g_gameState->gameData->drawBuf] - mesh->otBias * 4;
    if (offsetVariant) {
        ot += otOfs * 4;
    }
    src = (u8 *)mesh->prims;
    uv = (u8 *)mesh->uvs;
    col = (u8 *)mesh->colors;

#define UV(k) H(uv, (k) * 2)
#define COL(k) W(col, (k) * 4)
/* lo half of a uv word: the tpage, ored with the blend bits in btl_drawMesh */
#define TPAGE_LO() (offsetVariant ? page << 16 : (page | (f & 0xE0)) << 16)

    /* 0: FT4 */
    for (n = mesh->nFT4; n > 0; n--, src += 0x18) {
        w0 = W(src, 0);
        w1 = W(src, 4);
        i0 = w0 & 0xFFFF;
        i1 = w0 >> 16;
        i2 = w1 & 0xFFFF;
        i3 = w1 >> 16;
        f = B(src, 0x15);
        xy0 = xyTab[i0];
        xy1 = xyTab[i1];
        xy2 = xyTab[i2];
        if (!(f & 2) && (s32)meshNclip(xy0, xy1, xy2) <= 0) {
            continue;
        }
        xy3 = xyTab[i3];
        W(out, 0x8) = xy0;
        W(out, 0x10) = xy1;
        W(out, 0x18) = xy2;
        W(out, 0x20) = xy3;
        z = (zTab[i0] + zTab[i1] + zTab[i2] + zTab[i3]) >> 6;
        c = (W(src, 0x10) & 0xFFFFFF) | 0x2C000000;
        if (f & 1) {
            c |= 0x02000000;
        }
        W(out, 0x4) = c;
        page = tex[B(src, 0x13)];
        hi = page & 0xFFFF0000;
        lo = TPAGE_LO();
        w0 = W(src, 8);
        w1 = W(src, 0xC);
        W(out, 0xC) = UV(w0 & 0xFFFF) | hi;
        W(out, 0x14) = UV(w0 >> 16) | lo;
        W(out, 0x1C) = UV(w1 & 0xFFFF);
        W(out, 0x24) = UV(w1 >> 16);
        meshLink(ot, z << 2, out, 9);
        out += 0x28;
    }

    /* 1: FT3 */
    for (n = mesh->nFT3; n > 0; n--, src += 0x14) {
        w0 = W(src, 0);
        i0 = w0 & 0xFFFF;
        i1 = w0 >> 16;
        i2 = H(src, 4);
        f = B(src, 0x12);
        xy0 = xyTab[i0];
        xy1 = xyTab[i1];
        xy2 = xyTab[i2];
        if (!(f & 2) && (s32)meshNclip(xy0, xy1, xy2) <= 0) {
            W(out, 0x8) = xy0;
            continue;
        }
        W(out, 0x8) = xy0;
        W(out, 0x10) = xy1;
        W(out, 0x18) = xy2;
        z = meshAvsz3(zTab[i0], zTab[i1], zTab[i2]);
        c = (W(src, 8) & 0xFFFFFF) | 0x24000000;
        if (f & 1) {
            c |= 0x02000000;
        }
        W(out, 0x4) = c;
        page = tex[B(src, 6)];
        hi = page & 0xFFFF0000;
        lo = TPAGE_LO();
        w0 = W(src, 0xC);
        W(out, 0xC) = UV(w0 & 0xFFFF) | hi;
        W(out, 0x14) = UV(w0 >> 16) | lo;
        W(out, 0x1C) = UV(H(src, 0x10));
        meshLink(ot, z & ~3, out, 7);
        out += 0x20;
    }

    /* 2: GT4 */
    for (n = mesh->nGT4; n > 0; n--, src += 0x20) {
        w0 = W(src, 0);
        w1 = W(src, 4);
        i0 = w0 & 0xFFFF;
        i1 = w0 >> 16;
        i2 = w1 & 0xFFFF;
        i3 = w1 >> 16;
        f = B(src, 0x1C);
        xy0 = xyTab[i0];
        xy1 = xyTab[i1];
        xy2 = xyTab[i2];
        if (!(f & 2) && (s32)meshNclip(xy0, xy1, xy2) <= 0) {
            continue;
        }
        xy3 = xyTab[i3];
        W(out, 0x8) = xy0;
        W(out, 0x14) = xy1;
        W(out, 0x20) = xy2;
        W(out, 0x2C) = xy3;
        z = (zTab[i0] + zTab[i1] + zTab[i2] + zTab[i3]) >> 6;
        w0 = W(src, 0x10);
        w1 = W(src, 0x14);
        c = COL(w0 & 0xFFFF) | 0x3C000000;
        if (f & 1) {
            c |= 0x02000000;
        }
        W(out, 0x4) = c;
        W(out, 0x10) = COL(w0 >> 16);
        W(out, 0x1C) = COL(w1 & 0xFFFF);
        W(out, 0x28) = COL(w1 >> 16);
        page = tex[B(src, 0x1D)];
        hi = page & 0xFFFF0000;
        lo = TPAGE_LO();
        w0 = W(src, 8);
        w1 = W(src, 0xC);
        W(out, 0xC) = UV(w0 & 0xFFFF) | hi;
        W(out, 0x18) = UV(w0 >> 16) | lo;
        W(out, 0x24) = UV(w1 & 0xFFFF);
        W(out, 0x30) = UV(w1 >> 16);
        meshLink(ot, z << 2, out, 0xC);
        out += 0x34;
    }

    /* 3: GT3 */
    for (n = mesh->nGT3; n > 0; n--, src += 0x18) {
        w0 = W(src, 0);
        i0 = w0 & 0xFFFF;
        i1 = w0 >> 16;
        i2 = H(src, 4);
        f = B(src, 0xF);
        xy0 = xyTab[i0];
        xy1 = xyTab[i1];
        xy2 = xyTab[i2];
        if (!(f & 2) && (s32)meshNclip(xy0, xy1, xy2) <= 0) {
            W(out, 0x8) = xy0;
            continue;
        }
        W(out, 0x8) = xy0;
        W(out, 0x14) = xy1;
        W(out, 0x20) = xy2;
        z = meshAvsz3(zTab[i0], zTab[i1], zTab[i2]);
        w0 = W(src, 0x10);
        c = COL(w0 & 0xFFFF) | 0x34000000;
        if (f & 1) {
            c |= 0x02000000;
        }
        W(out, 0x4) = c;
        W(out, 0x10) = COL(w0 >> 16);
        W(out, 0x1C) = COL(H(src, 0x14));
        page = tex[B(src, 0x16)];
        hi = page & 0xFFFF0000;
        lo = TPAGE_LO();
        w0 = W(src, 8);
        W(out, 0xC) = UV(H(src, 6)) | hi;
        W(out, 0x18) = UV(w0 & 0xFFFF) | lo;
        W(out, 0x24) = UV(w0 >> 16);
        meshLink(ot, z & ~3, out, 9);
        out += 0x28;
    }

    /* 4: G4 */
    for (n = mesh->nG4; n > 0; n--, src += 0x18) {
        w0 = W(src, 0);
        w1 = W(src, 4);
        i0 = w0 & 0xFFFF;
        i1 = w0 >> 16;
        i2 = w1 & 0xFFFF;
        i3 = w1 >> 16;
        f = B(src, 0x14);
        xy0 = xyTab[i0];
        xy1 = xyTab[i1];
        xy2 = xyTab[i2];
        if (!(f & 2) && (s32)meshNclip(xy0, xy1, xy2) <= 0) {
            continue;
        }
        if (offsetVariant) {
            mode = out;
            out += 8;
            W(mode, 4) = 0xE1000000 | (f & 0xE0);
        }
        xy3 = xyTab[i3];
        W(out, 0x8) = xy0;
        W(out, 0x10) = xy1;
        W(out, 0x18) = xy2;
        W(out, 0x20) = xy3;
        z = (zTab[i0] + zTab[i1] + zTab[i2] + zTab[i3]) >> 6;
        w0 = W(src, 8);
        w1 = W(src, 0xC);
        c = COL(w0 & 0xFFFF) | 0x38000000;
        if (offsetVariant && (f & 1)) {
            c |= 0x02000000;
        }
        W(out, 0x4) = c;
        W(out, 0xC) = COL(w0 >> 16);
        W(out, 0x14) = COL(w1 & 0xFFFF);
        W(out, 0x1C) = COL(w1 >> 16);
        if (offsetVariant) {
            meshLinkMode(ot, z << 2, mode, out, 8);
        } else {
            meshLink(ot, z << 2, out, 8);
        }
        out += 0x24;
    }

    /* 5: G3 */
    for (n = mesh->nG3; n > 0; n--, src += 0x14) {
        w0 = W(src, 0);
        i0 = w0 & 0xFFFF;
        i1 = w0 >> 16;
        i2 = H(src, 4);
        f = B(src, 0x13);
        xy0 = xyTab[i0];
        xy1 = xyTab[i1];
        xy2 = xyTab[i2];
        if (!(f & 2) && (s32)meshNclip(xy0, xy1, xy2) <= 0) {
            if (!offsetVariant) {
                W(out, 0x8) = xy0;
            }
            continue;
        }
        if (offsetVariant) {
            mode = out;
            out += 8;
            W(mode, 4) = 0xE1000000 | (f & 0xE0);
        }
        W(out, 0x8) = xy0;
        W(out, 0x10) = xy1;
        W(out, 0x18) = xy2;
        z = meshAvsz3(zTab[i0], zTab[i1], zTab[i2]);
        w0 = W(src, 8);
        c = COL(w0 & 0xFFFF) | 0x30000000;
        if (offsetVariant && (f & 1)) {
            c |= 0x02000000;
        }
        W(out, 0x4) = c;
        W(out, 0xC) = COL(w0 >> 16);
        W(out, 0x14) = COL(H(src, 0xC));
        if (offsetVariant) {
            meshLinkMode(ot, z & ~3, mode, out, 6);
        } else {
            meshLink(ot, z & ~3, out, 6);
        }
        out += 0x1C;
    }

    /* 6: F4 (the color word's top byte is ored into the code, not masked) */
    for (n = mesh->nF4; n > 0; n--, src += 0x10) {
        w0 = W(src, 0);
        w1 = W(src, 4);
        i0 = w0 & 0xFFFF;
        i1 = w0 >> 16;
        i2 = w1 & 0xFFFF;
        i3 = w1 >> 16;
        f = B(src, 0xD);
        xy0 = xyTab[i0];
        xy1 = xyTab[i1];
        xy2 = xyTab[i2];
        if (!(f & 2) && (s32)meshNclip(xy0, xy1, xy2) <= 0) {
            continue;
        }
        xy3 = xyTab[i3];
        W(out, 0x8) = xy0;
        W(out, 0xC) = xy1;
        W(out, 0x10) = xy2;
        W(out, 0x14) = xy3;
        z = (zTab[i0] + zTab[i1] + zTab[i2] + zTab[i3]) >> 6;
        W(out, 0x4) = W(src, 8) | 0x28000000;
        meshLink(ot, z << 2, out, 5);
        out += 0x18;
    }

    /* 7: F3 */
    for (n = mesh->nF3; n > 0; n--, src += 0xC) {
        w0 = W(src, 0);
        i0 = w0 & 0xFFFF;
        i1 = w0 >> 16;
        i2 = H(src, 4);
        f = B(src, 0xB);
        xy0 = xyTab[i0];
        xy1 = xyTab[i1];
        xy2 = xyTab[i2];
        if (!(f & 2) && (s32)meshNclip(xy0, xy1, xy2) <= 0) {
            W(out, 0x8) = xy0;
            continue;
        }
        W(out, 0x8) = xy0;
        W(out, 0xC) = xy1;
        W(out, 0x10) = xy2;
        z = meshAvsz3(zTab[i0], zTab[i1], zTab[i2]);
        W(out, 0x4) = W(src, 8) | 0x20000000;
        meshLink(ot, z & ~3, out, 4);
        out += 0x14;
    }

#undef UV
#undef COL
#undef TPAGE_LO
end:
    out[7] = 0xFF;
    return out + 8;
}

u8 *btl_drawMesh(BtlUnit9C *m, u8 meshIdx, u8 *out) {
    return meshEmit(m, meshIdx, out, 0, 0);
}

/* btl_drawMesh with the OT moved by `otOfs` entries and E1 draw-mode words for the Gouraud types. */
u8 *btl_drawMeshOffset(BtlUnit9C *m, u8 meshIdx, u8 *out, s32 otOfs) {
    return meshEmit(m, meshIdx, out, otOfs, 1);
}
