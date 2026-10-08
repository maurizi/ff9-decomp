#ifndef OVL_0B_H
#define OVL_0B_H

/* Declarations local to overlay 0x0B (battle, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* ---- battle units ---- */

typedef struct BtlUnitA0 {
    u8 pad0[0x20];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 pad26[0x3A - 0x26];
    u8 unk3A;
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
    u8 pad12[0x1C - 0x12];
    u32 *unk1C; /* primitives */
    u8 pad20[0x28 - 0x20];
} BtlMesh; /* 0x28 */

typedef struct BtlUnit9C8 {
    u8 pad0[3];
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
} BtlUnit9CC;

typedef struct {
    s32 unk0[5];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} BtlBone; /* 0x20 */

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0xC - 5];
    s32 unkC;
    u8 pad10[0x12 - 0x10];
    s16 unk12;
} BtlUnit9C50; /* 0x14 */

typedef struct BtlUnit9C {
    u16 unk0; /* flags */
    u8 pad2[8 - 2];
    BtlUnit9C8 *unk8;
    BtlUnit9CC *unkC;
    u8 pad10[0x14 - 0x10];
    u32 unk14;
    u8 pad18[0x20 - 0x18];
    BtlBone *unk20;
    MATRIX unk24;
    u8 pad44[0x50 - 0x44];
    BtlUnit9C50 *unk50;
    s16 unk54;
    s16 unk56;
    s16 unk58;
} BtlUnit9C;

typedef struct BtlUnit98 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} BtlUnit98;

typedef struct {
    u16 unk0; /* max hp */
    s16 unk2; /* max mp (<< 4) */
} BtlUnit24;

/* battle unit; 8 of them at Battle.unk6FC, linked through unk0 */
typedef struct BtlUnit {
    struct BtlUnit *unk0; /* next */
    struct BtlCmd *unk4;
    s32 unk8;
    u8 padC[0x10 - 0xC];
    struct BtlCmd *unk10;
    struct BtlCmd *unk14;
    u8 pad18[0x1C - 0x18];
    u16 unk1C[4];
    BtlUnit24 *unk24;
    u16 unk28; /* hp */
    s16 unk2A; /* mp */
    s16 unk2C;
    u8 pad2E[0x32 - 0x2E];
    u8 unk32;
    u8 unk33;
    s32 unk34;
    u32 unk38; /* status */
    u32 unk3C; /* status */
    s16 unk40[3];
    s16 unk46[14];
    u8 pad62[0x68 - 0x62];
    u32 unk68; /* flags: bit 0 player, bits 1-4 character slot, bits 5-7 id */
    u8 pad6C[0x6D - 0x6C];
    u8 unk6D;
    u8 pad6E;
    u8 unk6F;
    u8 pad70[0x74 - 0x70];
    u8 *unk74;
    u8 unk78;
    u8 unk79;
    u8 unk7A;
    u8 pad7B;
    u8 unk7C;
    u8 pad7D;
    u16 unk7E;
    s16 unk80;
    s16 unk82;
    u8 pad84[0x86 - 0x84];
    s16 unk86[3];
    s32 unk8C; /* position, 16.16 */
    s32 unk90;
    s32 unk94;
    struct BtlUnit98 *unk98;
    BtlUnit9C *unk9C;
    BtlUnitA0 *unkA0;
    s32 *unkA4;
    BtlUnit9C *unkA8;
    u16 unkAC;
    u16 unkAE;
    u16 unkB0;
    u8 padB2;
    u8 unkB3;
    struct Unk80104F88Entry *unkB4; /* menu data (party members) */
    s16 unkB8; /* screen x */
    s16 unkBA; /* screen y */
    u8 unkBC; /* on screen */
    u8 padBD[0xC4 - 0xBD];
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
    u8 pad0[0xC];
    u16 unkC;
} BtlScript4;

typedef struct BtlScript {
    u32 unk0; /* flags */
    BtlScript4 *unk4;
    u8 *unk8; /* script pointer */
    u8 padC[0x12 - 0xC];
    s16 unk12;
    s16 unk14;
    u8 unk16;
    u8 unk17;
    s16 unk18;
    u8 pad1A;
    u8 unk1B;
    u8 pad1C[0x21 - 0x1C];
    u8 unk21;
    s16 unk22;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
} BtlScript;

