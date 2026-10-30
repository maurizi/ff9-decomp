#ifndef OVL_39_H
#define OVL_39_H

/* Declarations local to overlay 0x39 (save / load menu, menu 4, loaded at
 * 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#define FF9_BUILTIN_MEMCPY /* see memcpy in psyq.h */
#include "ff9.h"

/* ---- constants ---- */

/* SaveMenu.mode, from g_menu.arg; also the title text id (Save/Load/Save) */
enum {
    SAVE_MODE_SAVE,
    SAVE_MODE_LOAD,
    SAVE_MODE_NEXT_DISC /* disc change: location "Start of DISC N", disc and field from the 0x800B50xx overlay */
};

/* SaveMenu.state: index into s_menuSaveInputFns / s_menuSaveDrawFns / s_menuSaveUpdateFns */
enum {
    SAVE_ST_NONE,
    SAVE_ST_SLOT,    /* picking MEMORY CARD slot 1/2 */
    SAVE_ST_CHECK,   /* the CHECK window, "Checking..." the card */
    SAVE_ST_CONFIRM, /* Yes/No question (format, overwrite) */
    SAVE_ST_LIST,    /* picking a file */
    SAVE_ST_ACCESS,  /* formatting, saving or loading */
    SAVE_ST_SAVED    /* "Data saved." */
};

/* SaveMenu.phase */
enum {
    SAVE_PH_WAIT,    /* a window or fade is in progress */
    SAVE_PH_MESSAGE, /* message window open (menuSave_inputMessage) */
    SAVE_PH_READY    /* state input runs */
};

#define SAVE_FILES   15      /* rows of the file list, one per card block */
#define SAVE_VERSION 0x30005 /* SaveHeader.version of a valid save, others show as corrupt */
#define SAVE_ROW_H   0x3C    /* file row height in pixels */
#define SAVE_LIST_ROWS 3     /* rows shown at once */
#define SAVE_LIST_X  8
#define SAVE_LIST_Y  0x2A
#define SAVE_LIST_W  0x12C
#define SAVE_LIST_H  (SAVE_LIST_ROWS * SAVE_ROW_H)
#define PORTRAIT_NONE 0xFF   /* SaveMember.portrait of an empty party slot */

/* SaveFile.state: s_menuSaveFileDrawers index, text SAVE_TEXT_FILE_HELP + state */
enum {
    SAVE_FILE_NONE,    /* cleared while a message is up */
    SAVE_FILE_EMPTY,   /* "Empty block." */
    SAVE_FILE_OTHER,   /* "Data from another game." */
    SAVE_FILE_CORRUPT, /* "Data corrupted." (wrong SAVE_VERSION) */
    SAVE_FILE_USED     /* FF9 save */
};

/* Card event results (menuSave_testSwCard/testHwCard: IOE, ERROR, TIMOUT, NEW) and card status per slot */
enum {
    CARD_RES_DONE,
    CARD_RES_ERROR,
    CARD_RES_TIMEOUT, /* no card: "MEMORY CARD is not inserted" */
    CARD_RES_NEW      /* new card (_card_info), or unformatted (_card_load) */
};

/* s_menuSaveCardOp: what menuSave_cardPoll waits for */
enum {
    CARD_OP_INFO,  /* idle: polls _card_info on both slots in turn */
    CARD_OP_LOAD,  /* _card_load */
    CARD_OP_CLEAR, /* HwCARD events, as _card_clear gives (never set here) */
    CARD_OP_3,
    CARD_OP_READ,  /* read of one sector */
    CARD_OP_WRITE  /* write of one sector */
};

#define CARD_SECTOR 0x80   /* read/write unit */
#define CARD_BLOCK  0x2000 /* card block, the unit of DIRENTRY.size */
#define CARD_CHAN(slot) ((slot) << 4) /* slot 0/1 (g_menu.unk2) -> channel 0x00/0x10 */

/* s_menuSaveCheck.step: index into s_menuSaveCheckSteps */
enum {
    CHECK_STEP_NONE,
    CHECK_STEP_CLEAR,   /* new card: _card_clear, then load */
    CHECK_STEP_LOAD,    /* _card_load */
    CHECK_STEP_HEADERS, /* reading each FF9 file's header */
    CHECK_STEP_DONE,
    CHECK_STEP_FAILED
};
#define CHECK_HEADER_SIZE 0x180 /* CardFileHeader + SaveHeader, what the file list needs */

