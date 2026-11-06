#ifndef MENU_MAIN_H
#define MENU_MAIN_H

/* The main menu (src/ovl_0a/menu_main.c and src/ovl_0c/menu_main.c): the field and the world map overlays each
 * carry an identical copy, at their own addresses, so these declarations are shared by ovl_0a.h and ovl_0c.h
 * (each includes this after ff9.h). Window labels sit at WIN_TITLE_Y above their box. */

/* text windows (menuMain_setCursor .. menuMain_updatePanels) */
typedef struct {
    u8 cursor;
    u8 state; /* index into s_menuMainInput */
    u8 orderCursor; /* party slots being swapped */
    u8 orderTarget;
    u8 orderPicked;
    u8 pad5[3];
    WinDef panels[6];
} MenuMain;
/* MenuMain.state (menuMain_selectCommand) */
#define MENUMAIN_COMMAND 0 /* picking a command */
#define MENUMAIN_MEMBER 1 /* picking the party member for it (menuScreen_needsMember) */
#define MENUMAIN_ORDER 2 /* Order: swapping party members */

void menuMain_input(void);
void menuMain_updatePanels(void);
void menuMain_draw(void);
void menuMain_drawTimeGil(void);
void menuMain_drawLocation(void);
void menuMain_drawCursor(void);
void menuMain_drawParty(void);
void menuMain_initPanels(void);
void menuMain_swapPortraits(s32, s32);
void menuMain_updateHelp(void);
void menuMain_setExpHelp(s32);
void menuMain_selectCommand(void);
void menuMain_setEquipHelp(u8);
void menuMain_reorder(s32, s32);
void menuMain_drawMember(s32);
void menuMain_drawPortraitShade(RECT *, s32);

extern MenuMain s_menuMain;
extern void (*s_menuMainInput[])(void);
extern RECT *s_menuMainTimeGilRect;
extern u8 s_menuMainGilSuffix[];
extern RECT *s_menuMainLocationRect;
extern u8 *s_menuMainEquipLabels[5];
extern u8 s_menuMainText[]; /* text */
extern RECT s_menuMainPanelRects[6];
extern TweenDef s_menuMainSlideIn[4];
extern u8 s_menuMainExpText[16];
extern u8 s_menuMainNextText[16];
extern u8 s_menuMainExpLabel[];
extern u8 s_menuMainNextLabel[];
extern u8 s_menuMainUntilLabel[];
extern u8 s_menuMainStatusLabel[];
extern u8 s_menuMainStatusNormal[];
extern u8 s_menuMainStatusAbnormal[];

/* s_menuMainText entries (text_get; decoded with tools/ff9text.py text_table) */
#define MENU_MAIN_TEXT_NO_LOCATION 1 /* "Honolulu conversion code": a leftover placeholder, shown when GameData.locationName is empty */
#define MENU_MAIN_TEXT_EQUIP_HELP 4 /* "Select character." + the five equipment lines (string args 0-9) */
#define MENU_MAIN_TEXT_STATUS_HELP 5 /* "Select character." + Exp / Next lines (string args 2-7) */
#define MENU_MAIN_TEXT_EMPTY 0xD /* "": an empty equipment slot */
#define MENU_MAIN_TEXT_MEMBER_HELP 2 /* + cursor: the "Select character." help of each command (menuMain_updateHelp) */
#define MENU_MAIN_TEXT_ORDER_HELP 0xB /* + orderPicked: "Select character." / the front row / back row explanation */
#define MENU_MAIN_COMMAND_LAST (MENU_SCREEN_CONFIG - 1) /* command cursor range 0..this; cursor + 1 = MENU_SCREEN_* */

/* Main menu layout (menu_main.c, same in ovl_0a and ovl_0c). s_menuMainPanelRects: party panels at (0xE,
 * 0xD + 0x30 * slot) 0xF6 x 0x30, the Time/Gil window at (0xE1, 0x9A) 0x53 x 0x27, the location window
 * 0x1B high at y 0xC1. Offsets below are from the window's top left corner. */
