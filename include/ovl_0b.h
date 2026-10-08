#ifndef OVL_0B_H
#define OVL_0B_H

/* Declarations local to overlay 0x0B (battle, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* ---- battle units ---- */

typedef struct BtlUnitA0 {
    u8 pad0[0x3A];
    u8 unk3A;
} BtlUnitA0;

typedef struct BtlUnit9C8 {
    u8 pad0[3];
    u8 unk3;
    u8 pad4[8 - 4];
    u16 unk8;
    u16 unkA;
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
    u8 pad5[0x14 - 5];
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
    u8 pad24[0x50 - 0x24];
    BtlUnit9C50 *unk50;
    s16 unk54;
    s16 unk56;
    s16 unk58;
} BtlUnit9C;

typedef struct BtlUnit98 {
    u8 pad0[8];
    s32 unk8;
} BtlUnit98;

typedef struct {
    u16 unk0; /* max hp */
    s16 unk2; /* max mp (<< 4) */
} BtlUnit24;

/* battle unit; 8 of them at Battle.unk6FC, linked through unk0 */
typedef struct BtlUnit {
    struct BtlUnit *unk0; /* next */
    s32 unk4;
    s32 unk8;
    u8 padC[0x24 - 0xC];
    BtlUnit24 *unk24;
    u16 unk28; /* hp */
    s16 unk2A; /* mp */
    s16 unk2C;
    u8 pad2E[0x32 - 0x2E];
    u8 unk32;
    u8 pad33;
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
    u8 pad78;
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
    u8 padB4[0xC4 - 0xB4];
    u32 *unkC4;
    u8 padC8[0xCC - 0xC8];
    s16 unkCC;
    u8 unkCE;
    u8 padCF;
} BtlUnit; /* 0xD0 */

/* ability/command calculation context */
typedef struct BtlCalc8 {
    u8 pad0[8];
    u8 *unk8;
    u8 padC[0xF - 0xC];
    u8 unkF;
    u32 unk10;
} BtlCalc8;

typedef struct BtlCalc {
    BtlUnit *unk0; /* caster */
    BtlUnit *unk4; /* target */
    BtlCalc8 *unk8;
    u8 padC[0x12 - 0xC];
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u16 unk18; /* result flags */
} BtlCalc;

/* battle script interpreter context */
typedef struct BtlScript {
    u32 unk0; /* flags */
    s32 unk4;
    u8 *unk8; /* script pointer */
    u8 padC[0x12 - 0xC];
    s16 unk12;
    s16 unk14;
    u8 pad16;
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
    u8 pad2[0x8 - 2];
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
    u8 pad18[0x20 - 0x18];
    s32 *unk20;
    u8 *unk24; /* next free primitive */
    s32 unk28;
    struct Unk800FE3B8 *unk2C;
} Unk800F78A4;
extern Unk800F78A4 *D_800F78A4;

typedef struct {
    u8 pad0[0x12A4];
    s32 unk12A4;
    u8 pad12A8[0x12B4 - 0x12A8];
    u8 unk12B4;
    u8 pad12B5[0x12C4 - 0x12B5];
    u8 unk12C4;
    u8 unk12C5;
    u8 unk12C6;
    u8 pad12C7;
    s32 unk12C8[1];
} Unk80104F88;
extern Unk80104F88 *D_80104F88;

typedef struct {
    u8 pad0[4];
    s32 unk4[(0x80 - 4) / 4];
    s32 unk80[(0x1E0 - 0x80) / 4];
    u8 unk1E0[1];
} Unk80104FA8;
extern Unk80104FA8 *D_80104FA8;

typedef struct {
    u8 unk0;
    u8 pad1[3];
    u8 unk4;
    u8 pad5[0xB - 5];
    s8 unkB;
    u8 padC[0xE - 0xC];
    u8 unkE;
    u8 padF[0x15 - 0xF];
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
    u8 pad30[0x50 - 0x30];
    s16 unk50;
    u8 pad52[0x58 - 0x52];
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
extern s32 D_800FE514;
extern s32 D_800FE51C;
extern s16 D_80100DCC;
extern s32 *D_80104DF8; /* offset table, entries relative to its start */
extern u8 D_80104F90;
extern s8 D_80104FC0;
extern u8 D_80104FF0;
extern u8 D_80104FFC;
extern u8 D_80105020[];
extern u8 D_80105058;
extern u8 *D_80105078;
extern s32 D_80105274;
extern u8 D_8010527C;
extern s32 D_8010528C[];

typedef struct {
    u8 *unk0;
    u8 pad4[0x18 - 4];
} Battle488; /* 0x18 */

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x38 - 8];
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

typedef struct {
    u8 pad0[0xE];
    u8 unkE;
} Battle1088;

