#ifndef FF9_H
#define FF9_H

#include "common.h"
#include "psyq.h"

/* An argument the original code never passed: the callee reads an argument register that still holds this
 * value (usually the caller's own parameter), which only works on MIPS. Empty here, so the call compiles as
 * in the original (passing it explicitly changes register allocation); a port defines it as `x`.
 * tools/arg_check.py finds these. */
#ifndef REG_PASSTHROUGH
#define REG_PASSTHROUGH(x)
#endif

/* Pins a local to a MIPS register (`register T *v REG_ASM("$8") = ...;`) where register allocation alone keeps a
 * function from matching. No effect on behaviour; a port defines it empty. */
#ifndef REG_ASM
#define REG_ASM(r) asm(r)
#endif

/* Is this offset-or-pointer field still a file offset, i.e. not yet relocated in place? Relocated values are
 * KSEG0 pointers (0x80xxxxxx, negative as s32). A port whose pointers are positive defines its own. */
#ifndef IS_OFFSET
#define IS_OFFSET(x) ((s32)(x) >= 0)
#endif

/* Layouts are partial: only offsets seen in decompiled code are named.
 * Names are placeholders (unkXX = offset) until their purpose is known.
 * Split into per-subsystem headers once subsystems are identified. */

/* ---- game state (g_gameState) ---- */

typedef struct {
    u16 unk0; /* flags */
    u16 unk2;
    s32 fileId; /* id, -1 = none */
    u32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
} PreloadEntry; /* element passed to preload_clearEntry */

typedef struct PreloadQueue {
    u8 unk0;
    u8 state; /* state */
    s8 count; /* count */
    s8 cur;
    PreloadEntry unk4[7];
    s32 (*unk11C)(struct PreloadQueue *); /* hook, nonzero = handled */
} PreloadQueue;
PreloadEntry *preload_makeChunk(PreloadEntry *, PreloadEntry *);
s32 preload_startRead(void);

typedef struct {
    u8 unk0;
    u8 unk1;
} UnkPair;

typedef struct {
    u16 unk0; /* (second table) high byte: first two-byte code */
    u8 pad2[2];
    u16 unk4; /* high byte: first text code that takes an extra byte */
    u16 unk6;
    s16 unk8;
    u8 padA[2];
    u32 unkC[1]; /* glyph info, width in the top 4 bits */
} Unk7D4;

/* Character.unk34: packed ids, also read as a whole word */
typedef union {
    u32 w;
    struct {
        u32 id : 4;    /* character id (index into GameData.unk3C) */
        u32 unk4 : 5;  /* index into D_8006BB8C */
        u32 unk9 : 1;
        u32 unkA : 1;
        u32 unkB : 1;
        u32 row : 5;   /* ability table row (g_abilityRows) */
        u32 unk11 : 15;
    } b;
} CharBits;

/* Character record: GameData.characters[9]; GameData.party[] points at the party members. Field names checked
 * against the main menu at runtime (docs/runtime-findings.md). */
typedef struct {
    u8 unk0[0xA]; /* name: 0xFF-terminated text, read up to 7 chars */
    u8 unkA;      /* bit 4 tested */
    u8 level;
    u32 exp;      /* capped at 9999999 */
    u16 hp;
    s16 mp;
    u8 pad14[0x16 - 0x14];
    u8 unk16;
    u8 stones;    /* magic stones left */
    u16 maxHp;
    s16 maxMp;
    s16 maxAtb;   /* runtime: where the battle ATB gauge stops */
    u8 pad1E;
    u8 maxStones;
    u8 trance;    /* gauge 0-255; runtime: 121 with the status bar about half full */
    u8 unk21;
    u8 pad22[0x24 - 0x22];
    /* as the Status screen shows them (runtime, Zidane Lv12: 24 26 22 25 / 9 10 10 7) */
    u8 speed;
    u8 strength;
    u8 magic;
    u8 spirit;
    u8 defense;
    u8 evade;
    u8 magicDef;
    u8 magicEva;
    u16 unk2C;
    u16 unk2E;
    /* before equipment bonuses (runtime: 24 25 22 24 vs 24 26 22 25 shown) */
    u8 baseSpeed;
    u8 baseStrength;
    u8 baseMagic;
    u8 baseSpirit;
    CharBits unk34;
    u8 status;   /* persistent status bits shown by ui_drawStatusIcons (runtime); 0x45 = dead/stone/... (can't gain exp) */
    u8 equip[5]; /* item ids: weapon, head, arm, body, accessory (0xFF = none); runtime: Zidane 3 115 90 152 255 with no accessory */
    u16 unk3E[5]; /* stat bonuses from equipment */
    u8 pad48[0x58 - 0x48];
    u8 ap[0x30]; /* AP per ability slot of the character's g_abilityRows row (chr_getAp, chr_clearAp) */
    u32 supportAbilities[2]; /* equipped: bit (id - 0xC0) (runtime, chr_setSupportAbility) */
} Character; /* 0x90 */

/* GameData.unk8E0 entry */
typedef struct Unk8E0 {
    s32 unk0; /* flags */
    u8 pad4[8 - 4];
    struct BtlUnit9C *unk8;
    struct BtlUnitA0 *unkC;
    struct BtlUnit *unk10;
} Unk8E0;

typedef struct {
    u8 pad0[4];
    s32 unk4; /* load address */
} Unk1354;

typedef struct {
    u8 pad0[4];
    s16 unk4; /* set by game_setNextScene */
    u8 unk6;
    u8 pad7[0x10 - 7];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    u8 *unk1C; /* load address */
    u8 pad20[4];
    s16 unk24; /* set by game_setNextScene */
    u8 pad26[2];
    struct Unk11C0Slot *unk28; /* per-object model slots, indexed by unk3B */
} Unk11C0;

/* Unk11C0.unk28 entry (0x28 bytes), written by script opcodes in evt_modelOpBattle */
typedef struct Unk11C0Slot {
    u32 unk0_0 : 8;
    u32 unk0_8 : 12;
    u32 unk0_20 : 12;
    u32 unk4_0 : 12;
    u32 unk4_12 : 10;
    u32 unk4_22 : 10;
    POLY_FT4 *unk8[2][4]; /* prims, filled by func_80062E3C */
} Unk11C0Slot;

typedef struct {
    u8 pad0[0xA];
    s16 unkA; /* set by game_setNextScene */
    u8 padC[0x2C - 0xC];
    struct Unk11C0Slot *unk2C; /* per-object model slots, indexed by unk3B */
} Unk8E4C14;

typedef struct {
    u8 pad0[8];
    struct Unk8007B954 *unk8; /* message table */
    u8 padC[0x14 - 0xC];
    Unk8E4C14 *unk14;
} Unk8E4C;

typedef struct {
    u32 unk0;
    u8 pad4[4];
    Unk11C0 *unk8;
    Unk8E4C *unkC;
    u8 pad10[0xD80 - 0x10];
    u8 unkD80;
    u8 padD81[0x11C0 - 0xD81];
    Unk11C0 *unk11C0;
    u8 pad11C4[0x1340 - 0x11C4];
    u8 unk1340;
    u8 pad1341[0x1354 - 0x1341];
    Unk1354 *unk1354;
} Unk8E4;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Unk880; /* copied as a block by ovl_39 */

/* GameData.unk8E4: per-module context; its layout depends on the loaded
 * overlay. */
struct WorldMap; /* ovl_0c, include/ovl_0c.h */
struct FieldMap; /* ovl_0a, include/ovl_0a.h */
struct Battle;   /* ovl_0b, include/ovl_0b.h */
typedef union {
    Unk8E4 *ovl65;
    struct WorldMap *world;
    struct FieldMap *field;
    struct Battle *battle;
} Unk8E4Ptr;


/* 0x98-byte per-object record (GameData.unk8D4 / unk8DC) */
typedef struct Unk8D4 {
    u8 pad0[0x98];
} Unk8D4;

typedef struct CdReadCtx {
    s32 unk0; /* start sector */
    s32 unk4; /* total bytes */
    u8 *unk8; /* destination */
    s32 unkC; /* bytes done */
    s32 unk10; /* chunk size, -1 = all */
    void (*unk14)(struct CdReadCtx *); /* done callback */
    s32 unk18;
    s32 unk1C; /* state */
    u8 *unk20;
} CdReadCtx; /* GameData.unk854 */

/* runtime: menu_items shows Potion 46, Hi-Potion 10, Ether 12, Phoenix Down 14, ... = {236,46} {237,10} {238,12} {240,14} */
typedef struct {
    u8 id;
    u8 count;
} ItemSlot;

/* GameData.items: item inventory, copied as a block by the save code (ovl_39) */
typedef struct {
    ItemSlot slots[0x100];
} __attribute__((aligned(4))) ItemInventory; /* block copies are word-aligned */

/* GameData.unk774: 2 bits per item id */
typedef struct {
    u8 unk0[0x40];
} __attribute__((aligned(4))) ItemFlags;

/* g_gameState->unk1C: looks like the save-data block */
typedef struct {
    u32 unk0; /* flags */
    u32 unk4; /* allocator top (g_allocTop) after setup */
    u8 unk8; /* draw buffer index (0/1) */
    u8 unk9;
    u16 unkA; /* projection distance (SetFogNearFar) */
    u16 fieldId; /* current field (runtime: Dali inn 351 -> street 350) */
    u16 battleId; /* next battle (encounter bundle id), set by evt_setNextBattle / the field and world exits */
    u32 *unk10[2]; /* ordering tables, 0x1000 entries */
    MATRIX unk18;
    u8 unk38;
    u8 unk39;
    u16 unk3A;
    Character characters[9];
    Character *party[4];
    u16 unk55C;
    u16 firstUseFxSeen;
    u32 gil; /* capped at 9999999 (runtime: menu shows this value) */
    u8 pad564[0x568 - 0x564];
    s16 unk568;
    s16 unk56A;
    s16 unk56C;
    u8 battleEndKind; /* 6 = game over (ovl_65 battle_end; event system variable 27) */
    u8 unk56F;
    u8 unk570;
    u8 unk571;
    u8 pad572[2];
    ItemInventory items;
    ItemFlags unk774;
    u8 pad7B4[0x7D4 - 0x7B4];
    Unk7D4 *unk7D4;
    void *unk7D8;
    s16 unk7DC;
    u16 unk7DE;
    s32 unk7E0;
    struct FileTable *depTable; /* the loaded field/world file's dependency table (dep_find), -1 = none */
    u16 unk7E8;
    u16 unk7EA;
    s32 returnSector[2]; /* sceneSector/sceneSectors saved by fld_exit, to return to after a battle */
    s32 returnSectors[2];
    s32 sceneSector[2]; /* CD sectors of the next scene's files: field/world file + area, or battle bundle + background */
    s32 sceneSectors[2]; /* their sector counts */
    u8 *unk80C; /* name text */
    u8 *unk810;
    void *unk814;
    s32 unk818;
    u16 unk81C[2];
    u16 unk820[2];
    u8 unk824[2];
    u8 unk826[2];
    void *unk828;
    void *unk82C;
    u16 unk830[2];
    u16 unk834;
    u16 unk836;
    s32 (*unk838)(s32, void *);
    s32 (*unk83C)(s32, s32, s32, s32, s32);
    u16 unk840;
    u16 unk842;
    s32 chocograph; /* selected chocograph 1-24, 0 = none (runtime field/menu campaign) */
    s8 unk848;
    u8 unk849[1];
    u16 unk84A;
    Unk7D4 *unk84C; /* second (kanji) font table */
    u8 unk850;
    u8 unk851;
    u16 unk852;
    CdReadCtx unk854[1]; /* CD read context (cdRead_init) */
    s32 unk878;
    s32 unk87C; /* end of the world map data */
    Unk880 unk880;
    u8 unk890;
    u8 pad891;
    u8 unk892;
    u8 pad893[0x8A8 - 0x893];
    s32 unk8A8;
    s32 unk8AC;
    u16 unk8B0;
    s16 unk8B2;
    u16 unk8B4[2];
    u8 unk8B8[2];
    u8 unk8BA[2];
    void *unk8BC; /* callback, signature varies by module */
    s32 unk8C0;
    s32 unk8C4; /* VSync(-1) timestamp */
    PreloadQueue *unk8C8;
    u8 unk8CC[8];
    struct Unk8D4 *unk8D4; /* [9] */
    struct Unk8E0 *unk8D8; /* [9] */
    struct Unk8D4 *unk8DC;
    struct Unk8E0 *unk8E0; /* indexed by unk8E4.field->unk8 */
    Unk8E4Ptr unk8E4;
} GameData;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x24 - 0xC];
    s32 unk24;
    s32 unk28;
    u8 pad2C[0x34 - 0x2C];
    s32 unk34;
    s32 unk38;
    u8 pad3C[0x44 - 0x3C];
    s32 unk44;
    s32 unk48;
    u8 pad4C[0x94 - 0x4C];
    s32 unk94;
    s32 unk98;
    u8 pad9C[0xDC - 0x9C];
    s32 unkDC; /* battle data file start sector */
} GameStateUnk14;

typedef struct {
    u32 unk0; /* flags */
    u32 allocTop; /* allocator top after boot (g_allocTop, game_initState) */
    u8 mode; /* runtime: 0 title, 1 field (and its menus) */
    u8 vsyncMode; /* VSync mode */
    u8 unkA;
    u8 disc;          /* disc bit (1 << disc - 1) of the SLUS file file_initImg found, 0xFF = none */
    DISPENV *dispEnv;    /* [2] */
    DRAWENV *drawEnv;   /* [2] */
    GameStateUnk14 *dirs; /* FF9.IMG directory records (file_initImg) */
    void **dirTables; /* file tables per directory (file_loadDirTables) */
    GameData *unk1C;
    s32 imgSector; /* FF9.IMG's first sector, added to file start sectors (file_initImg) */
    u32 config; /* Config screen settings, viewed bit by bit as GameConfig (with padMap) */
    u32 padMap; /* custom pad mapping, 4 bits per action */
    u32 playTime; /* frames at 60/s, shown as h:mm:ss in the menu (runtime) */
    s32 dirCount; /* FF9.IMG directories (file_initImg) */
    u8 unk34[15];
} GameState;

extern GameState *g_gameState;
void file_loadDirTables(); /* game_initState calls it without arguments */

/* Inlined copy of chr_testSupportBit (support ability bit test). */
static inline u8 chr_testSupportBit_inline(u32 *bits, s32 b) {
    b -= 0xC0;
    return (bits[b >> 5] & (1 << (b & 0x1F))) != 0;
}

/* Inlined copy of item_findSlot (item search); the original source inlines it. */
static inline ItemSlot *item_findSlot_inline(s32 id) {
    s32 i;
    ItemSlot *e = g_gameState->unk1C->items.slots;

    for (i = 0; i < 0x100; i++, e++) {
        if (e->count != 0 && e->id == id) {
            return e;
        }
    }
    return NULL;
}

/* ---- event countdown timer and play-time clock (VSync based) ---- */

/* script countdown timer (event opcodes: timer_setSeconds/timer_enable/timer_setRunning, timer_getSeconds) */
typedef struct {
    u8 enabled; /* drawn by game_updateTimer */
    u8 running; /* counting down */
    u8 pad2[2];
    s32 startVsync; /* VSync(-1) when enabled */
    s32 initial; /* frames */
    s32 remaining; /* frames */
} EvtTimer;

typedef struct {
    s32 lastVsync; /* VSync(-1) at the last play-time update */
    u8 pkt[0x400]; /* timer window packets, 0x200 per buffer */
    u8 *pktPtr; /* write pointer into pkt */
    EvtTimer timer;
} ClockState;
extern ClockState g_clock;
extern EvtTimer g_timer; /* the same timer as g_clock.timer, under its own symbol in the original */
extern RECT g_timerRect; /* countdown timer window */

/* ---- CD / streaming state near libcd ---- */

/* CD request, queued by cd_submit (0x24 bytes) */
/* one contiguous read: byte count and destination */
typedef struct {
    u32 size; /* runtime: ovl_39's single chunk {0xE800, 0x801EDF00} */
    u8 *dest;
} CdChunk;

typedef struct {
    u8 flags; /* command; bit 2: run unk18 directly when idle */
    u8 pad1[3];
    s32 sector; /* start sector; runtime: 0x5FA for ovl_39 */
    s32 size;   /* bytes; runtime: 0xE800 (29 sectors) for ovl_39 */
    CdChunk *unkC; /* chunk list; -1 = use unk1C */
    void (*syncCallback)(u8, u8 *);
    void (*readyCallback)(u8, u8 *); /* data-ready handler */
    void (*doneCallback)(); /* callback */
    CdChunk unk1C; /* single chunk */
} CdRequest;

typedef struct {
    u8 state;
    u8 unk1;
    u8 hasQueued;
    u8 unk3;
    s32 sectorsLeft; /* runtime: 29 -> 0 while ovl_39 loads */
    CdlLOC unk8; /* current position */
    s32 bytesRead;   /* runtime: +0x800 per sector */
    u32 chunkOfs;    /* offset in the current chunk */
    s32 chunkIndex;
    s32 unk18;
    u8 pad1C[0x944 - 0x1C];
    CdRequest cur; /* current request */
    CdRequest queued; /* passed to cd_startRequest */
} CdLoadState;
extern CdLoadState g_cdLoad;

/* ---- drawing ---- */

extern u32 *g_ot; /* ordering table */
extern u8 *g_primPtr;  /* next free primitive in the packet buffer */
extern s16 D_80068F70[];
extern u8 D_8006BB8C[];

typedef struct {
    u16 unk0; /* name offset into g_itemNames */
    u16 unk2; /* name offset into the shop's text block */
    u16 unk4; /* price */
    u16 unk6; /* who can equip (mask) */
    u8 unk8; /* kind (icon) */
    u8 unk9; /* icon palette */
    u8 unkA; /* equip rank */
    u8 unkB; /* stat bonus row (D_8006D548) */
    u8 unkC[3]; /* abilities taught */
    u8 unkF;    /* mask tested against D_8006BB84[] */
    u8 unk10;   /* sort key */
    u8 pad11;
} Unk80069018; /* item table, 0x12-byte entries */
extern Unk80069018 g_itemTable[];
extern u8 D_8006CA9C[4]; /* max hp/mp support abilities */
extern u8 D_8006CAA0[4]; /* stat caps */
extern u8 D_8006D1C8[][4]; /* armor bonuses, indexed by item id (0x58..0xDF) */
/* weapon table entry (0xC bytes) */
typedef struct {
    u8 pad0[5];
    u8 unk5; /* attack */
    u8 pad6[0xC - 6];
} WeaponEntry;
#if !defined(OVL_0B_H) && !defined(OVL_38_H) /* those headers declare their own view */
extern WeaponEntry g_weaponTable[];
#endif
extern u8 D_8006BB84[];
extern u8 D_8006BB74[];

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy;
    u32 wh;
} TileWords; /* TILE written as whole words */

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 xy1;
    u32 xy2;
    u32 xy3;
} PolyF4Words; /* POLY_F4 written as whole words */

typedef struct {
    u32 tag;
    u32 code;
} DrTpageWords;

extern u16 g_textPalettes[];
extern u32 D_8006BBB4[]; /* per-style clut row offset for text_buildDigits */

/* sprite descriptor: uv/clut word, size, tpage (D_800686E8) */
typedef struct {
    u32 unk0; /* uv0 | clut << 16 */
    u8 unk4;  /* w */
    u8 unk5;  /* h */
    u16 unk6; /* tpage */
} Unk800686E8;
extern Unk800686E8 D_800686E8;
extern Unk800686E8 g_uiSprites[];
extern Unk800686E8 D_800687E0[]; /* digit glyphs */

