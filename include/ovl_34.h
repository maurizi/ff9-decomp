#ifndef OVL_34_H
#define OVL_34_H

/* Declarations local to overlay 0x34 (config menu, loaded at 0x801EDF00).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* config rows (list index), in the order of their titles (text 24-36) and s_menuConfigRowText */
enum {
    CONFIG_ROW_SOUND,         /* Stereo / Mono */
    CONFIG_ROW_CONTROLLER,    /* Normal / Custom (button mapping window) */
    CONFIG_ROW_CURSOR,        /* Initial / Memory */
    CONFIG_ROW_ATB,           /* Active / Wait */
    CONFIG_ROW_CAMERA,        /* Battle Camera: Auto / Fixed */
    CONFIG_ROW_BTL_CONTROL,   /* Battle Control: Normal / Custom (controller port per character window) */
    CONFIG_ROW_MOVEMENT,      /* Walk / Run */
    CONFIG_ROW_BTL_SPEED,     /* Battle Speed slider */
    CONFIG_ROW_MSG_SPEED,     /* Field Message slider */
    CONFIG_ROW_HERE_ICON,     /* ON / OFF */
    CONFIG_ROW_WINDOW_COLOR,  /* Normal / Classic */
    CONFIG_ROW_TARGET_WINDOW, /* Auto / Always */
    CONFIG_ROW_VIBRATION,     /* ON / OFF (greyed out without a vibration pad) */
    CONFIG_ROW_COUNT
};

#define CONFIG_PAD_SLOTS 8 /* actions in the button mapping window (s_menuConfigPadActions) */

enum { CONFIG_SLIDE_NONE, CONFIG_SLIDE_RIGHT, CONFIG_SLIDE_LEFT }; /* ConfigMenu.sliderDir */

/* g_configMenuText ids */
#define CONFIG_TEXT_PAD_EXIT   0    /* "Press Start to exit." (button mapping window) */
#define CONFIG_TEXT_PORT_EXIT  1    /* "Press Circle to exit." (controller port window) */
#define CONFIG_TEXT_ROW_HELP   2    /* help per row, + CONFIG_ROW_* */
#define CONFIG_TEXT_PAD_HELP   0xF  /* what each mapped button does, + slot */
#define CONFIG_TEXT_PORT_HELP  0x17 /* "Character controls can be assigned to Controller port 1 or 2..." */
                                    /* 24-36: row titles, 37-55: option names, 56-63: button actions */

/* Config menu state (s_menuConfig). */
typedef struct {
    u8 subCursor;      /* row in the custom window */
    u8 charCount;      /* characters listed in the controller port window */
    u8 editing;        /* a custom window is open */
    u8 rumbleTest;     /* vibration was just turned on: rumble for a moment */
    u8 sliderDir;      /* CONFIG_SLIDE_*: slider knob moving */
    u8 sliderTicks;    /* slider move frames (5 per step) */
    u8 rumbleTicks;    /* rumble frames (9) */
    u8 unk7;
    u8 charPortBit[9]; /* per port window row: bit of the character in the port bits (chr_getClass) */
    u8 charIdx[9];     /* per port window row: character index */
    u8 pad1A[2];
    WinDef wins[2];    /* custom windows: the list and its help line */
    MenuList list;     /* .unk5 = scroll top, .cursor = row */
    u8 *prim;          /* next free primitive */
    u8 *primBufs[2];   /* primitive buffers (0x2000 each), by frame */
    s32 primFree;      /* bytes left in last frame's buffer */
} ConfigMenu;

/* visible rows of the item list (menuConfig_getScrollView) */
typedef struct {
    u8 rows;  /* row count */
    u8 first; /* first item */
    s16 y;    /* y offset */
} ScrollPos;
extern ConfigMenu s_menuConfig;
extern const u8 g_configMenuText[]; /* menu text block */

/* GameState.unk24: the config word (main exe: candidates for ff9.h). Bit positions shown by the config rows
 * that read and write them (menuConfig_setOption, the row drawers). */
#define CFG_BIT_SOUND         0  /* 1 = mono */
#define CFG_BIT_CONTROLLER    1  /* 1 = custom button mapping (GameState.unk28) */
#define CFG_BIT_CURSOR        2  /* 1 = memory */
#define CFG_BIT_ATB           3  /* 1 = wait */
#define CFG_BIT_CAMERA        4  /* 1 = fixed */
#define CFG_BIT_BTL_CONTROL   5  /* 1 = custom: controller port per character (CFG_SHIFT_CHAR_PORT bits) */
#define CFG_BIT_MOVEMENT      6  /* 1 = run */
#define CFG_BIT_VIBRATION     7  /* 1 = off */
#define CFG_SHIFT_BTL_SPEED   8  /* 3 bits, 0..CFG_BTL_SPEED_MAX */
#define CFG_SHIFT_MSG_SPEED   14 /* 3 bits, 0..CFG_MSG_SPEED_MAX */
#define CFG_BIT_HERE_ICON     17 /* 1 = off */
#define CFG_BIT_WINDOW_COLOR  18 /* 1 = classic */
#define CFG_BIT_TARGET_WINDOW 19 /* 1 = always */
#define CFG_SHIFT_CHAR_PORT   20 /* one bit per character: 1 = controller port 2 */
#define CFG_BTL_SPEED_MAX     2
#define CFG_MSG_SPEED_MAX     6

