#ifndef OVL_0D_H
#define OVL_0D_H

/* Declarations local to overlay 0x0D (Tetra Master card game, loaded at
 * 0x800A7000). Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* ---- Tetra Master ---- */

/* Tasks (TmState.task), named by the debug strings tm_mainLoop passes to tm_debugPrint. */
enum {
    TM_TASK_PLAYER_SELECT, /* "PLAYER CARD SELECT", "PLAYER AUTO SELECT" with TmState.autoPlay */
    TM_TASK_PLAYER_SET,    /* "PLAYER CARD SET" */
    TM_TASK_ENEMY_SELECT,  /* "ENEMY CARD SELECT" */
    TM_TASK_RESULT,        /* "RESULT" */
    TM_TASK_INIT,          /* "INIT" */
    TM_TASK_FLASH,         /* "FLASH" */
    TM_TASK_BATTLE_SELECT, /* "BATTLE SELECT" */
    TM_TASK_BATTLE,        /* "BATTLE" */
    TM_TASK_TURN,          /* "TURN" */
    TM_TASK_BATTLE_WIN,    /* "BATTLE WIN" */
    TM_TASK_BATTLE_LOSE,   /* "BATTLE LOSE" */
    TM_TASK_TRY_AGAIN,     /* "TRY AGAIN" */
    TM_TASK_EXIT,          /* no handler, ends tm_mainLoop */
    TM_TASK_CARD_CHANGE,   /* "CARD_CHANGE" */
    TM_TASK_CARD_MOVE,     /* "CARD_MOVE" */
    TM_TASK_FLASH2,        /* "FLASH2" */
    TM_TASK_TRADE,         /* "TRADE" */
    TM_TASK_CHANGE2,       /* "CHANGE2" */
    TM_TASK_CHOICE,        /* "CHOICE" */
    TM_TASK_CHOICE2,       /* "CHOICE2" */
    TM_TASK_STAGE_SELECT,  /* "STAGE SELECT" */
    TM_TASK_FIELD_FADE,    /* "FIELD FADE" */
    TM_TASK_AWAY_INFO,     /* "AWAY INFO" */
    TM_TASK_CARD_AWAY,     /* "CARD AWAY" */
    TM_TASK_CARD_AWAY2,    /* "CARD AWAY2" */
    TM_TASK_CARD_LESS5,    /* "CARD LESS5" */
    TM_TASK_EXTENSION,     /* "EXTENSION" */
    TM_TASK_END_FADE,      /* "END FADE" */
    TM_TASK_COIN,          /* "COIN" */
    TM_TASK_FLASH_ARROW,   /* "FLASH ARROW" */
    TM_TASK_ENEMY_THINK,   /* "ENEMY THINK" */
    TM_TASK_ENEMY_APPEAR = 33, /* "ENEMY APEAR" */
    TM_TASK_GET_SELECT,    /* "GET SELECT" */
    TM_TASK_GAME_QUIT      /* "GAME QUIT" */
};

/* TmCard.owner, TmState.turn ("TURN PLAYER" / "TURN ENEMY"), the side argument of the tm_count* helpers */
#define TM_PLAYER 0
#define TM_ENEMY  1

/* TmState.result (tm_checkGameEnd) */
enum { TM_RESULT_WIN, TM_RESULT_LOSE, TM_RESULT_DRAW };

#define TM_HAND_SIZE 5 /* cards 0-4 the player's, 5-9 the enemy's */
#define TM_NUM_CARDS 10

/* Board on screen: square x, y is at (x * TM_SQUARE_W + TM_BOARD_X, y * TM_SQUARE_H + TM_BOARD_Y). */
#define TM_SQUARE_W 43
#define TM_SQUARE_H 52
#define TM_BOARD_X  0x48
#define TM_BOARD_Y  8
#define TM_CARD_W   0x2A
#define TM_CARD_H   0x33
#define TM_PLAYER_HAND_X 0x104 /* x of the player's hand column, 0x108 when the hand is small */
#define TM_ENEMY_HAND_X  0x10

