#ifndef FF9_H
#define FF9_H

#include "common.h"
#include "psyq.h"

/* Layouts are partial: only offsets seen in decompiled code are named.
 * Names are placeholders (unkXX = offset) until their purpose is known.
 * Split into per-subsystem headers once subsystems are identified. */

/* ---- game state (D_8006794C) ---- */

typedef struct {
    u16 unk0; /* flags */
    u16 unk2;
    s32 unk4; /* id, -1 = none */
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
} Unk80021938; /* element passed to func_80021938 */

typedef struct Unk8C8 {
    u8 unk0;
    u8 unk1; /* state */
    s8 unk2; /* count */
    s8 unk3;
    Unk80021938 unk4[7];
    s32 (*unk11C)(struct Unk8C8 *); /* hook, nonzero = handled */
} Unk8C8;
Unk80021938 *func_80021B34(Unk80021938 *, Unk80021938 *);
s32 func_80021ED4(void);

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
        u32 row : 5;   /* ability table row (D_8006CB28) */
        u32 unk11 : 15;
    } b;
} CharBits;

/* Character record: 9 of them at GameData+0x3C; GameData.unk54C[] points at
 * the party members. */
typedef struct {
    u8 unk0[0xA]; /* name: 0xFF-terminated text, read up to 7 chars */
    u8 unkA;      /* bit 4 tested */
    u8 unkB;      /* level (exp table has 99 entries) */
    u32 unkC;     /* exp, capped at 9999999 */
    u16 unk10;    /* hp */
    s16 unk12;    /* mp */
    u8 pad14[0x16 - 0x14];
    u8 unk16;
    u8 unk17;
    u16 unk18;    /* max hp */
    s16 unk1A;    /* max mp */
    u8 pad1C[0x1F - 0x1C];
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 pad22[0x24 - 0x22];
    u8 unk24; /* base stats, shown by the status menu (ovl_38) */
    u8 unk25;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u16 unk2C;
    u16 unk2E;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    CharBits unk34;
    u8 unk38;    /* status; 0x45 = dead/stone/... (can't gain exp) */
    u8 unk39[5]; /* equipment item ids (0xFF = none) */
    u16 unk3E[5]; /* stat bonuses from equipment */
    u8 pad48[0x58 - 0x48];
    u8 unk58[0x30]; /* learned AP per ability slot; cleared by func_800343C0 */
    u32 unk88[2]; /* support abilities equipped: bit (id - 0xC0) */
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
    s16 unk4; /* set by func_8001E2AC */
    u8 unk6;
    u8 pad7[0x10 - 7];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    u8 *unk1C; /* load address */
    u8 pad20[4];
    s16 unk24; /* set by func_8001E2AC */
    u8 pad26[2];
    struct Unk11C0Slot *unk28; /* per-object model slots, indexed by unk3B */
} Unk11C0;

/* Unk11C0.unk28 entry (0x28 bytes), written by script opcodes in func_80050410 */
typedef struct Unk11C0Slot {
    u32 unk0_0 : 8;
    u32 unk0_8 : 12;
    u32 unk0_20 : 12;
    u32 unk4_0 : 12;
    u32 unk4_12 : 10;
    u32 unk4_22 : 10;
    u8 pad8[0x28 - 0x8];
} Unk11C0Slot;

typedef struct {
    u8 pad0[0xA];
    s16 unkA; /* set by func_8001E2AC */
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

/* GameData.unk574: item inventory, copied as a block by the save code (ovl_39) */
typedef struct {
    UnkPair unk0[0x100]; /* {id, count} */
} __attribute__((aligned(4))) ItemInventory; /* block copies are word-aligned */

/* GameData.unk774: 2 bits per item id */
typedef struct {
    u8 unk0[0x40];
} __attribute__((aligned(4))) ItemFlags;

/* D_8006794C->unk1C: looks like the save-data block */
typedef struct {
    u32 unk0; /* flags */
    u32 unk4; /* allocator top (D_80067940) after setup */
    u8 unk8; /* draw buffer index (0/1) */
    u8 unk9;
    u16 unkA; /* projection distance (SetFogNearFar) */
    u16 unkC;
    u16 unkE;
    u32 *unk10[2]; /* ordering tables, 0x1000 entries */
    MATRIX unk18;
    u8 unk38;
    u8 unk39;
    u16 unk3A;
    Character unk3C[9];
    Character *unk54C[4]; /* party */
    u16 unk55C;
    u16 unk55E;
    u32 unk560; /* gil, capped at 9999999 */
    u8 pad564[0x568 - 0x564];
    s16 unk568;
    s16 unk56A;
    s16 unk56C;
    u8 unk56E;
    u8 unk56F;
    u8 unk570;
    u8 unk571;
    u8 pad572[2];
    ItemInventory unk574;
    ItemFlags unk774;
    u8 pad7B4[0x7D4 - 0x7B4];
    Unk7D4 *unk7D4;
    void *unk7D8;
    s16 unk7DC;
    u16 unk7DE;
    s32 unk7E0;
    struct FileTable *unk7E4; /* -1 = none */
    u16 unk7E8;
    u16 unk7EA;
    s32 unk7EC[2];
    s32 unk7F4[2];
    s32 unk7FC[2]; /* CD sector, per area */
    s32 unk804[2]; /* sector count */
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
    void (*unk83C)(s32, s32, s32, s32, s32);
    u16 unk840;
    u16 unk842;
    s32 unk844; /* selected chocograph (1-based, 0 = none) */
    s8 unk848;
    u8 unk849[1];
    u16 unk84A;
    Unk7D4 *unk84C; /* second (kanji) font table */
    u8 unk850;
    u8 unk851;
    u16 unk852;
    CdReadCtx unk854[1]; /* CD read context (func_8002177C) */
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
    Unk8C8 *unk8C8;
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
    u32 unk4; /* allocator top after boot (D_80067940) */
    u8 unk8;
    u8 unk9; /* VSync mode */
    u8 unkA;
    u8 unkB;          /* disc / pad connection bits, 0xFF = none */
    DISPENV *unkC;    /* [2] */
    DRAWENV *unk10;   /* [2] */
    GameStateUnk14 *unk14;
    void **unk18; /* file tables, indexed like unk14 */
    GameData *unk1C;
    s32 unk20; /* base CD sector added to file start sectors */
    u32 unk24;
    u32 unk28; /* 4-bit fields */
    u32 unk2C; /* frame counter? divided by 60 */
    s32 unk30;
    u8 unk34[15];
} GameState;

extern GameState *D_8006794C;
void func_8001DE50(); /* func_8001CB08 calls it without arguments */

/* Inlined copy of func_80034A04 (support ability bit test). */
static inline u8 func_80034A04_inline(u32 *bits, s32 b) {
    b -= 0xC0;
    return (bits[b >> 5] & (1 << (b & 0x1F))) != 0;
}

/* Inlined copy of func_80027EDC (item search); the original source inlines it. */
static inline UnkPair *func_80027EDC_inline(s32 id) {
    s32 i;
    UnkPair *e = D_8006794C->unk1C->unk574.unk0;

    for (i = 0; i < 0x100; i++, e++) {
        if (e->unk1 != 0 && e->unk0 == id) {
            return e;
        }
    }
    return NULL;
}

/* ---- timers (VSync based) ---- */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s32 unk8;
    s32 unkC;
} UnkTimer;

typedef struct {
    s32 unk0; /* VSync(-1) at last update */
    u8 unk4[0x400];
    u8 *unk404; /* write pointer into unk4 */
    UnkTimer unk408;
} Unk80076B60;
extern Unk80076B60 D_80076B60;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Unk80076F68;
extern Unk80076F68 D_80076F68;

/* ---- CD / streaming state near libcd ---- */

/* CD request, queued by func_800230B8 (0x24 bytes) */
/* one contiguous read: byte count and destination */
typedef struct {
    u32 unk0; /* size */
    u8 *unk4; /* destination */
} CdChunk;

typedef struct {
    u8 unk0; /* command; bit 2: run unk18 directly when idle */
    u8 pad1[3];
    s32 unk4;
    s32 unk8;
    CdChunk *unkC; /* chunk list; -1 = use unk1C */
    void (*unk10)(u8, u8 *);
    void (*unk14)(u8, u8 *); /* data-ready handler */
    void (*unk18)(); /* callback */
    CdChunk unk1C; /* single chunk */
} CdRequest;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4; /* sectors left */
    CdlLOC unk8; /* current position */
    s32 unkC;
    u32 unk10; /* offset in current chunk */
    s32 unk14; /* current chunk */
    s32 unk18;
    u8 pad1C[0x944 - 0x1C];
    CdRequest unk944; /* current request */
    CdRequest unk968; /* passed to func_8002267C */
} Unk800761D0;
extern Unk800761D0 D_800761D0;

/* ---- drawing ---- */

extern u32 *D_8007716C; /* ordering table */
extern u8 *D_80077170;  /* next free primitive in the packet buffer */
extern s16 D_80068F70[];
extern u8 D_8006BB8C[];

typedef struct {
    u16 unk0; /* name offset into D_8006A218 */
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
extern Unk80069018 D_80069018[];
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
extern WeaponEntry D_8006E0E8[];
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

extern u16 D_8006BBA0[];

/* sprite descriptor: uv/clut word, size, tpage (D_800686E8) */
typedef struct {
    u32 unk0; /* uv0 | clut << 16 */
    u8 unk4;  /* w */
    u8 unk5;  /* h */
    u16 unk6; /* tpage */
} Unk800686E8;
extern Unk800686E8 D_800686E8;
extern Unk800686E8 D_800686D0[];
extern Unk800686E8 D_800687E0[]; /* digit glyphs */

/* 12-tick toggle timer (func_80028CF8/D08/D2C) */
typedef struct {
    u8 unk0; /* active */
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4; /* ticks */
    void (*unk8)(void);
} UnkBlink;

/* ---- misc globals ---- */

extern UnkPair D_800739B0[2];

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2; /* memory card slot */
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 pad8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u32 unkC; /* bits 0-13: id, bits 14-15: disc */
    s32 unk10; /* frame counter */
    s32 unk14; /* saved VSync mode */
    s32 unk18;
    s32 unk1C;
    u8 unk20; /* saved background colour */
    u8 unk21;
    u8 unk22;
    u8 pad23;
    s32 unk24;
} Unk80077DB0;
extern Unk80077DB0 D_80077DB0;

extern s32 D_80077140;
extern s32 D_80077144;
extern void (*D_80068F80[])(RECT *, s32);
extern void (*D_8006BBEC[])(s32);
extern s32 D_8006BBFC[];
extern s32 D_8006BC18[];
extern s32 D_800770AC;
extern u8 *D_800770B0[]; /* menu strings (func_8002BE44) */
extern s32 D_800770F0[];
extern u8 D_80077130[];
extern s32 D_80077150;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    s32 unk4; /* free bytes in the packet buffer */
    s32 unk8;
    u8 *unkC[2];   /* packet buffers */
    u32 *unk14[2]; /* ordering tables */
    DVECTOR unk1C[2]; /* display x/y per buffer */
} Unk80077148;
extern Unk80077148 D_80077148;

