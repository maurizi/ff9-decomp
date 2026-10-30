#ifndef OVL_33_H
#define OVL_33_H

/* Declarations local to overlay 0x33 (equipment and ability menus, loaded at
 * 0x801EDF00). Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

typedef struct {
    u8 id;  /* action ability id */
    u8 cmd; /* the command it belongs to, index into s_menuAbilityCmds (menuAbility_findCommand) */
    u8 pad2[2];
} ActionRow;

/* battle command table entry (a copy of the battle's) */
typedef struct {
    u16 name;  /* name offset into s_menuAbilityCmdNames */
    u8 pad2[4];
    u8 kind;   /* MENU_CMD_ABILITIES: count abilities from s_menuAbilityCmdAbilities[first] */
    u8 count;
    s32 first; /* first index into s_menuAbilityCmdAbilities */
} MenuCmd; /* 0xC */
#define MENU_CMD_ABILITIES 1
extern MenuCmd s_menuAbilityCmds[];
extern u8 s_menuAbilityCmdNames[];
extern u8 s_menuAbilityCmdAbilities[];
extern u8 s_menuAbilityCharCmds[][2];

/* main-menu screens (g_menuScreen.screen, menuScreen_switch) */
#define MENU_SCREEN_ABILITY 2
#define MENU_SCREEN_EQUIP   3
#define MENU_NO_SLOT        0xFF /* party slot before the first member is shown */

#define ABILITY_MAX           0x30 /* entries in a character's g_abilityRows ability row */
#define ABILITY_SUPPORT_FIRST 0xC0 /* ability ids from here are support abilities (g_supportAbilities) */
#define SUPPORT_COUNT         0x40
#define EQUIP_SLOT_COUNT      5    /* Character.equip: weapon first (it can't be removed) */

/* ability states (menuAbility_getActionState, menuAbility_getSupportState), index into s_menuAbilityStateColor */
#define ABILITY_HIDDEN   0 /* not learnable from the equipment, or no AP yet */
#define ABILITY_GREYED   2 /* can't be used / equipped now */
#define ABILITY_USABLE   3
#define ABILITY_EQUIPPED 4 /* support ability turned on */

/* Ability screen */
enum { ABILITY_TAB_ACTION, ABILITY_TAB_SUPPORT }; /* "Use", "Equip" */
enum {
    ABILITY_STATE_BUSY,   /* no input */
    ABILITY_STATE_TABS,
    ABILITY_STATE_LIST,
    ABILITY_STATE_TARGET, /* target window */
    ABILITY_STATE_WAIT,   /* target window sliding */
    ABILITY_STATE_SWITCH  /* L1/R1: header sliding to another member */
};
#define ABILITY_TARGET_ALL 0x80 /* AbilityMenu.target: all members */

/* g_abilityMenuText (Ability screen text) ids */
#define ABILITY_TEXT_TAB_HELP          2   /* help of each tab, + ABILITY_TAB_* */
#define ABILITY_TEXT_STATUS_BLOCKS     4   /* "They cannot be used if you are Petrified or Silenced." */
#define ABILITY_TEXT_CANNOT_USE        5   /* "This character cannot use this." */
#define ABILITY_TEXT_NOT_HERE          6   /* "You cannot use this at this location." */
#define ABILITY_TEXT_CANNOT_EQUIP_NOW  7   /* "...cannot use this under present conditions." */
#define ABILITY_TEXT_CANNOT_EQUIP      8
#define ABILITY_TEXT_SELECT_CHAR       9   /* "Select character." */
#define ABILITY_TEXT_SELECT_CHAR_ALL   0xA /* the same, with how to toggle single/multiple targets */
#define ABILITY_TEXT_TO_EQUIP          0xF /* "Square (Equip Menu)" */
#define ABILITY_TEXT_AP                0x10
#define ABILITY_TEXT_EQUIP             0x11

