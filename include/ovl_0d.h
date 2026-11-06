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
#define TM_BOARD_SIZE 4 /* the board is 4 x 4 squares (tm_getCell, TmState.board) */
#define TM_STOCK_SIDE 10 /* the stock screen shows the CARD_KINDS ids as a 10 x 10 grid (tm_stockInput) */
#define TM_MAX_BLOCKS 6 /* block stones per game (tm_placeBlocks, TmState.blocks) */

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
enum { TM_DIR_UP, TM_DIR_UP_RIGHT, TM_DIR_RIGHT, TM_DIR_DOWN_RIGHT, TM_DIR_DOWN, TM_DIR_DOWN_LEFT, TM_DIR_LEFT, TM_DIR_UP_LEFT,
       TM_NUM_DIRS };

/* Opponents 0x7C-0x7F (TmState.opponent): no "Cancel game" in the stock, no quitting, no rematch, and a draw
 * plays again (EXTENSION). */
#define TM_OPP_FORCED 0x7C
#define TM_OPP_FORCED_NUM 4

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

/* Card types (TmCard.type): TM_TYPE_* in ff9.h (CardEntry.type) */

/* A card in a hand or on the board. Stats are hex digits on screen (value >> 4). */
/* TmState.timer in TM_TASK_BATTLE (tm_taskBattle): the values show, then the clash sprite with TM_SE_BATTLE,
 * then both values roll down to the random results; at the end the flash shows the winner */
#define TM_FIGHT_CLASH 0x23
#define TM_FIGHT_ROLL 0x43 /* the rolls are drawn here; 23 frames of countdown follow */
#define TM_FIGHT_END 0x5A

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

/* TmState.aiLevel (TmOpponent.aiLevel), tm_enemySelect [code]: levels 0-2 score the free squares only 29, 63 and
 * 93 percent of the time (else every free square scores 0 and the pick is random); TM_AI_EXPERT always scores them
 * and also weighs the odds of the battles a square starts. Names by that order; [runtime] R41b confirmed it from
 * g_tmOpponents: the higher an opponent's deck, the higher its minimum level (0 = easiest, 3 = hardest). */
enum { TM_AI_WEAK, TM_AI_FAIR, TM_AI_STRONG, TM_AI_EXPERT };

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
    u8 unk4; /* not read by C */
} TmCardData; /* g_tmCardData entry, base stats of a card id */

typedef struct {
    u8 ids[16]; /* card id by rarity roll, rarest last (tm_dealEnemyHand) */
} TmDeck; /* g_tmDecks entry */

typedef struct {
    u8 deck;    /* index into g_tmDecks */
    u8 aiLevel; /* TmState.aiLevel, TM_AI_* */
} TmOpponent; /* g_tmOpponents entry */

/* Image file (file 0x44C of directory 1, tm_loadImageFile): a table of {sector offset, ?} entries, then the
 * entries. Its RAM is reused as the two primitive buffers once tm_setup has copied everything out. */
#define TM_IMAGE_FILE     PSX_RAM(u8 *, 0x80104A00)
#define TM_PRIM_BUF       TM_IMAGE_FILE
#define TM_PRIM_BUF_SIZE  0x4B000
/* DB_SOUND_BANK ids of the two banks in FILE_TM_DATA (tm_loadSoundBlocks uploads both). [runtime] R41b: keys 0x900E2 /
 * 0x900FA looked up; afterwards GameData.musicBank[0] = 0xE2. */
#define TM_SOUND_BANK_MUSIC 0xE2 /* the Tetra Master song's instrument bank (96 instruments, SPU slot 0), GameData.musicBank[0] */
#define TM_SOUND_BANK_SE    0xFA /* the card game's sound effects (32 instruments, slot 160) */
#define TM_FILE_CARDS     0 /* the 100 card picture TIMs, 0xA80 bytes each */
#define TM_FILE_CARD_DATA 1 /* -> g_tmCardData */
#define TM_FILE_DECKS     2 /* -> g_tmDecks */
#define TM_FILE_OPPONENTS 3 /* -> g_tmOpponents */
#define TM_FILE_BG        4 /* background, VRAM 0x280 (tm_drawBackground's first 256 columns) */
#define TM_FILE_BOARD     5 /* board picture, VRAM 0x340 (tm_drawBoard) */
#define TM_FILE_RESULT_TEXT  6 /* VRAM 0x280,0x100 (8 bpp 256x256): "WIN" / "LOSE" / "PERFECT" / "DRAW" banners (and
                                * their blurred shadows), "Card", "Selection", "Discard", the "!" mark, an unused
                                * "Select", combo digits and "COMBO" ([runtime] R41b, R44) */
#define TM_FILE_SCORE_DIGITS 7 /* VRAM 0x300,0x100 (8 bpp 256x128): the big blue / orange score numerals, card backs,
                                * small arrows and a stat font ([runtime] R41b) */
