#ifndef OVL_15_H
#define OVL_15_H

/* Declarations local to overlay 0x15 (Blackjack minigame, loaded at
 * 0x800A7000). Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */
void func_800552B8(s32);
s32 func_8002B9B8(u8 *); /* text width */
void func_8001D9CC(s32, s32, s32, s32, s32);
s32 cd_findFile(s32);
void func_80022ED4(s32);
s32 func_80055290(void *);
void func_800553EC(s32, s32, s32, s32);
void func_80055704(s32);
void func_80055730(s32, s32);
void func_80055764(s32, s32, s32);
void func_800557A8(s32, s32, s32, s32);
void func_800557F0(s32);
void func_8005581C(s32, s32);
void func_80055850(s32, s32, s32);
void func_80055894(s32, s32, s32, s32);
void func_800558DC(s32);
void func_80055908(s32, s32);
void func_8005593C(s32, s32, s32);
void func_80055980(s32, s32, s32, s32);
void func_800559C8(s32, s32);
void func_800559FC(s32, s32, s32);
void func_80055A34(s32, s32, s32, s32);

/* ovl_64 (FMV player, 0x80198700) */
void MBG_init(s32, u8 **);
void mbg_play(s32, s32);
s32 mbg_update(s32);
void mbg_setRgb24(s32);
void mbg_shutdown(void);
void mbg_setEndCallback(void (*)(void));
void mbg_getLastFrame(void);
void mbg_streamSetBaseSector(s32);
typedef struct {
    s32 unk0;
    u8 pad4[0x14 - 4];
} Unk8019DC5CSub;
typedef struct {
    s32 unk0;
    Unk8019DC5CSub *unk4;
} Unk8019DC5C;
extern Unk8019DC5C g_mbgMovieLists[];

/* ---- this overlay ---- */

/* Card shoe: 6 decks of 52 cards. Card ids are rank * 4 + suit. */
typedef struct {
    u16 unk0; /* flags: 2 = shuffled, 4 = needs reshuffle */
    u16 unk2; /* next card */
    u8 unk4[6 * 52];
} Ovl15Shoe;

/* A blackjack hand. */
typedef struct {
    u16 unk0; /* flags: 1 = soft ace, 2 = bust, 8 = ace, 0x10 = hole card */
    u16 unk2; /* card count */
    u16 unk4; /* hand value */
    u16 unk6; /* value with aces as 1 */
    s16 unk8; /* screen x */
    s16 unkA; /* screen y */
    s32 unkC[22]; /* cards */
} Ovl15Hand;

typedef struct {
    void (*unk0)(void); /* init */
    void (*unk4)(void); /* enter */
    void (*unk8)(void); /* update */
    void (*unkC)(void); /* leave */
} Ovl15Mode;
extern Ovl15Mode g_sceneModes[5];

typedef struct {
    u8 unk0; /* current mode, index into g_sceneModes */
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Ovl15ModeState;
extern Ovl15ModeState g_scene;

/* Per-mode screen state (g_bjAction, g_bjWagerMenu, g_endingMenu). */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3; /* done */
    u8 pad4[2];
    u8 unk6;
    u8 unk7;
    s32 unk8; /* frame counter */
    s32 unkC;
} Ovl15Screen;
extern Ovl15Screen g_bjAction;
extern Ovl15Screen g_bjWagerMenu;
extern Ovl15Screen g_endingMenu;

typedef struct {
    s16 x;
    s16 y;
} Ovl15Pos;
extern Ovl15Pos g_bjActionCursorPos[4];
extern Ovl15Pos g_bjWagerCursorPos[1];
extern Ovl15Pos g_endingMenuCursorPos[4];
extern void *g_endingData;

extern s8 g_endingSoundPending; /* sound request kind, -1 = none */
extern u16 g_movieFlags; /* flags */
extern s32 g_movieFrame; /* frame counter */
extern u16 g_movieFlagQueue[2][4]; /* queued flag sets / clears */
extern s32 g_bjBankroll; /* bankroll */
extern s16 D_800AE430;
extern u16 g_endingImage;
extern u8 g_bjCanDouble; /* double allowed */
extern s32 g_bjWager; /* bet */
extern u8 g_bjCanSplit; /* split allowed */
extern u32 g_theEndState;
extern SPRT g_theEndSprites[2][2];
extern DR_TPAGE g_theEndTpages[2][2];
extern s32 g_endingCodePos;
extern u32 g_endingSecretCode[16];
s32 ending_theEndInit(void);
s32 ending_theEndExit(void);
void ending_theEndClearOt(void);
void ending_theEndSwap(void);
extern s16 D_800AE4A8;
extern s32 g_messagesFrame;
extern u8 *g_messagesPrim; /* next free primitive */
extern u32 g_movieState; /* FMV state */
extern u32 g_endingState; /* main state */
extern s32 g_bjSprtCount;
extern s32 g_bjTpageCount;
extern u8 g_messagesPrims[2][0x2000]; /* primitive buffers */
extern u32 g_messagesState;
extern RECT g_bjClutRect;
extern u32 g_bjClut[8]; /* CLUT */
extern DrTpageWords g_bjTpage;
extern u32 g_bjState;
extern SPRT g_bjSprites[2][256];
extern DR_TPAGE g_bjTpages[2][16];
/* Pip positions on a card face, [rank][pip]; rank 0-8 (cards 2-10) has rank + 2 pips. */
typedef struct {
    s32 x;
    s32 y;
} Ovl15Pip;
extern Ovl15Pip g_bjPips[][10];