/* Ability screen state (s_menuAbility) */
typedef struct {
    u8 tab;           /* ABILITY_TAB_* */
    u8 state;         /* ABILITY_STATE_*, index into s_menuAbilityInput */
    u8 slot;          /* party slot shown */
    u8 target;        /* party slot targeted, | ABILITY_TARGET_ALL */
    u8 tabEnabled[2]; /* 0 = greyed tab */
    u8 pad6[2];
    MenuList lists[2]; /* per tab */
    RECT tabRect;
    RECT playerRect;  /* PLAYER window */
    RECT infoRect;    /* INFO window */
    RECT commandRect; /* COMMAND window */
    RECT listRect;
    RECT targetRect;
    WinDef targetWin;
    u8 equipSlots[0x100];    /* per ability id: bit per equipment slot whose item teaches it */
    u8 equipItems[0x100][5]; /* per ability id: that item, per slot */
    ActionRow actions[ABILITY_MAX];
    u8 supports[SUPPORT_COUNT];
    u8 targetWinOpen;
    u8 actionCount;
    u8 supportCount;
    u8 animTick;      /* animation tick (0-6) */
    u8 animFrame;     /* equipped-support icon frame (0-6) */
    u8 targetMode;    /* target mode of the action (s_menuAbilityActionTable bits 5-7) */
    u8 partySize;
    u8 canTargetAll;  /* menuAbility_canTargetAll */
    UnkBlink header;  /* member header (ui_initMemberSwitch) */
} AbilityMenu;
extern AbilityMenu s_menuAbility;

/* Equip screen */
enum { EQUIP_TAB_EQUIP, EQUIP_TAB_OPTIMIZE, EQUIP_TAB_OFF };
enum {
    EQUIP_STATE_BUSY, /* no input */
    EQUIP_STATE_TABS,
    EQUIP_STATE_SLOT, /* picking an equipment slot */
    EQUIP_STATE_LIST  /* item list */
};
/* the first halfword of EquipMenu, {tab, state}, read as one by menuEquip_drawInfo */
#define EQUIP_TAB_STATE(tab, state) ((state) << 8 | (tab))

/* g_equipMenuText (Equip screen text) ids */
#define EQUIP_TEXT_TAB_HELP    3 /* help of each tab, + EQUIP_TAB_* */
#define EQUIP_TEXT_NOTHING     6 /* "Nothing is equipped." */
#define EQUIP_TEXT_TO_ABILITY  7 /* "Square (Ability Menu)" */
                                 /* 8-16: stat names */

/* Equip screen state (s_menuEquip) */
typedef struct {
    u8 tab;          /* EQUIP_TAB_* */
    u8 state;        /* EQUIP_STATE_*, index into s_menuEquipInput */
    u8 equipSlot;    /* equipment slot under the cursor */
    u8 slot;         /* party slot shown */
    MenuList list;   /* item list */
    RECT tabRect;
    RECT infoRect;   /* INFO window (stats) */
    RECT equipRect;  /* EQUIPMENT window */
    RECT abilityRect; /* ABILITY window */
    RECT listRect;
    ItemSlot items[0x100]; /* items that fit the slot, best first */
    WinDef listWin;
    UnkBlink header; /* member header (ui_initMemberSwitch) */
    u8 pad258[0x25C - 0x258];
    u8 initStones[4]; /* party members' magic stones when the screen opened */
    u8 listOpen;
    u8 savedState;   /* state saved while switching members */
} EquipMenu;
extern EquipMenu s_menuEquip;

typedef struct {
    unsigned int targetKind : 4; /* target kind (menuAbility_canTargetAll) */
    unsigned int unk0_4 : 1;
    unsigned int targetMode : 3; /* s_menuAbility.targetMode */
    unsigned int pad0_8 : 24;
    u8 effect; /* calculation script, item_useOnMember's kind */
    u8 pad5[0xA - 5];
    u8 mpCost; /* menuAbility_getMpCost */
    u8 flags; /* bit 0: usable from the menu; bit 1: clear = AP gauge flag; bit 2: mpCost x4 when D_8007AF02 */
    u8 padC[2];
    u16 nameOffset; /* name offset into s_menuAbilityActionNames */
} Unk801F34EC; /* 0x10 */
extern Unk801F34EC s_menuAbilityActionTable[];

