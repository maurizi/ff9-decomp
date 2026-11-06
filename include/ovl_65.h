#ifndef OVL_65_H
#define OVL_65_H

/* Declarations local to overlay 0x65 (battle rewards: "EXP Received", loaded
 * at 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* other overlays (battle, 0x800A7000+ / 0x800E....) */
s32 btl_soundCmd(s32, s32, s32, s32, s32);
void btlui_loadPortraits(void);
void btl_termBanner(void);

/* main exe: candidates for ff9.h */
#define WIN_TITLE_X 6    /* default Window.titleX / titleY (win_init): a window label's offset from its box */
#define WIN_TITLE_Y (-3)
void timer_enable(u8);
/* gameover_main steps (it counts them up with state++). Evidence: [code] */
#define GAMEOVER_ST_START 0 /* one frame */
#define GAMEOVER_ST_FADE_IN 1 /* the subtractive fade tile steps from 255 to 0 by 4: the picture fades in from black */
#define GAMEOVER_ST_WAIT 2 /* picture shown until Cross */
#define GAMEOVER_ST_FADE_OUT 3 /* back to black, then the screen is cleared */
#define GAMEOVER_ST_DONE 4
#define GAMEOVER_FADE_STEP 4 /* fade tile shade change per frame */
/* the GAME OVER picture: 16-bit SCREEN_W x SCREEN_H at VRAM (0, GAMEOVER_PIC_VRAM_Y), drawn as two sprites split at
 * x GAMEOVER_PIC_LEFT_W (a 16-bit sprite spans at most 256 pixels) */
#define GAMEOVER_PIC_VRAM_Y 256
#define GAMEOVER_PIC_LEFT_W 256
/* battleEnd_update's result (battleEnd_main). Evidence: [code] */
#define BATTLE_END_UPD_RUNNING 0
#define BATTLE_END_UPD_RETURN 1 /* rewards done: back to the mode the battle came from (work->returnMode) */
#define BATTLE_END_UPD_TITLE 2 /* after the game-over screen: GS_MODE_OPENING */
/* sounds of the rewards screen */
#define SE_EXP_COUNT  0x69  /* loop while exp counts up (snd_startSe / snd_stopSe) */
#define SE_GIL_COUNT  0x6D  /* loop while gil counts */
#define SE_LEARN      0x413 /* an ability learned window opens */
#define SE_LEVEL_UP   0x2AB /* level-up label */

/* g_rewardsText ids */
#define REWARDS_TEXT_EXP           0
#define REWARDS_TEXT_AP            1
#define REWARDS_TEXT_CONFIRM       7   /* "Press Cross to confirm." */
#define REWARDS_TEXT_NO_ITEM       9   /* "No item received." */
#define REWARDS_TEXT_ITEMS_FULL    0xA /* "You cannot carry more than 99 units of any item..." */
#define REWARDS_TEXT_CARD          0xB /* " Card" */
#define REWARDS_TEXT_NEXT          0xC
#define REWARDS_TEXT_RECEIVED_GIL  0xD
#define REWARDS_TEXT_CURRENT_GIL   0xE

/* Battle result (g_btlRewards). */
/* Rewards screen timings */
#define REWARDS_EXP_COUNT_FRAMES 16 /* EXP counts up over this many frames (1 per frame below it) */
#define REWARDS_EXP_STEP 32 /* from REWARDS_EXP_COUNT_FRAMES * this on: this much per frame ... */
#define REWARDS_EXP_MAX_FRAMES 192 /* ... but never longer than this */
#define REWARDS_POPUP_RISE_FRAMES 10 /* "LEVEL UP" popup: rises (s_rewardsPopupRise) */
#define REWARDS_POPUP_HOLD_FRAMES 0x40 /* stays */
#define REWARDS_POPUP_GAP_FRAMES 4 /* hidden before the next one */
#define REWARDS_POPUP_RISEN_Y (-0x11) /* its y after the rise (rewards_showLevelUp skips straight to it) */
#define REWARDS_LEARN_FRAMES 0x40 /* ability learned window shown this long */
#define REWARDS_GIL_END_FRAMES 24 /* after the gil count: wait, then close */
#define REWARDS_CLOSE_FADE_FRAMES 0x10
#define REWARDS_ITEMS_FADE_FRAMES 4 /* EXP / AP screen out */
#define REWARDS_ITEMS_FADE_IN_FRAMES 0xC /* gil / items screen in */
#define REWARDS_ITEMS_OPEN_FRAMES 0x10 /* items window open animation */
#define REWARDS_GIL_SHOWN_MAX 999999 /* Received gil shown capped (6 digits) */

