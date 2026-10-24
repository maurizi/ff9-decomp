#include "ovl_38.h"

void func_801EE440(void) {
    s32 i;
    RECT *r;

    D_801F120C.unk2 = 0xFF;
    D_801F120C.unk0 = 1;
    D_801F120C.unk1 = 0;
    for (i = 0; i < 6; i++) {
        D_801F120C.unk16[i] = 0;
        D_801F120C.unk10[i] = 0;
    }
    text_setBank(g_statusMenuText);
    func_80028CF8(&D_801F120C.unk8C, D_80077E50.unk3, func_801EFAF8);
    func_801EF568(D_80077E50.unk3, 0);
    r = &D_801F120C.unk8;
    r->x = 8;
    r->y = 0x26;
    r->w = 0x12C;
    r->h = 0xB6;
    D_801F120C.unk5 = func_80035648();
    func_80035658(0);
    func_8003556C(func_801EFB08);
    D_801F120C.unk80[0] = (u8 *)0x801F5F00;
    D_801F120C.unk80[1] = (u8 *)0x801F9F00;
    D_801F120C.unk7C = D_801F120C.unk80[g_gameState->unk1C->unk8];
}

void func_801EE568(void) {
    u8 cur;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        func_800361CC(0);
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        func_801EF494();
        return;
    }
    if (g_padRepeat & BTN_R1) {
        cur = D_801F120C.unk2;
        next = func_8003380C(cur);
        if (cur != next) {
            func_801EF568(next, 2);
            snd_playSe(0x417);
        }
    } else if (g_padRepeat & BTN_L1) {
        cur = D_801F120C.unk2;
        next = func_800337D0(cur);
        if (cur != next) {
            snd_playSe(0x417);
            func_801EF568(next, 1);
        }
    }
}

void func_801EE650(void) {
    RECT r;
    Character *c;
    RECT *w;
    s16 x;
    s16 y;

    w = &D_801F120C.unk8;
    c = g_gameState->unk1C->party[D_801F120C.unk2];
    x = w->x + 0x72;
    y = w->y + 4;
    setRECT(&r, x, y, 0x5C, 0x12);
    func_800273C0(x + 0x19, y + 0x14, 0x1E, c->stones, c->maxStones);
    func_80024428(w->x + 8, w->y + 6, 0x1D, (Unk80024428 *)&D_801F120C.unk8C);
    func_80026FC4(r.x + 4, r.y + 4, 0x1D, c->unk38, 0);
    func_8002DEE4(&r, 0x1D, 0);
}

void func_801EE740(void) {
    RECT r;
    StatusTable t;
    StatusPool pool;
    RECT *w;
    Character *c;
    s32 i;
    s32 y;
    s32 k;
    s32 off;
    StatusEntry *tp;

    w = &D_801F120C.unk8;
    c = g_gameState->unk1C->party[D_801F120C.unk2];
    t = D_801EDF04;
    pool = D_801EE144;
    r.w = 0x42;
    r.h = 0x48;
    r.x = w->x + 0xE4;
    r.y = w->y + 8;
    D_801EFBB4[1] = D_801EFBB8[c->unk34.b.row][0];
    D_801EFBB4[2] = D_801EFBB8[c->unk34.b.row][1];
    for (i = 0; i < 4; i++) {
        tp = t.v;
        y = i * 15 + 6;
        off = D_801EFBB4[i] * sizeof(StatusEntry);
        k = ((StatusEntry *)((u8 *)tp + off))->unk0;
        D_801F120C.unk7C = func_8002920C(r.x + 0xC, r.y + y, g_ot + 25, pool.s + k, 0, D_801F120C.unk7C);
    }
    ui_drawLabel(r.x + 6, r.y - 3, 0x19, D_8007042C, 0);
    func_8002DEE4(&r, 0x19, 2);
    func_8002705C(&r, 0x19, 2);
}

