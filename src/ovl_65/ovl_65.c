#include "ovl_65.h"

void func_801EF284(void) {
    Unk11C0 *s;
    GameData *ctx;
    u8 i;

    s = g_gameState->unk1C->unk8E4.ovl65->unk11C0;
    func_801EF45C();
    do {
        ClearOTagR(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], 0x1000);
        func_80019194();
        switch (func_801F3A7C()) {
        case 1:
            g_gameState->unk8 = s->unk6;
            break;
        case 2:
            g_gameState->unk8 = 0;
            break;
        }
        func_80023A0C();
        func_80032098();
        func_80023A4C();
        func_8002246C();
        DrawSync(0);
        VSync(g_gameState->unk9);
        ResetGraph(1);
        PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
        ctx = g_gameState->unk1C;
        i = ctx->unk8;
        DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->unk10[i]);
        g_gameState->unk1C->unk8 ^= 1;
        func_8002C0DC(g_gameState->unk1C->unk8);
    } while (g_gameState->unk8 == 8);
    func_801EF5C8();
}

void func_801EF45C(void) {
    Unk11C0 *s = g_gameState->unk1C->unk8E4.ovl65->unk11C0;
    FileEntry *e;

    func_8001CFB4();
    func_80021CDC(0);
    func_801F0E30();
    func_801EF6F8();
    func_801EF6D8();
    func_801EF814();
    if (s->unk10 != 0 && D_800FDF30.unkB != 0) {
        func_8004A06C(s->unk10);
        if (s->unk14 != 0) {
            func_8004BFB0();
        }
    }
    if (g_gameState->unk1C->unk56F & 1) {
        e = func_80063088(g_gameState->unk1C->unk7E4, g_gameState->unk1C->unkC | 0x50000);
        if (e != NULL) {
            g_gameState->unk1C->unk3A = e->unkC;
            g_gameState->unk1C->unk7FC[1] = e->unk4;
            g_gameState->unk1C->unk804[1] = e->unk8;
            e = func_80063088(g_gameState->unk1C->unk7E4, e->unkC);
            if (e != NULL) {
                g_gameState->unk1C->unk7FC[0] = e->unk4;
                g_gameState->unk1C->unk804[0] = e->unk8;
            }
        }
    }
    func_801EF890();
}

void func_801EF5C8(void) {
    GameData *g = g_gameState->unk1C;
    s32 i;

    if (!(g->unk56F & 8) && (g->unk0 & 0x20000000) && g->unk81C[0] != 0xFFFF) {
        g->unk0 |= 0x400000;
    }
    g_gameState->unk1C->unk7E4 = (FileTable *)-1;
    if (g_gameState->unk8 == 0) {
        func_800AD90C(0x8601, 0, 0x1E, 0, 0);
        for (i = 29; i >= 0; i--) {
            VSync(0);
        }
        g = g_gameState->unk1C;
        g->unk0 &= ~0x400000;
        func_80055210(g);
        D_80067950++;
        g_gameState->unk1C->unk0 |= 0x100000;
    }
}

s32 func_801EF6D8(void) {
    func_800E9DBC();
    return 0;
}

s32 func_801EF6F8(void) {
    FileEntry *e;
    Unk80021938 *f;

    if (func_800220E8(0x2713)) {
        f = func_80021E5C(0x2713);
        func_8001DAA4(D_80067948, f->unk8, f->unkC, 0);
        func_80022390(0x2713);
        DrawSync(0);
    } else {
        e = func_8001E218(1, 0x2713);
        func_8002177C(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, (void *)D_80067948, 0, -1, 0);
        while (func_800217C4(g_gameState->unk1C->unk854)) {
        }
    }
    g_gameState->unk1C->unk8E4.ovl65->unk1354->unk4 = D_80067948;
    func_8001C750(g_gameState->unk1C->unk8E4.ovl65->unk1354->unk4, 0);
    return 0;
}

s32 func_801EF814(void) {
    Unk1354 *p = g_gameState->unk1C->unk8E4.ovl65->unk1354;
    if (p->unk4 != 0) {
        func_8001C784((void *)p->unk4);
        g_gameState->unk1C->unk8E4.ovl65->unk1354->unk4 = 0;
    }
    return 0;
}

void func_801EF890(void) {
    Unk11C0 *b;
    FileEntry *f;
    GameData *g;
    s32 id;
    s32 size;
    s32 at;
    u8 *dst;
    s32 sector;
    s32 k;

    b = g_gameState->unk1C->unk8E4.ovl65->unk11C0;
    func_80021C60();
    g_gameState->unk1C->unk8BC = NULL;
    if (g_gameState->unk1C->unk56E == 6) {
        return;
    }
    id = 0x1000C;
    if (b->unk6 == 1) {
        id = 0x1000A;
    }
    f = func_8001E218(0, id);
    dst = b->unk1C;
    size = (f->unkC - f->unk4) << 11;
    func_80021974(func_80021CFC(), 0, id, (s32)dst, size, size, f->unk4, size);
    dst += size;
    if (b->unk6 == 1) {
        id = 0x2714;
    } else if (D_8007AEF0.h[0] >= 0x2B52) {
        id = 0x2716;
    } else {
        id = 0x2712;
    }
    f = func_8001E218(1, id);
    k = -0x146F00;
    size = (f->unkC - f->unk4) << 11;
    at = D_80067944 - (dst + k);
    at = at / 0x800 * 0x800;
    func_80021974(func_80021CFC(), 0, id, (s32)dst, at, size, f->unk4, size);
    dst += size;
    if (b->unk6 != 1) {
        return;
    }
    sector = g_gameState->unk1C->unk7FC[0];
    id = g_gameState->unk1C->unk3A | 0x60000;
    size = g_gameState->unk1C->unk804[0] << 11;
    at = D_80067944 - (dst + k);
    at = at / 0x800 * 0x800;
    func_80021974(func_80021CFC(), 0, id, (s32)dst, at, size, sector, size);
    dst += size;
    sector = g_gameState->unk1C->unk7FC[1];
    id = g_gameState->unk1C->unkC | 0x50000;
    size = g_gameState->unk1C->unk804[1] << 11;
    at = D_80067944 - (dst + k);
    at = at / 0x800 * 0x800;
    func_80021974(func_80021CFC(), 0, id, (s32)dst, at, size, sector, size);
    g = g_gameState->unk1C;
    if ((s16)g->unkC == g->unk7EA && g->unk8AC != -1) {
        sector = g->unk8AC;
        id = g->unk7E8 | 0x70000;
        func_80021974(func_80021CFC(), 1, id, 0, 0, 0, sector, -1);
    }
}

