#ifndef OVL_0B_H
#define OVL_0B_H

/* Declarations local to overlay 0x0B (battle, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* ---- battle units ---- */

typedef struct BtlUnitA0 {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 pad26[0x3A - 0x26];
    u8 unk3A;
    u8 unk3B;
} BtlUnitA0;

/* model mesh group: primitive counts per type */
typedef struct {
    u8 pad0[2];
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u8 pad12[0x14 - 0x12];
    u16 *unk14;
    SVECTOR *unk18;
    u32 *unk1C; /* primitives */
    u8 pad20[0x28 - 0x20];
} BtlMesh; /* 0x28 */

typedef struct BtlUnit9C8 {
    u8 pad0[2];
    u8 unk2;
    u8 unk3; /* count of unk10 */
    u8 pad4[8 - 4];
    u16 unk8;
    u16 unkA;
    u8 padC[0x10 - 0xC];
    BtlMesh *unk10;
} BtlUnit9C8;

typedef struct BtlUnit9CC {
    u8 pad0[2];
    u16 unk2;
    u8 pad4[0xC - 4];
    s32 unkC; /* offset, relocated on load */
    s32 unk10; /* offset, relocated on load */
} BtlUnit9CC;

typedef struct {
    s32 unk0[5];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} BtlBone; /* 0x20 */

typedef struct BtlUnit9C50 {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0xC - 5];
    s32 unkC;
    u8 pad10[0x12 - 0x10];
    s16 unk12;
} BtlUnit9C50; /* 0x14 */

typedef struct {
    s16 unk0;
    s16 unk2; /* tpage */
} BtlUnit9C18;

/* FileEntry viewed with its first word split (func_8001E218 type 0xD) */
typedef struct {
    u16 unk0;
    u16 unk2;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} FileEntry16;

/* double buffer used by the standalone display loop func_800DC510 */
typedef struct {
    DRAWENV unk0;
    DISPENV unk5C;
    s16 unk70;
    u8 pad72[0x74 - 0x72];
} Unk80100E30Buf; /* 0x74 */

typedef struct {
    Unk80100E30Buf unk0[2];
    Unk80100E30Buf *unkE8; /* current */
} Unk80100E30;
extern Unk80100E30 D_80100E30;

/* model animation event list (func_8001C8B0 section 2, tag 0x14) */
typedef struct {
    union {
        s32 w; /* sound id */
        struct {
            u8 pad0[2];
            u8 unk2; /* kind */
        } b;
    } unk0;
    u8 pad4[0x10 - 4];
} BtlAnimEvt; /* 0x10 */

typedef struct {
    u16 unk0;
    u16 unk2; /* count */
    BtlAnimEvt unk4[1];
} BtlAnimEvtList;

/* battle task with a step counter (func_800AE618) */
typedef struct {
    u8 pad0[6];
    s16 unk6; /* step */
} BtlStepTask;

/* work area of the effect step func_800E5538 */
typedef struct {
    u8 unk0;
    u8 unk1; /* phase */
    u8 unk2; /* unit id */
    u8 unk3;
    u8 pad4[0xC - 4];
    s16 unkC[8];
    s16 unk1C[6];
} Unk800E5538; /* 0x28 */

void func_800E436C(s32, s16 *, s32, s32, s32);
s32 func_800E43D8(s32, s32, s16 *, s32);
s32 func_800E84F0(s32);

typedef struct {
    u8 pad0[7];
    u8 unk7; /* count */
    s32 unk8; /* index into D_800FDD10 */
} Unk800FD6E0;
extern Unk800FD6E0 D_800FD6E0;
extern u8 D_800F8D84[]; /* ability ids */

/* main exe: candidates for ff9.h */
typedef struct {
    u8 unk0;
    u8 unk1; /* ap needed */
} Unk8006CB88;
extern Unk8006CB88 D_8006CB88[]; /* abilities */

/* draw environment pair built in the primitive buffer by func_800E6E1C */
typedef struct {
    DRAWENV unk0;
    DRAWENV unk5C; /* previous */
} DrawEnvPair; /* 0xB8 */

/* next byte of the random table, refilling it when exhausted (inlined) */
static inline s32 rand8_inline(void) {
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
    }
    return *D_8007B720++;
}

/* one primitive in a model's packed GPU packet buffer (BtlUnit9C.unk10) */
typedef union {
    P_TAG tag;
    u32 w[13]; /* w[1], w[3]/w[4], ...: rgb + code words */
} BtlPrim;

typedef struct BtlUnit9C {
    u16 unk0; /* flags */
    u8 pad2[4 - 2];
    u8 unk4;
    u8 unk5;
    u8 pad6[8 - 6];
    BtlUnit9C8 *unk8;
    BtlUnit9CC *unkC;
    s32 unk10;
    u32 unk14;
    BtlUnit9C18 *unk18;
    struct BtlUnit9C *unk1C;
    BtlBone *unk20;
    MATRIX unk24;
    u8 pad44[0x50 - 0x44];
    BtlUnit9C50 *unk50;
    s16 unk54;
    s16 unk56;
    s16 unk58;
    s16 unk5A;
    BtlUnit9C8 *unk5C; /* morph target */
} BtlUnit9C;

typedef struct BtlUnit98 {
    s32 unk0[3];
} BtlUnit98;

typedef struct {
    u16 unk0; /* max hp */
    s16 unk2; /* max mp (<< 4) */
    s16 unk4; /* max atb */
} BtlUnit24;

/* current stats at BtlUnit.unk28, parallel to BtlUnit24 */
typedef struct {
    u16 unk0; /* hp */
    s16 unk2; /* mp */
    s16 unk4; /* atb */
    s8 unk6;  /* atb speed */
} BtlUnit28;

/* battle unit; 8 of them at Battle.unk6FC, linked through unk0 */
typedef struct BtlUnit {
    struct BtlUnit *unk0; /* next */
    struct BtlCmd *unk4;
    struct BtlCmd *unk8;
    struct BtlCmd *unkC;
    struct BtlCmd *unk10;
    struct BtlCmd *unk14;
    s32 unk18;
    u16 unk1C[4];
    BtlUnit24 *unk24;
    u16 unk28; /* hp */
    s16 unk2A; /* mp */
    s16 unk2C;
    s8 unk2E;
    u8 pad2F;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    s32 unk34;
    u32 unk38; /* status */
    u32 unk3C; /* status */
    s16 unk40[3];
    s16 unk46[16]; /* status timers */
    u8 pad66[0x68 - 0x66];
    u32 unk68; /* flags: bit 0 player, bits 1-4 character slot, bits 5-7 id */
    u8 unk6C;
    u8 unk6D;
    u8 unk6E;
    u8 unk6F;
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 unk73;
    u8 *unk74;
    u8 unk78;
    u8 unk79;
    u8 unk7A;
    u8 unk7B;
    u8 unk7C;
    u8 unk7D;
    u16 unk7E;
    s16 unk80;
    s16 unk82;
    u16 unk84; /* model id */
    s16 unk86[3];
    s32 unk8C[3]; /* position, 16.16 */
    struct BtlUnit98 *unk98;
    BtlUnit9C *unk9C;
    BtlUnitA0 *unkA0;
    s32 *unkA4;
    BtlUnit9C *unkA8;
    u16 unkAC;
    u16 unkAE;
    u16 unkB0;
    u8 unkB2;
    u8 unkB3;
    struct Unk80104F88Entry *unkB4; /* menu data (party members) */
    s16 unkB8; /* screen x */
    s16 unkBA; /* screen y */
    u8 unkBC; /* on screen */
    u8 unkBD;
    u8 unkBE;
    u8 unkBF;
    u8 unkC0;
    u8 unkC1[3];
    u32 *unkC4;
    s16 unkC8; /* hp change shown */
    s16 unkCA;
    s16 unkCC;
    u8 unkCE;
    u8 unkCF;
} BtlUnit; /* 0xD0 */

/* ability/command calculation context */
typedef struct BtlCalc8 {
    u8 pad0[8];
    u8 *unk8;
    u16 unkC;
    u8 unkE;
    u8 unkF;
    u32 unk10;
} BtlCalc8;

typedef struct BtlCalc {
    BtlUnit *unk0; /* caster */
    BtlUnit *unk4; /* target */
    BtlCalc8 *unk8;
    u8 padC[0xE - 0xC];
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u16 unk18; /* result flags */
    u8 unk1A;
    u8 unk1B;
    u8 pad1C[0x1E - 0x1C];
    s16 unk1E;
} BtlCalc;

/* battle script interpreter context */
typedef struct {
    u8 pad0[8];
    struct BtlCmd8 *unk8;
    u16 unkC;
} BtlScript4;

typedef struct BtlScript {
    u32 unk0; /* flags */
    BtlScript4 *unk4;
    u8 *unk8; /* script pointer */
    u8 padC[0x10 - 0xC];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 unk16;
    u8 unk17;
    s16 unk18;
    u8 pad1A;
    u8 unk1B;
    u16 unk1C;
    s16 unk1E;
    u8 unk20;
    u8 unk21;
    s16 unk22;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
    s16 unk28[8]; /* per-command work area */
} BtlScript;

/* D_800F78A4->unkC */
typedef struct {
    BtlUnit *unk0;     /* id 0x10 */
    BtlUnit *unk4[8];  /* ids 0..7 */
    u8 unk24;          /* count of unk4 */
    u8 unk25[2];
    u8 unk27;          /* count of unk28 */
    BtlUnit *unk28[4]; /* ids 8.. */
    s16 unk38;
    s16 unk3A;
    s16 unk3C;
    u8 pad3E[0x40 - 0x3E];
    u16 unk40;
    u16 unk42;
    BtlUnit *unk44;    /* id 0x11 */
    u8 pad48[0x4C - 0x48];
} BtlUnitList; /* 0x4C */

typedef struct Unk800F78A40 {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x8 - 4];
    s16 unk8;
    u8 padA[0xF - 0xA];
    u8 unkF;
    u8 pad10[0x1C - 0x10];
    u32 unk1C;
} Unk800F78A40;

typedef struct {
    u8 pad0[0x30];
    s32 *unk30[1]; /* offset tables, entries relative to their start */
} Unk800F78A44;