void func_801EE9E8(void) {
    u8 buf[16];
    RECT *r;
    StatusMenu *m;
    Character *c;
    s32 y;
    s32 i;

    m = &D_801F120C;
    r = &m->unk8;
    c = g_gameState->unk1C->party[m->unk2];
    y = 0x3D;
    for (i = 0; i < 4; i++) {
        D_801F120C.unk7C = func_8002920C(r->x + 0xA, r->y + (i * 13 + y) - 4, g_ot + 0x1D, text_get(D_801EFBE0[i]), 0, D_801F120C.unk7C);
    }
    y = 0x72;
    for (i = 0; i < 5; i++) {
        D_801F120C.unk7C = func_8002920C(r->x + 0xA, r->y + (i * 13 + y) - 4, g_ot + 0x1D, text_get(D_801EFBF0[i]), 0, D_801F120C.unk7C);
    }
    text_formatNumber(c->speed, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x58, r->y + 0x3D, 0x1D, buf, 0);
    text_formatNumber(c->strength, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x58, r->y + 0x4A, 0x1D, buf, 0);
    text_formatNumber(c->magic, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x58, r->y + 0x57, 0x1D, buf, 0);
    text_formatNumber(c->spirit, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x58, r->y + 0x64, 0x1D, buf, 0);
    text_formatNumber(D_8006E0E8[c->equip[0]].unk5, buf, 3, 1);
    ui_drawSmallDigits(r->x + 0x51, r->y + 0x72, 0x1D, buf, 0);
    text_formatNumber(c->defense, buf, 3, 1);
    ui_drawSmallDigits(r->x + 0x51, r->y + 0x7F, 0x1D, buf, 0);
    text_formatNumber(c->evade, buf, 3, 1);
    ui_drawSmallDigits(r->x + 0x51, r->y + 0x8C, 0x1D, buf, 0);
    text_formatNumber(c->magicDef, buf, 3, 1);
    ui_drawSmallDigits(r->x + 0x51, r->y + 0x99, 0x1D, buf, 0);
    text_formatNumber(c->magicEva, buf, 3, 1);
    ui_drawSmallDigits(r->x + 0x51, r->y + 0xA6, 0x1D, buf, 0);
}

void func_801EED18(void) {
    u8 buf[16];
    RECT *r;
    StatusMenu *st;
    Character *c;
    u32 next;
    s32 bit;
    u8 show;

    r = &D_801F120C.unk8;
    st = &D_801F120C;
    c = g_gameState->unk1C->party[st->unk2];
    show = 0;
    if (D_8007AF00 != 0) {
        bit = c->unkA & 0x10;
        show = bit == 0;
    }
    if (show) {
        st->unk7C = func_8002920C(r->x + 0x74, r->y + 0x39, &g_ot[0x1D], text_get(0xB), 0, st->unk7C);
        ui_drawGauge(r->x + 0xB4, r->y + 0x3E, 0x1D, c->trance, 0xFF, 1, 1);
    }
    text_drawBlock(r->x + 0x74, r->y + 0x46, 0x1D, text_get(0xC), 0);
    text_drawBlock(r->x + 0x74, r->y + 0x53, 0x1D, text_get(0xD), 0);
    text_formatNumber(c->exp, buf, 7, 1);
    ui_drawSmallDigits(r->x + 0xAC, r->y + 0x4A, 0x1D, buf, 0);
    if (c->level >= 99) {
        next = c->exp;
    } else {
        next = g_levelExp[c->level];
    }
    text_formatNumber(next - c->exp, buf, 6, 1);
    ui_drawSmallDigits(r->x + 0xB3, r->y + 0x57, 0x1D, buf, 0);
}

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", D_801EE144);

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", D_801EE264);

void func_801EEF00(void) {
    RECT *r = &D_801F120C.unk8;
    StatusMenu *st;
    Character *c;
    s32 i;
    u8 id;
    u8 buf[8];

    st = &D_801F120C;
    c = g_gameState->unk1C->party[st->unk2];
    for (i = 0; i < 5; i++) {
        D_801F120C.unk7C = func_80027834(r->x + 0x74, r->y + 0x68 + i * 15 + 1, &g_ot[0x1D], i + 0xF9, 0, buf, D_801F120C.unk7C);
        D_801F120C.unk7C = func_80029D60(r->x + 0x84, r->y + 0x68 + i * 15 + 3, &g_ot[0x1D], D_801EE264, 0, D_801F120C.unk7C);
        id = c->equip[i];
        if (id != 0xFF) {
            ui_drawItemIcon(r->x + 0x8D, r->y + 0x68 + i * 15 - 1, 0x1D, id, 1);
            D_801F120C.unk7C = func_8002920C(r->x + 0x9D, r->y + 0x68 + i * 15 - 1, &g_ot[0x1D], D_8006A218 + D_80069018[id].unk0, 0, D_801F120C.unk7C);
        }
    }
}