void func_801EFB98(void) {
    RECT *r = &D_801F7A78.unkAC[0].unk4;
    s32 x, y;
    func_8002B524(r->x + 6, r->y - 3, 0xB, D_80070460, 0);
    func_8002DEE4(r, 0xB, 1);
    func_8002705C(r, 0xC, 1);
    x = r->x;
    y = r->y;
    D_801F7A78.unk374 = func_8002920C(x + 0x13, y + 6, g_ot + 10,
                                      text_get(D_801F3BB8[D_801F7A78.unkA]), 0, D_801F7A78.unk374);
}

void func_801EFC68(void) {
    u8 buf[16];
    RECT *r;
    Character *c;
    u32 i;
    u8 dim;
    s32 col;
    u8 alive;
    u32 n;
    u8 *p;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk14C[i + 2] != 0) {
            r = &D_801F7A78.unkAC[i + 2].unk4;
            c = g_gameState->unk1C->unk54C[i];
            if (c != NULL) {
                alive = func_801F171C(c, i);
                dim = !alive;
                col = dim * 2;
                if (!alive) {
                    func_801F0394(r);
                }
                func_80028AF0(r->x + 4, r->y + 4, 0xB, i, dim);
                func_801F2264(r->x + 0x24, r->y + 0x12, 0xB, &D_801F7A78.unk258[i]);
                D_801F7A78.unk374 = func_8002920C(r->x + 0x29, r->y + 4, g_ot + 0xB, c->unk0, col, D_801F7A78.unk374);
                text_formatNumber(c->unkB, buf, 2, 1);
                func_800275DC(r->x + 0x72, r->y + 7, 0xB, 0xB4, dim);
                p = buf;
                D_801F7A78.unk374 = func_8002B644(r->x + 0x84, r->y + 5, g_ot + 0xB, p, col, D_801F7A78.unk374);
                func_80026FC4(r->x + 0x28, r->y + 0x13, 0xB, c->unk38, dim);
                func_8002B5FC(r->x + 0x29, r->y + 0x1C, 0xB, text_get(0), col);
                text_formatNumber(c->unkC, p, 7, 1);
                func_8002B56C(r->x + 0x62, r->y + 0x20, 0xB, p, col);
                if (c->unkB < 99) {
                    n = D_8006DF5C[c->unkB];
                } else {
                    n = c->unkC;
                }
                text_formatNumber(n - c->unkC, buf, 6, 1);
                func_8002B56C(r->x + 0x69, r->y + 0x2A, 0xB, buf, col);
                func_8002B5FC(r->x + 0x29, r->y + 0x26, 0xB, text_get(0xC), col);
            }
            func_8002DEE4(r, 0xB, 0);
            func_8002705C(r, 0xC, 0);
        }
    }
}

void func_801EFF50(void) {
    RECT *r = &D_801F7A78.unkAC[6].unk4;
    u8 buf[16];
    s32 x, y;

    if (D_801F7A78.unk14C[6] != 0) {
        func_8002DEE4(r, 0xB, 2);
        func_8002705C(r, 0xC, 2);
        text_formatNumber(D_801F7A78.unk24.unk0, buf, 6, 1);
        x = r->x;
        y = r->y;
        D_801F7A78.unk374 = func_8002920C(x + 0xC, y + 5, g_ot + 10,
                                          text_get(0), 0, D_801F7A78.unk374);
        D_801F7A78.unk374 = func_80029D60(r->x + 0x5A, r->y + 9, g_ot + 10,
                                          buf, 0, D_801F7A78.unk374);
    }
}

void func_801F0044(void) {
    RECT *r = &D_801F7A78.unkAC[7].unk4;
    u8 buf[16];
    s32 x, y;

    if (D_801F7A78.unk14C[7] != 0) {
        func_8002DEE4(r, 0xB, 2);
        func_8002705C(r, 0xC, 2);
        text_formatNumber(D_801F7A78.unk24.unk4, buf, 3, 1);
        x = r->x;
        y = r->y;
        D_801F7A78.unk374 = func_8002920C(x + 0xC, y + 5, g_ot + 10,
                                          text_get(1), 0, D_801F7A78.unk374);
        D_801F7A78.unk374 = func_80029D60(r->x + 0x6F, r->y + 9, g_ot + 10,
                                          buf, 0, D_801F7A78.unk374);
    }
}

void func_801F0138(void) {
    RECT *r = &D_801F7A78.unkAC[8].unk4;
    u8 buf[16];
    s32 x, y;
    u32 gil = D_801F7A78.unk24.unk7C.unk0 - D_801F7A78.unk24.unk7C.unk8;

    if (gil > 999999) {
        gil = 999999;
    }
    if (D_801F7A78.unk14C[8] != 0) {
        func_8002DEE4(r, 0xE, 2);
        func_8002705C(r, 0xF, 2);
        text_formatNumber(gil, buf, 6, 1);
        func_80063108(buf, D_801F3BC0);
        x = r->x;
        y = r->y;
        D_801F7A78.unk374 = func_8002920C(x + 0xC, y + 5, g_ot + 13,
                                          text_get(0xD), 0, D_801F7A78.unk374);
        D_801F7A78.unk374 = func_80029D60(r->x + 0x54, r->y + 9, g_ot + 13,
                                          buf, 0, D_801F7A78.unk374);
    }
}

void func_801F0264(void) {
    RECT *r = &D_801F7A78.unkAC[9].unk4;
    u8 buf[16];
    s32 x, y;
    u32 gil = g_gameState->unk1C->gil;

    if (gil > 9999999) {
        gil = 9999999;
    }
    if (D_801F7A78.unk14C[9] != 0) {
        func_8002DEE4(r, 0xE, 2);
        func_8002705C(r, 0xF, 2);
        text_formatNumber(gil, buf, 7, 1);
        func_80063108(buf, D_801F3BC4);
        x = r->x;
        y = r->y;
        D_801F7A78.unk374 = func_8002920C(x + 0xC, y + 5, g_ot + 13,
                                          text_get(0xE), 0, D_801F7A78.unk374);
        D_801F7A78.unk374 = func_80029D60(r->x + 0x4D, r->y + 9, g_ot + 13,
                                          buf, 0, D_801F7A78.unk374);
    }
}

void func_801F0394(RECT *r) {
    u8 *p = D_801F7A78.unk374;

    setlen(p, 3);
    ((TileWords *)p)->rgbc = 0x62202020;
    ((TileWords *)p)->xy = ((r->x + 2) & 0xFFFF) | ((r->y + 2) << 16);
    ((TileWords *)p)->wh = (r->w - 4) | ((r->h - 4) << 16);
    addPrim(g_ot + 11, p);
    p += 0x10;
    SetDrawTPage(p, 1, 1, 0);
    addPrim(g_ot + 11, p);
    p += 8;
    D_801F7A78.unk374 = p;
}