typedef struct {
    Unk800F78A40 *unk0;
    Unk800F78A44 *unk4;
    u8 pad8[0xC - 8];
    BtlUnitList *unkC;
    u8 pad10[0x14 - 0x10];
    MATRIX *unk14;
    DISPENV *unk18;
    DRAWENV *unk1C;
    s32 *unk20; /* ordering table */
    u8 *unk24; /* next free primitive */
    s32 unk28;
    struct Unk800FE3B8 *unk2C;
    s32 unk30;
} Unk800F78A4;
extern Unk800F78A4 *D_800F78A4;

typedef struct {
    s32 unk0;
    s8 unk4[4];
    s8 unk8[4];
} Unk800EB518;

/* animated hp/mp readout */
typedef struct {
    s16 unk0; /* step */
    s16 unk2; /* shown */
    s16 unk4; /* target */
    u8 unk6; /* frames left */
    u8 pad7;
} Unk80104F88Gauge;

typedef struct Unk80104F88Entry {
    Unk80104F88Gauge unk0[2]; /* hp, mp */
    struct BtlUnit *unk10;
    Character *unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 *unk1C[0xC0]; /* -> Character.unk58[] */
    u8 unk31C[0xC0];
    u8 unk3DC[0xC0]; /* abilities available from equipment */
    Unk800EB518 unk49C;
} Unk80104F88Entry; /* 0x4A8, one per party member */

typedef struct {
    Unk80104F88Entry unk0[4];
    struct BtlUnit *unk12A0;
    Window *unk12A4[4];
    u8 unk12B4;
    u8 unk12B5; /* party count */
    u8 unk12B6;
    u8 unk12B7;
    union {
        u8 b[2]; /* set separately by func_800EAC88 */
        u16 h;   /* tested together */
    } unk12B8;
    u8 pad12BA[0x12BC - 0x12BA];
    s32 unk12BC;
    u32 unk12C0;
    u8 unk12C4; /* queue count */
    u8 unk12C5; /* queue head */
    u8 unk12C6;
    u8 pad12C7;
    struct BtlUnit *unk12C8[4];
} Unk80104F88;
extern Unk80104F88 *D_80104F88;

typedef struct {
    s16 unk0;          /* count of unk4 */
    u16 unk2;          /* count of unk80 */
    UnkPair *unk4[31]; /* inventory entries, item ids >= 0xE0 */
    UnkPair *unk80[0x58];
    u8 unk1E0[0x100];
} Unk80104FA8;
extern Unk80104FA8 *D_80104FA8;

/* battle menu / frame state */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    s8 unk3;
    u8 unk4;
    u8 pad5;
    u8 unk6;
    u8 pad7;
    u8 unk8;
    u8 unk9;
    u8 padA;
    s8 unkB;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
    u8 unk10;
    u8 pad11;
    u8 unk12;
    u8 unk13;
    s8 unk14;
    u8 unk15;
    u8 pad16[0x18 - 0x16];
    s32 unk18;
    s32 unk1C;
} Unk80104E68;
extern volatile Unk80104E68 D_80104E68;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    union {
        struct {
            u8 unk0[3];
            u8 unk3; /* count of unk0 */
        } b;
        u16 h[2]; /* cleared as two halfwords (func_800E5EAC) */
    } unkA;
    u8 unkE;
    u8 unkF;
    s32 unk10;
    u8 pad14[0x18 - 0x14];
    s32 unk18;
    s32 unk1C;
    u8 unk20;
    u8 pad21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
} Unk800FEC40;
extern Unk800FEC40 D_800FEC40;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
    u8 pad10[0x1A - 0x10];
    u16 unk1A;
    u8 pad1C[0x2C - 0x1C];
    union {
        s16 h[2];
        s32 w; /* set to -1 as a word */
    } unk2C;
    u8 pad30[0x38 - 0x30];
    s32 unk38;
    struct Heap *unk3C[2];
    u8 pad44[0x50 - 0x44];
    s16 unk50;
    u8 unk52;
    u8 unk53;
    struct Unk80100DC054 *unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 unk69;
} Unk80100DC0;
extern Unk80100DC0 D_80100DC0;

typedef struct {
    s32 unk0;
    struct Unk80104FD4 *unk4;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    struct BtlUnit *unk14;
    u8 unk18;
    u8 unk19;
} Unk80104FD0;
extern Unk80104FD0 D_80104FD0;

extern u8 D_800FE470;
extern s32 D_800F7270;
extern s32 D_800F7274;
extern MATRIX D_800F8D50;
void func_800CEEC8(s32);
extern s32 D_800FE50C;
extern s32 D_800FE510;
extern struct Unk800CEE74 *D_800FE514;
extern s32 D_800FE51C;
extern s32 D_800FE4F0;
extern struct BtlUnit9C50 *D_800FE4F4;
extern s32 D_800FE4F8;
extern u8 *D_800FE504;
extern s32 D_800FE508;
extern s32 D_800FE518;
extern s16 D_80100DCC;
extern s32 *D_80104DF8; /* offset table, entries relative to its start */
extern u8 D_80104FFC;
typedef struct {
    s16 unk0;          /* count of unk14 */
    s16 unk2;          /* count of unk4 */
    struct BtlUnit *unk4[4]; /* player units */
    s16 unk14[8][2];   /* screen positions */
} Unk80105020;
extern Unk80105020 D_80105020;
typedef struct {
    u8 unk0;
    u8 unk1;
} Unk80105058;
extern Unk80105058 D_80105058;
extern s32 D_80105274;
extern u8 D_8010527C;
extern s32 D_8010528C[];

typedef struct {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    BtlUnit24 unk4; /* max stats */
    u8 padA[0xC - 0xA];
    BtlUnit9CC *unkC[6];
    u16 unk24;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29[3];
    s32 unk2C;
    u16 unk30;
    u8 unk32[6];
    u8 unk38[6];
    u8 unk3E[6];
    u16 unk44; /* exp */
    u16 unk46; /* gil */
    union {
        s32 w;
        u8 b[4]; /* item drops, rarest last */
    } unk48;
    s32 unk4C; /* card */
} Battle4880; /* 0x50 */

typedef struct {
    Battle4880 *unk0;
    u32 unk4;
    union {
        s32 w;
        u8 b[4]; /* item drops, -1 = none */
    } unk8;
    u8 unkC[0x18 - 0xC]; /* position block (BtlUnit.unk98) */
} Battle488; /* 0x18 */

typedef struct {
    void *unk0[4];
    void *unk10[4];
    void *unk20[4];
    void *unk30[4];
} Battle1354;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xC - 2];
} Battle154C; /* 0xC */

typedef struct Battle1538 {
    u8 pad0[0x20];
    s32 unk20;
    s32 unk24;
} Battle1538;

typedef struct {
    s32 unk0;
    struct BtlCmd *unk4;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 unk16;
    u8 unk17;
    u8 pad18[0x21 - 0x18];
    u8 unk21;
    u8 pad22[0x25 - 0x22];
    u8 unk25;
    u8 unk26;
    u8 pad27[0x38 - 0x27];
} Battle122C; /* 0x38 */

typedef struct Battle11C4 {
    BtlUnit9CC **unk0; /* animations */
    u8 *unk4;
    s32 unk8[24]; /* model data */
    Battle122C unk68[4];
    struct BtlUnit *unk148;
    s32 unk14C;
    u16 unk150;
    s16 unk152;
    u16 unk154;
    u8 pad156[0x158 - 0x156];
} Battle11C4; /* 0x158 */

typedef struct {
    u8 unk0; /* enemy type */
    u8 unk1;
    u8 pad2[2];
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} Unk800C9EE0;

typedef struct {
    u8 unk0; /* weight */
    u8 unk1; /* count of unk8 */
    u8 unk2;
    u8 pad3;
    u16 unk4;
    u8 pad6[8 - 6];
    Unk800C9EE0 unk8[4]; /* enemy placements */
} Battle1330; /* 0x38 */

typedef struct {
    u8 pad0[0x1E];
    u16 unk1E;
    u8 pad20[0x62 - 0x20];
    u16 unk62;
    u8 pad64[0x74 - 0x64];
} Battle1334; /* 0x74 */

typedef struct Battle11C020 {
    u8 unk0;
    u8 unk1; /* count of unk8 */
    u8 unk2; /* count of enemy records following unk8 */
    u8 unk3; /* count of commands following the enemy records */
    u16 unk4;
    u8 pad6[8 - 6];
    Battle1330 unk8[1];
} Battle11C020;

/* battle command (queued action) */
typedef struct BtlCmd8 {
    u32 unk0_0 : 4;
    u32 unk0_4 : 4;
    u32 unk0_8 : 9;
    u32 unk0_17 : 15;
    u8 unk4;
    u8 pad5;
    u8 unk6;
    u8 pad7;
    u8 unk8;
    u8 pad9;
    u8 unkA; /* mp cost */
    u8 unkB;
    u8 padC[0xE - 0xC];
    u16 unkE; /* name text offset */
} BtlCmd8;

typedef struct BtlCmd {
    struct BtlCmd *unk0; /* next */
    BtlUnit *unk4;
    BtlCmd8 *unk8;
    s16 unkC;
    u8 unkE;
    u8 unkF;
    union {
        struct {
            u32 unk0_0 : 2;
            u32 unk0_2 : 2;
            u32 unk0_4 : 1;
            u32 unk0_5 : 1;
            u32 unk0_6 : 1;
            u32 unk0_7 : 2;
            u32 unk0_9 : 1;
            u32 unk0_10 : 1;
            u32 unk0_11 : 1;
        } b;
        u32 w; /* cleared as a word */
    } unk10;
} BtlCmd;

typedef struct Battle11C028 {
    u32 unk0_0 : 8;
    u32 unk0_8 : 12;
    u32 unk0_20 : 12;
    u32 unk4_0 : 12;
    u32 unk4_12 : 10;
    u32 unk4_22 : 10;
    u8 pad8[0x28 - 8];
} Battle11C028; /* 0x28 */

