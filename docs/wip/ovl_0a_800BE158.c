/*
 * ovl_0a func_800BE158 (0xC90 bytes): adds one background layer's tile sprites (BgSprt,
 * DR_TPAGE + SPRT_16) to the OT: flag 4 = scrolling layer (wraps at unk4/unk6, per-group
 * offsets D_800CA068.unkA8/unkB8), flag 0x80 = one-axis parallax (unk25 picks the axis),
 * otherwise a static layer clipped to the screen. Depth = header unk1E + layer depth + tile
 * depth, drawn when 8 <= d < 0xFF9.
 * WIP (pass 9, first draft): 809 vs 804 instructions, 194 aligned diff rows (build/s/cmp.py).
 * The three paths, their order and the per-tile tests match; left is register allocation:
 * the original keeps n (tile count) in t9, ox/oy in t8/t7, i in t0 and spills only ox + wx
 * (0x0(sp)) in the 0x80 path; here n (priority 0.057) loses to the hoisted sums and lands in
 * s0, which costs a ninth saved register (frame 0x30 vs 0x28) and the flag-4 path spill.
 * The permuter got 164 rows by merging tx and ty into one variable, but that changes the
 * result where both are stored at the end, so it is not used; merging them only where x0 is
 * stored before ty is computed is untried.
 * Found so far: unk1C/unk1E are read once into s32 locals (sx/sy) and the negative-scroll
 * terms are `k = 0x100 - sx; ... + k` (note: the y line also uses sx, as in the original).
 * The header Unk800C9DA4Hdr gained unk1E (base depth) for this.
 */
#include "ovl_0a.h"

