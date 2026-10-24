#include "ff9.h"

s32 func_80028DB0(s32 x, s32 hi, s32 c) {
    u8 buf[16];
    u8 w;
    u8 start;
    u8 width;
    u8 *dst;
    u8 *src;
    s32 row;
    s32 i;
    s32 j;
    u8 b;
    u8 *p;
    s32 px;
    u8 v, o;
    u8 t;

    dst = (u8 *)D_80077144;
    w = D_80010BD0[c];
    start = w >> 4;
    width = (w & 0xF) + 1;
    src = (u8 *)D_80077140 + (c << 6);
    memset(buf, 0, 0x10);
    for (row = 0; row < 12; row++) {
        for (i = 0; i < 4; i++) {
            b = src[row * 4 + i];
            for (j = 0; j < 4; j++) {
                buf[i * 4 + j] = b & 3;
                b >>= 2;
            }
        }
        for (i = 0; i < width; i++) {
            px = x + i;
            if (!(px & 1)) {
                p = dst + (px >> 1);
                v = buf[start + i];
                o = *p;
                if (hi) { *p = o | (v << 2); } else { *p = o | v; }
            } else {
                v = buf[start + i];
                px >>= 1;
                p = dst + px;
                o = *p;
                t = v << 4; if (hi) { *p = o | (v << 6); } else { *p = o | t; }
            }
        }
        dst += 0x54;
    }
    return width + x;
}

s32 func_80028F3C(s32 x, s32 y, s32 otz, u8 c, s32 mode) {
    u32 *tbl = g_gameState->unk1C->unk7D4->unkC;
    u32 g;
    s32 w;
    SprtWords *p;

    if (c == 0xFF) {
        return 0;
    }
    g = tbl[c];
    setlen(g_primPtr, 4);
    w = (g >> 16) & 0xF;
    p = (SprtWords *)g_primPtr;
    p->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    p->xy = (x + ((10 - w) >> 1)) | (y << 16);
    p->uvclut = (g & 0x40FFFF) | (D_8006BBA0[mode] << 16);
    p->wh = w | 0xD0000;
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
    return g >> 28;
}

void text_draw(s32 x, s32 y, s32 otz, u8 *s, s32 pal) {
    Unk7D4 *f = g_gameState->unk1C->unk7D4;
    s32 th = f->unk4 >> 8;
    SprtWords *p = (SprtWords *)g_primPtr;
    u32 *ot = &g_ot[otz];
    s32 spacing;
    s32 c;
    u32 g;
    u32 w;
    s32 gx;
    s32 k;

    while ((c = *s) != 0xFF) {
        if (c >= th) {
            c = (c << 8) + s[1] - f->unk4 + f->unk6;
            s++;
        }
        g = *(f->unkC + c);
        setlen(p, 4);
        k = (g >> 20) & 3;
        p->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
        w = g >> 28;
        spacing = 1;
        gx = x + k;
        x = x + spacing + w;
        p->xy = (gx & 0xFFFF) | (y << 16);
        p->uvclut = (g & 0x40FFFF) + (D_8006BBA0[pal] << 16);
        p->wh = ((g >> 16) & 0xF) | 0xD0000;
        addPrim(ot, p);
        p++;
        s++;
    }
    setlen(p, 1);
    ((DrTpageWords *)p)->code = get_mode(1, 1, 0x1E);
    addPrim(ot, p);
    g_primPtr = (u8 *)p + 8;
}

