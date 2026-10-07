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
    u8 pad2[0x28 - 2];
} Unk80021938; /* element passed to func_80021938 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
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
} Unk7D4;

typedef struct {
    u8 unk0[0xA]; /* name? 0xFF-terminated text, read up to 7 chars */
    u8 unkA;      /* bit 4 tested */
    u8 padB[0x10 - 0xB];
    u16 unk10;
    u8 pad12[0x34 - 0x12];
    u32 unk34; /* low nibble: index into GameData.unk3C */
} Unk54C;

typedef struct {
    u8 pad0[0x34];
    u32 unk34; /* flags; bit 11 tested, bits 4-8 index D_8006BB8C */
    u8 pad38;
    u8 unk39[5]; /* 5 item ids (equipment slots?) */
    u8 pad3E[0x58 - 0x3E];
    u8 unk58[0x30]; /* cleared by func_800343C0 */
    s32 unk88;
    s32 unk8C;
} Character; /* 0x90 bytes, 9 of them at GameData+0x3C: likely characters */

/* D_8006794C->unk1C: looks like the save-data block */
typedef struct {
    u8 pad0[8];
    u8 unk8;
    u8 pad9[0x10 - 9];
    u32 *unk10[2];
    u8 pad18[0x3C - 0x18];
    Character unk3C[9];
    Unk54C *unk54C[4];
    u8 pad55C[0x568 - 0x55C];
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
    u8 pad7D8[0x7E0 - 0x7D8];
    s32 unk7E0;
    u8 pad7E4[0x838 - 0x7E4];
    void (*unk838)(s32, void *);
    void (*unk83C)(s32, s32, s32, s32, s32);
    u8 pad840[0x8B2 - 0x840];
    s16 unk8B2;
    u8 pad8B4[0x8C8 - 0x8B4];
    Unk8C8 *unk8C8;
} GameData;

typedef struct {
    u8 pad0[8];
    u8 unk8;
    u8 pad9[0x1C - 9];
    GameData *unk1C;
    u8 pad20[4];
    u32 unk24;
    u32 unk28; /* 4-bit fields */
    u32 unk2C; /* frame counter? divided by 60 */
} GameState;

extern GameState *D_8006794C;

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
    u8 pad1[0xC - 1];
    s32 unkC;
    u8 pad10[0x18 - 0x10];
    s32 unk18;
    u8 pad1C[0x944 - 0x1C];
    u8 unk944;
    u8 pad945[0x958 - 0x945];
    void (*unk958)();
} Unk800761D0;
extern Unk800761D0 D_800761D0;

/* ---- drawing ---- */

extern u32 *D_8007716C; /* ordering table */
extern u8 *D_80077170;  /* next free primitive in the packet buffer */
extern s16 D_80068F70[];
extern u8 D_8006BB8C[];

typedef struct {
    u8 pad0[0xF];
    u8 unkF; /* mask tested against D_8006BB84[] */
    u8 pad10[2];
} Unk80069018; /* 0x12-byte table entries */
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

extern s32 D_80077140;
extern s32 D_80077144;
extern void (*D_80068F80[])(s32, s32);
extern void (*D_8006BBEC[])(s32);
extern s32 D_8006BBFC[];
extern s32 D_8006BC18[];
extern s32 D_800770AC;
extern s32 D_800770B0[];
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
void func_800235A0();
void func_8002423C(s32, s32, s32, Unk54C *);
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
void func_80027D84(u8, u8);
void func_80028204(s32, s32, s32, s32, s32, u8);
void func_800283E0(s32, s32, s32, s32, s32, u8);
void func_80028A40(s32, RECT *, RECT *, RECT *);
void func_80028BE8(u8 *, s32);
s32 func_80028DB0(s32, s32, u8);
u8 *func_80029B4C(s32, s32, u32 *, s32, s32, u8 *);
u8 *func_80029D60(s32, s32, u32 *, s32, s32, u8 *);
u8 *func_8002B644(s32, s32, u32 *, u8 *, s32, u8 *);
void func_8002B5B4(s32, s32, s32, u8 *, s32);
u8 *func_8002920C(s32, s32, u32 *, s32, s32, u8 *);
void func_8002AF70(s16 *, s32);
s32 func_80023938(void);
void func_8002C9B0(void);
void func_8002E180(void);

