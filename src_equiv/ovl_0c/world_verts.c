/* ovl_0c world map mesh renderer, vertex pass (hand-written asm in the original).
 *
 * Register convention: the argument in $a0 as for a C function, no result, but the routines use $s0-$s7
 * without a stack frame (they save them to the scratchpad at 0x1F8003E0-0x1F8003FF and restore them before
 * returning) and clobber $k1, $at and the $t registers. The C signatures are the ones in ovl_0c.h.
 *
 * They light and project the vertices of a mesh block `o` for the face pass (world_faces.c); func_800B3A80
 * in wm_misc.c is a sibling with its own fog formula.
 *   o+0x00: vertex count in the low byte (done three at a time, so effectively rounded up to a multiple of 3)
 *   o+0x04: vertices, SVECTORs; pad bits 0-7 = index into the normal table D_800CD794, pad bit 9 = which
 *           of the two D_800D1FE0 fog entries ({ colour, u16 fog offset, u16 lit offset }) applies.
 * For each group of three vertices:
 *   - RTPT on the raw vertices (current rotation/translation) for their depths SZ, then NCT on their normals;
 *   - colours, either
 *       fog (all but wm_blockVertsNear): each NCT colour is pulled toward the far colour with DPCS by
 *         depth(SZ, entry.unk6) into the lit colour D_800D23F0[i], and the entry's colour by
 *         4096 - depth(SZ, entry.unk4) into the fog colour D_800D1FF0[i], where
 *         depth(sz, ofs) = clamp((sz - 0x8000 + ofs) >> 2, 0, 4096) (>> 1 in wm_blockVertsFar); or
 *       the NCT colour straight into D_800D23F0[i] (wm_blockVertsNear);
 *   - the world's curvature (all but wm_modelVertsFlat): y *= (4096 - (SZ/16 + SZ/64)) / 4096, by a multiply, or
 *     in wm_blockVertsFar by INTPL toward a zero red far colour (the original order of GTE loads is kept there);
 *   - RTPT again: screen xy into the scratchpad (one word per vertex from 0x1F800000) and the depth into
 *     D_800D29F8[i], as SZ (u32) or SZ / 8 (u16).
 * Like func_800B3A80 the loop reads (but doesn't use) the six words after the last group.
 *
 * Equivalence: tools/equiv.py with GTE commands hashed (see the commit message for the trial counts).
 */
#include "ovl_0c.h"
#include "../src_equiv.h"

extern SVECTOR D_800CD794[]; /* normals, indexed by the low byte of a vertex's pad */
extern u32 D_800D1FF0[];     /* per-vertex fog colours */
extern u32 D_800D23F0[];     /* per-vertex lit colours */
extern u16 D_800D29F8[];     /* per-vertex depth: u32 SZ or u16 SZ / 8, depending on the routine */

#define WV_FOG 1        /* DPCS lit + fog colours (else: NCT colour to the lit colours only) */
#define WV_DEPTH_HALF 2 /* depth factor (sz - 0x8000 + ofs) / 2 instead of / 4 */
#define WV_CURVE_MUL 4  /* bend y with a multiply */
#define WV_CURVE_GTE 8  /* bend y with INTPL */
#define WV_SZ16 16      /* store SZ / 8 as u16 instead of SZ as u32 */

static s32 wv_depth(u32 sz, u32 ofs, s32 flags) {
    s32 p = (s32)(sz - 0x8000 + ofs) >> ((flags & WV_DEPTH_HALF) ? 1 : 2);

    if (p <= 0) {
        p = 0;
    }
    if (p > 0x1000) {
        p = 0x1000;
    }
    return p;
}

/* attr: the vertex's vz/pad word; bit 25 (pad bit 9) picks D_800D1FE0[0] or [1]. */
static void wv_fog(u32 rgb, u32 sz, u32 attr, s32 flags, u32 *lit, u32 *fog) {
    u32 *e = (u32 *)D_800D1FE0 + ((attr >> 25) & 1) * 2;
    u32 col = e[0];
    u32 ofs = e[1];
    u32 c;

    gte_ldrgbc(rgb);
    gte_lddp(wv_depth(sz, ofs >> 16, flags));
    gte_dpcs();
    gte_strgb2(c);
    *lit = c;
    gte_ldrgbc(col);
    gte_lddp(0x1000 - wv_depth(sz, ofs & 0xFFFF, flags));
    gte_dpcs();
    gte_strgb2(c);
    *fog = c;
}