/* s_menuSaveAccess.mode */
enum { ACCESS_SAVE, ACCESS_LOAD };

/* s_menuSaveAccess.stage */
enum {
    ACCESS_STAGE_NONE,
    ACCESS_STAGE_OPEN,       /* window opening, a 4-tick delay */
    ACCESS_STAGE_FORMAT,     /* save to an unformatted card: _card_format next */
    ACCESS_STAGE_FORMATTING, /* waiting for menuSave_onFormatDone */
    ACCESS_STAGE_IO,         /* open the file and start the read/write next */
    ACCESS_STAGE_WAIT        /* waiting for menuSave_onFileIo */
};
#define ACCESS_TICKS 4 /* ticks between access stages */
#define ACCESS_GAUGE_STEP 5 /* gauge pixels per sector */
#define ACCESS_GAUGE_W (SAVE_BUF_SIZE / CARD_SECTOR * ACCESS_GAUGE_STEP)

/* s_menuSaveConfirm.choice */
enum { CONFIRM_YES, CONFIRM_NO };

#define SAVED_MSG_TICKS 0x40 /* "Data saved." closes by itself after this */

/* Animated book of the CHECK and access windows: s_menuSaveBookImage goes to VRAM at
 * (BOOK_X, BOOK_Y), 8-bit, BOOK_STRIPS strips of BOOK_STRIP_H lines,
 * its CLUT to line BOOK_CLUT_Y; 4 frames of 0x2C x 0x21 (s_menuSaveBookFrames). */
#define BOOK_CLUT_Y     0xE0
#define BOOK_CLUT_SIZE  0x200 /* 256 colours */
#define BOOK_X          0x40
#define BOOK_Y          0x100
#define BOOK_W          0x58  /* in 16-bit VRAM units */
#define BOOK_STRIP_H    3
#define BOOK_STRIPS     11
#define BOOK_STRIP_SIZE (BOOK_W * 2 * BOOK_STRIP_H)
#define BOOK_ANIM_TICKS  8 /* ticks per frame */
#define BOOK_ANIM_FRAMES 4

#define CRC16_POLY 0x8408 /* reflected CCITT polynomial */
#define CRC16_INIT 0xFFFF

/* g_saveMenuText ids */
#define SAVE_TEXT_SLOT1           3    /* "slot 1", +1 "slot 2" */
#define SAVE_TEXT_SAVES_TO        5    /* "Saves to\nMEMORY CARD slot 1." (+ slot) */
#define SAVE_TEXT_LOADS_FROM      7    /* "Loads from\nMEMORY CARD slot 1." (+ slot) */
#define SAVE_TEXT_FILE_HELP       9    /* + SaveFile.state: 10 "There is no save data." .. 12 */
#define SAVE_TEXT_MEMBER1         0xD  /* party member name and level line, 0xD-0x10 */
#define SAVE_TEXT_NEWLINE         0x11
#define SAVE_TEXT_DO_NOT_REMOVE   0x12 /* "Do not remove MEMORY CARDs\nor Controllers..." */
#define SAVE_TEXT_EMPTY_BLOCK     0x13
#define SAVE_TEXT_OTHER_GAME      0x14
#define SAVE_TEXT_DATA_CORRUPTED  0x15
#define SAVE_TEXT_SELECT_SLOT     0x16 /* "Select MEMORY CARD slot." */
#define SAVE_TEXT_CHECKING        0x17
#define SAVE_TEXT_FORMATTING      0x19
#define SAVE_TEXT_SELECT_BLOCK    0x1A
#define SAVE_TEXT_SAVING          0x1D
#define SAVE_TEXT_LOADING         0x1E
#define SAVE_TEXT_NO_LOCATION     0x20 /* "Honolulu Harbor Court", when there is no location name */
#define SAVE_TEXT_YES             0x21
#define SAVE_TEXT_NO              0x22
#define SAVE_TEXT_UNFORMATTED     0x23 /* "MEMORY CARD is unformatted.\nDo you want to format?" (+ slot) */
#define SAVE_TEXT_FORMAT_YES      0x25 /* "Begins MEMORY CARD formatting." */
#define SAVE_TEXT_FORMAT_NO       0x26 /* "Cancels MEMORY CARD formatting." */
#define SAVE_TEXT_OVERWRITE       0x27 /* "Overwriting erases\nprevious file.\nOverwrite?" */
#define SAVE_TEXT_OVERWRITE_YES   0x28
#define SAVE_TEXT_OVERWRITE_NO    0x29 /* "Cancels the save." */
#define SAVE_TEXT_FORMATTING_CARD 0x2A /* "Formatting MEMORY CARD." */
#define SAVE_TEXT_DATA_SAVED      0x2C
#define SAVE_TEXT_NO_CARD         0x2D /* "MEMORY CARD is not inserted\ninto the selected slot." */
#define SAVE_TEXT_CHECK_FAILED    0x2E /* "Failed to check MEMORY CARD." */
#define SAVE_TEXT_CARD_FULL       0x2F /* "MEMORY CARD is full.\n1 block required to save." */
#define SAVE_TEXT_NO_FF9_DATA     0x30
#define SAVE_TEXT_FORMAT_FAILED   0x32
#define SAVE_TEXT_SAVE_FAILED     0x33
#define SAVE_TEXT_LOAD_FAILED     0x34
#define SAVE_TEXT_CORRUPTED       0x35 /* "Data corrupted." (CRC mismatch) */
#define SAVE_TEXT_DISC_START      0x36 /* "Start of DISC 1" .. 0x39 "Start of DISC 4" */

