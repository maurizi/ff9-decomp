#ifndef OVL_3A_H
#define OVL_3A_H

/* Declarations local to overlay 0x3A (name entry menu, loaded at 0x801EDF00).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#define FF9_BUILTIN_MEMCPY /* see memcpy in psyq.h */
#include "ff9.h"

#define NAME_MAX_LEN   8  /* letters in a name */
#define NAME_GRID_COLS 15 /* characters per grid row (3 groups of 5) */

#define NAME_ARG_CURRENT 12 /* menu argument >= this: id + NAME_ARG_CURRENT, start from the current name */

/* Name entry screen layout. Window rects (s_menuName.windows): */
#define NAME_TITLE_WIN_X 0xF0 /* "Name" title */
#define NAME_TITLE_WIN_Y 8
#define NAME_TITLE_WIN_W 0x44
#define NAME_TITLE_WIN_H 0x1A
#define NAME_PORTRAIT_WIN_X 0x16 /* character portrait */
#define NAME_PORTRAIT_WIN_Y 0x1A
#define NAME_PORTRAIT_WIN_W 0x24
#define NAME_PORTRAIT_WIN_H 0x32
#define NAME_CMD_WIN_X 0xE /* side menu: Alphabet / Symbol, Delete / Confirm / Default */
#define NAME_CMD_WIN_Y 0x52
#define NAME_CMD_WIN_W 0x44
#define NAME_CMD_WIN_H 0x69
#define NAME_HELP_WIN_X 0x1C /* "Pick a name." prompt, labelled Help */
#define NAME_HELP_WIN_Y 0x26
#define NAME_HELP_WIN_W 0x118
#define NAME_HELP_WIN_H 0x18
#define NAME_NAME_WIN_X 0x1C /* name slots + character grid, labelled Name */
#define NAME_NAME_WIN_Y 0x3E
#define NAME_NAME_WIN_W 0x118
#define NAME_NAME_WIN_H 0xA0
/* OT slots (each window's box one slot behind its text) */
#define NAME_OTZ_CURSOR    3
#define NAME_OTZ_PORTRAIT  4 /* portrait sprite; also the side menu text */
#define NAME_OTZ_CMD_TEXT  4
#define NAME_OTZ_CMD_BOX   5
#define NAME_OTZ_TEXT      5 /* title text, name window contents */
#define NAME_OTZ_BOX       6 /* title, portrait and name window boxes */
#define NAME_OTZ_HELP_TEXT 6
#define NAME_OTZ_HELP_BOX  7
#define NAME_TEXT_X 0xA /* title / side menu text, relative to the window */
#define NAME_TEXT_Y 6
#define NAME_RULE_LABEL_X 0x23 /* label of a WIN_STYLE_LINE_RULE window (Name, Help) */
#define NAME_RULE_LABEL_Y (-4)
#define NAME_PROMPT_X 0x3E /* help window prompt */
#define NAME_PORTRAIT_INSET 2 /* portrait sprite inside its window */
#define NAME_PORTRAIT_W 0x20
#define NAME_PORTRAIT_H 0x2E
#define NAME_PORTRAIT_V 0xB0 /* texture v of the portrait in its tpage */
#define NAME_PORTRAIT_VRAM_X 0x3C0 /* tpage */
#define NAME_PORTRAIT_VRAM_Y 0x100
#define NAME_PORTRAIT_CLUT_Y 0x100 /* CLUT row (x 0) */
#define NAME_CMD_SET_Y 9 /* side menu: first character-set row */
#define NAME_CMD_DIV_Y 0x2A /* etched divider between the set rows and the action rows */
#define NAME_CMD_ACTION_Y 0x31 /* first action row */
#define NAME_CMD_ROW_H 0x10
#define NAME_CURSOR_X (-0xE) /* side menu cursor, relative to the window */
#define NAME_CURSOR_DY 4 /* the cursor points this far below a row's text y */
#define NAME_SLOT_X 0x3E /* name slots: underline x (letters 1 to the right) */
#define NAME_SLOT_W 16
#define NAME_SLOT_LINE_Y 0x15 /* underline (light row, dark row below) */
#define NAME_SLOT_LINE_LEN 0xB
#define NAME_LETTER_Y 7
#define NAME_DIV_Y 0x1A /* etched divider between the name and the grid */
#define NAME_GRID_X 0x3E /* character grid */
#define NAME_GRID_Y 0x22
#define NAME_GRID_CELL_W 14
#define NAME_GRID_ROW_H 21
#define NAME_GRID_CURSOR_DX (-0x17) /* grid cursor, relative to a cell */
#define NAME_GROUP_COLS 5 /* symbol set: columns per group */
#define NAME_GROUP_W 72 /* symbol set: group spacing */
#define NAME_GROUP_CELL_W 13 /* symbol set: column spacing inside a group */

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
void SetLineF2(LINE_F2 *);
extern s32 g_labelName;
s32 text_drawChar(s32, s32, s32, u8, s32);
u8 *text_buildFontTpage(u32 *, void *);

/* x offset of a 5-column group in the name-entry character grid */
static inline s32 grp_x(s32 c) {
    return c * NAME_GROUP_W + NAME_GRID_X;
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

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
s32 chr_getPortrait(s32 charId); /* main */

#endif
