#ifndef OVL_37_H
#define OVL_37_H

/* Declarations local to overlay 0x37 (items menu, loaded at 0x801EDF00).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* ItemMenu.state: input handler in s_menuItemInput */
enum {
    ITEM_STATE_BUSY,     /* no input: a window is moving */
    ITEM_STATE_TABS,     /* Use / Arrange / Key */
    ITEM_STATE_LIST,     /* item or key item list */
    ITEM_STATE_TARGET,   /* picking the character to use the item on */
    ITEM_STATE_ARRANGE,  /* Auto / Manual window */
    ITEM_STATE_MANUAL,   /* manual arrange: pick two slots to swap */
    ITEM_STATE_KEY_VIEW  /* a key item's picture and description */
};

enum { ITEM_TAB_USE, ITEM_TAB_ARRANGE, ITEM_TAB_KEY }; /* ItemMenu.tab */

enum { KEY_VIEW_NONE, KEY_VIEW_OPENING, KEY_VIEW_SHOWN, KEY_VIEW_CLOSING }; /* ItemMenu.keyViewPhase */
#define KEY_VIEW_OPEN_STEPS 0x16 /* s_menuItemKeyItemSlide steps of the slide in */
#define KEY_VIEW_CLOSE_STEPS 8 /* and of the slide out */
#define ITEM_KWEH_FRAMES 0x60 /* "Kweeeh!" popup shown this long, then the menu closes */
#define ITEM_NEW_BLINK_PERIOD 48 /* key item "NEW" marker: blink cycle */
#define ITEM_NEW_BLINK_OFF 0x10 /* frames of the cycle it is hidden */

/* Item screen layout (screen coordinates unless noted) */
#define ITEM_TABS_WIN_X 8 /* Use / Arrange / Key tab window */
#define ITEM_TABS_WIN_Y 8
#define ITEM_TABS_WIN_W 0xE4
#define ITEM_TABS_WIN_H 0x1A
#define ITEM_TAB_TEXT_Y 6 /* tab texts, relative to the window (x: s_menuItemTabX) */
#define ITEM_LIST_WIN_X 8 /* item / key item list window */
#define ITEM_LIST_WIN_Y 0x2A
#define ITEM_LIST_WIN_W 0x12C
#define ITEM_LIST_WIN_H 0xAD
#define ITEM_LIST_ROWS 0xB /* visible rows */
#define ITEM_LIST_COLS 2
#define ITEM_ROW_H 15
#define ITEM_COL_W 0x96
#define ITEM_LIST_LABEL_X 0xE /* "Item" label over each column, relative to the window */
#define ITEM_COUNT_LABEL_R 0x8A /* right edge of the count label over each column, relative to the window */
#define ITEM_ICON_X 0x18 /* row: item icon (key items: the name), relative to the column */
#define ITEM_NAME_X 0x28 /* row: item name */
#define ITEM_COUNT_DX 0x6C /* row: count digits, relative to the icon */
#define ITEM_COUNT_DY 4
#define ITEM_TAB_CURSOR_DX (-0x10) /* cursor on the tabs: relative to s_menuItemTabCursorX */
#define ITEM_TAB_CURSOR_Y 0x10
#define ITEM_LIST_CURSOR_Y 0x34 /* cursor on list row 0 (x: column * ITEM_COL_W) */
#define ITEM_SWAP_CURSOR_X 2 /* Manual arrange: the picked item's cursor */
#define ITEM_SWAP_CURSOR_Y 0x32
#define ITEM_ARRANGE_CURSOR_X 0x7E /* cursor in the Auto / Manual popup */
#define ITEM_ARRANGE_CURSOR_Y 0xE
#define ITEM_ARRANGE_TEXT_X 10 /* Auto / Manual, relative to the popup */
#define ITEM_ARRANGE_TEXT_Y 6
#define ITEM_TARGET_WIN_X_LEFT 8 /* character select window: opens over the column the cursor is not in */
#define ITEM_TARGET_WIN_X_RIGHT 0xA4
#define ITEM_TARGET_WIN_Y 0x26
#define ITEM_TARGET_WIN_W 0x90
#define ITEM_TARGET_WIN_H 0xB6
#define ITEM_TARGET_ROW_X 8 /* member summaries, relative to the window */
#define ITEM_TARGET_ROW_Y 6
#define ITEM_TARGET_ROW_H 0x2B
#define ITEM_TARGET_CURSOR_X (-0xA) /* relative to the window */
#define ITEM_TARGET_CURSOR_Y 0x12
#define ITEM_OTZ_POPUP_CURSOR 0x12 /* cursor over a popup / the target window */
#define ITEM_OTZ_POPUP 0x14 /* target window, Arrange popup, key item card */
#define ITEM_OTZ_KWEH 0xF /* "Kweeeh!" popup */
#define ITEM_OTZ_CURSOR 0x19
#define ITEM_OTZ_TEXT 0x1C /* tab and list contents */
#define ITEM_OTZ_WIN 0x1D /* tab and list windows */
/* key item card (menuItem_drawKeyItem): slides in from the left to x KEY_VIEW_X */
#define KEY_VIEW_X 0x48
#define KEY_VIEW_Y 0x41
#define KEY_VIEW_W 0xB0
#define KEY_VIEW_H 0x80
#define KEY_VIEW_V 0x80 /* card texture v (its tpage is the font page) */
#define KEY_VIEW_CLUT_X 0x3C0 /* card CLUT; the shadow's CLUT is the next row */
#define KEY_VIEW_CLUT_Y 0x1B0
#define KEY_VIEW_SHADOW_D 3 /* drop shadow strips along the bottom and right edges, offset by this */
#define KEY_VIEW_SHADOW_W 6
#define KEY_VIEW_NAME_Y 0x50 /* key item name, centred on the card */
#define KEY_VIEW_DESC_X 0xD /* description, relative to the card */
#define KEY_VIEW_DESC_Y 0x66