typedef struct Battle11C0 {
    u8 pad0[0x20];
    Battle11C020 *unk20;
    u8 pad24[0x3C - 0x24];
    s32 *unk3C;
} Battle11C0;

/* GameData.unk8E4 while in battle */
typedef struct Battle {
    u8 pad0[0x488];
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
    u8 padD88[0x1088 - 0xD88];
    Battle1088 *unk1088;
    u8 pad108C[0x11B8 - 0x108C];
    u16 unk11B8;
    u8 pad11BA;
    u8 unk11BB;
    s16 unk11BC;
    s16 unk11BE;
    struct Battle11C0 *unk11C0;
    u8 pad11C4[0x122C - 0x11C4];
    Battle122C unk122C[4];
    u8 pad130C[0x1310 - 0x130C];
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
    void **unk1354;
    u8 pad1358[0x151C - 0x1358];
    s32 unk151C;
    s32 unk1520;
    u8 pad1524[0x1538 - 0x1524];
    u8 *unk1538;
    u8 pad153C[0x1692 - 0x153C];
    u16 unk1692;
    u8 pad1694[0x22CC - 0x1694];
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
    u8 pad53[0x5C - 0x53];
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
    u8 pad0[0xE];
    u8 unkE;
    u8 unkF;
} Unk800B4434;

typedef struct {
    u8 pad0[0x19];
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
} Unk800C412C;

typedef struct {
    s16 unk0; /* entry stride in bytes */
    s16 unk2; /* entry count */
    u8 pad4[0xC - 4];
    s32 unkC[1];
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


typedef struct {
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
    u8 pad13;
    s32 unk14;
    s16 unk18;
} Unk800CC278;

extern RECT D_800FDF50;
extern u32 *D_800FE360;
extern s32 D_800FE36C;
extern s32 D_800FE490;

typedef struct Unk800B3CEC {
    struct Unk800B3CEC *unk0; /* next */
    u8 pad4[8 - 4];
    s32 unk8;
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
} Unk800B3CEC;

extern u16 D_800FDF38;
extern u8 D_800F8CF4[][4];
extern char D_800A825C[];

typedef struct {
    u8 unk0;
    u8 pad1[3];
    s32 unk4;
} Unk800D8EA4;

typedef struct {
    s16 unk0;
    u8 pad2[8 - 2];
} Unk80104E30Entry;

typedef struct {
    s32 unk0;
    Unk80104E30Entry unk4[6];
} Unk80104E30;
extern Unk80104E30 D_80104E30;

extern u8 D_800FDF3A;
extern s8 D_800FEC54;

/* main exe: candidates for ff9.h */
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
void func_800AD90C(s32, s32, s32, s32, s32);
void func_800B3C00(void);
void func_800C0D24(BtlUnit *);
void func_800C1594(void);
void func_800C86C8(BtlUnit9C *, s32, s32, s32);
void func_800CDEF0(Battle1088 *);
s32 func_800D8D08(u8 *, s32, s32);
s32 func_800D8D68(u8 *, s32, s32);
s32 func_800D905C(u8 *, s32, struct Unk800F78A40 *);
void func_800E8408(BtlUnit *);
void func_800E8460(BtlUnit *);
s32 func_800E976C(s32); /* unit id, or an SVECTOR pointer */
void func_800B1250(s32, s32, s32, s32, s32);
u8 func_800B3E7C(s32);
void func_800BD414(BtlUnit *, u16);
void func_800BD464(BtlUnit *, u16);
s32 func_800C09A4(BtlUnit *, u32);
void func_800C09EC(BtlUnit *);
s32 func_800C0B6C(BtlUnit *, s32);
s32 func_800D87D0(s32);
void func_800DF7A0(BtlUnit9C *, s32, s32, s32, s32);
s32 func_800E7298(BtlUnit *);
u16 func_800E7370(BtlUnit *);
void func_800E748C(BtlUnit *, s32);
void func_800E753C(BtlUnit *, s32, s32);
void func_800E78B4(s32, s32, s32, s32, s32, s32, s32);
void func_800E7A44(s32, VECTOR *);
void func_800E7FC0(BtlUnit *, s32);
void func_800E9A00(s32, s32);
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
void func_800E9930(s32, s32, s32);
s32 func_800B4040(BtlUnit *);
void func_800C316C(s32);
void func_800C36D0(void);
void func_800CAEC4(void);
Unk800CC278 *func_800CC278(s32);
s32 func_800CC444(s16);
s32 func_800AE47C(s32, s32);
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
void func_800D8658(s32, s32);
void func_800D8914(s32);
void func_800D8A20(s32, s32);
BtlUnit *func_800E716C(s32);
void func_800E9D84(void);
void func_800EC070(void);
void func_800EC14C(s32);
void func_800EC170(s32);
void func_800ECB5C(void);
s32 func_800EB2BC(s32);
u8 func_800F1360(void);

#endif