void func_801EF0A0(void) {
    s32 i, j;
    s32 base = 0;
    RECT *r;
    s32 z;

    for (i = 0; i < D_801F120C.unk3; i++) {
        if (D_801F120C.unk10[i] != 0) {
            r = &D_801F120C.unk1C[i].unk4;
            for (j = 0; j < 8; j++) {
                if (base + j < D_801F120C.unk4) {
                    func_801EF258(r->x + 0xA, r->y + 8 + j * 15, 0x14 - i, base + j, D_801F120C.unk16[i]);
                }
            }
            z = 0x14 - i;
            ui_drawLabel(r->x + 6, r->y - 3, z, D_800703FC.unk4, 0);
            if (D_801F120C.unk16[i] != 0) {
                D_801EFC04[1] = i + 0x31;
                ui_drawLabel(r->x + 0x90, r->y - 3, z, D_800703FC.unk88, 0);
                ui_drawLabel(r->x + 0x9A, r->y - 3, z, (s32)D_801EFC04, 0);
            }
            func_8002DEE4(r, z, 2);
            func_8002705C(r, z, 2);
        }
        base += 8;
    }
}

void func_801EF258(s32 x, s32 y, s32 z, s32 i, u8 draw) {
    Character *c;
    u8 *name;
    u8 ok;
    s32 id;
    s32 max;
    s32 cur;
    s32 icon;
    u8 flag;
    ActiveAbility *a;
    Unk8006D128 *sup;

    c = g_gameState->unk1C->party[D_801F120C.unk2];
    ok = func_80034A30(c);
    if (ok && c->unk58[i] == 0) {
        return;
    }
    id = D_8006CB28[c->unk34.b.row][i].unk0;
    max = D_8006CB28[c->unk34.b.row][i].unk1;
    if (id == 0) {
        return;
    }
    cur = c->unk58[i];
    icon = 0xBC;
    if (id < 0xC0) {
        a = &D_801EFC08[id];
        name = D_801F0808 + a->unkE;
        flag = !(a->unkB & 2);
    } else {
        sup = &D_8006D128[id - 0xC0];
        name = D_801F0E50 + sup->unk2;
        if (func_80034A04(c->unk88, id)) {
            icon = 0xBD;
        } else {
            icon = 0xF4;
        }
        flag = 1;
    }
    if (draw) {
        ui_drawSprite(x, y, z, icon, 0);
        D_801F120C.unk7C = func_8002920C(x + 0x10, y, g_ot + z, name, 0, D_801F120C.unk7C);
    }
    if (ok) {
        D_801F120C.unk7C = func_80027170(x + 0x70, y + 3, g_ot + z, cur, max, flag, 0, D_801F120C.unk7C);
    } else {
        D_801F120C.unk7C = func_80027170(x + 0x70, y + 3, g_ot + z, 1, 1, 0, 0, D_801F120C.unk7C);
    }
}

void func_801EF494(void) {
    WinDef *w;

    if (D_801F120C.unk1 >= D_801F120C.unk3) {
        func_801EF9D4();
        return;
    }
    D_801F120C.unk0 = D_801F120C.unk1 != 0 ? 3 : 0;
    snd_playSe(0x417);
    w = &D_801F120C.unk1C[D_801F120C.unk1];
    D_801F120C.unk10[D_801F120C.unk1] = 1;
    D_801F120C.unk16[D_801F120C.unk1] = 1;
    w->unk4 = D_801F10E4[D_801F120C.unk1];
    func_80063118(w, &D_801F1134, 1);
}

