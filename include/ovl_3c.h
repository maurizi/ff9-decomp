#ifndef OVL_3C_H
#define OVL_3C_H

/* Declarations local to overlay 0x3C (the shop, menu type 2: a Buy/Sell/Exit or
 * Synth/Exit menu that runs the Buy, Sell or Synth mode, loaded at 0x801EDF00). Overlay addresses mean
 * different things in different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* Shop.kind (shop_getKind): row of s_shopModes */
enum { SHOP_KIND_ITEMS, SHOP_KIND_EQUIPMENT, SHOP_KIND_SYNTH };
#define SHOP_ID_SYNTH   0x20 /* shop ids from here on are synthesis shops */
#define SHOP_LIST_SIZE  0x20 /* items per s_shopItems entry, recipes per synthesis shop */
#define SHOP_RECIPE_COUNT 0x40
#define SHOP_MODE_COUNT 3    /* s_shopModes options per kind (the last menu option, Exit, has none) */
enum { SHOP_MODE_INIT, SHOP_MODE_UPDATE, SHOP_MODE_EXIT }; /* s_shopModes[kind][option][] */
#define SHOP_OTZ_MENU 5
#define SHOP_OTZ_MODE 0x24
#define SHOP_OTZ_SHADOW 0x28 /* title window shadow */
#define SHOP_OTZ_QTY 0xA /* quantity popup (its cursor one slot in front) */
#define SHOP_OTZ_CURSOR 0x14
#define SHOP_OTZ_PARTY_LABEL 0x1D /* party window label (s_shopBuyPartyLabel: weapon or armour) */
#define SHOP_OTZ_TEXT 0x1E /* buy / sell / synth window contents */
#define SHOP_OTZ_WIN 0x23 /* buy / sell / synth windows */
#define SHOP_PORTRAIT_COUNT 0xD /* portraits in s_shopImage (ui_loadPortraits) */

