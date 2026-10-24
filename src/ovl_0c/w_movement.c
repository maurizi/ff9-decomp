#include "ovl_0c.h"

void func_800AD204(void) {
    void *p = &g_gameState->unk1C->unk880;

    D_800CE0E8 = p;
    D_800CE0F0 = p;
    g_gameState->unk1C->unkA = 0xDC;
    SetGeomOffset(0xA0, 0x70);
    D_800CE1B0.vx = 0;
    D_800CE1B0.vy = -0x4E20;
    D_800CE1B0.vz = 0;
    D_800CE202 = 1;
    D_800CE0EF = 0;
    D_800CE100 = 0;
    D_800CE1E1 = 0;
    if (g_gameState->unkA == 1 && g_gameState->unk1C->unk890 == 0) {
        D_800CE0F0->unk2 = 0;
        D_800CE0F0->unk4 = 0;
        D_800CE0F0->unk8 = 0;
        D_800CE0F0->unkC = 0;
        D_800CE0E8->unk11 = 0;
    }
    if (D_800CE0F0->unk4 < 0) {
        D_800CE0F0->unk2 = 0;
    }
    if (D_800CE0F0->unk4 > 0) {
        D_800CE0F0->unk2 = 0x1000;
    }
    D_800CE1A8 = 0;
    D_800CE188 = 0;
    D_800CE1E8 = D_800CCA68[1];
    D_800CE0EC = 0;
    D_800CE0ED = 0;
    D_800CE0EE = 0;
    D_800CE1F2 = 1;
    D_800CE1F3 = 0;
}

void func_800AD39C(void) {
    SVECTOR r;
    VECTOR v;
    MATRIX m;
    s32 t;
    s32 a;

    if (D_800CE202 == 0) {
        func_800AE280();
    }
    g_gameState->unk1C->unkA = D_800CE1E8.unk0[0];
    D_800CE0F0->unkC &= 0xFFF;
    D_800CE1C4 = D_800CE1E8.unk0[4];
    if ((u8)D_800CE3C4 == 8 || (u8)D_800CE3C4 == 9) {
        t = D_800CE3DC->pos.vy + 0x1518;
        D_800CE1C4 = D_800CE1E8.unk0[4] - (t > 0 ? t >> 3 : t / 6);
    }
    D_800CE1C8.vx = math_sin(D_800CE0F0->unkC) * (D_800CE1E8.unk0[1] * D_800CCA24 / 4096) >> 12;
    D_800CE1C8.vy = D_800CE1E8.unk0[2];
    D_800CE1C8.vz = math_cos(D_800CE0F0->unkC) * (D_800CE1E8.unk0[1] * D_800CCA24 / 4096) >> 12;
    if (D_800CE202 == 0) {
        func_800AD738();
    }
    D_800CE190.vx = D_800CE198[0] - D_800CE108[0];
    D_800CE190.vy = D_800CE198[1] - D_800CE108[1];
    D_800CE190.vz = D_800CE198[2] - D_800CE108[2];
    memset(&r, 0, 8);
    r.vz = D_800CE100;
    r.vy = D_800CE0F0->unkC;
    func_80030AF4(&r, &m);
    m.t[0] = 0;
    m.t[1] = 0;
    m.t[2] = 0;
    ApplyMatrixLV(&m, &D_800CE1B0, &v);
    func_800AE880(D_800CF448, &D_800CE190, &D_800CE0F8, &v);
    D_800CE1D0[1] = 0;
    D_800CE1D0[0] = D_800CE108[0] - D_800CE198[0];
    D_800CE1D0[2] = D_800CE108[2] - D_800CE198[2];
    if (D_800CE1D0[0] > 0xFFFF) {
        D_800CE1D0[0] -= 0x60000;
    }
    if (D_800CE1D0[2] > 0xFFFF) {
        D_800CE1D0[2] -= 0x50000;
    }
    if (D_800CE1D0[0] < -0xFFFF) {
        D_800CE1D0[0] += 0x60000;
    }
    if (D_800CE1D0[2] < -0xFFFF) {
        D_800CE1D0[2] += 0x50000;
    }
    a = ratan2(D_800CE1D0[2], D_800CE1D0[0]);
    D_800CE1D0[0] = math_cos(a);
    D_800CE1D0[2] = math_sin(a);
    VectorNormal((VECTOR *)D_800CE1D0, (VECTOR *)D_800CE1D0);
    if (*D_800CF430 < 0x137E && ((D_800CCCC0[D_800CE3DC->unk11].unk8 & 0x3F00) >> 8) == 0xC) {
        D_800CE0F0->unk8 = 1;
    }
}

void func_800AD738(void) {
    VECTOR v;
    u32 c0;
    s32 t;
    s32 h0;
    u32 c1;
    s32 h1;
    u32 c2;
    s32 h2;
    u32 c3;
    s32 h3;
    u32 cell;
    s32 h;
    s32 res;
    s32 x;
    s32 y;
    s32 z;
    s32 d;
    s32 dir;
    s32 step;
    s32 a;
    s32 k;
    s32 x2, y2, z2, d2, step2, dir2;
    s32 dx, dz, s, r;
    u8 *p;

    res = 0;
    if (func_8004B1FC() == 0) {
        y = D_800CE3DC->pos.vy + D_800CE1C4;
        x = D_800CE3DC->pos.vx;
        z = D_800CE3DC->pos.vz;
        if ((D_800CE1E0 = D_800CE0EC | D_800CE0ED | D_800CE0EE) != 0) {
            if (D_800CE0EC == 1) {
                if (abs(D_800CE108[0] - x) < 16) {
                    D_800CE0EC = 0;
                } else {
                    D_800CE108[0] += (func_8004AE00()->pos.vx - D_800CE108[0]) / 8;
                }
            }
            if (D_800CE0ED == 1) {
                if (abs(D_800CE108[1] - y) < 16) {
                    D_800CE0ED = 0;
                } else {
                    D_800CE108[1] += (func_8004AE00()->pos.vy - D_800CE108[1] + D_800CE1C4) / 8;
                }
            }
            if (D_800CE0EE == 1) {
                if (abs(D_800CE108[2] - z) < 16) {
                    D_800CE0EE = 0;
                } else {
                    D_800CE108[2] += (func_8004AE00()->pos.vz - D_800CE108[2]) / 8;
                }
            }
            if (D_800CE0EC == 0) {
                D_800CE108[0] = x;
            }
            if (D_800CE0ED == 0) {
                D_800CE108[1] = y;
            }
            if (D_800CE0EE == 0) {
                D_800CE108[2] = z;
            }
        } else {
            step = 0;
            d = y - D_800CE108[1];
            dir = d > 0x80;
            if (d < -0x80) {
                dir = 2;
            }
            D_800CCAFC = (d >= 0 ? d : -d) >> 5;
            if (D_800CCAFC < 0x20) {
                D_800CCAFC = 0x20;
            }
            if (D_800CCAFC > 0x100) {
                D_800CCAFC = 0x100;
            }
            if (dir != 0) {
                switch (D_800CE3C8->unk0) {
                case 1:
                case 2:
                    switch (dir) {
                    case 2:
                        step = (d + 0x80) / 8;
                        break;
                    case 1:
                        step = (d - 0x80) / 2;
                        break;
                    }
                    break;
                case 0:
                case 3:
                    switch (dir) {
                    case 2:
                        step = ((d + 0x80) << 8) / (D_800CCAFC << 5);
                        break;
                    case 1:
                        step = ((d - 0x80) << 8) / (D_800CCAFC << 5);
                        break;
                    }
                    break;
                }
            }
            D_800CE108[0] = x;
            D_800CE108[2] = z;
            if (ABS(step) >= 5 && D_800CE0EF == 0) {
                D_800CE108[1] += step;
            }
        }
    } else {
        D_800CE3DC->pos.vx = D_800CE108[0];
        D_800CE3DC->pos.vy = D_800CE108[1];
        D_800CE3DC->pos.vz = D_800CE108[2];
    }
    if (func_8004B1EC() == 0) {
        k = 0;
        y2 = D_800CE3DC->pos.vy + D_800CE1C8.vy;
        x2 = D_800CE3DC->pos.vx + D_800CE1C8.vx;
        z2 = D_800CE3DC->pos.vz + D_800CE1C8.vz;
        v.vx = x2 + (D_800CE1D0[0] >> 2);
        v.vy = y2;
        v.vz = z2 + (D_800CE1D0[2] >> 2);
        if (D_800CE1F2 != 0) {
            v.vx += 0x580;
            func_800BC0A8(&v.vx, &v.vz);
            func_800BE748(&v, &c0, &t, &D_800CE118[0], (u16 *)&h0);
            v.vx -= 0xB00;
            func_800BC0A8(&v.vx, &v.vz);
            func_800BE748(&v, &c1, &t, &D_800CE118[1], (u16 *)&h1);
            v.vx += 0x580;
            v.vz += 0x580;
            func_800BC0A8(&v.vx, &v.vz);
            func_800BE748(&v, &c2, &t, &D_800CE118[2], (u16 *)&h2);
            v.vz -= 0xB00;
            func_800BC0A8(&v.vx, &v.vz);
            func_800BE748(&v, &c3, &t, &D_800CE118[3], (u16 *)&h3);
            h = h0;
            cell = c0;
            if (h1 < h) {
                h = h1;
                cell = c1;
            }
            if (h2 < h) {
                h = h2;
                cell = c2;
            }
            if (h3 < h) {
                h = h3;
                cell = c3;
            }
        } else {
            func_800BC0A8(&v.vx, &v.vz);
            func_800BE748(&v, &cell, &t, &D_800CE118[0], (u16 *)&h);
        }
        if (D_800CE3C8->unk0 == 1) {
            if (h < -0x2134) {
                h = -0x2134;
            }
        } else if (h < -0x251C) {
            h = -0x251C;
        }
        if (D_800CE1E8.unk0[3] + h < y2) {
            k = ((u8)cell >> 2 == 0x31) * 2;
            if (D_800CE26D != 0) {
                k = 2;
            }
            if (D_800CE25D != 0) {
                k = 2;
            }
            if (D_800CE0F0->unk2 != 0 && D_800CE0F0->unk2 != 0x1000) {
                k = 3;
            }
            y2 = D_800CE1E8.unk0[3] + h;
        }
        if (D_800CE3C8->unk0 == 1 && y2 > -0x1194) {
            y2 = -0x1194;
        }
        d = y2 - D_800CE198[1];
        dir = d > 0x100;
        step = 0;
        if (d < -0x100) {
            dir = 2;
        }
        D_800CCAFC = (d >= 0 ? d : -d) >> D_800CCB04;
        if (D_800CCAFC < D_800CCB08) {
            D_800CCAFC = D_800CCB08;
        }
        if (D_800CCAFC > D_800CCB0C) {
            D_800CCAFC = D_800CCB0C;
        }
        if (dir != 0) {
            switch (D_800CE3C8->unk0) {
            case 1:
            case 2:
                switch (dir) {
                case 2:
                    step = (d + 0x100) / 4;
                    break;
                case 1:
                    step = (d - 0x100) / 32;
                    break;
                }
                break;
            case 0:
            case 3:
                switch (dir) {
                case 2:
                    step = ((d + 0x100) << 8) / (((D_800CCB10 - 2) * D_800CCAFC * 7) >> k);
                    break;
                case 1:
                    step = ((d - 0x100) << 8) / (D_800CCB10 * D_800CCAFC * 3);
                    break;
                }
                a = step >= 0 ? step : -step;
                if (a > 0x200) {
                    a = 0x200;
                }
                a = (((0x200 - a) * D_800CCB14 >> 12) + 0x800) << 1;
                step *= a;
                step /= 4096;
                break;
            }
        }
        D_800CE198[0] = x2;
        D_800CE198[2] = z2;
        if (ABS(step) >= 5 && D_800CE0EF == 0) {
            D_800CE198[1] += step;
        }
        res = y2;
    }
    dx = D_800CE198[0] - 0x2FF77;
    dz = D_800CE198[2] + 0x14031;
    if (ABS(dx) < 0x36B0 && ABS(dz) < 0x36B0) {
        s = 0xBAEB900 - dx * dx;
        s -= dz * dz;
        r = -SquareRoot0(s) + 0x898;
        if (s > 0 && r < D_800CE198[1]) {
            D_800CE198[1] = r;
        }
    }
    switch (D_800CE1F3) {
    case 0:
        if (D_800D33F4 != 0) {
            D_800CE1F3 = 1;
            D_800CE198[1] = res;
            D_800CE108[1] = D_800CE3DC->pos.vy + D_800CE1C4;
        }
        break;
    case 1:
        D_800CE1F3 = 2;
        p = &g_evtGlobalVars.b[0x65];
        if (*p & 1) {
            func_8002FA10();
            g_evtGlobalVars.b[0x65] &= ~1;
        } else {
            func_8002F9D0();
        }
        break;
    }
    if (D_800CE198[1] < -0x47CF) {
        D_800CE198[1] = -0x47CF;
    }
    func_800BC0A8(&D_800CE108[0], &D_800CE108[2]);
    func_800BC0A8(&D_800CE198[0], &D_800CE198[2]);
}

