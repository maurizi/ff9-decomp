#include "ff9.h"

void config_reset(void) {
    GameConfig *cfg = (GameConfig *)&g_gameState->config;
    s32 i;

    cfg->soundMono = 0;
    cfg->padCustom = 0;
    cfg->cursorMemory = 0;
    cfg->atbWait = 1;
    cfg->battleCameraFixed = 0;
    cfg->battleControlCustom = 0;
    cfg->movementRun = 0;
    cfg->vibration = 1;
    cfg->battleSpeed = 1;
    cfg->battleMsgSpeed = 3;
    cfg->messageSpeed = 3;
    cfg->unk0_17 = 0;
    cfg->buttonMap = 0;
    for (i = 0; i < PAD_SLOT_COUNT; i++) {
        cfg->buttonMap |= i << (i * 4);
    }
    config_apply();
}

void config_applyPadMapping(void) {
    s32 *act;
    s32 port;
    s32 i;
    u32 *tbl;

    port = 0;
    tbl = g_configButtons;
    for (; port < 2; port++) {
        for (i = 0; i < PAD_SLOT_COUNT; i++) {
            act = g_configPadButtons;
            pad_setMapping(port, tbl[i], act[config_applyPadMapping_remap(i)]);
        }
        for (i = 0; i < 1; i++) {
            pad_setMapping(port, g_configExtraButtons[i], act[config_applyPadMapping_remap(g_configExtraSlots[i])]);
        }
        pad_setMapping(port, BTN_START, PAD_START);
        pad_setMapping(port, 0x02000000, PAD_SELECT);
        pad_setMapping(port, BTN_SELECT, PAD_SELECT);
    }
}

void config_init(void) {
    config_reset();
}

/* K&R: text_getButtonSprite calls it without an argument */
s32 config_remapButton(button)
s32 button;
{
    GameConfig *cfg = (GameConfig *)&g_gameState->config;

    if (cfg->padCustom != 1) {
        return button;
    }
    return (cfg->buttonMap >> (button * 4)) & 0xF;
}

void config_apply(void) {
    config_applySound();
    config_applyPadMapping();
}

void config_applySound(void) {
    akao_setOutputMode((g_gameState->config >> CFG_BIT_SOUND) & 1);
}

void game_initGfx(void) {
    GameData *gd = g_gameState->gameData;
    gfx_init(gd->ot[0], gd->ot[1], gd->packetBuf, gd->packetBuf + 0x4000);
}

void game_updateWindows(void) {
    win_runFrame();
}

void game_initTimer(void) {
    timer_resetAll();
}

void game_updateTimer(void) {
    timer_update();
}

void timer_draw(void) {
    EvtTimer *timer;
    u8 blink;
    u8 *pkt;
    u32 *ot;
    u8 shown;
    u8 colon;
    s32 w;
    s32 elapsed;
    s32 subSec;
    s32 dg;
    s32 hr;
    s32 min;

    timer = &g_clock.timer;
    elapsed = g_clock.lastVsync - timer->startVsync;
    blink = 0;
    min = timer->remaining / 60;
    hr = min / 60;
    ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf] + 1;
    g_timerRect.x = 4;
    pkt = g_clock.pktPtr;
    min %= 60;
    if (elapsed < 8) {
        g_timerRect.x = elapsed * 8 - 0x3C;
        blink = 1;
    }
    if (timer->running == 0) {
        subSec = VSync(-1) % 60;
        colon = 0;
        if (blink || subSec >= 30) {
            colon = 1;
        }
        shown = colon;
    } else if (hr != 0 || min != 0) {
        subSec = timer->remaining % 60;
        shown = 1;
        colon = subSec >= 30;
    } else {
        subSec = VSync(-1) % 60;
        colon = subSec >= 30;
        shown = colon;
    }
    if (shown) {
        dg = hr / 10;
        if (dg != 0) {
            pkt = ui_buildSprite(g_timerRect.x + 5, 8, ot, dg + UI_SPRITE_TIMER_DIGITS, timer->running == 0, &w, pkt);
        }
        dg = hr - dg * 10;
        pkt = ui_buildSprite(g_timerRect.x + 0xF, 8, ot, dg + UI_SPRITE_TIMER_DIGITS, timer->running == 0, &w, pkt);
        dg = min / 10;
        pkt = ui_buildSprite(g_timerRect.x + 0x23, 8, ot, dg + UI_SPRITE_TIMER_DIGITS, timer->running == 0, &w, pkt);
        dg = min - dg * 10;
        pkt = ui_buildSprite(g_timerRect.x + 0x2D, 8, ot, dg + UI_SPRITE_TIMER_DIGITS, timer->running == 0, &w, pkt);
    }
    if (colon) {
        pkt = ui_buildSprite(g_timerRect.x + 0x19, 8, ot, UI_SPRITE_TIMER_COLON, timer->running == 0, &w, pkt);
    }
    pkt = win_buildBorder(&g_timerRect, ot, 1, 1, 1, CONFIG_WINDOW_COLOR(), pkt);
    setlen(pkt, PRIM_LEN(TILE));
    ((TileWords *)pkt)->rgbc = GPU_CODE(GPU_TILE | GPU_ABE);
    ((TileWords *)pkt)->xy = ((g_timerRect.x + 2) & 0xFFFF) | ((g_timerRect.y + 2) << 16);
    ((TileWords *)pkt)->wh = (g_timerRect.w - 4) | ((g_timerRect.h - 4) << 16);
    addPrim(ot, pkt);
    pkt += sizeof(TileWords);
    SetDrawTPage(pkt, 1, 1, 0);
    addPrim(ot, pkt);
}