/* Arrow directions (TmCard.arrows bit, tm_stepDir), clockwise from up */
enum { TM_DIR_UP, TM_DIR_UP_RIGHT, TM_DIR_RIGHT, TM_DIR_DOWN_RIGHT, TM_DIR_DOWN, TM_DIR_DOWN_LEFT, TM_DIR_LEFT, TM_DIR_UP_LEFT };

/* Opponents 0x7C-0x7F (TmState.opponent): no "Cancel game" in the stock, no quitting, no rematch, and a draw
 * plays again (EXTENSION). */
#define TM_OPP_FORCED 0x7C

/* tm_playSe / tm_stopSe index into g_tmSeIds */
enum {
    TM_SE_CURSOR = 1, /* cursor moves */
    TM_SE_BUZZER,     /* refused: square taken, no card to battle, too few cards, stock full */
    TM_SE_CANCEL,     /* Circle backs out */
    TM_SE_RECEIVED,   /* a card is won (CARD_CHANGE) */
    TM_SE_CARD,       /* card placed, picked, dealt or scrolled */
    TM_SE_COIN,       /* coin toss, stopped by tm_stopSe when it lands */
    TM_SE_BLOCK,      /* a block appears */
    TM_SE_PERFECT,    /* win with all 10 cards */
    TM_SE_FLASH,      /* cards flip */
    TM_SE_COMBO,
    TM_SE_BATTLE,
    TM_SE_LOST,       /* a card is lost (CARD_CHANGE) */
    TM_SE_WIN,
    TM_SE_LOSE,
    TM_SE_DRAW
};

/* g_tmText ids (decoded from the text bank) */
#define TM_TEXT_REMATCH         2    /* "Play a rematch? Yes/No" */
#define TM_TEXT_RECEIVED        3    /* " Received " */
#define TM_TEXT_LOST            4    /* " Lost " */
#define TM_TEXT_CONFIRM         6    /* "Confirm selection? Yes/No" */
#define TM_TEXT_TRADE_END       7    /* "" after the card name */
#define TM_TEXT_SELECT_CARDS    0x13 /* "Select cards" */
#define TM_TEXT_OVER_100        0x16 /* "You have over 100 cards. Select card to discard." */
#define TM_TEXT_DISCARD         0x17 /* "Discard " */
#define TM_TEXT_DISCARD_ASK     0x18 /* "Discard? Yes/No" */
#define TM_TEXT_DRAW            0x19 /* "The game ended in a draw. Another game will begin." */
#define TM_TEXT_LESS_5          0x1A /* "You don't have 5 cards. End card game." */
#define TM_TEXT_ICON_R1         0x1C /* button icon next to the card name and copy count */
#define TM_TEXT_ICON_L1         0x1E
#define TM_HELP_SELECT          0x21 /* "Select cards to use. select with .. confirm with .." */
#define TM_HELP_PLACE           0x22 /* "Select where to place the card. .." */
#define TM_HELP_BATTLE          0x23 /* "Select a card to battle. .." */
#define TM_HELP_STOCK           0x24 /* "Select 5 cards to be used in the card game. Cancel game with .." */
#define TM_HELP_BEGIN           0x25 /* "Begin game with cards currently selected." */
#define TM_HELP_RESELECT        0x26 /* "Redo card selection." */
#define TM_HELP_DISCARD_PICK    0x27 /* "Select card to discard." */
#define TM_HELP_END_GAME        0x28 /* "End card game and continue the story." */
#define TM_HELP_PLAY_AGAIN      0x29 /* "Play another card game." */
#define TM_HELP_DISCARD_YES     0x2A /* "Discards selected card. .." */
#define TM_HELP_DISCARD_NO      0x2B /* "Reselect card to discard. .." */
#define TM_HELP_TAKE            0x2C /* "Select card to take." */
#define TM_TEXT_QUIT_ASK        0x2D /* "End card game? Yes/No" */
#define TM_HELP_QUIT_YES        0x2E /* "End card game." */
#define TM_HELP_QUIT_NO         0x2F /* "Continue card game." */
#define TM_HELP_PLACE_BLOCKED   0x30 /* "Select placement. You cannot place your card here. .." */
#define TM_HELP_NO_BATTLE       0x31 /* "Select card to battle. There is no card to battle in this position .." */
#define TM_HELP_STOCK_FORCED    0x32 /* "Select 5 cards to be used in the game." */
#define TM_TEXT_STOCK           0x33 /* "Stock: " */
#define TM_TEXT_STOCK_END       0x34 /* "" after the count */