void func_800AE280(void) {
    s32 k;
    s32 m;
    Unk800CE1E8 tmp;

    k = D_800CCA28[w_frameGetParameter(0xC0)];
    m = D_800CE3C8->unk11;
    if (D_800CCA20 != -1) {
        k = D_800CCA20;
    }
    if (D_800CE0F0->unk8 != 0) {
        D_800CE0F0->unk4 = -0x80;
    }
    D_800CE0F0->unk2 += D_800CE0F0->unk4;
    if (D_800CE0F0->unk2 <= 0) {
        D_800CE0F0->unk2 = 0;
        D_800CE0F0->unk4 = 0;
        D_800CE0F0->unk8 = 0;
    }
    if (D_800CE0F0->unk2 >= 0x1000) {
        D_800CE0F0->unk2 = 0x1000;
        D_800CE0F0->unk4 = 0;
        D_800CE0F0->unk8 = 0;
    }
    if (D_800CE188 != 0) {
        D_800CE1A8 += D_800CE188;
        if (D_800CE1A8 > 0x1000) {
            D_800CE1A8 = 0x1000;
        }
        func_800AE4C8(-D_800CE3DC->pos.vy, D_800CE1C0, k, D_800CE0F0->unk2, &tmp);
        D_800CE1E8.unk0[0] = func_800B822C(D_800CE1F8.unk0[0], tmp.unk0[0], D_800CE1A8, 9);
        D_800CE1E8.unk0[1] = func_800B822C(D_800CE1F8.unk0[1], tmp.unk0[1], D_800CE1A8, 0xF);
        D_800CE1E8.unk0[2] = func_800B822C(D_800CE1F8.unk0[2], tmp.unk0[2], D_800CE1A8, 0xE);
        D_800CE1E8.unk0[3] = func_800B822C(D_800CE1F8.unk0[3], tmp.unk0[3], D_800CE1A8, 0xE);
        D_800CE1E8.unk0[4] = func_800B822C(D_800CE1F8.unk0[4], tmp.unk0[4], D_800CE1A8, 0xE);
        if (D_800CE1A8 >= 0x1000) {
            D_800CE188 = 0;
        }
    } else {
        func_800AE4C8(-D_800CE3DC->pos.vy, m, k, math_sin(D_800CE0F0->unk2 / 4), &D_800CE1E8);
    }
}

void func_800AE4C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, Unk800CE1E8 *arg4) {
    Unk800CE1E8 *a;
    Unk800CE1E8 *b;
    Unk800CE1E8 *c;
    s32 t;
    s16 tmp[5];

    c = &D_800CCA68[D_800CCACC[arg1][arg2][2]];
    a = &D_800CCA68[D_800CCACC[arg1][arg2][0]];
    b = &D_800CCA68[D_800CCACC[arg1][arg2][1]];
    t = func_800AEA94(arg0) & 0xFFFF;
    tmp[0] = func_800B822C(a->unk0[0], b->unk0[0], t, 9);
    tmp[1] = func_800B822C(a->unk0[1], b->unk0[1], t, 0xE);
    tmp[2] = func_800B822C(a->unk0[2], b->unk0[2], t, 0xD);
    tmp[3] = func_800B822C(a->unk0[3], b->unk0[3], t, 0xD);
    tmp[4] = func_800B822C(a->unk0[4], b->unk0[4], t, 0xD);
    arg4->unk0[0] = func_800B822C(tmp[0], c->unk0[0], arg3, 9);
    arg4->unk0[1] = func_800B822C(tmp[1], c->unk0[1], arg3, 0xE);
    arg4->unk0[2] = func_800B822C(tmp[2], c->unk0[2], arg3, 0xD);
    arg4->unk0[3] = func_800B822C(tmp[3], c->unk0[3], arg3, 0xD);
    arg4->unk0[4] = func_800B822C(tmp[4], c->unk0[4], arg3, 0xD);
}

void func_800AE670(void) {
    u8 r;

    if (D_800CE0F0->unk8 == 0
        && (*D_800CF430 >= 0x137E || ((D_800CCCC0[D_800CE3DC->unk11].unk8 & 0x3F00) >> 8) != 0xC)) {
        r = 0;
        if (D_800CE0F0->unk2 == 0) {
            r = 1;
            D_800CE0F0->unk4 = 0x100;
        }
        if (D_800CE0F0->unk2 == 0x1000) {
            r = 1;
            D_800CE0F0->unk4 = -0x100;
        }
        if (r == 0) {
            D_800CE0F0->unk4 = -D_800CE0F0->unk4;
        }
    }
}

s32 *func_800AE744(void) {
    return D_800CE108;
}

s32 *func_800AE750(void) {
    return D_800CE198;
}

void func_800AE75C(void) {
    func_800AE280();
    func_800AD738();
}

void func_800AE784(s16 arg0, s16 arg1) {
    D_800CE1A8 = 0;
    D_800CE1C0 = arg0;
    D_800CE1F8 = D_800CE1E8;
    if (D_800D33F4 != 0) {
        D_800CE188 = arg1;
    } else {
        D_800CE188 = 0;
    }
}

s32 func_800AE7F0() {
    s16 v = D_800CE0F0->unk2;

    if (v == 0) {
        return 0;
    }
    if (v == 0x1000) {
        return 2;
    }
    return 1;
}

void func_800AE828(void) {
    if (D_800CE0E8->unk11 != 0) {
        D_800CE27C = 1;
    }
    D_800CE0E8->unk11 = ~D_800CE0E8->unk11;
}

void func_800AE860(void) {
    D_800CE0EC = 1;
    D_800CE0ED = 1;
    D_800CE0EE = 1;
}

