/* ovl_0c func_800CB1EC: the world map's copy of ovl_0a func_800C9AB8, with two extra context fields: +0x24 is
 * a size shift (half sizes << shift, texel span = half size >> (shift + 1)) and +0x28 an OT bias (slot = (SZ3 >> 3)
 * + bias; quads whose slot is >= 0x1000 are not linked, but their packet space is still used).
 *
 * Field version: (hand-written asm in the original, standard C ABI).
 *
 * src: a->unk1C entries of { s8 x, s8 y, u8 k } (x and y in units of 4). Each becomes a 10-word textured quad
 * (POLY_FT4 layout) of half size (a->unk8, a->unkC) around (x, y, 0), projected with RTPT + RTPS. k & 0xF
 * picks the u,v from the u16 table a->unk0 (the quad covers half the half-size in texels), k >> 4 picks the
 * colour from the 4-byte table a->unk4. If a->unk18 >= 0 the colour is scaled by unk18 / 4096 (GPF; the
 * result's low byte per channel is kept, as in the original). a->unk10 / unk14 are the texpage / CLUT and
 * a->unk20 the command byte. Each quad goes into ot[SZ3 >> 2].
 * If MAC0 is 0 after the RTPT, drawing stops there (that quad is left unlinked). Returns the next packet. */
#include "ovl_0c.h"

static u8 *draw(u8 *src, WmSprCtx *a, u8 *pkt, u32 *ot) {
    s32 sh = a->unk24;
    s32 bias = a->unk28;
    s32 hw = (u32)a->unk8 << sh;
    s32 hh = (u32)a->unkC << sh;
    u32 tpage = (u32)a->unk10 << 16;
    u32 clut = (u32)a->unk14 << 16;
    s32 mod = a->unk18;
    s32 n = a->unk1C;
    u32 code = (u32)a->unk20 << 24;
    SVECTOR v[4];
    SVECTOR ir;
    u32 *p;
    u32 *slot;
    s32 x, y, k;
    u32 uv, u, vv, ur, vb, c, mac0, sz3, e;
    u8 *ce;
    u32 z;

    if (n == 0) {
        return pkt;
    }
    do {
        p = (u32 *)pkt;
        x = (s8)src[0] * 4;
        y = (s8)src[1] * 4;
        v[0].vx = x - hw; v[0].vy = y - hh; v[0].vz = 0;
        v[1].vx = x + hw; v[1].vy = y - hh; v[1].vz = 0;
        v[2].vx = x - hw; v[2].vy = y + hh; v[2].vz = 0;
        v[3].vx = x + hw; v[3].vy = y + hh; v[3].vz = 0;
        gte_ldv3(&v[0], &v[1], &v[2]);
        gte_rtpt();
        k = src[2];
        src += 3;
        uv = a->unk0[k & 0xF];
        u = uv & 0xFF;
        vv = uv >> 8;
        ur = u + ((u32)hw >> (sh + 1));
        vb = vv + ((u32)hh >> (sh + 1));
        p[3] = u | (vv << 8) | clut;
        p[5] = ur | (vv << 8) | tpage;
        p[7] = u | (vb << 8);
        gte_stsxy3(&p[2], &p[4], &p[6]);
        p[9] = ur | (vb << 8);
        gte_stopz(&mac0);
        if (mac0 == 0) {
            break;
        }
        gte_stsz(&sz3);
        gte_ldv0(&v[3]);
        gte_rtps();
        gte_stsxy(&p[8]);
        ce = (u8 *)a->unk4 + (k >> 4) * 4;
        c = *(u16 *)ce | ((u32)ce[2] << 16);
        if (mod >= 0) {
            gte_ldir0123(mod, c & 0xFF, (c >> 8) & 0xFF, (c >> 16) & 0xFF);
            gte_gpf12();
            gte_stsv(&ir);
            c = (ir.vx & 0xFF) | ((ir.vy & 0xFF) << 8) | ((ir.vz & 0xFF) << 16);
        }
        p[1] = c | code;
        z = ((s32)sz3 >> 3) + bias;
        if (z < 0x1000) {
            slot = &ot[z];
            e = *slot;
            p[0] = (e & 0xFFFFFF) | 0x09000000;
            *slot = (e & 0xFF000000) | ((u32)p & 0xFFFFFF);
        }
        pkt += 0x28;
    } while (--n != 0);
    return pkt;
}

/* Prototype as in ovl_0c.h (packet and OT passed as s32). */
s32 func_800CB1EC(u8 *src, WmSprCtx *a, s32 pkt, s32 ot) {
    return (s32)draw(src, a, (u8 *)pkt, (u32 *)ot);
}
