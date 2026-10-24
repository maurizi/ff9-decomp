#include "ovl_0b.h"

void btlui_vsync(void) {
    GameData *g;
    D_80104E68.unk1 = 1;
    func_800EA5C4();
    func_800EA548();
    btlui_readPad();
    if (D_80104E68.unkF != 0) {
        func_800E9BBC();
    }
    g = g_gameState->unk1C;
    if (!(g->unk0 & 0x100) && !(g->unk8E4.battle->unk0 & 0x20)) {
        if (D_80104E68.unkC == 0 && (u8)func_800E9FF4()) {
            func_800EB578(0xBA);
            btlui_beginFrame();
            func_800EA4D0();
            func_800EA378();
            func_800EA400();
            btlui_drawWindows();
            func_800EA498();
            btlui_submitOt();
            func_800EB578(0xBB);
        }
    }
    D_80104E68.unk1C++;
    g_drawBufs.lastVsync++;
    D_80104E68.unk18++;
    D_80104E68.unkF = 0;
    D_80104E68.unk1 = 0;
}

void func_800E9BBC(void) {
    func_800EA5C4();
    g_drawBufs.cur = g_gameState->unk1C->unk8 ^ 1;
    if (D_80104E68.unk0 == 0) {
        func_800EA550();
    }
    if (!(g_gameState->unk1C->unk0 & 0x100)) {
        if (!(g_gameState->unk1C->unk8E4.battle->unk0 & 0x20)) {
            if (D_80104E68.unk0 != 0) {
                D_80104E68.unk8 = (g_padHeld >> 19) & 1;
                D_80104E68.unkD = D_80104E68.unk2;
                func_800EB238();
                func_800EBC30();
                func_800F3E60();
                func_800F28F0();
                func_800F1484();
                win_reapClosed();
            }
        }
        if (!(g_gameState->unk1C->unk0 & 0x100) && !(g_gameState->unk1C->unk8E4.battle->unk0 & 0x20) && D_80104E68.unkC != 0) {
            D_80104E68.unkC--;
        }
    }
}

void btlui_init(void) {
    func_800EB05C();
    func_800F3C08();
    func_800F4884();
    D_80104E68.unk6 = 1;
    D_80104E68.unk15 = 0;
    VSyncCallback(btlui_vsync);
}

void func_800E9D84(void) {
    void *p = db_lookupRegistered(4, 0x14, 0);
    if (p != NULL) {
        func_8002C158(p);
    }
}

void func_800E9DBC(void) {
    void *p = db_lookupRegistered(4, 0x1E, 0);
    if (p != NULL) {
        ui_loadPartyPortraits(p);
        func_80033D1C(0);
    }
}

void func_800E9DFC(void) {
    D_80104E68.unk1C = 0;
}

void btlui_reset(void) {
    memset(&D_80104E68, 0, 0x11C);
    func_800F2A0C();
    func_800F48DC();
    func_800F48E4();
    func_800F4630();
    func_800EBB34();
    func_800EC008();
    text_setBank(g_battleText);
    VSyncCallback(NULL);
}

void func_800E9E80(void) {
    func_800E9F3C(0);
    VSyncCallback(NULL);
    func_800F4700();
    func_800F4D20();
    D_80104E68.unkE = 0;
}

void func_800E9EC4(void) {
    DrawSync(0);
    func_800DA898();
    if (D_80104E68.unk1C < (u32)(g_gameState->vsyncMode - 1)) {
        VSync(g_gameState->vsyncMode - 1);
    }
    func_800F4384();
    D_80104E68.unkF = 1;
    VSync(0);
}

void func_800E9F3C(u8 arg0) {
    if (arg0) {
        D_80104E68.unk12 = 1;
    } else {
        D_80104E68.unk0 = 0;
    }
}

void func_800E9F68(u8 arg0) {
    if (D_80104E68.unk6 != 0) {
        if (D_80104E68.unk2 != arg0) {
            D_80104E68.unk2 = arg0;
            if (arg0 == 0) {
                func_800EB410();
                func_800EBD58();
            }
            func_800EB2FC();
        }
    }
}

BtlUnit *btlui_getActiveUnit(void) {
    BtlUnit *u = NULL;
    if (g_btlUi->menuState != 0) {
        u = g_btlUi->cmdUnit;
    }
    return u;
}

s32 func_800E9FF4(void) {
    return D_80104E68.unk0 != 0;
}

s32 func_800EA004(void) {
    return g_btlUi->menuState == 3;
}

u8 func_800EA024(void) {
    return D_80104E68.unk4;
}

u8 btl_isAtbRunning(void) {
    s32 f;
    if (D_80104E68.unk2 == 0) {
        return 0;
    }
    f = (g_gameState->config >> 3) & 1;
    if (f != 1) {
        return 1;
    }
    return g_btlUi->menuState < 2;
}

s32 func_800EA094(void) {
    return func_800F1360();
}

u8 *btl_getAbilityName(s32 arg0) {
    if (arg0 >= 0xC0) {
        return NULL;
    }
    return g_gameState->unk1C->unk8E4.battle->abilities[arg0].nameOfs + D_800F909C;
}

void func_800EA0F8(s32 arg0) {
    func_800EC170(arg0);
    func_800EBEFC(arg0);
}

void func_800EA124(s32 arg0) {
    func_800EC170(arg0);
}

void func_800EA144(s32 arg0) {
    func_800EC14C(arg0);
}

void func_800EA164(s32 arg0) {
    func_800EBEFC(arg0);
    if (btlui_getItemReserved(arg0) != 0 && item_getCount(arg0) == 0) {
        func_800EC170(arg0);
    }
}

void func_800EA1B0(s32 arg0) {
    func_800EC070(arg0);
}

void func_800EA1D0(s8 arg0) {
    if (D_80104E68.unk9 == 0) {
        D_80104E68.unk9 = 1;
        D_80104E68.unk14 = arg0;
    }
}

void func_800EA1F8(u32 *arg0, DRAWENV *arg1) {
    D_80104E68.unk10 = 1;
    DrawOTagEnv(arg0, arg1);
    D_80104E68.unk10 = 0;
}

void func_800EA230(u32 *arg0) {
    D_80104E68.unk10 = 1;
    DrawOTag(arg0);
    D_80104E68.unk10 = 0;
}

u8 func_800EA268(void) {
    return D_80104E68.unk15;
}

u8 func_800EA27C(void) {
    return D_80104E68.unkE;
}

void func_800EA290(void) {
    if (D_80104E68.unk6 == 0) {
        GameData *gd = g_gameState->unk1C;
        if (!(gd->unk8E4.battle->unk0 & 0x100)) {
            PutDispEnv(&g_gameState->unkC[gd->unk8]);
        }
    }
}

u8 func_800EA30C(void) {
    return D_80104E68.unk6 ? D_80104E68.unkB : 0;
}

void func_800EA338(s8 arg0) {
    D_80104E68.unkB = arg0;
}

void func_800EA34C(void) {
    u8 v = g_gameState->unk1C->unk8;
    D_80104E68.unkC = 2;
    g_drawBufs.cur = v;
}

void func_800EA378(void) {
    GameState *g = g_gameState;
    s32 r = 0;
    volatile Unk80104E68 *p = &D_80104E68;
    s32 f;
    if (((g->unk1C->unk8E4.battle->unk11B8 >> 1) ^ 1) & 1) {
        r = 1;
    } else {
        do {
            f = (g->config >> 19) & 1;
            if (f == 1) {
                r = 1;
            }
        } while (0);
    }
    p->unk3 = r;
    D_80104E68.unk4 = func_800F13CC() >= 2;
}

void func_800EA400(void) {
    if (D_80104E68.unk2 != 0) {
        func_800EBB78();
        btlui_menuInput();
    }
}

void btlui_drawWindows(void) {
    if (D_80104E68.unk8 == 0) {
        btlui_drawHelpWindow();
        btlui_drawPartyPanel();
        win_drawAll();
        func_800EA518();
    }
    btlui_drawTargetCursors();
}

void func_800EA498(void) {
    btlui_updateMenu();
    func_800F3D0C();
    win_updateAll();
    func_800EBBB4();
}

void func_800EA4D0(void) {
    if (D_80104E68.unkF != 0) {
        func_800F386C();
        func_800ECDB8();
        func_800F2374();
    }
}

void func_800EA518(void) {
    func_800F3A8C();
    func_800ECF0C();
    func_800F2560();
}

void func_800EA548(void) {
}

void func_800EA550(void) {
    if (D_80104E68.unk12 != 0) {
        D_80104E68.unk12 = 0;
        D_80104E68.unk0 = 1;
        if (D_80104E68.unk6 != 0 && D_80104E68.unk2 != 1) {
            D_80104E68.unk2 = 1;
            func_800EB2FC();
        }
        func_800F2AAC();
        func_800EC048();
    }
}

void func_800EA5C4(void) {
    if (D_80104E68.unkF != 0 && D_80104E68.unk6 != 0) {
        GameData *gd = g_gameState->unk1C;
        if (!(gd->unk8E4.battle->unk0 & 0x100)) {
            PutDispEnv(&g_gameState->unkC[gd->unk8]);
        }
    }
}

void func_800EA650(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    Unk80104F88Entry *e = g_btlUi->unk0;
    s8 n = 0;
    s8 f;
    while (u != NULL && (u->unk68 & 1)) {
        u->unkB4 = e;
        u->unkCF = 0;
        f = (u8)u->unk68 >> 5;
        e->unk10 = u;
        e->unk18 = f;
        e->unk14 = &g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF];
        e->unk19 = func_80034A30(e->unk14);
        e->unk1A = func_800339F4(e->unk14->unk34.b.id, (e->unk14->unkA >> 4) & 1);
        if ((g_gameState->config >> 5) & 1) {
            f = ((s32)(g_gameState->config >> 20) >> e->unk1A) & 1;
        } else {
            f = 0;
        }
        e->unk1B = f;
        func_800EB600(e);
        func_800EB54C(&e->unk49C);
        n++;
        e++;
        u = u->unk0;
    }
    g_btlUi->partyCount = n;
}

void func_800EA7B8(Unk80104F88Entry *arg0) {
    s32 k = arg0->unk14->unk34.b.row;
    u8 *s = D_800FDDF8[k];
    u8 *t = D_800FDDD0[k];
    Unk800FD56C *a;
    Unk800FD56C *b;
    s32 i;
    s32 j;
    u8 x;
    u8 y;
    if ((func_80034A30(arg0->unk14) & 0xFF) && (s[2] == 1 || s[2] == 2)) {
        i = 0;
        j = s[2] - 1;
        a = &D_800FD56C[t[j]];
        b = &D_800FD56C[s[j]];
        for (; i < a->unk7; i++) {
            x = D_800FDD10[a->unk8 + i];
            y = D_800FDD10[b->unk8 + i];
            if (x != y) {
                arg0->unk1C[y] = arg0->unk1C[x];
                arg0->unk31C[y] = arg0->unk31C[x];
                arg0->unk3DC[y] = arg0->unk3DC[x];
            }
        }
    }
}

