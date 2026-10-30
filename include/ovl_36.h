#ifndef OVL_36_H
#define OVL_36_H

/* Declarations local to overlay 0x36 (Card screen of the main menu: the Tetra Master
 * card collection and collector rank, loaded at 0x801EDF00). Overlay addresses mean different things
 * in different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

#define CARD_COUNT     100
#define CARD_IMAGE_FILE 0x32 /* file of dir 1 with the card images (s_menuCard.imageFile) */
#define CARD_GRID_ROWS 10 /* the CARD window is 10 x 10 cells */
#define CARD_GRID_COLS 10
#define CARD_RANK_COUNT 32
#define CARD_VRAM_SAVE_BUF PSX_RAM(u8 *, 0x801F6D30) /* past the overlay: VRAM saved by menuCard_loadImages */

/* CardMenu.state: index into s_menuCardInputFuncs */
enum { CARD_ST_NONE, CARD_ST_GRID, CARD_ST_DISCARD };
/* CardMenu.discardCursor */
enum { CARD_DISCARD_YES, CARD_DISCARD_NO };
/* g_cardMenuText ids */
#define CARD_TEXT_RANK         0    /* + rank: "Beginner" .. 31 "The Collector" */
#define CARD_TEXT_DISCARD      0x20 /* "Would you like to discard this card?", then 0x21 "Yes", 0x22 "No" */
#define CARD_TEXT_HELP_CARD    0x23 /* "Press (button) to discard one card." */
#define CARD_TEXT_HELP_STACK   0x24 /* the same plus "Press L1/R1 to scroll through cards." */
#define CARD_TEXT_HELP_DISCARD 0x25 /* + discardCursor: "Discard card." / "Cancel." */
#define CARD_TEXT_STOCK        0x27 /* "Stock" */
#define CARD_TEXT_TYPE         0x28 /* "Type" */
#define CARD_TEXT_COLLECTORS   0x29 /* "Collectors Lv" */
#define CARD_TEXT_WIN          0x2A /* "Win", then 0x2B "Lose", 0x2C "Draw" */
#define CARD_TEXT_LOSE         0x2B
#define CARD_TEXT_DRAW         0x2C

/* s_menuCardSprites entries (menuCard_drawSprite) */
#define CARD_SPR_FRAME      0    /* frame around the card face */
#define CARD_SPR_BACK       1    /* card of the stack behind the top one */
#define CARD_SPR_HEX        0xA  /* + 0-15: stat digits */
#define CARD_SPR_TYPE       0x1A /* + card type (P, M, X, A) */
#define CARD_SPR_EMPTY_CELL 0x1E /* grid cell of a card not owned */
#define CARD_SPR_CELL       0x1F /* + rarity (card_getRarity): grid cell of a single card */
#define CARD_SPR_CELL_STACK 0x26 /* + rarity: grid cell of several copies */
#define CARD_SPR_DIGIT      0x2D /* + 0-9: copy count on a grid cell */
#define CARD_SPR_NAME_ICON  0x37 /* before the card name in CARD INFO */

/* CardMenu.points[] (menuCard_calcCollectorPoints) */
enum {
    CARD_PTS_KINDS,  /* card_getInfoTable()[id].unk4 summed over the kinds owned */
    CARD_PTS_TYPES,  /* s_menuCardTypePoints[type] summed over every copy */
    CARD_PTS_ARROWS, /* 5 per arrow pattern seen */
    CARD_PTS_TOTAL
};

typedef struct {
    u8 used;
    u8 id;   /* card id */
    u8 vram; /* VRAM slot: clut row 0x1F4 + vram, 19x47 block (vram % 3, vram / 3) */
    u8 unk3;
} CardSlot;

/* Card image cache (s_menuCard.cache): 12 VRAM slots, filled round-robin. */
#define CARD_CACHE_SLOTS 12
#define CARD_NONE 0xFF
typedef struct {
    u8 oldest;    /* slot to evict when all are used */
    u8 next;      /* slot to fill next */
    u8 used;      /* slots used */
    u8 loading;   /* card being read */
    u8 pending;   /* card to read next, CARD_NONE = none */
    u8 cur;       /* card under the cursor */
    u8 prev;      /* card under the cursor before */
    u8 shown;     /* card whose image is in the face area (menuCard_showCachedImage), CARD_NONE = none */
    u8 fadeTicks;
    u8 fading;    /* set by menuCard_selectCard for 4 frames (menuCard_updateFade) */
    u8 reading;   /* CD read in progress */
    u8 padB[2];
    CardSlot slots[CARD_CACHE_SLOTS];
} CardCache; /* 0x3D */

