#ifndef OVL_39_H
#define OVL_39_H

/* Declarations local to overlay 0x39 (save / load menu, loaded at
 * 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#define FF9_BUILTIN_MEMCPY /* see memcpy in psyq.h */
#include "ff9.h"

typedef struct {
    u8 v[20];
} SaveName20; /* memory card file name (DIRENTRY.name) */

typedef struct {
    SaveName20 unk0;
    u8 unk14[2]; /* terminator */
} __attribute__((aligned(2))) SaveFileName; /* copied as a halfword-aligned block */

/* PsyQ libapi struct DIRENTRY */
typedef struct {
    SaveName20 unk0; /* name */
    s32 unk14; /* attr */
    s32 unk18; /* size */
    void *unk1C; /* next */
    s32 unk20; /* head */
    u8 unk24[4]; /* system */
} DirEnt;

typedef struct {
    u8 v[10];
} __attribute__((aligned(2))) SaveName; /* 10 bytes, copied as a halfword-aligned block */

typedef struct {
    u8 v[10];
} Name10; /* character name, byte-aligned copy */

typedef struct {
    s32 v[7];
} SaveBlk1C;

typedef struct {
    u8 unk0; /* present */
    u8 unk1;
    u8 unk2[0xA]; /* name */
    u8 unkC; /* level */
    u8 padD[3];
} SaveMember;

/* Per-slot entry of the save list (0x98 bytes; the first byte is the state). */
typedef struct {
    s32 unk0; /* version tag, 0x30005 */
    SaveFileName unk4;
    u8 unk1A; /* state: 1 = empty, 3 = foreign, 4 = used */
    u8 unk1B; /* slot index */
    u8 unk1C;
    u8 unk1D;
    SaveName unk1E;
    SaveBlk1C unk28;
    u8 unk44;
    u8 pad45[0x48 - 0x45];
    s32 unk48; /* config bit */
    u32 unk4C; /* save counter */
    s32 unk50;
    s32 unk54;
    SaveMember unk58[4]; /* party */
} SaveSlot;

/* Save file header block read into g_menuSaveScan.unk2D0 + 0x100 (menu_save_copyHeader). */
typedef struct {
    s32 unk0;
    u8 unk4;
    u8 unk5;
    SaveName unk6;
    SaveBlk1C unk10;
    s32 unk2C;
    s32 unk30;
    SaveMember unk34[4];
    s32 unk74;
    s32 unk78;
    u8 unk7C;
} SaveHeader;

/* Save menu state (g_menuSave). */
typedef struct {
    u8 unk0;
    u8 unk1; /* state */
    u8 unk2; /* 1 = busy, 2 = done */
    u8 unk3; /* selected slot */
    u8 unk4[2]; /* card status per slot */
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 pad9[3];
    u32 unkC;  /* highest save counter */
    s32 unk10; /* slot holding it */
    s32 unk14; /* saved func_8002F8E4 mode */
    s32 unk18[2]; /* saved D_80073998[] */
    u8 *unk20; /* help text */
    MenuList unk24; /* slot list */
    SaveSlot unk34[15];
} SaveMenu;
extern SaveMenu g_menuSave;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} Unk801FBAC8;
extern Unk801FBAC8 g_menuSaveCardAnim;

typedef struct Unk801FBEE0 {
    u8 unk0;
    u8 unk1;
    u16 unk2; /* text size: w | rows << 12 */
    s32 unk4;
    Window *unk8;
    void (*unkC)(struct Unk801FBEE0 *);
    u16 unk10; /* text ids */
    u16 unk12;
} Unk801FBEE0;
extern Unk801FBEE0 g_menuSaveConfirm;
s32 text_getInfo(s32); /* main exe */

typedef struct {
    u8 unk0;
    u8 unk1; /* state, index into g_menuSaveScanSteps */
    u8 unk2; /* slot */
    u8 unk3; /* attempts */
    u8 unk4[4]; /* attempts per result */
    s32 unk8;
    s32 unkC;  /* used slots */
    s32 unk10; /* file count */
    s32 unk14;
    s32 unk18; /* file descriptor */
    Window *unk1C;
    DirEnt unk20[15];
    u8 pad278[0x2B4 - 0x278];
    u8 unk2B4[15];
    u8 pad2C3[0x2D0 - 0x2C3];
    u8 unk2D0[0x180]; /* read buffer */
} Unk801FBEF8;
extern Unk801FBEF8 g_menuSaveScan;

