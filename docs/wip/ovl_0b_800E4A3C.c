/* WIP func_800E4A3C: 8 differing rows (build/cmp.py-style aligned diff). Body matches; only the prologue differs. The original keeps arg2/arg3/arg4 (a 5th param that callers never pass) in their stack homes: sw a2 to 0x28/0x2C and a copy to 0x30, with no store of a3 and no load of 0x30. Taking the addresses through an inline (cp3 below) forces them into memory but leaves the entry sw a3,0x2C and a dead lw 0x30 (the params were pseudos at assign_parms time). volatile params: 16-17 rows; (&arg2)[1] indexing: 23+ rows. */
#define func_800E4A3C func_800E4A3C_hdr
#include "ovl_0b.h"
#undef func_800E4A3C

static inline void cp3(s16 **a, s16 **b, s16 **c) {
    *b = *a;
    *c = *b;
}

Pool6CEntry *func_800E4A3C(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3, s16 *arg4) {
    Pool6CEntry *e;
    s32 k;

    e = NULL;
    cp3(&arg2, &arg3, &arg4);
    switch (arg1) {
    case 0:
    case 2:
        e = func_800E3CEC(func_800E454C(5), *(s32 *)&arg2);
        if (arg1 == 0) {
            if (e != NULL) {
                e->unk18[0].unk1C = func_800E454C(7);
                goto common;
            }
        } else {
            if (e != NULL) {
                e->unk18[0].unk1C = func_800E454C(8);
            common:
                if (e != NULL) {
                    e->unk18[1].unk1C = func_800E454C(6);
                    e->unk22 = 8;
                }
            }
        }
        if (func_800E865C(0x10, 0) == 0) {
            goto zero;
        }
        func_800E49D0(1, arg0);
        break;
        break;
    case 1:
    case 7:
        (&arg3)[0][1] -= 0x40;
        e = func_800E3CEC(func_800E454C(1), *(s32 *)&arg2);
        if (e != NULL) {
            e->unk18[0].unk1C = func_800E454C(0x14);
            e->unk18[1].unk1C = func_800E454C(6);
            e->unk18[2].unk1C = func_800E454C(2);
            e->unk2C = *(s32 *)&arg3;
            e->unk22 = 0x20;
        }
        func_800E49D0(2, arg0);
        break;
    case 4:
        e = func_800E3CEC(func_800E454C(4), *(s32 *)&arg2);
        if (e != NULL) {
            e->unk18[0].unk1C = func_800E454C(7);
            e->unk18[1].unk1C = func_800E454C(6);
            e->unk22 = 0x80;
        }
    zero:
        func_800E49D0(0, arg0);
        break;
    }
    return e;
}