/* Card types (TmCard.type): what the power is tested against (tm_getBattleValues) and the
 * letter tm_drawCardStats shows. */
enum {
    TM_TYPE_P, /* "P": power against pdef */
    TM_TYPE_M, /* "M": power against mdef */
    TM_TYPE_X, /* "X": power against the lower defence */
    TM_TYPE_A  /* "A": the highest own stat against the lowest of the target */
};

/* A card in a hand or on the board. Stats are hex digits on screen (value >> 4). */
typedef struct {
    u8 id;     /* 0-99 */
    u8 arrows; /* bit d = arrow in direction d (TM_DIR_*) */
    u8 power;  /* "AP" in tm_levelUpStat's printf */
    u8 type;   /* TM_TYPE_* */
    u8 pdef;   /* "PD" */
    u8 mdef;   /* "MD" */
    u8 pad6[2];
    s32 hidden;  /* face down (enemy hand), drawn as the card back */
    s32 x;
    s32 y;
    s32 owner;   /* TM_PLAYER / TM_ENEMY, the card's colour */
    s32 onBoard;
    s32 small;   /* drawn shrunk (POLY_FT4), the compact player hand */
    s32 fromX;   /* move animation start, 24.8 fixed point */
    s32 fromY;
    s32 toX;     /* move animation target, 24.8 fixed point */
    s32 toY;
    s32 depth;   /* ordering table index */
    s32 visible;
    u8 pad38[0x3C - 0x38];
} TmCard; /* 0x3C */

/* TmCell.state */
enum {
    TM_CELL_CARD,    /* a card lies here (TmCell.card) */
    TM_CELL_EMPTY,
    TM_CELL_BLOCK,   /* block stone (tm_placeBlocks) */
    TM_CELL_OFFBOARD /* g_tmOffBoardCell */
};

typedef struct {
    s32 state;    /* TM_CELL_* */
    TmCard *card;
} TmCell; /* board square */

/* TmFlip.mode, TmState.flashMode */
enum {
    TM_FLIP_NONE,
    TM_FLIP_CHANGE, /* card changes owner (unopposed capture, or changing hands after the game) */
    TM_FLIP_COMBO,  /* combo from a battle winner, arg = direction */
    TM_FLIP_BATTLE  /* the battle loser blinks */
};

typedef struct {
    s32 mode; /* TM_FLIP_* */
    s32 x;    /* board square */
    s32 y;
    s32 arg;  /* TM_FLIP_CHANGE: new owner, TM_FLIP_COMBO: direction */
    TmCard *card;
} TmFlip; /* a card flash / flip entry (TmState.flips) */

typedef struct {
    s32 hidden; /* not shown yet (tm_revealBlocks clears it) */
    s32 x;      /* screen position */
    s32 y;
    s32 style;  /* one of two block pictures */
} TmBlock; /* block stone on the board */

typedef struct {
    s32 x;
    s32 y;
} TmPos; /* board square */

typedef struct {
    s32 atk;
    s32 def;
} TmBattle; /* tm_getBattleValues */

typedef struct {
    u8 power;
    u8 type;
    u8 pdef;
    u8 mdef;
    u8 unk4;
} TmCardData; /* g_tmCardData entry, base stats of a card id */

typedef struct {
    u8 ids[16]; /* card id by rarity roll, rarest last (tm_dealEnemyHand) */
} TmDeck; /* g_tmDecks entry */

typedef struct {
    u8 deck;    /* index into g_tmDecks */
    u8 aiLevel; /* TmState.aiLevel */
} TmOpponent; /* g_tmOpponents entry */

/* Image file (file 0x44C of directory 1, tm_loadImageFile): a table of {sector offset, ?} entries, then the
 * entries. Its RAM is reused as the two primitive buffers once tm_setup has copied everything out. */