typedef struct {
    u8 unk0;
    u8 pad1[3];
    u32 unk4; /* frame counter */
    Window *unk8;
} Unk801FC348;
extern Unk801FC348 g_menuSaveSavedMsg;

typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2; /* text size: w | rows << 12 */
    Window *unk4;
    u8 *unk8; /* text */
} Unk801FC358;
extern Unk801FC358 g_menuSaveMessage;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    SaveFileName unk6; /* file name */
    Window *unk1C;
    s32 unk20; /* card channel */
    s32 unk24;
} Unk801FC388;
extern Unk801FC388 g_menuSaveAccess;

/* memory card status */
extern s32 g_menuSaveCardOp;
extern s32 g_menuSaveCardInfoBusy;
extern s32 g_menuSaveCardInfoChan;
extern s32 g_menuSaveFileChan;
extern s32 g_menuSaveSectorsDone;
extern s32 g_menuSaveCardStatus;
extern s32 D_801FB08C;
extern s32 g_menuSaveCardResult;
extern void (*g_menuSaveCardCallback)(void); /* memory card completion callback */
extern s32 g_menuSaveInfoCallbacks;
extern s32 D_801FB0A0;
extern u8 g_menuSaveImage[];
extern u16 g_menuSaveCrcTable[256]; /* CRC-16 table */
/* CRC accumulator: the union initializer emits a clobber, so gcc cannot prove
   the upper bits are zero and keeps both andi 0xFFFF (menu_save_crc16). */
typedef union { s32 w; u16 h; } Crc16;
extern s32 g_menuSaveSectorsLeft;
extern s32 g_menuSaveFd;
extern s32 g_menuSaveIoBuf;
extern s32 g_menuSaveCardChan;
extern s32 g_menuSaveSwEvents;
extern s32 D_801FC5F8;
extern s32 D_801FC5FC;
extern s32 D_801FC600;
extern s32 g_menuSaveHwEvents;
extern s32 D_801FC60C;
extern s32 D_801FC610;
extern s32 D_801FC614;

typedef struct {
    s32 unk0;
    Unk880 unk4;
    u8 unk14;
} Unk801FA078;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2[0x20 - 2];
    u16 unk20;
    u16 unk22;
    u16 unk24;
    s16 unk26;
    u16 unk28;
    u16 unk2A;
    u8 unk2C[0x14];
} Unk801F9BEC;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    Unk6 unk6[0x69];
    u16 unk27C;
    u16 unk27E;
} CardData; /* Tetra Master cards (func_80062824) */

/* other overlay */
u16 func_800B5084(void);
u16 func_800B5094(void);

typedef struct {
    EventVars unk0;
    s32 unk800; /* D_8007AEC4 */
} SaveEvents;

typedef struct {
    Character unk0[9];
    u8 unk510[4]; /* party: character index, 9 = none */
    u16 unk514;
    u16 unk516;
    u32 unk518; /* gil */
    u16 unk51C;
    u16 unk51E;
    u16 unk520;
} SaveParty; /* 0x524 */

/* inventory part of a save block (menu_save_packItems / menu_save_unpackItems) */
typedef struct {
    ItemInventory unk0;
    ItemFlags unk200;
    GameStateUnk24 unk240; /* GameState.unk24/unk28 */
    u8 pad248[0x258 - 0x248];
} SaveItems;

/* game data part of a save block */
typedef struct {
    Unk801F9BEC unk0;
    SaveEvents unk40;
    u8 pad844[0x850 - 0x844];
    SaveParty unk850;
    u8 padD74[0xD88 - 0xD74];
    Unk801FA078 unkD88;
    SaveItems unkDA0;
    u8 unkFF8[0x127C - 0xFF8];
} SaveData; /* 0x127C */

