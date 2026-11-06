#include "ovl_0a.h"

void fld_requestDiscChange(s32 disc, s16 fieldId) {
    g_fieldDiscChange.discMask = 1 << disc;
    g_fieldDiscChange.disc = disc;
    g_fieldDiscChange.fieldId = fieldId;
    g_fieldDiscChange.areaId = -1;
    g_gameState->gameData->ctx.field->flags |= FLD_MAP_DISC_CHANGE;
    g_gameState->flags |= GS_FLD_NEW_FIELD;
}

u8 fld_getDiscChangeDisc(void) {
    return g_fieldDiscChange.disc;
}

u8 fld_getDiscChangeMask(void) {
    return g_fieldDiscChange.discMask;
}

s16 fld_getDiscChangeFieldId(void) {
    return g_fieldDiscChange.fieldId;
}

s16 fld_getDiscChangeAreaId(void) {
    return g_fieldDiscChange.areaId;
}

void fld_runDiscChange(void) {
    fld_discChangeEnter();
    while (g_fieldDiscChange.mode != FLD_DISC_DONE) {
        fld_discChangeBeginFrame();
        fld_discChangeUpdate();
        fld_discChangeEndFrame();
    }
    fld_discChangeLeave();
}

void fld_discChangeEnter(void) {
    s32 i;

    DrawSync(0);
    for (i = 0; i < 2; i++) {
        g_fieldDiscChange.savedFlags[i] = g_padPortMask[i];
        g_padPortMask[i] = (BTN_START | PAD_R2 | PAD_L2);
    }
    g_fieldDiscChange.mode = FLD_DISC_PROMPT;
    g_fieldDiscChange.cursor = 0;
    g_gameState->gameData->ctx.field->flags = 0;
    win_closeAllNow();
    g_fieldDiscChangeModes[g_fieldDiscChange.mode].enter();
}

/* leave the current field mode */
void fld_discChangeLeave(void) {
    FieldMap *map;
    s32 i;

    if (g_fieldDiscChangeModes[g_fieldDiscChange.mode].leave != NULL) {
        g_fieldDiscChangeModes[g_fieldDiscChange.mode].leave();
    }
    DrawSync(0);
    for (i = 0; i < 2; i++) {
        g_padPortMask[i] = g_fieldDiscChange.savedFlags[i];
    }
    map = g_gameState->gameData->ctx.field;
    map->flags |= FLD_MAP_REFIND_SECTORS;
    file_loadDirTables((GameState *)map);
    fld_refindSectors();
}

void fld_discChangeBeginFrame(void) {
    GameData *gd = g_gameState->gameData;

    ClearOTagR(gd->ot[gd->drawBuf], OT_LEN);
    pad_update();
}

void fld_discChangeUpdate(void) {
    if (g_fieldDiscChangeModes[g_fieldDiscChange.mode].update != NULL) {
        g_fieldDiscChangeModes[g_fieldDiscChange.mode].update(g_fieldDiscChange.mode);
    }
}

void fld_discChangeEndFrame(void) {
    menu_update();
    game_updateWindows();
    game_updateFade();
    game_updateTimer();
    DrawSync(0);
    VSync(0);
    ResetGraph(1);
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    DrawOTagEnv(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][OT_LAST], &g_gameState->drawEnv[g_gameState->gameData->drawBuf]);
    g_gameState->gameData->drawBuf ^= 1;
    gfx_setBuffer(g_gameState->gameData->drawBuf);
}

void fld_discChangeSetMode(s32 mode) {
    if (mode != g_fieldDiscChange.mode) {
        if (g_fieldDiscChangeModes[g_fieldDiscChange.mode].leave != NULL) {
            g_fieldDiscChangeModes[g_fieldDiscChange.mode].leave();
        }
        g_fieldDiscChange.mode = mode;
        if (g_fieldDiscChangeModes[g_fieldDiscChange.mode].enter != NULL) {
            g_fieldDiscChangeModes[g_fieldDiscChange.mode].enter();
        }
    }
}

s32 fld_isDiscChangePending(void) {
    return (g_gameState->gameData->ctx.field->flags >> 20) & 1;
}

void fld_discPromptEnter(void) {
    fade_inDefault();
    help_init();
    help_setCallback(fld_discPromptHelp);
    text_setBank(g_fieldDiscChangeText);
}