u8 *func_8002920C(s32 x, s32 y, u32 *ot, u8 *s, s32 mode, u8 *pkt) {
    s32 w;
    s32 x0;
    u8 th;
    Unk7D4 *font;
    s32 c;
    s32 k;

    font = g_gameState->unk1C->unk7D4;
    D_800770AC = mode;
    x0 = x;
    th = font->unk4 >> 8;
    while ((c = *s) != 0xFF) {
        if (c == 0xF9) {
            s++;
            if (*s == 0x43) {
                s++;
                x += (s8)*s;
            } else if (*s == 0x44) {
                s++;
                x = x0 + *s;
            } else if (*s == 0x49) {
                s++;
                y += *s;
            } else if (*s == 0x4A) {
                s++;
                y -= *s;
            } else if (*s == 0x40) {
                s++;
                func_8002BBE8(D_8007BBA8[*s & 7], D_80077080, 0);
                pkt = func_8002995C(x, y, ot, (Character *)D_80077080, &w, pkt);
                x += w;
            } else if (*s == 0x68) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 0);
                pkt = func_8002995C(x, y, ot, (Character *)D_80077080, &w, pkt);
                x += w;
            } else if (*s == 0x69) {
                s++;
                pkt = func_8002995C(x, y, ot, (Character *)D_800770B0[*s], &w, pkt);
                x += w;
            } else if (*s == 0x6A) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 1);
                pkt = func_80029D60(x, y, ot, D_80077080, D_800770AC, pkt);
                x += func_8002B9B8(D_80077080);
            } else if (*s == 0x6B) {
                s++;
                D_800770A8 = D_800770B0[*s];
                pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                pkt = func_80029D60(x, y, ot, D_800770A8, D_800770AC, pkt);
                x += func_8002B9B8(D_800770A8);
            } else if (*s == 0x6E) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 1);
                pkt = ui_buildLabel(x, y, ot, (s32)D_80077080, D_800770AC, pkt);
                x += func_8002AE7C(D_80077080);
            } else if (*s == 0x6F) {
                s++;
                D_800770A8 = D_800770B0[*s];
                pkt = ui_buildLabel(x, y, ot, (s32)D_800770A8, D_800770AC, pkt);
                x += func_8002AE7C(D_800770A8);
            } else if (*s == 0x6C) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 1);
                pkt = func_8002B644(x, y, ot, D_80077080, D_800770AC, pkt);
                x += func_8002BA10(D_80077080);
            } else if (*s == 0x6D) {
                s++;
                D_800770A8 = D_800770B0[*s];
                pkt = func_8002B644(x, y, ot, D_800770A8, D_800770AC, pkt);
                x += func_8002BA10(D_800770A8);
            } else if (*s == 0x4B) {
                pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                pkt = ui_buildSprite(x, y, ot, *++s, 0, &w, pkt);
                x += w;
            } else if (*s == 0x4C) {
                s++;
                k = *s;
                if (k != 0xFF) {
                    pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                    pkt = ui_buildItemIcon(x, y, ot, k, 1, pkt);
                    x += D_800686D0[D_80069018[k].unk8 + 0x5C].unk4;
                }
            } else if (*s == 0x4D) {
                pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                pkt = ui_buildSprite(x, y, ot, D_80077130[*++s], 0, &w, pkt);
                x += w;
            } else if (*s == 0x4E) {
                s++;
                k = D_80077130[*s];
                if (k != 0xFF) {
                    pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                    pkt = ui_buildItemIcon(x, y, ot, k, 1, pkt);
                    x += D_800686D0[D_80069018[k].unk8 + 0x5C].unk4;
                }
            } else {
                pkt = func_80029FE4(x, y, ot, *s, &w, pkt);
                x += w;
            }
        } else if (c == 0xF7) {
            x = x0;
            y += 15;
        } else {
            if (c >= th) {
                c = (c << 8) + s[1] - font->unk4 + font->unk6;
                s++;
                pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &w, (SprtWords *)pkt);
                x += w;
            } else {
                pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &w, (SprtWords *)pkt);
                x += w;
            }
        }
        s++;
    }
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, 0x1E);
    addPrim(ot, pkt);
    pkt += sizeof(DrTpageWords);
    return pkt;
}

u8 *func_8002995C(s32 x, s32 y, u32 *ot, Character *ch, s32 *w, u8 *pkt) {
    s32 adv;
    s32 x0;
    u8 th;
    Unk7D4 *font;
    Unk7D4 *kanji;
    s32 th2;
    u8 *s;
    s32 c;
    s32 code;

    font = g_gameState->unk1C->unk7D4;
    s = ch->unk0;
    x0 = x;
    th = font->unk4 >> 8;
    kanji = g_gameState->unk1C->unk84C;
    th2 = 0;
    if (kanji != NULL) {
        th2 = kanji->unk0 >> 8;
    }
    while ((c = *s) != 0xFF) {
        if (kanji != NULL && c >= th2) {
            code = (c << 8) + s[1];
            s++;
            c = code - kanji->unk0;
            pkt = func_8002B7B0(x, y, ot, *(kanji->unkC + c), D_800770AC, &adv, (SprtWords *)pkt);
            x += adv;
        } else if (c >= th) {
            code = (c << 8) + s[1];
            s++;
            c = code - font->unk4 + font->unk6;
            pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &adv, (SprtWords *)pkt);
            x += adv;
        } else {
            pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &adv, (SprtWords *)pkt);
            x += adv;
        }
        s++;
    }
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, 0x1E);
    addPrim(ot, pkt);
    *w = x - x0;
    return pkt + 8;
}

