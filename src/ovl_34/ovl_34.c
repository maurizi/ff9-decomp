#include "ovl_34.h"

INCLUDE_RODATA("asm/us/ovl_34/nonmatchings/ovl_34", D_801EDF00);

void func_801EEA1C(void) {
    ConfigMenu *m;
    s32 i;

    func_8003575C(D_801EDF38);
    func_8002EDFC(&D_801F1558.unk3C, 0xD, 9, 1, 0xB4);
    m = &D_801F1558;
    m->unk0 = 0;
    m->unk2 = 0;
    m->unk3 = 0;
    m->unk4 = 0;
    m->unk5 = 0;
    m->unk6 = 0;
    m->unk50[0] = (u8 *)0x801F9F00;
    m->unk50[1] = (u8 *)0x801FBF00;
    m->unk4C = m->unk50[g_gameState->unk1C->unk8 ^ 1];
    m->unk1 = 0;
    for (i = 0; i < 9; i++) {
        if (g_gameState->unk1C->unk3C[i].unk34.b.unkB) {
            D_801F1558.unk8[D_801F1558.unk1] = func_80033964(g_gameState->unk1C->unk3C[i].unk34.b.row);
            D_801F1558.unk11[D_801F1558.unk1] = i;
            D_801F1558.unk1++;
        }
    }
    func_8003556C(D_800355DC);
}

void func_801EEB68(void) {
    ScrollPos sp;
    RECT r;
    RECT a, b;
    s32 i;
    MenuList *l = &D_801F1558.unk3C;

    func_8002D9B4(NULL, 0x17, 4);
    func_801EFF58(l, &sp);
    i = 0;
    r.x = 8;
    r.w = 0x12C;
    r.y = 0;
    r.h = 0x14;
    for (; i < sp.unk0; i++) {
        r.y = i * 0x14 + (s16)(sp.unk2 + 0x2A);
        func_801F0550(&r, 0x17, sp.unk1 + i);
        func_8002DEE4(&r, 0x17, 0);
    }
    if (l->unk8 & 4) {
        a.x = 8;
        a.y = 0x2A;
        a.w = 0x12C;
        a.h = 0xA8C;
        func_801F043C(l, &a, &b);
        func_8002D9B4(&b, 0x17, 4);
        func_801F049C();
    }
    r.y = 0x2A;
    r.h = 0xB4;
    func_8002D9B4(&r, 0x17, 4);
}

void func_801EECB4(void) {
    ConfigMenu *m = &D_801F1558;
    s32 dir;

    if (m->unk4 == 0 && m->unk3C.unk8 == 0) {
        if (g_padRepeat & 0x10000) {
            snd_playSe(0x65);
            func_800361CC(0);
            return;
        }
        if (g_padRepeat & 0x20000) {
            func_801EF938();
            return;
        }
        func_8002EF58(&m->unk3C, g_padRepeat);
        func_8002EEF0(&m->unk3C, g_padRepeat);
        func_801F0380(&m->unk3C, g_padRepeat);
        func_801F02CC(&m->unk3C, g_padRepeat);
        dir = 0;
        if ((g_padRepeat & 0x80) || (dir = 1, g_padRepeat & 0x20)) {
            func_801EF50C(dir);
        }
    }
}

void func_801EED9C(void) {
    s32 i;

    if (!func_801F125C()) {
        if (g_padRepeat & 8) {
            snd_playSe(0x65);
            for (i = 0; i < 2; i++) {
                func_80063118(&D_801F1558.unk1C[i], D_801F1464, 0);
            }
            func_8002397C();
            return;
        }
        if (g_padRepeat & 0x10) {
            snd_playSe(0x67);
            D_801F1558.unk0 = D_801F1558.unk0 != 0 ? D_801F1558.unk0 - 1 : 7;
            return;
        }
        if (g_padRepeat & 0x40) {
            snd_playSe(0x67);
            D_801F1558.unk0 = D_801F1558.unk0 >= 7 ? 0 : D_801F1558.unk0 + 1;
            return;
        }
        for (i = 0; i < 8; i++) {
            if (g_padPressed & D_801F1474[i]) {
                snd_playSe(0x67);
                func_801F11C4(D_801F1558.unk0, i);
                return;
            }
        }
    }
}