/* D_800F78A4->unkC */
typedef struct {
    BtlUnit *unk0;     /* id 0x10 */
    BtlUnit *unk4[8];  /* ids 0..7 */
    u8 unk24;          /* count of unk4 */
    u8 unk25[2];
    u8 unk27;          /* count of unk28 */
    BtlUnit *unk28[7]; /* ids 8..14 */
    BtlUnit *unk44;    /* id 0x11 */
} BtlUnitList;

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
    Unk800F78A40 *unk0;
    u8 pad4[0xC - 4];
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

typedef struct Unk80104F88Entry {
    s16 unk0;
    u16 unk2; /* hp shown */
    u16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA; /* mp shown */
    u16 unkC;
    s16 unkE;
    u8 pad10[0x14 - 0x10];
    Character *unk14;
    u8 pad18;
    u8 unk19;
    u8 pad1A;
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
    u8 pad12B5;
    u8 unk12B6;
    u8 pad12B7[0x12BC - 0x12B7];
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
    u8 pad1;
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
    u8 pad16[0x1C - 0x16];
    s32 unk1C;
} Unk80104E68;
extern volatile Unk80104E68 D_80104E68;

typedef struct {
    s16 unk0;
    u8 pad2[0x18 - 2];
    s32 unk18;
    s32 unk1C;
    u8 unk20;
    u8 pad21;
    u8 unk22;
    u8 pad23;
    u8 unk24;
} Unk800FEC40;
extern Unk800FEC40 D_800FEC40;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s16 unkC;
    s16 unkE;
    u8 pad10[0x1A - 0x10];
    u16 unk1A;
    u8 pad1C[0x2C - 0x1C];
    s16 unk2C[2];
    u8 pad30[0x38 - 0x30];
    s32 unk38;
    struct Heap *unk3C[2];
    u8 pad44[0x50 - 0x44];
    s16 unk50;
    u8 pad52[0x54 - 0x52];
    struct Unk80100DC054 *unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad60[0x64 - 0x60];
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
} Unk80100DC0;
extern Unk80100DC0 D_80100DC0;

typedef struct {
    u8 pad0[0x10];
    u16 unk10;
} Unk80104FD0;
extern Unk80104FD0 D_80104FD0;

extern u8 D_800FE470;
extern s32 D_800F7270;
extern s32 D_800F7274;
extern s32 D_800FE50C;
extern s32 D_800FE510;
extern struct Unk800CEE74 *D_800FE514;
extern s32 D_800FE51C;
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
    u8 pad3[0xC - 3];
    BtlUnit9CC *unkC[2];
    u8 pad14[0x24 - 0x14];
    u16 unk24;
} Battle4880;

typedef struct {
    Battle4880 *unk0;
    u32 unk4;
    u8 pad8[0x18 - 8];
} Battle488; /* 0x18 */

typedef struct {
    void *unk0[4];
    void *unk10[4];
    void *unk20[4];
    void *unk30[4];
} Battle1354;

typedef struct {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0xC - 6];
} Battle1548; /* 0xC */

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

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u8 pad6[8 - 6];
    u8 unk8;
} Battle1330;

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
    u8 pad64[0x74 - 0x64];
} Battle1334; /* 0x74 */

typedef struct {
    u8 pad0[4];
    u16 unk4;
} Battle11C020;

/* battle command (queued action) */
typedef struct {
    u32 unk0_0 : 8;
    u32 unk0_8 : 9;
    u32 unk0_17 : 15;
    u8 pad4[0xA - 4];
    u8 unkA; /* mp cost */
    u8 unkB;
} BtlCmd8;

typedef struct BtlCmd {
    struct BtlCmd *unk0; /* next */
    BtlUnit *unk4;
    BtlCmd8 *unk8;
    s16 unkC;
    u8 unkE;
    u8 unkF;
    u32 unk10_0 : 2;
    u32 unk10_2 : 2;
    u32 unk10_4 : 1;
    u32 unk10_5 : 1;
    u32 unk10_6 : 1;
    u32 unk10_7 : 2;
    u32 unk10_9 : 1;
    u32 unk10_10 : 1;
    u32 unk10_11 : 1;
} BtlCmd;

typedef struct Battle11C0 {
    u8 pad0[0x14];
    u8 *unk14;
    u8 pad18[0x20 - 0x18];
    Battle11C020 *unk20;
    u8 pad24[0x3C - 0x24];
    s32 *unk3C;
    u8 pad40[0x44 - 0x40];
    s32 *unk44;
} Battle11C0;

