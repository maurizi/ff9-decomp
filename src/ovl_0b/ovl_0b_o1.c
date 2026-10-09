#include "ovl_0b.h"

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C2598);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C28F4);

u8 func_800C2DF4(void) {
    return D_800FE470;
}

void func_800C2E04(void) {
    D_800F78A4->unk2C = &D_800FE3B8;
    func_8001A878(&D_800FE3B8, 0xD0);
    D_800FE3B8.unk60 = 0x200;
    D_800FE3B8.unk5E = 0x200;
}

void func_800C2E4C(void) {
    func_8001A878(&D_800FE488, 0x64);
    func_80063864(D_800F78A4->unkC, &D_800FE368, 0x4C);
    D_800FE3B8.unk5C = 0;
    D_800FE3B8.unk53 = func_800CF690() & 1;
    if (D_800F78A4->unk0->unk1C & 4) {
        func_800C57A4(D_800FE368.unk44, D_800FE488.unk50);
    }
    func_800C57A4(D_800FE368.unk0, D_800FE488.unk46);
    func_800C57A4(D_800FE368.unk4, D_800FE488.unk5A);
}

void func_800C2F1C(s32 arg0) {
    D_800FE3B8.unkB9 = 0;
    D_800FE3B8.unk52 = 0;
    D_800FE3B8.unk5C = 0;
    func_8001A878(&D_800FE488, 0x64);
    func_8001A878(&D_800FE368, 0x4C);
    func_800C316C(arg0);
    if (D_800FE3B8.unkCC != 0) {
        func_800C57A4(D_800FE3B8.unkCC, D_800FE488.unk46);
    }
}

void func_800C2F9C(s32 arg0) {
    D_800FE3B8.unkB9 = 1;
    D_800FE3B8.unk52 = 0;
    D_800FE488.unk8 = 0;
    if (D_800FE3B8.unk5C & 2) {
        D_800FE3B8.unk5C = 0;
    }
    func_800C36D0();
    func_800C316C(arg0);
}

void func_800C2FFC(s32 arg0) {
    func_8001A878(&D_800FE488, 0x64);
    D_800FE3B8.unk53 = func_800CF690() & 1;
    func_800C57A4(D_8006794C->unk1C->unk8E4.battle->unk11C4.unk148, D_800FE488.unk46);
    func_800C57A4((BtlUnit *)D_8006794C->unk1C->unk8E4.battle->unk11C4.unk14C, D_800FE488.unk5A);
    func_800C2F9C(arg0);
    D_800FE3B8.unkB9 = 2;
    D_800FE3B8.unk5C = 0;
}

void func_800C30B8(void) {
    BtlUnit *u = D_800F78A4->unkC->unk0;
    if (u->unk68 & 1) {
        D_800FE484 = u;
    }
}

void func_800C30F4(void) {
    if (D_800FE488.unk45 == 0) {
        D_800FE3B8.unkA8 = D_800FE488.unk18;
        func_800C316C(func_800CA2B0(1));
        D_800FE3B8.unk5E = D_800FE3B8.unk60;
    }
    D_800FE3B8.unk5C = 1;
    D_800FE488.unk45 = 0;
}

