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

typedef struct BtlUnit9C {
    u8 pad0[8];
    BtlUnit9C8 *unk8;
} BtlUnit9C;

typedef struct BtlUnit {
    u8 pad0[0x68];
    u32 unk68; /* flags */
    u8 pad6C[0x9C - 0x6C];
    BtlUnit9C *unk9C;
    BtlUnitA0 *unkA0;
} BtlUnit;

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
    u8 pad0[0xC];
    BtlUnitList *unkC;
    u8 pad10[0x24 - 0x10];
    s32 unk24;
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
    u8 pad0[0x58];
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

/* ovl_0b functions */
s32 func_800D8C70(s32, s32);
void func_800D8658(u8, s32);
void func_800D8914(u8);
void func_800D8A20(u8, s32);
BtlUnit *func_800E716C(s32);
void func_800E9D84(void);
void func_800EC070(void);
void func_800EC14C(s32);
void func_800EC170(s32);
void func_800ECB5C(void);
s32 func_800EB2BC(s32);
u8 func_800F1360(void);

#endif
