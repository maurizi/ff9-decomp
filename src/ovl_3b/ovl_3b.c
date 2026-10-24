#include "ovl_3b.h"

void menu_party_moveInParty(void) {
    if ((g_padRepeat & PAD_UP) && g_menuParty.cursor[g_menuParty.mode].idx != 0) {
        g_menuParty.cursor[g_menuParty.mode].idx--;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_DOWN) && g_menuParty.cursor[g_menuParty.mode].idx < g_menuParty.size - 1) {
        g_menuParty.cursor[g_menuParty.mode].idx++;
        snd_playSe(SE_CURSOR);
    } else if (g_padPressed & PAD_RIGHT) {
        if (g_menuParty.mode == PARTY_SEL_SWAP && menu_party_isFixed(0)) {
            return;
        }
        g_menuParty.cursor[g_menuParty.mode].row = PARTY_ROW_RESERVE;
        g_menuParty.partyIdx = g_menuParty.cursor[g_menuParty.mode].idx;
        g_menuParty.cursor[g_menuParty.mode].idx = g_menuParty.reserveIdx;
        snd_playSe(SE_CURSOR);
    }
}

void menu_party_moveInReserve(void) {
    Cursor *c;
    u8 v;

    if ((g_padPressed & PAD_LEFT) && !(g_menuParty.cursor[g_menuParty.mode].idx & 3)) {
        g_menuParty.cursor[g_menuParty.mode].row = PARTY_ROW_PARTY;
        v = g_menuParty.partyIdx;
        g_menuParty.reserveIdx = g_menuParty.cursor[g_menuParty.mode].idx;
        g_menuParty.cursor[g_menuParty.mode].idx = v;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_UP) && g_menuParty.cursor[g_menuParty.mode].idx >= 4) {
        g_menuParty.cursor[g_menuParty.mode].idx -= 4;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_DOWN) && g_menuParty.cursor[g_menuParty.mode].idx < 4) {
        g_menuParty.cursor[g_menuParty.mode].idx += 4;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_RIGHT) && (g_menuParty.cursor[g_menuParty.mode].idx & 3) != 3) {
        g_menuParty.cursor[g_menuParty.mode].idx++;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_LEFT) && (g_menuParty.cursor[g_menuParty.mode].idx & 3)) {
        g_menuParty.cursor[g_menuParty.mode].idx--;
        snd_playSe(SE_CURSOR);
    }
}

void menu_party_select(void) {
    s32 x;
    s32 y;
    s32 sx;
    s32 sy;
    u8 t;

    if (g_menuParty.mode == PARTY_SEL_PICK) {
        g_menuParty.mode = PARTY_SEL_SWAP;
        g_menuParty.cursor[1] = g_menuParty.cursor[0];
        snd_playSe(SE_CURSOR);
    } else {
        x = g_menuParty.cursor[0].idx;
        sx = g_menuParty.cursor[1].idx;
        y = g_menuParty.cursor[0].row;
        sy = g_menuParty.cursor[1].row;
        if (menu_party_isFixed(1) && y != sy) {
            snd_playSe(SE_BUZZER);
        } else {
            t = g_menuParty.slots[y][x];
            g_menuParty.slots[y][x] = g_menuParty.slots[sy][sx];
            g_menuParty.slots[sy][sx] = t;
            g_menuParty.cursor[0] = g_menuParty.cursor[1];
            g_menuParty.mode = PARTY_SEL_PICK;
            snd_playSe(SE_CURSOR);
        }
    }
}

void menu_party_drawParty(void) {
    s32 i;
    s32 id;
    s32 t;
    u8 *s;

    ui_drawLabel(g_menuPartyListRect.x + 8, g_menuPartyListRect.y - 3, 7, D_80070494, 0);
    for (i = 0; i < 3; i++) {
        func_800269F4(g_menuPartyListRect.x + 4, g_menuPartyListRect.y + 0x35 + i * 0x33, 7, g_menuPartyListRect.w - 9);
    }
    for (i = 0; i < 4; i++) {
        id = g_menuParty.slots[0][i];
        if (id != PARTY_EMPTY) {
            menu_party_drawMember(g_menuPartyListRect.x + 8, g_menuPartyListRect.y + 6 + i * 0x33, 6, id);
        } else {
            t = PARTY_TEXT_UNUSED;
            if (i < g_menuParty.size) {
                t = i + PARTY_TEXT_SLOT1;
            }
            s = text_get(t);
            text_drawBlock(g_menuPartyListRect.x + ((0x8D - func_8002BA68((s32)s)) >> 1), g_menuPartyListRect.y + 0x15 + i * 0x33, 6, s,
                          i >= g_menuParty.size ? 2 : 0);
        }
    }
    func_8002DEE4(&g_menuPartyListRect, 7, 2);
    func_8002705C(&g_menuPartyListRect, 7, 2);
}

