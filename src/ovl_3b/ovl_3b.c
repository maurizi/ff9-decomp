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
    Cursor *cur;
    u8 savedIdx;

    if ((g_padPressed & PAD_LEFT) && !(s_menuParty.cursor[s_menuParty.mode].idx & (PARTY_RESERVE_COLS - 1))) {
        s_menuParty.cursor[s_menuParty.mode].row = PARTY_ROW_PARTY;
        savedIdx = s_menuParty.partyIdx;
        s_menuParty.reserveIdx = s_menuParty.cursor[s_menuParty.mode].idx;
        s_menuParty.cursor[s_menuParty.mode].idx = savedIdx;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_UP) && s_menuParty.cursor[s_menuParty.mode].idx >= PARTY_RESERVE_COLS) {
        s_menuParty.cursor[s_menuParty.mode].idx -= PARTY_RESERVE_COLS;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_DOWN) && s_menuParty.cursor[s_menuParty.mode].idx < PARTY_RESERVE_COLS) {
        s_menuParty.cursor[s_menuParty.mode].idx += PARTY_RESERVE_COLS;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_RIGHT) && (s_menuParty.cursor[s_menuParty.mode].idx & (PARTY_RESERVE_COLS - 1)) != PARTY_RESERVE_COLS - 1) {
        s_menuParty.cursor[s_menuParty.mode].idx++;
        snd_playSe(SE_CURSOR);
    } else if ((g_padRepeat & PAD_LEFT) && (s_menuParty.cursor[s_menuParty.mode].idx & (PARTY_RESERVE_COLS - 1))) {
        s_menuParty.cursor[s_menuParty.mode].idx--;
        snd_playSe(SE_CURSOR);
    }
}

void menuParty_pick(void) {
    s32 x;
    s32 y;
    s32 sx;
    s32 sy;
    u8 member;

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
            member = s_menuParty.slots[y][x];
            s_menuParty.slots[y][x] = s_menuParty.slots[sy][sx];
            s_menuParty.slots[sy][sx] = member;
            s_menuParty.cursor[0] = s_menuParty.cursor[1];
            s_menuParty.mode = PARTY_SEL_PICK;
            snd_playSe(SE_CURSOR);
        }
    }
}

void menuParty_drawParty(void) {
    s32 i;
    s32 id;
    s32 textId;
    u8 *label;

    ui_drawLabel(s_menuPartyListRect.x + PARTY_LIST_LABEL_X, s_menuPartyListRect.y + WIN_TITLE_Y, PARTY_OTZ_LIST, g_labelParty, UI_LABEL_NORMAL);
    for (i = 0; i < PARTY_SIZE - 1; i++) {
        ui_drawEtchedLine(s_menuPartyListRect.x + 4, s_menuPartyListRect.y + PARTY_DIV_Y + i * PARTY_ROW_H, PARTY_OTZ_LIST, s_menuPartyListRect.w - 9);
    }
    for (i = 0; i < PARTY_SIZE; i++) {
        id = s_menuParty.slots[0][i];
        if (id != PARTY_EMPTY) {
            menuParty_drawMember(s_menuPartyListRect.x + PARTY_ROW_X, s_menuPartyListRect.y + PARTY_ROW_Y + i * PARTY_ROW_H, PARTY_OTZ_TEXT, id);
        } else {
            textId = PARTY_TEXT_UNUSED;
            if (i < s_menuParty.size) {
                textId = i + PARTY_TEXT_SLOT1;
            }
            label = text_get(textId);
            text_drawBlock(s_menuPartyListRect.x + ((PARTY_EMPTY_W - text_measureWidth((s32)label)) >> 1), s_menuPartyListRect.y + PARTY_EMPTY_Y + i * PARTY_ROW_H, PARTY_OTZ_TEXT, label,
                          i >= s_menuParty.size ? TEXT_PAL_GREY : TEXT_PAL_WHITE);
        }
    }
    win_drawBox(&s_menuPartyListRect, PARTY_OTZ_LIST, WIN_STYLE_BEVEL);
    win_drawShadow(&s_menuPartyListRect, PARTY_OTZ_LIST, WIN_STYLE_BEVEL);
}