void func_801F04BC(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        D_801F7A78.unk14C[i] = D_801F3BC8[i].unk8;
        D_801F7A78.unkAC[i].unk4 = D_801F3BC8[i].unk0;
        if (D_801F3BC8[i].unk8 != 0) {
            func_80063118(&D_801F7A78.unkAC[i], D_801F3C4C[i], 1);
        } else {
            func_80063118(&D_801F7A78.unkAC[i], NULL, 0);
        }
    }
}

void func_801F0594(void) {
    s32 i;
    Window *w;
    Window *w2;
    s32 dx;
    s32 dy;
    s32 tx;
    s32 ty;
    TweenDef *t;
    WinDef *d;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk14[i] != 0) {
            w = D_801F7A78.unk158[i].unk0;
            if (w != NULL && w->unk1C != 0) {
                tx = w->unk0 - 0x26;
                dx = D_801F7A78.unkAC[i + 2].unk4.x - tx;
                ty = w->unk2 - 2;
                dy = D_801F7A78.unkAC[i + 2].unk4.y - ty;
                w->unk0 += dx;
                D_801F7A78.unk158[i].unk0->unk8.x += dx;
                D_801F7A78.unk158[i].unk0->unk2 += dy;
                D_801F7A78.unk158[i].unk0->unk8.y += dy;
            }
        }
        if (D_801F7A78.unk18[i] != 0) {
            w2 = D_801F7A78.unk1D8[i].unk0;
            d = &D_801F7A78.unkAC[i + 2];
            if (w2 != NULL && w2->unk1C != 0) {
                t = d->unkC;
                if (t != NULL && d->unk2 >= 2) {
                    if (t->unk4 != NULL) {
                        w2->unk0 += t->unk4[d->unk2];
                        D_801F7A78.unk1D8[i].unk0->unk8.x += t->unk4[d->unk2];
                    }
                    if (t->unk8 != NULL) {
                        D_801F7A78.unk1D8[i].unk0->unk2 += t->unk8[d->unk2];
                        D_801F7A78.unk1D8[i].unk0->unk8.y += t->unk8[d->unk2];
                    }
                }
            }
        }
    }
}

void func_801F076C(void) {
    s32 se;

    if (D_801F7A78.unk3 == 0) {
        func_801F2C7C();
        func_801F325C();
        if (!func_8002F810() && (g_padRepeat & BTN_CROSS)) {
            func_801F1688();
        }
    } else if (++D_801F7A78.unkC >= 24) {
        se = D_801F3BA0[D_801F7A78.unk8];
        if (se != 0) {
            snd_playSe(se);
        }
        D_801F7A78.unk8++;
        func_8002F8E4(D_801F7A78.unk370);
        if (D_80077DB5 != 0) {
            func_800325E0();
        } else {
            func_8002FAAC(0x10);
            func_8002F8A8(func_801F17D4);
        }
    }
}

void func_801F0878(void) {
    Rewards *r = &D_801F7A78.unk24;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk14[i] != 0) {
            func_801F2560(i, r->unk1C[i].unk0 - r->unk1C[i].unk8);
            func_801F269C(i);
        }
    }
    func_8002F55C(0x69);
    snd_playSe(0x67);
    for (i = 0; i < 7; i++) {
        func_80063118(&D_801F7A78.unkAC[2 + i], &D_801F3CDC[i], 0);
    }
    D_801F7A78.unkA = 1;
    D_801F7A78.unk14C[9] = 1;
    D_801F7A78.unk14C[8] = 1;
    D_801F7A78.unk370 = func_8002F8E4(0xC);
    func_8002FAAC(4);
    func_8002F8A8(func_801F16F4);
}

void func_801F0978(void) {
    s32 i;
    s32 se;

    D_801F7A78.unk14C[7] = 0;
    D_801F7A78.unk14C[6] = 0;
    D_801F7A78.unk14C[5] = 0;
    D_801F7A78.unk14C[4] = 0;
    D_801F7A78.unk14C[3] = 0;
    D_801F7A78.unk14C[2] = 0;
    func_801F1318();
    for (i = 0; i < 4; i++) {
        WinSlot_clear(&D_801F7A78.unk1D8[i]);
        WinSlot_clear(&D_801F7A78.unk158[i]);
    }
    se = D_801F3BA0[D_801F7A78.unk8];
    if (se != 0) {
        snd_playSe(se);
    }
    D_801F7A78.unk8++;
}

void func_801F0A44(void) {
    s32 i;
    Character *c;

    for (i = 0; i < 4; i++) {
        D_801F7A78.unk18[i] = 0;
        D_801F7A78.unk14[i] = 0;
    }
    i = 0;
    D_801F7A78.unk9 = 0;
    for (; i < 4; i++) {
        if ((D_800FDF30.unkA >> i) & 1) {
            if (g_gameState->unk1C->unk54C[i] != NULL) {
                D_801F7A78.unk14[i] = func_801F_canExp(g_gameState->unk1C->unk54C[i]);
                D_801F7A78.unk18[i] = func_801F_canAp(g_gameState->unk1C->unk54C[i]);
                if (D_801F7A78.unk14[i]) {
                    D_801F7A78.unk9++;
                }
            }
        }
    }
}

void func_801F0C0C(void) {
    Rewards *rw;
    u32 j;
    u8 found;
    s32 i;
    u32 q;

    rw = &D_801F7A78.unk24;
    memset(rw, 0, 0x88);
    func_801F0A44();
    if (D_801F7A78.unk9 != 0) {
        q = D_800FDF30.unk4 / D_801F7A78.unk9;
    } else {
        q = 0;
    }
    rw->unk0 = q;
    rw->unkC = 0;
    rw->unk4 = D_800FDF30.unk8;
    rw->unk7C.unk0 = D_800FDF30.unk0;
    rw->unk8 = (func_800625F8() < 100) ? D_800FDF30.unk1C : 0xFF;
    for (i = 0; i < 16; i++) {
        found = 0;
        if (D_800FDF30.unkC[i] == 0xFF) {
            continue;
        }
        for (j = 0; j < rw->unkC; j++) {
            if (D_800FDF30.unkC[i] == rw->unk10[j][0]) {
                found = 1;
                rw->unk10[j][1]++;
                break;
            }
        }
        if (!found) {
            if (rw->unkC >= 6) {
                return;
            }
            rw->unk10[rw->unkC][0] = D_800FDF30.unkC[i];
            rw->unk10[rw->unkC][1] = 1;
            rw->unkC++;
        }
    }
}