#define TM_FILE_COIN         8 /* VRAM 0x300,0x180 (8 bpp 256x128): the coin-toss coin frames, arrow pointers, "NEW Card",
                                * "Last ONE!", L1 / R1 ([runtime] R41b) */
#define TM_FILE_BATTLE    9 /* battle animation frames, VRAM 0x200,0x100 (tm_taskBattle) */
#define TM_FILE_SELECT_PARTS 0xA /* VRAM 0x340,0x1E0 (4 bpp 256x32): card-type icons, digits, "Select!" ([runtime] R41b) */
#define TM_FILE_BG_RIGHT  0xB /* background right of x 256, VRAM 0x300 */

/* Tables tm_setup copies out of the image file, at fixed addresses. */
#define g_tmCardData  PSX_RAM(TmCardData *, 0x80103800) /* [100] */
#define g_tmDecks     PSX_RAM(TmDeck *, 0x80103C00)     /* [64] */
#define g_tmOpponents PSX_RAM(TmOpponent *, 0x80104000) /* [256], indexed by GameData cardOpponent (event op 0xAE); the first 64 entries are {id/4, id%4} */

/* Card stats as kept in the card stock (main's CardEntry) */
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
    TmCell board[TM_BOARD_SIZE][TM_BOARD_SIZE]; /* [y][x] */
    s32 timer;    /* frames in the current task, reset by most task changes (TM_FIGHT_*) */
    u8 brightness;
    u8 pad2ED[3];
    TmBlock blocks[TM_MAX_BLOCKS];
    TmFlip flips[TM_NUM_CARDS]; /* at most one per card */
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
    s32 aiLevel;  /* TM_AI_* (tm_enemySelect) */
    s32 opponent; /* opponent id from GameData.unk8B2, index into g_tmOpponents */
    s32 unk454; /* write-only (set to 1) */
    Window *msgWin; /* confirm / rematch / trade window */
    s32 result;   /* TM_RESULT_* */
    s32 moveFrames; /* length of the card move animations */
    s32 showScore;  /* tm_drawScore */
    s32 showBoard;  /* tm_drawBoard */
    s32 showBlocks; /* tm_drawBlocks */
    TmCard *tradeCard; /* card changing hands (CARD_CHANGE, CHANGE2) */
    s32 stockCol; /* stock grid cursor, card id = col * TM_STOCK_SIDE + row */
    s32 stockRow;
    s32 handCount; /* cards picked from the stock into the hand */
    s32 animFrame; /* cursor animation frame; never initialised (stack residue, F42) */
    Window *quitWin;
    s32 savedFrame; /* animFrame of the hand cursor kept while placing */
    TmCard *targets[TM_NUM_DIRS]; /* cards the placed card can battle, one per direction at most (BATTLE SELECT) */
    s32 numTargets;
    TmCard *attacker;
    s32 copyIdx;  /* which copy of the card under the stock cursor */
    Window *infoWin; /* over-100 / discard / draw windows */
    s32 unk4BC; /* write-only (cleared) */
    s32 unk4C0; /* write-only (set to 1) */
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
    s32 unk554; /* write-only (cleared) */
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
    s32 unk5C4; /* not used by C */
    RECT unk5C8; /* write-only: tm_mainLoop sets {50, 30, 200, 100} */
} TmState; /* tm_main stack local, at 0x801FF9E0 at runtime (docs/r32/3ds-requests-4.md) */
#define TM_SEL_YES 0 /* TmState.cursor / quitChoice in the Yes/No tasks */
#define TM_SEL_NO 1

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
    s32 flags;    /* TM_TIM_PMODE: pixel mode */
    u32 clutLen;  /* clut block length */
    s16 clutRect[4];
    u32 clut[1];
} TmTim; /* TIM image */
/* TmTim.flags: TIM pixel mode (tm_loadTim passes it to LoadTPage as the colour mode) */
#define TM_TIM_PMODE 0x3
#define TM_TIM_4BIT 0 /* LoadTPage tp 0, width w << 2, with a CLUT */
#define TM_TIM_8BIT 1 /* tp 1, width w << 1, with a CLUT */
#define TM_TIM_16BIT 2 /* not loaded by tm_loadTim */

typedef struct {
    u32 len;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    u32 data[1];
} TmTimImage; /* TIM pixel block */

extern s32 g_tmDataFile;
extern s8 g_tmMusicRequest; /* sound request kind, -1 = none */
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

/* ---- Tetra Master layout and tuning (R44) ---- */
/* Screen positions are pixels on the 320 x 240 card-game screen (the background is 20 x 15 tiles of 16). UVs are
 * texel positions inside the texture page the sprite is drawn with. */
#define TM_SCREEN_H 240 /* help-window bounds height (tm_setHelpArea, GET SELECT) */
#define TM_NUM_OPPONENTS 256 /* g_tmOpponents entries */
#define TM_NUM_DECKS 64      /* g_tmDecks entries */
#define TM_STARTER_CARDS 8   /* random cards tm_setup gives a player who has fewer than TM_HAND_SIZE */