typedef struct {
    u32 unk0_0 : 4;
    u32 unk0_4 : 1;
    u32 unk0_5 : 3;
    u32 unk0_8 : 21;
    u32 unk0_29 : 1;
    u32 unk0_30 : 1;
    u32 unk0_31 : 1;
    u8 pad4[0xE - 4];
    u16 unkE;
} Battle16CC; /* 0x10 */

typedef struct {
    struct BtlUnit *unk0;
    u8 pad4[0x1C - 4];
} Battle1358; /* 0x1C */

/* Inlined copy of func_800B3CEC (clear a command); the original source inlines it. */
static inline void func_800B3CEC_inline(BtlCmd *arg0) {
    arg0->unk0 = 0;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unkE = 0;
    arg0->unkF = 0;
    arg0->unk10_0 = 0;
    arg0->unk10_2 = 0;
    arg0->unk10_4 = 0;
    arg0->unk10_5 = 0;
    arg0->unk10_6 = 0;
    arg0->unk10_7 = 0;
    arg0->unk10_9 = 0;
    arg0->unk10_10 = 0;
    arg0->unk10_11 = 0;
}

/* GameData.unk8E4 while in battle */
typedef struct Battle {
    u16 unk0; /* flags */
    u8 pad2[8 - 2];
    s32 unk8[8][0x22]; /* per unit: loaded file addresses */
    u8 pad448[0x488 - 0x448];
    Battle488 unk488[16];
    u8 pad608[0x6F8 - 0x608];
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
    u8 padD88[0x1060 - 0xD88];
    BtlCmd unk1060;
    BtlCmd unk1074;  /* list head (only unk0 used) */
    BtlCmd *unk1088; /* current command */
    u8 pad108C[0x11B8 - 0x108C];
    u16 unk11B8;
    u8 unk11BA;
    u8 unk11BB;
    s16 unk11BC;
    u16 unk11BE;
    struct Battle11C0 *unk11C0;
    BtlUnit9CC **unk11C4;
    u8 *unk11C8;
    s32 unk11CC[24];
    Battle122C unk122C[4];
    s32 unk130C;
    s32 unk1310;
    u8 pad1314[0x131C - 0x1314];
    u8 unk131C;
    u8 pad131D[0x132C - 0x131D];
    u32 unk132C;
    Battle1330 *unk1330;
    Battle1334 *unk1334;
    u8 pad1338[0x133C - 0x1338];
    s16 *unk133C; /* offset table, entries relative to its start */
    u8 pad1340[0x1354 - 0x1340];
    Battle1354 *unk1354;
    Battle1358 unk1358[16];
    s16 unk1518;
    s16 unk151A;
    s32 unk151C;
    s32 unk1520;
    s32 unk1524;
    u8 pad1528[0x152C - 0x1528];
    u8 unk152C;
    u8 pad152D[0x1538 - 0x152D];
    u8 *unk1538;
    u8 pad153C[0x1548 - 0x153C];
    Battle1548 unk1548[20];
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
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Vec4; /* 16-byte block, copied as a whole */

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

typedef struct Unk800FE3B8 {
    u8 pad0[0x52];
    u8 unk52;
    u8 unk53;
    u8 pad54[0x5C - 0x54];
    u8 unk5C;
    u8 pad5D;
    s16 unk5E;
    s16 unk60;
    u8 pad62[0xA8 - 0x62];
    s32 unkA8;
    u8 padAC[0xB9 - 0xAC];
    u8 unkB9;
    u8 padBA[0xD0 - 0xBA];
} Unk800FE3B8;
extern Unk800FE3B8 D_800FE3B8;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    u8 padA[2];
    u8 unkC[16];
    u8 unk1C;
    u8 unk1D;
} Unk800FDF30;
extern Unk800FDF30 D_800FDF30;


typedef struct {
    u8 pad0[0x19];
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
} Unk800C412C;

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

typedef struct {
    u8 pad0[0x1C];
    s16 unk1C[4];
} Unk800B44A0;

typedef struct {
    u8 pad0[0x34];
    s16 unk34;
    u8 pad36[0x3B - 0x36];
    s8 unk3B;
} Unk800BBF30;

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
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x18 - 8];
} Unk80100C78Entry; /* 0x18 */

typedef struct {
    s32 unk0;
    Unk80100C78Entry unk4[11];
} Unk80100C78;
extern Unk80100C78 D_80100C78;
extern Heap D_800FF470; /* 0x1800 bytes */

