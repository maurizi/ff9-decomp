/* ovl_0c world map mesh renderer, face pass (hand-written asm in the original).
 *
 * Register convention: arguments in $a0-$a2 and the result in $v0 as for C functions, but the routines use
 * $s0-$s7 without a stack frame (they save them to the scratchpad at 0x1F8003E0-0x1F8003FF and restore them
 * before returning) and clobber $k1, $at and the $t registers. The C signatures are the ones in ovl_0c.h.
 *
 * Each routine walks the faces of a mesh block `o` whose vertices a vertex-pass routine (world_verts.c,
 * func_800B3A80 in wm_misc.c) has already projected: screen xy in the scratchpad (one word per vertex from
 * 0x1F800000), depths in D_800D29F8, lit colours in D_800D23F0 and fog colours in D_800D1FF0.
 *   o+0x02: face count (s16 in func_800B3FEC, u16 elsewhere; read once, do-while, so 0 means 65536)
 *   o+0x08: faces, 16 bytes each: word 0 = the three vertex indices in bytes 0-2 (bit 24: depths + 0x200
 *           in the world-block routines); words 1-3 = the POLY_GT3 uv0|clut, uv1|tpage and uv2 words (bit 17 of
 *           word 3: use the alternate texture page described by the 12-byte entry *D_800D27F0, which every
 *           routine but func_800B3FEC first copies to the scratchpad at 0x1F8003D0).
 * A face is drawn when NCLIP of its three screen points is > 0 and its OT slot is in range. It gets
 *   - a POLY_G3 of the fog colours, if any of them is non-zero (in the variants that draw fog); its code is
 *     0x30, or 0x32 (semi-transparent) when the face has a non-zero lit colour;
 *   - a POLY_GT3 of the lit colours (code 0x34, or 0x36 in the placed-model routines), in most variants only when a
 *     lit colour is non-zero.
 * Both go into the same OT slot, the G3 first. The routines return the next free packet address.
 * World-block faces in OT slots >= 0x9C4 get half-resolution UVs: with the alternate-page bit they are moved
 * into that page (CLUT and tpage from it), otherwise the CLUT moves back 16 entries and the tpage's colour
 * depth bits are cleared (4-bit). Nearer faces, and all placed-model faces, only swap in the alternate page's
 * CLUT offset and tpage when the bit is set.
 * A vertex index >= 0xF4 would read the scratchpad's alternate-page copy or register save area, as in the
 * original (the game's meshes stay below that).
 *
 * Equivalence: tools/equiv.py with GTE commands hashed (see EQUIV notes at each routine).
 */
#include "ovl_0c.h"
#include "../src_equiv.h"

extern u32 D_800D1FF0[]; /* per-vertex fog colours */
extern u32 D_800D23F0[]; /* per-vertex lit colours */
extern u16 D_800D29F8[]; /* per-vertex depth: u32 SZ or u16 SZ / 8, depending on the vertex routine */

#define WF_FIXED_OT 1   /* func_800B3FEC: every face goes to OT slot 0xFA0, no depth test */
#define WF_Z32 2        /* depth = max of the u32 depths (+ 0x200 if face bit 24), slot = depth >> 3 */
#define WF_Z32_GRID 4   /* as WF_Z32, but the maximum is chosen on ((depth >> 3) & ~0xF) and range-tested on that */
#define WF_Z16 8        /* slot = max of the s16 depths & ~0xF */
#define WF_FOG 0x10     /* POLY_G3 of the fog colours when one is non-zero */
#define WF_NEED_LIT 0x20 /* POLY_GT3 only when a lit colour is non-zero */
#define WF_CULL 0x40    /* drop faces whose three vertices are all right/below of or left of the screen */
#define WF_LOD 0x80     /* half-resolution UVs for slots >= 0x9C4 */
#define WF_ALT 0x100    /* alternate page for bit 17 of uv word 3 (near version) */
#define WF_SEMI 0x200   /* POLY_GT3 code 0x36 instead of 0x34 */

static void wf_link(u32 *slot, u32 *p, u32 len) {
    p[0] = (*slot & 0xFFFFFF) | (len << 24);
    *slot = (u32)p & 0xFFFFFF;
}