s32 func_800BE158(Unk800C9DB4Obj *o) {
    BgSprt *spr;
    u32 *ot;
    Unk800C9DA4Hdr *h;
    BgPos *pos;
    u32 n;
    s16 i;
    s16 x;
    s16 y;
    s16 z;
    s16 tx;
    s16 ty;
    s16 d;
    s16 k;
    s32 sx;
    s32 sy;
    u16 ox;
    u16 oy;
    u16 wx;
    u16 wy;
    u8 buf;

    spr = o->unk30;
    buf = *D_800C9DA4.unk0;
    n = o->unk26;
    ot = D_800C9DA4.unk8[buf];
    if (buf != 0) {
        spr += n;
    }
    h = D_800C9DA4.unk10;
    x = o->unkC + h->unk30;
    y = o->unkE + h->unk32;
    z = h->unk1E + o->unk1;
    pos = (BgPos *)((u8 *)h + o->unk28);
    if (o->unk0 & 4) {
        if (o->unk0 & 1) {
            ox = D_800CA068.unkA8[o->unk25_1][0];
            oy = D_800CA068.unkA8[o->unk25_1][1];
        } else {
            ox = D_800CA068.unkA8[o->unk25_1][0] + 0xA0 - D_800CA068.unk4.unk0;
            oy = D_800CA068.unkA8[o->unk25_1][1] + 0x70 - D_800CA068.unk4.unk2;
        }
        wx = D_800CA068.unkB8[o->unk25_1][0];
        wy = D_800CA068.unkB8[o->unk25_1][1];
        sx = o->unk1C;
        if (sx < 0) {
            k = 0x100 - sx;
            x = h->unk30 + ((((o->unkC << 8) | o->unk20) + k) >> 8);
        }
        sy = o->unk1E;
        if (sy < 0) {
            k = 0x100 - sx;
            y = h->unk32 + ((((o->unkE << 8) | o->unk22) + k) >> 8);
        }
        if (sx != 0) {
            x = (x - ((s16)wx - o->unk4)) % o->unk4 + (wx - o->unk4);
        }
        if (sy != 0) {
            y = (y - ((s16)wy - o->unk6)) % o->unk6 + (wy - o->unk6);
        }
        for (i = 0; i < n; i++) {
            if (o->unk0 & 1) {
                tx = x + pos[i].x;
                if (o->unk1C != 0) {
                    if (tx + 16 >= o->unk4) {
                        tx -= o->unk4;
                    } else if (tx < -15) {
                        tx += o->unk4;
                    }
                }
                if (tx + (s16)ox < -15 || (s16)wx < tx) {
                    continue;
                }
                ty = y + pos[i].y;
                if (o->unk1E != 0) {
                    if (ty + 16 >= o->unk6) {
                        ty -= o->unk6;
                    } else if (ty < -15) {
                        ty += o->unk6;
                    }
                }
                if (ty + (s16)oy < -15 || ty >= (s16)wy) {
                    continue;
                }
                d = z + pos[i].pad;
                if (d < 8 || d >= 0xFF9) {
                    continue;
                }
                spr[i].unk8.x0 = tx + ox;
                spr[i].unk8.y0 = ty + oy;
            } else {
                tx = x + pos[i].x;
                if (o->unk1C != 0) {
                    if (tx + 16 >= o->unk4) {
                        tx -= o->unk4;
                    } else if (tx < -15) {
                        tx += o->unk4;
                    }
                    if (tx + (s16)ox < -15 || tx >= (s16)wx) {
                        continue;
                    }
                    spr[i].unk8.x0 = tx + ox;
                } else {
                    if (tx < -15 || tx >= (s16)ox + (s16)wx) {
                        continue;
                    }
                    spr[i].unk8.x0 = tx;
                }
                ty = y + pos[i].y;
                if (o->unk1E != 0) {
                    if (ty + 16 >= o->unk6) {
                        ty -= o->unk6;
                    } else if (ty < -15) {
                        ty += o->unk6;
                    }
                    if (ty + (s16)oy < -15 || ty >= (s16)wy) {
                        continue;
                    }
                    spr[i].unk8.y0 = ty + oy;
                } else {
                    if (ty < -15 || ty >= (s16)oy + (s16)wy) {
                        continue;
                    }
                    spr[i].unk8.y0 = ty;
                }
                d = z + pos[i].pad;
                if (d < 8 || d >= 0xFF9) {
                    continue;
                }
            }
            addPrim(ot + d, &spr[i]);
        }
    } else if (o->unk0 & 0x80) {
        if (o->unk0 & 1) {
            ox = D_800CA068.unkA8[o->unk25_1][0];
            oy = D_800CA068.unkA8[o->unk25_1][1];
        } else {
            ox = D_800CA068.unkA8[o->unk25_1][0] + 0xA0 - D_800CA068.unk4.unk0;
            oy = D_800CA068.unkA8[o->unk25_1][1] + 0x70 - D_800CA068.unk4.unk2;
        }
        wx = D_800CA068.unkB8[o->unk25_1][0];
        wy = D_800CA068.unkB8[o->unk25_1][1];
        if (o->unk25) {
            y = (y - ((s16)wy - o->unk6)) % o->unk6 + (wy - o->unk6);
            x += y * o->unk1C / o->unk6 % o->unk4;
        } else {
            x = (x - ((s16)wx - o->unk4)) % o->unk4 + (wx - o->unk4);
            y += x * o->unk1E / o->unk4 % o->unk6;
        }
        for (i = 0; i < n; i++) {
            if (o->unk25) {
                k = 0;
                ty = y + pos[i].y;
                if (ty + 16 >= o->unk6) {
                    ty -= o->unk6;
                    k = -o->unk1C;
                } else if (ty < -15) {
                    ty += o->unk6;
                    k = o->unk1C;
                }
                if (ty + (s16)oy < -15 || ty >= (s16)wy) {
                    continue;
                }
                tx = k + (x + pos[i].x);
                if (tx < -15 || tx >= (s16)ox + (s16)wx) {
                    continue;
                }
                spr[i].unk8.x0 = tx;
                spr[i].unk8.y0 = ty + oy;
            } else {
                k = 0;
                tx = x + pos[i].x;
                if (tx + 16 >= o->unk4) {
                    tx -= o->unk4;
                    k = -o->unk1E;
                } else if (tx < -15) {
                    tx += o->unk4;
                    k = o->unk1E;
                }
                if (tx + (s16)ox < -15 || tx >= (s16)wx) {
                    continue;
                }
                ty = k + (y + pos[i].y);
                if (ty < -15 || ty >= (s16)oy + (s16)wy) {
                    continue;
                }
                spr[i].unk8.x0 = tx + ox;
                spr[i].unk8.y0 = ty;
            }
            d = z + pos[i].pad;
            if (d < 8 || d >= 0xFF9) {
                continue;
            }
            addPrim(ot + d, &spr[i]);
        }
    } else {
        for (i = 0; i < n; i++) {
            tx = x + pos[i].x;
            if (tx < -15 || tx >= 320) {
                continue;
            }
            ty = y + pos[i].y;
            if (ty < -15 || ty >= 224) {
                continue;
            }
            d = z + pos[i].pad;
            if (d < 8 || d >= 0xFF9) {
                continue;
            }
            spr[i].unk8.y0 = ty;
            spr[i].unk8.x0 = tx;
            addPrim(ot + d, &spr[i]);
        }
    }
    o->unk18 = x;
    o->unk1A = y;
    return 1;
}
