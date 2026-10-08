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
    u8 pad14[0x1C - 0x14];
    s32 unk1C;
    u8 pad20[0x28 - 0x20];
} Unk80021938; /* element passed to func_80021938 */

typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2; /* count */
    s8 unk3;
    Unk80021938 unk4[7];
    s32 unk11C;
} Unk8C8;

typedef struct {
    u8 unk0;
    u8 unk1;
} UnkPair;

typedef struct {
    u8 pad0[4];
    u16 unk4; /* high byte: first text code that takes an extra byte */
    u16 unk6;
    u8 pad8[4];
    u32 unkC[1]; /* glyph info, width in the top 4 bits */
} Unk7D4;

/* Character record: 9 of them at GameData+0x3C; GameData.unk54C[] points at
 * the party members. */
typedef struct {
    u8 unk0[0xA]; /* name: 0xFF-terminated text, read up to 7 chars */
    u8 unkA;      /* bit 4 tested */
    u8 unkB;      /* level (exp table has 99 entries) */
    u32 unkC;     /* exp, capped at 9999999 */
    u16 unk10;    /* hp */
    u8 pad12[0x17 - 0x12];
    u8 unk17;
    u8 pad18[0x1F - 0x18];
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 pad22[0x34 - 0x22];
    u32 unk34; /* low nibble: character id (index into GameData.unk3C);
                * bits 4-8 index D_8006BB8C; bit 11 tested;
                * bits 12-16: ability table row */
    u8 unk38;    /* status; 0x45 = dead/stone/... (can't gain exp) */
    u8 unk39[5]; /* equipment item ids (0xFF = none) */
    u8 pad3E[0x58 - 0x3E];
    u8 unk58[0x30]; /* learned AP per ability slot; cleared by func_800343C0 */
    u32 unk88[2]; /* support abilities equipped: bit (id - 0xC0) */
} Character; /* 0x90 */

typedef struct {
    u8 pad0[4];
    s32 unk4; /* load address */
} Unk1354;

typedef struct {
    u8 pad0[6];
    u8 unk6;
    u8 pad7[0x10 - 7];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    u8 *unk1C; /* load address */
} Unk11C0;