typedef struct Battle11C0 {
    s32 unk0;
    s16 unk4;
    u8 unk6;
    u8 unk7[1];
    s32 unk8;
    s32 *unkC; /* offset table, entries relative to its start */
    s32 unk10;
    u8 *unk14;
    struct Unk800CEE74 *unk18;
    s32 unk1C;
    Battle11C020 *unk20;
    u16 *unk24; /* two offsets: model file table, offset table */
    struct Battle11C028 *unk28;
    s32 unk2C[2]; /* per draw buffer */
    s32 unk34[2]; /* per draw buffer */
    s32 *unk3C;
    BtlUnit9C50 *unk40;
    u8 *unk44; /* work heap start */
    u8 *unk48; /* work heap cursor */
} Battle11C0;

typedef struct {
    u32 unk0_0 : 4;
    u32 unk0_4 : 1;
    u32 unk0_5 : 3;
    u32 unk0_8 : 21;
    u32 unk0_29 : 1;
    u32 unk0_30 : 1;
    u32 unk0_31 : 1;
    u8 pad4[8 - 4];
    u8 unk8;
    u8 pad9;
    u8 unkA; /* mp cost */
    u8 padB[0xE - 0xB];
    u16 unkE;
} Battle16CC; /* 0x10 */

/* Battle.unk1358 ring entry (func_800CC278 hands out the next one) */
typedef struct {
    u8 *unk0; /* animation, 0xFF-terminated */
    u16 unk4; /* glyph index into D_800686D0 */
    u8 pad6[8 - 6];
} Unk800F6A54;

typedef struct Battle1358 {
    struct BtlUnit *unk0;
    u8 pad4[8 - 4];
    s16 unk8;
    s16 unkA;
    s16 unkC;
    u16 unkE; /* frame */
    union {
        s32 w; /* cleared as a word */
        struct {
            s8 unk0;
            s8 unk1; /* y offset */
            u8 unk2;
            u8 unk3;
        } b;
    } unk10;
    union {
        u32 w; /* packed digits (func_800CC444) */
        Unk800F6A54 *p;
    } unk14;
    u16 unk18;
    u8 pad1A[0x1C - 0x1A];
} Battle1358; /* 0x1C */

typedef struct {
    Battle1358 unk0[16];
    s16 unk1C0; /* last entry handed out (counts down, wraps to 15) */
    s16 unk1C2;
} Battle1358Ring; /* Battle.unk1358 */

/* Inlined copy of func_800B3CEC (clear a command); the original source inlines it. */
static inline void func_800B3CEC_inline(BtlCmd *arg0) {
    arg0->unk0 = 0;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unkE = 0;
    arg0->unkF = 0;
    arg0->unk10.b.unk0_0 = 0;
    arg0->unk10.b.unk0_2 = 0;
    arg0->unk10.b.unk0_4 = 0;
    arg0->unk10.b.unk0_5 = 0;
    arg0->unk10.b.unk0_6 = 0;
    arg0->unk10.b.unk0_7 = 0;
    arg0->unk10.b.unk0_9 = 0;
    arg0->unk10.b.unk0_10 = 0;
    arg0->unk10.b.unk0_11 = 0;
}

typedef struct Battle108C {
    DR_TPAGE unk0;
    POLY_F3 unk8;
} Battle108C; /* 0x1C */

/* battle scene data (Battle.unk132C) */
typedef struct {
    u32 unk0; /* flags */
    Battle1330 *unk4;
    Battle1334 *unk8;
    u16 *unkC; /* model file table */
    s16 *unk10; /* offset table, entries relative to its start */
} Battle132C;

/* GameData.unk8E4 while in battle */
typedef struct Battle {
    u16 unk0; /* flags */
    u8 unk2[2];
    s32 unk4;
    s32 unk8[8][0x22]; /* per unit: loaded file addresses */
    u8 pad448[0x488 - 0x448];
    Battle488 unk488[4];
    Battle4880 unk4E8[3];
    struct BtlCmd8 unk5D8[18]; /* commands/abilities */
    BtlUnit *unk6F8; /* list of active units */
    BtlUnit unk6FC[8];
    s32 unkD7C;
    u8 unkD80;
    u8 unkD81;
    u8 unkD82;
    u8 unkD83;
    u8 unkD84;
    u8 unkD85;
    u8 unkD86;
    u8 unkD87;
    u8 padD88[0xD90 - 0xD88];
    BtlCmd unkD90[36]; /* per unit: 6 (players) or 3 (enemies) */
    BtlCmd unk1060;
    BtlCmd unk1074;  /* list head (only unk0 used) */
    BtlCmd *unk1088; /* current command */
    struct Battle108C unk108C[2][4];
    BtlUnitList unk116C;
    u16 unk11B8;
    u8 unk11BA;
    u8 unk11BB;
    s16 unk11BC;
    u16 unk11BE;
    struct Battle11C0 *unk11C0;
    struct Battle11C4 unk11C4;
    u8 unk131C;
    u8 pad131D[0x132C - 0x131D];
    Battle132C unk132C;
    u8 unk1340;
    u8 pad1341[0x1354 - 0x1341];
    Battle1354 *unk1354;
    Battle1358Ring unk1358;
    s32 unk151C;
    s32 unk1520;
    Battle488 *unk1524;
    u8 unk1528;
    u8 unk1529;
    u8 pad152A[0x152C - 0x152A];
    u8 unk152C;
    u8 pad152D[0x1530 - 0x152D];
    s32 unk1530;
    s32 unk1534; /* battle data file sector */
    struct Battle1538 **unk1538;
    s32 unk153C[4];
    Battle154C unk154C[19];
    u8 pad1630[0x1638 - 0x1630];
    u8 pad1638[0x1692 - 0x1638];
    u16 unk1692;
    u8 pad1694[0x16CC - 0x1694];
    Battle16CC unk16CC[0xC0];
    s32 unk22CC[64];
} Battle; /* 0x23CC */

/* battle work area, allocated at D_80067944 + 0x61800 */
typedef struct {
    Battle unk0;
    u8 unk23CC[0x240C - 0x23CC];
    u8 unk240C[0x241C - 0x240C];
    u8 unk241C[1]; /* becomes D_80067948 */
} BattleMem;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Unk80100D88;
extern Unk80100D88 D_80100D88;


typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u8 pad6;
    u8 unk7;
} Unk800BD8C0;

typedef struct {
    u16 unk0;
    u16 unk2;
} U16Pair;

extern BtlUnit *D_800FE484;

typedef struct {
    s16 unk0[4];
    u8 unk8;
    u8 unk9;
    u8 padA[0xC - 0xA];
} Unk800C412C0; /* 0xC */

typedef struct {
    Unk800C412C0 unk0[2];
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x20 - 0x1E];
} Unk800C412C; /* 0x20 */

typedef struct Unk800FE3B8 {
    SVECTOR unk0[6];
    SVECTOR unk30[4];
    s16 unk50;
    u8 unk52;
    u8 unk53;
    SVECTOR unk54;
    u8 unk5C;
    u8 unk5D;
    u16 unk5E;
    u16 unk60;
    u8 pad62[0x65 - 0x62];
    u8 unk65;
    u8 unk66;
    u8 unk67;
    Unk800C412C unk68[2];
    s32 unkA8;
    u32 unkAC;
    s32 unkB0;
    s32 unkB4;
    u8 unkB8;
    u8 unkB9;
    u8 unkBA;
    u8 unkBB;
    SVECTOR unkBC;
    SVECTOR unkC4;
    BtlUnit *unkCC;
} Unk800FE3B8;
extern Unk800FE3B8 D_800FE3B8;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    u8 unkA;
    u8 unkB;
    u8 unkC[16];
    u8 unk1C;
    u8 unk1D;
} Unk800FDF30;
extern Unk800FDF30 D_800FDF30;



typedef struct {
    u8 pad0[0xC];
    s32 unkC;
} Unk800DA1B88;

typedef struct Unk800DA1B8 {
    s16 unk0; /* entry stride in bytes */
    s16 unk2; /* entry count */
    s32 (*unk4)(s32, void *, Unk800F78A4 *, Unk800DA1B88 *, s32, struct Unk800DA1B8 *, s32);
    Unk800DA1B88 *unk8;
    s32 unkC[1]; /* entries: s16 in use, s16, data */
} Unk800DA1B8;

extern s32 D_800FEC38;
extern s32 D_800FEC3C;
extern s8 D_800FEC57;
extern s16 D_80100D8C;
extern u16 D_800F7460[];

typedef struct LinkNode {
    struct LinkNode *unk0;
    u8 pad4[0x10 - 4];
    u32 unk10;
} LinkNode;
void func_800B440C(LinkNode *, LinkNode **);
Unk16D8 *func_8004AEBC(void);
s16 func_800CA2E4(s16);
void func_800BBD3C(struct BtlUnit *, struct Unk8E0 *);
void func_800C9794(void);
void func_800CA3CC(void);
extern u8 D_800F5244[];

typedef struct {
    u8 pad0[0x1C];
    s16 unk1C[4];
} Unk800B44A0;


typedef struct {
    u8 unk0;
    u8 pad1[7];
    u16 unk8;
    u16 unkA;
} Unk800BC1E0;

typedef struct {
    u8 pad0[0x33];
    u8 unk33;
    u8 pad34[0x38 - 0x34];
    u8 unk38;
    u8 pad39[0x3C - 0x39];
    u8 unk3C;
    u8 pad3D[0x66 - 0x3D];
    s16 unk66;
} Unk800C09D8;

typedef struct {
    u8 pad0[0x2E];
    u8 unk2E;
} Unk800BD8F0;


typedef struct Heap {
    u8 *unk0;
    s32 unk4;
    s32 unk8;
    u8 unkC[1];
} Heap;

typedef struct {
    s32 unk0; /* frame */
    union {
        s32 w; /* cleared as a word */
        s16 h;
    } unk4; /* state */
    s16 unk8; /* heap index */
    s16 unkA; /* use the shared heap */
    void *unkC; /* work data */
    struct Unk800DA1B8 *unk10;
    s32 (*unk14)(s32, void *, Unk800F78A4 *, void *, void *);
} Unk80100C78Entry; /* 0x18 */

/* effect handler: (mode, work, camera, entry, args) */
typedef s32 (*EffFn)(s32, void *, Unk800F78A4 *, void *, void *);
extern EffFn *D_80100C70[];
extern EffFn D_800F8D10[];
extern EffFn D_800F72E0[];

