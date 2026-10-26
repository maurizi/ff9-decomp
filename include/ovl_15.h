#ifndef OVL_15_H
#define OVL_15_H

/* Declarations local to overlay 0x15 (the ending: last movie, "The End" screen and the Blackjack
 * minigame its secret code opens; loaded at 0x800A7000). Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#endif
#define SE_GAME_START 0xC18 /* the secret code accepted (ovl_14: New Game on the title) */
/* g_gameState->disc: one bit per disc of the game found in the drive (bit 0 = disc 1), 0xFF = no disc read.
 * The ending plays its movie only with disc 4 in (see ovl_14.h). */
#define DISC_NONE 0xFF
#define DISC_1    1
#define DISC_4    8
void akao_stopMusic(s32);
s32 text_measureSmall(u8 *); /* text width */
void file_setTimPos(s32, s32, s32, s32, s32);
s32 cd_findFile(char *);
u8 cd_seekStreamAsync(s32);
s32 akao_playMusic(void *);
s32 akao_playSe(s32, s32, s32, s32);
void akao_setAllSeVolume(s32);
void akao_fadeAllSeVolume(s32, s32);
void akao_setSeVolume(s32, s32, s32);
void akao_fadeSeVolume(s32, s32, s32, s32);
void akao_sendCmdAA(s32);
void akao_sendCmdAB(s32, s32);
void akao_sendCmdA2(s32, s32, s32);
void akao_sendCmdA3(s32, s32, s32, s32);
void akao_sendCmdAC(s32);
void akao_sendCmdAD(s32, s32);
void akao_sendCmdA4(s32, s32, s32);
void akao_sendCmdA5(s32, s32, s32, s32);
void akao_setMusicVolume(s32, s32);
void akao_fadeMusicVolume(s32, s32, s32);
void akao_fadeMusicVolumeFrom(s32, s32, s32, s32);

/* ovl_64 (FMV player, 0x80198700) */
s32 MBG_init(s32, u8 **);
s32 mbg_play(u32, s32);
s32 mbg_update(s32);
s32 mbg_setRgb24(s32);
s32 mbg_shutdown(void);
void mbg_setEndCallback(void (*)(void));
s32 mbg_getLastFrame(void);
s32 mbg_streamSetBaseSector(s32);
/* movie table (ovl_64's g_mbgMovieLists): list 4 is the disc 4 movies */
typedef struct {
    char *path; /* movie file name */
    s32 pad4[4];
} MbgMovie; /* 0x14 bytes */
typedef struct {
    s16 count;
    s16 pad2;
    MbgMovie *movies;
} MbgMovieList;
extern MbgMovieList g_mbgMovieLists[];

/* ---- this overlay ---- */

#define SCREEN_OTZ 8 /* OT slot of the end image and the message text */
#define BJ_OTZ 10     /* OT slot of all Blackjack sprites */

/* g_endingState: the states of ending_main */
enum {
    ENDING_MOVIE,     /* FMV059 */
    ENDING_MESSAGES,  /* never reached: ending_messages, then FMV060 */
    ENDING_MOVIE2,
    ENDING_THE_END,   /* the end screen, until the secret code */
    ENDING_BLACKJACK,
    ENDING_DONE       /* never reached */
};
#define ENDING_MOVIE_LIST 4    /* ovl_64 movie list of disc 4 */
#define ENDING_FMV059     0x12 /* its entries: \ENDING\FMV059.STR, FMV060.STR */
#define ENDING_FMV060     0x13

/* g_movieFlags (ending_playMovie), the bits of ovl_14's opening movie loop */
#define MOVIE_NO_SCENES 0x1
#define MOVIE_NO_FADE   0x2  /* don't run the screen fade (game_updateFade) */
#define MOVIE_NO_OT     0x4  /* ending_movieSwap only puts the draw env, the decoder draws */
#define MOVIE_PLAYING   0x10
#define MOVIE_DONE      0x80 /* movie over (ending_movieOnEnd, or no movie) */
/* g_movieState */
enum { MOVIE_ST_START, MOVIE_ST_OPEN, MOVIE_ST_PLAY, MOVIE_ST_END, MOVIE_ST_DONE };

/* db ids of the music the ending plays through ending_soundCmd, and of its sound bank (ending_loadMusic) */
#define ENDING_MUSIC      0x7009C
#define ENDING_MUSIC_BANK 0x901F3

/* g_theEndState: ending_theEnd */
enum {
    THEEND_ST_START,
    THEEND_ST_SHOW,     /* the image, waiting for the code */
    THEEND_ST_FADE,     /* code entered and Start held: fade out */
    THEEND_ST_UNUSED,   /* never set; would make ending_theEnd return 0 */
    THEEND_ST_BLACKJACK
};
#define ENDING_CODE_LEN 16 /* buttons in g_endingSecretCode */

