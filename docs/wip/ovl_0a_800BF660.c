/*
 * ovl_0a func_800BF660 (0x688 bytes): field camera follow. Projects the player actor
 * (D_8006794C mode 5: battle-scene actor, mode 1: func_8004AE00()) with the field camera
 * matrix, averages the screen position into D_800CA068.unk2C/2E, clamps the scroll into the
 * camera's unk28..unk2E limits, updates D_800CA068.unkC/unkE/unk4 and scrolls the layers by the
 * delta (same layer loop as func_800BF0C4, here with the deltas in stack shorts dx/dy).
 * WIP: 418 of 418 instructions, 39 aligned diff rows (build/s/cmp.py). The layer loop, the
 * entry, the projection and the stack frame match. Left:
 *   - register choice in the clamp block: the original has x in a0, y in a2 and the
 *     D_800CA068 base in a1; ours has x a1 / base a0 (global-alloc order);
 *   - `wx - (lim - 160)` is reassociated by fold into (wx + 160) - lim; the original subtracts
 *     a (lim - 160) temp (filled into both branch delay slots by reorg). The y half wants
 *     A - (lim - 112) computed after A. Temps in fresh variables did not help;
 *   - the y offset is reached through `lim = A - 112; y += lim` (45 -> 39 rows), probably wrong.
 * Frame layout notes: sxy must be a 4-byte non-BLKmode object (union {s32; DVECTOR}) so it
 * lands right after ofx/ofy; dx/dy are separate addressable shorts (an s16[2] or DVECTOR is
 * BLKmode and gets 8-byte slots). `y = sxy.w >> 16` gives the lh 0x22(sp); sxy.v.vy gives
 * lhu off the asm address register plus a sign extension.
 */
#include "ovl_0a.h"

