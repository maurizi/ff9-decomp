#ifndef OVL_32_H
#define OVL_32_H

/* Declarations local to overlay 0x32 (Chocograph menu, loaded at 0x801EDF00).
 * Addresses in the overlay range mean different things in different overlays,
 * so they live here, not in ff9.h. */
#include "ff9.h"

/* ---- overlay ---- */

/* ChocographMenu.mode: index into s_menuChocographInputFuncs */
enum { CHOCO_MODE_NONE, CHOCO_MODE_TABS, CHOCO_MODE_LIST };
/* ChocographMenu.tab */
enum { CHOCO_TAB_SELECT, CHOCO_TAB_CANCEL };

#define CHOCOGRAPH_COUNT 24
#define CHOCO_LIST_ROWS  7 /* visible list rows (menuList_init) */
/* Chocograph screen layout (screen coordinates unless noted) */
#define CHOCO_TITLE_WIN_X 0xE4 /* "Chocograph" title */
#define CHOCO_TITLE_WIN_Y 8
#define CHOCO_TITLE_WIN_W 0x50
#define CHOCO_TITLE_WIN_H 0x1A
#define CHOCO_TITLE_TEXT_X 7
#define CHOCO_TEXT_Y 6 /* title / tab text */
#define CHOCO_TABS_WIN_X 8 /* Select / Cancel tabs */
#define CHOCO_TABS_WIN_Y 8
#define CHOCO_TABS_WIN_W 0xD8
#define CHOCO_TABS_WIN_H 0x1A
#define CHOCO_TAB_TEXT_X 0x2C /* first tab text, relative to the window */
#define CHOCO_TAB_W 0x60 /* tab spacing */
#define CHOCO_TAB_CURSOR_X 0x1C /* tab cursor (screen) */
#define CHOCO_TAB_CURSOR_Y 0x11
#define CHOCO_SELECTED_WIN_X 8 /* SELECTED window: the chocograph being searched for */
#define CHOCO_SELECTED_WIN_Y 0x26
#define CHOCO_SELECTED_WIN_W 0xB7
#define CHOCO_SELECTED_WIN_H 0x43
#define CHOCO_SEL_ICON_X 0x10 /* relative to the window */
#define CHOCO_SEL_ICON_Y 0x2C
#define CHOCO_SEL_NAME_X 0x21
#define CHOCO_SEL_NAME_Y 0x2E
#define CHOCO_SEL_FRAME_X 0x48 /* picture frame */
#define CHOCO_SEL_FRAME_Y 7
#define CHOCO_FRAME_INSET 4 /* the picture inside its frame */
#define CHOCO_FRAME_SHADOW_D 3 /* the frame's drop shadow offset */
#define CHOCO_ABIL_WIN_X 0xBF /* CHOCOBO ABILITY window: the abilities learnt */
#define CHOCO_ABIL_WIN_Y 0x26
#define CHOCO_ABIL_WIN_W 0x75
#define CHOCO_ABIL_WIN_H 0x21
#define CHOCO_ABIL_ICON_X 0xB
#define CHOCO_ABIL_ICON_Y 5
#define CHOCO_ABIL_ICON_W 0x13 /* ability icon spacing (also in the hint window) */
#define CHOCO_COUNT_WIN_X 0xBF /* Inventory / Loc. Found counts */
#define CHOCO_COUNT_WIN_Y 0x47
#define CHOCO_COUNT_WIN_W 0x75
#define CHOCO_COUNT_WIN_H 0x22
#define CHOCO_COUNT_TEXT_X 9
#define CHOCO_COUNT_INV_Y 3
#define CHOCO_COUNT_FOUND_Y 0xF
#define CHOCO_COUNT_DIGITS_X 0x5E /* (digits 4 below their label's y) */
#define CHOCO_LIST_WIN_X 8 /* CHOCOGRAPH list */
#define CHOCO_LIST_WIN_Y 0x6B
#define CHOCO_LIST_WIN_W 0x86
#define CHOCO_LIST_WIN_H 0x71
#define CHOCO_LIST_LABEL_X 0x10
#define CHOCO_ROW_H 0xF
#define CHOCO_ROW_ICON_X 0xE /* row (screen x) */
#define CHOCO_ROW_NAME_DX 0x11
#define CHOCO_LIST_CURSOR_Y 0x74 /* list cursor on row 0 (screen) */
#define CHOCO_HINT_WIN_X 0x8E /* HINT window: picture, clue and Ability Required */
#define CHOCO_HINT_WIN_Y 0x6B
#define CHOCO_HINT_WIN_W 0xA6
#define CHOCO_HINT_WIN_H 0x71
#define CHOCO_HINT_FRAME_X 8
#define CHOCO_HINT_FRAME_Y 8
#define CHOCO_CLUE_X 0xC
#define CHOCO_CLUE_Y 0x31
#define CHOCO_REQ_X 0x42 /* "Ability Required" and its icons */
#define CHOCO_REQ_TEXT_Y 6
#define CHOCO_REQ_ICON_Y 0x16
#define CHOCO_LABEL_X 8 /* window labels */
#define CHOCO_OTZ_CURSOR 0x1B
#define CHOCO_OTZ_TEXT 0x1C
#define CHOCO_OTZ_WIN 0x1D
#define CHOCO_OTZ_SHADOW 0x1E /* SELECTED, count, HINT and ability window shadows */
/* chocograph pictures: CHOCO_PICTURE_W x CHOCO_PICTURE_H, 8-bit, CHOCO_PICTURE_COLS per row from VRAM (0,
 * CHOCO_PICTURE_VRAM_Y), CLUT row CHOCO_PICTURE_CLUT_Y + id */
