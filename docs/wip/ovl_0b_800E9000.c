/* WIP func_800E9000: 2 differing rows (build/cmp.py-style aligned diff). only the copy loop: the original schedules i++ (addiu a2) before the lw. */
#include "ovl_0b.h"

void func_800E9000(Unk800E9000 *arg0, s32 *src, s32 n, Unk80100DC054 *e) {
    Unk800E9000 *h = arg0;
    s32 *s;
    s32 i;
    s32 k;
    Unk800E9000Ent *a;
    Unk800E9000Ent *b;

    if (h->unk0 != 0x6F73) {
        func_8002EB64(0x53, 0x190);
    }
    k = (h->unk4 - 8) >> 3;
    e->unk2 = n;
    e->unk40 = NULL;
    e->unk4 = k;
    e->unk3C = (u8 *)h + h->unk4;
    s = src;
    for (i = 0; i < n; i++) {
        e->unk180[i] = *s++;
    }
    b = (Unk800E9000Ent *)((u8 *)h + h->unk6);
    a = h->unk8;
    for (i = 0; i < k; i++) {
        e->unk18[i] = a[i].unk0;
        e->unk24[i] = a[i].unk2;
        e->unk30[i] = b[i].unk2;
    }
    func_800DC748(&e->unk90, 1);
    D_80100DC0.unk54 = e;
    D_80100DC0.unk60 = func_800DB8E0(e, NULL);
}
