#include "ovl_0c.h"

void func_800C4A5C(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        D_800D4870[i].vx = i << 7;
        D_800D4870[i].vy = rand() % 4096;
        D_800D4870[i].vz = 0;
        D_800D4830[i] = i * 0x14 + 0x200;
        D_800D48F8[i] = 0x800;
        D_800D48E8[i] = 0;
        D_800D4910[i].vx = rand() % 4096;
        D_800D4910[i].vy = rand() % 4096;
        D_800D4910[i].vz = 0;
    }
    for (i = 0; i < 9; i++) {
        D_800D4960[i].vx = 0;
        D_800D4960[i].vy = 0;
        D_800D4960[i].vz = 0;
        D_800D49F0[i].vx = 0;
        D_800D49F0[i].vy = 0;
        D_800D49F0[i].vz = 0;
    }
    setVector(&D_800D4960[0], 0x4464C, -0x1E68, -0x32681);
    setVector(&D_800D4960[1], 0x3805B, 0x1F4, -0x30847);
    setVector(&D_800D4960[6], 0x2FFFA, -0x2A82, -0x14052);
    D_800D4960[2] = D_800D4960[1];
    D_800D4960[3] = D_800D4960[1];
    D_800D4960[4] = D_800D4960[1];
    D_800D4960[5] = D_800D4960[1];
    D_800D4960[7] = D_800D4960[6];
    D_800D4960[8] = D_800D4960[6];
    D_800D4848.unk4 = NULL;
    D_800D4848.unk8 = &D_800D4A40[0];
    D_800D4A40[0].unk4 = &D_800D4848;
    D_800D4A40[0].unk8 = &D_800D4A40[1];
    for (i = 1; i < 59; i++) {
        D_800D4A40[i].unk4 = &D_800D4A40[i - 1];
        D_800D4A40[i].unk8 = &D_800D4A40[i + 1];
    }
    D_800D4A40[i].unk4 = &D_800D4A40[i - 1];
    D_800D4A40[i].unk8 = &D_800D53E8;
    D_800D53E8.unk4 = &D_800D4A40[i];
    D_800D53E8.unk8 = NULL;
    D_800D53A0.unk4 = NULL;
    D_800D53A0.unk8 = &D_800D48C0;
    D_800D48C0.unk4 = &D_800D53A0;
    D_800D48C0.unk8 = NULL;
    D_800CDF80.vy = 0;
    D_800CDF88.vy = 0;
    D_800CDF90.vy = 0;
    D_800CDFA0[0].vy = 0;
    D_800CDFA0[1].vy = 0;
    D_800CDFA0[2].vy = 0;
    D_800D4A38 = 0;
    D_800D53CC = 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800C4A5C", D_800A849C);