/* save block buffer at 0x801FCB00 */
typedef struct {
    u8 pad0[4];
    u8 unk4;   /* GameState.unkB */
    u8 pad5[0x2C - 5];
    s32 unk2C; /* game_setPlayTime */
    u8 pad30[0x80 - 0x30];
} SaveHdr;

typedef struct {
    u8 unk0[0x100]; /* header */
    SaveHdr unk100;
    SaveData unk180;
    u8 pad13FC[2];
    u16 unk13FE; /* CRC-16 of everything before it */
} SaveBuf;

void menu_save_packData(SaveData *);
void menu_save_packMisc(Unk801FA078 *);
typedef struct {
    u8 pad0[0x12];
    u8 unk12; /* ASCII digits */
    u8 unk13;
} Unk801F6D24;

/* main exe: candidates for ff9.h */
Unk8004AB64 *func_8004AE00(void);
u8 func_800B5074(void); /* other overlay */
void menu_save_buildCardHeader(u8 *, s32);
s32 menu_save_getDiscBit(void);
void func_8002397C(void);
extern RECT g_menuSavePortHelpRect;
extern s32 g_menuSaveAccessTitles[]; /* text index per mode */
extern RECT g_menuSaveAccessRect;
void menu_save_resetCardAnim(void);
void menu_save_openConfirm(s32, s32, s32);
void menu_save_doSave(); /* also used as a Unk801FBEE0 callback */
void menu_save_setConfirmCallback(void (*)(Unk801FBEE0 *));
void menu_save_initScan(void);
void menu_save_initMessage(void);
void menu_save_initSavedMsg(void);
void menu_save_initConfirm(void);
void menu_save_initAccess(void);
void menu_save_loadImage(void);
void func_801F5A60(void);
void menu_save_loadCardIconImage(void);
void menu_save_setHelp(void);
u8 menu_save_formatCard(void);
void menu_save_drawScan(Window *);
void menu_save_onScanOpened(Window *);
void menu_save_onScanClosed(Window *);
u8 menu_save_enableCardAnim(u8);
extern RECT g_menuSaveScanRect;
extern s32 D_80070420;
void menu_save_onConfirmOpened(Window *);
void menu_save_onConfirmClosed(Window *);
void menu_save_drawConfirm(Window *);
void menu_save_drawConfirmCursor(Window *);
void menu_save_drawFile(RECT *, s32, s32);
void menu_save_drawPageScroll(void);
void menu_save_drawScrollArrows(void);
void menu_save_listPageClip(MenuList *, RECT *, RECT *);
extern s32 D_80070444;
extern u8 g_menuSaveHelpBuf[]; /* help text buffer */
void func_8002BE5C(s32, s32); /* main exe */
extern u8 *D_80077DF0; /* = D_80077DE8.unk8, referenced by its own name */
extern void (*g_menuSaveFileDrawers[])(RECT *, s32, s32); /* slot drawer per state */
typedef struct {
    u8 unk0; /* u */
    u8 unk1; /* v */
    u8 unk2; /* w */
    u8 unk3; /* h */
} Ovl39Sprite;
extern Ovl39Sprite g_menuSaveCardIconFrames[];
u8 menu_save_writeFile(void);
extern RECT g_menuSaveHelpRect;
extern RECT g_menuSavePortNameRect;
void menu_save_drawMessage(Window *);
void menu_save_onMessageOpened(Window *);
void menu_save_onMessageClosed(Window *);
void menu_save_setFileHelp(void);
u8 *menu_save_getConfirmHelp(void);
void menu_save_drawAccess(Window *);
s32 menu_save_getReadProgress();
s32 menu_save_getWriteProgress();
void menu_save_drawGauge(s32, s32, s32, s32, s32);
void menu_save_onAccessOpened(Window *);
void menu_save_onAccessClosed(Window *);
s32 firstfile(char *, s32); /* PsyQ libapi: struct DIRENTRY *firstfile(char *, struct DIRENTRY *) */
s32 func_80028954(s32); /* defined u8 in main; used unmasked */