typedef struct {
    u8 pad0[0x11C0];
    Unk11C0 *unk11C0;
    u8 pad11C4[0x1354 - 0x11C4];
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

/* D_8006794C->unk1C: looks like the save-data block */
typedef struct {
    u32 unk0; /* flags */
    u8 pad4[4];
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
    u8 pad55C[0x560 - 0x55C];
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
    UnkPair unk574[0x100]; /* {id, count}: likely the item inventory */
    u8 unk774[0x40];       /* 2 bits per item id */
    u8 pad7B4[0x7D4 - 0x7B4];
    Unk7D4 *unk7D4;
    u8 pad7D8[0x7DC - 0x7D8];
    s16 unk7DC;
    u16 unk7DE;
    s32 unk7E0;
    struct FileTable *unk7E4; /* -1 = none */
    u16 unk7E8;
    u16 unk7EA;
    s32 unk7EC[2];
    s32 unk7F4[2];
    s32 unk7FC;
    s32 unk800;
    s32 unk804;
    s32 unk808;
    u8 *unk80C; /* name text */
    u8 pad810[0x814 - 0x810];
    void *unk814;
    s32 unk818;
    u16 unk81C[2];
    u16 unk820[2];
    u8 unk824[2];
    u8 unk826[2];
    u8 pad828[0x830 - 0x828];
    u16 unk830[2];
    u8 pad834[0x838 - 0x834];
    void (*unk838)(s32, void *);
    void (*unk83C)(s32, s32, s32, s32, s32);
    u16 unk840;
    u16 unk842;
    s32 unk844; /* selected chocograph (1-based, 0 = none) */
    u8 unk848;
    u8 pad849[0x84C - 0x849];
    s32 unk84C;
    u8 unk850;
    u8 pad851[0x854 - 0x851];
    u8 unk854[0x880 - 0x854]; /* CD read context (func_8002177C) */
    Unk880 unk880;
    u8 unk890;
    u8 pad891;
    u8 unk892;
    u8 pad893[0x8AC - 0x893];
    s32 unk8AC;
    u16 unk8B0;
    s16 unk8B2;
    u16 unk8B4[2];
    u8 pad8B8[0x8BC - 0x8B8];
    void *unk8BC; /* callback, signature varies by module */
    u8 pad8C0[0x8C4 - 0x8C0];
    s32 unk8C4; /* VSync(-1) timestamp */
    Unk8C8 *unk8C8;
    u8 pad8CC[0x8E4 - 0x8CC];
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
    u8 pad4C[0xDC - 0x4C];
    s32 unkDC; /* battle data file start sector */
} GameStateUnk14;

typedef struct {
    u32 unk0; /* flags */
    u8 pad4[4];
    u8 unk8;
    u8 unk9; /* VSync mode */
    u8 unkA;
    u8 unkB;          /* disc / pad connection bits, 0xFF = none */
    DISPENV *unkC;    /* [2] */
    DRAWENV *unk10;   /* [2] */
    GameStateUnk14 *unk14;
    u8 pad18[4];
    GameData *unk1C;
    s32 unk20; /* base CD sector added to file start sectors */
    u32 unk24;
    u32 unk28; /* 4-bit fields */
    u32 unk2C; /* frame counter? divided by 60 */
} GameState;

extern GameState *D_8006794C;
void func_8001DE50(GameState *);

/* Inlined copy of func_80027EDC (item search); the original source inlines it. */
static inline UnkPair *func_80027EDC_inline(s32 id) {
    s32 i;
    UnkPair *e = D_8006794C->unk1C->unk574;

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
    u8 pad2[6];
    s32 unk8;
    s32 unkC;
} UnkTimer;

typedef struct {
    s32 unk0; /* VSync(-1) at last update */
    u8 pad[0x404];
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

typedef struct {
    u8 unk0;
    u8 pad1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0xC - 4];
    s32 unkC;
    u8 pad10[0x18 - 0x10];
    s32 unk18;
    u8 pad1C[0x944 - 0x1C];
    u8 unk944;
    u8 pad945[0x958 - 0x945];
    void (*unk958)();
    void (*unk95C)(void *);
    u8 pad960[0x968 - 0x960];
    u8 unk968[4]; /* passed to func_8002267C */
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
    u8 pad6[0xC - 6];
    u8 unkC[3]; /* abilities taught */
    u8 unkF;    /* mask tested against D_8006BB84[] */
    u8 pad10[2];
} Unk80069018; /* item table, 0x12-byte entries */
extern Unk80069018 D_80069018[];
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
    u32 code;
} DrTpageWords;

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
    u8 pad3[9 - 3];
    u8 unk9;
    u8 padA[2];
    u32 unkC; /* bits 0-13: id, bits 14-15: disc */
    s32 unk10; /* frame counter */
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
    u8 pad2[6];
    s32 unk8;
} Unk80077148;
extern Unk80077148 D_80077148;

/* ---- functions (main exe) ---- */

void func_8001C784(void *);
void func_8001D898(s32, s32, s32);
void func_800213A0(void (*)());
void func_80021938(Unk80021938 *);
void func_80023580(u8, u8 *);
void func_800235A0(u8, u8 *);
void func_8002423C(s32, s32, s32, Character *);
u8 *func_80024EB4(s32, u32 *, u8, u8, s32, u8 *);
u8 *func_800253D4(s32, u32 *, u8, u8, s32, s32, u8 *);
void func_8002642C(s16, s16, s32, s32, s32, s32);
u8 *func_80026C7C(s32, s32, u32 *, s32, u8, u8 *);
void func_80026F8C(s32, s32, s32, s32);
u8 *func_80027170(s32, s32, u32 *, s32, s32, u8, u8, u8 *);
void func_800275DC(s32, s32, s32, s32, s32);
void func_80028AF0(s32, s32, s32, s32, u8);

