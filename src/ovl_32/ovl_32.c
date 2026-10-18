#include "ovl_32.h"

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EDF04);

/* text bank for this menu (count + {offset, info} table, then strings) */
INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", g_chocographText);

void func_801F7AC0(void) {
    Ovl32State *st;

    if (g_gameState->unk1C->unk844 > 24) {
        g_gameState->unk1C->unk844 = 0;
    }
    st = &D_801F9850;
    st->unk1 = 1;
    D_801F9850.unk0 = 0;
    text_setBank(g_chocographText);
    func_8003556C(func_801F84F0);
    func_80035674(1);
    setRECT(&D_801F9850.unk20, 0xE4, 8, 0x50, 0x1A);
    setRECT(&D_801F9850.unk28, 8, 8, 0xD8, 0x1A);
    setRECT(&D_801F9850.unk30, 8, 0x26, 0xB7, 0x43);
    setRECT(&D_801F9850.unk48, 8, 0x6B, 0x86, 0x71);
    setRECT(&D_801F9850.unk40, 0xBF, 0x47, 0x75, 0x22);
    setRECT(&D_801F9850.unk38, 0xBF, 0x26, 0x75, 0x21);
    setRECT(&D_801F9850.unk50, 0x8E, 0x6B, 0xA6, 0x71);
    func_8002EDFC(&D_801F9850.unk10, 0x18, 7, 1, 0x71);
    func_801F85EC();
    func_801F86D0();
    if (g_gameState->unk1C->unk844 != 0) {
        func_801F9600(g_gameState->unk1C->unk844 - 1);
    }
    D_801F9850.unk2058 = D_801F9850.unk58 + (g_gameState->unk1C->unk8 << 12);
}

void func_801F7C8C(void) {
    u8 v;
    Ovl32State *st;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        func_800325E0();
    } else if (g_padRepeat & BTN_CROSS) {
        if (D_801F9850.unk0 == 0) {
            D_801F9850.unk1 = 2;
            snd_playSe(0x67);
        } else if (g_gameState->unk1C->unk844 == 0) {
            snd_playSe(0x66);
        } else {
            g_gameState->unk1C->unk844 = 0;
            snd_playSe(0x6B);
        }
    } else {
        if (g_padRepeat & PAD_LEFT) {
            snd_playSe(0x67);
            st = &D_801F9850;
            if (st->unk0 != 0) {
                v = st->unk0 - 1;
            } else {
                v = 1;
            }
        } else if (g_padRepeat & PAD_RIGHT) {
            snd_playSe(0x67);
            st = &D_801F9850;
            if (st->unk0 == 0) {
                v = st->unk0 + 1;
            } else {
                v = 0;
            }
        } else {
            return;
        }
        st->unk0 = v;
    }
}

void func_801F7DC0(void) {
    Ovl32State *st;
    MenuList *l = &D_801F9850.unk10;
    s32 idx;
    s32 n;

    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            D_801F9850.unk1 = 1;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            st = &D_801F9850;
            idx = l->unk5 + l->cursor;
            if (st->unk8 & (1 << idx)) {
                n = st->unk4;
                if (func_801F95E8(idx) < n) {
                    snd_playSe(0x6B);
                    g_gameState->unk1C->unk844 = l->unk5 + l->cursor + 1;
                    return;
                }
            }
            snd_playSe(0x66);
            return;
        }
        func_8002EE8C(l, g_padRepeat);
    }
}

void func_801F7EB4(void) {
    RECT *r = &D_801F9850.unk30;
    s32 sel;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1E, 2);
    ui_drawLabel(r->x + 8, r->y - 3, 0x1C, D_800704AC, 0);
    sel = g_gameState->unk1C->unk844;
    if (sel != 0) {
        func_801F9518(r->x + 0x10, r->y + 0x2C, 0x1C, (D_801F9850.unkC & (1 << (sel - 1))) ? 0xD : 0xC, 0);
        text_drawBlock(r->x + 0x21, r->y + 0x2E, 0x1C, text_get(g_gameState->unk1C->unk844 + 9), 0);
        func_801F88F4(r->x + 0x4C, r->y + 0xB, 0x1C, g_gameState->unk1C->unk844 - 1, 0);
        func_801F8AB0(r->x + 0x48, r->y + 7, 0x1C, 0);
    }
}