void func_800C4DD8(void) {
    WmNode *o;
    VECTOR a;
    VECTOR b;
    SVECTOR pos;
    s32 tex;
    s32 k;

    o = D_800D53A0.unk8;
    SetRotMatrix(D_800CF448);
    SetTransMatrix(D_800CF448);
    while (o->unk8 != NULL) {
        if ((u8)func_800BBE24((VECTOR *)&o->unk10, &a, &b, 0)) {
            pos.vx = a.vx;
            pos.vy = a.vy;
            pos.vz = a.vz;
            if (b.vz > 1000) {
                tex = D_800CF460[*D_800CF45C];
                switch ((u8)o->unkD) {
                case 2:
                    D_800CF442 = 0;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, NULL, o->unkE = o->unk1C * 400 + 6000,
                                               (s16)((s16)(o->unk1C / 4) % 26), (u8)o->unkC, -1, tex, 0, 2);
                    break;
                case 3:
                    D_800CF442 = 0;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, NULL, o->unkE = o->unk1C * 400 + 6000,
                                               (s16)((s16)(o->unk1C / 2) % 23), (u8)o->unkC, -1, tex, 0, 2);
                    break;
                case 8: {
                    SVECTOR r = D_800A849C;
                    D_800CF442 = 20;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, &r, o->unkE, o->unk1C, (u8)o->unkC,
                                               -1, tex, 0, 0);
                    break;
                }
                case 15:
                case 22: {
                    SVECTOR r = D_800A849C;
                    D_800CF442 = 20;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, &r, o->unkE, o->unk1C, (u8)o->unkC,
                                               D_800D4A38, tex, 0, 0);
                    break;
                }
                case 16: {
                    SVECTOR r = D_800A849C;
                    pos.vy -= 400;
                    o->unkE = (8 - o->unk1C) * 4000;
                    r.vy = o->unk1C * 128;
                    D_800CF442 = 0;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, &r, o->unkE, 0, 1, -1, tex, 0, 0);
                    r.vy = o->unk1C * 200;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, &r, o->unkE, 0, 2, -1, tex, 0, 0);
                    break;
                }
                case 14:
                case 18:
                case 19:
                    if (D_800D53E0 >= 0x1000) {
                        D_800D53E0 = 0xFFF;
                    }
                    if (D_800D53E0 < 0) {
                        D_800D53E0 = 0;
                    }
                    D_800D53CC += (D_800D53E0 - D_800D53CC) / 32;
                    D_800CF442 = 50;
                    k = D_800D4A38 * D_800D53CC >> 12;
                    if (k >= 25) {
                        D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, NULL, o->unkE, o->unk1C,
                                                   (u8)o->unkC, k, tex, 0, 0);
                    }
                    break;
                case 25:
                case 26: {
                    SVECTOR r;
                    memset(&r, 0, 8);
                    r.pad = 1;
                    r.vy = ratan2(0x30031 - o->unk10, -0x13F6E - o->unk18);
                    D_800CF442 = 0;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, &r, o->unkE, o->unk1C, (u8)o->unkC,
                                               -1, tex, 0, 0);
                    break;
                }
                default:
                    D_800CF442 = 40;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, NULL, o->unkE, o->unk1C, (u8)o->unkC,
                                               -1, tex, 0, 0);
                    break;
                }
            }
        }
        o = o->unk8;
    }
}

void func_800C53DC(void) {
    WmNode *o;
    WmNode *cur;
    s16 spd;
    u8 flag;

    flag = 0;
    o = D_800D53A0.unk8;
    spd = D_800CE3C8->unk2;
    if (spd != 0) {
        D_800D53C8 = abs((D_800CE3C0 << 12) / spd);
    } else {
        D_800D53C8 = 0;
    }
    D_800D4A38 += (D_800D53C8 - D_800D4A38) / 32;
    while (o->unk8 != NULL) {
        /* dead flag: combine folds the test (flag is only ever 0) and jump2 drops the
         * body after register allocation, which leaves s7 = 0 and its save behind */
        if (flag) {
            func_800C7050(o);
        }
        switch ((u8)o->unkD) {
        case 2:
            o->unk14 -= 60 + wm_rand8() % 24;
            o->unk10 -= 30 - wm_rand8() % 60;
            o->unk18 -= 30 - wm_rand8() % 60;
            break;
        case 3:
            o->unk14 -= 60 + wm_rand8() % 24;
            o->unk10 -= 60 - wm_rand8() % 60;
            o->unk18 -= 30 - wm_rand8() % 60;
            break;
        case 23:
            o->unk10 = math_sin((o->unk1C << 6) + o->unk20) / 2 + D_800CDF50.vx;
            o->unk14 -= 80;
            o->unk18 = math_cos((o->unk1C << 6) + o->unk20) / 2 + D_800CDF50.vz;
            /* fallthrough */
        case 24:
            o->unk10 -= 40 + wm_rand8() % 130;
            o->unk14 -= 40 - wm_rand8() % 80;
            o->unk18 += 300 + wm_rand8() % 300;
            break;
        }
        o->unk1C++;
        cur = o;
        o = o->unk8;
        switch ((u8)cur->unkD) {
        case 2:
            if (cur->unk1C >= 77) {
                func_800C7050(cur);
            }
            break;
        case 3:
            if (cur->unk1C >= 46) {
                func_800C7050(cur);
            }
            break;
        case 16:
        case 17:
            if (cur->unk1C >= 9) {
                func_800C7050(cur);
            }
            break;
        case 32:
            if (abs(cur->unk10 - D_800CE3DC->pos.vx) > 32000 || abs(cur->unk18 - D_800CE3DC->pos.vz) > 32000) {
                func_800C7050(cur);
            }
            break;
        default:
            if (cur->unk1C == func_800CAE00((u16 *)cur->unk0)) {
                func_800C7050(cur);
            }
            break;
        }
    }
}