/* Shop screen layout (screen coordinates unless noted). Buy and Synth share it. */
#define SHOP_TITLE_TEXT_X 0xA /* shop title, relative to s_shopTitleRect */
#define SHOP_TITLE_TEXT_Y 6
#define SHOP_ROW_H 0xF /* list and menu row height */
#define SHOP_MENU_CURSOR_X 0x6E /* Buy / Sell / Exit menu cursor */
#define SHOP_MENU_CURSOR_Y 0x6A
#define SHOP_LIST_WIN_X 8 /* item / recipe list */
#define SHOP_LIST_WIN_Y 0x26
#define SHOP_LIST_WIN_W 0xB4
#define SHOP_LIST_WIN_H 0xAD /* full height (item shop) */
#define SHOP_LIST_WIN_H_EQUIP 0x71 /* with the party window below (a shop that sells equipment) */
#define SHOP_LIST_ROWS 0xB
#define SHOP_LIST_ROWS_EQUIP 7
#define SHOP_LIST_LABEL_X 0xE /* "Item" label */
#define SHOP_PRICE_LABEL_R 0xA6 /* right edge of the "Price" label */
#define SHOP_ROW_ICON_X 0x18 /* row (screen x): item icon */
#define SHOP_ROW_NAME_DX 0x10 /* name after the icon */
#define SHOP_ROW_PRICE_DX 0x72 /* price digits after the icon */
#define SHOP_LIST_CURSOR_LEFT 8 /* list cursor: left of the window */
#define SHOP_LIST_CURSOR_DY 0xB
#define SHOP_INFO_WIN_X 0xBC /* INFO window: gil, owned, equipped (same height as the list) */
#define SHOP_INFO_WIN_Y 0x26
#define SHOP_INFO_WIN_W 0x78
#define SHOP_INFO_TEXT_X 8 /* row labels (y from s_shopBuyInfoRowY / s_shopSynthInfoRowY, text 4 higher) */
#define SHOP_GIL_X 0x39 /* gil amounts (7 digits + "G"), here and in the quantity window */
#define SHOP_COUNT_X 0x5C /* counts (2 digits), here and in the quantity window */
#define SHOP_SYNTH_DIV_Y 0x36 /* synth INFO: etched divider above the ingredients */
#define SHOP_INGREDIENT_ICON_X 0xC /* synth INFO: ingredient rows (name 0x10 right, 1 lower) */
#define SHOP_INGREDIENT_Y 0x4C
#define SHOP_INGREDIENT_ROW_H 17
#define SHOP_PARTY_WIN_X 8 /* party window: who can equip the item, with the stat change */
#define SHOP_PARTY_WIN_Y 0x97
#define SHOP_PARTY_WIN_W 0x12C
#define SHOP_PARTY_WIN_H 0x45
#define SHOP_PARTY_X 0xF /* first portrait */
#define SHOP_PARTY_Y 0x9E
#define SHOP_PARTY_COL_W 0x24
#define SHOP_PORTRAIT_H 0x2D
#define SHOP_STAT_Y 0x2E /* stat icon under the portrait (digits SHOP_STAT_DIGITS_X right, 1 lower) */
#define SHOP_STAT_DIGITS_X 0xC
#define SHOP_STAT_UP_Y 0x27 /* SHOP_ICON_UP / SHOP_ICON_DOWN above it */
#define SHOP_STAT_DOWN_Y 0x28
#define SHOP_QTY_WIN_X 0x62 /* quantity popup (buy / synth; sell's is higher) */
#define SHOP_QTY_WIN_Y 0x60
#define SHOP_QTY_WIN_W 0x7C
#define SHOP_QTY_WIN_H 0x45
#define SHOP_QTY_ICON_X 8 /* relative to the popup */
#define SHOP_QTY_NAME_X 0x18
#define SHOP_QTY_ITEM_Y 8
#define SHOP_QTY_DIV_Y 0x18
#define SHOP_QTY_TEXT_X 0xC /* "Quantity" / "Price" (values 4 lower) */
#define SHOP_QTY_QUANTITY_Y 0x1E
#define SHOP_QTY_PRICE_Y 0x2E
#define SHOP_QTY_CURSOR_LEFT 0xE /* quantity cursor: left of the popup, at the quantity */
#define SHOP_SELL_WIN_X 8 /* sell list: two columns over the whole width */
#define SHOP_SELL_WIN_Y 0x2A
#define SHOP_SELL_WIN_W 0x12C
#define SHOP_SELL_WIN_H 0xAD
#define SHOP_SELL_ROWS 0xB
#define SHOP_SELL_COLS 2
#define SHOP_SELL_COL_W 0x96
#define SHOP_SELL_ICON_X 0x18 /* row, relative to the column */
#define SHOP_SELL_NAME_X 0x28
#define SHOP_SELL_COUNT_X 0x84
#define SHOP_SELL_COUNT_LABEL_R 0x8A /* right edge of each column's count label */
#define SHOP_SELL_CURSOR_Y 0x34 /* sell list cursor on row 0 (x: column * SHOP_SELL_COL_W) */
#define SHOP_SELL_QTY_WIN_Y 0x40 /* sell quantity popup */
#define SHOP_SELL_INFO_WIN_Y 0x87 /* sell INFO popup under it */
#define SHOP_SELL_INFO_WIN_H 0x3E
#define SHOP_FADE_OTZ 7      /* fade_setOtz while the shop is open */

/* ShopBuy/ShopSynth/ShopSell.state: index into their input tables */
enum { SHOP_ST_NONE, SHOP_ST_LIST, SHOP_ST_QUANTITY };
#define SHOP_QUANTITY_STEP 10 /* Up/Down in the quantity window */
#define ITEM_MAX_COUNT 99
#define GIL_MAX        9999999

/* g_shopText ids */
#define SHOP_TEXT_TITLE        0    /* + kind: "Shop", "Weapons", "Synth" */
                                    /* 3-5 "Buy" "Sell" "Exit", 6-7 "Synth" "Exit": s_shopMenuText[kind] + option */
                                    /* 8-0xC: their help, s_shopMenuHelpText[kind] + option */
