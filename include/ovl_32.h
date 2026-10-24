#ifndef OVL_32_H
#define OVL_32_H

/* Declarations local to overlay 0x32 (Chocograph menu, loaded at 0x801EDF00).
 * Addresses in the overlay range mean different things in different overlays,
 * so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67  /* cursor move / pick */
#define SE_BUZZER 0x66  /* refused action */
#define SE_CANCEL 0x65  /* Circle backs out */
#endif
#ifndef SE_CONFIRM
#define SE_CONFIRM 0x6B /* a choice takes effect (chocograph set or cleared, equipment changed) */
#endif

/* ---- overlay ---- */

/* ChocographMenu.mode: index into g_menuChocoInputFuncs */
enum { CHOCO_MODE_NONE, CHOCO_MODE_COMMAND, CHOCO_MODE_LIST };
/* ChocographMenu.cmdCursor */
enum { CHOCO_CMD_SELECT, CHOCO_CMD_CANCEL };

#define CHOCOGRAPH_COUNT 24
#define CHOCO_LIST_ROWS  7 /* visible list rows (func_8002EDFC) */
#define CHOCO_LAST_PAGE  (CHOCOGRAPH_COUNT - CHOCO_LIST_ROWS) /* highest list scroll top */

/* Event variable bytes (g_evtGlobalVars.b) */
#define CHOCO_VAR_FOUND   0xB8 /* 3 bytes: bit per chocograph whose treasure was found */
#define CHOCO_VAR_OWNED   0xBB /* 3 bytes: bit per chocograph owned */
#define CHOCO_VAR_ABILITY 0xBF /* chocobo ability level, compared with g_menuChocoRequiredAbility */

/* g_chocographText ids */
#define CHOCO_TEXT_TITLE      0    /* "Chocograph" */
#define CHOCO_TEXT_SELECT     1    /* "Select", then 2 "Cancel" (+ command) */
#define CHOCO_TEXT_HELP_CMD   3    /* help of the commands (+ command) */
#define CHOCO_TEXT_INADEQUATE 5    /* "Inadequate ability, kupo!" appended to the hint */
#define CHOCO_TEXT_INVENTORY  7    /* "Inventory" */
#define CHOCO_TEXT_FOUND      8    /* "Loc. Found" */
#define CHOCO_TEXT_ABILITY    9    /* "Ability Required" */
#define CHOCO_TEXT_NAME       0xA  /* + id: chocograph names ("Streamside", ...) */
#define CHOCO_TEXT_HINT       0x22 /* + id: moogle hints, shown as help */
#define CHOCO_TEXT_CLUE       0x3A /* + id: clue text in the info window */

/* g_menuChocoIcons entries (menu_choco_drawIcon) */
#define CHOCO_ICON_LOCKED     0xB /* ability level not reached */
#define CHOCO_ICON_CHOCOGRAPH 0xC /* not found; +1 found; +2 / +3 the same when the ability is too low */

/* Chocograph menu state (g_menuChoco). */
typedef struct {
    u8 cmdCursor;  /* CHOCO_CMD_* */
    u8 mode;       /* CHOCO_MODE_* */
    u8 ownedCount;
    u8 foundCount;
    u8 ability;    /* CHOCO_VAR_ABILITY */
    u8 pad5[3];
    u32 owned;     /* bit per chocograph: CHOCO_VAR_OWNED */
    u32 found;     /* bit per chocograph: CHOCO_VAR_FOUND */
    MenuList list;
    RECT titleRect;    /* menu_choco_drawTitle */
    RECT cmdRect;      /* menu_choco_drawCommands */
    RECT selectedRect; /* menu_choco_drawSelected */
    RECT abilityRect;  /* menu_choco_drawAbility */
    RECT countRect;    /* menu_choco_drawCounts */
    RECT listRect;     /* menu_choco_drawList */
    RECT infoRect;     /* menu_choco_drawInfo */
    u8 textBuf[0x2000]; /* two 0x1000 halves, picked by frame parity, for func_8002920C */
    u8 *textPtr;
} ChocographMenu;
extern ChocographMenu g_menuChoco;

extern void (*g_menuChocoInputFuncs[])(void);
extern u8 g_menuChocoHelpBuf[]; /* help text buffer */
extern RECT g_menuChocoIconClutRect;
extern RECT g_menuChocoIconRect;
extern RECT g_menuChocoFrameClutRect;
extern RECT g_menuChocoFrameRect;

/* chocograph picture: 16-colour CLUT + 4bpp image, 24 of them in .rodata */
typedef struct {
    u8 clut[0x80];    /* 64 16-bit entries (0, 0x1E8 + i) */
    u8 pixels[0x498]; /* 21x28 halfwords */
} Ovl32Image;
extern Ovl32Image g_menuChocoPictures[];
extern u8 g_menuChocoIconImage[];
extern u8 g_menuChocoFrameImage[];
extern u8 g_menuChocoCmdNeedsSel[];
extern u8 g_menuChocoRequiredAbility[];
typedef struct {
    RECT r[16]; /* Icon entries (8 bytes each), copied as RECTs */
} IconTable;
extern IconTable g_menuChocoIcons;

void menu_choco_setHelpText(void);
void menu_choco_loadState(void);
void menu_choco_loadImages(void);
void menu_choco_drawSelected(void);
void menu_choco_drawList(void);
void menu_choco_drawCounts(void);
void menu_choco_drawInfo(void);
void menu_choco_drawCursor(void);
void menu_choco_restoreVram(void);
void menu_choco_drawPicture(s32, s32, s32, s32, u8);
void menu_choco_drawFrame(s32, s32, s32, u8);
void menu_choco_input(void);
void menu_choco_draw(void);
void menu_choco_updateList(void);
void menu_choco_drawTitle(void);
void menu_choco_drawCommands(void);
void menu_choco_drawAbility(void);
void menu_choco_drawListScroll(void);
void menu_choco_drawListItem(s32, s32, s32, MenuView *);
void menu_choco_drawIcon(s32, s32, s32, s32, u8);
s32 menu_choco_getRequiredAbility(s32);
/* inlined copy of menu_choco_getRequiredAbility */
static inline s32 menu_choco_getRequiredAbility_inline(s32 id) {
    return g_menuChocoRequiredAbility[id];
}
void menu_choco_scrollTo(s32);
void func_801F9660(void);
void menu_choco_getPictureRects(s32, RECT *, RECT *);

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
extern FrameRect g_menuChocoFramePieces[4]; /* frame pieces drawn by menu_choco_drawFrame */
extern u8 g_menuChocoFrameShadowPieces[2]; /* pieces that get a shadow */

#endif
