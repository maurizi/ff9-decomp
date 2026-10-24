/* ovl_0a fldmdl_drawMesh: link one mesh group's prebuilt primitives into the current OT (hand-written asm in the
 * original, standard C ABI otherwise).
 *
 * The primitives already hold their colours/UVs; this fills in the screen positions from g_vtxScreenXY (computed
 * earlier per model vertex), culls back faces with NCLIP unless the polygon's flag byte has bit 1 (double-sided),
 * and links each one into the OT. Quads use (z0 + z1 + z2 + z3) >> 4 from g_vtxScreenZ as the OT index, triangles
 * the GTE's AVSZ3 (so its ZSF3 setting applies). The OT base is GameData.unk10[GameData.unk8] (the current
 * buffer's table) minus the group's s8 bias at +0x13. A culled primitive gets a bare length tag and isn't linked.
 *
 * model:  +0x08 -> mesh data (+0x10: groups, 0x28 bytes each), +0x44 -> packet sets (12 bytes per group, +4 is the
 *         packet buffer).
 * group:  +0x00 offset of the second buffer's packets (used when GameData.unk8 != 0), +0x02..+0x10 counts of the
 *         eight primitive types below, +0x13 OT bias, +0x1C polygon data (all types back to back, in order).
 *
 * Checked against the asm by reading only: tools/equiv.py can't exercise it from random states (the counts are
 * random u16s, so the original either faults on an unaligned pointer or runs into the step limit).
 */
#include "ovl_0a.h"

typedef struct {
    u8 count;   /* offset of the count in the group */
    u8 quad;    /* 4 vertices (indices: u16 x 4) or 3 (u16 x 3) */
    u8 src;     /* polygon record size */
    u8 flags;   /* offset of the flag byte in the record */
    u8 size;    /* primitive size in bytes */
    u8 len;     /* tag length (words) */
    u8 xy[4];   /* offsets of the xy words in the primitive */
} PrimKind;

static const PrimKind kinds[8] = {
    { 0x2, 1, 0x18, 0x15, 0x28, 9, { 0x08, 0x10, 0x18, 0x20 } },   /* textured quad */
    { 0x4, 0, 0x14, 0x12, 0x20, 7, { 0x08, 0x10, 0x18 } },         /* textured triangle */
    { 0x6, 1, 0x20, 0x1C, 0x34, 12, { 0x08, 0x14, 0x20, 0x2C } },  /* gouraud textured quad */
    { 0x8, 0, 0x18, 0x0F, 0x28, 9, { 0x08, 0x14, 0x20 } },         /* gouraud textured triangle */
    { 0xA, 1, 0x18, 0x14, 0x24, 8, { 0x08, 0x10, 0x18, 0x20 } },   /* gouraud quad */
    { 0xC, 0, 0x14, 0x13, 0x1C, 6, { 0x08, 0x10, 0x18 } },         /* gouraud triangle */
    { 0xE, 1, 0x10, 0x0D, 0x18, 5, { 0x08, 0x0C, 0x10, 0x14 } },   /* flat quad */
    { 0x10, 0, 0x0C, 0x0B, 0x14, 4, { 0x08, 0x0C, 0x10 } },        /* flat triangle */
};

void fldmdl_drawMesh(Unk800C95C8 *model, u8 group) {
    u8 *m = (u8 *)model;
    u8 *grp = *(u8 **)(*(u8 **)(m + 8) + 0x10) + group * 0x28;
    u8 *pkt = *(u8 **)(*(u8 **)(m + 0x44) + group * 12 + 4);
    u8 *poly = *(u8 **)(grp + 0x1C);
    GameData *gd = g_gameState->unk1C;
    u32 *ot = gd->unk10[gd->unk8];
    s32 k;
    s32 n;
    const PrimKind *t;
    u32 i0, i1, i2, i3;
    u32 xy0, xy1, xy2;
    s32 nclip;
    u32 z;
    u32 *slot;

    if (gd->unk8 != 0) {
        pkt += *(u16 *)grp;
    }
    ot -= (s8)grp[0x13];
    for (k = 0; k < 8; k++) {
        t = &kinds[k];
        for (n = *(u16 *)(grp + t->count); n > 0; n--, poly += t->src, pkt += t->size) {
            i0 = *(u16 *)(poly + 0);
            i1 = *(u16 *)(poly + 2);
            i2 = *(u16 *)(poly + 4);
            i3 = t->quad ? *(u16 *)(poly + 6) : 0;
            xy0 = g_vtxScreenXY[i0];
            xy1 = g_vtxScreenXY[i1];
            xy2 = g_vtxScreenXY[i2];
            if (!(poly[t->flags] & 2)) {
                gte_ldsxy3(xy0, xy1, xy2);
                *(u32 *)pkt = (u32)t->len << 24;
                gte_nclip();
                gte_stopz(&nclip);
                if (nclip <= 0) {
                    continue;
                }
            }
            *(u32 *)(pkt + t->xy[0]) = xy0;
            *(u32 *)(pkt + t->xy[1]) = xy1;
            *(u32 *)(pkt + t->xy[2]) = xy2;
            if (t->quad) {
                *(u32 *)(pkt + t->xy[3]) = g_vtxScreenXY[i3];
                z = ((u32)g_vtxScreenZ[i0] + g_vtxScreenZ[i1] + g_vtxScreenZ[i2] + g_vtxScreenZ[i3]) >> 4;
            } else {
                gte_ldsz3(g_vtxScreenZ[i0], g_vtxScreenZ[i1], g_vtxScreenZ[i2]);
                gte_avsz3();
                gte_stotz(&z);
            }
            slot = &ot[z];
            *(u32 *)pkt = *slot | ((u32)t->len << 24);
            *slot = (u32)pkt & 0xFFFFFF;
        }
    }
}
