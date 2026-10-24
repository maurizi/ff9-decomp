#include "ovl_0c.h"

const char D_800A80BC[] = "w_frmbufRequest:reqover [%d/%d]\n";
const char D_800A80E0[] = "==== ERROR ===== / %s";
const char D_800A80F8[] = "-- w_framebufRequest Error --\n";

s32 w_frmbufRequest(s16 arg0) {
    WmLNode *p;
    u32 i;

    if (arg0 & 0x3F) {
        p = D_800D3F38.unk4;
        i = 0;
        while (1) {
            if (p->unk8 == arg0) {
                p->unk0->unk4 = p->unk4;
                p->unk4->unk0 = p->unk0;
                p->unk4 = D_800D3F38.unk4;
                p->unk0 = &D_800D3F38;
                D_800D3F38.unk4->unk0 = p;
                D_800D3F38.unk4 = p;
                break;
            }
            if (p->unk8 == -1) {
                D_800D3F20++;
                if (D_800D3F20 >= 3) {
                    sprintf(g_worldMapErrorMsg, D_800A80BC, D_800D3F20, 2);
                    printf(D_800A80E0, g_worldMapErrorMsg);
                    g_worldMapError = 1;
                }
                p->unk8 = arg0;
                p->unk0->unk4 = p->unk4;
                p->unk4->unk0 = p->unk0;
                p->unk4 = D_800D3F38.unk4;
                p->unk0 = &D_800D3F38;
                D_800D3F44++;
                D_800D3F38.unk4->unk0 = p;
                D_800D3F38.unk4 = p;
                break;
            }
            p = p->unk4;
            i++;
            if (i >= 4) {
                break;
            }
        }
        if (i == 4) {
            printf(D_800A80F8);
            return 0;
        }
    }
    return 1;
}

void func_800C23CC(void) {
    WmLNode *p;
    WmLNode *q;
    u32 i;

    D_800D3F20 = 0;
    if (D_800D3F44 != 0) {
        p = D_800D3F38.unk4;
        i = 0;
        D_800D3F44 = 0;
        q = p;
        do {
            p = p->unk4;
            i++;
        } while (i < 2);
        for (;;) {
            while ((p != &D_800D3F28) & (p->unkA == 0)) {
                if (((u8)p->unkA == 0) & (p->unk8 != -1)) {
                    p->unk8 = -1;
                }
                p = p->unk4;
            }
            if (p == &D_800D3F28) {
                break;
            }
            while (q->unkA != 0) {
                q = q->unk4;
            }
            q->unkA = p->unkA;
            p->unkA = 0;
            p->unkB = 0;
            p->unk8 = -1;
            p = p->unk4;
        }
    }
}

void func_800C24D8(void) {
    WmLNode *p;
    u32 n;
    u32 i;
    s32 r;
    s32 id;
    s32 k;
    s16 *px;
    s16 *py;
    s16 *cx;
    s16 *cy;

    n = 0;
    p = D_800D3F38.unk4;
    for (i = 0; i < 2; i++, p = p->unk4) {
        if (p->unk8 != -1 && p->unkA != 0 && p->unkB == 0) {
            r = func_800C2050(p->unk8, 1);
            if (r != 0) {
                D_800D3F78[n].unk0 = r;
                D_800D3F78[n].unk5 = p->unkA;
                D_800D3F78[n].unk4 = p->unk8 & 0x3F;
                n++;
                p->unkB = 1;
            }
        }
    }
    for (i = 0; i < n; i++) {
        id = D_800D3F78[i].unk4;
        k = D_800D3F78[i].unk5 - 1;
        if (g_worldMapDebugFlags & 0x200) {
            printf(D_800A8118, (u8)(id + 0x60), D_800D3F78[i].unk0, k);
        }
        px = &D_800CDF14[k];
        py = &D_800CDF18[k];
        cx = &D_800CDF1C[k];
        cy = &D_800CDF20[k];
        func_800B96CC((s32 *)D_800D3F78[i].unk0, *px, *py, *cx, *cy);
        D_800D2FF8[id].unk8 = getTPage(1, 1, *px, *py);
        D_800D2FF8[id].unkA = getClut(*cx, *cy);
    }
}

