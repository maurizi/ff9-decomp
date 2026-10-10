/* First draft, about 110 rows differ: the original reloads D_8007BD14 in the inner loop (after the xy0 store) and uses two vertex givs (t4 for x0, t1 = t4 + 0x50 for the rest); here the D_8007BD14 load is hoisted out of the inner loop. */
#include "ff9.h"

extern DVECTOR *D_8007BD14;
extern SVECTOR *D_8007BD18;

void func_80054650(void) {
    PolyFT4Words *p;
    PolyFT4Words *q;
    PolyFT4Words *prev;
    SVECTOR *v;
    DVECTOR *a;
    DVECTOR *b;
    s32 i;
    s32 j;
    s32 c;
    u32 rgbc;
    s16 x;
    u8 u0;
    u8 u1;
    u32 tp;

    c = (D_8007BD1C >> 4) & 0xFF;
    rgbc = (c << 16) | (c << 8) | c | 0x2E000000;
    p = (PolyFT4Words *)D_8007BD10 + 1;
    for (i = 0; i < 8; i++) {
        prev = p - 1;
        v = &D_8007BD18[i * 9];
        a = &D_8007BD14[i];
        x = a->vx;
        u0 = a->vx & 0x3F;
        u1 = (a->vx & 0x3F) + (a[1].vx - x) - 1;
        tp = ((((x + D_8006794C->unkC[D_8006794C->unk1C->unk8].disp.x) & 0x3FF) >> 6) | 0x100) << 16;
        for (j = 0; j < 8; j++) {
            q = p++;
            q->tag = 0x09000000;
            q->rgbc = rgbc;
            q->xy0 = (v[j].vy << 16) | (u16)v[j].vx;
            b = &D_8007BD14[j];
            q->uv0clut = (b[0].vy << 8) | u0;
            q->xy1 = (v[j + 9].vy << 16) | (u16)v[j + 9].vx;
            q->uv1tpage = (b[0].vy << 8) | u1 | tp;
            q->xy2 = (v[j + 1].vy << 16) | (u16)v[j + 1].vx;
            q->uv2 = ((b[1].vy - 1) << 8) | u0;
            q->xy3 = (v[j + 10].vy << 16) | (u16)v[j + 10].vx;
            q->uv3 = ((b[1].vy - 1) << 8) | u1;
            setaddr(prev, q);
            prev = q;
        }
    }
    p[-1].tag = (p[-1].tag & 0xFF000000) | (D_8007BD0C[1] & 0xFFFFFF);
    D_8007BD0C[1] = (D_8007BD0C[1] & 0xFF000000) | ((u32)(D_8007BD10 + 10) & 0xFFFFFF);
    D_8007BD10 = (u32 *)p;
}
