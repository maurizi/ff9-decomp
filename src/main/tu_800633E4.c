#include "ff9.h"

u16 *func_800633E4(s32 id) {
    s32 i;
    s32 j;
    u8 *blk;
    DbEntry12 *tab;
    DbEntry12 *e;

    for (i = g_dbList.unk2 - 1; i >= 0; i--) {
        blk = (u8 *)db_lookup((s32)g_dbList.unk4[i], 2, 0x120000, 0);
        if (blk != (u8 *)-1) {
            j = blk[1] - 1;
            tab = (DbEntry12 *)(blk + 4);
            for (; j >= 0; j--) {
                e = &tab[j];
                if (e->unk0 == (u16)id) {
                    return (u16 *)e;
                }
            }
        }
    }
    return NULL;
}

u8 *func_800634A8(s32 a) {
    u16 *p = func_800633E4(a);
    return (u8 *)p + (((u32 *)p)[1] & 0xFFFFFF);
}

u8 *func_800634D8(s32 a) {
    u8 *p = (u8 *)func_800633E4(a);
    return p + (((u32 *)p)[1] & 0xFFFFFF) + p[7] * 4;
}

u8 *func_80063514(s32 a) {
    DbEntry12 *p = (DbEntry12 *)func_800633E4(a);
    s32 n = p->unk7;
    u8 *q = (u8 *)p + p->unk4;
    return (long long)n * 8 + q;
}