/* sounds besides the menu ones */
#define SE_SAVE_LIST    0x414 /* card read, the file list fades in; also format done */
#define SE_SAVE_MESSAGE 0x416 /* a message window opens */
#define SE_SAVE_DONE    0x4ED /* save or load finished */

/* ---- card file layout ---- */

typedef struct {
    u8 v[20];
} SaveName20; /* memory card file name (DIRENTRY.name) */

/* "BASLUS-01251" + "00000-" + the file number in 2 ASCII digits */
#define SAVE_PREFIX_LEN  12   /* "BASLUS-01251", what marks an FF9 file */
#define SAVE_NAME_NUMBER 0x12 /* offset of the digits */

typedef struct {
    SaveName20 name;
    u8 term[2]; /* terminator */
} __attribute__((aligned(2))) SaveFileName; /* copied as a halfword-aligned block */

/* PsyQ libapi struct DIRENTRY */
typedef struct {
    SaveName20 name;
    s32 attr;
    s32 size;
    void *next;
    s32 head;
    u8 system[4];
} DirEnt;

typedef struct {
    u8 v[10];
} __attribute__((aligned(2))) SaveName; /* 10 bytes, copied as a halfword-aligned block */

typedef struct {
    u8 v[10];
} Name10; /* character name, byte-aligned copy */

typedef struct {
    s32 v[7];
} SaveLocation; /* location name, 0x1C bytes of text */

typedef struct {
    u8 present;
    u8 portrait; /* menuSave_getPortrait, 0xFF = none */
    u8 name[0xA];
    u8 level;
    u8 padD[3];
} SaveMember;

typedef struct {
    u8 v[0x23];
} TitleTmpl; /* memory card title template (SJIS) */
typedef struct {
    u8 v[0xA0];
} IconTmpl; /* memory card icon: CLUT (0x20) + 16x16 4-bit bitmap (0x80) */

/* PS1 memory card file header (the first two 0x80-byte frames of the file) */
#define CARD_MAGIC 0x4353 /* "SC" */
#define CARD_ICON_1FRAME 0x11
typedef struct {
    u16 magic;      /* CARD_MAGIC */
    u8 iconFlag;    /* 0x11: one-frame icon */
    u8 blocks;      /* 1 */
    u8 title[0x40]; /* SJIS, from s_menuSaveTitleTemplate */
    u8 pad44[0x1C];
    IconTmpl icon;
} CardFileHeader; /* 0x100 */

/* What the file list shows of a save, after the CardFileHeader (menuSave_buildHeader). */
typedef struct {
    s32 version; /* SAVE_VERSION */
    u8 discs;    /* menuSave_getDiscBit; GameState.unkB on load */
    u8 level;    /* leader's level */
    SaveName name; /* leader's name */
    SaveLocation location;
    s32 playTime; /* game_getPlayTime, frames */
    s32 gil;
    SaveMember party[4];
    s32 windowColor; /* config Window Color (GameState.unk24 bit 18, ovl_34 row 10), the row is drawn in it */
    u32 counter; /* save counter, the newest save gets the cursor */
    u8 cursor;   /* list cursor row when saved */
    u8 pad7D[3];
} SaveHeader; /* 0x80 */

