#include "ovl_3b.h"

void func_801F37A8(void) {
    if ((g_padRepeat & PAD_UP) && D_801F4748.unk4[D_801F4748.unk0].unk0 != 0) {
        D_801F4748.unk4[D_801F4748.unk0].unk0--;
        snd_playSe(0x67);
    } else if ((g_padRepeat & PAD_DOWN) && D_801F4748.unk4[D_801F4748.unk0].unk0 < D_801F4748.unk1 - 1) {
        D_801F4748.unk4[D_801F4748.unk0].unk0++;
        snd_playSe(0x67);
    } else if (g_padPressed & PAD_RIGHT) {
        if (D_801F4748.unk0 == 1 && func_801F4614(0)) {
            return;
        }
        D_801F4748.unk4[D_801F4748.unk0].unk1 = 1;
        D_801F4748.unk8 = D_801F4748.unk4[D_801F4748.unk0].unk0;
        D_801F4748.unk4[D_801F4748.unk0].unk0 = D_801F4748.unk9;
        snd_playSe(0x67);
    }
}

void func_801F38D8(void) {
    Cursor *c;
    u8 v;

    if ((g_padPressed & PAD_LEFT) && !(D_801F4748.unk4[D_801F4748.unk0].unk0 & 3)) {
        D_801F4748.unk4[D_801F4748.unk0].unk1 = 0;
        v = D_801F4748.unk8;
        D_801F4748.unk9 = D_801F4748.unk4[D_801F4748.unk0].unk0;
        D_801F4748.unk4[D_801F4748.unk0].unk0 = v;
        snd_playSe(0x67);
    } else if ((g_padRepeat & PAD_UP) && D_801F4748.unk4[D_801F4748.unk0].unk0 >= 4) {
        D_801F4748.unk4[D_801F4748.unk0].unk0 -= 4;
        snd_playSe(0x67);
    } else if ((g_padRepeat & PAD_DOWN) && D_801F4748.unk4[D_801F4748.unk0].unk0 < 4) {
        D_801F4748.unk4[D_801F4748.unk0].unk0 += 4;
        snd_playSe(0x67);
    } else if ((g_padRepeat & PAD_RIGHT) && (D_801F4748.unk4[D_801F4748.unk0].unk0 & 3) != 3) {
        D_801F4748.unk4[D_801F4748.unk0].unk0++;
        snd_playSe(0x67);
    } else if ((g_padRepeat & PAD_LEFT) && (D_801F4748.unk4[D_801F4748.unk0].unk0 & 3)) {
        D_801F4748.unk4[D_801F4748.unk0].unk0--;
        snd_playSe(0x67);
    }
}

void func_801F3A60(void) {
    s32 x;
    s32 y;
    s32 sx;
    s32 sy;
    u8 t;

    if (D_801F4748.unk0 == 0) {
        D_801F4748.unk0 = 1;
        D_801F4748.unk4[1] = D_801F4748.unk4[0];
        snd_playSe(0x67);
    } else {
        x = D_801F4748.unk4[0].unk0;
        sx = D_801F4748.unk4[1].unk0;
        y = D_801F4748.unk4[0].unk1;
        sy = D_801F4748.unk4[1].unk1;
        if (func_801F4614(1) && y != sy) {
            snd_playSe(0x66);
        } else {
            t = D_801F4748.unkA[y][x];
            D_801F4748.unkA[y][x] = D_801F4748.unkA[sy][sx];
            D_801F4748.unkA[sy][sx] = t;
            D_801F4748.unk4[0] = D_801F4748.unk4[1];
            D_801F4748.unk0 = 0;
            snd_playSe(0x67);
        }
    }
}

void func_801F3B3C(void) {
    s32 i;
    s32 id;
    s32 t;
    u8 *s;

    ui_drawLabel(D_801F4738.x + 8, D_801F4738.y - 3, 7, D_80070494, 0);
    for (i = 0; i < 3; i++) {
        ui_drawEtchedLine(D_801F4738.x + 4, D_801F4738.y + 0x35 + i * 0x33, 7, D_801F4738.w - 9);
    }
    for (i = 0; i < 4; i++) {
        id = D_801F4748.unkA[0][i];
        if (id != 0xFF) {
            func_801F4360(D_801F4738.x + 8, D_801F4738.y + 6 + i * 0x33, 6, id);
        } else {
            t = 9;
            if (i < D_801F4748.unk1) {
                t = i + 5;
            }
            s = text_get(t);
            text_drawBlock(D_801F4738.x + ((0x8D - func_8002BA68((s32)s)) >> 1), D_801F4738.y + 0x15 + i * 0x33, 6, s,
                          i >= D_801F4748.unk1 ? 2 : 0);
        }
    }
    win_drawBox(&D_801F4738, 7, 2);
    win_drawShadow(&D_801F4738, 7, 2);
}

