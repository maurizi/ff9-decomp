#include "ovl_0c.h"

void func_800BA528(WmPack *arg0) {
    s32 i;
    WmPackEntry *e;

    i = 0;
    arg0->unk4 += (s32)arg0;
    for (; i < arg0->unk0; i++) {
        e = (WmPackEntry *)((s32)arg0->unk8[i] + (s32)arg0);
        arg0->unk8[i] = e;
        e->unk4 += (s32)arg0;
        e->unk8 += (s32)arg0;
    }
}

void func_800BA58C(void) {
    s32 unused[26];
    void **f[6]; /* f[6] below overruns into the saved s0 slot, as in the original */
    void *p;
    void **t;
    Unk800CFEA0Tbl *dst;
    s32 i;
    s32 acc;
    s32 w;
    s32 h;

    acc = 0;
    func_800BE674();
    i = 0;
    func_800BC540();
    do {
        g_worldEncounterZoneStart[i] = acc;
        acc += g_worldZoneEncounterCount[i] * 2;
        i++;
    } while (i < 26);
    while (cd_getState() != 0) {
    }
    if (g_worldDataSet == 1) {
        D_800CE3F4 = db_lookupRegistered(4, 0x411, 0);
        D_800CF428 = db_lookupRegistered(4, 0x41B, 0);
        D_800CF40C = db_lookupRegistered(4, 0x425, 0);
        D_800CF410 = db_lookupRegistered(4, 0x42F, 0);
        f[4] = db_lookupRegistered(4, 0x439, 0);
        f[5] = db_lookupRegistered(4, 0x443, 0);
    } else {
        D_800CE3F4 = db_lookupRegistered(4, 0x414, 0);
        D_800CF428 = db_lookupRegistered(4, 0x41E, 0);
        D_800CF40C = db_lookupRegistered(4, 0x428, 0);
        D_800CF410 = db_lookupRegistered(4, 0x432, 0);
        f[4] = db_lookupRegistered(4, 0x43C, 0);
        f[5] = db_lookupRegistered(4, 0x446, 0);
    }
    f[0] = db_lookupRegistered(4, 0x5DC, 0);
    f[1] = db_lookupRegistered(4, 0x5DD, 0);
    f[2] = db_lookupRegistered(4, 0x5DE, 0);
    f[3] = db_lookupRegistered(4, 0x5DF, 0);
    f[6] = db_lookupRegistered(4, 0x5E0, 0);
    w = D_800CF40C->unk0;
    h = D_800CF40C->unk2;
    D_800D3304 = w;
    D_800D3308 = h;
    D_800D32F0 = w * 2;
    D_800D32F4 = h * 2;
    D_800D32D8 = w << 14;
    D_800D32DC = h << 14;
    *(WmCellMap *)D_800D30F8 = *(WmCellMap *)f[0];
    func_800BB508();
    w_frameExtruct(D_800CE3F4);
    w_frameExtruct((s32 *)D_800CF428);
    w_frameExtruct((s32 *)f[1]);
    w_frameExtruct((s32 *)f[2]);
    w_frameExtruct((s32 *)f[3]);
    w_frameExtruct((s32 *)f[4]);
    func_800CB09C(f[6]);
    t = f[5];
    w_frameExtruct((s32 *)t);
    for (i = 0; i < 16; i++) {
        p = w_framePackGetPtr(t, i + D_800D30C8[i] * 16);
        w_frameExtruct(p);
        func_800B96CC(p, 0, 0, 0, 0);
    }
    func_800B96CC((s32 *)f[4], 0, 0, 0, 0);
    func_800B96CC((s32 *)f[1], 0, 0, 0, 0);
    if (g_gameState->unk1C->unk844 != 0) {
        func_800B8DC0((u32 *)f[3], g_gameState->unk1C->unk844 - 1, 0x318, 0xB0, 0x280, 0xF6, 2);
    }
    if (*D_800CF430 >= 0x1766) {
        D_800D3E74 = 1;
    } else {
        D_800D3E74 = 0;
    }
    func_800B8DC0((u32 *)f[2], D_800D3E74 * 2, 0, 0, 0, 0, 0);
    func_800B8DC0((u32 *)f[2], (D_800D3E74 << 1) | 1, 0, 0, 0, 0, 1);
    func_800B8DC0((u32 *)f[2], 4, 0, 0, 0, 0, 0);
    D_800D3E75 = -1;
    switch (*D_800CF430) {
    case 0x960:
        D_800D3E75 = 0;
        break;
    case 0x1766:
        D_800D3E75 = 1;
        break;
    case 0x2585:
        D_800D3E75 = 2;
        break;
    case 0x26A2:
        D_800D3E75 = 3;
        break;
    }
    if (D_800D3E75 != -1) {
        if (D_800CCE40 == 0) {
            func_800B8DC0((u32 *)f[2], D_800D3E75 * 2 + 5, 0x300, 0x40, 0x280, 0xF4, 0);
            func_800B8DC0((u32 *)f[2], D_800D3E75 * 2 + 6, 0x320, 0x40, 0x290, 0xF4, 0);
        } else {
            func_800B8DC0((u32 *)f[2], D_800D3E75 * 2 + 5, 0x300, 0x40, 0x280, 0xF4, 0);
            func_800B8DC0((u32 *)f[2], D_800D3E75 * 2 + 6, 0x300, 0x60, 0x290, 0xF4, 0);
        }
    }
    for (i = 0x35; i < 0x43; i++) {
        func_800BA528(w_framePackGetPtr(D_800CF428, i));
    }
    D_800D32EC = w_framePackGetPtr(D_800CF428, 0x42);
    D_800CF478 = w_framePackGetPtr(D_800CF428, 0);
    g_worldEncounterTable = w_framePackGetPtr(D_800CF428, 3);
    D_800D3FA0 = *(Unk800D3FA0 *)w_framePackGetPtr(D_800CF428, 5);
    dst = (Unk800CFEA0Tbl *)D_800CFEA0;
    *dst = *(Unk800CFEA0Tbl *)w_framePackGetPtr(D_800CF428, 6);
    g_worldEncounterEventRows = w_framePackGetPtr(D_800CF428, 4);
}

void wm_applyEncounterEvents(void) {
    s32 i;
    s32 j;
    u16 (*row)[12];
    s32 mask;
    Unk800CF450 *tbl;
    u16 v;

    i = 0;
    tbl = g_worldEncounterTable;
    row = g_worldEncounterEventRows;
    mask = g_evtGlobalVars.b[0xC2] | (g_evtGlobalVars.b[0xC6] << 8);
    do {
        if ((mask >> i) & 1) {
            j = 0;
            do {
                v = (*row)[j];
                if (v != 0xFFFF) {
                    tbl[v - 1].unk6 = tbl[v - 1].unk4;
                }
                j++;
            } while (j < 12);
        }
        i++;
        row++;
    } while (i < 9);
    for (i = 0; i < 3; i++) {
        D_800D32F8[i] = 0xFFFF;
        D_800D330C[i] = 0;
    }
    if (*D_800CF430 - 0x1B4EU < 0x1004) {
        D_800D5410 = 0;
    } else {
        D_800D5410 = 1;
    }
    func_800BD674();
    func_800BD57C();
}

void func_800BAEC0(void) {
    Unk800B8388 *p;
    MATRIX *o;

    SetBackColor(D_800D3FA0.unk5E0[0].unk20.c[0], D_800D3FA0.unk5E0[0].unk20.c[2], D_800D3FA0.unk5E0[0].unk20.c[4]);
    SetLightMatrix(&D_800CF540);
    SetColorMatrix(&D_800CF500);
    func_800BD5A8();
    func_800BD78C();
    SetBackColor(D_800D3FA0.unk5E0[0].unk28.c[0], D_800D3FA0.unk5E0[0].unk28.c[2], D_800D3FA0.unk5E0[0].unk28.c[4]);
    if (D_800CF46D != 0) {
        MATRIX m1 = { { { 0x1000, 0, 0 }, { 0, 0x1000, 0 }, { 0, 0, 0x1000 } }, { 0, 0xE10, 0 } };
        MATRIX out;
        MATRIX m2 = *D_800CF448;

        if (g_worldFog != 0) {
            m1.t[1] = -D_800CE19C - 0x9C4;
        } else {
            m1.t[1] = -D_800CE19C - 0x258;
        }
        SetLightMatrix(&D_800CF520);
        SetColorMatrix(&D_800CF4E0);
        p = w_framePackGetPtr(D_800CF428, 0x3B);
        m2.t[0] = 0;
        m2.t[1] = 0;
        m2.t[2] = 0;
        o = &out;
        CompMatrix(&m1, &m2, o);
        SetRotMatrix(o);
        SetTransMatrix(o);
        func_800BA3F4(p->unk8, 0);
    }
}

void func_800BB07C(void) {
    s32 m;
    s32 flags;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 h;
    s32 v;

    m = D_800CE3DC->unk11;
    flags = D_800CCCC0[m].unk8;
    if (D_800D4820 != 0) {
        func_800BC690();
        func_800BDAB0();
    }
    for (i = 0; i < 3; i++) {
        dx = (D_800CE3DC->pos.vx >> 8) - (D_800CD078[i] >> 8);
        dz = (D_800CE3DC->pos.vz >> 8) - (D_800CD084[i] >> 8);
        D_800D32F8[i] = SquareRoot0(dx * dx + dz * dz);
        if (D_800D30C8[5] != 0 && i == 0) {
            D_800D32F8[0] = 100;
        }
        v = D_800D32F8[i];
        if (i == 0 && g_worldDataSet == 4) {
            continue;
        }
        if (v < 0x3F) {
            if (D_800D330C[i] == 0) {
                D_800D330C[i] = 1;
                func_800C4718(D_800CD090[i], 0);
            } else {
                dz = 0x7F - v * 2;
                if (dz > 0) {
                    func_800C47B0(D_800CD090[i], dz);
                }
            }
        } else {
            if (D_800D330C[i] == 1) {
                D_800D330C[i] = 0;
                func_800C48F4(D_800CD090[i]);
            }
        }
    }
    if (D_800D32F8[2] < 0x28) {
        d = 0x78 - D_800D32F8[2] * 3;
        if (d != 0 && D_800D33F4 != 0) {
            D_800CE108[1] += rand() % d - d / 2;
            D_800CE108[0] += rand() % d - d / 2;
            D_800CE108[2] += rand() % d - d / 2;
        }
    }
    if (D_800D32F8[1] < 0x22 && evt_isControlEnabled() != 0 && ((flags & 0xFC) >> 2) == 0x17 && m - 1 < 7U) {
        D_800CE3DC->pos.vz += (0x22 - D_800D32F8[1]) * 8;
        if (D_800CE3DC->pos.vx == D_800CE3DC->unk40.vx && D_800CE3DC->pos.vz == D_800CE3DC->unk40.vz) {
            D_800CE3DC->unk40.vx++;
        }
    }
    if (g_evtGlobalVars.b[0x66] == 8 || g_evtGlobalVars.b[0x66] == 0) {
        dx = (D_800CE3DC->pos.vx >> 3) - 0x790A;
        dz = (D_800CE3DC->pos.vz >> 3) + 0x5ACB;
        v = SquareRoot0(dx * dx + ((D_800CE3DC->pos.vy >> 3) + 0x66) * ((D_800CE3DC->pos.vy >> 3) + 0x66) * 8 + dz * dz);
        if (v < 0x47C) {
            h = 0x47C - v;
            if (h > 0x400) {
                h = 0x400;
            }
            if (h > 0x10) {
                h >>= 4;
                func_80050EFC(h, 0x40);
                D_800CF44C = 1;
                g_gameState->unk1C->unk570 = h >> 4;
                return;
            }
        }
    }
    D_800CF44C = 0;
    g_gameState->unk1C->unk570 = 0;
}