void func_800AE880(MATRIX *arg0, SVECTOR *arg1, SVECTOR *arg2, VECTOR *arg3) {
    VECTOR d;
    VECTOR x;
    VECTOR y;
    VECTOR z;

    d.vx = arg2->vx - arg1->vx;
    d.vy = arg2->vy - arg1->vy;
    d.vz = arg2->vz - arg1->vz;
    VectorNormal(&d, &z);
    if (z.vz == arg3->vz) {
        z.vz++;
    }
    gte_ldopv1(&z);
    gte_ldopv2(arg3);
    gte_op12();
    gte_stlvnl(&d);
    VectorNormal(&d, &x);
    gte_ldopv1(&z);
    gte_ldopv2(&x);
    gte_op12();
    gte_stlvnl(&d);
    VectorNormal(&d, &y);
    arg0->m[0][0] = x.vx;
    arg0->m[0][1] = x.vy;
    arg0->m[0][2] = x.vz;
    arg0->m[1][0] = y.vx;
    arg0->m[1][1] = y.vy;
    arg0->m[1][2] = y.vz;
    arg0->m[2][0] = z.vx;
    arg0->m[2][1] = z.vy;
    arg0->m[2][2] = z.vz;
    gte_SetRotMatrix(arg0);
    gte_ldv0(arg1);
    gte_rtv0();
    gte_stlvnl(arg0->t);
    arg0->t[0] = -arg0->t[0];
    arg0->t[1] = -arg0->t[1];
    arg0->t[2] = -arg0->t[2];
}

s32 func_800AEA94(s32 arg0) {
    arg0 = (arg0 - 1000) << 12;
    arg0 /= 4500;
    if (arg0 > 0x1000) {
        arg0 = 0x1000;
    }
    if (arg0 < 0) {
        arg0 = 0;
    }
    return arg0 & 0xFFFF;
}

void func_800AEADC(void) {
    s32 i;
    s32 j;

    D_800CE3DC = &D_800CE210;
    D_800CE3D8 = NULL;
    D_800CE3D0 = NULL;
    D_800CE268 = 0;
    D_800CE270 = 0;
    D_800CE3C4 = -1;
    D_800CE3C8 = 0;
    D_800CE25E = 0;
    D_800CE3BE = 0;
    D_800CE3C0 = 0;
    D_800CE278 = 0;
    D_800CE3E0 = 0;
    D_800CE3BC = 0;
    D_800CE276 = 0;
    D_800CE3CC = 0;
    D_800CE284 = 0;
    D_800CE3D4 = 0x1000;
    D_800CE260 = 0;
    D_800CE264 = 1;
    D_800CE274 = 0;
    D_800CE280 = 0;
    D_800CE25C = 0;
    D_800CE27C = 0;
    D_800CE25D = 0;
    D_800CE26D = 0;
    D_800CE209 = 0;
    for (i = 0; i < 11; i++) {
        D_800CE288[i].unk0 = 0;
        for (j = 0; j < 6; j++) {
            D_800CE288[i].unk4[j] = j;
            D_800CE288[i].unk10[j] = -1;
        }
    }
    for (i = 0; i < 22; i++) {
        D_800CCCC0[i].unk8 = 0;
        D_800CCCC0[i].unkC = 0;
        D_800CCCC0[i].unkE = 0;
    }
    if (g_worldDataSet == 4) {
        D_800CCB70[9].unkB = 3;
        D_800CCB70[0].unkB = 3;
    } else {
        D_800CCB70[9].unkB = 2;
        D_800CCB70[0].unkB = 0;
    }
}

void func_800AEC74(void) {
    s32 id;
    s32 k;
    WmPalAnim *e;
    u16 *pal;
    u16 *dst;
    Unk16D8 *n;
    Unk8004AB64 *a;
    s32 c;
    s32 i;
    s32 j;

    id = 0;
    k = 0;
    e = g_gameState->unk1C->unk8E4.world->unk8->unk28;
    pal = w_framePackGetPtr(D_800CF428, 1);
    func_800B36AC();
    for (n = func_8004AE9C(); n != NULL; n = n->unk0) {
        a = n->unk4;
        if (a->unkE == 1 && (a->unkF & 1) && a->unk5 == 4) {
            if ((a->unk11 >= 3 && a->unk11 < 8)) {
                id = a->unk34;
                switch (a->unk11) {
                case 3:
                    k = 0;
                    break;
                case 4:
                    k = 1;
                    break;
                case 5:
                    k = 2;
                    break;
                case 6:
                    k = 3;
                    break;
                case 7:
                    k = 4;
                    break;
                }
                if (D_800CE280 != 0) {
                    k = D_800CE280 - 1;
                }
            }
        }
    }
    c = 0;
    for (i = 0; i < g_gameState->unk1C->unk8E4.world->unk8->unk1A; i++, e++) {
        if (e->unk0 == id) {
            dst = e->unk8;
            switch (c) {
            case 0:
                for (j = 0; j < 16; j++) {
                    *dst++ = pal[k * 48 + j + 32];
                }
                break;
            case 1:
                for (j = 0; j < 16; j++) {
                    *dst++ = pal[k * 48 + j + 16];
                }
                break;
            case 2:
                for (j = 0; j < 16; j++) {
                    *dst++ = pal[k * 48 + j + 0];
                }
                break;
            }
            c++;
        }
    }
}

/* find the world map actors (vehicles) and sync the camera with the player */
void func_800AEF10(void) {
    Unk16D8 *n;
    Unk8004AB64 *a;

    for (n = func_8004AE9C(); n != NULL; n = n->unk0) {
        if (n->unk4->unk5 == 4) {
            a = n->unk4;
            switch (a->unk11) {
            case 1:
            case 2:
                D_800CE3D8 = a;
                break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                D_800CE268 = a;
                break;
            case 8:
                D_800CE3D0 = a;
                break;
            case 9:
                D_800CE3D0 = a;
                break;
            case 10:
                D_800CE3D0 = a;
                break;
            }
            if (a == func_8004AE00()) {
                if (g_gameState->unkA == 1 && g_gameState->unk1C->unk892 == 0) {
                    if (D_800CE3C8->unk10 != 0) {
                        D_800CE0F0->unkE = 0x800;
                    } else {
                        D_800CE0F0->unkE = 0;
                    }
                    D_800CE0F0->unkC = a->pos.rotY + D_800CE0F0->unkE;
                }
                D_800CE3CC = a->pos.rotY;
            }
        }
    }
    g_gameState->unk1C->unk890 = 0;
    D_800CF47C = 1;
}

void func_800AF08C(void) {
    SVECTOR n;
    VECTOR a;
    VECTOR b;
    VECTOR dv;
    SVECTOR s;
    s32 z;
    s32 base;
    s32 d;
    s32 ang;
    Unk16D8 *e;
    Unk8004AB64 *act;
    u8 f;

    VectorNormalS((VECTOR *)D_800CE1D0, &n);
    base = (-ratan2(n.vz, n.vx) + 0x400) % 4096;
    func_80064CDC(10, 100);
    for (e = func_8004AE9C(); e != NULL; e = e->unk0) {
        act = e->unk4;
        if (act->unk5 == 4 && act->unkE == 1 && (act->unkF & 1) && (u8)func_800BBE24((VECTOR *)&act->pos, &a, &b, &z)) {
            f = 1;
            dv.vx = act->pos.vx - (D_800CE198[0] - n.vx);
            dv.vz = act->pos.vz - (D_800CE198[2] - n.vz);
            if (dv.vx > D_800D32D8 - 0x8000) {
                dv.vx -= D_800D32D8;
            }
            if (dv.vz > D_800D32DC - 0x8000) {
                dv.vz -= D_800D32DC;
            }
            if (dv.vx < 0x8000 - D_800D32D8) {
                dv.vx = D_800D32D8 + dv.vx;
            }
            if (dv.vz < 0x8000 - D_800D32DC) {
                dv.vz = D_800D32DC + dv.vz;
            }
            ang = -ratan2(dv.vz, dv.vx) + 0x400;
            d = ang % 4096;
            d = d - base;
            if (d < -0x800) {
                d += 0x1000;
            }
            if (d > 0x800) {
                d -= 0x1000;
            }
            if (((d < 0 ? -d : d) < 400 || func_800AE7F0(ang) != 0) && (b.vz >= 2001 && b.vz < 29000) && b.vx > -20000 && b.vx < 20000 && b.vy > -20000 && b.vy < 20000 && a.vx > -29000 && a.vx < 29000 && a.vz > -29000 && a.vz < 29000) {
                s.vx = a.vx;
                s.vy = a.vy;
                s.vz = a.vz;
                func_800AF414(act, b.vz >> 3);
                func_800AF66C(act, &s, z);
                if (act->unk11 == 8) {
                    f = 0;
                }
                if (act->unk11 == 1 || act->unk11 == 2) {
                    if ((u8)D_800CE3C4 >= 1 && (u8)D_800CE3C4 <= 6) {
                        f = 0;
                    }
                }
                if (D_800CCCC0[act->unk11].unkC < 4 && f) {
                    func_800AF8C0(act, &s, b.vz);
                }
            }
        }
    }
    func_80064C24(0);
    SetFarColor(0, 0, 0);
}

void func_800AF414(Unk8004AB64 *arg0, s32 arg1) {
    s16 col[3];
    s32 t;
    s32 f;
    s32 a;
    s32 d;
    s32 i;
    s32 id;
    WmPalAnim *e;
    RECT *r;

    id = arg0->unk34;
    e = g_gameState->unk1C->unk8E4.world->unk8->unk28;
    t = -arg0->pos.vy;
    t -= 0xC1F;
    t <<= 12;
    t /= 2176;
    if (t > 0x1000) {
        t = 0x1000;
    }
    if (t < 0) {
        t = 0;
    }
    a = func_800B81C0(D_800D3FA0.unk5E0[0].unk30[1].unk0, D_800D3FA0.unk5E0[0].unk30[0].unk0, t);
    col[0] = func_800B81C0(D_800D3FA0.unk5E0[0].unk30[1].unk4[0], D_800D3FA0.unk5E0[0].unk30[0].unk4[0], t);
    col[1] = func_800B81C0(D_800D3FA0.unk5E0[0].unk30[1].unk4[1], D_800D3FA0.unk5E0[0].unk30[0].unk4[1], t);
    col[2] = func_800B81C0(D_800D3FA0.unk5E0[0].unk30[1].unk4[2], D_800D3FA0.unk5E0[0].unk30[0].unk4[2], t);
    SetFarColor((u8)col[0], (u8)col[1], (u8)col[2]);
    f = (arg1 * 8 + (d = a - 0x8000)) >> 2;
    if (f < 0) {
        f = 0;
    }
    if (f >= 0x1000) {
        f = 0xFFF;
    }
    for (i = 0; i < g_gameState->unk1C->unk8E4.world->unk8->unk1A; i++, e++) {
        if (e->unk0 == id) {
            r = (RECT *)D_800CF404;
            if (f < 0) {
                f = 0;
            }
            if (f > 0x1000) {
                f = 0x1000;
            }
            func_800B3998(e->unk8, e->unk28[*D_800CF45C], f, D_800D45D4);
            r->x = e->unk4;
            r->y = e->unk6;
            r->w = 16;
            r->h = 1;
            LoadImage(r, (u32 *)e->unk28[*D_800CF45C]);
        }
    }
}