/* Index of the largest of three depths, comparing by signed difference as the original does. */
static s32 wf_max3(u32 a, u32 b, u32 c) {
    if ((s32)(a - b) > 0) {
        return (s32)(a - c) > 0 ? 0 : 2;
    }
    return (s32)(b - c) > 0 ? 1 : 2;
}

static u32 *wf_face(u32 *face, u32 *pkt, u32 *ot, s32 flags, s32 limit) {
    u32 *sxyTab = (u32 *)SCRATCHPAD;
    u32 *alt = (u32 *)(SCRATCHPAD + 0x3D0);
    u32 w = face[0];
    s32 ix[3];
    u32 s[3];
    u32 z[3];
    u32 fg[3];
    u32 lt[3];
    u32 fogOr, litOr, code, mac0, otz, uv0, uv1, uv2, a, b, tp;
    u32 *p;
    u32 *slot;
    s32 k;

    ix[0] = w & 0xFF;
    ix[1] = (w >> 8) & 0xFF;
    ix[2] = (w >> 16) & 0xFF;
    s[0] = sxyTab[ix[0]];
    s[1] = sxyTab[ix[1]];
    s[2] = sxyTab[ix[2]];
    gte_ldsxy3(s[0], s[1], s[2]);
    gte_nclip();
    if ((flags & WF_CULL) && (s[0] & s[1] & s[2] & 0xFF008000)) {
        return pkt;
    }
    gte_stopz_r(mac0);
    if ((s32)mac0 <= 0) {
        return pkt;
    }

    if (flags & WF_FIXED_OT) {
        otz = 0xFA0;
    } else if (flags & WF_Z16) {
        z[0] = (s32)(s16)D_800D29F8[ix[0]] & ~0xF;
        z[1] = (s32)(s16)D_800D29F8[ix[1]] & ~0xF;
        z[2] = (s32)(s16)D_800D29F8[ix[2]] & ~0xF;
        otz = z[wf_max3(z[0], z[1], z[2])];
        if ((s32)otz <= 0 || (s32)otz >= limit) {
            return pkt;
        }
    } else {
        u32 bias = (w & 0x1000000) >> 15;
        u32 zg[3];

        for (k = 0; k < 3; k++) {
            z[k] = ((u32 *)D_800D29F8)[ix[k]] + bias;
            zg[k] = (z[k] >> 3) & ~0xF;
        }
        if (flags & WF_Z32_GRID) {
            k = wf_max3(zg[0], zg[1], zg[2]);
            if ((s32)zg[k] <= 0 || (s32)zg[k] >= limit) {
                return pkt;
            }
            otz = z[k] >> 3;
        } else {
            otz = z[wf_max3(z[0], z[1], z[2])] >> 3;
            if ((s32)otz <= 0 || (s32)otz >= limit) {
                return pkt;
            }
        }
    }

    for (k = 0; k < 3; k++) {
        fg[k] = D_800D1FF0[ix[k]];
        lt[k] = D_800D23F0[ix[k]];
    }
    fogOr = (fg[0] | fg[1] | fg[2]) & 0xFFFFFF;
    litOr = (lt[0] | lt[1] | lt[2]) & 0xFFFFFF;
    code = litOr ? 0x32000000 : 0x30000000;
    slot = ot + otz;

    if ((flags & WF_FOG) && fogOr) {
        p = pkt;
        p[2] = s[0];
        p[4] = s[1];
        p[6] = s[2];
        p[1] = (fg[0] & 0xFFFFFF) | code;
        p[3] = fg[1];
        p[5] = fg[2];
        wf_link(slot, p, 6);
        pkt += 7;
    }
    if ((flags & WF_NEED_LIT) && !litOr) {
        return pkt;
    }

    p = pkt;
    p[2] = s[0];
    p[5] = s[1];
    p[8] = s[2];
    p[1] = (lt[0] & 0xFFFFFF) | ((flags & WF_SEMI) ? 0x36000000 : 0x34000000);
    p[4] = lt[1];
    p[7] = lt[2];
    uv0 = face[1]; /* u0 v0 clut */
    uv1 = face[2]; /* u1 v1 tpage */
    uv2 = face[3]; /* u2 v2 flags */
    if ((flags & WF_ALT) || ((flags & WF_LOD) && (s32)otz < 0x9C4)) {
        if (uv2 & 0x20000) {
            a = alt[2];
            uv0 += a & 0xFFFF0000;
            uv1 = (uv1 & 0xFFFF) + (a << 16);
        }
    } else if (flags & WF_LOD) {
        if (uv2 & 0x20000) {
            a = alt[0];
            b = alt[1];
            tp = (a & 0xFFFF) << 16;
            a &= 0xFFFF0000;
            uv0 = ((((uv0 & 0xFEFE) >> 1) | (((uv0 & 0xFFFF0000) >> 6) + a)) + b);
            uv1 = (((uv1 & 0xFEFE) >> 1) | tp) + b;
            uv2 = ((uv2 & 0xFEFE) >> 1) + b;
        } else {
            uv0 = ((uv0 & 0xFEFE) >> 1) | ((uv0 & 0xFFFF0000) - 0x10000);
            uv1 = ((uv1 & 0xFEFE) >> 1) | (uv1 & 0xFE7F0000);
            uv2 = (uv2 & 0xFEFE) >> 1;
        }
    }
    p[3] = uv0;
    p[6] = uv1;
    p[9] = uv2;
    wf_link(slot, p, 9);
    return pkt + 10;
}

