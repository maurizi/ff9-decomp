/*
 * ovl_0c func_800B0378 (0x9F4 bytes): player walkmesh movement / slide along walls, plus
 * "no-encounter" region checks (D_800D30C6).
 * WIP: same length (637 insns), ~36 aligned diff rows (build/ad.py). Found so far: the
 * else branch reuses the loop's dx/dy/dz locals (s/a/yy -> dz/dx/dy) and the sqrt test
 * is `dz = dy - r1; abs; dz = dz < iabs(dy + r2); if (dz)`. Left:
 *  - `m = (D_800CE278 << 1) * D_800CE260`: our product stays tied to LO and is reloaded
 *    through t2 (mflo t2), which shifts the later `cc` reload from t2 to t3;
 *  - else branch (0x4FC..): the constants/ex/ez get a0/a1 swapped and the original computes
 *    dy in the first ABS branch's delay slot.
 */
#include "ovl_0c.h"

void func_800B3760(void);
extern u8 D_800CCE4A;
extern u8 D_800CCE55;
extern u8 D_800CCF6C;
extern u8 D_800CE208;
extern u8 D_800CE1E0;

static inline s32 iabs(s32 v) {
    if (v < 0) {
        v = -v;
    }
    return v;
}

void func_800B0378(Unk800CCCC0 *cc) {
    ActorPos p;
    ActorPos q;
    u32 cell;
    s32 h;
    u32 cell2;
    s32 h2;
    s32 saved;
    s32 spd;
    s32 i;
    s32 j;
    s32 dx, dy, dz;
    s32 ex, ez;
    s32 k;
    s32 dist;
    s32 m;
    s32 r3;
    s32 off, lim, y, yy, a, zz, s, r1, r2;

    if (D_800CE3C8->unkD != 0 && D_800CE25E != 0 && func_800B7A14(0xC1) != 0x34 && D_800D3E75 == -1) {
        D_800CCE54 = D_800CCE55;
    }
    if (D_800CCE4A == 0 && D_800D3E60 != 2 && D_800D4818 == 0 && D_800D33F4 != 0 && D_800CE1E0 == 0) {
        func_800B3760();
    }
    spd = D_800CE3C0;
    if (spd < 0) {
        spd /= 2;
    }
    D_800D30C6 = 1;
    if (D_800CE3C8->unk8 == 0) {
        saved = D_800CE3C4;
        if (D_800CCF6C != 0) {
            D_800CE3C4 = 10;
            spd *= 4;
        }
        i = 0;
        do {
            j = i;
            cell = -1;
            h = -1;
            p = D_800CE3DC->unk14;
            if (func_800B0D6C((VECTOR *)&p, i, D_800CE3C4, spd, &cell, &h) ||
                func_800B0D6C((VECTOR *)&p, j = -i, D_800CE3C4, spd, &cell, &h)) {
                dx = p.vx - D_800CE3DC->unk40.vx;
                dy = p.vy - D_800CE3DC->unk40.vy;
                dz = p.vz - D_800CE3DC->unk40.vz;
                if (func_800B36B4(cc->unk0, cell, 0) != 0) {
                    dist = SquareRoot0(dx * dx + dz * dz);
                } else {
                    dist = SquareRoot0(dx * dx + dy * dy + dz * dz);
                }
                if (dist != 0) {
                    k = spd * spd / dist;
                    if (spd < 0) {
                        k = -k;
                    }
                } else {
                    k = 0;
                }
                q = D_800CE3DC->unk14;
                if (func_800B0D6C((VECTOR *)&q, j, D_800CE3C4, k, &cell, &h)) {
                    D_800CE3DC->unk14 = q;
                    cc->unk8 = cell;
                    cc->unkE = q.vy;
                    D_800CE208 = h;
                    break;
                }
            }
            i += 0x80;
        } while (i < 0x400);
        if (D_800CCF6C != 0) {
            D_800CE3C4 = saved;
        }
    } else {
        q = D_800CE3DC->unk14;
        if (func_800B0D6C((VECTOR *)&q, 0, D_800CE3C4, spd, &cell2, &h2)) {
            D_800CE3DC->unk14.vx = q.vx;
            D_800CE3DC->unk14.vz = q.vz;
            D_800CE260 = D_800CE3DC->unk14.vy + 0x2A30;
            if (D_800CE260 < 0x7E8 && D_800CE202 == 0 && D_800CE0EF == 0 && D_800CE278 < 0) {
                m = (D_800CE278 << 1) * D_800CE260;
                D_800CE3DC->unk14.vy += m >> 12;
            } else {
                D_800CE3DC->unk14.vy += D_800CE278;
            }
            cc->unk8 = cell2;
            cc->unkE = q.vy;
            D_800CE208 = h2;
        }
        off = 0x1C20;
        if (D_800CF444 == 1) {
            off = 0x1964;
            lim = 0x1E78;
        } else {
            lim = 0x2134;
        }
        ex = D_800CE3DC->unk14.vx - 0x2FF77;
        y = D_800CE3DC->unk14.vy;
        ez = D_800CE3DC->unk14.vz + 0x14031;
        dy = y + off;
        if (ABS(ex) < 0x9C4 && ABS(ez) < 0x9C4) {
            dx = 0x5F5E10 - ex * ex;
            zz = ez * ez;
            dz = dx - dy * dy - zz;
            if (-lim < y) {
                D_800D30C6 = 0;
            }
            if (dz > 0) {
                r1 = SquareRoot0(dz);
                r2 = SquareRoot0(dz);
                dz = dy - r1;
                if (dz < 0) {
                    dz = -dz;
                }
                dz = dz < iabs(dy + r2);
                if (dz) {
                    D_800CE3DC->unk40.vy = D_800CE3DC->unk14.vy = SquareRoot0(dx - zz) - off;
                } else {
                    D_800CE3DC->unk40.vy = D_800CE3DC->unk14.vy = -SquareRoot0(dx - zz) - off;
                }
            }
        }
    }
    ex = D_800CE3DC->unk14.vx - 0x407F7;
    ez = D_800CE3DC->unk14.vz + 0x3BDC3;
    if (ABS(ex) < 0x800 && ABS(ez) < 0x800) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->unk14.vx - 0x37FA8;
    ez = D_800CE3DC->unk14.vz + 0x1606D;
    if (ABS(ex) < 0xC00 && ABS(ez) < 0xC00) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->unk14.vx - 0x4AD96;
    ez = D_800CE3DC->unk14.vz + 0x32133;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->unk14.vx - 0x489E4;
    ez = D_800CE3DC->unk14.vz + 0x34BBF;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->unk14.vx - 0x250D4;
    ez = D_800CE3DC->unk14.vz + 0x46B95;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->unk14.vx - 0x36F54;
    ez = D_800CE3DC->unk14.vz + 0xF131;
    if (ABS(ex) < 0x60C && ABS(ez) < 0x800) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->unk14.vx - 0x3D3D2;
    ez = D_800CE3DC->unk14.vz + 0x36C0F;
    if (ABS(ex) < 0x500 && ABS(ez) < 0x500) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->unk14.vx - 0x4A02F;
    ez = D_800CE3DC->unk14.vz + 0x11DD8;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->unk14.vx - 0xC02F;
    ez = D_800CE3DC->unk14.vz + 0x1D9EC;
    if (ABS(ex) < 0x500 && ABS(ez) < 0x500) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->unk14.vx - 0x17A3B;
    ez = D_800CE3DC->unk14.vz + 0x4014A;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        D_800D30C6 = 0;
    }
    func_800BC0A8(&D_800CE3DC->unk14.vx, &D_800CE3DC->unk14.vz);
}