void menu_party_drawInfo(void) {
    s32 i;
    s32 id;
    s32 dx;
    s32 dy;

    ui_drawLabel(g_menuPartyInfoRect.x + 0xC, g_menuPartyInfoRect.y - 3, 7, D_80070460, 0);
    func_800269F4(g_menuPartyInfoRect.x + 4, g_menuPartyInfoRect.y + 0x35, 8, g_menuPartyInfoRect.w - 8);
    func_8002DEE4(&g_menuPartyInfoRect, 8, 1);
    func_8002705C(&g_menuPartyInfoRect, 9, 1);
    id = g_menuParty.slots[g_menuParty.cursor[g_menuParty.mode].row][g_menuParty.cursor[g_menuParty.mode].idx];
    if (id != PARTY_EMPTY) {
        menu_party_drawMember(g_menuPartyInfoRect.x + 0x10, g_menuPartyInfoRect.y + 6, 6, id);
    }
    for (i = 0; i < 8; i++) {
        id = g_menuParty.slots[1][i];
        if (id != PARTY_EMPTY) {
            dx = (i & 3) * 0x25 + 0x10;
            dy = (i >> 2) * 0x30 + 0x3A;
            menu_party_drawPortrait(g_menuPartyInfoRect.x + dx, g_menuPartyInfoRect.y + dy, 6, id, 0x2D,
                          g_menuParty.fixed[id]);
        }
    }
}

