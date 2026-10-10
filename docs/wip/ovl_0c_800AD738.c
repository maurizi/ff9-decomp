/*
 * ovl_0c func_800AD738 (0xB48 bytes): world map camera follow (D_800CE108 = camera, D_800CE198 = target).
 * WIP: 721 of 722 instructions, all registers match. The only difference is one reorg
 * delay slot right after `if (D_800CE0EC == 1)` (0x8C): the original fills the bne slot
 * from the target (`lui a0, %hi(D_800CE0ED)`), we fill it from the fall-through
 * (`lui s0, %hi(D_800CE108)`), which also drops the duplicated lui at 0x828.
 * In fill_simple_delay_slots that means s0 counts as live at the bne target in the original
 * (or a label sits at the start of the then-block). Tried: switch with one case, labels,
 * statement orders. The odd `p = &D_8007AEF0.b[0x65]` pointer is needed for the flag clear.
 */
#include "ovl_0c.h"

s32 func_8004B1EC(void);
s32 func_8004B1FC(void);
void func_8002FA10(void);
extern u8 D_800CE1E0;
extern Unk800CE288 D_800CE118[4];
extern s32 D_800CCAFC;
extern s32 D_800CCB04;
extern s32 D_800CCB08;
extern s32 D_800CCB0C;
extern s32 D_800CCB10;
extern s32 D_800CCB14;

static inline s32 iabs(s32 v) {
    if (v < 0) {
        v = -v;
    }
    return v;
}

