/* 30 rows differ: the first SE call (unk26 == 0xFF path) is cross-jumped into the shared jalr of the three list-cursor SE calls here, but keeps its own jalr in the original; and the D_8007B958 stores of the 0x10/0x10000 paths are scheduled after the call setup. The || condition at the top fixed the 0xA0000/0x20000 test layout. */
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
            v = D_8007B958 - 1;
            if (D_8007B958 <= 0) {
                v = D_8007B95C - 1;
            }
            D_8007B958 = v;
            D_8006794C->unk1C->unk83C(0xD000, 0x67, 0x800000, 0x80, 0x7F);
            return 1;
        }
        if (pad & 0x40) {
            if (D_8007B958 < D_8007B95C - 1) {
                D_8007B958++;
            } else {
                D_8007B958 = 0;
            }
            D_8006794C->unk1C->unk83C(0xD000, 0x67, 0x800000, 0x80, 0x7F);
            return 1;
        }
        if ((pad & 0x10000) && D_8007B96C >= 0) {
            D_8007B958 = D_8007B96C;
            D_8006794C->unk1C->unk83C(0xD000, 0x65, 0x800000, 0x80, 0x7F);
            return 1;
        }
    }
    return 0;
}