void timer_resetAll(void) {
    timer_clear();
    g_clock.pktPtr = g_clock.pkt;
    g_gameState->playTime = 0;
    g_clock.lastVsync = VSync(-1);
}

void timer_update(void) {
    g_clock.pktPtr = &g_clock.pkt[g_gameState->gameData->drawBuf << 9];
    if (timer_isEnabled() != 0) {
        timer_draw();
    }
    game_tickClock();
}

void game_resumeClock(u8 resume) {
    s32 now = VSync(-1);
    s32 elapsed;
    EvtTimer *timer = &g_clock.timer;

    if (resume) {
        elapsed = now - g_clock.lastVsync;
        if (timer->enabled != 0 && timer->running != 0 && timer->remaining != 0) {
            timer->remaining -= elapsed;
            if (timer->remaining <= 0) {
                timer->remaining = 0;
            }
        }
        g_gameState->playTime += elapsed;
    } else {
        g_clock.lastVsync = now;
    }
}

void game_setPlayTime(s32 playTime) {
    g_gameState->playTime = playTime;
    g_clock.lastVsync = VSync(-1);
}

s32 game_getPlayTime(void) {
    return g_gameState->playTime;
}

u32 game_getPlayTimeSeconds(void) {
    return g_gameState->playTime / 60;
}

void timer_clear(void) {
    g_timer.enabled = 0;
    g_timer.remaining = 0;
    g_timer.initial = 0;
    g_timer.running = 0;
    g_timer.startVsync = VSync(-1);
}

void timer_enable(u8 enabled) {
    g_timer.enabled = enabled;
    g_timer.running = 0;
    g_timer.startVsync = VSync(-1);
}

u8 timer_isEnabled(void) {
    return g_timer.enabled;
}

void timer_setRunning(u8 running) {
    if (g_clock.timer.enabled != 0) {
        g_clock.timer.running = running;
    }
}

u8 timer_isRunning(void) {
    return g_timer.running;
}

void timer_setSeconds(s32 seconds) {
    ClockState *clock = &g_clock;
    s32 frames = seconds * 60;

    clock->timer.remaining = frames;
    clock->timer.initial = frames;
}

s32 timer_getSeconds(void) {
    return (g_timer.remaining + 59) / 60;
}

void game_tickClock(void) {
    s32 now;
    s32 dt;
    EvtTimer *timer;

    now = VSync(-1);
    dt = now - g_clock.lastVsync;
    timer = &g_clock.timer;
    if (timer->enabled != 0 && timer->running != 0 && timer->remaining != 0) {
        timer->remaining -= dt;
        if (timer->remaining <= 0) {
            timer->remaining = 0;
        }
    }
    g_gameState->playTime += dt;
    g_clock.lastVsync = now;
}
