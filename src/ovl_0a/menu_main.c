#include "ovl_0a.h"

void menuMain_initPanels(void) {
    u8 *s;
    u8 *p;
    s32 w;
    s32 i;

    p = g_gameState->gameData->locationName;
    if (p != NULL && *p != 0xFF) {
        s = p;
    } else {
        s = text_get(1);
    }
    w = text_measureStyle(s, 0) + 0x10;
    if (w < 0x44) {
        w = 0x44;
    }
    s_menuMainPanelRects[5].w = w;
    s_menuMainPanelRects[5].x = 0x134 - w;
    for (i = 0; i < 6; i++) {
        s_menuMain.panels[i].rect = s_menuMainPanelRects[i];
        tween_start(&s_menuMain.panels[i], NULL, 0);
    }
    if (g_menuScreen.slideIn != 0) {
        tween_start(&s_menuMain.panels[0], &s_menuMainSlideIn[0], 1);
        tween_start(&s_menuMain.panels[1], &s_menuMainSlideIn[1], 1);
        tween_start(&s_menuMain.panels[2], &s_menuMainSlideIn[2], 1);
        tween_start(&s_menuMain.panels[3], &s_menuMainSlideIn[3], 1);
    }
}

void menuMain_inputCommand(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuScreen_close();
    } else if (g_padRepeat & BTN_CROSS) {
        menuMain_selectCommand();
    } else if (g_padRepeat & PAD_UP) {
        snd_playSe(SE_CURSOR);
        s_menuMain.cursor = s_menuMain.cursor != 0 ? s_menuMain.cursor - 1 : 6;
    } else if (g_padRepeat & PAD_DOWN) {
        snd_playSe(SE_CURSOR);
        s_menuMain.cursor = s_menuMain.cursor < 6 ? s_menuMain.cursor + 1 : 0;
    }
}

void menuMain_inputMember(void) {
    MenuScreen *m;
    s32 old;
    s32 new;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        s_menuMain.state = MENUMAIN_COMMAND;
        return;
    } else if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        menuScreen_open(s_menuMain.cursor + 1);
        return;
    } else if (g_padRepeat & PAD_UP) {
        m = &g_menuScreen;
        old = m->member;
        new = party_getPrevMember(old);
    } else if (g_padRepeat & PAD_DOWN) {
        m = &g_menuScreen;
        old = m->member;
        new = party_getNextMember(old);
    } else {
        return;
    }
    if (old != new) {
        snd_playSe(SE_CURSOR);
        old = new;
        m->member = old;
    }
}

void menuMain_inputOrder(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        if (s_menuMain.orderPicked == 0) {
            s_menuMain.state = MENUMAIN_COMMAND;
        } else {
            s_menuMain.orderPicked = 0;
        }
    } else if (g_padRepeat & PAD_UP) {
        snd_playSe(SE_CURSOR);
        if (s_menuMain.orderPicked == 0) {
            s_menuMain.orderCursor = s_menuMain.orderCursor != 0 ? s_menuMain.orderCursor - 1 : 3;
        } else {
            s_menuMain.orderTarget = s_menuMain.orderTarget != 0 ? s_menuMain.orderTarget - 1 : 3;
        }
    } else if (g_padRepeat & PAD_DOWN) {
        snd_playSe(SE_CURSOR);
        if (s_menuMain.orderPicked == 0) {
            s_menuMain.orderCursor = s_menuMain.orderCursor < 3 ? s_menuMain.orderCursor + 1 : 0;
        } else {
            s_menuMain.orderTarget = s_menuMain.orderTarget < 3 ? s_menuMain.orderTarget + 1 : 0;
        }
    } else if (g_padRepeat & BTN_CROSS) {
        if (s_menuMain.orderPicked == 0) {
            snd_playSe(SE_CURSOR);
            s_menuMain.orderPicked = 1;
            s_menuMain.orderTarget = s_menuMain.orderCursor;
        } else {
            snd_playSe(SE_CURSOR);
            menuMain_reorder(s_menuMain.orderCursor, s_menuMain.orderTarget);
            s_menuMain.orderPicked = 0;
            s_menuMain.orderCursor = s_menuMain.orderTarget;
        }
    }
}

