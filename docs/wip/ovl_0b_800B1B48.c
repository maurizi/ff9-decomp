/* WIP func_800B1B48: 3 differing rows. Left: the inline's return-0 block; the original keeps it
   (bnez over j) and the later return-0 paths (case 0x38/0x3C) jump to it, ours sends them to the fail block's.
   Why (jump.c cross-jumping in jump2): jumps are processed forward and the *processed* jump is the one replaced.
   The inline's `v0 = 0; j epi` is preceded by a conditional jump around it, and find_cross_jump counts that as a
   match, so with just `v0 = 0` in common it reaches minimum 2 and merges into the first chain candidate (chain
   head = the last jump to the epilogue: case 0x3E's tail, or the fail block). In the original the inline's block
   found no partner, so it must have looked different at jump2 time. Tried: `fail: ...; goto ret0` with a ret0
   label on the inline's return (6 rows), case 0x3C as one if/else ending in `return 0` (3 rows, inline then merges
   into case 0x3E's tail), explicit `goto ret0` from 0x38/0x3C (11 rows). */
#include "ovl_0b.h"

extern u8 D_8007AF01;

/* the command's caster can't act (status 8 with flag 2): prints message 0x15 */
static inline u8 func_800B1B48_can(BtlCmd *arg1) {
    if ((arg1->unk4->unk3C & 8) && (arg1->unk8->unk8 & 2)) {
        func_800CD5C8(0x15);
        return 0;
    }
    return 1;
}

