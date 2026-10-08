#ifndef OVL_0A_H
#define OVL_0A_H

/* Declarations local to overlay 0x0A (field, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* field context, GameData.unk8E4.field */
/* 0x38-byte object hanging off FieldMap.unkC */
typedef struct {
    u8 pad0[8];
    void *unk8;  /* file 0x6xxxx */
    void *unkC;
    u16 unk10;
    u8 pad12[0x1C - 0x12];
    void *unk1C; /* file 0x1Fxxxx */
    u8 pad20[0x24 - 0x20];
    void *unk24;
    u8 pad28[0x38 - 0x28];
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
typedef struct {
    u8 pad0[4];
    u16 unk4;
    u16 unk6;
    u8 pad8[0x1C - 8];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x28 - 0x24];
} Unk800C9DF4;

/* 0xC-byte entries at D_800C9DCC */
typedef struct {
    u8 unk0;
    u8 pad1[0xB];
} Unk800C9DCC;

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

typedef struct {
    u8 pad0[0x2C];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
} Unk800C9DA4;

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s32 unk4;
    s32 unk8;
} Unk800C9DF0;

typedef struct {
    u16 unk0; /* flags */
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 padC[0x10 - 0xC];
    s16 unk10;
    s16 unk12;
    u8 pad14[0x18 - 0x14];
} Unk800CAE98; /* 0x18 */

/* ---- overlay functions ---- */
void func_800AD944(s32, s32, s32);
void func_800A8588(void);
void func_800B1CE4(void);
s32 func_800AA0C8(FieldMapObj *);
s32 func_800A8990(FieldMapObj *, s16);
void func_800B5430(s32);
s32 func_800B3BD8(s32);
s32 func_800B3D10(s32);
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
extern s32 D_800C9D58;
extern Unk800C9DA4 D_800C9DA4;
extern s32 D_800C9DB4;
extern s32 D_800C9DD0;
extern u16 D_800C9DDE;
extern u16 D_800C9DE0;
extern Unk800C9DF0 D_800C9DF0;
extern Unk800C9DF4 *D_800C9DF4;
extern Unk800C9DCC *D_800C9DCC;
extern s32 D_800C9F20[16];
extern u8 D_800CA05B;
extern s32 D_800CA0E0;
extern u8 D_800CA059;
extern u8 D_800CA05A;
extern s16 D_800CA05C;
extern s16 D_800CA05E;
extern Unk800CA068 D_800CA068;
extern u8 D_800CAA98[];
extern u8 D_800CAAB8[];
extern Unk800CAE98 D_800CAE98[];
extern u8 D_800CB188;

/* Inlined copy of func_800C8108. */
static inline Unk800CAE98 *func_800C8108_inline(s32 i) {
    return &D_800CAE98[i];
}

#endif