void menuMain_reorder(s32 from, s32 to) {
    Character *c;
    Character *t;

    if (from == to) {
        c = g_gameState->gameData->party[to];
        if (c != NULL) {
            c->info.b.battleRow ^= 1;
        }
    } else {
        t = g_gameState->gameData->party[s_menuMain.orderCursor];
        g_gameState->gameData->party[s_menuMain.orderCursor] = g_gameState->gameData->party[s_menuMain.orderTarget];
        g_gameState->gameData->party[s_menuMain.orderTarget] = t;
        menuMain_swapPortraits(from, to);
        if (g_gameState->gameData->party[g_menuScreen.member] == NULL) {
            g_menuScreen.member = party_getFirstMember();
        }
    }
}

void menuMain_swapPortraits(s32 slotA, s32 slotB) {
    RECT a0;
    RECT a1;
    RECT a2;
    RECT b0;
    RECT b1;
    RECT b2;

    ui_getPartyPortraitRects(slotA, &a0, &a1, &a2);
    ui_getPartyPortraitRects(slotB, &b0, &b1, &b2);
    StoreImage(&a0, g_portraitSavedVram);
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
    LoadImage(&b0, g_portraitSavedVram);
    DrawSync(0);
    MoveImage(&a1, b1.x, b1.y);
    DrawSync(0);
    MoveImage(&a2, b2.x, b2.y);
    DrawSync(0);
}

