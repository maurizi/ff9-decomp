#include "ovl_0b.h"

/* scrolling/waving 2D background layer (func_800D3030) */
typedef struct {
    u16 unk0; /* kind: 0 scroll, 1 horizontal wave, 2 fade, 3 vertical wave */
    u16 unk2; /* tpage */
    u16 unk4; /* clut */
    u8 unk6;  /* u */
    u8 unk7;  /* v */
    u16 unk8; /* abr */
    u16 unkA; /* palette frames */
    s32 unkC; /* palette animation offset */
    union {
        struct BtlBgScrollP {
            u8 unk0; /* x: sine motion */
            u8 unk1; /* y: sine motion */
            u8 pad2[2];
            s16 unk4; /* x speed */
            s16 unk6; /* y speed */
            u16 unk8; /* x amplitude */
            u16 unkA; /* y amplitude */
        } scroll;
        struct BtlBgWaveP {
            u16 unk0; /* strip size */
            u16 unk2; /* strip count */
            u16 unk4; /* phase step */
            u16 unk6; /* phase speed */
            u16 unk8; /* amplitude */
            s16 unkA; /* scroll speed */
        } wave;
        struct BtlBgFadeP {
            s32 unk0; /* data offset */
            u8 pad4;
            u8 unk5;  /* delay */
            u16 unk6; /* duration */
            s16 unk8;
            s16 unkA;
        } fade;
    } unk10;
} BtlBgLayer;

typedef struct {
    u16 unk0; /* layer count */
    u8 unk2;  /* width */
    u8 unk3;  /* height */
    u16 unk4; /* x */
    u16 unk6; /* y */
    s32 unk8[1]; /* layer offsets */
} BtlBgLayers;

void func_800E651C(u8 *, CVECTOR *, s32);
void func_800D2DBC(u8 *, s32, s32, s32, s32, s32);

#define WRAP2(a, n)          \
    {                       \
        (a) = (a) / (n);    \
        if (wt < 0) {       \
            (a)--;          \
        }                   \
        (a) *= (n);         \
        (a) = wt - (a);     \
    }
#define WRAP(a, n)          \
    {                       \
        s32 _t = (a);       \
        (a) = (a) / (n);    \
        if (_t < 0) {       \
            (a)--;          \
        }                   \
        (a) *= (n);         \
        (a) = _t - (a);     \
    }