/* game state part of the save data */
typedef struct {
    u8 mode; /* GameState.mode */
    u8 unk1; /* GameState.unkA */
    u8 reserved2[0x20 - 2]; /* written as 0 */
    u16 fieldId;
    u16 unk22; /* GameData.unk3A */
    u16 battleId;
    s16 unk26; /* GameData.unk848 */
    u16 unk28; /* GameData.unk7DC */
    u16 unk2A; /* GameData.unk7DE */
    u8 reserved2C[0x14]; /* written as 0 */
} SaveState;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    CardEntry unk6[0x69];
    u16 unk27C;
    u16 unk27E;
} CardData; /* Tetra Master card list, g_cards (card_getInventory) */

/* other overlay */
u16 fld_getDiscChangeFieldId(void);
u16 fld_getDiscChangeAreaId(void);

typedef struct {
    EventVars vars; /* g_evtGlobalVars */
    s32 unk800; /* g_evtStepCount */
} SaveEvents;

#define SAVE_CHARACTERS 9 /* GameData.characters; also "no member" in SaveParty.party */
typedef struct {
    Character characters[SAVE_CHARACTERS];
    u8 party[4]; /* character index, SAVE_CHARACTERS = none */
    u16 unk514; /* GameData.unk55C */
    u16 firstUseFxSeen;
    u32 gil;
    u16 unk51C;
    u16 unk51E;
    u16 unk520;
} SaveParty; /* 0x524 */

typedef struct {
    s32 unk0; /* GameData.unk844 */
    Unk880 unk4; /* GameData.unk880 */
    u8 unk14; /* GameData.unk892 */
} SaveMisc;

typedef struct {
    ItemInventory items;
    ItemFlags flags; /* GameData.unk774 */
    GameConfig unk240; /* GameState.unk24/unk28 */
    u8 pad248[0x258 - 0x248];
} SaveItems;

/* game data part of the save buffer (menuSave_packData) */
typedef struct {
    SaveState state;
    SaveEvents events;
    u8 pad844[0x850 - 0x844];
    SaveParty party;
    u8 padD74[0xD88 - 0xD74];
    SaveMisc misc;
    SaveItems items;
    u8 cards[0x127C - 0xFF8]; /* CardData */
} SaveData; /* 0x127C */

/* The save file's contents, written and read as SAVE_BUF_SIZE bytes at a fixed address past the overlay. */
typedef struct {
    CardFileHeader card;
    SaveHeader hdr;
    SaveData data;
    u8 pad13FC[2];
    u16 crc; /* menuSave_crc16 of everything before it */
} SaveBuf;
#define SAVE_BUF      PSX_RAM(SaveBuf *, 0x801FCB00)
#define SAVE_BUF_SIZE 0x1400
#define SAVE_CRC_LEN  0x13FE /* bytes covered by SaveBuf.crc: everything before it */

/* ---- menu state ---- */

/* One row of the file list. */
typedef struct {
    s32 version; /* SaveHeader.version */
    SaveFileName fileName;
    u8 state; /* SAVE_FILE_* */
    u8 index; /* row */
    u8 discs;
    u8 level;
    SaveName name;
    SaveLocation location;
    u8 cursor;
    u8 pad45[0x48 - 0x45];
    s32 windowColor; /* SaveHeader.windowColor */
    u32 counter;
    s32 playTime;
    s32 gil;
    SaveMember party[4];
} SaveFile; /* 0x98 */

/* Save menu state (s_menuSave). */
typedef struct {
    u8 mode;  /* SAVE_MODE_* */
    u8 state; /* SAVE_ST_* */
    u8 phase; /* SAVE_PH_* */
    u8 file;  /* selected row */
    u8 cardStatus[2]; /* per slot, CARD_RES_* from menuSave_getCardStatus */
    u8 formatted;  /* the checked card is formatted */
    u8 loaded;     /* a load succeeded (menuSave_isLoaded) */
    u8 showCursor; /* cursor and help shown; off while formatting */
    u8 pad9[3];
    u32 newestCounter; /* highest SaveFile.counter */
    s32 newestFile;    /* row holding it */
    s32 savedFadeOtz; /* fade_setOtz (fade_setOtz in docs/runtime-findings.md) old value, restored by menuSave_close */
    s32 unk18[2]; /* g_padPortMask[] restored by menuSave_exit */
    u8 *help; /* help text */
    MenuList list;
    SaveFile files[SAVE_FILES];
} SaveMenu;
extern SaveMenu s_menuSave;

/* Book animation of the CHECK and access windows (s_menuSaveBookAnim). */
typedef struct {
    u8 enabled;
    u8 tick;  /* 0-7 */
    u8 frame; /* 0-3, s_menuSaveBookFrames */
} BookAnim;
extern BookAnim s_menuSaveBookAnim;