/* mark the map cells changed by the story progress */
void func_800BB508(void) {
    s32 i;
    s32 n;
    u8 *p;

    i = 15;
    p = &D_800D30C8[i];
    for (; i >= 0; i--) {
        *p-- = 0;
    }
    for (i = 10; i >= 0; i--) {
        if (D_800CD060[i] <= *D_800CF430) {
            break;
        }
    }
    if (g_worldMapDebugFlags & 0x10) {
        printf("Scene:%d / Status:%d\n", *D_800CF430, i);
    }
    if (i >= 0 && g_worldDataSet == 1) {
        for (n = 0; n < 7; n++) {
            switch (D_800CD010[n][i]) {
            case 1:
                WM_CELL(18, 14) |= 0x80;
                WM_CELL(17, 12) |= 0x80;
                D_800D30C8[0] = 1;
                break;
            case 6:
                WM_CELL(13, 12) |= 0x80;
                WM_CELL(14, 12) |= 0x80;
                D_800D30C8[5] = 1;
                break;
            case 5:
                WM_CELL(13, 16) |= 0x80;
                WM_CELL(13, 17) |= 0x80;
                WM_CELL(14, 16) |= 0x80;
                WM_CELL(14, 17) |= 0x80;
                D_800D30C8[4] = 1;
                break;
            case 9:
                WM_CELL(14, 6) |= 0x80;
                WM_CELL(21, 10) |= 0x80;
                WM_CELL(22, 14) |= 0x80;
                D_800D30C8[8] = 1;
                break;
            case 3:
                WM_CELL(19, 10) |= 0x80;
                WM_CELL(19, 11) |= 0x80;
                WM_CELL(20, 10) |= 0x80;
                WM_CELL(20, 11) |= 0x80;
                D_800D30C8[2] = 1;
                break;
            case 11:
                WM_CELL(3, 9) |= 0x80;
                WM_CELL(9, 1) |= 0x80;
                D_800D30C8[10] = 1;
                break;
            case 4:
                WM_CELL(7, 1) |= 0x80;
                WM_CELL(8, 1) |= 0x80;
                WM_CELL(14, 15) |= 0x80;
                D_800D30C8[3] = 1;
                break;
            }
            /* the original second jump table is 8-aligned */
            __asm__(".rdata\n\t.word 0\n\t.text");
            if (g_worldMapDebugFlags & 0x10) {
                switch (D_800CD010[n][i]) {
                case 0:
                    printf("BGI Form : SGATE\n");
                    break;
                case 5:
                    printf("BGI Form : CLAY\n");
                    break;
                case 4:
                    printf("BGI Form : LIND\n");
                    break;
                case 8:
                    printf("BGI Form : KUROMA\n");
                    break;
                case 2:
                    printf("BGI Form : ALEX\n");
                    break;
                case 10:
                    printf("BGI Form : HOKORA(MIZU)\n");
                    break;
                case 3:
                    printf("BGI Form : HOKORA(HI)\n");
                    break;
                }
            }
        }
    }
    printf("General Global Flag = %x\n", D_8007AF55);
    if (D_8007AF55 & 0x80) {
        printf("- Replace Mog Net -\n");
        WM_CELL(16, 1) |= 0x80;
        WM_CELL(13, 4) |= 0x80;
        WM_CELL(14, 5) |= 0x80;
        D_800D30C8[12] = 1;
    }
    if (D_8007AF55 & 0x40) {
        printf("- Replace Chocobo Heaven -\n");
        WM_CELL(0, 0) |= 0x80;
        WM_CELL(16, 14) |= 0x80;
        WM_CELL(9, 17) |= 0x80;
        D_800D30C8[7] = 1;
    }
}

/* pick the encounter table entry for the player's area, zone and fog */
Unk800CF450 *wm_getEncounterEntry(void) {
    u8 zone;
    s32 area;
    s32 lo;
    s32 hi;
    s32 i;
    u32 t;
    s32 f;
    s32 add;

    area = (D_800CCCC0[D_800CE3DC->unk11].unk8 & 0x3F00) >> 8;
    zone = g_worldAreaZone[area];
    lo = g_worldEncounterZoneStart[zone];
    hi = g_worldEncounterZoneStart[zone + 1];
    if (g_worldMapDebugFlags & 8) {
        printf("- Encount Information -\n");
        printf("Now Area :%d\n", (D_800CCCC0[D_800CE3DC->unk11].unk8 & 0x3F00) >> 8);
        printf("Now Zone :%d\n", zone);
        printf("Now Fog  :%d\n", g_worldFog);
        printf("Topograph:%d\n", (u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2);
    }
    for (i = lo; i < hi; i++) {
        t = g_worldEncounterTable[i].unk8 >> 2;
        f = g_worldEncounterTable[i].unk9 & 1;
        if (g_worldMapDebugFlags & 8) {
            printf("Matching:%d  ( T:%d / F:%d )\n", i, t, f);
        }
        if (t == (u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2 && f == g_worldFog) {
            if (g_worldMapDebugFlags & 8) {
                printf("Hit %d encount data\n", i);
            }
            add = 0;
            if (g_worldDataSet == 4 && i < 100) {
                add = 0xFE;
            }
            return &g_worldEncounterTable[i + add];
        }
    }
    printf(D_800A7F48);
    return &g_worldEncounterTable[i + add - 1];
}

s32 func_800BBE24(VECTOR *arg0, VECTOR *arg1, VECTOR *arg2, s32 *arg3) {
    VECTOR v;
    s32 s;

    v.vx = arg0->vx - D_800CE198[0];
    v.vy = arg0->vy - D_800CE198[1];
    v.vz = arg0->vz - D_800CE198[2];
    if (v.vx > D_800D32D8 - 0x8000) {
        v.vx -= D_800D32D8;
    }
    if (v.vz > D_800D32DC - 0x8000) {
        v.vz -= D_800D32DC;
    }
    if (v.vx < 0x8000 - D_800D32D8) {
        v.vx = D_800D32D8 + v.vx;
    }
    if (v.vz < 0x8000 - D_800D32DC) {
        v.vz = D_800D32DC + v.vz;
    }
    ApplyMatrixLV(D_800CF448, &v, arg2);
    s = 0x1000 - ((arg2->vz >> 4) + (arg2->vz >> 6));
    v.vy = ((arg0->vy * s) >> 12) - D_800CE198[1];
    ApplyMatrixLV(D_800CF448, &v, arg2);
    arg1->vx = arg0->vx - D_800CE108[0];
    arg1->vy = ((arg0->vy * s) >> 12) - D_800CE108[1];
    arg1->vz = arg0->vz - D_800CE108[2];
    if (arg1->vx > 0xFFFF) {
        arg1->vx -= 0x60000;
    }
    if (arg1->vz > 0xFFFF) {
        arg1->vz -= 0x50000;
    }
    if (arg1->vx < -0xFFFF) {
        arg1->vx += 0x60000;
    }
    if (arg1->vz < -0xFFFF) {
        arg1->vz += 0x50000;
    }
    if (arg3 != NULL) {
        *arg3 = s;
    }
    if (arg1->vx < -0x8000 || arg1->vx > 0x7FFF || arg1->vz > 0x7FFF || arg1->vz < -0x8000) {
        return 0;
    }
    return 1;
}

void func_800BC098(void) {
}

void func_800BC0A0(void) {
}

void func_800BC0A8(s32 *arg0, s32 *arg1) {
    if (*arg0 < 0) {
        *arg0 += D_800D32D8;
    }
    if (*arg0 >= D_800D32D8) {
        *arg0 -= D_800D32D8;
    }
    if (*arg1 > 0) {
        *arg1 -= D_800D32DC;
    }
    if (*arg1 <= -D_800D32DC) {
        *arg1 += D_800D32DC;
    }
}

void func_800BC130(s32 arg0, s32 arg1, s32 *arg2) {
    s32 x;
    s32 z;

    func_800BC0A8_inline(&arg0, &arg1);
    x = arg0 / 0x2000;
    z = arg1 / 0x2000;
    *arg2 = -z * D_800D32F0 + x;
}

void func_800BC1FC(s32 arg0, s32 arg1, s32 *arg2) {
    s32 x;
    s32 z;

    func_800BC0A8_inline(&arg0, &arg1);
    x = arg0 / 0x4000;
    z = arg1 / 0x4000;
    *arg2 = -z * D_800D3304 + x;
}

void func_800BC2C8(s32 arg0, s32 *arg1, s32 *arg2) {
    *arg1 = (arg0 % D_800D32F0) << 13;
    *arg2 = -((arg0 / D_800D32F0) << 13);
}

void func_800BC354(s32 arg0, s32 *arg1) {
    *arg1 = arg0 / D_800D32F0 / 2 * D_800D3304 + arg0 % D_800D32F0 / 2;
}

void func_800BC3C4(s32 arg0, s32 *arg1, s32 *arg2) {
    *arg1 = (arg0 % D_800D3304) << 14;
    *arg2 = -((arg0 / D_800D3304) << 14);
}

void func_800BC450(s32 arg0, s32 arg1, WmCell *arg2) {
    WmCell *cells;

    cells = D_800CF40C->unk4;
    arg2->unk0 = cells[arg0 * 2 + arg1].unk0;
    arg2->unk2 = cells[arg0 * 2 + arg1].unk2;
    arg2->unk3 = cells[arg0 * 2 + arg1].unk3;
}

void func_800BC490(s32 arg0, s32 arg1, WmCell *arg2) {
    s32 k;
    WmCell *cells;

    k = arg0 % D_800D3304 * D_800D3308 + arg0 / D_800D3304;
    cells = D_800CF410->unk4;
    arg2->unk0 = cells[k * 2 + arg1].unk0;
    arg2->unk2 = cells[k * 2 + arg1].unk2;
    arg2->unk3 = cells[k * 2 + arg1].unk3;
}

u8 func_800BC528(s32 arg0) {
    return g_worldAreaZone[arg0];
}

/* initialise the texture pack list */
void func_800BC540(void) {
    u32 i;

    D_800D33F0 = 0;
    D_800D34A8[0].unk0 = &D_800D3358;
    D_800D34A8[0].unkE = 0;
    D_800D34A8[0].unk4 = &D_800D34A8[1];
    D_800D34A8[0].unkC = -1;
    D_800D34A8[0].unkF = 0;
    D_800D34A8[0].unk8 = (WmTexPack *)D_800CF408;
    for (i = 1; i < 31; i++) {
        D_800D34A8[i].unk0 = &D_800D34A8[i - 1];
        D_800D34A8[i].unk4 = &D_800D34A8[i + 1];
        D_800D34A8[i].unk8 = NULL;
        D_800D34A8[i].unkE = 0;
        D_800D34A8[i].unkC = -1;
        D_800D34A8[i].unkF = 0;
    }
    for (i = 1; i < 16; i++) {
        D_800D34A8[i].unk8 = (WmTexPack *)(D_800CF408 + i * 0x4800);
        D_800D34A8[i].unkF = 0;
    }
    i = 31;
    D_800D34A8[i].unk0 = &D_800D34A8[i - 1];
    D_800D34A8[i].unk4 = &D_800D3340;
    D_800D34A8[i].unkC = -1;
    D_800D34A8[i].unk8 = NULL;
    D_800D34A8[i].unkE = 0;
    D_800D34A8[i].unkF = 0;
    D_800D3358.unk0 = NULL;
    D_800D3358.unk4 = &D_800D34A8[0];
    D_800D3340.unk4 = &D_800D34A8[16];
    D_800D3750 = 0;
    D_800D3408 = -1;
    D_800D3340.unk0 = NULL;
    D_800D33F4 = 0;
    func_800C2040();
    func_800C26DC();
}

void func_800BC690(void) {
    s32 t;
    s32 a;
    s32 b;

    if (D_800CE3C4 < 10) {
        if (D_800CE3C4 >= 8) {
            t = D_800CE3C0;
            if (t >= 0x200) {
                t = 0x1FF;
            }
            if (t < 0x100) {
                t = 0x100;
            }
            D_800D3730[0] = D_800CE198[0] + ((D_800CE1D0[0] * D_800CE3C8->unkA * t) >> 9);
            D_800D3730[2] = D_800CE198[2] + ((D_800CE1D0[2] * D_800CE3C8->unkA * t) >> 9);
        } else {
            D_800D3730[0] = D_800CE198[0] + D_800CE1D0[0] * D_800CE3C8->unkA;
            D_800D3730[2] = D_800CE198[2] + D_800CE1D0[2] * D_800CE3C8->unkA;
        }
    } else {
        D_800D3730[0] = D_800CE198[0] + D_800CE1D0[0] * D_800CE3C8->unkA;
        D_800D3730[2] = D_800CE198[2] + D_800CE1D0[2] * D_800CE3C8->unkA;
    }
    func_800BC0A8(&D_800D3730[0], &D_800D3730[2]);
    func_800BC130(D_800D3730[0] - 0x6000, D_800D3730[2] + 0x6000, &a);
    func_800BC354(a, &b);
    if (b != D_800D3408) {
        func_800BC84C(b);
    }
    func_800BCBDC();
    D_800D3408 = b;
}

void func_800BC84C(s32 arg0) {
    s32 missing[16];
    s32 x;
    s32 z;
    s32 id;
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    Unk800D335C *p;
    Unk800D335C *h;
    u8 c;

    n = 0;
    h = &D_800D3358;
    func_800BC3C4(arg0, &x, &z);
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            func_800BC1FC(x + (i << 14), z - (j << 14), &id);
            p = h->unk4;
            for (k = 0; k < 16; k++, p = p->unk4) {
                if (p->unkC == id) {
                    c = D_800D30F8[p->unkC];
                    if (c & 0x3F) {
                        w_readtxRequest(c);
                    }
                    p->unk0->unk4 = p->unk4;
                    p->unk4->unk0 = p->unk0;
                    p->unk4 = h->unk4;
                    p->unk0 = h;
                    h->unk4->unk0 = p;
                    h->unk4 = p;
                    break;
                }
            }
            if (k == 16) {
                missing[n++] = id;
            }
        }
    }
    for (k = 0; k < n; k++) {
        if (k == 4) {
            D_800D3338++;
    D_800D3338 %= 4;
        }
        func_800BCA6C(missing[k]);
        c = D_800D30F8[missing[k]];
        if (c & 0x3F) {
            w_readtxRequest(c);
        }
    }
    func_800BD5D0();
    func_800C1F3C();
    D_800D3338++;
    D_800D3338 %= 4;
}