void func_801EEEF8(void) {
    s32 i;

    if (!func_801F125C()) {
        if (g_padRepeat & 0x10000) {
            snd_playSe(0x65);
            for (i = 0; i < 2; i++) {
                func_80063118(&D_801F1558.unk1C[i], D_801F14AC, 0);
            }
            return;
        }
        if ((g_padRepeat & 0x80) && (((s32)(g_gameState->unk24 >> 20) >> D_801F1558.unk8[D_801F1558.unk0]) & 1)) {
            snd_playSe(0x67);
            g_gameState->unk24 = (g_gameState->unk24 & 0xFFFFF) | (((g_gameState->unk24 >> 20) & ~(1 << D_801F1558.unk8[D_801F1558.unk0])) << 20);
            return;
        }
        if ((g_padRepeat & 0x20) && !(((s32)(g_gameState->unk24 >> 20) >> D_801F1558.unk8[D_801F1558.unk0]) & 1)) {
            snd_playSe(0x67);
            g_gameState->unk24 = (g_gameState->unk24 & 0xFFFFF) | (((g_gameState->unk24 >> 20) | (1 << D_801F1558.unk8[D_801F1558.unk0])) << 20);
            return;
        }
        if (g_padRepeat & 0x10) {
            snd_playSe(0x67);
            D_801F1558.unk0 = D_801F1558.unk0 != 0 ? D_801F1558.unk0 - 1 : D_801F1558.unk1 - 1;
        } else if (g_padRepeat & 0x40) {
            snd_playSe(0x67);
            D_801F1558.unk0 = D_801F1558.unk0 >= D_801F1558.unk1 - 1 ? 0 : D_801F1558.unk0 + 1;
        }
    }
}

void func_801EF110(void) {
    RECT *r = &D_801F1558.unk1C[0].unk4;
    RECT *r2;
    s32 i = 0;

    func_8002B524(r->x + 0xB, r->y - 3, 0x11, D_800703FC.unk84, 0);
    func_8002B524(r->x + 0x2D, r->y - 3, 0x11, D_800703FC.unk8C, 0);
    r2 = &D_801F1558.unk1C[1].unk4;
    func_8002B5FC(r2->x + 0x2A, r2->y + 8, 0x11, func_80035774(0), 0);
    for (; i < 8; i++) {
        func_800275DC(r->x + 0x10, r->y + 0xB + i * 0x11, 0x11, D_801F14FC[(g_gameState->unk28 >> (i * 4)) & 7].unk1, 0);
        func_801F0F48(r->x + 0x27, r->y + 0xE + i * 0x11, 0x11, D_801F14F8, 0);
        func_800275DC(r->x + 0x3A, r->y + 0xB + i * 0x11, 0x11, D_801F14FC[i].unk1, 0);
        func_801F0FE8(r->x + 0x54, r->y + 0xC + i * 0x11, 0x11, D_801F14FC[i].unk0, 0);
    }
}

void func_801EF2B4(void) {
    RECT *r = &D_801F1558.unk1C[0].unk4;
    RECT *r2;
    s32 i;
    u8 on;

    r2 = &D_801F1558.unk1C[1].unk4;
    func_8002B5FC(r2->x + 0x2A, r2->y + 8, 0x11, func_80035774(1), 0);
    for (i = 0; i < D_801F1558.unk1; i++) {
        on = ((s32)(g_gameState->unk24 >> 20) >> D_801F1558.unk8[i]) & 1;
        func_8002B5FC(r->x + 0xE, r->y + 0xA + i * 0x11, 0x11, g_gameState->unk1C->unk3C[D_801F1558.unk11[i]].unk0, 0);
        func_801F0F94(r->x + 0x60, r->y + 0xA + i * 0x11, 0x11, 0xF8, on);
        func_801F0F94(r->x + 0x92, r->y + 0xA + i * 0x11, 0x11, 0x100, on == 0);
    }
}