/* g_itemMenuText ids */
#define ITEM_TEXT_USE          0   /* "Use", "Arrange", "Key": the tabs, + ITEM_TAB_* */
#define ITEM_TEXT_AUTO         3   /* "Auto", "Manual" */
#define ITEM_TEXT_TAB_HELP     5   /* help of the tabs, + ITEM_TAB_* */
#define ITEM_TEXT_SELECT_CHAR  8   /* "Select character." */
#define ITEM_TEXT_ARRANGE_HELP 9   /* help of Auto / Manual, + cursor */
#define ITEM_TEXT_KWEH         0xB /* "Kweeeh!" (menuItem_callChocobo) */

/* Items menu state (s_menuItem). */
typedef struct {
    u8 tab;           /* ITEM_TAB_*, index into s_menuItemTabs */
    u8 state;         /* ITEM_STATE_* */
    u8 slot;          /* party slot of the target */
    u8 targetKind;    /* g_usableItemTable target kind of the item being used */
    u8 targetMode;    /* g_usableItemTable bits 5-7: 0 = use at once, else pick a character (cursor mode) */
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
    u8 keyViewStep;   /* step in s_menuItemKeyItemSlide */
    s32 keyViewX;     /* x of the view, moved by the s_menuItemKeyItemSlide deltas */
    u8 pad5C[4];
    u8 keyItems[KEY_ITEM_COUNT]; /* key item ids the party has */
    s32 keyCount;
} ItemMenu;
extern ItemMenu s_menuItem;

typedef struct {
    void (*select)(s32, MenuList *);           /* Cross on an entry */
    void (*drawRow)(s32, s32, s32, MenuView *); /* one entry */
    MenuList *list;
} ItemList; /* 12 bytes */
extern ItemList s_menuItemTabs[3]; /* per ItemMenu.tab */
extern s8 s_menuItemKeyItemSlide[24];
extern void (*s_menuItemInput[])(void);
extern u8 s_menuItemTargetClose[2][0x10];
extern RECT s_menuItemKeyClutRect;
extern RECT s_menuItemKeyImageRect;
extern u8 s_menuItemKeyImage[];

extern s32 s_menuItemTabX[3];
extern u8 s_menuItemTabText[3];
extern RECT s_menuItemArrangeRect;