/* Yes/No dialog (s_menuSaveConfirm). */
typedef struct SaveConfirm {
    u8 active;
    u8 choice; /* 0 = Yes, 1 = No */
    u16 textSize; /* text_getInfo: w | rows << 12 */
    s32 text; /* the question (u8 *) */
    Window *win;
    void (*onYes)(struct SaveConfirm *);
    u16 yesHelp; /* text ids */
    u16 noHelp;
} SaveConfirm;
extern SaveConfirm s_menuSaveConfirm;
s32 text_getInfo(s32); /* main exe */

/* Card check, the CHECK window and directory/header reads (s_menuSaveCheck). */
typedef struct {
    u8 active;
    u8 step; /* CHECK_STEP_* */
    u8 file; /* row whose header is being read */
    u8 tries;
    u8 triesPerResult[4]; /* CARD_RES_* */
    s32 dirCount;    /* directory entries */
    s32 otherBlocks; /* blocks of other games' files */
    s32 ff9Files;
    s32 headersRead;
    s32 fd;
    Window *win;
    DirEnt dir[SAVE_FILES];
    u8 pad278[0x2B4 - 0x278];
    u8 unk2B4[SAVE_FILES]; /* cleared, not read */
    u8 pad2C3[0x2D0 - 0x2C3];
    u8 buf[CHECK_HEADER_SIZE];
} SaveCheck;
extern SaveCheck s_menuSaveCheck;

/* "Data saved." window (s_menuSaveSavedMsg). */
typedef struct {
    u8 active; /* never set */
    u8 pad1[3];
    u32 timer; /* closes at 0x40 */
    Window *win;
} SavedMsg;
extern SavedMsg s_menuSaveSavedMsg;

/* Message window (s_menuSaveMessage). */
typedef struct {
    u8 active;
    u8 pad1;
    u16 textSize; /* text_getInfo: w | rows << 12 */
    Window *win;
    u8 *text;
} SaveMessage;
extern SaveMessage s_menuSaveMessage;

/* Format / save / load (s_menuSaveAccess). */
typedef struct {
    u8 active;
    u8 unk1;
    u8 stage; /* ACCESS_STAGE_* */
    u8 mode;  /* ACCESS_SAVE / ACCESS_LOAD */
    u8 lastProgress;  /* sectors done at the last draw */
    u8 progressTicks; /* draws since it changed, eases the gauge */
    SaveFileName fileName;
    Window *win;
    s32 fd;
    s32 delay; /* counts to ACCESS_TICKS */
} SaveAccess;
extern SaveAccess s_menuSaveAccess;

typedef struct {
    u8 u;
    u8 v;
    u8 w;
    u8 h;
} Ovl39Sprite;
extern Ovl39Sprite s_menuSaveBookFrames[];

/* ---- memory card ---- */

extern s32 s_menuSaveCardOp; /* CARD_OP_* */
extern s32 s_menuSaveCardInfoBusy;
extern s32 s_menuSaveCardInfoChan;
extern s32 s_menuSaveFileChan;
extern s32 s_menuSaveSectorsDone;
extern s32 s_menuSaveCardStatus[2];   /* per slot, CARD_RES_* */
extern s32 s_menuSaveCardTimeouts[2]; /* per slot, a timeout counts after one retry */
extern void (*s_menuSaveCardCallback)(void); /* card load/read/write completion */
/* per slot, called after a _card_info result (s32: called through a cast) */
extern s32 s_menuSaveInfoCallback1;
extern s32 s_menuSaveInfoCallback2;
extern s32 s_menuSaveSectorsLeft;
extern s32 s_menuSaveFd;
extern s32 s_menuSaveIoBuf;
extern s32 s_menuSaveCardChan;
/* SwCARD events by result (menuSave_testSwCard) */
extern s32 s_menuSaveSwEventIoe;
extern s32 s_menuSaveSwEventError;
extern s32 s_menuSaveSwEventTimeout;
extern s32 s_menuSaveSwEventNew;
extern s32 s_menuSaveCardResult;
/* HwCARD events by result (menuSave_testHwCard) */
extern s32 s_menuSaveHwEventIoe;
extern s32 s_menuSaveHwEventError;
extern s32 s_menuSaveHwEventTimeout;
extern s32 s_menuSaveHwEventNew;