static s32 wf_mesh(u8 *o, s32 pkt, s32 ot, s32 flags, s32 limit) {
    u32 *alt = (u32 *)(SCRATCHPAD + 0x3D0);
    u32 *src = (u32 *)D_800D27F0;
    u32 *face = *(u32 **)(o + 8);
    u16 n = *(u16 *)(o + 2);
    u32 *p = (u32 *)pkt;

    alt[0] = src[0];
    alt[1] = src[1];
    alt[2] = src[2];
    do {
        p = wf_face(face, p, (u32 *)ot, flags, limit);
        face += 4;
    } while (--n != 0);
    return (s32)p;
}

/* Faces of a func_800B3A80 mesh (u16 depths unused): all into OT slot 0xFA0, POLY_G3 fog under every
 * POLY_GT3. */
s32 func_800B3FEC(s32 o, s32 pkt, s32 ot) {
    u32 *face = *(u32 **)((u8 *)o + 8);
    s32 n = *(s16 *)((u8 *)o + 2);
    u32 *p = (u32 *)pkt;

    do {
        p = wf_face(face, p, (u32 *)ot, WF_FIXED_OT | WF_FOG, 0);
        face += 4;
    } while (--n != 0);
    return (s32)p;
}

/* World-block faces after func_800B4270 (g_worldFog set). */
s32 func_800B4FD4(void *o, s32 pkt, s32 ot) {
    return wf_mesh(o, pkt, ot, WF_Z32 | WF_FOG | WF_NEED_LIT | WF_LOD, 0xFFE);
}

/* World-block faces after func_800B53FC (no fog, far block: view z outside -10000..11000). */
s32 func_800B5880(void *o, s32 pkt, s32 ot) {
    return wf_mesh(o, pkt, ot, WF_Z32_GRID | WF_FOG | WF_NEED_LIT | WF_LOD, 0xFFE);
}

/* World-block faces after func_800B6338 (no fog, near block): off-screen faces culled, no fog primitive. */
s32 func_800B6598(void *o, s32 pkt, s32 ot) {
    return wf_mesh(o, pkt, ot, WF_Z32 | WF_CULL | WF_LOD, 0xF6A);
}

/* Placed-model faces (func_800BA458 modes 0 and 2): semi-transparent POLY_GT3 only. ot is the caller's OT
 * already offset by its layer argument. */
s32 func_800B5CD0(s32 o, s32 pkt, s32 ot) {
    return wf_mesh((u8 *)o, pkt, ot, WF_Z16 | WF_NEED_LIT | WF_ALT | WF_SEMI, 0xF6A);
}

/* Placed-model faces, mode 1: as func_800B5CD0 plus the fog POLY_G3. */
s32 func_800B5FD0(s32 o, s32 pkt, s32 ot) {
    return wf_mesh((u8 *)o, pkt, ot, WF_Z16 | WF_FOG | WF_NEED_LIT | WF_ALT | WF_SEMI, 0xF6A);
}