#define SHOP_TEXT_WELCOME      0xD  /* "Welcome!" */
#define SHOP_TEXT_ANYTHING_ELSE 0xE /* "Anything else I can help you with?" */
#define SHOP_TEXT_BUY_WHAT     0xF  /* "What would you like to buy?" */
#define SHOP_TEXT_SELL_WHAT    0x10 /* "What would you like to sell?" */
#define SHOP_TEXT_SYNTH_WHAT   0x11 /* "What would you like to synthesize?" */
#define SHOP_TEXT_BUY_HOW_MANY 0x12 /* "How many?" (buy), 0x13 sell, 0x14 synthesis */
#define SHOP_TEXT_SELL_HOW_MANY 0x13
#define SHOP_TEXT_SYNTH_HOW_MANY 0x14
                                    /* 0x15-0x18 "Funds" "Stock" "Equipped" "Required Item": INFO rows */
#define SHOP_TEXT_QUANTITY     0x19 /* "Quantity" */
#define SHOP_TEXT_PRICE        0x1A /* "Price" */
/* g_shopBuyText / g_shopSellText id 3, g_shopSynthText id 4: "Select quantity" help of the quantity window */
#define SHOP_BUY_TEXT_QUANTITY_HELP   3
#define SHOP_SELL_TEXT_QUANTITY_HELP  3
#define SHOP_SYNTH_TEXT_QUANTITY_HELP 4

/* s_shopIcons entries (shop_drawStatIcon): 0-4 per equip slot, then the comparison arrows */
#define SHOP_ICON_UP   5
#define SHOP_ICON_DOWN 6
#define EQUIP_SLOT_ACCESSORY 4 /* item_getEquipSlot: 0 weapon, 1-3 armor, 4 accessory (no stat shown) */

/* Shop state (s_shop). */
typedef struct {
    u8 kind;       /* SHOP_KIND_* */
    u8 cursor;     /* menu option: an s_shopModes entry, the last one is Exit */
    u8 entryCount; /* menu options (shop_countEntries) */
    u8 inMode;     /* a mode (Buy/Sell/Synth) is running */
    u8 visits;     /* modes entered: text SHOP_TEXT_ANYTHING_ELSE instead of SHOP_TEXT_WELCOME */
    u8 pad5[3];
    s32 id;        /* shop id (g_menu.arg when the menu opens) */
    s32 savedOtz;  /* fade_setOtz (fade otz) before the shop set its own */
    u8 *help;      /* message of the top window (shop_setHelp) */
    s32 memberCount;
    s8 members[8]; /* character indices for the stat comparison (shop_collectMembers) */
    s32 messageOtz; /* otz of the message window: SHOP_OTZ_MENU, SHOP_OTZ_MODE in a mode */
} Shop;
extern Shop s_shop;

/* Buy menu (s_shopBuy). */
typedef struct {
    u8 state;       /* SHOP_ST_*, index into s_shopBuyInputFuncs */
    u8 count;       /* items in the list */
    u8 quantity;
    u8 maxQuantity;
    u8 hasEquipment; /* the list has equipment: shorter list, party window */
    u8 pad5[3];
    RECT infoRect;     /* shopBuy_drawInfo */
    RECT listRect;     /* shopBuy_drawList */
    RECT partyRect;    /* shopBuy_drawParty */
    RECT quantityRect; /* quantity window */
    MenuList list;
    Window *quantityWin;
    u8 *items; /* s_shopItems[id] */
} ShopBuy;
extern ShopBuy s_shopBuy;

typedef struct {
    u16 price;
    u8 ingredients[2]; /* item ids, ITEM_NONE = unused */
    u8 product;        /* item id */
    u8 shopMask;       /* bit (shop id - SHOP_ID_SYNTH) per synthesis shop that offers it */
} SynthRecipe;
extern SynthRecipe s_shopRecipes[64];