u8 *ui_buildLabel(s32 x, s32 y, u32 *ot, s32 str, s32 mode, u8 *pkt) {
    u8 *s = (u8 *)str;
    SprtWords *p = (SprtWords *)pkt;
    SpriteDesc16 *g = NULL;
    SpriteDesc16 *tbl;
    u32 rgbc;
    u16 clut;
    s32 c;
    s32 w;

    rgbc = GPU_RGBC(GPU_SPRT, 0x404040);
    if (mode == 0) {
        rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    }
    tbl = (SpriteDesc16 *)D_800686D0;
    clut = tbl[46].clut;
    if (mode == 1) {
        clut += 0x40;
        rgbc = GPU_CODE(GPU_SPRT | GPU_ABE);
    }
    if (s == NULL) {
        return (u8 *)p;
    }
    while ((c = *s) != 0) {
        if ((u32)(c - 'A') < 26) {
            g = &((SpriteDesc16 *)D_800686D0)[*s - 8];
        } else if ((u32)(c - 'a') < 26) {
            g = &((SpriteDesc16 *)D_800686D0)[*s - 0x28];
        } else if ((u32)(c - '0') < 10) {
            g = &((SpriteDesc16 *)D_800686D0)[*s - 2];
        } else if ((u8)c == '&') {
            g = (SpriteDesc16 *)&D_80068890;
        } else if ((u8)c == '.') {
            g = (SpriteDesc16 *)&D_80068CE0;
        } else if ((u8)c == '_') {
            g = (SpriteDesc16 *)&D_80068DC8;
        }
        if (*s != ' ') {
            setlen(p, 4);
            p->rgbc = rgbc;
            p->xy = (x & 0xFFFF) | (y << 16);
            w = g->w;
            p->uvclut = g->uv | (clut << 16);
            p->wh = g->w | (g->h << 16);
            addPrim(ot, p);
            p++;
        } else {
            w = 5;
        }
        s++;
        x += w - 1;
    }
    setlen(p, 1);
    ((DrTpageWords *)p)->code = get_mode(1, 1, 0x1F);
    addPrim(ot, p);
    return (u8 *)p + 8;
}

u8 *func_80029D60(s32 x, s32 y, u32 *ot, u8 *s, s32 pal, u8 *pkt) {
    Sprt8Fields *p = (Sprt8Fields *)pkt;
    u8 kana = 0;
    s32 hi = 0;
    s32 c;
    s32 code;
    s32 row;
    s32 u;
    s32 v;
    s32 k;
    Sprt8Fields *q;

    if (*s == 0x40) {
        kana = 1;
        s++;
    }
    while ((c = *s) != 0) {
        if ((u8)c == 0x40) {
            kana ^= 1;
            s++;
        } else if ((u8)c == 0x20) {
            x += 7;
            s++;
        } else {
            if ((u32)(c - 0x21) < 0x3F) {
                code = *s - 0x20;
                if (*s == 0x5C) {
                    code = *s - 0x4D;
                }
                goto calc;
            } else if ((u32)(c - 0x61) < 0x1A) {
                code = *s - 0x40;
            calc:
                row = code >> 5;
                hi = (row >= 3) << 6;
                u = (code & 0x1F) * 8;
                v = (row % 3) * 8;
            } else if ((u8)c >= 0xA0) {
                code = *s - 0xA0;
                k = (code >> 5) + 2;
                if (!kana) {
                    k = (code >> 5) + 4;
                }
                hi = (k >= 3) << 6;
                u = (code & 0x1F) * 8;
                v = (k % 3) * 8;
            } else {
                v = 0;
                u = 0;
            }
            setlen(p, 3);
            p->rgbc = GPU_RGBC(GPU_SPRT_8, 0x808080);
            q = p;
            if ((u8)(*s + 0x22) < 2) {
                p->x = x - 7;
                p->y = y - 7;
            } else {
                p->x = x;
                x += 7;
                p->y = y;
            }
            q->u = u;
            q->v = v - 0x80;
            q->clut = D_8006BBA0[pal] | hi;
            addPrim(ot, p);
            p++;
            s++;
        }
    }
    setlen(p, 1);
    ((DrTpageWords *)p)->code = get_mode(1, 1, 0x1F);
    hi = getaddr(ot); /* reuses hi as the tag temp */
    setaddr(p, hi), setaddr(ot, p);
    return (u8 *)p + 8;
}