/* VRAM (tm_setup): where each graphics file and its CLUT row go (every CLUT at x 0), and the texture pages the draw
 * functions use. The 8-bit page at (TM_VRAM_PARTS_X, TM_VRAM_PARTS_Y) also covers the coin file below the parts file,
 * so the coin's sprites are drawn from that page with v >= 0x80. */
#define TM_VRAM_BG_X 0x280       /* TM_FILE_BG, y 0 */
#define TM_VRAM_BG_RIGHT_X 0x300 /* TM_FILE_BG_RIGHT, y 0 */
#define TM_VRAM_BOARD_X 0x340    /* TM_FILE_BOARD, y 0 */
#define TM_VRAM_PARTS_X 0x300    /* TM_FILE_SCORE_DIGITS: score numerals, card frames and backs, blocks, arrows */
#define TM_VRAM_PARTS_Y 0x100
#define TM_VRAM_COIN_Y 0x180     /* TM_FILE_COIN, at x TM_VRAM_PARTS_X: coin, pointer, badges, stat font */
#define TM_VRAM_BATTLE_X 0x200   /* TM_FILE_BATTLE: the clash animation */
#define TM_VRAM_BATTLE_Y 0x100
#define TM_VRAM_SELECT_X 0x340   /* TM_FILE_SELECT_PARTS: stock icons and digits, "Select!", battle digits */
#define TM_VRAM_SELECT_Y 0x1E0
#define TM_VRAM_RESULT_X 0x280   /* TM_FILE_RESULT_TEXT: result banners, stock titles, combo label */
#define TM_VRAM_RESULT_Y 0x100
#define TM_VRAM_CARDS_Y 0x100    /* card pictures: TM_CARDS_PER_PAGE per page, page p at x p * TM_CARD_PAGE_W */
#define TM_CARD_PAGE_W 128       /* VRAM columns of one card page (256 texels at 8 bpp) */
#define TM_CARDS_PER_PAGE 30
#define TM_CARDS_PER_ROW 6       /* TM_CARD_W x TM_CARD_H cells, 5 rows per page */
#define TM_CARD_TIM_SIZE 0xA80   /* each card's TIM in TM_FILE_CARDS */
#define TM_CLUT_BG 0xE0
#define TM_CLUT_BOARD 0xE7
#define TM_CLUT_RESULT 0xE9
#define TM_CLUT_PARTS 0xEA
#define TM_CLUT_COIN 0xEB
#define TM_CLUT_BATTLE 0xEC
#define TM_CLUT_SELECT 0xED
#define TM_CLUT_BG_RIGHT 0xEE   /* also the stock frame and record panel pieces, drawn from the BG_RIGHT page */
#define TM_CLUT_CARDS_X 0x180   /* card picture CLUTs: one row per card id from TM_CLUT_CARDS_Y */
#define TM_CLUT_CARDS_Y 0x19C

/* OT layers (higher = farther; the scene OT has OT_LEN entries) */
#define TM_OT_BG 0xFFE         /* background tiles */
#define TM_OT_BOARD 0xFFA      /* board picture, stock frame and titles */
#define TM_OT_TEXT TM_OT_BOARD /* labels and digits (text_drawBlock / ui_drawSmallDigits / ui_drawDigits) */
#define TM_OT_SCORE 0xFF0      /* captured-card counts */
#define TM_OT_BLOCK 100        /* block stones */
#define TM_OT_BLOCK_ZOOM 99    /* the block zooming in (ENEMY APEAR) */
#define TM_OT_CARDS_BACK 0x3C  /* hand card i at TM_OT_CARDS_BACK - i (earlier cards farther) */
#define TM_OT_CARDS 0x28       /* card depth while dealt or in the stock (+ i there), the selected captured card, the R1 label */
#define TM_OT_DEFENDER 0x27    /* the two cards of a battle, in front of the others */
#define TM_OT_ATTACKER 0x26
#define TM_OT_FLASH 30         /* capture flash, clash sprite, trade badge */
#define TM_OT_PANEL 11         /* stock grid, frame bars and counts; battle value digits */
#define TM_OT_COIN 10          /* coin toss, combo label */
#define TM_OT_WIN_TOP 6        /* confirm / don't-have-5 windows */
#define TM_OT_CURSOR_BACK 6    /* held pointer, stock hand behind the discard window */
#define TM_OT_WIN 5            /* other message windows, the R1 card name box */
#define TM_OT_HELP 4           /* g_menuHelp.otz */
#define TM_OT_CURSOR 3         /* pointer, hand cursor, the GET SELECT stock-count box */
#define TM_OT_RESULT 1         /* WIN / LOSE / PERFECT / DRAW banner */