s32 func_800AF66C(Unk8004AB64 *arg0, SVECTOR *arg1) {
    Unk800CCCC0 *c;
    WmObjSlot *s;
    Unk800C95C8 *m;
    WmAnimHdr *h;
    SVECTOR r2;
    SVECTOR r;
    s32 t;
    void *t2;
    s32 n;
    s32 i;

    c = &D_800CCCC0[arg0->unk11];
    s = (WmObjSlot *)func_8001E350(arg0->unk3B);
    m = s->unk8;
    memset(&r, 0, 8);
    r.vx = s->unkC->pos.unk20;
    r.vy = s->unkC->pos.rotY;
    r.vz = s->unkC->unk24;
    r2 = r;
    func_800316D8(&r2, &m->unk24);
    m->unk24.t[0] = arg1->vx;
    m->unk24.t[1] = arg1->vy;
    m->unk24.t[2] = arg1->vz;
    func_800C9484(m);
    m->unkC = h = db_lookupRegistered(4, s->unkC->unk36 | 0x30000, 0);
    if (h != NULL) {
        if (IS_OFFSET(h->unkC)) {
            t = h->unk10;
            h->unkC = (s32)h + h->unkC;
            if (t != 0) {
                h->unk10 = (s32)m->unkC + t;
            }
        }
        func_800C96AC(m, s->unkC->unk3A);
        if (arg0->unk11 != 0xE) {
            n = m->unk8->unk3;
            if (c->unk0 != 0 && c->unkC > 0) {
                m->unk5A = arg1->vy - c->unkC + 100;
                if (m->unk1C != NULL) {
                    m->unk5A = m->unk1C->unk5A;
                }
                for (i = 0; i < n; i++) {
                    func_800C9FCC(m, i);
                    func_800CA1D8(m, i);
                }
            } else {
                for (i = 0; i < n; i++) {
                    func_800C7914(m, i);
                    func_800C709C(m, i);
                }
            }
        }
    } else {
        printf("Illigal Animation Data\n");
    }
    return 0;
}

/* shadow under world map object arg0 at arg1; arg2 = distance from the camera */
void func_800AF8C0(Unk8004AB64 *arg0, SVECTOR *arg1, s32 arg2) {
    Unk800CCCC0 *c;
    s32 k;
    s32 d;
    s32 sx;
    s32 sz;
    s32 f;
    s32 a;
    s32 s;
    u32 *ot;

    c = &D_800CCCC0[arg0->unk11];
    k = arg0->unk3B;
    sx = 0x1000;
    if (arg0->unk11 != 0xE) {
        d = c->unkE - arg0->pos.vy;
        arg1->vy += d;
        arg1->vy -= d / 8;
        sz = sx - d / 2;
        sx -= d / 4;
        D_800CF442 = -c->unk1 * 4 - (s8)c->unk7;
        if (c->unk1 == 0) {
            return;
        }
        sx = (c->unk1 * sx) >> 11;
        sz = (c->unk2 * sz) >> 12;
        f = (0x8000 - arg2) >> 3;
        f *= D_800D45DC;
        f >>= 12;
        if (f < 0) {
            f = 0;
        }
        if (f >= 0x1000) {
            f = 0xFFF;
        }
        sz *= f;
        sz >>= 12;
        if (sz >= 0x100) {
            sz = 0xFF;
        }
        if (sz < 0) {
            sz = 0;
        }
        g_gameState->unk1C->unk8E4.world->unk8->unk1C[k].unk1 = sx * 14;
        g_gameState->unk1C->unk8E4.world->unk8->unk1C[k].unk2 = sx * 12;
        g_gameState->unk1C->unk8E4.world->unk8->unk1C[k].unk0 = sz;
        ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8];
        g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] = ot + D_800CF442;
        func_800AA418(g_gameState->unk1C->unk8E4.world->unk8->unk1C, k, arg1->vy, c->unk6);
        g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] = ot;
    } else {
        s = 0x7D0;
        if (g_worldFog != 0) {
            s = 0xAF0;
        }
        f = (0x7060 - arg2) >> 2;
        f *= D_800D4754;
        f >>= 12;
        if (f < 0) {
            f = 0;
        }
        if (f >= 0x1000) {
            f = 0xFFF;
        }
        a = (c->unk2 * s) >> 12;
        a *= f;
        a >>= 12;
        if (a >= 0x100) {
            a = 0xFF;
        }
        if (a < 0) {
            a = 0;
        }
        g_gameState->unk1C->unk8E4.world->unk8->unk1C[k].unk1 = 0x302;
        g_gameState->unk1C->unk8E4.world->unk8->unk1C[k].unk2 = 0x294;
        g_gameState->unk1C->unk8E4.world->unk8->unk1C[k].unk0 = a;
        ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8];
        D_800CF442 = -0x46;
        g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] -= 0x46;
        func_800AA418(g_gameState->unk1C->unk8E4.world->unk8->unk1C, k, arg1->vy, c->unk6);
        g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] = ot;
    }
}

void func_800AFCE0(void) {
    s32 tmp;
    s32 h;
    s32 cell;
    Unk16D8 *e;
    Unk8004AB64 *act;
    Unk800CCCC0 *cc;

    D_800CCE54 = 0;
    D_800CE3C0 = 0;
    D_800CE278 = 0;
    D_800CE3E0 = 0;
    if (func_8004AE00() != NULL) {
        D_800CE3DC = func_8004AE00();
        D_800CE3DC->unk40.vx = D_800CE3DC->pos.vx;
        D_800CE3DC->unk40.vy = D_800CE3DC->pos.vy;
        D_800CE3DC->unk40.vz = D_800CE3DC->pos.vz;
    }
    for (e = func_8004AE9C(); e != NULL; e = e->unk0) {
        act = e->unk4;
        cc = &D_800CCCC0[act->unk11];
        if (act->unk5 == 4 && act->unkE == 1 && (act->unkF & 1)) {
            switch (act->unk11) {
            case 7:
                cc->unk3 = D_800CE3C4 == 6;
                break;
            case 11:
                D_800CE270 = act;
                break;
            }
            if (act == D_800CE3DC && func_8004AE84() != 0) {
                func_800B0378(cc);
            } else {
                cc = &D_800CCCC0[act->unk11];
                func_800BE748((VECTOR *)&act->pos, &cc->unk8, &tmp, &D_800CE288[cc->unk5], (u16 *)&h);
                if (act->unk11 == 8) {
                    h = 0;
                }
                cc->unkE = h;
            }
            func_800B01F0(act);
        }
    }
    func_80064C24(0);
    switch (D_800CE3C8->unk0) {
    case 0:
    case 3:
        if (D_800D33F4 != 0) {
            func_800B2C24();
        }
        break;
    case 1:
    case 2:
        func_800B2E54();
        break;
    }
    if ((u8)D_800CE3C4 >= 1 && (u8)D_800CE3C4 <= 6) {
        D_800CCCC0[1].unkC = D_800CCCC0[D_800CE3DC->unk11].unkC;
    }
    if (D_800CE268 != NULL) {
        if (D_800CE268->unk24 < 0) {
            D_800CE268->unk24 += 10;
        }
        if (D_800CE268->unk24 > 0) {
            D_800CE268->unk24 -= 10;
        }
        if (D_800CE268->unk24 > -10 && D_800CE268->unk24 < 10) {
            D_800CE268->unk24 = 0;
        }
    }
    if (D_800CE3D0 != NULL) {
        if (D_800CE3C8->unk0 != 1) {
            if (D_800CE3D0->unk24 < 14) {
                D_800CE3D0->unk24 += 10;
            }
            if (D_800CE3D0->unk24 >= 15) {
                D_800CE3D0->unk24 -= 10;
            }
            if (D_800CE3D0->unk24 >= 5 && D_800CE3D0->unk24 < 24) {
                D_800CE3D0->unk24 = 14;
            }
        } else if (D_800CE209 == 0) {
            if (D_800CE3D0->unk24 < 14) {
                D_800CE3D0->unk24 += 16;
            }
            if (D_800CE3D0->unk24 >= 15) {
                D_800CE3D0->unk24 -= 16;
            }
            if (D_800CE3D0->unk24 >= -1 && D_800CE3D0->unk24 < 30) {
                D_800CE3D0->unk24 = 14;
            }
        }
    }
    if (func_8004AE00() != NULL) {
        if (D_800CE3E0 == 0 || D_800CE3C0 < 300) {
            if (D_800CE100 > 0) {
                D_800CE100 -= 10;
                if (D_800CE100 < 0) {
                    D_800CE100 = 0;
                }
            }
            if (D_800CE100 < 0) {
                D_800CE100 += 10;
                if (D_800CE100 > 0) {
                    D_800CE100 = 0;
                }
            }
        }
        cc = &D_800CCCC0[D_800CE3DC->unk11];
        if ((cc->unk8 & 0xC000) && D_800CCE4D != 0) {
            func_800BC130(D_800CE3DC->pos.vx, D_800CE3DC->pos.vz, &cell);
            func_8004B158(cell % D_800D32F0, cell / D_800D32F0, (cc->unk8 & 0xC000) >> 14);
        }
    }
}