/* 12-tick toggle timer (ui_initMemberSwitch/D08/D2C) */
typedef struct {
    u8 unk0; /* active */
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4; /* ticks */
    void (*unk8)(void);
} UnkBlink;

/* ---- misc globals ---- */

extern UnkPair g_padActuator[2];

typedef struct {
    u8 state; /* 0 closed, 1 loading, 2 waiting for the read/fade, 3 running (g_menuStateFuncs) */
    u8 type; /* menu_open type (g_menuTypes) */
    u8 unk2; /* memory card slot */
    u8 unk3;
    u8 unk4;
    u8 open; /* set by menu_doOpen, cleared by menu_finish */
    u8 reading; /* overlay/portrait read running (g_menuReading) */
    u8 fadingOut;
    u8 pad8;
    u8 drawBackground; /* menu_updateRun draws ui_drawBackground */
    u8 fadeInOnClose;
    u8 fadeOutOnClose;
    u32 arg; /* menu_open argument; bits 0-13: id, bits 14-15: disc */
    s32 frame; /* frame counter */
    s32 savedVsyncMode;
    s32 savedPadMask;
    s32 unk1C;
    u8 savedR; /* saved background colour */
    u8 savedG;
    u8 savedB;
    u8 pad23;
    s32 onClose; /* run by menu_finish */
} MenuState;
extern MenuState g_menu;

extern s32 g_nameGlyphs;
extern s32 g_nameImage;
extern u8 D_80010BD0[]; /* font glyph start/width nibbles, text_renderNameGlyph */
extern void (*g_winShadowFuncs[])(RECT *, s32);
extern s32 (*g_textMeasureFuncs[])(u8 *); /* text width of a value, per format */
extern u8 D_8006A20E;
s32 text_getCodeWidth(s32);
s32 text_measureNumber(s32, s32);
#if !defined(OVL_0A_H) && !defined(OVL_0C_H)
s32 text_measureStyle(u8 *, s32);
#endif
extern s32 D_8006BBFC[];
extern s32 g_buttonSprites[];
extern u8 g_textNumberBuf[];
extern u8 *D_800770A8;
extern s32 g_textColor;
extern u8 *g_textStringArgs[]; /* menu strings (text_setStringArg) */
extern s32 g_textNumberArgs[];
extern u8 g_textIconArgs[];
extern s32 g_winLastVsync;

typedef struct {
    u8 cur; /* buffer being built */
    u8 unk1;
    u8 unk2;
    u8 pad3;
    s32 freeBytes; /* free bytes in the packet buffer */
    s32 lastVsync; /* VSync(-1) of the last window update (g_winLastVsync) */
    u8 *prims[2];   /* packet buffers */
    u32 *ots[2]; /* ordering tables */
    DVECTOR dispPos[2]; /* display x/y per buffer */
} DrawBufs;
extern DrawBufs g_drawBufs;

/* window frame insets per style (rodata, win_setClip) */
typedef struct {
    s8 unk0[5][4];
} Unk80010CE0;
extern Unk80010CE0 D_80010CE0;

/* ---- functions (main exe) ---- */

s32 db_unregister(void *);
void memset(void *, s32, s32);
void (*CdReadyCallback(void (*)()))(); /* returns the previous callback */
void preload_clearEntry(PreloadEntry *);
void cd_controlF(u8, u8 *);
void cd_onStreamReady(u8, u8 *);
void ui_drawCharStatus(s32, s32, s32, Character *);
u8 *win_buildFill(RECT *, u32 *, u8, u8, s32, u8 *);
u8 *win_buildBorder(RECT *, u32 *, u8, u8, s32, s32, u8 *);
void win_drawHelpBorder(RECT *, s32, u8, u8, s32);
void win_drawHelpTailEdge(); /* (s16 x, s16 y, s32 otz, s32 n, u8 flag), K&R */
void win_drawMsgTailEdge(); /* (s16 x, s16 y, s32 otz, s32 n, u8 flag), K&R */
void win_drawTailFill(s32, s32, s32, s32, s32, s32);
void win_drawHelpFill(RECT *, s32);
void win_drawHelpTailFill(s32, s32, s32, s32);
void win_drawTailEdge(s16, s16, s32, s32, u8, u8);
u8 *ui_buildItemIcon(s32, s32, u32 *, s32, u8, u8 *);
void ui_drawMemberStats(s32, s32, s32, s32);
void ui_drawCharStatuses(s32, s32, s32, s32);
u8 *ui_buildApGauge(s32, s32, u32 *, s32, s32, u8, u8, u8 *);
s32 ui_drawSprite(s32, s32, s32, s32, u8);
void ui_drawPartyPortrait(s32, s32, s32, s32, u8);

void text_formatNumber(s32, u8 *, s32, u8);
void item_clearAll(void);
void keyItem_clearAll(void);
s32 item_add(s32, s32);
void ui_drawPortraitId(s32, s32, s32, s32, s32, u8);
extern s32 g_portraitCount; /* number of portraits (ui_drawPortraitId) */
void ui_drawPortrait(s32, s32, s32, s32, s32, u8);
void ui_getPartyPortraitRects(s32, RECT *, RECT *, RECT *);
void ui_loadPartyPortrait(u8 *, s32);
s32 text_renderNameGlyph(s32, s32, s32);
u8 *ui_buildLabel(s32, s32, u32 *, s32, s32, u8 *);
u8 *text_buildSmall(s32, s32, u32 *, u8 *, s32, u8 *);
u8 *text_buildDigits(s32, s32, u32 *, u8 *, s32, void *);
void ui_drawDigits(s32, s32, s32, u8 *, s32);
u8 *text_buildBlock(s32, s32, u32 *, u8 *, s32, u8 *);
typedef struct {
    s16 w;
    s16 h;
} TextSize;
#if defined(OVL_0B_H) /* ovl_0b passes its own buffer types */
void text_measure(void *, s32);
#else
TextSize text_measure(u8 *); /* measure a text string */
#endif
s32 config_remapButton(); /* (s32 index); unprototyped, as the original's callers pass ints unconverted */
void win_drawAll(void);
void win_updateAll(void);

/* Text encoding (font codes, 0xFF-terminated). Control codes as text_buildBlock/text_measure handle them;
 * 0xF9 is followed by one escape byte (TEXT_X_ADD/TEXT_X_SET/TEXT_Y_* and the argument codes take one more). */
#define TEXT_END 0xFF
#define TEXT_NEWLINE 0xF7 /* x back to the start, y += 15 */
#define TEXT_ESC 0xF9
/* escape bytes after TEXT_ESC */
#define TEXT_CHAR_NAME 0x10 /* + character 0-7 */
#define TEXT_PARTY_NAME 0x18 /* + party slot 0-3 */
#define TEXT_COLOR 0x20 /* + palette 0-9 (g_textColor) */
#define TEXT_MSG_NUMBER 0x40 /* + index: number from the message system (g_msgNumbers) */
#define TEXT_X_ADD 0x43 /* + s8 */
#define TEXT_X_SET 0x44 /* + x from the line start */
#define TEXT_Y_DOWN 0x49 /* + pixels */
#define TEXT_Y_UP 0x4A /* + pixels */
#define TEXT_SPRITE 0x4B /* + sprite id */
#define TEXT_ITEM_ICON 0x4C /* + item id */
#define TEXT_ARG_SPRITE 0x4D /* + arg index (g_textIconArgs) */
#define TEXT_ARG_ITEM_ICON 0x4E /* + arg index (g_textIconArgs) */
#define TEXT_ARG_NUMBER 0x68 /* + arg index (g_textNumberArgs), font */
#define TEXT_ARG_STRING 0x69 /* + arg index (g_textStringArgs), font */
#define TEXT_ARG_NUMBER_SMALL 0x6A
#define TEXT_ARG_STRING_SMALL 0x6B
#define TEXT_ARG_NUMBER_DIGITS 0x6C
#define TEXT_ARG_STRING_DIGITS 0x6D
#define TEXT_ARG_NUMBER_LABEL 0x6E
#define TEXT_ARG_STRING_LABEL 0x6F

/* ---- 0x54-byte objects, 10 at g_windows (win_reapClosed..8002E3F0) ---- */

typedef struct {
    s16 count; /* items */
    u8 rows; /* count / cols, rounded up */
    u8 visibleRows;
    u8 cols;
    u8 top; /* first visible row */
    u8 cursor; /* row on screen; runtime: battle spell list 0..3 */
    u8 col;
    u8 scroll; /* scroll animation flags: 1 down, 2 up, 4 page, 8 wrap column (menuList_update) */
    u8 scrollPos; /* pixels scrolled / page divider position */
    u8 unkA;
    u8 fillV; /* fill texture v offset, moves with the scroll */
    u8 pageTop; /* top row after a page scroll */
    u8 height; /* pixels */
    u8 pageStep;
    u8 padF;
} MenuList;
typedef MenuList Unk16; /* older name */

typedef struct {
    u8 unk0[4];
} Unk4; /* 4 bytes, byte-aligned (copied with lwl/lwr) */

/* Menu window object (win_openFree returns one of these). */
typedef struct Window {
    s16 unk0; /* x */
    s16 unk2; /* y */
    s16 unk4; /* w */
    s16 unk6; /* h */
    RECT unk8; /* inner/content rect */
    s16 unk10;
    s16 unk12;
    s16 unk14; /* open/close step x */
    s16 unk16; /* open/close step y */
    s16 unk18; /* OT layer */
    u8 unk1A;
    u8 unk1B; /* index */
    u8 unk1C; /* state: 2 = fire callback */
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    s16 unk20;
    s8 unk22;
    u8 unk23;
    s32 unk24;
    u8 unk28;
    u8 unk29;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    MenuList unk30;
    void (*unk40)(struct Window *);
    void (*unk44)(struct Window *);
    void (*unk48)(struct Window *);
    void (*unk4C)(struct Window *);
    s32 unk50; /* user data */
} Window; /* 0x54 */
extern Window g_windows[10];

void win_setOpenAnim(Window *, s32);
void win_setCloseAnim(Window *, s32);
void win_setTail(Window *, s32, s32);

/* ---- misc globals (batch 1) ---- */

extern u8 D_80068268[];
extern s8 g_mbgState;
extern s32 g_menuOnClose;
extern u8 g_menuFadeInOnClose;
extern u8 g_menuFadeOutOnClose;
extern u8 D_80077DB2;
extern s32 D_800704C8;
extern s32 D_8007041C;
extern u8 g_partyChanged;
extern struct MsgWindow *g_msgFocus;
extern s32 D_800834C8;
extern s32 D_8006798C;
extern u16 g_padFlags;
extern u8 D_8006825C;
extern u8 D_8006826C;

typedef struct {
    u8 pad0[0x24];
    u8 unk24;
    u8 pad25[0x2C - 0x25];
    s32 unk2C;
    u8 pad30[6];
    u8 unk36;
    u8 unk37;
    u8 unk38;
} Unk8001A084; /* pad state? (near libpad) */

typedef struct {
    s32 unk0;
    u8 pad4[0x10];
    s32 unk14;
} Unk8005F038;

/* Sound sequencer channel (AKAO): unk0 is the byte-code read pointer. */
typedef struct {
    u8 *unk0;
    u8 *unk4[4]; /* loop start addresses */
    u8 *unk14; /* return address? */
    u8 *unk18;
    s32 (*unk1C)(u32); /* LFO wave functions (pitch, volume, pan) */
    s32 (*unk20)(u32);
    s32 (*unk24)(u32);
    u32 unk28;
    u32 unk2C;
    s32 unk30;
    u32 unk34; /* flags */
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    u32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
    u32 unk84;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    u16 unk94;
    u16 unk96;
    u16 unk98;
    u16 unk9A;
    u16 unk9C;
    u16 unk9E;
    u16 unkA0;
    u16 unkA2[4]; /* loop counters */
    u16 unkAA[4];
    u16 unkB2;
    u16 unkB4;
    s16 unkB6;
    s16 unkB8;
    u16 unkBA;
    s16 unkBC;
    u16 unkBE;
    u16 unkC0;
    s16 unkC2;
    u16 unkC4;
    u16 unkC6;
    u16 unkC8;
    u16 unkCA;
    s16 unkCC;
    u16 unkCE;
    u16 unkD0;
    s16 unkD2;
    u16 unkD4;
    u16 unkD6;
    s16 unkD8;
    s16 unkDA;
    u16 unkDC;
    u16 unkDE;
    s16 unkE0;
    u16 unkE2;
    s16 unkE4;
    s16 unkE6;
    u16 unkE8;
    u16 unkEA;
    s16 unkEC;
    s16 unkEE;
    u16 unkF0;
    u16 unkF2;
    u16 unkF4; /* loop depth */
    u16 unkF6;
    u16 unkF8;
    s16 unkFA;
    s16 unkFC;
    s16 unkFE;
    u16 unk100;
    s16 unk102;
    s16 unk104;
    s16 unk106;
    s16 unk108;
    s16 unk10A;
    s16 unk10C;
    s16 unk10E;
    s16 unk110;
    s16 unk112;
    s16 unk114;
    u8 pad116[0x118 - 0x116];
    u32 voice; /* voice number */
    u32 voiceDirty;
    s32 voiceStartAddr;
    s32 voiceLoopAddr;
    u16 voicePitch; /* pitch */
    u16 voiceAdsr1;
    u16 voiceAdsr2;
    s16 voiceVolScale;
    s16 voiceVolL; /* volume left */
    s16 voiceVolR; /* volume right */
} AkaoChannel; /* 0x134 */
void func_80057D80(AkaoChannel *, u32); /* channel LFO / pitch update (func_800585E0 passes the channel bit) */

/* 8-byte key-split region of an AKAO instrument (AkaoChannel.unk18) */
typedef struct {
    u8 unk0; /* sample */
    u8 unk1; /* low key */
    u8 unk2; /* high key */
    u8 unk3;
    u8 unk4;
    u8 unk5; /* 0 = end */
    u8 unk6;
    u8 unk7;
} AkaoRegion;

/* voice setup parameters for func_80059E10 */
typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 pad9[0xC - 0x9];
    u16 unkC;
    u8 padE[0x10 - 0xE];
    s32 unk10;
} VoiceParams;

/* frame animation (sprAnim_update) */
typedef struct {
    u16 unk0; /* cell list index */
    u8 unk2; /* ticks */
    u8 pad3;
    s16 unk4; /* x offset */
    s16 unk6; /* y offset */
} Unk80035B90Frame;

typedef struct {
    s16 unk0; /* frame count */
    u8 pad2[2];
    Unk80035B90Frame unk4[1];
} Unk80035B90Data;

typedef struct Unk80035B90 {
    u8 unk0; /* active */
    u8 unk1; /* frame */
    u8 unk2; /* tick */
    u8 unk3; /* frame count */
    Unk80035B90Data *unk4;
    Unk80035B90Frame *unk8;
    Unk80035B90Frame *unkC; /* end of frames */
    Unk80035B90Data *unk10;
    void (*unk14)(); /* called with the object */
} Unk80035B90;


/* screen fade (g_fade, fade_*): runtime field/menu campaign, docs/runtime-findings.md */
typedef struct {
    u8 type; /* fade_start type, picks the g_fadeTypes handlers */
    u8 subtract; /* 1: subtractive (darkening) blend */
    u8 active;
    u8 covered; /* end colour non-zero: the screen stays covered after the fade (fade_setEndColor) */
    u8 eased; /* fadeFlat follows fade_ease's curve */
    u8 prevType;
    u8 pad6[0x8 - 6];
    s16 frames; /* length */
    s16 angle; /* z rotation of the wipe (fade_setAngle) */
    s32 frame; /* counts up to frames, then onDone runs */
    s32 onDone;
    u8 pad14[0x18 - 0x14];
    u8 *pkt; /* packet buffer cursor */
    s32 otz;
    Unk4 startColor;
    u8 endR;
    u8 endG;
    u8 endB;
    u8 pad27;
    Unk4 color; /* current colour */
} FadeState;
extern FadeState g_fade;
u32 *fade_getOt(s32);
#if !defined(OVL_0C_H) /* ovl_0c declares its own typed view */
void math_rotMatrixXYZ(void *, void *); /* rotation -> matrix */
#endif
s32 math_cos(s32); /* cos */
extern SVECTOR D_80077CB0;
extern SVECTOR D_80077CC0[2][4]; /* two screen-filling quads */

/* Select help window (g_menuHelp, help_* functions; layout merged from main + ovl_14 + ovl_32) */
typedef struct {
    s16 x; /* hand cursor position; the help box is placed beside it */
    s16 y;
    s32 otz; /* hand OT index */
    u8 *text; /* help text, set by the handler */
    void (*callback)(void); /* per-menu function run every frame while help is open: sets text */
    RECT bounds; /* help_setBounds: the window stays inside */
    u8 isOpen;
    u8 opening; /* open animation running (help_toggle .. help_onIconAnimDone) */
    u8 textShown;
    u8 locked; /* Select doesn't toggle help */
    u8 follow; /* 0: help box at x/y itself, else beside the hand */
    u8 cursorOnTop; /* hand drawn above the help box */
    u8 pad1E[2];
    Unk80035B90 iconAnim; /* moogle icon (sprAnim_*) */
} HelpState;
extern HelpState g_menuHelp;

/* sound sequencer state (AKAO), g_akaoSeq */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    union {
        s32 w;
        u16 h[2];
    } unk20; /* tempo, 16.16 */
    s32 unk24;
    s32 unk28; /* tempo accumulator */
    u8 *unk2C; /* sequence data */
    u16 *unk30;
    s32 unk34;
    s32 unk38;
    u32 unk3C;
    u32 unk40;
    u32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s16 unk58;
    u8 pad5A[0x5C - 0x5A];
    s32 unk5C;
    s32 unk60;
    s16 unk64;
    s16 unk66;
    u16 unk68;
    u16 unk6A;
    u16 unk6C;
    u16 unk6E;
    u16 unk70;
    u16 unk72;
    u16 unk74;
    u16 unk76;
    u16 unk78;
} Unk80080A10;
extern Unk80080A10 *g_akaoSeq;
extern Unk80080A10 g_akaoMainSeq; /* main sequencer state */
extern s32 g_akaoTickTimeSum;
extern u16 g_akaoTickCount; /* tick counter */
extern s32 g_akaoTickTimes[4]; /* recent tick timings */
void func_80058F94(void);
void func_800589F0(void);
void func_8005D628(Unk80080A10 *, AkaoChannel *);
extern AkaoChannel *D_800809EC;
extern s32 D_8007F8D8[2];
extern s32 D_8008314C;
extern s32 D_8007F8E0;
extern s32 D_80083238[3];
extern s32 D_80080A20;
extern s32 D_8007E3C8[2];
extern s32 D_8007E3D0;

typedef Character Unk800539FC;

typedef struct {
    u8 pad0[0x50];
    s32 unk50;
} Unk800589E0;

extern s32 D_80077DCC;
extern u8 D_80077E52;
extern struct Unk8004AB64 *D_8007AEB8; /* object at the world map's mark 0x40 */
extern struct Unk8004AB64 *D_8007AEBC; /* object at the world map's mark 0x80 */
extern s32 D_8007B950;
extern s32 D_80080A24;
extern s32 *D_80066764;

typedef struct {
    void (*unk0)(void);
    void (*unk4)(void);
    u8 pad8[0xC - 8];
    u8 unkC;
    u8 padD[3];
} Unk80077E44; /* 16-byte entries */
extern Unk80077E44 *D_80077E44;