/* character 3: also list the abilities of character record 1 and its equipment */
void func_800EA908(Unk80104F88Entry *arg0) {
    Unk800FD6E0 *m = &D_800FD6E0;
    Character *c = &g_gameState->unk1C->characters[1];
    Unk8006CB88 *ab = D_8006CB88;
    s32 i;
    s32 j;
    s32 k;
    s32 slot;
    s32 v;
    s32 id;
    s32 e;
    Unk80069018 *it;

    if (arg0->unk14->unk34.b.id == 3) {
        for (i = 0; i < m->unk7; i++) {
            slot = D_800FDD10[m->unk8 + i];
            v = func_80034430(1, D_800F8D84[i]);
            if (v >= 0) {
                arg0->unk1C[slot] = &c->unk58[v];
                arg0->unk31C[slot] = ab[v].unk1;
            }
        }
        for (i = 0; i < 5; i++) {
            e = c->equip[i];
            if (e != 0xFF) {
                it = &D_80069018[e];
                for (j = 0; j < 3; j++) {
                    id = it->unkC[j];
                    if (id != 0 && id < 0xC0) {
                        for (k = 0; k < m->unk7; k++) {
                            if (id == D_800F8D84[k]) {
                                slot = D_800FDD10[m->unk8 + k];
                                arg0->unk3DC[slot] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void btl_updateAtb(void) {
    BtlUnit *u;
    BtlUnit28 *s;
    BtlUnit24 *m;

    u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    if (btl_isAtbRunning() == 0) {
        return;
    }
    for (; u != NULL; u = u->unk0) {
        if (u->unkB3 != 0 || u->unkCF != 0 || u->hp == 0) {
            continue;
        }
        if (!((u->unk68 >> 16) & 1)) {
            continue;
        }
        if ((u->unk68 & 1) == g_btlUi->unk12B6) {
            continue;
        }
        s = (BtlUnit28 *)&u->hp;
        m = u->max;
        s->unk4 += s->unk6;
        if (s->unk4 < m->maxAtb) {
            continue;
        }
        s->unk4 = m->maxAtb;
        if ((u8)btl_hasStatus(u, (BTL_STATUS_VENOM | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE)) != 0) {
            continue;
        }
        if (u->unk68 & 1) {
            if ((u8)btl_hasStatus(u, (BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK)) != 0) {
                u->unkB3 = 1;
                btl_autoAttack(u);
            } else {
                func_800EB358(u);
            }
        } else if (evt_battleTrigger(0x2F, u->unkB0, 0, 0) != 0) {
            u->unkB3 = 1;
        }
    }
}

void func_800EAC88(void) {
    BtlUnit *u;
    s32 slot;

    u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    g_btlUi->unk12B7 = 0;
    g_btlUi->unk12B8.b[0] = 0;
    g_btlUi->unk12B8.b[1] = 0;
    for (; u != NULL; u = u->unk0) {
        if (!(u->unk68 & 1)) {
            break;
        }
        slot = (u->unk68 >> 1) & 0xF;
        if (slot == 1) {
            g_btlUi->unk12B7 = 1;
            if (u->hp == 0 || (u8)btl_hasStatus(u, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_SILENCE | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_MINI)) != 0) {
                g_btlUi->unk12B8.b[0] = 1;
            }
        } else if (slot == 3 && (u8)btl_hasStatus(u, BTL_STATUS_MINI) != 0) {
            g_btlUi->unk12B8.b[1] = 1;
        }
    }
}

BtlUnit *func_800EAD94(void) {
    BtlUnit *u;
    u8 ok;
    for (;;) {
        if (g_btlUi->unk12C4 == 0 || g_btlUi->menuState != 0) {
            return NULL;
        }
        u = g_btlUi->unk12C8[g_btlUi->unk12C5];
        g_btlUi->unk12C4--;
        g_btlUi->unk12C5 = (g_btlUi->unk12C5 + 1) & 3;
        if (u == NULL || u->hp == 0 || (btl_hasStatus(u, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) & 0xFF) || !((g_btlRewards.unkA >> ((u8)u->unk68 >> 5)) & 1)) {
            ok = 0;
        } else {
            ok = 1;
        }
        if (ok) {
            return u;
        }
        u->unkCF = 0;
    }
}

void func_800EAE8C(void) {
    BtlUnit *list[4];
    u8 seen[4];
    s32 n = 0;
    s32 head = g_btlUi->unk12C5;
    s32 cnt = g_btlUi->unk12C4;
    BtlUnit *u;
    s32 i;
    Unk80104F88 *q;
    BtlUnit *v;
    Unk80104F88 *r;
    memset(seen, 0, 4);
    while (cnt != 0) {
        u = g_btlUi->unk12C8[head];
        if (BtlUnitCanAct(u) && u->unkB3 != 1) {
            list[n++] = u;
        } else {
            u->unkCF = 0;
        }
        cnt--;
        head = (head + 1) & 3;
    }
    q = g_btlUi;
    if (n != q->unk12C4) {
        q->unk12C6 = 0;
        q->unk12C5 = 0;
        q->unk12C4 = 0;
        for (i = 0; i < n; i++) {
            if (seen[(u8)list[i]->unk68 >> 5] == 0) {
                seen[(u8)list[i]->unk68 >> 5] = 1;
                v = list[i];
                r = g_btlUi;
                v->unkCF = 1;
                g_btlUi->unk12C8[(r->unk12C5 + r->unk12C4) & 3] = v;
                r->unk12C4++;
            }
        }
    }
}

void func_800EB05C(void) {
    s32 i;
    Unk80104F88 *p = func_800F2A60(0x12D8);
    g_btlUi = p;
    p->menuState = 0;
    p->cmdUnit = NULL;
    g_btlUi->unk12C0 = 0;
    g_btlUi->unk12BC = g_gameState->unk1C->unk8E4.battle->unk132C.unk0 & 3;
    g_btlUi->unk12B6 = func_800EB9E0();
    func_800EB5AC();
    func_800EA650();
    func_800EB2E0();
    for (i = 0; i < 4; i++) {
        g_btlUi->unk12A4[i] = win_get(i);
    }
}

void btlui_menuInput(void) {
    u8 st;
    func_800EAE8C();
    st = g_btlUi->menuState;
    if (st == 4) {
        return;
    }
    if (st == 0) {
        BtlUnit *u = func_800EAD94();
        g_btlUi->cmdUnit = u;
        if (u != NULL) {
            func_800EC4EC();
        }
    } else if (!(func_800EB7B0(g_btlUi->cmdUnit) & 0xFF)) {
        func_800EB410();
    } else {
        D_800F8D94[g_btlUi->menuState]();
    }
}

void btlui_updateMenu(void) {
    void (*fn)(void);
    func_800EB81C();
    btlui_pollEscape();
    btl_updateAtb();
    func_800EAC88();
    fn = D_800F8DA8[g_btlUi->menuState];
    if (fn != NULL) {
        fn();
    }
}

void func_800EB238(void) {
    if (g_btlUi->unk12C0 < 75) {
        g_btlUi->unk12B6 = func_800EB9E0();
    } else {
        g_btlUi->unk12B6 = 2;
    }
    g_btlUi->unk12C0++;
}

s32 func_800EB2A4(void) {
    return g_btlUi->menuState;
}

s32 btlui_setMenuState(s32 arg0) {
    s32 old = g_btlUi->menuState;
    if (arg0 != 5) {
        g_btlUi->menuState = arg0;
    }
    return old;
}

void func_800EB2E0(void) {
    Unk80104F88 *p = g_btlUi;
    p->unk12C6 = 0;
    p->unk12C5 = 0;
    p->unk12C4 = 0;
}

void func_800EB2FC(void) {
    Unk80104F88 *p;
    s32 n = g_btlUi->unk12C4;
    s32 i = g_btlUi->unk12C5;
    while (n != 0) {
        n--;
        g_btlUi->unk12C8[i]->unkCF = 0;
        i = (i + 1) & 3;
    }
    p = g_btlUi;
    p->unk12C6 = 0;
    p->unk12C5 = 0;
    p->unk12C4 = 0;
}

void func_800EB358(BtlUnit *arg0) {
    Unk80104F88 *p = g_btlUi;
    arg0->unkCF = 1;
    g_btlUi->unk12C8[(p->unk12C5 + p->unk12C4) & 3] = arg0;
    p->unk12C4++;
}

void func_800EB3A0(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    func_800EB2E0();
    while (u != NULL) {
        if (u->unk68 & 1) {
            u->unkCF = 0;
        }
        u = u->unk0;
    }
}

void func_800EB410(void) {
    BtlUnit *u = g_btlUi->cmdUnit;
    if (u != NULL) {
        u->unkCF = 0;
    }
    if (g_btlUi->menuState != 0) {
        g_btlUi->menuState = 4;
        func_800EB46C();
    }
    func_800F142C();
}

void func_800EB46C(void) {
    s32 i;
    if (g_btlUi->menuState != 0) {
        if (!func_800F29A4()) {
            win_setOnClosed(g_btlUi->unk12A4[3], func_800EBA10);
            win_closeAll();
        } else {
            win_closeAll();
            func_800F2890(0);
        }
        for (i = 0; i < 2; i++) {
            pad_setRepeatEnabled(i, 2, 0);
            pad_setRepeatEnabled(i, 3, 0);
        }
    }
}

void func_800EB518(Unk800EB518 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg1 == 0) {
        arg0->unk0 = arg2;
        return;
    }
    arg0->unk8[arg0->unk0] = arg2;
    arg0->unk4[arg0->unk0] = arg3;
}

void func_800EB54C(Unk800EB518 *arg0) {
    s32 i;
    arg0->unk0 = 0;
    for (i = 0; i < 4; i++) {
        arg0->unk8[i] = 0;
        arg0->unk4[i] = 0;
    }
}

void func_800EB578(s32 arg0) {
}

u8 *func_800EB580(void) {
    return &g_btlUi->unk12C4;
}

BtlUnit *func_800EB590(s32 arg0) {
    return g_btlUi->unk12C8[arg0];
}

void func_800EB5AC(void) {
    s32 i, j;
    for (i = 0; i < 4; i++) {
        Unk80104F88Entry *e = &g_btlUi->unk0[i];
        for (j = 0; j < 0xC0; j++) {
            e->unk1C[j] = 0;
            e->unk31C[j] = 0;
            e->unk3DC[j] = 0;
        }
    }
}

void func_800EB600(Unk80104F88Entry *arg0) {
    func_800EB63C(arg0);
    func_800EB6FC(arg0);
    func_800EA7B8(arg0);
    func_800EA908(arg0);
}

void func_800EB63C(Unk80104F88Entry *arg0) {
    Character *c;
    s32 i;
    AbilityEntry *row;
    s32 v;
    c = arg0->unk14;
    if (arg0->unk19 != 0) {
        i = 0;
        row = D_8006CB28[c->unk34.b.row];
        for (; i < 0xC0; i++) {
            v = func_80034430(c->unk34.b.id, i);
            if (v >= 0) {
                arg0->unk1C[i] = &c->unk58[v];
                arg0->unk31C[i] = row[v].unk1;
            }
        }
    }
}

void func_800EB6FC(Unk80104F88Entry *arg0) {
    Character *c = arg0->unk14;
    s32 i;
    s32 j;
    memset((s32)arg0->unk3DC, 0, 0xC0);
    for (i = 0; i < 5; i++) {
        u8 item = c->equip[i];
        if (item != 0xFF) {
            Unk80069018 *e = &D_80069018[item];
            for (j = 0; j < 3; j++) {
                s32 a = e->unkC[j];
                if (a != 0 && a < 0xC0) {
                    arg0->unk3DC[a] = 1;
                }
            }
        }
    }
}

s32 func_800EB7B0(BtlUnit *arg0) {
    if (arg0 == NULL) {
        return 0;
    }
    if (arg0->hp != 0 && !(btl_hasStatus(arg0, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) & 0xFF)) {
        return (D_800FDF3A >> ((u8)arg0->unk68 >> 5)) & 1;
    }
    return 0;
}

void func_800EB81C(void) {
    BtlUnit *u;
    u8 c;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        c = u->unkB2;
        if (c >= 2) {
            if (c == 2) {
                u->unkB2 = 0;
                u->unk68 &= ~0x10000;
                u->unk68 &= ~0x10000000;
                if (g_btlUi->menuState == 3) {
                    func_800F11A4(u);
                }
            } else if (c == 3) {
                u->unkB2 = 1;
                u->unk68 |= 0x10000;
                u->unk68 |= 0x10000000;
                if (g_btlUi->menuState == 3) {
                    func_800F1150(u);
                }
            }
        }
    }
}

void btlui_pollEscape(void) {
    Unk80104E68 *p = (Unk80104E68 *)&D_80104E68;
    if (p->unk2 != 0) {
        if ((g_padHeld & (BTN_L1 | BTN_R1)) == 0x300000) {
            g_gameState->unk1C->unk8E4.battle->escapeHeld = 1;
            if (p->unk13++ >= 0x3D) {
                p->unk13 = 0;
                btl_tryEscape(1);
            } else {
                btl_tryEscape(0);
            }
        } else {
            g_gameState->unk1C->unk8E4.battle->escapeHeld = 0;
        }
    }
}

s32 func_800EB9E0(void) {
    s32 v = g_btlUi->unk12BC;
    if (v != 0) {
        return (v != 1) * 2;
    }
    return 1;
}

void func_800EBA10(Window *arg0) {
    g_btlUi->menuState = 0;
}

void btlui_drawHelpWindow(void) {
    RECT r;
    s32 m;
    if (D_80104F90.unk0 != 0 && D_80104E75 != 0) {
        btlui_getHelpRect(&D_800F8DBC, &r);
        win_setClip(NULL, 2, 4);
        if (D_80104F90.unk1 == 0) {
            ui_drawSprite(r.x + 8, r.y, 2, 0xB7, 0);
        }
        btlui_updateHelpText();
        if (D_80104F90.unk4 != 0) {
            win_setClip(NULL, 2, 3);
            m = func_8002BE88();
            text_drawBlock(r.x + 6, r.y + 10, 2, D_80104F90.unk4, 8);
            func_8002BE98(m);
            win_setClip(&r, 2, 3);
        }
        win_drawHelpFrame(&r, 2, 4, 0);
        win_setClip(&r, 2, 4);
    }
}

void func_800EBB34(void) {
    D_80104F90.unk0 = 0;
    D_80104F90.unk1 = 0;
    D_80104F90.unk2 = 0;
    D_80104F90.unk3 = 0;
    D_80104F90.unk4 = 0;
    D_80104F90.unkC = 0;
    D_80104FA0 = func_800F2A60(0x100);
}

void func_800EBB78(void) {
    if (g_padRepeat & 0x08000000) {
        func_800EBCC4(D_80104F90.unk0 == 0);
    }
}

void func_800EBBB4(void) {
    if (D_80104F90.unk0 != 0 && D_80104E75 != 0) {
        D_80104F90.unkC++;
        if (D_80104F90.unk1 != 0 && D_80104F90.unkC >= 7) {
            D_80104F90.unk1 = 0;
            func_800F3ECC(0);
        }
    }
}

void func_800EBC30(void) {
    if (func_800F3EA4()->unk5C == 0) {
        if (D_80104F90.unk0 != 0) {
            D_80104F90.unk3 = 1;
        }
    }
    if (D_80104F90.unk3 != 0 && D_80104F90.unk0 != 0) {
        D_80104F90.unk0 = 0;
        D_80104F90.unk3 = 0;
        func_800F3ECC(1);
    }
}

u8 func_800EBCB4(void) {
    return D_80104F90.unk0;
}

void func_800EBCC4(u8 arg0) {
    if (arg0) {
        if (D_80104F90.unk0 == 0) {
            snd_playSe(0x2AA);
            D_80104F90.unk0 = 1;
            D_80104F90.unk1 = 1;
            D_80104F90.unkC = 0;
        }
    } else if (D_80104F90.unk0 != 0 && D_80104F90.unk3 == 0) {
        snd_playSe(0x65);
        D_80104F90.unk3 = 1;
    }
}

void func_800EBD58(void) {
    if (D_80104F90.unk0 != 0 && D_80104F90.unk3 == 0) {
        D_80104F90.unk3 = 1;
    }
}

void btlui_updateHelpText(void) {
    s32 (*fn)(void);
    D_80104F90.unk4 = 0;
    if (g_gameState->unk1C->unk8E4.battle->escapeHeld != 0) {
        D_80104F90.unk4 = (s32)text_get(4);
    } else {
        fn = D_800F8DC4[g_btlUi->menuState];
        if (fn != NULL) {
            D_80104F90.unk4 = fn();
        }
    }
}

void btlui_getHelpRect(RECT *arg0, RECT *arg1) {
    Window *w;
    s16 x;
    s32 d;
    *arg1 = *arg0;
    w = func_800F1370();
    if (w != NULL && w->unk1C != 0) {
        if (arg1->x < w->unk8.x + w->unk8.w - 4) {
            x = w->unk8.x + w->unk8.w - 4;
            arg1->x = x;
            arg1->w = 0x137 - x;
        }
    }
    if (D_80104F90.unk1 != 0) {
        d = (7 - D_80104F90.unkC) * 24;
        arg1->x += d;
        arg1->w -= d;
    }
}

void func_800EBEFC(s32 arg0) {
    s32 i;
    if (item_remove(arg0, 1) != 0) {
        if (arg0 >= 0xE0) {
            i = func_800EC44C(arg0);
            if (i != 0xFF && D_80104FA8->unk4[i]->unk1 == 0) {
                D_80104FA8->unk4[i] = 0;
                D_80104FA8->unk0--;
            }
        }
        if (arg0 < 0x58 && (D_8006E0E8[arg0].unk0 & 4)) {
            i = func_800EC49C(arg0);
            if (i != 0xFF && D_80104FA8->unk80[i]->unk1 == 0) {
                D_80104FA8->unk80[i] = 0;
                D_80104FA8->unk2--;
            }
        }
    }
}

void func_800EC008(void) {
    Unk80104FA8 *p = func_800F2A60(0x2E0);
    D_80104FA8 = p;
    memset((s32)p->unk1E0, 0, 0x100);
    func_800EC048();
}

void func_800EC048(void) {
    func_800EC1B0();
    func_800EC258();
}

void func_800EC070(s32 arg0) {
    if (item_add(arg0, 1) == 0) {
        return;
    }
    if (arg0 >= 0xE0 && func_800EC44C(arg0) == 0xFF) {
        func_800EC334(arg0);
    }
    if (arg0 < 0x58 && (D_8006E0E8[arg0].unk0 & 4) && func_800EC49C(arg0) == 0xFF) {
        func_800EC3C0(arg0);
    }
}

UnkPair *btlui_getItemListEntry(s32 arg0) {
    return D_80104FA8->unk4[arg0];
}

UnkPair *func_800EC130(s32 arg0) {
    return D_80104FA8->unk80[arg0];
}

void func_800EC14C(s32 arg0) {
    D_80104FA8->unk1E0[arg0]++;
}

void func_800EC170(s32 arg0) {
    D_80104FA8->unk1E0[arg0]--;
}

s32 btlui_getItemReserved(s32 arg0) {
    return D_80104FA8->unk1E0[arg0];
}

void func_800EC1B0(void) {
    s32 n = 0;
    s32 i;
    UnkPair *p = g_gameState->unk1C->items.slots;
    Unk80104FA8 *f = D_80104FA8;
    for (i = 0; i < 0x100; i++, p++) {
        if (p->unk1 != 0 && p->unk0 != 0xFF && p->unk0 >= 0xE0) {
            f->unk4[n] = p;
            n++;
        }
    }
    for (i = n; i < 31; i++) {
        D_80104FA8->unk4[i] = NULL;
    }
    D_80104FA8->unk0 = n;
}

void func_800EC258(void) {
    s32 n = 0;
    s32 i = 0;
    UnkPair *e;
    Unk80104FA8 *p = D_80104FA8;
    e = g_gameState->unk1C->items.slots;
    for (; i < 0x100; i++, e++) {
        if (e->unk1 != 0 && e->unk0 != 0xFF && e->unk0 < 0x58 && (D_8006E0E8[e->unk0].unk0 & 4)) {
            p->unk80[n] = e;
            n++;
        }
    }
    for (i = n; i < 0x58; i++) {
        D_80104FA8->unk80[i] = NULL;
    }
    D_80104FA8->unk2 = n;
}

void func_800EC334(s32 arg0) {
    s32 i;
    for (i = 0; i < 31; i++) {
        if (D_80104FA8->unk4[i] == NULL) {
            D_80104FA8->unk4[i] = item_findSlot(arg0);
            break;
        }
    }
    D_80104FA8->unk0++;
}

void func_800EC3C0(s32 arg0) {
    s32 i;
    for (i = 0; i < 0x58; i++) {
        if (D_80104FA8->unk80[i] == NULL) {
            D_80104FA8->unk80[i] = item_findSlot(arg0);
            break;
        }
    }
    D_80104FA8->unk2++;
}

s32 func_800EC44C(s32 arg0) {
    s32 i;
    Unk80104FA8 *p = D_80104FA8;
    for (i = 0; i < 31; i++) {
        if (p->unk4[i] != NULL && arg0 == p->unk4[i]->unk0) {
            return i;
        }
    }
    return 0xFF;
}

s32 func_800EC49C(s32 arg0) {
    s32 i;
    Unk80104FA8 *p = D_80104FA8;
    for (i = 0; i < 0x58; i++) {
        if (p->unk80[i] != NULL && arg0 == p->unk80[i]->unk0) {
            return i;
        }
    }
    return 0xFF;
}

void func_800EC4EC(void) {
    Unk80104F88Entry *e;
    Window *w;
    Unk80104FC0 *p;

    p = &g_btlCmdMenu.unk10;
    e = g_btlUi->cmdUnit->unkB4;
    if (!((g_gameState->config >> 2) & 1)) {
        g_btlCmdMenu.cursor = 0;
    } else {
        g_btlCmdMenu.cursor = e->unk49C.unk0;
    }
    g_btlCmdMenu.unk1 = (u8)btl_hasStatus(g_btlUi->cmdUnit, BTL_STATUS_TRANCE) != 0;
    g_btlCmdMenu.charIndex = e->unk14->unk34.b.row;
    g_btlCmdMenu.unit = g_btlUi->cmdUnit;
    p->unk1 = 0;
    p->unk2 = 0;
    btlui_setMenuState(4);
    w = win_open(&D_800F8DDC, 0xF, 2, 3);
    g_btlUi->unk12A4[3] = w;
    win_setDrawContent(w, btlui_drawCmdList);
    win_setDrawOverlay(g_btlUi->unk12A4[3], btlui_drawCmdOverlay);
    win_setOnOpened(g_btlUi->unk12A4[3], func_800EDC3C);
    func_800ECB5C();
    snd_playSe(0x414);
}

void btlui_drawCmdList(Window *arg0) {
    s32 y = arg0->unk2 + 6;
    s32 x = arg0->unk0 + 0xA;
    s32 i;
    u8 *t;
    if (g_btlCmdMenu.unk10.unk1 != 0) {
        func_800EC928(REG_PASSTHROUGH(arg0));
    }
    for (i = 0; i < 4; i++) {
        t = D_800FD7AC + D_800FD56C[g_btlCmdMenu.cmdIds[i]].unk0;
        if (g_btlCmdMenu.unk8[i] == 0) {
            text_draw(x, y, arg0->unk18, t, 2);
        } else {
            text_draw(x, y, arg0->unk18, t, 0);
        }
        do {
            y += 15;
        } while (0);
    }
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", g_battleText);

/* TRANCE label */
INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", D_800A857C);

void btlui_drawCmdOverlay(Window *arg0) {
    u8 st = arg0->unk1C;
    RECT *r = &arg0->unk8;
    s32 dy = 10;
    RECT rc;
    if (arg0->unk1C == 3) {
        if (func_800EB2A4() == 1 || (func_800EB2A4() == st && func_800F13BC() == 1 && !(func_800F1360() & 0xFF) && (D_80104E80 & 1))) {
            ui_drawHand(arg0->unk8.x - 0xE, r->y + (g_btlCmdMenu.cursor * 15 + dy), arg0->unk18 - 1);
        }
    }
    if (arg0->unk1C != 3) {
        win_setClip(NULL, arg0->unk18, 4);
    }
    if (g_btlCmdMenu.unit != NULL && !(btl_hasStatus(g_btlCmdMenu.unit, BTL_STATUS_TRANCE) & 0xFF)) {
        ui_drawLabel(arg0->unk0 + 6, r->y - 3, arg0->unk18, D_800703FC.unk30, arg0->unk1E ? 0 : 2);
    } else if (arg0->unk1E != 0) {
        func_800ED3F8(arg0->unk0 + 6, r->y - 3, arg0->unk18, D_800A857C);
    } else {
        ui_drawLabel(arg0->unk0 + 6, r->y - 3, arg0->unk18, (s32)D_800A857C, 2);
    }
    if (arg0->unk1C != 3) {
        rc.x = r->x;
        rc.y = r->y - 3;
        rc.w = r->w;
        rc.h = 8;
        win_setClip(&rc, arg0->unk18, 4);
    }
}

void func_800EC928(Window *arg0) {
    s32 dx2;
    RECT r;
    s32 off = 0;
    s32 dx;
    s16 y;
    s32 x;
    Unk80104FC0 *p = &D_80104FC0;
    Window *w = arg0;

    dx2 = 6;
    if (p->unk2 == 1) {
        off = (5 - p->unk3) * 3;
    } else if (p->unk2 == 2) {
        off = p->unk3 * 5;
    }
    dx = 10;
    if (p->unk1 == 2) {
        dx = dx2;
        x = off + 4;
        r.x = w->unk0 + x;
    } else {
        r.x = w->unk0 - off;
    }
    y = w->unk2 + g_btlCmdMenu.cursor * 15;
    r.w = 0x3E;
    r.h = 0x11;
    r.y = y + 4;
    text_draw(r.x + dx, r.y + 2, arg0->unk18, text_get(D_800F8DE4[D_80104FC0.unk1]), 0);
    func_800ED02C(&r, arg0->unk18);
}

void func_800ECA50(void) {
    Unk800FD56C *e = &D_800FD56C[g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor]];
    void (*f)(Unk800FD56C *);
    if (g_btlCmdMenu.unk10.unk1 == 0) {
        if (g_btlCmdMenu.unk8[g_btlCmdMenu.cursor] != 0) {
            do {
                snd_playSe(0x67);
                func_800EB518(&g_btlCmdMenu.unit->unkB4->unk49C, 0, g_btlCmdMenu.cursor, 0);
                f = D_800F8DF0[e->unk6];
            } while (0);
        } else {
            snd_playSe(0x66);
            return;
        }
    } else {
        if (g_btlCmdMenu.unk10.unk0 == 0) {
            return;
        }
        snd_playSe(0x67);
        func_800EB518(&g_btlCmdMenu.unit->unkB4->unk49C, 0, g_btlCmdMenu.cursor, 0);
        f = D_800F8E00;
    }
    f(e);
}

void func_800ECB5C(void) {
    s32 k = g_btlCmdMenu.charIndex;
    s8 f;
    g_btlCmdMenu.unk1 = f = (u8)btl_hasStatus(g_btlCmdMenu.unit, BTL_STATUS_TRANCE) != 0;
    g_btlCmdMenu.cmdIds[0] = 1;
    g_btlCmdMenu.unk8[0] = 1;
    g_btlCmdMenu.cmdIds[3] = 0xE;
    g_btlCmdMenu.unk8[3] = 1;
    g_btlCmdMenu.unkC[3] = 0;
    g_btlCmdMenu.unkC[2] = 0;
    g_btlCmdMenu.unkC[1] = 0;
    g_btlCmdMenu.unkC[0] = 0;
    if (f == 0) {
        g_btlCmdMenu.cmdIds[1] = D_800FDDD0[k][0];
        g_btlCmdMenu.cmdIds[2] = D_800FDDD0[k][1];
    } else {
        g_btlCmdMenu.cmdIds[1] = D_800FDDF8[k][0];
        g_btlCmdMenu.cmdIds[2] = D_800FDDF8[k][1];
        if (D_800FDDF8[k][2] != 0xFF) {
            g_btlCmdMenu.unkC[D_800FDDF8[k][2]] = 1;
        }
    }
    g_btlCmdMenu.unk8[1] = 1;
    g_btlCmdMenu.unk8[2] = 1;
    if (g_btlCmdMenu.cmdIds[2] == 0x1F) {
        if (g_btlUi->unk12B7 == 0) {
            g_btlCmdMenu.cmdIds[2] = 0;
            if (g_btlCmdMenu.cursor == 2) {
                g_btlCmdMenu.cursor = 0;
            }
        } else if (g_btlUi->unk12B8.h != 0) {
            g_btlCmdMenu.unk8[2] = 0;
        }
    }
}

void func_800ECCA8(s32 arg0) {
    s32 i;
    s32 u = 0x3C0;
    s32 y = 0x9A;

    for (i = 0; i < 4; i++) {
        setlen(&D_801050D8->unk0[i][arg0], 4);
        setcode(&D_801050D8->unk0[i][arg0], 0x64);
        setRGB0(&D_801050D8->unk0[i][arg0], 0x80, 0x80, 0x80);
        D_801050D8->unk0[i][arg0].x0 = 0;
        D_801050D8->unk0[i][arg0].y0 = y;
        D_801050D8->unk0[i][arg0].u0 = u & 0x3F;
        D_801050D8->unk0[i][arg0].v0 = 0xE0;
        D_801050D8->unk0[i][arg0].w = 0xE;
        D_801050D8->unk0[i][arg0].h = 0x10;
        D_801050D8->unk0[i][arg0].clut = 0;
        u += 0x10;
        y += 0x10;
    }
    setlen(&D_801050D8->unkA0[arg0], 1);
    *D_801050D8->unkA0[arg0].code = 0xE100071F;
}

void func_800ECDB8(void) {
    RECT r = D_800A8584;
    s32 x = 0x3C0;
    s32 y = 0x1E0;
    s32 i = 0;
    r.x += D_801051E8[g_gameState->unk1C->unk8 & 1][0];
    r.y += D_801051E8[g_gameState->unk1C->unk8 & 1][1];
    do {
        SetDrawMove((DR_MOVE *)g_primPtr, &r, x, y);
        x += 0x10;
        i++;
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
        r.y += 0x10;
    } while (i < 4);
}

void func_800ECF0C(void) {
    s32 i;
    if (g_btlUi->unk12A4[3]->unk1C == 2 || g_btlUi->unk12A4[3]->unk1C == 3) {
        for (i = 0; i < 4; i++) {
            addPrim(&g_ot[18], &D_801050D8->unk0[i][g_drawBufs.cur]);
        }
        addPrim(&g_ot[18], &D_801050D8->unkA0[g_drawBufs.cur]);
    }
}

void func_800ED02C(RECT *r, s32 otz) {
    u32 *ot;
    u32 clut;
    Unk800689B0 *d2;
    Unk800689B0 *d1;
    RECT tw;
    RECT r2;
    u32 w;

    ot = &g_ot[otz];
    clut = win_getPalClut(D_80068E78[0].unk2) << 16;
    d2 = &D_80068E78[2];
    setlen(g_primPtr, 4);
    d1 = &D_80068E78[1];
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = r->x | (r->y << 16);
    w = D_80068E78[0].unk0;
    w |= D_80068E78[0].unk1 << 8;
    ((SprtWords *)g_primPtr)->uvclut = w | clut;
    ((SprtWords *)g_primPtr)->wh = D_80068E78[0].unk4 | (D_80068E78[0].unk5 << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = (r->x + r->w - d2->unk4) | (r->y << 16);
    ((SprtWords *)g_primPtr)->uvclut = D_80068E78[2].unk0 | (d2->unk1 << 8) | clut;
    ((SprtWords *)g_primPtr)->wh = d2->unk4 | (d2->unk5 << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    tw.y = 0;
    tw.h = 0xFF;
    tw.x = D_80068E78[1].unk0;
    tw.w = d1->unk4;
    SetTexWindow(g_primPtr, &D_800F8E10);
    addPrim(ot, g_primPtr);
    g_primPtr += 0xC;

    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = (r->x + D_80068E78[0].unk4) | (r->y << 16);
    ((SprtWords *)g_primPtr)->uvclut = (d1->unk1 << 8) | clut;
    ((SprtWords *)g_primPtr)->wh = (r->w - (D_80068E78[0].unk4 + d2->unk4)) | (d1->unk5 << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, d1->unk6);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(&tw);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    r2.x = r->x + 1;
    r2.y = r->y + 1;
    r2.w = r->w - 3;
    r2.h = r->h - 3;
    win_drawFill(&r2, otz, 0, 0, (g_gameState->config >> 18) & 1);
}

void func_800ED3F8(s32 x, s32 y, s32 otz, char *str) {
    s32 w;
    u32 wh;
    u32 xy;
    u32 tp;
    u32 stp;

    w = func_8002AE7C(D_800F8E18);
    wh = w | 0x70000;
    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = 0x62000000;
    xy = x | (y << 16);
    ((TileWords *)g_primPtr)->xy = xy;
    ((TileWords *)g_primPtr)->wh = wh;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
    setlen(g_primPtr, 1);
    tp = 0xE1000620;
    stp = 0xE6000001;
    ((DrTpageWords *)g_primPtr)->code = tp;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = stp;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    func_800ED7C0(x, y, otz, w, 7, D_800F8E20);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = 0xE6000002;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    ui_drawLabel(x, y, otz, (s32)str, 1);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = 0xE6000000;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = 0x62000000;
    ((TileWords *)g_primPtr)->xy = xy;
    ((TileWords *)g_primPtr)->wh = wh;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
    setlen(g_primPtr, 1);
    ((DrTpageWords *)g_primPtr)->code = tp;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = stp;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    ui_drawLabel(x, y, otz, (s32)str, 0);
}

void func_800ED7C0(s32 x, s32 y, s32 ot, s32 w, s32 h, CVECTOR *c) {
    CVECTOR c0;
    CVECTOR c1;
    CVECTOR *a;
    CVECTOR *b;
    CVECTOR *t;
    CVECTOR *u;
    s32 k;
    POLY_G4 *p;

    k = g_winLastVsync & 0x1F;
    if (k < 16) {
        if (k >= 8) {
            k = 15 - k;
        }
        a = &c[2];
        b = &c[3];
        u = t = &c[0];
    } else {
        k -= 16;
        if (k >= 8) {
            k = 15 - k;
        }
        a = &c[2];
        b = &c[3];
        u = t = &c[1];
    }
    c0.r = a->r + ((u->r - a->r) * k) / 7;
    c0.g = a->g + ((u->g - a->g) * k) / 7;
    c0.b = a->b + ((u->b - a->b) * k) / 7;
    c1.r = b->r + ((t->r - b->r) * k) / 7;
    c1.g = b->g + ((t->g - b->g) * k) / 7;
    c1.b = b->b + ((t->b - b->b) * k) / 7;
    p = (POLY_G4 *)g_primPtr;
    setPolyG4(p);
    setRGB0(p, c0.r, c0.g, c0.b);
    setRGB2(p, c0.r, c0.g, c0.b);
    setRGB1(p, c1.r, c1.g, c1.b);
    setRGB3(p, c1.r, c1.g, c1.b);
    setXYWH(p, x, y, w, h);
    addPrim(&g_ot[ot], p);
    g_primPtr += sizeof(POLY_G4);
}

void func_800EDAD4(void) {
    if (g_padRepeat & 0x01000000) {
        func_800EDDF8();
    } else if (g_padRepeat & BTN_CROSS) {
        func_800ECA50();
    } else if (g_padRepeat & PAD_DOWN) {
        func_800EDD1C();
    } else if (g_padRepeat & PAD_UP) {
        func_800EDCA0();
    } else {
        func_800EDD9C();
    }
}

void func_800EDB68(void) {
    func_800ECB5C();
}

void *btlui_getCmdHelpText(void) {
    s32 i;
    Unk800FD56C *e;
    if (g_btlCmdMenu.unk10.unk1 != 0 && g_btlCmdMenu.unk10.unk0 != 0) {
        i = 7;
        if (g_btlCmdMenu.unk10.unk1 == 1) {
            i = 4;
        }
    } else {
        i = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    }
    e = &D_800FD56C[i];
    func_8002BD54(D_80104FA0, e->unk2 + D_800FD8CC);
    text_append(D_80104FA0, text_get(0x19));
    return D_80104FA0;
}

void func_800EDC3C(void) {
    s32 v = g_btlCmdMenu.unit->unkB4->unk1B;
    btlui_setMenuState(1);
    if (v != D_80104E68.unk15) {
        pad_resetRepeats(v);
    }
    D_80104E68.unk15 = v;
}

void func_800EDCA0(void) {
    if (g_btlCmdMenu.unk10.unk1 == 0) {
        do {
            snd_playSe(0x67);
            g_btlCmdMenu.cursor = g_btlCmdMenu.cursor != 0 ? g_btlCmdMenu.cursor - 1 : 3;
        } while (g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor] == 0);
    }
}

void func_800EDD1C(void) {
    if (g_btlCmdMenu.unk10.unk1 == 0) {
        do {
            snd_playSe(0x67);
            g_btlCmdMenu.cursor = g_btlCmdMenu.cursor < 3 ? g_btlCmdMenu.cursor + 1 : 0;
        } while (g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor] == 0);
    }
}

void func_800EDD9C(void) {
    if (g_padHeld & PAD_LEFT) {
        func_800EE154();
    } else if (g_padHeld & PAD_RIGHT) {
        func_800EE24C();
    } else {
        func_800EE31C();
    }
}

void func_800EDDF8(void) {
    func_800EB518(&g_btlCmdMenu.unit->unkB4->unk49C, 0, g_btlCmdMenu.cursor, 0);
    btlui_setMenuState(4);
    win_setOnClosed(g_btlUi->unk12A4[3], func_800EE120);
    win_close(g_btlUi->unk12A4[3]);
}

void func_800EDE6C(Unk800EDE6C *arg0) {
    u32 hidden;
    Unk800EF80C s;
    Battle16CC *e;
    u8 b = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    e = &g_gameState->unk1C->unk8E4.battle->abilities[arg0->unk8];
    s.unk6 = arg0->unk8;
    s.unk0 = 1;
    s.unk1 = b;
    s.unk9 = g_btlCmdMenu.unk1;
    s.unk2 = e->unk0_0;
    s.unk3 = e->unk0_4;
    s.unk4 = e->unk0_31;
    s.unk7 = e->unk0_5;
    hidden = e->unk0_29;
    s.unk8 = 0;
    s.unk5 = hidden;
    func_800EF80C(&s);
}

void func_800EDF34(s32 arg0) {
    Unk800EE37C v;
    v.unk0 = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    v.unk4 = arg0;
    v.unk2 = g_btlCmdMenu.unk1;
    v.unk8 = D_800703FC.unk4;
    v.unkC = D_800703FC.unk7C;
    func_800EE37C(&v);
}

void func_800EDF90(s32 arg0) {
    Unk800EE37C v;
    v.unk0 = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    v.unk4 = arg0;
    v.unk2 = g_btlCmdMenu.unk1;
    v.unk8 = D_800703FC.unk68;
    v.unkC = D_800703FC.unk94;
    func_800EE37C(&v);
}

void func_800EDFEC(s32 arg0) {
    Unk800EE37C v;
    v.unk0 = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    v.unk4 = arg0;
    v.unk2 = g_btlCmdMenu.unk1;
    v.unk8 = D_800703FC.unkC0;
    v.unkC = D_800703FC.unk94;
    func_800EE37C(&v);
}

void func_800EE048(void) {
    s32 v = D_800F8E04[g_btlCmdMenu.unk10.unk1];
    g_btlCmdMenu.unit->unkCF = 0;
    g_btlCmdMenu.unit->unkB3 = 1;
    btl_setCmd((s32)g_btlCmdMenu.unit->unk4, v, D_800FD56C[v].unk8, g_btlCmdMenu.unit->unkB0, 0);
    btlui_setMenuState(4);
    win_setOnClosed(g_btlUi->unk12A4[3], func_800EE0FC);
    win_close(g_btlUi->unk12A4[3]);
}

void func_800EE0FC(Window *arg0) {
    btlui_setMenuState(0);
    g_btlCmdMenu.unk10.unk0 = 0;
}

void func_800EE120(Window *arg0) {
    btlui_setMenuState(0);
    g_btlCmdMenu.unk10.unk0 = 0;
    func_800EB358(g_btlCmdMenu.unit);
}

void func_800EE154(void) {
    if (D_80104FC0.unk1 == 2) {
        if (D_80104FC0.unk2 == 1) {
            if (++D_80104FC0.unk3 >= 5) {
                D_80104FC0.unk0 = 1;
                D_80104FC0.unk2 = 0;
            }
        } else if (D_80104FC0.unk2 == 2 && (g_padRepeat & PAD_LEFT)) {
            snd_playSe(0x417);
            D_80104FC0.unk2 = 1;
            D_80104FC0.unk1 = 2;
            D_80104FC0.unk0 = 0;
            D_80104FC0.unk3 = 0;
        }
    } else if (D_80104FC0.unk1 == 0) {
        if (g_padRepeat & PAD_LEFT) {
            snd_playSe(0x417);
            D_80104FC0.unk2 = 1;
            D_80104FC0.unk1 = 2;
            D_80104FC0.unk0 = 0;
            D_80104FC0.unk3 = 0;
        }
    } else {
        func_800EE31C();
    }
}

void func_800EE24C(void) {
    if (D_80104FC0.unk1 == 1) {
        if (D_80104FC0.unk2 == 1) {
            if (++D_80104FC0.unk3 >= 5) {
                D_80104FC0.unk0 = 1;
                D_80104FC0.unk2 = 0;
            }
        } else if (D_80104FC0.unk2 == 2 && (g_padRepeat & PAD_RIGHT)) {
            snd_playSe(0x417);
            D_80104FC0.unk2 = 1;
            D_80104FC0.unk1 = 1;
            D_80104FC0.unk0 = 0;
            D_80104FC0.unk3 = 0;
        }
    } else if (D_80104FC0.unk1 == 0) {
        if (g_padRepeat & PAD_RIGHT) {
            snd_playSe(0x417);
            D_80104FC0.unk2 = 1;
            D_80104FC0.unk1 = 1;
            D_80104FC0.unk0 = 0;
            D_80104FC0.unk3 = 0;
        }
    } else {
        func_800EE31C();
    }
}

void func_800EE31C(void) {
    Unk80104FC0 *p = &g_btlCmdMenu.unk10;
    if (p->unk1 != 0) {
        if (p->unk2 != 2) {
            p->unk2 = 2;
            p->unk3 = 0;
        } else if (p->unk3++ >= 3) {
            p->unk1 = 0;
            p->unk3 = 0;
        }
        p->unk0 = 0;
    }
}

void func_800EE37C(Unk800EE37C *arg0) {
    MenuList l;
    Unk80104F88Entry *e;
    u8 n;
    u8 f;
    Unk800EB518 *c;

    btlui_setMenuState(4);
    D_80104FD0.unk14 = g_btlUi->cmdUnit;
    D_80104FD0.unk0 = arg0->unk0;
    D_80104FD0.unk4 = (struct Unk80104FD4 *)arg0->unk4;
    D_80104FD0.unk8 = arg0->unk8;
    D_80104FD0.unkC = arg0->unkC;
    D_80104FD0.unk19 = arg0->unk2;
    D_80104FD0.unk10 = 0;
    D_80104FD0.unk12 = (arg0->unkC != 0) ? func_8002AE7C((u8 *)arg0->unkC) : 0;
    D_80104FD0.unk18 = func_800349BC((D_80104FD0.unk14->unk68 >> 1) & 0xF, 0xE2);
    n = D_80104FD0.unk4->unk7;
    menuList_init(&l, n, (n < 5) ? n : 4, 1, 0x44);
    f = (g_gameState->config >> 2) & 1;
    if (f == 1) {
        e = D_80104FD0.unk14->unkB4;
        c = &e->unk49C;
        l.top = c->unk8[e->unk49C.unk0];
        l.cursor = c->unk4[e->unk49C.unk0];
    }
    g_btlUi->unk12A4[2] = win_open(&D_800F8E30, 8, 1, 2);
    win_setList(g_btlUi->unk12A4[2], &l);
    win_setDrawContent(g_btlUi->unk12A4[2], btlui_drawListContent);
    win_setDrawOverlay(g_btlUi->unk12A4[2], btlui_drawListOverlay);
    win_setOnOpened(g_btlUi->unk12A4[2], (void (*)(Window *))func_800EF00C);
}

void btlui_listInput(void) {
    MenuList *l = (MenuList *)&g_btlUi->unk12A4[2]->unk30;
    Window *w;
    if (l->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            w = g_btlUi->unk12A4[2];
            func_800EB518(&D_80104FD0.unk14->unkB4->unk49C, 1, w->unk30.top, w->unk30.cursor);
            if (D_80104FD0.unk10 == 0) {
                func_800EF044();
            } else {
                D_80104FD0.unk10--;
            }
        } else if (g_padRepeat & BTN_CROSS) {
            func_800EEE3C();
        } else {
            menuList_up(l, g_padRepeat);
            menuList_down(l, g_padRepeat);
            if (g_gameState->unk1C->unk8E4.battle->escapeHeld == 0) {
                menuList_pageUp(l, g_padRepeat);
                menuList_pageDown(l, g_padRepeat);
            }
        }
    }
}

s32 func_800EE680(void) {
    Unk800EF80C s;
    MenuList *l = &g_btlUi->unk12A4[2]->unk30;
    s32 n = l->cursor + l->top;
    u8 k = D_800FDD10[D_80104FD0.unk4->unk8 + n];
    s32 r = btlui_getAbilityState(k);
    if (r != 2) {
        return 0;
    }
    {
        s.unk0 = r;
        s.unk1 = D_80104FD0.unk0;
        s.unk6 = func_800EF4D8(k);
        s.unk2 = g_gameState->unk1C->unk8E4.battle->abilities[k].unk0_0;
        s.unk3 = g_gameState->unk1C->unk8E4.battle->abilities[k].unk0_4;
        s.unk4 = g_gameState->unk1C->unk8E4.battle->abilities[k].unk0_31;
        s.unk7 = g_gameState->unk1C->unk8E4.battle->abilities[k].unk0_5;
        s.unk5 = g_gameState->unk1C->unk8E4.battle->abilities[k].unk0_29;
        s.unk8 = D_80104FD0.unk10;
        s.unk9 = D_80104FD0.unk19;
        func_800EF80C(&s);
        return 1;
    }
}

s32 func_800EE7F8(void) {
    Unk800EF80C s;
    MenuList *l = &g_btlUi->unk12A4[2]->unk30;
    UnkPair *p = btlui_getItemListEntry(l->cursor + l->top);
    u8 r = 0;
    Unk8006A774 *e = &D_8006A774[p->unk0];
    s32 n;

    if (p == NULL) {
        return 0;
    }
    if ((n = p->unk1) - btlui_getItemReserved(p->unk0) >= 0 && n != btlui_getItemReserved(p->unk0)) {
        s.unk0 = 2;
        s.unk1 = D_80104FD0.unk0;
        s.unk6 = p->unk0;
        s.unk2 = e->unk0_0;
        s.unk3 = e->unk0_4;
        s.unk4 = e->unk0_31;
        s.unk7 = e->unk0_5;
        s.unk5 = e->unk0_29;
        s.unk8 = D_80104FD0.unk10;
        s.unk9 = D_80104FD0.unk19;
        func_800EF80C(&s);
        r = 1;
    }
    return r;
}

s32 func_800EE938(void) {
    Unk800EF80C s;
    MenuList *l = &g_btlUi->unk12A4[2]->unk30;
    u8 r = 0;
    UnkPair *p = func_800EC130(l->cursor + l->top);
    Battle16CC *e = &g_gameState->unk1C->unk8E4.battle->abilities[D_80104FD0.unk4->unk8];
    s32 n;

    if (p == NULL) {
        return 0;
    }
    if ((n = p->unk1) - btlui_getItemReserved(p->unk0) >= 0 && n != btlui_getItemReserved(p->unk0)) {
        s.unk0 = 2;
        s.unk1 = D_80104FD0.unk0;
        s.unk6 = p->unk0;
        s.unk2 = e->unk0_0;
        s.unk3 = e->unk0_4;
        s.unk4 = e->unk0_31;
        s.unk7 = e->unk0_5;
        s.unk5 = e->unk0_29;
        s.unk8 = D_80104FD0.unk10;
        s.unk9 = D_80104FD0.unk19;
        func_800EF80C(&s);
        r = 1;
    }
    return r;
}

s32 btlui_getAbilityState(s32 arg0) {
    Unk80104F88Entry *e = D_80104FE4->unkB4;
    Battle16CC *a = &g_gameState->unk1C->unk8E4.battle->abilities[arg0];

    if (e->unk19 != 0 && e->unk3DC[arg0] == 0 && (s16)*e->unk1C[arg0] < (s16)e->unk31C[arg0]) {
        return 0;
    }
    if (a->unk8 & 2) {
        if ((u8)btl_hasStatus(D_80104FE4, BTL_STATUS_SILENCE)) {
            return 1;
        }
        if ((g_gameState->unk1C->unk8E4.battle->unk132C.unk0 >> 9) & 1) {
            return 1;
        }
    }
    if (btl_getMpCost((BtlCmd8 *)a) > D_80104FE4->mp) {
        return 1;
    }
    return 2;
}

void btlui_drawListOverlay(Window *arg0) {
    RECT *r = &arg0->unk8;
    s32 x;
    s32 y;
    s32 dy = 10;
    s32 dx = 0x74;
    if (D_80104FD0.unk8 != 0) {
        ui_drawLabel(arg0->unk8.x + 6, r->y - 4, arg0->unk18, D_80104FD0.unk8, 0);
    }
    if (D_80104FD0.unkC != 0) {
        ui_drawLabel(arg0->unk8.x - (D_80104FD0.unk12 - dx), r->y - 4, arg0->unk18, D_80104FD0.unkC, 0);
    }
    if (func_800EB2A4() == 2 || (func_800EB2A4() == 3 && func_800F13BC() == 2 && !(func_800F1360() & 0xFF) && (D_80104E80 & 1))) {
        x = arg0->unk8.x - 0x12;
        y = r->y + (arg0->unk30.cursor * 15 + dy);
        if (D_80104FD0.unk0 == 0x17 || D_80104FD0.unk0 == 0x15) {
            ui_drawSprite(x + 7, y + 2, arg0->unk18, D_80104FD0.unk10 + 0x5A, 0);
        }
        ui_drawHand(x, y, arg0->unk18);
    }
}

void func_800EED30(void) {
    if (D_80104FD0.unk0 == 0x1F) {
        if (g_btlUi->unk12B7 == 0 || g_btlUi->unk12B8.h != 0) {
            func_800EF044();
            snd_playSe(0x65);
        }
    }
    if (D_80104FD0.unk19 == 0 && (u8)btl_hasStatus(D_80104FD0.unk14, BTL_STATUS_TRANCE)) {
        func_800EF044();
        snd_playSe(0x65);
    }
}

void func_800EEDD4(void) {
    D_80104FD0.unk10++;
}

s32 func_800EEDF0(void) {
    s32 r = 0;
    s32 (*f)(void) = D_800F8E58[D_80104FD4->unk6];
    if (f != NULL) {
        r = f();
    }
    return r;
}

void func_800EEE3C(void) {
    Window *w;
    if ((u8)D_800F8E38[D_80104FD0.unk4->unk6]()) {
        snd_playSe(0x67);
        w = g_btlUi->unk12A4[2];
        func_800EB518(&D_80104FD0.unk14->unkB4->unk49C, 1, w->unk30.top, w->unk30.cursor);
    } else {
        snd_playSe(0x66);
    }
}

void btlui_drawListContent(Window *arg0) {
    RECT r;
    MenuView v;
    RECT *rc = &arg0->unk8;
    s32 i;
    menuList_getView((MenuList *)&arg0->unk30, &v);
    win_setClip(0, 7, 1);
    for (i = 0; i < v.unk0; i++) {
        func_800EF094(0, i, v.unk1 + i, &v);
    }
    if (((MenuList *)&arg0->unk30)->scroll & 4) {
        menuList_getRowsRect((MenuList *)&arg0->unk30, rc, &r);
        win_setClip(&r, 7, 1);
        func_800EEFA8(arg0);
    }
    win_setClip(rc, 7, 1);
}

void func_800EEFA8(Window *arg0) {
    MenuView v;
    s32 i;
    menuList_getPageView((MenuList *)&arg0->unk30, &v);
    for (i = 0; i < v.unk0; i++) {
        func_800EF094(0, i, v.unk1 + i, &v);
    }
}

void func_800EF00C(void) {
    btlui_setMenuState(2);
    g_btlUi->unk12A4[3]->unk1E = 0;
}

void func_800EF044(void) {
    btlui_setMenuState(4);
    win_setOnClosed(g_btlUi->unk12A4[2], func_800EF7D4);
    win_close(g_btlUi->unk12A4[2]);
}

void func_800EF094(s32 arg0, s32 arg1, s32 arg2, MenuView *arg3) {
    D_800F8E48[D_80104FD4->unk6](arg0, arg1, arg2, arg3);
}

void btlui_drawAbilityRow(s32 arg0, s32 arg1, s32 arg2, MenuView *arg3) {
    u8 buf[8];
    u8 k = D_800FDD10[D_80104FD4->unk8 + arg2];
    Window *w = g_btlUi->unk12A4[2];
    RECT *r = &w->unk8;
    s32 dy = w->unk6 - r->h;
    s32 res = btlui_getAbilityState(k);
    s16 x;
    s32 y;
    Battle16CC *a;
    s32 c;

    if (res != 0) {
        x = r->x;
        a = &g_gameState->unk1C->unk8E4.battle->abilities[k];
        y = r->y + arg1 * 15 + arg3->unk2 - dy;
        c = (res != 2) * 2;
        text_draw(x + 6, y, 7, D_800F909C + a->nameOfs, c);
        if (a->mpCost != 0) {
            text_formatNumber(btl_getMpCost((BtlCmd8 *)a), buf, 3, 1);
            ui_drawSmallDigits(x + 0x60, y + 4, 7, buf, c);
        }
    }
}

void btlui_drawItemRow(s32 arg0, s32 arg1, s32 arg2, Window *arg3) {
    u8 buf[8];
    s32 x5;
    UnkPair *e = btlui_getItemListEntry(arg2);
    Window *w = g_btlUi->unk12A4[2];
    RECT *r = &w->unk8;
    s32 dy = w->unk6 - r->h;
    s32 n;
    u8 id;
    s16 x;
    s32 y;
    Unk80069018 *it;
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            n = e->unk1 - btlui_getItemReserved(e->unk0);
        } else {
            n = 0;
        }
        if (n != 0) {
            id = e->unk0;
            x = r->x;
            it = &D_80069018[id];
            x5 = x + 5;
            y = r->y + arg1 * 15 + arg3->unk2 - dy;
            ui_drawItemIcon(x5, y, 7, id, 1);
            text_draw(x + 0x14, y, 7, D_8006A218 + it->unk0, 0);
            text_formatNumber(n, buf, 2, 1);
            ui_drawSmallDigits(x + 0x67, y + 4, 7, buf, 0);
        }
    }
}

void func_800EF390(s32 arg0, s32 arg1, s32 arg2, Window *arg3) {
    u8 buf[8];
    UnkPair *e = func_800EC130(arg2);
    Window *w = g_btlUi->unk12A4[2];
    RECT *r = &w->unk8;
    s32 dy = w->unk6 - r->h;
    s32 n;
    s32 x;
    s32 y;
    Unk80069018 *it;
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            n = e->unk1 - btlui_getItemReserved(e->unk0);
        } else {
            n = 0;
        }
        if (n != 0) {
            x = r->x;
            it = &D_80069018[e->unk0];
            y = r->y + arg1 * 15 + arg3->unk2 - dy;
            text_formatNumber(n, buf, 2, 1);
            text_draw(x + 5, y, 7, D_8006A218 + it->unk0, 0);
            ui_drawSmallDigits(x + 0x67, y + 4, 7, buf, 0);
        }
    }
}