void func_800C316C(s32 arg) {
    s16 *arg0 = (s16 *)arg;
    u16 *p[4];
    s32 f0;
    s32 flags;
    u16 *q;
    u16 *t;
    u8 *data;
    s32 i;
    s32 n;
    s32 r;
    BtlUnit *u;
    BtlUnit *w;
    BtlUnit9C8 *m;

    D_800FE488.unk4 = 1;
    f0 = flags = arg0[0];
    q = (u16 *)arg0 + 1;
    for (i = 0; i < 4; i++) {
        p[i] = NULL;
        if (flags & 1) {
            p[i] = (u16 *)((u8 *)arg0 + *q);
            q++;
        }
        flags >>= 1;
    }
    data = (u8 *)arg0 + *q;
    for (i = 0; i < 4; i++) {
        if (flags & 1) {
            func_80063864(data, &D_800FE3B8.unk0[i], 6);
            data += 6;
        } else {
            D_800FE3B8.unk0[i].vx = 0;
            D_800FE3B8.unk0[i].vy = 0;
            D_800FE3B8.unk0[i].vz = 0;
        }
        D_800FE3B8.unk30[i].vx = 0;
        D_800FE3B8.unk30[i].vy = 0;
        D_800FE3B8.unk30[i].vz = 0;
        flags >>= 1;
    }
    if (p[3] != NULL) {
        t = p[3];
        switch (*t >> 14) {
        case 0:
            D_800FE3B8.unkA8 = (*t >> 12) & 3;
            break;
        case 1:
            r = func_800CF708(0, 9);
            if (!(*t & 0x3000)) {
                if (r < ((*t >> 10) & 3) + 5) {
                    D_800FE3B8.unkA8 = 0;
                } else {
                    D_800FE3B8.unkA8 = 1;
                }
            } else {
                n = ((*t >> 10) & 3) + 3;
                if (r < n) {
                    D_800FE3B8.unkA8 = 0;
                } else if (r < n + 3) {
                    D_800FE3B8.unkA8 = 1;
                } else {
                    D_800FE3B8.unkA8 = 2;
                }
            }
            break;
        case 2:
            switch ((*t >> 12) & 3) {
            case 0:
                n = D_8006794C->unk1C->unk8E4.battle->unk132C & 3;
                if (n == 2) {
                    D_800FE3B8.unkA8 = n;
                } else {
                    D_800FE3B8.unkA8 = func_800CF708(0, 1);
                }
                D_800FE488.unk18 = D_800FE3B8.unkA8;
                break;
            case 1:
                D_800FE3B8.unkA8 = D_8006794C->unk1C->unk8E4.battle->unk131C;
                break;
            case 2:
                w = D_800FE368.unk4;
                n = 0;
                r = n;
                if (w != NULL) {
                    m = w->unk9C->unk8;
                    r = m->unk8;
                    n = m->unkA;
                }
                if (n == 0x4D7 || n >= 0xBB8) {
                    D_800FE3B8.unkA8 = 2;
                } else if (n < 0x65) {
                    D_800FE3B8.unkA8 = 0;
                } else {
                    D_800FE3B8.unkA8 = 1;
                    if (r < 0x159) {
                        D_800FE3B8.unkA8 = 0;
                    }
                    if (r >= 0x3E9) {
                        D_800FE3B8.unkA8 = 2;
                    }
                }
                break;
            case 3:
                if (D_800FE3B8.unkCC == NULL) {
                    D_800FE3B8.unkA8 = 0;
                } else if ((D_800FE3B8.unkCC->unk3C & 0x40001103) || !((D_800FDF30.unkA >> ((u8)D_800FE3B8.unkCC->unk68 >> 5)) & 1)) {
                    D_800FE3B8.unkA8 = 0;
                } else {
                    n = D_8006794C->unk1C->unk8E4.battle->unk132C & 3;
                    if (n == 2) {
                        D_800FE3B8.unkA8 = n;
                    } else {
                        r = 0;
                        for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                            if ((u->unk68 & 1) && !(u->unk3C & 0x40000100)) {
                                r++;
                            }
                        }
                        if (r < 3) {
                            D_800FE3B8.unkA8 = func_800CF708(0, 1) + 1;
                        } else {
                            r = func_800CF708(0, 5);
                            D_800FE3B8.unkA8 = 0;
                            if (r == 4) {
                                D_800FE3B8.unkA8 = 1;
                            }
                            if (r == 5) {
                                D_800FE3B8.unkA8 = 2;
                            }
                        }
                    }
                }
                break;
            }
            break;
        case 3:
            D_800FE3B8.unkA8 = D_800FE3B8.unkB8 & 3;
            if (D_800FE3B8.unkA8 == 2) {
                D_800FE3B8.unkA8 = 0;
            }
            if (D_800FE3B8.unkA8 == 3) {
                D_800FE3B8.unkA8 = 2;
            }
            break;
        }
    }
    D_800FE488.unk0 = p[D_800FE3B8.unkA8];
    if (f0 & 0x200) {
        D_800FE3B8.unkB8 = (f0 >> (D_800FE3B8.unkA8 * 2 + 10)) & 3;
    }
}

