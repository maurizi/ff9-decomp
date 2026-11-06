#ifndef OVL_3B_H
#define OVL_3B_H

/* Declarations local to overlay 0x3B (party select menu, loaded at
 * 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#include "ff9.h"

#define PARTY_EMPTY 0xFF /* empty slot in PartyRequest/PartySelect.slots */

/* Party select request passed to menuParty_loadRequest (g_menu.arg when the menu opens). */
typedef struct {
    u8 size;      /* members to pick */
    u8 pad1[3];
    u8 party[4];  /* current party (character ids, PARTY_EMPTY = empty) */
    u8 reserve[8];
    u8 fixed[8];  /* per character: nonzero = can't leave the party (menuParty_isFixed) */
} PartyRequest;

enum { PARTY_ROW_PARTY, PARTY_ROW_RESERVE };

typedef struct {
    u8 idx; /* slot in the row: 0-3 in the party, 0-7 (4 per line) in the reserve */
    u8 row; /* PARTY_ROW_* */
} Cursor;

enum { PARTY_SEL_PICK, PARTY_SEL_SWAP }; /* PartySelect.mode */

/* g_partyMenuText ids */
#define PARTY_TEXT_TITLE     0   /* "Form Party" */
                                 /* 1-4: "Select N member(s).", by party size */
#define PARTY_TEXT_SLOT1     5   /* "Character 1".."Character 4": empty slot */
#define PARTY_TEXT_UNUSED    9   /* slot past the party size */
#define PARTY_TEXT_MOVE_FROM 0xA /* "Move from this slot?", then 0xB "To which slot?" (+ mode) */

/* Party select state (s_menuParty). */
typedef struct {
    u8 mode;       /* PARTY_SEL_*: picking the first slot, or the slot to swap it with */
    u8 size;       /* party size */
    u8 pad2[2];
    Cursor cursor[2]; /* [mode]: [0] = picking, [1] = swap target */
    u8 partyIdx;   /* party cursor saved while in the reserve */
    u8 reserveIdx; /* reserve cursor saved while in the party */
    u8 slots[2][8]; /* [PARTY_ROW_*]: character ids, PARTY_EMPTY = empty */
    u8 fixed[8];   /* PartyRequest.fixed */
} PartySelect;
extern PartySelect s_menuParty;

#define PARTY_RESERVE_COLS 4 /* reserve portraits per line (2 lines) */
#define PARTY_RESERVE_COLS_SHIFT 2 /* log2(PARTY_RESERVE_COLS) */
#define PARTY_PORTRAIT_COUNT 0xD /* portraits in s_menuPartyImage (ui_loadPortraits) */

/* Party select screen layout (window rects are .data). Offsets are from the window unless noted. */
#define PARTY_OTZ_CURSOR 4
#define PARTY_OTZ_TEXT   6 /* members, portraits, texts */
#define PARTY_OTZ_LIST   7 /* party list window (labelled Party) */
#define PARTY_OTZ_BOX    8 /* title, help and info windows */
#define PARTY_OTZ_SHADOW 9 /* the WIN_STYLE_LINE_RULE windows' shadows */
#define PARTY_TITLE_X 9 /* "Form Party" */
#define PARTY_TEXT_Y 6 /* title / help text */
#define PARTY_HELP_TEXT_X 0x12 /* "Select N member(s)." */
#define PARTY_RULE_LABEL_X 0xC /* Help / Info labels */
#define PARTY_LIST_LABEL_X 8 /* Party label */
#define PARTY_ROW_X 8 /* party list: member rows */
#define PARTY_ROW_Y 6
#define PARTY_ROW_H 0x33
#define PARTY_DIV_Y 0x35 /* etched divider under each party row (and under the info window's member) */
#define PARTY_EMPTY_W 0x8D /* "Character N" of an empty slot is centred in this width */
#define PARTY_EMPTY_Y 0x15
#define PARTY_INFO_MEMBER_X 0x10 /* info window: the member under the cursor */
#define PARTY_RESERVE_X 0x10 /* info window: reserve portraits */
#define PARTY_RESERVE_Y 0x3A
#define PARTY_RESERVE_COL_W 0x25
#define PARTY_RESERVE_ROW_H 0x30
#define PARTY_PORTRAIT_W 0x20 /* menuParty_drawPortrait: status icons are centred in this width */
#define PARTY_PORTRAIT_H 0x2D
#define PARTY_ICON_Y0 8 /* first status icon this far above the portrait's bottom */
#define PARTY_ICON_STEP 7 /* each further icon this much higher */
#define PARTY_STATUS_DX 0x26 /* member row: name/level/HP after the portrait */
#define PARTY_STATUS_DY 3
/* cursor (screen coordinates): party slot pos at (PARTY_CURSOR_X, PARTY_CURSOR_Y + pos * PARTY_ROW_H), reserve
 * slot at PARTY_CURSOR_RESERVE_X/Y by column and line; the swap target's cursor is PARTY_SWAP_CURSOR_D lower right */
#define PARTY_CURSOR_X 0
#define PARTY_CURSOR_Y 0x20
#define PARTY_CURSOR_RESERVE_X 0x8C
#define PARTY_CURSOR_RESERVE_Y 0x86
#define PARTY_SWAP_CURSOR_D 4

/* Window RECTs (.data) */
extern RECT s_menuPartyTitleRect;
extern RECT s_menuPartyHelpRect;
extern RECT s_menuPartyListRect;
extern RECT s_menuPartyInfoRect;

/* main exe: candidates for ff9.h */
/* Defined as u8 in main, but callers here
 * (and ovl_3a) use the result unmasked, i.e. called it unprototyped. */
s32 chr_getPortrait(s32);

void menuParty_inputParty(void);
void menuParty_inputReserve(void);
void menuParty_pick(void);
void menuParty_drawParty(void);
void menuParty_drawInfo(void);
void menuParty_drawCursor(void);
void menuParty_input(void);
void menuParty_draw(void);
void menuParty_drawTitle(void);
void menuParty_drawHelp(void);
void menuParty_confirm(void);
void menuParty_drawMember(s32, s32, s32, s32);
void menuParty_drawPortrait(s32, s32, s32, s32, s32, u8);
u8 menuParty_isFixed(s32);
u8 menuParty_isValid(void);
void menuParty_loadRequest(PartyRequest *);
void menuParty_updateHelp(void);
extern u8 s_menuPartyImage[]; /* image data for ui_loadPortraits */

extern const u8 g_partyMenuText[]; /* text bank for this menu */

#endif