s32 func_800EF4D8(s32 arg0) {
    s32 t;
    if (arg0 == 0x44) {
        switch (D_80104FE4->unkB4->unk14->equip[4]) {
        case 0xE4:
            arg0 = 0x45;
            break;
        case 0xE5:
            arg0 = 0x46;
            break;
        case 0xE3:
            arg0 = 0x47;
            break;
        }
    } else if (arg0 == 0x42) {
        t = D_80104FE4->unkB4->unk14->equip[4] == 0xDE;
        arg0 = t + 0x42;
    }
    return arg0;
}

s32 btl_getMpCost(BtlCmd8 *arg0) {
    s32 cost = arg0->mpCost;
    if (arg0->unkB & 4) {
        if (D_8007AF02 != 0) {
            cost *= 4;
        }
    }
    if (D_80104FE8 != 0) {
        cost >>= 1;
    }
    return cost;
}

char *func_800EF5E0(void) {
    MenuList *l = &g_btlUi->unk12A4[2]->unk30;
    s32 n = l->cursor + l->top;
    u8 k = D_800FDD10[D_80104FD4->unk8 + n];
    if (btlui_getAbilityState(k) == 0) {
        return NULL;
    }
    return D_800F96E4 + D_800FB1F0[k].unk0;
}

u8 *func_800EF674(void) {
    Unk16 *w = &g_btlUi->unk12A4[2]->unk30;
    UnkPair *e = btlui_getItemListEntry(w->cursor + w->top);
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            if (e->unk1 != btlui_getItemReserved(e->unk0)) {
                return D_800FB4F0 + D_800FD36C[e->unk0];
            }
            return NULL;
        }
    }
    return NULL;
}