void func_800C26DC(void) {
}

void func_800C26E4(void) {
}

void func_800C26EC(void) {
    s32 t;

    if (D_800D3F9C != 0) {
        func_800C2830();
    }
    if (D_800D3F94 >= 0) {
        D_800D3F94 += D_800D3F90;
        if (D_800D3F94 > 0x1000) {
            D_800D3F94 = 0x1000;
        }
        func_800C3954(&D_800D3FA0.unk5E0[1], &D_800D3FA0.unk5E0[3], &D_800D3FA0.unk5E0[4], D_800D3F94, D_800D3F94);
        func_800C3954(&D_800D3FA0.unk5E0[2], &D_800D3FA0.unk5E0[5], &D_800D3FA0.unk5E0[6], D_800D3F94, D_800D3F94);
        if (D_800D3F94 == 0x1000) {
            D_800D3F94 = -1;
        }
    }
    t = -D_800CE108[1];
    t -= 0x898;
    t <<= 12;
    t /= 4500;
    if (t > 0x1000) {
        t = 0x1000;
    }
    if (t < 0) {
        t = 0;
    }
    func_800C3954(&D_800D3FA0.unk5E0[0], &D_800D3FA0.unk5E0[1], &D_800D3FA0.unk5E0[2], t, t);
    func_800B9774();
    func_800B8664();
    func_8001CCB4((u8)D_800D3FA0.unk5E0[0].unk30[1].unk4[0], (u8)D_800D3FA0.unk5E0[0].unk30[1].unk4[1],
                  (u8)D_800D3FA0.unk5E0[0].unk30[1].unk4[2]);
}

void func_800C2830(void) {
    s32 id = func_800C291C();

    if (id != D_800D3F8C) {
        D_800D3F8C = id;
        func_800C2A80(id, func_800C3E88() & 0xFF, 0x80);
    }
    if (g_evtGlobalVars.b[0x66] == 0 && D_800CF4AC == 10) {
        switch ((D_800CCCC0[D_800CE3DC->unk11].unk8 & 0x3F00) >> 8) {
        case 9:
        case 12:
        case 13:
            D_800D3FA0.unk0[3].unk30[0].unk0 = 0x7F58;
            D_800D3FA0.unk0[3].unk30[0].unk2 = 0x7F58;
            func_800C2A80(id, func_800C3E88() & 0xFF, 0x80);
            D_800CF46D = 0;
            break;
        }
    }
}

s32 func_800C291C(void) {
    s32 dx;
    s32 dz;
    s32 r;
    s32 d;

    dx = (D_800CE3DC->pos.vx >> 8) - 0x508;
    dz = (D_800CE3DC->pos.vz >> 8) + 0x3AD;
    r = SquareRoot0(dx * dx + dz * dz) < 0x56;
    dx = (D_800CE3DC->pos.vx >> 8) - 0x50B;
    dz = (D_800CE3DC->pos.vz >> 8) + 0x3A0;
    if (SquareRoot0(dx * dx + dz * dz) < 0x3B) {
        r = 2;
    }
    dx = (D_800CE3DC->pos.vx >> 8) - 0x1F4;
    dz = (D_800CE3DC->pos.vz >> 8) + 0x316;
    if (SquareRoot0(dx * dx + dz * dz) < 0xB4) {
        r = 1;
    }
    if (g_worldDataSet == 4) {
        dx = (D_800CE3DC->pos.vx >> 8) - 0x2FE;
        dz = (D_800CE3DC->pos.vz >> 8) + 0x140;
        d = SquareRoot0(dx * dx + dz * dz);
        D_800D3F88 = d;
        if (d < 0x65) {
            r = 3;
        }
    }
    return r;
}

