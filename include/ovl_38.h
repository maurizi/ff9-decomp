#ifndef OVL_38_H
#define OVL_38_H

/* Declarations local to overlay 0x38 (status menu, loaded at 0x801EDF00).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* StatusMenu.state: which input handler s_menuStatusInput runs */
enum {
    STATUS_STATE_BUSY,  /* no input: a window is moving */
    STATUS_STATE_MAIN,  /* stats page: Circle, Cross, L1/R1 */
    STATUS_STATE_PAGES, /* ability pages open: Circle, Cross */
    STATUS_STATE_WAIT   /* no input: waiting for a page or character animation */
};

#define STATUS_NO_SLOT      0xFF /* StatusMenu.slot before the first character is shown */
#define STATUS_PAGE_ROWS    8    /* abilities per page window */

/* Status screen layout (menuStatus_*). Positions are offsets from the main window s_menuStatus.win unless noted. */
#define STATUS_WIN_X 8 /* main window */
#define STATUS_WIN_Y 0x26
#define STATUS_WIN_W 0x12C
#define STATUS_WIN_H 0xB6
#define STATUS_OTZ_FRAME   0x1E /* main window frame and its etched dividers */
#define STATUS_OTZ_CONTENT 0x1D /* header, stats, EXP, equipment */
#define STATUS_OTZ_CMDS    0x19 /* the command box (in front of the main window) */
#define STATUS_OTZ_PAGE0   0x14 /* ability page i draws at STATUS_OTZ_PAGE0 - i (later pages in front) */
#define STATUS_HEADER_X 8 /* member header (portrait, name, level, HP/MP) */
#define STATUS_HEADER_Y 6
#define STATUS_ICONBOX_X 0x72 /* status-ailment icon box, top right of the header */
#define STATUS_ICONBOX_Y 4
#define STATUS_ICONBOX_W 0x5C
#define STATUS_ICONBOX_H 0x12
#define STATUS_STONES_DX 0x19 /* magic stones gauge, relative to the icon box */
#define STATUS_STONES_DY 0x14
#define STATUS_CMDBOX_X 0xE4 /* battle command box (labelled "Command") */
#define STATUS_CMDBOX_Y 8
#define STATUS_CMDBOX_W 0x42
#define STATUS_CMDBOX_H 0x48
#define STATUS_CMD_TEXT_X 0xC /* command names, relative to the command box */
#define STATUS_CMD_TEXT_Y 6
#define STATUS_CMD_ROW_H  15
#define STATUS_STAT_LABEL_X 0xA /* stat labels (s_menuStatusBaseStatText / s_menuStatusBattleStatText) */
#define STATUS_STAT_LABEL_DY (-4) /* labels sit this far above their value row */
#define STATUS_STAT_ROW_H 13
#define STATUS_BASE_STAT_Y 0x3D /* Speed, Strength, Magic, Spirit rows */
#define STATUS_BASE_STAT_X 0x58 /* their 2-digit values */
#define STATUS_BATTLE_STAT_Y 0x72 /* Attack, Defence, Evade, Magic Def, Magic Eva rows */
#define STATUS_BATTLE_STAT_X 0x51 /* their 3-digit values */
#define STATUS_EXP_LABEL_X 0x74 /* Trance / EXP / Next labels */
#define STATUS_TRANCE_LABEL_Y 0x39
#define STATUS_TRANCE_GAUGE_X 0xB4
#define STATUS_TRANCE_GAUGE_Y 0x3E
#define STATUS_EXP_LABEL_Y 0x46
#define STATUS_NEXT_LABEL_Y 0x53
#define STATUS_EXP_X 0xAC /* EXP value (7 digits) */
#define STATUS_EXP_Y 0x4A
#define STATUS_NEXT_X 0xB3 /* EXP to the next level (6 digits) */
#define STATUS_NEXT_Y 0x57
#define STATUS_EQUIP_Y 0x68 /* equipment rows (slot icon, colon, item icon, item name) */
#define STATUS_EQUIP_ROW_H 15
#define STATUS_EQUIP_SLOT_X 0x74
#define STATUS_EQUIP_COLON_X 0x84
#define STATUS_EQUIP_ICON_X 0x8D
#define STATUS_EQUIP_NAME_X 0x9D
#define STATUS_PAGE_ROW_X 0xA /* ability page rows, relative to the page window */
#define STATUS_PAGE_ROW_Y 8
#define STATUS_PAGE_ROW_H 15
#define STATUS_PAGE_NO_X 0x90 /* "No." page number label, relative to the page window */
#define STATUS_PAGE_NUM_X 0x9A
#define STATUS_ABIL_NAME_DX 0x10 /* ability row: name after the icon */
#define STATUS_ABIL_GAUGE_DX 0x70 /* ability row: AP gauge */
#define STATUS_ABIL_GAUGE_DY 3
#define STATUS_DIV_HEADER_Y 0x36 /* etched divider under the header */
#define STATUS_DIV_COLUMN_X 0x6B /* vertical divider between the stats and the EXP/equipment column */
#define STATUS_DIV_COLUMN_Y 0x38
#define STATUS_DIV_EQUIP_X 0x6D /* divider above the equipment */
#define STATUS_DIV_EQUIP_Y 0x63
#define STATUS_HELP_X 0xD8 /* help window anchor (g_menuHelp) */
#define STATUS_HELP_Y 0x52
#define STATUS_HELP_OTZ 0xA