void func_801F3CF0(void) {
    s32 i;
    s32 id;
    s32 dx;
    s32 dy;

    ui_drawLabel(D_801F4740.x + 0xC, D_801F4740.y - 3, 7, D_80070460, 0);
    ui_drawEtchedLine(D_801F4740.x + 4, D_801F4740.y + 0x35, 8, D_801F4740.w - 8);
    win_drawBox(&D_801F4740, 8, 1);
    win_drawShadow(&D_801F4740, 9, 1);
    id = D_801F4748.unkA[D_801F4748.unk4[D_801F4748.unk0].unk1][D_801F4748.unk4[D_801F4748.unk0].unk0];
    if (id != 0xFF) {
        func_801F4360(D_801F4740.x + 0x10, D_801F4740.y + 6, 6, id);
    }
    for (i = 0; i < 8; i++) {
        id = D_801F4748.unkA[1][i];
        if (id != 0xFF) {
            dx = (i & 3) * 0x25 + 0x10;
            dy = (i >> 2) * 0x30 + 0x3A;
            func_801F43FC(D_801F4740.x + dx, D_801F4740.y + dy, 6, id, 0x2D,
                          D_801F4748.unk1A[id]);
        }
    }
}

void func_801F3E58(void) {
    s32 x;
    s32 y;
    s32 pos;

    if (D_801F4748.unk0 == 0) {
        pos = D_801F4748.unk4[0].unk0;
        g_menuHelp.otz = 4;
        if (D_801F4748.unk4[0].unk1 == 0) {
            g_menuHelp.x = 0;
            g_menuHelp.y = pos * 0x33 + 0x20;
        } else {
            g_menuHelp.x = (pos & 3) * 0x25 + 0x8C;
            g_menuHelp.y = (pos >> 2) * 0x30 + 0x86;
        }
        func_800355E4();
    } else if (D_801F4748.unk0 == 1) {
        pos = D_801F4748.unk4[1].unk0;
        g_menuHelp.otz = 4;
        if (D_801F4748.unk4[1].unk1 == 0) {
            g_menuHelp.x = 4;
            g_menuHelp.y = pos * 0x33 + 0x24;
        } else {
            g_menuHelp.x = (pos & 3) * 0x25 + 0x90;
            g_menuHelp.y = (pos >> 2) * 0x30 + 0x8A;
        }
        func_800355E4();
        if (D_80077DC0 & 3) {
            pos = D_801F4748.unk4[0].unk0;
            if (D_801F4748.unk4[0].unk1 == 0) {
                x = 0;
                y = pos * 0x33 + 0x20;
            } else {
                x = (pos & 3) * 0x25 + 0x8C;
                y = (pos >> 2) * 0x30 + 0x86;
            }
            ui_drawHand(x, y, 4);
        }
    }
}

/* text bank for this menu (count + {offset, info} table, then strings) */
INCLUDE_RODATA("asm/us/ovl_3b/nonmatchings/ovl_3b", g_partyMenuText);

void func_801F3FEC(void) {
    text_setBank(g_partyMenuText);
    help_setCallback(func_801F46FC);
    ui_loadPortraits(D_801EDFE8, 0xD);
    D_801F4748.unk0 = 0;
    D_801F4748.unk4[1].unk0 = 0;
    D_801F4748.unk4[0].unk0 = 0;
    D_801F4748.unk4[1].unk1 = 0;
    D_801F4748.unk4[0].unk1 = 0;
    D_801F4748.unk9 = 0;
    D_801F4748.unk8 = 0;
    func_801F457C((PartyRequest *)D_80077DBC);
}

void func_801F405C(void) {
    func_801F40AC();
    func_801F4150();
}

void func_801F4084(void) {
    help_setCallback(NULL);
    ui_restorePortraits();
}

void func_801F40AC(void) {
    if (!func_8002F810()) {
        if (g_padPressed & BTN_CIRCLE) {
            func_801F42A4();
            return;
        }
        if (g_padPressed & BTN_CROSS) {
            func_801F3A60();
            return;
        }
        if (D_801F4748.unk4[D_801F4748.unk0].unk1 == 0) {
            func_801F37A8();
            return;
        }
        func_801F38D8();
    }
}