void func_8002BAE4(s32, u8 *, s32, s32);
void func_80027D18(void);
void func_80027D4C(void);
s32 func_80027D84(u8, u8);
void func_80028204(s32, s32, s32, s32, s32, u8);
void func_800283E0(s32, s32, s32, s32, s32, u8);
void func_80028A40(s32, RECT *, RECT *, RECT *);
void func_80028BE8(u8 *, s32);
s32 func_80028DB0(s32, s32, u8);
u8 *func_80029B4C(s32, s32, u32 *, s32, s32, u8 *);
u8 *func_80029D60(s32, s32, u32 *, u8 *, s32, u8 *);
u8 *func_8002B644(s32, s32, u32 *, u8 *, s32, u8 *);
void func_8002B5B4(s32, s32, s32, u8 *, s32);
u8 *func_8002920C(s32, s32, u32 *, u8 *, s32, u8 *);
void func_8002AF70(s16 *, s32);
s32 func_80023938(void);
void func_8002C9B0(void);
void func_8002E180(void);

/* ---- 0x54-byte objects, 10 at D_80077530 (func_8002E260..8002E3F0) ---- */

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    s16 unk8[4];
} Unk16; /* 16 bytes, 2-aligned (copied with lwl/lwr) */

typedef struct {
    u8 unk0[4];
} Unk4; /* 4 bytes, byte-aligned (copied with lwl/lwr) */

/* Menu window object (func_8002DC54 returns one of these). */
typedef struct Window {
    s16 unk0; /* x */
    s16 unk2; /* y */
    s16 unk4; /* w */
    s16 unk6; /* h */
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 pad12[0x18 - 0x12];
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
    u8 pad2C[4];
    Unk16 unk30;
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
extern u8 D_80077DE0;
extern s32 D_8007BB78;
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
    s32 unk18;
    u8 pad1C[0x24 - 0x1C];
    s32 unk24;
    u8 pad28[0x30 - 0x28];
    s32 unk30;
    u32 unk34; /* flags */
    s32 unk38;
    u8 pad3C[0x5C - 0x3C];
    u32 unk5C;
    u8 pad60[0x64 - 0x60];
    s32 unk64;
    u32 unk68;
    u8 pad6C[0x78 - 0x6C];
    s32 unk78;
    u8 pad7C[0x84 - 0x7C];
    s32 unk84;
    u8 pad88[0x94 - 0x88];
    u16 unk94;
    u16 unk96;
    u16 unk98;
    u16 unk9A;
    u8 pad9C[0xA0 - 0x9C];
    u16 unkA0;
    u16 unkA2[4]; /* loop counters */
    u16 unkAA[4];
    u16 unkB2;
    u8 padB4[0xB8 - 0xB4];
    s16 unkB8;
    s16 unkBA;
    u8 padBC[0xBE - 0xBC];
    u16 unkBE;
    s16 unkC0;
    s16 unkC2;
    u16 unkC4;
    u8 padC6[0xC8 - 0xC6];
    s16 unkC8;
    s16 unkCA;
    s16 unkCC;
    u8 padCE[0xD2 - 0xCE];
    s16 unkD2;
    u8 padD4[0xD6 - 0xD4];
    s16 unkD6;
    s16 unkD8;
    u8 padDA[0xE0 - 0xDA];
    s16 unkE0;
    u16 unkE2;
    s16 unkE4;
    u8 padE6[0xE8 - 0xE6];
    u16 unkE8;
    u16 unkEA;
    s16 unkEC;
    u8 padEE[0xF0 - 0xEE];
    u16 unkF0;
    u16 unkF2;
    u16 unkF4; /* loop depth */
    s16 unkF6;
    u16 unkF8;
    s16 unkFA;
    u8 padFC[0x106 - 0xFC];
    s16 unk106;
    s16 unk108;
    s16 unk10A;
    s16 unk10C;
    s16 unk10E;
    s16 unk110;
    s16 unk112;
    s16 unk114;
    u8 pad116[0x11C - 0x116];
    u32 unk11C;
    s32 unk120;
    s32 unk124;
    u8 pad128[0x12A - 0x128];
    u16 unk12A;
    u16 unk12C;
    s16 unk12E;
    u8 pad130[4];
} AkaoChannel; /* 0x134 */

