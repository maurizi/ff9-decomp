#include "ff9.h"

s32 func_800551F0(void) {
    akao_init();
    return 0;
}

s32 func_80055210(void) {
    akao_quit();
    return 0;
}

s32 func_80055230(u8 *a) {
    s32 r;
    u8 *p;

    r = func_800565DC((s32 *)a);
    if (r == 0) {
        p = a + 0x10;
        D_800809DC = (u16 *)p;
        p = a + 0x210;
        D_800809E4 = (u16 *)p;
        p = a + 0x310;
        D_80083148 = (u16 *)p;
        p = a + 0x410;
        D_800809E8 = p;
    }
    return r;
}

s32 func_80055290(s32 a) {
    g_akaoArgs.unk0 = a;
    return akao_command(0x10);
}

void func_800552B8(s32 a) {
    g_akaoArgs.unk0 = a;
    akao_command(0x11);
}

void func_800552E0(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    akao_command(0x14);
}

void func_80055310(void) {
    akao_command(0x40);
}

s32 func_80055330(void) {
    if (D_8007F790 == 0) {
        D_8007F95A = 0;
    }
    return 0;
}

void func_80055350(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0x7F;
    akao_command(0x19);
}

void func_80055384(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    g_akaoArgs.unk8 = c & 0x7F;
    akao_command(0x1A);
}

s32 func_800553BC(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    return akao_command(0x12);
}

s32 func_800553EC(s32 a, s32 b, s32 c, s32 d) {
    s32 r;

    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c & 0xFF;
    g_akaoArgs.unkC = d & 0x7F;
    if ((u32)a >= 0x400) {
        r = func_800565DC((s32 *)a);
        if (r != 0) {
            return r;
        }
        akao_command(0x24);
        return a;
    }
    return akao_command(0x20);
}

void func_8005546C(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    akao_command(0x21);
}

void func_800554A8(s32 a) {
    g_akaoArgs.unk0 = a & 0x3FF;
    akao_command(0x30);
}

u32 func_800554D4(void) {
    u32 mask = D_80080A70.unk0;
    AkaoChannel *c;
    u32 r;
    u32 bit;

    if (mask == 0) {
        return 0;
    }
    c = D_8007E3D8;
    r = 0;
    for (bit = 0x100; bit & 0xFFFFFF; bit <<= 1) {
        if (mask & bit) {
            r |= c->unk28;
        }
        c++;
    }
    r &= 0xFFFFFF;
    return r;
}

s32 func_8005553C(u32 id) {
    u32 mask;
    u32 bit;
    AkaoChannel *c;
    if (id == 0) {
        return 0;
    }
    mask = D_80080A70.unk0;
    if (mask == 0) {
        return 0;
    }
    c = D_8007E3D8;
    for (bit = 0x100; bit & 0xFFFFFF; bit <<= 1) {
        if ((mask & bit) && id == c->unk28) {
            return 1;
        }
        c++;
    }
    return 0;
}

void func_800555A8(s32 a) {
    if (a == 1) {
        a = 0x81;
    } else {
        a = 0x80;
    }
    akao_command(a);
}

void func_800555DC(s32 a) {
    g_akaoArgs.unk0 = a;
    akao_command(0x90);
}

void func_80055604(s32 a) {
    g_akaoArgs.unk0 = a;
    akao_command(0x92);
}

void func_8005562C(u32 x) {
    s32 cmd;
    switch (x) {
    case 1:
        cmd = 0x9B;
        break;
    case 2:
        cmd = 0x9D;
        break;
    case 3:
        cmd = 0x9F;
        break;
    default:
        cmd = 0x99;
        break;
    }
    akao_command(cmd);
}

void func_80055698(u32 x) {
    s32 cmd;
    switch (x) {
    case 1:
        cmd = 0x9A;
        break;
    case 2:
        cmd = 0x9C;
        break;
    case 3:
        cmd = 0x9E;
        break;
    default:
        cmd = 0x98;
        break;
    }
    akao_command(cmd);
}

void func_80055704(s32 a) {
    g_akaoArgs.unk0 = a & 0x7F;
    akao_command(0xA8);
}

void func_80055730(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0x7F;
    akao_command(0xA9);
}

void func_80055764(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c & 0x7F;
    akao_command(0xA0);
}

void func_800557A8(s32 a, s32 b, s32 c, s32 d) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c;
    g_akaoArgs.unkC = d & 0x7F;
    akao_command(0xA1);
}

void func_800557F0(s32 a) {
    g_akaoArgs.unk0 = a & 0xFF;
    akao_command(0xAA);
}