/* the same word, written through bitfields by menuConfig_setOption */
typedef struct {
    u32 soundMono : 1;
    u32 padCustom : 1;
    u32 cursorMemory : 1;
    u32 atbWait : 1;
    u32 battleCameraFixed : 1;
    u32 battleControlCustom : 1;
    u32 movementRun : 1;
    u32 vibration : 1;
    u32 battleSpeed : 3;
    u32 b11 : 3;
    u32 messageSpeed : 3;
    u32 hereIconOff : 1;
    u32 windowClassic : 1;
    u32 targetWindowAlways : 1;
    u32 charPort : 12;
} ConfigBits;
#define CFG (*(ConfigBits *)&g_gameState->config)
extern RECT s_menuConfigSubRects[2]; /* option window rects */
extern TweenDef s_menuConfigSubOpen; /* custom windows slide in */
extern TweenDef s_menuConfigPadMapClose; /* button mapping window slides out, then menuConfig_closeSubWindow */
extern TweenDef s_menuConfigBattleControlClose; /* port window slides out, then menuConfig_closeSubWindow */
extern s32 s_menuConfigPadBits[CONFIG_PAD_SLOTS]; /* pad bit per button mapping slot: Cross, Circle, Triangle, Square, L1, R1, L2, R2 */
extern s32 s_menuConfigSlowText; /* text id "Slow" (slider left label) */
extern s32 s_menuConfigFastText; /* text id "Fast" (slider right label) */
extern u8 s_menuConfigPadArrow[];
typedef struct {
    u8 text; /* action name text id */
    u8 icon; /* button sprite */
} ConfigPadEntry;
extern ConfigPadEntry s_menuConfigPadActions[CONFIG_PAD_SLOTS];

/* main exe: candidates for ff9.h */
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#endif
void help_nullHandler(void); /* empty function, passed to help_setCallback as a no-op callback */
extern s32 g_menuScreenFrame; /* = g_menuScreen.unk4, referenced by its own name */

typedef struct {
    u8 title;      /* title text id */
    u8 options[3]; /* option text ids */
} ConfigText;
extern ConfigText s_menuConfigRowText[CONFIG_ROW_COUNT];
extern void (*s_menuConfigSubInput[])(void);
extern void (*s_menuConfigRowDraw[CONFIG_ROW_COUNT])(RECT *, s32);

void menuConfig_inputList(void);
void menuConfig_inputPadMap(void);
void menuConfig_inputBattleControl(void);
void menuConfig_drawPadMap(void);
void menuConfig_drawBattleControl(void);
void menuConfig_stepSlider(void);
void menuConfig_updateHelp(void);
void menuConfig_input(void);
void menuConfig_draw(void);
void menuConfig_updateAnims(void);
void menuConfig_drawRows(void);
void menuConfig_drawCursor(void);
void menuConfig_setOption(u8);
void menuConfig_openSubWindow(void);
void menuConfig_drawSlider(s32, s32, s32, s32, s32);
void menuConfig_pageUp(MenuList *, u32);
void menuConfig_pageDown(MenuList *, u32);
void menuConfig_drawRow(RECT *, s32, s32);
void menuConfig_drawSubWindows(void);
void menuConfig_drawScrollArrows(void);
void menuConfig_drawChoice(RECT *, s32, s32, s32);
void menuConfig_drawSliderRow(RECT *, s32, s32, s32, s32);
void menuConfig_drawText(s32, s32, s32, s32, s32);
void menuConfig_stepWindows(void);
void menuConfig_stepScroll(void);
void menuConfig_stepRumble(void);
u8 menuConfig_hasVibration(void);
void menuConfig_setRumble(u8);
ScrollPos *menuConfig_getScrollView(MenuList *, ScrollPos *);
ScrollPos *menuConfig_getListView(MenuList *, ScrollPos *);
void menuConfig_getScrollClip(MenuList *, RECT *, RECT *);
void menuConfig_drawScrollRows(void);
void menuConfig_drawSmallText(s32, s32, s32, u8 *, s32);
void menuConfig_drawIcon(s32, s32, s32, s32, u8);
void menuConfig_setPadButton(s32, s32);
u8 menuConfig_isWindowMoving(void);

/* main exe: candidates for ff9.h */
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#endif
/* slider gauge pieces drawn by ui_drawSpriteDef: left cap, middle (repeated via DR_TWIN), right cap */
typedef struct {
    u8 twX;  /* texture window x */
    u8 twY;  /* texture window y */
    u16 uv;
    u8 w;
    u8 h;
    u8 pad6[2];
} GaugeIcon;
extern GaugeIcon D_80068988[3];
void SetTexWindow(void *p, RECT *r);

/* LINE_F2 written as whole words */
typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 xy1;
} LineWords;
extern u32 s_menuConfigSliderColors[3]; /* gauge line colours */
extern RECT s_menuConfigSliderTexWindow; /* gauge texture window */

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void config_apply(void); /* main */

#endif