/* frame animation (func_80035B90) */
typedef struct {
    u8 pad0[2];
    u8 unk2; /* ticks */
    u8 pad3[5];
} Unk80035B90Frame;

typedef struct Unk80035B90 {
    u8 unk0; /* active */
    u8 unk1; /* frame */
    u8 unk2; /* tick */
    u8 unk3; /* frame count */
    u8 pad4[4];
    Unk80035B90Frame *unk8;
    u8 padC[0x14 - 0xC];
    void (*unk14)(); /* called with the object */
} Unk80035B90;


typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 pad5[0x8 - 5];
    u16 unk8;
    u16 unkA;
    s32 unkC;
    s32 unk10;
    u8 pad14[0x18 - 0x14];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x24 - 0x20];
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
    u8 pad10[0x18 - 0x10];
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
    u8 padC[0x10 - 0xC];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    u8 pad28[0x30 - 0x28];
    u16 *unk30;
    s32 unk34;
    s32 unk38;
    u8 pad3C[0x48 - 0x3C];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x58 - 0x54];
    s16 unk58;
    u8 pad5A[0x66 - 0x5A];
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

typedef struct {
    u8 pad0[0x38];
    u8 unk38;
} Unk800539FC;

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
    u8 pad0[0xC];
    u8 unkC;
    u8 padD[3];
} Unk80077E44; /* 16-byte entries */
extern Unk80077E44 *D_80077E44;

/* field / world object (actor) */
typedef struct {
    u8 pad0[5];
    u8 unk5; /* kind: 4 = ... */
    u8 unk6;
    u8 unk7;
    u8 pad8[0x11 - 8];
    u8 unk11;
    u8 pad12[2];
    VECTOR unk14; /* position */
    u8 pad24[0x40 - 0x24];
    VECTOR unk40; /* previous position */
} Unk8004AB64;

typedef struct Unk16D8 {
    struct Unk16D8 *unk0; /* next */
    Unk8004AB64 *unk4;
} Unk16D8;

typedef struct {
    u8 pad0[0x16D8];
    Unk16D8 *unk16D8;
    u8 pad16DC[0x16E4 - 0x16DC];
    u8 unk16E4[8];
    u8 unk16EC;
    u8 unk16ED;
    u8 pad16EE[0x16FC - 0x16EE];
    s32 unk16FC;
} Unk8007AECC;
extern Unk8007AECC *D_8007AECC;

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

extern u8 D_80076B0C;
extern s32 D_80077E20;
extern s32 D_8007ADFC;
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
extern s16 D_8007F95A;
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
    u8 unk4;
} Unk80077E50;
extern Unk80077E50 D_80077E50;
extern u8 D_80077E53;
extern s32 D_8007AE54;
extern u8 D_8007954E;
extern u8 D_8007AD1E;
extern s32 D_8007BBA8[8];
extern s32 D_8007BC0C;
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
s32 func_8002F5A0(s32);
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
u16 func_8005C8BC(s32);
void func_80055D80(s32, s32);
void func_8005ECCC(AkaoChannel *, s32);
void func_80060938(void);
void func_80014E20(s32, s32, s32, s32);
void func_8003556C(void (*)(void));

typedef struct {
    u16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
} Unk8007BD38;
extern Unk8007BD38 D_8007BD38;

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
} Unk80080AA0;
extern Unk80080AA0 D_80080AA0;

typedef struct {
    u8 unk0; /* card id, 0xFF = none */
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[2];
} Unk6; /* card inventory entry */

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    Unk6 unk6[105];
} Unk80083248;
extern Unk80083248 D_80083248;

