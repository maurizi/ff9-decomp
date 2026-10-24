/* ovl_0a fixed-point helpers (hand-written asm in the original, standard C ABI).
 * tools/equiv.py: all four agree with the original on every trial (300-1000 each). */
#include "ovl_0a.h"

/* 16.16 multiply: bits 16..47 of the 64-bit product. */
s32 func_800C1DC4(s32 a, s32 b) {
    long long p = (long long)a * b;

    return (s32)(((u32)(p >> 32) << 16) | ((u32)p >> 16));
}

/* 24.8 / 24.8: a / (b >> 8), shifted back up by 8.
 * The original is a bare MIPS div with no guards, so the two cases C leaves undefined are spelled out with the
 * R3000's results: divisor 0 gives a quotient of -1 (a >= 0) or 1 (a < 0), and 0x80000000 / -1 gives 0x80000000. */
s32 func_800C1DE0(s32 a, s32 b) {
    s32 d = b >> 8;
    s32 q;

    if (d == 0) {
        q = (a < 0) ? 1 : -1;
    } else if (d == -1 && a == (s32)0x80000000) {
        q = a;
    } else {
        q = a / d;
    }
    return (s32)((u32)q << 8);
}

/* Round 16.16 to an integer. */
s32 func_800C1DF4(s32 a) {
    return (a + 0x8000) >> 16;
}

/* (a + 0x8000) >> 8. */
s32 func_800C1E04(s32 a) {
    return (a + 0x8000) >> 8;
}
