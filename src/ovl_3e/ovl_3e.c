#include "ovl_3e.h"

void discChange_init(void) {
    s32 *src;
    DiscChange *st;
    s32 i;
    u8 lidOpen;
    u8 state;

    src = g_padPortMask;
    st = &s_discChange;
    st->unk2 = 0;
    st->timer = 0;
    g_menu.drawBackground = 0;
    st->disc = DISC_REQ_DISC(g_menu.arg);
    st->nextLocation = DISC_REQ_LOCATION(g_menu.arg);
    for (i = 0; i < 2; i++) {
        st->savedFlags[i] = src[i];
        src[i] |= 0x300;
    }
    help_setCallback(0);
    discChange_loadPicture();
    cd_stopMotor();
    s_discChange.shellOpen = lidOpen = cd_checkShellOpen();
    state = DISC_ST_LID_CLOSED;
    if (lidOpen) {
        state = DISC_ST_LID_OPEN;
    }
    s_discChange.state = state;
}

void discChange_drawBackground(void) {
    SprtWords *sprt;
    s32 i;
    RECT unused[2];

    for (i = 0; i < 2; i++) {
        setlen(g_primPtr, 4);
        sprt = (SprtWords *)g_primPtr;
        sprt->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
        sprt->xy = i * DISC_PICTURE_W;
        sprt->uvclut = (i * DISC_PICTURE_W) & 0x3F;
        sprt->wh = 0xE000A0;
        g_primPtr = (u8 *)(sprt + 1);
        addPrim(&g_ot[6], sprt);
        SetDrawTPage(g_primPtr, 1, 1, (((i * DISC_PICTURE_W) >> 6) + 0x10) | 0x100);
        addPrim(&g_ot[6], g_primPtr);
        g_primPtr += 8;
    }
}

void discChange_drawPanel(void) {
    SprtWords *sprt;
    u32 clut = GetClut(0, 0x1FF) << 16;

    setlen(g_primPtr, 4);
    sprt = (SprtWords *)g_primPtr;
    sprt->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    sprt->xy = 0x1C004E;
    sprt->uvclut = clut;
    sprt->wh = 0xA800C2;
    g_primPtr = (u8 *)(sprt + 1);
    addPrim(&g_ot[6], sprt);
    SetDrawTPage(g_primPtr, 1, 1, 0x95);
    addPrim(&g_ot[6], g_primPtr);
    g_primPtr += 8;
}

u8 discChange_checkDisc(void) {
    CdlLOC loc;
    u8 result[8];
    s32 discBit;
    s32 i;
    s32 ready;
    s32 type;

    discBit = 1 << s_discChange.disc;
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

    ready = CdDiskReady(0);
    switch (ready) {
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
        file_initImg();
        if (g_gameState->disc != DISC_NONE) {
            break;
        }
    }
    if (g_gameState->disc == DISC_NONE || !(g_gameState->disc & discBit)) {
        cd_stopMotor();
        return DISC_CHECK_WRONG_DISC;
    }
    snd_playSe(SE_COMPLETE);
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
    dst = g_padPortMask;
    st = &s_discChange;
    g_gameState->flags |= GS_RELOAD_FILE_TABLES;
    for (i = 0; i < 2; i++) {
        dst[i] = st->savedFlags[i];
    }
}

void discChange_input(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = s_discChangeInputFuncs[s_discChange.state];
        if (fn != NULL) {
            fn();
        }
    }
}

void discChange_draw(void) {
    void (*fn)(void) = s_discChangeDrawFuncs[s_discChange.state];

    if (fn != NULL) {
        fn();
    }
}

void discChange_updateState(void) {
    void (*fn)(void) = s_discChangeUpdateFuncs[s_discChange.state];

    if (fn != NULL) {
        fn();
    }
}

void discChange_loadPicture(void) {
    u32 *picture = DISC_PICTURE_BUF;
    s32 size;
    u32 *clut = s_discChangeClut;
    s32 i;
    s32 sector;

    sector = g_gameState->imgSector + file_find(DIR_COMMON, s_discChangePictureFiles[s_discChange.disc & 3])->sector;
    size = DISC_PICTURE_HALF_BYTES;
    for (i = 0; i < 2; i++) {
        cd_readSync(sector, size, picture);
        discChange_loadPictureHalf(picture, i);
        sector += DISC_PICTURE_HALF_SECTORS;
    }
    LoadImage(&s_discChangeClutRect, clut);
    DrawSync(0);
    LoadImage(&s_discChangePanelRect, clut + 0x80);
    DrawSync(0);
}

void discChange_loadPictureHalf(u32 *pixels, s32 i) {
    RECT rect = s_discChangePictureRect;

    rect.x += i * DISC_PICTURE_W;
    LoadImage(&rect, pixels);
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
        fade_out(0x18);
        fade_setOnDone(discChange_closeError);
    }
}

void discChange_updateLidClosed(void) {
    u8 lidOpen;

    s_discChange.shellOpen = lidOpen = cd_checkShellOpen();
    if (lidOpen) {
        s_discChange.state = DISC_ST_LID_OPEN;
    }
}

void discChange_updateLidOpen(void) {
    u8 lidOpen;

    s_discChange.shellOpen = lidOpen = cd_checkShellOpen();
    if (!lidOpen) {
        snd_playSe(SE_CURSOR);
        discChange_startCheck();
    }
}

void discChange_updateError(void) {
    if (++s_discChange.timer >= DISC_ERROR_FRAMES) {
        s_discChange.state = DISC_ST_ERROR_CLOSE;
        fade_out(0x18);
        fade_setOnDone(discChange_closeError);
    }
}

void discChange_startCheck(void) {
    s_discChange.unk2 = 0;
    s_discChange.state = DISC_ST_BUSY;
    fade_out(0x18);
    fade_setOnDone(discChange_doCheck);
}

void discChange_doCheck(void) {
    u8 result;

    s_discChange.checkResult = result = discChange_checkDisc();
    s_discChange.timer = 0;
    switch (result) {
    case DISC_CHECK_SHELL_OPEN:
    default:
        s_discChange.state = DISC_ST_ERROR;
        fade_in(0x18);
        break;
    case DISC_CHECK_OK:
        menu_setFadeOutOnClose(0);
        menu_setFadeInOnClose(0);
        menu_close();
        break;
    }
}

void discChange_closeError(void) {
    s_discChange.state = DISC_ST_BUSY;
    fade_in(0x18);
    fade_setOnDone(discChange_resetState);
}

void discChange_resetState(void) {
    s_discChange.state = cd_checkShellOpen() ? DISC_ST_LID_OPEN : DISC_ST_LID_CLOSED;
}

INCLUDE_RODATA("asm/us/ovl_3e/nonmatchings/ovl_3e", s_discChangePictureRect);

INCLUDE_RODATA("asm/us/ovl_3e/nonmatchings/ovl_3e", s_discChangeClut);