typedef struct {
    u8 pad0[8];
    s16 unk8;
    u8 padA[0x14 - 0xA];
    s32 unk14;
    u8 pad18[0x2C - 0x18];
} Unk8007B9C0;
extern Unk8007B9C0 D_8007B9C0[];

extern u8 D_80077E2F;

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
    s32 unk4[16];
} Unk80073C40;
extern Unk80073C40 D_80073C40;

typedef struct {
    u32 unk0;
    u8 pad4[0x24 - 4];
} Unk80073940; /* 0x24-byte entries */
extern Unk80073940 D_80073940[];

typedef struct {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0xF - 6];
    u8 unkF;
} Unk800367D0;

typedef struct {
    u8 pad0[0x12];
    s16 unk12;
} Unk8006573C;

extern u16 D_80067950;
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
s32 func_80021380(s32);
void func_80021340(void);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[0xC - 8];
    u16 unkC;
    u16 unkE;
} Unk8005EBB4;
extern Unk8005EBB4 D_8007F970[];
void func_8005EBB4(AkaoChannel *, Unk8005EBB4 *, s32);

typedef struct {
    s32 unk0;
    s32 unk4;
    void (*unk8)(void);
    s32 unkC;
} Unk80077E28Entry;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7; /* = D_80077E2F */
    u8 pad8[0x10 - 8];
    s32 unk10; /* loaded file id */
    u8 pad14[0x1C - 0x14];
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
u8 func_80022D3C(s32);

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
} Unk8005AA78;

extern u8 D_8006CAD8[][5];
extern s32 D_800809B8;

void func_8005A658(Unk8005AA78 *);

/* ---- hoisted from overlay headers ---- */

/* file table entry (func_8001E218 / func_80063088) */
typedef struct {
    s32 unk0;
    s32 unk4; /* start sector */
    s32 unk8;
    s32 unkC; /* end sector / next id */
} FileEntry;

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
typedef struct {
    u8 pad0[2];
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

/* visible window of a MenuList (func_8002F344) */
typedef struct {
    u8 unk0;  /* visible rows */
    u8 unk1;  /* first index */
    s16 unk2; /* y */
    u8 pad4[4];
} MenuView;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x68 - 8];
    s32 unk68;
    u8 pad6C[0x7C - 0x6C];
    s32 unk7C;
    u8 pad80[0x88 - 0x80];
    s32 unk88;
    u8 pad8C[0x94 - 0x8C];
    s32 unk94;
    u8 pad98[0xC0 - 0x98];
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
s32 func_80016EC0(s32, s32, s32, s32);
void func_80016ED0(s32);
s32 func_8001C750(s32, s32);
s32 func_8001C8B0(s32, s32, s32, s32);
void func_8001CCB4(s32, s32, s32);
void func_8001CFB4(void);
void func_8001D8A8(s32);
void func_8001D9CC(s32, s32, s32, s32, s32);
void func_8001DAA4(s32, s32, s32, RECT *);
void func_8001DCB8(void);
FileEntry *func_8001E218(s32, s32);
void func_80021640(s32);
void func_8002177C(void *, s32, s32, void *, s32, s32, s32);
s32 func_800217C4(void *);
void func_80021C60(void);
void func_80021CDC(s32);
s32 func_80021CFC(void);
s32 func_800220E8(s32);
void func_80022390(s32);
void func_80022B28(void);
void func_80022BD0(void);
s32 func_80022DF0(); /* TODO: (sector, size, dst); ovl_14 passes a 4th argument */
s32 func_800230A0(void);
u8 func_8002306C(void);
void func_800239D0(void);
void func_80024BD8(void);
void func_8002705C(RECT *, s32, s32);
void func_80027710(s32, s32, s32, void *, s32);
void func_8002AD38(u8 *, u8 *);
s32 func_8002AE7C(s32);
void func_8002B524(s32, s32, s32, s32, s32);
void func_8002B56C(s32, s32, s32, u8 *, s32);
void func_8002B5FC(s32, s32, s32, u8 *, s32);
u8 *func_8002BD54(u8 *, u8 *);
void func_8002C064(s32);
void func_8002C0DC(s32);
void func_8002C308(s32);
void func_8002C4B0(s32);
void func_8002D9B4(RECT *, s32, s32);
Window *func_8002DC54(RECT *, s32, s32);
void func_8002DD00(Window *);
void func_8002DE10(Window *);
void func_8002DE60(void);
void func_8002DEE4(RECT *, s32, s32);
void func_8002E040(RECT *, s32, s32, void *);
void func_8002E2E4(Window *, void (*)(Window *));
void func_8002E2EC(Window *, void (*)(Window *));
void func_8002E2F4(Window *, void (*)(Window *));
void func_8002E2FC(Window *, void (*)(Window *));
void func_8002E304(Window *, s32);
void func_8002E31C(Window *, u8);
void func_8002E334(Window *, s16);
void func_8002EB64(s32, s32);
void func_8002EB74(MenuList *);
void func_8002ECA0(MenuList *, MenuView *);
void func_8002EDFC(MenuList *, s32, s32, s32, s32);
void func_8002EE8C(MenuList *, s32);
void func_8002F044(MenuList *, s32);
void func_8002F0EC(MenuList *, s32);
void func_8002F344(MenuList *, MenuView *);
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
void func_80033CCC(s32, s32);
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
void func_80055E14(s32, s32, s32);
s32 func_80056CAC(s32);
s32 func_80056CBC(s32);
void func_80062344(void);
void func_8006237C(void);
void func_80062420(u8, u8, u8, u8, s32, s32);
s32 func_800625F8(void);
s32 func_80062638(void);
s32 func_80062B3C(void);
s32 func_8006276C(s32); /* number to string */
FileEntry *func_80063088(FileTable *, s32);
void func_80063108(u8 *, u8 *);
/* text window (func_80063118 / func_800631AC) */
typedef struct {
    u8 pad0[4];
    RECT unk4;
    s32 unkC;
} WinDef; /* 0x10 */
void func_80063118(void *, void *, s32);
void func_800631AC(void *);
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