/* filled by an EffFn in mode 0 */
typedef struct {
    s32 unk0; /* work size */
    s32 unk4;
    s32 unk8;
    s32 padC[2];
    s32 unk14;
} EffInit;

s32 func_800D6D8C(Unk80100C78Entry *, s32);
s32 func_800DA104(s32, s32);
void func_800DA138(struct Unk800DA1B8 *, Unk80100C78Entry *, s32, s32, s32);

typedef struct {
    u16 unk0;
    u16 unk2; /* tpage */
    u16 unk4; /* clut */
    u16 pad6;
    u16 unk8; /* abr, 0xFF = opaque */
} SprInfo; /* sprite texture info (func_800D2C78) */

void func_800D0858(u32, s32, void *);


typedef struct {
    s32 unk0;
    Unk80100C78Entry unk4[11];
} Unk80100C78;
extern Unk80100C78 D_80100C78;
extern Heap D_800FF470; /* 0x1800 bytes */

typedef struct {
    s16 unk0[6][2];
} BtlTexInfo; /* 0x18, passed to func_800C7730 */

typedef struct {
    BtlUnit9C *unk0;
    BtlTexInfo unk4;
    u8 unk1C; /* in use */
    u8 unk1D;
    s16 unk1E; /* index */
    s16 unk20; /* kind */
    s16 unk22;
    s16 unk24;
    u8 pad26[0x28 - 0x26];
} BtlObjSlot; /* 0x28 */
extern BtlObjSlot D_80100F1C[32];

typedef struct {
    BtlUnit9C unk0;
    u8 pad60[0x98 - 0x60];
} BtlModel; /* 0x98 */

typedef struct {
    BtlUnit9C *unk0;
    s16 unk4[6][2];
    s16 unk1C[6][4];
    u8 unk4C; /* in use */
    u8 unk4D;
    s16 unk4E; /* index */
    u16 unk50; /* frame */
    u8 pad52[0x54 - 0x52];
} Unk8010141C; /* 0x54 */
extern Unk8010141C D_8010141C[1];

typedef struct {
    u8 pad0[0x1F];
    u8 unk1F;
} Unk800E0134;

typedef struct {
    u8 pad0[0xC];
    s16 unkC;
    u8 padE[0x14 - 0xE];
    s32 unk14;
} Unk800E0134_1;

extern u8 D_800FE415;
extern s8 D_800FEC65;
extern s32 D_800F750C;
extern u16 D_800F73A0[][19];
extern u8 D_800F6CB4[];
extern char D_800A7F60[];
extern char D_800A7F78[];
extern char D_800A81A8[];
/* effect element (0x28) in an effect file, after the header */
typedef struct {
    u8 unk0; /* kind, 9 = no texture */
    u8 unk1; /* 0xFF = no second texture */
    u8 pad2[0x1C - 2];
    s32 unk1C; /* offset, relocated on load */
    s32 unk20; /* offset, relocated on load */
    u8 pad24[0x28 - 0x24];
} EffElem; /* 0x28 */

typedef struct {
    u8 pad0[4];
    s16 unk4; /* count of EffElem following */
    u8 pad6[0x14 - 6];
} EffElemHdr; /* 0x14 */

typedef struct {
    s32 unk0;
    u8 pad4[0x3C - 4];
    s32 unk3C;
} EffTail;

typedef struct Pool6CEntry {
    u8 *unk0; /* effect file */
    u8 *unk4;
    u8 pad8[0xE - 8];
    s16 unkE;
    s16 unk10;
    u8 pad12[0x14 - 0x12];
    s32 unk14;
    EffElem *unk18;
    u8 pad1C[0x22 - 0x1C];
    s16 unk22;
    EffTail *unk24;
    SVECTOR *unk28;
    s32 unk2C;
    s16 unk30; /* in use */
    s16 unk32;
    s16 unk34;
    s16 unk36;
    u8 pad38[0x3A - 0x38];
    s16 unk3A;
    u8 pad3C[0x6C - 0x3C];
} Pool6CEntry; /* 0x6C */

typedef struct {
    s32 unk0; /* count */
    s32 unk4;
    s32 unk8;
    Pool6CEntry *unkC;
    Pool6CEntry unk10[1];
} Pool6C;

typedef struct {
    VECTOR unk0;
    u8 pad10[0x1A - 0x10];
    u8 unk1A;
    u8 unk1B;
    u8 pad1C[0x1F - 0x1C];
    u8 unk1F; /* in use */
    u8 pad20[0x2C - 0x20];
    Pool6CEntry *unk2C;
} Pool30Entry; /* 0x30 */

typedef struct {
    s32 unk0; /* count */
    s32 unk4;
    s32 unk8;
    Pool30Entry *unkC;
} Pool30;

extern Pool30Entry D_801014B8[0x100];
extern Pool30 D_801044B8;
extern Pool6C D_801044C8; /* 0x920 bytes */
extern Unk80100D88 D_80104DF0;
extern Unk80100D88 D_80104DF4;
extern s32 D_80104E00;
extern MATRIX D_80104E10;
extern u16 D_800F8D0C[];
extern Unk800F6A54 D_800F6A54[];
extern char D_800A7F34[];

typedef struct {
    u8 pad0[0xE];
    s16 unkE;
} Unk800BA938;

typedef struct {
    u8 pad0[0x8C];
    s32 unk8C[3];
    s32 *unk98;
} Unk800CB090;

typedef Battle1358 Unk800CC278;

extern RECT D_800FDF50;
extern u16 D_800FDF60[][0x100]; /* CLUT rows */
extern u32 *D_800FE360;
extern s32 D_800FE36C;
extern s32 D_800FE490;


extern u16 D_800FDF38;
extern u8 D_800F8CF4[][4];
extern char D_800A825C[];

typedef struct Unk800D8EA4 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    s32 unk4;
} Unk800D8EA4;

typedef struct Unk800D7E14 {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s16 unk8[4];
} Unk800D7E14;

typedef struct Unk800CEE74 {
    s16 unk0;
    s16 unk2; /* count of D_800FE4F4 entries */
    s16 unk4;
    s16 unk6;
    u8 pad8[0xA - 8];
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
    u8 unk10[1]; /* data */
} Unk800CEE74;

extern s8 D_800FEC55;

typedef struct {
    u16 unk0; /* sound id, 0 = free */
    s16 unk2; /* age */
    s32 unk4; /* unit id */
} Unk80104E30Entry;

typedef struct {
    s32 unk0;
    Unk80104E30Entry unk4[6];
} Unk80104E30;
extern Unk80104E30 D_80104E30;

extern u8 D_800FDF3A;
extern u8 D_800FDF3B;
void func_800CE05C(struct Unk800CEE74 *, BtlUnit9C50 *);
void func_800B0EF8(struct Battle *);
void func_800B0FC0(struct Battle *);
void func_800C972C(void);
void func_800AB580(s32);
extern s8 D_800FEC54;

extern char D_80101470[];

/* summon model header ('so' magic) */
typedef struct {
    u16 unk0;
    u16 unk2;
} Unk800E9000Ent;

typedef struct {
    u16 unk0; /* 0x6F73 */
    u16 unk2;
    u16 unk4; /* offset */
    u16 unk6; /* offset */
    Unk800E9000Ent unk8[1];
} Unk800E9000;

typedef struct Unk80100DC054 {
    u8 pad0[2];
    s16 unk2;
    s16 unk4;
    u8 pad6[0x18 - 6];
    u16 unk18[6];
    u16 unk24[6];
    u16 unk30[6];
    u8 *unk3C;
    BtlUnit9C50 *unk40;
    u8 pad44[0x50 - 0x44];
    s32 *unk50;
    u8 pad54[0x90 - 0x54];
    BtlModel unk90;
    u8 pad128[0x180 - 0x128];
    s32 unk180[1];
} Unk80100DC054;

typedef struct {
    u16 unk0;
    u8 pad2[2];
    s32 unk4;
} Unk80104E34; /* 8 */
extern Unk80104E34 D_80104E34[6];

extern u16 D_800F51E8[];
extern u8 D_800F8D70[];
extern s32 D_800F8D74;
extern u8 D_800F8D78[5]; /* battle message ids */
BtlUnit *func_800E9124(void);
extern u8 D_800F8D80;
extern u8 D_800F8D81;
extern char D_800A83AC[];

extern u8 D_800F909C[];
extern void (*D_800F8DA8[])(void);


typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    u8 pad8[0xC - 8];
    s32 unkC;
} Unk80104F90;
extern volatile Unk80104F90 D_80104F90;
extern u8 D_80104E75;

struct Unk800F412C;
typedef struct Unk800F3F10 {
    s16 unk0; /* dx per frame */
    s16 unk2; /* dy per frame */
    s32 unk4; /* frames */
    void (*unk8)(struct Unk800F412C *); /* on finish */
} Unk800F3F10;

/* moving menu sprite */
typedef struct Unk800F412C {
    s16 unk0; /* text id */
    s16 unk2;
    u8 unk4;
    u8 pad5;
    u16 unk6; /* frame */
    u16 unk8; /* x */
    u16 unkA; /* y */
    Unk800F3F10 *unkC;
} Unk800F412C; /* 0x10 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Unk800F412C unk4[5];
    u32 unk54;
    s32 unk58;
    u32 unk5C; /* count */
    s32 unk60;
    BtlUnit *unk64;
    BtlUnit *unk68[4];
    s8 unk78[4];
    u8 unk7C[4];
    u8 unk80[4];
    u8 unk84[4];
    u8 unk88[4];
} Unk80105078;
extern Unk80105078 *D_80105078;
extern s32 D_800F9028[];
extern Unk800F3F10 D_800F9084;
extern Unk800F3F10 D_800F9090;

extern void *D_80104FA0;
extern s32 (*D_800F8DC4[])(void);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Unk80104FC0; /* D_80104FB0.unk10 */

typedef struct {
    u8 unk0; /* cursor */
    u8 unk1;
    u8 unk2;
    u8 pad3;
    u8 unk4[4];
    u8 unk8[4];
    u8 unkC[4];
    Unk80104FC0 unk10;
    u8 pad14[0x18 - 0x14];
    BtlUnit *unk18;
} Unk80104FB0;
extern Unk80104FB0 D_80104FB0;
extern void (*D_800F8D94[])(void);



