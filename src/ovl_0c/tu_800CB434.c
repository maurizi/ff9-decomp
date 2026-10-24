#include "ovl_0c.h"

void func_800CB434(void) {
    u8 *s;
    u8 *p;
    s32 w;
    s32 i;

    p = g_gameState->unk1C->unk80C;
    if (p != NULL && *p != 0xFF) {
        s = p;
    } else {
        s = text_get(1);
    }
    w = func_8002B980(s, 0) + 0x10;
    if (w < 0x44) {
        w = 0x44;
    }
    D_800CDFCC[5].w = w;
    D_800CDFCC[5].x = 0x134 - w;
    for (i = 0; i < 6; i++) {
        D_800D5438.unk8[i].unk4 = D_800CDFCC[i];
        func_80063118(&D_800D5438.unk8[i], NULL, 0);
    }
    if (D_80077E51 != 0) {
        func_80063118(&D_800D5438.unk8[0], &D_800CE00C[0], 1);
        func_80063118(&D_800D5438.unk8[1], &D_800CE00C[1], 1);
        func_80063118(&D_800D5438.unk8[2], &D_800CE00C[2], 1);
        func_80063118(&D_800D5438.unk8[3], &D_800CE00C[3], 1);
    }
}

void func_800CB594(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        func_800362C8();
    } else if (g_padRepeat & BTN_CROSS) {
        func_800CC54C();
    } else if (g_padRepeat & PAD_UP) {
        snd_playSe(0x67);
        D_800D5438.unk0 = D_800D5438.unk0 != 0 ? D_800D5438.unk0 - 1 : 6;
    } else if (g_padRepeat & PAD_DOWN) {
        snd_playSe(0x67);
        D_800D5438.unk0 = D_800D5438.unk0 < 6 ? D_800D5438.unk0 + 1 : 0;
    }
}

void func_800CB664(void) {
    Unk80077E50 *m;
    s32 old;
    s32 new;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        D_800D5438.unk1 = 0;
        return;
    } else if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x67);
        func_800361CC(D_800D5438.unk0 + 1);
        return;
    } else if (g_padRepeat & PAD_UP) {
        m = &D_80077E50;
        old = m->unk3;
        new = func_800337D0(old);
    } else if (g_padRepeat & PAD_DOWN) {
        m = &D_80077E50;
        old = m->unk3;
        new = func_8003380C(old);
    } else {
        return;
    }
    if (old != new) {
        snd_playSe(0x67);
        old = new;
        m->unk3 = old;
    }
}

void func_800CB73C(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        if (D_800D5438.unk4 == 0) {
            D_800D5438.unk1 = 0;
        } else {
            D_800D5438.unk4 = 0;
        }
    } else if (g_padRepeat & PAD_UP) {
        snd_playSe(0x67);
        if (D_800D5438.unk4 == 0) {
            D_800D5438.unk2 = D_800D5438.unk2 != 0 ? D_800D5438.unk2 - 1 : 3;
        } else {
            D_800D5438.unk3 = D_800D5438.unk3 != 0 ? D_800D5438.unk3 - 1 : 3;
        }
    } else if (g_padRepeat & PAD_DOWN) {
        snd_playSe(0x67);
        if (D_800D5438.unk4 == 0) {
            D_800D5438.unk2 = D_800D5438.unk2 < 3 ? D_800D5438.unk2 + 1 : 0;
        } else {
            D_800D5438.unk3 = D_800D5438.unk3 < 3 ? D_800D5438.unk3 + 1 : 0;
        }
    } else if (g_padRepeat & BTN_CROSS) {
        if (D_800D5438.unk4 == 0) {
            snd_playSe(0x67);
            D_800D5438.unk4 = 1;
            D_800D5438.unk3 = D_800D5438.unk2;
        } else {
            snd_playSe(0x67);
            func_800CB8C8(D_800D5438.unk2, D_800D5438.unk3);
            D_800D5438.unk4 = 0;
            D_800D5438.unk2 = D_800D5438.unk3;
        }
    }
}

