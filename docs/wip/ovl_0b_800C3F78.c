/* WIP func_800C3F78: 3 differing rows (build/cmp.py-style aligned diff, -O1 -G0). -O1 unit. Left: case 5 test is bne->other + j case5 in the original (beq->case5 + j other here), and the table base lui/addiu comes before the index sll. */
#include "ovl_0b.h"

void func_800C3F78(Unk800C412C *arg0, Unk800C3F78 *arg1) {
    BtlUnit *u;
    Unk800F5758 *t;
    s32 idx;

    func_80063864(&arg0->unk0[1], &arg0->unk0[0], 0xC);
    arg0->unk0[1].unk8 = arg1->unk0;
    arg0->unk0[1].unk9 = arg1->unk1;
    if (arg1->unk2_14 != 0) {
        switch (arg1->unk0 & 0x1F) {
        case 1:
            u = D_800FE368.unk0;
            break;
        case 5:
            u = D_800FE368.unk4;
            break;
        default:
            goto other;
        }
        if (u->unk68 & 1) {
            t = &D_800F5758[((D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.w >> 4) & 0x1F) +
                            (arg1->unk2_14 - 1) * 19];
            arg0->unk0[1].unk0[3] = t->unk0 << 6;
            arg0->unk0[1].unk0[0] = t->unk1 << 4;
            arg0->unk0[1].unk0[1] = t->unk2 << 4;
            arg0->unk0[1].unk0[2] = t->unk3 << 4;
            return;
        }
    }
other:
    arg0->unk0[1].unk0[0] = arg1->unk2_0 << 5;
    arg0->unk0[1].unk0[1] = arg1->unk2_7 << 5;
    arg0->unk0[1].unk0[2] = arg1->unk4_0 << 5;
    arg0->unk0[1].unk0[3] = arg1->unk4_7 << 7;
}
