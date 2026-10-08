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
    u8 pad0[8];
    BtlUnit9C8 *unk8;
    BtlUnit9CC *unkC;
    u8 pad10[0x14 - 0x10];
    u32 unk14;
    u8 pad18[0x20 - 0x18];
    BtlBone *unk20;
    u8 pad24[0x50 - 0x24];
    BtlUnit9C50 *unk50;
} BtlUnit9C;

typedef struct {
    u16 unk0; /* max hp */
    s16 unk2; /* max mp (<< 4) */
} BtlUnit24;

/* battle unit; 8 of them at Battle.unk6FC, linked through unk0 */
typedef struct BtlUnit {
    struct BtlUnit *unk0; /* next */
    u8 pad4[0x24 - 4];
    BtlUnit24 *unk24;
    u16 unk28; /* hp */
    s16 unk2A; /* mp */
    s16 unk2C;
    u8 pad2E[0x38 - 0x2E];
    u32 unk38; /* status */
    u32 unk3C; /* status */
    u8 pad40[0x52 - 0x40];
    u8 unk52;
    u8 pad53[0x5C - 0x53];
    u8 unk5C;
    u8 pad5D;
    s16 unk5E;
    s16 unk60;
    u8 pad62[0x68 - 0x62];
    u32 unk68; /* flags: bit 0 player, bits 1-4 character slot, bits 5-7 id */
    u8 pad6C[0x6D - 0x6C];
    u8 unk6D;
    u8 pad6E[0x74 - 0x6E];
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
    u8 pad84[0x9C - 0x84];
    BtlUnit9C *unk9C;
    BtlUnitA0 *unkA0;
    s32 *unkA4;
    BtlUnit9C *unkA8;
    u8 padAC[0xB0 - 0xAC];
    u16 unkB0;
    u8 padB2;
    u8 unkB3;
    u8 padB4[0xB9 - 0xB4];
    u8 unkB9;
    u8 padBA[0xC4 - 0xBA];
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
    u8 pad4[4];
    u8 *unk8; /* script pointer */
    u8 padC[0x12 - 0xC];
    s16 unk12;
    u8 pad14[0x17 - 0x14];
    u8 unk17;
    s16 unk18;
    u8 pad1A;
    u8 unk1B;
    u8 pad1C[0x22 - 0x1C];
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
    u8 pad25[2];
    u8 unk27;          /* count of unk28 */
    BtlUnit *unk28[7]; /* ids 8..14 */
    BtlUnit *unk44;    /* id 0x11 */
} BtlUnitList;

typedef struct {
    s16 unk0;
    u8 pad2[0xF - 2];
    u8 unkF;
    u8 pad10[0x1C - 0x10];
    u32 unk1C;
} Unk800F78A40;

typedef struct {
    Unk800F78A40 *unk0;
    u8 pad4[0xC - 4];
    BtlUnitList *unkC;
    u8 pad10[0x20 - 0x10];
    s32 *unk20;
    u8 *unk24; /* next free primitive */
    s32 unk28;
    BtlUnit *unk2C;
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
    u8 pad1C[0x20 - 0x1C];
    u8 unk20;
    u8 pad21;
    u8 unk22;
    u8 pad23;
    u8 unk24;
} Unk800FEC40;
extern Unk800FEC40 D_800FEC40;

typedef struct {
    u8 pad0[0x2C];
    s16 unk2C[2];
    u8 pad30[0x58 - 0x30];
    s32 unk58;
    s32 unk5C;
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
    u8 pad0[8];
    u8 unk8;
} Battle1330;

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
    u8 pad64[0x74 - 0x64];
} Battle1334; /* 0x74 */

/* GameData.unk8E4 while in battle */
typedef struct Battle {
    u8 pad0[0x488];
    Battle488 unk488[16];
    u8 pad608[0x6F8 - 0x608];
    BtlUnit *unk6F8; /* list of active units */
    BtlUnit unk6FC[8];
    u8 padD7C[0x122C - 0xD7C];
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
    u8 pad1694[0x23CC - 0x1694];
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

extern BtlUnit D_800FE3B8;

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
    u8 pad0[0x34];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s16 unk40[3];
    s16 unk46[14];
} Unk800C09EC;

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
    s32 unk0;
    u8 pad4[0x4C - 4];
    u8 unk4C; /* in use */
    u8 unk4D;
    s16 unk4E; /* index */
    u8 pad50[0x54 - 0x50];
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
extern u8 D_801014B8[];
extern u8 D_801044B8[];
extern u8 D_801044C8[];
extern Unk80100D88 D_80104DF0;

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

/* main exe: candidates for ff9.h */
s8 func_80021698(void);

/* ovl_0b functions */
s32 func_800B4040(BtlUnit *);
void func_800C316C(s32);
void func_800C36D0(void);
void func_800CAEC4(void);
Unk800CC278 *func_800CC278(s32);
s32 func_800CC444(s16);
s32 func_800AE47C(s32, s32);
void func_800C22CC(BtlUnit *, s32, s32);
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
void func_800E00DC(s32, u8 *, u8 *);
void func_800E14D8(s32, u8 *);
void func_800E1534(u8 *, u8 *, s32, s32);
void func_800E1708(u8 *, s32, s32);
void func_800E1810(u8 *, s32);
void func_800E3B34(Heap *, s32);
void func_800E3DA8(void);
void func_800E4098(void);
s32 func_800E788C(void);
void func_800E8B28(s32, s32, s32);
s32 func_800A91EC(void);
void func_800BD4DC(BtlUnit *);
void func_800BD53C(BtlUnit *);
s32 func_800CFC0C(s32);
void func_800D07B4(s32, s32 *);
s32 func_800D6360(void);
void func_800D64C8(s32);
void func_800D85AC(s32, s32);
void func_800D85F8(s32, s32, s32);
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