void menuMain_drawMember(s32 slot) {
    u8 numText[16];
    RECT *r;
    Character *c;
    s32 x;
    s32 color;
    s16 maxmp;

    c = g_gameState->gameData->party[slot];
    r = &s_menuMain.panels[slot].rect;
    x = r->x + 8;
    if (!c->info.b.battleRow) {
        x = r->x + 0x16;
    }
    ui_drawPortrait(x, r->y + 1, 0x14, slot, 0x2D, c->hp == 0);
    text_drawBlock(r->x + 0x3C, r->y + 3, 0x14, c->name, TEXT_PAL_WHITE);
    ui_drawStatusIcons(r->x + 0x86, r->y + 4, 0x14, c->status, 0);
    text_formatNumber(c->level, numText, 2, TEXT_NUM_ASCII);
    ui_drawSprite(r->x + 0x3C, r->y + 0x12, 0x14, 0xB4, 0);
    ui_drawSmallDigits(r->x + 0x60, r->y + 0x12, 0x14, numText, TEXT_PAL_WHITE);
    ui_drawSprite(r->x + 0x3C, r->y + 0x1B, 0x14, 0xAF, 0);
    text_formatNumber(c->hp, numText, 4, TEXT_NUM_ASCII);
    numText[4] = 0x2F;
    if (c->hp != 0) {
        color = ((u16)(c->maxHp / 6) >= c->hp) * TEXT_PAL_YELLOW;
    } else {
        color = TEXT_PAL_RED;
    }
    text_formatNumber(c->maxHp, &numText[5], TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
    ui_drawSmallDigits(r->x + 0x52, r->y + 0x1B, 0x14, numText, color);
    ui_drawSprite(r->x + 0x3C, r->y + 0x24, 0x14, 0xB0, 0);
    text_formatNumber(c->mp, numText, 3, TEXT_NUM_ASCII);
    numText[3] = 0x2F;
    maxmp = c->maxMp;
    color = ((s16)(maxmp / 6) >= c->mp) * TEXT_PAL_YELLOW;
    text_formatNumber(maxmp, &numText[4], TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
    ui_drawSmallDigits(r->x + 0x59, r->y + 0x24, 0x14, numText, color);
    ui_drawStones(r->x + 0x9B, r->y + 0x14, 0x14, c->stones, c->maxStones);
}

void menuMain_drawPortraitShade(RECT *r, s32 otz) {
    TileWords *p;

    setlen(g_primPtr, PRIM_LEN(TILE));
    p = (TileWords *)g_primPtr;
    p->rgbc = GPU_CODE(GPU_TILE | GPU_ABE);
    p->xy = ((r->x + 8) & 0xFFFF) | (r->y << 16);
    p->wh = 0x30002E;
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[otz], p);
    SetDrawTPage(g_primPtr, 1, 1, 0);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

void menuMain_drawTimeGil(void) {
    u8 numText[16];
    s32 v;

    v = game_getPlayTime();
    ui_drawSprite(s_menuMainTimeGilRect->x + 6, s_menuMainTimeGilRect->y + 8, 0x13, 0xB1, 0);
    ui_drawSprite(s_menuMainTimeGilRect->x + 6, s_menuMainTimeGilRect->y + 0x15, 0x13, 0xB2, 0);
    ui_drawSmallDigits(s_menuMainTimeGilRect->x + 0x13, s_menuMainTimeGilRect->y + 9, 0x13, numText, text_formatTime(v, numText, 1));
    text_formatNumber(g_gameState->gameData->gil, numText, 7, TEXT_NUM_ASCII);
    strcat(numText, s_menuMainGilSuffix);
    ui_drawSmallDigits(s_menuMainTimeGilRect->x + 0x13, s_menuMainTimeGilRect->y + 0x16, 0x13, numText, TEXT_PAL_WHITE);
    ui_drawLabel(s_menuMainTimeGilRect->x + 6, s_menuMainTimeGilRect->y - 3, 0x13, g_labelTimeAndGil, UI_LABEL_NORMAL);
    win_drawBox(s_menuMainTimeGilRect, 0x14, WIN_STYLE_BEVEL);
}

void menuMain_drawLocation(void) {
    GameData *gd;

    ui_drawLabel(s_menuMainLocationRect->x + 8, s_menuMainLocationRect->y - 3, 0x13, g_labelLocation, UI_LABEL_NORMAL);
    win_drawBox(s_menuMainLocationRect, 0x14, WIN_STYLE_BEVEL);
    win_drawShadow(s_menuMainLocationRect, 0x14, WIN_STYLE_BEVEL);
    gd = g_gameState->gameData;
    if (gd->locationName != NULL && *gd->locationName != TEXT_END) {
        text_drawBlock(s_menuMainLocationRect->x + 8, s_menuMainLocationRect->y + 7, 0x13, gd->locationName, TEXT_PAL_WHITE);
    } else {
        text_drawBlock(s_menuMainLocationRect->x + 8, s_menuMainLocationRect->y + 7, 0x13, text_get(1), TEXT_PAL_WHITE);
    }
}

void menuMain_drawCursor(void) {
    u8 blink;

    blink = (g_menuScreenFrame & 3) != 0;
    help_setCursorOnTop(s_menuMain.state - 1U >= 2);
    if (!fade_isBusy()) {
        if (s_menuMain.state == MENUMAIN_COMMAND || blink) {
            g_menuHelp.x = 0xE0;
            g_menuHelp.otz = 1;
            g_menuHelp.y = s_menuMain.cursor * 0xF + 0x11;
            help_drawCursor();
        }
        if (s_menuMain.state == MENUMAIN_MEMBER) {
            g_menuHelp.otz = 0xC;
            g_menuHelp.x = s_menuMain.panels[0].rect.x - 0xE;
            g_menuHelp.y = g_menuScreenMember * 0x2E + 0x20;
            help_drawCursor();
        }
        if (s_menuMain.state == MENUMAIN_ORDER) {
            if (s_menuMain.orderPicked == 1) {
                g_menuHelp.otz = 0xB;
                g_menuHelp.x = s_menuMain.panels[0].rect.x - 0xA;
                g_menuHelp.y = s_menuMain.orderTarget * 0x2E + 0x24;
                help_drawCursor();
                if (blink) {
                    ui_drawHand(s_menuMain.panels[0].rect.x - 0xE, s_menuMain.orderCursor * 0x2E + 0x20, 0xC);
                }
            } else if (s_menuMain.orderPicked == 0) {
                g_menuHelp.otz = 0xC;
                g_menuHelp.x = s_menuMain.panels[0].rect.x - 0xE;
                g_menuHelp.y = s_menuMain.orderCursor * 0x2E + 0x20;
                help_drawCursor();
            }
        }
    }
}

void menuMain_updateHelp(void) {
    g_menuHelp.text = 0;
    if (s_menuMain.state == MENUMAIN_COMMAND) {
        g_menuHelp.text = (u8 *)menuScreen_getCommandHelp(s_menuMain.cursor);
    } else if (s_menuMain.state == MENUMAIN_MEMBER) {
        if (s_menuMain.cursor + 1 == MENU_SCREEN_EQUIP) {
            menuMain_setEquipHelp(s_menuMain.cursor);
        } else {
            s32 a = s_menuMain.cursor + 2;
            if (s_menuMain.cursor + 1 == MENU_SCREEN_STATUS) {
                menuMain_setExpHelp(a);
            } else {
                g_menuHelp.text = text_get(a);
            }
        }
    } else if (s_menuMain.state == MENUMAIN_ORDER) {
        g_menuHelp.text = text_get(s_menuMain.orderPicked + 0xB);
    }
}

void menuMain_setExpHelp(s32 unused) {
    Character *c;
    u32 next;

    c = g_gameState->gameData->party[g_menuScreenMember];
    if (c != NULL) {
        if (c->level >= 99) {
            next = c->exp;
        } else {
            next = g_levelExp[c->level];
        }
        text_setStringArg(0, c->name);
        text_formatNumber(c->exp, s_menuMainExpText, 7, TEXT_NUM_ASCII);
        text_formatNumber(next - c->exp, s_menuMainNextText, 7, TEXT_NUM_ASCII);
        text_setStringArg(1, s_menuMainExpLabel);
        text_setStringArg(2, s_menuMainExpText);
        text_setStringArg(3, s_menuMainNextLabel);
        text_setStringArg(4, s_menuMainUntilLabel);
        text_setStringArg(5, s_menuMainNextText);
        text_setStringArg(6, s_menuMainStatusLabel);
        text_setStringArg(7, c->status != 0 ? s_menuMainStatusAbnormal : s_menuMainStatusNormal);
        g_menuHelpText = text_get(5);
    }
}

void menuMain_selectCommand(void) {
    if (s_menuMain.cursor + 1 != MENU_SCREEN_ORDER) {
        if (menuScreen_needsMember(s_menuMain.cursor + 1) == 0) {
            menuScreen_open(s_menuMain.cursor + 1);
        } else {
            s_menuMain.state = MENUMAIN_MEMBER;
        }
    } else {
        s_menuMain.state = MENUMAIN_ORDER;
        s_menuMain.orderPicked = 0;
    }
    snd_playSe(SE_CURSOR);
}

void menuMain_onSlideInDone(void) {
    g_menuScreen.slideIn = 0;
}

void menuMain_setEquipHelp(u8 unused) {
    s32 i;
    s32 j;
    Character *c;
    s32 id;

    i = 0;
    j = 5;
    c = g_gameState->gameData->party[g_menuScreenMember];
    do {
        text_setStringArg(i, s_menuMainEquipLabels[i]);
        id = c->equip[i];
        if (id != 0xFF) {
            text_setStringArg(j, g_itemNames + g_itemTable[id].name);
        } else {
            text_setStringArg(j, text_get(0xD));
        }
        j++;
        text_setIconArg(i, id);
        i++;
    } while (i < 5);
    g_menuHelp.text = text_get(4);
}

void menuMain_init(void) {
    RECT r;

    if (g_menuScreen.slideIn != 0) {
        s_menuMain.cursor = 0;
        s_menuMain.orderCursor = 0;
    }
    s_menuMain.state = MENUMAIN_COMMAND;
    text_setBank(s_menuMainText);
    menuMain_initPanels();
    r.x = 8;
    r.y = 8;
    r.w = 0xE8;
    r.h = g_gameState->dispEnv->disp.h - 0xF;
    help_setBounds(&r);
    help_setCallback(menuMain_updateHelp);
    help_setCursorOnTop(1);
    if (g_gameState->gameData->party[g_menuScreen.member] == NULL) {
        g_menuScreen.member = party_getFirstMember();
    }
}

void menuMain_update(void) {
    menuMain_input();
    menuMain_draw();
    menuMain_updatePanels();
}

void menuMain_exit(void) {
    help_setCursorOnTop(1);
    help_setBounds(NULL);
    help_setCallback(NULL);
}

void menuMain_setCursor(u8 cursor) {
    s_menuMain.cursor = cursor;
}

void menuMain_input(void) {
    if (g_menuScreen.slideIn == 0 && fade_isBusy() == 0) {
        if (s_menuMainInput[s_menuMain.state] != NULL) {
            s_menuMainInput[s_menuMain.state]();
        }
    }
}

void menuMain_draw(void) {
    menuMain_drawTimeGil();
    menuMain_drawLocation();
    menuMain_drawParty();
    menuMain_drawCursor();
}

void menuMain_updatePanels(void) {
    s32 i;

    if (g_menuScreen.slideIn != 0) {
        for (i = 0; i < 6; i++) {
            if (s_menuMain.panels[i].anim != 0) {
                tween_step(&s_menuMain.panels[i]);
            }
        }
    }
}

void menuMain_drawParty(void) {
    RECT r;
    s16 y;
    s32 i;
    RECT *p;

    i = 0;
    do {
        p = &s_menuMain.panels[i].rect;
        r.x = p->x + 0x82;
        y = p->y;
        r.w = 0x72;
        r.h = 0x12;
        r.y = y;
        if (g_gameState->gameData->party[i] != NULL) {
            menuMain_drawMember(i);
        }
        menuMain_drawPortraitShade(p, 0x14);
        win_drawBox(&r, 0x14, WIN_STYLE_LINE);
        win_drawBox(p, 0x14, WIN_STYLE_LINE);
        win_drawShadow(p, 0x15, WIN_STYLE_LINE);
        i++;
    } while (i < 4);
}

s32 fld_getSpriteFrameCount(u16 *spr) {
    return *spr & 0x7FFF;
}

u8 *fld_drawSprite3D(u8 *pkt, FieldSprHdr *h, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 fade,
                  u32 *ot) {
    VECTOR s;
    SVECTOR zero;
    MATRIX m;
    FieldSprArgs c;
    u16 *p;
    u8 *b;
    s32 n;
    s32 f;
    u16 *o;

    if (rot == NULL) {
        rot = &zero;
        ((s32 *)&zero)[0] = 0;
        ((s32 *)&zero)[1] = 0;
    }
    n = h->frameCount & 0x7FFF;
    f = frame;
    if (f >= n) {
        f = frame % n;
    }
    o = &h->frameOffsets[f];
    n = n * 2 + 8;
    b = (u8 *)h + n;
    p = (u16 *)b;
    f = *p++;
    c.uvs = p;
    c.colors = (u8 *)(p + f + 1);
    c.tpage = h->tpage | ((abr & 3) << 5);
    c.clut = h->clut;
    c.halfH = (h->height - 1) * 2;
    c.halfW = (h->width - 1) * 2;
    n = *o;
    c.quadCount = ((u8 *)h)[n++];
    h = (FieldSprHdr *)((u8 *)h + n);
    gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    gte_ldv0(pos);
    gte_rtv0tr();
    math_rotMatrixYXZ(rot, &m);
    if (scale >= 0) {
        s.vx = s.vy = s.vz = scale;
        ScaleMatrix(&m, &s);
    }
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.primCode = (abr == 0xFF) ? 0x2C : 0x2E;
    c.fade = fade;
    return fld_emitSprite3D((u8 *)h, &c, pkt, ot);
}

/* upload the images and palettes of a texture file to VRAM */
void fld_loadImagePack(s32 *pack) {
    RECT r;
    RECT *q;
    u16 *p;
    u8 *data;
    s32 n;
    s32 i;
    s32 sz;
    s32 m;
    s32 ofs;

    ofs = pack[0];
    q = (RECT *)((u8 *)pack + ofs);
    ofs = pack[1];
    i = 0;
    p = (u16 *)((u8 *)pack + ofs);
    ofs = *(s32 *)q;
    n = pack[2];
    data = (u8 *)pack + ofs;
    for (; i < n; i++) {
        r.x = p[0];
        r.y = p[1];
        r.w = p[2];
        r.h = p[3];
        sz = r.w * r.h * 2;
        p += 4;
        LoadImage(&r, (u32 *)p);
        p = (u16 *)((u8 *)p + sz);
    }
    m = ((s32 *)q)[1];
    q++;
    for (i = 0; i < m; i++) {
        r.x = q->x;
        r.y = q->y;
        r.w = q->w;
        r.h = q->h;
        LoadImage(&r, (u32 *)data);
        q++;
        data += r.w * r.h * 2;
    }
}

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/menu_main", s_menuMainText);