void menuParty_drawInfo(void) {
    s32 i;
    s32 id;
    s32 dx;
    s32 dy;

    ui_drawLabel(s_menuPartyInfoRect.x + PARTY_RULE_LABEL_X, s_menuPartyInfoRect.y + WIN_TITLE_Y, PARTY_OTZ_LIST, g_labelInfo, UI_LABEL_NORMAL);
    ui_drawEtchedLine(s_menuPartyInfoRect.x + 4, s_menuPartyInfoRect.y + PARTY_DIV_Y, PARTY_OTZ_BOX, s_menuPartyInfoRect.w - 8);
    win_drawBox(&s_menuPartyInfoRect, PARTY_OTZ_BOX, WIN_STYLE_LINE_RULE);
    win_drawShadow(&s_menuPartyInfoRect, PARTY_OTZ_SHADOW, WIN_STYLE_LINE_RULE);
    id = s_menuParty.slots[s_menuParty.cursor[s_menuParty.mode].row][s_menuParty.cursor[s_menuParty.mode].idx];
    if (id != PARTY_EMPTY) {
        menuParty_drawMember(s_menuPartyInfoRect.x + PARTY_INFO_MEMBER_X, s_menuPartyInfoRect.y + PARTY_ROW_Y, PARTY_OTZ_TEXT, id);
    }
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuParty.slots[1]); i++) {
        id = s_menuParty.slots[1][i];
        if (id != PARTY_EMPTY) {
            dx = (i & (PARTY_RESERVE_COLS - 1)) * PARTY_RESERVE_COL_W + PARTY_RESERVE_X;
            dy = (i >> PARTY_RESERVE_COLS_SHIFT) * PARTY_RESERVE_ROW_H + PARTY_RESERVE_Y;
            menuParty_drawPortrait(s_menuPartyInfoRect.x + dx, s_menuPartyInfoRect.y + dy, PARTY_OTZ_TEXT, id, PARTY_PORTRAIT_H,
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
        g_menuHelp.otz = PARTY_OTZ_CURSOR;
        if (s_menuParty.cursor[0].row == PARTY_ROW_PARTY) {
            g_menuHelp.x = PARTY_CURSOR_X;
            g_menuHelp.y = pos * PARTY_ROW_H + PARTY_CURSOR_Y;
        } else {
            g_menuHelp.x = (pos & (PARTY_RESERVE_COLS - 1)) * PARTY_RESERVE_COL_W + PARTY_CURSOR_RESERVE_X;
            g_menuHelp.y = (pos >> PARTY_RESERVE_COLS_SHIFT) * PARTY_RESERVE_ROW_H + PARTY_CURSOR_RESERVE_Y;
        }
        help_drawCursor();
    } else if (s_menuParty.mode == PARTY_SEL_SWAP) {
        pos = s_menuParty.cursor[1].idx;
        g_menuHelp.otz = PARTY_OTZ_CURSOR;
        if (s_menuParty.cursor[1].row == PARTY_ROW_PARTY) {
            g_menuHelp.x = PARTY_CURSOR_X + PARTY_SWAP_CURSOR_D;
            g_menuHelp.y = pos * PARTY_ROW_H + (PARTY_CURSOR_Y + PARTY_SWAP_CURSOR_D);
        } else {
            g_menuHelp.x = (pos & (PARTY_RESERVE_COLS - 1)) * PARTY_RESERVE_COL_W + (PARTY_CURSOR_RESERVE_X + PARTY_SWAP_CURSOR_D);
            g_menuHelp.y = (pos >> PARTY_RESERVE_COLS_SHIFT) * PARTY_RESERVE_ROW_H + (PARTY_CURSOR_RESERVE_Y + PARTY_SWAP_CURSOR_D);
        }
        help_drawCursor();
        if (g_menuFrame & 3) {
            pos = s_menuParty.cursor[0].idx;
            if (s_menuParty.cursor[0].row == PARTY_ROW_PARTY) {
                x = PARTY_CURSOR_X;
                y = pos * PARTY_ROW_H + PARTY_CURSOR_Y;
            } else {
                x = (pos & (PARTY_RESERVE_COLS - 1)) * PARTY_RESERVE_COL_W + PARTY_CURSOR_RESERVE_X;
                y = (pos >> PARTY_RESERVE_COLS_SHIFT) * PARTY_RESERVE_ROW_H + PARTY_CURSOR_RESERVE_Y;
            }
            ui_drawHand(x, y, PARTY_OTZ_CURSOR);
        }
    }
}

/* text bank for this menu (count + {offset, info} table, then strings) */
INCLUDE_RODATA("asm/us/ovl_3b/nonmatchings/ovl_3b", g_partyMenuText);

void menuParty_init(void) {
    text_setBank(g_partyMenuText);
    help_setCallback(menuParty_updateHelp);
    ui_loadPortraits(s_menuPartyImage, PARTY_PORTRAIT_COUNT);
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
    win_drawBox(&s_menuPartyTitleRect, PARTY_OTZ_BOX, WIN_STYLE_BEVEL);
    win_drawShadow(&s_menuPartyTitleRect, PARTY_OTZ_BOX, WIN_STYLE_BEVEL);
    text_drawBlock(s_menuPartyTitleRect.x + PARTY_TITLE_X, s_menuPartyTitleRect.y + PARTY_TEXT_Y, PARTY_OTZ_TEXT, text_get(PARTY_TEXT_TITLE), TEXT_PAL_WHITE);
}

void menuParty_drawHelp(void) {
    ui_drawLabel(s_menuPartyHelpRect.x + PARTY_RULE_LABEL_X, s_menuPartyHelpRect.y + WIN_TITLE_Y, PARTY_OTZ_BOX, g_labelHelp, UI_LABEL_NORMAL);
    win_drawBox(&s_menuPartyHelpRect, PARTY_OTZ_BOX, WIN_STYLE_LINE_RULE);
    win_drawShadow(&s_menuPartyHelpRect, PARTY_OTZ_SHADOW, WIN_STYLE_LINE_RULE);
    text_drawBlock(s_menuPartyHelpRect.x + PARTY_HELP_TEXT_X, s_menuPartyHelpRect.y + PARTY_TEXT_Y, PARTY_OTZ_TEXT, text_get(s_menuParty.size), TEXT_PAL_WHITE);
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
        menuParty_drawPortrait(x, y, otz, id, PARTY_PORTRAIT_H, s_menuParty.fixed[id]);
        ui_drawCharStatus(x + PARTY_STATUS_DX, y + PARTY_STATUS_DY, otz, &g_gameState->gameData->characters[id]);
    }
}