void func_800C36D0(void) {
    s32 i;
    s32 t;
    for (i = 0; i < 2; i++) {
        t = D_800FE3B8.unk68[i].unk1D != 0;
        if (t && (u32)(D_800FE3B8.unk68[i].unk18 - 2) < 2) {
            func_800C416C(1, &D_800FE3B8.unk68[i], &D_800FE3B8.unk68[i].unkC);
            D_800FE3B8.unk68[i].unk18 = 1;
        }
    }
}

void func_800C3770(void) {
    if (D_800FE3B8.unk5D == 0 && func_800C37BC() != 0) {
        func_800C37BC();
    }
    D_800FE3B8.unk5D = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C37BC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C3F78);

void func_800C412C(Unk800C412C *arg0, u8 *arg1) {
    if (arg0->unk1B == 0) {
        arg0->unk1C = arg0->unk1D = arg1[0];
        arg0->unk1A = arg1[2];
        arg0->unk19 = arg1[3];
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C416C);

void func_800C493C(s16 *arg0) {
    s32 t;
    if (D_800FE488.unk24 != 0) {
        D_800FE488.unk24--;
        t = func_800CF9DC(D_800FE488.unk2C, D_800FE488.unk28, D_800FE488.unk24, D_800FE488.unk20) * 2;
        if (t != 0) {
            if (!(D_800FE488.unk1C & 1)) {
                arg0[1] += func_800CF708(0, t) * 4 - t;
            }
            if (!(D_800FE488.unk1C & 2)) {
                arg0[0] += func_800CF708(0, t) * 4 - t;
                arg0[2] += func_800CF708(0, t) * 4 - t;
            }
        }
    }
}

void func_800C4A3C(s16 *arg0, s16 *arg1) {
    s32 dx = arg0[0] - arg1[0];
    s32 dz = arg0[2] - arg1[2];
    s32 d = func_800CF6B8(dx * dx + dz * dz, 0xF);
    u32 v = 0;
    if (D_800FE488.unk10 != 0) {
        v = ((func_8004BB80(D_800FE488.unk14 << 6) * d) >> 13) * (D_800FE488.unk10 & 0xF0) >> 11;
        D_800FE488.unk14 += D_800FE488.unk10 & 0xF;
    } else if (D_800FE3B8.unkAC != 0) {
        v = ((func_8004BB80(D_800FE3B8.unkB0 << 6) * d) >> 13) * (D_800FE3B8.unkAC & 0xF0) >> 11;
        D_800FE3B8.unkB0 += D_800FE3B8.unkAC & 0xF;
    }
    arg0[1] += v;
    arg1[1] += v;
}

void func_800C4B9C(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3) {
    s16 a[4];
    s16 b[4];
    s32 c;
    s32 t;
    s32 ang;
    BtlUnit *u;

    func_800C4DC4(arg0, arg1, a, b, &c);
    if (arg1 & 0x80) {
        c = arg2[3];
    } else {
        c += arg2[3];
    }
    if (arg1 & 0x40) {
        t = (func_8004BB7C(arg2[0]) * c) >> 12;
        arg3[0] = (-func_8004BB80(arg2[1]) * t) >> 12;
        arg3[2] = (-func_8004BB7C(arg2[1]) * t) >> 12;
        arg3[1] = (func_8004BB80(arg2[0]) * c) >> 12;
    } else {
        t = (func_8004BB7C(arg2[0] + b[0]) * c) >> 12;
        ang = arg2[1] + b[1];
        u = (BtlUnit *)func_800C5890(arg0 & 0x1F);
        if (u != NULL && (u->unk68 & 1)) {
            ang ^= 0xFFF;
        }
        arg3[0] = (-func_8004BB80(ang) * t) >> 12;
        arg3[2] = (-func_8004BB7C(ang) * t) >> 12;
        arg3[1] = (func_8004BB80(arg2[0] + b[0]) * c) >> 12;
        arg2[2] += b[2];
    }
    arg3[0] += a[0];
    arg3[1] += a[1];
    arg3[2] += a[2];
}

void func_800C4DC4(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3, s32 *arg4) {
    s16 a[4], b[4];
    s32 c[2];
    func_800C4E84(arg0, arg2, arg3, arg4);
    if (arg1 & 0x1F) {
        arg1 = (arg0 & 0xC0) | (arg1 & 0x3F);
        func_800C4E84(arg1, a, b, c);
        arg2[0] = (s16)(arg2[0] + a[0]) >> 1;
        arg2[1] = (s16)(arg2[1] + a[1]) >> 1;
        arg2[2] = (s16)(arg2[2] + a[2]) >> 1;
    }
}

s32 func_800C4E84(s32 arg0, s16 *arg1, s16 *arg2, s32 *arg3) {
    s32 n;
    BtlUnit *u;
    s16 *s;
    GameState *gs;

    *arg3 = 0;
    n = arg0 & 0x1F;
    arg1[0] = 0;
    arg1[1] = 0;
    arg1[2] = 0;
    arg2[0] = 0;
    arg2[1] = 0;
    arg2[2] = 0;
    if (n == 0) {
        return;
    }
    if (n >= 0x15) {
        n -= 0x15;
        arg1[0] = D_800FE3B8.unk0[n].vx;
        arg1[1] = D_800FE3B8.unk0[n].vy;
        arg1[2] = D_800FE3B8.unk0[n].vz;
        if (n < 4) {
            arg2[0] = D_800FE3B8.unk30[n].vx;
            arg2[1] = D_800FE3B8.unk30[n].vy;
            arg2[2] = D_800FE3B8.unk30[n].vz;
        }
        return;
    }
    if (n == 0xB) {
        gs = D_8006794C;
        arg1[0] = gs->unk1C->unk8E4.battle->unk1330->unk8[0].unk4;
        arg1[2] = gs->unk1C->unk8E4.battle->unk1330->unk8[0].unk8;
        return;
    }
    s = D_800FE488.unk46;
    u = NULL;
    if (n == 1) {
        u = D_800FE368.unk0;
    }
    if (n == 0xA) {
        if (D_800FE3B8.unkCC == NULL) {
            return;
        }
        u = D_800FE3B8.unkCC;
    }
    if (n == 0xC) {
        u = D_8006794C->unk1C->unk8E4.battle->unk11C4.unk148;
    }
    if (n == 0xF) {
        u = D_800FE368.unk44;
        s = D_800FE488.unk50;
    }
    if (u != NULL) {
        if (arg0 & 0x20) {
            arg2[0] = u->unkA0->unk20;
            arg2[1] = u->unkA0->unk22;
            arg2[2] = u->unkA0->unk24;
        } else {
            arg2[0] = u->unk86[0];
            arg2[1] = u->unk86[1];
            arg2[2] = u->unk86[2];
        }
    } else {
        s = D_800FE488.unk5A;
        u = (BtlUnit *)func_800C5890(n);
        if (u == NULL) {
            u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
        }
        arg2[0] = u->unkA0->unk20;
        arg2[2] = u->unkA0->unk24;
        if (u->unk68 & 1) {
            arg2[1] = 0x800;
        } else {
            arg2[1] = 0;
        }
    }
    if (!((u->unk68 >> 23) & 1)) {
        if (arg0 & 0x20) {
            arg1[0] = u->unk98->unk0[0] >> 16;
            arg1[2] = u->unk98->unk0[2] >> 16;
        } else {
            arg1[0] = u->unk8C[0] >> 16;
            arg1[2] = u->unk8C[2] >> 16;
        }
    } else {
        arg1[0] = s[3];
        arg1[2] = s[4];
    }
    if (n == 9) {
        arg1[0] = D_800FE368.unk38;
        arg1[2] = D_800FE368.unk3C;
    }
    if (n == 0xE) {
        gs = D_8006794C;
        arg1[0] = gs->unk1C->unk8E4.battle->unk11C4.unk150;
        arg1[2] = gs->unk1C->unk8E4.battle->unk11C4.unk154;
    }
    *arg3 = u->unk9C->unk8->unkA;
    arg1[1] = 0;
    if (n != 0xB) {
        arg0 &= 0xC0;
        if (arg0 == 0) {
            arg1[1] = s[1];
        }
        if (arg0 == 0x40) {
            arg1[1] = s[2];
        }
        if (arg0 == 0xC0) {
            arg1[1] = s[0];
        }
    }
    if ((u8)func_800C09A4(u, 0x200000)) {
        return (u16)arg1[1];
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C52B4);

void func_800C5530(SVECTOR *arg0, SVECTOR *arg1) {
    s32 a, b;
    if (D_800FE488.unkC != 0) {
        D_800FE488.unkC--;
    }
    if (D_800FE3B8.unkB4 != 0) {
        D_800FE3B8.unkB4--;
    }
    if ((u8)func_800EA094() != 1 && ((u8)func_800EA004() || D_800FE488.unkC != 0)) {
        if (D_800FE3B8.unkBA != 1) {
            D_800FE3B8.unkBA = 1;
            D_800FE3B8.unkB4 = 2;
            D_800FE488.unkC = 0;
            func_80063864(arg0, &D_800FE3B8.unkBC, 8);
            func_80063864(arg1, &D_800FE3B8.unkC4, 8);
        }
        if (D_800FE3B8.unkBA == 1 && ((D_800FE3B8.unkB9 == 1 && !(D_800F78A4->unk0->unk1C & 8)) || D_800FE3B8.unkB9 == 2)) {
            if (D_800FE3B8.unkBB != 2) {
                if (D_800FE3B8.unk65 != 0 || D_800FE3B8.unkB4 == 0) {
                    func_80063864(&D_800FE3B8.unk0[4], arg0, 8);
                    func_80063864(&D_800FE3B8.unk0[5], arg1, 8);
                    D_800FE3B8.unkBB = 2;
                } else {
                    a = D_800FE3B8.unkB4 * 0x555;
                    b = 0x1000 - a;
                    LoadAverageShort12(&D_800FE3B8.unkBC, &D_800FE3B8.unk0[4], a, b, arg0);
                    LoadAverageShort12(&D_800FE3B8.unkC4, &D_800FE3B8.unk0[5], a, b, arg1);
                }
            }
            D_8006794C->unk1C->unkA = D_800FE3B8.unk60;
            D_800FE3B8.unk66 = 1;
        }
    } else {
        D_800FE3B8.unkBA = 0;
    }
    D_800FE3B8.unk65 = D_800FE3B8.unkBB;
}

void func_800C57A4(BtlUnit *arg0, s16 *arg1) {
    s32 i;
    if (arg0->unk68 & 1) {
        s32 h = arg0->unk9C->unk8->unk8;
        s16 y = arg0->unk8C[1] >> 16;
        arg1[0] = y - (h >> 2);
        arg1[1] = y - (h >> 1);
        arg1[2] = y - (h - (h >> 2));
    } else {
        for (i = 0; i < 3; i++) {
            arg1[i] = arg0->unk9C->unk20[D_8006794C->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF].unk0->unk29[i]].unk18;
        }
    }
    arg1[3] = arg0->unk8C[0] >> 16;
    arg1[4] = arg0->unk8C[2] >> 16;
}

s32 func_800C5890(s32 arg0) {
    s32 ret = 0;
    if (arg0 == 5 || arg0 == 9) {
        ret = D_800FE36C;
    }
    if ((u32)(arg0 - 0xD) < 2) {
        ret = D_8006794C->unk1C->unk8E4.battle->unk11C4.unk14C;
    }
    return ret;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C58E4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C6528);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C67BC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C7730);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C7B84);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C7CC8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C7DAC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C86C8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C8A3C);