u8 *func_80029FE4(s32 x, s32 y, u32 *ot, s32 code, s32 *w, u8 *pkt) {
    pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
    switch (code & 0xFF) {
    case 0x21:
        D_800770AC = 1;
        *w = 0;
        break;
    case 0x23:
        D_800770AC = 3;
        *w = 0;
        break;
    case 0x26:
        D_800770AC = 6;
        *w = 0;
        break;
    case 0x27:
        D_800770AC = 7;
        *w = 0;
        break;
    case 0x25:
        D_800770AC = 5;
        *w = 0;
        break;
    case 0x24:
        D_800770AC = 4;
        *w = 0;
        break;
    case 0x20:
        D_800770AC = 0;
        *w = 0;
        break;
    case 0x22:
        D_800770AC = 2;
        *w = 0;
        break;
    case 0x28:
        D_800770AC = 8;
        *w = 0;
        break;
    case 0x29:
        D_800770AC = 9;
        *w = 0;
        break;
    case 0x30:
        pkt = ui_buildSprite(x, y + 2, ot, 5, 0, w, pkt);
        break;
    case 0x31:
        pkt = ui_buildSprite(x, y + 2, ot, 4, 0, w, pkt);
        break;
    case 0x32:
        pkt = ui_buildSprite(x, y + 2, ot, 7, 0, w, pkt);
        break;
    case 0x33:
        pkt = ui_buildSprite(x, y + 2, ot, 6, 0, w, pkt);
        break;
    case 0x34:
        pkt = ui_buildSprite(x, y + 2, ot, 8, 0, w, pkt);
        break;
    case 0x35:
        pkt = ui_buildSprite(x, y + 2, ot, 10, 0, w, pkt);
        break;
    case 0x36:
        pkt = ui_buildSprite(x, y + 2, ot, 9, 0, w, pkt);
        break;
    case 0x37:
        pkt = ui_buildSprite(x, y + 2, ot, 11, 0, w, pkt);
        break;
    case 0x60:
        pkt = ui_buildSprite(x, y + 2, ot, func_8002BE10(1), 0, w, pkt);
        break;
    case 0x61:
        pkt = ui_buildSprite(x, y + 2, ot, func_8002BE10(0), 0, w, pkt);
        break;
    case 0x62:
        pkt = ui_buildSprite(x, y + 2, ot, func_8002BE10(2), 0, w, pkt);
        break;
    case 0x63:
        pkt = ui_buildSprite(x, y + 2, ot, func_8002BE10(3), 0, w, pkt);
        break;
    case 0x64:
        pkt = ui_buildSprite(x, y + 2, ot, func_8002BE10(4), 0, w, pkt);
        break;
    case 0x65:
        pkt = ui_buildSprite(x, y + 2, ot, func_8002BE10(5), 0, w, pkt);
        break;
    case 0x66:
        pkt = ui_buildSprite(x, y + 2, ot, func_8002BE10(6), 0, w, pkt);
        break;
    case 0x67:
        pkt = ui_buildSprite(x, y + 2, ot, func_8002BE10(7), 0, w, pkt);
        break;
    case 0x38:
        pkt = ui_buildSprite(x, y + 2, ot, 0x11, 0, w, pkt);
        break;
    case 0x39:
        pkt = ui_buildSprite(x, y + 2, ot, 0x12, 0, w, pkt);
        break;
    case 0x3A:
        pkt = func_8002B890(x, y + 2, ot, 1, 0, w, pkt);
        break;
    case 0x3B:
        pkt = func_8002B890(x, y + 2, ot, 2, 0, w, pkt);
        break;
    case 0x3C:
        pkt = func_8002B890(x, y + 2, ot, 4, 0, w, pkt);
        break;
    case 0x3D:
        pkt = func_8002B890(x, y + 2, ot, 8, 0, w, pkt);
        break;
    case 0x3E:
        pkt = func_8002B890(x, y + 2, ot, 3, 0, w, pkt);
        break;
    case 0x3F:
        pkt = func_8002B890(x, y + 2, ot, 12, 0, w, pkt);
        break;
    case 0x10:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[0], w, pkt);
        break;
    case 0x11:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[1], w, pkt);
        break;
    case 0x12:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[2], w, pkt);
        break;
    case 0x13:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[3], w, pkt);
        break;
    case 0x14:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[4], w, pkt);
        break;
    case 0x15:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[5], w, pkt);
        break;
    case 0x16:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[6], w, pkt);
        break;
    case 0x17:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[7], w, pkt);
        break;
    case 0x18:
        if (g_gameState->unk1C->party[0] != NULL) {
            pkt = func_8002995C(x, y, ot, g_gameState->unk1C->party[0], w, pkt);
        }
        break;
    case 0x19:
        if (g_gameState->unk1C->party[1] != NULL) {
            pkt = func_8002995C(x, y, ot, g_gameState->unk1C->party[1], w, pkt);
        }
        break;
    case 0x1A:
        if (g_gameState->unk1C->party[2] != NULL) {
            pkt = func_8002995C(x, y, ot, g_gameState->unk1C->party[2], w, pkt);
        }
        break;
    case 0x1B:
        if (g_gameState->unk1C->party[3] != NULL) {
            pkt = func_8002995C(x, y, ot, g_gameState->unk1C->party[3], w, pkt);
        }
        break;
    }
    return pkt;
}

