#include "ovl_3b.h"

void menuParty_inputParty(void) {
    if ((g_padRepeat & PAD_UP) && s_menuParty.cursor[s_menuParty.mode].idx != 0) {
        s_menuParty.cursor[s_menuParty.mode].idx--;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_DOWN) && s_menuParty.cursor[s_menuParty.mode].idx < s_menuParty.size - 1) {
        s_menuParty.cursor[s_menuParty.mode].idx++;
        snd_playSe(SE_CURSOR);
    } else if (g_padPressed & PAD_RIGHT) {
        if (s_menuParty.mode == PARTY_SEL_SWAP && menuParty_isFixed(0)) {
            return;
        }
        s_menuParty.cursor[s_menuParty.mode].row = PARTY_ROW_RESERVE;
        s_menuParty.partyIdx = s_menuParty.cursor[s_menuParty.mode].idx;
        s_menuParty.cursor[s_menuParty.mode].idx = s_menuParty.reserveIdx;
        snd_playSe(SE_CURSOR);
    }
}

void menuParty_inputReserve(void) {
    Cursor *c;
    u8 v;

    if ((g_padPressed & PAD_LEFT) && !(s_menuParty.cursor[s_menuParty.mode].idx & 3)) {
        s_menuParty.cursor[s_menuParty.mode].row = PARTY_ROW_PARTY;
        v = s_menuParty.partyIdx;
        s_menuParty.reserveIdx = s_menuParty.cursor[s_menuParty.mode].idx;
        s_menuParty.cursor[s_menuParty.mode].idx = v;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_UP) && s_menuParty.cursor[s_menuParty.mode].idx >= 4) {
        s_menuParty.cursor[s_menuParty.mode].idx -= 4;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_DOWN) && s_menuParty.cursor[s_menuParty.mode].idx < 4) {
        s_menuParty.cursor[s_menuParty.mode].idx += 4;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_RIGHT) && (s_menuParty.cursor[s_menuParty.mode].idx & 3) != 3) {
        s_menuParty.cursor[s_menuParty.mode].idx++;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_LEFT) && (s_menuParty.cursor[s_menuParty.mode].idx & 3)) {
        s_menuParty.cursor[s_menuParty.mode].idx--;
        snd_playSe(SE_CURSOR);
    }
}

void menuParty_pick(void) {
    s32 x;
    s32 y;
    s32 sx;
    s32 sy;
    u8 t;

    if (s_menuParty.mode == PARTY_SEL_PICK) {
        s_menuParty.mode = PARTY_SEL_SWAP;
        s_menuParty.cursor[1] = s_menuParty.cursor[0];
        snd_playSe(SE_CURSOR);
    } else {
        x = s_menuParty.cursor[0].idx;
        sx = s_menuParty.cursor[1].idx;
        y = s_menuParty.cursor[0].row;
        sy = s_menuParty.cursor[1].row;
        if (menuParty_isFixed(1) && y != sy) {
            snd_playSe(SE_BUZZER);
        } else {
            t = s_menuParty.slots[y][x];
            s_menuParty.slots[y][x] = s_menuParty.slots[sy][sx];
            s_menuParty.slots[sy][sx] = t;
            s_menuParty.cursor[0] = s_menuParty.cursor[1];
            s_menuParty.mode = PARTY_SEL_PICK;
            snd_playSe(SE_CURSOR);
        }
    }
}

void menuParty_drawParty(void) {
    s32 i;
    s32 id;
    s32 t;
    u8 *s;

    ui_drawLabel(s_menuPartyListRect.x + 8, s_menuPartyListRect.y - 3, 7, D_80070494, 0);
    for (i = 0; i < 3; i++) {
        ui_drawEtchedLine(s_menuPartyListRect.x + 4, s_menuPartyListRect.y + 0x35 + i * 0x33, 7, s_menuPartyListRect.w - 9);
    }
    for (i = 0; i < 4; i++) {
        id = s_menuParty.slots[0][i];
        if (id != PARTY_EMPTY) {
            menuParty_drawMember(s_menuPartyListRect.x + 8, s_menuPartyListRect.y + 6 + i * 0x33, 6, id);
        } else {
            t = PARTY_TEXT_UNUSED;
            if (i < s_menuParty.size) {
                t = i + PARTY_TEXT_SLOT1;
            }
            s = text_get(t);
            text_drawBlock(s_menuPartyListRect.x + ((0x8D - text_measureWidth((s32)s)) >> 1), s_menuPartyListRect.y + 0x15 + i * 0x33, 6, s,
                          i >= s_menuParty.size ? 2 : 0);
        }
    }
    win_drawBox(&s_menuPartyListRect, 7, 2);
    win_drawShadow(&s_menuPartyListRect, 7, 2);
}

