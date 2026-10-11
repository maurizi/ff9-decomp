/* Sine/cosine and the random-table refill (main 0x8004BB7C-0x8004BFAF), hand-written asm in the original.
 *
 * math_cos falls through into math_sin after adding a quarter turn. func_8004BBBC, func_8004BBD0 and
 * func_8004BBDC are not functions: they are the quadrant jump targets inside math_sin (it computes
 * D_8004BBAC + quadrant * 16 and jumps there with v0 = g_sinTable, v1 = 0x800, a0 = 2 * (a & 0x3FF)). Nothing calls
 * them, so no equivalents are given. Standard C ABI otherwise. rand_init is already C in src/main/main.c.
 */
#include "ff9.h"

/* Angle units: 0x1000 = full turn (only bits 0-11 matter); result: 4096 = 1.0. */
s32 math_sin(s32 a) {
    u32 i = a & 0x3FF;

    switch (((u32)a >> 10) & 3) {
    case 0:
        return g_sinTable[i];
    case 1:
        return g_sinTable[0x400 - i];
    case 2:
        return -g_sinTable[i];
    default:
        return -g_sinTable[0x400 - i];
    }
}

s32 math_cos(s32 a) {
    return math_sin(a + 0x400);
}

/* Regenerates g_randTable (x[i] ^= x[i + 489] for i < 32, then x[i] ^= x[i - 32]) and rewinds g_randPtr. The original
 * does the second pass a word at a time over bytes 0x20..0x20B, so it also XORs the three bytes just past the
 * 0x209-byte table (g_randTableEnd and the next two); that is reproduced here. */
void rand_refill(void) {
    u8 *t = g_randTable;
    s32 i;

    g_randPtr = t;
    for (i = 0; i < 0x20; i++) {
        t[i] ^= t[i + 0x1E9];
    }
    for (i = 0x20; i < 0x20C; i++) {
        t[i] ^= t[i - 0x20];
    }
}

/* Next random byte: the out-of-line twin of the inline g_randPtr readers. */
u8 func_8004BF74(void) {
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    return *g_randPtr++;
}