void func_801EF3F8(void) {
    s32 i;

    i = D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6;
    if (!func_8002F810()) {
        D_80077DE8.unk0 = 0;
        D_80077DE8.unk4 = 0x15;
        D_80077DE8.unk2 = D_801F1558.unk3C.unk6 * 0x14 + 0x31;
        if (D_801F1558.unk2 == 0 || (D_80077E54 & 3)) {
            func_800355E4();
        }
        if (D_801F1558.unk2 != 0 && !func_801F125C()) {
            if (i == 1) {
                D_80077DE8.unk0 = 0x66;
                D_80077DE8.unk2 = D_801F1558.unk0 * 0x11 + 0x33;
            } else if (i == 5) {
                D_80077DE8.unk0 = 0x65;
                D_80077DE8.unk2 = D_801F1558.unk0 * 0x11 + 0x33;
            }
            D_80077DE8.unk4 = 0x10;
            func_800355E4();
        }
    }
}

void func_801EF50C(u8 v) {
    u8 changed = 0;

    switch (D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6) {
    case 0:
        if (CFG.b0 != v) {
            changed = 1;
        }
        CFG.b0 = v;
        break;
    case 1:
        if (CFG.b1 != v) {
            changed = 1;
        }
        CFG.b1 = v;
        break;
    case 2:
        if (CFG.b2 != v) {
            changed = 1;
        }
        CFG.b2 = v;
        break;
    case 3:
        if (CFG.b3 != v) {
            changed = 1;
        }
        CFG.b3 = v;
        break;
    case 4:
        if (CFG.b4 != v) {
            changed = 1;
        }
        CFG.b4 = v;
        break;
    case 5:
        if (CFG.b5 != v) {
            changed = 1;
        }
        CFG.b5 = v;
        break;
    case 6:
        if (CFG.b6 != v) {
            changed = 1;
        }
        CFG.b6 = v;
        break;
    case 9:
        if (CFG.b17 != v) {
            changed = 1;
        }
        CFG.b17 = v;
        break;
    case 10:
        if (CFG.b18 != v) {
            changed = 1;
        }
        CFG.b18 = v;
        break;
    case 11:
        if (CFG.b19 != v) {
            changed = 1;
        }
        CFG.b19 = v;
        break;
    case 7:
        if (v != 0 ? (s32)CFG.b8 < 2 : CFG.b8 != 0) {
            D_801F1558.unk4 = v != 0 ? 1 : 2;
            changed = 1;
        }
        break;
    case 8:
        if (v != 0 ? (s32)CFG.b14 < 6 : CFG.b14 != 0) {
            D_801F1558.unk4 = v != 0 ? 1 : 2;
            changed = 1;
        }
        break;
    case 12:
        if (!func_801F126C()) {
            snd_playSe(0x66);
            return;
        }
        if (CFG.b7 != v) {
            changed = 1;
            CFG.b7 = v;
            if (v == 0) {
                D_801F1558.unk3 = changed;
                D_801F1558.unk6 = 0;
                func_801F12D0(1);
            }
        }
        break;
    }
    if (changed) {
        snd_playSe(0x67);
        func_8002397C();
    }
}

void func_801EF938(void) {
    s32 on;
    s32 i;
    s32 j;

    i = D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6;
    on = 1;
    if ((i == on && ((g_gameState->unk24 >> i) & on) == on) || (i == 5 && ((g_gameState->unk24 >> 5) & 1) == on)) {
        D_801F1558.unk0 = 0;
        D_801F1558.unk2 = 1;
        for (j = 0; j < 2; j++) {
            D_801F1558.unk1C[j].unk4 = D_801F1534[j];
            func_80063118(&D_801F1558.unk1C[j], D_801F1524, 1);
        }
        snd_playSe(0x67);
    }
}