/* g_messagesState: ending_messages shows its pages with a pause after each */
enum {
    MESSAGES_ST_START,
    MESSAGES_ST_PAGE1, /* message 0 */
    MESSAGES_ST_PAGE2, /* messages 1 and 2 */
    MESSAGES_ST_PAGE3, /* message 3 */
    MESSAGES_ST_PAGE4, /* messages 4 and 5, then MESSAGES_ST_DONE */
    MESSAGES_ST_PAUSE, /* then the next page */
    MESSAGES_ST_DONE
};

/* Module W ("scene" table, the same source as ovl_14's): one SceneMode per scene id. */
typedef struct {
    void (*init)(void);   /* scene_init calls all of them */
    void (*enter)(void);
    void (*update)(void); /* scene_update, every frame */
    void (*leave)(void);
} SceneMode;

/* scene ids of the ending, index into g_sceneModes */
enum {
    SCENE_NONE,
    SCENE_MENU,      /* ending_menu*: never entered */
    SCENE_BJ_WAGER,  /* set the wager */
    SCENE_BJ_ACTION, /* Stand / Hit / Double / Split */
    SCENE_CHOSEN,    /* no scene: the menus set it when Cross picked, bj_main waits for it */
    SCENE_COUNT
};
extern SceneMode g_sceneModes[SCENE_COUNT];

typedef struct {
    u8 cur; /* scene id */
    u8 unk1;
    u8 unk2;
    u8 fadeScreen; /* ending_menuStartFadeIn: 0 fades the menu in, 1 shows it and fades the screen in */
} SceneState;
extern SceneState g_scene;

/* State of a menu scene (g_bjAction, g_bjWagerMenu, g_endingMenu), one template. */
typedef struct {
    u8 unk0;
    u8 shown;    /* input and cursor on */
    u8 fadingIn; /* no input while set */
    u8 cursor;
    u8 pad4[2];
    u8 unk6;
    u8 unk7;
    s32 fadeFrames; /* the fade-in ends after SCENE_MENU_FADE_FRAMES */
    s32 unkC;
} SceneMenu;
#define SCENE_MENU_FADE_FRAMES 48
extern SceneMenu g_bjAction;
extern SceneMenu g_bjWagerMenu;
extern SceneMenu g_endingMenu;

/* ---- Blackjack ---- */

/* Cards are rank * 4 + suit. Ranks 0-8 are the 2 to the 10 (worth rank + 2), 9-11 the face cards
 * (bj_drawCard draws picture rank - 9), 12 the ace. */
#define BJ_RANK_JACK 9
#define BJ_RANK_ACE  12
#define BJ_ACE_VALUE 11 /* an ace counts 11, or BJ_ACE_LOW when that busts */
#define BJ_ACE_LOW   1
#define BJ_TEN_VALUE 10 /* face cards */
#define BJ_BLACKJACK 21
#define BJ_BUST      22 /* hand values from here are bust */
#define BJ_DEALER_STAND 17 /* the dealer draws below this */

#define BJ_DECKS     6
#define BJ_DECK_SIZE 52
#define BJ_SHOE_SIZE (BJ_DECKS * BJ_DECK_SIZE)
#define BJ_SHUFFLE_PASSES 5
#define BJ_CUT_CARD  249 /* dealing card 249 flags the shoe for the end of the session */

/* BjShoe.flags */
#define BJ_SHOE_FRESH     0x1 /* filled in order (bj_shoeInit) */
#define BJ_SHOE_SHUFFLED  0x2
#define BJ_SHOE_CUT       0x4 /* the cut card came out: bj_main ends after this round */

/* The card shoe. */
typedef struct {
    u16 flags; /* BJ_SHOE_* */
    u16 next;  /* next card to deal */
    u8 cards[BJ_SHOE_SIZE];
} BjShoe;

/* BjHand.flags */
#define BJ_HAND_SOFT       0x1  /* an ace counts BJ_ACE_VALUE */
#define BJ_HAND_BUST       0x2
#define BJ_HAND_DOUBLED    0x4  /* pays twice the wager, ends after one card */
#define BJ_HAND_SPLIT_ACES 0x8  /* ends after one card */
#define BJ_HAND_HOLE       0x10 /* the dealer's second card is drawn face down */

#define BJ_HAND_MAX 22

/* A blackjack hand. */
typedef struct {
    u16 flags;     /* BJ_HAND_* */
    u16 count;     /* cards */
    u16 value;     /* hand value */
    u16 hardValue; /* value with aces as 1 */
    s16 x;         /* screen position */
    s16 y;
    s32 cards[BJ_HAND_MAX];
} BjHand;

/* bj_handOptions */
#define BJ_CAN_SPLIT  0x1 /* a pair */
#define BJ_CAN_DOUBLE 0x2 /* two cards */

/* bj_actionGetCursor: the player's move */
enum { BJ_STAND, BJ_HIT, BJ_DOUBLE, BJ_SPLIT };

