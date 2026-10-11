/* func_8005658C (main), filed in config/library_funcs.us.txt as libspu but called by the AKAO bank loader
 * (main.c ~16821) to relocate a sample table in place: n 16-byte entries, the first two words get + base
 * (SPU addresses), the other two are copied. n is assumed >= 1 (the original is a do-while). */
#include "ff9.h"

void func_8005658C(s32 *src, Unk8005EBB4 *dst, u32 base, s32 n) {
    s32 *d = (s32 *)dst;

    do {
        d[0] = src[0] + base;
        d[1] = src[1] + base;
        d[2] = src[2];
        d[3] = src[3];
        src += 4;
        d += 4;
    } while (--n != 0);
}

/* func_80056700 (main), filed as libspu but AKAO code: load the sample bank `bank` (an AKAO-headed buffer)
 * into SPU RAM unless func_800565DC rejects it. Returns -1 if rejected, else 0. */
s32 func_80056700(s32 bank, s32 wait) {
    s32 *b = (s32 *)bank;

    if (func_800565DC(b) != 0) {
        return -1;
    }
    func_80056750(b, wait, b[6], b[4]);
    return 0;
}

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
