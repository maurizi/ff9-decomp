/* 10 rows differ: the original stores D_8007AE10[3] through a la+6 base (sh v0,6(v1)) after the loop setup; every form we tried folds it into %lo(D_8007AE10+6). */
#include "ff9.h"

static inline u16 inl(s32 a) {
    s32 i = a & 7;
    if (i >= 2 && i < 5) {
        D_8007AE10[i] = func_800BEC64(i - 2);
    }
    return D_8007AE10[i];
}
static inline u16 inl2(s32 i) {
    D_8007AE10[i] = func_800BEC64(i - 2);
    return D_8007AE10[i];
}
static inline u16 inl4(s16 *v, s32 i) {
    v[i] = func_800BEC64(i - 2);
    return v[i];
}
static inline u16 inl5(s32 i) {
    u16 r = func_800BEC64(i - 2);
    s16 *v = D_8007AE10;
    v[i] = r;
    return v[i];
}
static inline void set5(s16 *v, s32 i, u16 r) {
    v[i] = r;
}
static inline u16 inl3(s32 i) {
    s16 *p = &D_8007AE10[i];
    *p = func_800BEC64(i - 2);
    return *p;
}

void func_80048E80(void) {
    s32 i;
    s32 m;
    Unk8007AED0 *o;

    if (D_8006794C->unk1C->unk8E4.ovl65->unkD80 != 0) {
        m = func_8004B404_inline(3);
        for (i = 4; i < 8; i++) {
            o = D_8007AED0[i];
            if (o != NULL) {
                switch (o->unkE) {
                case 1:
                    if (!(m & 0x10)) {
                        o->unkE = 3;
                    }
                    break;
                case 3:
                    if (m & 0x10) {
                        o->unkE = 1;
                    }
                    break;
                }
            }
            m >>= 1;
        }
    }
}
