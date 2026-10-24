#include "ovl_0b.h"

void btlbg_init(Unk800CEE74 *arg0, BtlUnit9C50 *arg1) {
    s32 unused[4];
    s32 i;
    D_800FE514 = arg0;
    D_800FE504 = (BtlBgScroll *)arg0->unk10;
    D_800FE50C = arg0->unk0;
    btlbg_setOtShift(0);
    D_800FE4F8 = 0;
    if (D_800FE514->unk6 != 0) {
        D_800FE4F8 = 1;
    }
    D_800FE510 = D_800FE514->unk4;
    D_800FE4F4 = arg1;
    D_800FE508 = D_800FE514->unk2;
    if (D_800FE508 != 0) {
        btl_relocTexAnims((TexAnimSet *)arg1, NULL);
        for (i = 0; i < D_800FE508; i++) {
            BtlUnit9C50 *a;
            BtlUnit9C50 *e;
            a = D_800FE4F4 + i;
            a->unk4 |= 1;
            e = D_800FE4F4 + i;
            e->unkC = 0;
            e->unk12 = 0x1000;
        }
    }
    D_800FE518 = D_800FE514->unkA;
    func_800CEEC8(0xF);
    D_800FE4F0 = 0;
    g_btlbgRainCount = g_gameState->unk1C->unk570;
    if (g_btlbgRainCount >= 0x1F) {
        g_btlbgRainCount = 0x1F;
    }
}

u8 *btlbg_draw(MATRIX *arg0, s32 arg1, s32 arg2, u32 *arg3, s32 arg4) {
    MATRIX m;
    SVECTOR rot;
    VECTOR v;
    VECTOR t;
    MATRIX m2;
    SVECTOR rot2;
    SVECTOR sv;
    s32 done;
    u8 *out;
    BtlBgGroup *grp;
    BtlBgRec *rec;
    s32 a;
    s32 d;
    s32 i;
    s32 k;
    s32 flag;

    out = (u8 *)arg2;
    memset(&rot, 0, 8);
    memset(&rot2, 0, 8);
    memset(&sv, 0, 8);
    done = 0;
    D_800FE520++;
    if (!(u8)btlbg_getBrightness()) {
        return out;
    }
    t.vx = arg0->t[0];
    t.vy = arg0->t[1];
    t.vz = arg0->t[2];
    d = arg0->m[2][1];
    if (d < 0) {
        d = -d;
    }
    t.vz = ((0x1000 - d) * 12500 + arg0->t[2] * d) / 0x1000;
    t.vx = 0;
    TransMatrix(&m2, &t);
    if (D_800F7270 == 0) {
        for (i = 0; i < D_800FE518; i++) {
            a = arg1 + D_800FE504[i].unk0;
            ((u8 *)a)[0] += D_800FE504[i].unk3;
            ((u8 *)a)[1] += D_800FE504[i].unk4;
            D_800FE504[i].unk5++;
            if (D_800FE504[i].unk5 >= D_800FE504[i].unk6) {
                switch (D_800FE504[i].unk2) {
                case 0:
                    ((u8 *)a)[0] -= D_800FE504[i].unk6 * D_800FE504[i].unk3;
                    ((u8 *)a)[1] -= D_800FE504[i].unk6 * D_800FE504[i].unk4;
                    break;
                case 1:
                    D_800FE504[i].unk3 = -D_800FE504[i].unk3;
                    D_800FE504[i].unk4 = -D_800FE504[i].unk4;
                    break;
                }
                D_800FE504[i].unk5 = 0;
            }
        }
    }
    if (D_800FE508 != 0) {
        btl_updateTexAnims((TexAnimSet *)D_800FE4F4);
    }
    SetRotMatrix(arg0);
    SetTransMatrix(arg0);
    if (g_btlbgRainCount != 0) {
        out = btlbg_drawRain(out, arg3, arg0);
    }
    grp = (BtlBgGroup *)arg1;
    k = 0;
    for (;;) {
        rec = (BtlBgRec *)((u8 *)grp + grp->unk6);
        for (i = 0; i < grp->unk4; rec++, i++) {
            switch (rec->unk0) {
            case 8:
                if (!done) {
                    D_800FE4F0 += D_800FE510;
                    rot.vy = D_800FE4F0 >> 3;
                    func_80030AF4(&rot, &m);
                    done = 1;
                    MulMatrix2(arg0, &m);
                    SetRotMatrix(&m);
                }
                if (D_800FE51C & 8) {
                    if (D_800FE4F8 != 0) {
                        SetTransMatrix(&m2);
                        SetGeomScreen(0x200);
                        out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                        SetGeomScreen(g_gameState->unk1C->unkA);
                    } else {
                        out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                    }
                }
                break;
            case 4:
                if (D_800FE51C & 4) {
                    out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                }
                break;
            case 2:
                if (D_800FE51C & 2) {
                    out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                }
                break;
            case 0:
                if (D_800FE51C & 1) {
                    out = btlbg_drawRecord(rec, out, arg3, grp);
                }
                break;
            }
        }
        if (k == D_800FE514->unk8) {
            return out;
        }
        k++;
        grp = (BtlBgGroup *)((u8 *)grp + grp->unk0);
        flag = 0;
        switch (D_800FE514->unk0) {
        case 0x44:
            a = math_sin((VSync(-1) * 26) & 0xFFE);
            sv.vx = 0x429;
            sv.vy = -0x541;
            sv.vz = 0xEA6;
            a /= 5;
            rot2.vz = a;
            break;
        case 0x70:
            switch (k) {
            case 1:
                a = (VSync(-1) * 5) & 0xFFF;
                rot2.vz = 0xFFF - a;
                a = (VSync(-1) * 3) & 0xFFF;
                rot2.vy = 0xFFF - a;
                a = math_sin(((VSync(-1) + 8) * 22) & 0xFFE);
                sv.vx = -0x834;
                sv.vz = -0x352;
                sv.vy = a / 45 - 250;
                break;
            case 2:
                rot2.vy = 0;
                rot2.vz = 0;
                a = math_sin((VSync(-1) * 20) & 0xFFC);
                sv.vx = 0x6BD;
                sv.vy = a / 64 - 0x5DC;
                sv.vz = -0x4B;
                break;
            case 3:
                a = (VSync(-1) * 4) & 0xFFF;
                rot2.vz = a;
                a = (VSync(-1) * 3) & 0xFFF;
                rot2.vy = a;
                a = math_sin(((VSync(-1) + 16) * 21) & 0xFFF);
                sv.vx = 0x6D6;
                sv.vz = 0x401;
                sv.vy = a / 50 - 0x307;
                break;
            }
            break;
        case 0x6E:
            a = math_sin((VSync(-1) * 12) & 0xFFC);
            a /= 64;
            rot2.vy = 0x200;
            sv.vx = 0x5DC;
            sv.vy = -0x1B58;
            rot2.vz = a;
            sv.vz = 0xEA6;
            break;
        case 0xA8:
            rot2.vx = 0;
            rot2.vz = 0;
            if (k == 1) {
                rot2.vy = (VSync(-1) / 16) & 0xFFF;
            } else {
                rot2.vy = (VSync(-1) / 8) & 0xFFF;
            }
            break;
        case 0xAB:
            if (k == 1) {
                a = -(VSync(-1) * 12) & 0xFFF;
                sv.vy = -0x947;
                rot2.vz = a;
                sv.vx = 0;
                sv.vz = 0xEA6;
            } else {
                a = -(VSync(-1) * 22) & 0xFFF;
                flag = 1;
                rot2.vz = 0xE00;
                sv.vy = -0x8CA;
                rot2.vy = a;
                sv.vx = 0;
                sv.vz = 0x1DC9;
            }
            break;
        case 7:
            if (k == 1) {
                if (!((D_800FE520 + 0x1F) & 0x3F)) {
                    D_800FE4FC = rand() & 0x1FF;
                }
                rot2.vy = D_800FE4FC + (rand() & 0x3F);
            } else {
                if (!(D_800FE520 & 0x3F)) {
                    D_800FE500 = rand() & 0x3FF;
                }
                rot2.vy = D_800FE500 + (rand() & 0x7F);
            }
            break;
        default:
            sv.vx = 0;
            sv.vy = -10;
            sv.vz = 0;
            rot2.vy += 3;
            break;
        }
        ApplyMatrix(arg0, &sv, &v);
        m.t[0] = arg0->t[0] + v.vx;
        m.t[1] = arg0->t[1] + v.vy;
        m.t[2] = arg0->t[2] + v.vz;
        if (flag == 0) {
            func_80030AF4(&rot2, &m);
        } else {
            func_80031244(&rot2, &m);
        }
        MulMatrix2(arg0, &m);
        SetRotMatrix(&m);
        SetTransMatrix(&m);
    }
}

