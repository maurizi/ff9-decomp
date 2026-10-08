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
    u8 pad0[8];
    u16 unk8;
} BtlUnit9C8;

typedef struct BtlUnit9CC {
    u8 pad0[2];
    u16 unk2;
} BtlUnit9CC;

typedef struct BtlUnit9C {
    u8 pad0[8];
    BtlUnit9C8 *unk8;
    BtlUnit9CC *unkC;
} BtlUnit9C;

typedef struct BtlUnit {
    u8 pad0[0x38];
    u32 unk38; /* status */
    u32 unk3C; /* status */
    u8 pad40[0x68 - 0x40];
    u32 unk68; /* flags */
    u8 pad6C[0x6D - 0x6C];
    u8 unk6D;
    u8 pad6E[0x74 - 0x6E];
    u8 *unk74;
    u8 pad78;
    u8 unk79;
    u8 pad7A[0x7C - 0x7A];
    u8 unk7C;
    u8 pad7D;
    u16 unk7E;
    u8 pad80[0x9C - 0x80];
    BtlUnit9C *unk9C;
    BtlUnitA0 *unkA0;
    s32 *unkA4;
    u8 padA8[0xC4 - 0xA8];
    u32 *unkC4;
} BtlUnit;

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
    u8 pad0[0xF];
    u8 unkF;
} Unk800F78A40;

typedef struct {
    Unk800F78A40 *unk0;
    u8 pad4[0xC - 4];
    BtlUnitList *unkC;
    u8 pad10[0x20 - 0x10];
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

/* GameData.unk8E4 while in battle */
typedef struct Battle {
    u8 pad0[0x131C];
    u8 unk131C;
    u8 pad131D[0x133C - 0x131D];
    s16 *unk133C; /* offset table, entries relative to its start */
    u8 pad1340[0x1354 - 0x1340];
    u8 *unk1354;
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

typedef struct Unk800FE3B8 {
    u8 pad0[0x5E];
    s16 unk5E;
    s16 unk60;
    u8 pad62[0xD0 - 0x62];
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

/* main exe: candidates for ff9.h */
s8 func_80021698(void);

/* ovl_0b functions */
void func_800A91EC(void);
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