void func_800C5A48(u8 arg0) {
    VECTOR v;
    MATRIX m;
    s32 i;
    s32 j;

    SetLightMatrix(&D_800CF4C0);
    SetColorMatrix(&D_800CF560);
    SetBackColor((u8)((u16)D_800D3FA0.unk5E0[0].unk54[0] >> 5), (u8)((u16)D_800D3FA0.unk5E0[0].unk54[1] >> 5),
                 (u8)((u16)D_800D3FA0.unk5E0[0].unk54[2] >> 5));
    switch (arg0) {
    case 4:
        if (D_800D30CD == 0 && g_worldDataSet == 1) {
            VECTOR scale;

            func_80030AF4(&D_800CDF80, &m);
            if ((u8)func_800BBE24(&D_800CDF50, (VECTOR *)m.t, &v, 0)) {
                scale = D_800A8588;
                ScaleMatrix(&m, &scale);
                CompMatrix(D_800CF448, &m, &m);
                SetRotMatrix(&m);
                SetTransMatrix(&m);
                func_800BA458(WM_PACK0(0x3A), 0x50, 1);
                func_800BA458(WM_PACK0(0x35), 0x3C, 0);
                for (j = 0; j < 3; j++) {
                    func_80030AF4(&D_800CDFA0[j], &m);
                    func_800BBE24(&D_800CDF50, (VECTOR *)m.t, &v, 0);
                    CompMatrix(D_800CF448, &m, &m);
                    SetRotMatrix(&m);
                    SetTransMatrix(&m);
                    func_800BA458(WM_PACK0(j + 0x36), 0, 0);
                }
            }
        }
        break;
    case 7:
        func_80030AF4(&D_800CDF48, &m);
        func_800BBE24(&D_800CDF38, (VECTOR *)m.t, &v, 0);
        CompMatrix(D_800CF448, &m, &m);
        SetRotMatrix(&m);
        SetTransMatrix(&m);
        func_800BA458(WM_PACK0(0x39), 0, 1);
        break;
    case 6:
        if (g_worldDataSet == 4) {
            SVECTOR pos;
            u32 *ot;
            WmSprHdr *spr;
            s32 n;

            ot = (u32 *)D_800CF460[*D_800CF45C];
            func_800BBE24(&D_800CDF60, (VECTOR *)m.t, &v, 0);
            D_800CF442 = 0;
            pos.vx = m.t[0];
            pos.vy = m.t[1];
            pos.vz = m.t[2];
            spr = w_framePackGetPtr(D_800CF428, 0x25);
            n = func_800CAE00((u16 *)spr) - 1;
            if (n != 0) {
                D_800CF404 = func_800CAE0C(D_800CF404, spr, &pos, NULL, D_800CDFBC, D_800CF4B0 / 2 % n, 1, -1,
                                           (s32)ot, 0, 0);
            }
            func_800BBE24(&D_800CDF60, (VECTOR *)m.t, &v, 0);
            CompMatrix(D_800CF448, &m, &m);
            func_80030AF4(&D_800CDF98, &m);
            SetRotMatrix(&m);
            SetTransMatrix(&m);
            func_800BA458(WM_PACK0(0x3F), D_800CDFB8, 2);
            func_80030AF4(&D_800CDF88, &m);
            func_800BBE24(&D_800CDF60, (VECTOR *)m.t, &v, 0);
            CompMatrix(D_800CF448, &m, &m);
            SetRotMatrix(&m);
            SetTransMatrix(&m);
            func_800BA458(WM_PACK0(0x3C), 0, 2);
            func_80030AF4(&D_800CDF90, &m);
            func_800BBE24(&D_800CDF60, (VECTOR *)m.t, &v, 0);
            CompMatrix(D_800CF448, &m, &m);
            SetRotMatrix(&m);
            SetTransMatrix(&m);
            func_800BA458(WM_PACK0(0x3D), 0, 2);
            for (i = 0; i < 10; i++) {
                MATRIX m1;
                MATRIX m2;
                VECTOR d;
                VECTOR scale;
                VECTOR s;
                u8 c;

                m1 = D_800CD774;
                m2 = D_800CD774;
                if (D_800D48E8[i] == 0) {
                    if (rand() % 5 == 0) {
                        D_800D4910[i].vx = rand() % 4096;
                        D_800D4910[i].vy = rand() % 4096;
                        D_800D4910[i].vz = rand() % 4096;
                        D_800D48F8[i] = rand() % 1024 + 0x400;
                        D_800D48E8[i] = 1;
                    }
                } else {
                    D_800D48F8[i] += rand() % 86 - 10;
                    if (++D_800D48E8[i] == 33) {
                        D_800D48E8[i] = 0;
                    }
                }
                if (D_800D48E8[i] != 0) {
                    c = ~(D_800D48E8[i] * 7);
                    SetBackColor(c >> 1, c >> 2, c);
                    m2.t[1] = -700;
                    func_80030AF4(&D_800D4910[i], &m1);
                    gte_CompMatrix(&m1, &m2, &m);
                    memset(&s, 0, sizeof(VECTOR));
                    s.vx = 0x2FA0;
                    s.vy = D_800D48F8[i];
                    s.vz = 0x1830;
                    scale = s;
                    ScaleMatrix(&m, &scale);
                    func_800BBE24(&D_800CDF60, &d, &v, 0);
                    m.t[0] += d.vx;
                    m.t[1] += d.vy;
                    m.t[2] += d.vz;
                    gte_CompMatrix(D_800CF448, &m, &m);
                    SetRotMatrix(&m);
                    SetTransMatrix(&m);
                    switch (rand() % 3) {
                    case 0:
                        func_800BA458(WM_PACK0(0x40), 0, 2);
                        break;
                    case 1:
                        func_800BA458(WM_PACK0(0x41), 0, 2);
                        break;
                    }
                }
            }
            for (i = 0; i < 10; i++) {
                MATRIX m1;
                MATRIX m2;
                VECTOR d;
                VECTOR scale;
                s32 c;

                m1 = D_800CD774;
                m2 = D_800CD774;
                c = (math_sin(D_800CF4B0 * 16 + i * 400) >> 5) + 0x80;
                if (c >= 0x100) {
                    c = 0xFF;
                }
                SetBackColor((u8)c >> 1, (u8)c >> 2, (u8)c);
                if (c == 0) {
                    D_800D4830[i] = 0x400;
                } else {
                    D_800D4830[i] += 3;
                }
                m2.t[2] = -D_800D4830[i];
                func_80030EA4(&D_800D4870[i], &m1);
                gte_CompMatrix(&m1, &m2, &m);
                scale = D_800A8598;
                ScaleMatrix(&m, &scale);
                func_800BBE24(&D_800CDF60, &d, &v, 0);
                m.t[0] += d.vx;
                m.t[1] += d.vy;
                m.t[2] += d.vz;
                gte_CompMatrix(D_800CF448, &m, &m);
                SetRotMatrix(&m);
                SetTransMatrix(&m);
                func_800BA458(WM_PACK0(0x3E), 0, 2);
            }
        }
        break;
    }
    SetLightMatrix(&D_800CF540);
    SetColorMatrix(&D_800CF500);
    SetBackColor(D_800D3FA0.unk5E0[0].unk20.c[0], D_800D3FA0.unk5E0[0].unk20.c[2], D_800D3FA0.unk5E0[0].unk20.c[4]);
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800C4A5C", D_800A8588);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800C4A5C", D_800A8598);