/* Rewards screen layout (s_rewards.wins: 0 INFO title, 2-5 member panels, 6 EXP, 7 AP, 8 received gil,
 * 9 current gil). Offsets are from the window. */
#define REWARDS_OTZ_POPUP 6 /* ability learned / items full popups */
#define REWARDS_OTZ_TEXT 10
#define REWARDS_OTZ_WIN 0xB /* INFO, member, EXP, AP windows and their contents */
#define REWARDS_OTZ_SHADOW 0xC
#define REWARDS_FADE_OTZ 0xC /* fade_setOtz between the two screens */
#define REWARDS_OTZ_GIL_TEXT 13
#define REWARDS_OTZ_GIL_WIN 0xE /* gil windows, items window */
#define REWARDS_OTZ_GIL_SHADOW 0xF
#define REWARDS_TITLE_X 0x13 /* INFO / Help window text */
#define REWARDS_TITLE_Y 6
#define REWARDS_PORTRAIT_X 4 /* member panel */
#define REWARDS_PORTRAIT_Y 4
#define REWARDS_LEVELUP_X 0x24 /* "LEVEL UP" popup over the portrait (+ Popup.y) */
#define REWARDS_LEVELUP_Y 0x12
#define REWARDS_MEMBER_TEXT_X 0x29 /* name, "EXP", "Next" */
#define REWARDS_NAME_Y 4
#define REWARDS_LV_X 0x72 /* "Lv" sprite */
#define REWARDS_LV_Y 7
#define REWARDS_LEVEL_X 0x84
#define REWARDS_LEVEL_Y 5
#define REWARDS_STATUS_X 0x28 /* status-ailment icons */
#define REWARDS_STATUS_Y 0x13
#define REWARDS_EXP_LABEL_Y 0x1C /* (values 4 lower) */
#define REWARDS_EXP_X 0x62 /* EXP (7 digits) */
#define REWARDS_NEXT_LABEL_Y 0x26
#define REWARDS_NEXT_X 0x69 /* to the next level (6 digits) */
#define REWARDS_TOTAL_TEXT_X 0xC /* EXP / AP / gil windows: label */
#define REWARDS_TOTAL_TEXT_Y 5
#define REWARDS_TOTAL_DIGITS_Y 9
#define REWARDS_EXP_TOTAL_X 0x5A /* EXP each (6 digits) */
#define REWARDS_AP_TOTAL_X 0x6F /* AP (3 digits) */
#define REWARDS_RECEIVED_GIL_X 0x54 /* (6 digits + unit) */
#define REWARDS_CURRENT_GIL_X 0x4D /* (7 digits + unit) */
#define REWARDS_LEARN_WIN_X 0x26 /* ability learned window, relative to the member panel */
#define REWARDS_LEARN_WIN_Y 0x1B
#define REWARDS_LEARN_WIN_W 0x70
#define REWARDS_LEARN_WIN_H 0x19
#define REWARDS_LEARN_ICON_X 6
#define REWARDS_LEARN_ICON_Y 7
#define REWARDS_LEARN_NAME_X 0x16
#define REWARDS_LEARN_NAME_Y 6
#define REWARDS_ITEM_LABEL_X 8 /* items window */
#define REWARDS_ITEM_COUNT_LABEL_R 0x97 /* right edge of the count label */
#define REWARDS_ITEM_Y 7 /* rows (count digits 3 lower) */
#define REWARDS_ITEM_ROW_H 15
#define REWARDS_ITEM_ICON_X 7
#define REWARDS_ITEM_NAME_X 0x16 /* item name; card name */
#define REWARDS_ITEM_COUNT_X 0x88
/* "LEVEL UP" image: REWARDS_LEVELUP_W x _H, 8-bit at tpage (0x3C0, 0x100), CLUT row 0x1FF; the glow drawn
 * additively over it is the same size, REWARDS_LEVELUP_GLOW_V lower in the page */