/* actor position (VECTOR layout, the pad word holds two rotation angles) */
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    s16 unk20; /* rotation x */
    s16 rotY; /* facing; runtime: 0 toward the camera, 3072 walking right */
} ActorPos;

/* field / world object (actor) */
typedef struct Unk8004AB64 {
    u8 *unk0; /* script pc */
    u8 unk4;
    u8 unk5; /* kind: 4 = ... */
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 pad13;
    ActorPos pos; /* runtime: x rises walking right, z falls walking toward the camera */
    s16 unk24; /* rotation z */
    u8 unk26;
    u8 pad27;
    u16 unk28;
    u16 unk2A;
    u8 unk2C;
    u8 unk2D;
    s8 unk2E;
    u8 unk2F;
    s16 unk30;
    s16 unk32;
    u16 unk34;
    u16 unk36; /* animation file */
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
    u8 unk3C;
    u8 unk3D;
    u8 unk3E;
    u8 pad3F;
    ActorPos unk40; /* previous position; unk20/unk22: turn left/right anims */
    u16 unk50; /* flags */
    s16 unk52; /* target direction */
    s16 unk54;
    s16 unk56;
    s16 unk58;
    u16 unk5A; /* animation to restore after turning */
    u8 unk5C;
    u8 unk5D;
    u8 unk5E;
    u8 unk5F;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[0x68 - 0x65];
    u8 unk68;
    u8 unk69;
    u8 unk6A;
    u8 unk6B;
    u16 unk6C; /* rotation y speed */
    s16 unk6E; /* also used as a tiny script buffer */
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 unk73;
    u16 unk74; /* idle animation */
    u16 unk76;
    s32 unk78;
    union { u32 w; u8 b[4]; } unk7C; /* colour */
    u16 unk80;
    u16 unk82;
    s16 unk84; /* smoothed look-at point (evt_relaxLookAt) */
    s16 unk86;
    s16 unk88;
    s16 unk8A; /* speech bubble anchor offset (copied from D_8007AE5C..64) */
    s16 unk8C;
    s16 unk8E;
    u8 unk90;
    u8 unk91;
    u8 unk92;
    u8 unk93;
    s16 unk94;
    s16 unk96;
    struct Unk8004AB64 *unk98; /* object collided with (evt_movePlayer) */
    s32 unk9C;
    struct Unk8004AB64 *unkA0;
    u16 unkA4;
    u16 unkA6;
    u16 unkA8;
    s16 unkAA;
} Unk8004AB64;

/* model header as seen by evt_relaxLookAt (BtlUnit9C in ovl_0b) */
typedef struct {
    u16 unk0; /* flags */
    u8 pad2[0x48 - 0x2];
    s16 unk48; /* look-at point */
    s16 unk4A;
    s16 unk4C;
} Unk80047B90;
void evt_onAnimLimit(Unk8004AB64 *);
s32 func_80047AB8(Unk8004AB64 *, s32);
void evt_relaxLookAt(Unk8004AB64 *, Unk80047B90 *);
s32 func_80052DEC(s32, s32, s32); /* distance */
s32 func_80052EEC(s32);
s32 func_80052E48(s32, s32, s32);

typedef struct Unk8007AECCNode {
    Unk8004AB64 *unk0; /* key = start of the block in unk50 */
    u32 *unk4; /* end of the block */
    struct Unk8007AECCNode *unk8; /* next */
} Unk8007AECCNode;

typedef struct Unk16D8 {
    struct Unk16D8 *unk0; /* next */
    Unk8004AB64 *unk4;
} Unk16D8;

typedef struct {
    u8 unk0[0x50]; /* cleared by evt_init */
    u32 unk50[0x500]; /* object pool (evt_allocObject) */
    Unk8007AECCNode unk1450[32];
    Unk8007AECCNode *unk15D0; /* active list */
    Unk8007AECCNode *unk15D4; /* free list */
    Unk16D8 unk15D8[32];
    Unk16D8 *unk16D8;
    Unk16D8 *unk16DC; /* tail */
    Unk16D8 *unk16E0; /* free list */
    u8 unk16E4[4]; /* party member object ids */
    s16 unk16E8; /* analog / d-pad camera direction offsets */
    s16 unk16EA;
    u8 controlEnabled; /* runtime: 1 while the player walks, 0 during a dialog (field/menu campaign) */
    u8 playerUid; /* runtime: uid of the object the pad moves (Zidane, 19, in the Dali inn) */
    u8 unk16EE; /* field mode the state was saved in */
    u8 unk16EF;
    u8 *unk16F0; /* script data base */
    u16 unk16F4; /* field / world id the state was saved for */
    u8 pad16F6[0x16F8 - 0x16F6];
    u8 unk16F8;
    u8 pad16F9[0x16FC - 0x16F9];
    s32 unk16FC;
    s16 unk1700;
    u8 pad1702[0x17C8 - 0x1702];
} Unk8007AECC; /* 0x17C8 */
extern Unk8007AECC *g_evtState;
extern Unk8007AECC D_80077E60; /* the script system state g_evtState points at */
extern Unk8007AECC D_80079630; /* saved copy, restored when returning to the same field */
extern Unk4 D_80011A44; /* rodata */
extern s32 D_8007B6FC;
extern s32 D_8007AEC0;
extern s32 g_evtEncounterPending;
void msg_resetPlayerBubble(void);

/* Inlined copy of evt_getObject (find object by id). */
static inline Unk8004AB64 *evt_getObject_inline(s32 id) {
    Unk16D8 *n;
    for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
        if (n->unk4->unk7 == id) {
            break;
        }
    }
    return n != NULL ? n->unk4 : NULL;
}


typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Unk8001E3A4;

typedef struct {
    u8 pad0[5];
    u8 unk5;
} Unk800659D0Sub;

typedef struct {
    u8 pad0[8];
    Unk800659D0Sub *unk8;
    u8 padC[0x12 - 0xC];
    s16 unk12;
    s16 unk14;
    s16 unk16;
} Unk800659D0;

typedef struct {
    s32 unk0;
    s32 unk4;
} Unk8007B988;
extern Unk8007B988 D_8007B988;
extern s32 D_8007B994;
extern s32 D_8007B998;

extern s32 g_textBank;
extern s32 g_evtScriptCount;
extern s32 D_8007AE00; /* party mask, set by evt_loadScripts */
extern u32 g_padPortHeld[];
extern u32 D_8007B97C;
extern u32 D_8007B980;
extern s32 D_8007BBEC;
extern s32 D_8007BBF4;
extern u8 g_menuReading;
extern u8 D_80077E2C;
extern s16 D_8007AE10[8];
extern s32 D_8007B978;
extern s8 D_8006F440[];
extern s16 D_8007F96E;
extern s32 D_80083154;
extern s16 D_8007F96C;
extern s32 D_80083150;


typedef struct {
    u16 unk0;
    u8 pad2[0x20 - 2];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
} Unk800219D8;

extern Unk4 g_padAnalogThresholds;

extern u8 D_800739B8[];
extern Unk80080A10 *g_akaoSubSeq;
extern Unk80080A10 g_akaoSavedSeq;
/* AKAO sequence file header (func_8005A320) */
typedef struct {
    u8 pad0[4];
    u16 unk4; /* sequence id */
    u8 pad6[2];
    u16 unk8;
    u8 padA[0x14 - 0xA];
    s32 unk14;
    u8 pad18[0x20 - 0x18];
    u32 unk20; /* channel mask */
    s32 unk24;
    s32 unk28;
    u8 pad2C[0x30 - 0x2C];
    s32 unk30; /* offset from &unk30 */
    s32 unk34; /* offset from &unk34 */
    u8 pad38[0x40 - 0x38];
    u16 unk40[1]; /* per-channel offsets from the entry itself */
} AkaoSeqHeader;
void func_8005A320(s32, s32);
void func_8005963C(s32, s32);
extern s16 D_8007F95A;
extern s32 D_8007F8DC;
extern s32 D_8008322C;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Unk80083158;
extern Unk80083158 g_akaoFlags;
/* menu state */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3; /* selected party slot */
    s32 unk4; /* frame counter */
    s32 unk8;
} Unk80077E50;
extern Unk80077E50 g_menuScreen;
extern u8 D_80077E53;
extern s32 g_evtLastEncounter;
extern u8 D_8007954E;
extern u8 D_8007AD1E;
extern s32 g_msgNumbers[8];
extern u8 *D_8007BC0C; /* help-arrow animation script */
extern s32 D_8007BC08;
extern s32 D_8007BBF0;
extern volatile s32 g_akaoSpuBusy;

void db_clear(void);
s32 game_unloadCommonDb(void);
s32 strncmp(char *, char *, s32);
void game_setNextScene(u16);
void config_reset(void);
void win_runFrame(void);
void timer_resetAll(void);
void timer_update(void);
void fade_in(s32);
void fade_out(s32);
void fade_inAdditive(s32);
void fade_outAdditive(s32);
void fadeWipe_getColors(u8 *);
s32 fade_reverse(void (*)(void));
MATRIX *math_buildRotMatrixXYZ(SVECTOR *, MATRIX *, s32);
MATRIX *math_buildRotMatrixZXY(SVECTOR *, MATRIX *, s32);
MATRIX *math_buildRotMatrixZYX(SVECTOR *, MATRIX *, s32);
MATRIX *math_buildRotMatrixYXZ(SVECTOR *, MATRIX *, s32);

/* sine table, 0x400 entries for a quarter wave (4096 = 360 degrees) */
extern s16 g_sinTable[];

/* out = sin(a) from g_sinTable. a is the angle (bits above 0xFFF ignored), mk the same angle & 0xFFF. */
#define ISIN(out, a, mk)                              \
    if ((a) & 0x400) {                                \
        out = g_sinTable[0x400 - ((a) & 0x3FF)];      \
    } else {                                          \
        out = g_sinTable[(a) & 0x3FF];                \
    }                                                 \
    if ((mk) & 0x800) {                               \
        out = -out;                                   \
    }
void fade_init(void);
void fade_update(void);
void fade_start(s32, s32, s32, s32);
void menu_dispatch(void);
void menu_doOpen(s32, s32);
void akao_init(void);
void akao_quit(void);
s32 akao_command(u32);
void akao_loadBankRetry(s32, s32);
void akao_opEnd(AkaoChannel *, s32);
void func_80060938(void);
void help_setCallback(void (*)(void));

typedef struct {
    u8 pad0[4];
    u16 unk4; /* row count */
    u16 unk6; /* offset of a u16[][6] flag table */
} Unk8007BD44;

typedef struct {
    u16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    Unk8007BD44 *unkC;
} Unk8007BD38;
extern Unk8007BD38 g_vib;
u8 func_800EA27C(void); /* ovl_0b */

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
} Unk80021910;

extern u8 D_8006CAA4[][2];
extern u8 D_800110A0[];
extern u8 D_8006FD60[];
extern s32 D_8007BC10;
extern s32 D_8007BC14;
/* event / script variables; h[0] is the story progress counter */
typedef union {
    s16 h[0x400];
    u8 b[0x800];
} EventVars;
extern EventVars g_evtGlobalVars;

void config_applySound(void);
void config_applyPadMapping(void);
void menu_reset(void);
void func_8003264C(void);
s32 text_getFrom(u8 *, s32);
s32 text_measureWidth(s32);
void akao_setOutputMode(s32);
void chr_clearAp(s32);
void chr_clearSupport(s32);
void bzero(void *, s32);

/* argument block for sound command akao_command */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} Unk80080AA0;
void func_8005A014(); /* (VoiceParams *, u8 *, u8 *, s32); callers pass Unk80080AA0 * */
extern Unk80080AA0 g_akaoArgs;
extern Unk80080AA0 g_akaoCommandBlock; /* command block passed to the handlers */
extern void (*g_akaoCommandHandlers[])(Unk80080AA0 *); /* sound command handlers */

typedef struct {
    u8 unk0; /* card id, 0xFF = none */
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
} Unk6; /* card inventory entry */

/* Tetra Master card inventory (runtime field/menu campaign) */
typedef struct {
    s16 wins;
    s16 losses;
    s16 draws;
    Unk6 cards[105]; /* id 0xFF = empty */
} CardInventory;
extern CardInventory g_cards;

/* message window (g_msgWindows[10]; runtime field/menu campaign) */
typedef struct MsgWindow {
    struct MsgWindow *parent; /* next in the focus chain (msg_update) */
    s16 x;
    s16 y;
    s16 open;
    s16 unkA;
    s32 (*onInput)(struct MsgWindow *, s32); /* pad handler, msg_update */
    s32 unk10;
    Window *window;
    s16 w;
    s16 unk1A;
    s16 unk1C;
    s16 msgId; /* message index */
    s16 textPos; /* offset of the next character */
    s16 pageStart;
    s16 unk24;
    u8 unk26;
    u8 h;
    u8 flags; /* 2 modal (msg_allocWindow), 4, 8 */
    u8 speed; /* text speed */
    u8 unk2A;
    u8 unk2B;
} MsgWindow;
extern MsgWindow g_msgWindows[];

extern u8 D_80077E2F;
extern s16 D_80077E30; /* = g_menuScreenSwitch.unk8 */

void menuScreen_loadOverlay(void);
void akao_stopSeChannels(s32, s32);
s32 preload_poll(void);
void preload_clearBusy(void);
void help_init(void);
void config_init(void);
void game_initFade(void);
void func_80061070(s32, s32, s32, void (*)(void));
void func_800611BC(void);
void func_800611EC(void);
void func_8006121C(void);
void func_8006124C(void);
void akao_onSpuTransferDone(void);

typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    void *unk4[16];
} Unk80073C40;
extern Unk80073C40 g_dbList;

typedef struct {
    u32 unk0;
    u8 pad4[0x24 - 4];
} Unk80073940; /* 0x24-byte entries */
extern Unk80073940 g_padBuf[];

typedef Unk8004AB64 Unk800367D0; /* evt_createObject creates a script object */

typedef struct {
    u8 pad0[0x12];
    s16 unk12;
} Unk8006573C;

extern s16 D_80067950;
extern u32 D_80077DB4;
extern u8 D_8006CAB8[][2];

u16 *db_findModelInfo(s32);
void item_initNew(void);
void func_8003431C(void);
void party_initNewGame(void);
s32 snd_quit(); /* unprototyped: main calls it with no args, ovl_65 passes a pointer */
void db_quit(void);
void *db_lookupRegistered(s32, s32, s32); /* mode 4: address, 5: size */
void win_drawBoxEx(RECT *, s32, s32, s32);
void menu_start(void);
Unk800367D0 *evt_createObject(s32, s32, s32, s32);
s32 CdSyncCallback(void (*)(u8, u8 *));
void CdFlush(void);

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8; /* fine tune */
    s16 unkA; /* base note */
    u16 unkC;
    u16 unkE;
} Unk8005EBB4;
extern u32 g_akaoSemitonePitch[12]; /* semitone pitch table */
extern Unk8005EBB4 g_akaoInstruments[];
u16 akao_calcPitch(Unk8005EBB4 *p, s32 note, u8 fine, s32 *out);
extern u8 D_8006F374[]; /* operand length of opcodes 0xA0-0xFF, 0 = special */
extern u8 D_8006F3D4[]; /* operand length of 0xFE xx opcodes */
s32 func_8005DB40(AkaoChannel *c);
extern void (*g_akaoOps[])(AkaoChannel *, u32); /* opcode 0xA0-0xFF handlers */
extern void (*g_akaoOpsFE[])(AkaoChannel *, u32); /* 0xFE xx opcode handlers */
extern u16 D_8006F3F4[]; /* note lengths */

/* drum kit key entry (func_8005E170) */
typedef struct {
    u8 unk0; /* sample */
    u8 unk1; /* note */
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7; /* pan, bit 7: flag */
} AkaoDrumKey;
void akao_setChannelInstrument(AkaoChannel *, Unk8005EBB4 *, s32);

typedef struct {
    void (*unk0)(void);
    s32 unk4;
    void (*unk8)(void);
    s32 unkC;
} Unk80077E28Entry;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7; /* = D_80077E2F */
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s32 unk10; /* loaded file id */
    RECT unk14;
    Unk80077E28Entry *unk1C;
    void (*unk20)(void);
} Unk80077E28;
extern Unk80077E28 g_menuScreenSwitch;

extern s16 g_akaoCdVolumeFadeTicks;
extern s32 g_akaoCdVolume;
extern s32 D_80077E58;
extern u32 *g_penOt;

void akao_applyCdVolume(void);
void menu_close(void);
void game_initTimer(void);
s32 fade_setOtz(s32);

/* one auto-repeat channel: pad_updateRepeat ORs its buttons into g_padRepeat after delay, then every interval frames */
typedef struct {
    u8 enabled;
    u8 repeating; /* past the first delay */
    u8 armed; /* buttons were released since the reset */
    u8 pad3;
    s32 prevHeld;
    s32 lastVsync; /* VSync(-1) of the last repeat */
    s32 mask; /* buttons this channel repeats */
    u16 delay;
    u16 interval;
} PadRepeat; /* 20 bytes */
extern PadRepeat g_padRepeats[][23];


typedef struct {
    u8 pad0[0x12];
    u16 unk12;
    u8 pad14[2];
    u16 unk16;
    s16 unk18;
    s16 unk1A;
} Unk80065ADC;

void msg_beginDraw(void);
void pen_setColor(s32);
void preload_setEntry(PreloadEntry *, s32, s32, s32, s32, s32, s32, s32);
PreloadEntry *preload_find(s32);
s32 preload_wait(PreloadEntry *);
void gfx_init(u32 *, u32 *, s32, s32);

typedef struct {
    u8 pad0[0x8C];
    s32 unk8C;
} Unk80065A44Sub;

typedef struct {
    Unk80065A44Sub *unk0;
    u8 pad4[0x16 - 4];
    s16 unk16;
} Unk80065A44;

typedef struct {
    u8 pad0[0x34];
    u16 unk34;
} Unk80021698;

extern s32 D_8007B960;
extern s32 g_msgChoice;
extern s32 g_msgChoiceCount;
extern s32 g_msgChoiceCancel;
extern s32 D_8007B974;
extern s32 D_8007BB94;

s32 SpuRead(s32, s32);
void akao_beginSpuTransfer(void);
void akao_opAE(AkaoChannel *, s32);
void akao_opAF(AkaoChannel *, s32);
u8 cd_syncAsync(void (*)(u8 *));



extern u8 D_8006CAD8[][5];
extern s32 D_800809B8;
extern u8 D_80083156; /* tempo scale */
void akao_execChannel(AkaoChannel *, u32);
void func_800579B4(AkaoChannel *, u32, s32);

void akao_cmdPlayMusic(Unk80080AA0 *);

/* ---- hoisted from overlay headers ---- */

/* file table entry (file_find / dep_find) */
typedef struct {
    s32 unk0;
    s32 unk4; /* start sector */
    s32 unk8;
    s32 unkC; /* end sector / next id */
} FileEntry;

/* g_gameState->dirs[i]: per-directory header (file_find) */
typedef struct {
    s32 unk0; /* 3 = flat u16 table at unk18[13] */
    s32 unk4; /* entry count */
    s32 unk8;
    s32 unkC;
} FileDirEntry;

/* g_gameState->dirTables[i]: 8-byte {id, sector} entries searched by file_find */
typedef struct {
    u16 unk0; /* id */
    u16 unk2;
    s32 unk4;
} FileIdEntry;

