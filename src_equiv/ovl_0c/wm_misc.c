/* ovl_0c world map helpers (hand-written asm in the original, standard C ABI). */
#include "ovl_0c.h"
#include "../src_equiv.h"

/* Is any of the object's points on screen inside the rectangle pair rects[idx] (near, SZ3 >= 0x2260) or
 * rects[idx + 1] (far, SZ3 < 0x2260)? Each rectangle is { top-left, bottom-right } as packed xy words.
 * o+0x04: points (8-byte SVECTORs), o+0x0C: s8 point indices, o+0x20: s16 count (>= 1).
 * Each point is transformed (MVMVA, current RT/TR), its y scaled by (4096 - MAC3/16)/4096 (the world's
 * curvature), then projected with RTPS. No callers in the game. */
s32 func_800B3844(u8 *o, u32 *rects, s32 idx) {
    u8 *pts = *(u8 **)(o + 4);
    s32 n = *(s16 *)(o + 0x20);
    s8 *ix = (s8 *)(o + 0xC);
    SVECTOR v;
    s32 m1, m2, m3;
    u32 sxy;
    s32 sz;
    s32 x, y, f;
    u32 *r;

    do {
        v = *(SVECTOR *)(pts + *ix++ * 8);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stmac123(m1, m2, m3);
        f = 0x1000 - (m3 >> 4);
        v.vy = (v.vy * f) >> 12;
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&sxy);
        gte_stsz(&sz);
        r = rects + ((sz < 0x2260) + idx) * 2;
        x = (s16)sxy;
        y = (s32)sxy >> 16;
        if ((s32)r[0] >> 16 > y || (s16)r[0] > x) {
            continue;
        }
        if (y > (s32)r[1] >> 16 || x > (s16)r[1]) {
            continue;
        }
        return 1;
    } while (--n != 0);
    return 0;
}

/* Depth-cue a palette: dst[0] = src[0], then 15 colours (15-bit BGR + bit 15) go through DPCL with IR0 = depth
 * and IR1-3 = col[0..2]; bit 15 is kept. */
void wm_depthCueClut(u16 *src, u16 *dst, s32 depth, s16 *col) {
    s32 i;
    u32 c;
    u32 rgb;

    *dst++ = *src++;
    for (i = 0; i < 15; i++) {
        c = *src++;
        gte_ldrgbc(((c & 0x7C00) << 9) | ((c & 0x3E0) << 6) | ((c & 0x1F) << 3));
        gte_ldir0123(depth, col[0], col[1], col[2]);
        gte_dpcl();
        gte_strgb2(rgb);
        *dst++ = (c & 0x8000) | ((rgb >> 9) & 0x7C00) | ((rgb >> 6) & 0x3E0) | ((rgb >> 3) & 0x1F);
    }
}

extern SVECTOR D_800CD794[]; /* normals, indexed by the low byte of a vertex's pad */
extern u32 D_800D1FF0[];     /* per-vertex fog colours */
extern u32 D_800D23F0[];     /* per-vertex lit colours */
extern u16 D_800D29F8[];     /* per-vertex SZ / 16 */

/* (sz - 0x8000 + ofs) / 2, clamped to 0..0x1000 (an IR0 depth-cue factor). */
static s32 wm_backdropVerts_depth(s32 sz, s32 ofs) {
    s32 p = (sz - 0x8000 + ofs) >> 1;

    if (p <= 0) {
        p = 0;
    }
    if (p > 0x1000) {
        p = 0x1000;
    }
    return p;
}

/* Light and project a world-map mesh, three vertices at a time (the count is rounded up to a multiple of 3).
 * o+0x00: count in the low byte, o+0x04: vertices (SVECTORs, normal index in the low byte of pad).
 * For each vertex: NCT with its normal gives a colour, which DPCS pulls toward the far colour by
 * depth(SZ, g_worldBackdropFog.unk6) into D_800D23F0; g_worldBackdropFog's colour pulled by 1 - depth(SZ, g_worldBackdropFog.unk4)
 * goes into D_800D1FF0. Vertices with y >= 0 get lit colour 0 and the plain fog colour instead.
 * Then y is scaled by (4096 - SZ/16)/4096 (the world's curvature), and RTPT writes screen xy to the
 * scratchpad (3 words per triangle) and SZ/16 to D_800D29F8. */
void wm_backdropVerts(s32 arg0) {
    u8 *o = (u8 *)arg0;
    s32 n = *(u32 *)o & 0xFF;
    SVECTOR *v = *(SVECTOR **)(o + 4);
    u32 *fog = D_800D1FF0;
    u32 *lit = D_800D23F0;
    u32 *sxy = (u32 *)SCRATCHPAD;
    u16 *zs = D_800D29F8;
    u32 fogCol = *(u32 *)&g_worldBackdropFog.unk0;
    s32 fogOfs = (s16)g_worldBackdropFog.unk4;
    s32 litOfs = (s16)g_worldBackdropFog.unk6;
    SVECTOR w[3];
    SVECTOR nrm[3];
    s32 z[3];
    u32 rgb[3];
    s32 i;

    do {
        w[0] = v[0];
        w[1] = v[1];
        w[2] = v[2];
        gte_ldv3c(w);
        gte_rtpt();
        for (i = 0; i < 3; i++) {
            nrm[i] = D_800CD794[(u8)w[i].pad];
        }
        gte_ldv3c(nrm);
        gte_nct();
        gte_stsz3c(z);
        gte_strgb3(rgb);
        for (i = 0; i < 3; i++) {
            gte_ldrgbc(rgb[i]);
            gte_lddp(wm_backdropVerts_depth(z[i], litOfs));
            gte_dpcs();
            gte_strgb2(lit[i]);
            gte_ldrgbc(fogCol);
            gte_lddp(0x1000 - wm_backdropVerts_depth(z[i], fogOfs));
            gte_dpcs();
            gte_strgb2(fog[i]);
        }
        for (i = 0; i < 3; i++) {
            if (w[i].vy >= 0) {
                lit[i] = 0;
                fog[i] = fogCol;
            }
        }
        fog += 3;
        lit += 3;
        for (i = 0; i < 3; i++) {
            w[i].vy = (w[i].vy * (0x1000 - ((u32)z[i] >> 4))) >> 12;
        }
        gte_ldv3c(w);
        gte_rtpt();
        gte_stsxy3c(sxy);
        gte_stsz3c(z);
        for (i = 0; i < 3; i++) {
            zs[i] = (u32)z[i] >> 4;
        }
        sxy += 3;
        zs += 3;
        v += 3;
        n -= 3;
    } while (n > 0);
}
