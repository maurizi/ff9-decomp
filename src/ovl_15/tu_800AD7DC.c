#include "ovl_15.h"

s32 ending_messages(void) {
    s32 t;
    s32 page;
    u32 *ot;
    u8 *msg;
    s32 x;

    page = 1;
    t = -1;
    ending_messagesInit();
    do {
        ending_messagesClearOt();
        pad_update();
        switch (g_messagesState) {
        case 0:
            page = 1;
            t = -1;
            g_messagesState = 1;
            break;
        case 1:
            if (t == -1) {
                t = 0xA0;
                func_800320B8(1, 1, 0x14, 0);
            } else if (t != 0) {
                ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 8;
                msg = ending_getMessage(g_endingMessages, 0);
                x = 0xA0 - (ending_textWidth(msg) >> 1);
                g_messagesPrim = ending_drawText(x, 0x64, ot, msg, g_messagesPrim);
                if (t == 0x14) {
                    func_800320B8(1, 1, 0x14, 0xFFFFFF);
                }
                t--;
            } else {
                t = -1;
                g_messagesState = 5;
            }
            break;
        case 2:
            if (t == -1) {
                t = 0xDC;
                func_800320B8(1, 1, 0x14, 0);
            } else if (t != 0) {
                ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 8;
                msg = ending_getMessage(g_endingMessages, 1);
                x = 0xA0 - (ending_textWidth(msg) >> 1);
                g_messagesPrim = ending_drawText(x, 0x5F, ot, msg, g_messagesPrim);
                if (t < 0x8C) {
                    msg = ending_getMessage(g_endingMessages, 2);
                    x = 0xA0 - (ending_textWidth(msg) >> 1);
                    g_messagesPrim = ending_drawText(x, 0x7D, ot, msg, g_messagesPrim);
                }
                if (t == 0x14) {
                    func_800320B8(1, 1, 0x14, 0xFFFFFF);
                }
                t--;
            } else {
                t = -1;
                g_messagesState = 5;
            }
            break;
        case 3:
            if (t == -1) {
                t = 0xA0;
                func_800320B8(1, 1, 0x14, 0);
            } else if (t != 0) {
                ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 8;
                msg = ending_getMessage(g_endingMessages, 3);
                x = 0xA0 - (ending_textWidth(msg) >> 1);
                g_messagesPrim = ending_drawText(x, 0x64, ot, msg, g_messagesPrim);
                if (t == 0x14) {
                    func_800320B8(1, 1, 0x14, 0xFFFFFF);
                }
                t--;
            } else {
                t = -1;
                g_messagesState = 5;
            }
            break;
        case 4:
            if (t == -1) {
                t = 0x118;
                func_800320B8(1, 1, 0x14, 0);
            } else if (t != 0) {
                ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 8;
                msg = ending_getMessage(g_endingMessages, 4);
                x = 0xA0 - (ending_textWidth(msg) >> 1);
                g_messagesPrim = ending_drawText(x, 0x5F, ot, msg, g_messagesPrim);
                if (t < 0x8C) {
                    msg = ending_getMessage(g_endingMessages, 5);
                    x = 0xA0 - (ending_textWidth(msg) >> 1);
                    g_messagesPrim = ending_drawText(x, 0x7D, ot, msg, g_messagesPrim);
                }
                if (t == 0x14) {
                    func_800320B8(1, 1, 0x14, 0xFFFFFF);
                }
                t--;
            } else {
                t = -1;
                g_messagesState = 6;
            }
            break;
        case 5:
            if (t == -1) {
                t = 0x3C;
            } else if (t != 0) {
                t--;
            } else {
                t = -1;
                g_messagesState = page + 1;
                page++;
            }
            break;
        }
        func_80032098();
        ending_messagesSwap();
    } while (g_messagesState < 6);
    return 0;
}

s32 ending_messagesInit(void) {
    g_gameState->vsyncMode = 0;
    func_800320B8(1, 1, 0, 0xFFFFFF);
    g_messagesState = 0;
    D_800AE4A8 = 0;
    g_messagesFrame = 0;
    D_80073998[0] = 0;
    return 0;
}

void ending_messagesClearOt(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
    g_messagesPrim = g_messagesPrims[g_gameState->unk1C->unk8];
}

void ending_messagesSwap(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    ctx = g_gameState->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->unk10[i]);
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
    D_800AE4A8 = 0;
    g_messagesFrame++;
}

u8 *ending_getMessage(u8 *arg0, s32 arg1) {
    return arg0 + (arg0[arg1 * 4 + 4] + 4);
}

void *ending_drawText(s32 x, s32 y, u32 *ot, u8 *s, void *p) {
    Unk7D4 *f = g_gameState->unk1C->unk7D4;
    s32 w;
    s32 c;
    s32 hi = f->unk4 >> 8;

    c = *s;
    while (c != 0xFF) {
        if (c >= hi) {
            c = (c << 8) + s[1] - f->unk4 + f->unk6;
            s++;
            p = ending_drawGlyph(x, y, ot, *(f->unkC + c), &w, p);
            x += w;
        } else {
            p = ending_drawGlyph(x, y, ot, *(f->unkC + c), &w, p);
            x += w;
        }
        c = *++s;
    }
    SetDrawTPage(p, 1, 1, 0x1E);
    addPrim(ot, p);
    return (DR_TPAGE *)p + 1;
}

s32 ending_textWidth(u8 *s) {
    Unk7D4 *f;
    s32 n;
    s32 c;
    u8 hi;
    u32 *g;

    n = 0;
    f = g_gameState->unk1C->unk7D4;
    hi = f->unk4 >> 8;
    while ((c = *s) != 0xFF) {
        if (c >= hi) {
            c = (c << 8) + s[1] - f->unk4 + f->unk6;
            s++;
            n += (f->unkC[c] >> 28) + 1;
        } else {
            g = &f->unkC[c];
            n += (*g >> 28) + 1;
        }
        s++;
    }
    return n;
}

void *ending_drawGlyph(s32 x, s32 y, u32 *ot, u32 a, s32 *out, SprtWords *p) {
    u32 n;

    setlen(p, 4);
    p->rgbc = 0x64808080;
    p->xy = ((x + ((a >> 20) & 3)) & 0xFFFF) | (y << 16);
    p->uvclut = (a & 0x40FFFF) + 0x38270000;
    p->wh = ((a >> 16) & 0xF) | 0xD0000;
    addPrim(ot, p);
    p++;
    n = a >> 28;
    if (out != NULL) {
        *out = n + 1;
    }
    return p;
}

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800AD7DC", g_endingMessages);