/* file id table (GameData.depTable) */
typedef struct FileTable {
    u16 unk0;
    u16 unk2; /* count */
    FileEntry unk4[1];
} FileTable;

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy;
    u32 uvclut;
    u32 wh;
} SprtWords; /* SPRT written as whole words */

/* menu list/cursor state passed to menuList_input etc. */


/* visible window of a MenuList (menuList_getPageView) */
typedef struct {
    u8 unk0;  /* visible rows */
    u8 unk1;  /* first index */
    s16 unk2; /* y */
    u8 pad4[4];
} MenuView;

typedef struct {
    s32 unk0[1]; /* indexed past its end: text table */
    s32 unk4;
    u8 pad8[0x30 - 8];
    s32 unk30;
    u8 pad34[0x3C - 0x34];
    s32 unk3C;
    u8 pad40[0x68 - 0x40];
    s32 unk68;
    u8 pad6C[0x7C - 0x6C];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    s32 unk94;
    u8 pad98[0xA0 - 0x98];
    s32 unkA0;
    s32 unkA4;
    u8 padA8[0xC0 - 0xA8];
    s32 unkC0;
} Unk800703FC; /* text table? */
extern Unk800703FC g_winLabelStrings;

extern u8 *g_loadAreaEnd; /* end of load area */
extern s32 D_80067948;
extern u32 g_levelExp[]; /* exp needed for next level, by level */
extern s32 D_80070424;
extern s32 D_80070428;
extern s32 D_80070450;
extern s32 D_80070454;
extern s32 D_80070460;
extern s32 D_800704AC;
extern s32 g_padPortMask[];
/* Pad bits in g_padHeld/g_padPressed/g_padRepeat and g_padPort*: the low 16 are the controller's own
 * button bits, the high ones the game's buttons, named after their default mapping (checked at runtime by
 * holding each button, docs/runtime-findings.md). Triangle also sets 0x1000000, Select 0x2000000 | 0x8000000. */
#define PAD_SELECT   0x0001
#define PAD_START    0x0008
#define PAD_UP       0x0010
#define PAD_RIGHT    0x0020
#define PAD_DOWN     0x0040
#define PAD_LEFT     0x0080
#define PAD_L2       0x0100
#define PAD_R2       0x0200
#define PAD_L1       0x0400
#define PAD_R1       0x0800
#define PAD_TRIANGLE 0x1000
#define PAD_CIRCLE   0x2000
#define PAD_CROSS    0x4000
#define PAD_SQUARE   0x8000
#define BTN_CIRCLE   0x10000
#define BTN_CROSS    0x20000
#define BTN_TRIANGLE 0x40000
#define BTN_SQUARE   0x80000
#define BTN_L1       0x100000
#define BTN_R1       0x200000
#define BTN_L2       0x400000
#define BTN_R2       0x800000
#define BTN_START    0x4000000  /* raw Start (pad_setMapping in config_applyPadMapping) */
#define BTN_SELECT   0x8000000  /* raw Select; toggles the help window (help_update) */
#define PAD_SOFT_RESET (PAD_SELECT | PAD_START | PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1) /* 0xF09: fld/wm_endFrame set GS_RESET when all are held */
#define PAD_ID_DIGITAL 4 /* controller-type nibble ((PadBuf.unk0 >> 12) & 0xF) pad_update accepts */
#define PAD_ID_ANALOG 7 /* pad_update reads the stick bytes only for this type */

/* GameState.unk0 flags. Each mode loop runs until its exit bits are set (battle: 0x100/0x200 in ovl_0b). */
#define GS_RESET 0x1 /* game_main's inner loop ends: soft reset (PAD_SOFT_RESET) or restart */
#define GS_FLD_LEAVE 0x2 /* fld_entry loop ends */
#define GS_FLD_NEW_AREA 0x4 /* fld_runArea loop ends: next field in another area */
#define GS_FLD_NEW_FIELD 0x8 /* fld_main loop ends: next field in the same area */
#define GS_WM_LEAVE 0x1000 /* wm_entry loop ends */
#define GS_WM_NEW_MAP 0x2000 /* wm_main loop ends without leaving wm_entry */
#define GS_RELOAD_FILE_TABLES 0x100000 /* set at boot (game_resetState) and after a disc change; file_loadDirTables reloads, then clears it */

/* GameData.unk0 flags */
#define GD_MENU_DISABLED 0x1 /* menu_doOpen refuses to open */
#define GD_MENU_OPEN 0x2 /* set by menu_doOpen, cleared by menu_finish; the field/world event update is skipped */
#define GD_HIDE_MODELS 0x4
#define GD_HIDE_BG 0x8
#define GD_HIDE_SPRITES 0x10
#define GD_HIDE_ALL (GD_HIDE_MODELS | GD_HIDE_BG | GD_HIDE_SPRITES) /* 0x1C: menu_onFadedOut sets it, menu_finish clears it */
#define GD_NO_FLIP 0x20 /* fld/wm_endFrame keep the draw buffer (pause screens) */
#define GD_PAUSED 0x100
#define GD_MUSIC_STOP_PENDING 0x200000
#define GD_SFX0_LOADED 0x01000000 /* resident SE slot 0 copied (fld_soundCommand with SND_RESIDENT) */
#define GD_SFX0_PLAYING 0x02000000
#define GD_SFX1_LOADED 0x04000000
#define GD_SFX1_PLAYING 0x08000000
#define GD_MUSIC_LOADED 0x10000000
#define GD_MUSIC_PLAYING 0x20000000
#define GD_MUSIC2_LOADED 0x40000000
#define GD_MUSIC2_PLAYING 0x80000000

/* menu sound effects (snd_playSe ids) */
#define SE_CANCEL 0x65 /* Circle backs out; help closes */
#define SE_BUZZER 0x66 /* refused action */
#define SE_CURSOR 0x67 /* cursor move / pick (menuList_*) */
#define SE_PAGE 0x417 /* menuList page / L1-R1 switch */

/* inventory and character limits */
#define ITEM_NONE 0xFF /* empty item id / equipment slot */
#define ITEM_SLOT_COUNT 0x100 /* GameData.items slots */
#define KEY_ITEM_COUNT 0x50 /* key item ids (keyItem_has) */
#define LEVEL_MAX 99
#define EXP_MAX 9999999
#define GIL_MAX 9999999
#define ABILITY_MAX 0x30 /* entries in a character's g_abilityRows ability row (Character.ap) */
#define ABILITY_SUPPORT_FIRST 0xC0 /* ability ids from here are support abilities (g_supportAbilities) */
#define SUPPORT_COUNT 0x40

/* main-menu screens (g_menuScreen.screen, menuScreen_open/switch; runtime field/menu campaign) */
#define MENU_SCREEN_MAIN 0
#define MENU_SCREEN_ITEM 1
#define MENU_SCREEN_ABILITY 2
#define MENU_SCREEN_EQUIP 3
#define MENU_SCREEN_STATUS 4
#define MENU_SCREEN_ORDER 5
#define MENU_SCREEN_CARD 6
#define MENU_SCREEN_CONFIG 7

/* w_frameGetParameter ids: event-script expression variables >= 0xC0 on the world map
 * (main evt_getSysVar's default branch) */
#define WM_PARAM_AREA 0xC0 /* area of the cell under the player ("Now Area" in wm_getEncounterEntry) */
#define WM_PARAM_TOPOGRAPHY 0xC1 /* topography of that cell ("Topograph") */
#define WM_PARAM_REQ0 0xC3 /* g_worldReqParams[0..2], e.g. the WM_REQ_GET_GETOFF_POS result */
#define WM_PARAM_REQ1 0xC4
#define WM_PARAM_REQ2 0xC5
#define WM_PARAM_CAMERA_YAW 0xC7 /* g_worldCamera->unkC >> 4 */
#define WM_PARAM_CD_BUSY 0xC8 /* "Event:CDAccessWait": g_worldCdUser | music request pending */
#define WM_PARAM_PLAYER_X 0xC9
#define WM_PARAM_PLAYER_Y 0xCA
#define WM_PARAM_PLAYER_Z 0xCB
#define WM_PARAM_WEATHER_POINT_DIST 0xCC /* g_worldWeatherPointDist */
#define WM_PARAM_EVENT_BATTLE 0xCD /* 1 in g_worldEventBattleRate + 1 while walking on topography 0x24-0x26 */
#define WM_PARAM_PLAYER_DIR 0xCE /* player rotY >> 4 */
#define WM_PARAM_ZONE 0xCF /* wm_getAreaZone(area) */
#define WM_PARAM_RUNNING 0xD1 /* g_worldState == WM_STATE_RUNNING */
#define WM_PARAM_TITLE_PHASE 0xD2 /* g_worldTitlePhase */
#define WM_PARAM_NAVI_VISIBLE 0xD3 /* g_worldNaviVisible */

/* db keys: kind << 16 | id (db_lookup, docs/encounters.md) */
#define DB_MODEL 0x20000 /* evt_createModel: an object's model id */
#define DB_ANIM 0x30000 /* model animation (evt_getAnimFrameCount) */
#define DB_TIM 0x40000 /* TIM images (file_loadTim, game_loadCommonImages) */
#define DB_FIELD 0x50000 /* field / world file and its event script */
#define DB_AREA 0x60000 /* area bundle */
#define DB_MUSIC 0x70000 /* music sequences */
#define DB_SE 0x80000 /* sound effects */
#define DB_SOUND_BANK 0x90000 /* AKAO banks (game_loadCommonAkao, game_loadCommonDb's db) */
#define DB_BG 0xA0000 /* field background */
#define DB_WALKMESH 0xB0000 /* field walkmesh (bgi) */
#define DB_FONT 0xD0000 /* the font (GameData.unk7D4, game_loadCommonImages) */
#define DB_BATTLE 0x110000 /* encounter bundle */
#define DB_MODEL_INFO 0x120000 /* per-model tables */
#define DB_DEP_TABLE 0x140000 /* dependency table (GameData.depTable) */
#define DB_SPRITE 0x160000
#define DB_IMAGE_PACK 0x170000
#define DB_TEX_ANIM 0x190000 /* model texture animations */
#define DB_PACK 0x1B0000 /* nested container inside field/world files */
#define DB_LIGHTS 0x1F0000

/* Sound command words for the mode's sound handler (GameData.unk83C: fld_soundCommand / wm_soundCommand).
 * Bits 12-13 type (0 music, 1 SE, 3 bank), 8-11 kind, 0-5 sub-command, 7 resident SE slot, 6 slot index.
 * The SE/state/volume words carry the world map's debug-print names (ovl_0c WM_SND_*). */
#define SND_MUSIC_PLAY 0x0000
#define SND_MUSIC_STOP 0x0100
#define SND_MUSIC_LOAD 0x0700 /* load only */
#define SND_MUSIC_SUSPEND 0x0800 /* save song/volume, then stop */
#define SND_MUSIC_RESUME 0x0900 /* replay the saved song */
#define SND_GET_STATE 0x0C00 /* "Waiting [ State : %d ]": non-zero while loading */
#define SND_SE_PLAY 0xD000 /* "SE Play" (snd_playSe) */
#define SND_SE_STOP 0x5100 /* "SE Stop" */
#define SND_SE_VOL 0x9201 /* "SE Vol" */
#define SND_SE_PITCH 0x9203
#define SND_SE_VOL_INTPL 0xD301 /* "SE Vol Intpl" */
#define SND_SONG_VOL_INTPL 0x8301
#define SND_MUSIC_VOL_INTPL 0x8601 /* "Music Vol Intpl", "Music Fade Force" */
#define SND_SE_SUSPEND 0x1800
#define SND_SE_RESUME 0x1900
#define SND_BANK_LOAD 0x7700
#define SND_RESIDENT 0x80
extern u32 g_padRepeat; /* press, then auto-repeat after 15 frames, every 5 */
extern u8 D_80077DB5;
extern s32 D_80077DC0; /* frame counter? */

s32 func_80012AD4(void);
s32 EnterCriticalSection(void);
void ExitCriticalSection(void);
s32 OpenEvent(s32, s32, s32, void (*)(void)); /* BIOS OpenEvent */
s32 EnableEvent(s32); /* BIOS EnableEvent */
s32 db_register(s32, s32);
s32 db_lookup(s32, s32, s32, s32);
s32 db_getSize(s32); /* file size */
void game_setClearColor(s32, s32, s32);
void game_freezeScreen(void);
void file_loadTim(s32);
void file_setTimPos(s32, s32, s32, s32, s32);
s32 file_copyViaVram(s32, s32, s32, RECT *); /* copy through VRAM, returns bytes done */
extern u8 D_80068234;      /* next slot in D_80073CE8 */
extern RECT D_80073CE8[6]; /* transfer rects (file_copyViaVram) */
void file_initImg(void);
FileEntry *file_find(s32, s32);
void evt_setNextLocation(s32);
void timer_clear(void);
void pad_resetRepeat(s32, s32);
void pad_clearRepeat(s32, s32);
void pad_setRepeatEnabled(s32, s32, s32);
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Rgb32;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    Rgb32 unk24; /* current */
    Rgb32 unk30; /* output */
    s32 unk3C;
    s32 unk40;
    s32 unk44;
} Unk80077D68; /* screen fade state */
extern Unk80077D68 g_fadeFlat;
s32 fade_divRoundOut(s32, s32);
void chr_init(s32);
void pad_setRepeatMask(s32, s32, s32);
typedef struct {
    s32 mask;
    u8 enabled;
    u8 delay;
    u8 interval;
    u8 pad7;
} PadRepeatDef;
extern PadRepeatDef g_padRepeatDefs[23]; /* pad_initRepeat defaults */
typedef struct {
    u8 x0, y0, x1, y1, x2, y2, x3, y3;
} Unk80068F48;
extern Unk80068F48 g_winTailShapes[];
void akao_setVoiceAttack(s32, s32, s32);
void akao_setVoiceSustain(s32, s32, s32);
s32 chr_getClass(u32);
s32 chr_sumEquipBonus(u8 *, s32);
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
} Unk8006DD88; /* base stats per character class */
extern Unk8006DD88 D_8006DD88[];
void pad_initRepeat(void);
void win_initAll(void);
void preload_onReadDone(u8 *);
u8 cd_seekAsync(s32, void (*)(u8 *));
void akao_stopSeChannel(AkaoChannel *, u32);
typedef struct {
    u8 unk0[4]; /* stat bonuses */
    u8 unk4[4]; /* OR'd into BtlUnit.unk70[] (btl_initEquipElements) */
    u8 unk8;    /* OR'd into BtlUnit.unk79 */
    u8 pad9[0xC - 9];
} Unk8006D548;
extern Unk8006D548 D_8006D548[];
void chr_setRecruited(u32);
void msg_setFocus(MsgWindow *);
MsgWindow *msg_getFocus(void);
extern s32 g_msgChoiceLine;
extern s32 g_msgChoiceWindow;
extern Unk4 D_8007B99C;
extern Unk4 D_80011A6C; /* rodata constant, used by msg_advanceText */


void func_800615F8(void);
extern u8 D_8006F40C[];
extern Unk80080A10 *D_800831C8[24];
void akao_setVoiceRelease(s32, s32, s32);
void help_draw(void);
void help_toggle(void);
void sprAnim_update(Unk80035B90 *);
void menu_onPortraitsRead(u8 *);
void akao_relocInstruments(s32 *, Unk8005EBB4 *, u32, s32);
s32 akao_loadBankAt(s32 *, s32, s32, u32);
typedef struct {
    s32 unk0[4];
} Unk80010CD0;
extern Unk80010CD0 D_80010CD0; /* min widths per window style */
void func_80059414(void);
typedef struct {
    s16 unk0; /* x */
    s16 unk2; /* w */
    u8 pad4[4];
    u8 unk8; /* y */
    u8 unk9; /* h */
} Unk8004EBC0;
s32 msg_overlapsRect(Unk8004EBC0 *r);
extern s32 D_8007F720[]; /* AKAO header copy (0x70 bytes) */
extern u8 D_8006CACC[];
void menu_onOverlayRead(u8 *);
extern AkaoChannel g_akaoSavedChannels[];
void akao_copy(); /* (u32 *src, u32 *dst, u32 size): word copy */
u16 akao_shiftInstrumentDown(u32, AkaoChannel *);
s32 text_initNameImage(s32);
void text_renderPartyName(s32);
extern RECT D_8006BC38;
void func_80061C1C(void);
void akao_setVoicePitch(s32, s32);
void akao_setVoiceVolume(s32, u32, u32, s32);
u8 fade_isActive(void);
void menu_onFadedOut(void);
u16 pad_getFlags(void);
void pad_setFlags(u16, s32);
void pad_getAnalogThresholds(Unk4 *);
void pad_setAnalogThresholds(Unk4 *);
extern u8 g_padSavedStickFlag;
extern Unk4 g_padSavedThresholds;
extern Unk4 g_padStickDpadThresholds;
void menuScreen_drawCommands(void);
void menuScreen_updateOpen(void);
void func_80060D1C(s32, s32, s32, s32);
void func_80060ED8(s32, s32, void (*)(void));
s32 bgi_updateActor(u32); /* field overlay (ovl_0a) */
s32 fldbg_applyGeomOffset(void); /* field overlay (ovl_0a) */
s32 fldbg_resetGeomOffset(void); /* field overlay (ovl_0a) */
extern CdRequest g_cdReadRequest;
s32 func_8005EB40(s32 *, s32, s32, s32);
s32 CdMode(void);
s32 cd_startTransfer(); /* called with a u8 it ignores; callers see an int return (no prototype in the original) */
u8 timer_isEnabled(void);
void timer_draw(void);
void game_tickClock(void);
extern Unk80035B90Data D_80010E48;
void sprAnim_init(Unk80035B90 *, Unk80035B90Data *);
s32 akao_checkHeader(s32 *);
extern u16 *D_800809DC;
extern u16 *D_800809E4;
extern u16 *D_80083148;
s32 func_8005A5FC(s32);
void func_8005A20C(u8 **, u8 **, s32);
void akao_keyOn(u32);
extern u8 *D_800809E8;
extern s32 D_800809C0[];
extern s32 D_800809D8; /* = D_800809C0[6] */
void func_800616F8(s32, s32, s32, s32);
s32 preload_isBusy(PreloadEntry *);
u8 fade_clearEase(); /* called with an argument it ignores */
extern void (*g_menuStateFuncs[])(void);
extern s32 g_menuTypeOvl[];
void menu_readOverlay(void);
void menu_readPortraits(void);
typedef struct {
    u16 unk0;
    u16 unk2;
} Unk8006DDD0;
extern Unk8006DDD0 D_8006DDD0[];
typedef struct Unk8007BC20Node {
    struct Unk8007BC20Node *unk0; /* next */
    u8 pad4[0xC - 4];
} Unk8007BC20Node;
typedef struct {
    Unk8007BC20Node *unk0; /* free list */
    s32 unk4;
    s32 unk8;
    s32 unkC;
    DR_TPAGE unk10;
    DR_TPAGE unk18;
    u8 pad20[0x24 - 0x20];
    s32 unk24;
} Unk8007BC20;
extern u8 *D_8007BC18;
extern u8 *D_8007BC48;
extern Unk8007BC20 D_8007BC20;
extern Unk8007BC20 D_8007BC50;
void func_8005338C(s32);  /* hand-written asm module */
s32 func_80053498(s32, u8, u32 *, s32);
void func_80053658(s32);
s32 func_80053764(s32, u8, u32 *, s32);
void chr_setLevel(s32, s32, u32 *, u8);
extern u32 D_8006CA78[];
extern u32 g_fadeBattleOt[];
s32 akao_loadBank(s32, s32);
void pen_init(void);
void msg_resetWindows(void);
void msg_resetPad(void);
void rand_init(s32);
void evt_reset(void);
extern u16 D_8007AE12;
void menu_finish(void);
void menu_loadOverlay(void);
void menuScreen_onSwitchFaded(void);
CdReadCtx *cdRead_init(void *, s32, s32, void *, s32, s32, void (*)(CdReadCtx *));
s32 cdRead_update(void *);
void preload_init(); /* takes no args, but ovl_0b passes one */
void preload_setHook(s32);
PreloadEntry *preload_add(void);
s32 preload_isReady(s32);
void preload_remove(s32);
s32 cd_cancel(void);
void cd_stopMotor(void);
s32 cd_readSync(); /* TODO: (sector, size, dst); ovl_14 passes a 4th argument */
s32 cd_isShellOpen(void);
u8 cd_checkShellOpen(void);
void game_initGfx(void);
void ui_drawBackground(void);
void win_drawShadow(RECT *, s32, s32);
s32 ui_drawSpriteDef(s32, s32, s32, void *, u8);
u8 *text_append(u8 *, u8 *);
s32 ui_measureLabel(u8 *);
s32 text_measureSmall(u8 *s);
s32 text_measureDigits(u8 *s);
typedef struct {
    u8 pad0[4];
    u8 unk4; /* advance width */
    u8 pad5[3];
} Glyph8;
extern Glyph8 D_80068690[]; /* indexed by ASCII code */
extern Glyph8 D_80068890; /* & */
extern Glyph8 D_80068CE0; /* . */
extern Glyph8 D_80068DC8; /* _ */
void ui_drawLabel(s32, s32, s32, s32, s32);
void ui_drawSmallDigits(s32, s32, s32, u8 *, s32);
void text_drawBlock(s32, s32, s32, u8 *, s32);
u8 *text_copy(u8 *, u8 *);
void func_8002C064(s32);
void gfx_setBuffer(s32);
void pad_updateRepeat(s32);
void pad_setStickAsDpad(u8);
void win_setClip(RECT *, s32, s32);
Window *win_openFree(RECT *, s32, s32);
Window *win_init(RECT *, s32, s32, Window *);