void func_800AD738(void) {
    VECTOR v;
    u32 c0;
    s32 t;
    s32 h0;
    u32 c1;
    s32 h1;
    u32 c2;
    s32 h2;
    u32 c3;
    s32 h3;
    u32 cell;
    s32 h;
    s32 res;
    s32 x;
    s32 y;
    s32 z;
    s32 d;
    s32 dir;
    s32 step;
    s32 a;
    s32 k;
    s32 x2, y2, z2, d2, step2, dir2;
    s32 dx, dz, s, r;
    u8 *p;

    res = 0;
    if (func_8004B1FC() == 0) {
        y = D_800CE3DC->unk14.vy + D_800CE1C4;
        x = D_800CE3DC->unk14.vx;
        z = D_800CE3DC->unk14.vz;
        if ((D_800CE1E0 = D_800CE0EC | D_800CE0ED | D_800CE0EE) != 0) {
            if (D_800CE0EC == 1) {
                if (iabs(D_800CE108[0] - x) < 16) {
                    D_800CE0EC = 0;
                } else {
                    D_800CE108[0] += (func_8004AE00()->unk14.vx - D_800CE108[0]) / 8;
                }
            }
            if (D_800CE0ED == 1) {
                if (iabs(D_800CE108[1] - y) < 16) {
                    D_800CE0ED = 0;
                } else {
                    D_800CE108[1] += (func_8004AE00()->unk14.vy - D_800CE108[1] + D_800CE1C4) / 8;
                }
            }
            if (D_800CE0EE == 1) {
                if (iabs(D_800CE108[2] - z) < 16) {
                    D_800CE0EE = 0;
                } else {
                    D_800CE108[2] += (func_8004AE00()->unk14.vz - D_800CE108[2]) / 8;
                }
            }
            if (D_800CE0EC == 0) {
                D_800CE108[0] = x;
            }
            if (D_800CE0ED == 0) {
                D_800CE108[1] = y;
            }
            if (D_800CE0EE == 0) {
                D_800CE108[2] = z;
            }
        } else {
            step = 0;
            d = y - D_800CE108[1];
            dir = d > 0x80;
            if (d < -0x80) {
                dir = 2;
            }
            D_800CCAFC = (d >= 0 ? d : -d) >> 5;
            if (D_800CCAFC < 0x20) {
                D_800CCAFC = 0x20;
            }
            if (D_800CCAFC > 0x100) {
                D_800CCAFC = 0x100;
            }
            if (dir != 0) {
                switch (D_800CE3C8->unk0) {
                case 1:
                case 2:
                    switch (dir) {
case 2:
step = (d + 0x80) / 8;
break;
case 1:
step = (d - 0x80) / 2;
break;
}
                    break;
                case 0:
                case 3:
                    switch (dir) {
case 2:
step = ((d + 0x80) << 8) / (D_800CCAFC << 5);
break;
case 1:
step = ((d - 0x80) << 8) / (D_800CCAFC << 5);
break;
}
                    break;
                }
            }
            D_800CE108[0] = x;
            D_800CE108[2] = z;
            if (ABS(step) >= 5 && D_800CE0EF == 0) {
                D_800CE108[1] += step;
            }
        }
    } else {
        D_800CE3DC->unk14.vx = D_800CE108[0];
        D_800CE3DC->unk14.vy = D_800CE108[1];
        D_800CE3DC->unk14.vz = D_800CE108[2];
    }
    if (func_8004B1EC() == 0) {
        k = 0;
        y2 = D_800CE3DC->unk14.vy + D_800CE1C8.vy;
        x2 = D_800CE3DC->unk14.vx + D_800CE1C8.vx;
        z2 = D_800CE3DC->unk14.vz + D_800CE1C8.vz;
        v.vx = x2 + (D_800CE1D0[0] >> 2);
        v.vy = y2;
        v.vz = z2 + (D_800CE1D0[2] >> 2);
        if (D_800CE1F2 != 0) {
            v.vx += 0x580;
            func_800BC0A8(&v.vx, &v.vz);
            func_800BE748(&v, &c0, &t, &D_800CE118[0], (u16 *)&h0);
            v.vx -= 0xB00;
            func_800BC0A8(&v.vx, &v.vz);
            func_800BE748(&v, &c1, &t, &D_800CE118[1], (u16 *)&h1);
            v.vx += 0x580;
            v.vz += 0x580;
            func_800BC0A8(&v.vx, &v.vz);
            func_800BE748(&v, &c2, &t, &D_800CE118[2], (u16 *)&h2);
            v.vz -= 0xB00;
            func_800BC0A8(&v.vx, &v.vz);
            func_800BE748(&v, &c3, &t, &D_800CE118[3], (u16 *)&h3);
            h = h0;
            cell = c0;
            if (h1 < h) {
                h = h1;
                cell = c1;
            }
            if (h2 < h) {
                h = h2;
                cell = c2;
            }
            if (h3 < h) {
                h = h3;
                cell = c3;
            }
        } else {
            func_800BC0A8(&v.vx, &v.vz);
            func_800BE748(&v, &cell, &t, &D_800CE118[0], (u16 *)&h);
        }
        if (D_800CE3C8->unk0 == 1) {
            if (h < -0x2134) {
                h = -0x2134;
            }
        } else if (h < -0x251C) {
            h = -0x251C;
        }
        if (D_800CE1E8.unk0[3] + h < y2) {
            k = ((u8)cell >> 2 == 0x31) * 2;
            if (D_800CE26D != 0) {
                k = 2;
            }
            if (D_800CE25D != 0) {
                k = 2;
            }
            if (D_800CE0F0->unk2 != 0 && D_800CE0F0->unk2 != 0x1000) {
                k = 3;
            }
            y2 = D_800CE1E8.unk0[3] + h;
        }
        if (D_800CE3C8->unk0 == 1 && y2 > -0x1194) {
            y2 = -0x1194;
        }
        d = y2 - D_800CE198[1];
        dir = d > 0x100;
        step = 0;
        if (d < -0x100) {
            dir = 2;
        }
        D_800CCAFC = (d >= 0 ? d : -d) >> D_800CCB04;
        if (D_800CCAFC < D_800CCB08) {
            D_800CCAFC = D_800CCB08;
        }
        if (D_800CCAFC > D_800CCB0C) {
            D_800CCAFC = D_800CCB0C;
        }
        if (dir != 0) {
            switch (D_800CE3C8->unk0) {
            case 1:
            case 2:
                switch (dir) {
case 2:
step = (d + 0x100) / 4;
break;
case 1:
step = (d - 0x100) / 32;
break;
}
                break;
            case 0:
            case 3:
                switch (dir) {
case 2:
step = ((d + 0x100) << 8) / (((D_800CCB10 - 2) * D_800CCAFC * 7) >> k);
break;
case 1:
step = ((d - 0x100) << 8) / (D_800CCB10 * D_800CCAFC * 3);
break;
}
                a = step >= 0 ? step : -step;
                if (a > 0x200) {
                    a = 0x200;
                }
                a = (((0x200 - a) * D_800CCB14 >> 12) + 0x800) << 1;
                step *= a;
                step /= 4096;
                break;
            }
        }
        D_800CE198[0] = x2;
        D_800CE198[2] = z2;
        if (ABS(step) >= 5 && D_800CE0EF == 0) {
            D_800CE198[1] += step;
        }
        res = y2;
    }
    dx = D_800CE198[0] - 0x2FF77;
    dz = D_800CE198[2] + 0x14031;
    if (ABS(dx) < 0x36B0 && ABS(dz) < 0x36B0) {
        s = 0xBAEB900 - dx * dx;
        s -= dz * dz;
        r = -SquareRoot0(s) + 0x898;
        if (s > 0 && r < D_800CE198[1]) {
            D_800CE198[1] = r;
        }
    }
    switch (D_800CE1F3) {
    case 0:
        if (D_800D33F4 != 0) {
            D_800CE1F3 = 1;
            D_800CE198[1] = res;
            D_800CE108[1] = D_800CE3DC->unk14.vy + D_800CE1C4;
        }
        break;
    case 1:
        D_800CE1F3 = 2;
        p = &D_8007AEF0.b[0x65];
        if (*p & 1) {
            func_8002FA10();
            D_8007AEF0.b[0x65] &= ~1;
        } else {
            func_8002F9D0();
        }
        break;
    }
    if (D_800CE198[1] < -0x47CF) {
        D_800CE198[1] = -0x47CF;
    }
    func_800BC0A8(&D_800CE108[0], &D_800CE108[2]);
    func_800BC0A8(&D_800CE198[0], &D_800CE198[2]);
}
