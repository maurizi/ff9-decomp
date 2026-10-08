#ifndef OVL_0A_H
#define OVL_0A_H

/* Declarations local to overlay 0x0A (field, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* field context, GameData.unk8E4.field */
typedef struct {
    u8 unk0; /* flags */
    u8 pad1[0x14 - 1];
} Unk800AB0B0Entry; /* 0x14 */

typedef struct Unk800AB0B0 {
    u16 unk0; /* count */
    u8 pad2[2];
    Unk800AB0B0Entry unk4[1];
} Unk800AB0B0;

/* 0x330-byte block, FieldMapObj.unk14 (allocated by func_800A9350) */
typedef struct {
    u32 unk0; /* flags */
    u8 pad4[0xA - 4];
    s16 unkA;
    void *unkC;  /* file 0x5xxxx */
    u8 pad10[0x14 - 0x10];
    struct Unk800C9DB4 *unk14;
    void *unk18;
    void *unk1C; /* file 0x1Fxxxx */
    s32 unk20;
    void *unk24;
    u8 *unk28; /* = unk6C */
    u8 pad2C[0x41 - 0x2C];
    u8 unk41;
    u8 pad42[0x4A - 0x42];
    u8 unk4A;
    u8 pad4B;
    void *unk4C; /* file 0x1Exxxx */
    struct Unk800AB0B0 *unk50;
    u8 *unk54; /* = unk90 */
    u8 *unk58; /* = unk270 */
    u8 pad5C[0x6C - 0x5C];
    u8 unk6C[0x90 - 0x6C];
    u8 unk90[0x270 - 0x90];
    u8 unk270[0x330 - 0x270];
} FieldScene;

/* 0x38-byte object hanging off FieldMap.unkC */
typedef struct {
    u8 pad0[4];
    s32 unk4;    /* end of the load area */
    void *unk8;  /* file 0x6xxxx */
    void *unkC;
    u16 unk10;
    u8 pad12[0x14 - 0x12];
    FieldScene *unk14;
    u8 pad18[0x38 - 0x18];
} FieldMapObj;

/* field context, GameData.unk8E4.field (0x50 bytes) */
typedef struct FieldMap {
    u32 unk0; /* flags */
    s32 unk4;
    s8 unk8;
    s8 unk9;
    s16 unkA;
    FieldMapObj *unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x20 - 0x1C];
    s32 unk20;
    u8 unk24[0x2C];
} FieldMap;

/* 0x28-byte entries at D_800C9DF4 */
typedef struct Unk800C9DF4 {
    s16 unk0;
    s16 unk2;
    u16 unk4;
    s16 unk6;
    u8 pad8[0x1C - 8];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
} Unk800C9DF4;

/* 0xC-byte entries at D_800C9DCC */
typedef struct {
    u8 unk0;
    u8 pad1[0xB];
} Unk800C9DCC;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s16 unk4;
    s16 unk6;
} Unk800CA058;

typedef struct {
    u8 pad0[4];
    void (*unk4)(u8);
    u8 pad8[4];
} Unk800C9D5C; /* 0xC */

/* field state (D_800CA068) */
typedef struct {
    s32 unk0; /* flags */
    u16 unk4;
    u16 unk6;
    u8 pad8[0x75 - 8];
    u8 unk75;
    s16 unk76;
    u8 pad78[0xA8 - 0x78];
    s16 unkA8[4][2];
    s16 unkB8[4][2];
    s16 unkC8[4];
    s16 unkD0[4];
} Unk800CA068;

typedef struct Unk800C9DB4 {
    u8 pad0[0x1C];
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u16 unk22;
    u16 unk24;
    u16 unk26;
} Unk800C9DB4;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 *unk18;
    u8 *unk1C; /* 3 bytes per entry */
    u8 *unk20;
    u8 unk24;
    u8 pad25[3];
    void *unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
} Unk800C9DA4;

typedef struct {
    u8 pad0[6];
    u8 unk6; /* count */
    u8 pad7[2];
    u8 unk9;
    u8 padA[2];
    u8 unkC[1][12];
} Unk800B3254;