void func_801F0D98(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4) {
    D_801F7A78.unk374 = func_80029D60(arg0, arg1, g_ot + arg2, arg3, arg4, D_801F7A78.unk374);
}

void func_801F0DE4(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4) {
    D_801F7A78.unk374 = func_8002920C(arg0, arg1, g_ot + arg2, arg3, arg4, D_801F7A78.unk374);
}

void func_801F0E30(void) {
    func_800239D0();
    func_8002C064(0);
    func_8003556C(0);
    g_gameState->unk9 = 2;
    D_801F7A78.unkA = 0;
    D_801F7A78.unk8 = 0;
    D_801F7A78.unk36C = 0;
    D_801F7A78.unk10 = 0;
    D_801F7A78.unk0 = 1;
    D_801F7A78.unkC = 0;
    D_801F7A78.unk3 = 0;
    D_801F7A78.unk4 = 0;
    switch (func_801F1004()) {
    case 0:
        func_801F1060();
        break;
    case 1:
        func_801F107C();
        break;
    case 2:
        func_801F114C();
        break;
    }
}

u8 func_801F0F04(void) {
    if (D_801F7A78.unk1 != 0) {
        if (D_801F7A78.unk2 != 0) {
            goto count;
        }
        D_801F7A78.unk374 = D_801F7A78.unk378[g_drawBufs.unk0];
        if (D_80077DB5 == 0) {
            func_8002C308(0);
        }
        func_801F1160();
        func_801F11B8();
    }
    if (D_801F7A78.unk2 != 0) {
    count:
        if (++D_801F7A78.unk10 >= 2) {
            D_801F7A78.unk4 = 1;
        }
    }
    return D_801F7A78.unk4 == 0;
}

void func_801F0FC4(void) {
    func_8003556C(0);
    func_801F2E24();
    D_801F7A78.unk2 = 1;
}

void func_801F0FF4(void) {
    D_801F7A78.unk2 = 1;
}

s32 func_801F1004(void) {
    u8 mode = g_gameState->unk1C->unk56E;
    s32 ret = 0;

    if (mode == 4) {
        ret = D_800FDF4D != 0;
    } else if (mode != 7) {
        ret = 1;
        if (D_800FDF30.unkB != 0) {
            ret = 2;
        }
    }
    return ret;
}

void func_801F1060(void) {
    D_801F7A78.unk1 = 0;
    D_801F7A78.unk2 = 1;
    D_801F7A78.unk4 = 0;
}

void func_801F107C(void) {
    s32 i;

    D_801F7A78.unk1 = 1;
    D_801F7A78.unk2 = 0;
    func_801F0C0C();
    func_801F1300(&D_801F7A78.unk348);
    for (i = 0; i < 4; i++) {
        func_801F1300(&D_801F7A78.unk158[i]);
        func_801F1300(&D_801F7A78.unk1D8[i]);
    }
    text_setBank(g_rewardsText);
    func_801F12E0();
    func_801F2490();
    func_801F1FEC();
    func_801F3250();
    func_801F2D74();
    func_801F2BBC();
    func_801F04BC();
    g_gameState->unk9 = 0;
    func_8002F9D0();
}

void func_801F114C(void) {
    D_801F7A78.unk1 = 0;
    D_801F7A78.unk2 = 0;
}

void func_801F1160(void) {
    func_80024BD8();
    func_801EFB98();
    func_801F1228();
    func_801EFC68();
    func_801EFF50();
    func_801F0044();
    func_801F0138();
    func_801F0264();
}

void func_801F11B8(void) {
    void (*fn)(void) = D_801F3B88[D_801F7A78.unk8];

    if (fn != NULL) {
        fn();
    }
    func_801F13B4();
    func_801F0594();
    D_801F7A78.unk36C++;
}

void func_801F1228(void) {
    s32 x, y;
    RECT *r = &D_801F7A78.unkAC[1].unk4;
    func_8002B524(r->x + 6, r->y - 3, 0xB, D_80070450, 0);
    func_8002DEE4(r, 0xB, 1);
    func_8002705C(r, 0xC, 1);
    x = r->x;
    y = r->y;
    D_801F7A78.unk374 = func_8002920C(x + 0x13, y + 6, g_ot + 10,
                                      text_get(7), 0, D_801F7A78.unk374);
}

void func_801F12E0(void) {
    D_801F7A78.unk378[0] = D_801F5278[0];
    D_801F7A78.unk378[1] = D_801F5278[1];
}

void func_801F1300(WinSlot *arg0) {
    arg0->unk4 = 0;
    arg0->unk6 = 0;
    arg0->unk1C = 0;
    arg0->unk8 = 0;
    arg0->unk0 = 0;
}

void func_801F1318(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk14[i] != 0 && D_801F7A78.unk158[i].unk0 != NULL) {
            func_8002DD00(D_801F7A78.unk158[i].unk0);
        }
        if (D_801F7A78.unk18[i] != 0 && D_801F7A78.unk1D8[i].unk0 != NULL) {
            func_8002DD00(D_801F7A78.unk1D8[i].unk0);
        }
    }
}

void func_801F13B4(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if (D_801F7A78.unk14C[i] != 0 && D_801F7A78.unkAC[i].unkC != 0) {
            func_800631AC(&D_801F7A78.unkAC[i]);
        }
    }
}

void func_801F143C(void) {
    s32 se;

    if (!func_8002F810() && (g_padRepeat & BTN_CROSS)) {
        se = D_801F3BA0[D_801F7A78.unk8];
        if (se != 0) {
            snd_playSe(se);
        }
        D_801F7A78.unk8++;
        func_801F1560();
    }
}

void func_801F14C0(void) {
    func_801F2538();
    func_801F2080();
    if (!func_8002F810() && (g_padRepeat & BTN_CROSS)) {
        func_801F0878();
    }
}

void func_801F1518(void) {
    if (!func_8002F810() && (g_padRepeat & BTN_CROSS)) {
        func_801F160C();
    }
}

void func_801F1560(void) {
    Rewards *r = &D_801F7A78.unk24;
    s32 i;
    u8 any = 0;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk18[i] != 0) {
            func_801F1A04(i, r->unk4C[i].unk0);
        }
        if (D_801F7A78.unk14[i] != 0) {
            any = 1;
        }
    }
    if (any) {
        func_8002F514(0x69);
    }
}