/* window frame insets per style (rodata, func_8002D9B4) */
typedef struct {
    s8 unk0[5][4];
} Unk80010CE0;
extern Unk80010CE0 D_80010CE0;

/* ---- functions (main exe) ---- */

s32 func_8001C784(void *);
void func_8001D898(void *, s32, s32); /* memset */
void func_800213A0(void (*)());
void func_80021938(Unk80021938 *);
void func_80023580(u8, u8 *);
void func_800235A0(u8, u8 *);
void func_8002423C(s32, s32, s32, Character *);
u8 *func_80024EB4(RECT *, u32 *, u8, u8, s32, u8 *);
u8 *func_800253D4(RECT *, u32 *, u8, u8, s32, s32, u8 *);
void func_80026BDC(RECT *, s32, u8, u8, s32);
void func_80027568(); /* (s16 x, s16 y, s32 otz, s32 n, u8 flag), K&R */
void func_800275A4(); /* (s16 x, s16 y, s32 otz, s32 n, u8 flag), K&R */
void func_800265B0(s32, s32, s32, s32, s32, s32);
void func_800274CC(RECT *, s32);
void func_80026900(s32, s32, s32, s32);
void func_8002642C(s16, s16, s32, s32, u8, u8);
u8 *func_80026C7C(s32, s32, u32 *, s32, u8, u8 *);
void func_80026F8C(s32, s32, s32, s32);
void func_80027A4C(s32, s32, s32, s32);
u8 *func_80027170(s32, s32, u32 *, s32, s32, u8, u8, u8 *);
s32 func_800275DC(s32, s32, s32, s32, u8);
void func_80028AF0(s32, s32, s32, s32, u8);

void func_8002BAE4(s32, u8 *, s32, u8);
void func_80027D18(void);
void func_80027D4C(void);
s32 func_80027D84(s32, s32);
void func_80028204(s32, s32, s32, s32, s32, u8);
extern s32 D_80076F7C; /* number of portraits (func_80028204) */
void func_800283E0(s32, s32, s32, s32, s32, u8);
void func_80028A40(s32, RECT *, RECT *, RECT *);
void func_80028BE8(u8 *, s32);
s32 func_80028DB0(s32, s32, s32);
u8 *func_80029B4C(s32, s32, u32 *, s32, s32, u8 *);
u8 *func_80029D60(s32, s32, u32 *, u8 *, s32, u8 *);
u8 *func_8002B644(s32, s32, u32 *, u8 *, s32, void *);
void func_8002B5B4(s32, s32, s32, u8 *, s32);
u8 *func_8002920C(s32, s32, u32 *, u8 *, s32, u8 *);
void func_8002AF70(s16 *, s32);
s32 func_80023938(); /* takes an index; func_8002BE10 calls it without one */
void func_8002C9B0(void);
void func_8002E180(void);

/* ---- 0x54-byte objects, 10 at D_80077530 (func_8002E260..8002E3F0) ---- */

typedef struct {
    s16 unk0; /* item count */
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5; /* scroll top */
    u8 unk6; /* cursor row */
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u8 unkA; /* row height */
    u8 unkB; /* flags */
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 padF;
} MenuList;
typedef MenuList Unk16; /* older name */

typedef struct {
    u8 unk0[4];
} Unk4; /* 4 bytes, byte-aligned (copied with lwl/lwr) */

/* Menu window object (func_8002DC54 returns one of these). */
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
extern Window D_80077530[10];

void func_8002CF20(Window *, s32);
void func_8002D1AC(Window *, s32);
void func_8002D398(Window *, s32, s32);

/* ---- misc globals (batch 1) ---- */

extern u8 D_80068268[];
extern s8 D_80071E32;
extern s32 D_80077DD4;
extern u8 D_80077DBA;
extern u8 D_80077DBB;
extern u8 D_80077DB2;
extern s32 D_800704C8;
extern s32 D_8007041C;
extern u8 D_80077DE0;
extern struct Unk8007B9C0 *D_8007BB78;
extern s32 D_800834C8;
extern s32 D_8006798C;
extern u16 D_80073720;
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
    s16 unkBA;
    s16 unkBC;
    u16 unkBE;
    u16 unkC0;
    s16 unkC2;
    u16 unkC4;
    u16 unkC6;
    s16 unkC8;
    s16 unkCA;
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
    s16 unkF6;
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
    u32 unk118; /* voice number */
    u32 unk11C;
    s32 unk120;
    s32 unk124;
    u16 unk128; /* pitch */
    u16 unk12A;
    u16 unk12C;
    s16 unk12E;
    s16 unk130; /* volume left */
    s16 unk132; /* volume right */
} AkaoChannel; /* 0x134 */

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

/* frame animation (func_80035B90) */
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


typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 pad6[0x8 - 6];
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 pad14[0x18 - 0x14];
    u8 *unk18; /* packet buffer cursor */
    s32 unk1C;
    Unk4 unk20;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 pad27;
    Unk4 unk28;
} Unk80077C78;
extern Unk80077C78 D_80077C78;

/* menu cursor / help window (layout merged from main + ovl_14 + ovl_32) */
typedef struct {
    s16 unk0; /* x */
    s16 unk2; /* y */
    s32 unk4;
    u8 *unk8; /* help text */
    void (*unkC)(void);
    RECT unk10;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
    Unk80035B90 unk20;
} Unk80077DE8;
extern Unk80077DE8 D_80077DE8;

/* sound sequencer state (AKAO), D_80080A10 */
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
extern Unk80080A10 *D_80080A10;
extern Unk80080A10 D_8007F858; /* main sequencer state */
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
extern s32 D_8007AEB8;
extern s32 D_8007AEBC;
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
    s16 unk22; /* rotation y */
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
    ActorPos unk14; /* position, rotation x/y */
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
    s16 unk84; /* smoothed look-at point (func_80047B90) */
    s16 unk86;
    s16 unk88;
    u8 pad8A[0x90 - 0x8A];
    u8 unk90;
    u8 unk91;
    u8 unk92;
    u8 unk93;
    s16 unk94;
    s16 unk96;
    u8 pad98[0xA0 - 0x98];
    struct Unk8004AB64 *unkA0;
    u16 unkA4;
    u16 unkA6;
    u16 unkA8;
    s16 unkAA;
} Unk8004AB64;

/* model header as seen by func_80047B90 (BtlUnit9C in ovl_0b) */
typedef struct {
    u16 unk0; /* flags */
    u8 pad2[0x48 - 0x2];
    s16 unk48; /* look-at point */
    s16 unk4A;
    s16 unk4C;
} Unk80047B90;
void func_80049B48(Unk8004AB64 *);

typedef struct Unk8007AECCNode {
    Unk8004AB64 *unk0; /* key */
    u8 pad4[8 - 4];
    struct Unk8007AECCNode *unk8; /* next */
} Unk8007AECCNode;

typedef struct Unk16D8 {
    struct Unk16D8 *unk0; /* next */
    Unk8004AB64 *unk4;
} Unk16D8;

typedef struct {
    u8 pad0[0x1450];
    Unk8007AECCNode unk1450[32];
    Unk8007AECCNode *unk15D0; /* active list */
    Unk8007AECCNode *unk15D4; /* free list */
    Unk16D8 unk15D8[32];
    Unk16D8 *unk16D8;
    Unk16D8 *unk16DC; /* tail */
    Unk16D8 *unk16E0; /* free list */
    u8 unk16E4[8];
    u8 unk16EC;
    u8 unk16ED;
    u8 pad16EE;
    u8 unk16EF;
    u8 *unk16F0; /* script data base */
    u8 pad16F4[0x16F8 - 0x16F4];
    u8 unk16F8;
    u8 pad16F9[0x16FC - 0x16F9];
    s32 unk16FC;
    s16 unk1700;
} Unk8007AECC;
extern Unk8007AECC *D_8007AECC;

/* Inlined copy of func_8004B040 (find object by id). */
static inline Unk8004AB64 *func_8004B040_inline(s32 id) {
    Unk16D8 *n;
    for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
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

extern s32 D_80077E20;
extern s32 D_8007ADFC;
extern s32 D_8007AE00; /* party mask, set by func_80049ED8 */
extern u32 D_800739A0[];
extern u32 D_8007B97C;
extern u32 D_8007B980;
extern s32 D_8007BBEC;
extern s32 D_8007BBF4;
extern u8 D_80077DB6;
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

extern Unk4 D_80068158;

extern u8 D_800739B8[];
extern Unk80080A10 *D_8007F790;
extern Unk80080A10 D_8007F8F0;
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
extern Unk80083158 D_80083158;
/* menu state */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3; /* selected party slot */
    s32 unk4; /* frame counter */
    s32 unk8;
} Unk80077E50;
extern Unk80077E50 D_80077E50;
extern u8 D_80077E53;
extern s32 D_8007AE54;
extern u8 D_8007954E;
extern u8 D_8007AD1E;
extern s32 D_8007BBA8[8];
extern u8 *D_8007BC0C; /* help-arrow animation script */
extern s32 D_8007BC08;
extern s32 D_8007BBF0;
extern volatile s32 D_800809E0;

void func_8001C720(void);
s32 func_8001D848(void);
s32 func_8001ED40(char *, char *, s32); /* BIOS A(18h) strncmp */
void func_8001E2AC(u16);
void func_800236C4(void);
void func_8002BFD0(void);
void func_80023EB8(void);
void func_80023F00(void);
void func_8002FA50(s32);
void func_8002FAAC(s32);
void func_8002FB08(s32);
void func_8002FB64(s32);
void func_80030770(u8 *);
s32 func_8002F5A0(void (*)(void));
void *func_80030B34(void *, void *, s32);
void *func_80030EE4(void *, void *, s32);
void *func_80031284(void *, void *, s32);
void *func_80031718(void *, void *, s32);
void func_8002FC38(void);
void func_8002F6C8(void);
void func_8002FC8C(s32, s32, s32, s32);
void func_80032658(void);
void func_80032140(s32, s32);
void func_80056B18(void);
void func_80056C08(void);
s32 func_8005C8BC(u32);
void func_80055D80(s32, s32);
void func_8005ECCC(AkaoChannel *, s32);
void func_80060938(void);
void func_80014E20(s32, s32, s32, s32);
void func_8003556C(void (*)(void));

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
extern Unk8007BD38 D_8007BD38;
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
extern EventVars D_8007AEF0;