#define REWARDS_LEVELUP_W 0x4C
#define REWARDS_LEVELUP_H 0x14
#define REWARDS_LEVELUP_V 0xB0
#define REWARDS_LEVELUP_GLOW_V 0xC4
#define REWARDS_LEVELUP_VRAM_X 0x3C0
#define REWARDS_LEVELUP_VRAM_Y 0x100
#define REWARDS_LEVELUP_CLUT_Y 0x1FF
#define BTL_DROP_COUNT 0x10

typedef struct {
    u32 gil;
    u32 exp;       /* split among the members in expMask */
    u16 ap;
    u8 expMask;    /* bit per party slot: receives exp */
    u8 hasPostEvent; /* encounter flag 0x800, story battles (runtime) */
    u8 items[BTL_DROP_COUNT]; /* dropped item ids, ITEM_NONE = none */
    u8 card;       /* card won */
    u8 fleeGil;    /* set when Flee-Gil turned the escape into gil (ovl_0b btl_victory), rewards_getKind shows the rewards after an escape */
} BattleResult;
extern BattleResult g_btlRewards;

/* ---- overlay 0x65 ---- */

/* a number counted up on screen */
typedef struct {
    u32 target;
    u32 step;   /* per frame */
    u32 cur;
} Counter;

/* a member's queue of pop-up windows ("Learned" ability names) */
typedef struct {
    Window *win;  /* the window up now */
    u16 queued;   /* windows still to open */
    u16 shown;    /* index into ids of the window up */
    u16 delay;    /* frames before the next one opens */
    u8 ids[0x12]; /* ability ids */
    s32 timer;    /* frames the window has been up (closes at 0x40) */
} WinSlot; /* 0x20 */

enum { POPUP_NONE, POPUP_RISE, POPUP_HOLD, POPUP_GAP, POPUP_DONE }; /* Popup.phase (rewards_stepPopup) */

/* the level-up label over a member's panel */
typedef struct {
    u8 active;
    u8 phase;   /* POPUP_* */
    u8 enabled;
    u8 pad3;
    s32 timer;
    u8 pad8[0x14 - 8];
    u8 pending; /* level-ups still to show */
    u8 count;   /* level-ups shown */
    s16 y;      /* offset, moved by s_rewardsPopupRise while rising */
} Popup; /* 0x18 */

typedef struct {
    u8 pad0[0x10];
    u8 count;
    u8 ids[0x13]; /* abilities learned */
} LearnList; /* 0x24 */

/* 0x88 bytes at s_rewards+0x24, cleared as a block */
#define REWARDS_ITEM_KINDS 6

typedef struct {
    u32 expEach;   /* exp per member */
    u32 ap;
    s32 card;      /* ITEM_NONE = none (or the card inventory is full) */
    u32 itemCount; /* item kinds */
    u8 items[REWARDS_ITEM_KINDS][2]; /* {item id, count} */
    Counter exp[4];
    Counter apGain[4];
    Counter gil;
} Rewards;

/* RewardsMenu.state: handler in s_rewardsStates */
enum {
    REWARDS_STATE_OPENING,  /* windows sliding in */
    REWARDS_STATE_RECEIVED, /* "Received EXP and AP!": Cross gives the AP */
    REWARDS_STATE_EXP,      /* exp counting, level-ups, learned abilities; Cross ends it */
    REWARDS_STATE_ITEMS,    /* "Received Item(s) and Gil": Cross takes the items */
    REWARDS_STATE_GIL,      /* gil counting; Cross ends it, then the screen closes */
    REWARDS_STATE_CLOSING
};

/* battle rewards screen (s_rewards) */
typedef struct {
    u8 active;
    u8 running;     /* screen up: draw and run the state handler */
    u8 closing;     /* counting the last frames before rewards_update returns 0 */
    u8 gilSkipped;  /* Cross pressed in REWARDS_STATE_GIL */
    u8 done;
    u8 pad5[3];
    u8 state;       /* REWARDS_STATE_*, index into s_rewardsStates */
    u8 expMembers;  /* party members receiving exp */
    u16 screen;     /* 0 = EXP and AP, 1 = items and gil (s_rewardsTitleText) */
    u32 timer;
    u32 closeFrames;
    u8 getsExp[4];
    u8 getsAp[4];
    u8 levelsUp[4]; /* will level up (rewards_willLevelUp) */
    u8 learns[4];   /* will learn an ability (rewards_willLearn) */
    Rewards rewards;
    WinDef wins[10]; /* help, title, 4 member panels, EXP, AP, received gil, current gil (s_rewardsWinInit) */
    u8 winShown[10];
    u8 pad156[2];
    WinSlot levelWins[4];
    WinSlot learnWins[4];
    Popup levelPopups[4];
    LearnList learned[4];
    WinSlot fullWin;  /* "You cannot carry more than 99 units" */
    Window *itemsWin; /* received items */
    s32 frame;
    s32 savedOtz;     /* fade_setOtz value restored on close */
    u8 *prim;         /* next free primitive */
    u8 *primBufs[2];
} RewardsMenu;