/* ---- 0x54-byte objects, 10 at D_80077530 (func_8002E260..8002E3F0) ---- */

typedef struct {
    s16 unk0[8];
} Unk16;

typedef struct {
    u8 unk0[4];
} Unk4; /* 4 bytes, byte-aligned (copied with lwl/lwr) */
 /* 16 bytes, 2-aligned (copied with lwl/lwr) */

typedef struct Unk80077530 {
    u8 pad0[0x10];
    s16 unk10;
    u8 pad12[0x1A - 0x12];
    u8 unk1A;
    u8 unk1B; /* index */
    u8 unk1C; /* state: 2 = fire callback */
    u8 unk1D;
    u8 pad1E;
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
    s32 unk40;
    s32 unk44;
    s32 unk48;
    void (*unk4C)(struct Unk80077530 *);
    u8 pad50[4];
} Unk80077530;
extern Unk80077530 D_80077530[10];

void func_8002CF20(void *, s32); /* window, see func_8002D1AC */
void func_8002D1AC(void *, s32); /* window: Unk80077530 (main) / Window (ovl_65), to unify */
void func_8002D398(Unk80077530 *, s32, s32);

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
extern u8 D_800121C8[];
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
    u8 pad4[0x14 - 4];
    u8 *unk14; /* return address? */
    u8 pad18[0x34 - 0x18];
    u32 unk34; /* flags */
    u8 pad38[0x78 - 0x38];
    s32 unk78;
    u8 pad7C[0x94 - 0x7C];
    u16 unk94;
    u16 unk96;
    u16 unk98;
    u16 unk9A;
    u8 pad9C[0xB2 - 0x9C];
    u16 unkB2;
    u8 padB4[0xB8 - 0xB4];
    s16 unkB8;
    s16 unkBA;
    u8 padBC[0xBE - 0xBC];
    u16 unkBE;
    s16 unkC0;
    u8 padC2[0xC4 - 0xC2];
    u16 unkC4;
    u8 padC6[0xC8 - 0xC6];
    s16 unkC8;
    s16 unkCA;
    s16 unkCC;
    u8 padCE[0xE2 - 0xCE];
    u16 unkE2;
    u8 padE4[0xEA - 0xE4];
    u16 unkEA;
    u8 padEC[0xF0 - 0xEC];
    u16 unkF0;
    u16 unkF2;
    u8 padF4[0xF6 - 0xF4];
    s16 unkF6;
    u16 unkF8;
    s16 unkFA;
    u8 padFC[0x106 - 0xFC];
    s16 unk106;
    u8 pad108[0x10E - 0x108];
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
} AkaoChannel;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3[0x14 - 3];
    s32 unk14;
} Unk80035B74;

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
} Unk80077DE8;
extern Unk80077DE8 D_80077DE8;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x34 - 0xC];
    s32 unk34;
    s32 unk38;
    u8 pad3C[0x6C - 0x3C];
    u16 unk6C;
    u8 pad6E[2];
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

typedef struct {
    u8 pad0[6];
    u8 unk6;
} Unk8004AB64;

typedef struct {
    u8 pad0[0x16D8];
    s32 unk16D8;
    u8 pad16DC[0x16E4 - 0x16DC];
    u8 unk16E4[8];
    u8 unk16EC;
    u8 pad16ED[0x16FC - 0x16ED];
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
extern u32 D_800739A0;
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
    u8 pad0[4];
    s32 unk4;
} Unk800219FC;

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
extern s32 D_8007F790;
extern s16 D_8007F95A;
extern s32 D_8008322C;
extern s32 D_80083158;
extern u8 D_80077E50;
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
void func_80030770(void *);
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
extern u8 D_8007AEF0[];

void func_800239A4(void);
void func_80023784(void);
void func_800325A4(void);
void func_8003264C(void);
s32 func_800357C0(u8 *, s32);
s16 func_8002BA68(s32);
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
    u8 unk0;
    u8 pad1[5];
} Unk6;

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
u16 *func_8001C7FC(s32, s32, s32);
void func_8002CD4C(s32, s32, s32, s32);
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
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0x20 - 6];
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
    u8 pad1[0xC - 1];
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
Unk80021938 *func_80021E5C(void);
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

#endif