/* Inlined copy of win_close (close window). */
static inline void win_close_inline(Window *w) {
    if (w != NULL && w->unk1C != 0 && w->unk1C != 2) {
        w->unk1C = 2;
        if (g_drawBufs.unk1 != 2) {
            win_setCloseAnim(w, w->unk28);
        }
    }
}

void win_close(Window *);
void win_closeNow(Window *);
void win_closeAllNow(void);
void win_drawBox(RECT *, s32, s32);
void win_drawListBox(RECT *, s32, s32, MenuList *);
void win_setOnOpened(Window *, void (*)(Window *));
void win_setDrawContent(Window *, void (*)(Window *));
void win_setDrawOverlay(Window *, void (*)(Window *));
void win_setOnClosed(Window *, void (*)(Window *));
void win_setTitle(Window *, s32);
void win_setShadow(Window *, u8);
void win_setOpenFrames(Window *, s16);
void SystemError(s32, s32);
void menuList_update(MenuList *);
void win_stepOpen(Window *);
void win_stepClose(Window *);
MenuView *menuList_getView(MenuList *, MenuView *);
void menuList_init(MenuList *, s32, s32, s32, s32);
void menuList_input(MenuList *, s32);
void menuList_left(MenuList *, s32);
void menuList_right(MenuList *, s32);
MenuView *menuList_getPageView(MenuList *, MenuView *);
void menuList_getRowsRect(MenuList *, RECT *, RECT *);
void snd_playSe(s32);
void func_8002F514(s32);
void func_8002F55C(s32);
u8 fade_isBusy(void);
s32 fade_setOnDone(void (*)(void));
u16 fade_setAngle(u16);
void fade_inDefault(void);
void game_startFade(s32, s32, s32, s32);
void menu_open(s32, s32);
void menu_initNewGame(void);
void menu_initForMode(void);
void menu_setOnClose(void (*)(void));
void menu_setFadeInOnClose(u8);
void menu_setFadeOutOnClose(u8);
s32 chr_levelUp(s32, s32);
void party_setChanged(u8);
u8 chr_hasSupportAbility(s32, s32);
u8 func_80034A30(Character *);
u8 chr_testSupportBit(u32 *, s32);
void help_setBounds(RECT *);
void help_drawCursor(); /* unprototyped: the argument is unused and most callers pass none */
void help_setCursorOnTop(u8);
void text_setBank(const u8 *);
u8 *text_get(s32); /* text lookup */
#if defined(OVL_0A_H) || defined(OVL_0B_H) || defined(OVL_0C_H) || defined(OVL_65_H)
void evt_init(s32); /* the overlays pass the script data as an integer */
#else
void evt_init(u8 *); /* start the field script system on a script data block */
#endif
void evt_initSystem(void);
void msg_init(void);
void evt_clearGlobalVars(void);
void func_8005392C(void);
s32 snd_init(void);
void akao_stopMusicNow(void);
void akao_stopAllSe(void);
s32 akao_beginBankUpload(void);
s32 akao_uploadBank(s32 *, u32, s32);
s32 CloseEvent(s32);
s32 DisableEvent(s32);
void card_initAll(void);
void func_8006237C(void);
s32 card_addWithStats(u8, u8, u8, u8, u8, u8);
s32 card_add(s32);
s32 card_countAll(void);
s32 card_countKinds(void);
s32 rand(void);
s32 card_getName(s32); /* number to string */
void card_addWin(void);
void card_addLoss(void);
void card_addDraw(void);
void func_80062750(s32);
FileEntry *dep_find(FileTable *, s32);
void strcat(u8 *, u8 *);
/* per-frame offset animation of a window (tween_start / tween_step) */
typedef struct TweenDef {
    u16 unk0; /* start delay */
    s16 unk2; /* frame count */
    s8 *unk4; /* x deltas */
    s8 *unk8; /* y deltas */
    void (*unkC)(struct TweenDef *); /* called when done */
} TweenDef;

/* text window (tween_start / tween_step) */
typedef struct {
    s16 unk0; /* start delay */
    s16 unk2; /* frame */
    RECT unk4;
    TweenDef *unkC; /* running animation, NULL when idle */
} WinDef; /* 0x10 */
void tween_start(void *, void *, s32);
void gfx_initTpageSprt(DR_TPAGE *, s32);
void gfx_initTpageTile(DR_TPAGE *, s32);
void game_newGame(void);
s32 TestEvent(s32);
void _card_info(s32);
void _card_wait(s32);

/* ---- main batch 1 ---- */

typedef struct {
    u8 unk0[8];
} Unk8; /* 8 bytes, byte-aligned (copied with lwl/lwr) */
extern Unk8 g_cdResult; /* last CD status/result bytes */

s32 evt_callObjectFunc(Unk8004AB64 *, s32, s32, s32);
extern s32 D_8007AE34;
extern CdlATV D_8007F718;
void func_8005A284(); /* (Unk80080A10 *, AkaoChannel *) */
void card_rollStats(Unk6 *, u8);
extern s32 g_akaoLfoWaves[];

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC; /* also accessed as D_80080A24 */
} Unk80080A18;
extern Unk80080A18 D_80080A18;


extern AkaoChannel g_akaoChannels[];
extern s32 D_80080AC4;
extern AkaoChannel g_akaoSeChannels[];
extern s32 D_800831C4;

typedef struct {
    s32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    s32 unk10;
    union {
        u32 w;
        u16 h[2];
    } unk14; /* tempo, 16.16 */
    u32 unk18; /* tempo accumulator */
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u16 unk28;
} Unk80080A70;
extern Unk80080A70 g_akaoSeState;
void akao_opC4(AkaoChannel *, u32);
void akao_opC6(AkaoChannel *, u32);
extern u16 D_80080A98; /* = g_akaoSeState.unk28 */
extern s32 D_80080A80;


typedef struct {
    void (*unk0)(u8);
    void (*unk4)(u8);
    s32 unk8;
} Unk8006C9D4;
extern Unk8006C9D4 g_menuTypes[];
s32 win_resetClock(void);
void help_update(void);

extern s32 g_evtArgFlags;
extern s32 g_evtArgRead;
extern struct Unk8004AB64 *g_evtCurObj; /* current script object */
void evt_evalExpr(void);
s32 evt_popResult(void);
void func_80052D70(s32);
extern s32 D_8007B718;
void msg_update(void);
void evt_updateIdleTimer(void);
s32 evt_updateObjects(void); /* per-frame field object update */
extern s32 D_8007AEA8;
extern s32 D_8007AEAC; /* the player moved this frame */
extern s32 D_8007AEB0;
extern s32 D_8007AEB4;
extern u8 g_worldPlayerStepped; /* world map (ovl_0c) data */
void evt_movePlayer(Unk8004AB64 *);
void evt_updateLookAt(Unk8004AB64 *);
void evt_checkPlayerAction(Unk8004AB64 *);
void evt_updateBattleObjects(void);
void evt_stepAnim(Unk8004AB64 *);
void msg_drawHereIcon(Unk8004AB64 *);
s32 func_80052870(Unk16D8 *);
s32 func_80052EC0(s32, s32, s32);
s32 func_8002170C(Unk8004AB64 *);
s32 evt_rollEncounter(Unk8004AB64 *);
#if !defined(OVL_0A_H) /* field overlay functions; ovl_0a has the definitions */
s32 func_800B3444(s32, s32, s32);
/* field walkmesh actor entry returned by ovl_0a bgi_getActor (ovl_0a calls it BgiActor) */
typedef struct {
    s16 unk0;
    u8 pad2[6];
    u8 unk8;
    u8 pad9;
    u16 unkA;
} Unk800C748C;
void *bgi_getActor(s32);
s32 bgi_getActorRadius(s32, s32 *);
s32 bgi_revertActor(s32);
#endif
Window *msg_getWindow(s32);
void evt_followBalloon(Unk8004AB64 *, Window *);
s32 func_800531D0(Unk8004AB64 *, Unk8004AB64 *, s32);
void func_800530A8(SVECTOR *, s32, s32);
void msg_updatePlayerBubble(void);
void msg_drawAtePrompt(void);

u16 btl_getUnitMask(u32); /* ovl_0b code */
void menuScreen_startTitleAnim(void);
void menuScreen_requestOverlay(void);
void menuScreen_onOpenFaded(void);
void fade_outDefault(void);

void cd_startRequest(CdRequest *);
u8 cd_submit(CdRequest *);
void cd_onSync(u8, u8 *);
void cd_onDataReady(u8, u8 *);
s32 akao_remapInstrument(s32, s32);
s32 func_8005EC8C(s32, s32);
void akao_setChannelProgram(AkaoChannel *, s32);
void akao_spuWrite(void *, s32);
void akao_waitSpuTransfer(void);
void akao_initState(void);
s32 akao_tick(void); /* root counter 2 callback; returns the ticks this tick took */
extern u8 D_80080990[];
void UnDeliverEvent(s32, s32);
void akao_keyOff(s32);
s32 StopRCnt(s32);
void SpuQuit(void);
extern s32 g_akaoTickEvent;
extern u8 D_8006F334[];
extern s32 g_evtMode;
/* offset-table blob: count, then per-entry u16 offsets relative to unk4 */
typedef struct Unk8007B954 {
    s32 unk0; /* count */
    u16 unk4[1];
} Unk8007B954;
extern Unk8007B954 *g_msgTable;
extern u8 *D_8007BB80[];

typedef struct {
    u8 pad0[2];
    s16 unk2;
    u8 pad4[9 - 4];
    u8 unk9;
} Unk8004EFB0;

/* ---- main batch 2 ---- */

extern CdRequest g_cdRetryRequest;
extern CdRequest g_cdRestartRequest;
extern Unk8004AB64 *D_8007B704;

/* Same as evt_getArg8_inline, but written as `>>= (g_evtArgRead = 1)` (srav). */
static inline s32 evt_getArg8_inline2(void) {
    s32 r;
    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        r = evt_popResult();
    } else {
        r = *g_evtCurObj->unk0++;
    }
    g_evtArgFlags >>= (g_evtArgRead = 1);
    return r;
}

/* Inlined copy of func_8004B4C4 (resolve a script object reference). */
static inline Unk8004AB64 *func_8004B4C4_inline(s32 id) {
    if (id == 0xFF) {
        return D_8007B704;
    }
    if (id == 0xFA) {
        id = g_evtState->playerUid;
    } else if ((u32)(id - 0xFB) < 4) {
        id = g_evtState->unk16E4[id - 0xFB];
    }
    return evt_getObject_inline(id);
}
extern s32 D_8007B710;
extern s32 D_8007BBC8;
extern s32 D_8007BBE0;
extern s32 D_8007BBE8;
extern AkaoChannel *g_akaoSubChannels;

typedef struct {
    u8 pad0[4];
    s16 unk4;
    u8 pad6[2];
} Unk8007F798;
extern Unk8007F798 D_8007F798[24];
void func_80058830(); /* (AkaoChannel *, s32 voice) */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    Rgb32 unk30;
    Rgb32 unk3C;
    Rgb32 unk48; /* output */
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u32 unk60; /* first-phase frames */
    s32 unk64; /* second-phase frames */
} Unk80077D00;
extern Unk80077D00 g_fadeWipe;

typedef struct {
    s32 unk0;
    s32 unk4;
} Unk8005AE14;

/* ---- main batch 3 ---- */
typedef struct {
    u8 pad0;
    u8 unk1;
    u16 unk2; /* name offset */
    u8 pad4[4];
} Unk8006D128; /* support abilities, indexed by id - 0xC0 */
extern Unk8006D128 g_supportAbilities[];
void chr_setSupportAbility(s32, s32, u8);
extern s32 g_menuScreenOvl[];
extern RECT D_8006E508;
extern Unk80077E28Entry g_menuScreensField[];
extern Unk80077E28Entry g_menuScreensWorld[];
u8 cd_readAsync(s32, s32, void *, void (*)(u8 *));
void menuScreen_onOverlayRead(u8 *);
extern void *g_evtUnitObjects[8];
#ifndef OVL_0B_H /* battle overlay code; ovl_0b.h has the real prototypes */
struct BtlUnit *btl_findUnitByBit(u16);
s32 btl_getUnitById(s32, s32);
void btl_setUnitAttr(s32, s32, s32);
#endif
void cd_onDiskError(s32);
void cd_onSyncQueued(u8);
extern void (*g_cdStateHandlers[])(s32);

/* ---- main batch 4 ---- */
u8 sprAnim_setPlaying(Unk80035B90 *, u8);
void sprAnim_setOnDone(Unk80035B90 *, void (*)());
void sprAnim_reset(Unk80035B90 *);
void help_onIconAnimDone(void);
extern s32 D_8007B9A0;
extern u8 *D_8007B9A4;
extern u8 *D_8007B9A8;
extern u32 D_8007B9B0[];
extern u8 **g_penPktPtr;
void pen_putGlyph(s32);
extern u8 D_8007B930[8]; /* name text being converted */
u32 pen_getGlyph(s32); /* glyph info of a text code, width in the top 4 bits */
s32 func_8004B120(s32);
void msg_measureLine(u8 **, s32 *); /* add the width of a text control code */

typedef struct {
    u8 *unk0; /* stream read pointer */
    u8 *unk4; /* loop start */
    s32 unk8;
    u32 unkC;
    s32 unk10;
    u32 unk14; /* bytes left */
    u32 unk18; /* loop length (initial) */
    u32 unk1C; /* loop length */
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u32 unk38;
    u32 unk3C;
    s32 unk40; /* volume */
    s32 unk44;
    s32 unk48;
    s32 unk4C; /* pan in bits 8-15 */
    u8 pad50[0x58 - 0x50];
    s32 unk58;
} Unk80083168;
u32 SpuWrite(u8 *, u32);
void func_80060FA8(void);
void akao_stopSeq(Unk80080A10 *, AkaoChannel *, s32);
void SpuGetReverbModeType(s32 *); /* libspu SpuGetReverbModeType */
void func_800618C0(s32, s32);
void func_80060AE0(s32, s32, s32);
extern Unk80083168 g_akaoStream;
s32 func_80061BC8(); /* K&R: callers pass one or two arguments */
s32 func_80061FBC(s32, s32, s32, void (*)(void));
void akao_setVoiceLoopAddr(s32, s32);
void akao_setReverbMode(s32 mode);
void func_80062118(void);
void func_80061E1C(s32, s32, void (*)(void));
void func_800621D4(void);

/* ---- main batch 5 ---- */
s32 preload_isUsed(PreloadEntry *);
PreloadEntry *preload_getCurrent(void);
void ui_loadPartyPortraits(u8 *);
u8 party_hasChanged(void);
void FlushCache(void);
extern u8 *g_randPtr;
extern u8 g_randTable[]; /* random byte table */

extern u8 g_randTableEnd[];
void rand_refill(void);

/* ---- hoisted for menu overlays ---- */
void ui_initMemberSwitch(UnkBlink *, s32, void (*)(void));
void ui_startMemberSwitch(UnkBlink *, s32, s32, s32);
void ui_updateMemberSwitch(UnkBlink *);
void text_setStringArg(s32, u8 *);
void text_setIconArg(s32, s32);
u8 help_getFollow(void);
void help_setFollow(u8);
void menuScreen_open(s32);
s32 party_getPrevMember(s32);
s32 party_getNextMember(s32);
void ui_drawEtchedLine(s32, s32, s32, s32);
typedef struct {
    u8 unk0; /* ability id, 0 = none */
    u8 unk1; /* ap needed? */
} AbilityEntry;
extern AbilityEntry g_abilityRows[16][0x30]; /* rows indexed by Character.unk34.b.row */

/* Inlined copy of chr_findAbilitySlot (ability slot of a character). */
static inline s32 chr_findAbilitySlot_inline(s32 ch, s32 id) {
    s32 row;
    s32 i;
    if (ch >= 9) {
        return -1;
    }
    row = ((g_gameState->unk1C->characters + ch)->unk34.w >> 12) & 0x1F;
    if (row >= 16) {
        return -1;
    }
    for (i = 0; i < 0x30; i++) {
        if (g_abilityRows[row][i].unk0 == id) {
            return i;
        }
    }
    return -1;
}

void ui_drawGauge(s32, s32, s32, s32, s32, s32, u8);
/* gauge colour set per style (ui_drawGauge): c[2]/c[3] are the bar's left/right
   colours, c[0]/c[1] the blink targets; c[0] is also the frame colour */
typedef struct {
    CVECTOR c[4];
} GaugeColors;
extern GaugeColors D_80068FD8[];
/* o = f + (t - f) * n / d per channel (o is a CVECTOR, f/t point to CVECTORs) */
#define CVECTOR_LERP(o, f, t, n, d)                         \
    (o).r = (f)->r + ((t)->r - (f)->r) * (n) / (d);         \
    (o).g = (f)->g + ((t)->g - (f)->g) * (n) / (d);         \
    (o).b = (f)->b + ((t)->b - (f)->b) * (n) / (d)
