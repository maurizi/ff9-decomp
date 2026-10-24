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

/* Name entry state (s_menuName). */
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
    u8 savedClut[0x104]; /* CLUT row saved by menuName_loadPortrait (StoreImage), restored on exit */
    s32 nameId;        /* chr_setDefaultName id */
    s32 charIdx;       /* character record index */
    RECT windows[5];   /* title, portrait, side menu, help, name + grid */
} NameEntry;
extern NameEntry s_menuName;

extern RECT s_menuNameHelpBounds;
extern u8 s_menuNameGridSize[2];
extern RECT *s_menuNameTitleWin; /* window pointers into s_menuName.windows */
extern RECT *s_menuNamePortraitWin;
extern RECT *s_menuNameCommandWin;
extern RECT *s_menuNameHelpWin;
extern RECT *s_menuNameNameWin;
extern RECT s_menuNameClutRect;
extern RECT s_menuNamePortraitRect;
extern RECT s_menuNameClutRestoreRect;
extern s32 s_menuNameIds[12];
extern u8 s_menuNameCharIdx[12];
extern u8 g_nameMenuText[];
extern u8 s_menuNamePortraits[][0x6C0];

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
s32 text_drawChar(s32, s32, s32, u8, s32);
u8 *text_buildFontTpage(u32 *, void *);
extern s32 D_80077DEC; /* = g_menuHelp.otz, referenced by its own name */

/* x offset of a 5-column group in the name-entry character grid */
static inline s32 grp_x(s32 c) {
    return c * 72 + 0x3E;
}

void menuName_init(void);
void menuName_input(void);
void menuName_inputGrid(void);
void menuName_confirm(void);
void menuName_drawPortrait(void);
void menuName_drawCommands(void);
void menuName_drawNameWindow(void);
void menuName_drawCursor(void);
void menuName_draw(void);
void menuName_inputCommands(void);
void menuName_drawTitle(void);
void menuName_drawHelp(void);
void menuName_loadPortrait(s32);
void menuName_restoreClut(void);
s32 menuName_getNameId(s32);
s32 menuName_getCharIdx(s32);
void menuName_updateHelp(void);
void menuName_clampCursor(void);

#endif