/* request texture pack arg0: reuse a free or matching slot and move it behind the loaded ones */
s32 func_800BCA6C(s16 arg0) {
    Unk800D335C *prev;
    Unk800D335C *p;
    u32 n;

    n = 0;
    prev = &D_800D3358;
    while (prev->unk4->unkE == 1) {
        if (prev->unk4->unkC == arg0) {
            return 0;
        }
        prev = prev->unk4;
        n++;
    }
    p = prev->unk4;
    do {
        if (p->unkC == -1) {
            if (n >= 16) {
                p->unkC = arg0;
                p->unk0->unk4 = p->unk4;
                p->unk4->unk0 = p->unk0;
                p->unk4 = prev->unk4;
                p->unk0 = prev;
                D_800D33F0++;
                prev->unk4->unk0 = p;
                prev->unk4 = p;
            } else {
                p->unkC = arg0;
                p->unkE = 0;
            }
            break;
        }
        if (p->unkC == arg0) {
            p->unk0->unk4 = p->unk4;
            p->unk4->unk0 = p->unk0;
            p->unk4 = prev->unk4;
            p->unk0 = prev;
            prev->unk4->unk0 = p;
            prev->unk4 = p;
            break;
        }
        p = p->unk4;
        n++;
    } while (p != &D_800D3340);
    if (p != &D_800D3340) {
        p->unkF = D_800D3338;
    }
    return n != 32;
}

void func_800BCBDC(void) {
    WmCell a;
    WmCell b;
    Unk800D335C *n;
    u8 *cell;
    s32 id;
    s16 id2;
    s32 i;
    s32 r;
    s32 base;
    Unk800D335C *h;

    if (D_800CF454 == 0) {
        func_800BD150(D_800D33F8, &D_800D3320[D_800CD094], &D_800D3490[D_800CD094]);
        if (D_800D3490[D_800CD094] == 0) {
            goto end;
        }
        for (i = 0; i < D_800D3320[D_800CD094]; i++) {
            n = D_800D33F8[i];
            h = &D_800D3358;
            id2 = n->unkC;
            id = n->unkC;
            n->unk0->unk4 = n->unk4;
            n->unk4->unk0 = n->unk0;
            n->unk4 = h->unk4;
            n->unk0 = &D_800D3358;
            cell = &D_800D30F8[id];
            h->unk4->unk0 = n;
            h->unk4 = n;
            n->unkE = 1;
            if (D_800D3490[D_800CD094] == 1) {
                func_800BC450(id, 0, &a);
                func_800BC450(id, 1, &b);
            } else {
                func_800BC490(id, 0, &a);
                func_800BC490(id, 1, &b);
            }
            if ((*cell & 0x80) && b.unk2 == 0) {
                if (g_worldMapDebugFlags & 0x10) {
                    printf(D_800A7F94, id2);
                }
                *cell &= 0x7F;
            }
            r = (*cell & 0x3F) ? func_800C2050(*cell, 0) : 0;
            n->unk10 = 1;
            if (!(*cell & 0x80)) {
                D_800D3410[i * 4 + 0].unk0 = a.unk2 << 11;
                D_800D3410[i * 4 + 0].unk4 = n->unk8;
                D_800D3410[i * 4 + 1].unk0 = a.unk3 << 11;
                D_800D3410[i * 4 + 1].unk4 = (void *)r;
                D_800D3410[i * 4 + 2].unk0 = b.unk2 << 11;
                D_800D3410[i * 4 + 2].unk4 = NULL;
                D_800D3410[i * 4 + 3].unk0 = b.unk3 << 11;
                D_800D3410[i * 4 + 3].unk4 = NULL;
                if (D_800D3410[i * 4 + 1].unk0 == 0) {
                    D_800D3410[i * 4 + 1].unk4 = NULL;
                }
                if (D_800D3410[i * 4 + 0].unk0 == 0) {
                    D_800D3410[i * 4 + 0].unk4 = NULL;
                    n->unk10 = 0;
                    D_800D3740[i] = NULL;
                    D_800D3310[i] = NULL;
                } else {
                    D_800D3740[i] = D_800D3410[i * 4 + 0].unk4;
                    D_800D3310[i] = D_800D3410[i * 4 + 1].unk4;
                }
            } else {
                D_800D3410[i * 4 + 0].unk0 = a.unk2 << 11;
                D_800D3410[i * 4 + 0].unk4 = NULL;
                D_800D3410[i * 4 + 1].unk0 = a.unk3 << 11;
                D_800D3410[i * 4 + 1].unk4 = NULL;
                D_800D3410[i * 4 + 2].unk0 = b.unk2 << 11;
                D_800D3410[i * 4 + 2].unk4 = n->unk8;
                D_800D3410[i * 4 + 3].unk0 = b.unk3 << 11;
                D_800D3410[i * 4 + 3].unk4 = (void *)r;
                if (D_800D3410[i * 4 + 3].unk0 == 0) {
                    D_800D3410[i * 4 + 3].unk4 = NULL;
                }
                if (D_800D3410[i * 4 + 2].unk0 == 0) {
                    D_800D3410[i * 4 + 2].unk4 = NULL;
                    n->unk10 = 0;
                    D_800D3740[i] = NULL;
                    D_800D3310[i] = NULL;
                } else {
                    D_800D3740[i] = D_800D3410[i * 4 + 2].unk4;
                    D_800D3310[i] = D_800D3410[i * 4 + 3].unk4;
                }
            }
        }
        if (D_800D3490[D_800CD094] == 1) {
            func_800BC450(D_800D33F8[0]->unkC, 0, &a);
            base = D_800CF5A4;
        } else {
            func_800BC490(D_800D33F8[0]->unkC, 0, &a);
            base = D_800CF5A8;
        }
        D_800D3728 = 0;
        for (i = 0; i < D_800D3320[D_800CD094] * 4; i++) {
            if (D_800D3410[i].unk0 != 0) {
                D_800D3370[D_800D3728].unk4 = D_800D3410[i].unk4;
                D_800D3370[D_800D3728++].unk0 = D_800D3410[i].unk0;
            }
        }
        D_800CF454 = 1;
        if (D_800D3728 != 0) {
            cd_readChunks(a.unk0 + base, D_800D3370, D_800D3728, 0);
        }
    }
end:
    if (D_800CF454 == 1 && cd_getState() == 0) {
        func_800BD3F8();
    }
}

