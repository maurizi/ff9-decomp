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
#define SHOP_FADE_OTZ 7      /* fade_setOtz while the shop is open */

/* ShopBuy/ShopSynth/ShopSell.state: index into their input tables */
enum { SHOP_ST_NONE, SHOP_ST_LIST, SHOP_ST_QUANTITY };
#define SHOP_QUANTITY_STEP 10 /* Up/Down in the quantity window */
#define ITEM_NONE      0xFF
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
#ifndef SE_CURSOR
#define SE_CURSOR 0x67  /* cursor move / pick */
#define SE_BUZZER 0x66  /* refused action */
#define SE_CANCEL 0x65  /* Circle backs out */
#endif
#define SE_SHOP 0x415   /* Cross in a shop quantity window: buy / sell / synthesize */
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
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Unk8006D328; /* equipment stat bonuses, from item 0x58 */
extern Unk8006D328 g_armorStatBonus[];

void shop_drawMessage(void);
void shop_inputMenu(void);
void shop_drawMenu(void);
void shop_setHelp(s32);
void shop_drawTitle(void);
void shop_drawMenuAll(void);
void shop_updateStub(void);
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