typedef struct Unk80104FD4 {
    u8 pad0[6];
    u8 unk6;
    u8 unk7; /* count */
    s32 unk8;
} Unk80104FD4;
extern Unk80104FD4 *D_80104FD4;
extern s32 (*D_800F8E58[])(void);
extern void (*D_800F8E48[])(s32, s32, s32, MenuView *);
extern u8 D_80104FE8;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 pad5;
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
    s8 unk12;
    u8 unk13;
    u8 unk14;
    u8 pad15[0x18 - 0x15];
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    BtlUnit *unk20;
    BtlUnit *unk24; /* last target */
} Unk80104FF0;
extern Unk80104FF0 D_80104FF0;
extern u8 D_80104FFF;
extern u8 D_80104FFA;
extern BtlUnit *D_80104FE4;
extern u8 D_800F8EE4[];
extern u8 D_80105059;
extern void (*D_800F8F04[])(Window *);

typedef struct {
    s32 unk0;
    s32 unk4; /* next free */
    s32 unk8;
    s32 unkC; /* used */
    s32 unk10; /* free */
} Unk80105060;
extern volatile Unk80105060 D_80105060;


typedef struct {
    s16 unk0;
    u16 unk2;
} Unk801051F088;

typedef struct {
    u8 unk0;
    u8 pad1[0x80 - 1];
    s32 unk80;
    s32 unk84;
    Unk801051F088 unk88; /* text size */
    u8 unk8C;
    u8 unk8D;
    u8 pad8E[2];
    s32 unk90;
    u8 pad94[0x98 - 0x94];
    u32 *unk98;
    u8 pad9C[0xA4 - 0x9C];
    RECT unkA4;
} Unk801051F0;
extern Unk801051F0 D_801051F0;
extern s32 D_800FDE3C[];
extern s16 D_800F5850[];
/* enemy data record (func_800C9D1C) */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u16 unk12;
    s32 unk14;
    s32 unk18;
    u16 unk1C;
    u8 pad1E[0x20 - 0x1E];
    u16 unk20[6]; /* file ids */
    s32 unk2C;
    u16 unk30;
    u8 pad32[0x34 - 0x32];
    u8 unk34[4];
    u8 pad38[0x3C - 0x38];
    s32 unk3C;
    u8 unk40;
    u8 unk41;
    u8 pad42;
    u8 unk43[4];
    u8 unk47;
    u8 unk48[3];
    u8 unk4B;
    u16 unk4C;
    u8 unk4E;
    u8 unk4F;
    u8 unk50[6];
    u8 unk56[6];
    u8 unk5C[6];
    u8 pad62[0x64 - 0x62];
    u16 unk64;
    u16 unk66;
    u8 unk68;
    u8 unk69;
    s16 unk6A;
    s16 unk6C;
    u8 unk6E;
    u8 pad6F[0x74 - 0x6F];
} Unk800C9D1C;
extern u32 D_80104E80;
typedef struct {
    SPRT unk0[4][2];
    DR_TPAGE unkA0[2];
    SPRT unkB0[2];
    SPRT unkD8[2];
    DR_TPAGE unk100[2];
    SPRT unk110[4][2];
    SPRT unk1B0[4][2];
    SPRT unk250[4][2];
    DR_TPAGE unk2F0[2];
    DR_TPAGE unk300[2];
    POLY_G4 unk310[2];
    DR_TPAGE unk358[2];
} Unk801050D8; /* 0x368, double-buffered menu primitives */
extern Unk801050D8 *D_801050D8;
extern u8 D_801051E0;
extern u8 D_80104E94[2][0x18];

typedef struct {
    s32 unk0; /* frames */
    s16 unk4;
    s16 unk6;
    s32 unk8;
} Unk800E6270;
s32 func_800E813C(u32, s32);

/* main exe: candidates for ff9.h */
void func_8002E260(void);
u8 func_800339F4(s32, u8);
void func_80026C34(s32, s32, s32, s32, u8);
extern u32 D_800686D8;
void func_800316D8(void *, void *);
void func_8002C084(s32);
void func_8002C740(s32, s32, s32);
void func_8002E32C(Window *, s32);
void func_8002E380(Window *, s32);
void func_8002C584(s32);
void func_8002DD00(Window *);
Window *func_8002E43C(s32);
UnkPair *func_80027EDC(s32);
typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2;
    u8 pad4[0xC - 4];
} Unk8006E0E8; /* 0xC */
extern Unk8006E0E8 D_8006E0E8[];
void VSyncCallback(void (*)(void));
void func_8002C158(void *);
void func_800553EC(s32, s32, s32, s32);
s32 func_80027F34(s32); /* main.c defines it returning u8 */
typedef struct {
    u8 pad0[8];
    s32 unk8;
} Unk8006B1F4; /* 0xC */
extern Unk8006B1F4 D_8006B1F4[];
void func_800639D4(s32);
s32 func_800645C4(void);
s32 func_80055DCC(void);
void func_80056388(s32, s32);
s8 func_80021698(void);

/* ovl_0b functions */
s32 func_800D71C8(s32, s16 *, s32, s32, s32, s16 *);
void func_800E7824(s32, s16 *);
u16 func_800B4478(u16 *);
void func_800EB358(BtlUnit *);
void func_800ECCA8(s32);
s32 func_800EFB3C(s32);
s32 func_800EFD1C(s32);
void func_800EFA10(s32);
void func_800EF7D4(Window *);
void func_800F07F0(void);
s32 func_800F0118(void);
s32 func_800F028C(void);
void func_800F0904(void);
void func_800F1AF8(Window *);
void func_800F2684(s32);
void func_800F2C74(s32);
void func_800F2DC0(s32);
void func_800F2ED0(s32);
void func_800F316C(void);
void func_800F3DE0(s32);
void func_800F3EDC(s32);
void func_800F3F10(void);
void func_800F3FDC(void);
void func_800F42F0(void);
void func_800F4D80(u32 *, u32);
void func_800F4FE8(s32);
void func_800EB518(Unk800EB518 *, s32, s32, s32);
void func_800EE120(Window *);
s32 func_800D8CD8(u8 *, s32, s32);
void func_800C1854(BtlUnit *, s32);
void func_800C19F8(u8);
void func_800CC044(BtlUnit *);
s32 func_800E8D60(void);
void func_800EA650(void);
BtlUnit *func_800EAD94(void);
void func_800EAE8C(void);
void func_800EB5AC(void);
s32 func_800EB7B0(BtlUnit *);
void func_800EC4EC(void);
void func_800EE154(void);
void func_800EE24C(void);
void func_800EE31C(void);
s32 func_800BC78C(BtlUnit *);
u32 func_800C0ACC(BtlUnit *, u32);
void func_800C8A3C(BtlUnit9C *, s32, s32, s32);
void func_800C8E80(BtlUnit9C *, s32);
void func_800E7EC0(BtlUnit *, s32);
void func_800EA7B8(Unk80104F88Entry *);
void func_800EA908(Unk80104F88Entry *);
void func_800EB63C(Unk80104F88Entry *);
void func_800EB6FC(Unk80104F88Entry *);
void func_800EBCC4(u8);
void func_800EC1B0(void);
void func_800EC258(void);
void func_800EC334(s32);
void func_800EC3C0(s32);
s32 func_800EC44C(s32);
s32 func_800EC49C(s32);
void func_800ECA50(void);
void func_800EDCA0(void);
void func_800EDD1C(void);
void func_800EDD9C(void);
void func_800EDDF8(void);
void *func_800F2A60(s32);
Unk80105078 *func_800F3EA4(void);
void func_800F3ECC(u8);
void func_800DA898(void);
void func_800EAAFC(void);
void func_800EAC88(void);
void func_800EB114(void);
void func_800EB1CC(void);
void func_800EB2E0(void);
void func_800EB2FC(void);
void func_800EB410(void);
void func_800EB46C(void);
void func_800EB81C(void);
void func_800EB924(void);
s32 func_800EB9E0(void);
void func_800EBA20(void);
void func_800EBB78(void);
void func_800EBBB4(void);
void func_800EBD58(void);
void func_800EBEFC(s32);
void func_800EC048(void);
s32 func_800EC194(s32);
void func_800ECDB8(void);
void func_800ECF0C(void);
void func_800EA518(void);
void func_800F0DF8(void);
s32 func_800F13CC(void);
void func_800F142C(void);
void func_800F2374(void);
void func_800F2560(void);
void func_800F2AAC(void);
void func_800F3060(void);
void func_800F386C(void);
void func_800F3A8C(void);
void func_800F3D0C(void);
void func_800F4384(void);
void func_800AB4FC(void);
u32 func_800BEDBC(BtlUnit *, u32);
u32 func_800C1F00(u32);
void func_800C2F9C(s32);
s32 func_800E454C(s32);
void func_800E73B4(BtlUnit *, u16 *);
void func_800E86E0(s32 *, s32 *, u32);
s32 func_800E9638(s32);
void func_800E9A74(void);
void func_800E9F3C(u8);
void func_800EB05C(void);
void func_800EBB34(void);
void func_800EC008(void);
void func_800F2A0C(void);
void func_800F3C08(void);
void func_800F4630(void);
void func_800F4700(void);
void func_800F4884(void);
void func_800F48DC(void);
void func_800F48E4(void);
void func_800F4D20(void);
void func_800B2804(BtlUnit *);
void func_800B2DD4(Battle *);
void func_800B4320(BtlCmd **, u8);
void func_800B4434(BtlCmd *);
void func_800B44C0(s32, s32, s32, s32);
s32 func_800B9550(BtlUnit *, s16);
void func_800B97A0(BtlUnit *);
s32 func_800BF558(BtlUnit *, u32);
s32 func_800C1E88(BtlUnit *, s32);
void func_800C1524(void);
void func_800C2E04(void);
void func_800CBF90(void);
void func_800CD5C8(u32, ...);
/* psyq-style varargs (stdarg.h) */
typedef char *va_list;
#define __va_rounded_size(TYPE) (((sizeof(TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(AP, LASTARG) (AP = ((char *)&(LASTARG) + __va_rounded_size(LASTARG)))
#define va_arg(AP, TYPE) (AP += __va_rounded_size(TYPE), *((TYPE *)(AP - __va_rounded_size(TYPE))))

