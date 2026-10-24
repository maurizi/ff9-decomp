#ifndef OVL_0D_H
#define OVL_0D_H

/* Declarations local to overlay 0x0D (Tetra Master card game, loaded at
 * 0x800A7000). Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* ---- Tetra Master ---- */

typedef struct {
    u8 unk0; /* card id? */
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;  /* x */
    s32 unk10; /* y */
    s32 unk14; /* owner: 0 player, 1 enemy */
    s32 unk18; /* 1: on the board */
    s32 unk1C;
    s32 unk20; /* x << 8 */
    s32 unk24; /* y << 8 */
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x3C - 0x38];
} TmCard; /* 0x3C */

typedef struct {
    s32 unk0;
    TmCard *unk4;
} TmCell; /* board square */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    TmCard *unk10;
} TmFlip;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} TmBlock;

typedef struct {
    s32 unk0; /* x */
    s32 unk4; /* y */
} TmPos;

typedef struct {
    s32 unk0; /* offset << 11 of entry */
    s32 unk4;
} TmFileEntry; /* file table loaded at 0x80104A00 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
} TmCardData; /* card stats table loaded at 0x80103800 */

typedef struct {
    u8 v[16];
} TmDeckOdds; /* card id by rarity roll, loaded at 0x80103C00 */

typedef struct {
    u8 unk0;
    u8 unk1;
} TmLevel; /* opponent level table, loaded at 0x80104000 */

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
    u8 savedConfig[12]; /* func_8002C700 values restored on exit */
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
    s32 unk0; /* x */
    s32 unk4; /* y */
    s32 unk8; /* x when flipped */
    s32 unkC; /* y when flipped */
    s32 unk10; /* u */
    s32 unk14; /* v */
} TmArrow;

typedef struct {
    TmArrow v[8];
} TmArrowTbl; /* arrow sprite for each direction */
extern TmArrowTbl g_tmArrowSprites;

typedef struct {
    s32 unk0; /* x */
    s32 unk4; /* u */
    s32 unk8; /* v */
    s32 unkC; /* w */
} TmCoinFrame;

typedef struct {
    TmCoinFrame v[8];
} TmCoinFrames; /* coin toss animation frames */
extern TmCoinFrames g_tmCoinFrames;

typedef struct {
    u32 unk0; /* id */
    s32 unk4; /* flags: bpp in low bits */
    u32 unk8; /* clut block length */
    s16 unkC[4]; /* clut rect */
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
extern u8 g_cardGameText[];
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
void func_800A9B0C(TmState *);
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
void *func_800ABEBC(u32 *, void *, TmState *);
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
void *func_800B3480(u32 *, void *, TmState *);
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

void tm_getBattleValues(s32, TmPos *, TmCard *, TmCard *);
TmCard *tm_findDefender(TmState *, s32, s32, TmCard *);
void tm_drawConfirmWin(Window *);
void tm_drawDiscardWin(Window *);
void tm_drawQuitWin(Window *);
s32 tm_getStockCursorId(TmState *);
void tm_setHelpArea(s16);
extern u8 g_tmSlash[];

/* main exe: candidates for ff9.h */
void func_800355DC(void); /* empty function, passed to func_8003556C as a no-op callback */
s32 func_8002BA10(u8 *);
void func_80023A0C(void);
void SetSprt16(SPRT_16 *);
s32 func_80062794(s32); /* width of a number string? */
s32 func_80062498(void);
s32 func_800624D8(void);
s32 func_8006251C(void);
u8 func_8002C700(s32, s32);
void func_80062724();
extern u8 *D_80077DF0; /* = D_80077DE8.unk8, referenced by its own name */
s32 func_80055290(void *);
void func_800552B8(s32);
void func_800553EC(s32, s32, s32, s32);
void func_80055704(s32);
void func_80055730(s32, s32);
void func_80055764(s32, s32, s32);
void func_800557A8(s32, s32, s32, s32);
void func_800557F0(s32);
void func_8005581C(s32, s32);
void func_80055850(s32, s32, s32);
void func_80055894(s32, s32, s32, s32);
void func_800559C8(s32, s32);
void func_800559FC(s32, s32, s32);
void func_80055A34(s32, s32, s32, s32);


/* Capture flash bar pieces (tm_drawFlash), one per direction. */
typedef struct {
    s32 unk0; /* 0: horizontal bar, 1: corner, 2: vertical bar */
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14; /* x offset */
    s32 unk18; /* y offset */
} TmFlash;

typedef struct {
    TmFlash v[8];
} TmFlashTbl;
extern TmFlashTbl g_tmFlashBars; /* INCLUDE_RODATA */

extern char g_tmNoLabel[]; /* "NO" label (INCLUDE_RODATA) */

#endif