#define TM_IMAGE_FILE     0x80104A00
#define TM_PRIM_BUF       0x80104A00
#define TM_PRIM_BUF_SIZE  0x4B000
#define TM_FILE_CARDS     0 /* the 100 card picture TIMs, 0xA80 bytes each */
#define TM_FILE_CARD_DATA 1 /* -> g_tmCardData */
#define TM_FILE_DECKS     2 /* -> g_tmDecks */
#define TM_FILE_OPPONENTS 3 /* -> g_tmOpponents */
#define TM_FILE_BG        4 /* background, VRAM 0x280 (tm_drawBackground's first 256 columns) */
#define TM_FILE_BOARD     5 /* board picture, VRAM 0x340 (tm_drawBoard) */
#define TM_FILE_BATTLE    9 /* battle animation frames, VRAM 0x200,0x100 (tm_taskBattle) */
#define TM_FILE_BG_RIGHT  0xB /* background right of x 256, VRAM 0x300 */

/* Tables tm_setup copies out of the image file, at fixed addresses. */
#define g_tmCardData  ((TmCardData *)0x80103800) /* [100] */
#define g_tmDecks     ((TmDeck *)0x80103C00)     /* [64] */
#define g_tmOpponents ((TmOpponent *)0x80104000) /* [256] */

/* Card stats as kept in the card stock (main's Unk6) */
typedef struct {
    u8 id;
    u8 arrows;
    u8 power;
    u8 type;
    u8 pdef;
    u8 mdef;
} TmCardStats;

/* Game state, a local of tm_mainLoop passed to every handler. */
typedef struct {
    s32 task;     /* TM_TASK_*, the handler tm_mainLoop runs this frame */
    s32 cursor;   /* hand card index (PLAYER CARD SELECT), else the Yes (0) / No (1) choice */
    s32 boardX;   /* board cursor (PLAYER CARD SET, BATTLE SELECT) */
    s32 boardY;
    TmCard cards[10]; /* 0-4 player hand, 5-9 enemy hand */
    TmCell board[4][4]; /* [y][x] */
    s32 timer;    /* frames in the current task, reset by most task changes */
    u8 brightness;
    u8 pad2ED[3];
    TmBlock blocks[6];
    TmFlip flips[10];
    s32 phase;    /* sub-step of the current task or animation */
    s32 flipOwner; /* owner the flipped cards get */
    s32 flashNext; /* task after the flash, -1 = tm_checkBattles again */
    s32 srcX;     /* square of the card just placed (the attacker) */
    s32 srcY;
    s32 dstX;     /* square of the card it battles */
    s32 dstY;
    s32 turn;     /* 0 player, 1 enemy ("TURN PLAYER" / "TURN ENEMY") */
    s32 autoPlay; /* "PLAYER AUTO SELECT" instead of "PLAYER CARD SELECT" */
    s32 wins;
    s32 losses;
    s32 draws;
    s32 deck;     /* opponent's deck, index into g_tmDecks */
    s32 aiLevel;  /* opponent's AI level 0-3 (tm_enemySelect) */
    s32 opponent; /* opponent id from GameData.unk8B2, index into g_tmOpponents */
    s32 unk454;
    Window *msgWin; /* confirm / rematch / trade window */
    s32 result;   /* TM_RESULT_* */
    s32 moveFrames; /* length of the card move animations */
    s32 showScore;  /* tm_drawScore */
    s32 showBoard;  /* tm_drawBoard */
    s32 showBlocks; /* tm_drawBlocks */
    TmCard *tradeCard; /* card changing hands (CARD_CHANGE, CHANGE2) */
    s32 stockCol; /* stock grid cursor, card id = col * 10 + row */
    s32 stockRow;
    s32 handCount; /* cards picked from the stock into the hand */
    s32 animFrame; /* cursor animation frame */
    Window *quitWin;
    s32 savedFrame; /* animFrame of the hand cursor kept while placing */
    TmCard *targets[8]; /* cards the placed card can battle (BATTLE SELECT) */
    s32 numTargets;
    TmCard *attacker;
    s32 copyIdx;  /* which copy of the card under the stock cursor */
    Window *infoWin; /* over-100 / discard / draw windows */
    s32 unk4BC;
    s32 unk4C0;
    Window *less5Win;
    s32 arrowOdds[9]; /* percent chance of a card having 0-8 arrows (tm_makeCard) */
    s32 oddsRow;  /* arrowOdds entry edited in STAGE SELECT */
    s32 comboTimer; /* frames left of the combo label (tm_drawCombo) */
    s32 comboX;
    s32 comboY;
    s32 comboShown; /* number the combo label shows */
    s32 comboCount;
    s32 flashMode; /* TM_FLIP_* of the current flash */
    TmCard *comboCard; /* card whose arrows blink in the combo flash */
    u8 comboDirs; /* arrow bits of the combo */
    u8 pad50D[0x510 - 0x50D];
    TmCardStats lostCard; /* card the enemy won last game, dealt back at lostSlot */
    u8 pad516[0x54C - 0x516];
    s32 hasLostCard;
    s32 lostSlot;
    s32 unk554;
    u8 savedRepeat[12]; /* key-repeat flags of 12 buttons (pad_isRepeatEnabled), restored on exit */
    s32 nameSlide;  /* R1 card name slide-out (PLAYER CARD SELECT) */
    s32 scrollTimer; /* L1/R1 copy scroll animation in the stock */
    s32 blockIdx;   /* block appearing now (ENEMY APEAR) */
    s32 numBlocks;
    s32 atkShown;   /* battle values tm_drawBattleValues shows */
    s32 defShown;
    s32 valuesTimer; /* frames left of the battle values */
    u8 pad580[0x590 - 0x580];
    s32 landed[5];  /* enemy card has dropped in (ENEMY APEAR), plays its sound once */
    s32 thinkDelay; /* ENEMY THINK frames */
    s32 blockTimer;
    s32 blocksDone;
    s32 getCursor;  /* GET SELECT cursor over the captured enemy cards */
    s32 quitChoice; /* GAME QUIT Yes (0) / No (1) */
    s32 quitting;   /* game quit from the stock, hand cards were returned */
    s32 atkRoll;    /* battle rolls, the loser is the side with the lower one */
    s32 defRoll;
    s32 unk5C4;
    RECT unk5C8;
} TmState;