/* Fades and timings (frames) */
#define TM_START_FADE_FRAMES 12    /* fade_in at tm_setup */
#define TM_FIELD_FADE_STEP 4       /* FIELD FADE: the background darkens / the board brightens by this per frame, */
#define TM_FIELD_FADE_FRAMES 0x20  /* and after this many frames the enemy hand is dealt (ENEMY APEAR) */
#define TM_END_FADE_FRAMES 30      /* END FADE: screen wipe and music fade-out */
#define TM_MUSIC_FADE_IN_FRAMES 10 /* song volume ramp when the music starts (tm_soundCmd) */
#define TM_RESULT_FADE_STEP 8      /* result banner brightness per frame */
#define TM_RESULT_INPUT_DELAY 0x22 /* RESULT: frames before Circle / Cross continue */
#define TM_NAME_SLIDE_FRAMES 10    /* R1 held: frames the card name box takes to slide in */
#define TM_NAME_SLIDE_STEP 12      /* pixels per frame of that slide */
#define TM_STOCK_SCROLL_FRAMES 4   /* L1 / R1 step to another copy of the card in the stock */
#define TM_GATHER_FRAMES 20        /* cards gliding back into the hands after the game (tm_startCardMove) */
#define TM_GATHER_PAUSE 10         /* frames after that before the trade starts */
#define TM_TRADE_MOVE_FRAMES 10    /* a traded card gliding to the middle and back (tm_nextTrade) */
#define TM_VALUES_FRAMES 60        /* battle values stay this long after the last battle frame */
#define TM_COMBO_FRAMES 50         /* combo label */
#define TM_FLASH_IN_STEP 30        /* FLASH / FLASH2 brightening per frame */
#define TM_FLASH_HOLD 10           /* FLASH: frames from the start until it fades */
#define TM_FLASH_OUT_STEP 15       /* FLASH fade-out per frame */
#define TM_FLASH2_OUT_STEP 12      /* FLASH2 (trade flips) fade-out per frame */
#define TM_COMBO_BAR_STEP 9        /* combo sweep bars fade per frame */
#define TM_COMBO_SWEEP_FRAMES 28   /* the bars cross the whole card in this many frames */
#define TM_FLASH_BAR_W 4           /* thickness of a combo sweep bar */
#define TM_FLASH_ARROW_FRAMES 0x15 /* FLASH ARROW: the combo arrow blinks this long */
#define TM_APPEAR_DELAY 15         /* ENEMY APEAR: frames before the first enemy card lands */
#define TM_APPEAR_STAGGER 6        /* frames between two enemy cards landing */
#define TM_APPEAR_FRAMES 0x29      /* ENEMY APEAR lasts at least this long (and until the blocks are done) */
#define TM_BLOCK_INTERVAL 9        /* frames between two block stones appearing */
#define TM_BLOCK_ZOOM_FRAMES 10    /* a block stone shrinks onto its square over this many frames */
#define TM_THINK_MIN 15            /* ENEMY THINK: first pause, TM_THINK_MIN + (rand & TM_THINK_RAND) frames, */
#define TM_THINK_RAND 0xF
#define TM_THINK_CARD_FRAMES 4     /* then the hand cards are pulled out one after another, this long each */
#define TM_LUNGE_FRAMES 15         /* BATTLE: the two cards lunge at each other from TM_FIGHT_CLASH */
#define TM_CLASH_W 40              /* clash animation frame size: 4 frames per row of the battle file, 4 ticks */
#define TM_CLASH_H 0x30            /* each; the first TM_LUNGE_FRAMES frames are TM_CLASH_W tall */
#define TM_COIN_SPIN_FRAMES 0x29   /* COIN: minimum spin before the coin stops on the winner's face */
#define TM_COIN_FRAMES 8          /* spin animation frames (g_tmCoinFrames), one per frame */
#define TM_COIN_FACE_PLAYER 7      /* coin frame (g_tmCoinFrames) that shows the player's face */
#define TM_COIN_FACE_ENEMY 3
#define TM_COIN_FLY_START 0x1E     /* after landing: frames until the coin flies off, */
#define TM_COIN_FLY_FRAMES 12      /* and the flight; COIN ends at TM_COIN_FLY_START + TM_COIN_FLY_FRAMES */
#define TM_COIN_FADE_STEP 10       /* coin brightness lost per frame of the flight */

/* Odds */
/* tm_dealEnemyHand: a roll 0-255 picks TmDeck.ids[slot]. Slots 0-11 each take TM_DECK_ODDS rolls, the rare slots
 * 12-15 the rest: 12 / 256, 2 / 256, 1 / 256, 1 / 256. */
