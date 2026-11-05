#include "ovl_65.h"

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/gameover", s_gameoverClearRect);

void gameover_main(s32 image) {
    s16 shade = 0xFF;
    u8 state = GAMEOVER_ST_START;
    DispBuf bufs[2];
    RECT rect = s_gameoverClearRect;
    DispBuf *cur;

    gameover_initDisp(bufs);
    cur = bufs;
    DrawSync(0);
    gameover_loadImage(image);
    while (1) {
        cur = (cur == &bufs[0]) ? &bufs[1] : &bufs[0];
        ClearOTag(cur->ot, 2);
        switch (state) {
        case GAMEOVER_ST_START:
            state++;
            break;
        case GAMEOVER_ST_FADE_IN:
            setRGB0(&cur->fade, shade, shade, shade);
            shade -= 4;
            addPrim(&cur->ot[0], &cur->picTpage0);
            addPrim(&cur->ot[0], &cur->picTpage1);
            addPrim(&cur->ot[1], &cur->fadeTpage);
            if (shade < 0) {
                shade = 0;
                state++;
            }
            break;
        case GAMEOVER_ST_WAIT:
            setRGB0(&cur->fade, 0, 0, 0);
            addPrim(&cur->ot[0], &cur->picTpage0);
            addPrim(&cur->ot[0], &cur->picTpage1);
            addPrim(&cur->ot[1], &cur->fadeTpage);
            if (g_padPortPressed[0] & BTN_CROSS) {
                state++;
            }
            break;
        case GAMEOVER_ST_FADE_OUT:
            shade += 4;
            if (shade >= 0x100) {
                ClearImage(&rect, 0, 0, 0);
                DrawSync(0);
                if (shade > 0x100) {
                    state++;
                }
            } else {
                setRGB0(&cur->fade, shade, shade, shade);
                addPrim(&cur->ot[0], &cur->picTpage0);
                addPrim(&cur->ot[0], &cur->picTpage1);
                addPrim(&cur->ot[1], &cur->fadeTpage);
            }
            break;
        case GAMEOVER_ST_DONE:
            ClearImage(&rect, 0, 0, 0);
            DrawSync(0);
            return;
        }
        pad_update();
        DrawSync(0);
        VSync(0);
        PutDispEnv(&cur->disp);
        DrawOTagEnv(cur->ot, &cur->draw);
    }
}

void gameover_initDisp(DispBuf *bufs) {
    RECT rect = s_gameoverVramRect;
    u32 i;

    ClearImage(&rect, 0, 0, 0);
    SetDefDrawEnv(&bufs[0].draw, 0, 0, SCREEN_W, SCREEN_H);
    SetDefDispEnv(&bufs[0].disp, SCREEN_W, 0, SCREEN_W, SCREEN_H);
    SetDefDrawEnv(&bufs[1].draw, SCREEN_W, 0, SCREEN_W, SCREEN_H);
    SetDefDispEnv(&bufs[1].disp, 0, 0, SCREEN_W, SCREEN_H);
    for (i = 0; i < 2; i++) {
        bufs[i].draw.dtd = 1;
        bufs[i].draw.isbg = 1;
        bufs[i].disp.isinter = 0;
        setRECT(&bufs[i].disp.screen, 0, 8, 256, SCREEN_H);
        gfx_initTpageSprt(&bufs[i].picTpage0, GetTPage(2, 0, 0, 256));
        setXY0(&bufs[i].pic0, 0, 0);
        setUV0(&bufs[i].pic0, 0, 0);
        setWH(&bufs[i].pic0, 256, 224);
        setRGB0(&bufs[i].pic0, 0x80, 0x80, 0x80);
        gfx_initTpageSprt(&bufs[i].picTpage1, GetTPage(2, 0, 256, 256));
        setXY0(&bufs[i].pic1, 256, 0);
        setUV0(&bufs[i].pic1, 0, 0);
        setWH(&bufs[i].pic1, 64, 224);
        setRGB0(&bufs[i].pic1, 0x80, 0x80, 0x80);
        gfx_initTpageTile(&bufs[i].fadeTpage, GetTPage(0, 2, 0, 0));
        setXY0(&bufs[i].fade, 0, 0);
        setWH(&bufs[i].fade, SCREEN_W, SCREEN_H);
        setRGB0(&bufs[i].fade, 0, 0, 0);
        SetSemiTrans(&bufs[i].fade, 1);
    }
}

void gameover_loadImage(s32 dest) {
    FileEntry *file = file_find(DIR_COMMON, FILE_GAMEOVER_IMAGE);

    cd_readSync(g_gameState->imgSector + file->sector, (file->link - file->sector) << 11, dest);
    file_loadTim(dest);
}

s32 battleEnd_update(void) {
    GameData *gd;

    gd = g_gameState->gameData;
    msg_beginDraw();
    switch (gd->battleResult) {
    case BATTLE_END_VICTORY:
    case 2:
    case BATTLE_END_DEFEAT:
    case BATTLE_END_ESCAPE:
    case 5:
    case 7:
        if (!rewards_update()) {
            if (gd->battleResult == 2) {
                gd->battleResult = BATTLE_END_VICTORY;
            }
            btl_termBanner();
            rewards_exit();
            return BATTLE_END_UPD_RETURN;
        }
        evt_update();
        return BATTLE_END_UPD_RUNNING;
    case BATTLE_END_GAME_OVER:
        if (timer_isEnabled()) {
            timer_enable(0);
        }
        btl_termBanner();
        gameover_main((s32)g_gameState->gameData->ctx.ovl65->work->bgData);
        return BATTLE_END_UPD_TITLE;
    }
    return BATTLE_END_UPD_RUNNING;
}

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/gameover", s_gameoverVramRect);