s32 func_800B1B48(Battle *arg0, BtlCmd *arg1) {
    BtlUnit *u;
    BtlUnit *t;
    BtlCmd *m;
    BtlCmd8 *c;
    u32 a;
    u32 g;
    s32 r;
    s32 k;
    s32 msg;
    u8 st;
    s32 v;

    u = arg1->unk4;
    if (u != NULL) {
        if (arg1->unkE < 0x37) {
            v = (u->unk68 & 1) << 11;
            u->unkA0->unk22 = v;
            u->unk86[1] = v;
        }
        if (D_8007AF01 != 0 && (u->unk68 & 0x1F) == 5 && arg1->unkE < 0x30) {
            if (func_80038D3C_rand() < 0x40) {
                msg = 0x1B;
                goto fail;
            }
        }
        if (func_800B1B48_can(arg1) == 0) {
            return 0;
        }
    }
    switch (arg1->unkE) {
    case 1:
        if (((arg0->unk132C.unk0 >> 8) & 1) && (*u->unk74 & 1) && (u16)arg1->unkC >= 0x10) {
            msg = 0x17;
            goto fail;
        }
        return 1;
    case 3:
    case 0xC:
        func_800BEDBC(u, 0x40000000);
        u->unk10->unkE = arg1->unkE;
        m = u->unk10;
        if (arg1->unkE == 3) {
            m->unkC = arg1->unkC;
        } else {
            m->unkC = func_800C1CA0(1, 0);
        }
        arg1->unkC = u->unkB0;
        break;
    case 0xB:
        u->unk10->unkC = func_800C1CA0(1, 0);
        break;
    case 0xE:
    case 0xF:
    case 0x33:
        if (func_80027F34(arg1->unkF) == 0) {
            msg = 0x18;
            goto fail;
        }
        if (arg1->unkE == 0x33) {
            func_800CD5C8(9);
        }
        return 1;
    case 0x10:
    case 0x12:
    case 0x14:
        func_800B989C(arg1);
        return 1;
    case 0x16:
    case 0x17:
        if (arg1->unkF == 0x2E) {
            func_800B95BC(arg1);
        }
        return 1;
    case 0x18:
        if (arg1->unkF == 0x52) {
            arg1->unkC = func_800C1F7C(func_80038D3C_rand() & 1);
        } else if (arg1->unkF == 0x5D) {
            a = func_80027F34(arg1->unk8->unk5);
            g = func_800C1F00(1);
            msg = 0x18;
            if (a >= g) {
                break;
            }
            goto fail;
        } else {
            return 1;
        }
        break;
    case 0x1C:
    case 0x1D:
        if (arg1->unkF == 0x7E || arg1->unkF == 0x86) {
            g = arg1->unk8->unk5 * u->unk7A;
            if (D_8006794C->unk1C->unk560 < g) {
                msg = 0x14;
                goto fail;
            }
            D_8006794C->unk1C->unk560 -= g;
        } else if (arg1->unkF == 0x81 || arg1->unkF == 0x89) {
            c = arg1->unk8;
            r = func_80038D3C_rand();
            c->unk6 = 1 << (r % 8);
        } else {
            return 1;
        }
        break;
    case 0x1F:
        return (u8)func_800B96AC(u, arg1->unk8->unkA);
    case 0x31:
        t = func_800BED10(arg1->unkC);
        if (t == NULL || !((t->unk68 >> 28) & 1) || (t->unk3C & 0x100)) {
            return 0;
        }
    case 0x34:
        if ((arg0->unk132C.unk0 >> 8) & 1) {
            msg = 0x17;
            if (*u->unk74 & 1) {
            fail:
                func_800CD5C8(msg);
                return 0;
            }
        }
        if (arg1->unkE == 0x31) {
            func_800CD5C8(7);
        }
        return 1;
    case 0x32:
        t = func_800BED10(arg1->unkC);
        if (t == NULL || !((t->unk68 >> 28) & 1) || (t->unk3C & 0x100)) {
            return 0;
        }
        func_800CD5C8(8);
        return 1;
    case 0x38:
        if (arg0->unkD80 != 4) {
            return 1;
        }
        for (t = arg0->unk6F8; t != NULL; t = t->unk0) {
            if ((t->unk68 & 1) && !(t->unk3C & 0x42021143) && t->unk28 != 0) {
                if (t->unk9C->unkC != (BtlUnit9CC *)t->unkA4[0x11]) {
                    t->unk9C->unkC = (BtlUnit9CC *)t->unkA4[0x11];
                    t->unkA0->unk3A = 0;
                }
                arg0->unkD80 = 5;
                arg0->unkD81 = 3;
            }
        }
        if (*(u16 *)&arg0->unkD80 == 0x305) {
            func_800E9F68(0);
            D_8006794C->unk1C->unk55C++;
            if (arg1->unkF == 0) {
                D_8006794C->unk1C->unk56F |= 4;
            }
            func_800B2F24(arg0);
        } else {
            arg0->unk11B8 &= ~1;
        }
        return 0;
    case 0x39:
        if (arg1->unkF != arg0->unk11BB) {
            arg1->unkF = arg0->unk11BB;
            arg1->unk8 = (BtlCmd8 *)&arg0->unk16CC[arg1->unkF];
        }
        break;
    case 0x3B:
        if (u->unk3C & 0x4000) {
            func_800CD5C8(6);
            u->unk84 = D_800F5200[(u->unk68 & 1) ? D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.b.unk4 + 0x13 : 0x26];
        } else {
            u->unk84 = D_800F5200[(u->unk68 & 1) ? (D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.w >> 4) & 0x1F : 0x13];
        }
        break;
    case 0x3C:
        func_800C15DC(u);
        if (u->unk7D != 0) {
            return 0;
        }
        if (u->unk68 & 1) {
            if (u->unk9C->unkC == (BtlUnit9CC *)u->unkA4[4]) {
                u->unk7D = 5;
                return 0;
            }
            u->unk7D = 1;
        } else if ((u->unk68 & 0x900000) == 0x100000) {
            u->unk7D = 3;
        } else {
            u->unk7D = 1;
        }
        return 0;
    case 0x3D:
        u->unk28 = 1;
        func_800BF558(u, 0x100);
        u->unk68 |= 0x20000;
        return 0;
    case 0x3E:
        func_800C0A70(u, 1);
        u->unk3C |= 1;
        u->unk68 &= ~0x10000;
        func_800C0EC8(u);
        if (u->unk68 & 1) {
            func_800C0544(u);
        }
        func_800BF558(u, 0x80000000);
        func_800C0430(u, 1);
        return 0;
    }
    return 1;
}
