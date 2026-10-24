#include "ovl_65.h"

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/gameover", s_gameoverClearRect);

void gameover_main(s32 arg0) {
    s16 c = 0xFF;
    u8 state = 0;
    DispBuf b[2];
    RECT rect = s_gameoverClearRect;
    DispBuf *cur;

    gameover_initDisp(b);
    cur = b;
    DrawSync(0);
    gameover_loadImage(arg0);
    while (1) {
        cur = (cur == &b[0]) ? &b[1] : &b[0];
        ClearOTag(cur->ot, 2);
        switch (state) {
        case 0:
            state++;
            break;
        case 1:
            setRGB0(&cur->fade, c, c, c);
            c -= 4;
            addPrim(&cur->ot[0], &cur->picTpage0);
            addPrim(&cur->ot[0], &cur->picTpage1);
            addPrim(&cur->ot[1], &cur->fadeTpage);
            if (c < 0) {
                c = 0;
                state++;
            }
            break;
        case 2:
            setRGB0(&cur->fade, 0, 0, 0);
            addPrim(&cur->ot[0], &cur->picTpage0);
            addPrim(&cur->ot[0], &cur->picTpage1);
            addPrim(&cur->ot[1], &cur->fadeTpage);
            if (g_padPortPressed[0] & BTN_CROSS) {
                state++;
            }
            break;
        case 3:
            c += 4;
            if (c >= 0x100) {
                ClearImage(&rect, 0, 0, 0);
                DrawSync(0);
                if (c > 0x100) {
                    state++;
                }
            } else {
                setRGB0(&cur->fade, c, c, c);
                addPrim(&cur->ot[0], &cur->picTpage0);
                addPrim(&cur->ot[0], &cur->picTpage1);
                addPrim(&cur->ot[1], &cur->fadeTpage);
            }
            break;
        case 4:
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

void gameover_initDisp(DispBuf *b) {
    RECT rect = s_gameoverVramRect;
    u32 i;

    ClearImage(&rect, 0, 0, 0);
    SetDefDrawEnv(&b[0].draw, 0, 0, 320, 224);
    SetDefDispEnv(&b[0].disp, 320, 0, 320, 224);
    SetDefDrawEnv(&b[1].draw, 320, 0, 320, 224);
    SetDefDispEnv(&b[1].disp, 0, 0, 320, 224);
    for (i = 0; i < 2; i++) {
        b[i].draw.dtd = 1;
        b[i].draw.isbg = 1;
        b[i].disp.isinter = 0;
        setRECT(&b[i].disp.screen, 0, 8, 256, 224);
        gfx_initTpageSprt(&b[i].picTpage0, GetTPage(2, 0, 0, 256));
        setXY0(&b[i].pic0, 0, 0);
        setUV0(&b[i].pic0, 0, 0);
        setWH(&b[i].pic0, 256, 224);
        setRGB0(&b[i].pic0, 0x80, 0x80, 0x80);
        gfx_initTpageSprt(&b[i].picTpage1, GetTPage(2, 0, 256, 256));
        setXY0(&b[i].pic1, 256, 0);
        setUV0(&b[i].pic1, 0, 0);
        setWH(&b[i].pic1, 64, 224);
        setRGB0(&b[i].pic1, 0x80, 0x80, 0x80);
        gfx_initTpageTile(&b[i].fadeTpage, GetTPage(0, 2, 0, 0));
        setXY0(&b[i].fade, 0, 0);
        setWH(&b[i].fade, 320, 224);
        setRGB0(&b[i].fade, 0, 0, 0);
        SetSemiTrans(&b[i].fade, 1);
    }
}

void gameover_loadImage(s32 arg0) {
    FileEntry *e = file_find(1, 0xB);

    cd_readSync(g_gameState->imgSector + e->unk4, (e->unkC - e->unk4) << 11, arg0);
    file_loadTim(arg0);
}

s32 battleEnd_update(void) {
    GameData *g;

    g = g_gameState->unk1C;
    msg_beginDraw();
    switch (g->battleResult) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
        if (!rewards_update()) {
            if (g->battleResult == 2) {
                g->battleResult = 1;
            }
            func_800F4E68();
            rewards_exit();
            return 1;
        }
        evt_update();
        return 0;
    case BATTLE_END_GAME_OVER:
        if (timer_isEnabled()) {
            timer_enable(0);
        }
        func_800F4E68();
        gameover_main((s32)g_gameState->unk1C->unk8E4.ovl65->work->unk1C);
        return 2;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/gameover", s_gameoverVramRect);
