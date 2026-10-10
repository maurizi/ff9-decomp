/* 28 rows differ: the first SE call (unk26 == 0xFF path) now keeps its own jalr like the original, because the three list-cursor branches are an if/else-if chain that falls into one shared return 1 (separate returns let jump2 cross-jump all four calls). Remaining: jump2 now merges the whole argument setup of the 0x10/0x40/0x10000 calls, while the original shares only the jalr (each branch keeps its own setup and jumps to the jalr), and in the 0x10000 path the original stores D_8007B958 = D_8007B96C after the setup, right before the jalr. That suggests the store sits between the setup and the call in RTL, which no C order we tried gives. The 0x10 path is written as if (D_8007B958 > 0) D_8007B958--; else D_8007B958 = D_8007B95C - 1; (bgtz with the decrement in the delay slot). */
#include "ff9.h"

extern s32 D_8007B95C;
extern s32 D_8007B96C;

static inline void se_play(s32 id) {
    D_8006794C->unk1C->unk83C(0xD000, id, 0x800000, 0x80, 0x7F);
}

s32 func_8004DAD0(Unk8007B9C0 *w, s32 pad) {
    u8 f;
    s32 v;

    if ((D_8007B708 == 1 && (pad & 0xA0000)) || (D_8007B708 != 1 && (pad & 0x20000))) {
        f = w->unk28;
        if (!(f & 0x40)) {
            if (w->unk26 == 0xFF) {
                w->unk26 = 1;
                if (D_8007B968 >= 0) {
                    se_play(0x67);
                }
                return 1;
            }
            if (!(f & 8)) {
                w->unk28 = f | 4;
                return 1;
            }
        }
    }
    if (D_8007B968 >= 0) {
        if (D_8007B970 != w->unkA) {
            return 0;
        }
        if (pad & 0x10) {
            if (D_8007B958 > 0) {
                D_8007B958--;
            } else {
                D_8007B958 = D_8007B95C - 1;
            }
            D_8006794C->unk1C->unk83C(0xD000, 0x67, 0x800000, 0x80, 0x7F);
        } else if (pad & 0x40) {
            if (D_8007B958 < D_8007B95C - 1) {
                D_8007B958++;
            } else {
                D_8007B958 = 0;
            }
            D_8006794C->unk1C->unk83C(0xD000, 0x67, 0x800000, 0x80, 0x7F);
        } else if ((pad & 0x10000) && D_8007B96C >= 0) {
            D_8007B958 = D_8007B96C;
            D_8006794C->unk1C->unk83C(0xD000, 0x65, 0x800000, 0x80, 0x7F);
        } else {
            return 0;
        }
        return 1;
    }
    return 0;
}
