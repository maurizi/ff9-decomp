/*
 * ovl_0c func_800BEECC (0x678 bytes): world map navigation cursor (D_800CD104/108) update:
 * stick movement, snap to the nearest marker (D_800CD10C table), cycling with L1/R1.
 * WIP: 413 of 414 instructions, ~65 aligned diff rows, all register/frame:
 *  - the original frame is 0x50, 8 bytes more than ours: there is an unused 8-byte slot
 *    above dx/dz (0x20..0x27) that was allocated after them; a dummy local array lands below.
 *  - in both 64-entry loops the original keeps the index in s0 and the strength-reduced
 *    `i * 12` in s1/s5; ours swaps them (decl order / types / condition forms don't help).
 *  - last loop: the original does not hoist &D_800CD10C (lui t0 inside the loop) and loads
 *    D_800CD70C before the marker filter.
 */
#include "ovl_0c.h"

extern u8 D_8007AF57;
s32 func_800C1604(u32 arg0);

void func_800BEECC(void) {
    s32 dx;
    s32 dz;

    s32 pad;
    s32 dir;
    s32 ang;
    s32 d;
    s32 best;
    s32 bi;
    s32 i;
    s32 ex, ez;
    s32 fx, fz;
    s32 t;
    s32 e;
    s32 ax, az;
    s32 last;
    Unk800CD10C *p;

    if (D_800D3E80 != 0) {
        pad = 0;
        if (D_800D3E60 == 2) {
            dir = 1;
            dx = 0;
            dz = 0;
            if (D_800D3E84 == 1) {
                func_800C1B00(D_800CE3DC->unk14.vx, D_800CE3DC->unk14.vz, &D_800CD104, &D_800CD108, D_800D3E74 * 2 + 1);
                D_800CD104 <<= 12;
                D_800CD108 <<= 12;
            }
            if (D_800D3E78 != 0) {
                func_800B3398(&dx);
                func_800B345C(&dz);
                pad = func_800B84C0();
                if (D_800D3E64 == 0) {
                    pad = 0;
                    dx = 0;
                    dz = 0;
                }
            }
            dz = -dz;
            ang = (-ratan2(dz, dx) + 0x400) & 0xFFF;
            if (dx | dz) {
                t = SquareRoot0(dx * dx + dz * dz) << 5;
                d = t + (t >> 1);
            } else {
                d = 0;
            }
            if (d != 0) {
                D_800CD104 += func_8004BB80(ang) * d >> 12;
                D_800CD108 -= func_8004BB7C(ang) * d >> 12;
                if (D_800CD104 < 0) {
                    D_800CD104 += 0xF0000;
                }
                if (D_800CD108 < 0) {
                    D_800CD108 += 0xC8000;
                }
                if (D_800CD104 > 0xF0000) {
                    D_800CD104 -= 0xF0000;
                }
                if (D_800CD108 > 0xC8000) {
                    D_800CD108 -= 0xC8000;
                }
            }
            if (pad & 0x300000) {
                if (D_8007AF57 == 0) {
                    best = 0x7FFFFFFF;
                    bi = -1;
                    for (i = 0; i < 64; i++) {
                        if ((u8)func_800C1604(i)) {
                            fx = ((D_800CD10C[D_800D3E74][i].unk0 << 12) - D_800CD104) >> 12;
                            fz = ((D_800CD10C[D_800D3E74][i].unk2 << 12) - D_800CD108) >> 12;
                            e = SquareRoot0(fx * fx + fz * fz);
                            if ((D_800D3E74 != 1 || ((u32)(i - 6) >= 4 && i != 15)) && e < best) {
                                best = e;
                                bi = i;
                            }
                        }
                    }
                    if (bi != -1) {
                        dir = 0;
                        D_800D3E89 = bi;
                        D_800CD104 = D_800CD10C[D_800D3E74][D_800D3E89].unk0 << 12;
                        D_800CD108 = D_800CD10C[D_800D3E74][D_800D3E89].unk2 << 12;
                    }
                }
                if (D_8007AF57 != 0 && dir != 0) {
                    if (pad & 0x100000) {
                        dir = 1;
                    }
                    if (pad & 0x200000) {
                        dir = -1;
                    }
                    do {
                        D_800D3E89 = (D_800D3E89 + dir) & 0x3F;
                        D_800D3E79 = 1;
                    } while (!(u8)func_800C1604(D_800D3E89) || (D_800CD10C[D_800D3E74][D_800D3E89].unk0 == D_800CD10C[D_800D3E74][D_800D3E89 - dir].unk0 &&
                             D_800CD10C[D_800D3E74][D_800D3E89].unk2 == D_800CD10C[D_800D3E74][D_800D3E89 - dir].unk2 &&
                             (u8)func_800C1604(D_800D3E89 - dir)));
                    D_800CD104 = D_800CD10C[D_800D3E74][D_800D3E89].unk0 << 12;
                    D_800CD108 = D_800CD10C[D_800D3E74][D_800D3E89].unk2 << 12;
                }
            }
            last = -1;
            for (i = 0; i < 64; i++) {
                p = &D_800CD10C[D_800D3E74][i];
                ex = (p->unk0 << 12) - D_800CD104;
                ez = (p->unk2 << 12) - D_800CD108;
                if (D_800D3E74 != 1 || ((u32)(i - 6) >= 4 && i != 15)) {
                    ax = ex;
                    if (ax < 0) {
                        ax = -ax;
                    }
                    if (ax < D_800CD70C) {
                        az = ez;
                        if (az < 0) {
                            az = -az;
                        }
                        if (az < D_800CD70C && (u8)func_800C1604(i)) {
                            D_800CD104 += ex >> 3;
                            D_800CD108 += ez >> 3;
                            if (ax < 0x1000 && az < 0x1000) {
                                D_800D3E89 = i;
                                last = i;
                            }
                        }
                    }
                }
            }
            if (D_800D3E79 == 1) {
                D_800D3E79 = 0;
                D_8007AF57 = 0;
            } else {
                D_8007AF57 = last + 1;
            }
        }
    }
    if ((func_800B8498() & 0x10000) && D_800D3E77 != 0) {
        D_800D3E64 = 0;
    } else {
        D_800D3E64 = 1;
    }
    D_800D3E84 = D_800D3E60;
}
