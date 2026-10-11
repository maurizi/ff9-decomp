/* STATUS: 52 rows (cmp.py). Fixed: init order (o = NULL first), xx = x * x before the call with a separate m for the max, pd = &dist right before the first func_80038618 call (the original keeps sp+0x20 in s0). Open: the original computes x * x before the jal with a0 = x copied first, so xx shares s1 with x; ours schedules the mult after the call. (pad & 0x10000) ? run ^ 1 : run gives bne v1,1 instead of xori. We use one s-reg more (s8). */
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
    s32 xx;
    Unk8004AB64 *o;
    s32 *pd;
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
            xx = x * x;
            y = 0x80 - y;
            dir = func_80052F24(x, y);
            m = y * y;
            if (m < xx) {
                m = xx;
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
        pd = &dist;
        o = func_80038618(obj, 0, pd);
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
                if (func_80038618(obj, 0, pd) != NULL && dist <= 0) {
                    obj->unk14.vx -= v.vx;
                    obj->unk14.vz -= v.vz;
                }
                D_8007AEA8 = 2;
            }
    }
    D_8007AEB0 = D_8007AEA8 != 0 ? (D_8007AEB0 < 25 ? D_8007AEB0 + D_8007AEB4 : -25)
                                 : (D_8007AEB0 < 0 ? D_8007AEB0 + 1 : 0);
}