void menuParty_drawPortrait(s32 x, s32 y, s32 otz, s32 id, s32 h, u8 flag) {
    Icon *icon;
    s32 yy;
    s32 base;
    Character *chr;

    base = y + h;
    yy = base - PARTY_ICON_Y0;
    if (id != PARTY_EMPTY) {
        chr = &g_gameState->gameData->characters[id];
        if (chr->status & CHR_STATUS_VENOM) {
            icon = &g_venomStatusSprite;
            ui_drawSpriteDef(x + ((PARTY_PORTRAIT_W - icon->w) >> 1), yy, otz, icon, 0);
            yy = base - (PARTY_ICON_Y0 + PARTY_ICON_STEP);
        }
        if (chr->status & CHR_STATUS_PETRIFY) {
            icon = &g_petrifyStatusSprite;
            ui_drawSpriteDef(x + ((PARTY_PORTRAIT_W - icon->w) >> 1), yy, otz, icon, 0);
            yy -= PARTY_ICON_STEP;
        }
        if (chr->hp == 0) {
            icon = &g_koStatusSprite;
            ui_drawSpriteDef(x + ((PARTY_PORTRAIT_W - icon->w) >> 1), yy, otz, icon, 0);
        }
        ui_drawPortraitId(x, y, otz, chr_getPortrait(id), h, flag);
    }
}

void menuParty_loadRequest(PartyRequest *req) {
    s32 i;

    s_menuParty.size = req->size;
    for (i = 0; i < PARTY_SIZE; i++) {
        s_menuParty.slots[0][i] = req->party[i];
    }
    for (i = 0; i < (s32)ARRAY_COUNT(req->reserve); i++) {
        s_menuParty.slots[1][i] = req->reserve[i];
    }
    for (i = 0; i < (s32)ARRAY_COUNT(req->fixed); i++) {
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
    s32 memberCount;
    u32 alive;
    u8 id;

    alive = 0;
    memberCount = 0;
    for (i = 0; i < PARTY_SIZE; i++) {
        id = s_menuParty.slots[0][i];
        if (id != PARTY_EMPTY) {
            memberCount++;
            if (g_gameState->gameData->characters[id].hp != 0 && !(g_gameState->gameData->characters[id].status & (CHR_STATUS_PETRIFY | CHR_STATUS_VENOM))) {
                alive++;
            }
        }
    }
    return memberCount >= s_menuParty.size ? alive > 0 : 0;
}

void menuParty_updateHelp(void) {
    g_menuHelp.text = text_get(s_menuParty.mode + PARTY_TEXT_MOVE_FROM);
}

INCLUDE_RODATA("asm/us/ovl_3b/nonmatchings/ovl_3b", s_menuPartyImage);