void func_801F8010(void) {
    RECT sub;
    MenuView view;
    s32 i;
    RECT *r = &D_801F9850.unk48;
    MenuList *list;

    ui_drawLabel(r->x + 0x10, r->y - 3, 0x1C, D_80070428, 0);
    list = &D_801F9850.unk10;
    func_8002E040(r, 0x1D, 1, list);
    func_8002705C(r, 0x1D, 1);
    menuList_getView(list, &view);
    win_setClip(NULL, 0x1C, 1);
    for (i = 0; i < view.unk0; i++) {
        func_801F93FC(0, i, view.unk1 + i, &view);
    }
    if (list->unk8 & 4) {
        func_8002F40C(list, r, &sub);
        win_setClip(&sub, 0x1C, 1);
        func_801F9394();
    }
    win_setClip(r, 0x1C, 1);
}

void func_801F8130(void) {
    u8 buf[8];
    RECT *r = &D_801F9850.unk40;
    Ovl32State *st;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1E, 2);
    text_drawBlock(r->x + 9, r->y + 3, 0x1C, text_get(7), 0);
    text_drawBlock(r->x + 9, r->y + 0xF, 0x1C, text_get(8), 0);
    st = &D_801F9850;
    text_formatNumber(st->unk2, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5E, r->y + 7, 0x1C, buf, 0);
    text_formatNumber(st->unk3, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5E, r->y + 0x13, 0x1C, buf, 0);
}

void func_801F8240(void) {
    RECT *r = &D_801F9850.unk50;
    Ovl32State *st = &D_801F9850;
    s32 id;
    s32 n;
    s32 i;

    id = st->unk10.unk5 + st->unk10.cursor;
    ui_drawLabel(r->x + 8, r->y - 3, 0x1C, D_80070454, 0);
    func_8002DEE4(r, 0x1D, 1);
    func_8002705C(r, 0x1E, 1);
    if (st->unk1 == 2 && (st->unk8 & (1 << id))) {
        func_801F95E8(id);
        func_801F88F4(r->x + 0xC, r->y + 0xC, 0x1C, id, 0);
        func_801F8AB0(r->x + 8, r->y + 8, 0x1C, 0);
        st->unk2058 = func_8002920C(r->x + 0xC, r->y + 0x31, &g_ot[0x1C], text_get(id + 0x3A), 0, st->unk2058);
        text_drawBlock(r->x + 0x42, r->y + 6, 0x1C, text_get(9), 0);
        n = func_801F95E8(id);
        for (i = 0; i <= n; i++) {
            func_801F9518(r->x + 0x42 + i * 0x13, r->y + 0x16, 0x1C, i < st->unk4 ? i : 0xB, i >= st->unk4);
        }
    }
}

void func_801F8404(void) {
    func_80035674(D_801F9850.unk1 != 1);
    if (!func_8002F810()) {
        if (D_801F9850.unk1 == 1 || (D_80077DC0 & 3)) {
            D_80077DE8.unk2 = 0x11;
            D_80077DE8.unk4 = 0x1B;
            D_80077DE8.unk0 = D_801F9850.unk0 * 0x60 + 0x1C;
            func_800355E4(&D_80077DE8);
        }
        if (D_801F9850.unk1 == 2) {
            D_80077DE8.unk0 = 0;
            D_80077DE8.unk4 = 0x1B;
            D_80077DE8.unk2 = D_801F9850.unk10.cursor * 0xF + 0x74;
            func_800355E4();
        }
    }
}

void func_801F84F0(void) {
    s32 idx;

    D_80077DE8.unk8 = NULL;
    func_80035580(0);
    if (D_801F9850.unk1 == 1) {
        D_80077DE8.unk8 = text_get(D_801F9850.unk0 + 3);
    } else if (D_801F9850.unk1 == 2) {
        idx = D_801F9850.unk10.unk5 + D_801F9850.unk10.cursor;
        if (D_801F9850.unk8 & (1 << idx)) {
            func_8002BD54(D_801F9750, text_get(idx + 0x22));
            if (D_801F9850.unk4 <= func_801F95E8_inline(idx)) {
                text_append(D_801F9750, text_get(5));
            }
            D_80077DE8.unk8 = D_801F9750;
        }
    }
}

void func_801F85EC(void) {
    s32 i;
    u32 bit;

    D_801F9850.unkC = 0;
    D_801F9850.unk8 = 0;
    D_801F9850.unk3 = 0;
    D_801F9850.unk2 = 0;
    D_801F9850.unk4 = g_evtGlobalVars.b[0xBF];
    for (i = 0; i < 3; i++) {
        D_801F9850.unk8 |= g_evtGlobalVars.b[i + 0xBB] << (i * 8);
        D_801F9850.unkC |= g_evtGlobalVars.b[i + 0xB8] << (i * 8);
    }
    for (i = 0; i < 0x18; i++) {
        bit = 1 << i;
        if (D_801F9850.unk8 & bit) {
            D_801F9850.unk2++;
        }
        if (D_801F9850.unkC & bit) {
            D_801F9850.unk3++;
        }
    }
}