void func_801EFA54(void) {
    s32 i;
    s32 d;

    if (D_801F1558.unk4 != 0 && ++D_801F1558.unk5 >= 5) {
        i = D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6;
        d = 0xFF;
        if (D_801F1558.unk4 == 1) {
            d = 1;
        }
        D_801F1558.unk5 = 0;
        D_801F1558.unk4 = 0;
        switch (i) {
        case 7:
            g_gameState->unk24 = (g_gameState->unk24 & ~0x700) | (((d + ((g_gameState->unk24 >> 8) & 7)) & 7) << 8);
            break;
        case 8:
            g_gameState->unk24 = (g_gameState->unk24 & ~0x1C000) | (((d + ((g_gameState->unk24 >> 14) & 7)) & 7) << 14);
            break;
        }
    }
}

void func_801EFB38(void) {
    s32 i;
    s32 j;

    D_80077DE8.unk8 = NULL;
    i = D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6;
    if (D_801F1558.unk2 == 0) {
        D_80077DE8.unk8 = func_80035774(i + 2);
        for (j = 0; j < 2; j++) {
            func_8002BE44(j, func_80035774(D_801F13F0[i].unk1[j]));
        }
        return;
    }
    if (!func_801F125C()) {
        if (i == 1) {
            D_80077DE8.unk8 = func_80035774(D_801F1558.unk0 + 0xF);
        } else if (i == 5) {
            D_80077DE8.unk8 = func_80035774(0x17);
        }
    }
}