void func_800CB8C8(s32 arg0, s32 arg1) {
    Character *c;
    Character *t;

    if (arg0 == arg1) {
        c = g_gameState->unk1C->party[arg1];
        if (c != NULL) {
            c->unk34.b.unk9 ^= 1;
        }
    } else {
        t = g_gameState->unk1C->party[D_800D5438.unk2];
        g_gameState->unk1C->party[D_800D5438.unk2] = g_gameState->unk1C->party[D_800D5438.unk3];
        g_gameState->unk1C->party[D_800D5438.unk3] = t;
        func_800CB9D8(arg0, arg1);
        if (g_gameState->unk1C->party[D_80077E50.unk3] == NULL) {
            D_80077E50.unk3 = party_getFirstMember();
        }
    }
}

void func_800CB9D8(s32 arg0, s32 arg1) {
    RECT a0;
    RECT a1;
    RECT a2;
    RECT b0;
    RECT b1;
    RECT b2;

    func_80028A40(arg0, &a0, &a1, &a2);
    func_80028A40(arg1, &b0, &b1, &b2);
    StoreImage(&a0, D_80076F80);
    DrawSync(0);
    MoveImage(&a1, 0x3C0, 0x1B0);
    DrawSync(0);
    MoveImage(&a2, 0x3D0, 0x1B0);
    DrawSync(0);
    MoveImage(&b0, a0.x, a0.y);
    DrawSync(0);
    MoveImage(&b1, a1.x, a1.y);
    DrawSync(0);
    MoveImage(&b2, a2.x, a2.y);
    DrawSync(0);
    a1.x = 0x3C0;
    a1.y = 0x1B0;
    a2.x = 0x3D0;
    a2.y = 0x1B0;
    LoadImage(&b0, D_80076F80);
    DrawSync(0);
    MoveImage(&a1, b1.x, b1.y);
    DrawSync(0);
    MoveImage(&a2, b2.x, b2.y);
    DrawSync(0);
}

void func_800CBB4C(s32 arg0) {
    u8 buf[16];
    RECT *r;
    Character *c;
    s32 x;
    s32 color;
    s16 maxmp;

    c = g_gameState->unk1C->party[arg0];
    r = &D_800D5438.unk8[arg0].unk4;
    x = r->x + 8;
    if (!c->unk34.b.unk9) {
        x = r->x + 0x16;
    }
    func_800283E0(x, r->y + 1, 0x14, arg0, 0x2D, c->hp == 0);
    text_drawBlock(r->x + 0x3C, r->y + 3, 0x14, c->unk0, 0);
    func_80026FC4(r->x + 0x86, r->y + 4, 0x14, c->unk38, 0);
    text_formatNumber(c->level, buf, 2, 1);
    ui_drawSprite(r->x + 0x3C, r->y + 0x12, 0x14, 0xB4, 0);
    ui_drawSmallDigits(r->x + 0x60, r->y + 0x12, 0x14, buf, 0);
    ui_drawSprite(r->x + 0x3C, r->y + 0x1B, 0x14, 0xAF, 0);
    text_formatNumber(c->hp, buf, 4, 1);
    buf[4] = 0x2F;
    if (c->hp != 0) {
        color = ((u16)(c->maxHp / 6) >= c->hp) * 4;
    } else {
        color = 3;
    }
    text_formatNumber(c->maxHp, &buf[5], -1, 1);
    ui_drawSmallDigits(r->x + 0x52, r->y + 0x1B, 0x14, buf, color);
    ui_drawSprite(r->x + 0x3C, r->y + 0x24, 0x14, 0xB0, 0);
    text_formatNumber(c->mp, buf, 3, 1);
    buf[3] = 0x2F;
    maxmp = c->maxMp;
    color = ((s16)(maxmp / 6) >= c->mp) * 4;
    text_formatNumber(maxmp, &buf[4], -1, 1);
    ui_drawSmallDigits(r->x + 0x59, r->y + 0x24, 0x14, buf, color);
    func_800273C0(r->x + 0x9B, r->y + 0x14, 0x14, c->stones, c->maxStones);
}

void func_800CBDF8(RECT *r, s32 otz) {
    TileWords *p;

    setlen(g_primPtr, 3);
    p = (TileWords *)g_primPtr;
    p->rgbc = 0x62000000;
    p->xy = ((r->x + 8) & 0xFFFF) | (r->y << 16);
    p->wh = 0x30002E;
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[otz], p);
    SetDrawTPage(g_primPtr, 1, 1, 0);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