/* Synthesis menu (s_shopSynth). */
typedef struct {
    u8 state;       /* SHOP_ST_*, index into s_shopSynthInputFuncs */
    u8 count;       /* recipes in the list */
    u8 quantity;
    u8 maxQuantity;
    u8 hasEquipment; /* a product is equipment: shorter list, party window */
    u8 pad5[3];
    RECT infoRect;     /* shopSynth_drawInfo */
    RECT listRect;     /* shopSynth_drawList */
    RECT partyRect;    /* shopSynth_drawParty */
    RECT quantityRect; /* quantity window */
    MenuList list;
    Window *quantityWin;
    SynthRecipe recipes[SHOP_LIST_SIZE];
    u8 padFC[4];
} ShopSynth;
extern ShopSynth s_shopSynth;

/* Sell menu (s_shopSell). */
typedef struct {
    u8 state;       /* SHOP_ST_*, index into s_shopSellInputFuncs */
    u8 quantity;
    u8 maxQuantity;
    u8 pad3;
    MenuList list;  /* rows of 2 inventory slots */
    RECT listRect;
    RECT winRects[2]; /* QUANTITY, INFO */
    Window *wins[2];
} ShopSell;
extern ShopSell s_shopSell;

typedef struct {
    s32 style; /* win_openFree 3rd argument */
    void (*draw)(Window *);
    s32 otz;
    s32 label; /* index into g_winLabelStrings */
} ShopWinDef;
extern ShopWinDef s_shopSellWindows[2];
void shopSell_onQuantityOpened(Window *);
void shopSell_onQuantityClosed(Window *);

extern void (*s_shopBuyInputFuncs[])(void);
extern void (*s_shopSynthInputFuncs[])(void);
extern void (*s_shopSellInputFuncs[])(void);
/* item names: data, splat starts .data at 0xA8F0 instead of 0xA8EC */
extern u8 s_shopItemHelp[];

/* {init, update, exit} per shop kind and option */
extern void (*s_shopModes[][3][3])(void);
extern RECT s_shopTitleRect;
extern RECT s_shopMessageRect;
extern RECT s_shopMenuRect;
extern u8 s_shopMenuText[];
extern RECT s_shopHelpRect;
extern u8 s_shopMenuHelpText[];
extern RECT s_shopIconClutRect;
extern RECT s_shopIconRect;
extern u8 s_shopItems[][0x20]; /* item list per shop, 0xFF = none */
extern u8 s_shopIconImage[];
extern u8 g_shopText[];
extern u8 s_shopBuyQuantityGil[]; /* gil suffix */
extern u8 s_shopSynthQuantityGil[]; /* gil suffix */
extern RECT *s_shopSellListRectPtr;
extern u8 s_shopSellQuantityGil[]; /* gil suffix */
void shopSell_drawScrollRows(void);
typedef struct {
    RECT r[7];
} RectTbl7;
extern RectTbl7 s_shopIcons; /* icon source rects */
extern u8 s_shopImage[];