s32 func_8002A730(s32 c) {
    s32 r = 0;

    switch (c & 0xFF) {
    case 0x30:
        r = D_800686D0[5].unk4;
        break;
    case 0x31:
        r = D_800686D0[4].unk4;
        break;
    case 0x32:
        r = D_800686D0[7].unk4;
        break;
    case 0x33:
        r = D_800686D0[6].unk4;
        break;
    case 0x34:
        r = D_800686D0[8].unk4;
        break;
    case 0x35:
        r = D_800686D0[10].unk4;
        break;
    case 0x36:
        r = D_800686D0[9].unk4;
        break;
    case 0x37:
        r = D_800686D0[11].unk4;
        break;
    case 0x60:
        r = D_800686D0[func_8002BE10(0)].unk4;
        break;
    case 0x61:
        r = D_800686D0[func_8002BE10(1)].unk4;
        break;
    case 0x62:
        r = D_800686D0[func_8002BE10(2)].unk4;
        break;
    case 0x63:
        r = D_800686D0[func_8002BE10(3)].unk4;
        break;
    case 0x64:
        r = D_800686D0[func_8002BE10(4)].unk4;
        break;
    case 0x65:
        r = D_800686D0[func_8002BE10(5)].unk4;
        break;
    case 0x66:
        r = D_800686D0[func_8002BE10(6)].unk4;
        break;
    case 0x67:
        r = D_800686D0[func_8002BE10(7)].unk4;
        break;
    case 0x38:
        r = D_800686D0[17].unk4;
        break;
    case 0x39:
        r = D_800686D0[18].unk4;
        break;
    case 0x3A:
    case 0x3B:
    case 0x3C:
    case 0x3D:
    case 0x3E:
    case 0x3F:
        r = D_800686D0[12].unk4;
        break;
    case 0x10:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[0]);
        break;
    case 0x11:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[1]);
        break;
    case 0x12:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[2]);
        break;
    case 0x13:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[3]);
        break;
    case 0x14:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[4]);
        break;
    case 0x15:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[5]);
        break;
    case 0x16:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[6]);
        break;
    case 0x17:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[7]);
        break;
    case 0x18:
        if (g_gameState->unk1C->party[0] != NULL) {
            r = func_8002BA68((s32)g_gameState->unk1C->party[0]);
        } else {
            r = 0;
        }
        break;
    case 0x19:
        if (g_gameState->unk1C->party[1] != NULL) {
            r = func_8002BA68((s32)g_gameState->unk1C->party[1]);
        } else {
            r = 0;
        }
        break;
    case 0x1A:
        if (g_gameState->unk1C->party[2] != NULL) {
            r = func_8002BA68((s32)g_gameState->unk1C->party[2]);
        } else {
            r = 0;
        }
        break;
    case 0x1B:
        if (g_gameState->unk1C->party[3] != NULL) {
            r = func_8002BA68((s32)g_gameState->unk1C->party[3]);
        } else {
            r = 0;
        }
        break;
    }
    return r;
}

s32 func_8002AA04(s32 frames, u8 *buf, s32 blink) {
    s32 r = 0;
    s32 sec;
    s32 min;
    s32 hr;
    u8 sub;

    sec = (u32)frames / 60;
    min = sec / 60;
    sec %= 60;
    hr = min / 60;
    min %= 60;
    if (hr >= 601) {
        hr = 599;
        sec = 59;
        min = 59;
    }
    if (hr >= 100) {
        r = D_8006BBD4[hr / 100];
        hr %= 100;
    }
    if ((u8)blink) {
        sub = (u32)frames % 60;
    } else {
        sub = 0;
    }
    text_formatNumber_inline(hr, buf, 2, 1);
    text_formatNumber_inline(min, buf + 3, 2, 1);
    if (buf[3] == ' ') {
        buf[3] = '0';
    }
    text_formatNumber_inline(sec, buf + 6, 2, 1);
    if (buf[6] == ' ') {
        buf[6] = '0';
    }
    if ((u8)sub < 30) {
        buf[2] = ':';
        buf[5] = ':';
    } else {
        buf[5] = ' ';
        buf[2] = ' ';
    }
    if (hr < 10) {
        buf[0] = '0';
    }
    return r;
}