#define CHOCO_PICTURE_W 0x2A
#define CHOCO_PICTURE_H 0x1C
#define CHOCO_PICTURE_COLS 3
#define CHOCO_PICTURE_VRAM_Y 0x100
#define CHOCO_PICTURE_CLUT_Y 0x1E8
#define CHOCO_FRAME_CLUT_X 0x3C0 /* picture frame CLUT (its shadow uses the next row) */
#define CHOCO_FRAME_CLUT_Y 0x1FE
/* on the world map, menuChocograph_restoreVram copies the selected chocograph's CLUT and pixels here first */
#define CHOCO_WM_CLUT_X 0x280
#define CHOCO_WM_CLUT_Y 0xF6
#define CHOCO_WM_PICTURE_X 0x318
#define CHOCO_WM_PICTURE_Y 0xB0
#define CHOCO_LAST_PAGE  (CHOCOGRAPH_COUNT - CHOCO_LIST_ROWS) /* highest list scroll top */

/* Event variable bytes (g_evtGlobalVars.b) */
#define CHOCO_VAR_FOUND   0xB8 /* 3 bytes: bit per chocograph whose treasure was found */
#define CHOCO_VAR_OWNED   0xBB /* 3 bytes: bit per chocograph owned */
#define CHOCO_VAR_ABILITY 0xBF /* chocobo ability level, compared with s_menuChocographRequiredAbility */

/* g_chocographText ids */
#define CHOCO_TEXT_TITLE      0    /* "Chocograph" */
#define CHOCO_TEXT_SELECT     1    /* "Select", then 2 "Cancel" (+ tab) */
#define CHOCO_TEXT_HELP_TAB   3    /* help of the tabs (+ tab) */
#define CHOCO_TEXT_INADEQUATE 5    /* "Inadequate ability, kupo!" appended to the hint */
#define CHOCO_TEXT_INVENTORY  7    /* "Inventory" */
#define CHOCO_TEXT_FOUND      8    /* "Loc. Found" */
#define CHOCO_TEXT_ABILITY    9    /* "Ability Required" */
#define CHOCO_TEXT_NAME       0xA  /* + id: chocograph names ("Streamside", ...) */
#define CHOCO_TEXT_HINT       0x22 /* + id: moogle hints, shown as help */
#define CHOCO_TEXT_CLUE       0x3A /* + id: clue text in the info window */

/* s_menuChocographIcons entries (menuChocograph_drawIcon) */
#define CHOCO_ICON_LOCKED     0xB /* ability level not reached */
#define CHOCO_ICON_CHOCOGRAPH 0xC /* not found; +1 found; +2 / +3 the same when the ability is too low */