s32 func_800BF660(void) {
    SVECTOR sv;
    long ofx, ofy;
    union {
        s32 w;
        DVECTOR v;
    } sxy;
    s16 dx, dy;
    s16 *px, *py;
    Unk800C9DB4 *h;
    Unk800C9DB4Cam *c;
    Unk8004AB64 *a;
    s32 x;
    s32 y;
    s32 cx;
    s32 sx;
    s32 sy;
    s32 lim;
    s32 wx;
    s32 ox;
    s32 oy;
    u8 mode;
    s32 flags;
    s32 i;
    Unk800C9DB4Obj *o;
    s32 s;
    s32 v;
    flags = D_800CA068.unk0;
    if (flags & 7) {
        return 0;
    }
    if (!(flags & 0x40)) {
        return 0;
    }
    mode = D_8006794C->unk8;
    if (mode == 5) {
        a = (Unk8004AB64 *)D_8006794C->unk1C->unk8E0[D_8006794C->unk1C->unk8E4.field->unk8].unkC;
    } else if (mode == 1) {
        a = func_8004AE00();
    } else {
        a = NULL;
    }
    h = D_800C9DB4;
    c = (Unk800C9DB4Cam *)((u8 *)h + h->unk18);
    c += D_800CA068.unk75;
    if (a != NULL) {
        sv.vx = a->unk14.vx;
        sv.vy = a->unk14.vy - D_800CA068.unk76;
        sv.vz = a->unk14.vz;
    } else {
        sv.vx = 0;
        sv.vy = 0;
        sv.vz = 0;
    }
    ReadGeomOffset(&ofx, &ofy);
    gte_SetGeomOffset(0xA0, 0x70);
    gte_SetRotMatrix(D_800CA068.unk1C);
    gte_SetTransMatrix(D_800CA068.unk1C);
    gte_SetGeomScreen(*D_800CA068.unk20);
    gte_ldv0(&sv);
    gte_rtps();
    gte_stsxy(&sxy);
    gte_SetGeomOffset(ofx, ofy);
    x = sxy.v.vx;
    y = sxy.w >> 16;
    if (D_800CA068.unk0 & 0x80) {
        D_800CA068.unk2C = x;
        D_800CA068.unk2E = y;
    }
    x = (D_800CA068.unk2C + x) >> 1;
    y = (D_800CA068.unk2E + y) >> 1;
    sx = 320 - x;
    sy = 224 - y;
    D_800CA068.unk2C = x;
    D_800CA068.unk2E = y;
    wx = (c->unk24 >> 1) + (s16)c->unk20;
    cx = wx - 160;
    cx += x;
    lim = (c->unk26 >> 1) + (s16)c->unk22 - 112;
    y += lim;
    if (cx < (lim = c->unk28) || (lim = c->unk2A) < cx) {
        sx = wx - (lim - 160);
    }
    if (y < (lim = c->unk2C) || (lim = c->unk2E) < y) {
        sy = (c->unk26 >> 1) + (s16)c->unk22 - (lim - 112);
    }
    D_800CA068.unkC = sx - c->unk20;
    D_800CA068.unkE = sy - c->unk22;
    px = &dx;
    py = &dy;
    if (D_800CA068.unk0 & 0x40) {
        ox = D_800CA068.unk4.unk0;
        oy = D_800CA068.unk4.unk2;
        if ((s16)cx < c->unk28) {
            D_800CA068.unk4.unk0 = c->unk28;
        } else if (c->unk2A < (s16)cx) {
            D_800CA068.unk4.unk0 = c->unk2A;
        } else {
            D_800CA068.unk4.unk0 = cx;
        }
        if ((s16)y < c->unk2C) {
            D_800CA068.unk4.unk2 = c->unk2C;
        } else if (c->unk2E < (s16)y) {
            D_800CA068.unk4.unk2 = c->unk2E;
        } else {
            D_800CA068.unk4.unk2 = y;
        }
        *px = D_800CA068.unk4.unk0 - ox;
        *py = D_800CA068.unk4.unk2 - oy;
    }
    o = (Unk800C9DB4Obj *)((u8 *)h + h->unk10);
    for (i = 0; i < h->unk6; o++, i++) {
        if (o->unk0 & 4) {
            if (o->unk1C != 0) {
                o->unkC += dx;
            }
            if (o->unk1E != 0) {
                o->unkE += dy;
            }
        s = D_800CA068.unkC8[o->unk25_1] << 8;
        v = dx << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->unkC << 16) - func_800C1DC4(v, -s);
            } else {
                v = (o->unkC << 16) + func_800C1DC4(v, s);
            }
            o->unkC = v >> 16;
            o->unk20 = (v >> 8) & 0xFF;
        }
        s = D_800CA068.unkD0[o->unk25_1] << 8;
        v = dy << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->unkE << 16) - func_800C1DC4(v, -s);
            } else {
                v = (o->unkE << 16) + func_800C1DC4(v, s);
            }
            o->unkE = v >> 16;
            o->unk22 = (v >> 8) & 0xFF;
        }
        } else if (o->unk0 & 0x80) {
            if (o->unk25) {
                o->unkE += dy;
            } else {
                o->unkC += dx;
            }
        s = D_800CA068.unkC8[o->unk25_1] << 8;
        v = dx << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->unkC << 16) - func_800C1DC4(v, -s);
            } else {
                v = (o->unkC << 16) + func_800C1DC4(v, s);
            }
            o->unkC = v >> 16;
            o->unk20 = (v >> 8) & 0xFF;
        }
        s = D_800CA068.unkD0[o->unk25_1] << 8;
        v = dy << 16;
        if (s != 0x10000) {
            if (s < 0) {
                v = (o->unkE << 16) - func_800C1DC4(v, -s);
                o->unkE = v >> 16;
                o->unk22 = (v >> 8) & 0xFF;
            } else {
                v = (o->unkE << 16) + func_800C1DC4(v, s);
                o->unkE = v >> 16;
                o->unk22 = (v >> 8) & 0xFF;
            }
        }
        }
    }
    return 1;
}