u8 *func_800EF724(void) {
    Unk16 *w = &g_btlUi->unk12A4[2]->unk30;
    UnkPair *e = func_800EC130(w->cursor + w->top);
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            if (e->unk1 != btlui_getItemReserved(e->unk0)) {
                return D_800FB4F0 + D_800FD36C[e->unk0];
            }
            return NULL;
        }
    }
    return NULL;
}

void func_800EF7D4(Window *arg0) {
    btlui_setMenuState(1);
    g_btlUi->unk12A4[3]->unk1E = 1;
}

void func_800EF80C(Unk800EF80C *arg0) {
    s32 i;

    g_btlTarget.unk20 = g_btlUi->cmdUnit;
    g_btlTarget.unk0 = arg0->unk0;
    g_btlTarget.unk1 = arg0->unk1;
    g_btlTarget.unk2 = arg0->unk2;
    g_btlTarget.unk3 = arg0->unk3;
    g_btlTarget.unk4 = arg0->unk4;
    g_btlTarget.unkA = arg0->unk3;
    g_btlTarget.unk1A = arg0->unk6;
    g_btlTarget.unkF = arg0->unk7;
    g_btlTarget.unkB = arg0->unk5;
    g_btlTarget.unk10 = arg0->unk8;
    g_btlTarget.unk6 = D_800F8E84[g_btlTarget.unk2];
    g_btlTarget.unkC = 0;
    g_btlTarget.unkD = func_800349BC((g_btlTarget.unk20->unk68 >> 1) & 0xF, 0xE2);
    g_btlTarget.unkE = arg0->unk9;
    g_btlTarget.unk7 = (u32)(arg0->unk2 - 6) < 3;
    g_btlTarget.unk8 = arg0->unk2 == 0 || arg0->unk2 == 3 || arg0->unk2 == 9 || arg0->unk2 == 6;
    g_btlTarget.unk9 = (u32)(arg0->unk2 - 3) < 3;
    D_80105020.unk2 = 0;
    D_80105020.unk0 = 0;
    func_800F142C();
    func_800EFA10(1);
    if (g_btlTarget.unkF == 0) {
        if (D_80104E6B == 0) {
            btlui_setMenuState(3);
        } else {
            func_800F1970(3);
        }
    } else {
        if (D_80104E6B != 0) {
            func_800F1970(5);
        }
        func_800F27CC(g_btlTarget.unkF);
    }
    for (i = 0; i < 2; i++) {
        pad_setRepeatEnabled(i, 2, 1);
        pad_setRepeatEnabled(i, 3, 1);
    }
}