extern RewardsMenu s_rewards;

typedef struct {
    u8 pad0[0xE];
    u16 nameOffset; /* name offset into s_rewardsActionNames */
} Ovl65Ability; /* active ability table */
extern Ovl65Ability s_rewardsActionTable[];
extern u8 s_rewardsActionNames[]; /* ability names */
extern u8 s_rewardsSupportNames[]; /* support ability names */
extern RECT s_rewardsLearnRect;  /* level-up window rect */
extern RECT *s_rewardsLearnPanel; /* window it is placed under */
extern u8 s_rewardsLearnLabel[];  /* level-up text */
extern u8 g_rewardsText[];
extern void (*s_rewardsStates[])(void);
extern s32 s_rewardsStateSe[]; /* sound effect per state */
extern s32 s_rewardsTitleText[]; /* text id */
extern u8 s_rewardsGilUnit[];
extern u8 s_rewardsGilUnit2[];
typedef struct {
    RECT rect;
    u8 shown; /* initially shown */
    u8 pad9;
} WinInit;
extern WinInit s_rewardsWinInit[10];
extern u8 s_rewardsWinOpen[10][0x10];
extern u8 s_rewardsWinClose[7][0x10];
extern u8 s_rewardsImage0[];
extern u8 s_rewardsImage1[];
extern s8 s_rewardsPopupRise[];
extern RECT s_rewardsClutRect0;
extern RECT s_rewardsImageRect0;
extern RECT s_rewardsClutRect1;
extern RECT s_rewardsImageRect1;
extern RECT s_rewardsVramRestoreRect;
extern RECT s_gameoverClearRect; /* const {0, 0, 320, 224} */
extern RECT s_gameoverVramRect; /* const {0, 0, 640, 224} */

/* Double-buffered display setup (gameover_initDisp). */
typedef struct {
    DRAWENV draw;
    DISPENV disp;
    u32 ot[2];
    DR_TPAGE picTpage0;
    SPRT pic0;          /* the GAME OVER picture, left 256 pixels */
    DR_TPAGE picTpage1;
    SPRT pic1;          /* right 64 pixels */
    DR_TPAGE fadeTpage;
    TILE fade;          /* subtractive black-to-clear fade over the picture */
} DispBuf; /* 0xC8 */
extern RECT s_rewardsItemsRect;
extern u8 s_rewardsPrimBufs[2][0x1400]; /* primitive buffers */
extern u32 s_rewardsVramSave[0x100];   /* VRAM backup */
extern u8 s_rewardsFullShown;

static inline u8 func_801F_alive(Character *c) {
    if (c == NULL) {
        return 0;
    }
    if (c->hp == 0) {
        return 0;
    }
    if (c->status & 0x45) {
        return 0;
    }
    return 1;
}

/* Member can still gain exp (alive, exp below the cap). */
static inline u8 func_801F_canExp(Character *c) {
    if (!func_801F_alive(c)) {
        return 0;
    }
    if (c->exp >= EXP_MAX) {
        return 0;
    }
    return 1;
}

/* Member can learn abilities (alive, chr_canLearnAbilities). */
static inline u8 func_801F_canAp(Character *c) {
    if (!func_801F_alive(c)) {
        return 0;
    }
    if (!chr_canLearnAbilities(c)) {
        return 0;
    }
    return 1;
}

/* Amount to add to a counter this frame. */
static inline u32 Counter_step(Counter *c) {
    u32 step;
    if (c->target < c->cur + c->step) {
        step = c->target - c->cur;
    } else {
        step = c->step;
    }
    return step;
}