void func_801F160C(void) {
    s32 se;

    if (func_801F3264()) {
        se = D_801F3BA0[D_801F7A78.unk8];
        if (se != 0) {
            snd_playSe(se);
        }
        D_801F7A78.unk8++;
        if (D_800FDF30.unk0 != 0) {
            func_8002F514(0x6D);
        }
    }
}

void func_801F1688(void) {
    Rewards *r = &D_801F7A78.unk24;

    if (r->unk7C.unk0 != r->unk7C.unk8) {
        func_801F2D08(r->unk7C.unk0 - r->unk7C.unk8);
        r->unk7C.unk8 = r->unk7C.unk0;
        func_8002F55C(0x6D);
        D_801F7A78.unkC = 0;
    } else {
        D_801F7A78.unkC = 24;
    }
    D_801F7A78.unk3 = 1;
}

void func_801F16F4(void) {
    func_801F332C();
    func_8002FA50(0xC);
}

u8 func_801F171C(Character *arg0, s32 arg1) {
    if (!func_801F_alive(arg0)) {
        return 0;
    }
    return (D_800FDF30.unkA >> arg1) & 1;
}

void func_801F1774(void) {
    s32 se = D_801F3BA0[D_801F7A78.unk8];

    if (se != 0) {
        snd_playSe(se);
    }
    D_801F7A78.unk8++;
}

void func_801F17D4(void) {
    D_801F7A78.unk2 = 1;
    func_8002DE60();
}

void func_801F17FC(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4) {
    D_801F7A78.unk374 = func_8002B644(arg0, arg1, g_ot + arg2, arg3, arg4, D_801F7A78.unk374);
}

void func_801F1848(void) {
    WinSlot *w;
    RECT *r;
    s32 i;
    RECT *rr;

    w = D_801F7A78.unk1D8;
    for (i = 0; i < 4; w++, i++) {
        if (D_801F7A78.unk18[i] != 0) {
            if (w->unk8 == 0) {
                if (w->unk0 == NULL) {
                    if (w->unk4 != 0) {
                        D_801F8200 = r = &D_801F7A78.unkAC[i + 2].unk4;
                        D_801F81F8.x = r->x + 0x26;
                        rr = &D_801F81F8;
                        rr->y = r->y + 0x1B;
                        rr->w = 0x70;
                        rr->h = 0x19;
                        w->unk0 = func_8002DC54(rr, 6, 2);
                        w->unk0->unk50 = i;
                        func_8002E304(w->unk0, (s32)D_801F5228);
                        func_8002E2EC(w->unk0, func_801F1D94);
                        func_8002E2FC(w->unk0, func_801F2248);
                        func_8002CF20(w->unk0, 0);
                        snd_playSe(0x413);
                        w->unk4--;
                    }
                } else if (++w->unk1C >= 0x40) {
                    func_8002D1AC(w->unk0, 0);
                    func_8002DD00(w->unk0);
                    w->unk8 = 4;
                    w->unk1C = 0;
                    w->unk6++;
                }
            } else {
                w->unk8--;
            }
        }
    }
}