typedef struct {
    u8 v[8];
} TmDirMask; /* card arrow bit for each direction */
extern TmDirMask g_tmArrowBits; /* single-bit masks, one per arrow */
extern TmDirMask g_tmBackArrowBits;

typedef struct {
    s32 x;      /* offset from the card */
    s32 y;
    s32 smallX; /* offset from a small card (TmCard.small) */
    s32 smallY;
    s32 u;
    s32 v;
} TmArrow;

typedef struct {
    TmArrow v[8];
} TmArrowTbl; /* arrow sprite for each direction */
extern TmArrowTbl g_tmArrowSprites;

typedef struct {
    s32 x; /* offset */
    s32 u;
    s32 v;
    s32 w;
} TmCoinFrame;

typedef struct {
    TmCoinFrame v[8];
} TmCoinFrames; /* coin toss animation frames */
extern TmCoinFrames g_tmCoinFrames;

typedef struct {
    u32 id;
    s32 flags;    /* bpp in the low bits */
    u32 clutLen;  /* clut block length */
    s16 clutRect[4];
    u32 clut[1];
} TmTim; /* TIM image */

typedef struct {
    u32 len;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    u32 data[1];
} TmTimImage; /* TIM pixel block */

extern s32 g_tmDataFile;
extern s8 D_800B3664; /* sound request kind, -1 = none */
extern u8 g_tmText[];
extern s32 g_tmDataCopy;
extern s32 g_tmSeIds[];
extern s32 g_tmWindowOpening;
extern s32 g_tmTradeCardId;
extern s32 g_tmTradeBadge;
extern s32 g_tmTradeLost;
extern TmCell g_tmOffBoardCell; /* returned for out-of-range board squares */
extern s32 g_tmStockCount;
extern s32 g_tmStockTypes;