typedef struct {
    s16 unk0; /* offset from unk4 */
    s16 unk2;
} TextTblEnt;

typedef struct {
    u16 unk0; /* count */
    u16 pad2;
    TextTblEnt unk4[1];
} TextTbl; /* message table (D_800F6CB4) */

void func_8002BBE8(s32, u8 *, s32);

u8 *func_800CD75C(u8);
typedef struct {
    u8 pad0[8];
    u8 unk8[4]; /* steal items, 0xFF = none */
} Unk800B7F20; /* enemy steal slots */

void func_800EA1B0(s32);

u32 func_800D89E8(s32);
s32 func_800D9F5C(u8 *, s32, s32);
void func_800DC634(char *);
void func_800E3D80(void);
u8 func_800EA038(void);
void func_800EA164(s32);
void func_800E9F68(u8);
void func_800EF094(s32, s32, s32, MenuView *);
void func_800F4DFC(void);
s32 func_800AB26C(void **, s32);
void func_800AB43C(void);
void func_800E9E80(void);
void func_800C02D4(BtlUnit *);
void func_800C0430(BtlUnit *, s32);
void func_800C2344(BtlUnit9C *);
void func_800C8D18(BtlUnit9C *, u8, s16, s16, s16);
Battle122C *func_800CA5AC(BtlCmd *);
struct Unk800CEE74 *func_800CEE74(void);
s32 func_800D8D94(struct Unk800D8EA4 *, s32, s32 *);
s32 func_800D8E10(struct Unk800D8EA4 *, s32, s32 *);
s32 func_800D8EA4(struct Unk800D8EA4 *, s32, s32 *);
s32 func_800D8F1C(struct Unk800D8EA4 *, s32, s32 *);
s32 func_800D8FAC(struct Unk800D8EA4 *, s32, s32 *);
s32 func_800D908C(struct Unk800D8EA4 *, s32, s32 *);
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s16 unk8[4];
} Unk800D9260;
s32 func_800D9260(Unk800D9260 *, s32, s32 *);
s32 func_800D960C(struct Unk800D7E14 *, s32, s32);
s32 func_800AD90C(s32, s32, s32, s32, s32);
void func_800B3C00(void);
void func_800C0D24(BtlUnit *);
void func_800C1594(void);
void func_800C86C8(BtlUnit9C *, s32, s32, s32);
void func_800CDEF0(BtlCmd *);
s32 func_800D8D08(u8 *, s32, s32);
s32 func_800D8D68(u8 *, s32, s32);
s32 func_800D905C(u8 *, s32, struct Unk800F78A40 *);
void func_800E8408(BtlUnit *);
void func_800E8460(BtlUnit *);
s32 func_800E976C(s32); /* unit id, or an SVECTOR pointer */
void func_800B1250(s32, s32, s32, s32, s32);
u8 func_800B3E7C(BtlCmd *);
void func_800BD414(BtlUnit *, u16);
void func_800BD464(BtlUnit *, u16);
s32 func_800C09A4(BtlUnit *, u32);
void func_800C09EC(BtlUnit *);
s32 func_800C0B6C(BtlUnit *, u32);
s32 func_800D87D0(s32);
u16 func_800DF7A0(BtlUnit9C *, u16, s32, s32 *, s32);
s32 func_800E7298(BtlUnit *);
s32 func_800E7370(BtlUnit *);
void func_800E748C(BtlUnit *, s16 *);
void func_800E753C(BtlUnit *, s32, s16 *);
typedef union {
    s32 w[2];
    s16 h[4];
} SVec4; /* SVECTOR accessed as words */
void func_800E78B4(s32, s16 *, s16 *, s32, s32, s32, s32);
void func_800E7A44(s32, VECTOR *);
void func_800E7FC0(BtlUnit *, s32);
s32 func_800E9A00(s32, s32);
BtlUnit *func_800BED10(u16);
void func_800A8600(s16);
void func_800A8CD4(void);
void func_800A8DE0(void);
s32 func_800AB324(s32, s32 *, s32 *);
s32 func_800D9038(u8 *, s32, s32);
s32 func_800D9110(u8 *, s32, s32);
s32 func_800DA030(u8 *, s32, s32);
void func_800C2200(BtlUnit *, BtlUnit *);
Unk8010141C *func_800DD02C(s32, char *);
void func_800DC710(void);
void func_800DC7E0(void);
void func_800D616C(void);
void func_800D6D3C(void);
void func_800DA4E8(void);
void func_800E3C58(void);
void func_800E72B8(void);
void func_800E8F40(s32);
void func_800E954C(void);
s32 func_800E9930(s32, s32, s32);
s32 func_800B4040(BtlUnit *);
void func_800C316C(s32);
void func_800C36D0(void);
s32 func_800CAEC4(BtlScript *, BtlUnit *);
Unk800CC278 *func_800CC278(struct BtlUnit *);
s32 func_800CC444(u32);
s32 func_800AE47C(s32, u16);
void func_800C22CC(BtlUnit *, u16, u8);
s32 func_800C37BC(void);
void func_800D6E70(s32, s32, s32, s32);
void func_800D86E0(s32, s32);
s32 func_800D8740(s32, s32);
s32 func_800D8B20(s32);
s32 func_800DA06C(s32, s32);
s32 func_800DA3D8(s32, s32);
void func_800DC510(char *);
void func_800DC9D0(BtlUnit9C *, s32, u16 *);
void func_800DCA18(BtlUnit9C *, s32, BtlBone *);
BtlObjSlot *func_800DCFB0(s32, char *);
u8 func_800DD10C(BtlUnit9C *);
void func_800E00DC(s32, Pool30Entry *, Pool30 *);
void func_800E14D8(u32, Pool6C *);
Pool6CEntry *func_800E1534(Pool30 *, Pool6C *, u8 *, s32);
void func_800E1708(Pool30 *, s32, s32);
void func_800E1810(Pool30 *, Pool6CEntry *);
void func_800E0168(Pool30 *);
void func_800E1874(Pool30 *, Pool6C *);
void func_800E2000(Pool30 *, Pool6C *);
void func_800E3B34(Heap *, s32);
void func_800E3DA8(void);
void func_800E4098(void);
s32 func_800E788C(s32);
void func_800E8B28(s32, s32, s32);
s32 func_800A91EC(void);
void func_800BD4DC(BtlUnit *);
void func_800BD53C(BtlUnit *);
s32 func_800CFC0C(s32);
void func_800D07B4(); /* (s32 code, u32 *ot, void *prim, s32 tpage) */
s32 func_800D6360(s32);
void func_800D64C8(s32);
void func_800D85AC(s32, s32);
void func_800D85F8(struct Unk800F78A40 *, s32, s32);
void func_800D8820(s32, s32);
void func_800D8988(s32, s32, s32);
s32 func_800D8A64(s32);
s32 func_800D8B70(s32);
s32 func_800DA0B8(s32);
void func_800EA124(s32);
void func_800A9BE4(s32);
void func_800ABBC4(s32, s32);
u32 func_800D8C70(s32, u32);
void func_800D8658(s32, u8);
void func_800D8914(s32);
void func_800D8A20(s32, s32);
BtlUnit *func_800E716C(s32);
void func_800E9D84(void);
void func_800EC070(s32);
void func_800EC14C(s32);
void func_800EC170(s32);
void func_800ECB5C(void);
extern RECT D_800F8DDC; /* command window rect */
void func_800EC628(Window *);
void func_800EC730(Window *);
void func_800EDC3C(); /* used as a Window callback */
s32 func_800EB2BC(s32);
u8 func_800F1360(void);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    s16 unk8[4];
    s16 unk10[4];
} Unk800D955C;

typedef struct {
    u16 unk0;
    u16 unk2;
} Unk800E8F50Entry;

typedef struct {
    u16 unk0; /* magic 'so' */
    u16 unk2;
    u16 unk4; /* offset */
    u16 unk6; /* offset */
    Unk800E8F50Entry unk8[1];
} Unk800E8F50;

typedef struct {
    s16 unk0; /* state */
    u16 unk2; /* next state */
    u8 pad4[0x10 - 4];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
} Unk800AA08C;

typedef struct {
    s16 unk0; /* state */
    u16 unk2; /* next state */
    u8 pad4[0xA - 0x4];
    s16 unkA;
    u8 padC[0x10 - 0xC];
    union {
        s32 w[2];
        u16 h[4];
    } unk10;
    u8 pad18[0x24 - 0x18];
    s32 unk24;
} Unk800AA958; /* party model load state */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Unk800AA958Req; /* CD read request */

void func_80022CC4(s32, void *, s32, s32);
typedef struct {
    u8 pad0[8];
    s32 unk8; /* file address */
    s32 unkC; /* bytes done */
    s32 unk10; /* chunk size */
} Unk800AE844; /* streamed file read */

extern s16 D_800F51EC;
s32 func_8001CA70(s32);
void func_80056048(s32, s32, s32);

s32 func_800AB07C(Unk800AA958 *, Unk800AA958Req *, s16, s32);


s32 func_800D955C(Unk800D955C *arg0, s32 arg1, s32 *arg2);
s32 func_800D72BC(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s16 *arg4, s32 arg5);
void func_800CFA00(s16 *, s16 *, s32, s16 *);
u16 func_800DB6B8(BtlUnit9C8 *, BtlTexInfo *, s16 *, s32);
u16 func_800DB4EC(BtlUnit9C8 *);
void func_800CF808(s16 *, s16 *, s16 *);
void func_800CFD78(s16 *, s16 *, s32, s16 *);

typedef struct {
    u16 unk0; /* text offset into D_800FD7AC */
    u16 unk2; /* text offset into D_800FD8CC */
    u8 pad4[6 - 4];
    u8 unk6;
    u8 unk7;
    s32 unk8;
} Unk800FD56C; /* 0xC */
extern Unk800FD56C D_800FD56C[];
extern u8 D_800FD8CC[];
extern u16 D_800FD36C[];
extern u8 D_800FB4F0[];
extern s32 D_800F8E04[];
extern RECT D_800F8EF4;
extern RECT D_800F8EFC;
Window *func_8002DCAC(RECT *, s32, s32, s32);
void func_800F1A8C(Window *);
void func_800F29B4(Window *);
void func_800F0A54(Window *);
void func_800F0BE0(Window *);
void func_800EE0FC(Window *);