void func_800239A4(void);
void func_80023784(void);
void func_800325A4(void);
void func_8003264C(void);
s32 func_800357C0(u8 *, s32);
s32 func_8002BA68(s32);
void func_800555A8(s32);
void func_800343C0(s32);
void func_80034404(s32);
void func_8001A878(void *, s32); /* BIOS A(28h) bzero */

/* argument block for sound command func_8005C8BC */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} Unk80080AA0;
void func_8005A014(); /* (VoiceParams *, u8 *, u8 *, s32); callers pass Unk80080AA0 * */
extern Unk80080AA0 D_80080AA0;
extern Unk80080AA0 D_80080970; /* command block passed to the handlers */
extern void (*D_8006F740[])(Unk80080AA0 *); /* sound command handlers */

typedef struct {
    u8 unk0; /* card id, 0xFF = none */
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
} Unk6; /* card inventory entry */

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    Unk6 unk6[105];
} Unk80083248;
extern Unk80083248 D_80083248;

typedef struct Unk8007B9C0 {
    struct Unk8007B9C0 *unk0; /* parent */
    s16 unk4; /* x */
    s16 unk6; /* y */
    s16 unk8;
    s16 unkA;
    s32 (*unkC)(struct Unk8007B9C0 *, s32); /* input handler */
    s32 unk10;
    Window *unk14;
    s16 unk18; /* w */
    s16 unk1A;
    s16 unk1C;
    s16 unk1E; /* message index */
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 unk26;
    u8 unk27; /* h */
    u8 unk28;
    u8 unk29; /* text speed */
    u8 unk2A;
    u8 unk2B;
} Unk8007B9C0;
extern Unk8007B9C0 D_8007B9C0[];

extern u8 D_80077E2F;
extern s16 D_80077E30; /* = D_80077E28.unk8 */

void func_80036470(void);
void func_80059AA8(s32, s32);
s32 func_80021FB4(void);
void func_80022050(void);
void func_80035430(void);
void func_80023918(void);
void func_80032078(void);
void func_80061070(s32, s32, s32, void (*)(void));
void func_800611BC(void);
void func_800611EC(void);
void func_8006121C(void);
void func_8006124C(void);
void func_800565F0(void);

typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    void *unk4[16];
} Unk80073C40;
extern Unk80073C40 D_80073C40;

typedef struct {
    u32 unk0;
    u8 pad4[0x24 - 4];
} Unk80073940; /* 0x24-byte entries */
extern Unk80073940 D_80073940[];

typedef Unk8004AB64 Unk800367D0; /* func_800367D0 creates a script object */

typedef struct {
    u8 pad0[0x12];
    s16 unk12;
} Unk8006573C;

extern s16 D_80067950;
extern u32 D_80077DB4;
extern u8 D_8006CAB8[][2];

u16 *func_800633E4(s32);
void func_80027CB0(void);
void func_8003431C(void);
void func_80033690(void);
s32 func_80055210(); /* unprototyped: main calls it with no args, ovl_65 passes a pointer */
void func_8001C718(void);
void *func_8001C7FC(s32, s32, s32); /* mode 4: address, 5: size */
void func_8002CD4C(RECT *, s32, s32, s32);
void func_80032A44(void);
Unk800367D0 *func_800367D0(s32, s32, s32, s32);
s32 func_80021380(void (*)(u8, u8 *));
void func_80021340(void);

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8; /* fine tune */
    s16 unkA; /* base note */
    u16 unkC;
    u16 unkE;
} Unk8005EBB4;
extern u32 D_8006F410[12]; /* semitone pitch table */
extern Unk8005EBB4 D_8007F970[];
u16 func_8005DFE0(Unk8005EBB4 *p, s32 note, u8 fine, s32 *out);

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
void func_8005EBB4(AkaoChannel *, Unk8005EBB4 *, s32);

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
extern Unk80077E28 D_80077E28;

extern s16 D_80080A2C;
extern s32 D_80080AC0;
extern s32 D_80077E58;
extern u32 *D_8007BCA4;

void func_8005CDE4(void);
void func_800325E0(void);
void func_80023A2C(void);
s32 func_8002F8E4(s32);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    s32 unk4;
    s32 unk8; /* VSync(-1) timestamp */
    s32 unkC;
    u16 unk10;
    u16 unk12;
} Unk80077188; /* 20 bytes */
extern Unk80077188 D_80077188[][23];


typedef struct {
    u8 pad0[0x12];
    u16 unk12;
    u8 pad14[2];
    u16 unk16;
    s16 unk18;
    s16 unk1A;
} Unk80065ADC;

void func_8004C204(void);
void func_800512E4(s32);
void func_80021974(Unk80021938 *, s32, s32, s32, s32, s32, s32, s32);
Unk80021938 *func_80021E5C(s32);
s32 func_80021C20(Unk80021938 *);
void func_8002BEEC(u32 *, u32 *, s32, s32);

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
extern s32 D_8007B958;
extern s32 D_8007B974;
extern s32 D_8007BB94;

s32 func_8005703C(s32, s32);
void func_80056614(void);
void func_8006008C(AkaoChannel *, s32);
void func_800600C0(AkaoChannel *, s32);
u8 func_80022D3C(void (*)(u8 *));



extern u8 D_8006CAD8[][5];
extern s32 D_800809B8;
extern u8 D_80083156; /* tempo scale */
void func_8005E3FC(AkaoChannel *, u32);
void func_800579B4(AkaoChannel *, u32, s32);

void func_8005A658(Unk80080AA0 *);

/* ---- hoisted from overlay headers ---- */

/* file table entry (func_8001E218 / func_80063088) */
typedef struct {
    s32 unk0;
    s32 unk4; /* start sector */
    s32 unk8;
    s32 unkC; /* end sector / next id */
} FileEntry;

/* D_8006794C->unk14[i]: per-directory header (func_8001E218) */
typedef struct {
    s32 unk0; /* 3 = flat u16 table at unk18[13] */
    s32 unk4; /* entry count */
    s32 unk8;
    s32 unkC;
} FileDirEntry;

/* D_8006794C->unk18[i]: 8-byte {id, sector} entries searched by func_8001E218 */
typedef struct {
    u16 unk0; /* id */
    u16 unk2;
    s32 unk4;
} FileIdEntry;

/* file id table (GameData.unk7E4) */
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

/* menu list/cursor state passed to func_8002EE8C etc. */


/* visible window of a MenuList (func_8002F344) */
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
extern Unk800703FC D_800703FC;

extern u8 *D_80067944; /* end of load area */
extern s32 D_80067948;
extern u32 D_8006DF5C[]; /* exp needed for next level, by level */
extern s32 D_80070424;
extern s32 D_80070428;
extern s32 D_80070450;
extern s32 D_80070454;
extern s32 D_80070460;
extern s32 D_800704AC;
extern s32 D_80073998[];
extern u32 D_80077528; /* pad buttons pressed this frame */
extern u8 D_80077DB5;
extern s32 D_80077DC0; /* frame counter? */

s32 func_80012AD4(void);
void func_80012D08(void);
void func_80012D18(void);
s32 func_80016EC0(s32, s32, s32, void (*)(void)); /* BIOS OpenEvent */
s32 func_80016ED0(s32); /* BIOS EnableEvent */
s32 func_8001C750(s32, s32);
s32 func_8001C8B0(s32, s32, s32, s32);
s32 func_8001CA70(s32); /* file size */
void func_8001CCB4(s32, s32, s32);
void func_8001CFB4(void);
void func_8001D8A8(s32);
void func_8001D9CC(s32, s32, s32, s32, s32);
s32 func_8001DAA4(s32, s32, s32, RECT *); /* copy through VRAM, returns bytes done */
extern u8 D_80068234;      /* next slot in D_80073CE8 */
extern RECT D_80073CE8[6]; /* transfer rects (func_8001DAA4) */
void func_8001DCB8(void);
FileEntry *func_8001E218(s32, s32);
void func_80021640(s32);
void func_80024070(void);
void func_8002C5CC(s32, s32);
void func_8002C694(s32, s32);
void func_8002C740(s32, s32, s32);
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
extern Unk80077D68 D_80077D68;
s32 func_8002FD98(s32, s32);
void func_80032B08(s32);
void func_8002C974(s32, s32, s32);
typedef struct {
    s32 unk0;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 pad7;
} Unk8006BC40;
extern Unk8006BC40 D_8006BC40[23];
typedef struct {
    u8 x0, y0, x1, y1, x2, y2, x3, y3;
} Unk80068F48;
extern Unk80068F48 D_80068F48[];
void func_80057708(s32, s32, s32);
void func_80057788(s32, s32, s32);
s32 func_80033964(u32);
s32 func_8003502C(u8 *, s32);
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
} Unk8006DD88; /* base stats per character class */
extern Unk8006DD88 D_8006DD88[];
void func_8002C210(void);
void func_8002E490(void);
void func_800225BC(u8 *);
u8 func_80022C10(s32, void (*)(u8 *));
void func_8005EC20(AkaoChannel *, u32);
typedef struct {
    u8 unk0[4]; /* stat bonuses */
    u8 unk4[4]; /* OR'd into BtlUnit.unk70[] (func_800BC0D0) */
    u8 unk8;    /* OR'd into BtlUnit.unk79 */
    u8 pad9[0xC - 9];
} Unk8006D548;
extern Unk8006D548 D_8006D548[];
void func_80033BD0(u32);
void func_8004EA30(Unk8007B9C0 *);
Unk8007B9C0 *func_8004EA3C(void);
extern s32 D_8007B968;
extern s32 D_8007B970;
extern Unk4 D_8007B99C;