void func_801F4150(void) {
    func_801F4190();
    func_801F4204();
    func_801F3B3C();
    func_801F3CF0();
    func_801F3E58();
}

void func_801F4190(void) {
    win_drawBox(&D_801F4728, 8, 2);
    win_drawShadow(&D_801F4728, 8, 2);
    text_drawBlock(D_801F4728.x + 9, D_801F4728.y + 6, 6, text_get(0), 0);
}

void func_801F4204(void) {
    ui_drawLabel(D_801F4730.x + 0xC, D_801F4730.y - 3, 8, D_80070450, 0);
    win_drawBox(&D_801F4730, 8, 1);
    win_drawShadow(&D_801F4730, 9, 1);
    text_drawBlock(D_801F4730.x + 0x12, D_801F4730.y + 6, 6, text_get(D_801F4748.unk1), 0);
}

void func_801F42A4(void) {
    s32 i;

    if (D_801F4748.unk0 == 1) {
        D_801F4748.unk0 = 0;
        snd_playSe(0x65);
    } else if (!func_801F465C()) {
        snd_playSe(0x66);
    } else {
        for (i = 0; i < D_801F4748.unk1; i++) {
            party_setMember(i, D_801F4748.unkA[0][i] != 0xFF ? D_801F4748.unkA[0][i] : -1);
        }
        func_800325E0();
        snd_playSe(0x67);
    }
}

void func_801F4360(s32 x, s32 y, s32 otz, s32 id) {
    if (id != 0xFF) {
        func_801F43FC(x, y, otz, id, 0x2D, D_801F4748.unk1A[id]);
        ui_drawCharStatus(x + 0x26, y + 3, otz, &g_gameState->unk1C->characters[id]);
    }
}

void func_801F43FC(s32 x, s32 y, s32 otz, s32 id, s32 h, u8 arg5) {
    Icon *icon;
    s32 yy;
    s32 base;
    Character *c;

    base = y + h;
    yy = base - 8;
    if (id != 0xFF) {
        c = &g_gameState->unk1C->characters[id];
        if (c->unk38 & 2) {
            icon = &D_800687C0;
            ui_drawSpriteDef(x + ((0x20 - icon->unk4) >> 1), yy, otz, icon, 0);
            yy = base - 0xF;
        }
        if (c->unk38 & 1) {
            icon = &D_80068E38;
            ui_drawSpriteDef(x + ((0x20 - icon->unk4) >> 1), yy, otz, icon, 0);
            yy -= 7;
        }
        if (c->hp == 0) {
            icon = &D_80068E30;
            ui_drawSpriteDef(x + ((0x20 - icon->unk4) >> 1), yy, otz, icon, 0);
        }
        ui_drawPortraitId(x, y, otz, chr_getPortrait(id), h, arg5);
    }
}

void func_801F457C(PartyRequest *req) {
    s32 i;

    D_801F4748.unk1 = req->unk0;
    for (i = 0; i < 4; i++) {
        D_801F4748.unkA[0][i] = req->unk4[i];
    }
    for (i = 0; i < 8; i++) {
        D_801F4748.unkA[1][i] = req->unk8[i];
    }
    for (i = 0; i < 8; i++) {
        D_801F4748.unk1A[i] = req->unk10[i];
    }
}

u8 func_801F4614(s32 i) {
    u8 id;

    id = D_801F4748.unkA[D_801F4748.unk4[i].unk1][D_801F4748.unk4[i].unk0];
    if (id == 0xFF) {
        return 0;
    }
    return D_801F4748.unk1A[id];
}

u8 func_801F465C(void) {
    s32 i;
    s32 n;
    u32 alive;
    u8 id;

    alive = 0;
    n = 0;
    for (i = 0; i < 4; i++) {
        id = D_801F4748.unkA[0][i];
        if (id != 0xFF) {
            n++;
            if (g_gameState->unk1C->characters[id].hp != 0 && !(g_gameState->unk1C->characters[id].unk38 & 3)) {
                alive++;
            }
        }
    }
    return n >= D_801F4748.unk1 ? alive > 0 : 0;
}

void func_801F46FC(void) {
    g_menuHelp.text = text_get(D_801F4748.unk0 + 0xA);
}

INCLUDE_RODATA("asm/us/ovl_3b/nonmatchings/ovl_3b", D_801EDFE8);