void func_801EFC20(s32 x, s32 y, s32 otz, s32 pos, s32 w) {
    GaugeIcon *ic;
    GaugeIcon *mid;
    GaugeIcon *ec;
    s32 width;
    SprtWords *p;
    LineWords *l;
    RECT r;
    s32 i;

    ic = D_80068988;
    mid = &ic[1];
    ec = &ic[2];
    width = w + 8;
    if (pos != 0) {
        for (i = 0; i < 3; i++) {
            s32 k = i + 2;
            s32 yy = (y + k) << 16;
            setlen(g_primPtr, 3);
            l = (LineWords *)g_primPtr;
            l->rgbc = D_801F154C[i];
            l->xy0 = (x + 4) | yy;
            l->xy1 = (pos + x + 3) | yy;
            addPrim(&g_ot[otz], l);
            g_primPtr = (u8 *)(l + 1);
        }
    }
    func_80027710(x, y, otz, ic, 0);
    func_80027710(x + width - ec->unk4, y, otz, ec, 0);
    SetTexWindow(g_primPtr, &D_801F1544);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 0xC;
    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = 0x64808080;
    p->xy = ((x + ic->unk4) & 0xFFFF) | (y << 16);
    p->uvclut = mid->unk2 << 16;
    p->wh = (width - (ic->unk4 + ec->unk4)) | (mid->unk5 << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
    r.x = mid->unk0;
    r.y = mid->unk1;
    r.w = mid->unk4;
    r.h = mid->unk5;
    SetTexWindow(g_primPtr, &r);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 0xC;
    SetDrawTPage(g_primPtr, 1, 1, 0x1F);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

ScrollPos *func_801EFF58(MenuList *l, ScrollPos *p) {
    s32 q;
    s32 f;
    s32 d;

    f = l->unk8;
    if (f & 4) {
        if (f & 2) {
            d = l->unkD - l->unk9;
            q = (d + 19) / 20;
            p->unk0 = q;
            p->unk1 = l->unk5 + (l->unk3 - q);
            p->unk2 = (l->unk3 - p->unk0) * 20;
        } else {
            d = l->unkD - l->unk9;
            p->unk0 = (d + 19) / 20;
            p->unk1 = l->unk5;
            p->unk2 = 0;
        }
    } else if (f & 2) {
        p->unk0 = l->unk3 + 1;
        p->unk1 = l->unk5 - 1;
        p->unk2 = l->unk9 - 20;
    } else if (f & 1) {
        p->unk1 = l->unk5;
        p->unk0 = l->unk3 + 1;
        p->unk2 = -l->unk9;
    } else {
        p->unk1 = l->unk5;
        p->unk0 = l->unk3;
        p->unk2 = 0;
    }
    return p;
}

void func_801F00A8(void) {
    func_801F00F8();
    func_801F0184();
    func_801F025C();
}

void func_801F00D8(void) {
    func_8003556C(NULL);
}

void func_801F00F8(void) {
    s32 i;
    void (*fn)(void);

    i = D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6;
    if (!func_8002F810()) {
        if (D_801F1558.unk2 == 0) {
            func_801EECB4();
            return;
        }
        fn = D_801F1424[i];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F0184(void) {
    s32 k = 0x2000;

    D_801F1558.unk58 = D_801F1558.unk50[g_gameState->unk1C->unk8 ^ 1] - (D_801F1558.unk4C - k);
    D_801F1558.unk4C = D_801F1558.unk50[g_gameState->unk1C->unk8];
    func_8002B524(0x10, 0x23, 0x16, D_80070430, 0);
    func_80025A28(8, 0x26, 0x16, 0, 0x12C, (g_gameState->unk24 >> 18) & 1);
    func_801EEB68();
    func_801F0BE8();
    func_801F0C78();
    func_801EF3F8();
}

void func_801F025C(void) {
    if (func_8003561C()) {
        func_801EFB38();
    }
    if (D_801F1558.unk2 == 0) {
        func_801EFA54();
    } else {
        func_801F1074();
    }
    func_801F10C8();
    func_801F1168();
}

void func_801F02CC(MenuList *l, u32 pad) {
    s32 h;
    s32 top;

    if ((pad & 0x100000) && !(g_padHeld & 0x200000) && l->unk5 != 0) {
        h = l->unkD;
        l->unk9 = h >> 2;
        l->unk8 = 7;
        top = l->unk5 - l->unk3;
        l->unkE = (h - l->unk9 + 0xE) / 15;
        if (top < 0) {
            top = 0;
        }
        l->unkC = top;
        snd_playSe(0x67);
    }
}

void func_801F0380(MenuList *l, u32 pad) {
    s32 h;
    s32 page;
    s32 top;
    s32 max;

    if ((pad & 0x200000) && l->unk5 < l->unk2 - l->unk3) {
        h = l->unkD;
        l->unk9 = h >> 2;
        page = l->unk3;
        max = l->unk2 - page;
        l->unk8 = 5;
        top = l->unk5 + page;
        l->unkE = (h - l->unk9 + 0xE) / 15;
        if (top < max) {
            max = top;
        }
        l->unkC = max;
        snd_playSe(0x67);
    }
}

void func_801F043C(MenuList *m, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (m->unk8 & 2) {
        dst->y = src->y + m->unk9;
    } else {
        dst->y = src->y;
    }
    dst->h = src->h - m->unk9;
}

void func_801F049C(void) {
    ScrollPos sp;
    RECT r;
    s32 i;
    s32 y;

    func_801F1334(&D_801F1558.unk3C, &sp);
    i = 0;
    r.x = 8;
    r.w = 0x12C;
    r.y = 0;
    r.h = 0x14;
    for (; i < sp.unk0; i++) {
        r.y = i * 0x14 + (s16)(sp.unk2 + 0x2A);
        func_801F0550(&r, 0x17, sp.unk1 + i);
        func_8002DEE4(&r, 0x17, 0);
    }
}

void func_801F0550(RECT *r, s32 otz, s32 i) {
    func_800269F4(r->x + 0x70, r->y + 1, otz, 4 - r->h);
    D_801F14BC[i](r, otz);
}

void func_801F05D0(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[0].unk0, 0);
    func_801F0D20(r, otz, g_gameState->unk24 & 1, 0);
}

void func_801F0640(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[1].unk0, 0);
    func_801F0D20(r, otz, (g_gameState->unk24 >> 1) & 1, 1);
}

void func_801F06B4(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[2].unk0, 0);
    func_801F0D20(r, otz, (g_gameState->unk24 >> 2) & 1, 2);
}

void func_801F0728(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[3].unk0, 0);
    func_801F0D20(r, otz, (g_gameState->unk24 >> 3) & 1, 3);
}

