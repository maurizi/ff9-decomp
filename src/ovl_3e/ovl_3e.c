#include "ovl_3e.h"

void menu_disc_init(void) {
    s32 *src;
    DiscSwap *st;
    s32 i;
    u8 r;
    u8 state;

    src = D_80073998;
    st = &g_menuDisc;
    st->unk2 = 0;
    st->timer = 0;
    g_menu.unk9 = 0;
    st->disc = DISC_REQ_DISC(g_menu.unkC);
    st->nextLocation = DISC_REQ_LOCATION(g_menu.unkC);
    for (i = 0; i < 2; i++) {
        st->savedFlags[i] = src[i];
        src[i] |= 0x300;
    }
    func_8003556C(0);
    menu_disc_loadImages();
    cd_stopMotor();
    g_menuDisc.shellOpen = r = cd_checkShellOpen();
    state = DISC_ST_LID_CLOSED;
    if (r) {
        state = DISC_ST_LID_OPEN;
    }
    g_menuDisc.state = state;
}

void menu_disc_drawPicture(void) {
    SprtWords *p;
    s32 i;
    RECT unused[2];

    for (i = 0; i < 2; i++) {
        setlen(g_primPtr, 4);
        p = (SprtWords *)g_primPtr;
        p->rgbc = 0x64808080;
        p->xy = i * DISC_PICTURE_W;
        p->uvclut = (i * DISC_PICTURE_W) & 0x3F;
        p->wh = 0xE000A0;
        g_primPtr = (u8 *)(p + 1);
        addPrim(&g_ot[6], p);
        SetDrawTPage(g_primPtr, 1, 1, (((i * DISC_PICTURE_W) >> 6) + 0x10) | 0x100);
        addPrim(&g_ot[6], g_primPtr);
        g_primPtr += 8;
    }
}

void menu_disc_drawError(void) {
    SprtWords *p;
    u32 clut = GetClut(0, 0x1FF) << 16;

    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = 0x64808080;
    p->xy = 0x1C004E;
    p->uvclut = clut;
    p->wh = 0xA800C2;
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[6], p);
    SetDrawTPage(g_primPtr, 1, 1, 0x95);
    addPrim(&g_ot[6], g_primPtr);
    g_primPtr += 8;
}

u8 menu_disc_check(void) {
    CdlLOC loc;
    u8 result[8];
    s32 mask;
    s32 i;
    s32 ret;
    s32 type;

    mask = 1 << g_menuDisc.disc;
    CdControlB(CdlNop, NULL, result);
    i = 0;
    if (result[0] & CdlStatShellOpen) {
        return DISC_CHECK_SHELL_OPEN;
    }
    CdControlB(CdlStandby, NULL, NULL);
    do {
        VSync(0);
        if (i >= 600) {
            return DISC_CHECK_NO_DISC;
        }
        CdControlB(CdlNop, NULL, result);
        i++;
    } while (!(result[0] & CdlStatStandby));

    ret = CdDiskReady(0);
    switch (ret) {
    case CdlDiskError:
        return DISC_CHECK_OTHER_FORMAT;
    case CdlComplete:
        break;
    case CdlStatShellOpen:
        return DISC_CHECK_SHELL_OPEN;
    default:
        return DISC_CHECK_ERROR;
    }
    type = CdGetDiskType();
    switch (type) {
    case CdlOtherFormat:
        return DISC_CHECK_OTHER_FORMAT;
    case CdlStatNoDisk:
        return DISC_CHECK_NO_DISC;
    case CdlCdromFormat:
        break;
    case CdlStatShellOpen:
        return DISC_CHECK_SHELL_OPEN;
    default:
        return DISC_CHECK_ERROR;
    }
    CdIntToPos(5, &loc);
    CdControlB(CdlSeekL, (u8 *)&loc, result);
    if (result[0] & CdlStatError) {
        return DISC_CHECK_ERROR;
    }
    if (result[1] & 0x40) {
        return DISC_CHECK_ERROR;
    }
    CdControlB(CdlSetmode, (u8 *)CdlModeSpeed, result);
    VSync(3);
    for (i = 0; i < 10; i++) {
        func_8001DCB8();
        if (g_gameState->unkB != 0xFF) {
            break;
        }
    }
    if (g_gameState->unkB == 0xFF || !(g_gameState->unkB & mask)) {
        cd_stopMotor();
        return DISC_CHECK_WRONG_DISC;
    }
    snd_playSe(0x4ED);
    return DISC_CHECK_OK;
}

void menu_disc_update(void) {
    menu_disc_input();
    menu_disc_draw();
    menu_disc_poll();
}

