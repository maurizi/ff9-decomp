#ifndef OVL_3B_H
#define OVL_3B_H

/* Declarations local to overlay 0x3B (party select menu, loaded at
 * 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#include "ff9.h"

#define PARTY_EMPTY 0xFF /* empty slot in PartyRequest/PartySelect.slots */

/* Party select request passed to menuParty_loadRequest (g_menu.unkC when the menu opens). */
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

/* Window RECTs (.data) */
extern RECT s_menuPartyTitleRect;
extern RECT s_menuPartyHelpRect;
extern RECT s_menuPartyListRect;
extern RECT s_menuPartyInfoRect;

/* main exe: candidates for ff9.h */
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#endif
/* Defined as u8 in main, but callers here
 * (and ovl_3a) use the result unmasked, i.e. called it unprototyped. */
s32 func_80028954(s32);

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
extern u8 s_menuPartyImage[]; /* image data for func_80028720 */

extern const u8 g_partyMenuText[]; /* text bank for this menu */

#endif