void func_800EFA10(s32 arg0) {
    BtlUnit *u;
    if (g_btlTarget.unk2 == 0xC) {
        for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
            if (((u->unk68 >> 28) & 1) && (!(u->status & BTL_STATUS_KO) || g_btlTarget.unkB != 0)) {
                u->unk68 |= 0x20000000;
            }
        }
    } else if (g_btlTarget.unk2 == 0xD) {
        g_btlTarget.unk20->unk68 |= 0x20000000;
    } else if (arg0 & 0xFF) {
        func_800F1790(D_800F8E68[g_btlTarget.unk2][0], D_800F8E68[g_btlTarget.unk2][1]);
    } else {
        func_800F16DC(D_800F8E68[g_btlTarget.unk2][0], D_800F8E68[g_btlTarget.unk2][1]);
    }
}

s32 func_800EFB3C(s32 arg0) {
    BtlUnit *list[4];
    BtlUnit *p;
    BtlUnit *u;
    s32 cur = 0;
    s32 n;
    s32 k;
    if (g_btlTarget.unk6 != 0) {
        return 0;
    }
    if (g_btlTarget.unk2 == 0xD) {
        return 0;
    }
    u = BtlFindTargetCursor();
    if (u == NULL) {
        return 0;
    }
    memset((s32)list, 0, 0x10);
    n = 0;
    for (p = g_gameState->unk1C->unk8E4.battle->unk6F8; p != NULL; p = p->unk0) {
        if (((p->unk68 >> 28) & 1) && (p->unk68 & 1) == g_btlTarget.unkA && (!(p->status & BTL_STATUS_KO) || g_btlTarget.unkB != 0)) {
            list[n] = p;
            if (p == u) {
                cur = n;
            }
            n++;
        }
    }
    if (n >= 2) {
        u->unk68 &= ~0x20000000;
        if (!(arg0 & 0xFF)) {
            k = cur - 1;
            if (cur == 0) {
                k = n - 1;
            }
        } else {
            k = 0;
            if (cur < n - 1) {
                k = cur + 1;
            }
        }
        list[k]->unk68 |= 0x20000000;
        return 1;
    }
    return 0;
}

s32 func_800EFD1C(u8 arg0) {
    BtlUnit *cur;
    BtlUnit *u;
    BtlUnit *t;
    BtlUnit *list[4];
    s32 n;
    s32 i;
    u8 side;
    s32 k;

    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if ((u->unk68 & 0x30000000) == 0x30000000) {
            cur = u;
            goto found;
        }
    }
    cur = NULL;
found:
    if (g_btlTarget.unk8 != 0) {
        if (arg0 == (k = cur->unk68 & 1)) {
            return 0;
        }
        side = !k;
        g_btlTarget.unkA = side;
        memset(list, 0, sizeof(list));
        n = 0;
        for (t = g_gameState->unk1C->unk8E4.battle->unk6F8; t != NULL; t = t->unk0) {
            if (((t->unk68 >> 28) & 1) && (t->unk68 & 1) == side) {
                if (!(t->status & BTL_STATUS_KO) || g_btlTarget.unkB) {
                    list[n++] = t;
                }
            }
        }
        if (n != 0) {
            cur->unk68 &= ~0x20000000;
            if (g_btlTarget.unk6 == 0) {
                list[0]->unk68 |= 0x20000000;
            } else {
                func_800F142C();
                for (i = 0; i < n; i++) {
                    list[i]->unk68 |= 0x20000000;
                }
            }
            return 1;
        }
    }
    return 0;
}

void func_800EFF14(void) {
    u8 old = g_btlTarget.unk6;
    BtlUnit *u;
    BtlUnit *p;
    s32 n;
    if (g_btlTarget.unk9 != 0) {
        if (old != 0) {
            if ((g_btlTarget.unk24->unk68 >> 29) & 1) {
                u = g_btlTarget.unk24;
            } else {
                u = BtlFindTargetCursor();
            }
            func_800F142C();
            u->unk68 |= 0x20000000;
            g_btlTarget.unk6 = 0;
        } else {
            p = g_gameState->unk1C->unk8E4.battle->unk6F8;
            u = BtlFindTargetCursor();
            n = 0;
            g_btlTarget.unk24 = u;
            func_800F142C();
            for (; p != NULL; p = p->unk0) {
                if (((p->unk68 >> 28) & 1) && (p->unk68 & 1) == (u->unk68 & 1) && (!(p->status & BTL_STATUS_KO) || g_btlTarget.unkB != 0)) {
                    p->unk68 |= 0x20000000;
                    n++;
                }
            }
            if (n != 0) {
                g_btlTarget.unk6 = 1;
            }
        }
        if (old != g_btlTarget.unk6) {
            snd_playSe(0x67);
        }
    }
}

s32 btlui_isCmdUsable(void) {
    Unk800FD56C *e = &D_800FD56C[g_btlTarget.unk1];
    Battle16CC *b;
    u8 cost;
    if (g_btlTarget.unk1 == 0x1F && (g_btlUi->unk12B7 == 0 || g_btlUi->unk12B8.h != 0)) {
        return 0;
    }
    if (g_btlTarget.unkE == 0 && (btl_hasStatus(g_btlTarget.unk20, BTL_STATUS_TRANCE) & 0xFF)) {
        return 0;
    }
    if (e->unk6 == 1) {
        b = &g_gameState->unk1C->unk8E4.battle->abilities[g_btlTarget.unk1A];
        cost = b->mpCost;
        if (g_btlTarget.unkD != 0) {
            cost >>= 1;
        }
        if (g_btlTarget.unk20->mp < cost) {
            return 0;
        }
        if ((b->unk8 & 2) && (btl_hasStatus(g_btlTarget.unk20, BTL_STATUS_SILENCE) & 0xFF)) {
            return 0;
        }
    }
    if ((u32)(e->unk6 - 2) < 2 && item_getCount(g_btlTarget.unk1A) - btlui_getItemReserved(g_btlTarget.unk1A) <= 0) {
        return 0;
    }
    return 1;
}

s32 func_800F028C(void) {
    BtlUnit *u = BtlFindTargetCursor();
    BtlUnit *p;
    s32 n;
    if (u == NULL) {
        func_800EFA10(0);
        return 1;
    }
    if (g_btlTarget.unk6 == 0) {
        if ((u->status & BTL_STATUS_KO) && g_btlTarget.unkB == 0) {
            u->unk68 &= ~0x20000000;
            func_800EFA10(0);
            return 1;
        }
        return 1;
    }
    if (g_btlTarget.unk2 != 0xC) {
        p = g_gameState->unk1C->unk8E4.battle->unk6F8;
        n = 0;
        func_800F142C();
        while (p != NULL) {
            if (((p->unk68 >> 28) & 1) && (p->unk68 & 1) == (u->unk68 & 1) && (!(p->status & BTL_STATUS_KO) || g_btlTarget.unkB != 0)) {
                p->unk68 |= 0x20000000;
                n++;
            }
            p = p->unk0;
        }
        if (n != 0) {
            g_btlTarget.unk6 = 1;
        }
    }
    return 1;
}

void btlui_confirmTarget(void) {
    s32 mask = 0;
    u8 ok = 1;
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    Unk800FD56C *e = &D_800FD56C[g_btlTarget.unk1];
    Unk80104FF0 *d;
    for (; u != NULL; u = u->unk0) {
        if ((u->unk68 & 0x30000000) == 0x30000000) {
            mask |= u->unkB0;
        }
    }
    d = &g_btlTarget;
    btlui_setMenuState(4);
    if (d->unk1 == 0x17 || d->unk1 == 0x15) {
        ok = func_800F0620(mask);
    } else {
        d->unk20->unkB3 = 1;
        d->unk20->unkCF = 0;
        btl_setCmd((s32)g_btlTarget.unk20->unk4, g_btlTarget.unk1, g_btlTarget.unk1A, mask & 0xFFFF, func_800F1B78_inline());
    }
    func_800F142C();
    if (ok) {
        if ((u32)(e->unk6 - 2) < 2) {
            func_800EC14C(g_btlTarget.unk1A);
        }
        func_800EB46C();
    }
    snd_playSe(0x67);
}

