/*
 * ovl_0a func_800BF0C4 (0x59C bytes): field camera scroll step. Moves D_800CA068.unk4 from
 * unk10 towards unk14 over unk1A frames (linear, or cosine-eased when flag 8 is set), then
 * scrolls the background layers by the delta: layers with flag 4 scroll on both axes, flag 0x80
 * layers on one axis (unk25 bit), each scaled by the per-group factors D_800CA068.unkC8/unkD0
 * (func_800C1DC4 = 16.16 multiply). Shares the layer loop with func_800BF660.
 * WIP: 359 of 359 instructions, 1 aligned diff row (build/s/cmp.py):
 *   ours  addu a1,a1,s4   theirs  addu a1,s4,a1   (the h + h->unk18 add of the camera pointer)
 * What got it here (from ~200 rows):
 *   - the D0 (y) half of the flag-0x80 path has the stores duplicated in both sign branches;
 *     that is what makes cross-jumping merge exactly the original's tails;
 *   - ox/oy are u16, reused as the deltas (`ox = unk4.unk0 - ox`); the x delta is shifted as
 *     `(s16)ox << 16` inside the loop (loop.c hoists the sign extension into the preheader);
 *     in the flag-4 path that shift is a separate `dx = (s16)ox << 16` after the two adds;
 *   - the cosine temp is `y`, the 160 constant lives in `v` (global pseudo, so it is not
 *     rematerialised at its use), and the camera pointer is built in `s`.
 * What is left: the camera pointer has to be one pseudo from the h->unk18 load on and live in
 * a1 while 160 is in a0. A local `Unk800C9DB4Cam *c` gets a0 from local-alloc before the global
 * `v` is placed, so the only working shape found reuses the int `s` (global, a1). But then
 * `s = (s32)h + s` is swapped by expand_binop (target == op1), giving (plus s h). The pointer
 * form (h + off) has the right operand order but a separate temp. Not tried: an extra global
 * pointer variable that is live elsewhere without crossing a call.
 * Unnatural bits to clean up once it matches: the int `s` used as the camera pointer and the
 * `*(s16 *)&D_800CA068.unk4.unkN` reads (lh; the struct has u16 fields).
 */
#include "ovl_0a.h"

s32 func_800BF0C4(void) {
    s16 dur;
    u16 ox;
    u16 oy;
    Unk800C9DB4Obj *o;
    Unk800C9DB4 *h;
    s16 wx;
    s16 wy;
    s32 dx;
    s32 flags;
    s32 smooth;
    s32 ax;
    s32 ay;
    s32 i;
    Unk800C9DB4Cam *c;
    s16 cnt;
    s32 s;
    s32 v;
    s32 t;
    s32 y;
    s32 mode;
    flags = D_800CA068.unk0;
    if (!(flags & 0x40)) {
        return 0;
    }
    mode = flags & 7;
    smooth = flags & 8;
    if (mode == 1 || mode == 2 || mode == 3) {
        ox = D_800CA068.unk4.unk0;
        oy = D_800CA068.unk4.unk2;
        h = D_800C9DB4;
        wx = D_800CA068.unk14 - D_800CA068.unk10.unk0;
        wy = D_800CA068.unk16 - D_800CA068.unk10.unk2;
        cnt = D_800CA068.unk18;
        dur = D_800CA068.unk1A;
        if (!smooth) {
            D_800CA068.unk4.unk0 = D_800CA068.unk10.unk0 + wx * cnt / dur;
            D_800CA068.unk4.unk2 = D_800CA068.unk10.unk2 + wy * cnt / dur;
        } else {
            y = func_8004BB7C(((cnt << 16) >> 5) / dur + 0x800) + 0x1000;
            ax = wx * y / 0x2000;
            ay = wy * y / 0x2000;
            D_800CA068.unk4.unk0 = D_800CA068.unk10.unk0 + ax;
            D_800CA068.unk4.unk2 = D_800CA068.unk10.unk2 + ay;
        }
        ox = D_800CA068.unk4.unk0 - ox;
        oy = D_800CA068.unk4.unk2 - oy;
        o = (Unk800C9DB4Obj *)((u8 *)h + h->unk10);
        for (i = 0; i < h->unk6; o++, i++) {
            if (o->unk0 & 4) {
                if (o->unk1C != 0) {
                    o->unkC += ox;
                }
                if (o->unk1E != 0) {
                    o->unkE += oy;
                }
                dx = (s16)ox << 16;
            s = D_800CA068.unkC8[o->unk25_1] << 8;
            v = dx;
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
            v = oy << 16;
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
                    o->unkE += oy;
                } else {
                    o->unkC += ox;
                }
            s = D_800CA068.unkC8[o->unk25_1] << 8;
            v = (s16)ox << 16;
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
            v = oy << 16;
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
        s = h->unk18;
        s = (s32)h + s;
        s = s + D_800CA068.unk75 * sizeof(Unk800C9DB4Cam);
        v = 160;
        y = *(s16 *)&D_800CA068.unk4.unk0 - (((Unk800C9DB4Cam *)s)->unk24 >> 1);
        i = v - y;
        t = 112 - (*(s16 *)&D_800CA068.unk4.unk2 - (((Unk800C9DB4Cam *)s)->unk26 >> 1));
        D_800CA068.unkC = i;
        D_800CA068.unkE = t;
        if (mode == 1) {
            D_800CA068.unk0 = (D_800CA068.unk0 & ~7) | 2;
        }
        if (++D_800CA068.unk18 > dur) {
            if ((D_800CA068.unk0 & 7) == 2) {
                D_800CA068.unk0 = (D_800CA068.unk0 & ~7) | 0x84;
            } else {
                D_800CA068.unk0 = (D_800CA068.unk0 & ~7) | 0x80;
            }
        }
    }
    return 1;
}
