#include "ovl_15.h"

s32 ending_messages(void) {
    s32 timer;
    s32 page;
    u32 *ot;
    u8 *msg;
    s32 x;

    page = 1;
    timer = -1;
    ending_messagesInit();
    do {
        ending_messagesClearOt();
        pad_update();
        switch (g_messagesState) {
        case MESSAGES_ST_START:
            page = 1;
            timer = -1;
            g_messagesState = MESSAGES_ST_PAGE1;
            break;
        case MESSAGES_ST_PAGE1:
            if (timer == -1) {
                timer = 0xA0;
                game_startFade(1, 1, 0x14, 0);
            } else if (timer != 0) {
                ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf] + SCREEN_OTZ;
                msg = ending_getMessage(g_endingMessages, 0);
                x = 0xA0 - (ending_textWidth(msg) >> 1);
                g_messagesPrim = ending_drawText(x, 0x64, ot, msg, g_messagesPrim);
                if (timer == 0x14) {
                    game_startFade(1, 1, 0x14, 0xFFFFFF);
                }
                timer--;
            } else {
                timer = -1;
                g_messagesState = MESSAGES_ST_PAUSE;
            }
            break;
        case MESSAGES_ST_PAGE2:
            if (timer == -1) {
                timer = 0xDC;
                game_startFade(1, 1, 0x14, 0);
            } else if (timer != 0) {
                ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf] + SCREEN_OTZ;
                msg = ending_getMessage(g_endingMessages, 1);
                x = 0xA0 - (ending_textWidth(msg) >> 1);
                g_messagesPrim = ending_drawText(x, 0x5F, ot, msg, g_messagesPrim);
                if (timer < 0x8C) {
                    msg = ending_getMessage(g_endingMessages, 2);
                    x = 0xA0 - (ending_textWidth(msg) >> 1);
                    g_messagesPrim = ending_drawText(x, 0x7D, ot, msg, g_messagesPrim);
                }
                if (timer == 0x14) {
                    game_startFade(1, 1, 0x14, 0xFFFFFF);
                }
                timer--;
            } else {
                timer = -1;
                g_messagesState = MESSAGES_ST_PAUSE;
            }
            break;
        case MESSAGES_ST_PAGE3:
            if (timer == -1) {
                timer = 0xA0;
                game_startFade(1, 1, 0x14, 0);
            } else if (timer != 0) {
                ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf] + SCREEN_OTZ;
                msg = ending_getMessage(g_endingMessages, 3);
                x = 0xA0 - (ending_textWidth(msg) >> 1);
                g_messagesPrim = ending_drawText(x, 0x64, ot, msg, g_messagesPrim);
                if (timer == 0x14) {
                    game_startFade(1, 1, 0x14, 0xFFFFFF);
                }
                timer--;
            } else {
                timer = -1;
                g_messagesState = MESSAGES_ST_PAUSE;
            }
            break;
        case MESSAGES_ST_PAGE4:
            if (timer == -1) {
                timer = 0x118;
                game_startFade(1, 1, 0x14, 0);
            } else if (timer != 0) {
                ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf] + SCREEN_OTZ;
                msg = ending_getMessage(g_endingMessages, 4);
                x = 0xA0 - (ending_textWidth(msg) >> 1);
                g_messagesPrim = ending_drawText(x, 0x5F, ot, msg, g_messagesPrim);
                if (timer < 0x8C) {
                    msg = ending_getMessage(g_endingMessages, 5);
                    x = 0xA0 - (ending_textWidth(msg) >> 1);
                    g_messagesPrim = ending_drawText(x, 0x7D, ot, msg, g_messagesPrim);
                }
                if (timer == 0x14) {
                    game_startFade(1, 1, 0x14, 0xFFFFFF);
                }
                timer--;
            } else {
                timer = -1;
                g_messagesState = MESSAGES_ST_DONE;
            }
            break;
        case MESSAGES_ST_PAUSE:
            if (timer == -1) {
                timer = 0x3C;
            } else if (timer != 0) {
                timer--;
            } else {
                timer = -1;
                g_messagesState = page + 1;
                page++;
            }
            break;
        }
        game_updateFade();
        ending_messagesSwap();
    } while (g_messagesState < MESSAGES_ST_DONE);
    return 0;
}

s32 ending_messagesInit(void) {
    g_gameState->vsyncMode = 0;
    game_startFade(1, 1, 0, 0xFFFFFF);
    g_messagesState = MESSAGES_ST_START;
    g_messagesFrameWord = 0;
    g_messagesFrame = 0;
    g_padPortMask[0] = 0;
    return 0;
}

void ending_messagesClearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->ot[ctx->drawBuf], OT_LEN);
    g_messagesPrim = g_messagesPrims[g_gameState->gameData->drawBuf];
}

void ending_messagesSwap(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    ctx = g_gameState->gameData;
    i = ctx->drawBuf;
    DrawOTagEnv(ctx->ot[i] + OT_LAST, &g_gameState->drawEnv[i]);
    g_gameState->gameData->drawBuf ^= 1;
    gfx_setBuffer(g_gameState->gameData->drawBuf);
    g_messagesFrameWord = 0;
    g_messagesFrame++;
}

u8 *ending_getMessage(u8 *bank, s32 idx) {
    return bank + (bank[idx * 4 + 4] + 4);
}

void *ending_drawText(s32 x, s32 y, u32 *ot, u8 *str, void *prim) {
    Font *font = g_gameState->gameData->font;
    s32 w;
    s32 ch;
    s32 hi = font->extCode >> 8;

    ch = *str;
    while (ch != 0xFF) {
        if (ch >= hi) {
            ch = (ch << 8) + str[1] - font->extCode + font->extBase;
            str++;
            prim = ending_drawGlyph(x, y, ot, *(font->glyphs + ch), &w, prim);
            x += w;
        } else {
            prim = ending_drawGlyph(x, y, ot, *(font->glyphs + ch), &w, prim);
            x += w;
        }
        ch = *++str;
    }
    SetDrawTPage(prim, 1, 1, 0x1E);
    addPrim(ot, prim);
    return (DR_TPAGE *)prim + 1;
}

s32 ending_textWidth(u8 *str) {
    Font *font;
    s32 width;
    s32 ch;
    u8 hi;
    u32 *glyph;

    width = 0;
    font = g_gameState->gameData->font;
    hi = font->extCode >> 8;
    while ((ch = *str) != 0xFF) {
        if (ch >= hi) {
            ch = (ch << 8) + str[1] - font->extCode + font->extBase;
            str++;
            width += (font->glyphs[ch] >> 28) + 1;
        } else {
            glyph = &font->glyphs[ch];
            width += (*glyph >> 28) + 1;
        }
        str++;
    }
    return width;
}

void *ending_drawGlyph(s32 x, s32 y, u32 *ot, u32 glyph, s32 *out, SprtWords *sprt) {
    u32 glyphW;

    setlen(sprt, 4);
    sprt->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    sprt->xy = ((x + ((glyph >> 20) & 3)) & 0xFFFF) | (y << 16);
    sprt->uvclut = (glyph & 0x40FFFF) + 0x38270000;
    sprt->wh = ((glyph >> 16) & 0xF) | 0xD0000;
    addPrim(ot, sprt);
    sprt++;
    glyphW = glyph >> 28;
    if (out != NULL) {
        *out = glyphW + 1;
    }
    return sprt;
}

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ending_messages", g_endingMessages);