#define TM_DECK_ODDS 20
#define TM_DECK_BAND(slot) (((slot) + 1) * TM_DECK_ODDS) /* roll below this: slot or lower */
#define TM_DECK_BAND_12 252
#define TM_DECK_BAND_13 254
#define TM_DECK_BAND_14 255
/* tm_placeBlocks: a roll 0-99 picks the number of block stones (cumulative bands; 6 otherwise, 3 %) */
#define TM_BLOCKS_BAND_0 3  /* none, 3 % */
#define TM_BLOCKS_BAND_1 7  /* 1 block, 4 % */
#define TM_BLOCKS_BAND_2 12 /* 2, 5 % */
#define TM_BLOCKS_BAND_3 17 /* 3, 5 % */
#define TM_BLOCKS_BAND_4 27 /* 4, 10 % */
#define TM_BLOCKS_BAND_5 97 /* 5, 70 % */
/* Card type changes: rand() & mask == 0 */
#define TM_UPGRADE_X_MASK 0x3F /* a P or M card that wins a battle becomes X (1 / 64) */
#define TM_UPGRADE_A_MASK 0x7F /* an X card that wins a battle becomes A (1 / 128) */
#define TM_RANDOM_X_MASK 0x7F  /* a freshly made card is X whatever its data says (1 / 128, tm_makeCard) */
/* tm_enemySelect scoring. A square scores (cards it captures unopposed) * weight + (win percent of the battle it
 * starts) * multiplier; the weights depend on the current card counts. */
#define TM_AI_TIED_WEIGHT 0x40  /* counts equal */
#define TM_AI_AHEAD_WEIGHT 0x43 /* enemy ahead */
#define TM_AI_BEHIND_WEIGHT 1   /* enemy behind: battles count TM_AI_BEHIND_MUL times instead */
#define TM_AI_BEHIND_MUL 5
#define TM_AI_WEAK_PCT 29       /* chance per move that TM_AI_WEAK scores at all (else a random free square) */
#define TM_AI_FAIR_PCT 63
#define TM_AI_STRONG_PCT 93
#define TM_AI_EXPERT_RISK_PCT 60   /* TM_AI_EXPERT: a battle below this win percent scores win% - 100 (negative), */
#define TM_AI_EXPERT_RISK_BASE 145 /* one at or above it TM_AI_EXPERT_RISK_BASE - win% (best just above the line) */
#define TM_AI_SCORE_NONE (-0x7FFF) /* occupied square / no best yet */
#define TM_AI_TIE_ROLL 1000        /* ties: each equal square is taken with chance 1 / count, rolled out of this */

/* Card stats tm_levelUpStat raises (one at random after a win) */
enum { TM_STAT_POWER, TM_STAT_PDEF, TM_STAT_MDEF, TM_NUM_STATS };

/* Board and hands */
#define TM_BOARD_PIC_X 0x30     /* board picture: 14 x 15 tiles from here; the background only shows beside it */
#define TM_BOARD_PIC_TILES 14
#define TM_BG_TILES_X 16        /* TM_FILE_BG: 256 pixels wide; TM_FILE_BG_RIGHT fills the last 4 tile columns */
#define TM_BG_RIGHT_TILES 4
#define TM_BG_TILES_Y 15
#define TM_SMALL_CARD_W 0x22    /* a small card (TmCard.small, the 5-card player hand) */
#define TM_SMALL_CARD_H 0x29
#define TM_PLAYER_HAND_Y 8      /* first player hand card; later ones TM_HAND_STEP (TM_HAND_STEP_SMALL) apart */
#define TM_HAND_STEP 0x34
#define TM_HAND_STEP_SMALL 0x2A
#define TM_SMALL_HAND_DX 4      /* small hand cards sit this much right of TM_PLAYER_HAND_X */
#define TM_ENEMY_HAND_Y 0x1A    /* enemy hand: cards TM_ENEMY_HAND_STEP apart */
#define TM_ENEMY_HAND_STEP 14
#define TM_ENEMY_PULL_X 0x1A    /* ENEMY THINK: the card being considered, pulled out of the hand */
#define TM_CAPTURED_SEL_X 0x16  /* GET SELECT: the captured card under the cursor */
#define TM_DEAL_Y 0x12C         /* enemy cards start below the screen and drop in */
#define TM_PILE_Y 0x19          /* after the game: hands stacked from here, TM_PILE_STEP apart (tm_stackHands */
#define TM_PILE_STEP 32         /* squeezes 5 or more into TM_PILE_SPAN) */
#define TM_PILE_SPAN 0x80
#define TM_TRADE_CARD_X 0x8B    /* a card changing hands is shown here (screen middle) */
#define TM_TRADE_CARD_Y 0x55
#define TM_TRADE_DROP_Y 0x99    /* CHANGE2: it glides from there towards this y, then joins its pile */
#define TM_PILE_DIM_STEP 22     /* stock screen: the copies behind the shown card get this much darker each */
#define TM_CARD_DIM 0x40        /* unselected hand cards (PLAYER CARD SET), non-target board cards (BATTLE SELECT) */
#define TM_TARGET_BLINK 24      /* BATTLE SELECT: a target card's face darkens by this every other 16 frames */