/* Copy flip animation (s_menuCard.flip): L1/R1 in the grid (menuCard_flipCopy). */
enum { CARD_FLIP_PREV, CARD_FLIP_NEXT }; /* CardFlip.dir: L1, R1 */
#define CARD_FLIP_FRAMES 4
typedef struct {
    u8 active;
    u8 dir;   /* CARD_FLIP_* */
    u8 ticks;
    u8 id;    /* card id */
} CardFlip;

typedef struct {
    s32 unk0;
    s32 unk4;
} Unk801F4DC4;

/* Run-length packed block (menuCard_decodeRle). */
typedef struct {
    u8 unk0;
    u8 notKey; /* ~key: bytes above key start a run of (byte - key) copies of the next byte */
    u16 size;  /* packed size */
    u8 data[1];
} RleHeader;

/* Card menu state (s_menuCard). */
typedef struct {
    u8 state;      /* CARD_ST_*, index into s_menuCardInputFuncs */
    u8 row;        /* cursor row (0-9) */
    u8 col;        /* cursor column (0-9); card id = col * CARD_GRID_ROWS + row */
    u8 kinds;      /* card_countKinds (count of card kinds) on init */
    u8 loadingId;  /* card whose image the CD read is for */
    u8 discardCursor; /* CARD_DISCARD_* */
    u8 unk6;       /* set to 0xFF on init, never read */
    u8 rank;       /* collector rank (= text id) */
    u8 count[CARD_COUNT]; /* copies owned per card id */
    u8 shown[CARD_COUNT]; /* copy shown in CARD INFO per card id */
    u8 clutBuf[0x200];    /* decoded card image: clut */
    u8 pixelBuf[0xE00];   /* decoded card image: pixels */
    u8 sectorBuf[0x800];  /* CD sector read for the card image */
    RECT gridRect;     /* CARD window (menuCard_drawGrid) */
    RECT infoRect;     /* INFO window (menuCard_drawInfo) */
    RECT cardInfoRect; /* CARD INFO window (menuCard_drawCardInfo) */
    Window *discardWin;
    FileEntry *imageFile; /* file 0x32 of dir 1: the card images, one sector per card */
    s32 points[4]; /* collector points: CARD_PTS_* */
    CardCache cache;
    u8 pad193D[4];
    CardFlip flip;
    u8 pad1945[3];
    Unk801F4DC4 saved[4]; /* pad_getRepeatDelay / pad_getRepeatInterval of ids 0-3, restored on exit */
} CardMenu;
extern CardMenu s_menuCard;
void menuCard_drawCell(s32, s32, s32, s32);

typedef struct {
    s16 sprite; /* s_menuCardSprites entry */
    s8 x;
    s8 y;
} CardArrow; /* per arrow bit of a card (menuCard_drawArrows) */

extern s32 D_801F30E4[4];
extern void (*s_menuCardInputFuncs[])(); /* state handlers, called without arguments */
extern RECT s_menuCardDiscardRect;
extern CardArrow s_menuCardArrows[8];
extern s16 s_menuCardRankPoints[32]; /* points needed per rank */
extern RECT s_menuCardImage1ClutRect;
extern RECT s_menuCardImage1Rect;
extern RECT s_menuCardImage2ClutRect;
extern RECT s_menuCardImage2Rect;
extern u8 s_menuCardTypePoints[4];
extern u8 s_menuCardArrowsSeen[0x100];
extern u8 s_menuCardImage1[];
extern u8 s_menuCardImage2[];
extern s32 s_menuCardImageOffsets[];

