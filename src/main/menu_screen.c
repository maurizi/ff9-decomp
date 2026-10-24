#include "ff9.h"

void menuScreen_init(void) {
    Unk80077E44 *t;

    printf("path=%s\n", "f:/code/dms/loc_us/ff9db.h");
    g_menuScreen.unk2 = 0;
    g_menuScreen.unk0 = 1;
    g_menuScreen.unk1 = 1;
    g_menuScreen.unk4 = 0;
    g_menuScreen.unk8 = fade_getOtz();
    g_menuScreen.unk3 = party_getFirstMember();
    D_80077E28.unk14 = D_8006E508;
    D_80077E28.unk8 = 0;
    D_80077E28.unk0 = 0;
    D_80077E28.unk10 = 0xFF;
    D_80077E28.unk1 = 0;
    D_80077E28.unk4 = 0;
    D_80077E28.unk6 = 0;
    D_80077E28.unk5 = 0;
    D_80077E28.unk1C = g_gameState->mode != 3 ? g_menuScreensField : g_menuScreensWorld;
    t = D_80077E44;
    t[D_80077E52].unk0();
}

void menuScreen_drawCommands(void) {
    u8 all = 0;
    RECT *r = &D_80077E28.unk14;
    s32 sel;
    s32 i;
    s32 y;

    if (D_80077E52 == 0 || (D_80077E28.unk0 != 0 && (*(u32 *)&D_80077E28 & 0xFFFF00))) {
        all = 1;
    }
    win_setClip(NULL, 2, 2);
    if (D_80077E28.unk7 != 0) {
        sel = D_80077E28.unk7 - 1;
    } else {
        sel = D_80077E52 - 1;
    }
    for (i = 0; i < 7; i++) {
        y = i * 15 + 6;
        if (all || i == sel) {
            text_drawBlock(r->x + 10, r->y + y + D_80077E28.unk8, 2,
                          (u8 *)text_getFrom(D_800110A0, i), 0);
        }
    }
    win_setClip(r, 2, 2);
    win_drawBox(r, 2, 2);
    win_drawShadow(r, 2, 2);
}

void menuScreen_updateOpen(void) {
    if (D_80077E28.unk0 != 0) {
        if (D_80077E28.unk3 != 0) {
            menuScreen_requestOverlay();
        }
        if (D_80077E28.unk1 != 0) {
            menuScreen_stepTitleResize();
        }
        if (D_80077E28.unk2 != 0) {
            menuScreen_stepTitleScroll();
        }
        if (D_80077E28.unk6 == 0 && D_80077E28.unk3 == 0 && *(u16 *)&D_80077E28.unk4 == 0 &&
            !(*(u32 *)&D_80077E28 & 0xFFFF00)) {
            D_80077E28.unk0 = 0;
            g_menuScreen.unk4 = 0;
            g_menuScreen.unk2 = D_80077E28.unk7;
            fade_inDefault();
            DrawSync(0);
            EnterCriticalSection();
            FlushCache();
            ExitCriticalSection();
            D_80077E28.unk1C[g_menuScreen.unk2].unk0();
        }
    }
}

void menuScreen_stepTitleScroll(void) {
    D_80077E28.unk8 += D_80077E28.unkC;
    if (D_80077E28.unkC >= 0) {
        if (D_80077E28.unk8 >= D_80077E28.unkA) {
            D_80077E28.unk8 = D_80077E28.unkA;
            D_80077E28.unk2 = 0;
            D_80077E28.unkE = 0;
        }
    } else if (D_80077E28.unkA >= D_80077E28.unk8) {
        D_80077E28.unk8 = D_80077E28.unkA;
        D_80077E28.unk2 = 0;
        D_80077E28.unkE = 0;
    }
    D_80077E28.unkE++;
}

void menuScreen_update(void) {
    void (*fn)(void);
    Unk80077E44 *t;
    menuScreen_drawCommands();
    menuScreen_updateOpen();
    t = D_80077E44;
    fn = t[g_menuScreen.unk2].unk4;
    if (fn != NULL) {
        fn();
    }
    g_menuScreen.unk4++;
}

void menuScreen_exit(void) {
    help_setCallback(0);
    g_menuScreen.unk0 = 0;
}