typedef struct {
    BtlUnit9C *unk0;
    u8 pad4[0x1C - 4];
    u8 unk1C; /* in use */
    u8 pad1D;
    s16 unk1E; /* index */
    u8 pad20[0x28 - 0x20];
} BtlObjSlot; /* 0x28 */
extern BtlObjSlot D_80100F1C[32];

typedef struct {
    BtlUnit9C *unk0;
    u8 pad4[0x4C - 4];
    u8 unk4C; /* in use */
    u8 unk4D;
    s16 unk4E; /* index */
    s16 unk50;
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
typedef struct Pool6CEntry {
    u8 pad0[0x30];
    s16 unk30;
    u8 pad32[0x6C - 0x32];
} Pool6CEntry; /* 0x6C */

typedef struct {
    s32 unk0; /* count */
    s32 unk4;
    s32 unk8;
    Pool6CEntry *unkC;
    Pool6CEntry unk10[1];
} Pool6C;

typedef struct {
    u8 pad0[0x1A];
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
extern u8 D_800F6A54[][8];
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

typedef struct {
    u8 pad0[0x10];
    s8 unk10;
    u8 pad11;
    u8 unk12;
    u8 unk13;
    s32 unk14;
    s16 unk18;
} Unk800CC278;

extern RECT D_800FDF50;
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
} Unk800D7E14;

typedef struct Unk800CEE74 {
    u8 pad0[0xC];
    u8 unkC;
    u8 unkD;
    u8 unkE;
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
extern s8 D_800FEC54;

extern char D_80101470[];

typedef struct Unk80100DC054 {
    u8 pad0[0x50];
    s32 *unk50;
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
    u8 pad0[4];
    u8 unk4;
    u8 pad5;
    u16 unk6; /* frame */
    u16 unk8; /* x */
    u16 unkA; /* y */
    Unk800F3F10 *unkC;
} Unk800F412C; /* 0x10 */

typedef struct {
    u8 unk0;
    u8 pad1[3];
    Unk800F412C unk4[5];
    u32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    BtlUnit *unk68[4];
    s8 unk78[4];
} Unk80105078;
extern Unk80105078 *D_80105078;

extern void *D_80104FA0;
extern s32 (*D_800F8DC4[])(void);

typedef struct {
    u8 unk0; /* cursor */
    u8 unk1;
    u8 pad2[2];
    u8 unk4[4];
    u8 pad8[0x10 - 8];
    u8 unk10;
    u8 unk11;
    u8 pad12[0x18 - 0x12];
    BtlUnit *unk18;
} Unk80104FB0;
extern Unk80104FB0 D_80104FB0;
extern void (*D_800F8D94[])(void);

extern BtlUnit *D_80104FC8;

typedef struct {
    s8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Unk80104FC0;
extern Unk80104FC0 D_80104FC0;

typedef struct {
    u8 pad0[6];
    u8 unk6;
} Unk80104FD4;
extern Unk80104FD4 *D_80104FD4;
extern s32 (*D_800F8E58[])(void);
extern void (*D_800F8E48[])(void);
extern u8 D_80104FE8;

typedef struct {
    u8 unk0;
    u8 pad1;
    u8 unk2;
    u8 pad3[6 - 3];
    u8 unk6;
    u8 pad7[0xB - 7];
    u8 unkB;
    u8 unkC;
    u8 padD[0x11 - 0xD];
    u8 unk11;
    s8 unk12;
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
    u8 unk0;
    u8 pad1[0x80 - 1];
    s32 unk80;
    s32 unk84;
    s16 unk88;
    u16 unk8A;
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
extern u32 D_80104E80;
extern void *D_801050D8;
extern u8 D_801051E0;
extern u8 D_80104E94[2][0x18];

/* main exe: candidates for ff9.h */
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
void func_800F2C74(void);
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
void func_800EB518(Unk800EB518 *, s32, s32, s8);
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
void func_800BC78C(BtlUnit *);
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
void func_800CD5C8(s32, ...);
u8 *func_800CD75C(u8);
u32 func_800D89E8(s32);
s32 func_800D9F5C(u8 *, s32, s32);
void func_800DC634(char *);
void func_800E3D80(void);
u8 func_800EA038(void);
void func_800EA164(s32);
void func_800E9F68(u8);
void func_800F4DFC(void);
s32 func_800AB26C(void **, s32);
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
s32 func_800D9260(u8 *, s32, s32);
s32 func_800D960C(struct Unk800D7E14 *, s32, s32);
void func_800AD90C(s32, s32, s32, s32, s32);
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
void func_800DF7A0(BtlUnit9C *, s32, s32, s32, s32);
s32 func_800E7298(BtlUnit *);
u16 func_800E7370(BtlUnit *);
void func_800E748C(BtlUnit *, s16 *);
void func_800E753C(BtlUnit *, s32, s32);
void func_800E78B4(s32, s32, s32, s32, s32, s32, s32);
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
void func_800CAEC4(void);
Unk800CC278 *func_800CC278(s32);
s32 func_800CC444(s16);
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
void func_800E1534(Pool30 *, Pool6C *, s32, s32);
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
void func_800D07B4(s32, s32 *);
s32 func_800D6360(void);
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
    u8 pad2[4 - 2];
} Unk800E8F50Entry;

typedef struct {
    u16 unk0; /* magic 'so' */
    u16 unk2;
    u16 unk4; /* offset */
    u16 unk6; /* offset */
    Unk800E8F50Entry unk8[1];
} Unk800E8F50;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x20 - 0x18];
    s32 unk20;
    s32 unk24;
} Unk800AA08C;

s32 func_800D955C(Unk800D955C *arg0, s32 arg1, s32 *arg2);
s32 func_800D72BC(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s16 *arg4, s32 arg5);
void func_800CFA00(s16 *, s16 *, s32, s16 *);
void func_800DB6B8(void *, Unk800E8F50Entry *, void *, s32);
void func_800DB4EC(void *);
void func_800CF808(s16 *, s16 *, s16 *);
void func_800CFD78(s16 *, s16 *, s32, s16 *);

typedef struct {
    u8 pad0[2];
    u16 unk2; /* text offset into D_800FD8CC */
    u8 pad4[8 - 4];
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

typedef struct {
    u32 *unk0; /* next free primitive */
    u32 *unk4; /* ordering table entry */
    s32 unk8;  /* rgb */
    u8 padC[0x10 - 0xC];
    s32 unk10; /* xy */
    u16 unk14; /* clut */
} Unk800CCD00;

typedef struct {
    s32 unk0; /* uv */
    u8 unk4;  /* w */
    u8 unk5;  /* h */
} Unk800CCD00_1;

extern u8 D_800FE488[0x64];
extern u8 D_800FE4CE[]; /* inside D_800FE488 */
extern u8 D_800FE4E2[]; /* inside D_800FE488 */
extern u32 D_800F8EA4[][4];
extern void (*D_800F8E94[])(void);
extern RECT D_800FDE58;
u8 *func_800F511C(u8 *, RECT *);
void func_800C7B84(BtlUnit9C *);
void func_800DCB20(BtlUnit9C *, s32);
void func_800C57A4(s32, u8 *);
u32 func_800CF690(void);
s32 func_800D7D64(s32, Unk800D955C *, s32, s32 *, s32 *);
s32 func_800D8414(s32, u8 *, s32, s32 *, s32 *);


extern u8 D_80104E6A;

s32 func_8005553C(s32);
void func_80055850(s32, s32, s32);
s32 func_80034430(s32, s32);
void func_800F3D3C(s32);
void func_800F1BFC(Window *);
s32 func_800D001C(s32, s32, s32 *);
void func_800C4E84(s32, s16 *, s16 *, s16 *);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s16 unk4;
    u8 pad6[8 - 6];
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
} Unk800D805C;

extern s8 D_800FEC56;
s32 func_800D9488(Unk800D7C9C *, s32, s32 *);
s32 func_800D9AB0(Unk800D805C *, s32, s32 *);
s32 func_800D9D58(Unk800D805C *, s32, s32 *);
s32 func_800D8ACC(s32, s32);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[0x44 - 8];
    s32 unk44;
    u8 pad48[0x4C - 0x48];
} Unk800FE368;
extern Unk800FE368 D_800FE368;
extern u8 D_800FE4D8[];
extern u8 D_80104E76;
void func_80063864(void *, void *, s32);
void func_800C94B8(void);
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
    u8 pad0[8];
    s16 unk8; /* heap index */
    s16 unkA; /* use the shared heap */
} Unk800D6D8C;

typedef struct {
    u32 tag;
    u32 code[5];
} DR_MOVE;

extern s32 D_800F78AC;
void SetDrawMove(DR_MOVE *p, RECT *rect, int x, int y);
void func_8005562C(s32);
void func_800BCAA4(BtlUnit *);
s32 func_800E3B70(Heap *, s32);
void func_800C2598(BtlCmd *, s32, s32);

#endif