void func_801F1A04(s32 arg0, u32 arg1) {
    Character *c;
    s32 i;
    s32 j;
    Unk80069018 *e;
    s32 k;
    s32 need;
    s32 have;

    c = g_gameState->unk1C->unk54C[arg0];
    if (arg1 != 0 && func_80034A30(c)) {
        D_801F7A78.unk24.unk4C[arg0].unk8 += arg1;
        for (i = 0; i < 5; i++) {
            if (c->unk39[i] != 0xFF) {
                e = &D_80069018[c->unk39[i]];
                for (j = 0; j < 3; j++) {
                    if (e->unkC[j] != 0) {
                        k = func_80034430(c->unk34.b.id, e->unkC[j]);
                        if (k >= 0) {
                            need = D_8006CB28[c->unk34.b.row][k].unk1;
                            have = c->unk58[k];
                            if (have < need) {
                                if (arg1 >= need - have) {
                                    c->unk58[k] = need;
                                    func_801F21DC(arg0, e->unkC[j]);
                                } else {
                                    c->unk58[k] = have + arg1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

u8 func_801F1BA4(Character *c, s32 arg1) {
    u8 ret;
    s32 i;
    s32 j;
    s32 ch;
    s32 k;
    u8 *eq;
    Unk80069018 *e;
    s32 have;

    ret = 0;
    eq = c->unk39;
    ch = c->unk34.b.id;
    memset(D_801F8208, 0, 0x30);
    for (i = 0; i < 5; i++) {
        if (eq[i] != 0xFF) {
            e = &D_80069018[eq[i]];
            for (j = 0; j < 3; j++) {
                if (e->unkC[j] != 0) {
                    k = func_80034430(ch & 0xFF, e->unkC[j]);
                    if (k >= 0) {
                        have = c->unk58[k];
                        if (have < D_8006CB28[c->unk34.b.row][k].unk1) {
                            D_801F8208[k]++;
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < 0x30; i++) {
        if (D_801F8208[i] != 0) {
            have = c->unk58[i];
            if (D_801F8208[i] * arg1 >= D_8006CB28[c->unk34.b.row][i].unk1 - have) {
                ret = 1;
                break;
            }
        }
    }
    return ret;
}

void func_801F1D94(Window *w) {
    Window *v;
    WinSlot *s;
    TileWords *t;
    u8 *name;
    s32 slot;
    s32 icon;
    s32 id;

    v = w;
    slot = w->unk50;
    s = &D_801F7A78.unk1D8[slot];
    id = s->unkA[s->unk6];
    if (id < 0xC0) {
        name = D_801F494C + D_801F3D4C[id].unkE;
        func_800275DC(w->unk0 + 6, w->unk2 + 7, w->unk18, 0xBC, 0);
    } else {
        name = D_801F4F94 + D_8006D128[id - 0xC0].unk2;
        if (func_80034A04(g_gameState->unk1C->unk54C[slot]->unk88, id)) {
            icon = 0xBD;
        } else {
            icon = 0xF4;
        }
        func_800275DC(w->unk0 + 6, w->unk2 + 7, w->unk18, icon, 0);
    }
    func_8002B5FC(v->unk0 + 0x16, v->unk2 + 6, w->unk18, name, 0);
    setlen(g_primPtr, 3);
    t = (TileWords *)g_primPtr;
    t->rgbc = 0x62101010;
    t->xy = (v->unk0 + 4) | ((v->unk2 + 4) << 16);
    t->wh = (v->unk4 - 4) | ((v->unk6 - 4) << 16);
    addPrim(&g_ot[w->unk18], t);
    g_primPtr += sizeof(TileWords);
    SetDrawTPage(g_primPtr, 1, 1, 0x20);
    addPrim(&g_ot[w->unk18], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
}

void func_801F1FEC(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk18[i] != 0) {
            D_801F7A78.unk20[i] = func_801F1BA4(g_gameState->unk1C->unk54C[i], func_801F20A8(i));
        } else {
            D_801F7A78.unk20[i] = 0;
        }
    }
}

void func_801F2080(void) {
    func_801F213C();
    func_801F1848();
}

s32 func_801F20A8(s32 i) {
    u32 ap = D_801F7A78.unk24.unk4;
    Counter *c = &D_801F7A78.unk24.unk4C[i];

    if (func_800349BC(g_gameState->unk1C->unk54C[i]->unk34.b.id, 0xEC)) {
        c->unk0 = ap * 2;
    } else {
        c->unk0 = ap;
    }
    c->unk4 = 1;
    c->unk8 = 0;
    return c->unk0;
}

void func_801F213C(void) {
    s32 i;
    Counter *c;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk18[i] != 0) {
            c = &D_801F7A78.unk24.unk4C[i];
            if (c->unk8 < c->unk0) {
                func_801F1A04(i, Counter_step(c));
            }
        }
    }
}

void func_801F21DC(s32 arg0, u8 arg1) {
    D_801F7A78.unk1D8[arg0].unkA[D_801F7A78.unk1D8[arg0].unk4] = arg1;
    D_801F7A78.unk1D8[arg0].unk4++;
    D_801F7A78.unk2B8[arg0].unk11[D_801F7A78.unk2B8[arg0].unk10] = arg1;
    D_801F7A78.unk2B8[arg0].unk10++;
}

void func_801F2248(Window *arg0) {
    D_801F7A78.unk1D8[arg0->unk50].unk0 = 0;
}

void func_801F2264(s32 x, s32 y, s32 otz, Popup *p) {
    SprtWords *s;
    u8 *pkt;
    u32 clut;
    u32 xy;
    DR_TPAGE *t;

    if (p->unk0 != 0 && p->unk1 != 3) {
        y += p->unk16;
        clut = GetClut(0, 0x1FF) << 16;
        setlen(g_primPtr, 4);
        xy = (x & 0xFFFF) | (y << 16);
        pkt = g_primPtr;
        s = (SprtWords *)pkt;
        s->rgbc = 0x64808080;
        s->xy = xy;
        s->uvclut = clut | 0xB000;
        s->wh = 0x14004C;
        g_primPtr = (u8 *)(s + 1);
        addPrim(&g_ot[otz], s);
        t = (DR_TPAGE *)g_primPtr;
        SetDrawTPage(t, 1, 1, 0x9F);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DR_TPAGE);
        setlen(g_primPtr, 4);
        s = (SprtWords *)g_primPtr;
        s->rgbc = 0x66808080;
        s->xy = xy;
        s->uvclut = clut | 0xC400;
        s->wh = 0x14004C;
        g_primPtr = (u8 *)(s + 1);
        addPrim(&g_ot[otz], s);
        SetDrawTPage((DR_TPAGE *)g_primPtr, 1, 1, 0xDF);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DR_TPAGE);
    }
}

void func_801F2490(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk14[i] != 0) {
            D_801F7A78.unk1C[i] = func_801F27F0(g_gameState->unk1C->unk54C[i], func_801F26F8(i));
        } else {
            D_801F7A78.unk1C[i] = 0;
        }
        func_801F2A4C(&D_801F7A78.unk258[i]);
    }
}

void func_801F2538(void) {
    func_801F2834();
    func_801F28F0();
}

void func_801F2560(s32 i, u32 exp) {
    u32 need;
    Character *c = g_gameState->unk1C->unk54C[i];

    if (exp != 0) {
        if (c->unkC > 9999998) {
            c->unkC = 9999999;
            return;
        }
        D_801F7A78.unk24.unk1C[i].unk8 += exp;
        c->unkC += exp;
        if (c->unkC > 9999998) {
            c->unkC = 9999999;
        }
        if (c->unkB < 99 && (need = D_8006DF5C[c->unkB]) <= c->unkC) {
            do {
                func_801F29BC(i);
            } while (c->unkB < 99 && (need = D_8006DF5C[c->unkB]) <= c->unkC);
        }
    }
}

void func_801F269C(s32 arg0) {
    Popup *p = &D_801F7A78.unk258[arg0];

    if (p->unk14 != 0 || p->unk0 != 0) {
        p->unk1 = 2;
        p->unk16 = -0x11;
        p->unk4 = 0;
        p->unk2 = 0;
        p->unk0 = 1;
    }
}

u32 func_801F26F8(s32 arg0) {
    u32 exp = D_801F7A78.unk24.unk0;
    Character *m = g_gameState->unk1C->unk54C[arg0];
    Counter *c = &D_801F7A78.unk24.unk1C[arg0];
    u32 step;

    c->unk0 = exp;
    if (func_800349BC(m->unk34.b.id, 0xEB)) {
        c->unk0 += exp >> 1;
    }
    if (exp < 16) {
        c->unk4 = 1;
    } else if (exp < 512) {
        c->unk4 = (exp + 15) >> 4;
    } else {
        step = 32;
        c->unk4 = step;
        if (exp / step > 192) {
            c->unk4 = (exp + 191) / 192;
        }
    }
    c->unk8 = 0;
    return c->unk0;
}

s32 func_801F27F0(Character *arg0, s32 arg1) {
    if (arg0->unkB >= 99) {
        return 0;
    }
    return arg0->unkC + arg1 >= D_8006DF5C[arg0->unkB];
}

void func_801F2834(void) {
    s32 i;
    Counter *c;
    s32 step;
    u8 done = 1;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk14[i] != 0) {
            c = &D_801F7A78.unk24.unk1C[i];
            if (c->unk8 < c->unk0) {
            step = Counter_step(c);
            func_801F2560(i, step);
            done = 0;
            }
        }
    }
    if (done) {
        func_8002F55C(0x69);
    }
}

void func_801F28F0(void) {
    Popup *p;
    s32 i;

    for (p = D_801F7A78.unk258, i = 0; i < 4; p++, i++) {
        if (D_801F7A78.unk14[i] != 0) {
            if (p->unk0 == 0) {
                if (p->unk14 != 0) {
                    func_801F2A6C(p);
                }
            } else if (p->unk2 != 0 && func_801F2AC8(p) == 4) {
                func_801F2AAC(p);
            }
        }
    }
}

void func_801F29BC(s32 arg0) {
    Character *m = g_gameState->unk1C->unk54C[arg0];

    func_80033CCC(m->unk34.b.id, m->unkB + 1);
    D_801F7A78.unk158[arg0].unk4++;
    D_801F7A78.unk258[arg0].unk14++;
}

void func_801F2A4C(Popup *arg0) {
    arg0->unk0 = 0;
    arg0->unk14 = 0;
    arg0->unk16 = 0;
    arg0->unk4 = 0;
    arg0->unk1 = 0;
    arg0->unk2 = 1;
}

void func_801F2A6C(Popup *arg0) {
    arg0->unk4 = 0;
    arg0->unk16 = 0;
    arg0->unk1 = 1;
    arg0->unk0 = 1;
    arg0->unk14--;
    snd_playSe(0x2AB);
}

void func_801F2AAC(Popup *arg0) {
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk16 = 0;
    arg0->unk15++;
}

s32 func_801F2AC8(Popup *arg0) {
    switch (arg0->unk1) {
    case 1:
        arg0->unk16 += D_801F5230[arg0->unk4];
        if (++arg0->unk4 >= 10) {
            arg0->unk1 = 2;
            arg0->unk4 = 0;
        }
        break;
    case 2:
        if (++arg0->unk4 >= 0x40) {
            arg0->unk4 = 0;
            if (arg0->unk14 != 0) {
                arg0->unk1 = 3;
            } else {
                arg0->unk1 = 4;
            }
        }
        break;
    case 3:
        if (++arg0->unk4 >= 4) {
            arg0->unk1 = 4;
            arg0->unk4 = 0;
        }
        break;
    }
    return arg0->unk1;
}

void func_801F2BBC(void) {
    Counter *c = &D_801F7A78.unk24.unk7C;
    s32 i;
    u8 found = 0;
    Character *m;

    for (i = 0; i < 4; i++) {
        m = g_gameState->unk1C->unk54C[i];
        if (m != NULL && func_800349BC(m->unk34.b.id, 0xED)) {
            found = 1;
        }
    }
    if (found) {
        c->unk0 += c->unk0 >> 1;
    }
    c->unk4 = 1;
    c->unk8 = 0;
}

void func_801F2C7C(void) {
    Counter *c = &D_801F7A78.unk24.unk7C;
    s32 step;

    if (c->unk8 < c->unk0) {
        step = Counter_step(c);
        func_801F2D08(step);
        if (c->unk8 >= c->unk0) {
            func_8002F55C(0x6D);
        }
    }
}

void func_801F2D08(s32 arg0) {
    u32 gil;

    if (arg0 != 0) {
        D_801F7A78.unk24.unk7C.unk8 += arg0;
        gil = g_gameState->unk1C->gil;
        if (gil + arg0 > 9999999) {
            arg0 = 9999999 - gil;
            D_801F7A78.unk24.unk7C.unk8 = D_801F7A78.unk24.unk7C.unk0;
        }
        g_gameState->unk1C->gil += arg0;
    }
}

void func_801F2D74(void) {
    StoreImage(&D_801F524C, D_801F7DF8);
    DrawSync(0);
    LoadImage(&D_801F523C, (u32 *)D_801EE03C);
    DrawSync(0);
    LoadImage(&D_801F5244, (u32 *)(D_801EE03C + 0x20));
    DrawSync(0);
    LoadImage(&D_801F524C, (u32 *)D_801EE19C);
    DrawSync(0);
    LoadImage(&D_801F5254, (u32 *)(D_801EE19C + 0x200));
    DrawSync(0);
}

void func_801F2E24(void) {
    LoadImage(&D_801F525C, D_801F7DF8);
    DrawSync(0);
}

u8 func_801F2E58(void) {
    TextSize ts;
    RECT rr;
    u32 i;
    u8 full;
    u8 f;
    Rewards *rw;
    WinSlot *ws;
    s32 n;
    s32 k;

    full = 0;
    rw = &D_801F7A78.unk24;
    ws = &D_801F7A78.unk348;
    for (i = 0; i < rw->unkC; i++) {
        n = rw->unk10[i][1];
        if (n != func_80027D84(rw->unk10[i][0], n)) {
            full = 1;
        }
    }
    f = full;
    if (rw->unk8 != 0xFF) {
        func_8006238C(rw->unk8);
    }
    if (f) {
        k = 0x10;
        ts = func_8002AF70(text_get(0xA));
        rr.x = D_801F7A78.unk368->unk0 + ((D_801F7A78.unk368->unk4 - k - ts.w) >> 1);
        rr.y = D_801F7A78.unk368->unk2 + ((D_801F7A78.unk368->unk6 - k - ts.h) >> 1);
        rr.w = ts.w + 0x10;
        rr.h = ts.h + 0xF;
        ws->unk0 = func_8002DC54(&rr, 6, 2);
        func_8002E2EC(ws->unk0, func_801F3434);
        func_8002E2FC(ws->unk0, (void (*)(Window *))func_801F347C);
        func_8002CF20(ws->unk0, 0);
        func_8002E31C(ws->unk0, 1);
        func_8002E304(ws->unk0, D_800704C8);
    }
    return !f;
}

void func_801F2FE8(Window *w) {
    Window *r;
    Unk80069018 *it;
    u8 *s;
    u32 i;
    s32 off;
    s32 y;
    s32 y2;
    s32 k;
    s32 k1;
    s32 k3;
    Rewards *rw;

    r = w;
    i = 0;
    rw = &D_801F7A78.unk24;
    off = r->unk6 - r->unk8.h;
    for (; i < rw->unkC; i++) {
        it = &D_80069018[rw->unk10[i][0]];
        y = i * 15 + 7;
        y2 = i * 15 + 10;
        text_formatNumber(rw->unk10[i][1], D_801F8238, 2, 1);
        func_80026C34(r->unk0 + 7, r->unk2 + y - off, w->unk18, rw->unk10[i][0], 1);
        func_8002B5FC(r->unk0 + 0x16, r->unk2 + y - off, w->unk18, D_8006A218 + it->unk0, 0);
        func_8002B56C(r->unk0 + 0x88, r->unk2 + y2 - off, w->unk18, D_801F8238, 0);
    }
    if (rw->unkC == 0) {
        s = text_get(9);
        func_8002B5FC(r->unk0 + ((r->unk4 - (text_getInfo(9) & 0xFFF)) >> 1), r->unk2 - (k1 = off - 0x28), w->unk18, s, 0);
    }
    if (rw->unk8 != 0xFF) {
        k = off - 0x66;
        func_80027710(r->unk0 + 8, r->unk2 - k, w->unk18, &D_801F526C, 0);
        func_8002BD54(D_801F8238, (u8 *)func_8006276C(rw->unk8));
        func_8002AD38(D_801F8238, text_get(0xB));
        func_8002B5FC(r->unk0 + 0x16, r->unk2 - k, w->unk18, D_801F8238, 0);
    }
    func_800269F4(r->unk0 + 2, r->unk2 - (k3 = off - 0x61), w->unk18, r->unk4 - 5);
}

void func_801F3250(void) {
    D_801F8278 = 0;
}

void func_801F325C(void) {
}

u8 func_801F3264(void) {
    u8 ret = 0;

    if (D_801F8278 != 1) {
        if (!func_801F2E58()) {
            snd_playSe(0x416);
            D_801F8278 = 1;
        } else {
            ret = 1;
        }
    } else {
        func_8002D1AC(D_801F7A78.unk348.unk0, 0);
        func_8002DD00(D_801F7A78.unk348.unk0);
        ret = 1;
    }
    if (ret) {
        if (D_801F7A78.unk24.unkC != 0 || D_801F7A78.unk24.unk8 != 0xFF) {
            snd_playSe(0x6C);
        }
    }
    return ret;
}

void func_801F332C(void) {
    D_801F7A78.unk368 = func_8002DC54(&D_801F5264, 0xE, 1);
    func_8002E2EC(D_801F7A78.unk368, func_801F2FE8);
    func_8002E2F4(D_801F7A78.unk368, func_801F33A0);
    func_8002E31C(D_801F7A78.unk368, 1);
    func_8002E334(D_801F7A78.unk368, 0x10);
}

void func_801F33A0(Window *w) {
    s32 n = func_8002AE7C((u8 *)D_800703FC.unk94);
    s32 k = 0x97;

    func_8002B524(w->unk0 + 8, w->unk2 - 3, w->unk18 - 1, D_800703FC.unk68, 0);
    func_8002B524(w->unk0 - (n - k), w->unk2 - 3, w->unk18 - 1, D_800703FC.unk94, 0);
}

void func_801F3434(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 8, arg0->unk2 + 8, arg0->unk18, text_get(0xA), 0);
}

void func_801F347C(void) {
    D_801F7A78.unk348.unk0 = NULL;
}

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", g_rewardsText);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EE03C);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EE19C);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EF260);

void func_801F3488(s32 arg0) {
    s16 c = 0xFF;
    u8 state = 0;
    DispBuf b[2];
    RECT rect = D_801EF260;
    DispBuf *cur;

    func_801F3818(b);
    cur = b;
    DrawSync(0);
    func_801F3A20(arg0);
    while (1) {
        cur = (cur == &b[0]) ? &b[1] : &b[0];
        ClearOTag(cur->unk70, 2);
        switch (state) {
        case 0:
            state++;
            break;
        case 1:
            setRGB0(&cur->unkB8, c, c, c);
            c -= 4;
            addPrim(&cur->unk70[0], &cur->unk78);
            addPrim(&cur->unk70[0], &cur->unk94);
            addPrim(&cur->unk70[1], &cur->unkB0);
            if (c < 0) {
                c = 0;
                state++;
            }
            break;
        case 2:
            setRGB0(&cur->unkB8, 0, 0, 0);
            addPrim(&cur->unk70[0], &cur->unk78);
            addPrim(&cur->unk70[0], &cur->unk94);
            addPrim(&cur->unk70[1], &cur->unkB0);
            if (g_padPortPressed[0] & BTN_CROSS) {
                state++;
            }
            break;
        case 3:
            c += 4;
            if (c >= 0x100) {
                ClearImage(&rect, 0, 0, 0);
                DrawSync(0);
                if (c > 0x100) {
                    state++;
                }
            } else {
                setRGB0(&cur->unkB8, c, c, c);
                addPrim(&cur->unk70[0], &cur->unk78);
                addPrim(&cur->unk70[0], &cur->unk94);
                addPrim(&cur->unk70[1], &cur->unkB0);
            }
            break;
        case 4:
            ClearImage(&rect, 0, 0, 0);
            DrawSync(0);
            return;
        }
        func_80019194();
        DrawSync(0);
        VSync(0);
        PutDispEnv(&cur->disp);
        DrawOTagEnv(cur->unk70, &cur->draw);
    }
}

void func_801F3818(DispBuf *b) {
    RECT rect = D_801EF27C;
    u32 i;

    ClearImage(&rect, 0, 0, 0);
    SetDefDrawEnv(&b[0].draw, 0, 0, 320, 224);
    SetDefDispEnv(&b[0].disp, 320, 0, 320, 224);
    SetDefDrawEnv(&b[1].draw, 320, 0, 320, 224);
    SetDefDispEnv(&b[1].disp, 0, 0, 320, 224);
    for (i = 0; i < 2; i++) {
        b[i].draw.dtd = 1;
        b[i].draw.isbg = 1;
        b[i].disp.isinter = 0;
        setRECT(&b[i].disp.screen, 0, 8, 256, 224);
        func_80063580(&b[i].unk78, GetTPage(2, 0, 0, 256));
        setXY0(&b[i].unk80, 0, 0);
        setUV0(&b[i].unk80, 0, 0);
        setWH(&b[i].unk80, 256, 224);
        setRGB0(&b[i].unk80, 0x80, 0x80, 0x80);
        func_80063580(&b[i].unk94, GetTPage(2, 0, 256, 256));
        setXY0(&b[i].unk9C, 256, 0);
        setUV0(&b[i].unk9C, 0, 0);
        setWH(&b[i].unk9C, 64, 224);
        setRGB0(&b[i].unk9C, 0x80, 0x80, 0x80);
        func_800635E0(&b[i].unkB0, GetTPage(0, 2, 0, 0));
        setXY0(&b[i].unkB8, 0, 0);
        setWH(&b[i].unkB8, 320, 224);
        setRGB0(&b[i].unkB8, 0, 0, 0);
        SetSemiTrans(&b[i].unkB8, 1);
    }
}

void func_801F3A20(s32 arg0) {
    FileEntry *e = func_8001E218(1, 0xB);

    func_80022DF0(g_gameState->unk20 + e->unk4, (e->unkC - e->unk4) << 11, arg0);
    func_8001D8A8(arg0);
}

s32 func_801F3A7C(void) {
    GameData *g;

    g = g_gameState->unk1C;
    func_8004C204();
    switch (g->unk56E) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
        if (!func_801F0F04()) {
            if (g->unk56E == 2) {
                g->unk56E = 1;
            }
            func_800F4E68();
            func_801F0FC4();
            return 1;
        }
        func_8004BC38();
        return 0;
    case 6:
        if (func_800240E8()) {
            func_800240B0(0);
        }
        func_800F4E68();
        func_801F3488((s32)g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk1C);
        return 2;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EF27C);
