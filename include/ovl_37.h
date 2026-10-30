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

#define ITEM_SLOT_COUNT   0x100 /* GameData.items slots */
#define KEY_ITEM_COUNT    0x50  /* key item ids (keyItem_has) */
#define ITEM_FIRST_USABLE 0xE0  /* items from here have a use entry in D_8006B1F4 */

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
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#endif
#define SE_USE_ITEM 0x6A /* an item was used (menuItem_useOnTarget) */
#ifndef ITEM_NONE
#define ITEM_NONE 0xFF /* empty item id */
#endif
void help_nullHandler(void); /* empty function, passed to help_setCallback as a no-op callback */
extern s32 g_menuScreenFrame; /* = g_menuScreen.unk4, referenced by its own name */
typedef struct {
    unsigned int targetKind : 4; /* target kind (s_menuItem.targetKind) */
    unsigned int unk0_4 : 1;
    unsigned int targetMode : 3;
    unsigned int pad0_8 : 24;
    u8 effect; /* calculation script, item_useOnMember's kind */
    u8 pad5[3];
    s32 unk8;
} Unk8006B1F4; /* 0xC, items 0xE0.. (also in ovl_0b.h) */
extern Unk8006B1F4 D_8006B1F4[];
u8 item_useOnMember(Character *, Character *, Unk8006B1F4 *, s32, s32);
extern s32 D_80070408;
void func_801F3038(void);
void menuItem_collectKeyItems(void);
void menuItem_loadImages(void);
void menuItem_drawScrollRows(void);
extern RECT *s_menuItemListRect;
extern RECT s_menuItemScrollClip;

extern u8 s_menuItemTargetOpen[2][0x10];
extern RECT s_menuItemKwehRect;
extern u8 D_801F3140[];
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
void func_801F2F30(void);
void menuItem_showKeyItem(s32);
void menuItem_hideKeyItem(void);

extern const u8 g_itemMenuText[]; /* text bank for this menu */

#endif