s32 func_800F0620(s32 arg0) {
    BtlUnit *u;
    s32 n;
    s8 v;
    s32 r = 0;
    if (g_btlTarget.unk10 == 0) {
        g_btlTarget.unk1C = arg0;
        g_btlTarget.unk13 = g_btlTarget.unk1;
        g_btlTarget.unk18 = g_btlTarget.unk1A;
        if ((u32)(g_btlTarget.unk2 - 9) < 3) {
            v = 2;
        } else {
            n = 0;
            for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                if ((u->unk68 & 0x30000000) == 0x30000000) {
                    n++;
                }
            }
            v = n >= 2;
        }
        g_btlTarget.unk14 = v;
        btlui_cancelTarget();
        func_800EEDD4();
    } else {
        g_btlTarget.unk20->unkCF = 0;
        g_btlTarget.unk20->unkB3 = 1;
        btl_setCmd((s32)g_btlTarget.unk20->unk10, g_btlTarget.unk13, g_btlTarget.unk18, (u16)g_btlTarget.unk1C, g_btlTarget.unk14);
        if ((u32)(g_btlTarget.unk2 - 9) < 3) {
            v = 2;
        } else {
            n = 0;
            for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                if ((u->unk68 & 0x30000000) == 0x30000000) {
                    n++;
                }
            }
            v = n >= 2;
        }
        btl_setCmd((s32)g_btlTarget.unk20->unk4, g_btlTarget.unk1, g_btlTarget.unk1A, arg0 & 0xFFFF, v);
        r = 1;
    }
    return r;
}

void btlui_cancelTarget(void) {
    BtlUnit *u;
    s32 i;
    if (g_btlTarget.unkF == 0) {
        if (g_btlTarget.unkC != 0) {
            func_800F1A20(g_btlTarget.unk0);
        } else {
            btlui_setMenuState(g_btlTarget.unk0);
        }
    } else {
        if (g_btlTarget.unkC != 0) {
            func_800F1A20(5);
        }
        func_800F2890(g_btlTarget.unk0);
    }
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        u->unk68 &= ~0x20000000;
    }
    for (i = 0; i < 2; i++) {
        pad_setRepeatEnabled(i, 2, 0);
        pad_setRepeatEnabled(i, 3, 0);
    }
}

void btlui_updateTargetWindow(void) {
    if ((g_btlTarget.unkC == 0 || (u32)(g_btlUi->unk12A4[0]->unk1C - 1) >= 2) && g_btlTarget.unkC != D_80104E6B) {
        if (D_80104E6B != 0) {
            if (g_btlTarget.unkC == 0) {
                btlui_setMenuState(4);
                g_btlUi->unk12A4[0] = win_open(&D_800F8EF4, 5, 2, 0);
                win_setOpenAnim(g_btlUi->unk12A4[0], 4);
                win_setOnOpened(g_btlUi->unk12A4[0], func_800F1A8C);
                win_setDrawContent(g_btlUi->unk12A4[0], func_800F0A54);
                win_setDrawOverlay(g_btlUi->unk12A4[0], func_800F0BE0);
                g_btlTarget.unk11 = 3;
                g_btlTarget.unkC = 1;
            }
        } else if (g_btlTarget.unkC != 0) {
            g_btlTarget.unk12 = 3;
            btlui_setMenuState(4);
            win_setOnClosed(g_btlUi->unk12A4[0], func_800F1AF8);
            win_close(g_btlUi->unk12A4[0]);
        }
    }
}

void func_800F0A54(Window *arg0) {
    s32 y1 = arg0->unk2 + 8;
    s32 y2 = y1;
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 m;
    u32 f;
    u8 *name;
    s32 c;
    Window *w = arg0;
    ui_drawEtchedLine(arg0->unk0 + 0x6E, arg0->unk2 + 4, arg0->unk18, 9 - arg0->unk6);
    m = func_8002BE88();
    for (; u != NULL; u = u->unk0) {
        name = btl_getUnitName(u);
        f = u->unk68;
        if ((f >> 28) & 1) {
            if (f & 1) {
                if (u->hp != 0) {
                    c = ((u16)(u->max->maxHp / 6) >= u->hp) * 4;
                } else {
                    c = 3;
                }
                text_draw(w->unk0 + 0x7A, y2, arg0->unk18, name, c);
                y2 += 15;
            } else {
                c = ((u8)btl_hasStatus(u, BTL_STATUS_KO) != 0) * 2;
                text_drawBlock(w->unk0 + 0xC, y1, arg0->unk18, name, c);
                y1 += 15;
            }
        }
    }
    func_8002BE98(m);
}

void func_800F0BE0(Window *arg0) {
    Window *w = arg0;
    s32 n = 0;
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    RECT *r = &arg0->unk8;
    BtlUnit *p;
    RECT rc;
    s32 yl;
    s32 yr;
    for (p = u; p != NULL; p = p->unk0) {
        if ((p->unk68 >> 29) & 1) {
            n++;
        }
    }
    if (arg0->unk1C != 3) {
        win_setClip(NULL, arg0->unk18, 4);
    }
    ui_drawLabel(w->unk0 + 8, r->y - 3, arg0->unk18, D_800703FC.unk3C, 0);
    ui_drawLabel(w->unk0 + 0x76, r->y - 3, arg0->unk18, D_800703FC.unkA0, 0);
    if (arg0->unk1C != 3) {
        rc.x = r->x;
        rc.y = r->y - 3;
        rc.w = r->w;
        rc.h = 8;
        win_setClip(&rc, arg0->unk18, 4);
    }
    if (arg0->unk1C == 3 && (n < 2 || !(D_80104E80 & 1))) {
        yl = r->y + 0xC;
        yr = yl;
        for (; u != NULL; u = u->unk0) {
            if ((u->unk68 >> 28) & 1) {
                if ((u->unk68 >> 29) & 1) {
                    if (u->unk68 & 1) {
                        ui_drawHand(r->x + 0x62, yr, arg0->unk18 - 1);
                    } else {
                        ui_drawHand(r->x - 0xC, yl, arg0->unk18 - 1);
                    }
                }
                if (u->unk68 & 1) {
                    yr += 15;
                } else {
                    yl += 15;
                }
            }
        }
    }
}

void btlui_drawTargetCursors(void) {
    u32 c = 0x64808080;
    Unk80105020 *b = &D_80105020;
    u32 *clut = &D_800686D8;
    s32 i;
    SprtWords *p;
    DR_TPAGE *t;
    if (b->unk0 >= 2 && !(D_80104E80 & 3)) {
        c = 0x64404040;
    }
    for (i = 0; i < b->unk0; i++) {
        setlen(g_primPtr, 4);
        p = (SprtWords *)g_primPtr;
        p->rgbc = c;
        p->xy = (u16)b->unk14[i][0] | (b->unk14[i][1] << 16);
        p->uvclut = *clut;
        p->wh = 0xE0018;
        addPrim(&g_ot[17], p);
        g_primPtr = (u8 *)(p + 1);
    }
    setlen(g_primPtr, 1);
    t = (DR_TPAGE *)g_primPtr;
    t->code[0] = 0xE100061F;
    addPrim(&g_ot[17], t);
    g_primPtr = (u8 *)(t + 1);
}

void *func_800F0F60(void) {
    BtlUnit *u;
    BtlUnit *t;
    s32 n;
    s32 k;

    if (g_btlTarget.unk2 < 3) {
        t = find_target();
        if (t == NULL) {
            return NULL;
        }
        func_8002BD54(D_80104FA0, text_get(D_80104FF2 + 5));
        text_append(D_80104FA0, btl_getUnitName(t));
    } else if ((u8)(g_btlTarget.unk2 - 3) < 3) {
        n = 0;
        k = g_btlTarget.unk2 - 3;
        for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            if ((u->unk68 >> 29) & 1) {
                n++;
            }
        }
        if (n < 2) {
            t = find_target();
            if (t == NULL) {
                return NULL;
            }
            func_8002BD54(D_80104FA0, text_get(k + 0x13));
            text_append(D_80104FA0, btl_getUnitName(t));
        } else {
            func_8002BD54(D_80104FA0, text_get(k + 0x16));
        }
    } else {
        func_8002BD54(D_80104FA0, text_get(g_btlTarget.unk2 + 5));
    }
    text_append(D_80104FA0, text_get(0x1A));
    return D_80104FA0;
}

void func_800F1150(BtlUnit *arg0) {
    if (g_btlTarget.unk6 != 0 && D_800F8EE4[g_btlTarget.unk2] != 0) {
        func_800EFA10(0);
    }
}

void func_800F11A4(BtlUnit *arg0) {
    if ((arg0->unk68 >> 29) & 1) {
        arg0->unk68 &= ~0x20000000;
        if (g_btlTarget.unk6 == 0) {
            func_800EFA10(0);
        }
    }
}

void btlui_targetInput(void) {
    u32 v;
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        btlui_cancelTarget();
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        btlui_confirmTarget();
        return;
    }
    if (g_gameState->unk1C->unk8E4.battle->escapeHeld == 0 && (g_padRepeat & (BTN_L1 | BTN_R1))) {
        func_800EFF14();
        return;
    }
    if (g_padRepeat & BTN_L2) {
        snd_playSe(0x67);
        v = g_gameState->config;
        g_gameState->config = (v & ~0x80000) | ((((v >> 19) & 1) ^ 1) << 19);
        return;
    }
    btlui_targetMove();
}

void func_800F1300(void) {
    if (!(btlui_isCmdUsable() & 0xFF)) {
        snd_playSe(0x65);
        btlui_cancelTarget();
    } else if (func_800F028C() & 0xFF) {
        btlui_updateTargetWindow();
    }
}

u8 func_800F1360(void) {
    return D_80104FFC;
}

Window *func_800F1370(void) {
    return g_btlUi->unk12A4[0];
}

s32 func_800F1388(void) {
    s32 ret = 0;
    if (g_btlUi->menuState == 3) {
        ret = D_80104FFF != 0;
    }
    return ret;
}

s32 func_800F13BC(void) {
    return g_btlTarget.unk0;
}

s32 func_800F13CC(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 n = 0;
    while (u != NULL) {
        if ((u->unk68 >> 29) & 1) {
            n++;
        }
        u = u->unk0;
    }
    return n;
}

void func_800F142C(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    while (u != NULL) {
        u->unk68 &= ~0x20000000;
        u = u->unk0;
    }
}

void func_800F1484(void) {
    s32 n;
    s32 m;
    BtlUnit *u;
    Unk80105020 *p;
    m = n = 0;
    u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    p = &D_80105020;
    if (D_80104E6A != 0) {
        for (; u != NULL; u = u->unk0) {
            if (((u->unk68 >> 28) & 1) && u->unkBC != 0) {
                p->unk14[n][0] = u->unkB8 - 0x18;
                p->unk14[n][1] = u->unkBA - 7;
                n++;
                if (u->unk68 & 1) {
                    p->unk4[m] = u;
                    m++;
                }
            }
        }
    }
    p->unk0 = n;
    p->unk2 = m;
}

Unk80105020 *func_800F1548(void) {
    return &D_80105020;
}

void btlui_targetMove(void) {
    s32 i;
    s32 m;
    if (D_80104FFC == 0) {
        m = func_800C2DF4();
    } else {
        m = 0;
    }
    for (i = 0; i < 4; i++) {
        if (g_padRepeat & D_800F8EA4[m][i]) {
            D_800F8E94[i]();
        }
    }
}

void func_800F160C(void) {
    if (func_800EFB3C(0) & 0xFF) {
        snd_playSe(0x67);
    }
}

void func_800F1640(void) {
    if (func_800EFB3C(1) & 0xFF) {
        snd_playSe(0x67);
    }
}

void func_800F1674(void) {
    if (func_800EFD1C(0) & 0xFF) {
        snd_playSe(0x67);
    }
}

void func_800F16A8(void) {
    if (func_800EFD1C(1) & 0xFF) {
        snd_playSe(0x67);
    }
}

void func_800F16DC(u8 arg0, u8 arg1) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    if (arg0 == 0xFF) {
        arg0 = D_80104FFA;
    }
    for (; u != NULL; u = u->unk0) {
        if (((u->unk68 >> 28) & 1) && (u->unk68 & 1) == arg0) {
            if (!(u->status & BTL_STATUS_KO) || g_btlTarget.unkB != 0) {
                u->unk68 |= 0x20000000;
                if (arg1 == 0) {
                    return;
                }
            }
        }
    }
}

void func_800F1790(u8 arg0, u8 arg1) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    u8 found = 0;
    BtlUnit *t;
    u32 f;
    u32 side;

    if (arg0 == 0xFF) {
        arg0 = D_80104FFA;
    }
    if (g_btlTarget.unk4 != 0) {
        t = func_800F18F0(arg0);
        if (t != NULL) {
            t->unk68 |= 0x20000000;
            return;
        }
    }
    for (; u != NULL; u = u->unk0) {
        f = u->unk68;
        side = f & 1;
        if (!((f >> 28) & 1) || side != arg0) {
            continue;
        }
        if ((u->status & BTL_STATUS_KO) && g_btlTarget.unkB == 0) {
            continue;
        }
        if (arg1 == 0 && side != 0) {
            if (u == g_btlTarget.unk20) {
                u->unk68 = f | 0x20000000;
                found = 1;
            }
        } else {
            found = 1;
            u->unk68 |= 0x20000000;
        }
        if (arg1 == 0 && found) {
            return;
        }
    }
}

BtlUnit *func_800F18F0(u8 arg0) {
    BtlUnit *u;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if (((u->unk68 >> 28) & 1) && (u->unk68 & 1) == arg0 && (u->status & BTL_STATUS_KO)) {
            return u;
        }
    }
    return 0;
}

void func_800F1970(s8 arg0) {
    Window *w;
    btlui_setMenuState(4);
    w = win_open(&D_800F8EF4, 5, 2, 0);
    g_btlUi->unk12A4[0] = w;
    win_setOpenAnim(w, 4);
    win_setOnOpened(g_btlUi->unk12A4[0], func_800F1A8C);
    win_setDrawContent(g_btlUi->unk12A4[0], func_800F0A54);
    win_setDrawOverlay(g_btlUi->unk12A4[0], func_800F0BE0);
    g_btlTarget.unk11 = arg0;
    g_btlTarget.unkC = 1;
}

void func_800F1A20(s32 arg0) {
    if (g_btlTarget.unkC != 0) {
        g_btlTarget.unk12 = arg0;
        btlui_setMenuState(4);
        win_setOnClosed(g_btlUi->unk12A4[0], func_800F1AF8);
        win_close(g_btlUi->unk12A4[0]);
    }
}

void func_800F1A8C(Window *arg0) {
    if (g_btlTarget.unk0 == 2) {
        win_setVisible(g_btlUi->unk12A4[2], 0);
    }
    win_setVisible(g_btlUi->unk12A4[3], 0);
    btlui_setMenuState(g_btlTarget.unk11);
}

void func_800F1AF8(Window *arg0) {
    if (g_btlTarget.unk0 == 2) {
        win_setVisible(g_btlUi->unk12A4[2], 1);
    }
    win_setVisible(g_btlUi->unk12A4[3], 1);
    g_btlTarget.unkC = 0;
    g_btlUi->unk12A4[0] = 0;
    btlui_setMenuState((u8)g_btlTarget.unk12);
}

s32 func_800F1B78(void) {
    BtlUnit *u;
    s32 n;
    if (g_btlTarget.unk2 >= 9 && g_btlTarget.unk2 <= 11) {
        return 2;
    }
    n = 0;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if ((u->unk68 & 0x30000000) == 0x30000000) {
            n++;
        }
    }
    return n >= 2;
}