extern u8 D_8007AF00;
typedef struct {
    u8 unk0; /* show second character */
    u8 unk1; /* slide direction */
    u8 unk2; /* party slot */
    u8 unk3; /* second party slot */
    s32 unk4; /* slide step */
} Unk80024428;
void ui_drawMemberHeader(s32, s32, s32, Unk80024428 *);
void ui_drawStatusIcons(s32, s32, s32, u8, s32); /* arg4 unused, callers pass 0 */
void ui_drawStones(s32, s32, s32, s32, s32);
void chr_setDefaultName(u8 *, s32); /* default name by id */
s32 text_getLength(u8 *); /* strlen */
extern u32 g_menuArg; /* = g_menu.arg, read on its own by ovl_3a */
extern u32 g_padPressed; /* buttons that went down this frame */
void ui_restorePortraits(void);
void ui_getPortraitRects(s32, RECT *, RECT *);
typedef struct {
    u32 *unk0; /* portrait image data, 0x6C0 bytes each */
    s32 unk4;  /* count */
} Unk80076F78;
extern Unk80076F78 g_portraits;
extern u32 g_portraitSavedVram[]; /* first CLUT, right after g_portraits */
s32 item_remove(s32, s32);
typedef struct {
    u8 pad0[4];
    u8 unk4; /* width */
    u8 pad5[3];
} Icon; /* sprite drawn by ui_drawSpriteDef */
extern Icon D_800687C0;
extern Icon D_80068E30;
extern Icon D_80068E38;
extern s32 D_80070494;
void ui_drawHand(s32, s32, s32);
void pad_setRepeatDelay(s32, s32, s32);
void pad_setRepeatInterval(s32, s32, s32);
s32 cd_getState(void);
s32 card_countOf(); /* defined K&R-style: u8 param, callers pass ints unmasked */
Unk6 *card_find(s32, s32);
s32 card_remove(s32, s32);
typedef struct {
    u8 unk0; /* power (x2) */
    u8 unk1; /* type */
    u8 unk2; /* physical defence (x2) */
    u8 unk3; /* magic defence (x2) */
    u8 unk4; /* collector points */
} CardInfo; /* 5 bytes */
extern CardInfo D_800121C8[]; /* card table */
CardInfo *card_getInfoTable(void);
s32 card_getRarity(s32);
extern s32 D_800704C4;
extern s32 D_80070418;
void help_setLocked(u8);
s32 keyItem_has(s32);
s32 tween_sumDeltas(s32, s8 *);
extern s32 g_padPortState[];
u8 *ui_buildSprite(s32, s32, u32 *, s32, u8, s32 *, void *);
u8 *text_buildPlain(s32, s32, u32 *, Character *, s32 *, u8 *);
u8 *text_buildDpad(s32, s32, u32 *, u32, u8, s32 *, u8 *);
u8 *text_buildCode(s32, s32, u32 *, s32, s32 *, u8 *); /* draw a text control icon */
#if !defined(OVL_3A_H)
u8 *text_buildFontTpage(u32 *, DrTpageWords *);
#endif
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} Unk8006BC0C;
extern Unk8006BC0C D_8006BC0C[4];
u8 help_isOpen(void);
void ui_drawRule(s32, s32, s32, s32, s32, s32);
void menuList_down(MenuList *, u32);
void menuList_pageUp(MenuList *, s32);
void menuList_pageDown(MenuList *, s32);
void menuList_up(MenuList *, u32);
extern u32 g_padHeld; /* buttons held */
extern s32 D_80070430;
extern u8 D_8007AF02;
void chr_updateStats(Character *);
s32 chr_calcAppearance(s32, s32, u8 *);
void ui_loadPortraits(void *, s32);
void func_8002C1E4(void);
s32 nextfile(s32);
s32 close(s32);
s32 _card_load(s32);
s32 write(s32, s32, s32);
s32 read(s32, s32, s32);
void *card_getInventory(void);
extern s32 D_80070440;
extern s32 D_8007048C;
void party_setMember(s32, s32);


/* ---- hoisted for the world map (ovl_0c) ---- */
s32 func_80062E3C(s32, s32, s32, s32, s32, s32, s32);
#if defined(OVL_0A_H) || defined(OVL_0C_H) /* the field code only reads port 1 */
extern s32 g_padPortPressed;
#else
extern s32 g_padPortPressed[2]; /* went down this frame, per port */
#endif
extern s32 g_padPortReleased[2]; /* buttons released this frame */
extern s32 g_padPortPrevHeld[2]; /* previous g_padPortHeld */
extern s32 g_padHoldFrames[2][32]; /* frames each button has been held */
extern Unk4 g_padAnalog[2]; /* analog stick bytes */
extern u8 g_padActAlign[]; /* actuator alignment */
/* byte view of a g_padBuf pad buffer */
typedef struct {
    u8 unk0; /* status, 0 = ok */
    u8 unk1;
    u16 unk2; /* buttons, active low */
    u8 unk4[4]; /* analog sticks */
} PadBuf;
#define PADBUF(i) ((PadBuf *)&g_padBuf[i])
void pad_update(void);
void game_updateFade(void);
void game_updateTimer(void);
void game_setScreenHeight(s32);
s32 evt_update(); /* defined K&R-style: u8 param (unused); some callers pass none */
u8 menuScreen_needsMember(s32);
s32 menuScreen_getCommandHelp(s32);
void menuScreen_close(void);
s32 vib_clear(void);
s32 func_80050AAC(void);
void akao_stopSe(s32, s32);
void akao_sendCmdE2(void);
s32 game_getPlayTime(void);
s32 text_formatTime(s32, u8 *, s32);
extern s32 D_800704C0;
extern u8 g_itemNames[]; /* item names */
extern u8 D_8006ADAC[]; /* key item names */
typedef struct {
    s16 unk0; /* name offset into D_8006ADAC */
    s16 unk2; /* help offset */
    u8 pad4[2];
    s16 unk6; /* description offset */
} Unk8006B374;
extern Unk8006B374 D_8006B374[];
Unk16D8 *evt_getObjectList(void);
extern s32 D_8007046C;
void SetBackColor(s32, s32, s32);
void SetFarColor(s32, s32, s32);
void SetFogNearFar(s32, s32, s32);
s32 party_getFirstMember(void);
extern u8 D_80073939;
extern u8 D_8007393A; /* pad analog stick */
extern u8 D_8007393B;
void printf(const char *, ...);

/* Inlined body of msg_closeAll for one entry (close and unlink). */
static inline void msg_closeAll_inline(MsgWindow *p) {
    MsgWindow *q;
    if (p != NULL && p->open != 0) {
        p->open = 0;
        if (msg_getFocus() == p) {
            msg_setFocus(p->parent);
        }
        for (q = g_msgWindows; q < g_msgWindows + 10; q++) {
            if (q->open != 0 && q->parent == p) {
                q->parent = NULL;
            }
        }
    }
}

/* Inlined copy of msg_closeWindow (close an entry, its window and its children). */
static inline void msg_closeWindow_inline(MsgWindow *p) {
    if (p != NULL && p->open != 0) {
        if (p->window != NULL) {
            win_close(p->window);
        }
        if (g_msgChoiceLine >= 0 && g_msgChoiceWindow == p->unkA) {
            pad_setAnalogThresholds(&D_8007B99C);
        }
        msg_closeAll_inline(p);
    }
}
extern s32 D_8007B990;

/* actor-like object updated by func_8004B9B8 */
typedef struct {
    u8 pad0[0xF];
    u8 unkF;
    u8 pad10[0x22 - 0x10];
    u16 unk22;
    u8 pad24[0x34 - 0x24];
    u16 unk34;
    u16 unk36;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
    u8 pad3C[0x50 - 0x3C];
    u16 unk50;
    u16 unk52;
    u16 unk54;
    u8 pad56[0x5A - 0x56];
    u16 unk5A;
    u8 pad5C;
    u8 unk5D;
    u8 pad5E[0x60 - 0x5E];
    u8 unk60;
    u8 unk61;
    u8 unk62; /* flags */
    u8 unk63; /* timer */
    u8 unk64;
    u8 pad65[0x69 - 0x65];
    u8 unk69;
    u8 pad6A[0x7C - 0x6A];
    u8 unk7C[4];
} Unk8004B9B8;
s32 evt_getAnimFrameCount();
s32 game_getModel(s32);

/* Inlined copy of func_8004B9B8. */
static inline void func_8004B9B8_inline(Unk8004B9B8 *p) {
    s32 v;
    p->unk22 = p->unk52 & 0xFFF;
    v = p->unk5A;
    if (v != p->unk36) {
        p->unk36 = v;
        p->unk3A = 0;
        p->unk64 = 0;
        p->unk38 = evt_getAnimFrameCount(p->unk34, v);
        p->unk5D = p->unk69;
    }
    p->unkF &= 0x7F;
}

s32 evt_getSysVar(s32);
extern u8 *D_800CF478;
/* help-window rect plus frame style and x offset (help_layout) */
typedef struct {
    RECT unk0;
    s16 unk8; /* style */
    s16 unkA; /* x offset */
} Unk80035214;
void help_layout(Unk80035214 *);
void sprAnim_draw(s32, s32, s32, Unk80035B90 *);
s32 fade_stepClamp(s32, s32, s32);
s32 fade_ease(s32, s32, s32);
typedef struct {
    u32 tag;
    u32 rgbc;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} LineF2Words; /* LINE_F2 */
extern u32 D_80068F20[][4]; /* line colours */
extern s8 D_80068F94[4][4]; /* shadow line offsets (win_drawLineBorder) */
typedef struct {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} LineXY;
u8 *ui_buildSpriteDef(s32 x, s32 y, u32 *ot, Unk800686E8 *d, u8 flag, s32 *out, SprtWords *s);
u8 *ui_buildDigits(s32 x, s32 y, u32 *ot, u8 *s, s32 unused, SprtWords *p);
/* Inlined copy of ui_buildSpriteDef (SPRT + DR_TPAGE from a sprite descriptor). */
static inline u8 *ui_buildSpriteDef_inline(s32 x, s32 y, u32 *ot, Unk800686E8 *d, u8 flag, s32 *out, SprtWords *s) {
    DrTpageWords *t;
    setlen(s, 4);
    if (flag) {
        s->rgbc = 0x66606060;
    } else {
        s->rgbc = 0x64808080;
    }
    s->xy = (x & 0xFFFF) | (y << 16);
    s->uvclut = d->unk0;
    s->wh = d->unk4 | (d->unk5 << 16);
    addPrim(ot, s);
    s++;
    t = (DrTpageWords *)s;
    setlen(t, 1);
    t->code = (d->unk6 & 0x9FF) | 0xE1000600;
    addPrim(ot, t);
    *out = d->unk4;
    s = (SprtWords *)(t + 1);
    return (u8 *)s;
}
/* Inlined copy of func_8004B2F4 (move an object's node to the free list). */
static inline void func_8004B2F4_inline(Unk8004AB64 *key) {
    Unk8007AECCNode *n;
    Unk8007AECCNode *prev = NULL;
    for (n = g_evtState->unk15D0; n != NULL; n = n->unk8) {
        if (n->unk0 == key) {
            break;
        }
        prev = n;
    }
    if (prev != NULL) {
        prev->unk8 = n->unk8;
    } else {
        g_evtState->unk15D0 = n->unk8;
    }
    n->unk8 = g_evtState->unk15D4;
    g_evtState->unk15D4 = n;
}
void win_drawScrollArrows(RECT *, s32, MenuList *);
void win_drawSplitFill(RECT *, s32, MenuList *, s32);
void win_drawDivider(RECT *, s32, MenuList *, s32);
void win_drawLineBorder(RECT *, s32, u8, s32);
void win_drawFill(RECT *, s32, u8, u8, s32);
extern s32 D_8008315C;
extern s32 D_800831A8; /* sound effect volume */
extern s32 D_800831C0;
extern s16 D_8006F640;
extern s16 g_akaoPanCurve[]; /* pan curve, 256 entries */
/* item icon sprites (D_800689B0) */
typedef struct {
    u8 unk0; /* u */
    u8 unk1; /* v */
    u16 unk2; /* clut */
    u8 unk4; /* w */
    u8 unk5; /* h */
    u16 unk6; /* tpage */
} Unk800689B0;
extern Unk800689B0 D_800689B0[];
extern Unk800689B0 g_winTailSprites[];
u8 win_isVisible(Window *);
void win_drawFrame(Window *);
void win_drawTitle(Window *);
/* sprite cell of a frame animation (sprAnim_drawCell) */
typedef struct {
    u8 unk0; /* u */
    u8 unk1; /* v */
    u8 unk2; /* x + 0x80 */
    u8 unk3; /* y + 0x80 */
    u32 unk4; /* clut | tpage bits << 16 */
    u16 unk8; /* w */
    u16 unkA; /* h */
} Unk800357E0;
/* debug/system text output (hand-written asm, 0x80051xxx) */
void pen_begin(void);
void pen_setPos(s32, s32);
void pen_move(s32, s32);
void pen_drawSprite(s32);
void pen_end(void);
extern s32 *g_evtExprSp; /* script value stack pointer */
extern void *D_8007B700; /* script current object */

/* Inlined copy of func_8004B404 (read script variable, refreshing 2-4). */
static inline u16 func_8004B404_inline(s32 arg0) {
    s32 i = arg0 & 7;

    switch (i) {
    case 2:
        D_8007AE10[2] = btl_getUnitMask(0);
        break;
    case 3:
        D_8007AE10[3] = btl_getUnitMask(1);
        break;
    case 4:
        D_8007AE10[4] = btl_getUnitMask(2);
        break;
    }
    return D_8007AE10[i];
}
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Unk8007BB98;
extern Unk8007BB98 D_8007BB98;
extern s32 D_8007B964;
void msg_renderText(MsgWindow *, u8 *); /* draw a window's text */
extern DVECTOR *g_penStackPtr; /* pen position stack (pen_pushPos) */
typedef union {
    s32 w;
    struct {
        s16 x;
        s16 y;
    } s;
} PenPos;
extern PenPos g_penPos; /* current pen position */
/* "!" balloon sprite (msg_drawHereIcon) */
typedef struct {
    u8 unk0; /* u */
    u8 unk1; /* v */
    u16 unk2; /* clut */
    u8 unk4; /* w */
    u8 unk5; /* h */
} Unk80068790;
extern Unk80068790 D_80068790;
extern s32 D_8007BBE4; /* balloon flipped (pen x < 24) */
extern u8 D_8006E67C[];
void func_800512B4(s32);
void pen_setLastSemiTrans(void);
s32 msg_emitNumber(s32, s32);
s32 msg_emitString(u8 *, s32);
s32 msg_emitItemName(s32, s32);
s32 msg_emitCharName(s32, s32);
s32 msg_emitCharNameCaps(s32, s32);
void msg_advanceText(MsgWindow *, u8 *);
void msg_stepControl(MsgWindow *, u8 *);
void pen_pushPos(void);
void pen_popPos(void);
void pen_setPalette(s32);

/* Inlined lookup of message i in the current offset table (g_msgTable). */
static inline u8 *msg_drawText_msg(s32 i) {
    u8 *e = NULL;
    u16 *t;

    if (i >= 0 && i < g_msgTable->unk0) {
        t = g_msgTable->unk4;
        e = (u8 *)t;
        e += t[(g_evtMode == 2 || g_evtMode == 4) ? i * 2 : i * 4 + 2];
    }
    return e;
}
/* AP learned by a character for ability id, -1 if not learnable (inlined helper). */
static inline s32 chr_findAbilitySlot_ap_inline(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    if (i < 0) {
        return -1;
    }
    return g_gameState->unk1C->characters[ch].ap[i];
}

/* AP needed to master ability id (inlined form of chr_getApNeeded). */
static inline s32 chr_getApNeeded_inline(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    Character *c = g_gameState->unk1C->characters + ch;
    if (i < 0) {
        return 0;
    }
    return g_abilityRows[c->unk34.b.row][i].unk1;
}
void akao_setVoiceStartAddr(s32, s32);
void akao_setVoiceDecay(s32, s32);
void akao_setVoiceSustainLevel(s32, s32);
/* streamed AKAO sound header (func_800616F8) */
typedef struct {
    s32 unk0; /* "AKAO" */
    s32 unk4;
    u8 pad8[0x10 - 0x8];
    u32 unk10; /* data size */
    u32 unk14; /* loop start offset */
    s32 unk18; /* flags */
    u16 unk1C;
    u8 pad1E[0x20 - 0x1E];
    u32 unk20; /* loop length */
    u8 pad24[0x40 - 0x24];
    u8 unk40[1]; /* sample data */
} AkaoStream;
s32 func_80060A08(void);
void func_80061624(void);
void func_8006100C(void);
void func_80060A98(void);
u8 *text_buildGlyph(s32, s32, u32 *, u32, s32, s32 *, SprtWords *);
void win_drawTail(Window *);
s32 chr_calcBaseSpeed(s32, s32, u8);
s32 chr_calcBaseStrength(s32, s32, u8);
s32 chr_calcBaseMagic(s32, s32, u8);
s32 chr_calcBaseSpirit(s32, s32, u8);
s32 chr_calcMaxHp(s32, s32);
s32 chr_calcMaxMp(s32, s32);
s32 chr_calcMaxStones(s32, s32, u8);
void chr_fitSupportAbilities(Character *);
POLY_FT4 *pen_addPolyFT4(void);
typedef struct {
    u8 unk0; /* u */
    u8 unk1; /* v */
    u16 unk2; /* clut */
} Unk80068788;
extern Unk80068788 D_80068788;
extern s32 D_8007BC00; /* colour */
extern s32 D_8007BBF8; /* half width */
extern s32 D_8007BBFC; /* height */
extern s32 D_8007BC04;
extern s32 D_8007AEC8;
extern u8 D_8006E6D8[];
extern u8 D_8006E690[];
extern u8 D_8006E6F0[];
Unk8004AB64 *evt_getObject(s32);
void evt_setDrawPosAtObject(Unk8004AB64 *, s32, s32);
void msg_drawPlayerBubble(void);
/* zero the first three bytes (r, g, b) of a 4-byte colour */
static inline void fade_init_clear3(u8 *p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
}
typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x18 - 4];
    s16 unk18;
    u8 pad1A[0x50 - 0x1A];
    MsgWindow *unk50;
} Unk8004F5D4;
void pen_endFont(void);
void msg_drawText(MsgWindow *);
/* 8-byte message-table entry (g_msgTable in modes other than 4) */
typedef struct {
    u32 unk0;
    u16 unk4;
    u8 unk6;
    u8 unk7;
} MsgEntry8;