void menu_party_drawCursor(void) {
    s32 x;
    s32 y;
    s32 pos;

    if (g_menuParty.mode == PARTY_SEL_PICK) {
        pos = g_menuParty.cursor[0].idx;
        D_80077DE8.unk4 = 4;
        if (g_menuParty.cursor[0].row == PARTY_ROW_PARTY) {
            D_80077DE8.unk0 = 0;
            D_80077DE8.unk2 = pos * 0x33 + 0x20;
        } else {
            D_80077DE8.unk0 = (pos & 3) * 0x25 + 0x8C;
            D_80077DE8.unk2 = (pos >> 2) * 0x30 + 0x86;
        }
        func_800355E4();
    } else if (g_menuParty.mode == PARTY_SEL_SWAP) {
        pos = g_menuParty.cursor[1].idx;
        D_80077DE8.unk4 = 4;
        if (g_menuParty.cursor[1].row == PARTY_ROW_PARTY) {
            D_80077DE8.unk0 = 4;
            D_80077DE8.unk2 = pos * 0x33 + 0x24;
        } else {
            D_80077DE8.unk0 = (pos & 3) * 0x25 + 0x90;
            D_80077DE8.unk2 = (pos >> 2) * 0x30 + 0x8A;
        }
        func_800355E4();
        if (D_80077DC0 & 3) {
            pos = g_menuParty.cursor[0].idx;
            if (g_menuParty.cursor[0].row == PARTY_ROW_PARTY) {
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

void menu_party_init(void) {
    text_setBank(g_partyMenuText);
    func_8003556C(menu_party_setHelpText);
    func_80028720(g_menuPartyImage, 0xD);
    g_menuParty.mode = PARTY_SEL_PICK;
    g_menuParty.cursor[1].idx = 0;
    g_menuParty.cursor[0].idx = 0;
    g_menuParty.cursor[1].row = PARTY_ROW_PARTY;
    g_menuParty.cursor[0].row = PARTY_ROW_PARTY;
    g_menuParty.reserveIdx = 0;
    g_menuParty.partyIdx = 0;
    menu_party_loadRequest((PartyRequest *)D_80077DBC);
}

void menu_party_update(void) {
    menu_party_input();
    menu_party_draw();
}

void menu_party_exit(void) {
    func_8003556C(NULL);
    func_8002886C();
}

void menu_party_input(void) {
    if (!func_8002F810()) {
        if (g_padPressed & BTN_CIRCLE) {
            menu_party_back();
            return;
        }
        if (g_padPressed & BTN_CROSS) {
            menu_party_select();
            return;
        }
        if (g_menuParty.cursor[g_menuParty.mode].row == PARTY_ROW_PARTY) {
            menu_party_moveInParty();
            return;
        }
        menu_party_moveInReserve();
    }
}

void menu_party_draw(void) {
    menu_party_drawTitle();
    menu_party_drawHelp();
    menu_party_drawParty();
    menu_party_drawInfo();
    menu_party_drawCursor();
}

void menu_party_drawTitle(void) {
    func_8002DEE4(&g_menuPartyTitleRect, 8, 2);
    func_8002705C(&g_menuPartyTitleRect, 8, 2);
    text_drawBlock(g_menuPartyTitleRect.x + 9, g_menuPartyTitleRect.y + 6, 6, text_get(PARTY_TEXT_TITLE), 0);
}

void menu_party_drawHelp(void) {
    ui_drawLabel(g_menuPartyHelpRect.x + 0xC, g_menuPartyHelpRect.y - 3, 8, D_80070450, 0);
    func_8002DEE4(&g_menuPartyHelpRect, 8, 1);
    func_8002705C(&g_menuPartyHelpRect, 9, 1);
    text_drawBlock(g_menuPartyHelpRect.x + 0x12, g_menuPartyHelpRect.y + 6, 6, text_get(g_menuParty.size), 0);
}

void menu_party_back(void) {
    s32 i;

    if (g_menuParty.mode == PARTY_SEL_SWAP) {
        g_menuParty.mode = PARTY_SEL_PICK;
        snd_playSe(SE_CANCEL);
    } else if (!menu_party_isValid()) {
        snd_playSe(SE_BUZZER);
    } else {
        for (i = 0; i < g_menuParty.size; i++) {
            party_setMember(i, g_menuParty.slots[0][i] != PARTY_EMPTY ? g_menuParty.slots[0][i] : -1);
        }
        func_800325E0();
        snd_playSe(SE_CURSOR);
    }
}

void menu_party_drawMember(s32 x, s32 y, s32 otz, s32 id) {
    if (id != PARTY_EMPTY) {
        menu_party_drawPortrait(x, y, otz, id, 0x2D, g_menuParty.fixed[id]);
        func_8002423C(x + 0x26, y + 3, otz, &g_gameState->unk1C->characters[id]);
    }
}

void menu_party_drawPortrait(s32 x, s32 y, s32 otz, s32 id, s32 h, u8 arg5) {
    Icon *icon;
    s32 yy;
    s32 base;
    Character *c;

    base = y + h;
    yy = base - 8;
    if (id != PARTY_EMPTY) {
        c = &g_gameState->unk1C->characters[id];
        if (c->unk38 & 2) {
            icon = &D_800687C0;
            func_80027710(x + ((0x20 - icon->unk4) >> 1), yy, otz, icon, 0);
            yy = base - 0xF;
        }
        if (c->unk38 & 1) {
            icon = &D_80068E38;
            func_80027710(x + ((0x20 - icon->unk4) >> 1), yy, otz, icon, 0);
            yy -= 7;
        }
        if (c->hp == 0) {
            icon = &D_80068E30;
            func_80027710(x + ((0x20 - icon->unk4) >> 1), yy, otz, icon, 0);
        }
        func_80028204(x, y, otz, func_80028954(id), h, arg5);
    }
}

void menu_party_loadRequest(PartyRequest *req) {
    s32 i;

    g_menuParty.size = req->size;
    for (i = 0; i < 4; i++) {
        g_menuParty.slots[0][i] = req->party[i];
    }
    for (i = 0; i < 8; i++) {
        g_menuParty.slots[1][i] = req->reserve[i];
    }
    for (i = 0; i < 8; i++) {
        g_menuParty.fixed[i] = req->fixed[i];
    }
}

u8 menu_party_isFixed(s32 i) {
    u8 id;

    id = g_menuParty.slots[g_menuParty.cursor[i].row][g_menuParty.cursor[i].idx];
    if (id == PARTY_EMPTY) {
        return 0;
    }
    return g_menuParty.fixed[id];
}

u8 menu_party_isValid(void) {
    s32 i;
    s32 n;
    u32 alive;
    u8 id;

    alive = 0;
    n = 0;
    for (i = 0; i < 4; i++) {
        id = g_menuParty.slots[0][i];
        if (id != PARTY_EMPTY) {
            n++;
            if (g_gameState->unk1C->characters[id].hp != 0 && !(g_gameState->unk1C->characters[id].unk38 & 3)) {
                alive++;
            }
        }
    }
    return n >= g_menuParty.size ? alive > 0 : 0;
}

void menu_party_setHelpText(void) {
    D_80077DE8.unk8 = text_get(g_menuParty.mode + PARTY_TEXT_MOVE_FROM);
}

INCLUDE_RODATA("asm/us/ovl_3b/nonmatchings/ovl_3b", g_menuPartyImage);
