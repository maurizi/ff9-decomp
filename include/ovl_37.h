#ifndef OVL_37_H
#define OVL_37_H

/* Declarations local to overlay 0x37 (items menu, loaded at 0x801EDF00).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* ItemMenu.state: input handler in g_menuItemsInputFns */
enum {
    ITEM_STATE_BUSY,     /* no input: a window is moving */
    ITEM_STATE_CMD,      /* Use / Arrange / Key */
    ITEM_STATE_LIST,     /* item or key item list */
    ITEM_STATE_TARGET,   /* picking the character to use the item on */
    ITEM_STATE_ARRANGE,  /* Auto / Manual window */
    ITEM_STATE_MANUAL,   /* manual arrange: pick two slots to swap */
    ITEM_STATE_KEY_VIEW  /* a key item's picture and description */
};

enum { ITEM_CMD_USE, ITEM_CMD_ARRANGE, ITEM_CMD_KEY }; /* ItemMenu.cmd, the top row */

enum { KEY_VIEW_NONE, KEY_VIEW_OPENING, KEY_VIEW_SHOWN, KEY_VIEW_CLOSING }; /* ItemMenu.keyViewPhase */

#define ITEM_SLOT_COUNT   0x100 /* GameData.items slots */
#define KEY_ITEM_COUNT    0x50  /* key item ids (func_800281A4) */
#define ITEM_FIRST_USABLE 0xE0  /* items from here have a use entry in D_8006B1F4 */

/* g_itemMenuText ids */
#define ITEM_TEXT_USE          0   /* "Use", "Arrange", "Key": the top row, + ITEM_CMD_* */
#define ITEM_TEXT_AUTO         3   /* "Auto", "Manual" */
#define ITEM_TEXT_CMD_HELP     5   /* help of the top row, + ITEM_CMD_* */
#define ITEM_TEXT_SELECT_CHAR  8   /* "Select character." */
#define ITEM_TEXT_ARRANGE_HELP 9   /* help of Auto / Manual, + cursor */
#define ITEM_TEXT_KWEH         0xB /* "Kweeeh!" (menu_items_callChocobo) */

/* Items menu state (g_menuItems). */
typedef struct {
    u8 cmd;           /* ITEM_CMD_*, index into g_menuItemsLists */
    u8 state;         /* ITEM_STATE_* */
    u8 slot;          /* party slot of the target */
    u8 targetKind;    /* D_8006B1F4 target kind of the item being used */
    u8 targetMode;    /* D_8006B1F4 bits 5-7: 0 = use at once, else pick a character (cursor mode) */
    u8 arrangeCursor; /* 0 = Auto, 1 = Manual */
    u8 swapFrom;      /* manual arrange: first slot picked */
    u8 swapPicked;    /* manual arrange: swapFrom is set */
    u8 charWinOpen;   /* the character window is shown */
    u8 pad9[3];
    MenuList itemList;
    MenuList keyList;
    RECT cmdRect;     /* top row window */
    RECT listRect;    /* list window */
    WinDef charWin;   /* character window (slides in from the list side) */
    Window *popup;    /* Auto/Manual window, or the "Kweeeh!" window */
    u32 popupTimer;   /* frames the "Kweeeh!" window has been up, closes the menu after 0x60 */
    u8 keyView;       /* key item view is up */
    u8 keyViewPhase;  /* KEY_VIEW_* */
    u8 keyViewId;     /* key item shown */
    u8 keyViewStep;   /* step in g_menuItemsKeyViewSlide */
    s32 keyViewX;     /* x of the view, moved by the g_menuItemsKeyViewSlide deltas */
    u8 pad5C[4];
    u8 keyItems[KEY_ITEM_COUNT]; /* key item ids the party has */
    s32 keyCount;
} ItemMenu;
extern ItemMenu g_menuItems;

typedef struct {
    void (*select)(s32, MenuList *);           /* Cross on an entry */
    void (*drawRow)(s32, s32, s32, MenuView *); /* one entry */
    MenuList *list;
} ItemList; /* 12 bytes */
extern ItemList g_menuItemsLists[3]; /* per ItemMenu.cmd */
extern s8 g_menuItemsKeyViewSlide[24];
extern void (*g_menuItemsInputFns[])(void);
extern u8 g_menuItemsTargetClose[2][0x10];
extern RECT g_menuItemsKeyClutRect;
extern RECT g_menuItemsKeyImageRect;
extern u8 g_menuItemsKeyImage[];