extern u8 g_shopBuyText[];
/* main exe: candidates for ff9.h */
#define WIN_TITLE_X 6    /* default Window.titleX / titleY (win_init): a window label's offset from its box */
#define WIN_TITLE_Y (-3)
s32 item_getEquipSlot(s32); /* equip slot of an item, -1 = not equipment */
s32 chr_getPortrait(s32); /* main.c defines it returning u8; used unmasked */
typedef struct {
    u8 v[5];
} Equip5; /* Character.unk39 copied as a whole */
extern s32 s_shopBuyPartyLabel[2]; /* buy: text ids, weapon / armor */
extern u8 s_shopBuyStatIcon[5];  /* buy: icon per equip slot */
extern u16 s_shopBuyEquipMask[];  /* buy: equip mask per character kind */
extern s32 s_shopBuyNoStat;    /* buy: "--" shown for accessories */
extern s32 s_shopSynthPartyLabel[2]; /* synth: as s_shopBuyPartyLabel */
extern u8 s_shopSynthStatIcon[5];
extern u16 s_shopSynthEquipMask[];
extern s32 s_shopSynthNoStat;
s32 item_getCount(s32); /* main.c defines it returning u8 */
s32 item_countEquipped(s32);
extern s32 s_shopBuyInfoRowY[3]; /* row y offsets */
extern u8 s_shopBuyInfoRowAlways[4];
extern u8 s_shopBuyInfoGil[]; /* gil suffix */
extern s32 s_shopBuyInfoRowText[3]; /* row label text ids */
extern s32 s_shopSynthInfoRowY[4];
extern u8 s_shopSynthInfoRowAlways[4];
extern u8 s_shopSynthInfoGil[]; /* gil suffix */
extern s32 s_shopSynthInfoRowText[4];
extern s32 s_shopSellInfoRowY[3]; /* sell: row y offsets */
extern u8 s_shopSellInfoRowAlways[4];
extern u8 s_shopSellInfoGil[]; /* gil suffix */
extern s32 s_shopSellInfoRowText[3]; /* sell: row label text ids */
void ui_drawItemIcon(s32, s32, s32, s32, u8);
extern u8 g_shopSynthText[];
extern u8 g_shopSellText[];
void shopSynth_loadRecipes(void);
void shopBuy_updateHelp(void);
void shopSynth_updateHelp(void);
void shopSell_updateHelp(void);
s32 shop_sumArmorStat(s32, u8 *);
typedef struct {
    u8 defense;
    u8 evade;
    u8 magicDef;
    u8 magicEvade;
} Unk8006D328; /* equipment stat bonuses, from item 0x58 */
extern Unk8006D328 g_armorStatBonus[];

void shop_drawMessage(void);
void shop_inputMenu(void);
void shop_drawMenu(void);
void shop_setHelp(s32);
void shop_drawTitle(void);
void shop_drawMenuAll(void);
void shop_onUpdate(void);
void shop_drawMenuCursor(void);
void shop_onModeLeft(void);
s32 shop_countEntries(s32);
void shop_selectMenu(void);
void shop_updateHelp(void);
void shop_close(void);
void shop_loadImages(void);
s32 shop_getKind(s32);
void shop_enterMode(void);
void shop_onModeEntered(void);
void shopBuy_drawInfo(void);
void shopBuy_drawList(void);
void shopBuy_drawParty(void);
void shopBuy_drawCursor(void);
s32 shop_isEquipmentShop(s32);
void shopBuy_input(void);
void shopBuy_draw(void);
void shopBuy_openQuantity(void);
void shopBuy_drawRow(s32, s32, s32, MenuView *);
void shopBuy_closeQuantity(void);
void shopBuy_drawScrollRows(void);
void shopBuy_drawQuantityWindow(Window *);
void shopBuy_onQuantityOpened(Window *);
void shopBuy_onQuantityClosed(Window *);
extern s32 g_labelQuantity;
void shopSynth_confirm(void);
void shopSell_closeQuantity(void);
void shopSynth_drawScrollRows(void);
void shopSynth_drawInfo(void);
void shopSynth_drawList(void);
void shopSynth_drawParty(void);
void shopSynth_drawCursor(void);
void shopSynth_drawRow(s32, s32, s32, MenuView *);
void shopSynth_openQuantity(void);
void shopSynth_drawQuantityWindow(Window *);
void shopSynth_onQuantityOpened(void);
void shopSynth_onQuantityClosed(void);
#define MIN(a, b) ((a) < (b) ? (a) : (b))
void shopSynth_input(void);
void shopSynth_draw(void);
void shopSynth_updateList(void);
void shopSynth_closeQuantity(void);
void shopSell_drawList(void);
void shopSell_drawCursor(void);
void shopSell_openQuantity(void);
void shopSell_input(void);
void shopSell_draw(void);
void shopSell_updateList(void);
void shopSell_drawRow(s32, s32, s32, MenuView *);
s32 shop_getId(void);
void shop_leaveMode(void);
void shopBuy_updateList(void);

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void shopBuy_confirm(void);
void shopBuy_loadList(void);
void shopSell_confirm(void);

#endif
