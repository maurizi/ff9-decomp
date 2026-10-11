/* ovl_0a fixed-point helpers (hand-written asm in the original, standard C ABI). */
#include "ovl_0a.h"

/* 16.16 multiply: bits 16..47 of the 64-bit product. */
s32 func_800C1DC4(s32 a, s32 b) {
    long long p = (long long)a * b;

    return (s32)(((u32)(p >> 32) << 16) | ((u32)p >> 16));
}

/* 24.8 / 24.8: a / (b >> 8), shifted back up by 8. */
s32 func_800C1DE0(s32 a, s32 b) {
    return (a / (b >> 8)) << 8;
}

/* Round 16.16 to an integer. */
s32 func_800C1DF4(s32 a) {
    return (a + 0x8000) >> 16;
}

/* (a + 0x8000) >> 8. */
s32 func_800C1E04(s32 a) {
    return (a + 0x8000) >> 8;
}