/* Pointer (tm_drawCursor): an animated 24 x 16 hand from the coin file, with a shadow TM_POINTER_SHADOW down-right */
enum {
    TM_CURSOR_POINT, /* the active pointer */
    TM_CURSOR_HELD,  /* translucent copy left at the hand card while placing it */
    TM_CURSOR_MIRROR /* pointing left (POLY_FT4 with swapped u), right of a captured card in GET SELECT */
};
#define TM_POINTER_W 0x18
#define TM_POINTER_H 0x10
#define TM_POINTER_FRAMES 8 /* animation frames, TM_POINTER_W apart */
#define TM_POINTER_TICKS 6 /* frames per animation frame */
#define TM_POINTER_SHADOW 4
#define TM_UV_POINTER_V 0x80
#define TM_CURSOR_DX (-0xF) /* pointer left of a card or square */
#define TM_CURSOR_DY 6
#define TM_CURSOR_SMALL_DX (-0x12) /* left of a small card */
#define TM_CURSOR_SMALL_DY 3
#define TM_HELP_DX 2 /* g_menuHelp.x/y relative to the pointer */
#define TM_HELP_DY 1
#define TM_GET_CURSOR_DX 0x22 /* GET SELECT mirrored pointer, from the captured card */
#define TM_GET_CURSOR_DY 5
#define TM_GET_HELP_DX 0x25
#define TM_GET_HELP_DY 6
#define TM_GET_HELP_W 0x12C   /* help bounds right of the card */

/* Sprites in the parts page (TM_VRAM_PARTS_*) */
#define TM_UV_FRAME_V 0x40       /* card frames and block stones, TM_CARD_W x TM_CARD_H each */
#define TM_UV_FRAME_PLAYER_U 8   /* blue frame (also the shrunk small cards) */
#define TM_UV_FRAME_ENEMY_U 0x38 /* red frame */
#define TM_UV_BLOCK_U 0x68       /* block stone, TmBlock.style 0 */
#define TM_UV_BLOCK2_U 0x98      /* style 1 */
#define TM_UV_CARD_BACK_U 0xC8   /* TmCard.hidden */
#define TM_UV_STAT_U 0x50        /* stat font (coin file, 8 x 8): glyph g at u TM_UV_STAT_U + g * 8 */
#define TM_UV_STAT_V 0xC0
#define TM_GLYPH_A 0xA           /* the type letters; A shares the hex digit's glyph */
#define TM_GLYPH_P 0x10
#define TM_GLYPH_M 0x11
#define TM_GLYPH_X 0x12
#define TM_STAT_Y 0x28           /* stat row on a card: power, type, physical and magic defence */
#define TM_STAT_POWER_X 9
#define TM_STAT_TYPE_X 15
#define TM_STAT_PDEF_X 0x15
#define TM_STAT_MDEF_X 0x1B
#define TM_UV_NEW_CARD_U 0x20    /* trade badge (CARD_CHANGE, coin file): "NEW Card" when you win your first copy, */
#define TM_UV_NEW_CARD_V 0x90
#define TM_UV_LAST_ONE_U 0xC0    /* "Last ONE!" when you lose your last copy */
#define TM_UV_LAST_ONE_V 0x80
#define TM_BADGE_X 0xA3          /* at the top right of the traded card */
#define TM_BADGE_Y 0x45
#define TM_BADGE_W 0x38
#define TM_BADGE_H 0x18

/* Score: captured-card counts (tm_drawScore), big numerals: 1-9 at u (n - 1) * 16, 0 and 10 separate */
#define TM_SCORE_PLAYER_X 0x28
#define TM_SCORE_PLAYER_Y 0xB9
#define TM_SCORE_ENEMY_X 0x1A
#define TM_SCORE_ENEMY_Y 0x93
#define TM_SCORE_DIGIT_W 0x10
#define TM_SCORE_H 0x20
#define TM_UV_SCORE_PLAYER_V 0   /* blue */
#define TM_UV_SCORE_ENEMY_V 0x20 /* orange */
#define TM_UV_SCORE_ZERO_U 0x90
#define TM_UV_SCORE_TEN_U 0xA0   /* "10", two digits wide */
#define TM_SCORE_SLASH_X 0x10    /* the 0x30-wide ornamented diagonal slash between the two counts */
#define TM_SCORE_SLASH_Y 0xAA

/* Battle values (tm_drawBattleValues): select-parts digits over each fighting card */
#define TM_VALUE_DX 8
#define TM_VALUE_DY 0x12
#define TM_BIG_DIGIT_ADV 9 /* advance per digit (10 x 16 glyphs) */

/* Combo label (tm_drawCombo): digit at the square's pointer position, "COMBO" right of it, both shadowed */
#define TM_COMBO_DY 0x11 /* comboY from the square's top (comboX: the pointer's TM_CURSOR_DX) */
#define TM_COMBO_LABEL_DX 0x10
#define TM_LABEL_SHADOW 2