u8 func_800BD150(Unk800D335C **out, s32 *count, s32 *dir) {
    Unk800D335C *h[10];
    Unk800D335C *v[10];
    s32 hhi;
    s32 hlo;
    s32 vhi;
    s32 vlo;
    s32 i;
    Unk800D335C *p;

    hhi = 4;
    hlo = 4;
    *dir = 0;
    *count = 0;
    vhi = 4;
    vlo = 4;
    for (p = D_800D3358.unk4; p->unk8 != NULL; p = p->unk4) {
        if (p->unkE == 0 && p->unkC != -1 && p->unkF == D_800CD098) {
            h[hlo] = p;
            h[hhi] = p;
            v[vlo] = p;
            v[vhi] = p;
            for (i = 0; i < 4; i++) {
                for (p = D_800D3358.unk4; p->unk8 != NULL; p = p->unk4) {
                    if (p->unkE == 0) {
                        if (h[hlo]->unkC - 1 == p->unkC) {
                            h[--hlo] = p;
                        }
                        if (h[hhi]->unkC + 1 == p->unkC) {
                            h[++hhi] = p;
                        }
                        if (v[vlo]->unkC - 24 == p->unkC) {
                            v[--vlo] = p;
                        }
                        if (v[vhi]->unkC + 24 == p->unkC) {
                            v[++vhi] = p;
                        }
                    }
                }
            }
            if (hhi - hlo < vhi - vlo) {
                *dir = 24;
                for (i = vlo; i <= vhi; i++) {
                    out[(*count)++] = v[i];
                }
            } else {
                *dir = 1;
                for (i = hlo; i <= hhi; i++) {
                    out[(*count)++] = h[i];
                }
            }
            break;
        }
    }
    D_800CD098++;
    D_800CD098 %= 4;
    return *count;
}

void func_800BD3F8(void) {
    s32 i;
    u8 st;

    for (i = 0; i < D_800D3320[D_800CD094]; i++) {
        st = D_800D33F8[i]->unkE;
        if (st == 1) {
            D_800D33F8[i]->unkE = 2;
            if (D_800D3740[i] != NULL) {
                func_800BA528(D_800D3740[i]);
            }
            if (D_800D3310[i] != NULL) {
                func_800C20B0(D_800D3310[i]);
            }
            if (D_800D33F4 == 0) {
                if (++D_800D3750 == 16) {
                    D_800D30C5 = st;
                    D_800CE202 = 0;
                    D_800D33F4 = st;
                    D_800CF46C = 2;
                    g_padPortMask[0] &= 0xFFF4FFFF;
                    func_800B30C0();
                }
            }
        }
    }
    D_800CF454 = 0;
}

void func_800BD57C(void) {
    func_800C1C98();
    func_800C2134();
    D_800D3338 = 0;
}

void func_800BD5A8(void) {
    func_800C2048();
    func_800C26E4();
}

void func_800BD5D0(void) {
    Unk800D335C *a;
    Unk800D335C *b;
    u32 i;

    if (D_800D33F0 != 0) {
        b = D_800D3358.unk4;
        D_800D33F0 = 0;
        a = b;
        for (i = 0; i < 16; i++) {
            b = b->unk4;
        }
        while (1) {
            if (b == &D_800D3340) {
                break;
            }
            while (a->unk8 != 0) {
                a = a->unk4;
            }
            a->unk8 = b->unk8;
            b->unk8 = 0;
            b->unkC = -1;
            b->unkE = 0;
            b = b->unk4;
        }
    }
}

void func_800BD66C(void) {
}

/* initialise the texture list */
void func_800BD674(void) {
    u32 i;

    D_800D3790 = 0;
    D_800D3798[0].unk0 = &D_800D3758;
    D_800D3798[0].unkC = &D_800D3AB8[0];
    D_800D3798[0].unk4 = &D_800D3798[1];
    D_800D3798[0].unk8 = NULL;
    D_800D3798[0].unk12 = 0;
    D_800D3798[0].unk10 = -1;
    D_800D3798[0].unk13 = 0;
    for (i = 1; i < 39; i++) {
        D_800D3798[i].unk0 = &D_800D3798[i - 1];
        D_800D3798[i].unk4 = &D_800D3798[i + 1];
        D_800D3798[i].unk8 = NULL;
        D_800D3798[i].unkC = NULL;
        D_800D3798[i].unk12 = 0;
        D_800D3798[i].unk10 = -1;
        D_800D3798[i].unk13 = -1;
    }
    for (i = 1; i < 20; i++) {
        D_800D3798[i].unkC = &D_800D3AB8[i];
        D_800D3798[i].unk13 = i;
    }
    i = 39;
    D_800D3798[i].unk0 = &D_800D3798[i - 1];
    D_800D3798[i].unk4 = &D_800D3BF8;
    D_800D3798[i].unk8 = NULL;
    D_800D3798[i].unkC = NULL;
    D_800D3798[i].unk12 = 0;
    D_800D3798[i].unk10 = -1;
    D_800D3798[i].unk13 = -1;
    D_800D3758.unk0 = NULL;
    D_800D3758.unk4 = &D_800D3798[0];
    D_800D3BF8.unk4 = &D_800D3798[20];
    D_800D3BF8.unk0 = NULL;
}