extern void (*s_menuAbilityInput[])(void);
extern void (*s_menuEquipInput[])(void);
extern TweenDef s_menuAbilityTargetCloseR;
extern u8 s_menuEquipHelpBuf[]; /* help text buffer */
extern u8 s_menuEquipHelpArg[]; /* "ABILITY" */
extern u8 s_menuEquipItemHelp[]; /* item help texts */
u8 item_useOnMember(Character *, Character *, Unk801F34EC *, s32, s32);
extern TweenDef s_menuAbilityTargetCloseL;
extern RECT s_menuAbilityClutRect;
extern RECT s_menuAbilityImageRect;
extern Icon s_menuAbilityIcons[];
extern u8 s_menuAbilityImage[];
extern TweenDef s_menuEquipListOpen;
extern TweenDef s_menuEquipListClose;

void menuAbility_updateAnims(void);
void menuAbility_updateHelp(void);
void menuEquip_updateHelp(void);
extern s32 s_menuAbilityTabX[2]; /* title x offsets */
extern s32 s_menuEquipTabX[3];
void menuAbility_drawRow(s32, s32, s32, MenuView *);
void menuAbility_drawActionRow(s32, s32, s32, Character *);
void menuAbility_drawSupportRow(s32, s32, s32, Character *);
void menuEquip_closeList(void);
void menuAbility_selectAction(void);
void menuAbility_toggleSupport(void);
void menuEquip_onMemberSwitched(void);
void menuEquip_setMember(s32, s32);
void menuAbility_setMember(s32, s32);
void menuAbility_onMemberSwitched(void);
void menuAbility_drawScrollRows(void);
void ui_drawApGauge(s32, s32, s32, s32, s32, u8, u8); /* main exe */
s32 chr_getAp(s32, s32); /* main exe */
s32 chr_getApNeeded(s32, s32); /* main exe */
extern u8 D_8007AFD3;
u8 menuAbility_findCommand(s32, Character *);
void menuAbility_initActionCursor(void);
void menuAbility_initSupportCursor(void);
extern s32 D_8007043C; /* main exe */
extern u8 D_801EE644[];
extern u16 s_menuEquipCharMask[]; /* character masks */
extern u8 s_menuEquipSlotMask[]; /* slot masks */
s32 chr_getDefaultSet(Character *); /* main exe (returns u8) */
extern u8 s_menuEquipTaught[0x40];
extern u8 s_menuEquipOptSlotMask[5]; /* slot masks */
extern u16 s_menuEquipOptCharMask[]; /* character masks */
void menuAbility_loadImages(void);
void func_801F1148(void);
void menuEquip_openList(void);
void menuEquip_removeSlot(void);
void menuEquip_buildItemList(void);
void menuEquip_optimize(void);
void menuScreen_switch(s32); /* main exe */
extern u8 g_abilityMenuText[];
s32 menuAbility_canTargetAll(s32);
extern TweenDef s_menuAbilityTargetOpenR;
extern TweenDef s_menuAbilityTargetOpenL;
extern s16 s_menuEquipTabCursorX[3][2]; /* cursor x per column */
extern u8 g_equipMenuText[];
extern s32 s_menuEquipSlotLabel[]; /* text index per party slot */
void menuAbility_useOnTarget(void);
void menuAbility_closeTarget(void);
s32 menuAbility_getActionState(s32, Character *);
u32 menuAbility_getSupportState(s32, Character *);
s32 menuAbility_getMpCost(Unk801F34EC *);
void menuAbility_drawIcon(s32, s32, s32, s32);
extern s32 s_menuAbilityStateColor[]; /* colour by state */
extern u8 s_menuAbilityActionNames[];  /* item names */
extern u8 s_menuAbilitySupportNames[];  /* ability names */
extern u16 s_menuAbilityTabCursorX[][2]; /* cursor x per slot */
/* main exe: candidates for ff9.h */
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused; SE_PAGE is menuList_pageUp/pageDown's
 * sound, also played on L1/R1 member switches. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#define SE_PAGE   0x417