void menu_disc_exit(void) {
    s32 *dst;
    DiscSwap *st;
    s32 i;

    if (g_menuDisc.nextLocation != DISC_NO_LOCATION) {
        evt_setNextLocation(g_menuDisc.nextLocation);
    }
    dst = D_80073998;
    st = &g_menuDisc;
    g_gameState->unk0 |= 0x100000;
    for (i = 0; i < 2; i++) {
        dst[i] = st->savedFlags[i];
    }
}

void menu_disc_input(void) {
    void (*f)(void);

    if (!func_8002F810()) {
        f = g_menuDiscInputFuncs[g_menuDisc.state];
        if (f != NULL) {
            f();
        }
    }
}

void menu_disc_draw(void) {
    void (*f)(void) = g_menuDiscDrawFuncs[g_menuDisc.state];

    if (f != NULL) {
        f();
    }
}

void menu_disc_poll(void) {
    void (*f)(void) = g_menuDiscPollFuncs[g_menuDisc.state];

    if (f != NULL) {
        f();
    }
}

void menu_disc_loadImages(void) {
    u32 *buf = (u32 *)0x801CDF00;
    s32 size;
    u32 *clut = g_menuDiscClut;
    s32 i;
    s32 sector;

    sector = g_gameState->unk20 + file_find(1, g_menuDiscPictureFiles[g_menuDisc.disc & 3])->unk4;
    size = DISC_PICTURE_HALF_BYTES;
    for (i = 0; i < 2; i++) {
        cd_readSync(sector, size, buf);
        menu_disc_loadPictureHalf(buf, i);
        sector += DISC_PICTURE_HALF_SECTORS;
    }
    LoadImage(&g_menuDiscClutRect, clut);
    DrawSync(0);
    LoadImage(&g_menuDiscErrorRect, clut + 0x80);
    DrawSync(0);
}

void menu_disc_loadPictureHalf(u32 *p, s32 i) {
    RECT r = g_menuDiscPictureRect;

    r.x += i * DISC_PICTURE_W;
    LoadImage(&r, p);
    DrawSync(0);
}

void menu_disc_inputLidClosed(void) {
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        menu_disc_startCheck();
    }
}

void menu_disc_inputLidOpen(void) {
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        menu_disc_startCheck();
    }
}

void menu_disc_inputError(void) {
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        g_menuDisc.state = DISC_ST_ERROR_CLOSE;
        func_8002FAAC(0x18);
        func_8002F8A8(menu_disc_closeError);
    }
}

void menu_disc_pollLidClosed(void) {
    u8 r;

    g_menuDisc.shellOpen = r = cd_checkShellOpen();
    if (r) {
        g_menuDisc.state = DISC_ST_LID_OPEN;
    }
}

void menu_disc_pollLidOpen(void) {
    u8 r;

    g_menuDisc.shellOpen = r = cd_checkShellOpen();
    if (!r) {
        snd_playSe(SE_CURSOR);
        menu_disc_startCheck();
    }
}

void menu_disc_pollError(void) {
    if (++g_menuDisc.timer >= DISC_ERROR_FRAMES) {
        g_menuDisc.state = DISC_ST_ERROR_CLOSE;
        func_8002FAAC(0x18);
        func_8002F8A8(menu_disc_closeError);
    }
}

void menu_disc_startCheck(void) {
    g_menuDisc.unk2 = 0;
    g_menuDisc.state = DISC_ST_BUSY;
    func_8002FAAC(0x18);
    func_8002F8A8(menu_disc_doCheck);
}

void menu_disc_doCheck(void) {
    u8 r;

    g_menuDisc.checkResult = r = menu_disc_check();
    g_menuDisc.timer = 0;
    switch (r) {
    case DISC_CHECK_SHELL_OPEN:
    default:
        g_menuDisc.state = DISC_ST_ERROR;
        func_8002FA50(0x18);
        break;
    case DISC_CHECK_OK:
        func_80032640(0);
        func_80032634(0);
        func_800325E0();
        break;
    }
}

void menu_disc_closeError(void) {
    g_menuDisc.state = DISC_ST_BUSY;
    func_8002FA50(0x18);
    func_8002F8A8(menu_disc_resetState);
}

void menu_disc_resetState(void) {
    g_menuDisc.state = cd_checkShellOpen() ? DISC_ST_LID_OPEN : DISC_ST_LID_CLOSED;
}

INCLUDE_RODATA("asm/us/ovl_3e/nonmatchings/ovl_3e", g_menuDiscPictureRect);

INCLUDE_RODATA("asm/us/ovl_3e/nonmatchings/ovl_3e", g_menuDiscClut);