extern u16 s_menuSaveCrcTable[256];
/* CRC accumulator: the union initializer emits a clobber, so gcc cannot prove
   the upper bits are zero and keeps both andi 0xFFFF (menuSave_crc16). */
typedef union { s32 w; u16 h; } Crc16;

/* main exe: candidates for ff9.h */
/* PsyQ libapi event classes and specs of the memory card */
#ifndef SwCARD
#define SwCARD     0xF4000001
#define HwCARD     0xF0000011
#define EvSpIOE    0x0004
#define EvSpERROR  0x8000
#define EvSpTIMOUT 0x0100
#define EvSpNEW    0x2000
#define EvMdNOINTR 0x2000
#endif
/* PsyQ sys/file.h open() modes; FCREAT takes the block count in bits 16+ */
#ifndef FREAD
#define FREAD  0x0001
#define FWRITE 0x0002
#define FCREAT 0x0200
#define FASYNC 0x8000
#endif
/* GameState.unk24 config word bit (ovl_34): Window Color, 1 = classic */
#ifndef CFG_BIT_WINDOW_COLOR
#define CFG_BIT_WINDOW_COLOR 18
#endif
/* Window.unk1C once win_stepOpen has fully opened the window */
#ifndef WIN_OPEN
#define WIN_OPEN 3
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#endif
EvtObject *evt_getPlayer(void);
u8 fld_getDiscChangeMask(void); /* other overlay */
u8 fld_getDiscChangeDisc(void); /* other overlay */
void config_apply(void);
/* Window labels: entries of main's label string table g_winLabelStrings (ASCII), e.g. D_80070440 "ERROR",
 * D_8007048C "NOTICE", D_80070450 "HELP" (declared in ff9.h) */