void func_801F079C(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[4].unk0, 0);
    func_801F0D20(r, otz, (g_gameState->unk24 >> 4) & 1, 4);
}

void func_801F0810(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[5].unk0, 0);
    func_801F0D20(r, otz, (g_gameState->unk24 >> 5) & 1, 5);
}

void func_801F0884(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[6].unk0, 0);
    func_801F0D20(r, otz, (g_gameState->unk24 >> 6) & 1, 6);
}

void func_801F08F8(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[7].unk0, 0);
    func_801F0DD8(r, otz, (g_gameState->unk24 >> 8) & 7, 2, 7);
}

void func_801F0974(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[8].unk0, 0);
    func_801F0DD8(r, otz, (g_gameState->unk24 >> 14) & 7, 6, 8);
}

void func_801F09F0(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[9].unk0, 0);
    func_801F0D20(r, otz, (g_gameState->unk24 >> 17) & 1, 9);
}

void func_801F0A64(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[10].unk0, 0);
    func_801F0D20(r, otz, (g_gameState->unk24 >> 18) & 1, 10);
}

void func_801F0AD8(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[11].unk0, 0);
    func_801F0D20(r, otz, (g_gameState->unk24 >> 19) & 1, 11);
}

void func_801F0B4C(RECT *r, s32 otz) {
    s32 c;
    u32 sel;

    if (func_801F126C()) {
        c = 0;
        sel = (u8)g_gameState->unk24;
        sel >>= 7;
    } else {
        sel = c = 2;
    }
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[12].unk0, c);
    func_801F0D20(r, otz, sel, 12);
}

void func_801F0BE8(void) {
    s32 i;

    i = D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6;
    if (D_801F1558.unk2 != 0) {
        func_8002DEE4(&D_801F1558.unk1C[0].unk4, 0x12, 2);
        func_8002DEE4(&D_801F1558.unk1C[1].unk4, 0x12, 2);
        if (i == 1) {
            func_801EF110();
            return;
        }
        if (i == 5) {
            func_801EF2B4();
        }
    }
}

void func_801F0C78(void) {
    MenuList *l;
    s32 blink;

    l = &D_801F1558.unk3C;
    blink = (((u32)D_80077DC0 >> 4) ^ 1) & 1;
    func_800275DC(0x12E, 0x2C, 0x14, 0xB8, l->unk5 != 0 ? blink : 1);
    func_800275DC(0x12E, 0xCF, 0x14, 0xB9, l->unk5 < l->unk2 - l->unk3 ? blink : 1);
}

void func_801F0D20(RECT *r, s32 otz, s32 sel, s32 item) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_801F0FE8(r->x + 0x90 + i * 0x5A, r->y + 6, otz, D_801F13F0[item].unk1[i], sel != i ? 2 : 0);
    }
}

void func_801F0DD8(RECT *r, s32 otz, s32 v, s32 n, s32 item) {
    s32 step = 0x12 / n;
    s32 x;
    s32 y;
    s32 pos;
    s32 cur;
    s32 w;

    pos = v * step * 5;
    x = r->x;
    y = r->y + 6;
    cur = D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6;
    w = func_8002BA68(func_80035774(D_801F14F0));
    if (item == cur && D_801F1558.unk4 != 0) {
        if (D_801F1558.unk4 == 1) {
            pos += D_801F1558.unk5 * step;
        } else {
            pos += -D_801F1558.unk5 * step;
        }
    }
    func_801F0FE8(x - w + 0x9C, y, otz, (u8)D_801F14F0, 0);
    func_801F0FE8(x + 0x103, y, otz, (u8)D_801F14F4, 0);
    func_801EFC20(x + 0x9E, y, otz, pos, 0x5A);
}

void func_801F0F48(s32 x, s32 y, s32 otz, u8 *s, s32 arg4) {
    D_801F1558.unk4C = func_80029D60(x, y, &g_ot[otz], s, arg4, D_801F1558.unk4C);
}