void menuParty_drawInfo(void) {
    s32 i;
    s32 id;
    s32 dx;
    s32 dy;

    ui_drawLabel(s_menuPartyInfoRect.x + 0xC, s_menuPartyInfoRect.y - 3, 7, D_80070460, 0);
    ui_drawEtchedLine(s_menuPartyInfoRect.x + 4, s_menuPartyInfoRect.y + 0x35, 8, s_menuPartyInfoRect.w - 8);
    win_drawBox(&s_menuPartyInfoRect, 8, 1);
    win_drawShadow(&s_menuPartyInfoRect, 9, 1);
    id = s_menuParty.slots[s_menuParty.cursor[s_menuParty.mode].row][s_menuParty.cursor[s_menuParty.mode].idx];
    if (id != PARTY_EMPTY) {
        menuParty_drawMember(s_menuPartyInfoRect.x + 0x10, s_menuPartyInfoRect.y + 6, 6, id);
    }
    for (i = 0; i < 8; i++) {
        id = s_menuParty.slots[1][i];
        if (id != PARTY_EMPTY) {
            dx = (i & 3) * 0x25 + 0x10;
            dy = (i >> 2) * 0x30 + 0x3A;
            menuParty_drawPortrait(s_menuPartyInfoRect.x + dx, s_menuPartyInfoRect.y + dy, 6, id, 0x2D,
                          s_menuParty.fixed[id]);
        }
    }
}