/* main exe: candidates for ff9.h */
#define WIN_TITLE_X 6    /* default Window.titleX / titleY (win_init): a window label's offset from its box */
#define WIN_TITLE_Y (-3)
void help_nullHandler(void); /* empty function, passed to help_setCallback as a no-op callback */
extern s32 g_menuScreenFrame; /* = g_menuScreen.unk4, referenced by its own name */
typedef struct {
    unsigned int targetKind : 4; /* target kind (s_menuItem.targetKind) */
    unsigned int unk0_4 : 1;
    unsigned int targetMode : 3;
    unsigned int pad0_8 : 24;
    u8 effect; /* battle formula (BTL_FORMULA_*), item_useOnMember's kind */
    u8 pad5[3];
    s32 statuses; /* statuses added or removed (ovl_0b Unk8006B1F4.statuses: btl_addStatuses, btl_removeStatuses); not read in ovl_37 */
} Unk8006B1F4; /* 0xC, items 0xE0.. (also in ovl_0b.h) */
extern Unk8006B1F4 g_usableItemTable[];
u8 item_useOnMember(Character *, Character *, Unk8006B1F4 *, s32, s32);
extern s32 g_labelArrange;
void menuItem_onInit(void);
void menuItem_collectKeyItems(void);
void menuItem_loadImages(void);
void menuItem_drawScrollRows(void);
extern RECT *s_menuItemListRect;
extern RECT s_menuItemScrollClip;

extern u8 s_menuItemTargetOpen[2][0x10];
extern RECT s_menuItemKwehRect;
extern u8 s_menuItemChocoboTitle[];
extern RECT s_menuItemArrangeHelpBounds;
extern u8 s_menuItemHelp[]; /* item help text */
extern u8 s_menuItemKeyDesc[]; /* key item descriptions */
extern s32 s_menuItemTabCursorX[3]; /* list cursor x per list */
extern u8 s_menuItemKeyHelp[]; /* key item help text */
s32 menuItem_getSlot(MenuList *);

/* other overlay */
s8 wm_canCallChocobo(void);
void wm_callChocobo(void);

extern ItemSlot s_menuItemSortBuf[0x100]; /* inventory sort buffer */
extern ItemSlot s_menuItemSortTmp; /* swap temp for the inventory sort */

void menuItem_inputTabs(void);
void menuItem_openTarget(void);
void menuItem_useOnTarget(s32);
void menuItem_closeTarget(void);
u8 keyItem_isSeen(s32); /* main exe */
void ui_drawItemIcon(s32, s32, s32, s32, u8); /* main exe */
void menuItem_onKwehOpened(Window *);
void menuItem_drawKweh(Window *);
void menuItem_openArrange(void);
void keyItem_setSeen(s32); /* main exe */
void ui_drawMemberSummary(s32, s32, s32, s32, s32); /* main exe */
void menuItem_sortAuto(void);
void menuItem_selectRow(void);
void menuItem_closeArrange(void);
void menuItem_drawArrangeWindow(Window *);
void menuItem_onArrangeOpened(Window *);
void menuItem_onArrangeClosed(Window *);
void menuItem_inputTarget(void);
void menuItem_inputManualArrange(void);
void menuItem_selectItem(s32, MenuList *);
void menuItem_drawList(void);
void menuItem_drawCursor(void);
void menuItem_updateHelp(void);
void menuItem_updateKeyItemAnim(void);
void menuItem_drawKeyItem(void);
void menuItem_input(void);
void menuItem_draw(void);
void menuItem_updateAnims(void);
void menuItem_inputList(void);
void menuItem_inputArrange(void);
void menuItem_inputKeyItem(void);
void menuItem_selectKeyItem(s32);
void menuItem_drawTabs(void);
void menuItem_drawTargetWindow(void);
void menuItem_drawRow(s32, s32, s32, MenuView *);
void menuItem_drawKeyRow(s32, s32, s32, MenuView *);
void menuItem_stepKweh(void);
void menuItem_restoreVram(void);
void menuItem_showKeyItem(s32);
void menuItem_hideKeyItem(void);

extern const u8 g_itemMenuText[]; /* text bank for this menu */

#endif