void func_8005581C(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    akao_command(0xAB);
}

void func_80055850(s32 a, s32 b, u8 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c;
    akao_command(0xA2);
}

void func_80055894(s32 a, s32 b, s32 c, u8 d) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c;
    g_akaoArgs.unkC = d;
    akao_command(0xA3);
}

void func_800558DC(s32 a) {
    g_akaoArgs.unk0 = a & 0xFF;
    akao_command(0xAC);
}

void func_80055908(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    akao_command(0xAD);
}

void func_8005593C(s32 a, s32 b, u8 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c;
    akao_command(0xA4);
}

void func_80055980(s32 a, s32 b, s32 c, u8 d) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c;
    g_akaoArgs.unkC = d;
    akao_command(0xA5);
}

void func_800559C8(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0x7F;
    akao_command(0xC0);
}

void func_800559FC(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    g_akaoArgs.unk8 = c & 0x7F;
    akao_command(0xC1);
}

void func_80055A34(s32 a, s32 b, s32 c, s32 d) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    g_akaoArgs.unk8 = c & 0x7F;
    g_akaoArgs.unkC = d & 0x7F;
    akao_command(0xC2);
}

void func_80055A74(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0x7F;
    akao_command(0xC4);
}

void func_80055AA8(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    g_akaoArgs.unk8 = c & 0x7F;
    akao_command(0xC5);
}

void akao_setCdVolume(s32 a) {
    g_akaoArgs.unk0 = a;
    akao_command(0x70);
}

void akao_fadeCdVolume(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    akao_command(0x71);
}

void akao_fadeCdVolumeFrom(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    g_akaoArgs.unk8 = c;
    akao_command(0x72);
}

void func_80055B6C(s32 a) {
    g_akaoArgs.unk0 = a & 0xFF;
    akao_command(0xD0);
}

void func_80055B98(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    akao_command(0xD1);
}

void func_80055BCC(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    g_akaoArgs.unk8 = c & 0xFF;
    akao_command(0xD2);
}

void func_80055C08(s32 a) {
    g_akaoArgs.unk0 = a & 0xFF;
    akao_command(0xD4);
}

void func_80055C34(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    akao_command(0xD5);
}

void func_80055C68(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    g_akaoArgs.unk8 = c & 0xFF;
    akao_command(0xD6);
}

void func_80055CA4(s32 a) {
    g_akaoArgs.unk0 = a & 0xFF;
    akao_command(0xD8);
}

void func_80055CD0(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    akao_command(0xD9);
}

void func_80055D04(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    g_akaoArgs.unk8 = c & 0xFF;
    akao_command(0xDA);
}

void func_80055D40(void) {
    akao_command(0xF0);
}

void func_80055D60(void) {
    akao_command(0xF1);
}

void func_80055D80(s32 a, s32 b) {
    while (func_80056700(a, b) == 1) {
    }
}

s32 func_80055DCC(void) {
    s32 a[2];
    do {
        a[0] = D_800809E0;
        a[1] = D_800809E0;
    } while (a[0] != a[1]);
    return a[0];
}

s32 func_80055DF4(void) {
    D_8008322C = 0;
    D_80083158.unk0 |= 1;
    return 0;
}

s32 func_80055E14(s32 *buf, u32 size, s32 wait) {
    u32 n;
    u32 m;
    s32 slot;

    if (D_80083158.unk0 & 1) {
        if (D_80083228.unk4 == 0) {
            if (buf[0] == 0x4F414B41) {
                func_8005CE04(buf, D_80080A30, 0x40);
                buf += 16;
                size -= 0x40;
                D_80083228.unk4 = D_80080A30[4];
                D_80080A30[6] = slot = D_80080A30[6];
                D_80083228.unk8 = D_80080A30[5];
                D_80083228.unk0 = &D_8007F970[slot];
                D_80083228.unkC = D_80080A30[7] * 16;
            } else {
                size = 0;
                D_80083228.unk8 = 0;
                D_80083228.unkC = 0;
            }
        }
        if (D_80083228.unkC != 0) {
            n = D_80083228.unkC;
            if (size == 0) {
                goto end;
            }
            if (n >= size) {
                n = size;
            }
            func_8005CE04(buf, D_80083228.unk0, n);
            buf += n >> 2;
            size -= n;
            D_80083228.unk0 = (Unk8005EBB4 *)((s32 *)D_80083228.unk0 + (n >> 2));
            D_80083228.unkC -= n;
            if (D_80083228.unkC == 0) {
                func_8005658C((s32 *)&D_8007F970[D_80080A30[6]], &D_8007F970[D_80080A30[6]], D_80080A30[4], D_80080A30[7]);
            }
        }
        if (size != 0) {
            if (D_80083228.unk8 == 0) {
                goto clear;
            }
            m = D_80083228.unk8;
            if (m >= size) {
                m = size;
            }
            size = m;
            SpuSetTransferStartAddr(D_80083228.unk4);
            func_80056644(buf, size);
            D_80083228.unk4 += size;
            D_80083228.unk8 -= size;
            if (wait != 0) {
                func_800566D0();
            }
        }
    end:
        if (D_80083230 == 0) {
        clear:
            D_80083158.unk0 &= ~1;
        }
    }
    return D_80083230;
}