void menuParty_drawCursor(void) {
    s32 x;
    s32 y;
    s32 pos;

    if (s_menuParty.mode == PARTY_SEL_PICK) {
        pos = s_menuParty.cursor[0].idx;
        g_menuHelp.otz = 4;
        if (s_menuParty.cursor[0].row == PARTY_ROW_PARTY) {
            g_menuHelp.x = 0;
            g_menuHelp.y = pos * 0x33 + 0x20;
        } else {
            g_menuHelp.x = (pos & 3) * 0x25 + 0x8C;
            g_menuHelp.y = (pos >> 2) * 0x30 + 0x86;
        }
        help_drawCursor();
    } else if (s_menuParty.mode == PARTY_SEL_SWAP) {
        pos = s_menuParty.cursor[1].idx;
        g_menuHelp.otz = 4;
        if (s_menuParty.cursor[1].row == PARTY_ROW_PARTY) {
            g_menuHelp.x = 4;
            g_menuHelp.y = pos * 0x33 + 0x24;
        } else {
            g_menuHelp.x = (pos & 3) * 0x25 + 0x90;
            g_menuHelp.y = (pos >> 2) * 0x30 + 0x8A;
        }
        help_drawCursor();
        if (g_menuFrame & 3) {
            pos = s_menuParty.cursor[0].idx;
            if (s_menuParty.cursor[0].row == PARTY_ROW_PARTY) {
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

void menuParty_init(void) {
    text_setBank(g_partyMenuText);
    help_setCallback(menuParty_updateHelp);
    ui_loadPortraits(s_menuPartyImage, 0xD);
    s_menuParty.mode = PARTY_SEL_PICK;
    s_menuParty.cursor[1].idx = 0;
    s_menuParty.cursor[0].idx = 0;
    s_menuParty.cursor[1].row = PARTY_ROW_PARTY;
    s_menuParty.cursor[0].row = PARTY_ROW_PARTY;
    s_menuParty.reserveIdx = 0;
    s_menuParty.partyIdx = 0;
    menuParty_loadRequest((PartyRequest *)g_menuArg);
}

void menuParty_update(void) {
    menuParty_input();
    menuParty_draw();
}

void menuParty_exit(void) {
    help_setCallback(NULL);
    ui_restorePortraits();
}

void menuParty_input(void) {
    if (!fade_isBusy()) {
        if (g_padPressed & BTN_CIRCLE) {
            menuParty_confirm();
            return;
        }
        if (g_padPressed & BTN_CROSS) {
            menuParty_pick();
            return;
        }
        if (s_menuParty.cursor[s_menuParty.mode].row == PARTY_ROW_PARTY) {
            menuParty_inputParty();
            return;
        }
        menuParty_inputReserve();
    }
}

void menuParty_draw(void) {
    menuParty_drawTitle();
    menuParty_drawHelp();
    menuParty_drawParty();
    menuParty_drawInfo();
    menuParty_drawCursor();
}

void menuParty_drawTitle(void) {
    win_drawBox(&s_menuPartyTitleRect, 8, 2);
    win_drawShadow(&s_menuPartyTitleRect, 8, 2);
    text_drawBlock(s_menuPartyTitleRect.x + 9, s_menuPartyTitleRect.y + 6, 6, text_get(PARTY_TEXT_TITLE), 0);
}

void menuParty_drawHelp(void) {
    ui_drawLabel(s_menuPartyHelpRect.x + 0xC, s_menuPartyHelpRect.y - 3, 8, D_80070450, 0);
    win_drawBox(&s_menuPartyHelpRect, 8, 1);
    win_drawShadow(&s_menuPartyHelpRect, 9, 1);
    text_drawBlock(s_menuPartyHelpRect.x + 0x12, s_menuPartyHelpRect.y + 6, 6, text_get(s_menuParty.size), 0);
}

void menuParty_confirm(void) {
    s32 i;

    if (s_menuParty.mode == PARTY_SEL_SWAP) {
        s_menuParty.mode = PARTY_SEL_PICK;
        snd_playSe(SE_CANCEL);
    } else if (!menuParty_isValid()) {
        snd_playSe(SE_BUZZER);
    } else {
        for (i = 0; i < s_menuParty.size; i++) {
            party_setMember(i, s_menuParty.slots[0][i] != PARTY_EMPTY ? s_menuParty.slots[0][i] : -1);
        }
        menu_close();
        snd_playSe(SE_CURSOR);
    }
}

void menuParty_drawMember(s32 x, s32 y, s32 otz, s32 id) {
    if (id != PARTY_EMPTY) {
        menuParty_drawPortrait(x, y, otz, id, 0x2D, s_menuParty.fixed[id]);
        ui_drawCharStatus(x + 0x26, y + 3, otz, &g_gameState->unk1C->characters[id]);
    }
}

void menuParty_drawPortrait(s32 x, s32 y, s32 otz, s32 id, s32 h, u8 arg5) {
    Icon *icon;
    s32 yy;
    s32 base;
    Character *c;

    base = y + h;
    yy = base - 8;
    if (id != PARTY_EMPTY) {
        c = &g_gameState->unk1C->characters[id];
        if (c->status & 2) {
            icon = &D_800687C0;
            ui_drawSpriteDef(x + ((0x20 - icon->unk4) >> 1), yy, otz, icon, 0);
            yy = base - 0xF;
        }
        if (c->status & 1) {
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

void menuParty_loadRequest(PartyRequest *req) {
    s32 i;

    s_menuParty.size = req->size;
    for (i = 0; i < 4; i++) {
        s_menuParty.slots[0][i] = req->party[i];
    }
    for (i = 0; i < 8; i++) {
        s_menuParty.slots[1][i] = req->reserve[i];
    }
    for (i = 0; i < 8; i++) {
        s_menuParty.fixed[i] = req->fixed[i];
    }
}

u8 menuParty_isFixed(s32 i) {
    u8 id;

    id = s_menuParty.slots[s_menuParty.cursor[i].row][s_menuParty.cursor[i].idx];
    if (id == PARTY_EMPTY) {
        return 0;
    }
    return s_menuParty.fixed[id];
}

u8 menuParty_isValid(void) {
    s32 i;
    s32 n;
    u32 alive;
    u8 id;

    alive = 0;
    n = 0;
    for (i = 0; i < 4; i++) {
        id = s_menuParty.slots[0][i];
        if (id != PARTY_EMPTY) {
            n++;
            if (g_gameState->unk1C->characters[id].hp != 0 && !(g_gameState->unk1C->characters[id].status & 3)) {
                alive++;
            }
        }
    }
    return n >= s_menuParty.size ? alive > 0 : 0;
}

void menuParty_updateHelp(void) {
    g_menuHelp.text = text_get(s_menuParty.mode + PARTY_TEXT_MOVE_FROM);
}

INCLUDE_RODATA("asm/us/ovl_3b/nonmatchings/ovl_3b", s_menuPartyImage);