void func_800BD78C(void) {
    Unk800D375C *n;
    Unk800D3AB8 *pos;
    MATRIX m;
    Unk800A7FB8 quad = D_800A7FB8;
    long sxy[4];
    VECTOR a;
    VECTOR b;
    long p;
    long flag;
    long xy3;
    u8 snd;
    s32 far;
    s32 k;
    u32 *ot;
    POLY_F4 *f4;

    n = D_800D3758.unk4;
    while (n->unk10 != -1) {
        pos = n->unkC;
        D_800D3770.t[0] = pos->unk0[0] - D_800CE108[0];
        D_800D3770.t[1] = pos->unk0[1] - D_800CE108[1];
        D_800D3770.t[2] = pos->unk0[2] - D_800CE108[2];
        CompMatrix(D_800CF448, &D_800D3770, &m);
        SetRotMatrix(&m);
        SetTransMatrix(&m);
        func_800BBE24((VECTOR *)pos, &a, &b, 0);
        far = b.vz < -10000 || b.vz > 11000;
        k = n->unk10 % D_800D32F0 + n->unk10 / D_800D32F0;
        if (n->unk12 == 2) {
            func_800BA2DC((u8 *)n->unk8, (n->unk10 % D_800D32F0 + n->unk10 / D_800D32F0) % 2, far);
            if (RotTransPers4(&quad.v[0], &quad.v[1], &quad.v[2], &quad.v[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag) < 7000
                && D_800CE3DC->pos.vy > -7000) {
                f4 = (POLY_F4 *)D_800CF404;
                ot = (u32 *)D_800CF460[*D_800CF45C];
                *(s32 *)&f4->x0 = sxy[0];
                *(s32 *)&f4->x1 = sxy[1];
                *(s32 *)&f4->x2 = sxy[2];
                xy3 = sxy[3];
                setlen(f4, 5);
                setRGB0(f4, 0, 0, 0);
                f4->code = 0x28;
                *(s32 *)&f4->x3 = xy3;
                addPrim(&ot[0xFFA], f4);
                f4++;
                D_800CF404 = (s32)f4;
            }
        }
        if (n->unk8 != NULL) {
            snd = n->unk8->unk21;
            if (snd != 0) {
                func_800C5A48(snd);
            }
        }
        n = n->unk4;
    }
}

void func_800BDAB0(void) {
    VECTOR pos;
    VECTOR p2;
    s16 sx[20];
    s16 sy[20];
    s32 z[20];
    VECTOR d;
    WmNearCell tmp;
    MATRIX m;
    SVECTOR sv;
    VECTOR out;
    s32 cell;
    long flag;
    long p;
    long sxy;
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 hit;
    s32 cnt;
    s32 cnt2;
    s32 last;
    s32 id;
    WmTexEnt *e;
    SVECTOR *verts;
    WmScrRect *r;
    WmNearCell *c;
    Unk800D375C *nd;
    s32 lim;
    s32 t;

    pos.vy = 0;
    func_800BD674();
    func_80064CDC(10, 10);
    n = 0;
    if (D_800CE3C4 < 6) {
        if (D_800CE3DC->pos.vy >= -0x9C3) {
            D_800CD09C[1].y0 = D_800CD09C[0].y0 = -0x3C;
        } else {
            D_800CD09C[1].y0 = D_800CD09C[0].y0 = -0x1E;
        }
    }
    for (j = -3; j < 4; j++) {
        for (i = -3; i < 4; i++) {
            func_800BC130(D_800CE108[0] + (i << 13) + D_800CE1D0[0] * 2,
                          D_800CE108[2] - (j << 13) + D_800CE1D0[2] * 2, &cell);
            e = (WmTexEnt *)func_800BE4A8(cell, NULL);
            if (e != NULL) {
                func_800BC2C8(cell, &pos.vx, &pos.vz);
                D_800D3770.t[0] = pos.vx - D_800CE108[0];
                D_800D3770.t[1] = pos.vy - D_800CE108[1];
                D_800D3770.t[2] = pos.vz - D_800CE108[2];
                CompMatrix(D_800CF448, &D_800D3770, &m);
                SetRotMatrix(&m);
                SetTransMatrix(&m);
                hit = 0;
                if (m.t[2] >= -0x1D4B) {
                    k = (func_800AE7F0() == 0) * 2;
                    if (D_800CE3C8->unk0 == 1) {
                        k += 4;
                    }
                    if (D_800CE3C8->unk0 == 2) {
                        k += 8;
                    }
                    verts = e->unk4;
                    for (cnt = 0; cnt < e->unk20; cnt++) {
                        sv = verts[e->unkC[cnt]];
                        RotTrans(&sv, &out, &flag);
                        sv.vy = sv.vy * (0x1000 - (out.vz >> 4)) >> 12;
                        z[cnt] = RotTransPers(&sv, &sxy, &p, &flag);
                        sy[cnt] = (u32)sxy >> 16;
                        sx[cnt] = sxy;
                        if (z[cnt] < 0x898) {
                            k |= 1;
                        } else {
                            k &= ~1;
                        }
                        if ((sx[cnt] >= D_800CD09C[k].x0) & (D_800CD09C[k].x1 >= sx[cnt]) & (sy[cnt] >= D_800CD09C[k].y0) & (D_800CD09C[k].y1 >= sy[cnt])) {
                            hit = 1;
                            break;
                        }
                    }
                }
                if (hit == 1) {
                    p2.vx = pos.vx + 0x1000;
                    p2.vy = pos.vy;
                    p2.vz = pos.vz - 0x1000;
                    d.vx = p2.vx - D_800CE198[0];
                    d.vz = p2.vz - D_800CE198[2];
                    if (d.vx > 0xFFFF) {
                        d.vx -= D_800D32D8;
                    }
                    if (d.vx < -0xFFFF) {
                        d.vx += D_800D32D8;
                    }
                    if (d.vz > 0xFFFF) {
                        d.vz -= D_800D32DC;
                    }
                    if (d.vz < -0xFFFF) {
                        d.vz += D_800D32DC;
                    }
                    D_800D3C10[n].unk8 = SquareRoot0((d.vx >> 2) * (d.vx >> 2) + (d.vz >> 2) * (d.vz >> 2));
                    D_800D3C10[n].unk4 = e;
                    D_800D3C10[n].unk0 = cell;
                    n++;
                }
            }
        }
    }
    for (j = 0; j < n - 1; j++) {
        for (i = n - 1; j < i; i = t) {
            t = i - 1;
            if (D_800D3C10[t].unk8 > D_800D3C10[i].unk8) {
                tmp = D_800D3C10[i];
                D_800D3C10[i] = D_800D3C10[t];
                D_800D3C10[t] = tmp;
            }
        }
    }
    lim = 16;
    if (D_800CE3C4 == 7) {
        lim = 14;
    }
    if (g_worldFog == 0) {
        lim++;
    }
    if (lim < n) {
        n = lim;
    }
    cnt2 = 0;
    last = -1;
    for (i = 0; i < n; i++) {
        id = D_800D3C10[i].unk4->unk1;
        func_800BE138(D_800D3C10[i].unk0);
        if (id != 0) {
            if (cnt2 < 2) {
                w_frmbufRequest(id);
            }
            if (id != last) {
                cnt2++;
            }
            if (cnt2 == 1) {
                last = id;
            }
        }
    }
    func_800C23CC();
    func_800C24D8();
    func_800BE6B4();
    func_800BE238();
    nd = D_800D3758.unk4;
    D_800D1FD0 = 0;
    while (nd->unk10 != -1) {
        {
            u8 s = nd->unk8->unk21;
            if (s != 0) {
                func_800C69F4(s);
            }
        }
        nd = nd->unk4;
    }
}

s32 func_800BE138(s16 arg0) {
    Unk800D375C *n = D_800D3758.unk4;
    u32 i = 0;

    do {
        if (n->unk10 == -1) {
            if (i >= 20) {
                n->unk10 = arg0;
                n->unk0->unk4 = n->unk4;
                n->unk4->unk0 = n->unk0;
                n->unk4 = D_800D3758.unk4;
                n->unk0 = &D_800D3758;
                D_800D3790++;
                D_800D3758.unk4->unk0 = n;
                D_800D3758.unk4 = n;
            } else {
                n->unk10 = arg0;
                n->unk12 = 0;
            }
            break;
        }
        if (n->unk10 == arg0) {
            n->unk0->unk4 = n->unk4;
            n->unk4->unk0 = n->unk0;
            n->unk4 = D_800D3758.unk4;
            n->unk0 = &D_800D3758;
            D_800D3758.unk4->unk0 = n;
            D_800D3758.unk4 = n;
            break;
        }
        n = n->unk4;
        i++;
    } while (i < 40);
    return i != 40;
}

void func_800BE238(void) {
    Unk800D375C *n;
    Unk800D335C *p;
    u32 i;
    u32 j;
    s32 id;
    s16 m;
    s32 k;

    n = D_800D3758.unk4;
    for (i = 0; i < 20; i++, n = n->unk4) {
        if (n->unk10 < 0) {
            return;
        }
        func_800BC354(n->unk10, &id);
        n->unk12 = 0;
        p = D_800D3358.unk4;
        for (j = 0; j < 16; j++, p = p->unk4) {
            if (id == p->unkC) {
                if (p->unkE == 2) {
                    n->unk12 = 2;
                    func_800BC2C8(n->unk10, &n->unkC->unk0[0], &n->unkC->unk0[2]);
                    n->unkC->unk0[1] = 0;
                    m = n->unk10 % 2;
                    k = m + (n->unk10 / D_800D32F0) % 2 * 2;
                    if (p->unk10 != 0) {
                        n->unk8 = p->unk8->unk8[k];
                        n->unk8->unk1 = p->unk8->unk2;
                    } else {
                        n->unk8 = D_800D32EC->unk8[k];
                        n->unk8->unk1 = 0;
                    }
                    break;
                }
                n->unk12 = 2;
                n->unkC->unk0[1] = 0;
                m = n->unk10 % 2;
                k = m + (n->unk10 / D_800D32F0) % 2 * 2;
                func_800BC2C8(n->unk10, &n->unkC->unk0[0], &n->unkC->unk0[2]);
                n->unk8 = D_800D32EC->unk8[k];
                n->unk8->unk1 = 0;
            }
        }
    }
}

s32 func_800BE4A8(s16 arg0, u8 *arg1) {
    s32 id;
    s32 i;
    Unk800D335C *p;
    s16 m;
    s32 k;

    p = D_800D3358.unk4;
    func_800BC354(arg0, &id);
    for (i = 0; i < 16; i++, p = p->unk4) {
        if (id == p->unkC && p->unkE == 2) {
            if (arg1 != NULL) {
                *arg1 = 0;
            }
            m = arg0 % 2;
            k = m + (arg0 / D_800D32F0) % 2 * 2;
            if (p->unk10 != 0) {
                return (s32)p->unk8->unk8[k];
            }
            return (s32)D_800D32EC->unk8[k];
        }
    }
    if (arg1 != NULL) {
        *arg1 = 1;
    }
    m = arg0 % 2;
    k = m + (arg0 / D_800D32F0) % 2 * 2;
    return (s32)D_800D32EC->unk8[k];
}

void func_800BE674(void) {
    D_800D3770.m[2][2] = 0x1000;
    D_800D3770.m[1][1] = 0x1000;
    D_800D3770.m[0][0] = 0x1000;
    D_800D3770.t[2] = 0;
    D_800D3770.t[1] = 0;
    D_800D3770.t[0] = 0;
    D_800D3770.m[2][1] = 0;
    D_800D3770.m[2][0] = 0;
    D_800D3770.m[1][2] = 0;
    D_800D3770.m[1][0] = 0;
    D_800D3770.m[0][2] = 0;
    D_800D3770.m[0][1] = 0;
}

void func_800BE6B4(void) {
    u32 i;
    Unk800D375C *a;
    Unk800D375C *b;

    if (D_800D3790 != 0) {
        a = D_800D3758.unk4;
        D_800D3790 = 0;
        b = a;
        for (i = 0; i < 20; i++) {
            b = b->unk4;
        }
        for (i = 0; i < 20; i++) {
            if (a->unkC == 0) {
                a->unk8 = b->unk8;
                a->unkC = b->unkC;
                b->unk8 = 0;
                b->unkC = 0;
                b->unk10 = -1;
                b->unk12 = 0;
                b = b->unk4;
            }
            a = a->unk4;
        }
    }
}

s32 func_800BE748(VECTOR *arg0, u32 *arg1, s32 *arg2, Unk800CE288 *arg3, u16 *arg4) {
    VECTOR pos;
    VECTOR rel;
    VECTOR o;
    s32 cell;
    u8 flag;
    s32 r;

    pos = *arg0;
    func_800BC0A8(&pos.vx, &pos.vz);
    func_800BC130(pos.vx, pos.vz, &cell);
    r = func_800BE4A8(cell, &flag);
    if (flag == 0 && r != 0) {
        func_800BC2C8(cell, &o.vx, &o.vz);
        rel.vx = pos.vx - o.vx;
        rel.vy = pos.vy;
        rel.vz = pos.vz - o.vz;
        /* callers pass a 16-bit variable but a word is stored */
        *(s32 *)arg4 = func_800B9808(cell, (WmTexEnt *)r, &rel, arg1, arg2, arg3);
        if ((u8)D_800CE3C4 - 2U < 2) {
            if (*arg1 - 0x38 < 2) {
                *arg1 = 0x36;
            }
        }
        return 1;
    }
    *(s32 *)arg4 = pos.vy;
    return 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A7F48);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A7F94);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A7FB8);

void func_800BE894(void) {
    s32 i;
    u8 *b;

    D_800D3E78 = 1;
    D_800D3E80 = 1;
    D_800D3E84 = -1;
    D_800D3E76 = 0;
    D_800D3E6C = 0x14;
    D_800D3E89 = 0x3F;
    D_800D3E79 = 0;
    D_800D3E88 = 3;
    D_800D3E77 = 1;
    D_800D3E64 = 1;
    D_800D3E60 = (b = g_evtGlobalVars.b)[0x64];
    i = 0x2E;
    if (*D_800CF430 >= 0x1766) {
        g_evtGlobalVars.h[i] |= 0x7C0;
        if (g_evtGlobalVars.h[i] & 0xC000) {
            g_evtGlobalVars.h[i] |= 0xC000;
        }
    }
    if (g_worldMapDebugFlags & 0x80) {
        printf("DEF SET NAVIMODE :%d\n", D_800D3E60);
    }
}

/* navi map markers */
void func_800BE984(void) {
    s32 x;
    s32 z;
    u8 page;

    if (D_800D3E75 >= 0) {
        func_800C0F34();
    }
    if (D_800D3E80 == 0) {
        return;
    }
    page = D_800D3E74;
    switch (D_800D3E60) {
    case 1:
        if (D_800CE3D0 != NULL && D_800CE3D0->unkE == 1 && (D_800CE3D0->unkF & 1) &&
            !((u8)D_800CE3C4 >= 7 && (u8)D_800CE3C4 < 10)) {
            CVECTOR c0 = D_800A7FF0;
            CVECTOR c1 = D_800A7FF4;
            func_800C1B00(D_800CE3D0->pos.vx, D_800CE3D0->pos.vz, &x, &z, page * 2);
            func_800BF544(x, z, 1, 1, c0, c1, 0x40, 0xC8, 0x82, 1);
        }
        if (D_800CE268 != NULL && D_800CE268->unkE == 1 && (D_800CE268->unkF & 1) &&
            !((u8)D_800CE3C4 >= 1 && (u8)D_800CE3C4 < 7)) {
            CVECTOR c0 = D_800A7FF8;
            CVECTOR c2 = D_800A7FFC;
            func_800C1B00(D_800CE268->pos.vx, D_800CE268->pos.vz, &x, &z, page * 2);
            func_800BF544(x, z, 1, 1, c0, c2, 0x40, 0xC8, 0x82, 1);
        }
        func_800C0888();
        if (g_gameState->unk1C->unk844 != 0) {
            func_800C16C0();
        }
        break;
    case 2:
        if (D_800D3E64 != 0) {
            if (D_800CE3D0 != NULL && D_800CE3D0->unkE == 1 && (D_800CE3D0->unkF & 1) &&
                !((u8)D_800CE3C4 >= 7 && (u8)D_800CE3C4 < 10)) {
                CVECTOR c0 = D_800A7FF0;
                CVECTOR c3 = D_800A7FF4;
                func_800C1B00(D_800CE3D0->pos.vx, D_800CE3D0->pos.vz, &x, &z, page * 2 + 1);
                func_800BF544(x, z, 1, 1, c0, c3, 0x40, 0x28, 9, 1);
            }
            if (D_800CE268 != NULL && D_800CE268->unkE == 1 && (D_800CE268->unkF & 1) &&
                !((u8)D_800CE3C4 >= 1 && (u8)D_800CE3C4 < 7)) {
                CVECTOR c0 = D_800A7FF8;
                CVECTOR c1 = D_800A7FFC;
                func_800C1B00(D_800CE268->pos.vx, D_800CE268->pos.vz, &x, &z, page * 2 + 1);
                func_800BF544(x, z, 1, 1, c0, c1, 0x40, 0x28, 9, 1);
            }
            if (D_800D3E77 != 0) {
                func_8004E76C(5);
                func_800515B8();
                func_800512B8((D_800CD104 >> 12) + 18, (D_800CD108 >> 12) + 6);
                func_800513CC(1);
                func_800515EC();
                func_8004E76C(2);
            }
            func_800C1428();
        }
        func_800BF978();
        break;
    }
}

