/* WIP func_800D7094: 30 differing rows (build/cmp.py-style aligned diff). loop givs: the original keeps i (slti 11) and two pointers, d+4 (entry, for the arg and unk0++) and d+0x14 (all other fields); here the givs come out d+0 based. */
#include "ovl_0b.h"

s32 func_800D7094(void) {
    Unk80100C78 *d = &D_80100C78;
    Unk80100C78Entry *e;
    s32 n = 0;
    s32 i;
    s32 r;

    for (i = 0; i < 11; i++) {
        e = &d->unk4[i];
        if (d->unk4[i].unk4.h != 0) {
            n++;
            D_800F78A4->unk28 = 0;
            if (d->unk4[i].unk14(2, d->unk4[i].unkC, D_800F78A4, e, NULL) != 0) {
                d->unk4[i].unk4.h = 2;
            } else {
                e->unk0++;
            }
            if (d->unk4[i].unk10 != NULL) {
                r = func_800DA2AC(d->unk4[i].unk10);
            } else {
                r = 0;
            }
            if (r == 0 && d->unk4[i].unk4.h == 2) {
                d->unk4[i].unk4.h = 0;
                d->unk0--;
                if (d->unk4[i].unkC != NULL) {
                    func_800E3C50(d->unk4[i].unkC);
                }
                if (d->unk4[i].unk10 != NULL) {
                    func_800E3C50(d->unk4[i].unk10);
                }
            }
        }
    }
    return n;
}