void func_801F86D0(void) {
    RECT r0;
    RECT r1;
    Ovl32Image *src = &D_801EF1E4[1];
    Ovl32Image *dst = &D_801EF1E4[0];
    s32 i;

    func_801F9660();
    LoadImage(&D_801F970C, (u32 *)D_801F713C);
    DrawSync(0);
    LoadImage(&D_801F9714, (u32 *)(D_801F713C + 0x1C0));
    DrawSync(0);
    LoadImage(&D_801F971C, (u32 *)D_801F7948);
    DrawSync(0);
    LoadImage(&D_801F9724, (u32 *)(D_801F7948 + 0x40));
    DrawSync(0);
    for (i = 0; i < 24; i++) {
        func_801F9668(i, &r0, &r1);
        StoreImage(&r0, (u32 *)dst->unk0);
        DrawSync(0);
        StoreImage(&r1, (u32 *)dst->unk80);
        DrawSync(0);
        LoadImage(&r0, (u32 *)src->unk0);
        DrawSync(0);
        LoadImage(&r1, (u32 *)src->unk80);
        DrawSync(0);
        dst++;
        src++;
    }
}

void func_801F8800(void) {
    RECT r0;
    RECT r1;
    s32 i;
    Ovl32Image *img = D_801EF1E4;
    s32 sel;

    if (g_gameState->mode == 3) {
        sel = g_gameState->unk1C->unk844;
        if (sel != 0) {
            func_801F9668(sel - 1, &r0, &r1);
            MoveImage(&r0, 0x280, 0xF6);
            DrawSync(0);
            MoveImage(&r1, 0x318, 0xB0);
            DrawSync(0);
        }
    }
    for (i = 0; i < 24; i++, img++) {
        func_801F9668(i, &r0, &r1);
        LoadImage(&r0, (u32 *)img->unk0);
        DrawSync(0);
        LoadImage(&r1, (u32 *)img->unk80);
        DrawSync(0);
    }
}

void func_801F88F4(s32 x, s32 y, s32 z, s32 id, u8 dim) {
    SprtWords *p;
    u16 cl;
    u16 clut = GetClut(0, id + 0x1E8);
    s32 u = (id % 3) * 0x2A;
    s32 v = (id / 3) * 0x1C;

    setlen(g_primPtr, 4);
    cl = clut;
    ((SprtWords *)g_primPtr)->rgbc = dim ? 0x66808080 : 0x64808080;
    p = (SprtWords *)g_primPtr;
    p->xy = x | (y << 16);
    p->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (cl << 16);
    p->wh = 0x1C002A;
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[z], p);
    SetDrawTPage(g_primPtr, 0, 1, 0x90);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += 8;
}