typedef struct {
    s32 unk0;
    u32 unk4 : 24; /* offset of the tables from this header */
    u32 unk7 : 8;  /* table length */
} Unk800AB758;

/* rotation part of a MATRIX, used where no translation is stored */
typedef struct {
    s16 m[3][3];
} RotMatrix33;

/* packed screen coordinate */
typedef union {
    s32 w;
    struct {
        s16 x;
        s16 y;
    } s;
} XY32;

typedef struct {
    u32 *unk0; /* next free primitive */
    u32 *unk4; /* ordering table entry */
    s32 unk8;  /* rgb */
    u8 padC[0x10 - 0xC];
    XY32 unk10;
    u16 unk14; /* clut */
} Unk800CCD00;

void func_800CCD00(Unk800CCD00 *, Unk800686E8 *);
void func_800CCBC0(Unk800CCD00 *, u32);
void func_800CCAF0(struct Battle1358 *, Unk800CCD00 *, s32, s32);
extern u8 D_800F6A28[];
extern u8 D_800F6A38[];
/* main exe: candidates for ff9.h */
extern Unk800686E8 D_80068BE8;
s32 func_8004AB7C(s32, s32, s32, s32);
void func_80030AF4(void *, void *); /* rotation -> matrix */
s32 func_800CF91C(SVECTOR *, s32, s32);
void func_800CF8EC(s32, s32, s32);
extern Unk800686E8 D_80068BF0[]; /* digits */

typedef struct {
    u8 *unk0; /* camera script */
    u32 unk4;
    s32 unk8;
    s32 unkC;
    u32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    s16 unk46[5];
    s16 unk50[5];
    s16 unk5A[5];
} Unk800FE488; /* 0x64 */
extern Unk800FE488 D_800FE488;
extern u32 D_800F8EA4[][4];
extern void (*D_800F8E94[])(void);
extern RECT D_800FDE58;
u8 *func_800F511C(u8 *, RECT *);
void func_800C7B84(BtlUnit9C *);
void func_800DCB20(BtlUnit9C *, VECTOR *);
void func_800C57A4(BtlUnit *, s16 *);
s32 func_800CF690(void);
s32 func_800D7D64(s32, Unk800D955C *, s32, s32 *, s32 *);
s32 func_800D8414(s32, u8 *, s32, s32 *, s32 *);


extern u8 D_80104E6A;
extern u8 D_80104E6B;
extern u8 D_800F8E84[];
extern u8 D_800FDDF8[][3];
extern u8 D_800FDDD0[][2];
extern RECT D_800A8584;
void func_800B10F8(BtlUnit *);
extern u16 D_800F5200[];
void func_800BB4A0(Character *, BtlUnit *, u16, u16);
void func_800C1990(BtlUnit *);
void func_800C9A28(void);
void func_800C0C00(BtlUnit *, s32);
void func_800BBF30(Unk8004AB64 *, s16);
extern s32 D_800FDF34;
extern RECT D_800A858C;
extern RECT D_800A8594;
void func_800BFF20(BtlUnit *);
void func_800BCEF0(BtlUnit *);
s32 func_800C20B8(GameData *, u32, s32);
void func_800C2F1C(s32);
void func_800B2F24(Battle *);
extern s16 D_801051E8[][2];
void func_800B4234(BtlUnit *);
void func_800ED3F8(s32, s32, s32, char *);
s32 func_800EB2A4(void);
s32 func_800F13BC(void);
extern u8 D_800FD7AC[];
void func_800EC928(); /* called without arguments from func_800EC628 */
s32 func_800E865C(s32, s32);
void func_800DDF10(BtlUnit9C *, u8);
u8 *func_800DE11C(BtlUnit9C *, u8, u8 *);
void func_800DF15C(BtlUnit9C *, u8, u16);
u8 *func_800DD27C(BtlUnit9C *, u8, u8 *, s32);
void *func_800CD78C(BtlUnit *);
extern u8 D_800F7124[];
extern u8 D_800F71E4[];
void func_800CB1D4(u16, u16 *, u16 *);
void func_800C2FFC(s32);
void func_800CA28C(BtlCmd8 *, BtlCmd8 *);
s32 func_800EF598(BtlCmd8 *);
void func_800C9EE0(Unk800C9EE0 *, BtlUnit *, s32 *, s32);
void func_800CA0A0(Unk800C9D1C *, BtlUnit *);
void func_800C9D1C(Unk800C9D1C *, Battle4880 *);
s32 func_800C1184(u32 *);
void func_800CF560(s32);
s32 func_800CF574(void);
UnkPair *func_800EC114(s32);
Battle1330 *func_800C9600(Battle11C020 *);
u32 func_800AAEF0(u32, u16 *, u16 *, u16 **, s32 *);
u16 func_800DB8E0(struct Unk80100DC054 *, s16 *);
typedef struct {
    u32 unk0; /* uv/clut */
    u8 unk4;  /* w */
    u8 unk5;  /* h */
    u16 unk6; /* tpage */
} Unk80068EE8; /* sprite descriptor */
extern Unk80068EE8 D_80068EE8[];
extern u16 D_80068EEE; /* D_80068EE8[0].unk6 */
extern Unk80068EE8 D_80068E90;
extern Unk80068EE8 D_80068ED0;
extern Unk80068EE8 D_80068ED8;
extern Unk80068EE8 D_80068EE0;
extern u8 D_80104E7D;
extern SprtWords D_801050A0;
extern SprtWords D_801050C0;
extern DR_TPAGE D_80105080;
extern DR_STP D_80105090;
extern u32 D_801050D4;
s32 func_800AB2C8(void);
Unk8010141C *func_800DC4CC(void);
typedef struct {
    s16 unk0[6][2];
} Unk800DD120;
u8 *func_800634A8(s32);
u8 *func_800634D8(s32);
typedef struct {
    u32 unk0_0 : 4;
    u32 unk0_4 : 1;
    u32 unk0_5 : 3;
    u32 unk0_8 : 21;
    u32 unk0_29 : 1;
    u32 unk0_30 : 1;
    u32 unk0_31 : 1;
    u8 pad4[0xC - 4];
} Unk8006A774;
extern Unk8006A774 D_8006A774[];
u8 *func_800C7DAC(BtlUnit9C *, s32, u8 *);
void func_800C6528(BtlUnit9C *, u8);
u8 *func_800C58E4(BtlUnit9C *, u8, u8 *);
void func_800DCA68(BtlUnit9C *, SVECTOR *, VECTOR *, VECTOR *);
void func_800DC988(s32, s32, BtlBone *);
void func_800C9040(BtlUnit9C50 *);
BtlUnit *func_800C1DEC(void);
void func_800CF58C(s32);
void func_80063640(DR_TPAGE *, u16);
void func_800C9374(BtlUnit9C50 *, s32);
void func_800B0E04(BtlUnit *, u8 *);
void func_800EEDD4(void);
void func_800C2250(BtlUnit *, u16);
void func_800EA550(void);
void func_800EB238(void);
void func_800EBC30(void);
void func_800F3E60(void);
void func_800F28F0(void);
void func_800F1484(void);
s32 func_800AB6F0(s32, s32);
void func_800AB758(s32, s32, s32 **, s32 **, s32 **);
BtlUnit *func_800E9FCC(void);
void func_800CA7CC(void);
u8 *func_800CE1B4(MATRIX *, s32, s32, u32 *, s32);
u8 *func_800E3EEC(u8 *);
u8 *func_800CC4F0(u8 *);
void func_800BD93C(BtlUnit *);
void func_800BFAF4(BtlUnit *);
void func_800ED02C(RECT *, s16);
extern s32 D_800F8DE4[];
Window *func_800F1370(void);
void func_800EBE1C(RECT *, RECT *);
void func_800EBD88(void);
s32 func_8002BE88(void);
void func_8002BE98(s32);
void func_8002C138(u8);
void func_8002DF18(RECT *, s32, s32, s32);
extern RECT D_800F8DBC;
void func_800BC21C(BtlUnit *, s32 *);
void func_800B3460(GameData *, Battle *, BtlUnit *);
void func_800B1764(Battle *);
void func_800B05C0(GameData *, Battle *, s32);

s32 func_8005553C(s32);
void func_80055850(s32, s32, s32);
s32 func_80034430(s32, s32);
void func_800F3D3C(s32);
void func_800F1BFC(Window *);
s32 func_800D001C(s32, s32, s32 *);
s32 func_800C4E84(s32, s16 *, s16 *, s32 *);
void func_800C4DC4(s32, s32, s16 *, s16 *, s32 *);
s32 func_800C5890(s32);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s16 unkC[4];
    s16 unk14[4];
} Unk800D7C9C;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    s32 unk8;
    s16 unkC[3]; /* rotation delta (func_800D9AB0) */
} Unk800D805C;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    s16 unk8[3];
} Unk800D8124;

extern s8 D_800FEC56;
s32 func_800D9488(Unk800D7C9C *, s32, s32 *);
s32 func_800D9AB0(Unk800D805C *, s32, s32 *);
s32 func_800D9D58(Unk800D8124 *, s32, s32 *);
s32 func_800D8ACC(s32, s32);

typedef struct {
    BtlUnit *unk0;
    BtlUnit *unk4;
    u8 pad8[0x38 - 8];
    s32 unk38;
    s32 unk3C;
    u8 pad40[0x44 - 0x40];
    BtlUnit *unk44;
    u8 pad48[0x4C - 0x48];
} Unk800FE368;
extern Unk800FE368 D_800FE368;
extern u8 D_80104E76;
void func_80063864(void *, void *, s32);
struct Battle11C020 *func_800C94B8(void);
u8 func_8002C700(s32, s32);
void func_8002C8E8(s32, s32, s32, s32);
void func_8002C64C(s32);
u8 func_800F47D8(void);
void func_800E77D8(s32, s16 *);