extern s32 g_menuItemsCmdX[3];
extern u8 g_menuItemsCmdText[3];
extern RECT g_menuItemsArrangeRect;

/* main exe: candidates for ff9.h */
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#endif
#define SE_USE_ITEM 0x6A /* an item was used (menu_items_useItem) */
#ifndef ITEM_NONE
#define ITEM_NONE 0xFF /* empty item id */
#endif
void func_800355DC(void); /* empty function, passed to func_8003556C as a no-op callback */
extern s32 D_80077E54; /* = D_80077E50.unk4, referenced by its own name */
typedef struct {
    unsigned int unk0 : 4;   /* target kind */
    unsigned int unk0_4 : 1;
    unsigned int unk0_5 : 3;
    unsigned int pad0_8 : 24;
    u8 unk4;
    u8 pad5[3];
    s32 unk8;
} Unk8006B1F4; /* 0xC, items 0xE0.. (also in ovl_0b.h) */
extern Unk8006B1F4 D_8006B1F4[];
u8 func_800654F8(Character *, Character *, Unk8006B1F4 *, s32, s32);
extern s32 D_80070408;
void func_801F3038(void);
void menu_items_initKeyItems(void);
void menu_items_loadImages(void);
void menu_items_drawListScroll(void);
extern RECT *g_menuItemsListRect;
extern RECT g_menuItemsScrollClip;

extern u8 g_menuItemsTargetOpen[2][0x10];
extern RECT g_menuItemsKwehRect;
extern u8 D_801F3140[];
extern RECT g_menuItemsArrangeHelpArea;
extern u8 g_menuItemsHelp[]; /* item help text */
extern u8 g_menuItemsKeyDesc[]; /* key item descriptions */
extern s32 g_menuItemsCmdCursorX[3]; /* list cursor x per list */
extern u8 g_menuItemsKeyHelp[]; /* key item help text */
s32 menu_items_getCursorIndex(MenuList *);

/* other overlay */
s8 func_800B8124(void);
void func_800B81A0(void);

extern ItemSlot g_menuItemsSortBuf[0x100]; /* inventory sort buffer */
extern ItemSlot g_menuItemsSortTmp; /* swap temp for the inventory sort */

void menu_items_inputCmd(void);
void menu_items_openTarget(void);
void menu_items_useItem(s32);
void menu_items_closeTarget(void);
u8 func_800281D0(s32); /* main exe */
void ui_drawItemIcon(s32, s32, s32, s32, u8); /* main exe */
void menu_items_onKwehOpened(Window *);
void menu_items_drawKweh(Window *);
void menu_items_openArrange(void);
void func_80028128(s32); /* main exe */
void func_80026EA0(s32, s32, s32, s32, s32); /* main exe */
void menu_items_sort(void);
void menu_items_select(void);
void menu_items_closeArrange(void);
void menu_items_drawArrange(Window *);
void menu_items_onArrangeOpened(Window *);
void menu_items_onArrangeClosed(Window *);
void menu_items_inputTarget(void);
void menu_items_inputManual(void);
void menu_items_selectItem(s32, MenuList *);
void menu_items_drawList(void);
void menu_items_drawCursor(void);
void menu_items_setHelpText(void);
void menu_items_stepKeyView(void);
void menu_items_drawKeyView(void);
void menu_items_input(void);
void menu_items_draw(void);
void menu_items_animate(void);
void menu_items_inputList(void);
void menu_items_inputArrange(void);
void menu_items_inputKeyView(void);
void menu_items_selectKeyItem(s32);
void menu_items_drawCmds(void);
void menu_items_drawTargets(void);
void menu_items_drawItemRow(s32, s32, s32, MenuView *);
void menu_items_drawKeyRow(s32, s32, s32, MenuView *);
void menu_items_stepKweh(void);
void func_801F2F30(void);
void menu_items_openKeyView(s32);
void menu_items_closeKeyView(void);

extern const u8 g_itemMenuText[]; /* text bank for this menu */

#endif
