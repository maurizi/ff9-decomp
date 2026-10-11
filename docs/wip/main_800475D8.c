/* STATUS: 89 rows (pass-16 cmp.py; the pass-15 draft scores 88 with the same tool), but the frame and s-reg count now match (0x50, s0-s7, no s8). Pass 16: (1) x is reused for x * x (`x *= x` after the func_80052F24 call). With a separate xx, sched1 moves the mult after the call (x crosses the earlier func_80019750 call, so nothing pins the mult before it); written to x, the mult stays before the call like the original (a0 = x copy, mult, mflo into x's s-reg). (2) No pd variable: pd = &dist was live at entry (the o != NULL block is reachable on the dir == 0x8000 path), so it conflicted with every s-reg pseudo and took s8. With &dist in both calls the frame matches, but the original still keeps sp+0x20 in s0 (shared with y) for both calls. In the original the dir == 0x8000 branch jumps straight to the tail (beq s2 -> L80047A38), so the o block is threaded away on that path; ours goes to the o test. Moving the o block inside `if (dir != 0x8000)` threads it but kills `o = NULL`, whose zero the original keeps in s3 (analog and func_8001D898's a1 copy it). Remaining: x/y swapped (s0/s1), o s7 instead of s3 (obj/speed/pad/analog shift by one), the run-bit test (`li 1; bne` here, `xori; bnez` in the original; ^=, !=, ?: with assignment, 1 - run, u8 casts all fail), &dist in s0. Older notes: init order o = NULL first; pd = &dist right before the first call was the pass-12 fix for s0. */
#include "ff9.h"

extern s32 D_8007AEB4;

void func_800475D8(Unk8004AB64 *obj) {
    SVECTOR v;
    u8 dz[4];
    s32 dist;
    s32 speed;
    s32 dir;
    s32 analog;
    s32 pad;
    s32 x, y;
    s32 run;
    s32 cur, d;
    s32 c, k;
    Unk8004AB64 *o;
    s32 m;

    o = NULL;
    speed = 30;
    dir = 0x8000;
    analog = 0;
    func_8001D898(&v, 0, 8);
    pad = func_8004E8D4();
    if (((D_80073940[0].unk0 >> 12) & 0xF) == 7) {
        x = D_80073938[0].unk0[2];
        y = D_80073938[0].unk0[3];
        func_80019750((Unk4 *)dz);
        if (x < dz[2] || dz[3] < x || y < dz[0] || dz[1] < y) {
            analog = 1;
        }
        x -= 0x80;
        if (analog) {
            y = 0x80 - y;
            dir = func_80052F24(x, y);
            x *= x;
            m = y * y;
            if (m < x) {
                m = x;
            }
            speed = (m * 16 - m) * 4 >> 14;
        }
    }
    if (!analog) {
        run = (D_8006794C->unk24 >> 6) & 1;
        if ((pad & 0x10000) ? run ^ 1 : run) {
            speed = 60;
        }
        if (pad & 0x80) {
            dir = 0x400;
        }
        if (pad & 0x20) {
            dir = -0x400;
        }
        if (pad & 0x40) {
            if (dir == 0x8000) {
                dir = 0;
            } else {
                dir -= dir >> 1;
            }
        }
        if (pad & 0x10) {
            if (dir == 0x8000) {
                dir = 0x800;
            } else {
                dir += dir >> 1;
            }
        }
    }
    D_8007AEAC = dir != 0x8000;
    if (dir != 0x8000) {
        if (analog) {
            dir += D_8007AECC->unk16E8;
        } else {
            dir += D_8007AECC->unk16EA;
        }
        dir &= 0xFFF;
        cur = obj->unk14.unk22 & 0xFFF;
        d = dir - cur;
        if (d < -0x7FF) {
            cur -= 0x1000;
        } else if (d >= 0x800) {
            cur += 0x1000;
        }
        obj->unk14.unk22 = ((cur + dir) >> 1) & 0xFFF;
        if (D_8007AECC->unk16F8 != 0 && speed > 30) {
            speed = 30;
        }
        if (speed > 30) {
            D_8007AEC4 += 2;
        } else {
            D_8007AEC4 += 1;
        }
        obj->unk5C = speed;
        v.vz = -speed;
        func_800530A8(&v, 0, dir);
        obj->unk14.vx += v.vx;
        obj->unk14.vz += v.vz;
        o = func_80038618(obj, 0, &dist);
        D_8007AEB4 = ((o->unkF >> 4) ^ 1) & 1;
        if (D_8007AEB4 == 0) {
            D_8007AEB0 = 0;
        }
    }
    if (o != NULL && dist <= 0 && D_8007AEB0 >= 0) {
            obj->unk98 = o;
            obj->unk9C = dist;
            c = func_800531D0(obj, o, dir);
            if ((u32)(c + 0x400) <= 0x800) {
                obj->unk14.vx -= v.vx;
                obj->unk14.vz -= v.vz;
                k = func_8004BB80(c);
                v.vx = (v.vx * k) >> 13;
                v.vz = (v.vz * k) >> 13;
                func_800530A8(&v, 0, 0x400);
                obj->unk14.vx += v.vx;
                obj->unk14.vz += v.vz;
                if (func_80038618(obj, 0, &dist) != NULL && dist <= 0) {
                    obj->unk14.vx -= v.vx;
                    obj->unk14.vz -= v.vz;
                }
                D_8007AEA8 = 2;
            }
    }
    D_8007AEB0 = D_8007AEA8 != 0 ? (D_8007AEB0 < 25 ? D_8007AEB0 + D_8007AEB4 : -25)
                                 : (D_8007AEB0 < 0 ? D_8007AEB0 + 1 : 0);
}
