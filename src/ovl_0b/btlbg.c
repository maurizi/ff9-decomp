#include "ovl_0b.h"

void btlbg_init(BtlBgHeader *arg0, BtlUnit9C50 *arg1) {
    s32 unused[4];
    s32 i;
    g_btlbgHeader = arg0;
    g_btlbgScrolls = (BtlBgScroll *)arg0->unk10;
    g_btlbgId = arg0->id;
    btlbg_setOtShift(0);
    g_btlbgSkyOwnProj = 0;
    if (g_btlbgHeader->skyOwnProj != 0) {
        g_btlbgSkyOwnProj = 1;
    }
    g_btlbgSkySpeed = g_btlbgHeader->skySpeed;
    g_btlbgTexAnims = arg1;
    g_btlbgTexAnimCount = g_btlbgHeader->texAnimCount;
    if (g_btlbgTexAnimCount != 0) {
        btl_relocTexAnims((TexAnimSet *)arg1, NULL);
        for (i = 0; i < g_btlbgTexAnimCount; i++) {
            BtlUnit9C50 *a;
            BtlUnit9C50 *e;
            a = g_btlbgTexAnims + i;
            a->unk4 |= 1;
            e = g_btlbgTexAnims + i;
            e->unkC = 0;
            e->unk12 = 0x1000;
        }
    }
    g_btlbgScrollCount = g_btlbgHeader->scrollCount;
    btlbg_setLayerMask(0xF);
    g_btlbgSkyAngle = 0;
    g_btlbgRainCount = g_gameState->gameData->rainCount;
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
    g_btlbgFrame++;
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
    if (g_btlbgScrollPaused == 0) {
        for (i = 0; i < g_btlbgScrollCount; i++) {
            a = arg1 + g_btlbgScrolls[i].unk0;
            ((u8 *)a)[0] += g_btlbgScrolls[i].unk3;
            ((u8 *)a)[1] += g_btlbgScrolls[i].unk4;
            g_btlbgScrolls[i].unk5++;
            if (g_btlbgScrolls[i].unk5 >= g_btlbgScrolls[i].unk6) {
                switch (g_btlbgScrolls[i].unk2) {
                case 0:
                    ((u8 *)a)[0] -= g_btlbgScrolls[i].unk6 * g_btlbgScrolls[i].unk3;
                    ((u8 *)a)[1] -= g_btlbgScrolls[i].unk6 * g_btlbgScrolls[i].unk4;
                    break;
                case 1:
                    g_btlbgScrolls[i].unk3 = -g_btlbgScrolls[i].unk3;
                    g_btlbgScrolls[i].unk4 = -g_btlbgScrolls[i].unk4;
                    break;
                }
                g_btlbgScrolls[i].unk5 = 0;
            }
        }
    }
    if (g_btlbgTexAnimCount != 0) {
        btl_updateTexAnims((TexAnimSet *)g_btlbgTexAnims);
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
                    g_btlbgSkyAngle += g_btlbgSkySpeed;
                    rot.vy = g_btlbgSkyAngle >> 3;
                    math_rotMatrixXYZ(&rot, &m);
                    done = 1;
                    MulMatrix2(arg0, &m);
                    SetRotMatrix(&m);
                }
                if (g_btlbgLayerMask & 8) {
                    if (g_btlbgSkyOwnProj != 0) {
                        SetTransMatrix(&m2);
                        SetGeomScreen(0x200);
                        out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                        SetGeomScreen(g_gameState->gameData->unkA);
                    } else {
                        out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                    }
                }
                break;
            case 4:
                if (g_btlbgLayerMask & 4) {
                    out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                }
                break;
            case 2:
                if (g_btlbgLayerMask & 2) {
                    out = btlbg_drawRecord(rec, out, (u32 *)arg4, grp);
                }
                break;
            case 0:
                if (g_btlbgLayerMask & 1) {
                    out = btlbg_drawRecord(rec, out, arg3, grp);
                }
                break;
            }
        }
        if (k == g_btlbgHeader->lastGroup) {
            return out;
        }
        k++;
        grp = (BtlBgGroup *)((u8 *)grp + grp->unk0);
        flag = 0;
        switch (g_btlbgHeader->id) {
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
                if (!((g_btlbgFrame + 0x1F) & 0x3F)) {
                    D_800FE4FC = rand() & 0x1FF;
                }
                rot2.vy = D_800FE4FC + (rand() & 0x3F);
            } else {
                if (!(g_btlbgFrame & 0x3F)) {
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
            math_rotMatrixXYZ(&rot2, &m);
        } else {
            math_rotMatrixZYX(&rot2, &m);
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
    SetGeomScreen(g_gameState->gameData->unkA);
    return arg0;
}

BtlBgHeader *btlbg_getHeader(void) {
    return g_btlbgHeader;
}

void btlbg_pauseScroll(s32 arg0) {
    g_btlbgScrollPaused = arg0;
}

s32 btlbg_getId(void) {
    return g_btlbgId;
}

void btlbg_addSkySpeed(s32 arg0) {
    g_btlbgSkySpeed += arg0;
}

s32 btlbg_getSkySpeed(void) {
    return g_btlbgSkySpeed;
}

void btlbg_setLayerMask(s32 arg0) {
    g_btlbgLayerMask = arg0;
}