void func_80056028(s32 a, s32 b) {
    func_80055D80(a, b);
}

s32 func_80056048(s32 *a, s32 b, s32 c) {
    s32 *h = a;
    s32 y;
    s32 id;
    u32 i;

    for (i = 0; i < 7; i++) {
        if (D_800809C0[i] == h[1]) {
            D_800809C0[i] = 0;
        }
    }
    i = h[6] + b * 8;
    y = h[4];
    if (b != 0) {
        id = h[1];
        if ((u32)(id - 500) < 100) {
            y += b << 12;
        } else if ((u32)(id - 200) < 100) {
            y += (b * 5) << 12;
        } else if ((u32)(id - 300) < 200) {
            y += b << 14;
            b += 4;
        }
    }
    D_800809C0[b] = h[1];
    func_80056750(a, c, i, y);
    return 0;
}

s32 func_80056130(s32 vol) {
    u8 v;

    if (D_8008315C & 2) {
        CdlATV *atv = &D_8007F718;

        v = (u32)(vol * 46448) >> 17;
        atv->val3 = v;
        atv->val1 = v;
        atv->val2 = v;
        D_8007F718.val0 = v;
    } else {
        D_8007F718.val2 = vol;
        D_8007F718.val0 = vol;
        D_8007F718.val3 = 0;
        D_8007F718.val1 = 0;
    }
    CdMix(&D_8007F718);
    return 0;
}

void func_800561C4(s32 *a, s32 b, s32 c) {
    if (func_800565DC(a) == 0) {
        g_akaoArgs.unk0 = (s32)a;
        g_akaoArgs.unk4 = (b & 0xFF) << 8;
        g_akaoArgs.unk8 = c;
        akao_command(0xE0);
    }
}

void func_80056228(void) {
    akao_command(0xE2);
}

void func_80056248(s32 a) {
    g_akaoArgs.unk0 = (a & 0x7F) << 8;
    akao_command(0xE4);
}

void func_80056278(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = (b & 0x7F) << 8;
    akao_command(0xE5);
}

void func_800562B0(s32 a) {
    g_akaoArgs.unk0 = (a & 0xFF) << 8;
    akao_command(0xE6);
}

s32 func_800562E0(s32 *a, s32 b) {
    s32 r = func_800565DC(a);
    s32 addr;
    s32 *h;

    if (r == 0) {
        func_800566D0();
        addr = 0x59100;
        if (b == 0) {
            addr = 0x4F100;
        }
        h = a;
        a += 0x10;
        SpuSetTransferStartAddr(addr);
        func_80056644(a, h[4]);
        h[8] = addr;
        func_8005CE04(h, D_8007F720, 0x70);
    } else {
        D_8007F740 = 0;
    }
    return r;
}

void func_80056388(s32 a, s32 b) {
    g_akaoArgs.unk0 = (a & 0xFF) << 8;
    g_akaoArgs.unk4 = b;
    akao_command(0xED);
}

void func_800563C0(s32 *a, s32 b, s32 c, s32 d) {
    s32 x;
    if (func_800565DC(a) == 0) {
        x = 0x59100;
        if (c == 0) {
            x = 0x4F100;
        }
        g_akaoArgs.unk0 = (s32)a;
        g_akaoArgs.unk4 = (b & 0xFF) << 8;
        g_akaoArgs.unk8 = x;
        g_akaoArgs.unkC = d;
        akao_command(0xEC);
    }
}

s32 func_80056448(s32 a, s32 b) {
    if (b == 0) {
        return -1;
    }
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    akao_command(0xE8);
    return 0;
}

s32 func_80056488(void) {
    D_80083168.unk24++;
    D_80083168.unk38++;
    if (D_80083168.unk3C - 1 < D_80083168.unk38) {
        D_80083168.unk38 = 0;
    }
    if ((D_80083168.unk8 & 0x1000000) && D_80083168.unk38 >= 2) {
        func_80061C1C();
    }
    return D_80083168.unk34;
}
