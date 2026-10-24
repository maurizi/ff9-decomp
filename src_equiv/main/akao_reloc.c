#include "ff9.h"

/* akao_relocInstruments and akao_loadBank turned out to be ordinary game C and are now matched in src/main/main.c. */

/* func_80059504 (main), filed as libspu: the number of set bits in p->unk4 (word at +4). No callers. */
s32 func_80059504(u32 *p) {
    u32 w = p[1];
    u32 bit = 1;
    s32 n = 0;

    if (w == 0) {
        return 0;
    }
    do {
        if (w & bit) {
            n++;
        }
        bit <<= 1;
    } while (bit != 0 && !(w < bit));
    return n;
}