void func_800F1BFC(Window *arg0) {
    Window *w = arg0;
    s32 n = 0;
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    Unk80105020 *b = func_800F1548();
    Unk80105078 *m = func_800F3EA4();
    u32 c;
    s32 y;
    s32 x;
    s32 k;
    s32 yy;
    Unk80068EE8 *d;
    SprtWords *p;
    DR_TPAGE *t;

    if (b->unk2 == 0 || arg0->unk1C != 3 || m->unk0 == 0) {
        return;
    }
    c = 0x64808080;
    if (b->unk2 >= 2 && !(D_80104E80 & 3)) {
        c = 0x64404040;
    }
    y = w->unk2 + 5;
    x = w->unk0 - 0xE;
    for (; u != NULL; u = u->unk0) {
        if (!(u->unk68 & 1)) {
            continue;
        }
        if (u == b->unk4[n]) {
            for (k = 0, yy = y; k < 2; k++, yy += 6) {
                d = &D_80068EE8[k];
                setlen(g_primPtr, 4);
                p = (SprtWords *)g_primPtr;
                p->rgbc = c;
                p->xy = x | (yy << 16);
                p->uvclut = d->unk0;
                p->wh = d->unk4 | (d->unk5 << 16);
                addPrim(&g_ot[arg0->unk18 - 1], p);
                g_primPtr = (u8 *)(p + 1);
            }
            n++;
            if (n >= b->unk2) {
                break;
            }
        }
        y += 0xC;
    }
    setlen(g_primPtr, 1);
    t = (DR_TPAGE *)g_primPtr;
    t->code[0] = (D_80068EEE & 0x9FF) | 0xE1000600;
    addPrim(&g_ot[arg0->unk18], t);
    g_primPtr = (u8 *)(t + 1);
}

void func_800F1E84(Window *arg0) {
    u8 a[8];
    u8 b[8];
    BtlUnit *u;
    Window *w = arg0;
    s32 y = arg0->unk2 + 6;
    s32 dy = arg0->unk6 - arg0->unk8.h;
    s32 c;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0 && (u->unk68 & 1); u = u->unk0) {
        c = 3;
        if (u->hp != 0) {
            c = ((u->status & BTL_STATUS_LOW_HP) != 0) * 4;
        }
        b[0] = 0x2F;
        text_formatNumber(u->hp, a, 4, 1);
        text_formatNumber(u->max->maxHp, b + 1, -1, 1);
        ui_drawDigits(w->unk0 + 4, y, arg0->unk18, a, c);
        ui_drawDigits(w->unk0 + 0x20, y - dy, arg0->unk18, b, c);
        y += 12;
    }
}

void func_800F1FBC(Window *arg0) {
    u8 hp[8];
    u8 max[8];
    Window *w = arg0;
    s32 y = arg0->unk2 + 6;
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 dy = arg0->unk6 - arg0->unk8.h;
    s32 yy;
    s32 c;
    while (u != NULL) {
        if (u->unk68 & 1) {
            yy = y - dy;
            c = (u->mp < (s16)(u->max->maxMp / 6)) * 4;
            max[0] = 0x2F;
            text_formatNumber(u->mp, hp, 3, 1);
            y += 0xC;
            text_formatNumber((s16)u->max->maxMp, &max[1], -1, 1);
            ui_drawDigits(w->unk0 + 4, yy, arg0->unk18, hp, c);
            ui_drawDigits(w->unk0 + 0x19, yy, arg0->unk18, max, c);
        }
        u = u->unk0;
    }
}

void func_800F2118(Window *arg0) {
    BtlUnit *u;
    s32 i;
    s32 n;
    s32 x;
    Window *w = arg0;
    s32 d = arg0->unk6 - arg0->unk8.h;
    s32 y;
    u32 st;
    d -= 6;
    y = arg0->unk2 - d;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if (u->unk68 & 1) {
            n = 0;
            st = u->status | u->unk38;
            x = w->unk0 + 4;
            for (i = 0; i < 16; i++) {
                if (st & D_800F8F18[i].unk0) {
                    ui_drawSprite(x, y, arg0->unk18, D_800F8F18[i].unk4, 0);
                    x += 12;
                    if (++n >= 9) {
                        break;
                    }
                }
            }
            y += 12;
        }
    }
}

void func_800F224C(Window *arg0) {
    BtlUnit *u;
    s32 i;
    s32 x;
    Window *w = arg0;
    s32 d = arg0->unk6 - arg0->unk8.h;
    s32 y;
    u32 st;
    d -= 6;
    y = arg0->unk2 - d;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if (u->unk68 & 1) {
            st = u->status | u->unk38;
            x = w->unk0 + 4;
            for (i = 0; i < 8; i++) {
                if (st & D_800F8F98[i].unk0) {
                    ui_drawSprite(x, y, arg0->unk18, D_800F8F98[i].unk4, 0);
                }
                x += D_800F8F98[i].unk6;
            }
            y += 12;
        }
    }
}

void func_800F2374(void) {
    Unk80105078 *p;
    RECT r;

    p = func_800F3EA4();
    setRECT(&r, 0xD0, p->unk60 - 4, 0x40, 4);
    r.x += D_801051E8[g_gameState->unk1C->unk8 & 1][0];
    r.y += D_801051E8[g_gameState->unk1C->unk8 & 1][1];
    SetDrawMove((DR_MOVE *)g_primPtr, &r, 0x3C0, 0x1F8);
    addPrim(&g_ot[19], g_primPtr);
    g_primPtr += sizeof(DR_MOVE);
    setRECT(&r, 0x110, p->unk60 - 4, 0x30, 4);
    r.x += D_801051E8[g_gameState->unk1C->unk8 & 1][0];
    r.y += D_801051E8[g_gameState->unk1C->unk8 & 1][1];
    SetDrawMove((DR_MOVE *)g_primPtr, &r, 0x3C0, 0x1FC);
    addPrim(&g_ot[19], g_primPtr);
    g_primPtr += sizeof(DR_MOVE);
}

void func_800F2560(void) {
    SPRT *a = &D_801050D8->unkB0[g_drawBufs.cur];
    SPRT *b = &D_801050D8->unkD8[g_drawBufs.cur];
    a->y0 = b->y0 = func_800F3EA4()->unk60 - 4;
    addPrim(&g_ot[18], a);
    addPrim(&g_ot[18], b);
    addPrim(&g_ot[18], &D_801050D8->unk100[g_drawBufs.cur]);
}

void func_800F2684(s32 arg0) {
    setlen(&D_801050D8->unkB0[arg0], 4);
    setcode(&D_801050D8->unkB0[arg0], 0x64);
    setRGB0(&D_801050D8->unkB0[arg0], 0x80, 0x80, 0x80);
    D_801050D8->unkB0[arg0].x0 = 0xD0;
    D_801050D8->unkB0[arg0].u0 = 0;
    D_801050D8->unkB0[arg0].v0 = 0xF8;
    D_801050D8->unkB0[arg0].clut = 0;
    D_801050D8->unkB0[arg0].w = 0x40;
    D_801050D8->unkB0[arg0].h = 4;
    setlen(&D_801050D8->unkD8[arg0], 4);
    setcode(&D_801050D8->unkD8[arg0], 0x64);
    setRGB0(&D_801050D8->unkD8[arg0], 0x80, 0x80, 0x80);
    D_801050D8->unkD8[arg0].x0 = 0x110;
    D_801050D8->unkD8[arg0].u0 = 0;
    D_801050D8->unkD8[arg0].v0 = 0xFC;
    D_801050D8->unkD8[arg0].clut = 0;
    D_801050D8->unkD8[arg0].w = 0x30;
    D_801050D8->unkD8[arg0].h = 4;
    setlen(&D_801050D8->unk100[arg0], 1);
    *D_801050D8->unk100[arg0].code = 0xE100071F;
}

void func_800F27CC(s32 arg0) {
    Unk80105078 *p = func_800F3EA4();
    Window *w;
    D_800F8EFC.y = p->unk60;
    D_800F8EFC.h = p->unk5C * 12 + 8;
    w = win_open(&D_800F8EFC, 0xA, 0, 1);
    g_btlUi->unk12A4[1] = w;
    win_setDrawContent(w, func_800F29B4);
    win_setDrawOverlay(g_btlUi->unk12A4[1], func_800F1BFC);
    win_setOpenFrames(g_btlUi->unk12A4[1], 7);
    func_800F3D3C(arg0);
    D_80105058.unk1 = arg0;
    D_80105058.unk0 = 1;
}

void func_800F2890(s32 arg0) {
    win_setCloseFrames(g_btlUi->unk12A4[1], 7);
    win_close(g_btlUi->unk12A4[1]);
    func_800F3DE0(arg0);
    D_80105058.unk0 = 0;
}

/* resizes the battle list window to the current entry count */
void func_800F28F0(void) {
    Unk80105078 *p = func_800F3EA4();
    Window *w;
    RECT *r;
    s16 h;

    if (D_80105058.unk0 != 0 && p->unk5C != 0) {
        w = g_btlUi->unk12A4[1];
        r = &w->unk8;
        r->y = w->unk2 = p->unk60;
        h = p->unk5C * 12 + 8;
        w->unk6 = h;
        if (w->unk1C == 2 || w->unk1C == 3) {
            r->h = h;
        } else if (w->unk1C == 1) {
            if (h < r->h) {
                r->h = h;
            }
        }
    }
}

u8 func_800F29A4(void) {
    return D_80105058.unk0;
}

void func_800F29B4(Window *arg0) {
    if (func_800F3EA4()->unk0 != 0) {
        D_800F8F04[D_80105059](arg0);
    }
}

void func_800F2A0C(void) {
    D_80105060.unk4 = g_gameState->unk1C->unk7E0;
    D_80105060.unk0 = D_80105060.unk4;
    D_80105060.unk10 = 0x8000;
    D_80105060.unk8 = D_80105060.unk10;
    D_80105060.unkC = 0;
}

void func_800F2A58(void) {
}

void *func_800F2A60(s32 arg0) {
    s32 size = (arg0 + 3) >> 2 << 2;
    s32 p = D_80105060.unk4;
    D_80105060.unk4 += size;
    D_80105060.unkC += size;
    D_80105060.unk10 -= size;
    return (void *)p;
}

void func_800F2AAC(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 n = 0;
    for (; u != NULL; u = u->unk0) {
        if (u->unk68 & 1) {
            D_80105078->unk68[n] = u;
            D_80105078->unk7C[n] = (btl_hasStatus(u, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) & 0xFF) != 0;
            D_80105078->unk80[n] = (u8)u->unk68 >> 5;
            if (btl_hasStatus(u, (BTL_STATUS_STOP | BTL_STATUS_SLOW)) & 0xFF) {
                D_80105078->unk84[n] = 2;
            } else if (btl_hasStatus(u, BTL_STATUS_HASTE) & 0xFF) {
                D_80105078->unk84[n] = 3;
            } else {
                D_80105078->unk84[n] = 0;
            }
            if ((u->unk68 >> 30) & 1) {
                D_80105078->unk88[n] = (btl_hasStatus(u, BTL_STATUS_TRANCE) & 0xFF) ? 2 : 1;
            } else {
                D_80105078->unk88[n] = 0;
            }
            n++;
        }
    }
    D_80105078->unk64 = g_btlUi->cmdUnit;
    D_80105078->unk5C = n;
    D_80105078->unk60 = D_800F9028[n - 1];
    D_80105078->unk1 = 0;
    D_80105078->unk2 = 0xFF;
}

void func_800F2C74(s32 arg0) {
    setPolyG4(&D_801050D8->unk310[arg0]);
    setSemiTrans(&D_801050D8->unk310[arg0], 1);
    setRGB0(&D_801050D8->unk310[arg0], 0x40, 0x40, 0);
    setRGB1(&D_801050D8->unk310[arg0], 0x10, 0x10, 0);
    setRGB2(&D_801050D8->unk310[arg0], 0x40, 0x40, 0);
    setRGB3(&D_801050D8->unk310[arg0], 0x10, 0x10, 0);
    D_801050D8->unk310[arg0].x0 = 0x8E;
    D_801050D8->unk310[arg0].x1 = 0x137;
    D_801050D8->unk310[arg0].x3 = 0x137;
    D_801050D8->unk310[arg0].x2 = 0x8E;
    setlen(&D_801050D8->unk358[arg0], 1);
    *D_801050D8->unk358[arg0].code = 0xE1000620;
}

void func_800F2DC0(s32 arg0) {
    s32 i;
    SPRT *s;
    for (i = 0; i < 4; i++) {
        setlen(&D_801050D8->unk110[i][arg0], 4);
        setcode(&D_801050D8->unk110[i][arg0], 0x64);
        setRGB0(&D_801050D8->unk110[i][arg0], 0x80, 0x80, 0x80);
        D_801050D8->unk110[i][arg0].x0 = 0x92;
        D_801050D8->unk110[i][arg0].h = 0xC;
        D_801050D8->unk110[i][arg0].w = 0x54;
        if (i & 1) {
            D_801050D8->unk110[i][arg0].u0 = 0x54;
        } else {
            D_801050D8->unk110[i][arg0].u0 = 0;
        }
        D_801050D8->unk110[i][arg0].v0 = 0xF0;
    }
    setlen(&D_801050D8->unk2F0[arg0], 1);
    *D_801050D8->unk2F0[arg0].code = 0xE100061E;
}

void func_800F2ED0(s32 arg0) {
    s32 i;
    SPRT *s;
    for (i = 0; i < 4; i++) {
        setlen(&D_801050D8->unk1B0[i][arg0], 4);
        setcode(&D_801050D8->unk1B0[i][arg0], 0x64);
        setRGB0(&D_801050D8->unk1B0[i][arg0], 0x80, 0x80, 0x80);
        D_801050D8->unk1B0[i][arg0].x0 = 0xD0;
        D_801050D8->unk1B0[i][arg0].u0 = 0;
        D_801050D8->unk1B0[i][arg0].w = 0x1D;
        D_801050D8->unk1B0[i][arg0].h = 0xA;
        D_801050D8->unk1B0[i][arg0].clut = 0;
        setlen(&D_801050D8->unk250[i][arg0], 4);
        setcode(&D_801050D8->unk250[i][arg0], 0x64);
        setRGB0(&D_801050D8->unk250[i][arg0], 0x80, 0x80, 0x80);
        D_801050D8->unk250[i][arg0].x0 = 0xEF;
        D_801050D8->unk250[i][arg0].u0 = 0x20;
        D_801050D8->unk250[i][arg0].w = 0x16;
        D_801050D8->unk250[i][arg0].h = 0xA;
        D_801050D8->unk250[i][arg0].clut = 0;
    }
    setlen(&D_801050D8->unk300[arg0], 1);
    *D_801050D8->unk300[arg0].code = 0xE100071F;
}

void btlui_drawPartyPanel(void) {
    u32 i;
    s32 j;
    if (D_80105078->unk0 != 0 && D_80105078->unk5C != 0) {
        btlui_drawPanelHighlight();
        btlui_drawPanelHeader();
        for (i = 0; i < D_80105078->unk5C; i++) {
            for (j = 0; j < 4; j++) {
                btlui_updatePanelRowColor(i);
                if (D_80105078->unk4[j].unk4 != 0 && D_800F9038[j] != 0) {
                    D_800F9038[j](D_80105078->unk68[i], i);
                }
            }
        }
    }
}

void func_800F316C(void) {
    BtlUnit *u;
    Unk80104F88Entry *e;
    Unk80104F88Gauge *g;
    Unk80104F88Gauge *mp;
    s32 i;
    s32 cur;
    s32 max;
    s32 d;
    s32 a;
    s32 t;

    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (!(u->unk68 & 1)) {
            continue;
        }
        e = u->unkB4;
        mp = &e->unk0[1];
        for (i = 0; i < 2; i++) {
            g = mp;
            if (i == 0) {
                g = &e->unk0[0];
            }
            if (g->unk6 != 0) {
                if (g->unk0 >= 0) {
                    if (g->unk2 + g->unk0 >= g->unk4) {
                        g->unk2 = g->unk4;
                        g->unk6 = 0;
                    } else {
                        g->unk2 += g->unk0;
                        g->unk6--;
                    }
                } else {
                    if (g->unk2 + g->unk0 <= g->unk4) {
                        g->unk2 = g->unk4;
                        g->unk6 = 0;
                    } else {
                        g->unk2 += g->unk0;
                        g->unk6--;
                    }
                }
            } else {
                cur = (i == 0) ? u->hp : u->mp;
                max = (i == 0) ? u->max->maxHp : u->max->maxMp;
                d = cur - g->unk2;
                if (d != 0) {
                    a = (d >= 0) ? d : -d;
                    g->unk4 = cur;
                    if (a < 0x10) {
                        g->unk6 = a;
                    } else {
                        g->unk6 = (a << 6) / max;
                        if (g->unk6 < 0x10) {
                            a = 0x10;
                            g->unk6 = a;
                        }
                    }
                    if (d >= 0) {
                        t = d - 1;
                        g->unk0 = (t + g->unk6) / g->unk6;
                    } else {
                        t = d + 1;
                        g->unk0 = (t - g->unk6) / g->unk6;
                    }
                }
            }
        }
    }
}