/* header of the block D_800C9DF0.unk8 points to */
typedef struct {
    u8 pad0[0x2C];
    u16 unk2C; /* entry count for D_800C9DF0.unk20 */
} Unk800C9DF8;

typedef struct {
    u16 unk0; /* flags */
    u8 pad2[2];
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    u8 padC[4];
} Unk800C9E10; /* 0x10 */

typedef struct {
    u8 pad0[8];
    u16 unk8;
    s16 unkA;
    u8 padC[0x28 - 0xC];
} Unk800C9E00; /* 0x28 */

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    struct Unk800C9DF4 *unk4;
    Unk800C9DF8 *unk8;
    u8 padC[0x10 - 0xC];
    Unk800C9E00 *unk10;
    u8 pad14[0x20 - 0x14];
    Unk800C9E10 *unk20;
} Unk800C9DF0;

typedef struct {
    u16 unk0; /* flags */
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s32 unk14;
} Unk800CAE98; /* 0x18 */

/* file 0x1C0000 header */
typedef struct {
    u8 pad0[0x30];
    u16 unk30;
    u16 unk32;
} Unk800AA01C;

typedef struct {
    s16 unk0;
    u8 pad2[2];
    s16 unk4;
    u8 pad6[0x14 - 6];
    s16 unk14;
    u8 pad16[0x1E - 0x16];
    s16 unk1E;
} Unk800C9EF0;

typedef struct {
    u8 unk0;
    u8 unk1; /* index into D_800C9E14 */
} Unk800CB188;

/* ---- overlay functions ---- */
void func_800AD944(s32, s32, s32);
void func_800A8588(void);
void func_800C6EEC();
void func_80054A28(void *);
void func_800B0124(s32, s32, s32, s32, s32);
void func_800B36E0(void);
void func_800B4894(void);
void func_800B3ACC(void);
s32 func_8001E350(s32);
void func_800B31B8(s32, s32, s32, s32);
void func_800B5938(void);
void func_800AC67C(s32);
void func_800C0038(s32, s32);
void func_800A8198(void);
void func_800A82F8(void);
void func_800A7AAC(s32);
void func_800A7B10(s16);
void func_800A85F4(s32);
void func_800A8698(void);
void func_800B57C8(void);
void func_800B1CE4(void);
s32 func_800AA0C8(FieldScene *);
s32 func_800A8990(FieldMapObj *, s16);
void func_800B5430(s32);
s32 *func_800B3BD8(s32);
s32 *func_800B3D10(s32);
void func_800C951C(void);
void func_800C95B8(void);
void func_800C9580(void);
void func_800B559C(void);
void func_800B5660(void);
void func_800B58B8(void);
void func_800C8C08(void);
void func_800C8D40(void);
void func_800C8E2C(void);
void func_800C9630(void);

/* ---- overlay data ---- */
extern u8 D_800A717C[]; /* text */
extern s32 D_800C9D58;
extern Unk800C9D5C D_800C9D5C[];
extern Unk800C9DA4 D_800C9DA4;
extern Unk800C9DB4 *D_800C9DB4;
extern s32 D_800C9DD0;
extern u16 D_800C9DDE;
extern u16 D_800C9DE0;
extern Unk800C9DF0 D_800C9DF0;
extern Unk800C9DCC *D_800C9DCC;
extern s32 D_800C9F20[16];
extern s8 D_800C9FF0[5];
extern s32 D_800C9FF8[5];
extern s32 D_800CA0E0;
extern s32 D_800CA010[8];
extern s32 D_800CA030[8];
extern u8 D_800CA050; /* D_800CA010 count */
extern u8 D_800CA051; /* D_800CA030 count */
extern Unk800CA058 D_800CA058;
extern Unk800CA068 D_800CA068;
extern u8 D_800CAA98[];
extern u8 D_800CAAB8[];
extern Unk800CAE98 D_800CAE98[];
extern Unk800CB188 D_800CB188;
extern void (*D_800C9E14[])(void);
extern Unk800C9EF0 D_800C9EF0;

/* Inlined copy of func_800C8108. */
static inline Unk800CAE98 *func_800C8108_inline(s32 i) {
    return &D_800CAE98[i];
}

#endif
