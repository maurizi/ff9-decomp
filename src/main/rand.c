#include "ff9.h"

void rand_init(s32 seed) {
    u32 tbl[0x209];
    u32 v = 0;
    s32 i;
    s32 j;
    for (i = 0; i < 17; i++) {
        for (j = 31; j >= 0; j--) {
            seed = seed * 0x5D588B65 + 1;
            v = (v >> 1) | (seed & 0x80000000);
        }
        tbl[i] = v;
    }
    tbl[16] = (tbl[16] << 23) ^ (tbl[0] >> 9) ^ tbl[15];
    for (i = 17; i < 0x209; i++) {
        tbl[i] = (tbl[i - 17] << 23) ^ (tbl[i - 16] >> 9) ^ tbl[i - 1];
    }
    for (i = 0; i < 0x209; i++) {
        g_randTable[i] = tbl[i];
    }
    rand_refill();
}