void func_800615F8(void);
extern u8 D_8006F40C[];
extern Unk80080A10 *D_800831C8[24];
void func_800577BC(s32, s32, s32);
void func_80035108(void);
void func_80035680(void);
void func_80035B90(Unk80035B90 *);
void func_80032A18(u8 *);
void func_8005658C(s32 *, Unk8005EBB4 *, u32, s32);
s32 func_80056750(s32 *, s32, s32, u32);
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
extern s32 D_8007F720[]; /* AKAO header copy (0x70 bytes) */
extern u8 D_8006CACC[];
void func_800329FC(u8 *);
extern AkaoChannel D_80080AC8[];
void func_8005CE04(); /* (u32 *src, u32 *dst, u32 size): word copy */
u16 func_800593C0(u32, AkaoChannel *);
s32 func_8002B43C(s32);
void func_8002B47C(s32);
extern RECT D_8006BC38;
void func_80061C1C(void);
void func_80057688(s32, s32);
void func_80057640(s32, u32, u32, s32);
u8 func_8002F800(void);
void func_80032238(void);
u16 func_80019150(void);
void func_80019160(u16, s32);
void func_80019750(Unk4 *);
void func_80019774(Unk4 *);
extern u8 D_80077178;
extern Unk4 D_8007717C;
extern Unk4 D_8006BCF8;
void func_80035E40(void);
void func_80035F94(void);
void func_80060D1C(s32, s32, s32, s32);
void func_80060ED8(s32, s32, void (*)(void));
s32 func_800C44F4(u32); /* field overlay (ovl_0a) */
s32 func_800C0F08(void); /* field overlay (ovl_0a) */
s32 func_800C0FB0(void); /* field overlay (ovl_0a) */
extern CdRequest D_80076148;
s32 func_8005EB40(s32 *, s32, s32, s32);
s32 func_80021320(void);
s32 func_800233CC(); /* called with a u8 it ignores; callers see an int return (no prototype in the original) */
u8 func_800240E8(void);
void func_80023A6C(void);
void func_8002417C(void);
extern Unk80035B90Data D_80010E48;
void func_80035B24(Unk80035B90 *, Unk80035B90Data *);
s32 func_800565DC(s32 *);
extern u16 *D_800809DC;
extern u16 *D_800809E4;
extern u16 *D_80083148;
s32 func_8005A5FC(s32);
void func_8005A20C(u8 **, u8 **, s32);
void func_800575B4(u32);
extern u8 *D_800809E8;
extern s32 D_800809C0[];
extern s32 D_800809D8; /* = D_800809C0[6] */
void func_800616F8(s32, s32, s32, s32);
s32 func_80021A1C(Unk80021938 *);
u8 func_8002F8BC(); /* called with an argument it ignores */
extern void (*D_8006CA64[])(void);
extern s32 D_8006CA40[];
void func_80032708(void);
void func_80032798(void);
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
void func_80032C4C(s32, s32, u32 *, u8);
extern u32 D_8006CA78[];
extern u32 D_80077CA4[];
s32 func_80056700(s32, s32);
void func_80051568(void);
void func_8004E924(void);
void func_8004E7E0(void);
void func_8004BE00(s32);
void func_8004AC80(void);
extern u16 D_8007AE12;
void func_80032384(void);
void func_800326B0(void);
void func_80036560(void);
CdReadCtx *func_8002177C(void *, s32, s32, void *, s32, s32, void (*)(CdReadCtx *));
s32 func_800217C4(void *);
void func_80021C60(); /* takes no args, but ovl_0b passes one */
void func_80021CDC(s32);
Unk80021938 *func_80021CFC(void);
s32 func_800220E8(s32);
void func_80022390(s32);
s32 func_80022B28(void);
void func_80022BD0(void);
s32 func_80022DF0(); /* TODO: (sector, size, dst); ovl_14 passes a 4th argument */
s32 func_800230A0(void);
u8 func_8002306C(void);
void func_800239D0(void);
void func_80024BD8(void);
void func_8002705C(RECT *, s32, s32);
s32 func_80027710(s32, s32, s32, void *, u8);
u8 *func_8002AD38(u8 *, u8 *);
s32 func_8002AE7C(u8 *);
typedef struct {
    u8 pad0[4];
    u8 unk4; /* advance width */
    u8 pad5[3];
} Glyph8;
extern Glyph8 D_80068690[]; /* indexed by ASCII code */
extern Glyph8 D_80068890; /* & */
extern Glyph8 D_80068CE0; /* . */
extern Glyph8 D_80068DC8; /* _ */
void func_8002B524(s32, s32, s32, s32, s32);
void func_8002B56C(s32, s32, s32, u8 *, s32);
void func_8002B5FC(s32, s32, s32, u8 *, s32);
u8 *func_8002BD54(u8 *, u8 *);
void func_8002C064(s32);
void func_8002C0DC(s32);
void func_8002C308(s32);
void func_8002C4B0(u8);
void func_8002D9B4(RECT *, s32, s32);
Window *func_8002DC54(RECT *, s32, s32);
Window *func_8002EA7C(RECT *, s32, s32, Window *);

/* Inlined copy of func_8002DD00 (close window). */
static inline void func_8002DD00_inline(Window *w) {
    if (w != NULL && w->unk1C != 0 && w->unk1C != 2) {
        w->unk1C = 2;
        if (D_80077148.unk1 != 2) {
            func_8002D1AC(w, w->unk28);
        }
    }
}

void func_8002DD00(Window *);
void func_8002DE10(Window *);
void func_8002DE60(void);
void func_8002DEE4(RECT *, s32, s32);
void func_8002E040(RECT *, s32, s32, MenuList *);
void func_8002E2E4(Window *, void (*)(Window *));
void func_8002E2EC(Window *, void (*)(Window *));
void func_8002E2F4(Window *, void (*)(Window *));
void func_8002E2FC(Window *, void (*)(Window *));
void func_8002E304(Window *, s32);
void func_8002E31C(Window *, u8);
void func_8002E334(Window *, s16);
void func_8002EB64(s32, s32);
void func_8002EB74(MenuList *);
void func_8002D550(Window *);
void func_8002D7E8(Window *);
MenuView *func_8002ECA0(MenuList *, MenuView *);
void func_8002EDFC(MenuList *, s32, s32, s32, s32);
void func_8002EE8C(MenuList *, s32);
void func_8002F044(MenuList *, s32);
void func_8002F0EC(MenuList *, s32);
MenuView *func_8002F344(MenuList *, MenuView *);
void func_8002F40C(MenuList *, RECT *, RECT *);
void func_8002F488(s32);
void func_8002F514(s32);
void func_8002F55C(s32);
u8 func_8002F810(void);
s32 func_8002F8A8(void (*)(void));
u16 func_8002F93C(u16);
void func_8002F9D0(void);
void func_800320B8(s32, s32, s32, s32);
void func_80032120(s32, s32);
void func_8003253C(void);
void func_80032574(void);
void func_80032628(void (*)(void));
void func_80032634(u8);
void func_80032640(u8);
s32 func_80033CCC(s32, s32);
void func_80033D1C(u8);
u8 func_800349BC(s32, s32);
u8 func_80034A30(Character *);
u8 func_80034A04(u32 *, s32);
void func_80035580(RECT *);
void func_800355E4(); /* unprototyped: the argument is unused and most callers pass none */
void func_80035674(u8);
void func_8003575C(const u8 *);
u8 *func_80035774(s32); /* text lookup */
void func_8004A06C(s32);
void func_8004BBF0(void);
void func_8004BFB0(void);
void func_80050A84(void);
void func_8005392C(void);
s32 func_800551F0(void);
void func_80055D40(void);
void func_80055D60(void);
s32 func_80055DF4(void);
s32 func_80055E14(s32 *, u32, s32);
s32 func_80056CAC(s32);
s32 func_80056CBC(s32);
void func_80062344(void);
void func_8006237C(void);
s32 func_80062420(u8, u8, u8, u8, u8, u8);
s32 func_8006238C(s32);
s32 func_800625F8(void);
s32 func_80062638(void);
s32 func_80062B3C(void);
s32 func_8006276C(s32); /* number to string */
void func_800624A8(void);
void func_800624E8(void);
void func_8006252C(void);
void func_80062750(s32);
FileEntry *func_80063088(FileTable *, s32);
void func_80063108(u8 *, u8 *);
/* per-frame offset animation of a window (func_80063118 / func_800631AC) */
typedef struct TweenDef {
    u8 pad0[2];
    s16 unk2; /* frame count */
    s8 *unk4; /* x deltas */
    s8 *unk8; /* y deltas */
    void (*unkC)(struct TweenDef *); /* called when done */
} TweenDef;

/* text window (func_80063118 / func_800631AC) */
typedef struct {
    s16 unk0; /* start delay */
    s16 unk2; /* frame */
    RECT unk4;
    TweenDef *unkC; /* running animation, NULL when idle */
} WinDef; /* 0x10 */
void func_80063118(void *, void *, s32);
void func_80063580(DR_TPAGE *, s32);
void func_800635E0(DR_TPAGE *, s32);
void func_800651D8(void);
s32 func_80065208(s32);
void func_80065218(s32);
void func_80065228(s32);

/* ---- main batch 1 ---- */

typedef struct {
    u8 unk0[8];
} Unk8; /* 8 bytes, byte-aligned (copied with lwl/lwr) */
extern Unk8 D_80076B0C; /* last CD status/result bytes */

s32 func_800384D0(Unk8004AB64 *, s32, s32, s32);
extern s32 D_8007AE34;
extern CdlATV D_8007F718;
void func_8005A284(); /* (Unk80080A10 *, AkaoChannel *) */
void func_80062830(Unk6 *, u8);
extern s32 D_8006FB40[];

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC; /* also accessed as D_80080A24 */
} Unk80080A18;
extern Unk80080A18 D_80080A18;


extern AkaoChannel D_8007BD48[];
extern s32 D_80080AC4;
extern AkaoChannel D_8007E3D8[];
extern s32 D_800831C4;

typedef struct {
    s32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    s32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u16 unk28;
} Unk80080A70;
extern Unk80080A70 D_80080A70;
void func_8005FD10(AkaoChannel *, u32);
void func_8005FDD8(AkaoChannel *, u32);
extern u16 D_80080A98; /* = D_80080A70.unk28 */
extern s32 D_80080A80;


typedef struct {
    void (*unk0)(u8);
    void (*unk4)(u8);
    s32 unk8;
} Unk8006C9D4;
extern Unk8006C9D4 D_8006C9D4[];
s32 func_8002C1EC(void);
void func_80035490(void);

extern s32 D_8007B6F4;
extern s32 D_8007B6F8;
extern struct Unk8004AB64 *D_8007B70C; /* current script object */
void func_80052DA0(void);
s32 func_80052D40(void);
void func_80052D70(s32);
extern s32 D_8007B718;
void func_8004E7FC(void);
void func_8004883C(void);
s32 func_80048F54(void);
void func_8004FBD8(void);
void func_8004FE0C(void);

u16 func_800BEC64(u32); /* overlay code at 0x800BEC64 */
void func_800363BC(void);
void func_80036444(void);
void func_80036528(void);
void func_8002F9F0(void);

void func_8002267C(CdRequest *);
u8 func_800230B8(CdRequest *);
void func_800231A0(u8, u8 *);
void func_800227C8(u8, u8 *);
s32 func_8005DD54(s32, s32);
s32 func_8005EC8C(s32, s32);
void func_8005EBE8(AkaoChannel *, s32);
void func_80056644(void *, s32);
void func_800566D0(void);
void func_8005681C(void);
void func_8005D6BC(void);
extern u8 D_80080990[];
void func_80056CCC(s32, s32);
void func_800575D0(s32);
s32 StopRCnt(s32);
void SpuQuit(void);
extern s32 D_8006F318;
extern u8 D_8006F334[];
extern s32 D_8007B708;
/* offset-table blob: count, then per-entry u16 offsets relative to unk4 */
typedef struct Unk8007B954 {
    s32 unk0; /* count */
    u16 unk4[1];
} Unk8007B954;
extern Unk8007B954 *D_8007B954;
extern u8 *D_8007BB80[];