extern u16 D_800F524C[][0x22];
Unk800CC278 *func_800CC31C(s32, s16, s16, u8);
Unk800CC278 *func_800CC37C(s32, s16, s16, u8);
void func_800EA338(s8);
s32 func_800D9990(Unk800D7C9C *, s32, s32 *);
s32 func_800D974C(Unk800D7C9C *, s32, s32 *);

typedef struct {
    u32 tag;
    u32 code[5];
} DR_MOVE;

extern s32 D_800F78AC;
void SetDrawMove(DR_MOVE *p, RECT *rect, int x, int y);
void func_8005562C(s32);
void func_800D0940(void);
extern u8 D_800F6FB8[];
extern u8 D_800A7C20[]; /* "/\xFF" suffix string */
void func_800F4F04(s32, s32);
void func_800F4F44(s32, s32);
s32 func_800F4FC8(void);
extern u8 D_80104E77;
void func_800F11A4(BtlUnit *);
void func_800F1150(BtlUnit *);
u32 func_800AB810(s32, s32);
void func_800A923C(void);
void func_800AEA48(void);
void func_800A8BF4(void);
u8 func_800EA30C(void);
void func_800A882C(void);
s32 func_800AF2E0(void);
void func_800E9D30(void);
void func_800AF1D0(void);
s32 func_8002246C(void);
void func_800AEE68(void);
void func_800A94F8(void);
s32 func_800AC280(u32, s32 **, s32 **, s32 **);
u32 func_8004AB64(s32);
/* object slot returned by func_8001E350 in battle */
typedef struct {
    u32 unk0; /* flags */
    s32 unk4; /* model id */
    BtlUnit9C *unk8;
    s32 unkC;
} BtlObj;
void func_800A8E24(void);
void func_800A8E6C(void);
s32 func_800A8F10(void);
s32 func_800A8F18(void);
s32 func_800A8F8C(void);
s32 func_800A902C(void);
s32 func_800A9090(void);
s32 func_800A90B0(void);
s32 func_800AF1C8(void);
extern void (*D_800F8DF0[])(Unk800FD56C *);
extern void (*D_800F8E00)(Unk800FD56C *);
extern void (*D_800F9038[])(BtlUnit *, s32);
void func_800F3390(void);
u8 func_800B3EF8(BtlUnit *, u8);
void func_800F16DC(u8, u8);
typedef struct {
    u32 unk0; /* status mask */
    s16 unk4; /* icon */
    s16 unk6; /* width */
} Unk800F8F98;
extern Unk800F8F98 D_800F8F98[];
extern Unk800F8F98 D_800F8F18[];
void func_800B41A4(BtlUnit *, u8);
void func_800E9BBC(void);
s32 func_800E9FF4(void);
void func_800EA5C4(void);
void func_800EA548(void);
void func_800F4778(void);
void func_800EB578(s32);
void func_800F4D40(void);
void func_800EA4D0(void);
void func_800EA400(void);
void func_800EA440(void);
void func_800EA498(void);
void func_800F4A00(void);
s32 func_800B0CF8(s32, u32);
u32 func_800C8DE0(u32, s32, s32, s32);
void func_8002F1CC(MenuList *, s32);
void func_8002F284(MenuList *, s32);
extern s16 D_800F9048[];
void func_800E3C50(void *);
s32 func_800DA2AC(Unk800DA1B8 *);
void func_800F1790(u8, u8);
BtlUnit *func_800F18F0(u8);
extern u8 D_80104FF2;

/* first unit flagged as the current target (bits 28 and 29 of unk68) */
static inline BtlUnit *find_target(void) {
    BtlUnit *u;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if ((u->unk68 & 0x30000000) == 0x30000000) {
            return u;
        }
    }
    return NULL;
}

extern u8 D_800F8E68[][2];
void func_800F1A20(s32);
s32 func_8001DA6C(s16);
s32 func_80063830(s32);
void func_800BCAA4(BtlUnit *);
s32 func_800E3B70(Heap *, s32);
void func_800C2598(BtlCmd *, s32, s16 *);

s32 func_800D9350(Unk800D7C9C *, s32, s32 *);
s32 func_8004BB80(s32);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
} Unk800EF80C;

typedef struct {
    u8 pad0[8];
    s32 unk8;
} Unk800EDE6C;

typedef struct {
    u32 unk0; /* tag */
    u32 unk4;
    u32 unk8;
    u32 unkC; /* ordering table */
} Unk800D0940;

extern s32 D_800F7278;
void func_800EF80C(Unk800EF80C *);

void func_800BCC64(BtlUnit *);
void func_800BCDBC(BtlUnit *);
s32 func_800D9F88(u8 *, s32, s32 *);

extern u16 D_800FEC42;
extern Unk800F412C D_800F8FD8[5];
s32 func_8004BB7C(s32);
void func_800E8DC8(s32);
void func_800E8E84(s32);
void func_800F0444(void);
void func_800EFF14(void);
void func_800F40D0(Unk800F412C *);
void func_800F4010(s32);

BtlObjSlot *func_800DC494(void);
void func_800C7730(BtlUnit9C *, s16 *, s16 *);

void func_800DF588(BtlUnit9C *, s32, s32);
s32 func_800DF670(BtlUnit9C *, s32);

/* argument block of func_800EE37C */
typedef struct {
    s16 unk0;
    u8 unk2;
    u8 pad3;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Unk800EE37C;
void func_800EE37C(Unk800EE37C *);
extern RECT D_800F8E30;
void func_8002E3A8(Window *, MenuList *);
void func_800EEED4(Window *);
void func_800EEBB8(Window *);
void func_800EF00C(void);
s32 func_800D8A94(s32, s32);
s32 func_800DA064(u8 *, s32, s32);
extern u8 D_80104E70;
extern u8 D_800FDD10[];
extern U16Pair D_800FB1F0[];
extern char D_800F96E4[];
s32 func_800EEA98(s32);
s32 *func_800F4FB4(s32);
u8 *func_800F5064(u8 *);
u8 *func_800F4E70(u8 *);
typedef struct Unk800D9134 {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s16 unk8[4];
    s16 unk10[4];
} Unk800D9134;
s32 func_800D9134(Unk800D9134 *, s32, s32 *);
extern s32 D_80104DE8;
extern s32 D_800F78B4[];
extern u16 D_800F8CB4[];
extern s32 (*D_800F8E38[])(void);
s32 func_800E4134(s32);
u16 *func_800E4530(s32);
s32 func_800D6188(s32);
extern u8 D_800F727C[][2];  /* summon id upgrades */
extern u16 D_800F72A0[16][2];
void func_800C30B8(void);
u32 func_800DF940(BtlUnit9C *, u16, s32);
extern Unk800F3F10 D_800F9058;
extern Unk800F3F10 D_800F9064;
extern U16Pair D_800F9070[];
s32 func_800C1F7C(s32);
s32 func_800D8CB4(u8 *, s32, s32);
void func_800EF044(void);
extern u16 D_800F6A74[];
extern struct Unk80100DC054 *D_80100E14; /* D_80100DC0.unk54 */
s32 func_800D9FD8(u8 *, s32, s32 *);
u8 func_800F29A4(void);
void func_800F2890(s32);
void func_800EBA10(Window *);

/* main exe: candidates for ff9.h */
void func_800562E0(s32 *, s32);
void func_80029058(s32, s32, s32, u8 *, s32);
void func_8002DD54(void);
s32 func_80033A1C(Character *);

/* battle menu: unit under the target cursor (inlined by its callers) */
static inline BtlUnit *BtlFindTargetCursor(void) {
    BtlUnit *p;
    for (p = D_8006794C->unk1C->unk8E4.battle->unk6F8; p != NULL; p = p->unk0) {
        if ((p->unk68 & 0x30000000) == 0x30000000) {
            return p;
        }
    }
    return NULL;
}

/* inlined copy of func_800F1B78 */
static inline s32 func_800F1B78_inline(void) {
    BtlUnit *u;
    s32 n;
    if (D_80104FF0.unk2 >= 9 && D_80104FF0.unk2 <= 11) {
        return 2;
    }
    n = 0;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if ((u->unk68 & 0x30000000) == 0x30000000) {
            n++;
        }
    }
    return n >= 2;
}

/* Queue-entry validity check inlined into func_800EAE8C. */
static inline u8 BtlUnitCanAct(BtlUnit *u) {
    if (u == NULL) {
        return 0;
    }
    if (u->unk28 == 0 || (u8)func_800C09A4(u, 0x42021D03) != 0) {
        return 0;
    }
    if ((D_800FDF30.unkA >> ((u8)u->unk68 >> 5)) & 1) {
        return 1;
    }
    return 0;
}

/* -O1 unit (ovl_0b_o1.c) */
s32 func_800CA2B0(s32);
void func_800C416C(s32, Unk800C412C *, s16 *);
s16 func_800CF9B0(s32, s32, s32);

typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
    u16 unk4;
} Unk800C3F78;
void func_800C3F78(Unk800C412C *, Unk800C3F78 *);
void func_800C412C(Unk800C412C *, u8 *);
void func_800C52B4(MATRIX *, SVECTOR *, SVECTOR *);
void func_800C5530(SVECTOR *, SVECTOR *);
void func_800C4A3C(s16 *, s16 *);
void func_800C493C(s16 *);
s32 func_800CF9DC(s32, s32, s32, s32);
s32 func_800CF708(s32, s32);
s32 func_800CF6B8(s32, s32);
s32 func_800EA004(void);
s32 func_800EA094(void);

typedef struct {
    u8 pad0[0x17];
    u8 unk17;
    u8 pad18[0x44 - 0x18];
    RECT *unk44;
    s16 unk48;
} Unk800DA790;

typedef struct {
    s32 unk0; /* frames */
    s32 unk4; /* shade */
} Unk800E6410;

void func_800D0A0C(u32 *, s32, s32, s32, s32);

void func_800E73EC(s32, s16 *);
void func_800E76DC(s32, s16 *);
void func_800E7650(s32, s16 *);

/* main exe: candidates for ff9.h */
void LoadAverageShort12(SVECTOR *, SVECTOR *, s32, s32, SVECTOR *);

#endif