typedef struct {
    u8 pad0[2];
    s16 unk2;
    u8 pad4[9 - 4];
    u8 unk9;
} MsgInfo;
/* SPU hardware voice registers (0x1F801C00 + voice * 16) */
typedef struct {
    u16 volL; /* volume left */
    u16 volR; /* volume right */
    u16 pitch; /* pitch */
    u16 startAddr; /* start address */
    u16 adsr1; /* adsr 1 */
    u16 adsr2; /* adsr 2 */
    u16 adsrVol;
    u16 loopAddr; /* loop address */
} SpuVoiceReg;
#define SPU_VOICE_REG(v, r) (*(u16 *)(SPU_VOICE_BASE + (r) + (v) * 16))
#define SPU_VOICE_REG8(v, r) (*(u8 *)(SPU_VOICE_BASE + (r) + (v) * 16))
#define SPU_REG16(a) (*(u16 *)(a))
/* SPU registers (psx-spx names) */
#define SPU_VOICE_BASE 0x1F801C00 /* 24 voices x 16 bytes (SpuVoiceReg) */
#define SPU_MAIN_VOL_L 0x1F801D80
#define SPU_MAIN_VOL_R 0x1F801D82
#define SPU_KEY_ON_LO 0x1F801D88
#define SPU_KEY_ON_HI 0x1F801D8A
#define SPU_KEY_OFF_LO 0x1F801D8C
#define SPU_KEY_OFF_HI 0x1F801D8E
#define SPU_PITCH_MOD_LO 0x1F801D90
#define SPU_PITCH_MOD_HI 0x1F801D92
#define SPU_NOISE_ON_LO 0x1F801D94
#define SPU_NOISE_ON_HI 0x1F801D96
#define SPU_REVERB_ON_LO 0x1F801D98
#define SPU_REVERB_ON_HI 0x1F801D9A
#define SPU_CTRL 0x1F801DAA
#define SPU_CD_VOL_L 0x1F801DB0
#define SPU_CD_VOL_R 0x1F801DB2
extern s16 g_akaoCdVolumeHi;
extern s32 g_akaoCdVolumeStep;
s32 akao_isqrt(s32);
extern s32 D_8007F8E8;
extern s32 D_8007F8E4;
extern s32 D_8007F740;
extern Unk80080AA0 D_800809F0;
typedef struct {
    u16 unk0[6];
} Unk8007BD44Row;
extern u32 D_80083174;
/* voice parameter block written to the SPU by akao_writeVoice */
typedef struct {
    u8 pad0[4];
    s32 dirty;
    u32 startAddr;  /* start address */
    u32 loopAddr;  /* loop address */
    u16 pitch; /* pitch */
    u16 adsr1; /* adsr 1 */
    u16 adsr2; /* adsr 2 */
    u16 volScale; /* volume scale */
    s16 volL; /* volume left */
    s16 volR; /* volume right */
} AkaoVoiceRegs;
/* script object (g_evtUnitObjects[]) */
typedef struct Unk8007AED0 {
    u8 pad0[0xE];
    u8 unkE;
} Unk8007AED0;
typedef struct {
    void (*unk0)(FadeState *);
    s32 unk4;
    s32 unk8;
} Unk8006BD04;
extern Unk8006BD04 g_fadeTypes[];
extern u8 D_8006BCFC[]; /* window style -> colour */
typedef struct {
    u8 unk0[8];
} Bytes8;
extern Bytes8 D_800121C0; /* card arrow bits (card_rollStats) */
typedef struct {
    u8 unk0[5];
} Bytes5;
typedef struct {
    u32 unk0[2];
} Words2;
/* stat calculation input (chr_calcEquipStats) */
typedef struct {
    Bytes8 unk0; /* = Character.unk2C..unk33 */
    Words2 unk8; /* = Character.unk88 */
    u16 unk10;
    u16 unk12;
    Bytes5 unk14; /* equipment */
} Unk80032E84In;
/* stat calculation output (chr_calcEquipStats) */
typedef struct {
    u16 unk0; /* hp */
    u16 unk2; /* mp */
    u16 unk4; /* max hp */
    u16 unk6; /* max mp */
    u8 unk8[4]; /* stats */
    u16 unkC[5]; /* attack, defences */
} Unk80032E84Out;
Unk80032E84Out *chr_calcEquipStats(Unk80032E84In *, Unk80032E84Out *);

/* Inlined copy of chr_updateStats (recompute a character's stats). */
static inline void chr_updateStats_inline(Character *c) {
    Unk80032E84In in;
    Unk80032E84Out out;

    in.unk0 = *(Bytes8 *)&c->unk2C;
    in.unk10 = c->hp;
    in.unk12 = c->mp;
    in.unk8 = *(Words2 *)c->supportAbilities;
    in.unk14 = *(Bytes5 *)c->equip;
    chr_calcEquipStats(&in, &out);
    c->speed = out.unk8[0];
    c->strength = out.unk8[1];
    c->magic = out.unk8[2];
    c->spirit = out.unk8[3];
    c->defense = out.unkC[1];
    c->evade = out.unkC[2];
    c->magicDef = out.unkC[3];
    c->magicEva = out.unkC[4];
    c->hp = out.unk0;
    c->mp = out.unk2;
    c->maxHp = out.unk4;
    c->maxMp = out.unk6;
}
extern void (*g_fadeSeekFuncs[])(FadeState *, s32);
extern s32 D_8007BBD4;
extern u32 *D_8007BBCC;
extern s32 D_8007B94C;
void win_closeAll(void);
void msg_resetChoices(void);
extern u8 *g_evtScripts; /* script data */
/* 8-byte entry of the field script directory at g_evtScripts */
typedef struct {
    u16 unk0; /* offset from g_evtScripts */
    u16 unk2; /* size */
    u8 unk4;
    u8 unk5; /* flags */
    u8 pad6[2];
} ScriptEntry;

u8 chr_getClassFlags(u32);
void chr_clearAbilities(s32);
s32 chr_getDefaultSet(Character *);
void chr_setDefaultEquip(u8 *, s32);
typedef struct {
    u32 tag;
    u32 rgbc;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} PolyF4Half; /* POLY_F4 with the colour/code written as one word */
typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 xy1;
} LineF2W; /* LINE_F2 written as whole words */
typedef struct {
    u32 tag;
    u32 rgbc0;
    u32 xy0;
    u32 rgb1;
    u32 xy1;
    u32 rgb2;
    u32 xy2;
    u32 rgb3;
    u32 xy3;
} PolyG4Words; /* POLY_G4 written as whole words */
typedef struct {
    u32 tag;
    u32 code[2];
} DrMode2Words; /* DR_MODE / DR_TWIN */
extern Unk800689B0 D_80068DC0;
extern Unk800689B0 g_winFillSprite;
/* window frame tables (win_buildBorder) */
extern u32 D_80068FA4[2]; /* edge rgbc, by flag */
extern u32 D_80068FAC[2]; /* corner rgbc, by flag */
extern u8 D_80068FB4[][4]; /* corner sprites (g_uiSprites index) per style */
extern u8 D_80068FC0[][2]; /* top/bottom edge sprites per style */
extern u8 D_80068FC8[][2]; /* left/right edge sprites per style */
extern u8 D_80068FD0[][3]; /* title bar sprites (left, right, middle) per style */
typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy;
    u32 uvclut;
} Sprt8Words; /* SPRT_8 written as whole words */
typedef struct {
    u32 tag;
    u32 rgbc;
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    u16 clut;
} Sprt8Fields; /* SPRT_8 with rgb+code as one word */
typedef struct {
    u16 uv;
    u16 clut;
    u8 w;
    u8 h;
    u16 tpage;
} SpriteDesc16; /* Unk800686E8 with uv and clut as halves */
typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 uv0clut;
    u32 xy1;
    u32 uv1tpage;
    u32 xy2;
    u32 uv2;
    u32 xy3;
    u32 uv3;
} PolyFT4Words; /* POLY_FT4 written as whole words */

u16 win_getClut(s32, s32);
extern RECT D_80068F40;
void func_80058F34(AkaoChannel *, u32 *, u32, u32);
/* ---- main: boot, system setup, TIM helpers ---- */

/* GameState.config bits (ovl_34 rows, runtime field/menu campaign) that GameConfig can't name */
#define CONFIG_HERE_ICON_OFF (1 << 17)
#define CONFIG_WINDOW_CLASSIC (1 << 18)
#define CONFIG_TARGET_WINDOW_ALWAYS (1 << 19)
#define CONFIG_WINDOW_COLOR() ((g_gameState->config >> 18) & 1) /* the window palette (pal/abr) of all window drawing */

/* GameState.config/padMap viewed as a pair: the config menu's settings (saved by ovl_39) */
typedef struct {
    u32 soundMono : 1; /* Config rows (runtime field/menu campaign, ovl_34): Sound */
    u32 padCustom : 1; /* Controller: Custom (buttonMap is used, config_remapButton) */
    u32 cursorMemory : 1;
    u32 atbWait : 1;
    u32 battleCameraFixed : 1;
    u32 battleControlCustom : 1;
    u32 movementRun : 1;
    u32 vibration : 1;
    u32 battleSpeed : 3;
    u32 unk0_11 : 3;
    u32 messageSpeed : 3;
    u32 unk0_17 : 15; /* bit 17 HERE Icon off, 18 Window Color classic, 19 Target Window always (CONFIG_*) */
    u32 buttonMap; /* 8 x 4 bits: game button i -> pad button (GameState.padMap) */
} GameConfig;

/* GameState.unk14 viewed as its 15 directory records */
typedef struct {
    s32 unk0; /* kind; 3 = table of u16 ids */
    s32 unk4; /* entry count */
    s32 unk8;
    s32 unkC;
} DirInfo;

/* file_loadDirTables: 8-byte entries of directory lists 8 and 11 */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
} Unk8001DE50;

/* file_loadDirTables: 16-byte records copied out of directory list 11 */
typedef struct {
    u8 pad0[8];
    u16 unk8;
    u8 padA[6];
} Unk8001DE50b;

/* 8-byte file directory entry (GameState.unk18 tables) */
typedef struct {
    u16 unk0; /* id */
    u16 unk2;
    s32 unk4;
} DirEntry;

/* PreloadEntry.unk4 split into its parts */
typedef struct {
    u16 unk0;
    u8 unk2;
    u8 unk3;
} FileIdParts;

/* TIM image: header, optional CLUT block (flag bit 3), then the pixel block */
typedef struct {
    u32 bnum; /* block size in bytes, header included */
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} TimBlock;

typedef struct {
    u32 id;
    u32 flag;
    TimBlock clut;
} TimHeader;

extern u32 g_allocTop; /* bump allocator for the boot-time state block */
void game_allocState(void);
void game_resetState(void);
s32 cd_init(void);
void CdSetDebug(s32);
void _bu_init(void);
void pad_init(void);

s32 game_loadCommonAkao(void);
void game_allocData(void);
void game_resetData(void);
void func_8001E3A4(); /* (Unk8001E3A4 *); ovl_0b declares it with void * */
s32 game_loadCommonImages(void);
s32 game_loadCommonDb(void);

s32 CdGetSector(void *, s32);
s32 preload_isLoaded(PreloadEntry *);
void cd_onSectorReady(void);

extern SpuReverbAttr D_80073690;
extern s32 D_800736A4; /* reverb mode */
extern DRAWENV D_80073C88;
void pad_resetState(void);
void cd_resetState(void);
void db_init(void);
void mbg_resetState(void);
void menu_init(void);
void func_80053924(void);

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

/* first sector of FF9.IMG: directory records copied to GameState.unk14 */
typedef struct {
    u8 pad0[8];
    s32 unk8; /* directory count */
    u8 padC[4];
    DirInfo unk10[15];
} Ff9ImgHeader;


extern u32 D_80068680[8];
extern s32 D_800686A0[];
extern u32 D_800686C0[1]; /* extra button masks (parallel to D_800686C4) */
extern s32 D_800686C4[1]; /* their action indices */
void pad_setMapping(s32, u32, s32);
extern s32 g_padMapping[2][32]; /* indexed by leading-zero count (pad_setMapping) */
extern s32 g_padHoldFrames[2][32];
extern s32 g_padPortPrevHeld[];
extern s32 g_padPortReleased[];

void __main(void);
void game_loadModeOverlay(s32);
void FlushCache(void); /* BIOS FlushCache */
s32 EnterCriticalSection(void); /* BIOS EnterCriticalSection */
void ExitCriticalSection(void); /* BIOS ExitCriticalSection */
void game_quitData(void);
void game_quitState(void);
void game_shutdown(void);
void game_freezeScreen(void);

/* overlay load addresses by game mode (game_loadModeOverlay). The rodata block
 * D_80010034 also holds the next word and libgpu's version string. */
typedef struct {
    u32 *unk0[10];
} LoadAddrs;
extern LoadAddrs D_80010034;

/* battle calculation context (fieldCalc_checkAlive ... chr_addMp) */
typedef struct {
    u32 unk0; /* low nibble: kind */
    u8 pad4;
    u8 unk5; /* power */
    u8 pad6[2];
    u8 unk8; /* checked against the target by chr_cureStatusMask */
    u8 unk9;
} MainCalcCmd;

typedef struct {
    Character *unk0; /* caster */
    Character *unk4; /* target */
    MainCalcCmd *unk8;
    s32 unkC;
    u8 unk10; /* bit 0: miss */
    u8 unk11;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} MainCalc;

u8 chr_hasStatus(Character *, s32);
s32 chr_cureStatusMask(Character *, s32);
u8 fieldCalc_apply(MainCalc *);
u8 fieldCalc_checkAlive(MainCalc *);
u8 fieldCalc_checkDead(MainCalc *);
s32 menuItem_callChocobo(void); /* ovl_37 */
void exit(s32); /* BIOS B0 call */

void chr_addHp(Character *, s32);
void chr_addMp(Character *, s32);

/* 12-byte entry of a type-0x12 resource table (db_findModelInfo) */
typedef struct {
    u16 unk0; /* id */
    u16 unk2;
    u32 unk4 : 24; /* data offset from the entry */
    u32 unk7 : 8;
    u32 unk8;
} DbEntry12;

extern TimHeader D_800834D0;
extern s32 D_8007BCB0;

void text_formatInt(s32, u8 *, s32);

/* Inlined copy of text_formatNumber (number to fixed-width text). */
static inline void text_formatNumber_inline(s32 n, u8 *buf, s32 width, u8 ascii) {
    s32 base = 0;
    s32 fill;
    s32 i;
    s32 q;
    s32 d;
    u8 *end;

    if (ascii) {
        base = '0';
    }
    fill = ' ';
    if (!ascii) {
        fill = 0xF;
    }
    if (width < 0) {
        text_formatInt(n, buf, ascii);
        return;
    }
    memset(buf, fill, width);
    for (i = 0; i < width; i++) {
        d = n % 10;
        q = n / 10;
        buf[width - i - 1] = d + base;
        if (q == 0) {
            break;
        }
        n = q;
    }
    end = buf + width;
    if (!ascii) {
        *end = 0xFF;
    } else {
        *end = 0;
    }
}
extern s32 D_8006BBD4[];

extern u8 D_80077878[2][0x200]; /* fade packet buffers, one per draw buffer */

typedef void (*FadeFn)(FadeState *);

/* frame animation cells (sprAnim_draw) */
typedef struct {
    s16 unk0; /* cell count */
    s16 unk2; /* byte offset of the first cell in unk10 */
} AnimCellList;

typedef struct {
    Unk800357E0 unk0;
    u8 padC[0x10 - 0xC];
    u16 unk10; /* x scale, 4096 = 1.0 (sprAnim_drawScaledCell) */
    u16 unk12; /* y scale */
} AnimCell;

void sprAnim_drawScaledCell(s32, s32, s32, Unk80035B90 *, AnimCell *);


/* dialog window layout (msg_getLayout / evt_getMsgLayout) */
typedef struct {
    s16 unk0; /* x */
    s16 unk2; /* w */
    s16 unk4;
    u16 unk6; /* flags */
    s8 unk8;  /* y */
    u8 unk9;  /* h */
    u8 unkA;  /* anchor */
    u8 unkB;  /* bit 0: don't make it the current window */
} WinLayout;
extern s32 D_8007BB90;
s32 msg_input(MsgWindow *, s32);
void msg_drawWindowContent(Unk8004F5D4 *);
void msg_drawChoiceCursor(Window *);

WinLayout *msg_getLayout(s32, s32 *, s32 *, s32 *);
extern WinLayout g_msgLayout;
void evt_placeBalloon(Unk8004AB64 *, WinLayout *, s32, s32);
extern s32 D_8007AE5C;
extern s32 D_8007AE60;
extern s32 D_8007AE64;


s32 tween_step(WinDef *);

/* AKAO sequence header (akao_cmdPlaySeBank) */
typedef struct {
    u8 pad0[4];
    s32 unk4; /* track count */
    s32 unk8; /* id */
    u8 padC[4];
    u32 unk10[1]; /* track offsets */
} AkaoSeqHdr;



/* ---- event script interpreter (evt_execInstr) ---- */
/* Event script opcodes (first byte of an instruction run by evt_execInstr), runtime-checked ones only */
#define EVT_OP_SET_HP 0xF1 /* character, value: set HP (capped at maxHp); runtime: the Moogle's Tent */
#define EVT_OP_SET_MP 0xF2 /* character, value: set MP (capped at maxMp); runtime: the Moogle's Tent */
#define EVT_OP_GAME_OVER 0xF5 /* field game over: returns 8 -> field exit 7 -> mode 7 (runtime: patched into the Dali Moogle script) */
/* overlay entry points as called from the main exe (the overlays have their own prototypes) */
#if !defined(OVL_0A_H) && !defined(OVL_0B_H) && !defined(OVL_0C_H) && !defined(OVL_0D_H) && !defined(OVL_14_H) && !defined(OVL_15_H) && !defined(OVL_32_H) && !defined(OVL_33_H) && !defined(OVL_34_H) && !defined(OVL_36_H) && !defined(OVL_37_H) && !defined(OVL_38_H) && !defined(OVL_39_H) && !defined(OVL_3A_H) && !defined(OVL_3B_H) && !defined(OVL_3C_H) && !defined(OVL_3E_H) && !defined(OVL_64_H) && !defined(OVL_65_H)
/* mode entry points in overlays, linked through config/xrefs.us.txt */
void btl_entry(void);
void battleEnd_main(void);
void fld_spriteCommand(s32, s32, s32, s32, s32);
void fld_getSoundPanVolume(s16, s16, s16, u8 *arg4, u8 *arg5);
s32 fld_setObjectColor(s32, s32, s32, s32);
void fld_requestDiscChange(s32, s16);
void w_frameSetParameter(u32, s32); /* ovl_0c */
void fld_gameOver(void); /* ovl_0a */
s32 fldbg_addFollowOverlay(s32, s16, s16, s8, u8, u8, u8);
s32 fldbg_setCamera(s32);
s32 fldbg_moveOrigin(s16, s16, s16);
s32 fldbg_scrollTo(s16, s16, u16, s32);
s32 fldbg_getScroll(u16 *arg0, u16 *arg1);
s32 fldbg_setFollow(s32, s32, s32);
s32 fldbg_setFollowHeight(s16);
s32 fldbg_disableScrollLimits(void);
s32 fldbg_enableScrollLimits(void);
s32 fldbg_setViewport(s32, s16, s16, s16, s16);
s32 fldbg_setViewportScrollRate(s32, s32, s32);
s32 fldbg_moveOverlay(s32, s16, s16, s16);
s32 fldbg_setOverlayColor(s32, u8, u8, u8);
s32 fldbg_showOverlay(s32, s32);
s32 fldbg_setOverlayLoop(s32, s32, s32, s32);
s32 fldbg_setOverlayFlag1(s32, s32);
s32 fldbg_setOverlaySkewLoop(s32, s32, s32, s32, s32);
s32 fldbg_setOverlayParallax(s32, s32, s32, s32);
s32 fldbg_setOverlayPos(s32, s32, s32);
s32 fldbg_setOverlayViewport(s32, s32);
s32 fldbg_setAnimFrame(s32, s32);
s32 fldbg_showAnimFrame(s32, s32);
s32 fldbg_playAnim(s32, s32);
s32 fldbg_setAnimSpeed(s32, s32);
s32 fldbg_setAnimStepDelay(s32, s32, s32);
s32 fldbg_setAnimFlags(s32, s32);
s32 fldbg_playAnimRange(s32, u32, u32);
s32 fldbg_resetAnim(s32, s32);
s32 fldbg_setScrollLimits(s32, s16, s16, s16, s16);
s32 bgi_setFloorActive(s32, s32);
s32 bgi_showAnimFrame(u32, u32);
s32 bgi_setMaskFilter(u8);
s32 bgi_setTriangleActive(s32, s32);
s32 bgi_setAnimFrame(u32, u32);
s32 bgi_setAnimPlaying(u32, s32);
s32 bgi_setAnimRate(u32, s16);
s32 bgi_setAnimFrameValues(u32, s32);
s32 bgi_setAnimFrameValue(u32, u32, s32);
s32 bgi_setAnimMode(u32, s32);
s32 bgi_resetAnim(u32, s32);
s32 bgi_startFloorMove(s32, s32);
s32 bgi_stopFloorMove(s32, s32);
s32 bgi_setFloorMovePlaying(s32, s32);
s32 bgi_setFloorMoveMode(s32, s32);
s32 bgi_setFloorMoveFloor(s32, s32);
s32 bgi_setFloorMoveSpeed(s32, s16);
s32 bgi_setFloorMoveCurve(s32, s32);
s32 bgi_setFloorMoveRange(s32, s16, s16);
s32 bgi_setFloorMoveAxis(s32, s32);
void btl_setBanner(s32, s32);
void rewards_requestClose(void);
s32 fld_queueModelBundle(s32); /* uid; queues the actor's model */
s32 bgi_setActorActive(s32, s32);
s32 bgi_setActorRadius(s32, u16);
void btl_setEnemyCmd(s32, s32, s32, s32);
void btlseq_startDirect(s32, s32, u16);
void btl_setBattleVar(s32, s32);
s32 fld_modelCommand(s32, s32, s32, s32, s32);
s32 fld_movieCommand(s32, s32, s32);
VECTOR *wm_getCameraEye(void);
VECTOR *wm_getCameraTarget(void);
void bgi_playAnimRange(s32, s32, s32);
#endif