u8 *text_append(u8 *dst, u8 *src) {
    s32 c;
    u8 th;
    u8 *p;
    u8 *ret;
    u8 *s;
    s32 c2;
    u8 th2;

    ret = dst;
    s = ret;
    p = s;
    th = g_gameState->unk1C->unk7D4->unk4 >> 8;
    while ((c = *p) != 0xFF) {
        p++;
        if (c == 0xF9) {
            p++;
            if ((u32)(p[-2] - 0x43) < 2) {
                p++;
            }
        } else if (c >= th) {
            p++;
        }
    }
    dst += p - s;
    th2 = g_gameState->unk1C->unk7D4->unk4 >> 8;
    while ((c2 = *src) != 0xFF) {
        *dst++ = *src++;
        if (c2 == 0xF9) {
            *dst++ = *src++;
            if ((u32)(src[-2] - 0x43) < 2) {
                *dst++ = *src++;
            }
        } else if (c2 >= th2) {
            *dst++ = *src++;
        }
    }
    *dst = 0xFF;
    return ret;
}

s32 func_8002AE7C(u8 *s) {
    s32 w;
    Glyph8 *g;
    u8 c;
    s32 adv;

    w = 0;
    g = NULL;
    while ((c = *s) != 0) {
        if (c >= 'A' && c <= 'Z') {
            g = &D_80068690[*s];
        } else if (c >= 'a' && c <= 'z') {
            g = &D_80068690[*s - 32];
        } else if (c >= '0' && c <= '9') {
            g = &D_80068690[*s + 6];
        } else if (c == '&') {
            g = &D_80068890;
        } else if (c == '.') {
            g = &D_80068CE0;
        } else if (c == '_') {
            g = &D_80068DC8;
        }
        if (*s == ' ') {
            adv = 5;
        } else {
            adv = g->unk4;
        }
        s++;
        w += adv - 1;
    }
    return w;
}

TextSize func_8002AF70(u8 *s) {
    TextSize r;
    s32 w;
    s32 maxw;
    s32 h;
    s32 c;
    s32 code;
    Unk7D4 *font;
    Unk7D4 *kanji;
    u8 th;
    u8 th2;
    s32 idx;
    Unk7D4 *tbl;

    memset(&r, 0, 4);
    w = 0;
    maxw = 0;
    font = g_gameState->unk1C->unk7D4;
    kanji = g_gameState->unk1C->unk84C;
    h = 15;
    th2 = 0;
    if (s == NULL) {
        return r;
    }
    th = font->unk4 >> 8;
    if (kanji != NULL) {
        th2 = kanji->unk0 >> 8;
    }
    while ((c = *s) != 0xFF) {
        if (c == 0xF9) {
            s++;
            if (*s == 0x43) {
                s++;
                w += (s8)*s;
            } else if (*s == 0x44) {
                s++;
                w = *s;
            } else if (*s == 0x40) {
                s++;
                w += func_8002BEA4(D_8007BBA8[*s], 0);
            } else if (*s == 0x68) {
                s++;
                w += func_8002BEA4(D_800770F0[*s], 0);
            } else if (*s == 0x69) {
                s++;
                w += D_8006BBEC[0](D_800770B0[*s]);
            } else if (*s == 0x6A) {
                s++;
                w += func_8002BEA4(D_800770F0[*s], 1);
            } else if (*s == 0x6B) {
                s++;
                w += D_8006BBEC[1](D_800770B0[*s]);
            } else if (*s == 0x6E) {
                s++;
                w += func_8002BEA4(D_800770F0[*s], 2);
            } else if (*s == 0x6F) {
                s++;
                w += D_8006BBEC[2](D_800770B0[*s]);
            } else if (*s == 0x6C) {
                s++;
                w += func_8002BEA4(D_800770F0[*s], 3);
            } else if (*s == 0x6D) {
                s++;
                w += D_8006BBEC[3](D_800770B0[*s]);
            } else if (*s == 0x4B) {
                s++;
                w += D_800686D0[*s].unk4;
            } else if (*s == 0x4C) {
                s++;
                if (*s == 0xFF) {
                    w += D_800686D0[D_8006A20E + 0x5C].unk4;
                }
            } else if (*s == 0x4D) {
                s++;
                w += D_800686D0[D_80077130[*s]].unk4;
            } else if (*s == 0x4E) {
                s++;
                if (D_80077130[*s] == 0xFF) {
                    w += D_800686D0[D_8006A20E + 0x5C].unk4;
                }
            } else if (*s == 0x49) {
                s++;
                h += *s;
            } else if (*s == 0x4A) {
                s++;
                h -= *s;
            } else {
                w += func_8002A730(*s);
            }
        } else if (c == 0xF7) {
            h += 15;
            if (maxw < w) {
                maxw = w;
            }
            w = 0;
        } else if (kanji != NULL && c >= th2) {
            code = (c << 8) + s[1];
            c = code - kanji->unk0;
            s++;
            w += 1 + (*(kanji->unkC + c) >> 28);
        } else if (c < th) {
            w += 1 + (*(font->unkC + c) >> 28);
        } else {
            code = (c << 8) + s[1];
            c = code - font->unk4 + font->unk6;
            s++;
            w += 1 + (*(font->unkC + c) >> 28);
        }
        s++;
    }
    r.w = maxw > w ? maxw : w;
    r.h = h;
    return r;
}

