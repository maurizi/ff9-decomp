#include "ovl_3e.h"

void discChange_init(void) {
    s32 *src;
    DiscChange *st;
    s32 i;
    u8 r;
    u8 state;

    src = D_80073998;
    st = &s_discChange;
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
    discChange_loadPicture();
    cd_stopMotor();
    s_discChange.shellOpen = r = cd_checkShellOpen();
    state = DISC_ST_LID_CLOSED;
    if (r) {
        state = DISC_ST_LID_OPEN;
    }
    s_discChange.state = state;
}

void discChange_drawBackground(void) {
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

void discChange_drawPanel(void) {
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

u8 discChange_checkDisc(void) {
    CdlLOC loc;
    u8 result[8];
    s32 mask;
    s32 i;
    s32 ret;
    s32 type;

    mask = 1 << s_discChange.disc;
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

void discChange_update(void) {
    discChange_input();
    discChange_draw();
    discChange_updateState();
}

void discChange_exit(void) {
    s32 *dst;
    DiscChange *st;
    s32 i;

    if (s_discChange.nextLocation != DISC_NO_LOCATION) {
        evt_setNextLocation(s_discChange.nextLocation);
    }
    dst = D_80073998;
    st = &s_discChange;
    g_gameState->unk0 |= 0x100000;
    for (i = 0; i < 2; i++) {
        dst[i] = st->savedFlags[i];
    }
}

void discChange_input(void) {
    void (*f)(void);

    if (!func_8002F810()) {
        f = s_discChangeInputFuncs[s_discChange.state];
        if (f != NULL) {
            f();
        }
    }
}

void discChange_draw(void) {
    void (*f)(void) = s_discChangeDrawFuncs[s_discChange.state];

    if (f != NULL) {
        f();
    }
}

void discChange_updateState(void) {
    void (*f)(void) = s_discChangeUpdateFuncs[s_discChange.state];

    if (f != NULL) {
        f();
    }
}

void discChange_loadPicture(void) {
    u32 *buf = DISC_PICTURE_BUF;
    s32 size;
    u32 *clut = s_discChangeClut;
    s32 i;
    s32 sector;

    sector = g_gameState->unk20 + file_find(1, s_discChangePictureFiles[s_discChange.disc & 3])->unk4;
    size = DISC_PICTURE_HALF_BYTES;
    for (i = 0; i < 2; i++) {
        cd_readSync(sector, size, buf);
        discChange_loadPictureHalf(buf, i);
        sector += DISC_PICTURE_HALF_SECTORS;
    }
    LoadImage(&s_discChangeClutRect, clut);
    DrawSync(0);
    LoadImage(&s_discChangePanelRect, clut + 0x80);
    DrawSync(0);
}

void discChange_loadPictureHalf(u32 *p, s32 i) {
    RECT r = s_discChangePictureRect;

    r.x += i * DISC_PICTURE_W;
    LoadImage(&r, p);
    DrawSync(0);
}

void discChange_inputLidClosed(void) {
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        discChange_startCheck();
    }
}

void discChange_inputLidOpen(void) {
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        discChange_startCheck();
    }
}

void discChange_inputError(void) {
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        s_discChange.state = DISC_ST_ERROR_CLOSE;
        func_8002FAAC(0x18);
        func_8002F8A8(discChange_closeError);
    }
}

void discChange_updateLidClosed(void) {
    u8 r;

    s_discChange.shellOpen = r = cd_checkShellOpen();
    if (r) {
        s_discChange.state = DISC_ST_LID_OPEN;
    }
}

void discChange_updateLidOpen(void) {
    u8 r;

    s_discChange.shellOpen = r = cd_checkShellOpen();
    if (!r) {
        snd_playSe(SE_CURSOR);
        discChange_startCheck();
    }
}

void discChange_updateError(void) {
    if (++s_discChange.timer >= DISC_ERROR_FRAMES) {
        s_discChange.state = DISC_ST_ERROR_CLOSE;
        func_8002FAAC(0x18);
        func_8002F8A8(discChange_closeError);
    }
}

void discChange_startCheck(void) {
    s_discChange.unk2 = 0;
    s_discChange.state = DISC_ST_BUSY;
    func_8002FAAC(0x18);
    func_8002F8A8(discChange_doCheck);
}

void discChange_doCheck(void) {
    u8 r;

    s_discChange.checkResult = r = discChange_checkDisc();
    s_discChange.timer = 0;
    switch (r) {
    case DISC_CHECK_SHELL_OPEN:
    default:
        s_discChange.state = DISC_ST_ERROR;
        func_8002FA50(0x18);
        break;
    case DISC_CHECK_OK:
        func_80032640(0);
        func_80032634(0);
        func_800325E0();
        break;
    }
}

void discChange_closeError(void) {
    s_discChange.state = DISC_ST_BUSY;
    func_8002FA50(0x18);
    func_8002F8A8(discChange_resetState);
}

void discChange_resetState(void) {
    s_discChange.state = cd_checkShellOpen() ? DISC_ST_LID_OPEN : DISC_ST_LID_CLOSED;
}

INCLUDE_RODATA("asm/us/ovl_3e/nonmatchings/ovl_3e", s_discChangePictureRect);

INCLUDE_RODATA("asm/us/ovl_3e/nonmatchings/ovl_3e", s_discChangeClut);