void func_801F8AB0(s32 x, s32 y, s32 otz, u8 dim) {
    u16 clut;
    s32 i;

    clut = GetClut(0x3C0, 0x1FE);
    for (i = 0; i < 4; i++) {
        setlen(g_primPtr, 9);
        ((Ft4W *)g_primPtr)->rgbc = dim ? 0x2E808080 : 0x2C808080;
        if (i < 2) {
            ((Ft4W *)g_primPtr)->x0 = D_801F972C[i].x + x;
            ((Ft4W *)g_primPtr)->y0 = D_801F972C[i].y + y;
            ((Ft4W *)g_primPtr)->x1 = D_801F972C[i].x + x + D_801F972C[i].w;
            ((Ft4W *)g_primPtr)->y1 = D_801F972C[i].y + y;
            ((Ft4W *)g_primPtr)->x2 = D_801F972C[i].x + x;
            ((Ft4W *)g_primPtr)->y2 = D_801F972C[i].y + y + D_801F972C[i].h;
            ((Ft4W *)g_primPtr)->x3 = D_801F972C[i].x + x + D_801F972C[i].w;
            ((Ft4W *)g_primPtr)->y3 = D_801F972C[i].y + y + D_801F972C[i].h;
        } else {
            ((Ft4W *)g_primPtr)->x0 = D_801F972C[i].x + x;
            ((Ft4W *)g_primPtr)->y0 = D_801F972C[i].y + y + D_801F972C[i].w;
            ((Ft4W *)g_primPtr)->x1 = D_801F972C[i].x + x;
            ((Ft4W *)g_primPtr)->y1 = y + D_801F972C[i].y - (i != 2);
            ((Ft4W *)g_primPtr)->x2 = D_801F972C[i].x + x + D_801F972C[i].h;
            ((Ft4W *)g_primPtr)->y2 = D_801F972C[i].y + y + D_801F972C[i].w;
            ((Ft4W *)g_primPtr)->x3 = D_801F972C[i].x + x + D_801F972C[i].h;
            ((Ft4W *)g_primPtr)->y3 = y + D_801F972C[i].y - (i != 2);
        }
        ((Ft4W *)g_primPtr)->u0 = D_801F972C[i].u;
        ((Ft4W *)g_primPtr)->v0 = D_801F972C[i].v;
        ((Ft4W *)g_primPtr)->u1 = D_801F972C[i].u + D_801F972C[i].w;
        ((Ft4W *)g_primPtr)->v1 = D_801F972C[i].v;
        ((Ft4W *)g_primPtr)->u2 = D_801F972C[i].u;
        ((Ft4W *)g_primPtr)->v2 = D_801F972C[i].v + D_801F972C[i].h;
        ((Ft4W *)g_primPtr)->u3 = D_801F972C[i].u + D_801F972C[i].w;
        ((Ft4W *)g_primPtr)->v3 = D_801F972C[i].v + D_801F972C[i].h;
        ((Ft4W *)g_primPtr)->tpage = 0x1F;
        ((Ft4W *)g_primPtr)->clut = clut;
        addPrim(&g_ot[otz], (Ft4W *)g_primPtr);
        g_primPtr += sizeof(Ft4W);
    }
    x += 3;
    y += 3;
    for (i = 0; i < 2; i++) {
        s32 k = D_801F974C[i];
        setlen(g_primPtr, 9);
        ((Ft4W *)g_primPtr)->rgbc = 0x2E101010;
        if (k < 2) {
            ((Ft4W *)g_primPtr)->x0 = D_801F972C[k].x + x;
            ((Ft4W *)g_primPtr)->y0 = D_801F972C[k].y + y;
            ((Ft4W *)g_primPtr)->x1 = D_801F972C[k].x + x + D_801F972C[k].w;
            ((Ft4W *)g_primPtr)->y1 = D_801F972C[k].y + y;
            ((Ft4W *)g_primPtr)->x2 = D_801F972C[k].x + x;
            ((Ft4W *)g_primPtr)->y2 = D_801F972C[k].y + y + D_801F972C[k].h;
            ((Ft4W *)g_primPtr)->x3 = D_801F972C[k].x + x + D_801F972C[k].w;
            ((Ft4W *)g_primPtr)->y3 = D_801F972C[k].y + y + D_801F972C[k].h;
        } else {
            ((Ft4W *)g_primPtr)->x0 = D_801F972C[k].x + x;
            ((Ft4W *)g_primPtr)->y0 = D_801F972C[k].y + y + D_801F972C[k].w;
            ((Ft4W *)g_primPtr)->x1 = D_801F972C[k].x + x;
            ((Ft4W *)g_primPtr)->y1 = D_801F972C[k].y + y - 1;
            ((Ft4W *)g_primPtr)->x2 = D_801F972C[k].x + x + D_801F972C[k].h;
            ((Ft4W *)g_primPtr)->y2 = D_801F972C[k].y + y + D_801F972C[k].w;
            ((Ft4W *)g_primPtr)->x3 = D_801F972C[k].x + x + D_801F972C[k].h;
            ((Ft4W *)g_primPtr)->y3 = D_801F972C[k].y + y - 1;
        }
        ((Ft4W *)g_primPtr)->u0 = D_801F972C[k].u;
        ((Ft4W *)g_primPtr)->v0 = D_801F972C[k].v;
        ((Ft4W *)g_primPtr)->u1 = D_801F972C[k].u + D_801F972C[k].w;
        ((Ft4W *)g_primPtr)->v1 = D_801F972C[k].v;
        ((Ft4W *)g_primPtr)->u2 = D_801F972C[k].u;
        ((Ft4W *)g_primPtr)->v2 = D_801F972C[k].v + D_801F972C[k].h;
        ((Ft4W *)g_primPtr)->u3 = D_801F972C[k].u + D_801F972C[k].w;
        ((Ft4W *)g_primPtr)->v3 = D_801F972C[k].v + D_801F972C[k].h;
        ((Ft4W *)g_primPtr)->tpage = 0x1F;
        ((Ft4W *)g_primPtr)->clut = clut + 0x40;
        addPrim(&g_ot[otz], (Ft4W *)g_primPtr);
        g_primPtr += sizeof(Ft4W);
    }
}

