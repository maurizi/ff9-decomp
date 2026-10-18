#ifndef FX_0B3_H
#define FX_0B3_H

/* Effect module fx_0b3 (dir 13 file 0xB3). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0[8]; /* enemy positions */
    s32 unk40; /* unit count */
} Fx0b3Work86;

extern Unk800F78A4 *D_801ED5E0;
extern s32 D_801EB5D4;
extern SVECTOR D_801EE340[16];

typedef struct {
    u16 unk0; /* file id, track 0 */
    u16 unk2; /* file id, track 1 */
    u16 unk4; /* end frame */
    u16 unk6;
} Fx0b3Track87;

extern Fx0b3Track87 D_801EB5E8[];

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0b3Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[8];
    SVECTOR unk10; /* morph model position */
    u8 pad18[0x28 - 0x18];
    Fx0b3Vec3 unk28; /* morph model position */
    Fx0b3Vec3 unk34; /* morph model scale */
    SVECTOR unk40; /* morph model rotation */
    Fx0b3Vec3 unk48; /* model position */
    Fx0b3Vec3 unk54; /* model scale */
    SVECTOR unk60; /* model rotation */
    Pool6CEntry *unk68;
    s32 unk6C; /* model */
    s32 unk70; /* morph frame */
    s32 unk74; /* morph weight */
} Fx0b3Work80; /* 0x78 */

extern BtlModel D_801ED5F0[];

typedef struct {
    u8 pad0[0x190];
    BtlUnit9CC *unk190;
} Fx0b3Unit;

typedef struct {
    u8 pad0[0x54];
    Fx0b3Unit *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx0b3Summon;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[8];
    SVECTOR unk10; /* morph model position */
    u8 pad18[0x28 - 0x18];
    SVECTOR unk28;
    SVECTOR unk30;
    SVECTOR unk38;
    Fx0b3Vec3 unk40; /* position */
    Fx0b3Vec3 unk4C; /* scale */
    SVECTOR unk58; /* rotation */
    Fx0b3Vec3 unk60;
    Fx0b3Vec3 unk6C;
    SVECTOR unk78;
    Pool6CEntry *unk80;
    Pool6CEntry *unk84;
    s32 unk88[4]; /* models */
    s32 unk98; /* morph frame */
    s32 unk9C; /* morph weight */
} Fx0b3Work81; /* 0xA0 */

extern s32 D_801EE270; /* model */
extern s32 D_801EB5AC[4]; /* start frames */
extern s32 D_801EB5BC[4]; /* file ids */

void func_801E7754(u8 *data, s32 frame, SVECTOR *a, SVECTOR *b);
void func_801E77DC(u8 *data, s32 frame, SVECTOR *a, SVECTOR *b, SVECTOR *c);
extern u16 D_801EB5A0[6]; /* morph model file ids */
extern s32 D_801EE280[6]; /* morph models */

void func_801E7704(u8 *data, s32 frame, SVECTOR *out);

s32 func_801E9DB8(s32 a);
s32 func_801EA6C8(SVECTOR *v);

#endif
