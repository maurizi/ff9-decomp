/* 38 rows by cmp, about 11 of them only the symbol of a %lo(D_8007B960/5C/40)-relative offset (verify would accept those). Real diffs: (1) case 0x4F second count-down loop: the original keeps -1 in sel's register a1 (li a1,-1 early, cnt-1 via addu v1,v1,a1, loop compare via a copy a0 = a1); here -1 is its own constant. `while (--cnt != -1)` gives the right loop shape (precheck + compare to -1) for both loops; the identical-branch `if (done)` around `cnt = *p++` is needed (89 rows without). (2) case 0xD tail: the original reads p via 16(sp) for the trailing `v = *p++; v |= *p++ << 8` (same code as the head read, no v reload) and via s1 (= &p) for the first skip-loop byte; we get the reverse. Tried: skip as do-while/for/while inline or direct, tail via next(&p), loop forms, sel = -1 positions (360 statement orders), dup/reuse/dw sweeps, permuter (~25 min). */
#include "ff9.h"

extern WinLayout D_8007B940;
extern s32 D_8007B95C;
extern s32 D_8007B96C;
void func_8004C2C0(u8 **, s32 *);

/* Inlined copy of func_8004EFB0 (message i, its size into out). */
static inline u8 *func_8004EFB0_inline(s32 i, WinLayout *out) {
    u8 *e = NULL;
    s32 v;

    if (i >= 0 && i < D_8007B954->unk0) {
        e = (u8 *)D_8007B954->unk4;
        v = ((u32 *)e)[D_8007B708 == 4 ? i : i * 2];
        if (out != NULL) {
            out->unk2 = ((v >> 16) & 0xFFF) + 9;
            out->unk9 = (D_8007B708 == 4 ? (u32)v >> 28 : ((MsgEntry8 *)e)[i].unk6) * 14 + 6;
        }
        e += v & 0xFFFF;
    }
    return e;
}

static inline s32 func_8004E248_next(u8 **pp) {
    return *(*pp)++;
}

static inline void func_8004E248_skip(u8 **pp) {
    while (*(*pp)++ != 0xFF) {
    }
}

WinLayout *func_8004E248(s32 id, s32 *x, s32 *y, s32 *z) {
    u8 *p;
    s32 v;
    s32 done;
    s32 t;
    s32 n, i;
    s32 mask;
    s32 sel, cnt;

    p = func_8004EFB0_inline(id, &D_8007B940);
    if (p != NULL) {
        done = 0;
        D_8007BB90 = 0;
        D_8007B940.unkA = 0;
        D_8007B940.unkB = 0;
        *x = 0;
        *y = 0;
        *z = 0;
        while (!done) {
            if (*p++ != 0xF9) {
                break;
            }
            switch (*p++) {
            case 0x7:
                D_8007B960 = -1;
                D_8007B95C = *p++;
                D_8007B96C = *p++ - 1;
                break;
            case 0x4F:
                D_8007B960 = D_8007B974;
                n = i = *p;
                mask = D_8007B974;
                while (--i != -1) {
                    n -= (mask ^ 1) & 1;
                    mask >>= 1;
                }
                mask = D_8007B960;
                D_8007B95C = n;
                D_8007B940.unk9 -= (*p++ - n) * 14;
                if (done) {
                    cnt = *p++;
                } else {
                    cnt = *p++;
                }
                sel = -1;
                while (--cnt != -1) {
                    sel += mask & 1;
                    mask >>= 1;
                }
                if (sel >= D_8007B95C) {
                    sel = D_8007B95C - 1;
                }
                D_8007B96C = sel;
                break;
            case 0x3:
                D_8007B940.unk0 = *p++;
                D_8007B940.unk0 |= *p++ << 8;
                D_8007B940.unk8 = *p++;
                D_8007B940.unkA = 0xFF;
                break;
            case 0x50:
            case 0x51:
            case 0x52:
            case 0x53:
            case 0x54:
            case 0x55:
            case 0x58:
            case 0x59:
            case 0x5A:
            case 0x5B:
            case 0x5C:
                D_8007B940.unkA = p[-1];
                break;
            case 0x5F:
                *x = *p;
                *x |= *(s8 *)++p << 8;
                *y = *++p;
                *y |= *(s8 *)++p << 8;
                *z = *++p;
                *z |= *(s8 *)++p << 8;
                p++;
                break;
            case 0xB:
                D_8007BB90 = 1;
                break;
            case 0x45:
                D_8007B940.unkB = 1;
                break;
            case 0xD:
                v = *p++;
                v |= *p++ << 8;
                while (v != 0xFFFF) {
                    t = (v >> 12) & 0xF;
                    v &= 0xFFF;
                    if (t == 0 || ((D_8007B960 >> (t - 1)) & 1)) {
                        func_8004C2C0(&p, &v);
                        if (D_8007B940.unk2 < v) {
                            D_8007B940.unk2 = v;
                        }
                    } else {
                        func_8004E248_skip(&p);
                    }
                    v = *p++;
                    v |= *p++ << 8;
                }
                break;
            default:
                done = 1;
                break;
            }
        }
    }
    return &D_8007B940;
}