void menuScreen_open(s32 arg0) {
    s32 prev = D_80077E52;

    if (D_80077E28.unk1C[arg0].unk4 != 0) {
        D_80077E28.unk7 = arg0;
        D_80077E28.unk0 = 1;
        D_80077E28.unk1 = 1;
        D_80077E28.unk5 = 1;
        D_80077E28.unk3 = 0;
        D_80077E28.unk4 = 0;
        D_80077E28.unk6 = 0;
        menuScreen_startTitleAnim();
        menuScreen_requestOverlay();
        D_80077E28.unk20 = D_80077E28.unk1C[prev].unk8;
        fade_outDefault();
        fade_setOtz(3);
        fade_setOnDone(menuScreen_onOpenFaded);
    }
}

void menuScreen_switch(u8 a) {
    D_80077E28.unk0 = 1;
    D_80077E28.unk6 = 1;
    D_80077E28.unk7 = a;
    menuScreen_startTitleAnim();
    fade_out(0xC);
    fade_setOnDone(menuScreen_onSwitchFaded);
}

u8 menuScreen_getCurrent(void) {
    return D_80077E52;
}

void menuScreen_close(void) {
    help_setCallback(NULL);
    fade_setOtz(D_80077E58);
    func_800325E0();
}

u8 menuScreen_needsMember(s32 i) {
    return D_80077E44[i].unkC;
}

s32 menuScreen_getCommandHelp(s32 a) {
    return text_getFrom(D_800110A0, a + 9);
}

void menuScreen_stepTitleResize(void) {
    if (D_80077E28.unk7 == 0) {
        D_80077E28.unk14.h += 8;
        if (D_80077E28.unk14.h >= 0x74) {
            D_80077E28.unk14.h = 0x74;
            D_80077E28.unk1 = 0;
        }
    } else {
        D_80077E28.unk14.h -= 8;
        if (D_80077E28.unk14.h < 0x1B) {
            D_80077E28.unk14.h = 0x1A;
            D_80077E28.unk1 = 0;
        }
    }
}

void menuScreen_startTitleAnim(void) {
    s32 v;
    s32 d;
    s32 q;
    s32 t = D_80077E2F;
    if (t != 0) {
        v = (1 - t) * 15;
    } else {
        v = 0;
    }
    d = v - D_80077E30;
    if (d >= 0) {
        q = (d + 11) / 12;
    } else {
        q = (d - 11) / 12;
    }
    D_80077E28.unkC = q;
    D_80077E28.unkA = v;
    D_80077E28.unkE = 0;
    D_80077E28.unk2 = 1;
}

void menuScreen_requestOverlay(void) {
    if (D_80077E2F != 0) {
        menuScreen_loadOverlay();
    }
}

void menuScreen_loadOverlay(void) {
    s32 id = g_menuScreenOvl[D_80077E28.unk7];
    FileEntry *f;

    if (id != 0xFF && id != D_80077E28.unk10) {
        D_80077E28.unk3 = 1;
        D_80077E28.unk10 = id;
        f = file_find(0, id);
        if (cd_readAsync(g_gameState->unk20 + f->unk4, (f->unkC - f->unk4) << 11, (void *)0x801EDF00, menuScreen_onOverlayRead)) {
            D_80077E28.unk4 = 1;
            D_80077E28.unk3 = 0;
        }
    }
}

void menuScreen_onOpenFaded(void) {
    D_80077E28.unk5 = 0;
    if (D_80077E28.unk20 != NULL) {
        D_80077E28.unk20();
    }
}

void menuScreen_onSwitchFaded(void) {
    D_80077E28.unk1C[g_menuScreen.unk2].unk8();
    D_80077E28.unk0 = 0;
    g_menuScreen.unk2 = D_80077E28.unk7;
    D_80077E28.unk1C[g_menuScreen.unk2].unk0();
    if (g_gameState->mode != 3) {
        func_800C9510(g_menuScreen.unk2 - 1);
    } else {
        func_800CC820(g_menuScreen.unk2 - 1);
    }
    fade_in(12);
    fade_setOnDone(NULL);
}

void menuScreen_onOverlayRead(u8 *a) {
    if (*a == 0) {
        D_80077E2C = 0;
    }
}

INCLUDE_RODATA("asm/us/main/nonmatchings/menu_screen", D_800110A0);
