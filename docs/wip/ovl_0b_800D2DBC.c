/* WIP func_800D2DBC: 63 differing rows (build/cmp.py-style aligned diff). New in pass 8 (sprite set drawn as SPRTs plus a DR_MODE; same header as func_800D1D74). Structure is right: col/mode/ctab/clut must share one stack struct (its address goes to the GTE asm, so the original keeps mode/ctab/clut at sp+0x18..0x20 and reloads them in the loop), header fields read before arg0 advances, wv/hv are ints, prim walked as SPRT * with the rgb store through &r0. Left: register allocation in the header block (frame copied to t2 at entry, n in v1) and the loop temporaries; 10 min of permuter only found invalid rewrites. */
#include "ovl_0b.h"

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u8 unk6;
    u8 unk7;
    u16 unk8[1];
} Unk800D1D74Hdr;

typedef struct {
    u32 rgbc;
    s16 x;
    s16 y;
    u32 uvclut;
    s16 w;
    s16 h;
} SprtBody;

void func_800D2DBC(u8 *arg0, s32 x, s32 y, s32 frame, s32 tp, s32 p) {
    struct {
        u16 rg;
        u8 b;
        u8 code;
        s32 pad4;
        u32 *mode;
        u8 *ctab;
        s32 clut;
    } col;
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)arg0;
    s32 n;
    u16 *q;
    u16 *pal;
    s32 off;
    s32 cnt;
    s32 wv;
    s32 hv;
    s32 num;
    u8 *prim;
    SprtBody *s;
    s32 i;
    u8 *c;
    s32 b;
    s32 tpage;
    s32 f;
    s32 clut;

    n = h->unk0 & 0x7FFF;
    f = frame;
    if (f >= n) {
        f %= n;
    }
    q = (u16 *)(arg0 + (n * 2 + 8));
    off = h->unk8[f];
    pal = q + 1;
    cnt = *q;
    tpage = h->unk2;
    clut = h->unk4;
    hv = h->unk6 - 1;
    wv = h->unk7 - 1;
    num = arg0[off];
    arg0 += off + 1;
    clut <<= 16;
    prim = D_800F78A4->unk24;
    col.mode = (u32 *)(prim + num * 20);
    D_800F78A4->unk24 = (u8 *)(col.mode + 2);
    tpage |= ((tp & 3) << 5) | 0xE1000000;
    col.mode[1] = tpage;
    col.ctab = (u8 *)(pal + cnt + 1);
    col.clut = clut;
    col.code = (tp != 0xFF) ? 0x66 : 0x64;
    for (i = 0; i < num; i++) {
        ((SPRT *)prim)->x0 = (s8)arg0[0] + x - (wv >> 1);
        ((SPRT *)prim)->y0 = (s8)arg0[1] + y - (hv >> 1);
        b = arg0[2];
        *(u32 *)&((SPRT *)prim)->u0 = pal[b & 0xF] | col.clut;
        c = col.ctab + ((b >> 2) & 0x3C);
        arg0 += 3;
        col.rg = *(u16 *)c;
        col.b = c[2];
        gte_DpqColorFar_nst(&col, p);
        ((SPRT *)prim)->w = wv;
        ((SPRT *)prim)->h = hv;
        gte_strgb(&((SPRT *)prim)->r0);
        func_800D0858(0x04000000, 0xFFE, prim);
        prim += 20;
    }
    func_800D0858(0x01000000, 0xFFE, col.mode);
}