void btlui_drawPanelHeader(void) {
    RECT r;
    s32 i;
    Unk800F412C *e;

    ui_drawRule(0x8E, D_80105078->unk60, 8, 1, 0xA9, (g_gameState->config >> 18) & 1);
    r.x = 0x8E;
    r.y = D_80105078->unk60 - 3;
    r.w = g_gameState->unkC->disp.w - 0x8E;
    r.h = 7;
    win_setClip(NULL, 7, 4);
    e = D_80105078->unk4;
    for (i = 0; i < 5; e++, i++) {
        if (e->unk4 != 0) {
            ui_drawLabel((s16)e->unk8, D_80105078->unk60 + (s16)e->unkA, 7,
                          ((s32 *)&D_800703FC)[e->unk0], 0);
        }
    }
    win_setClip(&r, 7, 4);
}

void btlui_drawPanelHighlight(void) {
    u8 c = D_80105078->unk2;
    POLY_G4 *p;
    s32 y;
    s32 d = 4;
    if (c != 0xFF && D_80104E77 != 0) {
        y = D_80105078->unk60 + (c * 12 + d);
        p = &D_801050D8->unk310[g_drawBufs.cur];
        p->y0 = p->y1 = y;
        p->y2 = p->y3 = y + 13;
        addPrim(&g_ot[20], p);
        addPrim(&g_ot[20], &D_801050D8->unk358[g_drawBufs.cur]);
    }
}

void btlui_drawPanelName(s32 arg0, u32 arg1) {
    s32 k = D_80105078->unk80[arg1];
    SPRT *s = &D_801050D8->unk110[k][g_drawBufs.cur];
    s->y0 = D_80105078->unk60 + arg1 * 12 + 3;
    s->clut = D_800F9048[(u8)D_80105078->unk78[arg1]] + ((k >> 1) << 6);
    addPrim(&g_ot[16], s);
    if (arg1 >= D_80105078->unk5C - 1) {
        addPrim(&g_ot[16], &D_801050D8->unk2F0[g_drawBufs.cur]);
    }
}

void btlui_drawPanelGauges(BtlUnit *arg0, s32 arg1) {
    s32 m = D_80105078->unk88[arg1];
    u8 f = 0;
    u32 y = D_80105078->unk60 + arg1 * 12;
    if (arg0->unkB3 == 0) {
        f = D_80105078->unk7C[arg1] == 0;
    }
    if (m == 2) {
        ui_drawGauge(0x10C, y + 10, 14, arg0->unk78, 0xFF, 1, f);
        ui_drawGauge(0x108, y + 6, 14, arg0->atb, arg0->max->maxAtb, D_80105078->unk84[arg1], f);
    } else if (m == 1) {
        ui_drawGauge(0x108, y + 6, 14, arg0->atb, arg0->max->maxAtb, D_80105078->unk84[arg1], f);
        ui_drawGauge(0x10C, y + 10, 14, arg0->unk78, 0xFF, m, f);
    } else {
        ui_drawGauge(0x108, y + 8, 14, arg0->atb, arg0->max->maxAtb, D_80105078->unk84[arg1], f);
    }
}

void func_800F386C(void) {
    RECT a = D_800A858C;
    RECT b = D_800A8594;
    s16 h;
    if (D_80105078->unk0 != 0 && D_80105078->unk5C != 0) {
        a.x += D_801051E8[g_gameState->unk1C->unk8 & 1][0];
        b.x += D_801051E8[g_gameState->unk1C->unk8 & 1][0];
        a.y += D_801051E8[g_gameState->unk1C->unk8 & 1][1] + D_80105078->unk60;
        b.y += D_801051E8[g_gameState->unk1C->unk8 & 1][1] + D_80105078->unk60;
        h = D_80105078->unk5C * 12;
        b.h = h;
        a.h = h;
        SetDrawMove((DR_MOVE *)g_primPtr, &a, 0x3C0, 0x1B0);
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
        SetDrawMove((DR_MOVE *)g_primPtr, &b, 0x3E0, 0x1B0);
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
    }
}

void func_800F3A8C(void) {
    s32 y = D_80105078->unk60 + 6;
    u8 v = 0xB0;
    u32 i;
    SPRT *a;
    SPRT *b;
    if (D_80105078->unk0 != 0 && D_80105078->unk5C != 0) {
        for (i = 0; i < D_80105078->unk5C; i++) {
            b = &D_801050D8->unk1B0[i][g_drawBufs.cur];
            a = &D_801050D8->unk250[i][g_drawBufs.cur];
            a->y0 = y;
            b->y0 = y;
            y += 12;
            a->v0 = v;
            b->v0 = v;
            v += 12;
            addPrim(&g_ot[18], b);
            addPrim(&g_ot[18], a);
        }
        addPrim(&g_ot[18], &D_801050D8->unk300[g_drawBufs.cur]);
    }
}

void func_800F3C08(void) {
    s32 i;
    D_80105078 = func_800F2A60(0x8C);
    func_800F2AAC();
    for (i = 0; i < 5; i++) {
        D_80105078->unk4[i] = D_800F8FD8[i];
        func_800F40D0(&D_80105078->unk4[i]);
    }
    for (i = 0; i < D_80105078->unk5C; i++) {
        func_800F4010(i);
    }
    D_80105078->unk54 = 0;
    D_80105078->unk0 = 1;
    D_80105078->unk64 = 0;
}

void func_800F3D0C(void) {
    func_800F3F10();
    func_800F316C();
    func_800F3FDC();
}

void func_800F3D3C(s32 arg0) {
    func_800F412C(&D_80105078->unk4[1], &D_800F9058);
    func_800F412C(&D_80105078->unk4[2], &D_800F9058);
    func_800F412C(&D_80105078->unk4[3], &D_800F9058);
    func_800F412C(&D_80105078->unk4[4], &D_800F9064);
    D_80105078->unk4[4].unk0 = D_800F9070[arg0].unk0;
    btlui_setMenuState(4);
}

void func_800F3DE0(s32 arg0) {
    D_80105078->unk58 = arg0;
    func_800F412C(&D_80105078->unk4[1], &D_800F9084);
    func_800F412C(&D_80105078->unk4[2], &D_800F9084);
    func_800F412C(&D_80105078->unk4[3], &D_800F9084);
    func_800F412C(&D_80105078->unk4[4], &D_800F9090);
    btlui_setMenuState(4);
}

void func_800F3E60(void) {
    func_800F2AAC();
    if (D_80105078->unk64 != 0) {
        func_800F42F0();
    }
}

Unk80105078 *func_800F3EA4(void) {
    return D_80105078;
}

u8 func_800F3EB4(void) {
    return D_80105078->unk0;
}

void func_800F3ECC(u8 arg0) {
    D_80105078->unk0 = arg0;
}

void func_800F3EDC(s32 arg0) {
    func_800F2C74(arg0);
    func_800F2DC0(arg0);
    func_800F2ED0(arg0);
}

void func_800F3F10(void) {
    s32 i;
    Unk800F412C *e = D_80105078->unk4;
    for (i = 0; i < 5; i++) {
        if (e->unkC != 0) {
            e->unk8 += e->unkC->unk0;
            e->unkA += e->unkC->unk2;
            if (++e->unk6 >= e->unkC->unk4) {
                e->unk6 = 0;
                if (e->unkC->unk8 != 0) {
                    e->unkC->unk8(e);
                }
                e->unkC = 0;
            }
        }
        e++;
    }
}

void func_800F3FDC(void) {
    Unk80105078 *p = D_80105078;
    u32 v = D_80104E80 & 0x3F;
    p->unk54 = v;
    if (v >= 0x20) {
        p->unk54 = 0x3F - v;
    }
}

void func_800F4010(s32 arg0) {
    BtlUnit *u = D_80105078->unk68[arg0];
    Unk80104F88Entry *e = u->unkB4;
    Unk80104F88Gauge *g;
    s16 v;
    e->unk0[0].unk4 = e->unk0[0].unk2 = u->hp;
    v = u->mp;
    g = &e->unk0[1];
    g->unk6 = 0;
    g->unk4 = g->unk2 = v;
    e->unk0[0].unk6 = 0;
    e->unk0[1].unk0 = 0;
    e->unk0[0].unk0 = 0;
}

void btlui_updatePanelRowColor(s32 arg0) {
    BtlUnit *u = D_80105078->unk68[arg0];
    s32 hp = u->hp;
    if (hp == 0) {
        D_80105078->unk78[arg0] = 3;
        return;
    }
    D_80105078->unk78[arg0] = ((s32)(u16)(u->max->maxHp / 6) >= hp) << 2;
}

void func_800F40D0(Unk800F412C *arg0) {
    if (arg0->unk2 == 1) {
        arg0->unk8 -= func_8002AE7C(((u8 **)&D_800703FC)[arg0->unk0]);
    }
}

void func_800F412C(Unk800F412C *arg0, Unk800F3F10 *arg1) {
    arg0->unkC = arg1;
    arg0->unk4 = 1;
    arg0->unk6 = 0;
}

void func_800F4140(void) {
    D_80105078->unk4[1].unk4 = 0;
    D_80105078->unk4[2].unk4 = 0;
    D_80105078->unk4[3].unk4 = 0;
    btlui_setMenuState(3);
}

void func_800F4180(void) {
    D_80105078->unk4[4].unk4 = 0;
    btlui_setMenuState(D_80105078->unk58);
}

void btlui_drawPanelHp(BtlUnit *arg0, s32 arg1) {
    u8 buf[8];
    s32 dy = 6;
    text_formatNumber(arg0->unkB4->unk0[0].unk2, buf, 4, 1);
    ui_drawDigits(0xD0, D_80105078->unk60 + (arg1 * 12 + dy), 0xE, buf, (u8)D_80105078->unk78[arg1]);
}

void btlui_drawPanelMp(BtlUnit *arg0, s32 arg1) {
    u8 buf[8];
    s32 mp;
    s32 c;
    s32 dy = 6;
    mp = arg0->unkB4->unk0[1].unk2 < 0 ? 0 : arg0->unkB4->unk0[1].unk2;
    c = ((s16)(arg0->max->maxMp / 6) >= mp) << 2;
    text_formatNumber(mp, buf, 3, 1);
    ui_drawDigits(0xEF, D_80105078->unk60 + (arg1 * 12 + dy), 0xE, buf, c);
}

void func_800F42F0(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 i = 0;
    if (D_80105078->unk5C != 0 && D_80104E70 == 0) {
        for (; u != 0; u = u->unk0) {
            if (u->unk68 & 1) {
                if (D_80105078->unk64 == u) {
                    D_80105078->unk2 = i;
                    return;
                }
                i++;
            }
        }
    }
}

void func_800F4384(void) {
    GameData *g = g_gameState->unk1C;
    Unk80068EE8 *s;
    s32 f;

    if (!(g->unk0 & 0x100) && !(g->unk8E4.battle->unk0 & 0x20) && D_80104E68.unk0 != 0 &&
        (f = (g_gameState->config >> 5) & 1) == 1) {
        ClearOTag(&D_801050D4, 1);
        if (D_80104E68.unk18 & 0x20) {
            if (D_80104E68.unk15 == 0) {
                s = &D_80068ED8;
            } else {
                s = &D_80068EE0;
            }
            setlen(&D_801050C0, 4);
            D_801050C0.rgbc = 0x64808080;
            D_801050C0.xy = 0x6012B;
            D_801050C0.uvclut = s->unk0;
            D_801050C0.wh = s->unk4 | (s->unk5 << 16);
            addPrim(&D_801050D4, &D_801050C0);
        }
        if (D_80104E7D == 0) {
            s = &D_80068E90;
        } else {
            s = &D_80068ED0;
        }
        setlen(&D_801050A0, 4);
        D_801050A0.rgbc = 0x64808080;
        D_801050A0.xy = 0x60124;
        D_801050A0.uvclut = s->unk0;
        D_801050A0.wh = s->unk4 | (s->unk5 << 16);
        addPrim(&D_801050D4, &D_801050A0);
        setlen(&D_80105080, 1);
        D_80105080.code[0] = 0xE1000600 | (s->unk6 & 0x9FF);
        addPrim(&D_801050D4, &D_80105080);
        setlen(&D_80105090, 2);
        D_80105090.code[0] = 0xE6000001;
        D_80105090.code[1] = 0;
        addPrim(&D_801050D4, &D_80105090);
        DrawOTag(&D_801050D4);
        DrawSync(0);
    }
}

void func_800F4630(void) {
    s32 i;
    s32 j;
    u8 *row;
    pad_setStickAsDpad(1);
    i = 0;
    row = D_80104E94[0];
    for (; i < 2; i++, row += 0x18) {
        for (j = 0; j < 0x17; j++) {
            row[j] = pad_isRepeatEnabled(i, j);
        }
        pad_setRepeatEnabled(i, 2, 0);
        pad_setRepeatEnabled(i, 3, 0);
        pad_setRepeatTiming(i, 0xE, 0xF, 0xA);
        pad_setRepeatEnabled(i, 0xE, 1);
        pad_clearRepeats(i);
    }
    D_80104E76 = func_800F47D8();
}

void func_800F4700(void) {
    s32 i, j;
    u8 *p;
    for (i = 0, p = D_80104E94[0]; i < 2; i++) {
        for (j = 0; j < 0x17; j++) {
            pad_setRepeatEnabled(i, j, p[j]);
        }
        p += 0x18;
    }
    pad_setStickAsDpad(0);
}

void btlui_readPad(void) {
    pad_update();
    pad_updateRepeat(D_80104E68.unk15);
    if ((g_padRepeat & 8) && D_80104E68.unkB == 0) {
        D_80104E68.unkB = 1;
    }
}

u8 func_800F47D8(void) {
    s32 i;
    s32 f = (g_gameState->config >> 5) & 1;
    if (f != 1) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        Character *c = g_gameState->unk1C->party[i];
        if (c == 0) {
            continue;
        }
        if (((s32)(g_gameState->config >> 20) >> chr_getDefaultSet(c)) & 1) {
            return 1;
        }
    }
    return 0;
}

void func_800F4884(void) {
    s32 i;
    D_801050D8 = func_800F2A60(0x368);
    for (i = 0; i < 2; i++) {
        func_800ECCA8(i);
        func_800F2684(i);
        func_800F3EDC(i);
    }
}

void func_800F48DC(void) {
}

void func_800F48E4(void) {
    u32 *a = func_800F2A60(0xC0);
    s32 b = (s32)func_800F2A60(0x4000);
    DRAWENV *e;
    gfx_init(a, a + 0x18, b, b + 0x2000);
    func_8002C064(2);
    gfx_setBufferIndex(g_gameState->unk1C->unk8);
    D_801051E0 = 0;
    g_ot = g_drawBufs.ots[0];
    g_primPtr = g_drawBufs.prims[0];
    e = g_gameState->unk10;
    e[1].dfe = 1;
    e[0].dfe = 1;
    D_801051E8[0][0] = g_gameState->unkC[0].disp.x;
    D_801051E8[0][1] = g_gameState->unkC[0].disp.y;
    D_801051E8[1][0] = g_gameState->unkC[1].disp.x;
    D_801051E8[1][1] = g_gameState->unkC[1].disp.y;
    ClearOTagR(g_drawBufs.ots[0], 0x18);
    ClearOTagR(g_drawBufs.ots[1], 0x18);
    DrawSync(0);
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", D_800A8584);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", D_800A858C);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", D_800A8594);