void fld_discPromptUpdate(void) {
    pad_updateRepeat(0);
    fld_discPromptInput();
    fld_discPromptDraw();
    help_update();
}

void fld_discPromptLeave(void) {
    help_setBounds(NULL);
    help_setCallback(NULL);
}

void fld_discPromptInput(void) {
    if (!fade_isBusy()) {
        if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
            snd_playSe(SE_CURSOR);
            g_fieldDiscChange.cursor ^= 1;
        } else if (g_padRepeat & BTN_CIRCLE) {
            if (g_fieldDiscChange.cursor == 0) {
                snd_playSe(SE_CURSOR);
                g_fieldDiscChange.cursor = 1;
            }
        } else if (g_padRepeat & BTN_CROSS) {
            snd_playSe(SE_CURSOR);
            fade_outDefault();
            fade_setOnDone(fld_discPromptSelect);
        }
    }
}

/* DISC CHANGE label */
INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/fld_disc", g_fieldDiscChangeLabel);

void fld_discPromptDraw(void) {
    s32 y;
    HelpState *help;

    ui_drawLabel(g_fieldDiscChangeRect.x + 8, g_fieldDiscChangeRect.y + WIN_TITLE_Y, 0xA, g_fieldDiscChangeLabel, UI_LABEL_NORMAL);
    text_drawBlock(g_fieldDiscChangeRect.x + 0xE, g_fieldDiscChangeRect.y + 0xA, 0xA, text_get(FLD_DISC_TEXT_PROMPT), TEXT_PAL_WHITE);
    text_drawBlock(g_fieldDiscChangeRect.x + 0x30, g_fieldDiscChangeRect.y + 0x2E, 0xA, text_get(FLD_DISC_TEXT_YES), TEXT_PAL_WHITE);
    text_drawBlock(g_fieldDiscChangeRect.x + 0x30, g_fieldDiscChangeRect.y + 0x3D, 0xA, text_get(FLD_DISC_TEXT_NO), TEXT_PAL_WHITE);
    if (!fade_isBusy()) {
        help = &g_menuHelp;
        help->x = g_fieldDiscChangeRect.x + 0x16;
        y = g_fieldDiscChangeRect.y;
        if (g_fieldDiscChange.cursor == 0) {
            help->y = y + 0x32;
        } else {
            help->y = y + 0x41;
        }
        g_menuHelp.otz = 9;
        help_drawCursor();
    }
    win_drawBox(&g_fieldDiscChangeRect, 0xA, WIN_STYLE_BEVEL);
}

void fld_discPromptSelect(void) {
    if (g_fieldDiscChange.cursor == 0) {
        fld_discChangeSetMode(FLD_DISC_SAVE);
    } else {
        fld_discChangeSetMode(FLD_DISC_SWAP);
    }
}

void fld_discPromptHelp(void) {
    RECT bounds;
    s32 id;
    HelpState *help = &g_menuHelp;

    help->text = NULL;
    if (!fade_isBusy()) {
        if (g_fieldDiscChange.cursor == 0) {
            help_setBounds(NULL);
            id = 3;
        } else {
            bounds.x = 0;
            bounds.y = help->y;
            bounds.w = g_gameState->dispEnv->disp.w;
            bounds.h = g_gameState->dispEnv->disp.h - help->y;
            help_setBounds(&bounds);
            id = 4;
        }
        help->text = text_get(id);
    }
}

void fld_discChangeOpenMenu4(void) {
    menu_open(MENU_SAVE, 2);
    menu_setOnClose(fld_discChangeBackToPrompt);
    menu_setFadeInOnClose(0);
}

void fld_discChangeMenu4Update(void) {
}

void fld_discChangeMenu4Leave(void) {
}

void fld_discChangeBackToPrompt(void) {
    fld_discChangeSetMode(FLD_DISC_PROMPT);
}

void fld_discChangeOpenMenu8(void) {
    menu_open(MENU_DISC, MENU_DISC_ARG(g_fieldDiscChange.disc, g_fieldDiscChange.fieldId));
    menu_setOnClose(fld_discChangeFinish);
    menu_setFadeInOnClose(0);
}

void fld_discChangeMenu8Update(void) {
}

void fld_discChangeMenu8Leave(void) {
}

void fld_discChangeFinish(void) {
    fld_discChangeSetMode(FLD_DISC_DONE);
}

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/fld_disc", g_fieldDiscChangeText);