void func_801F0F94(s32 x, s32 y, s32 otz, s32 n, u8 arg4) {
    s32 buf[2];

    D_801F1558.unk4C = func_80027834(x, y, &g_ot[otz], n, arg4, buf, D_801F1558.unk4C);
}

void func_801F0FE8(s32 x, s32 y, s32 otz, s32 text, s32 color) {
    D_801F1558.unk4C = func_8002920C(x, y - 4, &g_ot[otz], func_80035774(text & 0xFF), color, D_801F1558.unk4C);
}

void func_801F1074(void) {
    if (D_801F1558.unk1C[0].unkC != 0) {
        func_800631AC(&D_801F1558.unk1C[0]);
    }
    if (D_801F1558.unk1C[1].unkC != 0) {
        func_800631AC(&D_801F1558.unk1C[1]);
    }
}

void func_801F10C8(void) {
    MenuList *l = &D_801F1558.unk3C;
    s32 v;
    s32 dec;

    if (l->unk8 != 0) {
        if (!(l->unk8 & 4)) {
            dec = l->unk8 & 2;
            l->unk9 += 4;
            if (l->unk9 >= 0x14) {
                v = l->unk5;
                if (dec) {
                    v--;
                } else {
                    v++;
                }
                l->unk5 = v;
                l->unk8 = 0;
                l->unk9 = 0;
            }
        } else {
            l->unk9 += 9;
            if (l->unk9 >= 0xB4) {
                l->unk8 = 0;
                l->unk9 = 0;
                l->unk5 = l->unkC;
            }
        }
    }
}

void func_801F1168(void) {
    if (D_801F1558.unk3 != 0) {
        if (++D_801F1558.unk6 >= 9) {
            D_801F1558.unk3 = 0;
            D_801F1558.unk6 = 0;
            func_801F12D0(0);
        }
    }
}

void func_801F11C4(s32 slot, s32 v) {
    s32 i;
    s32 j;
    u32 w;
    u32 old;

    for (i = 0; i < 8; i++) {
        if (v == ((g_gameState->unk28 >> (i * 4)) & 7)) {
            j = i;
            goto found;
        }
    }
    j = 0;
found:
    w = g_gameState->unk28;
    old = (w >> (slot * 4)) & 7;
    g_gameState->unk28 = (((w & ~(7 << (slot * 4))) | (v << (slot * 4))) & ~(7 << (j * 4))) | (old << (j * 4));
}

void func_801F1250(void) {
    D_801F1558.unk2 = 0;
}

u8 func_801F125C(void) {
    return D_801F1558.unk1C[0].unkC != 0;
}

u8 func_801F126C(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (((D_80073940[i].unk0 >> 12) & 0xF) == 7) {
            return 1;
        }
        if (D_800739A8[i] == 6) {
            return 1;
        }
    }
    return 0;
}

void func_801F12D0(u8 on) {
    s32 i;

    if (on) {
        for (i = 0; i < 2; i++) {
            D_800739B0[i].unk1 = 0xFF;
            D_800739B0[i].unk0 = 1;
        }
        return;
    }
    for (i = 0; i < 2; i++) {
        D_800739B0[i].unk0 = 0;
        D_800739B0[i].unk1 = 0;
    }
}

ScrollPos *func_801F1334(MenuList *l, ScrollPos *p) {
    s32 v;

    if (l->unk8 & 2) {
        p->unk0 = (l->unk9 + 0x13) / 20;
        p->unk1 = l->unkC + (l->unk3 - p->unk0);
        p->unk2 = l->unk9 - p->unk0 * 0x14;
    } else {
        v = l->unk9;
        p->unk2 = l->unkD - v;
        p->unk0 = (v + 0x13) / 20;
        p->unk1 = l->unkC;
    }
    return p;
}

/* Defined last: its .rodata follows func_801EF50C's jump table. */
/* menu text block */
INCLUDE_RODATA("asm/us/ovl_34/nonmatchings/ovl_34", D_801EDF38);