void func_800D3030(BtlBgLayers *arg0, s32 arg1) {
    RECT r;
    s16 ofs[2];
    CVECTOR col;
    DR_TPAGE *tp;
    DR_AREA *da;
    DR_OFFSET *dofs;
    s32 *offs;
    s32 i;
    s32 n;
    s32 w;
    s32 h;
    s32 step;
    s32 x, y;
    BtlBgLayer *L;
    s32 u, v;
    s32 a;
    s32 k;
    s32 j;
    s32 tw;
    s32 cnt;
    s32 wt;
    TILE *t;
    struct BtlBgScrollP *sc;
    struct BtlBgWaveP *wv;
    struct BtlBgFadeP *fd;

    da = (DR_AREA *)D_800F78A4->unk24;
    n = arg0->unk0;
    w = arg0->unk2;
    h = arg0->unk3;
    x = arg0->unk4;
    y = arg0->unk6;
    D_800F78A4->unk24 = (u8 *)(da + 2);
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SetDrawArea(da, &r);
    ((XY32 *)&r)[0].w = ((XY32 *)&D_800F78A4->unk1C->clip)[0].w;
    ((XY32 *)&r)[1].w = ((XY32 *)&D_800F78A4->unk1C->clip)[1].w;
    SetDrawArea(&da[1], &r);
    i = 0;
    dofs = (DR_OFFSET *)D_800F78A4->unk24;
    D_800F78A4->unk24 = (u8 *)(dofs + 2);
    ofs[0] = x;
    ofs[1] = y;
    SetDrawOffset(dofs, ofs);
    SetDrawOffset(&dofs[1], D_800F78A4->unk1C->ofs);
    tp = (DR_TPAGE *)D_800F78A4->unk24;
    D_800F78A4->unk24 = (u8 *)(tp + 2);
    tp[0].code[0] = 0xE1000000;
    tp[1].code[0] = 0xE1000200;
    func_800D0858(0x01000000, 0xFFE, &tp[1]);
    func_800D0858(0x02000000, 0xFFE, &da[1]);
    func_800D0858(0x02000000, 0xFFE, &dofs[1]);
    offs = arg0->unk8;
    for (; i < n; i++) {
        L = (BtlBgLayer *)((u8 *)arg0 + offs[i]);
        if (L->unkC != 0) {
            func_800E651C((u8 *)arg0 + L->unkC, &col, arg1 % L->unkA);
        } else {
            *(s32 *)&col = 0x808080;
        }
        switch (L->unk0) {
        case 0:
            sc = &L->unk10.scroll;
            if (sc->unk0 == 0) {
                wt = u = arg1 * sc->unk4;
            } else {
                wt = u = (func_8004BB80(sc->unk4 * arg1) * sc->unk8) >> 12;
            }
            WRAP2(u, w);
            if (sc->unk1 == 0) {
                wt = v = arg1 * sc->unk6;
            } else {
                wt = v = (func_8004BB80(sc->unk6 * arg1) * sc->unkA) >> 12;
            }
            WRAP2(v, h);
            func_800D2C78((SprInfo *)L, u, u, v, v, L->unk6, L->unk7, w, h, &col, 1, 1);
            if (u != 0) {
                func_800D2C78((SprInfo *)L, u - w, u - w, v, v, L->unk6, L->unk7, w, h, &col, 0, 1);
            }
            if (v != 0) {
                func_800D2C78((SprInfo *)L, u, u, v - h, v - h, L->unk6, L->unk7, w, h, &col, 1, 0);
            }
            if (u != 0 && v != 0) {
                func_800D2C78((SprInfo *)L, u - w, u - w, v - h, v - h, L->unk6, L->unk7, w, h, &col, 0, 0);
            }
            break;
        case 1:
            wv = &L->unk10.wave;
            v = arg1 * wv->unkA;
            WRAP(v, h);
            cnt = 1;
            if (v != 0) {
                cnt = 2;
            }
            step = wv->unk4;
            tw = wv->unk0;
            a = arg1 * wv->unk6 + v * step / tw;
            do {
                k = 0;
                for (j = 0; j < wv->unk2; j++) {
                    u = ((func_8004BB80(a) * wv->unk8) >> 12) - (w >> 1);
                    a += step;
                    func_800D2C78((SprInfo *)L, u, ((func_8004BB80(a) * wv->unk8) >> 12) - (w >> 1), v, v, L->unk6, L->unk7 + k, w * 2, tw, &col, 0, 0);
                    v += tw;
                    k += tw;
                }
                cnt--;
                v -= h * 2;
                a -= step * wv->unk2 * 2;
            } while (cnt != 0);
            break;
        case 2:
            fd = &L->unk10.fade;
            if (arg1 >= fd->unk5) {
                s32 e, f;
                e = arg1 - fd->unk5;
                if (e < fd->unk6) {
                    f = (e << 12) / fd->unk6;
                } else {
                    f = 0x1000;
                }
                if (fd->unk0 != 0) {
                    func_800D2DBC((u8 *)arg0 + fd->unk0 + 4, fd->unk8, fd->unkA, e, L->unk8, f);
                }
            }
            break;
        case 3:
            wv = &L->unk10.wave;
            u = arg1 * wv->unkA;
            WRAP(u, w);
            cnt = 1;
            if (u != 0) {
                cnt = 2;
            }
            step = wv->unk4;
            tw = wv->unk0;
            a = arg1 * wv->unk6 + u * step / tw;
            do {
                k = 0;
                for (j = 0; j < wv->unk2; j++) {
                    v = ((func_8004BB80(a) * wv->unk8) >> 12) - (h >> 1);
                    a += step;
                    func_800D2C78((SprInfo *)L, u, u, v, ((func_8004BB80(a) * wv->unk8) >> 12) - (h >> 1), L->unk6 + k, L->unk7, tw, h * 2, &col, 0, 0);
                    u += tw;
                    k += tw;
                }
                cnt--;
                u -= w * 2;
                a -= step * wv->unk2 * 2;
            } while (cnt != 0);
            break;
        }
    }
    t = (TILE *)D_800F78A4->unk24;
    D_800F78A4->unk24 = (u8 *)(t + 1);
    *(s32 *)&t->r0 = 0;
    t->code = 0x60;
    t->y0 = 0;
    t->x0 = 0;
    t->w = w;
    t->h = h;
    func_800D0858(0x03000000, 0xFFE, t);
    func_800D0858(0x01000000, 0xFFE, tp);
    func_800D0858(0x02000000, 0xFFE, da);
    func_800D0858(0x02000000, 0xFFE, dofs);
}
