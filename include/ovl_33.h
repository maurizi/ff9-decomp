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
#define ABIL_ICON_ANIM_TICKS 7 /* frames per equipped-support icon frame */
#define ABIL_ICON_ANIM_FRAMES 7 /* s_menuAbilityIcons 0-6: the equipped-support animation */
#define ABIL_ICON_SUPPORT_CLASSIC 7 /* s_menuAbilityIcons: support icon in the Classic window colour */
#define ABIL_ICON_SUPPORT_CLASSIC_NA 8 /* the same, greyed */
#define ABIL_ICON_SUPPORT 9 /* support icon in the Normal window colour */
#define ABIL_ICON_SUPPORT_NA 10 /* the same, greyed */

/* Ability screen layout (screen coordinates unless noted) */
#define ABIL_TABS_WIN_X 8 /* Use / Equip tabs */
#define ABIL_TABS_WIN_Y 8
#define ABIL_TABS_WIN_W 0xE4
#define ABIL_TABS_WIN_H 0x1A
#define ABIL_TAB_TEXT_Y 6 /* relative to the window (x: s_menuAbilityTabX) */
#define ABIL_TAB_CURSOR_LEFT 0x18 /* left of s_menuAbilityTabCursorX */
#define ABIL_TAB_CURSOR_DY 8
#define ABIL_PLAYER_WIN_X 8 /* PLAYER window: member header, status icons, stones */
#define ABIL_PLAYER_WIN_Y 0x26
#define ABIL_PLAYER_WIN_W 0xD0
#define ABIL_PLAYER_WIN_H 0x38
#define ABIL_HEADER_X 8 /* relative to the PLAYER window */
#define ABIL_HEADER_Y 5
#define ABIL_ICONBOX_X 0x72 /* status-ailment icon box, relative to the PLAYER window */
#define ABIL_ICONBOX_Y 4
#define ABIL_ICONBOX_W 0x5B
#define ABIL_ICONBOX_H 0x12
#define ABIL_STONES_DX 0x19 /* magic stones gauge, relative to the icon box */
#define ABIL_STONES_DY 0x14
#define ABIL_INFO_WIN_X 0xD8 /* INFO window: AP gauge and the equipment teaching the ability */
#define ABIL_INFO_WIN_Y 0x26
#define ABIL_INFO_WIN_W 0x5C
#define ABIL_INFO_WIN_H 0x38
#define ABIL_INFO_TEXT_X 8 /* "AP" / "Equip" labels and the item icons, relative to the window */
#define ABIL_INFO_AP_Y 8
#define ABIL_INFO_EQUIP_Y 0x16
#define ABIL_INFO_GAUGE_X 0x2D
#define ABIL_INFO_GAUGE_Y 0xC
#define ABIL_INFO_ITEMS_Y 0x24
#define ABIL_INFO_ITEM_W 0x10 /* item icon spacing */
#define ABIL_CMD_WIN_X 0x8E /* COMMAND window: the command an action ability belongs to */
#define ABIL_CMD_WIN_Y 0x40
#define ABIL_CMD_WIN_W 0x46
#define ABIL_CMD_WIN_H 0x1A
#define ABIL_CMD_TEXT_X 8
#define ABIL_CMD_TEXT_Y 6
#define ABIL_LIST_WIN_X 8 /* ability list (also the top of the help bounds) */
#define ABIL_LIST_WIN_Y 0x5E
#define ABIL_LIST_WIN_W 0x12C
#define ABIL_LIST_WIN_H 0x80
#define ABIL_LIST_ROWS 8 /* visible rows */
#define ABIL_LIST_COLS 2
#define ABIL_ROW_H 15
#define ABIL_COL_W 0x96
#define ABIL_ROW_X 8 /* first column, relative to the window */
#define ABIL_LIST_LABEL_X 0xE /* "Ability" label over each column, relative to the window */
#define ABIL_COST_LABEL_X 0x7C /* "MP" / stone cost label over each column */
#define ABIL_ROW_ACTION_NAME_X 8 /* row, relative to the column: action ability name */
#define ABIL_ROW_ICON_X 8 /* support ability icon */
#define ABIL_ROW_SUPPORT_NAME_X 0x18 /* support ability name */
#define ABIL_ROW_COST_X 0x6C /* MP / stone cost digits */
#define ABIL_ROW_COST_DY 4
#define ABIL_LIST_CURSOR_LEFT 10 /* list cursor: left of the window + column * ABIL_COL_W */
#define ABIL_LIST_CURSOR_DY 10
#define ABIL_TARGET_WIN_X_LEFT 8 /* target window: opens over the column the cursor is not in */
#define ABIL_TARGET_WIN_X_RIGHT 0xA4
#define ABIL_TARGET_WIN_Y 0x26
#define ABIL_TARGET_WIN_W 0x90
#define ABIL_TARGET_WIN_H 0xB6
#define ABIL_TARGET_LABEL_X 8
#define ABIL_TARGET_ROW_X 8 /* member summaries, relative to the window */
#define ABIL_TARGET_ROW_Y 6
#define ABIL_TARGET_ROW_H 0x2B
#define ABIL_TARGET_CURSOR_X (-10)
#define ABIL_TARGET_CURSOR_Y 0x12
#define ABIL_OTZ_TARGET_CURSOR 0x12
#define ABIL_OTZ_TARGET_TEXT 0x13
#define ABIL_OTZ_TARGET 0x14 /* target window */
#define ABIL_OTZ_CURSOR 0x17
#define ABIL_OTZ_CMD 0x1B /* COMMAND window (in front of PLAYER) */
#define ABIL_OTZ_TEXT 0x1C /* window contents */
#define ABIL_OTZ_WIN 0x1D /* tab, PLAYER, INFO and list windows */

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
    MemberSwitch header;  /* member header (ui_initMemberSwitch) */
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
/* Equip screen layout (screen coordinates unless noted) */
#define EQUIP_TABS_WIN_X 8 /* Equip / Optimize / Off tabs */
#define EQUIP_TABS_WIN_Y 8
#define EQUIP_TABS_WIN_W 0xE4
#define EQUIP_TABS_WIN_H 0x1A
#define EQUIP_TAB_TEXT_Y 6 /* relative to the window (x: s_menuEquipTabX) */
#define EQUIP_TAB_CURSOR_LEFT 0x18 /* tab cursor: left of the window + s_menuEquipTabCursorX */
#define EQUIP_TAB_CURSOR_DY 8
#define EQUIP_INFO_WIN_X 8 /* INFO window: member header and the stat comparison */
#define EQUIP_INFO_WIN_Y 0x26
#define EQUIP_INFO_WIN_W 0x8C
#define EQUIP_INFO_WIN_H 0xB6
#define EQUIP_HEADER_X 8 /* relative to the INFO window */
#define EQUIP_HEADER_Y 9
#define EQUIP_INFO_DIV_Y 0x3B /* etched divider under the header */
#define EQUIP_STAT_LABEL_X 8 /* stat rows, relative to the INFO window */
#define EQUIP_STAT_ROW_H 0xC
#define EQUIP_BASE_STAT_Y 0x44 /* Speed, Strength, Magic, Spirit */
#define EQUIP_BASE_STAT_X 0x4A /* current value (2 digits) */
#define EQUIP_BASE_STAT_NEW_X 0x6D /* value with the new item */
#define EQUIP_BATTLE_STAT_Y 0x78 /* Attack, Defence, Evade, Magic Def, Magic Eva */
#define EQUIP_BATTLE_STAT_X 0x43 /* current value (3 digits) */
#define EQUIP_BATTLE_STAT_NEW_X 0x66
#define EQUIP_STAT_ARROW_X 0x5C /* the arrow between them (blinks when the value changes) */
#define EQUIP_SLOTS_WIN_X 0x94 /* EQUIPMENT window (the item list opens at the same rect) */
#define EQUIP_SLOTS_WIN_Y 0x26
#define EQUIP_SLOTS_WIN_W 0xA0
#define EQUIP_SLOTS_WIN_H 0x62
#define EQUIP_SLOT_ROW_H 17 /* EQUIPMENT rows, relative to the window: */
#define EQUIP_SLOT_ICON_X 10 /* slot icon */
#define EQUIP_SLOT_ICON_Y 10
#define EQUIP_SLOT_COLON_X 0x19 /* ':' */
#define EQUIP_SLOT_COLON_Y 12
#define EQUIP_SLOT_ITEM_X 0x21 /* item icon */
#define EQUIP_SLOT_ITEM_Y 8 /* item icon and name */
#define EQUIP_SLOT_NAME_X 0x31
#define EQUIP_SLOT_CURSOR_LEFT 0x10 /* slot cursor: left of the window */
#define EQUIP_SLOT_CURSOR_DY 0xC
#define EQUIP_ABIL_WIN_X 0x94 /* ABILITY window: the item's abilities (header row: the slot and item, 1 higher) */
#define EQUIP_ABIL_WIN_Y 0x8A
#define EQUIP_ABIL_WIN_W 0xA0
#define EQUIP_ABIL_WIN_H 0x52
#define EQUIP_ABIL_DIV_Y 0x1A /* etched divider under the item */
#define EQUIP_ABIL_ROW_Y 0x21 /* ability rows: name y (icon 1 lower, AP gauge 5 lower) */
#define EQUIP_ABIL_ROW_H 15
#define EQUIP_ABIL_ICON_X 8
#define EQUIP_ABIL_NAME_X 0x18
#define EQUIP_ABIL_GAUGE_X 0x72
#define EQUIP_LIST_ROWS 6 /* item list: visible rows */
#define EQUIP_LIST_ROW_H 15
#define EQUIP_LIST_ICON_X 0xB /* relative to the list window */
#define EQUIP_LIST_NAME_X 0x1B
#define EQUIP_LIST_COUNT_X 0x8A
#define EQUIP_COUNT_LABEL_R 0x97 /* right edge of the count label */
#define EQUIP_LIST_CURSOR_LEFT 0xD /* list cursor: left of the window */
#define EQUIP_LIST_CURSOR_DY 0xA
#define EQUIP_OTZ_SUB_CURSOR 0x12 /* slot / list cursor */
#define EQUIP_OTZ_LIST_TEXT 0x13 /* item list contents; the ABILITY window's label */
#define EQUIP_OTZ_LIST 0x14 /* item list window */
#define EQUIP_OTZ_CURSOR 0x1B /* tab cursor */
#define EQUIP_OTZ_TEXT 0x1D /* window contents */
#define EQUIP_OTZ_WIN 0x1E /* tab, INFO, EQUIPMENT and ABILITY windows */
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
    MemberSwitch header; /* member header (ui_initMemberSwitch) */
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
    u8 effect; /* battle formula (BTL_FORMULA_*), item_useOnMember's kind */
    u8 pad5[0xA - 5];
    u8 mpCost; /* menuAbility_getMpCost */
    u8 flags; /* ABILITY_ACT_*; bit 1 clear: ui_drawApGauge's flag argument */
    u8 padC[2];
    u16 nameOffset; /* name offset into s_menuAbilityActionNames */
} Unk801F34EC; /* 0x10 */
/* Unk801F34EC.flags */
#define ABILITY_ACT_MENU 0x1 /* usable from the menu (menuAbility_getActionState greys it out otherwise) */
#define ABILITY_ACT_MP_X4 0x4 /* menuAbility_getMpCost: mpCost x4 while g_evtGlobalVars.b[EVT_GB_SUMMON_MP_X4] is set */
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
u8 menuAbility_findCommand(s32, Character *);
void menuAbility_initActionCursor(void);
void menuAbility_initSupportCursor(void);
extern s32 g_labelEquipment; /* main exe */
extern u8 s_menuEquipColon[];
extern u16 s_menuEquipCharMask[]; /* character masks */
extern u8 s_menuEquipSlotMask[]; /* slot masks */
s32 chr_getDefaultSet(Character *); /* main exe (returns u8) */
extern u8 s_menuEquipTaught[0x40];
extern u8 s_menuEquipOptSlotMask[5]; /* slot masks */
extern u16 s_menuEquipOptCharMask[]; /* character masks */
void menuAbility_loadImages(void);
void menuAbility_onInit(void);
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
void help_nullHandler(void); /* empty function, passed to help_setCallback as a no-op callback */
extern s32 g_labelAbility;
extern s32 g_labelTarget;
extern s32 g_menuScreenFrame; /* = g_menuScreen.unk4, referenced by its own name */
extern s32 g_labelPlayer;
extern s32 g_labelCommand;
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
    u16 layout; /* lines << 12 | text width, the packing of main's message-table word (MSG_ENTRY_W / MSG_ENTRY_LINES, msg_getEntry); the line count matches the 0xF7 newlines + 1 in all 192 entries; no access in any binary */
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
void menuEquip_drawStatCompare(EquipStatsOut *cur, EquipStatsOut *new);

extern Unk801F34EC s_menuEquipActionTable[]; /* action abilities (ability menu), same layout as s_menuAbilityActionTable */
extern u8 s_menuEquipActionNames[]; /* action ability names (ability menu) */
extern u8 s_menuEquipSupportNames[]; /* support ability names (ability menu) */

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
s32 menuAbility_getSlot(MenuList *l);
void menuEquip_drawScrollRows(void);

#endif