static inline void WinSlot_clear(WinSlot *s) {
    s->queued = 0;
    s->shown = 0;
    s->timer = 0;
    s->delay = 0;
    s->win = 0;
}

extern u8 s_rewardsAbilityItems[0x30]; /* per ability slot: equipped items teaching it */
s32 chr_findAbilitySlot(s32, s32); /* main exe */

void gameover_initDisp(DispBuf *);
void gameover_loadImage(s32);
s32 battleEnd_update(void);
void gameover_main(s32);
void battleEnd_init(void);
void battleEnd_exit(void);
s32 battleEnd_loadPortraits(void);
s32 battleEnd_loadDb(void);
s32 battleEnd_unloadDb(void);
void battleEnd_preloadScene(void);
void rewards_drawInfo(void);
void rewards_drawMembers(void);
void rewards_drawExp(void);
void rewards_drawAp(void);
void rewards_drawReceivedGil(void);
void rewards_drawCurrentGil(void);
void rewards_drawPanelShade(RECT *);
void rewards_initWindows(void);
void rewards_moveMemberWins(void);
void rewards_stateGil(void);
void rewards_finishExp(void);
void rewards_onExpWindowsClosed(void);
void rewards_findMembers(void);
void rewards_compute(void);
void rewards_drawSmallText(s32, s32, s32, u8 *, s32);
void rewards_drawText(s32, s32, s32, u8 *, s32);
void rewards_init(void);
u8 rewards_update(void);
void rewards_exit(void);
void rewards_requestClose(void);
s32 rewards_getKind(void);
void rewards_skip(void);
void rewards_open(void);
void rewards_waitEvent(void);
void rewards_draw(void);
void rewards_updateState(void);
void rewards_drawHelp(void);
void rewards_initPrimBufs(void);
void rewards_clearWinSlot(WinSlot *);
void rewards_closeMemberWins(void);
void rewards_stepWindows(void);
void rewards_stateReceived(void);
void rewards_stateExp(void);
void rewards_stateItems(void);
void rewards_giveAp(void);
void rewards_takeItems(void);
void rewards_finishGil(void);
u8 rewards_isRewarded(Character *, s32);
void rewards_onItemsFaded(void);
void rewards_onWindowsOpened(void);
void rewards_onClosed(void);
void rewards_drawDigits(s32, s32, s32, u8 *, s32);
void rewards_updateLearnWins(void);
void rewards_addAp(s32, u32);
u8 rewards_willLearn(Character *, s32);
void rewards_drawLearnWin(Window *);
void rewards_initLearns(void);
void rewards_updateAp(void);
s32 rewards_initApCounter(s32);
void rewards_stepAp(void);
void rewards_addLearned(s32, u8);
void rewards_onLearnWinClosed(Window *);
void rewards_drawLevelUp(s32, s32, s32, Popup *);
void rewards_initExp(void);
void rewards_updateExp(void);
void rewards_addExp(s32, u32);
void rewards_showLevelUp(s32);
u32 rewards_initExpCounter(s32);
s32 rewards_willLevelUp(Character *, s32);
void rewards_stepExp(void);
void rewards_updateLevelUps(void);
void rewards_levelUp(s32);
void rewards_initPopup(Popup *);
void rewards_startPopup(Popup *);
void rewards_endPopup(Popup *);
s32 rewards_stepPopup(Popup *);
void rewards_initGil(void);
void rewards_stepGil(void);
void rewards_addGil(s32);
void rewards_loadImages(void);
void rewards_restoreVram(void);
u8 rewards_addItems(void);
void rewards_drawItems(Window *);
void rewards_resetFullWarning(void);
u8 rewards_tryAddItems(void);
void rewards_onStepGil(void);
void rewards_openItemsWin(void);
void rewards_drawItemsLabels(Window *);
void rewards_drawFullWarning(Window *);
void rewards_onFullWarningClosed(void);

extern u8 s_rewardsTextBuf[]; /* text buffer */
extern Icon s_rewardsCardIcon; /* card icon */
/* main exe: candidates for ff9.h */
s32 text_getInfo(s32); /* text size of a menu string */

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void game_updateWindows(void); /* main */
s32 preload_update(void); /* main */
void ui_drawItemIcon(s32 x, s32 y, s32 otz, s32 item, u8 flag); /* main */

#endif