s32 tm_loadSoundBlocks(void);
s32 tm_loadMusic(void);
s32 tm_loadDataFile(void);
s32 tm_copyDataFile(void);
s32 tm_freeDataFile(void);
s32 tm_freeDataCopy(void);
u8 *tm_getFileEntry(s32);
void tm_checkBattles(TmState *);
void tm_levelUpStat(TmState *, TmCard *, s32);
void tm_levelUpRandom(TmState *, TmCard *);
void tm_makeCard(TmState *, TmCard *, s32, s32);
void tm_placeBlocks(TmState *);
s32 tm_evalPlacement(TmState *, s32, s32, s32, TmCard *);
TmCard *tm_getCardAt(TmState *, s32, s32);
void tm_stackHands(TmState *, s32);
void tm_nextTrade(TmState *);
void tm_startCardMove(TmState *);
void tm_returnHand(TmState *);
void tm_returnKept(TmState *);
void tm_hideCards(TmState *);
void tm_askRematch(TmState *);
void tm_drawDrawWin(Window *);
void tm_drawOver100Win(Window *);
void tm_drawLess5Win(Window *);
void *tm_drawCursor(u32 *, void *, TmState *, s32, s32, s32);
void *tm_drawBigDigit(u32 *, SPRT *, s32, s32, s32);
s32 tm_setFlips(TmState *, s32, s32, s32, s32, s32);
void tm_startEndFade(TmState *);
void tm_init(void);
void tm_exit(void);
s32 tm_soundCmd(s32, s32, s32, s32, s32);
void tm_mainLoop(void);
void tm_onWindowOpened(void);
void tm_setWindowOpening(s32);
void tm_endTrade(TmState *);
void tm_playSe(s32);
TmCell *tm_getCell(TmState *, s32, s32);
TmCard *tm_getDefender(TmState *, TmPos *, s32, s32);
void tm_dealEnemyHand(TmState *);
void tm_stepDir(TmPos *, s32);
s32 tm_countInHand(TmState *, s32);
s32 tm_countPlayerHand(TmState *, s32);
s32 tm_countCaptured(TmState *);
TmCard *tm_getCaptured(TmState *, s32);
void tm_clearFlips(TmState *);
s32 tm_countOwned(TmState *, s32);
s32 tm_countKept(TmState *, s32);
s32 tm_countOnBoard(TmState *, s32);
TmCard *tm_getHandCard(TmState *, s32, s32);
void tm_initHands(TmState *);
void tm_initBoard(TmState *);
void tm_openStock(TmState *);
void tm_resetStockCursor(TmState *);
void tm_waitWindow(Window *);
void tm_drawRematchWin(Window *);
SPRT *tm_drawStatGlyph(void *, SPRT *, s32, s32, s32, s32, s32);
void tm_drawTradeWin(Window *);

/* Main loop (tm_mainLoop) and its per-state handlers. */
void tm_clearOt(void);
void tm_endFrame(TmState *);
void tm_setup(TmState *);
void tm_debugPrint(TmState *, char *); /* debug print, stubbed */
void tm_updatePad(TmState *);
void tm_taskInit(TmState *);
void tm_loadImageFile(void);
void tm_copyFileEntry(u8 *, s32, s32);
void tm_loadTim(TmTim *, s32, s32, s32, s32);
void *tm_drawScore(u32 *, SPRT *, TmState *);
void *tm_drawBackground(u32 *, SPRT_16 *, TmState *);
void *tm_drawBoard(u32 *, SPRT_16 *, TmState *);
void *tm_taskPlayerSelect(u32 *, void *, TmState *);
void *tm_taskPlayerSet(u32 *, void *, TmState *);
void *tm_taskBattleSelect(u32 *, void *, TmState *);
void *tm_taskGetSelect(u32 *, void *, TmState *);
void *tm_drawBattleSelect(u32 *, void *, TmState *);
void *tm_taskBattle(u32 *, void *, TmState *);
void *tm_drawFlash(u32 *, TILE *, TmState *);
void *tm_drawFlash2(u32 *, TILE *, TmState *);
void *tm_drawCards(u32 *, void *, TmState *);
void *tm_taskCardMove(u32 *, void *, TmState *);
void *tm_taskCardChange(u32 *, void *, TmState *);
void *tm_drawTradeBadge(u32 *, SPRT *, TmState *);
void *tm_taskChange2(u32 *, void *, TmState *);
void *tm_drawResult(u32 *, SPRT *, TmState *);
void *tm_drawBlocks(u32 *, void *, TmState *);
void *tm_taskEnemyThink(u32 *, void *, TmState *);
void *tm_drawStock(u32 *, SPRT *, TmState *);
void *tm_drawCardStats(u32 *, SPRT *, TmState *, TmCard *, u8);
void *tm_drawArrows(u32 *, SPRT_16 *, TmState *, s32, s32, u8, s32, u8, s32);
void *tm_drawCardFace(u32 *, void *, TmState *, TmCard *, u8, s32);
void *tm_taskCoin(u32 *, SPRT *, TmState *);
void *tm_drawCombo(u32 *, SPRT *, TmState *);
void *tm_drawBattleValues(u32 *, void *, TmState *);
void *tm_drawTryAgain(u32 *, void *, TmState *);
void tm_taskResult(TmState *);
void tm_taskExtension(TmState *);
void tm_autoPlace(TmState *);
void tm_taskTrade(TmState *);
void tm_taskTryAgain(TmState *);
void tm_stockInput(TmState *);
void tm_taskChoice2(TmState *);
void tm_taskStageSelect(TmState *);
void tm_taskAwayInfo(TmState *);
void tm_taskCardAway2(TmState *);
void tm_taskCardLess5(TmState *);
void tm_taskGameQuit(TmState *);
void tm_taskBattleWin(TmState *);
void tm_taskBattleLose(TmState *);
void tm_taskTurn(TmState *);
void tm_taskFlashArrow(TmState *);
void tm_layoutCards(TmState *);
void tm_showStockCard(TmState *);
void tm_taskEnemyAppear(TmState *);
void tm_animEnemyThink(TmState *);
void tm_checkGameEnd(TmState *);
void tm_revealBlocks(TmState *);
void tm_enemySelect(TmState *);
void tm_taskEndFade(TmState *);