void func_800C2A80(s32 mode, s32 flag, s32 arg2) {
    if (mode == 5) {
        mode = D_800D3F98;
    }
    if (flag == 2) {
        flag = g_worldFog;
    }
    D_800D3F98 = mode;
    g_worldFog = flag;
    D_800D3F90 = arg2;
    D_800D3F94 = 0;
    D_800D3FA0.unk5E0[3] = D_800D3FA0.unk5E0[1];
    D_800D3FA0.unk5E0[5] = D_800D3FA0.unk5E0[2];
    if (g_worldFog != 0) {
        switch (mode) {
        case 0:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[3];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[2];
            return;
        case 1:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[7];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[6];
            return;
        case 2:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[11];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[10];
            return;
        case 3:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[15];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[14];
            return;
        }
    } else {
        switch (mode) {
        case 0:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[1];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[0];
            return;
        case 1:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[5];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[4];
            return;
        case 2:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[9];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[8];
            return;
        case 3:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[13];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[12];
            return;
        }
    }
    printf(D_800A8178, mode);
}

/* blend the sky parameter blocks arg1 -> arg2 by arg3 (0..0x1000) into arg0 */
void func_800C3954(WmSky *arg0, WmSky *arg1, WmSky *arg2, s32 arg3, s32 arg4) {
    s16 *d;
    s16 *a;
    s16 *b;

    if (arg3 < 0) {
        arg3 = 0;
    }
    if (arg3 > 0x1000) {
        arg3 = 0x1000;
    }
    if (arg4 < 0) {
        arg4 = 0;
    }
    if (arg4 > 0x1000) {
        arg4 = 0x1000;
    }
    arg0->unk0[0].vx = func_800B822C(arg1->unk0[0].vx, arg2->unk0[0].vx, arg3, 0xD);
    arg0->unk0[0].vy = func_800B822C(arg1->unk0[0].vy, arg2->unk0[0].vy, arg3, 0xD);
    arg0->unk0[0].vz = func_800B822C(arg1->unk0[0].vz, arg2->unk0[0].vz, arg3, 0xD);
    d = &arg0->unk0[1].vx;
    a = &arg1->unk0[1].vx;
    b = &arg2->unk0[1].vx;
    d[0] = func_800B822C(a[0], b[0], arg3, 0xD);
    d[1] = func_800B822C(a[1], b[1], arg3, 0xD);
    d[2] = func_800B822C(a[2], b[2], arg3, 0xD);
    d = &arg0->unk0[2].vx;
    a = &arg1->unk0[2].vx;
    b = &arg2->unk0[2].vx;
    d[0] = func_800B822C(a[0], b[0], arg3, 0xD);
    d[1] = func_800B822C(a[1], b[1], arg3, 0xD);
    d[2] = func_800B822C(a[2], b[2], arg3, 0xD);
    d = &arg0->unk18.vx;
    a = &arg1->unk18.vx;
    b = &arg2->unk18.vx;
    d[0] = func_800B822C(a[0], b[0], arg3, 0xD);
    d[1] = func_800B822C(a[1], b[1], arg3, 0xD);
    d[2] = func_800B822C(a[2], b[2], arg3, 0xD);
    d = arg0->unk54;
    a = arg1->unk54;
    b = arg2->unk54;
    d[0] = func_800B822C(a[0], b[0], arg3, 0xD);
    d[1] = func_800B822C(a[1], b[1], arg3, 0xD);
    d[2] = func_800B822C(a[2], b[2], arg3, 0xD);
    arg0->unk5C = func_800B822C((s16)arg1->unk5C, (s16)arg2->unk5C, arg3, 0xD);
    d = &arg0->unk20.v.vx;
    a = &arg1->unk20.v.vx;
    b = &arg2->unk20.v.vx;
    d[0] = func_800B81C0(a[0] << 12, b[0] << 12, arg3) >> 12;
    d[1] = func_800B81C0(a[1] << 12, b[1] << 12, arg3) >> 12;
    d[2] = func_800B81C0(a[2] << 12, b[2] << 12, arg3) >> 12;
    d = &arg0->unk28.v.vx;
    a = &arg1->unk28.v.vx;
    b = &arg2->unk28.v.vx;
    d[0] = func_800B81C0(a[0] << 12, b[0] << 12, arg3) >> 12;
    d[1] = func_800B81C0(a[1] << 12, b[1] << 12, arg3) >> 12;
    d[2] = func_800B81C0(a[2] << 12, b[2] << 12, arg3) >> 12;
    d = arg0->unk30[0].unk4;
    a = arg1->unk30[0].unk4;
    b = arg2->unk30[0].unk4;
    d[0] = func_800B81C0(a[0], b[0], arg3);
    d[1] = func_800B81C0(a[1], b[1], arg3);
    d[2] = func_800B81C0(a[2], b[2], arg3);
    d = arg0->unk30[1].unk4;
    a = arg1->unk30[1].unk4;
    b = arg2->unk30[1].unk4;
    d[0] = func_800B81C0(a[0], b[0], arg3);
    d[1] = func_800B81C0(a[1], b[1], arg3);
    d[2] = func_800B81C0(a[2], b[2], arg3);
    d = arg0->unk30[2].unk4;
    a = arg1->unk30[2].unk4;
    b = arg2->unk30[2].unk4;
    d[0] = func_800B81C0(a[0], b[0], arg3);
    d[1] = func_800B81C0(a[1], b[1], arg3);
    d[2] = func_800B81C0(a[2], b[2], arg3);
    arg0->unk30[0].unk0 = func_800B81C0(arg1->unk30[0].unk0, arg2->unk30[0].unk0, arg3);
    arg0->unk30[0].unk2 = func_800B81C0((u16)arg1->unk30[0].unk2, (u16)arg2->unk30[0].unk2, arg3);
    arg0->unk30[1].unk0 = func_800B81C0(arg1->unk30[1].unk0, arg2->unk30[1].unk0, arg3);
    arg0->unk30[1].unk2 = func_800B81C0((u16)arg1->unk30[1].unk2, (u16)arg2->unk30[1].unk2, arg3);
    arg0->unk30[2].unk0 = func_800B81C0(arg1->unk30[2].unk0, arg2->unk30[2].unk0, arg3);
    arg0->unk30[2].unk2 = func_800B81C0((u16)arg1->unk30[2].unk2, (u16)arg2->unk30[2].unk2, arg3);
    arg0->unk30[0].unk0 &= 0xFFFE;
    arg0->unk30[0].unk2 = (u16)arg0->unk30[0].unk2 & 0xFFFE;
    arg0->unk30[1].unk0 &= 0xFFFE;
    arg0->unk30[1].unk2 = (u16)arg0->unk30[1].unk2 & 0xFFFE;
    arg0->unk30[2].unk0 &= 0xFFFE;
    arg0->unk30[2].unk2 = (u16)arg0->unk30[2].unk2 & 0xFFFE;
}

void func_800C3DEC(void) {
    D_800D3F9C = 1;
    D_800D3F98 = -2;
    D_800D3F8C = -1;
    D_800D3F94 = 0;
    D_800D3F90 = 0x80;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_frmbuf", D_800A8118);

void func_800C3E28(void) {
    D_800D3F8C = func_800C291C();
    g_worldFog = func_800C3E88();
    printf("start weather mode : %d / fog : %d\n", D_800D3F8C, g_worldFog);
    func_800C2A80(D_800D3F8C, g_worldFog, 0x1000);
}

s32 func_800C3E88(void) {
    return (u32)(*D_800CF430 - 0x1766) >= 0x13ED;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_frmbuf", D_800A8178);