void btlbg_startRain(void) {
    g_btlbgRainCount = 0x1F;
}

void btlbg_stopRain(void) {
    g_btlbgRainCount = 0;
}

u8 *btlbg_drawRain(u8 *arg0, u32 *arg1, MATRIX *arg2) {
    SVECTOR v;
    MATRIX m;
    long p;
    long flag;
    s32 i;

    SetGeomScreen(0x200);
    i = 0;
    PushMatrix();
    SetDrawStp((DR_STP *)arg0, 0);
    AddPrim(&arg1[2], arg0);
    arg0 += sizeof(DR_STP);
    m = *arg2;
    m.t[0] = 0;
    m.t[1] = 0;
    m.t[2] = 1000;
    SetTransMatrix(&m);
    for (; i < g_btlbgRainCount; i++) {
        SetLineG2((LINE_G2 *)arg0);
        SetSemiTrans(arg0, 1);
        v.vx = (((rand() & 0x1FF) - 0x100) * 41) / 32;
        v.vy = ((rand() & 0xFF) * 50) / 32 - 0xDC;
        v.vz = (((rand() & 0x1FF) - 0x100) * 41) / 32;
        RotTransPers(&v, (long *)&((LINE_G2 *)arg0)->x0, &p, &flag);
        v.vy += 0x55 + (rand() & 0x1F);
        RotTransPers(&v, (long *)&((LINE_G2 *)arg0)->x1, &p, &flag);
        setRGB0((LINE_G2 *)arg0, 0x19, 0x19, 0x32);
        setRGB1((LINE_G2 *)arg0, 0x50, 0x50, 0x82);
        AddPrim(&arg1[2], arg0);
        arg0 += sizeof(LINE_G2);
    }
    SetDrawStp((DR_STP *)arg0, 1);
    AddPrim(&arg1[2], arg0);
    arg0 += sizeof(DR_STP);
    SetDrawTPage(arg0, 0, 0, GetTPage(0, 3, 0, 0));
    AddPrim(&arg1[2], arg0);
    PopMatrix();
    arg0 += sizeof(DR_TPAGE);
    SetGeomScreen(g_gameState->unk1C->unkA);
    return arg0;
}

Unk800CEE74 *btlbg_getHeader(void) {
    return D_800FE514;
}

void func_800CEE84(s32 arg0) {
    D_800F7270 = arg0;
}

s32 func_800CEE90(void) {
    return D_800FE50C;
}

void func_800CEEA0(s32 arg0) {
    D_800FE510 += arg0;
}

s32 func_800CEEB8(void) {
    return D_800FE510;
}

void func_800CEEC8(s32 arg0) {
    D_800FE51C = arg0;
}
