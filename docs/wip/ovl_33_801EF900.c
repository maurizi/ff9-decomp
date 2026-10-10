/* STATUS: near miss, 13 rows. ovl_33 func_801EF900 (0x28C, build the equipment/ability lists for a party slot). Left: the original keeps &D_801FBF84 in s2 for the prologue and copies it to s7 for the ability-list store in the loop (s2 is then reused for the ability-table pointer); ours keeps one register. m2 = m copies are coalesced by gcc. */
#include "ovl_33.h"

void func_801EF900(s32 slot, s32 arg1) {
    EquipMenu *m;
    EquipMenu *m2;
    UnkBlink *b;
    EquipMenu *m3;
    Character *c;
    AbilityEntry *a;
    Unk80069018 *it;
    s32 i;
    s32 j;
    s32 nItems;
    s32 nAbils;
    s32 ok;
    s32 id;

    nItems = 0;
    nAbils = 0;
    b = &D_801FBF84.unk770;
    m = &D_801FBF84;
    c = D_8006794C->unk1C->unk54C[slot];
    func_80028D08(b, m->unk2, slot, arg1);
    if (arg1 != 0) {
        m->unk1 = 5;
    }
    m->unk2 = slot;
    if (c->unk34.b.row < 16) {
        a = D_8006CB28[c->unk34.b.row];

        for (i = 0; i < 0x30; i++) {
            if (a->unk0 != 0) {
                if (a->unk0 < 0xC0) {
                    D_801FBF84.unk668[nItems].unk0 = a->unk0;
                    D_801FBF84.unk668[nItems].unk1 = func_801F0C10(a->unk0, c);
                    nItems++;
                } else {
                    m->unk728[nAbils] = a->unk0;
                    nAbils++;
                }
            }
            a++;
        }
    }
    func_8002EDFC(&D_801FBF84.unk8[0], nItems, 8, 2, 0x80);
    func_8002EDFC(&D_801FBF84.unk8[1], nAbils, 8, 2, 0x80);
    D_801FBF84.unk769 = nItems;
    D_801FBF84.unk76A = nAbils;
    func_8001D898(D_801FBF84.unk68, 0, 0x100);
    for (i = 0; i < 5; i++) {
        id = c->unk39[i];
        if (id != 0xFF) {
            it = &D_80069018[id];
            for (j = 0; j < 3; j++) {
                D_801FBF84.unk68[it->unkC[j]] |= 1 << i;
                D_801FBF84.unk168[it->unkC[j]][i] = id;
            }
        }
    }
    D_801FBF84.unk4[0] = c->unk10 != 0 && !(c->unk38 & 9) && nItems != 0 && D_8007AFD3 == 0;
    D_801FBF84.unk4[1] = func_80034A30(c);
    func_801F1048();
    func_801F0410();
}
