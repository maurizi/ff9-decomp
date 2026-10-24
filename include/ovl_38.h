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
#define STATUS_ABILITY_MAX  0x30 /* entries in a character's D_8006CB28 ability row */
#define STATUS_SUPPORT_FIRST 0xC0 /* ability ids from here are support abilities (D_8006D128) */

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
    u8 savedHelp;    /* func_80035648 value, restored on exit */
    u8 pad6[2];
    RECT win;        /* main window */
    u8 pageOpen[6];  /* ability page window is up */
    u8 pageOnTop[6]; /* the topmost page: draws names and the page number */
    WinDef pageWins[6];
    u8 *prim;        /* next free primitive */
    u8 *primBufs[2]; /* primitive buffers (0x4000 each), by frame */
    s32 primFree;    /* bytes left in last frame's buffer */
    UnkBlink header; /* character name/level header (func_80028CF8, slides on L1/R1) */
} StatusMenu;
extern StatusMenu s_menuStatus;

/* window slide animation for func_80063118: main's TweenDef, but with a signed delay (unk0), which is what
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

/* string and value arguments of the help text's codes (func_8002BE44 / func_8002BE74) */
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
    u8 unkB; /* bit 1 tested */
    u8 padC[2];
    u16 unkE; /* name offset into s_menuStatusActionNames */
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

/* local copy of the battle command table (built on the stack from .rodata) */
typedef struct {
    u16 unk0; /* offset of the command name in the string pool */
    u16 unk2;
    s32 unk4;
    s32 unk8;
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
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused; SE_PAGE is menuList_pageUp/pageDown's
 * sound, also played on L1/R1 character switches. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#define SE_PAGE   0x417
#endif
#ifndef ITEM_NONE
#define ITEM_NONE 0xFF /* empty Character.equip slot */
#endif
extern s32 D_8007042C;
void ui_drawItemIcon(s32, s32, s32, s32, u8);
typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2;
    u8 pad4;
    u8 unk5; /* attack power */
    u8 pad6[0xC - 6];
} Unk8006E0E8; /* weapon stats, 0xC (also in ovl_0b.h) */
extern Unk8006E0E8 D_8006E0E8[];

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
void func_801EF9B4(void);
void menuStatus_closeAbilityPages(void);
void menuStatus_onMemberSwitched(void);
void menuStatus_updateHelp(void);

extern const u8 D_801EE264[]; /* name strings */

#endif