void func_800BEECC(void) {
    VECTOR stick;
    s32 pad;
    s32 dir;
    s32 ang;
    s32 d;
    s32 best;
    s32 bi;
    s32 i;
    s32 ex, ez;
    s32 fx, fz;
    s32 t;
    s32 e;
    s32 last;
    s32 lim;

    if (D_800D3E80 != 0) {
        pad = 0;
        if (D_800D3E60 == 2) {
            dir = 1;
            stick.vx = 0;
            stick.vy = 0;
            if (D_800D3E84 == 1) {
                func_800C1B00(D_800CE3DC->pos.vx, D_800CE3DC->pos.vz, &D_800CD104, &D_800CD108, D_800D3E74 * 2 + 1);
                D_800CD104 <<= 12;
                D_800CD108 <<= 12;
            }
            if (D_800D3E78 != 0) {
                func_800B3398(&stick.vx);
                func_800B345C(&stick.vy);
                pad = func_800B84C0();
                if (D_800D3E64 == 0) {
                    pad = 0;
                    stick.vx = 0;
                    stick.vy = 0;
                }
            }
            stick.vy = -stick.vy;
            ang = (-ratan2(stick.vy, stick.vx) + 0x400) & 0xFFF;
            if (stick.vx | stick.vy) {
                t = SquareRoot0(stick.vx * stick.vx + stick.vy * stick.vy) << 5;
                d = t + (t >> 1);
            } else {
                d = 0;
            }
            if (d != 0) {
                D_800CD104 += math_sin(ang) * d >> 12;
                D_800CD108 -= math_cos(ang) * d >> 12;
                if (D_800CD104 < 0) {
                    D_800CD104 += 0xF0000;
                }
                if (D_800CD108 < 0) {
                    D_800CD108 += 0xC8000;
                }
                if (D_800CD104 > 0xF0000) {
                    D_800CD104 -= 0xF0000;
                }
                if (D_800CD108 > 0xC8000) {
                    D_800CD108 -= 0xC8000;
                }
            }
            if (pad & 0x300000) {
                if (g_evtGlobalVars.b[0x67] == 0) {
                    best = 0x7FFFFFFF;
                    bi = -1;
                    for (i = 0; i < 64; i++) {
                        if ((u8)func_800C1604(i)) {
                            fx = ((D_800CD10C[D_800D3E74][i].unk0 << 12) - D_800CD104) >> 12;
                            fz = ((D_800CD10C[D_800D3E74][i].unk2 << 12) - D_800CD108) >> 12;
                            e = SquareRoot0(fx * fx + fz * fz);
                            if ((D_800D3E74 != 1 || ((u32)(i - 6) >= 4 && i != 15)) && e < best) {
                                best = e;
                                bi = i;
                            }
                        }
                    }
                    if (bi != -1) {
                        dir = 0;
                        D_800D3E89 = bi;
                        D_800CD104 = D_800CD10C[D_800D3E74][D_800D3E89].unk0 << 12;
                        D_800CD108 = D_800CD10C[D_800D3E74][D_800D3E89].unk2 << 12;
                    }
                }
                if (g_evtGlobalVars.b[0x67] != 0 && dir != 0) {
                    if (pad & 0x100000) {
                        dir = 1;
                    }
                    if (pad & 0x200000) {
                        dir = -1;
                    }
                    do {
                        D_800D3E89 = (D_800D3E89 + dir) & 0x3F;
                        D_800D3E79 = 1;
                    } while (!(u8)func_800C1604(D_800D3E89) || (D_800CD10C[D_800D3E74][D_800D3E89].unk0 == D_800CD10C[D_800D3E74][D_800D3E89 - dir].unk0 &&
                             D_800CD10C[D_800D3E74][D_800D3E89].unk2 == D_800CD10C[D_800D3E74][D_800D3E89 - dir].unk2 &&
                             (u8)func_800C1604(D_800D3E89 - dir)));
                    D_800CD104 = D_800CD10C[D_800D3E74][D_800D3E89].unk0 << 12;
                    D_800CD108 = D_800CD10C[D_800D3E74][D_800D3E89].unk2 << 12;
                }
            }
            last = -1;
            for (i = 0; i < 64; i++) {
                ex = (D_800CD10C[D_800D3E74][i].unk0 << 12) - D_800CD104;
                ez = (D_800CD10C[D_800D3E74][i].unk2 << 12) - D_800CD108;
                if (D_800D3E74 != 1 || ((u32)(i - 6) >= 4 && i != 15)) {
                    lim = D_800CD70C;
                    best = ex >= 0 ? ex : -ex;
                    if (best < lim) {
                        ang = ez >= 0 ? ez : -ez;
                        if (ang < lim && (u8)func_800C1604(i)) {
                            D_800CD104 += ex >> 3;
                            D_800CD108 += ez >> 3;
                            if (best < 0x1000 && ang < 0x1000) {
                                D_800D3E89 = i;
                                last = i;
                            }
                        }
                    }
                }
            }
            if (D_800D3E79 == 1) {
                D_800D3E79 = 0;
                g_evtGlobalVars.b[0x67] = 0;
            } else {
                g_evtGlobalVars.b[0x67] = last + 1;
            }
        }
    }
    if ((func_800B8498() & 0x10000) && D_800D3E77 != 0) {
        D_800D3E64 = 0;
    } else {
        D_800D3E64 = 1;
    }
    D_800D3E84 = D_800D3E60;
}

void func_800BF544(s32 arg0, s32 arg1, s32 arg2, s32 arg3, CVECTOR arg4, CVECTOR arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    CVECTOR c;
    u32 *ot;

    ot = (u32 *)D_800CF460[*D_800CF45C];
    c.r = func_800B81C0(arg4.r, arg5.r, math_cos((D_800CF4B0 * arg6) & 0xFFF));
    c.g = func_800B81C0(arg4.g, arg5.g, math_cos((D_800CF4B0 * arg6) & 0xFFF));
    c.b = func_800B81C0(arg4.b, arg5.b, math_cos((D_800CF4B0 * arg6) & 0xFFF));
    if (arg9 == 0) {
        setXYWH((POLY_F4 *)D_800CF404, arg0 - arg2 + arg7, arg1 - arg3 + arg8, arg2 * 2 + 1, arg3 * 2 + 1);
        setRGB0((POLY_F4 *)D_800CF404, c.r, c.g, c.b);
        setPolyF4((POLY_F4 *)D_800CF404);
        addPrim(&ot[10], (POLY_F4 *)D_800CF404);
        D_800CF404 += sizeof(POLY_F4);
    } else {
        setXYWH((POLY_FT4 *)D_800CF404, arg0 + arg7 - 1, arg1 + arg8 - 1, 4, 4);
        setUV4((POLY_FT4 *)D_800CF404, 0, 0xFA, 4, 0xFA, 0, 0xFE, 4, 0xFE);
        setRGB0((POLY_FT4 *)D_800CF404, c.r, c.g, c.b);
        setPolyFT4((POLY_FT4 *)D_800CF404);
        ((POLY_FT4 *)D_800CF404)->tpage = 0x2B;
        ((POLY_FT4 *)D_800CF404)->clut = 0x3FEC;
        setSemiTrans((POLY_FT4 *)D_800CF404, 1);
        addPrim(&ot[10], (POLY_FT4 *)D_800CF404);
        D_800CF404 += sizeof(POLY_FT4);
        setXYWH((POLY_FT4 *)D_800CF404, arg0 + arg7 - 1, arg1 + arg8 - 1, 4, 4);
        setUV4((POLY_FT4 *)D_800CF404, 0, 0xFA, 4, 0xFA, 0, 0xFE, 4, 0xFE);
        setRGB0((POLY_FT4 *)D_800CF404, 0x60, 0x60, 0x60);
        setPolyFT4((POLY_FT4 *)D_800CF404);
        ((POLY_FT4 *)D_800CF404)->tpage = 0x4B;
        ((POLY_FT4 *)D_800CF404)->clut = 0x3FEC;
        setSemiTrans((POLY_FT4 *)D_800CF404, 1);
        addPrim(&ot[10], (POLY_FT4 *)D_800CF404);
        D_800CF404 += sizeof(POLY_FT4);
    }
}

#define TILE(x0_, y0_, x1_, y1_, u0_, v0_, u1_, v1_, cl)                                   \
    setXY4((POLY_FT4 *)D_800CF404, x0_, y0_, x1_, y0_, x0_, y1_, x1_, y1_);              \
    setUV4((POLY_FT4 *)D_800CF404, u0_, v0_, u1_, v0_, u0_, v1_, u1_, v1_);              \
    setRGB0((POLY_FT4 *)D_800CF404, D_800CD734, D_800CD738, D_800CD73C);                 \
    ((POLY_FT4 *)D_800CF404)->tpage = 0xA;                                                \
    ((POLY_FT4 *)D_800CF404)->clut = cl;                                                  \
    setPolyFT4((POLY_FT4 *)D_800CF404);                                                   \
    addPrim(&ot[10], (POLY_FT4 *)D_800CF404);                                             \
    D_800CF404 += sizeof(POLY_FT4);