/* Coin toss (tm_taskCoin) */
#define TM_COIN_X 0x9D
#define TM_COIN_Y 0x5C
#define TM_COIN_H 0x28

/* Result banner (tm_drawResult): two layers at y TM_RESULT_Y: the gold word drawn additive, and a blurred dark copy of
 * it drawn subtractive at the same spot (a soft shadow; both go to one OT slot, so the shadow is drawn first) */
#define TM_RESULT_Y 0x58
#define TM_RESULT_H 0x28
#define TM_RESULT_PERFECT_X 0x4A
#define TM_RESULT_WIN_X 0x6C
#define TM_RESULT_LOSE_X 0x6A
#define TM_RESULT_DRAW_X 0x5C

/* BATTLE SELECT target marker ("Select!", select-parts page) */
#define TM_SELECT_MARK_DX 5
#define TM_SELECT_MARK_DY 0x15

/* R1 card name box (PLAYER CARD SELECT) and the GET SELECT stock-count box: text TM_BOX_PAD_X in, TM_BOX_TEXT_DY
 * down */
#define TM_NAME_BOX_CX 0x11A /* centred on the hand column, kept left of the screen edge minus TM_BOX_PAD_X * 2 */
#define TM_BOX_H 0x19
#define TM_BOX_PAD_X 8
#define TM_BOX_TEXT_DY 6
#define TM_GET_BOX_X 0x52    /* GET SELECT "Stock: n" box, TM_GET_BOX_DY above the card */
#define TM_GET_BOX_DY 9
#define TM_R1_ICON_X 0x12    /* top left: the R1 button icon (TM_TEXT_ICON_R1) */
#define TM_R1_ICON_Y 9
#define TM_R1_NAME_LABEL_X 0x1F /* and the "-NAME" label beside it */
#define TM_R1_NAME_LABEL_Y 0xC

/* Message windows (win_openFree, centred unless an x is given): text TM_WIN_TEXT_X in, width = text +
 * 2 * TM_WIN_TEXT_X (2 * TM_BOX_PAD_X for the trade window) */
#define TM_WIN_TEXT_X 9
#define TM_WIN_DRAW_Y 0x59    /* "The game ended in a draw..." */
#define TM_WIN_DRAW_H 0x2B
#define TM_WIN_LESS5_Y 0x5B   /* "You don't have 5 cards..." */
#define TM_WIN_LESS5_H 0x2B
#define TM_WIN_OVER100_Y 0x5B /* "You have over 100 cards..." */
#define TM_WIN_OVER100_H 0x2A
#define TM_WIN_QUIT_Y 0x54    /* "End card game? Yes/No" */
#define TM_WIN_QUIT_H 0x38
#define TM_WIN_CONFIRM_Y 0x53 /* "Confirm selection? Yes/No" */
#define TM_WIN_CONFIRM_H 0x3A
#define TM_WIN_REMATCH_Y 0x54 /* "Play a rematch? Yes/No" */
#define TM_WIN_REMATCH_H 0x39
#define TM_WIN_DISCARD_X 0xB9 /* "Discard? Yes/No", right of the stock grid */
#define TM_WIN_DISCARD_Y 0x35
#define TM_WIN_DISCARD_H 0x38
#define TM_WIN_TRADE_Y 0x96   /* " Received " / " Lost " + card name */
#define TM_WIN_TRADE_H 0x1A
#define TM_TRADE_TEXT_CX 0x9F
/* Yes / No hand positions and help widths (tm_setHelpArea) */
#define TM_ASK_HAND_X 0x7B    /* confirm and quit windows */
#define TM_ASK_YES_Y 0x6C
#define TM_ASK_NO_Y 0x7B
#define TM_ASK_HELP_W 0x8D
#define TM_REMATCH_HAND_X 0x7D
#define TM_REMATCH_YES_Y 0x6B
#define TM_REMATCH_NO_Y 0x7B
#define TM_REMATCH_HELP_W 0x89
#define TM_DISCARD_HAND_X 0xBC
#define TM_DISCARD_YES_Y 0x4D
#define TM_DISCARD_NO_Y 0x5C
#define TM_DISCARD_HELP_W 0xCE
#define TM_HAND_HELP_W 0x104  /* PLAYER CARD SELECT help width (TM_HAND_HELP_W_SMALL for the 5-card hand) */
#define TM_HAND_HELP_W_SMALL 0x108