void func_800C69F4(s32 arg0) {
    WmEmitter *e;
    u8 on;
    s32 i;
    s32 r;
    s32 x;
    s32 y;
    s32 z;
    s32 b;
    u16 t;

    on = 0;
    switch ((u8)arg0) {
    case 2:
        D_800D1FD0 = 1;
        on = 1;
        break;
    case 5:
        on = 1;
        break;
    case 10:
        if (D_800D30D2 != 0) {
            on = 1;
        }
        break;
    case 3:
        if ((u16)(*D_800CF430 - 0x24EA) < 0x1B9) {
            on = 1;
        }
        break;
    case 7:
        if (D_800D5410 != 0) {
            D_800CDF48.vy += 0x40;
        }
        break;
    case 4:
        on = 1;
        D_800CDF80.vy -= 0x100;
        D_800CDFA0[0].vy += 0x100;
        D_800CDFA0[1].vy -= 0x40;
        D_800CDFA0[2].vy -= 0x180;
        break;
    case 6:
        D_800CDF88.vz -= 8;
        D_800CDF88.vy -= 0x10;
        D_800CDF90.vz -= 0x10;
        D_800CDF90.vy -= 8;
        for (i = 0; i < 10; i++) {
            t = D_800D4870[i].vy - 10;
            D_800D4870[i].vy = t - i / 4;
        }
        for (i = 0; i < 10; i++) {
            D_800D4910[i].vx += -6 + rand() % 13;
            D_800D4910[i].vy += -6 + rand() % 13;
        }
        break;
    }
    if (on) {
        e = w_framePackGetPtr(D_800CF428, (u8)arg0 + 0x28);
        for (; e->unk24 != -1; e++) {
            if (rand() % 4096 < e->unk28) {
                x = e->unk0 - e->unkC / 2;
                y = e->unk4 - e->unk10 / 2;
                z = e->unk8 - e->unk14 / 2;
                if (e->unkC != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                        if (g_randPtr >= g_randTableEnd) {
                            rand_refill();
                        }
                    }
                    b = *g_randPtr++;
                    x += ((b << 8) | b) % e->unkC;
                }
                if (e->unk10 != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                        if (g_randPtr >= g_randTableEnd) {
                            rand_refill();
                        }
                    }
                    b = *g_randPtr++;
                    y += ((b << 8) | b) % e->unk10;
                }
                if (e->unk14 != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                        if (g_randPtr >= g_randTableEnd) {
                            rand_refill();
                        }
                    }
                    b = *g_randPtr++;
                    z += ((b << 8) | b) % e->unk14;
                }
                func_800C6F00(x, y, z, e->unk24, e->unk26);
            }
        }
    }
}