/* Chocograph menu state (s_menuChocograph). */
typedef struct {
    u8 tab;  /* CHOCO_CMD_* */
    u8 mode;       /* CHOCO_MODE_* */
    u8 ownedCount;
    u8 foundCount;
    u8 ability;    /* CHOCO_VAR_ABILITY */
    u8 pad5[3];
    u32 owned;     /* bit per chocograph: CHOCO_VAR_OWNED */
    u32 found;     /* bit per chocograph: CHOCO_VAR_FOUND */
    MenuList list;
    RECT titleRect;    /* menuChocograph_drawTitle */
    RECT tabRect;      /* menuChocograph_drawTabs */
    RECT selectedRect; /* menuChocograph_drawSelected */
    RECT abilityRect;  /* menuChocograph_drawAbilities */
    RECT countRect;    /* menuChocograph_drawCounts */
    RECT listRect;     /* menuChocograph_drawList */
    RECT infoRect;     /* menuChocograph_drawHint */
    u8 textBuf[0x2000]; /* two 0x1000 halves, picked by frame parity, for text_buildBlock */
    u8 *textPtr;
} ChocographMenu;
extern ChocographMenu s_menuChocograph;

extern void (*s_menuChocographInputFuncs[])(void);
extern u8 s_menuChocographHelpBuf[]; /* help text buffer */
extern RECT s_menuChocographIconClutRect;
extern RECT s_menuChocographIconRect;
extern RECT s_menuChocographFrameClutRect;
extern RECT s_menuChocographFrameRect;

/* chocograph picture: 16-colour CLUT + 4bpp image, 24 of them in .rodata */
typedef struct {
    u8 clut[0x80];    /* 64 16-bit entries (0, 0x1E8 + i) */
    u8 pixels[0x498]; /* 21x28 halfwords */
} Ovl32Image;
extern Ovl32Image s_menuChocographPictures[];
extern u8 s_menuChocographIconImage[];
extern u8 s_menuChocographFrameImage[];
extern u8 s_menuChocographTabNeedsSel[];
extern u8 s_menuChocographRequiredAbility[];
typedef struct {
    RECT r[16]; /* Icon entries (8 bytes each), copied as RECTs */
} IconTable;
extern IconTable s_menuChocographIcons;

void menuChocograph_updateHelp(void);
void menuChocograph_readFlags(void);
void menuChocograph_loadImages(void);
void menuChocograph_drawSelected(void);
void menuChocograph_drawList(void);
void menuChocograph_drawCounts(void);
void menuChocograph_drawHint(void);
void menuChocograph_drawCursor(void);
void menuChocograph_restoreVram(void);
void menuChocograph_drawPicture(s32, s32, s32, s32, u8);
void menuChocograph_drawFrame(s32, s32, s32, u8);
void menuChocograph_input(void);
void menuChocograph_draw(void);
void menuChocograph_updateList(void);
void menuChocograph_drawTitle(void);
void menuChocograph_drawTabs(void);
void menuChocograph_drawAbilities(void);
void menuChocograph_drawScrollRows(void);
void menuChocograph_drawRow(s32, s32, s32, MenuView *);
void menuChocograph_drawIcon(s32, s32, s32, s32, u8);
s32 menuChocograph_getRequiredAbility(s32);
/* inlined copy of menuChocograph_getRequiredAbility */
static inline s32 menuChocograph_getRequiredAbility_inline(s32 id) {
    return s_menuChocographRequiredAbility[id];
}
void menuChocograph_scrollTo(s32);
void menuChocograph_onLoadImages(void);
void menuChocograph_getPictureRects(s32, RECT *, RECT *);

extern const u8 g_chocographText[]; /* text bank for this menu */

typedef struct {
    u32 tag;
    u32 rgbc;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad1;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad2;
} Ft4W; /* POLY_FT4 with the colour/code written as one word */

typedef struct {
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    u8 w;
    u8 h;
} FrameRect; /* window frame piece: position, uv, size */
extern FrameRect s_menuChocographFramePieces[4]; /* frame pieces drawn by menuChocograph_drawFrame */
extern u8 s_menuChocographFrameShadowPieces[2]; /* pieces that get a shadow */

/* main exe: candidates for ff9.h */
#define WIN_TITLE_X 6    /* default Window.titleX / titleY (win_init): a window label's offset from its box */
#define WIN_TITLE_Y (-3)

#endif