void func_800BF978(void) {
    Unk800A7FB8 arrow;
    VECTOR v[4];
    SVECTOR rot;
    SVECTOR rot0;
    MATRIX m;
    s32 x;
    s32 y;
    s32 *px;
    s32 *py;
    u32 *ot;
    s32 i;
    s32 c;
    POLY_FT4 *p;
    s32 a;
    s32 b;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;

    ot = (u32 *)D_800CF460[*D_800CF45C];
    arrow = D_800A8000;
    memset(&rot0, 0, 8);
    rot0.vz = D_800CE3DC->pos.rotY - 0x400;
    rot = rot0;
    math_rotMatrixXYZ(&rot, &m);
    for (i = 0; i < 4; i++) {
        ApplyMatrix(&m, &arrow.v[i], &v[i]);
    }
    px = &x;
    py = &y;
    func_800C1B00(D_800CE3DC->pos.vx, D_800CE3DC->pos.vz, px, py, D_800D3E74 * 2 + 1);
    c = (math_sin(D_800CF4B0 << 6) >> 7) + 0x80;
    p = (POLY_FT4 *)D_800CF404;
    setRGB0(p, c, c, c);
    p->x0 = x + v[0].vx + 0x28;
    p->y0 = y + v[0].vy + 9;
    p->x1 = x + v[1].vx + 0x28;
    p->y1 = y + v[1].vy + 9;
    p->x2 = x + v[2].vx + 0x28;
    p->y2 = y + v[2].vy + 9;
    p->x3 = x + v[3].vx + 0x28;
    p->y3 = y + v[3].vy + 9;
    setUV4(p, 0x20, 0xD0, 0x40, 0xD0, 0x20, 0xD7, 0x40, 0xD7);
    p->tpage = GetTPage(0, 0, 0x300, 0xD0);
    p->clut = GetClut(0x280, 0xF5);
    setPolyFT4(p);
    if (D_800D3E64) {
        addPrim(&ot[10], p);
    }
    p++;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->x0 = x + v[0].vx + 0x28;
    p->y0 = y + v[0].vy + 9;
    p->x1 = x + v[1].vx + 0x28;
    p->y1 = y + v[1].vy + 9;
    p->x2 = x + v[2].vx + 0x28;
    p->y2 = y + v[2].vy + 9;
    p->x3 = x + v[3].vx + 0x28;
    p->y3 = y + v[3].vy + 9;
    setUV4(p, 0, 0xD0, 0x20, 0xD0, 0, 0xD7, 0x20, 0xD7);
    p->tpage = GetTPage(0, 0, 0x300, 0xD0);
    p->clut = GetClut(0x280, 0xF5);
    setPolyFT4(p);
    if (D_800D3E64) {
        addPrim(&ot[10], p);
    }
    p++;
    D_800CF404 = (s32)p;
    a = 0xF00 - D_800CE0F0->unkC;
    b = 0x1100 - D_800CE0F0->unkC;
    if (a < 0) {
        a += 0x1000;
    }
    if (b < 0) {
        b += 0x1000;
    }
    a %= 0x1000;
    b %= 0x1000;
    ax = math_sin(a) * 0x18 >> 12;
    ay = math_cos(a) * 0x18 >> 12;
    bx = math_sin(b) * 0x18 >> 12;
    by = math_cos(b) * 0x18 >> 12;
    func_800C1B00(D_800CE3DC->pos.vx, D_800CE3DC->pos.vz, px, py, D_800D3E74 * 2 + 1);
    x1 = x + ax + 0x28;
    y1 = y + ay + 9;
    x2 = x + bx + 0x28;
    y2 = y + by + 9;
    D_800D3E7C = (POLY_G3 *)D_800CF404;
    D_800D3E7C->r0 = D_800CD728;
    D_800D3E7C->g0 = D_800CD72C;
    D_800D3E7C->b0 = D_800CD730;
    D_800D3E7C->r1 = 0;
    D_800D3E7C->g1 = 0;
    D_800D3E7C->b1 = 0;
    D_800D3E7C->r2 = 0;
    D_800D3E7C->g2 = 0;
    D_800D3E7C->b2 = 0;
    D_800D3E7C->x0 = x + 0x28;
    D_800D3E7C->y0 = y + 9;
    D_800D3E7C->x1 = x1;
    D_800D3E7C->y1 = y1;
    D_800D3E7C->x2 = x2;
    D_800D3E7C->y2 = y2;
    setPolyG3(D_800D3E7C);
    setSemiTrans(D_800D3E7C, 1);
    if (D_800D3E64) {
        addPrim(&ot[10], D_800D3E7C);
    }
    D_800D3E7C++;
    D_800CF404 = (s32)D_800D3E7C;
    setDrawTPage((DR_TPAGE *)D_800CF404, 0, 1, GetTPage(0, 1, 0, 0));
    addPrim(&ot[10], (DR_TPAGE *)D_800CF404);
    D_800CF404 += sizeof(DR_TPAGE);
    TILE(0x20, 0xB5, 0x4E, 0xD9, 0, 0xB4, 0x2E, 0xD7, 0x3C6B)
    if (D_800D3E74 == 0) {
        TILE(0x28, 9, 0x118, 0xD1, 8, 8, 0xF7, 0xCF, 0x3C6A)
    } else {
        TILE(0x28, 9, 0x93, 0x42, 8, 8, 0x73, 0x41, 0x3C6C)
        TILE(0x93, 9, 0x118, 0x5C, 0x73, 8, 0xF8, 0x5B, 0x3CEC)
        TILE(0x28, 0x42, 0x93, 0xD1, 8, 0x41, 0x73, 0xD0, 0x3CAC)
        TILE(0x93, 0x5C, 0x118, 0xD1, 0x73, 0x5B, 0xF8, 0xD0, 0x3C2C)
    }
    TILE(0x20, 1, 0x120, 0xD9, 0, 0, 0xFF, 0xD8, 0x3C6B)
    setXY4((POLY_F4 *)D_800CF404, 0, 0, 0x140, 0, 0, 0xDA, 0x140, 0xDA);
    setRGB0((POLY_F4 *)D_800CF404, 0, 0, 0);
    setPolyF4((POLY_F4 *)D_800CF404);
    addPrim(&ot[10], (POLY_F4 *)D_800CF404);
    D_800CF404 += sizeof(POLY_F4);
}

void func_800C0888(void) {
    Unk800A7FB8 arrow;
    VECTOR v[4];
    SVECTOR rot;
    SVECTOR rot0;
    MATRIX m;
    s32 x;
    s32 y;
    s32 *px;
    s32 *py;
    u32 *ot;
    s32 i;
    s32 c;
    POLY_FT4 *p;
    s32 a;
    s32 b;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;

    ot = (u32 *)D_800CF460[*D_800CF45C];
    arrow = D_800A8020;
    memset(&rot0, 0, 8);
    rot0.vz = D_800CE3DC->pos.rotY - 0x400;
    rot = rot0;
    c = (math_sin(D_800CF4B0 << 6) >> 7) + 0x80;
    math_rotMatrixXYZ(&rot, &m);
    for (i = 0; i < 4; i++) {
        ApplyMatrix(&m, &arrow.v[i], &v[i]);
    }
    px = &x;
    py = &y;
    func_800C1B00(D_800CE3DC->pos.vx, D_800CE3DC->pos.vz, px, py, D_800D3E74 * 2);
    p = (POLY_FT4 *)D_800CF404;
    setRGB0(p, c, c, c);
    p->x0 = x + v[0].vx + 200;
    p->y0 = y + v[0].vy + 130;
    p->x1 = x + v[1].vx + 200;
    p->y1 = y + v[1].vy + 130;
    p->x2 = x + v[2].vx + 200;
    p->y2 = y + v[2].vy + 130;
    p->x3 = x + v[3].vx + 200;
    p->y3 = y + v[3].vy + 130;
    setUV4(p, 0, 0xD0, 0x20, 0xD0, 0, 0xD7, 0x20, 0xD7);
    p->tpage = GetTPage(0, 0, 0x300, 0xD0);
    p->clut = GetClut(0x280, 0xF5);
    setPolyFT4(p);
    addPrim(&ot[10], p);
    p++;
    D_800CF404 = (s32)p;
    a = 0xF00 - D_800CE0F0->unkC;
    b = 0x1100 - D_800CE0F0->unkC;
    if (a < 0) {
        a += 0x1000;
    }
    if (b < 0) {
        b += 0x1000;
    }
    a %= 0x1000;
    b %= 0x1000;
    ax = math_sin(a) * 12 >> 12;
    ay = math_cos(a) * 12 >> 12;
    bx = math_sin(b) * 12 >> 12;
    by = math_cos(b) * 12 >> 12;
    func_800C1B00(D_800CE3DC->pos.vx, D_800CE3DC->pos.vz, px, py, D_800D3E74 * 2);
    D_800D3E7C = (POLY_G3 *)D_800CF404;
    x1 = x + ax + 200;
    y1 = y + ay + 130;
    x2 = x + bx + 200;
    y2 = y + by + 130;
    D_800D3E7C->r0 = 0x8E;
    D_800D3E7C->g0 = 0xC8;
    D_800D3E7C->b0 = 0;
    D_800D3E7C->r1 = 0;
    D_800D3E7C->g1 = 0;
    D_800D3E7C->b1 = 0;
    D_800D3E7C->r2 = 0;
    D_800D3E7C->g2 = 0;
    D_800D3E7C->b2 = 0;
    D_800D3E7C->x0 = x + 200;
    D_800D3E7C->y0 = y + 130;
    D_800D3E7C->x1 = x1;
    D_800D3E7C->y1 = y1;
    D_800D3E7C->x2 = x2;
    D_800D3E7C->y2 = y2;
    setPolyG3(D_800D3E7C);
    setSemiTrans(D_800D3E7C, 1);
    addPrim(&ot[10], D_800D3E7C);
    D_800D3E7C++;
    D_800CF404 = (s32)D_800D3E7C;
    setDrawTPage((DR_TPAGE *)D_800CF404, 0, 1, GetTPage(0, 1, 0, 0));
    addPrim(&ot[10], (DR_TPAGE *)D_800CF404);
    D_800CF404 += sizeof(DR_TPAGE);
    setXY4((POLY_FT4 *)D_800CF404, 200, 130, 296, 130, 200, 210, 296, 210);
    setUV4((POLY_FT4 *)D_800CF404, 0, 0x80, 0x5F, 0x80, 0, 0xCF, 0x5F, 0xCF);
    setRGB0((POLY_FT4 *)D_800CF404, 0x80, 0x80, 0x80);
    ((POLY_FT4 *)D_800CF404)->tpage = 0xC;
    ((POLY_FT4 *)D_800CF404)->clut = 0x3C69;
    setPolyFT4((POLY_FT4 *)D_800CF404);
    setSemiTrans((POLY_FT4 *)D_800CF404, 1);
    addPrim(&ot[10], (POLY_FT4 *)D_800CF404);
    D_800CF404 += sizeof(POLY_FT4);
}