typedef struct {
    u8 pad0[2];
    s16 unk2;
    u8 pad4[9 - 4];
    u8 unk9;
} Unk8004EFB0;

/* ---- main batch 2 ---- */

extern CdRequest D_80076178;
extern CdRequest D_800761A8;
extern Unk8004AB64 *D_8007B704;

/* Same as func_8004AEDC_inline, but written as `>>= (D_8007B6F8 = 1)` (srav). */
static inline s32 func_8004AEDC_inline2(void) {
    s32 r;
    if (D_8007B6F4 & 1) {
        func_80052DA0();
        r = func_80052D40();
    } else {
        r = *D_8007B70C->unk0++;
    }
    D_8007B6F4 >>= (D_8007B6F8 = 1);
    return r;
}

/* Inlined copy of func_8004B4C4 (resolve a script object reference). */
static inline Unk8004AB64 *func_8004B4C4_inline(s32 id) {
    if (id == 0xFF) {
        return D_8007B704;
    }
    if (id == 0xFA) {
        id = D_8007AECC->unk16ED;
    } else if ((u32)(id - 0xFB) < 4) {
        id = D_8007AECC->unk16E4[id - 0xFB];
    }
    return func_8004B040_inline(id);
}
extern s32 D_8007B710;
extern s32 D_8007BBC8;
extern s32 D_8007BBE0;
extern s32 D_8007BBE8;
extern AkaoChannel *D_8007F71C;

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
extern Unk80077D00 D_80077D00;

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
extern Unk8006D128 D_8006D128[];
void func_8003493C(s32, s32, u8);
extern s32 D_8006E510[];
extern RECT D_8006E508;
extern Unk80077E28Entry D_8006E530[];
extern Unk80077E28Entry D_8006E5B0[];
u8 func_80022C60(s32, s32, void *, void (*)(u8 *));
void func_80036634(u8 *);
extern void *D_8007AED0[8];
#ifndef OVL_0B_H /* battle overlay code; ovl_0b.h has the real prototypes */
s32 D_800BED6C(s32, s32);
s32 func_800BED10(s32, s32);
void func_800BE1E0(s32, s32, s32);
#endif
void func_80023304(s32);
void func_8002325C(u8);
extern void (*D_80068668[])(s32);

/* ---- main batch 4 ---- */
u8 func_80035B64(Unk80035B90 *, u8);
void func_80035B74(Unk80035B90 *, void (*)());
void func_80035B7C(Unk80035B90 *);
void func_80035744(void);
extern s32 D_8007B9A0;
extern u8 *D_8007B9A4;
extern u8 *D_8007B9A8;
extern u32 D_8007B9B0[];
extern u8 **D_8007BCA8;
void func_80051004(s32);
extern u8 D_8007B930[8]; /* name text being converted */

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
u32 func_8005709C(u8 *, u32); /* libspu SpuWrite? */
void func_80060FA8(void);
void func_800599D4(Unk80080A10 *, AkaoChannel *, s32);
void func_80018750(s32 *); /* libspu SpuGetReverbModeType */
void func_800618C0(s32, s32);
void func_80060AE0(s32, s32, s32);
extern Unk80083168 D_80083168;
s32 func_80061BC8(); /* K&R: callers pass one or two arguments */
s32 func_80061FBC(s32, s32, s32, void (*)(void));
void func_800576BC(s32, s32);
void func_80062118(void);
void func_80061E1C(s32, s32, void (*)(void));
void func_800621D4(void);

/* ---- main batch 5 ---- */
s32 func_800219FC(Unk80021938 *);
Unk80021938 *func_80021E20(void);
void func_80028B20(u8 *);
u8 func_80033D28(void);
void func_80012CF8(void);
extern u8 *D_8007B720;
extern u8 D_8007B724[]; /* random byte table */

extern u8 D_8007B92D[];
void func_8004BF20(void);

/* ---- hoisted for menu overlays ---- */
void func_80028CF8(UnkBlink *, s32, void (*)(void));
void func_80028D08(UnkBlink *, s32, s32, s32);
void func_80028D2C(UnkBlink *);
void func_8002BE44(s32, u8 *);
void func_8002BE74(s32, s32);
u8 func_80035648(void);
void func_80035658(u8);
void func_800361CC(s32);
s32 func_800337D0(s32);
s32 func_8003380C(s32);
void func_800269F4(s32, s32, s32, s32);
typedef struct {
    u8 unk0; /* ability id, 0 = none */
    u8 unk1; /* ap needed? */
} AbilityEntry;
extern AbilityEntry D_8006CB28[16][0x30]; /* rows indexed by Character.unk34.b.row */

/* Inlined copy of func_80034430 (ability slot of a character). */
static inline s32 func_80034430_inline(s32 ch, s32 id) {
    s32 row;
    s32 i;
    if (ch >= 9) {
        return -1;
    }
    row = ((D_8006794C->unk1C->unk3C + ch)->unk34.w >> 12) & 0x1F;
    if (row >= 16) {
        return -1;
    }
    for (i = 0; i < 0x30; i++) {
        if (D_8006CB28[row][i].unk0 == id) {
            return i;
        }
    }
    return -1;
}

void func_80025ED4(s32, s32, s32, s32, s32, s32, u8);
extern u8 D_8007AF00;
typedef struct {
    u8 unk0; /* show second character */
    u8 unk1; /* slide direction */
    u8 unk2; /* party slot */
    u8 unk3; /* second party slot */
    s32 unk4; /* slide step */
} Unk80024428;
void func_80024428(s32, s32, s32, Unk80024428 *);
void func_80026FC4(s32, s32, s32, u8, s32); /* arg4 unused, callers pass 0 */
void func_800273C0(s32, s32, s32, s32, s32);
void func_80033D38(u8 *, s32); /* default name by id */
void func_80015C28(u8 *, u8 *, s32); /* memcpy */
s32 func_8002BCC0(u8 *); /* strlen */
extern u32 D_80077DBC; /* = D_80077DB0.unkC, read on its own by ovl_3a */
extern u32 D_80077520; /* pad buttons (repeat?) */
void func_8002886C(void);
void func_80028994(s32, RECT *, RECT *);
typedef struct {
    u32 *unk0; /* portrait image data, 0x6C0 bytes each */
    s32 unk4;  /* count */
} Unk80076F78;
extern Unk80076F78 D_80076F78;
extern u32 D_80076F80[]; /* first CLUT, right after D_80076F78 */
s32 func_80027E5C(s32, s32);
typedef struct {
    u8 pad0[4];
    u8 unk4; /* width */
    u8 pad5[3];
} Icon; /* sprite drawn by func_80027710 */
extern Icon D_800687C0;
extern Icon D_80068E30;
extern Icon D_80068E38;
extern s32 D_80070494;
void func_800270EC(s32, s32, s32);
void func_8002C870(s32, s32, s32);
void func_8002C8AC(s32, s32, s32);
s32 func_80022B18(void);
s32 func_80062560(); /* defined K&R-style: u8 param, callers pass ints unmasked */
Unk6 *func_800625A0(s32, s32);
s32 func_800626D0(s32, s32);
typedef struct {
    u8 unk0; /* power (x2) */
    u8 unk1; /* type */
    u8 unk2; /* physical defence (x2) */
    u8 unk3; /* magic defence (x2) */
    u8 unk4; /* collector points */
} CardInfo; /* 5 bytes */
extern CardInfo D_800121C8[]; /* card table */
CardInfo *func_80062B30(void);
s32 func_800627BC(s32);
extern s32 D_800704C4;
extern s32 D_80070418;
void func_8003563C(u8);
s32 func_800281A4(s32);
s32 func_800632B4(s32, s8 *);
extern s32 D_800739A8[];
u8 *func_80027834(s32, s32, u32 *, s32, u8, s32 *, void *);
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} Unk8006BC0C;
extern Unk8006BC0C D_8006BC0C[4];
u8 func_8003561C(void);
void func_80025A28(s32, s32, s32, s32, s32, s32);
void func_8002EF58(MenuList *, u32);
void func_8002F1CC(MenuList *, s32);
void func_8002F284(MenuList *, s32);
void func_8002EEF0(MenuList *, u32);
extern u32 D_80077524; /* pad buttons */
extern s32 D_80070430;
extern u8 D_8007AF02;
void func_8003384C(Character *);
s32 func_80033280(s32, s32, u8 *);
void func_80028720(void *, s32);
void func_8002C1E4(void);
s32 func_80065EB0(s32);
s32 func_80065EA0(s32);
s32 func_80066170(s32);
s32 func_80065E90(s32, s32, s32);
s32 func_80065E80(s32, s32, s32);
void *func_80062824(void);
extern s32 D_80070440;
extern s32 D_8007048C;
void func_80033AF4(s32, s32);


/* ---- hoisted for the world map (ovl_0c) ---- */
s32 func_80062E3C(s32, s32, s32, s32, s32, s32, s32);
extern s32 D_80073990;
void func_80019194(void);
void func_80032098(void);
void func_80023A4C(void);
void func_8001CD04(s32);
s32 func_8004BC38(); /* defined K&R-style: u8 param (unused); some callers pass none */
u8 func_80036300(s32);
s32 func_8003631C(s32);
void func_800362C8(void);
s32 func_80054F80(void);
s32 func_80050AAC(void);
void func_8005546C(s32, s32);
void func_80056228(void);
s32 func_80024030(void);
s32 func_8002AA04(s32, u8 *, s32);
extern s32 D_800704C0;
extern u8 D_8006A218[]; /* item names */
extern u8 D_8006ADAC[]; /* key item names */
typedef struct {
    s16 unk0; /* name offset into D_8006ADAC */
    s16 unk2; /* help offset */
    u8 pad4[2];
    s16 unk6; /* description offset */
} Unk8006B374;
extern Unk8006B374 D_8006B374[];
Unk16D8 *func_8004AE9C(void);
extern s32 D_8007046C;
void SetBackColor(s32, s32, s32);
void SetFarColor(s32, s32, s32);
void SetFogNearFar(s32, s32, s32);
s32 func_8003378C(void);
extern u8 D_80073939;
extern u8 D_8007393A; /* pad analog stick */
extern u8 D_8007393B;
void func_80015C38(const char *, ...); /* printf */

/* Inlined body of func_8004E958 for one entry (close and unlink). */
static inline void func_8004E958_inline(Unk8007B9C0 *p) {
    Unk8007B9C0 *q;
    if (p != NULL && p->unk8 != 0) {
        p->unk8 = 0;
        if (func_8004EA3C() == p) {
            func_8004EA30(p->unk0);
        }
        for (q = D_8007B9C0; q < D_8007B9C0 + 10; q++) {
            if (q->unk8 != 0 && q->unk0 == p) {
                q->unk0 = NULL;
            }
        }
    }
}

