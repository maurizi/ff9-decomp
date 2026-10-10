/* WIP func_800C9374: 2 differing rows (build/cmp.py-style aligned diff). Pass 7 rewrite: index every field through set->unk4[i] (the original's t1/t5/t4 copies are per-loop givs of set + 20*i), r = unk10 relocated in place, two pointers f/g in the frame loop. Left: the frame-table load goes to v0 then addu a2 (original loads straight into a2). With f = load; f = f + set; store f the load is right but f/g swap a0/a2 (16 rows). Needs the prototype changed to (TexAnimSet *, u16 (*)[4]) in ovl_0b.h (callers pass BtlUnit9C50 * / s32). */
#define func_800C9374 func_800C9374_hdr
#include "ovl_0b.h"
#undef func_800C9374

void func_800C9374(TexAnimSet *set, u16 (*ofs)[4]) {
    u32 i;
    u32 j;
    u32 k;
    TexAnimRect *r;
    u16 *f;
    u16 *g;
    u8 idx;

    if (set == NULL) {
        return;
    }
    for (i = 0; i < set->unk0; i++) {
        set->unk4[i].unkE = 1;
        r = set->unk4[i].unk10;
        if ((u32)r > 0x80000000) {
            return;
        }
        if (r == NULL) {
            continue;
        }
        idx = set->unk4[i].unkD;
        r = (TexAnimRect *)((u8 *)r + (s32)set);
        set->unk4[i].unk10 = r;
        if (ofs != NULL) {
            r->unk0 += ofs[idx][2];
            r->unk2 += ofs[idx][3];
        }
        for (j = 0; j < set->unk4[i].unkC; j++) {
            f = (u16 *)r[j].unk8;
            r[j].unk8 = (void *)(f = (u16 *)((u8 *)f + (s32)set));
            if (ofs != NULL) {
                for (k = 0; k < set->unk4[i].unk1; k++) {
                    g = f + 1;
                    *f += ofs[idx][0];
                    *g += ofs[idx][1];
                    f += 2;
                }
            }
        }
    }
}
