/* WIP func_800E4A3C: 7 differing rows (build/cmp.py), one instruction short; everything after the prologue matches. Pass 10: the third param is a 12-byte struct passed by value (a2, a3, stack 0x30). That gives the stack homes without the entry `sw a3,0x2C` (flow deletes the dead store of a3 because 0x2C is written again) and without the dead `lw 0x30`. e->unk2C must read the field as a scalar (*(s32 *)&arg2.b): a struct-field read can't be scheduled above the struct store to e->unk18[2] (MEM_IN_STRUCT alias rule), a scalar read can. Left: the original stores c from a copy (sw a2,0x2C; addu v0,a2; sw v0,0x30); ours stores a2 twice (one pseudo). Tried: c = b / c = a / chained / temp k / pointer-indexed / inline copy helper, 2-field struct + 5th param (dead store deleted), scalar arg2 + struct {b, c} (arg2 then gets an s-reg instead of its stack home). Integration also needs the header prototype changed or an asm-label declaration, since callers pass two pointers. Pass 14: five plain scalar params with `arg3 = arg2; arg4 = arg3;` and arg3 used directly is the right length (114) but arg2/arg3 get s-registers instead of their stack homes (37 rows); taking &arg2 only keeps arg2 in memory and the arg3/arg4 stores die (9 rows). Pass 15: a 3-param definition (s32, s32, s16 *arg2) that writes the 4th/5th slots through `(&arg2)[1] = (&arg2)[0]; (&arg2)[2] = (&arg2)[1];` (K&R-style, callers pass 4 args): arg2's home is used, but CSE makes &arg2 a register (addiu v0,sp,40; sw a2,4(v0)): 17 rows; through a pointer variable p = &arg2 p lands in s1 (29 rows). Struct-param variants of the two copies (order, chained, scalar casts, k temp): 7-8. Pass 17: not re-tested. The v0 copy needs the value stored to the c slot to be a pseudo that CSE did not replace with a2. a2 stays live until the late `sw a2,0x28`, so local-alloc would then put that pseudo in v0. */
#define func_800E4A3C func_800E4A3C_hdr
#include "ovl_0b.h"
#undef func_800E4A3C

typedef struct { s16 *a; s16 *b; s16 *c; } E4Arg;

Pool6CEntry *func_800E4A3C(s32 arg0, s32 arg1, E4Arg arg2) {
    Pool6CEntry *e;
    s32 k;

    e = NULL;
    arg2.b = arg2.a;
    arg2.c = arg2.b;
    switch (arg1) {
    case 0:
    case 2:
        e = func_800E3CEC(func_800E454C(5), (s32)arg2.a);
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
        arg2.b[1] -= 0x40;
        e = func_800E3CEC(func_800E454C(1), (s32)arg2.a);
        if (e != NULL) {
            e->unk18[0].unk1C = func_800E454C(0x14);
            e->unk18[1].unk1C = func_800E454C(6);
            e->unk18[2].unk1C = func_800E454C(2);
            e->unk2C = *(s32 *)&arg2.b;
            e->unk22 = 0x20;
        }
        func_800E49D0(2, arg0);
        break;
    case 4:
        e = func_800E3CEC(func_800E454C(4), (s32)arg2.a);
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