void func_800384D0(s32, s32, s32, s32);
void func_8005A284(Unk80080A10 *, AkaoChannel *, void *);
void func_80062830(Unk6 *, u8, s32, s32);
extern s32 D_8006FB40[];

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Unk80080A18;
extern Unk80080A18 D_80080A18;


extern AkaoChannel D_8007BD48[];
extern AkaoChannel D_8007E3D8[];
extern s32 D_800831C4;

typedef struct {
    s32 unk0;
    u8 pad4[0x10 - 4];
    s32 unk10;
    u8 pad14[0x28 - 0x14];
    u16 unk28;
} Unk80080A70;
extern Unk80080A70 D_80080A70;
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
extern u8 **D_8007B70C;
void func_80052DA0(void);
s32 func_80052D40(void);

s16 func_800BEC64(s32); /* overlay code at 0x800BEC64 */
void func_800363BC(void);
void func_80036444(void);
void func_80036528(void);
void func_8002F9F0(void);

void func_8002267C(void *);
s32 func_8005DD54(s32, s32);
s32 func_8005EC8C(s32, s32);
void func_8005EBE8(AkaoChannel *, s32);
void func_80056644(void *, s32);
void func_800566D0(void);
void func_80056CCC(s32, s32);
void func_800575D0(s32);
s32 StopRCnt(s32);
void SpuQuit(void);
extern s32 D_8006F318;
extern u8 D_8006F334[];
extern s32 D_8007B708;
extern s32 *D_8007B954;

typedef struct {
    u8 pad0[2];
    s16 unk2;
    u8 pad4[9 - 4];
    u8 unk9;
} Unk8004EFB0;

/* ---- main batch 2 ---- */