static void wv_transform(u8 *o, s32 flags) {
    s32 n = *(u32 *)o & 0xFF;
    u32 *v = *(u32 **)(o + 4);
    u32 *fog = D_800D1FF0;
    u32 *lit = D_800D23F0;
    u32 *sxy = (u32 *)SCRATCHPAD;
    u32 *zs32 = (u32 *)D_800D29F8;
    u16 *zs16 = D_800D29F8;
    u32 w[6]; /* three vertices as raw words: w[2i] = x | y << 16, w[2i + 1] = z | pad << 16 */
    u32 sz[3];
    u32 rgb[3];
    u32 f, m, rfc;
    s32 i, y;

    do {
        for (i = 0; i < 6; i++) {
            w[i] = v[i];
        }
        gte_ldv3c(w);
        gte_rtpt();
        gte_ldv3(&D_800CD794[(w[1] >> 16) & 0xFF], &D_800CD794[(w[3] >> 16) & 0xFF],
                 &D_800CD794[(w[5] >> 16) & 0xFF]);
        v += 6;
        n -= 3;
        gte_nct();
        gte_stsz3c(sz);
        gte_strgb3(rgb);
        if (flags & WV_FOG) {
            for (i = 0; i < 3; i++) {
                wv_fog(rgb[i], sz[i], w[2 * i + 1], flags, &lit[i], &fog[i]);
            }
            fog += 3;
        } else {
            lit[0] = rgb[0];
            lit[1] = rgb[1];
            lit[2] = rgb[2];
        }
        lit += 3;

        if (flags & WV_CURVE_MUL) {
            for (i = 0; i < 3; i++) {
                f = 0x1000 - ((sz[i] >> 4) + (sz[i] >> 6));
                y = (s32)(w[2 * i] & 0xFFFF0000) >> 16;
                y = (s32)((u32)y * f) >> 12;
                w[2 * i] = (w[2 * i] & 0xFFFF) | ((u32)y << 16);
            }
        }
        if (flags & WV_CURVE_GTE) {
            /* IR1 = y, IR0 = SZ/16 + SZ/64, far colour red = 0: MAC1 = y - y * IR0 / 4096. The original loads
             * V0 before the third INTPL and V1 after it; kept in that order. */
            gte_strfc(rfc);
            gte_ldrfc(0);
            for (i = 0; i < 3; i++) {
                gte_lddp((sz[i] >> 4) + (sz[i] >> 6));
                gte_ldir1((s32)w[2 * i] >> 16);
                w[2 * i] &= 0xFFFF;
                if (i == 2) {
                    gte_ldv0(&w[0]);
                }
                gte_intpl();
                if (i == 2) {
                    gte_ldv1(&w[2]);
                }
                gte_stmac1(m);
                w[2 * i] |= m << 16;
            }
            gte_ldv2(&w[4]);
            gte_ldrfc(rfc);
        } else {
            gte_ldv3c(w);
        }
        gte_rtpt();
        gte_stsxy3c(sxy);
        sxy += 3;
        gte_stsz3c(sz);
        if (flags & WV_SZ16) {
            zs16[0] = sz[0] >> 3;
            zs16[1] = sz[1] >> 3;
            zs16[2] = sz[2] >> 3;
            zs16 += 3;
        } else {
            zs32[0] = sz[0];
            zs32[1] = sz[1];
            zs32[2] = sz[2];
            zs32 += 3;
        }
    } while (n > 0);
}

/* Fog, curved, depth as u32 SZ. World blocks (func_800BA2DC) while g_worldFog is set. */
void wm_blockVertsFog(void *o) {
    wv_transform(o, WV_FOG | WV_CURVE_MUL);
}

/* Fog, curved, depth as u16 SZ / 8. Placed models (func_800BA458), modes 0 and 1. */
void wm_modelVerts(s32 o) {
    wv_transform((u8 *)o, WV_FOG | WV_CURVE_MUL | WV_SZ16);
}

/* Fog, flat, depth as u16 SZ / 8. Placed models (func_800BA458), mode 2. */
void wm_modelVertsFlat(s32 o) {
    wv_transform((u8 *)o, WV_FOG | WV_SZ16);
}

/* Fog with the steeper ramp, curved through the GTE, depth as u32 SZ. Far world blocks without fog. */
void wm_blockVertsFar(void *o) {
    wv_transform(o, WV_FOG | WV_DEPTH_HALF | WV_CURVE_GTE);
}

/* NCT colour only, curved, depth as u32 SZ. Near world blocks without fog. */
void wm_blockVertsNear(void *o) {
    wv_transform(o, WV_CURVE_MUL);
}