/* bj_drawResult banners */
enum { BJ_RESULT_WIN, BJ_RESULT_LOSE, BJ_RESULT_PUSH };
#define BJ_SE_LOSE 0x6B
#define BJ_SE_WIN  0x6C

#define BJ_MIN_BET  10 /* also the wager step; a bankroll below it ends the session */
#define BJ_MAX_BET  1000
#define BJ_BANKROLL 1000 /* g_bjBankroll's start, and the refill when it fell below BJ_MIN_BET */

/* Where cards are dealt from (bj_drawShoe) */
#define BJ_SHOE_X 0xF0
#define BJ_SHOE_Y 0xA
#define BJ_DEAL_FRAMES 30 /* frames a dealt card flies */
#define BJ_CARD_STEP 13 /* x step between the cards of a hand */
#define BJ_HAND_X    0x1E
#define BJ_PLAYER_Y  0x78
#define BJ_DEALER_Y  0xA
#define BJ_SPLIT1_Y  0x8C /* the player hand after a split */
#define BJ_SPLIT2_Y  0x60 /* the split hand */
#define BJ_SHAKE_FRAMES 10 /* BJ_ST_BUST shakes the hand this long */
#define BJ_RESULT_FRAMES 0x94

/* g_bjState: bj_main */
enum {
    BJ_ST_START,
    BJ_ST_SHUFFLE,  /* new shoe, shuffle animation */
    BJ_ST_BET,      /* SCENE_BJ_WAGER */
    BJ_ST_DEAL,     /* two cards each, then blackjack checks */
    BJ_ST_PLAYER,   /* SCENE_BJ_ACTION, the current hand */
    BJ_ST_DEALER,   /* the dealer draws to BJ_DEALER_STAND */
    BJ_ST_SETTLE_SPLIT, /* both split hands against the dealer */
    BJ_ST_BUST,     /* shakes the busted hand */
    BJ_ST_BLACKJACK, /* pays 3:2, or loses to the dealer's */
    BJ_ST_WIN,
    BJ_ST_PUSH,
    BJ_ST_LOSE,
    BJ_ST_NEXT,     /* next round, or BJ_ST_END when the cut card came out */
    BJ_ST_END,      /* fade out; Start or a bankroll below BJ_MIN_BET also get here */
    BJ_ST_DONE
};

typedef struct {
    s16 x;
    s16 y;
} ScreenPos;
extern ScreenPos g_bjActionCursorPos[4];
extern ScreenPos g_bjWagerCursorPos[1];
extern ScreenPos g_endingMenuCursorPos[4];
extern void *g_endingData;

extern s8 g_endingSoundPending; /* sound request kind, -1 = none */
extern u16 g_movieFlags; /* flags */
extern s32 g_movieFrame; /* frame counter */
extern u16 g_movieFlagQueue[2][4]; /* queued flag sets / clears */
extern s32 g_bjBankroll; /* bankroll */
extern s16 D_800AE430; /* cleared by ending_theEndInit, never read */
extern u16 g_endingImage;
extern u8 g_bjCanDouble; /* double allowed */
extern s32 g_bjWager; /* bet */
extern u8 g_bjCanSplit; /* split allowed */
extern u32 g_theEndState;
extern SPRT g_theEndSprites[2][2];
extern DR_TPAGE g_theEndTpages[2][2];
extern s32 g_endingCodePos;
extern u32 g_endingSecretCode[ENDING_CODE_LEN]; /* pad bits, in order */
s32 ending_theEndInit(void);
s32 ending_theEndExit(void);
void ending_theEndClearOt(void);
void ending_theEndSwap(void);
extern s16 D_800AE4A8; /* cleared by ending_messagesInit and ending_messagesSwap, never read */
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
} BjPip;
extern BjPip g_bjPips[][10];

s32 bj_drawCardBack(s32, s32);
s32 bj_handInit(BjHand *, s16, s16);
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
s32 bj_splitHand(BjHand *, BjHand *);
s32 bj_isBlackjack(BjHand *);

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
extern BjShoe g_bjShoe;
s32 bj_shoeInit(BjShoe *);
s32 bj_shoeShuffle(BjShoe *);
s32 bj_dealCard(BjHand *, BjShoe *);
s32 bj_handOptions(BjHand *);
s32 bj_drawHand(BjHand *, s32, s32, s32);
void scene_update(void);
s32 bj_drawResult(s32, s32, s32, s32);
void bj_clearOt(void);
void bj_swapBuffers(void);
s32 bj_exit(void);
s32 bj_init(void);
s32 bj_drawShoe(s32, s32);

/* Random 0..2 from the main exe's random byte stream. */
#define RND3() (({ s32 r_; if (g_randPtr >= g_randTableEnd) { rand_refill(); } r_ = *g_randPtr++; r_; }) % 3)

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
s32 bj_actionGetCursor(void);
s32 scene_get(void);

#endif