extern RECT g_menuSaveTitleRect; /* a RECT in .data (splat starts .data late) */
extern u8 g_saveMenuText[];
extern u8 g_menuSavePortraitIds[]; /* save icon per character model */
extern u16 D_801FDEFE;
void menu_save_initCrcTable(void);
extern RECT g_menuSavePortListRect;
extern u8 g_menuSaveVramBackup[]; /* CLUT + image buffer */
extern u8 g_menuSaveCardIconImage[]; /* icon images, 0x210 each */
s32 menu_save_readDirectory(s32, s32);
extern u8 g_menuSaveLocationBuf[0x26]; /* name buffer */
u8 func_800B5064(void); /* other overlay */
void menu_save_getLocation(u8 *);
extern u8 g_menuSaveScanRetries[]; /* retries allowed per result */
void menu_save_retryScan(s32);
void menu_save_onCardFormatted(void);
void menu_save_onCardUnformatted(void);
void menu_save_onFileListFadedIn(void);
char *strcpy(char *, const char *); /* C library (builtin) */
s32 open(char *, s32); /* main exe: open */
s32 menu_save_openFile(s32, char *);
u8 menu_save_createFile(s32, char *);
void game_setPlayTime(s32); /* main exe */
void menu_save_unpackState(Unk801F9BEC *);
void menu_save_unpackItems(SaveItems *);
void menu_save_unpackCards(void *);
void menu_save_unpackMisc(Unk801FA078 *);
void menu_save_buildHeader(u8 *);
u16 menu_save_crc16(u8 *, s32);
void menu_save_packState(Unk801F9BEC *);
void menu_save_packEvents(SaveEvents *);
void menu_save_unpackEvents(SaveEvents *);
void menu_save_unpackParty(SaveParty *);
void menu_save_packParty(SaveParty *);
extern s32 D_8007AEC4;
void menu_save_packItems(SaveItems *);
void menu_save_packCards(void *);
void menu_save_onHeaderRead(void);


void menu_save_drawHelp(void);
void menu_save_drawTitle(void);
void menu_save_draw(void);
void menu_save_input(void);
void menu_save_tick(void);
void menu_save_returnToPorts(void);
void menu_save_drawCardIcon(s32, s32, s32, s32);
void menu_save_openMessage(s32, s32);
void menu_save_finishScan(void);
void menu_save_drawCardAnim(s32, s32, s32);
void menu_save_enterFileList(void);
void menu_save_findNewest(void);
void menu_save_scanError(s32);
u8 menu_save_isMessageActive(void);
void menu_save_selectLoadFile(s32, s32);
void menu_save_selectSaveFile(s32, s32);
s32 menu_save_writeSector(void);
void menu_save_clearHwCard(void);
s32 menu_save_isCardBusy(void);
s32 menu_save_cardLoad(s32, void (*)(void));
s32 menu_save_cardFormat(s32);
void menu_save_onCardLoaded(void);
void menu_save_scanInfo(s32);
void menu_save_openSavedMsg(SaveMenu *);
void menu_save_showInfo(s32);
void menu_save_onPortsFadedIn(void);
void menu_save_abortAccess(s32);
s32 menu_save_testSwCard(void);
void menu_save_clearSwCard(void);
void menu_save_showError(s32);
u8 menu_save_startAccess(s32);
void menu_save_copyCards(CardData *, CardData *);
s32 menu_save_cardLoadStart(s32);
s32 menu_save_writeStart(s32, s32, s32);
s32 menu_save_readStart(s32, s32, s32);
void menu_save_cardIdle(void);
void menu_save_drawCursor(void);
void menu_save_restoreVram(void);
u8 menu_save_readDir(void);
s32 menu_save_findUsed(s32);
void menu_save_readHeader(s32);
void menu_save_copyHeader(SaveSlot *, SaveHeader *);
void menu_save_freeImage(void);
void menu_save_onAccessAborted(Window *);
s32 menu_save_closeFile(s32);
s32 menu_save_nextFile(s32);
s32 menu_save_testHwCard(void);
void menu_save_closeCardEvents(void);
u8 menu_save_isPortReady(s32, u8);
s32 menu_save_firstFile(s32, s32);
void menu_save_onFormatDone(void);
extern u8 g_menuSaveFilePrefix[13];
extern u8 g_menuSaveProductCode[13]; /* "BASLUS-01251" */
s32 menu_save_cardClearSync(s32);
s32 menu_save_readSector(void);
s32 menu_save_getCardStatus(s32);
void menu_save_cardPoll(void);
void menu_save_messageInput(void);
void menu_save_saveTick(void);
void menu_save_loadTick(void);
void menu_save_doLoad(void);
void menu_save_startScan(void);
void menu_save_updateHelpVisibility(void);
void menu_save_tickCardAnim(void);
extern RECT g_menuSaveSavedMsgRect;
extern u8 g_menuSaveResetAfterMsg[];
extern s32 D_801FBF18;
s32 menu_save_cardInfoSync(s32);
s32 menu_save_getCardResult(); /* unprototyped: callers pass the channel or nothing */
void menu_save_onFileIoDone(void);
void menu_save_onFileIoFailed(s32);
void menu_save_resetToPorts(void);
void menu_save_onSavedMsgOpened(Window *);
void menu_save_onSavedMsgClosed(Window *);
void menu_save_drawSavedMsg(Window *);
u8 menu_save_readFile(void);
void menu_save_onFileIo(void);
MenuView *menu_save_listGetView(MenuList *, MenuView *);
void menu_save_storeSavedFile(s32, SaveHeader *);
u8 menu_save_applyLoaded(void);
void menu_save_backToPorts(void);
void menu_save_close(void);
void menu_save_selectFile(void);
void menu_save_listPageUp(MenuList *, s32);
void menu_save_listPageDown(MenuList *, s32);
MenuView *menu_save_listGetPageView(MenuList *, MenuView *);
void menu_save_drawFile(RECT *, s32, s32);
extern void (*g_menuSaveInputFns[])(void);
extern void (*g_menuSaveTickFns[])(void);
extern void (*g_menuSaveDrawFns[])(void);
extern void (*g_menuSaveScanSteps[])(void);



