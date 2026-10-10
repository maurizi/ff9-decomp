/* STATUS: near miss, 46 rows (was 63). ovl_64 func_80199B28 (0x408, start a movie: pick the table entry, set up the MDEC/decode state, open the CD stream, decode the first frame). Pass 5: the first-frame loop (func_8019_decode_first, shared with func_80199F30) is a do/while that only fetches and tests; a non-NULL frame does 'goto found' to a block placed after the loop, the unk0 == 1 exit is 'res = -1; goto check' (cross-jumped into the too-big path), and res = -1 is set at the top of the outer while(1). That fixed the loop layout. Left: register assignment (original s3 &D_8019F4D8, s4 &D_8019DC94, s5 its copy, s6 &unk24, s7 the constant 1, s0 tries, s1 res, s2 data; ours shifted), the constant 1 hoisted into s7 is also used by the setup stores before the loop (st->unk6 = 1, D_80071E32++), and the ok flag after func_8019C7E4/func_8019C1B0 (original keeps it in v0). The permuter strips static inlines (now restored by tools/permute.py). */
#include "ovl_64.h"

extern char D_8019903C[];
extern s8 D_80071E32;
void func_8019A5B0(u8);
void func_8019AE44(void *);
void func_8019AEC0(u8 *, u32 *, void *);
s32 func_8019C1B0(s32 unused, u32 arg1, s32 arg2, void (*arg3)(void), s32 arg4);
s32 func_8019C7E4(void);
s32 func_8019C884(Ovl64RingHdr *ring, u32 per, u32 size);
s32 func_8019C954(u8 *arg0, s32 arg1, u32 arg2);
s32 func_8019C98C(s32 arg0);
s32 func_8019CA1C(CdlLOC *loc);
s32 func_8019CAC0(CdlLOC *loc);
s32 func_8019CC0C(void);
s32 func_8019CCBC(u8 *data);
u32 func_8019D3F0(u16 *p);
void func_80199554(void);
int DecDCTReset(int);


static inline s32 func_8019_decode_first(void) {
    s32 tries;
    s32 res;
    u8 *data;

    tries = 1000;
    while (1) {
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
            return 1;
        }
        if ((s8)D_8019DC94.unk0 != 0) {
            return 0;
        }
        func_8019CAC0(&D_8019DC94.unk24);
        func_8019CA1C(&D_8019DC94.unk24);
        tries = 1000;
    }
}

s32 func_80199B28(u32 movie, s32 idx) {
    Ovl64Size sz;
    s32 w;
    s32 file;
    s32 tries;
    s32 res;
    u8 *data;
    void (*cb)(void);
    s32 ok;
    Ovl64State *st;

    if (movie >= 5 || idx < 0 || idx >= D_8019DC5C[movie].unk0) {
        return 0;
    }
    D_8019DCF8 = &D_8019DC5C[movie].unk4[idx];
    D_80071E30 = D_8019DCF8->unk5;
    if (D_80071E31 < 0) {
        func_8019A5B0(D_8019DCF8->unk4);
    }
    w = D_8019DCF8->unkA;
    sz.unk4 = 0;
    sz.unk0 = w;
    if (D_80071E31) {
        D_8019DC94.unk1 = 1;
        D_8019F4D8.unk18[0].x = w + 0x1E0;
        D_8019F4D8.unk8 = 0;
        D_8019F4D8.unk14 = 0;
        D_8019F4D8.unk18[0].y = 0;
        D_8019F4D8.unk18[1].x = w;
        D_8019F4D8.unk18[1].y = 0;
        D_8019F4D8.unk28.w = 0x18;
        D_8019F4D8.unk18[0].w = 0x1E0;
        D_8019F4D8.unk18[1].w = 0x1E0;
        D_8019F4D8.unk30 = 0;
        D_8019F4D8.unk0[0] = (u32 *)D_8019DC94.unk48;
        D_8019F4D8.unk0[1] = (u32 *)D_8019DC94.unk4C;
        D_8019F4D8.unkC[0] = (u32 *)D_8019DC94.unk50;
        D_8019F4D8.unkC[1] = (u32 *)D_8019DC94.unk54;
    } else {
        D_8019DC94.unk1 = 0;
        D_8019F4D8.unk18[0].x = w + 0x140;
        D_8019F4D8.unk8 = 0;
        D_8019F4D8.unk14 = 0;
        D_8019F4D8.unk18[0].y = 0;
        D_8019F4D8.unk18[1].x = w;
        D_8019F4D8.unk18[1].y = 0;
        D_8019F4D8.unk28.w = 0x10;
        D_8019F4D8.unk18[0].w = 0x140;
        D_8019F4D8.unk18[1].w = 0x140;
        D_8019F4D8.unk30 = 0;
        D_8019F4D8.unk0[0] = (u32 *)D_8019DC94.unk48;
        D_8019F4D8.unk0[1] = (u32 *)D_8019DC94.unk4C;
        D_8019F4D8.unkC[0] = (u32 *)D_8019DC94.unk50;
        D_8019F4D8.unkC[1] = (u32 *)D_8019DC94.unk54;
    }
    file = D_8019DCF8->unk0;
    cb = func_80199554;
    DecDCTReset(0);
    DecDCToutCallback(cb);
    func_8019C884((Ovl64RingHdr *)D_8019DC94.unk58, 8, 0x40);
    func_8019C954((u8 *)D_8019DC94.unk5C, 2, 0xC);
    ok = 0;
    if (func_8019C7E4() == 0) {
        if (D_8006794C->unk8 == 1 || D_8006794C->unk8 == 5) {
            func_8019C98C(((Ovl64Field8E4 *)D_8006794C->unk1C->unk8E4.ovl65)->unkC->unk14->unk44);
        }
        ok = func_8019C1B0(file, D_8019DCF8->unk6, 99999, NULL, 0) == 0;
    }
    if (!ok) {
        return 0;
    }
    st = &D_8019DC94;
    D_8019F4C8.unk0 = 0;
    D_8019F4C8.unk2 = 0;
    D_8019F4C8.unk4 = 0;
    D_8019F4C8.unk8 = 0;
    D_8019F4C8.unkC = 0;
    if (st->unk44 == NULL) {
        func_80015C38(D_8019903C);
    }
    func_8019AE44(st->unk44);
    D_80071E38 = 0;
    D_80071E30 = 0;
    st->unk0 = 0;
    st->unk4 = 0;
    st->unk5 = 0;
    st->unk6 = 1;
    st->unk8 = 0;
    st->unk34 = 0;
    D_80071E32++;
    return func_8019_decode_first();
}