s32 func_8002B43C(s32 arg0) {
    D_80077140 = arg0;
    D_80077144 = arg0 + 0x4000;
    memset(D_80077144, 0, 0x3F0);
    return D_80077144;
}

void func_8002B47C(s32 arg0) {
    s32 x = (arg0 & 1) * 0x54;
    s32 y = arg0 >> 1;
    s32 i;
    u8 *name = g_gameState->unk1C->party[arg0]->unk0;

    for (i = 0; i < 7; i++) {
        if (*name == 0xFF) {
            break;
        }
        x = func_80028DB0(x, y, *name++);
    }
}

void ui_drawLabel(s32 arg0, s32 arg1, s32 otz, s32 arg3, s32 arg4) {
    g_primPtr = ui_buildLabel(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void ui_drawSmallDigits(s32 arg0, s32 arg1, s32 otz, u8 *arg3, s32 arg4) {
    g_primPtr = func_80029D60(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void ui_drawDigits(s32 arg0, s32 arg1, s32 otz, u8 *arg3, s32 arg4) {
    g_primPtr = func_8002B644(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void text_drawBlock(s32 arg0, s32 arg1, s32 otz, u8 *arg3, s32 arg4) {
    g_primPtr = func_8002920C(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

u8 *func_8002B644(s32 x, s32 y, u32 *ot, u8 *s, s32 style, void *pkt) {
    SprtWords *p = pkt;
    DrTpageWords *t;
    u32 rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    s32 c;
    s32 u;

    if (style == 2) {
        rgbc = GPU_RGBC(GPU_SPRT | GPU_ABE, 0x606060);
    }
    while ((c = *s) != 0) {
        if ((u8)c != ' ') {
            do { } while (0);
            if ((u8)c != '/') {
                u = 0x1E8;
                u = c * 8 - u;
            } else {
                u = 0xE8;
            }
            setlen(p, 4);
            p->rgbc = rgbc;
            p->xy = (x & 0xFFFF) | (y << 16);
            p->wh = 0xA0008;
            p->uvclut = ((u & 0xFF) | 0x4000) | ((D_8006BBB4[style] + 0x3EE4) << 16);
            addPrim(ot, p);
            p++;
        }
        s++;
        x += 7;
    }
    t = (DrTpageWords *)p;
    setlen(t, 1);
    t->code = get_mode(1, 1, 0x1F);
    addPrim(ot, t);
    return (u8 *)(t + 1);
}

u8 *func_8002B7B0(s32 x, s32 y, u32 *ot, u32 a, s32 mode, s32 *out, SprtWords *p) {
    u32 rgbc = GPU_CODE(GPU_SPRT | GPU_ABE);
    s32 dx = (a >> 20) & 3;
    s32 h = (a >> 16) & 0xF;
    s32 n = a >> 28;
    if (mode != 1) {
        rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    }
    setlen(p, 4);
    p->xy = ((x + dx) & 0xFFFF) | (y << 16);
    p->rgbc = rgbc;
    p->uvclut = (a & 0x40FFFF) + (D_8006BBA0[mode] << 16);
    p->wh = h | 0xD0000;
    addPrim(ot, p);
    if (out != NULL) {
        *out = n + 1;
    }
    return (u8 *)(p + 1);
}

u8 *func_8002B890(s32 x, s32 y, u32 *ot, u32 mask, u8 b, s32 *c, u8 *pkt) {
    s32 tmp[2];
    s32 i;
    for (i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            pkt = ui_buildSprite(x + D_8006BC0C[i].unk0, y + D_8006BC0C[i].unk1, ot, D_8006BC0C[i].unk2, b, tmp, pkt);
        }
    }
    return ui_buildSprite(x, y, ot, 0xC, b, c, pkt);
}

s32 func_8002B980(u8 *s, s32 idx) {
    return D_8006BBEC[idx](s);
}

s32 func_8002B9B8(u8 *s) {
    s32 w = 0;
    u8 c;

    while ((c = *s) != 0) {
        if (c != 0x40 && c != 0xDE && c != 0xDF) {
            w += 7;
        }
        s++;
    }
    return w;
}

s32 func_8002BA10(u8 *s) {
    s32 w = 0;
    u8 c;

    while ((c = *s) != 0) {
        if (c != 0x40 && c != 0xDE && c != 0xDF) {
            w += 7;
        }
        s++;
    }
    return w;
}

s32 func_8002BA68(s32 arg0) {
    return func_8002AF70((u8 *)arg0).w;
}

u8 *func_8002BA90(u32 *ot, DrTpageWords *p) {
    setlen(p, 1);
    p->code = get_mode(1, 1, 0x1E);
    addPrim(ot, p);
    return (u8 *)(p + 1);
}

void text_formatNumber(s32 n, u8 *buf, s32 width, u8 ascii) {
    s32 base = 0;
    s32 fill;
    s32 i;
    s32 q;
    s32 d;
    u8 *end;

    if (ascii) {
        base = '0';
    }
    fill = ' ';
    if (!ascii) {
        fill = 0xF;
    }
    if (width < 0) {
        func_8002BBE8(n, buf, ascii);
        return;
    }
    memset(buf, fill, width);
    for (i = 0; i < width; i++) {
        d = n % 10;
        q = n / 10;
        buf[width - i - 1] = d + base;
        if (q == 0) {
            break;
        }
        n = q;
    }
    end = buf + width;
    if (!ascii) {
        *end = 0xFF;
    } else {
        *end = 0;
    }
}

void func_8002BBE8(s32 v, u8 *buf, s32 ascii) {
    u8 tmp[16];
    s32 n = 0;
    s32 i;
    s32 base;
    s32 minus;
    s32 term;
    if (ascii == 0) {
        base = 0;
        minus = 0xB;
        term = 0xFF;
    } else {
        base = '0';
        minus = '-';
        term = 0;
    }
    if (v == 0) {
        *buf++ = base;
    } else {
        if (v < 0) {
            *buf++ = minus;
            v = -v;
        }
        while (v != 0) {
            tmp[n++] = base + v % 10;
            v /= 10;
        }
        for (i = 0; i < n;) {
            *buf++ = tmp[n - ++i];
        }
    }
    *buf = term;
}

s32 func_8002BCC0(u8 *s) {
    u8 *start = s;
    u8 lim = g_gameState->unk1C->unk7D4->unk4 >> 8;
    s32 c;

    while ((c = *s) != 0xFF) {
        s++;
        if (c == 0xF9) {
            s++;
            if ((u32)(s[-2] - 0x43) < 2) {
                s++;
            }
        } else if (c >= lim) {
            s++;
        }
    }
    return s - start;
}

u8 *func_8002BD54(u8 *dst, u8 *src) {
    u8 *start = dst;
    u8 lim = g_gameState->unk1C->unk7D4->unk4 >> 8;
    s32 c;

    while ((c = *src) != 0xFF) {
        *dst++ = *src++;
        if (c == 0xF9) {
            *dst++ = *src++;
            if ((u32)(src[-2] - 0x43) < 2) {
                *dst++ = *src++;
            }
        } else if (c >= lim) {
            *dst++ = *src++;
        }
    }
    *dst = 0xFF;
    return start;
}

s32 func_8002BE10(s32 arg0) {
    return D_8006BC18[config_remapButton(arg0)];
}

void func_8002BE44(s32 i, u8 *v) {
    D_800770B0[i] = v;
}

void func_8002BE5C(s32 i, s32 v) {
    D_800770F0[i] = v;
}

void func_8002BE74(s32 i, s32 v) {
    D_80077130[i] = v;
}

s32 func_8002BE88(void) {
    return D_800770AC;
}

void func_8002BE98(s32 v) {
    D_800770AC = v;
}

s32 func_8002BEA4(s32 n, s32 idx) {
    s32 r = 0;
    s32 step = D_8006BBFC[idx];

    while (n != 0) {
        n /= 10;
        r += step;
    }
    return r - 1;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/text", D_80010BD0);