s32 bj_drawCardBack(s32, s32);
s32 bj_handInit(Ovl15Hand *, s16, s16);
s32 bj_drawCard(s32, s32, s32, s32);
void bj_actionInput(void);
void ending_movieClearOt(void);
void ending_movieSwap(void);
s32 ending_movieInit(s32, s32);
void ending_movieOnEnd(void);
s32 ending_playMovie(s32, s32);
s32 bj_main(void);
s32 ending_theEnd(void);
void ending_init(void);
void ending_exit(void);
void scene_init(void);
void scene_exit(void);
s32 ending_messages(void);
void bj_actionDraw(void);
void bj_actionTick(void);
void func_800AB3C4(void);
void func_800AB3CC(void);
void bj_actionDrawCursor(void);
void bj_actionDecide(void);
void bj_wagerDecide(void);
void ending_menuDecide(void);
void bj_actionStepFadeIn(void);
s32 ending_soundCmd(s32, s32, s32, s32, s32);
void ending_loadMusic(void);
void ending_loadData(void);
void scene_set(s32);
s32 scene_discIndex(s32);
u8 scene_isDiscGone(s32, s32);
u8 scene_getFadeScreen(void);
void bj_wagerInput(void);
void bj_wagerDraw(void);
void bj_wagerTick(void);
void func_800AD380(void);
void func_800AD388(void);
void bj_wagerDrawCursor(void);
void bj_wagerStepFadeIn(void);
void ending_menuInput(void);
void ending_menuDraw(void);
void ending_menuTick(void);
void func_800AD678(void);
void func_800AD680(void);
void func_800AD688(void);
void ending_menuDrawCursor(void);
void ending_menuStepFadeIn(void);
void ending_menuOnFadeOut(void);
void ending_menuStartFadeIn(void);
extern u8 g_endingMessages[]; /* message table */
s32 ending_messagesInit(void);
void ending_messagesClearOt(void);
void ending_messagesSwap(void);
u8 *ending_getMessage(u8 *, s32);
void *ending_drawText(s32, s32, u32 *, u8 *, void *);
s32 ending_textWidth(u8 *);
void *ending_drawGlyph(s32, s32, u32 *, u32, s32 *, SprtWords *);
s32 bj_splitHand(Ovl15Hand *, Ovl15Hand *);
s32 bj_isBlackjack(Ovl15Hand *);

/* bj_main's format strings and UI labels (INCLUDE_RODATA, never C literals) */
extern const char g_bjFmt2d[]; /* "%2d" */
extern const char g_bjStrBankroll[]; /* "Bankroll" */
extern const char g_bjFmtLong[]; /* "%ld" */
extern const char g_bjStrWager[]; /* "Wager" */
extern const char g_bjStrStand[]; /* "Stand" */
extern const char g_bjStrHit[]; /* "Hit" */
extern const char g_bjStrDouble[]; /* "Double" */
extern const char g_bjStrSplit[]; /* "Split" */

/* used by bj_main */
extern Ovl15Shoe g_bjShoe;
s32 bj_shoeInit(Ovl15Shoe *);
s32 bj_shoeShuffle(Ovl15Shoe *);
s32 bj_dealCard(Ovl15Hand *, Ovl15Shoe *);
s32 bj_handOptions(Ovl15Hand *);
s32 bj_drawHand(Ovl15Hand *, s32, s32, s32);
void scene_update(void);
s32 bj_drawResult(s32, s32, s32, s32);
void bj_clearOt(void);
void bj_swapBuffers(void);
s32 bj_exit(void);
s32 bj_init(void);
s32 bj_drawShoe(s32, s32);
/* bj_actionGetCursor and scene_get (u8, defined later in ovl_15.c) stay undeclared for
 * bj_main: it calls them as implicit int (no andi 0xFF on the result). */

/* Random 0..2 from the main exe's random byte stream. */
#define RND3() (({ s32 r_; if (g_randPtr >= g_randTableEnd) { rand_refill(); } r_ = *g_randPtr++; r_; }) % 3)

#endif