void tm_getBattleValues(s32, TmBattle *, TmCard *, TmCard *);
TmCard *tm_findDefender(TmState *, s32, s32, TmCard *);
void tm_drawConfirmWin(Window *);
void tm_drawDiscardWin(Window *);
void tm_drawQuitWin(Window *);
s32 tm_getStockCursorId(TmState *);
void tm_setHelpArea(s16);
extern u8 g_tmSlash[];

/* main exe: candidates for ff9.h */
void help_nullHandler(void); /* empty function, passed to help_setCallback as a no-op callback */
s32 text_measureDigits(u8 *);
void game_updateWindows(void);
void SetSprt16(SPRT_16 *);
s32 card_getNameWidth(s32); /* text width of a card name (text_measureWidth of card_getName) */
s32 card_getWins(void);
s32 card_getLosses(void);
s32 card_getDraws(void);
u8 pad_isRepeatEnabled(s32, s32);
void card_clearAll();
extern u8 *g_menuHelpText; /* = g_menuHelp.text, referenced by its own name */
s32 akao_playMusic(void *);
void akao_stopMusic(s32);
void akao_playSe(s32, s32, s32, s32);
void akao_setAllSeVolume(s32);
void akao_fadeAllSeVolume(s32, s32);
void akao_setSeVolume(s32, s32, s32);
void akao_fadeSeVolume(s32, s32, s32, s32);
void akao_sendCmdAA(s32);
void akao_sendCmdAB(s32, s32);
void akao_sendCmdA2(s32, s32, s32);
void akao_sendCmdA3(s32, s32, s32, s32);
void akao_setMusicVolume(s32, s32);
void akao_fadeMusicVolume(s32, s32, s32);
void akao_fadeMusicVolumeFrom(s32, s32, s32, s32);


/* Capture flash bar pieces (tm_drawFlash), one per direction. */
typedef struct {
    s32 shape;   /* 0: horizontal bar, 1: corner, 2: vertical bar */
    s32 dirX;    /* -1/0/1: direction the bar sweeps in */
    s32 dirY;
    s32 cornerX; /* corner: side and direction of its vertical piece */
    s32 cornerY;
    s32 offX;    /* start offset from the square */
    s32 offY;
} TmFlash;

typedef struct {
    TmFlash v[8];
} TmFlashTbl;
extern TmFlashTbl g_tmFlashBars; /* INCLUDE_RODATA */

extern char g_tmNoLabel[]; /* "NO" label (INCLUDE_RODATA) */

#endif
