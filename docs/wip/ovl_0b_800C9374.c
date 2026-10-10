/* WIP func_800C9374: 32 differing rows (build/cmp.py-style aligned diff). semantic draft: the original walks entries from the set base (+0x14 fields) and copies the entry pointer to t4/t5 for the inner loops; needs the prototype changed to (TexAnimSet *, u16 (*)[4]). */
#define func_800C9374 func_800C9374_hdr
#include "ovl_0b.h"
#undef func_800C9374

void func_800C9374(TexAnimSet *set, u16 (*ofs)[4]) {
    u32 i;
    u32 j;
    u32 k;
    TexAnim *a;
    TexAnimRect *r;
    u16 *f;
    u8 idx;

    if (set == NULL) {
        return;
    }
    for (i = 0; i < set->unk0; i++) {
        a = &set->unk4[i];
        a->unkE = 1;
        if ((u32)a->unk10 > 0x80000000) {
            return;
        }
        if (a->unk10 == NULL) {
            continue;
        }
        idx = a->unkD;
        r = a->unk10 = (TexAnimRect *)((u8 *)a->unk10 + (s32)set);
        if (ofs != NULL) {
            r->unk0 += ofs[idx][2];
            r->unk2 += ofs[idx][3];
        }
        for (j = 0; j < a->unkC; j++, r++) {
            f = (u16 *)(r->unk8 = (void *)((u8 *)r->unk8 + (s32)set));
            if (ofs != NULL) {
                for (k = 0; k < a->unk1; k++) {
                    f[0] += ofs[idx][0];
                    f[1] += ofs[idx][1];
                    f += 2;
                }
            }
        }
    }
}