#define MENU_MAIN_OTZ_PANEL 0x14 /* windows and the party panels' contents */
#define MENU_MAIN_OTZ_SHADOW 0x15 /* party panel shadows, behind them */
#define MENU_MAIN_OTZ_TEXT 0x13 /* Time/Gil and location contents, over their windows */
#define MENU_MAIN_OTZ_COMMAND_HAND 1 /* command list cursor */
#define MENU_MAIN_OTZ_MEMBER_HAND 0xC /* party member cursor */
#define MENU_MAIN_OTZ_TARGET_HAND 0xB /* Order: the second (swap target) cursor */
#define MENU_MAIN_LOCATION_RIGHT 0x134 /* the location window is right-aligned to this x */
#define MENU_MAIN_LOCATION_PAD_W 0x10 /* width added to the location name's */
#define MENU_MAIN_LOCATION_MIN_W 0x44
#define MENU_MAIN_LOCATION_TEXT_X 8 /* "Location" label and the location name */
#define MENU_MAIN_LOCATION_TEXT_Y 7 /* the location name */
#define MENU_MAIN_TIMEGIL_ICON_X 6 /* the TIME / GIL sprites and the "Time & Gil" label (= WIN_TITLE_X) */
#define MENU_MAIN_TIMEGIL_DIGITS_X 0x13 /* play time and gil digits */
#define MENU_MAIN_TIME_ICON_Y 8
#define MENU_MAIN_TIME_DIGITS_Y 9
#define MENU_MAIN_GIL_ICON_Y 0x15
#define MENU_MAIN_GIL_DIGITS_Y 0x16
#define MENU_MAIN_GIL_WIDTH 7 /* digits (text_formatNumber) */
#define MENU_MAIN_EXP_WIDTH 7 /* Exp / Next digits in the Status help (menuMain_setExpHelp) */
#define MENU_MAIN_COMMAND_HAND_X 0xE0 /* command list cursor: x, first row y and row height */
#define MENU_MAIN_COMMAND_HAND_Y 0x11
#define MENU_MAIN_COMMAND_ROW_H 0xF
#define MENU_MAIN_MEMBER_HAND_DX 0xE /* party member cursor: left of panel 0's x, first row y, row step */
#define MENU_MAIN_MEMBER_HAND_Y 0x20
#define MENU_MAIN_MEMBER_ROW_H 0x2E
#define MENU_MAIN_TARGET_HAND_DX 0xA /* Order: the swap target's cursor, offset down-right of the first */
#define MENU_MAIN_TARGET_HAND_Y 0x24
#define MENU_MAIN_HELP_X 8 /* help window bounds (help_setBounds): left of the command list */
#define MENU_MAIN_HELP_Y 8
#define MENU_MAIN_HELP_W 0xE8
#define MENU_MAIN_HELP_BOTTOM 0xF /* lines above the display's bottom */
/* party panel */
#define MENU_MAIN_PORTRAIT_X_FRONT 8 /* portrait x with battleRow set (front row) */
#define MENU_MAIN_PORTRAIT_X_BACK 0x16 /* portrait x with battleRow clear (back row): shifted right */
#define MENU_MAIN_PORTRAIT_Y 1
#define MENU_MAIN_PORTRAIT_H 0x2D
#define MENU_MAIN_SHADE_X 8 /* the translucent tile behind the portrait (menuMain_drawPortraitShade): the dark backdrop
                            * of the portrait slot, spanning both rows' positions */
#define MENU_MAIN_SHADE_W 0x2E
#define MENU_MAIN_SHADE_H 0x30
#define MENU_MAIN_STATS_X 0x3C /* name, LV, HP and MP column */
#define MENU_MAIN_NAME_Y 3
#define MENU_MAIN_LV_Y 0x12
#define MENU_MAIN_HP_Y 0x1B
#define MENU_MAIN_MP_Y 0x24
#define MENU_MAIN_LV_DIGITS_X 0x60
#define MENU_MAIN_HP_DIGITS_X 0x52 /* "hhhh/max" */
#define MENU_MAIN_MP_DIGITS_X 0x59 /* "mmm/max" */
#define MENU_MAIN_HP_WIDTH 4 /* digits before the '/' */
#define MENU_MAIN_MP_WIDTH 3
#define MENU_MAIN_STATUS_BOX_X 0x82 /* the box framing the status icons, along the panel's top */
#define MENU_MAIN_STATUS_BOX_W 0x72
#define MENU_MAIN_STATUS_BOX_H 0x12
#define MENU_MAIN_STATUS_X 0x86 /* status icons */
#define MENU_MAIN_STATUS_Y 4
#define MENU_MAIN_STONES_X 0x9B /* magic stones (ui_drawStones) */
#define MENU_MAIN_STONES_Y 0x14
#define MENU_MAIN_LOW_STAT_DIV 6 /* HP / MP at or below max / this are shown in TEXT_PAL_YELLOW */
/* menuMain_swapPortraits: spare VRAM for the second and third portrait rects (the first goes to
 * g_portraitSavedVram) while two party slots trade portraits */
#define MENU_MAIN_SWAP_VRAM_X1 0x3C0
#define MENU_MAIN_SWAP_VRAM_X2 0x3D0
#define MENU_MAIN_SWAP_VRAM_Y 0x1B0

#endif