/* g_statusMenuText ids */
#define STATUS_TEXT_HELP   0   /* help: the status ailments */
                               /* 2-10: stat labels (s_menuStatusBaseStatText, s_menuStatusBattleStatText) */
#define STATUS_TEXT_TRANCE 0xB
#define STATUS_TEXT_EXP    0xC
#define STATUS_TEXT_NEXT   0xD

/* Status menu state (s_menuStatus). */
typedef struct {
    u8 state;        /* STATUS_STATE_*, index into s_menuStatusInput */
    u8 pagesOpen;    /* ability page windows opened so far */
    u8 slot;         /* party slot shown */
    u8 pageCount;    /* ability pages of this character */
    u8 abilityCount; /* abilities this character can learn */
    u8 savedHelp;    /* help_getFollow value, restored on exit */
    u8 pad6[2];
    RECT win;        /* main window */
    u8 pageOpen[6];  /* ability page window is up */
    u8 pageOnTop[6]; /* the topmost page: draws names and the page number */
    WinDef pageWins[6];
    u8 *prim;        /* next free primitive */
    u8 *primBufs[2]; /* primitive buffers (0x4000 each), by frame */
    s32 primFree;    /* bytes left in last frame's buffer */
    MemberSwitch header; /* character name/level header (ui_initMemberSwitch, slides on L1/R1) */
} StatusMenu;
extern StatusMenu s_menuStatus;

/* window slide animation for tween_start: main's TweenDef, but with a signed delay (unk0), which is what
 * matches menuStatus_closeAbilityPages */
typedef struct {
    s16 delay;  /* start delay */
    s16 frames; /* frame count */
    s8 *dx;     /* x deltas */
    s8 *dy;     /* y deltas */
    void (*onDone)(void);
} TextTween;
extern TextTween s_menuStatusPageCloseTweens[8]; /* page close, delay set per page */
extern RECT s_menuStatusPageRects[];      /* page window positions */
extern TextTween s_menuStatusPageOpenTween;    /* page open, then menuStatus_onPageOpened */

/* string and value arguments of the help text's codes (text_setStringArg / text_setIconArg) */
typedef struct {
    u8 *str;
    s32 val;
} HelpArg;
extern HelpArg s_menuStatusHelpArgs[7];
extern u8 s_menuStatusHelpArg7[];
extern u8 g_statusMenuText[];
extern void (*s_menuStatusInput[])(void);

typedef struct {
    u8 pad0[0xB];
    u8 flags; /* bit 1 clear: AP gauge flag */
    u8 padC[2];
    u16 nameOffset; /* name offset into s_menuStatusActionNames */
} ActiveAbility; /* 0x10 */
extern ActiveAbility s_menuStatusActionAbilities[];
extern u8 s_menuStatusActionNames[];
extern u8 s_menuStatusSupportNames[];
extern u8 s_menuStatusPageLabel[];

/* status page tables in .data */
extern u8 s_menuStatusCmds[4];       /* [1], [2] patched per character */
extern u8 s_menuStatusCharCmds[][2];     /* by ability table row */
extern s32 s_menuStatusBaseStatText[4];      /* text ids */
extern s32 s_menuStatusBattleStatText[5];      /* text ids */

/* local copy of the battle command table (built on the stack from .rodata). s_menuStatusCmdTable is byte-identical
 * to ovl_0b's g_btlCmdTable (0x240 bytes, 48 entries; Unk800FD56C there), so the members follow that struct; ovl_38
 * reads only nameOffset. Evidence: [code] */
typedef struct {
    u16 nameOffset; /* offset of the command name in the string pool */
    u16 helpOfs; /* help text offset (ovl_0b Unk800FD56C.helpOfs into g_btlCmdHelp); not read in ovl_38 */
    u8 pad4[6 - 4];
    u8 kind; /* ovl_0b Unk800FD56C.kind (confirm handler); not read in ovl_38 */
    u8 count; /* abilities of the command (ovl_0b Unk800FD56C.count); not read in ovl_38 */
    s32 first; /* first ability index into ovl_0b g_btlCmdAbilities (Unk800FD56C.first); not read in ovl_38 */
} StatusEntry;
typedef struct {
    StatusEntry v[48];
} StatusTable;
typedef struct {
    u8 s[0x11E];
} StatusPool; /* battle command names */
extern StatusTable s_menuStatusCmdTable;
extern StatusPool s_menuStatusCmdNames;

/* main exe: candidates for ff9.h */
extern s32 g_labelCommand;
void ui_drawItemIcon(s32, s32, s32, s32, u8);

void menuStatus_inputMain(void);
void menuStatus_drawHeader(void);
void menuStatus_drawCommands(void);
void menuStatus_drawStats(void);
void menuStatus_drawExp(void);
void menuStatus_drawEquipment(void);
void menuStatus_drawAbilityPages(void);
void menuStatus_drawAbilityRow(s32, s32, s32, s32, u8);
void menuStatus_openAbilityPage(void);
void menuStatus_setMember(s32, s32);
void menuStatus_input(void);
void menuStatus_draw(void);
void menuStatus_updateAnims(void);
void menuStatus_inputAbilities(void);
void menuStatus_drawFrame(void);
void menuStatus_pollFade(void);
void menuStatus_closeAbilityPages(void);
void menuStatus_onMemberSwitched(void);
void menuStatus_updateHelp(void);

extern const u8 s_menuStatusColon[]; /* name strings */

#endif