extern s32 D_80070420; /* "CHECK" */
extern s32 D_80070444; /* "FILE" */
void text_setNumberArg(s32, s32);
extern u8 *g_menuHelpText; /* = g_menuHelp.text, referenced by its own name */
s32 firstfile(char *, s32); /* PsyQ libapi: struct DIRENTRY *firstfile(char *, struct DIRENTRY *) */
s32 chr_getPortrait(s32); /* defined u8 in main; used unmasked */
char *strcpy(char *, const char *); /* C library (builtin) */
s32 open(char *, s32);
void game_setPlayTime(s32);
extern s32 g_evtStepCount;
void lseek(s32, s32, s32);
/* slider gauge pieces drawn by ui_drawSpriteDef: left cap, middle (repeated via DR_TWIN), right cap */
typedef struct {
    u8 unk0; /* texture window x */
    u8 unk1; /* texture window y */
    u16 unk2; /* uv */
    u8 unk4; /* width */
    u8 unk5; /* height */
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

/* ---- data ---- */

extern u8 g_saveMenuText[];
extern u8 s_menuSaveImage[];
extern u8 s_menuSaveBookImage[]; /* CLUT (0x200) then 11 strips of 0x58x3, 0x210 each */
extern TitleTmpl s_menuSaveTitleTemplate;
extern IconTmpl s_menuSaveIconEven;
extern IconTmpl s_menuSaveIconOdd;
extern IconTmpl s_menuSaveIconMinute;
extern IconTmpl s_menuSaveIconHour;
extern u8 s_menuSaveProductCode[13]; /* "BASLUS-01251" */
extern RECT s_menuSaveSlotHelpRect;
extern RECT s_menuSaveTitleRect; /* a RECT in .data (splat starts .data late) */
extern RECT s_menuSaveHelpRect;
extern RECT s_menuSaveSlotNameRect;
extern RECT s_menuSaveSlotWindowRect;
extern RECT s_menuSaveCheckRect;
extern RECT s_menuSaveSavedMsgRect;
extern RECT s_menuSaveAccessRect;
extern void (*s_menuSaveInputFns[])(void);
extern void (*s_menuSaveDrawFns[])(void);
extern void (*s_menuSaveUpdateFns[])(void);
extern void (*s_menuSaveFileDrawers[])(RECT *, s32, s32); /* by SaveFile.state */
extern void (*s_menuSaveCheckSteps[])(void);
extern u8 s_menuSaveCheckRetries[]; /* tries allowed per CARD_RES_* */
extern u8 s_menuSaveResetAfterMsg[]; /* per SAVE_ST_* */
extern u8 s_menuSavePortraitIds[]; /* portrait per chr_getPortrait result */
extern s32 s_menuSaveAccessTitles[]; /* g_winLabelStrings index per ACCESS_* */
extern RECT s_menuSaveGaugeTexWindow;
extern u32 s_menuSaveGaugeColors[3];
extern u8 s_menuSaveHelpBuf[];
extern u8 s_menuSaveVramBackup[]; /* CLUT row + first image strip */
extern u8 s_menuSaveFilePrefix[13];
extern u8 s_menuSaveLocationBuf[0x26];

/* ---- functions ---- */

/* menu */
void menuSave_drawHelp(void);
void menuSave_drawTitle(void);
void menuSave_drawState(void);
void menuSave_input(void);
void menuSave_updateState(void);
void menuSave_drawCursor(void);
void menuSave_updateHelp(void);
void menuSave_setFileHelp(void);
void menuSave_updateHelpVisibility(void);
void menuSave_backToSlots(void);
void menuSave_returnToSlots(void);
void menuSave_resetToSlots(void);
void menuSave_onSlotsFadedIn(void);
void menuSave_close(void);
void menuSave_selectFile(void);
void func_801F5A60(void);
void menuSave_loadImage(void);
void menuSave_freeImage(void);
u8 menuSave_isSlotReady(s32, u8);

/* file list */
void menuSave_drawFile(RECT *, s32, s32);
void menuSave_drawScrollRows(void);
void menuSave_drawScrollArrows(void);
MenuView *menuSave_getListView(MenuList *, MenuView *);
MenuView *menuSave_getScrollView(MenuList *, MenuView *);
void menuSave_getScrollClip(MenuList *, RECT *, RECT *);
void menuSave_pageUp(MenuList *, s32);
void menuSave_pageDown(MenuList *, s32);

/* book animation */
void menuSave_saveVram(void);
void menuSave_restoreVram(void);
void menuSave_drawBookIcon(s32, s32, s32, s32);
void menuSave_updateBookAnim(void);
void menuSave_resetBookAnim(void);
u8 menuSave_enableBookAnim(u8);
void menuSave_drawBook(s32, s32, s32);

/* confirm */
void menuSave_openConfirm(s32, s32, s32);
void menuSave_initConfirm(void);
void menuSave_setConfirmCallback(void (*)(SaveConfirm *));
u8 *menuSave_getConfirmHelp(void);
void menuSave_drawConfirm(Window *);
void menuSave_drawConfirmCursor(Window *);
void menuSave_onConfirmOpened(Window *);
void menuSave_onConfirmClosed(Window *);

/* card check */
void menuSave_initCheck(void);
void menuSave_openCheck(void);
void menuSave_onCardLoaded(void);
u8 menuSave_readDir(void);
void menuSave_onHeaderRead(void);
void menuSave_enterFileList(void);
void menuSave_findNewest(void);
s32 menuSave_findLastEmpty(void);
s32 menuSave_findUsed(s32);
s32 menuSave_parseFileNumber(u8 *);
void menuSave_onCardFormatted(void);
void menuSave_onCardUnformatted(void);
void menuSave_retryCheck(s32);
void menuSave_drawCheck(Window *);
void menuSave_onCheckOpened(Window *);
void menuSave_onCheckClosed(Window *);
s32 menuSave_checkSpace(void);
s32 menuSave_countFiles(void);
void menuSave_finishCheck(void);
void menuSave_onFileListFadedIn(void);
void menuSave_readHeader(s32);
void menuSave_copyHeader(SaveFile *, SaveHeader *);
void menuSave_checkError(s32);
void menuSave_checkNotice(s32);

/* messages */
void menuSave_initSavedMsg(void);
void menuSave_openSavedMsg(SaveMenu *);
void menuSave_onSavedMsgOpened(Window *);
void menuSave_onSavedMsgClosed(Window *);
void menuSave_drawSavedMsg(Window *);
void menuSave_openMessage(s32, s32);
void menuSave_initMessage(void);
void menuSave_inputMessage(void);
void menuSave_showError(s32);
void menuSave_showNotice(s32);
u8 menuSave_isMessageActive(void);
void menuSave_drawMessage(Window *);
void menuSave_onMessageOpened(Window *);
void menuSave_onMessageClosed(Window *);

/* access */
u8 menuSave_startAccess(s32);
void menuSave_initAccess(void);
void menuSave_updateSave(void);
void menuSave_updateLoad(void);
void menuSave_drawAccess(Window *);
u8 menuSave_writeFile(void);
u8 menuSave_readFile(void);
u8 menuSave_formatCard(void);
void menuSave_drawGauge(s32, s32, s32, s32, s32);
u8 *menuSave_getFilePrefix(void);
void menuSave_onAccessOpened(Window *);
void menuSave_onAccessClosed(Window *);
void menuSave_onFileIo(void);
void menuSave_onFileIoDone(void);
void menuSave_onFileIoFailed(s32);
void menuSave_storeSavedFile(s32, SaveHeader *);
void menuSave_abortAccess(s32);
void menuSave_onFormatDone(void);
void menuSave_onAccessAborted(Window *);

/* save data */
void menuSave_saveFile(s32, s32);
void menuSave_loadFile(s32, s32);
void menuSave_startSave(); /* also used as a SaveConfirm callback */
void menuSave_startLoad(void);
void menuSave_buildHeader(SaveBuf *);
void menuSave_getLocation(u8 *);
void menuSave_buildCardHeader(CardFileHeader *, s32);
s32 menuSave_getDiscBit(void);
void menuSave_initCrcTable(void);
u16 menuSave_crc16(u8 *, s32);
u8 menuSave_applyLoaded(void);
void menuSave_packData(SaveData *);
void menuSave_packState(SaveState *);
void menuSave_unpackState(SaveState *);
void menuSave_packEvents(SaveEvents *);
void menuSave_unpackEvents(SaveEvents *);
void menuSave_packParty(SaveParty *);
void menuSave_unpackParty(SaveParty *);
void menuSave_packMisc(SaveMisc *);
void menuSave_unpackMisc(SaveMisc *);
void menuSave_packItems(SaveItems *);
void menuSave_unpackItems(SaveItems *);
void menuSave_copyCards(CardData *, CardData *);
void menuSave_packCards(void *);
void menuSave_unpackCards(void *);

/* memory card wrappers */
s32 menuSave_firstFile(s32, s32);
s32 menuSave_nextFile(s32);
s32 menuSave_readDirectory(s32, s32);
s32 menuSave_getCardStatus(s32);
s32 menuSave_cardInfoSync(s32);
s32 menuSave_getReadProgress();
s32 menuSave_getWriteProgress();
s32 menuSave_isCardBusy(void);
s32 menuSave_cardClearSync(s32);
s32 menuSave_cardLoad(s32, void (*)(void));
s32 menuSave_cardLoadStart(s32);
s32 menuSave_openFile(s32, char *);
s32 menuSave_closeFile(s32);
s32 menuSave_writeStart(s32, s32, s32);
s32 menuSave_writeSector(void);
s32 menuSave_readStart(s32, s32, s32);
s32 menuSave_readSector(void);
s32 menuSave_createFile(s32, char *);
s32 menuSave_cardFormat(s32);
void menuSave_cardPoll(void);
s32 menuSave_getCardResult(); /* unprototyped: callers pass the channel or nothing */
void menuSave_cardIdle(void);
s32 menuSave_testSwCard(void);
s32 menuSave_testHwCard(void);
void menuSave_clearSwCard(void);
void menuSave_clearHwCard(void);
void menuSave_closeCardEvents(void);

/* Inlined copy of menuSave_isSlotReady. */
static inline u8 menuSave_isSlotReady_inline(s32 slot, u8 allowNew) {
    s32 st = s_menuSave.cardStatus[slot];
    s32 r;

    if (allowNew) {
        r = 0;
        if (st == CARD_RES_DONE || st == CARD_RES_NEW) {
            r = 1;
        }
        return r;
    }
    return st == CARD_RES_DONE;
}

/* card status helpers inlined into menuSave_cardPoll */
static inline void menuSave_cardPoll_set(s32 chan, s32 r) {
    s32 *p = s_menuSaveCardStatus;
    p[chan / 16] = r;
    s_menuSaveCardTimeouts[chan / 16] = 0;
}

static inline void menuSave_cardPoll_retry(s32 chan, s32 r) {
    s32 *p = s_menuSaveCardTimeouts;
    if (p[chan / 16] > 0) {
        s_menuSaveCardStatus[chan / 16] = r;
    } else {
        p[chan / 16]++;
    }
}

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void menuSave_cardInit(void);
void menuSave_openCardEvents(void);
s32 menuSave_readAsync(s32 fd, s32 buf, s32 size, void (*cb) (void));
void menuSave_resetReadProgress(void);
void menuSave_resetWriteProgress(void);
void menuSave_selectSlot(void);
s32 menuSave_writeAsync(s32 fd, s32 buf, s32 size, void (*cb) (void));

#endif
