/*
 * ovl_0c func_800C53DC (0x66C bytes): world map effect nodes, per-frame update (sibling of the
 * renderer func_800C4DD8).
 * WIP: 408 of 411 instructions, everything else structurally identical. Left:
 *  - the original saves s7 and sets `s7 = 0` (in the delay slot of `if (spd != 0)`) but never
 *    reads it: some dead local (counter?) that survived until after register allocation. Its
 *    save/restore also makes the frame 0x38 instead of 0x30. A plain unused counter is
 *    removed completely by our build.
 *  - quotient/abs at the top: v0/v1 swapped and the %hi(D_800D53C8) lui lands in the bgez slot.
 *  - case 32: operand registers of the two subtractions swapped.
 */
#include "ovl_0c.h"

void func_800C7050(WmNode *);
s32 func_800CAE00(u16 *);

static inline s32 iabs(s32 v) {
    if (v < 0) {
        v = -v;
    }
    return v;
}

static inline s32 wm_rand8(void) {
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
    }
    return *D_8007B720++;
}

void func_800C53DC(void) {
    WmNode *o;
    WmNode *cur;
    s16 spd;

    spd = D_800CE3C8->unk2;
    o = D_800D53A0.unk8;
    if (spd != 0) {
        D_800D53C8 = iabs((D_800CE3C0 << 12) / spd);
    } else {
        D_800D53C8 = 0;
    }
    D_800D4A38 += (D_800D53C8 - D_800D4A38) / 32;
    while (o->unk8 != NULL) {
        switch ((u8)o->unkD) {
        case 2:
            o->unk14 -= 60 + wm_rand8() % 24;
            o->unk10 -= 30 - wm_rand8() % 60;
            o->unk18 -= 30 - wm_rand8() % 60;
            break;
        case 3:
            o->unk14 -= 60 + wm_rand8() % 24;
            o->unk10 -= 60 - wm_rand8() % 60;
            o->unk18 -= 30 - wm_rand8() % 60;
            break;
        case 23:
            o->unk10 = func_8004BB80((o->unk1C << 6) + o->unk20) / 2 + D_800CDF50.vx;
            o->unk14 -= 80;
            o->unk18 = func_8004BB7C((o->unk1C << 6) + o->unk20) / 2 + D_800CDF50.vz;
            /* fallthrough */
        case 24:
            o->unk10 -= 40 + wm_rand8() % 130;
            o->unk14 -= 40 - wm_rand8() % 80;
            o->unk18 += 300 + wm_rand8() % 300;
            break;
        }
        o->unk1C++;
        cur = o;
        o = o->unk8;
        switch ((u8)cur->unkD) {
        case 2:
            if (cur->unk1C >= 77) {
                func_800C7050(cur);
            }
            break;
        case 3:
            if (cur->unk1C >= 46) {
                func_800C7050(cur);
            }
            break;
        case 16:
        case 17:
            if (cur->unk1C >= 9) {
                func_800C7050(cur);
            }
            break;
        case 32:
            if (iabs(cur->unk10 - D_800CE3DC->unk14.vx) > 32000 || iabs(cur->unk18 - D_800CE3DC->unk14.vz) > 32000) {
                func_800C7050(cur);
            }
            break;
        default:
            if (cur->unk1C == func_800CAE00((u16 *)cur->unk0)) {
                func_800C7050(cur);
            }
            break;
        }
    }
}