void func_800C6EF8(void) {
}

void func_800C6F00(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    WmNode *p;
    s32 v;
    u8 k;
    WmNode *h;

    p = D_800D4848.unk8;
    h = &D_800D53A0;
    switch (arg3) {
    case 2:
    case 3:
        k = 2;
        break;
    case 16:
        k = 0xFF;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
        k = 1;
        break;
    default:
        k = 1;
        break;
    }
    if (p->unk8 != 0) {
        v = (s32)w_framePackGetPtr(D_800CF428, arg3 + 7);
        p->unk4->unk8 = p->unk8;
        p->unk8->unk4 = p->unk4;
        p->unk4 = h;
        p->unk8 = h->unk8;
        h->unk8->unk4 = p;
        h->unk8 = p;
        p->unkC = k;
        p->unk1C = 0;
        p->unk10 = arg0;
        p->unk14 = arg1;
        p->unk18 = arg2;
        p->unk0 = v;
        p->unkE = arg4;
        p->unkD = arg3;
        p->unk20 = 0;
        p->unk24 = 0;
        if (arg3 == 0x17) {
            p->unk20 = rand() & 0xFFF;
        }
    }
}

void func_800C7050(WmNode *arg0) {
    arg0->unk4->unk8 = arg0->unk8;
    arg0->unk8->unk4 = arg0->unk4;
    arg0->unk4 = &D_800D4848;
    arg0->unk8 = D_800D4848.unk8;
    D_800D4848.unk8->unk4 = arg0;
    D_800D4848.unk8 = arg0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800C4A5C", D_800A8644);