void func_800C0F34(void) {
    u32 *ot;
    POLY_FT4 *p;
    u32 a;
    u32 b;

    ot = (u32 *)D_800CF460[*D_800CF45C];
    D_800D3E88 = 0;
    a = D_800D3E6C + 0x78;
    b = D_800D3E6C + 0xB4;
    if ((u32)D_800D3E6C < (u32)D_800CF4AC) {
        D_800D3E88 = 1;
    }
    if (a < (u32)D_800CF4AC) {
        D_800D3E88 = 2;
    }
    if (b < (u32)D_800CF4AC) {
        D_800D3E88 = 3;
    }
    if (b <= (u32)D_800CF4AC) {
        g_padPortMask[0] &= ~0x02080000;
    } else {
        g_padPortMask[0] |= 0x02080000;
    }
    if (D_800D3E88 == 1) {
        if (D_800D3E76 == 0) {
            *D_800CF430 += 10;
        }
        if (D_800D3E76 != 0x80) {
            D_800D3E76 += 4;
        }
    }
    if (D_800D3E88 == 2) {
        if (D_800D3E76 != 0) {
            D_800D3E76 -= 4;
        }
    }
    if (D_800D3E88 == 3) {
        D_800D3E75 = -1;
    }
    D_800D3E68 = (POLY_FT4 *)D_800CF404;
    if (D_800D3E76 != 0 && D_800D3E88 != 0) {
        setRGB0(D_800D3E68, D_800D3E76, D_800D3E76, D_800D3E76);
        if (D_800CCE40 == 0) {
            setXY4(D_800D3E68, 0x60, 0x32, 0xDF, 0x32, 0x60, 0x71, 0xDF, 0x71);
            setUV4(D_800D3E68, 0, 0x40, 0x7F, 0x40, 0, 0x7F, 0x7F, 0x7F);
        } else {
            setXY4(D_800D3E68, 0x20, 0x32, 0x11F, 0x32, 0x20, 0x51, 0x11F, 0x51);
            setUV4(D_800D3E68, 0, 0x40, 0xFF, 0x40, 0, 0x5F, 0xFF, 0x5F);
        }
        D_800D3E68->tpage = 0x2C;
        D_800D3E68->clut = 0x3D28;
        setlen(D_800D3E68, 9);
        setcode(D_800D3E68, 0x2C);
        setSemiTrans(D_800D3E68, 1);
        addPrim(&ot[10], D_800D3E68);
        D_800D3E68++;
        setRGB0(D_800D3E68, D_800D3E76, D_800D3E76, D_800D3E76);
        if (D_800CCE40 == 0) {
            setXY4(D_800D3E68, 0x60, 0x32, 0xDF, 0x32, 0x60, 0x71, 0xDF, 0x71);
            setUV4(D_800D3E68, 0x80, 0x40, 0xFF, 0x40, 0x80, 0x7F, 0xFF, 0x7F);
        } else {
            setXY4(D_800D3E68, 0x20, 0x32, 0x11F, 0x32, 0x20, 0x51, 0x11F, 0x51);
            setUV4(D_800D3E68, 0, 0x60, 0xFF, 0x60, 0, 0x7F, 0xFF, 0x7F);
        }
        D_800D3E68->tpage = 0x4C;
        D_800D3E68->clut = 0x3D28;
        setlen(D_800D3E68, 9);
        setcode(D_800D3E68, 0x2C);
        setSemiTrans(D_800D3E68, 1);
        p = D_800D3E68;
        setaddr(p, getaddr(&ot[10]));
        D_800D3E68 = p + 1;
        setaddr(&ot[10], p);
    }
    D_800CF404 = (s32)D_800D3E68;
}

void func_800C1428(void) {
    CVECTOR c0;
    s32 i;
    u8 f;

    c0 = func_800C1428_color();
    {
        CVECTOR c1 = func_800C1428_color();

        for (i = 0; i < 64; i++) {
            if ((D_800CD10C[D_800D3E74][i].unk0 | D_800CD10C[D_800D3E74][i].unk2) != 0) {
                f = 0;
                if (i >= 7 && (i < 11 || i == 15)) {
                    f = 1;
                }
                if ((D_800D3E74 == 0 || !f) && (u8)func_800C1604(i)) {
                    func_800BF544(D_800CD10C[D_800D3E74][i].unk0, D_800CD10C[D_800D3E74][i].unk2, 1, 1, c0, c1, 16, 40, 9, 1);
                }
            }
        }
    }
}

s32 func_800C1604(u32 arg0) {
    u32 bit = 0;
    u16 mask = 0;

    if (arg0 < 0x10) {
        mask = g_evtGlobalVars.h[0x2E];
        bit = arg0;
    }
    if (arg0 - 0x10 < 0x10) {
        mask = g_evtGlobalVars.h[0x2F];
        bit = arg0 - 0x10;
    }
    if (arg0 - 0x20 < 0x10) {
        mask = g_evtGlobalVars.h[0x30];
        bit = arg0 - 0x20;
    }
    if (arg0 - 0x30 < 0x10) {
        mask = g_evtGlobalVars.h[0x31];
        bit = arg0 - 0x30;
    }
    if ((D_800CD10C[D_800D3E74][arg0].unk0 | D_800CD10C[D_800D3E74][arg0].unk2) == 0) {
        return 0;
    }
    return (mask >> bit) & 1;
}

void func_800C16C0(void) {
    u32 *ot;

    ot = (u32 *)D_800CF460[*D_800CF45C];
    ((POLY_FT4 *)D_800CF404)->y0 = 0x60;
    ((POLY_FT4 *)D_800CF404)->y1 = 0x60;
    ((POLY_FT4 *)D_800CF404)->x0 = 0xF9;
    ((POLY_FT4 *)D_800CF404)->x1 = 0x123;
    ((POLY_FT4 *)D_800CF404)->y2 = 0x7C;
    ((POLY_FT4 *)D_800CF404)->x2 = 0xF9;
    ((POLY_FT4 *)D_800CF404)->x3 = 0x123;
    ((POLY_FT4 *)D_800CF404)->y3 = 0x7C;
    ((POLY_FT4 *)D_800CF404)->u0 = 0x30;
    ((POLY_FT4 *)D_800CF404)->v0 = 0xB0;
    ((POLY_FT4 *)D_800CF404)->u1 = 0x5A;
    ((POLY_FT4 *)D_800CF404)->v1 = 0xB0;
    ((POLY_FT4 *)D_800CF404)->u2 = 0x30;
    ((POLY_FT4 *)D_800CF404)->v2 = 0xCC;
    ((POLY_FT4 *)D_800CF404)->u3 = 0x5A;
    ((POLY_FT4 *)D_800CF404)->v3 = 0xCC;
    ((POLY_FT4 *)D_800CF404)->r0 = 0x80;
    ((POLY_FT4 *)D_800CF404)->g0 = 0x80;
    ((POLY_FT4 *)D_800CF404)->b0 = 0x80;
    ((POLY_FT4 *)D_800CF404)->tpage = 0x8C;
    ((POLY_FT4 *)D_800CF404)->clut = 0x3DA8;
    setlen(D_800CF404, 9);
    ((POLY_FT4 *)D_800CF404)->code = 0x2C;
    addPrim(&ot[10], D_800CF404);
    D_800CF404 += sizeof(POLY_FT4);
    func_800C1824(0xF5, 0x5C, 9, 0);
}

/* draw the four compass frame pieces at (arg0, arg1) */
void func_800C1824(s32 arg0, s32 arg1) {
    POLY_FT4 *p;
    s16 clut;
    u32 *ot;
    s32 i;

    p = (POLY_FT4 *)D_800CF404;
    clut = GetClut(0x280, 0xF0);
    ot = (u32 *)D_800CF460[*D_800CF45C];
    for (i = 0; i < 4; i++) {
        SetPolyFT4(p);
        if (i < 2) {
            setXYWH(p, D_800CD744[i].unk0 + arg0, D_800CD744[i].unk2 + arg1, D_800CD744[i].unk8,
                    D_800CD744[i].unkA);
        } else {
            /* rotated a quarter turn */
            setXY4(p, D_800CD744[i].unk0 + arg0, D_800CD744[i].unk2 + arg1 + D_800CD744[i].unk8,
                   D_800CD744[i].unk0 + arg0, arg1 + D_800CD744[i].unk2 - (i != 2),
                   D_800CD744[i].unk0 + arg0 + D_800CD744[i].unkA,
                   D_800CD744[i].unk2 + arg1 + D_800CD744[i].unk8,
                   D_800CD744[i].unk0 + arg0 + D_800CD744[i].unkA, arg1 + D_800CD744[i].unk2 - (i != 2));
        }
        setUVWH(p, D_800CD744[i].unk4, D_800CD744[i].unk6, D_800CD744[i].unk8, D_800CD744[i].unkA);
        setRGB0(p, 0x80, 0x80, 0x80);
        p->tpage = 0xC;
        p->clut = clut;
        addPrim(&ot[10], p);
        p++;
    }
    D_800CF404 = (s32)p;
}

void func_800C1B00(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, u8 arg4) {
    switch (arg4) {
    case 0:
        *arg2 = ((arg0 - 0x30000) * 14 / 16 >> 11) + 15;
        *arg3 = ((-0x22000 - arg1) * 14 / 16 >> 11) + 6;
        break;
    case 1:
        *arg2 = (arg0 - 0x30000) / 819 * 14 / 16 + 31;
        *arg3 = (-0x22000 - arg1) / 819 * 14 / 16 + 12;
        break;
    case 2:
        *arg2 = arg0 >> 12;
        *arg3 = -arg1 >> 12;
        break;
    case 3:
        *arg2 = arg0 / 1638;
        *arg3 = -arg1 / 1638;
        break;
    }
}

/* initialise the frame pack slot list */
void func_800C1C98(void) {
    u32 i;

    D_800D3EC0[0].unk0 = &D_800D3EA8;
    D_800D3EC0[0].unk4 = &D_800D3EC0[1];
    D_800D3EC0[0].unkE = 0;
    D_800D3EC0[0].unkC = -1;
    D_800D3EC0[0].unk8 = D_800CE3E8[0];
    for (i = 1; i < 5; i++) {
        D_800D3EC0[i].unk0 = &D_800D3EC0[i - 1];
        D_800D3EC0[i].unk4 = &D_800D3EC0[i + 1];
        D_800D3EC0[i].unk8 = NULL;
        D_800D3EC0[i].unkE = 0;
        D_800D3EC0[i].unkC = -1;
    }
    for (i = 1; i < 3; i++) {
        D_800D3EC0[i].unk8 = D_800CE3E8[i];
    }
    i = 5;
    D_800D3EC0[i].unk0 = &D_800D3EC0[i - 1];
    D_800D3EC0[i].unk4 = &D_800D3E98;
    D_800D3EC0[i].unkC = -1;
    D_800D3EC0[i].unk8 = NULL;
    D_800D3EC0[i].unkE = 0;
    D_800D3EA8.unk0 = NULL;
    D_800D3EA8.unk4 = &D_800D3EC0[0];
    D_800D3E98.unk4 = &D_800D3EC0[3];
    D_800D3E90 = 0;
    D_800D3E98.unk0 = NULL;
    D_800D3EB8 = 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A7FF0);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A7FF4);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A7FF8);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A7FFC);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A8000);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A8020);