/* Inlined copy of menu_save_isPortReady (slot usable?). */
static inline u8 menu_save_isPortReady_inline(s32 slot, u8 any) {
    s32 st = g_menuSave.unk4[slot];
    s32 r;

    if (any) {
        r = 0;
        if (st == 0 || st == 3) {
            r = 1;
        }
        return r;
    }
    return st == 0;
}

extern s32 g_menuSaveCardTimeouts[2]; /* per card port: retries; g_menuSaveCardStatus holds the status */
/* main exe: candidates for ff9.h */
void lseek(s32, s32, s32);
/* card status helpers inlined into menu_save_cardPoll */
static inline void menu_save_cardPoll_set(s32 chan, s32 r) {
    s32 *p = &g_menuSaveCardStatus;
    p[chan / 16] = r;
    g_menuSaveCardTimeouts[chan / 16] = 0;
}

static inline void menu_save_cardPoll_retry(s32 chan, s32 r) {
    s32 *p = g_menuSaveCardTimeouts;
    if (p[chan / 16] > 0) {
        (&g_menuSaveCardStatus)[chan / 16] = r;
    } else {
        p[chan / 16]++;
    }
}

s32 menu_save_countFiles(void);
u8 *menu_save_getFilePrefix(void);
s32 menu_save_parseFileNumber(Unk801F6D24 *);
s32 menu_save_findLastEmpty(void);
s32 menu_save_checkSpace(void);

typedef struct {
    u8 v[0x23];
} TitleTmpl; /* memory card title template (SJIS) */
typedef struct {
    u8 v[0xA0];
} IconTmpl; /* memory card icon CLUT + bitmap */
extern TitleTmpl g_menuSaveTitleTemplate;
extern IconTmpl g_menuSaveIconEven;
extern IconTmpl g_menuSaveIconOdd;
extern IconTmpl g_menuSaveIconMinute;
extern IconTmpl g_menuSaveIconHour;

/* main exe: candidates for ff9.h */
/* slider gauge pieces drawn by func_80027710: left cap, middle (repeated via DR_TWIN), right cap */
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
extern u32 g_menuSaveGaugeColors[3]; /* gauge line colours */
extern RECT g_menuSaveGaugeTexWindow; /* gauge texture window */


#endif