void func_800B01F0(Unk8004AB64 *arg0) {
    u8 flat;
    s32 h;
    Unk800CCCC0 *c;
    s32 y;
    s16 d;

    if (D_800D33F4 != 0) {
        c = &D_800CCCC0[arg0->unk11];
        if (c->unk5 != 0xA) {
            h = func_800B36B4(c->unk0, c->unk8, &flat);
            if (c->unk3 == 0) {
                if (c->unk0 != 0) {
                    if (c->unk4 == 7) {
                        c->unkC = 0x118;
                    } else if (flat != 0 || h == 0) {
                        c->unkC = h;
                    } else {
                        if (c->unkC < h - 0x15) {
                            c->unkC += 0x14;
                        }
                        if (c->unkC > h + 0x15) {
                            c->unkC -= 0x14;
                        }
                    }
                }
                arg0->pos.vy = c->unkE + c->unkC;
            } else {
                y = c->unkE + h;
                if (y < arg0->pos.vy) {
                    arg0->pos.vy = y;
                }
                if (D_800CE202 == 0 && D_800CE3DC->pos.vy < -0x2A30 && D_800CE0EF == 0) {
                    D_800CE3DC->pos.vy = -0x2A30;
                }
                d = arg0->pos.vy - c->unkE;
                c->unkC = d;
                if (d < 0) {
                    c->unkC = 0;
                }
            }
        }
    }
}

void func_800B0378(Unk800CCCC0 *cc) {
    ActorPos p;
    ActorPos q;
    u32 cell;
    s32 h;
    u32 cell2;
    s32 h2;
    s32 saved;
    s32 spd;
    s32 i;
    s32 j;
    s32 dx, dy, dz;
    s32 ex, ez;
    s32 k;
    s32 dist;
    s32 m;
    s32 off, lim, y, zz, wz, r1, r2;

    if (D_800CE3C8->unkD != 0 && D_800CE25E != 0 && w_frameGetParameter(0xC1) != 0x34 && D_800D3E75 == -1) {
        D_800CCE54 = D_800CCE55;
    }
    if (D_800CCE4A == 0 && D_800D3E60 != 2 && D_800D4818 == 0 && D_800D33F4 != 0 && D_800CE1E0 == 0) {
        func_800B3760();
    }
    spd = D_800CE3C0;
    if (spd < 0) {
        spd /= 2;
    }
    D_800D30C6 = 1;
    if (D_800CE3C8->unk8 == 0) {
        saved = D_800CE3C4;
        if (D_800CCF6C != 0) {
            D_800CE3C4 = 10;
            spd *= 4;
        }
        i = 0;
        do {
            j = i;
            cell = -1;
            h = -1;
            p = D_800CE3DC->pos;
            if (func_800B0D6C((VECTOR *)&p, i, D_800CE3C4, spd, &cell, &h) ||
                func_800B0D6C((VECTOR *)&p, j = -i, D_800CE3C4, spd, &cell, &h)) {
                dx = p.vx - D_800CE3DC->unk40.vx;
                dy = p.vy - D_800CE3DC->unk40.vy;
                dz = p.vz - D_800CE3DC->unk40.vz;
                if (func_800B36B4(cc->unk0, cell, 0) != 0) {
                    dist = SquareRoot0(dx * dx + dz * dz);
                } else {
                    dist = SquareRoot0(dx * dx + dy * dy + dz * dz);
                }
                if (dist != 0) {
                    k = spd * spd / dist;
                    if (spd < 0) {
                        k = -k;
                    }
                } else {
                    k = 0;
                }
                q = D_800CE3DC->pos;
                if (func_800B0D6C((VECTOR *)&q, j, D_800CE3C4, k, &cell, &h)) {
                    D_800CE3DC->pos = q;
                    cc->unk8 = cell;
                    cc->unkE = q.vy;
                    D_800CE208 = h;
                    break;
                }
            }
            i += 0x80;
        } while (i < 0x400);
        if (D_800CCF6C != 0) {
            D_800CE3C4 = saved;
        }
    } else {
        q = D_800CE3DC->pos;
        if (func_800B0D6C((VECTOR *)&q, 0, D_800CE3C4, spd, &cell2, &h2)) {
            D_800CE3DC->pos.vx = q.vx;
            D_800CE3DC->pos.vz = q.vz;
            D_800CE260 = D_800CE3DC->pos.vy + 0x2A30;
            if (D_800CE260 < 0x7E8 && D_800CE202 == 0 && D_800CE0EF == 0 && D_800CE278 < 0) {
                m = (D_800CE278 << 1) * D_800CE260;
                m >>= 12;
                D_800CE3DC->pos.vy += m;
            } else {
                D_800CE3DC->pos.vy += D_800CE278;
            }
            cc->unk8 = cell2;
            cc->unkE = q.vy;
            D_800CE208 = h2;
        }
        off = 0x1C20;
        if (g_worldDataSet == 1) {
            off = 0x1964;
            lim = 0x1E78;
        } else {
            lim = 0x2134;
        }
        /* ez holds the x offset here; the names follow the original register use */
        ez = D_800CE3DC->pos.vx - 0x2FF77;
        y = D_800CE3DC->pos.vy;
        wz = D_800CE3DC->pos.vz + 0x14031;
        dy = y + off;
        if (ABS(ez) < 0x9C4 && ABS(wz) < 0x9C4) {
            dx = 0x5F5E10 - ez * ez;
            zz = wz * wz;
            dz = dx - dy * dy - zz;
            if (-lim < y) {
                D_800D30C6 = 0;
            }
            if (dz > 0) {
                r1 = SquareRoot0(dz);
                r2 = SquareRoot0(dz);
                dz = dy - r1;
                if (dz < 0) {
                    dz = -dz;
                }
                dz = dz < abs(dy + r2);
                if (dz) {
                    D_800CE3DC->unk40.vy = D_800CE3DC->pos.vy = SquareRoot0(dx - zz) - off;
                } else {
                    D_800CE3DC->unk40.vy = D_800CE3DC->pos.vy = -SquareRoot0(dx - zz) - off;
                }
            }
        }
    }
    ex = D_800CE3DC->pos.vx - 0x407F7;
    ez = D_800CE3DC->pos.vz + 0x3BDC3;
    if (ABS(ex) < 0x800 && ABS(ez) < 0x800) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->pos.vx - 0x37FA8;
    ez = D_800CE3DC->pos.vz + 0x1606D;
    if (ABS(ex) < 0xC00 && ABS(ez) < 0xC00) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->pos.vx - 0x4AD96;
    ez = D_800CE3DC->pos.vz + 0x32133;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->pos.vx - 0x489E4;
    ez = D_800CE3DC->pos.vz + 0x34BBF;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->pos.vx - 0x250D4;
    ez = D_800CE3DC->pos.vz + 0x46B95;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->pos.vx - 0x36F54;
    ez = D_800CE3DC->pos.vz + 0xF131;
    if (ABS(ex) < 0x60C && ABS(ez) < 0x800) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->pos.vx - 0x3D3D2;
    ez = D_800CE3DC->pos.vz + 0x36C0F;
    if (ABS(ex) < 0x500 && ABS(ez) < 0x500) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->pos.vx - 0x4A02F;
    ez = D_800CE3DC->pos.vz + 0x11DD8;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->pos.vx - 0xC02F;
    ez = D_800CE3DC->pos.vz + 0x1D9EC;
    if (ABS(ex) < 0x500 && ABS(ez) < 0x500) {
        D_800D30C6 = 0;
    }
    ex = D_800CE3DC->pos.vx - 0x17A3B;
    ez = D_800CE3DC->pos.vz + 0x4014A;
    if (ABS(ex) < 0x400 && ABS(ez) < 0x400) {
        D_800D30C6 = 0;
    }
    func_800BC0A8(&D_800CE3DC->pos.vx, &D_800CE3DC->pos.vz);
}