/* Stock screen (tm_drawStock, tm_stockInput, tm_showStockCard) */
#define TM_STOCK_GRID_X 0x12  /* the TM_STOCK_SIDE x TM_STOCK_SIDE card-kind grid, column = id / TM_STOCK_SIDE */
#define TM_STOCK_GRID_Y 0x2D
#define TM_STOCK_CELL 15      /* grid pitch; each cell a 16 x 16 rarity icon */
#define TM_STOCK_HAND_DY 3    /* hand cursor at the cell's TM_CURSOR_DX, this far down */
#define TM_STOCK_COUNT_DX 9   /* copy count: ones digit this far into the cell, the others TM_STOCK_DIGIT_W left */
#define TM_STOCK_COUNT_DY 8
#define TM_STOCK_DIGIT_W 4    /* 4 x 5 digits */
#define TM_STOCK_FRAME_X 9    /* grid frame: horizontal bars TM_STOCK_FRAME_W wide */
#define TM_STOCK_FRAME_TOP_Y 0x29
#define TM_STOCK_FRAME_BOTTOM_Y 0xBE
#define TM_STOCK_FRAME_W 0xA8
#define TM_STOCK_FRAME_LEFT_X 0xD /* vertical bars TM_STOCK_FRAME_H tall */
#define TM_STOCK_FRAME_RIGHT_X 0xA5
#define TM_STOCK_FRAME_SIDE_Y 0x2B
#define TM_STOCK_FRAME_H 0x98
#define TM_STOCK_HELP_X 0xD   /* help bounds while picking in the grid */
#define TM_STOCK_HELP_W 0x9E
#define TM_STOCK_HELP_H 0xDE
#define TM_STOCK_LABEL_Y 0xCB /* "Stock:" n  "Type:" n under the grid */
#define TM_STOCK_VALUE_Y 0xC9
#define TM_STOCK_COUNT_LABEL_X 0x19
#define TM_STOCK_COUNT_X 0x43
#define TM_STOCK_TYPES_LABEL_X 0x65
#define TM_STOCK_TYPES_X 0x89
#define TM_RECORD_LABEL_X 0xBC /* WIN / LOSE / DRAW record panel (card selection only) */
#define TM_RECORD_VALUE_X 0xDC
#define TM_RECORD_WIN_Y 0x42
#define TM_RECORD_LOSE_Y 0x4C
#define TM_RECORD_DRAW_Y 0x56
#define TM_RECORD_LEFT_X 0xB5  /* its frame: 8 x 8 corners and edges */
#define TM_RECORD_RIGHT_X 0xF6
#define TM_RECORD_TOP_Y 0x3D
#define TM_RECORD_BOTTOM_Y 0x5A
#define TM_RECORD_EDGE_X 0xB9  /* first top / bottom edge piece (8 of them) */
#define TM_RECORD_SIDE_Y 0x43  /* first side piece (3 of them) */
#define TM_RECORD_FILL_X 0xB8
#define TM_RECORD_FILL_Y 0x41
#define TM_RECORD_FILL_W 0x46
#define TM_RECORD_FILL_H 0x1E
#define TM_STOCK_CARD_X 0xC2   /* the card under the cursor, its copies piled TM_PILE_DX right and 1 down each */
#define TM_STOCK_CARD_Y 0x81
#define TM_PILE_DX 4
#define TM_STOCK_NO_X 0xD8     /* "NO" label and card number above it, centred on the number's width */
#define TM_STOCK_NO_Y 0x78
#define TM_STOCK_NUM_X 0xE6
#define TM_STOCK_NUM_Y 0x76
#define TM_STOCK_NAME_CX 0xD9  /* card name below it */
#define TM_STOCK_NAME_Y 0xC7
#define TM_COPY_NUM_X 0xBF     /* "n / total" copy counter between the L1 and R1 icons */
#define TM_COPY_SLASH_X 0xD4
#define TM_COPY_TOTAL_X 0xDB
#define TM_COPY_Y 0xBA
#define TM_COPY_L1_X 0xB5
#define TM_COPY_R1_X 0xF1
#define TM_COPY_ICON_Y 0xB6
/* Stock title sprites (result-text page). Card selection shows "Card" + "Selection"; discarding shows "Discard"
 * between two blinking "!" marks. */
#define TM_TITLE_CARD_X 0x58
#define TM_TITLE_CARD_Y 0xA
#define TM_TITLE_SELECTION_X 0x8C
#define TM_TITLE_SELECTION_Y 0xC
#define TM_TITLE_DISCARD_X 0x79
#define TM_TITLE_DISCARD_Y 0xE
#define TM_TITLE_MARK_R_X 0xCA
#define TM_TITLE_MARK_L_X 0x66
#define TM_TITLE_MARK_Y 0x12
#define TM_TITLE_TEXT_Y 0x10   /* title window callbacks no C code installs (tm_drawSelectTitle, tm_drawDiscardTitle) */
#define TM_TITLE_TEXT_SELECT_X 0x8A
#define TM_TITLE_TEXT_DISCARD_X 0x82

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
s32 akao_playSe(s32, s32, s32, s32);
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

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void tm_addRandomCard(); /* (TmState *, u8); unprototyped: callers pass the argument unconverted */

#endif
