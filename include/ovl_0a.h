#ifndef OVL_0A_H
#define OVL_0A_H

/* Declarations local to overlay 0x0A (field, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* field context, GameData.unk8E4.field */
typedef struct FieldMap {
    u8 pad0[0xC];
    u8 *unkC;
} FieldMap;

/* field state (D_800CA068) */
typedef struct {
    s32 unk0; /* flags */
    u16 unk4;
    u16 unk6;
    u8 pad8[0x75 - 8];
    u8 unk75;
    s16 unk76;
    u8 pad78[0xC8 - 0x78];
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
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 pad8[0x12 - 8];
    s16 unk12;
    u8 pad14[0x18 - 0x14];
} Unk800CAE98; /* 0x18 */

/* ---- overlay functions ---- */
void func_800AD944(s32, s32, s32);
void func_800B5430(s32);
s32 func_800B3BD8(s32);
s32 func_800B3D10(s32);

/* ---- overlay data ---- */
extern s32 D_800C9D58;
extern Unk800C9DA4 D_800C9DA4;
extern s32 D_800C9DB4;
extern s32 D_800C9DD0;
extern u16 D_800C9DDE;
extern u16 D_800C9DE0;
extern Unk800C9DF0 D_800C9DF0;
extern s32 D_800C9DF4;
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