u8 func_800B0D6C(VECTOR *arg0, s32 arg1, s32 arg2, s32 arg3, u32 *arg4, s32 *arg5) {
    VECTOR v;
    u32 cell;
    s32 h;
    Unk800CE3C8 *mode;
    Unk800CE288 *c;
    u8 ok;
    s32 k;
    s32 ang;
    u8 a;
    u8 b;
    u8 x;
    u8 y;

    k = D_800CCCC0[D_800CE3DC->unk11].unk5;
    mode = &D_800CCB70[arg2];
    v = *arg0;
    c = &D_800CE288[k];
    v.vx += math_sin(D_800CE3CC + (ang = arg1 + 0x800)) * arg3 >> 12;
    v.vz += math_cos(D_800CE3CC + ang) * arg3 >> 12;
    if ((u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2 == 0x34 && D_800CE3C8->unk0 != 1) {
        func_800BE748(&v, &cell, &h, NULL, (u16 *)&v.vy);
    } else {
        func_800BE748(&v, &cell, &h, c, (u16 *)&v.vy);
    }
    if (h >= 0) {
        ok = func_800B3340(mode->unk14, cell);
        if (ok) {
            if (mode->unk1 != 0) {
                a = func_800B3340(D_800CCB20, cell);
                b = func_800B3340(D_800CCB18, cell);
                x = func_800B3340(D_800CCB20, D_800CCCC0[D_800CE3DC->unk11].unk8);
                y = func_800B3340(D_800CCB18, D_800CCCC0[D_800CE3DC->unk11].unk8);
                if (b & x) {
                    ok = 0;
                }
                if (y & a) {
                    ok = 0;
                }
            }
            if (ok) {
                if (arg0 != NULL) {
                    *arg0 = v;
                }
                if (arg5 != NULL) {
                    *arg5 = h;
                }
                if (arg4 != NULL) {
                    *arg4 = cell;
                }
                return 1;
            }
        }
    }
    return 0;
}

u8 func_800B105C(ActorPos *out) {
    Unk8004AB64 *act;
    s32 d[2];
    ActorPos v;
    u32 cell;
    u8 ok;
    u8 r;
    s32 a;
    s32 i;
    s32 j;
    s32 t;
    s32 k;
    s32 x;
    s32 z;

    memset(&v, 0, sizeof(v));
    ok = 0;
    if (D_800D32FC < 0x16 && ((u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2) == 0x17) {
        return 0;
    }
    D_800CE209 = 0;
    act = D_800CE3DC;
    D_800D30C4 = 1;
    v = act->pos;
    switch (D_800CE3C4) {
    case 8:
    case 9:
        x = (D_800CE3DC->pos.vx >> 3) - 0x17FC;
        z = (D_800CE3DC->pos.vz >> 3) + 0x3B48;
        if (SquareRoot0(x * x + z * z) < 0xB4) {
            return 0;
        }
        if (!func_800B0D6C((VECTOR *)&v, 0, 0, 0, &cell, 0)) {
            return 0;
        }
        t = (u8)cell >> 2;
        if (t >= 0x2D && (t < 0x2F || t == 0x34)) {
            return 0;
        }
        break;
    case 7:
        if (!func_800B0D6C((VECTOR *)&v, 0, 10, 0, &cell, 0)) {
            return 0;
        }
        if (((u8)cell >> 2) != 0x35) {
            return 0;
        }
        break;
    default:
        if (!func_800B0D6C((VECTOR *)&v, 0, 0, 0, &cell, 0)) {
            return 0;
        }
        if (((u8)cell >> 2) == 0x34) {
            return 0;
        }
        break;
    }
    D_800D30C4 = 0;
    for (a = 0; a < 0x800; a += 0x100) {
        d[0] = (a + 0xC00) % 0x1000;
        d[1] = -a + 0xC00;
        for (i = 0; i < 2; i++) {
            r = 1;
            if (ok) {
                continue;
            }
            for (j = 1; j < 9; j++) {
                t = D_800CE3C8->unkE * j / 8;
                v = D_800CE3DC->pos;
                r &= func_800B0D6C((VECTOR *)&v, d[i], 0, t, &cell, 0);
            }
            ok = r;
            if (ok) {
                switch (D_800CE3C4) {
                case 0:
                    if (abs(v.vy - D_800CE3DC->pos.vy) >= 0x1F5) {
                        ok = 0;
                    }
                    break;
                case 7:
                    if (func_800B3340(D_800CCB18, cell)) {
                        ok = 0;
                    }
                    break;
                case 8:
                case 9:
                    if (abs(v.vy - w_frameGetParameter(0xC2)) >= 0x15F) {
                        ok = 0;
                    }
                    break;
                }
            }
        }
        if (ok) {
            k = (u8)cell >> 2;
            switch (k) {
            case 0x24:
            case 0x25:
            case 0x26:
                k = 0;
                break;
            case 0x35:
                k = 1;
                break;
            case 0x36:
                k = 2;
                break;
            case 0x37:
                k = 3;
                break;
            case 0x38:
                k = 4;
                break;
            case 0x39:
                k = 5;
                break;
            case 0x33:
                k = 6;
                break;
            case 0x30:
                k = 7;
                break;
            default:
                k = 8;
                break;
            }
            v.vy += D_800CCB28[1][k];
            *out = v;
            return 1;
        }
    }
    return 0;
}

void w_movementChange(void) {
    s8 old;
    Unk800CE3C8 *p;
    Unk800CE3C8 *prev;

    func_8004B20C(0);
    D_800CE3BC = 0;
    D_800CE276 = 0;
    func_800B3638();
    old = D_800CE3C4;
    prev = D_800CE3C8;
    D_800CE3C4 = g_evtGlobalVars.b[0xBE];
    D_800CE3C8 = p = &D_800CCB70[D_800CE3C4];
    if (old == 7) {
        func_800C48F4(prev->unkC);
    }
    if (p->unkC != 0) {
        func_800C4718(p->unkC, 0);
        if (D_800CE3C4 != 7) {
            func_800C4850(p->unkC, 0x3C, 0x3F);
        }
        if ((u8)(D_800CE3C4 - 8) < 2) {
            func_800C4718(0x26, 0);
        }
        D_800CE0F0->unk8 = 1;
    }
    if (prev == NULL || prev->unkB != p->unkB) {
        w_musicRequestSend(p->unkB);
    }
    if (D_800CE3C4 == 0 && (u8)(old - 1) < 6) {
        D_800CE3CC = (D_800CE268->pos.rotY + 0x400) & 0xFFF;
    }
    if ((u8)(D_800CE3C4 - 1) < 6 && old == 0) {
        D_800CE3CC = D_800CE268->pos.rotY & 0xFFF;
    }
    if (D_800CCB70[old].unk8 == D_800CCB70[D_800CE3C4].unk8) {
        func_800AE784(p->unk11, 0x200);
    } else {
        func_800AE784(p->unk11, 0x40);
    }
    D_800CE1F2 = D_800CE3C8->unk8 == 0;
    func_800AFCE0();
    if (g_worldMapDebugFlags & 0x20) {
        printf("w_movementChange : %d -> %d\n", old, D_800CE3C4);
    }
}

void func_800B17B4(void) {
    s32 x;
    s32 z;
    s32 ang;
    u16 mv;
    s32 a;
    s16 cur;
    u32 k;
    s32 rx;
    s32 lo;
    s32 t;

    func_800B3398(&x);
    func_800B345C(&z);
    z = -z;
    t = -ratan2(z, x) + 0x400;
    D_800CE25D = 0;
    ang = t & 0xFFF;
    D_800CE26D = 0;
    D_800CE209 = 0;
    mv = ((x | z) != 0) << 12;
    if (func_800B8498() & 0x40) {
        D_800CE26D = 1;
    } else {
        D_800CE26D = 0;
    }
    if (func_800B8498() & 0x100000) {
        D_800CE25D = 1;
        D_800CE3BE = 1;
        D_800CE0F0->unkC -= 0x20;
    }
    if (func_800B8498() & 0x200000) {
        D_800CE25D = 1;
        D_800CE3BE = 1;
        D_800CE0F0->unkC += 0x20;
    }
    D_800CE0F0->unkC &= 0xFFF;
    if (mv != 0 || D_800CE25D != 0) {
        D_800CE0F0->unk0 = math_sin(ang);
        if (mv != 0) {
            D_800CE3BE = 0;
            if ((u32)(ang - 0x600) <= 0x400) {
                D_800CE0F0->unkE = 0x800;
            } else {
                D_800CE0F0->unkE = 0;
            }
            D_800CE3CC = (D_800CE0F0->unkC + ang) & 0xFFF;
            D_800CE3C0 = mv * D_800CE3C8->unk2 >> 12;
        }
    }
    a = D_800CE3CC;
    cur = D_800CE3DC->pos.rotY;
    if (a - cur > 0x800) {
        a -= 0x1000;
    }
    if (a - cur < -0x800) {
        a += 0x1000;
    }
    a -= cur;
    if ((x | z) != 0) {
        D_800CE3DC->pos.rotY = (D_800CE3DC->pos.rotY + a / 3) & 0xFFF;
    }
    if (D_800CE25E != 0 && mv != 0) {
        if (func_800B3340_inline(D_800CCB20, (u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2)) {
            func_800C6F00(D_800CE3DC->pos.vx, -100, D_800CE3DC->pos.vz, 22, 24000);
        }
        if (!(D_800CF4B0 & 3)) {
            k = (u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2;
            if (k == 0x1E || k == 0x22 || k == 0x23) {
                func_800C6F00(D_800CE3DC->pos.vx, D_800CE3DC->pos.vy, D_800CE3DC->pos.vz, 7, 0x900);
            }
            if (((u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2) == 0x29) {
                func_800C6F00(D_800CE3DC->pos.vx, D_800CE3DC->pos.vy, D_800CE3DC->pos.vz, 27, 0x2000);
            }
        }
        if (!(D_800CF4B0 & 1) && (u32)(((u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2) - 0x24) < 3) {
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            rx = D_800CE3DC->unk40.vx + *g_randPtr++ - 0x80;
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            lo = *g_randPtr++;
            func_800C6F00(rx, D_800CE3DC->unk40.vy, D_800CE3DC->unk40.vz + lo - 0x80, 16, 0x6D80);
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            rx = D_800CE3DC->unk40.vx + *g_randPtr++ - 0x80;
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            lo = *g_randPtr++;
            func_800C6F00(rx, D_800CE3DC->unk40.vy, D_800CE3DC->unk40.vz + lo - 0x80, 16, 0x6D80);
        }
    }
}

void func_800B1CD0(void) {
    s32 lx;
    s32 rx;
    s32 ly;
    s16 v;
    s32 sp;
    s32 vol;
    s32 a;
    s32 ang;
    Unk800CE3C8 *m;
    s32 dx;
    s32 dz;

    func_800B3524(&lx);
    func_800B3398(&rx);
    if ((u8)func_800B345C(&ly)) {
        ly = 0;
    }
    ly = -ly;
    if (lx > 0 && ly < lx) {
        ly = lx;
    }
    if (lx < 0 && lx < ly) {
        ly = lx;
    }
    a = D_800CE3C8->unk4 * rx >> 7;
    D_800CE3BE = 0;
    D_800CE3E0 = a;
    if (func_800B8498() & 0x100000) {
        D_800CE3BE = 1;
        D_800CE0F0->unkC -= 0x20;
    }
    if (func_800B8498() & 0x200000) {
        D_800CE3BE = 1;
        D_800CE0F0->unkC += 0x20;
    }
    D_800CE0F0->unkE = 0;
    D_800CE0F0->unkC &= 0xFFF;
    m = D_800CE3C8;
    v = D_800CE3BC + (m->unk2 * ly / D_800CCE20 - D_800CE3BC) / D_800CCE24;
    D_800CE3BC = v;
    sp = v / D_800CCE28;
    D_800CE3C0 = sp;
    vol = ABS(sp) / 8 + 0x14;
    if (vol >= 0x80) {
        vol = 0x7F;
    }
    func_800C4850(m->unkC, 0x3C, vol);
    D_800CE3CC = D_800CE3DC->pos.rotY = (D_800CE3DC->pos.rotY + D_800CE3E0) & 0xFFF;
    if (ABS(D_800CE3C0) > 80 && D_800CE25E != 0) {
        ang = (D_800CE3C0 <= 0) << 11;
        dx = math_sin(ang + D_800CE3DC->pos.rotY) >> 2;
        dz = math_cos(ang + D_800CE3DC->pos.rotY) >> 2;
        func_800C6F00(D_800CE3DC->pos.vx + dx, -100, D_800CE3DC->pos.vz + dz, 15, 15000);
        if (D_800CF4B0 & 1) {
            dx = math_sin(ang + D_800CE3DC->pos.rotY + 0x80) >> 3;
            dz = math_cos(ang + D_800CE3DC->pos.rotY + 0x80) >> 3;
            func_800C6F00(D_800CE3DC->pos.vx + dx, -100, D_800CE3DC->pos.vz + dz, 10, 15000);
            dx = math_sin(ang + D_800CE3DC->pos.rotY - 0x80) >> 3;
            dz = math_cos(ang + D_800CE3DC->pos.rotY - 0x80) >> 3;
            func_800C6F00(D_800CE3DC->pos.vx + dx, -100, D_800CE3DC->pos.vz + dz, 10, 15000);
        }
    }
}

void func_800B20B8(void) {
    s32 x;
    s32 y;
    s32 z;
    s32 ox;
    s32 btn;
    s32 dx;
    s32 dz;
    s32 d;
    s32 k;
    s32 r;
    s32 h;
    u32 t;
    u8 f;
    s32 q;
    u32 w;
    s32 a;
    s32 b;
    s16 sa;
    s16 sb;
    s32 ex;
    s32 ez;

    func_800B3398(&x);
    func_800B345C(&y);
    func_800B3524(&z);
    if (x != 0) {
        D_800CE209 = 1;
    } else {
        D_800CE209 = 0;
    }
    if (z < 0) {
        D_800CE26D = 1;
    } else {
        D_800CE26D = 0;
    }
    D_800CE3BE = 0;
    y = -y;
    ox = x;
    btn = func_800B8498();
    if ((btn & 0x300000) && D_800CE0E8->unk11 == 0 && D_800CE25C != 0) {
        func_800AE828();
    }
    if (D_800CE0E8->unk11 == 0) {
        if (btn & 0x100000) {
            x = -0x7F;
        }
        if (btn & 0x200000) {
            x = 0x7F;
        }
    } else {
        if (btn & 0x200000) {
            D_800CE3BE = 1;
            D_800CE0F0->unkC += 0x10;
        }
        if (btn & 0x100000) {
            D_800CE3BE = 1;
            D_800CE0F0->unkC -= 0x10;
        }
    }
    D_800CE0F0->unkE = 0;
    a = D_800CE3C8->unk2 * z / D_800CCE20;
    D_800CE3E0 = D_800CE3C8->unk4 * x >> 7;
    q = D_800CCE24;
    b = D_800CE3C8->unk5 * y / D_800CCE20;
    a = (a - D_800CE3BC) / q;
    b = (b - D_800CE276) / q;
    sa = D_800CE3BC + a;
    sb = D_800CE276 + b;
    D_800CE3BC = sa;
    D_800CE276 = sb;
    D_800CE3C0 = sa / D_800CCE28;
    D_800CE278 = sb / D_800CCE28;
    if (D_800CE274 != 0) {
        D_800CE3E0 *= 2;
        D_800CE3C0 *= 2;
    }
    if (D_800CE3C4 == 9) {
        D_800CE3E0 = D_800CE3E0 * (600 - D_800CE3C0 / 2) >> 8;
    }
    if (D_800CE3C4 == 8) {
        D_800CE3E0 = D_800CE3E0 * (580 - D_800CE3C0 / 2) >> 8;
    }
    if (x != 0 || z < 0) {
        func_800B3638();
    }
    switch (D_800CE25C) {
    case 1:
        ex = (D_800CD10C[D_800D3E74][D_800CF480].unk4 - D_800CE3DC->pos.vx) >> 5;
        ez = (D_800CD10C[D_800D3E74][D_800CF480].unk8 - D_800CE3DC->pos.vz) >> 5;
        r = SquareRoot0(ex * ex + ez * ez);
        D_800CE3C0 = r;
        if (D_800CE3C0 > 400) {
            D_800CE3C0 = 400;
        }
        if (D_800CE3E4 < D_800CE3C0 || D_800CE3C0 < 50) {
            func_800B3638();
        }
        D_800CE3E4 = D_800CE3C0;
    case 2:
        dx = D_800CE3DC->pos.vx - D_800CD10C[D_800D3E74][D_800CF480].unk4;
        dz = D_800CE3DC->pos.vz - D_800CD10C[D_800D3E74][D_800CF480].unk8;
        if (dx > 0x30000) {
            dx -= 0x60000;
        }
        if (dx < -0x30000) {
            dx += 0x60000;
        }
        if (dz > 0x28000) {
            dz -= 0x50000;
        }
        if (dz < -0x28000) {
            dz += 0x50000;
        }
        dz = (-ratan2(dz, dx) + 0x400) & 0xFFF;
        dz -= D_800CE3DC->pos.rotY;
        if (dz < -0x800) {
            dz += 0x1000;
        }
        if (dz > 0x800) {
            dz -= 0x1000;
        }
        if (D_800CE3E4 > 50) {
            if (dz > 20) {
                D_800CE3E0 = 20;
            } else if (dz < -20) {
                D_800CE3E0 = -20;
            } else if (D_800CE25C == 2) {
                D_800CE25C = 1;
            }
        }
        break;
    }
    D_800CE3CC = D_800CE3DC->pos.rotY = (D_800CE3DC->pos.rotY + D_800CE3E0) & 0xFFF;
    if (D_800CE3E0 > 0 && -ox < D_800CE100) {
        D_800CE100 -= ox / 8;
    }
    if (D_800CE3E0 < 0 && D_800CE100 < -ox) {
        D_800CE100 -= ox / 8;
    }
    if (D_800CE3C0 == 0) {
        D_800CE100 += ox / 8;
    }
    if ((s8)D_800CE3C8->unk6 < D_800CE100) {
        D_800CE100 = (s8)D_800CE3C8->unk6;
    }
    if (D_800CE100 < -(s8)D_800CE3C8->unk6) {
        D_800CE100 = -(s8)D_800CE3C8->unk6;
    }
    if (D_800CE3E0 > 0 && -(ox * 2) < D_800CE3DC->unk24) {
        D_800CE3DC->unk24 -= ox / 4;
    }
    if (D_800CE3E0 < 0 && D_800CE3DC->unk24 < -(ox * 2)) {
        D_800CE3DC->unk24 -= ox / 4;
    }
    if (D_800CE3DC->unk24 > (s8)D_800CE3C8->unk6 * 2) {
        D_800CE3DC->unk24 = (s8)D_800CE3C8->unk6 * 2;
    }
    if (D_800CE3DC->unk24 < -(s8)D_800CE3C8->unk6 * 2) {
        D_800CE3DC->unk24 = -(s8)D_800CE3C8->unk6 * 2;
    }
    if (D_800CF4B0 & 1) {
        t = (u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2;
        f = func_800B3340_inline2(D_800CCB20, t);
        switch (D_800CE3DC->unk11) {
        case 9:
        case 10:
            if (f && abs(D_800CE3C0) > 60) {
                h = D_800CE3DC->pos.vy;
                if (h >= -0x800) {
                    D_800D53E0 = 0x1000 - abs(h + 400) * 2;
                    func_800C6F00(D_800CE3DC->pos.vx, -200, D_800CE3DC->pos.vz, 18, 32000);
                    h = abs(D_800CE3C0);
                    if (h >= 0x200) {
                        h = 0x1FF;
                    }
                    dx = abs(D_800CE3DC->pos.vy) >> 6;
                    if (dx > 48) {
                        dx = 48;
                    }
                    dx = 48 - dx;
                    dx *= h;
                    func_800C4850(0x26, 20, (dx >> 9) & 0xFF);
                } else {
                    func_800C4850(0x26, 30, 0);
                }
            } else {
                func_800C4850(0x26, 30, 0);
            }
            if (t == 0x29 && D_800CE3C0 != 0 && D_800CE3DC->pos.vy >= -0x708) {
                D_800D53E0 = 0x1000 - (D_800CE3DC->pos.vy - 0x514) * 2;
                func_800C6F00(D_800CE3DC->pos.vx, D_800CE3DC->pos.vy + 100, D_800CE3DC->pos.vz, 19, 32000);
            }
            break;
        case 7:
            if (f && abs(D_800CE3C0) > 30 && D_800CE3DC->pos.vy >= -100) {
                D_800D53E0 = 0x1000 - abs(D_800CE3DC->pos.vy + 100) * 2;
                func_800C6F00(D_800CE3DC->pos.vx, -200, D_800CE3DC->pos.vz, 6, 19200);
            }
            if (t == 0x29 && D_800CE3C0 != 0 && D_800CE3DC->pos.vy >= -0x708) {
                D_800D53E0 = 0x1000 - abs(D_800CE3DC->pos.vy + 0x708) * 2;
                func_800C6F00(D_800CE3DC->pos.vx, D_800CE3DC->pos.vy + 100, D_800CE3DC->pos.vz, 14, 25600);
            }
            break;
        }
    }
}

void func_800B2C24(void) {
    s32 a;
    s32 cam;
    s32 d;
    s32 t;
    s32 q;

    a = (D_800CE0F0->unkC + D_800CE0F0->unkE) & 0xFFF;
    cam = D_800CE3CC & 0xFFF;
    if (a - cam >= 0x800) {
        a -= 0x1000;
    }
    if (a - cam < -0x800) {
        a += 0x1000;
    }
    d = (D_800CE3CC - a) * 0xC00 / 4096;
    D_800CCE3C = abs(D_800CE0F0->unk0) >> 3;
    if (abs(d) < D_800CCE3C) {
        d = 0;
    }
    if (D_800CE3C0 != 0) {
        D_800CE3D4 = 0x1000;
    } else {
        D_800CE3D4 -= 0x200;
        if (D_800CE3D4 < 0) {
            D_800CE3D4 = 0;
        }
    }
    if (D_800CE3BE == 0 && D_800CE0E8->unk11 == 0 && D_800CF46C == 2) {
        D_800CCE30 = abs(d);
        if (abs(d) < D_800CCE34) {
            D_800CCE30 = D_800CCE34;
        }
        if (D_800CCE30 > D_800CCE38) {
            D_800CCE30 = D_800CCE38;
        }
        t = (((0x800 - abs(d)) * D_800CCE2C >> 12) + 0x800) * (2 * D_800CE3D4);
        q = (d * 256 / D_800CCE30) * (t / 4096);
        D_800CE0F0->unkC += q / 4096;
    }
    D_800CE0F0->unkC &= 0xFFF;
}

void func_800B2E54(void) {
    s32 a;
    s32 step;
    s32 q;
    s32 cur;

    a = D_800CE0F0->unkC;
    if (a - D_800CE3DC->pos.rotY > 0x800) {
        D_800CE0F0->unkC = a - 0x1000;
    }
    if (D_800CE0F0->unkC - D_800CE3DC->pos.rotY < -0x800) {
        D_800CE0F0->unkC += 0x1000;
    }
    if (a < 0) {
        a += 0x1000;
    }
    a %= 4096;
    cur = D_800CE3DC->pos.rotY;
    if (a - cur > 0x800) {
        a -= 0x1000;
    }
    if (a - cur < -0x800) {
        a += 0x1000;
    }
    a = cur - a;
    a %= 4096;
    if (D_800CE3BE == 0 && D_800CE0E8->unk11 == 0) {
        step = 16;
        if (D_800CE27C != 0) {
            step = ABS(a) * 256 >> 12;
            if (step == 0) {
                step = 1;
            }
        }
        q = a % 4096 / step;
        if (q != 0) {
            D_800CE0F0->unkC += q;
        } else if (step >= 2) {
            do {
                step /= 2;
                if (step == 0) {
                    break;
                }
                q = a % 4096 / step;
            } while (q == 0);
            D_800CE0F0->unkC += q;
        } else {
            D_800CE0F0->unkC = D_800CE3DC->pos.rotY;
        }
        D_800CE0F0->unkC &= 0xFFF;
        if (ABS(a) < 100) {
            D_800CE27C = 0;
        }
    }
}

void func_800B30C0(void) {
    Unk16D8 *n;
    Unk8004AB64 *o;
    Unk800CCCC0 *c;
    ActorPos pos;
    s32 tmp;
    u16 h;

    n = func_8004AE9C();
    if (n != NULL) {
        do {
            o = n->unk4;
            if (o->unk5 == 4) {
                pos = o->pos;
                c = &D_800CCCC0[o->unk11];
                func_800BE748((VECTOR *)&pos, &c->unk8, &tmp, NULL, &h);
                c->unkE = h;
                c->unkC = func_800B36B4(c->unk0, c->unk8, 0);
                func_800B01F0(o);
                o->unk40 = o->pos;
            }
            n = n->unk0;
        } while (n != NULL);
    }
}

void w_movementChangeSP1(void) {
    s8 old;
    Unk800CE3C8 *p;

    func_800B3638();
    D_800CE3BC = 0;
    D_800CE276 = 0;
    old = D_800CE3C4;
    p = &D_800CCB70[0];
    func_800C4850(D_800CE3C8->unkC, 0x7F, 0);
    func_800C4850(0x26, 0x7F, 0);
    w_musicRequestSend(p->unkB);
    func_800AE784(p->unk11, 0x40);
    if (g_worldMapDebugFlags & 0x20) {
        printf("w_movementChange SP1 : %d -> %d\n", old, 0);
    }
}

/* padded: the next jump table is 8-aligned */
const char D_800A7164[0x1C] = "w_movementChange SP2\n";

void w_movementChangeSP2(void) {
    func_8004B20C(0);
    func_800C48F4(0x26);
    func_800C48F4(D_800CE3C8->unkC);
    D_800CE3C4 = g_evtGlobalVars.b[0xBE];
    D_800CE3C8 = &D_800CCB70[D_800CE3C4];
    if (func_8004AE00() != NULL) {
        D_800CE3DC = func_8004AE00();
    }
    D_800CE1F2 = D_800CE3C8->unk8 == 0;
    func_800AFCE0();
    if (g_worldMapDebugFlags & 0x20) {
        printf(D_800A7164);
    }
}

u8 func_800B3340(u32 *a, u32 c) {
    c = (c & 0xFC) >> 2;
    if (c >= 32) {
        if ((a[0] >> (c - 32)) & 1) {
            return 1;
        }
    } else if ((a[1] >> c) & 1) {
        return 1;
    }
    return 0;
}

s32 func_800B3398(s32 *arg0) {
    s32 v;
    s32 b;

    *arg0 = 0;
    if (((D_80073940[0].unk0 >> 12) & 0xF) == 7) {
        v = D_8007393A - 0x80;
        if (D_8007393A - 0x40U <= 0x80) {
            v = 0;
        }
        if (v > 0x40) {
            v = (v - 0x40) * 2;
        }
        if (v < -0x40) {
            v = (v + 0x40) * 2;
        }
        if (v != 0) {
            *arg0 = v;
            return 1;
        }
    }
    b = func_800B8498();
    if (b & 0x80) {
        *arg0 = -0x80;
    }
    if (b & 0x20) {
        *arg0 = 0x80;
    }
    return 0;
}

s32 func_800B345C(s32 *arg0) {
    s32 v;

    *arg0 = 0;
    if (((D_80073940[0].unk0 >> 12) & 0xF) == 7) {
        v = D_8007393B - 0x80;
        if (D_8007393B - 0x40U <= 0x80) {
            v = 0;
        }
        if (v > 0x40) {
            v = (v - 0x40) * 2;
        }
        if (v < -0x40) {
            v = (v + 0x40) * 2;
        }
        if (v != 0) {
            *arg0 = v;
            return 1;
        }
    }
    if (func_800B8498() & 0x10) {
        *arg0 = -0x80;
    }
    if (func_800B8498() & 0x40) {
        *arg0 = 0x80;
    }
    return 0;
}

s32 func_800B3524(s32 *arg0) {
    s32 v;

    *arg0 = 0;
    if (((D_80073940[0].unk0 >> 12) & 0xF) == 7) {
        v = 0x80 - D_80073939;
        if (v + 0x40U <= 0x80) {
            v = 0;
        }
        if (v > 0x40) {
            v = (v - 0x40) * 2;
        }
        if (v < -0x40) {
            v = (v + 0x40) * 2;
        }
        if (v != 0) {
            *arg0 = v;
            return 1;
        }
    }
    if (func_800B8498() & 0x80000) {
        *arg0 = -0x80;
    }
    if (func_800B8498() & 0x20000) {
        *arg0 = 0x80;
    }
    return 0;
}

void func_800B35F4(void) {
    D_800CE25C = 2;
    D_800CE3E4 = 10000;
    D_800CF480 = g_evtGlobalVars.b[0x67] - 1;
    D_800CE26C = D_800CE0E8->unk11;
}

void func_800B3638(void) {
    if (D_800CE25C != 0) {
        D_800CE3E4 = 0;
        D_800CE25C = 0;
        if (D_800CE26C != D_800CE0E8->unk11) {
            func_800AE828();
        }
        D_800CE0E8->unk11 = D_800CE26C;
    }
}

void func_800B36AC(void) {
}

s32 func_800B36B4(s32 arg0, u32 arg1, u8 *arg2) {
    s32 i;
    switch ((s32)(arg1 & 0xFC) >> 2) {
    case 0x24:
    case 0x25:
    case 0x26:
        i = 0;
        break;
    case 0x35:
        i = 1;
        break;
    case 0x36:
        i = 2;
        break;
    case 0x37:
        i = 3;
        break;
    case 0x38:
        i = 4;
        break;
    case 0x39:
        i = 5;
        break;
    case 0x33:
        i = 6;
        break;
    case 0x30:
        i = 7;
        break;
    default:
        i = 8;
        break;
    }
    if (arg2 != NULL) {
        *arg2 = i < 1;
    }
    return D_800CCB28[arg0][i];
}

void func_800B3760(void) {
    switch (D_800CE3C8->unk0) {
    case 0:
        func_800B17B4();
        break;
    case 2:
        func_800B1CD0();
        break;
    case 1:
        func_800B20B8();
        break;
    }
    if (D_800CE3C8->unk9 != 0 && (func_800B84C0() & 0x800000)) {
        func_800AE670();
    }
    if (func_800B84C0() & 0x400000) {
        func_800AE828();
    }
}

void func_800B383C(void) {
}