typedef struct {
    s32 unk0[9];
} Unk24; /* 0x24-byte block copied from D_800761D0.unk944 */
extern Unk24 D_80076178;
extern s32 D_8007B704;
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
void func_80058830(AkaoChannel *, s32, u16);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x48 - 0x18];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x60 - 0x54];
    u32 unk60;
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
void func_8003493C(s32, s32, s32);
extern s32 D_8006E510[];
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
void func_80051004(u8);

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x10 - 0xC];
    s32 unk10;
    u8 pad14[0x34 - 0x14];
    u8 unk34[4];
} Unk80083168;
extern Unk80083168 D_80083168;
void func_80061BC8(void *, s32);
s32 func_80061FBC(s32, s32, s32, void (*)(void));
void func_800576BC(s32, s32);
void func_80062118(void);
void func_800621D4(void);

/* ---- main batch 5 ---- */
s32 func_800219FC(Unk80021938 *);
Unk80021938 *func_80021E20(void);
void func_80028B20(u8 *);
u8 func_80033D28(void);
void func_80012CF8(void);
extern u8 *D_8007B720;
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
extern AbilityEntry D_8006CB28[16][0x30]; /* rows indexed by Character.unk34 bits 12-16 */
void func_80025ED4(s32, s32, s32, s32, s32, s32, s32);
extern u8 D_8007AF00;
void func_80024428(s32, s32, s32, void *);
void func_80026FC4(s32, s32, s32, u8, s32); /* arg4 unused, callers pass 0 */
void func_800273C0(s32, s32, s32, s32, s32);
void func_80033D38(u8 *, s32); /* default name by id */
void func_80015C28(u8 *, u8 *, s32); /* memcpy */
s32 func_8002BCC0(u8 *); /* strlen */
extern u32 D_80077DBC; /* = D_80077DB0.unkC, read on its own by ovl_3a */
extern u32 D_80077520; /* pad buttons (repeat?) */
void func_8002886C(void);
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
s32 func_80062560(u8);
Unk6 *func_800625A0(s32, s32);
void func_800626D0(s32, s32);
typedef struct {
    u8 pad0[4];
    u8 unk4; /* collector points */
} CardInfo; /* 5 bytes */
extern CardInfo D_800121C8[]; /* card table */
CardInfo *func_80062B30(void);
extern s32 D_800704C4;
extern s32 D_80070418;
void func_8003563C(u8);
s32 func_800281A4(s32);
s32 func_800632B4(s32, s8 *);
extern s32 D_800739A8[];
u8 *func_80027834(s32, s32, u32 *, s32, s32, u8 *, u8 *);
u8 func_8003561C(void);
void func_80025A28(s32, s32, s32, s32, s32, s32);
void func_8002EF58(MenuList *, u32);
void func_8002EEF0(MenuList *, u32);
extern u32 D_80077524; /* pad buttons */
extern s32 D_80070430;
extern u8 D_8007AF02;
void func_8003384C(Character *);
s32 func_80033280(s32, s32, u8 *);
void func_80028720(u8 *, s32);
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
void func_80062E3C(s32, s32, s32, s32, s32, s32, s32);
extern s32 D_80073990;
void func_80019194(void);
void func_80032098(void);
void func_80023A4C(void);
void func_8001CD04(s32);
s32 func_8004BC38(u8);
u8 func_80036300(s32);
s32 func_8003631C(s32);
void func_800362C8(void);
void func_80054F80(void);
void func_80050AAC(void);
void func_8005546C(s32, s32);
void func_80056228(void);
s32 func_80024030(void);
s32 func_8002AA04(s32, u8 *, s32);
extern s32 D_800704C0;
extern u8 D_8006A218[]; /* item names */
Unk16D8 *func_8004AE9C(void);
extern s32 D_8007046C;
void SetBackColor(s32, s32, s32);
void SetFarColor(s32, s32, s32);
void SetFogNearFar(s32, s32, s32);
u8 func_8003378C(void);
extern u8 D_80073939;
extern u8 D_8007393A; /* pad analog stick */
extern u8 D_8007393B;
void func_80015C38(const char *, ...); /* printf */

#endif