void func_800CBF18(void) {
    u8 buf[16];
    s32 v;

    v = game_getPlayTime();
    ui_drawSprite(D_800CE04C->x + 6, D_800CE04C->y + 8, 0x13, 0xB1, 0);
    ui_drawSprite(D_800CE04C->x + 6, D_800CE04C->y + 0x15, 0x13, 0xB2, 0);
    ui_drawSmallDigits(D_800CE04C->x + 0x13, D_800CE04C->y + 9, 0x13, buf, func_8002AA04(v, buf, 1));
    text_formatNumber(g_gameState->unk1C->gil, buf, 7, 1);
    strcat(buf, D_800CE050);
    ui_drawSmallDigits(D_800CE04C->x + 0x13, D_800CE04C->y + 0x16, 0x13, buf, 0);
    ui_drawLabel(D_800CE04C->x + 6, D_800CE04C->y - 3, 0x13, D_800704C0, 0);
    func_8002DEE4(D_800CE04C, 0x14, 2);
}

void func_800CC050(void) {
    GameData *gd;

    ui_drawLabel(D_800CE054->x + 8, D_800CE054->y - 3, 0x13, D_8007046C, 0);
    func_8002DEE4(D_800CE054, 0x14, 2);
    func_8002705C(D_800CE054, 0x14, 2);
    gd = g_gameState->unk1C;
    if (gd->unk80C != NULL && *gd->unk80C != 0xFF) {
        text_drawBlock(D_800CE054->x + 8, D_800CE054->y + 7, 0x13, gd->unk80C, 0);
    } else {
        text_drawBlock(D_800CE054->x + 8, D_800CE054->y + 7, 0x13, text_get(1), 0);
    }
}

void func_800CC13C(void) {
    u8 blink;

    blink = (D_80077E54 & 3) != 0;
    help_setDrawHand(D_800D5438.unk1 - 1U >= 2);
    if (!func_8002F810()) {
        if (D_800D5438.unk1 == 0 || blink) {
            g_help.x = 0xE0;
            g_help.otz = 1;
            g_help.y = D_800D5438.unk0 * 0xF + 0x11;
            func_800355E4();
        }
        if (D_800D5438.unk1 == 1) {
            g_help.otz = 0xC;
            g_help.x = D_800D5438.unk8[0].unk4.x - 0xE;
            g_help.y = D_80077E53 * 0x2E + 0x20;
            func_800355E4();
        }
        if (D_800D5438.unk1 == 2) {
            if (D_800D5438.unk4 == 1) {
                g_help.otz = 0xB;
                g_help.x = D_800D5438.unk8[0].unk4.x - 0xA;
                g_help.y = D_800D5438.unk3 * 0x2E + 0x24;
                func_800355E4();
                if (blink) {
                    ui_drawHand(D_800D5438.unk8[0].unk4.x - 0xE, D_800D5438.unk2 * 0x2E + 0x20, 0xC);
                }
            } else if (D_800D5438.unk4 == 0) {
                g_help.otz = 0xC;
                g_help.x = D_800D5438.unk8[0].unk4.x - 0xE;
                g_help.y = D_800D5438.unk2 * 0x2E + 0x20;
                func_800355E4();
            }
        }
    }
}

void func_800CC324(void) {
    g_help.text = 0;
    if (D_800D5438.unk1 == 0) {
        g_help.text = (u8 *)func_8003631C(D_800D5438.unk0);
    } else if (D_800D5438.unk1 == 1) {
        if (D_800D5438.unk0 + 1 == 3) {
            func_800CC5D8(D_800D5438.unk0);
        } else {
            s32 a = D_800D5438.unk0 + 2;
            if (D_800D5438.unk0 + 1 == 4) {
                func_800CC3E0(a);
            } else {
                g_help.text = text_get(a);
            }
        }
    } else if (D_800D5438.unk1 == 2) {
        g_help.text = text_get(D_800D5438.unk4 + 0xB);
    }
}