void func_801EF568(s32 slot, s32 dir) {
    Character *c;
    s32 i;

    c = g_gameState->unk1C->party[slot];
    func_80028D08(&D_801F120C.unk8C, D_801F120C.unk2, slot, dir);
    if (dir != 0) {
        D_801F120C.unk0 = 3;
    }
    D_801F120C.unk2 = slot;
    D_801F120C.unk3 = 0;
    D_801F120C.unk4 = 0;
    if ((s32)(c->unk34.b.row) < 0x10) {
        for (i = 0; i < 0x30; i++) {
            if (D_8006CB28[c->unk34.b.row][i].unk0 != 0) {
                D_801F120C.unk4++;
            }
        }
        D_801F120C.unk3 = (D_801F120C.unk4 + 7) / 8;
    }
}

void func_801EF69C(void) {
    func_801EF708();
    func_801EF760();
    func_801EF80C();
}

void func_801EF6CC(void) {
    D_80077E53 = D_801F120C.unk2;
    func_80035658(D_801F120C.unk5);
    func_8003556C(NULL);
}

void func_801EF708(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801EFBA4[D_801F120C.unk0];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801EF760(void) {
    u8 *p = D_801F120C.unk7C - 0x4000;

    D_801F120C.unk88 = D_801F120C.unk80[g_gameState->unk1C->unk8 ^ 1] - p;
    D_801F120C.unk7C = D_801F120C.unk80[g_gameState->unk1C->unk8];
    func_801EF904();
    func_801EE650();
    func_801EE740();
    func_801EE9E8();
    func_801EED18();
    func_801EEF00();
    func_801EF0A0();
    func_801EF9B4();
}

void func_801EF80C(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (D_801F120C.unk10[i] != 0 && D_801F120C.unk1C[i].unkC != 0) {
            func_800631AC(&D_801F120C.unk1C[i]);
        }
    }
    func_80028D2C(&D_801F120C.unk8C);
}

void func_801EF8A0(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        func_801EF9D4();
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x67);
        func_801EF494();
    }
}

void func_801EF904(void) {
    RECT *r = &D_801F120C.unk8;

    func_800269F4(r->x + 0x6D, r->y + 0x63, 0x1E, r->w - 0x71);
    func_800269F4(r->x + 0x6B, r->y + 0x38, 0x1E, 0x3E - r->h);
    func_800269F4(r->x + 4, r->y + 0x36, 0x1E, r->w - 9);
    func_8002DEE4(r, 0x1E, 2);
    func_8002705C(r, 0x1E, 2);
}

void func_801EF9B4(void) {
    func_8002F810();
}

void func_801EF9D4(void) {
    s32 i;

    if (D_801F120C.unk1 != 0) {
        D_801F120C.unk0 = 0;
        for (i = 0; i < D_801F120C.unk1; i++) {
            if (D_801F120C.unk10[i] != 0) {
                D_801F114C[i].unk0 = D_801F120C.unk1 - 1 - i;
                func_80063118(&D_801F120C.unk1C[i], &D_801F114C[i], 0);
            }
        }
        snd_playSe(0x417);
    }
}

void func_801EFA80(void) {
    D_801F120C.unk0 = 2;
    if (D_801F120C.unk1 != 0) {
        D_801F120C.unk16[D_801F120C.unk1 - 1] = 0;
    }
    D_801F120C.unk1++;
}

void func_801EFABC(void) {
    s32 i;

    D_801F120C.unk0 = 1;
    for (i = 0; i < 6; i++) {
        D_801F120C.unk16[i] = 0;
        D_801F120C.unk10[i] = 0;
    }
    D_801F120C.unk1 = 0;
}

void func_801EFAF8(void) {
    D_801F120C.unk0 = 1;
}

void func_801EFB08(void) {
    s32 i;

    D_80077DE8.unk0 = 0xD8;
    D_80077DE8.unk2 = 0x52;
    D_80077DE8.unk4 = 0xA;
    D_80077DE8.unk8 = text_get(0);
    for (i = 0; i < 7; i++) {
        func_8002BE44(i, D_801F11CC[i].unk0);
        func_8002BE74(i, D_801F11CC[i].unk4);
    }
    func_8002BE44(i, D_801F1204);
}

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", g_statusMenuText);
