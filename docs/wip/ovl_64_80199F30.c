/*
 * ovl_64 func_80199F30 (0x680): per-frame FMV step: CD/MDEC bookkeeping, start the MDEC on the
 * current slice, run the first-frame style decode loop (shared shape with func_80199B28), wait
 * for the frame (volatile 0x800000 countdown, twice), then flip the display.
 * WIP (pass 1, new draft): 416 of 416 instructions, 1 aligned diff row.
 * Needs (already in include/ovl_64.h): Ovl64State.unk2/unkC (CdlLOC)/unk28, Ovl64FieldC14.unk30.
 * What fixed it: inside the loop everything is direct globals (&D_8019F4D8 for the call, xor and
 * found block, D_8019DC94.unk44/unk28/unk2, &D_8019DC94.unk24), so loop.c hoists the lo_sums and
 * cse2 turns them into the copies the original has (s3 = s0, s5 = s2, s6 = s1); the pre-loop part
 * uses a `dec` pointer but `(s8)D_8019DC94.unk1` directly; the wait is a static inline taking the
 * Ovl64Dec pointer (a macro or a no-arg inline is worse); the env address only comes out
 * multiply-first as integer arithmetic `(DISPENV *)(idx * 20 + (s32)D_8006794C->unkC)`
 * (pointer forms give base-first).
 * Left: the found block's `D_8019F4D8.unk0[D_8019F4D8.unk8]` is `addu v0,v0,s3` (ours) vs
 * `addu v0,s3,v0`. Cause: the symbol address is expanded as the second operand of the plus,
 * and cse1 follows `goto found` (single-use label after a barrier), so it knows the base is a
 * constant and keeps it second. Base-first needs a pointer variable unknown to cse there
 * (a pointer set before the loop, as in func_80199B28's inline). But every such variant
 * (block-local d for the loop, d only for found/xor, the pre-loop dec, the loop in an inline with
 * a dec param) loses the hoisting of wait #1's %hi(D_8019F4D8) into fp (it has no matching movable
 * left) and costs 24-90 rows. The permuter could not parse the file
 * ("conversion to non-scalar type requested" from its rewritten source).
 */
#include "ovl_64.h"

typedef struct {
    u8 pad0[3];
    u8 unk3;
    u32 unk4;
    u32 unk8;
} Ovl64Stp;
extern Ovl64Stp D_8019F4A8[2];
extern s32 D_8019DC9C;
s32 func_8019AAF8(void);
s32 func_8019C9D4(void);
int DecDCTin(u32 *, int);

static inline void func_8019_wait_frame(Ovl64Dec *dec) {
    volatile s32 n;

    n = 0x800000;
    while (dec->unk30 == 0) {
        if (--n == 0) {
            dec->unk30 = 1;
            dec->unk28.x = dec->unk18[D_8006794C->unk1C->unk8].x;
            dec->unk28.y = dec->unk18[D_8006794C->unk1C->unk8].y;
            DecDCTReset(1);
        }
    }
    dec->unk30 = 0;
}

s32 func_80199F30(s32 arg0) {
    Ovl64State *st;
    Ovl64Dec *dec;
    s32 tries;
    s32 res;
    u8 *data;
    u8 mode;
    DISPENV *env;
    CdlLOC *loc;

    if (D_80071E32 < 2) {
        return 0;
    }
    st = &D_8019DC94;
    if ((s8)st->unk0 != 0) {
        func_8019AAF8();
        return 0;
    }
    if (func_8019C9D4() == 0) {
        if (st->unk34++ == st->unk38) {
            func_80056278(st->unk3C, st->unk40);
        }
    }
    st = &D_8019DC94;
    if ((s8)st->unk3 == 2) {
        func_8019A7A8(0xE0, D_80071E31);
        st->unk3 = 0;
    }
    st->unk24 = st->unkC;
    mode = D_8006794C->unk8;
    if (mode == 1 || mode == 5) {
        func_800C1B5C(&st->unk2);
        st->unk28 = ((Ovl64Field8E4 *)D_8006794C->unk1C->unk8E4.ovl65)->unkC->unk14->unk30;
        ((Ovl64Field8E4 *)D_8006794C->unk1C->unk8E4.ovl65)->unkC->unk14->unk30 = 0;
    }
    D_8019F4C8.unk0 = 0;
    D_8019F4C8.unk2 = 0;
    D_8019F4C8.unk4 = D_8019E018[D_8006794C->unk1C->unk8];
    D_8019F4C8.unkC = D_8019F4C8.unk8 = (s32)&D_8019E028[D_8006794C->unk1C->unk8];
    if (D_80071E31 == 0 && D_8019F4C8.unk4 != 0) {
        Ovl64Stp *p = &D_8019F4A8[D_8006794C->unk1C->unk8];
        p->unk3 = 2;
        p->unk4 = 0xE6000002;
        p->unk8 = 0;
    }
    dec = &D_8019F4D8;
    dec->unk28.x = dec->unk18[D_8006794C->unk1C->unk8].x;
    dec->unk28.y = dec->unk18[D_8006794C->unk1C->unk8].y;
    DecDCTin(dec->unk0[dec->unk8], (s8)D_8019DC94.unk1);
    DecDCTout(dec->unkC[dec->unk14], (dec->unk28.w * dec->unk28.h) >> 1);
    while (1) {
        tries = 1000;
        res = -1;
        do {
            data = (u8 *)func_801990A0((Ovl64Out *)&D_8019F4D8);
            if (data != NULL) {
                goto found;
            }
            if ((s8)D_8019DC94.unk0 == 1) {
                res = -1;
                goto check;
            }
        } while (--tries != 0);
        goto check;
    found:
        D_80071E34++;
        D_8019F4D8.unk8 ^= 1;
        if (func_8019D3F0((u16 *)data) > 0x4000) {
            func_8019CCBC(data);
            res = -1;
        } else {
            func_8019AEC0(data, D_8019F4D8.unk0[D_8019F4D8.unk8], D_8019DC94.unk44);
            if (D_80071E38) {
                func_8019CC0C();
                res = 0;
            } else {
                func_8019CCBC(data);
                res = 0;
            }
        }
    check:
        if (res != -1) {
            break;
        }
        if ((s8)D_8019DC94.unk0 != 0) {
            func_8019_wait_frame(&D_8019F4D8);
            mode = D_8006794C->unk8;
            if (mode == 1 || mode == 5) {
                ((Ovl64Field8E4 *)D_8006794C->unk1C->unk8E4.ovl65)->unkC->unk14->unk30 = D_8019DC94.unk28;
                func_800C0BCC((s8)D_8019DC94.unk2);
            }
            return 1;
        }
        func_8019CAC0(&D_8019DC94.unk24);
        func_8019CA1C(&D_8019DC94.unk24);
    }
    func_8019_wait_frame(&D_8019F4D8);
    D_8006794C->unk9 = 4;
    if (D_80071E34 != 0 && D_80071E34 % 50 == 0) {
        D_8019DC9C = 1;
    }
    if (D_8019DC94.unk8 != 0 && D_8006794C->unk9 >= 4) {
        D_8006794C->unk9--;
        D_8019DC94.unk8 = 0;
    }
    if (arg0 != 0) {
        env = (DISPENV *)(D_8006794C->unk1C->unk8 * 20 + (s32)D_8006794C->unkC);
        VSync(0);
        PutDispEnv(env);
        SetDispMask(1);
    }
    return 1;
}