/* Inlined copy of func_8004EEA0 (close an entry, its window and its children). */
static inline void func_8004EEA0_inline(Unk8007B9C0 *p) {
    if (p != NULL && p->unk8 != 0) {
        if (p->unk14 != NULL) {
            func_8002DD00(p->unk14);
        }
        if (D_8007B968 >= 0 && D_8007B970 == p->unkA) {
            func_80019774(&D_8007B99C);
        }
        func_8004E958_inline(p);
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
s32 func_800216D8();
s32 func_8001E350(s32);

/* Inlined copy of func_8004B9B8. */
static inline void func_8004B9B8_inline(Unk8004B9B8 *p) {
    s32 v;
    p->unk22 = p->unk52 & 0xFFF;
    v = p->unk5A;
    if (v != p->unk36) {
        p->unk36 = v;
        p->unk3A = 0;
        p->unk64 = 0;
        p->unk38 = func_800216D8(p->unk34, v);
        p->unk5D = p->unk69;
    }
    p->unkF &= 0x7F;
}

s32 func_80037D6C(s32);
extern u8 *D_800CF478;
/* help-window rect plus frame style and x offset (func_80035214) */
typedef struct {
    RECT unk0;
    s16 unk8; /* style */
    s16 unkA; /* x offset */
} Unk80035214;
void func_80035214(Unk80035214 *);
void func_80035C2C(s32, s32, s32, Unk80035B90 *);
s32 func_8002FD70(s32, s32, s32);
s32 func_8002FDE8(s32, s32, s32);
typedef struct {
    u32 tag;
    u32 rgbc;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} LineF2Words; /* LINE_F2 */
extern u32 D_80068F20[][4]; /* line colours */
extern s8 D_80068F94[4][4]; /* shadow line offsets (func_80025108) */
typedef struct {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} LineXY;
u8 *func_80027948(s32 x, s32 y, u32 *ot, Unk800686E8 *d, u8 flag, s32 *out, SprtWords *s);
u8 *func_80027B5C(s32 x, s32 y, u32 *ot, u8 *s, s32 unused, SprtWords *p);
/* Inlined copy of func_80027948 (SPRT + DR_TPAGE from a sprite descriptor). */
static inline u8 *func_80027948_inline(s32 x, s32 y, u32 *ot, Unk800686E8 *d, u8 flag, s32 *out, SprtWords *s) {
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
    for (n = D_8007AECC->unk15D0; n != NULL; n = n->unk8) {
        if (n->unk0 == key) {
            break;
        }
        prev = n;
    }
    if (prev != NULL) {
        prev->unk8 = n->unk8;
    } else {
        D_8007AECC->unk15D0 = n->unk8;
    }
    n->unk8 = D_8007AECC->unk15D4;
    D_8007AECC->unk15D4 = n;
}
void func_8002E718(RECT *, s32, MenuList *);
void func_8002E7FC(RECT *, s32, MenuList *, s32);
void func_8002E960(RECT *, s32, MenuList *, s32);
void func_80025108(RECT *, s32, u8, s32);
void func_8002709C(RECT *, s32, u8, u8, s32);
extern s32 D_8008315C;
extern s32 D_800831A8; /* sound effect volume */
extern s32 D_800831C0;
extern s16 D_8006F640;
extern s16 D_8006F540[]; /* pan curve, 256 entries */
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
extern Unk800689B0 D_80068D70[];
u8 func_8002E4BC(Window *);
void func_8002CB18(Window *);
void func_8002E588(Window *);
/* sprite cell of a frame animation (func_800357E0) */
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
void func_800515B8(void);
void func_800512B8(s32, s32);
void func_800512C8(s32, s32);
void func_800513CC(s32);
void func_80051610(void);
extern s32 *D_8007B6F0; /* script value stack pointer */
extern void *D_8007B700; /* script current object */

/* Inlined copy of func_8004B404 (read script variable, refreshing 2-4). */
static inline u16 func_8004B404_inline(s32 arg0) {
    s32 i = arg0 & 7;

    switch (i) {
    case 2:
        D_8007AE10[2] = func_800BEC64(0);
        break;
    case 3:
        D_8007AE10[3] = func_800BEC64(1);
        break;
    case 4:
        D_8007AE10[4] = func_800BEC64(2);
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
void func_8004C8C4(Unk8007B9C0 *, u8 *);
void func_8004D2C0(Unk8007B9C0 *, u8 *);
void func_8004D69C(Unk8007B9C0 *, u8 *);
void func_80051300(void);
void func_80051380(void);
void func_80050F4C(s32);

/* Inlined lookup of message i in the current offset table (D_8007B954). */
static inline u8 *func_8004D940_msg(s32 i) {
    u8 *e = NULL;
    u16 *t;

    if (i >= 0 && i < D_8007B954->unk0) {
        t = D_8007B954->unk4;
        e = (u8 *)t;
        e += t[(D_8007B708 == 2 || D_8007B708 == 4) ? i * 2 : i * 4 + 2];
    }
    return e;
}
/* AP learned by a character for ability id, -1 if not learnable (inlined helper). */
static inline s32 func_80034430_ap_inline(s32 ch, s32 id) {
    s32 i = func_80034430_inline(ch, id);
    if (i < 0) {
        return -1;
    }
    return D_8006794C->unk1C->unk3C[ch].unk58[i];
}

/* AP needed to master ability id (inlined form of func_800347D8). */
static inline s32 func_800347D8_inline(s32 ch, s32 id) {
    s32 i = func_80034430_inline(ch, id);
    Character *c = D_8006794C->unk1C->unk3C + ch;
    if (i < 0) {
        return 0;
    }
    return D_8006CB28[c->unk34.b.row][i].unk1;
}
void func_800576A0(s32, s32);
void func_80057738(s32, s32);
void func_80057760(s32, s32);
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
u8 *func_8002B7B0(s32, s32, u32 *, u32, s32, s32 *, SprtWords *);
void func_8002E660(Window *);
s32 func_80034A64(s32, s32, u8);
s32 func_80034B50(s32, s32, u8);
s32 func_80034C64(s32, s32, u8);
s32 func_80034D78(s32, s32, u8);
s32 func_80034F7C(s32, s32);
s32 func_80034FD4(s32, s32);
s32 func_80034E80(s32, s32, u8);
void func_80033DCC(Character *);
POLY_FT4 *func_80051530(void);
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
Unk8004AB64 *func_8004B040(s32);
void func_8004ACA4(Unk8004AB64 *, s32, s32);
void func_8004F9A0(void);
/* zero the first three bytes (r, g, b) of a 4-byte colour */
static inline void func_8002FC38_clear3(u8 *p) {
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
    Unk8007B9C0 *unk50;
} Unk8004F5D4;
void func_80051600(void);
void func_8004D940(Unk8007B9C0 *);
/* 8-byte message-table entry (D_8007B954 in modes other than 4) */
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
    u16 unk0; /* volume left */
    u16 unk2; /* volume right */
    u16 unk4; /* pitch */
    u16 unk6; /* start address */
    u16 unk8; /* adsr 1 */
    u16 unkA; /* adsr 2 */
    u16 unkC;
    u16 unkE; /* loop address */
} SpuVoiceReg;
#define SPU_VOICE_REG(v, r) (*(u16 *)(0x1F801C00 + (r) + (v) * 16))
#define SPU_VOICE_REG8(v, r) (*(u8 *)(0x1F801C00 + (r) + (v) * 16))
#define SPU_REG16(a) (*(u16 *)(a))
extern s16 D_80080AC2;
extern s32 D_80080A28;
s32 func_8005715C(s32);
extern s32 D_8007F8E8;
extern s32 D_8007F8E4;
extern s32 D_8007F740;
extern Unk80080AA0 D_800809F0;
typedef struct {
    u16 unk0[6];
} Unk8007BD44Row;
extern u32 D_80083174;
/* voice parameter block written to the SPU by func_800577EC */
typedef struct {
    u8 pad0[4];
    s32 unk4;
    u32 unk8;  /* start address */
    u32 unkC;  /* loop address */
    u16 unk10; /* pitch */
    u16 unk12; /* adsr 1 */
    u16 unk14; /* adsr 2 */
    u16 unk16; /* volume scale */
    s16 unk18; /* volume left */
    s16 unk1A; /* volume right */
} Unk800577EC;
/* script object (D_8007AED0[]) */
typedef struct Unk8007AED0 {
    u8 pad0[0xE];
    u8 unkE;
} Unk8007AED0;
typedef struct {
    void (*unk0)(Unk80077C78 *);
    s32 unk4;
    s32 unk8;
} Unk8006BD04;
extern Unk8006BD04 D_8006BD04[];
extern u8 D_8006BCFC[]; /* window style -> colour */
typedef struct {
    u8 unk0[8];
} Bytes8;
extern Bytes8 D_800121C0; /* card arrow bits (func_80062830) */
typedef struct {
    u8 unk0[5];
} Bytes5;
typedef struct {
    u32 unk0[2];
} Words2;
/* stat calculation input (func_80032E84) */
typedef struct {
    Bytes8 unk0; /* = Character.unk2C..unk33 */
    Words2 unk8; /* = Character.unk88 */
    u16 unk10;
    u16 unk12;
    Bytes5 unk14; /* equipment */
} Unk80032E84In;
/* stat calculation output (func_80032E84) */
typedef struct {
    u16 unk0; /* hp */
    u16 unk2; /* mp */
    u16 unk4; /* max hp */
    u16 unk6; /* max mp */
    u8 unk8[4]; /* stats */
    u16 unkC[5]; /* attack, defences */
} Unk80032E84Out;
Unk80032E84Out *func_80032E84(Unk80032E84In *, Unk80032E84Out *);

/* Inlined copy of func_8003384C (recompute a character's stats). */
static inline void func_8003384C_inline(Character *c) {
    Unk80032E84In in;
    Unk80032E84Out out;

    in.unk0 = *(Bytes8 *)&c->unk2C;
    in.unk10 = c->unk10;
    in.unk12 = c->unk12;
    in.unk8 = *(Words2 *)c->unk88;
    in.unk14 = *(Bytes5 *)c->unk39;
    func_80032E84(&in, &out);
    c->unk24 = out.unk8[0];
    c->unk25 = out.unk8[1];
    c->unk26 = out.unk8[2];
    c->unk27 = out.unk8[3];
    c->unk28 = out.unkC[1];
    c->unk29 = out.unkC[2];
    c->unk2A = out.unkC[3];
    c->unk2B = out.unkC[4];
    c->unk10 = out.unk0;
    c->unk12 = out.unk2;
    c->unk18 = out.unk4;
    c->unk1A = out.unk6;
}
extern void (*D_8006BD28[])(Unk80077C78 *, s32);
extern s32 D_8007BBD4;
extern u32 *D_8007BBCC;
extern s32 D_8007B94C;
void func_8002DD54(void);
void func_8004E16C(void);
extern u8 *D_8007ADF8; /* script data */
/* 8-byte entry of the field script directory at D_8007ADF8 */
typedef struct {
    u16 unk0; /* offset from D_8007ADF8 */
    u16 unk2; /* size */
    u8 unk4;
    u8 unk5; /* flags */
    u8 pad6[2];
} ScriptEntry;

u8 func_80033A50(u32);
void func_80034394(s32);
s32 func_80033A1C(Character *);
void func_80033D8C(u8 *, s32);
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
extern Unk800689B0 D_80068CE8;
/* window frame tables (func_800253D4) */
extern u32 D_80068FA4[2]; /* edge rgbc, by flag */
extern u32 D_80068FAC[2]; /* corner rgbc, by flag */
extern u8 D_80068FB4[][4]; /* corner sprites (D_800686D0 index) per style */
extern u8 D_80068FC0[][2]; /* top/bottom edge sprites per style */
extern u8 D_80068FC8[][2]; /* left/right edge sprites per style */
typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy;
    u32 uvclut;
} Sprt8Words; /* SPRT_8 written as whole words */
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

u16 func_800274B4(s32, s32);
extern RECT D_80068F40;
void func_80058F34(AkaoChannel *, u32 *, u32, u32);
/* ---- main: boot, system setup, TIM helpers ---- */

/* GameState.unk24/unk28 viewed as a pair */
typedef struct {
    u32 unk0_0 : 1;
    u32 unk0_1 : 1; /* unk4 remaps indices (4 bits each) */
    u32 unk0_2 : 1;
    u32 unk0_3 : 1;
    u32 unk0_4 : 1;
    u32 unk0_5 : 1;
    u32 unk0_6 : 1;
    u32 unk0_7 : 1;
    u32 unk0_8 : 3;
    u32 unk0_11 : 3;
    u32 unk0_14 : 3;
    u32 unk0_17 : 15;
    u32 unk4;
} GameStateUnk24;

/* GameState.unk14 viewed as its 15 directory records */
typedef struct {
    s32 unk0; /* kind; 3 = table of u16 ids */
    s32 unk4; /* entry count */
    s32 unk8;
    s32 unkC;
} DirInfo;

/* 8-byte file directory entry (GameState.unk18 tables) */
typedef struct {
    u16 unk0; /* id */
    u16 unk2;
    s32 unk4;
} DirEntry;

/* Unk80021938.unk4 split into its parts */
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

extern u32 D_80067940; /* bump allocator for the boot-time state block */
void func_8001CB78(void);
void func_8001CBD0(void);
s32 func_80022A28(void);
void func_800236A4(s32);
void func_80016BA0(void);
void func_80018F20(void);

s32 func_8001D610(void);
void func_8001D32C(void);
void func_8001D3C4(void);
void func_8001E3A4(); /* (Unk8001E3A4 *); ovl_0b declares it with void * */
s32 func_8001D67C(void);
s32 func_8001D768(void);

s32 func_800213C0(void *, s32);
s32 func_80021A8C(Unk80021938 *);
void func_8002291C(void);

extern SpuReverbAttr D_80073690;
extern s32 D_800736A4; /* reverb mode */
extern DRAWENV D_80073C88;
void func_80018F50(void);
void func_80022A80(void);
void func_8001C6F8(void);
void func_80021770(void);
void func_800320D8(void);
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
void func_80019708(s32, u32, s32);
extern s32 D_80073730[2][32]; /* indexed by leading-zero count (func_80019708) */

void __main(void);
void func_80012900(s32);
void func_80012CF8(void); /* BIOS FlushCache */
void func_80012D08(void); /* BIOS EnterCriticalSection */
void func_80012D18(void); /* BIOS ExitCriticalSection */
void func_8001D30C(void);
void func_8001CB70(void);
void func_80016B6C(void);
void func_8001CFB4(void);

/* overlay load addresses by game mode (func_80012900). The rodata block
 * D_80010034 also holds the next word and libgpu's version string. */
typedef struct {
    u32 *unk0[10];
} LoadAddrs;
extern LoadAddrs D_80010034;

/* battle calculation context (func_800657F0 ... func_80065DF4) */
typedef struct {
    u32 unk0; /* low nibble: kind */
    u8 pad4;
    u8 unk5; /* power */
    u8 pad6[2];
    u8 unk8; /* checked against the target by func_8005398C */
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

u8 func_800539FC(Character *, s32);
s32 func_8005398C(Character *, s32);
u8 func_8006576C(MainCalc *);
u8 func_800657F0(MainCalc *);
u8 func_80065850(MainCalc *);
s32 func_801F1A7C(void); /* ovl_37 */
void func_800637C0(s32); /* BIOS B0 call */

void func_80065D90(Character *, s32);
void func_80065DF4(Character *, s32);

/* 12-byte entry of a type-0x12 resource table (func_800633E4) */
typedef struct {
    u16 unk0; /* id */
    u16 unk2;
    u32 unk4 : 24; /* data offset from the entry */
    u32 unk7 : 8;
    u32 unk8;
} DbEntry12;

extern TimHeader D_800834D0;
extern s32 D_8007BCB0;

void func_8002BBE8(s32, u8 *, s32);

/* Inlined copy of func_8002BAE4 (number to fixed-width text). */
static inline void func_8002BAE4_inline(s32 n, u8 *buf, s32 width, u8 ascii) {
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
        func_8002BBE8(n, buf, ascii);
        return;
    }
    func_8001D898(buf, fill, width);
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

typedef void (*FadeFn)(Unk80077C78 *);

/* frame animation cells (func_80035C2C) */
typedef struct {
    s16 unk0; /* cell count */
    s16 unk2; /* byte offset of the first cell in unk10 */
} AnimCellList;

typedef struct {
    Unk800357E0 unk0;
    u8 padC[0x10 - 0xC];
    u16 unk10; /* x scale, 4096 = 1.0 (func_8003595C) */
    u16 unk12; /* y scale */
} AnimCell;

void func_8003595C(s32, s32, s32, Unk80035B90 *, AnimCell *);


/* dialog window layout (func_8004E248 / func_80039660) */
typedef struct {
    s16 unk0; /* x */
    s16 unk2; /* w */
    s16 unk4;
    u16 unk6; /* flags */
    s8 unk8;  /* y */
    u8 unk9;  /* h */
    u8 unkA;  /* anchor */
} WinLayout;

WinLayout *func_8004E248(s32, s32 *, s32 *, s32 *);
void func_80038FD8(s32, WinLayout *, s32, s32);
extern s32 D_8007AE5C;
extern s32 D_8007AE60;
extern s32 D_8007AE64;


s32 func_800631AC(WinDef *);

/* AKAO sequence header (func_8005ACB4) */
typedef struct {
    u8 pad0[4];
    s32 unk4; /* track count */
    s32 unk8; /* id */
    u8 padC[4];
    u32 unk10[1]; /* track offsets */
} AkaoSeqHdr;



/* ---- event script interpreter (func_80039C38) ---- */
/* overlay entry points as called from the main exe (the overlays have their own prototypes) */
#if !defined(OVL_0A_H) && !defined(OVL_0B_H) && !defined(OVL_0C_H) && !defined(OVL_0D_H) && !defined(OVL_14_H) && !defined(OVL_15_H) && !defined(OVL_32_H) && !defined(OVL_33_H) && !defined(OVL_34_H) && !defined(OVL_36_H) && !defined(OVL_37_H) && !defined(OVL_38_H) && !defined(OVL_39_H) && !defined(OVL_3A_H) && !defined(OVL_3B_H) && !defined(OVL_3C_H) && !defined(OVL_3E_H) && !defined(OVL_64_H) && !defined(OVL_65_H)
void func_800AFAC0(s32, s32, s32, s32, s32);
void func_800B1A20(s16, s16, s16, u8 *arg4, u8 *arg5);
s32 func_800B2CC0(s32, s32, s32, s32);
void func_800B5004(s32, s16);
void func_800B73F8(u32, s32);
s32 func_800C02D0(s32, s16, s16, s8, u8, u8, u8);
s32 func_800C0BCC(s32);
s32 func_800C0DB0(s16, s16, s16);
s32 func_800C0DF4(s16, s16, u16, s32);
s32 func_800C0E6C(u16 *arg0, u16 *arg1);
s32 func_800C0E94(s32, s32, s32);
s32 func_800C1024(s16);
s32 func_800C104C(void);
s32 func_800C1100(void);
s32 func_800C119C(s32, s16, s16, s16, s16);
s32 func_800C11C8(s32, s32, s32);
s32 func_800C11E8(s32, s16, s16, s16);
s32 func_800C1368(s32, u8, u8, u8);
s32 func_800C142C(s32, s32);
s32 func_800C147C(s32, s32, s32, s32);
s32 func_800C14DC(s32, s32);
s32 func_800C152C(s32, s32, s32, s32, s32);
s32 func_800C15BC(s32, s32, s32, s32);
s32 func_800C1614(s32, s32, s32);
s32 func_800C1668(s32, s32);
s32 func_800C16B0(s32, s32);
s32 func_800C16F4(s32, s32);
s32 func_800C17AC(s32, s32);
s32 func_800C17F0(s32, s32);
s32 func_800C1868(s32, s32, s32);
s32 func_800C18A0(s32, s32);
s32 func_800C18D4(s32, u32, u32);
s32 func_800C196C(s32, s32);
s32 func_800C1A8C(s32, s16, s16, s16, s16);
s32 func_800C47E4(s32, s32);
s32 func_800C6408(u32, u32);
s32 func_800C7464(u8);
s32 func_800C778C(s32, s32);
s32 func_800C7AF8(u32, u32);
s32 func_800C7B68(u32, s32);
s32 func_800C7BC4(u32, s16);
s32 func_800C7C04(u32, s32);
s32 func_800C7C70(u32, u32, s32);
s32 func_800C7CDC(u32, s32);
s32 func_800C7DF0(u32, s32);
s32 func_800C7F34(s32, s32);
s32 func_800C7F7C(s32, s32);
s32 func_800C7FBC(s32, s32);
s32 func_800C8010(s32, s32);
s32 func_800C8044(s32, s32);
s32 func_800C8068(s32, s16);
s32 func_800C808C(s32, s32);
s32 func_800C80B0(s32, s16, s16);
s32 func_800C80E4(s32, s32);
void func_800F4F04(s32, s32);
void func_801F0FF4(void);
void func_800AA3F4(s32);
s32 func_800C3E18(s32, s32);
s32 func_800C7570(s32, u16);
void func_800B1520(s32, s32, s32, s32);
void func_800CA694(s32, s32, u16);
void func_800BE99C(s32, s32);
void func_800ABA98(s32, s32, s32, s32, s32);
s32 func_800AD944(s32, s32, s32);
VECTOR *func_800AE750(void);
VECTOR *func_800AE744(void);
void func_800C7D30(s32, s32, s32);
#endif

extern u8 *D_8007AEA0;
extern s32 D_8007AE04;
Unk8004AB64 *func_80038150(void);
void func_80038278(Unk8004AB64 *);
s32 func_800500A8(Unk8004AB64 *, s32);
s32 func_80050410(Unk8004AB64 *, s32);
s32 func_80050744(Unk8004AB64 *, s32);

extern s32 D_8007AEA4;
extern Unk8004AB64 *D_8007AE68;
extern s32 D_8007AE6C;
extern s32 D_8007BBD8;
extern s32 D_8007BB7C; /* message counter */
extern u8 D_8006E688[]; /* text speed per config setting */
void func_800398D0(Unk8004AB64 *, s32, s32, s32);
s32 func_80052F24(s32, s32);
void func_8004EC74(u32, s32);
void func_8004ED00(s32, s32);
void func_8004EFA0(void);
WinLayout *func_80039660(s32, s32, s32);
void func_8004DCEC(s32, s32, WinLayout *, s32);

static inline s32 func_8004AF58_inline(void) {
    s32 r;
    if (D_8007B6F4 & 1) {
        func_80052DA0();
        r = func_80052D40();
    } else {
        Unk8004AB64 *pp = D_8007B70C;
        r = *pp->unk0++;
        r |= *pp->unk0++ << 8;
        r = (s16)r;
    }
    D_8007B6F4 >>= 1;
    D_8007B6F8 = 1;
    return r;
}

/* uid of the object the current script runs for */
#define CUR_UID() (((u8 *)((s32 *)D_8007B70C + D_8007B70C->unk9) + D_8007B70C->unkA * 4)[-1])

static inline s32 func_8004AEDC_inline(void) {
    s32 r;
    if (D_8007B6F4 & 1) {
        func_80052DA0();
        r = func_80052D40();
    } else {
        r = *D_8007B70C->unk0++;
    }
    D_8007B6F4 >>= 1;
    D_8007B6F8 = 1;
    return r;
}

extern s32 D_8007B710;
extern s32 D_8007AE24;
extern s32 D_8007AE28;
void func_8004EA4C(s32);
void func_8004E924(void);
void func_8002DD54(void);
void func_8004ED3C(void);
void *func_8004F058(s32);
s32 func_80038990(s32, s32, s32, s32);
void func_8004B9B8(Unk8004B9B8 *);

/* Inlined helper: start the animation set up in unk60/unk61/unk62. */
static inline void func_80039C38_anim(Unk8004AB64 *p, s32 anim) {
    if ((p->unk62 & 0x80) && (p->unkF & 0x80)) {
        func_8004B9B8((Unk8004B9B8 *)p);
    }
    p->unk36 = anim;
    p->unk3A = p->unk60;
    p->unk62 &= ~0x46;
    p->unk62 |= (p->unk60 > p->unk61) ? 0xC0 : 0x80;
    p->unk64 = 0;
    p->unk38 = func_800216D8(p->unk34, anim);
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
        q = (Unk8E0 *)func_8001E350(p->unk3B);
        if (q != NULL) {
            return (Unk8004AFF0 *)q->unk8;
        }
    }
    return NULL;
}

extern u8 D_8007BD2C;
extern s32 D_8007AE50;
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 unk8;
} Unk8007AE40;
extern Unk8007AE40 D_8007AE40;
extern u8 D_8006E658[];
void func_80021660(u16);
void func_80021680();
void func_8004FF98(s32);
void func_80052D0C(u8 **);
Unk16D8 *func_80036978(Unk8004AB64 *);
Unk8004AB64 *func_80036650(s32);
void func_800499D8(Unk8004B9B8 *);

extern s32 D_8007AE2C;
extern s32 D_8007AE30;
extern s32 D_8007AE38;
extern s32 D_8007AE3C;
void func_80050068(s32);
void func_80050080(s32);
void func_8004A9E8(void);
void func_80028074(s32);
void func_800280CC(s32);
s32 func_8006238C(s32);
void func_80033C0C(u32);

/* Inlined copy of func_8004B914 (24-bit script argument). */
static inline s32 func_8004B914_inline(void) {
    s32 v;
    Unk8004AB64 *pp;

    if (D_8007B6F4 & 1) {
        func_80052DA0();
        v = func_80052D40();
    } else {
        pp = D_8007B70C;
        v = *pp->unk0++;
        v |= *pp->unk0++ << 8;
        v |= *pp->unk0++ << 16;
    }
    D_8007B6F4 >>= (D_8007B6F8 = 1);
    return v;
}

extern u8 D_8006E630[];
extern u8 D_8006E63C[];
s32 func_8004E7A4(s32);
void func_800333B0(s32, u8, s32);
s32 func_8003470C(s32, s32);
s32 func_80034220(s32, s32);
s32 func_80053960(Unk800539FC *, s32);
void func_8002412C(s32);
void func_800240B0(u8);
void func_800240F8(u8);
void func_8004BA20(SVECTOR *, s32 *, s32 *);
s32 func_80054FB8(s32);
s32 func_80054FE0(s32);
s32 func_80054E88(s32, s32, s32);
s32 func_80055060(s32, s32, s32);
s32 func_800550C0(s16);
s32 func_800550D0(s32);
s32 func_800550F0(s32, s32);
extern u8 D_8007AE90[];
extern u8 *D_8007AE8C;

typedef struct {
    s32 unk0;
    u8 unk4[4]; /* party character ids */
    u8 unk8[8];
    u8 unk10[9];
} Unk8007AE70;
extern Unk8007AE70 D_8007AE70;

/* Inlined helper: rebuild D_8007AECC->unk16E4 from the current party. */
static inline void func_80039C38_party(void) {
    s32 mask = 0;
    s32 i = 0;
    u8 *p = D_8007AECC->unk16E4;
    s32 r;

    *(s32 *)D_8007AECC->unk16E4 = -1;
    for (; i < 4; i++) {
        r = func_8004E7A4(i);
        if (r >= 0) {
            mask |= 1 << D_8006E63C[r];
        }
    }
    for (i = 0; mask != 0; i++) {
        if (mask & 1) {
            *p++ = D_8007ADFC - 9 + D_8006E630[i];
        }
        mask >>= 1;
    }
}

static inline void func_8004B3A8_inline(s32 a, s32 b) {
    func_800367D0(a, b, 0x14, 0x10)->unk5 = 2;
}

static inline void func_8004B3D0_inline(s32 a, s32 b) {
    Unk800367D0 *p = func_800367D0(a, b, 0x38, 0x10);
    p->unk5 = 3;
    p->unkF = 1;
}

static inline void func_80039C38_newactor(s32 uid, s32 b) {
    Unk8004AB64 *p;

    p = (Unk8004AB64 *)func_800367D0(uid, b, 0xAC, 0x10);
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
    switch (D_8007B708) {
    case 1:
        func_800AA3F4(uid);
        break;
    case 2:
        D_8007AED0[D_8007AE04++] = p;
        break;
    }
}

/* Inlined helper: give the window opened by obj the focus (b = window flags). */
static inline void func_80039C38_window(Unk8004AB64 *obj, s32 b, s32 c) {
    Unk16D8 *n;
    Unk8004AB64 *o;

    if ((b & 0xA0) == 0x80 && obj->unk5 == 4) {
        for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
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
static inline s32 func_80039C38_chr(s32 id) {
    if (id >= 9) {
        id -= 4;
    }
    return id;
}



/* Inlined helpers for the pad binding setup (func_80023784): GameState.unk24 bit 1
 * enables the custom button map in unk28 (4 bits per action). */
static inline s32 func_80023784_custom(u32 *c) {
    return (c[0] >> 1) & 1;
}
static inline s32 func_80023784_remap(s32 k) {
    u32 *c = &D_8006794C->unk24;
    if (func_80023784_custom(c) == 1) {
        return (c[1] >> (k * 4)) & 0xF;
    }
    return k;
}


/* Inlined max helper of func_80054A28 (keeps the running maximum in memory). */
static inline void func_80054A28_max(u32 *m, u32 v) {
    if (*m < v) {
        *m = v;
    }
}

/* primitive buffer / ordering table used by func_800540E8 */
extern u32 *D_8007BD0C;
extern u32 *D_8007BD10;
extern u16 D_8007BD1E;
extern u8 D_8006F308[];
extern u8 D_8006F310[];

/* sound stream header (D_80083168.unk2C + 0x80), func_80061C1C */
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

void func_80056278(s32 a, s32 b);
void func_80061EE4(void);

/* AKAO sample stream state, func_80055E14 */
typedef struct {
    Unk8005EBB4 *unk0; /* destination of the next sample-info copy */
    u32 unk4;          /* SPU transfer address */
    u32 unk8;          /* sample bytes left */
    u32 unkC;          /* sample-info bytes left */
} Unk80083228;
extern Unk80083228 D_80083228;
extern s32 D_80080A30[16]; /* AKAO header copy */
extern u32 D_80083230; /* = D_80083228.unk8 */

/* ovl_0a functions called from the main script interpreter */
s32 func_800C76E8(s32, u16 *, u16 *);
s32 func_800C7EE8(u32, s32 *);

/* party member index -> character slot (9.. map to 5..) */
static inline s32 func_80037ACC_chr(s32 i) {
    if (i >= 9) {
        i -= 4;
    }
    return i;
}

/* func_80037D6C (script system variables) */
extern s32 D_8007AE20;
void *func_80038618(Unk8004AB64 *, s32, s32 *); /* nearest object of a kind */
extern s32 D_8007AEC4;
s32 func_800C1B5C(u8 *);
s32 func_800CA774(void);
s32 func_800B7A14(s32);
s32 func_800BE784(s32);
u32 func_80024048(void);
s32 func_8002414C(void);
s32 func_8006275C(void);
s32 func_8004ECC4(void);

/* random encounter check (func_80038D3C). ovl_0c declares func_800BBBA8 with its own
 * table type, so these are hidden there. */
#ifndef OVL_0C_H
extern s32 D_8007AE4C;
extern u8 D_8006E648[][4];
Unk8007AE40 *func_800BBBA8();

static inline s32 func_80038D3C_rand(void) {
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
    }
    return *D_8007B720++;
}

static inline u16 func_80038D3C_pick(void) {
    Unk8007AE40 *t;
    s32 r;
    s32 i;
    u8 *tbl;

    if (D_8007B708 != 1) {
        t = func_800BBBA8();
    } else {
        t = &D_8007AE40;
    }
    r = func_80038D3C_rand();
    tbl = D_8006E648[t->unk8 & 3];
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

/* start an object's idle animation (func_8004883C) */
static inline void func_8004883C_idle(Unk8004AB64 *q) {
    q->unk60 = 0;
    q->unk61 = 0xFF;
    func_80039C38_anim(q, q->unk74);
}

#endif