extern u8 *g_evtInstrStart;
extern s32 D_8007AE04;
Unk8004AB64 *evt_getObjectArg(void);
void evt_returnFromFunc(Unk8004AB64 *);
s32 evt_modelOpField(Unk8004AB64 *, s32);
s32 evt_modelOpBattle(Unk8004AB64 *, s32);
s32 evt_modelOpWorld(Unk8004AB64 *, s32);

extern s32 D_8007AEA4;
extern Unk8004AB64 *D_8007AE68;
extern s32 D_8007AE6C;
extern s32 D_8007BBD8;
extern s32 D_8007BB7C; /* message counter */
extern u8 D_8006E688[]; /* text speed per config setting */
void evt_turnObject(Unk8004AB64 *, s32, s32, s32);
s32 func_80052F24(s32, s32);
void msg_setNumber(u32, s32);
void msg_setChoiceMask(s32, s32);
void msg_keepWindowsOnInit(void);
WinLayout *evt_getMsgLayout(Unk8004AB64 *, s32, s32);
void msg_open(s32, s32, WinLayout *, s32);

static inline s32 evt_getArg16_inline(void) {
    s32 r;
    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        r = evt_popResult();
    } else {
        Unk8004AB64 *pp = g_evtCurObj;
        r = *pp->unk0++;
        r |= *pp->unk0++ << 8;
        r = (s16)r;
    }
    g_evtArgFlags >>= 1;
    g_evtArgRead = 1;
    return r;
}

/* uid of the object the current script runs for */
#define CUR_UID() (((u8 *)((s32 *)g_evtCurObj + g_evtCurObj->unk9) + g_evtCurObj->unkA * 4)[-1])

static inline s32 evt_getArg8_inline(void) {
    s32 r;
    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        r = evt_popResult();
    } else {
        r = *g_evtCurObj->unk0++;
    }
    g_evtArgFlags >>= 1;
    g_evtArgRead = 1;
    return r;
}

extern s32 D_8007B710;
extern s32 D_8007AE24;
extern s32 D_8007AE28;
void msg_close(s32);
void msg_resetWindows(void);
void win_closeAll(void);
void func_8004ED3C(void);
void *msg_getText(s32);
s32 evt_moveObjectTo(s32, s32, s32, s32);
void func_8004B9B8(Unk8004B9B8 *);

/* Inlined helper: start the animation set up in unk60/unk61/unk62. */
static inline void evt_execInstr_anim(Unk8004AB64 *p, s32 anim) {
    if ((p->unk62 & 0x80) && (p->unkF & 0x80)) {
        func_8004B9B8((Unk8004B9B8 *)p);
    }
    p->unk36 = anim;
    p->unk3A = p->unk60;
    p->unk62 &= ~0x46;
    p->unk62 |= (p->unk60 > p->unk61) ? 0xC0 : 0x80;
    p->unk64 = 0;
    p->unk38 = evt_getAnimFrameCount(p->unk34, anim);
    p->unk5D = p->unk69;
}

/* event region (quad) object, same header as Unk8004AB64 */
typedef struct {
    s16 unk0; /* x */
    s16 unk2; /* z */
} Unk80039C38Pt;
typedef struct {
    u8 pad0[0x14];
    s32 unk14; /* vertex count */
    Unk80039C38Pt unk18[8];
} Unk80039C38Quad;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0xC - 5];
    s32 unkC;
    u8 pad10[0x12 - 0x10];
    s16 unk12;
} Unk8004AFF0Sub; /* 0x14 */

/* model object (Unk8E0.unk8) */
typedef struct Unk8004AFF0 {
    u16 unk0; /* flags */
    u8 pad2[4 - 2];
    u8 unk4;
    u8 pad5[0x14 - 5];
    u32 unk14;
    u8 pad18[0x1C - 0x18];
    struct Unk8004AFF0 *unk1C;
    u8 pad20[0x50 - 0x20];
    struct { Unk8004AFF0Sub unk0[1]; } *unk50;
    s16 unk54;
    s16 unk56;
    s16 unk58;
    u8 pad5A[0x74 - 0x5A];
    s16 unk74;
    s16 unk76;
    s16 unk78;
} Unk8004AFF0;

/* Inlined copy of func_8004AFF0. */
static inline Unk8004AFF0 *func_8004AFF0_inline(Unk8004AB64 *p) {
    Unk8E0 *q;

    if (p != NULL) {
        if (p->unk34 == 0xFFFF) {
            return NULL;
        }
        q = (Unk8E0 *)game_getModel(p->unk3B);
        if (q != NULL) {
            return (Unk8004AFF0 *)q->unk8;
        }
    }
    return NULL;
}

extern u8 D_8007BD2C;
extern s32 g_evtEncounterChance;
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 unk8;
} Unk8007AE40;
extern Unk8007AE40 g_evtEncounterTable;
extern u8 D_8006E658[];
void evt_setNextBattle(u16);
void func_80021680();
void msg_startHereIcon(s32);
void func_80052D0C(u8 **);
Unk16D8 *evt_deleteObject(Unk8004AB64 *);
Unk8004AB64 *evt_allocObject(s32);
void evt_onAnimEnd(Unk8004B9B8 *);

extern s32 D_8007AE2C;
extern s32 D_8007AE30;
extern s32 D_8007AE38;
extern s32 D_8007AE3C;
void msg_showPlayerBubble(s32);
void msg_setAtePrompt(s32);
void evt_onMenuOpen(void);
void keyItem_add(s32);
void keyItem_remove(s32);
s32 card_add(s32);
void chr_clearRecruited(u32);

/* Inlined copy of evt_getArg24 (24-bit script argument). */
static inline s32 evt_getArg24_inline(void) {
    s32 v;
    Unk8004AB64 *pp;

    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        v = evt_popResult();
    } else {
        pp = g_evtCurObj;
        v = *pp->unk0++;
        v |= *pp->unk0++ << 8;
        v |= *pp->unk0++ << 16;
    }
    g_evtArgFlags >>= (g_evtArgRead = 1);
    return v;
}

extern u8 D_8006E630[];
extern u8 D_8006E63C[];
s32 msg_getPartyMemberId(s32);
void chr_join(s32, u8, s32);
s32 chr_resetAp(s32, s32);
s32 chr_learnAbility(s32, s32);
s32 chr_cureStatusBit(Unk800539FC *, s32);
void timer_setSeconds(s32);
void timer_enable(u8);
void timer_setRunning(u8);
void evt_projectToScreen(SVECTOR *, s32 *, s32 *);
s32 vib_play(s32);
s32 vib_setPlaying(s32);
s32 vib_selectTrack(s32, s32, s32);
s32 vib_setTrackEnabled(s32, s32, s32);
s32 vib_setRate(s16);
s32 vib_setLoopMode(s32);
s32 vib_setRange(s32, s32);
extern u8 D_8007AE90[];
extern u8 *D_8007AE8C;

typedef struct {
    s32 unk0;
    u8 unk4[4]; /* party character ids */
    u8 unk8[8];
    u8 unk10[9];
} Unk8007AE70;
extern Unk8007AE70 D_8007AE70;

/* Inlined helper: rebuild g_evtState->unk16E4 from the current party. */
static inline void evt_execInstr_party(void) {
    s32 mask = 0;
    s32 i = 0;
    u8 *p = g_evtState->unk16E4;
    s32 r;

    *(s32 *)g_evtState->unk16E4 = -1;
    for (; i < 4; i++) {
        r = msg_getPartyMemberId(i);
        if (r >= 0) {
            mask |= 1 << D_8006E63C[r];
        }
    }
    for (i = 0; mask != 0; i++) {
        if (mask & 1) {
            *p++ = g_evtScriptCount - 9 + D_8006E630[i];
        }
        mask >>= 1;
    }
}

static inline void func_8004B3A8_inline(s32 a, s32 b) {
    evt_createObject(a, b, 0x14, 0x10)->unk5 = 2;
}

static inline void func_8004B3D0_inline(s32 a, s32 b) {
    Unk800367D0 *p = evt_createObject(a, b, 0x38, 0x10);
    p->unk5 = 3;
    p->unkF = 1;
}

static inline void evt_execInstr_newactor(s32 uid, s32 b) {
    Unk8004AB64 *p;

    p = (Unk8004AB64 *)evt_createObject(uid, b, 0xAC, 0x10);
    p->unk3C = 0x10;
    p->unk3D = 0x10;
    p->unk34 = 0xFFFF;
    p->unk3E = 0xFF;
    p->unk2F = 0x40;
    p->unk5 = 4;
    p->unk69 = 0x10;
    p->unk5C = 0x1E;
    p->unk5D = 0x10;
    p->unk61 = 0xFF;
    p->unk5E = 0x1F;
    p->unk5F = 0x10;
    p->unk6E = 4;
    p->unk71 = 0x50;
    p->unk72 = 4;
    p->unk73 = 0x10;
    p->unk93 = 0xFF;
    p->unk7C.w = 0x10101010;
    p->unkF |= 4;
    switch (g_evtMode) {
    case 1:
        fld_queueModelBundle(uid);
        break;
    case 2:
        g_evtUnitObjects[D_8007AE04++] = p;
        break;
    }
}

/* Inlined helper: give the window opened by obj the focus (b = window flags). */
static inline void evt_execInstr_window(Unk8004AB64 *obj, s32 b, s32 c) {
    Unk16D8 *n;
    Unk8004AB64 *o;

    if ((b & 0xA0) == 0x80 && obj->unk5 == 4) {
        for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
            o = n->unk4;
            if (o->unk5 == 4 && o->unk3E == c) {
                o->unk3E = 0xFF;
            }
        }
        obj->unk3E = c;
        D_8007AE68 = obj;
        D_8007AE6C = 0;
    }
}

/* Inlined helper: script character id -> GameData.unk3C index. */
static inline s32 evt_execInstr_chr(s32 id) {
    if (id >= 9) {
        id -= 4;
    }
    return id;
}



/* Inlined helpers for the pad binding setup (config_applyPadMapping): GameState.unk24 bit 1
 * enables the custom button map in unk28 (4 bits per action). */
static inline s32 config_applyPadMapping_custom(u32 *c) {
    return (c[0] >> 1) & 1;
}
static inline s32 config_applyPadMapping_remap(s32 k) {
    u32 *c = &g_gameState->config;
    if (config_applyPadMapping_custom(c) == 1) {
        return (c[1] >> (k * 4)) & 0xF;
    }
    return k;
}


/* Inlined max helper of vib_init (keeps the running maximum in memory). */
static inline void vib_init_max(u32 *m, u32 v) {
    if (*m < v) {
        *m = v;
    }
}

/* primitive buffer / ordering table used by swirl_drawFlash */
extern u32 *g_swirlOt;
extern u32 *g_swirlPkt;
extern u16 D_8007BD1E;
/* battle-entry warp grid (swirl_buildMesh fills it, swirl_drawMesh draws it) */
typedef struct {
    s16 x, y;
    s16 pad[2];
} WarpVert; /* 9x9 screen vertices */
extern DVECTOR *g_swirlColumns; /* 9 texture columns/rows */
extern WarpVert *g_swirlMesh;
/* battle-entry swirl (swirl_update) */
typedef struct {
    s16 unk0; /* rotation */
    s16 unk2; /* rotation speed */
    s16 unk4; /* scale */
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
} SwirlDef; /* 0x14 */
extern SwirlDef D_8006F2E0[2];
extern SwirlDef *g_swirlParams;
extern u16 D_8007BD1C;
extern s16 D_8007BD20;
extern s16 D_8007BD22 __asm__("D_8007BD20+2"); /* no symbol of its own */
extern s16 D_8007BD24;
extern s32 g_swirlFrame; /* frame */
#if !defined(OVL_0C_H) /* ovl_0c declares it as s32 */
#if !defined(OVL_0A_H) && !defined(OVL_0C_H) /* the overlays type it themselves */
extern u32 *D_8007BD30;
#endif
#endif
void fade_clear(void);
s32 math_sin(s32); /* sin */
void swirl_drawFlash(s32, s32);
u32 *swirl_buildMesh(u32 *, s32, s32);
void swirl_drawMesh(void);
#if !defined(OVL_0B_H) /* ovl_0b declares it with its DR_MOVE */
void SetDrawMove(void *p, RECT *rect, int x, int y);
#endif
extern u8 D_8006F308[];
extern u8 D_8006F310[];

/* sound stream header (g_akaoStream.unk2C + 0x80), func_80061C1C */
typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u32 unk14;
    s32 unk18;
    u16 unk1C;
    u8 pad1E[0x20 - 0x1E];
    s32 unk20;
    u8 pad24[0x28 - 0x24];
    s32 unk28;
} StreamHeader;

void akao_sendCmdE5(s32 a, s32 b);
void func_80061EE4(void);

/* AKAO sample stream state, akao_uploadBank */
typedef struct {
    Unk8005EBB4 *unk0; /* destination of the next sample-info copy */
    u32 unk4;          /* SPU transfer address */
    u32 unk8;          /* sample bytes left */
    u32 unkC;          /* sample-info bytes left */
} Unk80083228;
extern Unk80083228 g_akaoUpload;
/* header of an AKAO instrument bank (akao_loadBankAt, akao_uploadBank); instrument entries follow, then the samples */
#define AKAO_MAGIC 0x4F414B41 /* "AKAO" */
typedef struct {
    s32 magic; /* AKAO_MAGIC */
    s32 id; /* bank id (D_800809C0 keeps the loaded ones) */
    s32 unk8;
    s32 unkC;
    s32 spuAddr; /* where the samples go in SPU RAM */
    s32 sampleSize;
    s32 slot; /* first g_akaoInstruments entry */
    s32 instCount; /* 16-byte instrument entries after the header */
    s32 unk20[8];
} AkaoBankHeader; /* 0x40 */
extern AkaoBankHeader g_akaoBankHeader; /* AKAO header copy */
extern u32 D_80083230; /* = g_akaoUpload.unk8 */

/* ovl_0a functions called from the main script interpreter */
s32 bgi_getActorTriangle(s32, u16 *, u16 *);
s32 bgi_getAnimFrame(u32, s32 *);

/* party member index -> character slot (9.. map to 5..) */
static inline s32 evt_exprSpecial_chr(s32 i) {
    if (i >= 9) {
        i -= 4;
    }
    return i;
}

/* evt_getSysVar (script system variables) */
extern s32 D_8007AE20;
void *evt_findNearestObject(Unk8004AB64 *, s32, s32 *); /* nearest object of a kind */
extern s32 D_8007AEC4;
s32 fldbg_getCamera(u8 *);
s32 btlseq_isBusy(void);
s32 w_frameGetParameter(s32); /* ovl_0c */
s32 btl_getBattleVar(s32);
u32 game_getPlayTimeSeconds(void);
s32 timer_getSeconds(void);
s32 func_8006275C(void);
s32 msg_getChoiceIndex(void);

/* random encounter check (evt_rollEncounter). ovl_0c declares wm_getEncounterEntry with its own
 * table type, so these are hidden there. */
#ifndef OVL_0C_H
extern s32 g_evtEncounterDist;
extern u8 g_encounterSlotThresholds[][4];
Unk8007AE40 *wm_getEncounterEntry();

static inline s32 evt_rollEncounter_rand(void) {
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    return *g_randPtr++;
}

static inline u16 evt_rollEncounter_pick(void) {
    Unk8007AE40 *t;
    s32 r;
    s32 i;
    u8 *tbl;

    if (g_evtMode != 1) {
        t = wm_getEncounterEntry();
    } else {
        t = &g_evtEncounterTable;
    }
    r = evt_rollEncounter_rand();
    tbl = g_encounterSlotThresholds[t->unk8 & 3];
    if (r >= tbl[0]) {
        if (r >= tbl[1]) {
            i = 6;
            if (r < tbl[2]) {
                i = 4;
            }
        } else {
            i = 2;
        }
    } else {
        i = 0;
    }
    return *(u16 *)((u8 *)t + i); /* i is a byte offset into unk0..unk6 */
}
#endif

/* start an object's idle animation (evt_updateIdleTimer) */
static inline void evt_updateIdleTimer_idle(Unk8004AB64 *q) {
    q->unk60 = 0;
    q->unk61 = 0xFF;
    evt_execInstr_anim(q, q->unk74);
}

/* party slot key for func_8004B85C: slots 9+ map down by 4 */
static inline s32 func_8004B85C_key(s32 slot) {
    s32 key = slot;
    if (slot >= 9) {
        key = slot - 4;
    }
    return key;
}

/* is the party member with this key present, kind < 14 for slot < 8, kind >= 14 otherwise */
static inline s32 func_8004B85C_find(s32 key, s32 slot) {
    Character **p;
    Character *c;
    s32 kind;
    s32 ge;
    s32 lt;

    for (p = g_gameState->unk1C->party; p < &g_gameState->unk1C->party[4]; p++) {
        c = *p;
        if (c == NULL) {
            continue;
        }
        if (c->unk34.b.id != key) {
            continue;
        }
        kind = c->unk34.b.unk4;
        lt = kind < 14;
        if (kind) {
            ge = lt ^ 1;
        } else {
            ge = lt ^ 1;
        }
        if (slot >= 8) {
            if (lt) {
                continue;
            }
        } else {
            if (ge) {
                continue;
            }
        }
        break;
    }
    return p < &g_gameState->unk1C->party[4];
}

extern const u8 D_80010DC4[]; /* default character names: count, then {offset, length} entries */
extern const u8 D_80011EFC[]; /* "ACTIVE TIME EVENT" window title */
extern const u8 D_80011F10[]; /* "MOGNET" window title */

/* sound driver (AKAO) routines used by func_800589F0 */
void akao_setReverbVoices(u32 x);
void akao_setNoiseVoices(u32 x);
void akao_setPitchModVoices(u32 x);
void akao_flushVoice(s32 v, AkaoVoiceRegs *p);
void func_80058118(AkaoChannel *c, u32 bit);
void func_800585E0(AkaoChannel *ch, u32 mask, u32 keep, u32 *used);
void func_800588B0(u32 extra);
void func_80058990(AkaoChannel *c);
void func_800589E0(Unk800589E0 *a);
void func_80059140(u32 *out, u32 m1, u32 m2, u32 extra);
extern s32 D_80080A74;

#endif
