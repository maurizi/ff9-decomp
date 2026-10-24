#ifndef OVL_3A_H
#define OVL_3A_H

/* Declarations local to overlay 0x3A (name entry menu, loaded at 0x801EDF00).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#define FF9_BUILTIN_MEMCPY /* see memcpy in psyq.h */
#include "ff9.h"

#define NAME_MAX_LEN   8  /* letters in a name */
#define NAME_GRID_COLS 15 /* characters per grid row (3 groups of 5) */

enum { NAME_FOCUS_MENU, NAME_FOCUS_GRID }; /* NameEntry.focus */

/* side menu rows (NameEntry.menuCursor), in the order of their texts */
enum { NAME_MENU_ALPHABET, NAME_MENU_SYMBOL, NAME_MENU_DELETE, NAME_MENU_CONFIRM, NAME_MENU_DEFAULT };

/* g_nameMenuText ids */
#define NAME_TEXT_TITLE     0   /* "Name" */
#define NAME_TEXT_PROMPT    1   /* "Pick a name." */
#define NAME_TEXT_ALPHABET  2   /* "Alphabet", "Symbol": the character-set rows */
#define NAME_TEXT_DELETE    4   /* "Delete", "Confirm", "Default": the action rows */
#define NAME_TEXT_MENU_HELP 7   /* help for each side-menu row, + NAME_MENU_* */
#define NAME_TEXT_GRID_HELP 0xC /* button help while in the grid */
#define NAME_TEXT_CHARSET   0xD /* the grid characters: alphabet, then symbols */

/* Name entry state (g_menuName). */
typedef struct {
    u8 focus;          /* NAME_FOCUS_* */
    u8 menuCursor;     /* NAME_MENU_* */
    u8 charSet;        /* 0 = alphabet, 1 = symbols */
    u8 gridCursor;     /* NAME_GRID_COLS per row */
    u8 len;            /* name length (max NAME_MAX_LEN) */
    u8 gridSize[2];    /* characters in each set's grid */
    u8 name[10];       /* name being edited, 0xFF-terminated */
    u8 defaultName[11];
    u8 *charSets[2];   /* NAME_TEXT_CHARSET strings */
    u8 startDefault;   /* menu argument < 12: the name starts as the default name, else as the current one */
    u8 pad25[3];
    u8 savedClut[0x104]; /* CLUT row saved by menu_name_loadPortrait (StoreImage), restored on exit */
    s32 nameId;        /* chr_setDefaultName id */
    s32 charIdx;       /* character record index */
    RECT windows[5];   /* title, portrait, side menu, help, name + grid */
} NameEntry;
extern NameEntry g_menuName;

extern RECT g_menuNameHelpRect;
extern u8 g_menuNameGridSize[2];
extern RECT *g_menuNameTitleWin; /* window pointers into g_menuName.windows */
extern RECT *g_menuNamePortraitWin;
extern RECT *g_menuNameMenuWin;
extern RECT *g_menuNameHelpWin;
extern RECT *g_menuNameEntryWin;
extern RECT g_menuNameClutRect;
extern RECT g_menuNamePortraitRect;
extern RECT g_menuNameClutRestoreRect;
extern s32 g_menuNameIds[12];
extern u8 g_menuNameCharIdx[12];
extern u8 g_nameMenuText[];
extern u8 g_menuNamePortraits[][0x6C0];

/* main exe: candidates for ff9.h */
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#endif
void SetLineF2(LINE_F2 *);
extern s32 D_8007047C;
s32 func_80028F3C(s32, s32, s32, u8, s32);
u8 *func_8002BA90(u32 *, void *);
extern s32 D_80077DEC; /* = D_80077DE8.unk4, referenced by its own name */

/* x offset of a 5-column group in the name-entry character grid */
static inline s32 grp_x(s32 c) {
    return c * 72 + 0x3E;
}

void menu_name_init(void);
void menu_name_input(void);
void menu_name_moveInGrid(void);
void menu_name_select(void);
void menu_name_drawPortrait(void);
void menu_name_drawMenu(void);
void menu_name_drawEntry(void);
void menu_name_drawCursor(void);
void menu_name_draw(void);
void menu_name_moveInMenu(void);
void menu_name_drawTitle(void);
void menu_name_drawHelp(void);
void menu_name_loadPortrait(s32);
void menu_name_restoreClut(void);
s32 menu_name_getNameId(s32);
s32 menu_name_getCharIdx(s32);
void menu_name_setHelpText(void);
void menu_name_clampCursor(void);

#endif