void func_801F8FF8(void) {
    D_801F9850.unk2058 = D_801F9850.unk58 + (g_gameState->unk1C->unk8 << 12);
    func_801F907C();
    func_801F90D4();
    func_801F912C();
}

void func_801F904C(void) {
    func_80035674(1);
    func_8003556C(0);
    func_801F8800();
}

void func_801F907C(void) {
    void (*f)(void);

    if (!func_8002F810()) {
        f = D_801F96E4[D_801F9850.unk1];
        if (f != NULL) {
            f();
        }
    }
}

void func_801F90D4(void) {
    func_801F9164();
    func_801F91D8();
    func_801F7EB4();
    func_801F8010();
    func_801F8130();
    func_801F92C0();
    func_801F8240();
    func_801F8404();
}

void func_801F912C(void) {
    if (D_801F9850.unk1 == 2) {
        func_8002EB74(&D_801F9850.unk10);
    }
}

void func_801F9164(void) {
    RECT *r = &D_801F9850.unk20;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1D, 2);
    text_drawBlock(r->x + 7, r->y + 6, 0x1C, text_get(0), 0);
}

void func_801F91D8(void) {
    RECT *r = &D_801F9850.unk28;
    s32 i;
    s32 x;
    u8 none = g_gameState->unk1C->unk844 == 0;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1D, 2);
    for (i = 0, x = 0x2C; i < 2; i++, x += 0x60) {
        text_drawBlock(r->x + x, r->y + 6, 0x1C, text_get(i + 1), (D_801F96F0[i] != 0 && none) ? 2 : 0);
    }
}

void func_801F92C0(void) {
    s32 i;
    s32 x;
    RECT *r = &D_801F9850.unk38;

    ui_drawLabel(r->x + 8, r->y - 3, 0x1C, D_80070424, 0);
    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1E, 2);
    for (i = 0; i < D_801F9850.unk4; i++) {
        func_801F9518(r->x + 0xB + i * 0x13, r->y + 5, 0x1C, i, 0);
    }
}

void func_801F9394(void) {
    MenuView view;
    s32 i;

    func_8002F344(&D_801F9850.unk10, &view);
    for (i = 0; i < view.unk0; i++) {
        func_801F93FC(0, i, view.unk1 + i, &view);
    }
}

void func_801F93FC(s32 col, s32 row, s32 id, MenuView *v) {
    u8 got;
    u8 near;
    s32 y;
    s32 icon;
    s32 k = 0x6B;
    s32 x;

    if (D_801F9850.unk8 & (1 << id)) {
        got = (D_801F9850.unkC & (1 << id)) != 0;
        near = func_801F95E8(id) < D_801F9850.unk4;
        x = 0xE;
        y = row * 0xF + (v->unk2 + k);
        if (got) {
            icon = near ? 0xD : 0xF;
        } else {
            icon = near ? 0xC : 0xE;
        }
        func_801F9518(x, y, 0x1C, icon, near == 0);
        text_drawBlock(x + 0x11, y, 0x1C, text_get(id + 0xA), near ? 0 : 2);
    }
}

void func_801F9518(s32 x, s32 y, s32 z, s32 icon, u8 arg4) {
    RectTbl16 rects = D_801EDF04;

    func_80027710(x, y, z, &rects.r[icon], arg4);
}

s32 func_801F95E8(s32 id) {
    return D_801F96F4[id];
}

void func_801F9600(s32 idx) {
    MenuList *l = &D_801F9850.unk10;

    if (idx > 0x10) {
        l->unk5 = 0x11;
        l->cursor = idx - 0x11;
    } else {
        l->cursor = idx % 7;
        l->unk5 = idx - l->cursor;
    }
}

void func_801F9660(void) {
}

void func_801F9668(s32 i, RECT *a, RECT *b) {
    a->x = 0;
    a->y = i + 0x1E8;
    a->w = 0x40;
    a->h = 1;
    b->x = (i % 3) * 0x15;
    b->y = (i / 3) * 0x1C + 0x100;
    b->w = 0x15;
    b->h = 0x1C;
}

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EF1E4);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EF6FC);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801F713C);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801F7948);
