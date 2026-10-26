#ifndef OVL_65_H
#define OVL_65_H

/* Declarations local to overlay 0x65 (battle rewards: "EXP Received", loaded
 * at 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* other overlays (battle, 0x800A7000+ / 0x800E....) */
s32 btl_soundCmd(s32, s32, s32, s32, s32);
void btlui_loadPortraits(void);
void func_800F4E68(void);

/* main exe: candidates for ff9.h */
void timer_enable(u8);
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#endif
#ifndef ITEM_NONE
#define ITEM_NONE 0xFF
#endif
#define LEVEL_MAX 99
#define EXP_MAX   9999999
#define GIL_MAX   9999999
#define ABILITY_SUPPORT_FIRST 0xC0 /* ability ids from here are support abilities (g_supportAbilities) */
/* support abilities, names from this overlay's name table (s_rewardsSupportNames) */
#define SUPPORT_LEVEL_UP    0xEB /* "Level Up": exp x1.5 */
#define SUPPORT_ABILITY_UP  0xEC /* "Ability Up": AP x2 */
#define SUPPORT_MILLIONAIRE 0xED /* "Millionaire": gil x1.5 */
#define BATTLE_END_GAME_OVER 6   /* GameData.unk56E: party defeated (gameover_main) */

/* sounds of the rewards screen */
#define SE_EXP_COUNT  0x69  /* loop while exp counts up (func_8002F514 / func_8002F55C) */
#define SE_GIL_COUNT  0x6D  /* loop while gil counts */
#define SE_ITEMS      0x6C  /* items or a card received */
#define SE_LEARN      0x413 /* an ability learned window opens */
#define SE_LEVEL_UP   0x2AB /* level-up label */
#define SE_ITEMS_FULL 0x416 /* "You cannot carry more than 99 units" */

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
#define BTL_DROP_COUNT 0x10

typedef struct {
    u32 gil;
    u32 exp;       /* split among the members in expMask */
    u16 ap;
    u8 expMask;    /* bit per party slot: receives exp */
    u8 unkB;
    u8 items[BTL_DROP_COUNT]; /* dropped item ids, ITEM_NONE = none */
    u8 card;       /* card won */
} BattleResult;
extern BattleResult g_btlRewards;
extern u8 D_800FDF4D;

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
    u16 unkE; /* name offset into s_rewardsActionNames */
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

/* Member can learn abilities (alive, func_80034A30). */
static inline u8 func_801F_canAp(Character *c) {
    if (!func_801F_alive(c)) {
        return 0;
    }
    if (!func_80034A30(c)) {
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

void func_801EF28C(void);
void gameover_initDisp(DispBuf *);
void gameover_loadImage(s32);
s32 battleEnd_update(void);
void gameover_main(s32);
void battleEnd_init(void);
void battleEnd_exit(void);
s32 func_801EF6D8(void);
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
void func_801F325C(void);
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
void ui_drawItemIcon(s32 arg0, s32 arg1, s32 otz, s32 arg3, u8 arg4); /* main */

#endif
