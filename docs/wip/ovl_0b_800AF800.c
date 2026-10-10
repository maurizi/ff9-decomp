/* WIP func_800AF800: 17 differing rows. Left: case 0 0x1003 test gets fully cross-jumped into case 1
   (original shares only the done=0 tail); unk56F |= 8 is CSE'd with the earlier |= 0x40 (original reloads). */
#include "ovl_0b.h"

void func_800AF800(GameData *arg0, Battle *arg1) {
    s32 sp18;
    s32 done;
    BtlUnit *u;
    u32 exp;
    s32 sp18x;

    sp18 = arg1->unk11C0->unk34[arg0->unk8];
    done = 1;
    if (D_800F78A4->unk0->unk0 != -1 || arg1->unk1074.unk0 != NULL || arg1->unk1088 != NULL) {
        done = 0;
    }
    for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
        if (!((u->unk68 >> 22) & 1)) {
            func_800BC21C(u, &sp18);
            switch (arg1->unkD81) {
            case 0:
                func_800BD93C(u);
                if (!(u->unk68 & 1)) {
                    if (!(u->unk3C & 0x1003)) {
                        done = 0;
                    }
                } else if (u->unk3C & 0x100) {
                    if (u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[4]) {
                        done = 0;
                    }
                }
                break;
            case 1:
                if ((u->unk68 & 1) && u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[4] && !(u->unk3C & 0x1003)) {
                    done = 0;
                }
                break;
            case 2:
                func_800BD93C(u);
                if (u->unk68 & 1) {
                    func_800C0B6C(u, 0x02009400);
                }
                if (arg1->unk11BA != 0) {
                    done = 0;
                }
                if (u->unk3C & 0x100) {
                    if (u->unk7D == 0 && func_800B3E7C(u->unkC) == 0) {
                        func_800B1250(u->unkC, 0x3C, 0, u->unkB0, 0);
                    }
                    if (u->unk7D != 6) {
                        done = 0;
                    }
                } else if (!(u->unk3C & 0x02001003)) {
                    if (u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[0] &&
                        u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[1]) {
                        done = 0;
                    }
                }
                break;
            case 3:
                if (u->unk3C & 0x100) {
                    if (u->unk7D == 0) {
                        u->unk7D = 1;
                    }
                    if (u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[4]) {
                        done = 0;
                    } else if (func_800B3EF8(u, 0x3D) != 0) {
                        done = 0;
                    }
                } else if ((u->unk68 & 1) && !(u->unk3C & 0x42021143)) {
                    u->unk8C[2] += 0xFF9C0000;
                    func_800C1A8C(u, (s8)arg1->unkD84);
                    if ((s8)arg1->unkD84 <= 0) {
                        u->unk68 |= 0x400000;
                    } else {
                        done = 0;
                        if ((s8)arg1->unkD84 == 0x20) {
                            func_800AD90C(0xD000, 0xB5A, 0, 0x80, 0x7F);
                            func_800AD90C(0xD000, 0xB5B, 0, 0x80, 0x7F);
                            func_800AD90C(0xD000, 0xB5C, 0, 0x80, 0x7F);
                            arg1->unkD84 -= 2;
                        }
                    }
                }
                break;
            }
            func_800BFF20(u);
        }
        func_800BCEF0(u);
    }
    if (arg1->unkD81 == 3 && (s8)arg1->unkD84 < 0x20) {
        arg1->unkD84 -= 2;
    }
    if (done) {
        func_800E9F3C(0);
        switch (arg1->unkD81) {
        case 0:
        case 4:
            arg0->unk56E = 1;
            arg0->unk56F |= 0x40;
            if ((arg1->unk132C.unk0 >> 6) & 1) {
                if ((u8)func_800C20B8(arg0, 0x1E, 5) == 0) {
                    break;
                }
                for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
                    if (u->unk68 & 1) {
                        func_800C0B6C(u, 0xBFFFBC00);
                        if (!(u->unk3C & 0x1103)) {
                            if (u->unk28 != 0) {
                                u->unk86[1] = u->unkA0->unk22 = (u->unk68 & 1) * 0x800;
                                u->unk68 = (u->unk68 & ~0x40000) | ((func_800C09A4(u, 0x30202) & 1) << 18);
                                u->unk68 &= ~0x80000;
                                if (((D_8006794C->unk1C->unk3C + ((u->unk68 >> 1) & 0xF))->unk34.w >> 10) & 1) {
                                    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[0x12];
                                } else {
                                    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
                                }
                                u->unkA0->unk3A = 0;
                            } else {
                                func_800BEDBC(u, 0x100);
                            }
                        }
                    }
                }
                func_800C2F1C(func_800CA2B0(2));
            } else if ((arg1->unk132C.unk0 >> 14) & 1) {
                arg0->unk56F |= 8;
            }
            arg1->unkD80 = 6;
            break;
        case 1:
            arg0->unk56E = 3;
            arg1->unkD80 = 7;
            break;
        case 2:
            if ((arg1->unk132C.unk0 & 0x4040) == 0x4000) {
                arg0->unk56F |= 8;
            }
            arg1->unkD80 = 1;
            func_800B2F24(arg1);
            func_800B0EF8(arg1);
            for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
                func_800B10F8(u);
                if (!(u->unk68 & 1)) {
                    u->unkA0->unk22 = 0;
                    u->unk86[1] = 0;
                }
            }
            break;
        case 3:
            exp = D_800FDF30.unk0;
            arg0->unk56F |= 0x40;
            for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
                if (!(u->unk68 & 1)) {
                    exp += D_8006794C->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk44;
                }
            }
            if ((arg1->unk132C.unk0 & 0x4040) == 0x4000) {
                arg0->unk56F |= 8;
            }
            arg0->unk56E = 4;
            arg1->unkD80 = 8;
            func_800C1594();
            if (arg0->unk56F & 4) {
                exp /= 10;
                if (exp < arg0->unk560) {
                    arg0->unk560 -= exp;
                } else {
                    exp = arg0->unk560;
                    arg0->unk560 = 0;
                }
                func_800CD5C8(0x1E, exp);
            } else if ((u8)func_800B0CF8(1, 0x4000) != 0) {
                D_800FDF30.unk1D = 1;
                D_800FDF30.unk0 = exp / 10;
            }
            break;
        }
        if (arg1->unkD80 != 5) {
            if (arg1->unkD80 == 6 && !((arg1->unk132C.unk0 >> 6) & 1)) {
                arg1->unkD81 = 1;
            } else {
                arg1->unkD81 = 0;
            }
        }
    } else {
        func_800B1764(arg1);
    }
    func_800B05C0(arg0, arg1, sp18);
}