typedef struct {
    u32 uvclut;
    u8 w;
    u8 h;
    u16 tpage;
} Ovl36Sprite;
extern Ovl36Sprite s_menuCardSprites[];
extern RECT s_menuCardSlotClutRect;
extern RECT s_menuCardSlotRect;
extern RECT s_menuCardSaveClutRect0;
extern RECT s_menuCardSaveClutRect1;
extern RECT s_menuCardSaveRect;
extern RECT s_menuCardRestoreClutRect0;
extern RECT s_menuCardRestoreClutRect1;
extern RECT s_menuCardRestoreRect;
void menuCard_drawArrows(s32, s32, s32, CardEntry *);
void menuCard_drawStats(s32, s32, s32, CardEntry *);
void menuCard_drawFace(s32, s32, s32, s32);
void menuCard_openDiscard(void);
void menuCard_refresh(void);
void menuCard_initImageCache(void);
void menuCard_loadImages(void);
void menuCard_initFlip(void);
extern s32 D_801F30D4[4];
void menuCard_drawGrid(void);
void menuCard_drawInfo(void);
void menuCard_drawCardStack(s32);
void menuCard_drawCellHighlight(void);
void menuCard_drawCursor(void);
void menuCard_loadCardImage(void);
void menuCard_updateImageLoad(void);
void menuCard_drawSprite(s32, s32, s32, s32, s32);
void menuCard_saveVram(u8 *);
void menuCard_restoreVram(u8 *);
void menuCard_calcCollectorPoints(s32 *);
void menuCard_updateHelp(void);
void menuCard_input(void);
void menuCard_draw(void);
void menuCard_updateAnims(void);
void menuCard_drawCardInfo(void);
void menuCard_updateFlip(void);
void menuCard_updateFade(void);
void menuCard_selectCard(s32);
CardSlot *menuCard_findCachedImage(s32);
void menuCard_showCachedImage(CardSlot *);
void menuCard_unloadImages(void);
void menuCard_onDiscardOpened(Window *);
void menuCard_onDiscardClosed(Window *);
void menuCard_drawDiscardWindow(Window *);
void menuCard_flipCopy(s32, s32);
s32 menuCard_decodeRle(u8 *, u8 *);
u32 menuCard_getStatsColor(void);

extern u8 s_menuCardPointsUnit[];
extern const u8 g_cardMenuText[]; /* text bank for this menu */
/* main exe: candidates for ff9.h */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67  /* cursor move / pick */
#define SE_BUZZER 0x66  /* refused action */
#define SE_CANCEL 0x65  /* Circle backs out */
#endif
#ifndef SE_PAGE
#define SE_PAGE 0x417   /* menuList page / L1-R1 switch */
#endif
s32 card_getWins(void); /* collector points */
s32 card_getLosses(void);
s32 card_getDraws(void);
/* defined as returning u16 in main.c, but called here without masking */
s32 pad_getRepeatDelay(s32, s32);
s32 pad_getRepeatInterval(s32, s32);
/* text width; the call result passes through an inline return value */
static inline s32 menuCard_drawInfo_width(u8 *s) {
    return text_measureWidth((s32)s);
}

/* Inlined copy of menuCard_showCachedImage (show a cached card picture), as expanded in menuCard_loadCardImage. The
   x, y, w, h statement order matters there: the w/h constants live in s-registers and their stores
   go through the &a argument register, so they get output dependences on the x/y stores. */
static inline void menuCard_showCachedImage_inline(CardSlot *s) {
    RECT a, b;
    u32 n = s->vram;

    a.x = 0;
    a.y = n + 0x1F4;
    a.w = 0x100;
    a.h = 1;
    b.x = (u8)(n % 3) * 0x13;
    b.y = (u8)(n / 3) * 0x2F + 0x100;
    b.w = 0x13;
    b.h = 0x2F;
    MoveImage(&a, 0, 0x1F2);
    MoveImage(&b, 0x3C0, 0x1B0);
    DrawSync(0);
    s_menuCard.cache.shown = s->id;
}

/* Start the CD read of the next requested card picture (s_menuCard.unk1900.unk4).
   menuCard_updateImageLoad expands this in both arms of its if/else; the original's two copies of
   the call are what make gcc give the argument loads no launch priority (REG_N_SETS == 2),
   after which jump2 cross-jumps the tails. */
static inline void menuCard_updateImageLoad_read(CardCache *c) {
    s32 id;
    u32 x;

    id = c->pending;
    if (id != 0xFF) {
        c->pending = 0xFF;
        c->loading = id;
        s_menuCard.cache.reading = 1;
        x = c->loading;
        s_menuCard.loadingId = x;
        cd_readAsync(g_gameState->imgSector + s_menuCard.imageFile->sector + x, 0x800, s_menuCard.sectorBuf, NULL);
    }
}

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void menuCard_drawCard(s32 x, s32 y, s32 z, s32 id, s32 dim, u8 flag);
s32 text_getButtonSprite(s32 arg0); /* main */

#endif