void func_800CC3E0(s32 arg0) {
    Character *c;
    u32 next;

    c = g_gameState->unk1C->party[D_80077E53];
    if (c != NULL) {
        if (c->level >= 99) {
            next = c->exp;
        } else {
            next = g_levelExp[c->level];
        }
        func_8002BE44(0, c->unk0);
        text_formatNumber(c->exp, D_800D5418, 7, 1);
        text_formatNumber(next - c->exp, D_800D5428, 7, 1);
        func_8002BE44(1, D_800CE06C);
        func_8002BE44(2, D_800D5418);
        func_8002BE44(3, D_800CE070);
        func_8002BE44(4, D_800CE078);
        func_8002BE44(5, D_800D5428);
        func_8002BE44(6, D_800CE080);
        func_8002BE44(7, c->unk38 != 0 ? D_800CE090 : D_800CE088);
        D_80077DF0 = text_get(5);
    }
}

void func_800CC54C(void) {
    if (D_800D5438.unk0 + 1 != 5) {
        if (func_80036300(D_800D5438.unk0 + 1) == 0) {
            func_800361CC(D_800D5438.unk0 + 1);
        } else {
            D_800D5438.unk1 = 1;
        }
    } else {
        D_800D5438.unk1 = 2;
        D_800D5438.unk4 = 0;
    }
    snd_playSe(0x67);
}

void func_800CC5CC(void) {
    D_80077E50.unk1 = 0;
}

void func_800CC5D8(u8 arg0) {
    s32 i;
    s32 j;
    Character *c;
    s32 id;

    i = 0;
    j = 5;
    c = g_gameState->unk1C->party[D_80077E53];
    do {
        func_8002BE44(i, D_800CE058[i]);
        id = c->equip[i];
        if (id != 0xFF) {
            func_8002BE44(j, D_8006A218 + D_80069018[id].unk0);
        } else {
            func_8002BE44(j, text_get(0xD));
        }
        j++;
        func_8002BE74(i, id);
        i++;
    } while (i < 5);
    g_help.text = text_get(4);
}

void func_800CC6E4(void) {
    RECT r;

    if (D_80077E50.unk1 != 0) {
        D_800D5438.unk0 = 0;
        D_800D5438.unk2 = 0;
    }
    D_800D5438.unk1 = 0;
    text_setBank(g_worldMapText);
    func_800CB434();
    r.x = 8;
    r.y = 8;
    r.w = 0xE8;
    r.h = g_gameState->unkC->disp.h - 0xF;
    help_setBounds(&r);
    help_setHandler(func_800CC324);
    help_setDrawHand(1);
    if (g_gameState->unk1C->party[D_80077E50.unk3] == NULL) {
        D_80077E50.unk3 = party_getFirstMember();
    }
}

void func_800CC7C0(void) {
    func_800CC82C();
    func_800CC890();
    func_800CC8C8();
}

void func_800CC7F0(void) {
    help_setDrawHand(1);
    help_setBounds(0);
    help_setHandler(0);
}

void func_800CC820(u8 arg0) {
    D_800D5438.unk0 = arg0;
}

void func_800CC82C(void) {
    if (D_80077E50.unk1 == 0 && func_8002F810() == 0) {
        if (D_800CDFC0[D_800D5438.unk1] != NULL) {
            D_800CDFC0[D_800D5438.unk1]();
        }
    }
}

void func_800CC890(void) {
    func_800CBF18();
    func_800CC050();
    func_800CC940();
    func_800CC13C();
}

void func_800CC8C8(void) {
    s32 i;

    if (D_80077E50.unk1 != 0) {
        for (i = 0; i < 6; i++) {
            if (D_800D5438.unk8[i].unkC != 0) {
                func_800631AC(&D_800D5438.unk8[i]);
            }
        }
    }
}

void func_800CC940(void) {
    RECT r;
    s16 y;
    s32 i;
    RECT *p;

    i = 0;
    do {
        p = &D_800D5438.unk8[i].unk4;
        r.x = p->x + 0x82;
        y = p->y;
        r.w = 0x72;
        r.h = 0x12;
        r.y = y;
        if (g_gameState->unk1C->party[i] != NULL) {
            func_800CBB4C(i);
        }
        func_800CBDF8(p, 0x14);
        func_8002DEE4(&r, 0x14, 0);
        func_8002DEE4(p, 0x14, 0);
        func_8002705C(p, 0x15, 0);
        i++;
    } while (i < 4);
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800CB434", g_worldMapText);