#endif
#define SE_USE_ITEM 0x6A /* an ability was used (as ovl_37's item use) */
#define SE_EQUIP    0x6B /* equipment or a support ability changed */
#ifndef ITEM_NONE
#define ITEM_NONE 0xFF /* empty item id / equipment slot */
#endif
#define ITEM_SLOT_COUNT 0x100 /* GameData.items slots */
void help_nullHandler(void); /* empty function, passed to help_setCallback as a no-op callback */
extern s32 D_80070400;
extern s32 D_800704B8;
extern s32 g_menuScreenFrame; /* = g_menuScreen.unk4, referenced by its own name */
extern s32 D_80070498;
extern s32 D_8007042C;
void ui_drawMemberSummary(s32, s32, s32, s32, s32);
s32 chr_findAbilitySlot(s32, s32);
void ui_drawItemIcon(s32, s32, s32, s32, u8); /* main exe */
void menuEquip_applyEquip(Character *);
void menuEquip_equipSelected(void);
void menuAbility_drawPlayer(void);
void menuAbility_drawInfo(void);
void menuAbility_drawCommand(void);
void menuAbility_drawList(void);
void menuAbility_drawTargetWindow(void);
void menuAbility_drawCursor(void);
void menuAbility_input(void);
void menuAbility_draw(void);
void menuAbility_drawTabs(void);
Icon *menuAbility_getIcon(s32);
void menuEquip_drawInfo(void);
void menuEquip_drawEquipment(void);
void menuEquip_drawAbilities(void);
void menuEquip_drawList(void);
void menuEquip_drawCursor(void);
void menuEquip_dropUnlearnedSupport(Character *);
void menuEquip_input(void);
void menuEquip_draw(void);
void menuEquip_updateAnims(void);
void menuEquip_drawTabs(void);
void menuEquip_drawListRow(s32, s32, s32, MenuView *);

typedef struct {
    u16 helpOffset;
    u16 unk2;
} HelpOfs; /* item help text offsets into s_menuAbilityActionHelp */
extern HelpOfs s_menuAbilityActionHelpOfs[];
extern u8 s_menuAbilityActionHelp[];
extern u8 s_menuAbilitySupportHelp[];
extern u8 s_menuAbilityHelpArg[];
extern u8 s_menuAbilityHelpBuf[];
extern u8 *g_menuHelpText; /* = g_menuHelp.text, referenced by its own name */
typedef struct {
    u8 pad0[4];
    u16 helpOffset;
    u8 pad6[2];
} SupportHelp; /* g_supportAbilities entry: +4 = help text offset into s_menuAbilitySupportHelp */

/* support ability help text */
static inline u8 *menuAbility_updateHelp_suphelp(s32 i) {
    SupportHelp *b = (SupportHelp *)g_supportAbilities;
    i -= ABILITY_SUPPORT_FIRST;
    return s_menuAbilitySupportHelp + b[i].helpOffset;
}

void menuEquip_drawStatLabel(s32 x, s32 y, s32 z, u8 id, s32 color);
extern u8 s_menuEquipArrow[]; /* arrow */
extern u8 s_menuEquipBaseStatText[4]; /* stat names */
extern u8 s_menuEquipBattleStatText[5]; /* attack/defence names */
void menuEquip_drawStatCompare(Unk80032E84Out *cur, Unk80032E84Out *new);

extern Unk801F34EC s_menuEquipActionTable[]; /* action abilities (ability menu), same layout as s_menuAbilityActionTable */
extern u8 s_menuEquipActionNames[]; /* action ability names (ability menu) */
extern u8 s_menuEquipSupportNames[]; /* support ability names (ability menu) */

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
s32 menuAbility_getSlot(MenuList *l);
void menuEquip_drawScrollRows(void);

#endif
