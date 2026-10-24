#ifndef OVL_3C_H
#define OVL_3C_H

/* Declarations local to overlay 0x3C (shop menus: a top-level hub that runs
 * one of several sub-menus, loaded at 0x801EDF00). Overlay addresses mean
 * different things in different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* Shop hub state (s_shop). */
typedef struct {
    u8 unk0; /* shop kind */
    u8 unk1; /* cursor */
    u8 unk2; /* option count */
    u8 unk3; /* sub-menu running */
    u8 unk4; /* sub-menu has been used */
    u8 pad5[3];
    s32 unk8;  /* shop id (D_80077DBC) */
    s32 unkC;  /* saved func_8002F8E4 mode */
    u8 *unk10; /* title text */
    s32 unk14; /* party member count */
    s8 unk18[8]; /* party member indices */
    s32 unk20;   /* OT layer */
} ShopHub;
extern ShopHub s_shop;

/* Buy menu (s_shopBuy). */
typedef struct {
    u8 unk0; /* state, index into s_shopBuyInputFuncs */
    u8 unk1; /* item count */
    u8 unk2; /* quantity */
    u8 unk3; /* max quantity */
    u8 unk4; /* sells equipment */
    u8 pad5[3];
    RECT unk8;
    RECT unk10;
    RECT unk18;
    RECT unk20;
    MenuList unk28;
    Window *unk38;
    u8 *unk3C; /* item list */
} ShopBuy;
extern ShopBuy s_shopBuy;

typedef struct {
    u16 unk0; /* price */
    u8 unk2[2]; /* ingredients */
    u8 unk4;  /* result */
    u8 unk5;  /* shop mask */
} SynthRecipe;
extern SynthRecipe s_shopRecipes[64];

/* Synthesis menu (s_shopSynth). */
typedef struct {
    u8 unk0; /* state, index into s_shopSynthInputFuncs */
    u8 unk1;
    u8 unk2; /* quantity */
    u8 unk3; /* max quantity */
    u8 unk4;
    u8 pad5[3];
    RECT unk8;
    RECT unk10;
    RECT unk18;
    RECT unk20;
    MenuList unk28;
    Window *unk38;
    SynthRecipe unk3C[32];
    u8 padFC[4];
} ShopSynth;
extern ShopSynth s_shopSynth;

/* Sell menu (s_shopSell). */
typedef struct {
    u8 unk0; /* state, index into s_shopSellInputFuncs */
    u8 unk1; /* quantity */
    u8 unk2; /* max quantity */
    u8 pad3;
    MenuList unk4;
    RECT unk14;
    RECT unk1C[2];
    Window *unk2C[2];
} ShopSell;
extern ShopSell s_shopSell;

typedef struct {
    s32 unk0; /* OT layer */
    void (*unk4)(Window *);
    s32 unk8;
    s32 unkC; /* text index into D_800703FC */
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
s32 func_80028014(s32); /* equip slot of an item, -1 = not equipment */
s32 func_80028954(s32); /* main.c defines it returning u8; used unmasked */
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
s32 func_80027F94(s32);
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
extern Unk8006D328 D_8006D328[];

void shop_drawMessage(void);
void shop_inputMenu(void);
void shop_drawMenu(void);
void shop_setHelp(s32);
void shop_drawTitle(void);
void shop_drawMenuAll(void);
void func_801F47F0(void);
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
extern s32 D_800704A4;
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

#endif
