#ifndef FX_0BA_H
#define FX_0BA_H

/* Effect module fx_0ba (dir 13 file 0xBA). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[1];
} Fx0baStrip;

typedef struct {
    s32 unk0; /* position, 20.12 */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14; /* trail index */
    s16 unk16; /* point index */
    u8 *unk18; /* strip point */
} Fx0baPart; /* 0x1C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[17];
} Fx0baTrail; /* 0x8C */

typedef struct {
    u8 pad0[8];
    SVECTOR unk8; /* rotation */
} Fx0baWork85; /* 0x10 */

extern Fx0baTrail D_801EED20[5];

typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    u8 unk10; /* path index */
    u8 unk11; /* state */
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15; /* amplitude */
    u8 pad16[2];
    s32 *unk18; /* arc profile */
} Fx0baBolt;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[4];
} Fx0baStrip4; /* 0x24 */

extern s32 D_801EC550;
extern SVECTOR D_801EC554;
extern Fx0baTrail D_801ED420[];
extern Fx0baStrip4 D_801EE5A0[];
extern s32 D_801EC54C;
extern SVECTOR D_801ECAB0;

typedef struct {
    SVECTOR unk0[17];
    s32 unk88;
} Fx0baPath; /* 0x8C */

extern Unk800F78A4 *D_801EC5F0;
extern Fx0baPath D_801ED424[32];
extern s32 D_801EEFDC;

typedef struct {
    s32 *unk0; /* arc profile table */
    s32 unk4; /* spawn counter */
    s32 unk8; /* state */
} Fx0baWork83;

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad[3];
} Fx0baPt; /* 0xC */

extern s32 D_801EEA20[];
extern s32 D_801EC55C;
extern SVECTOR D_801ECAB8;

void func_801E7704(u32 *ot, s32 tpage, s32 w, s32 r, s32 g, s32 b);
void func_801E8778(Unk80100C78Entry *entry, Fx0baStrip *buf, SVECTOR *from, SVECTOR *to, s32 amp, s32 s, s32 step,
                   s32 a7, s32 a8, s32 pad);
void func_801EA850(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *buf, s32 size, s32 *tab, SVECTOR *from,
                   SVECTOR *to, s32 r0, s32 r1, s32 s, s32 step, s32 a12, s32 a13, s32 pad, s32 wave);
void func_801EAAF0(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *path, Fx0baStrip *buf, s32 size, s32 *tab,
                   s32 i, s32 a12, s32 a13, s32 pad);
s32 func_801EB1CC(SVECTOR *v);
s32 func_801EBFB8(s32 mode, Fx0baPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
s32 func_801EAC08(s32 mode, Fx0baBolt *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void func_801EC124(void);
void func_801EC130(Unk80100C78Entry *entry, SVECTOR *center, s32 r, SVECTOR *from, SVECTOR *to);
s32 fx_0ba_op83(s32 mode, Fx0baWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *unk180;
    BtlUnit9CC *unk184;
    BtlUnit9CC *unk188;
} Fx0baUnit;
typedef struct {
    u8 pad0[0x54];
    Fx0baUnit *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx0baSummon;
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0baVec3;
typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    u8 pad8[8];
    SVECTOR unk10; /* orb position */
    SVECTOR unk18; /* bone position */
    u8 pad20[8];
    Fx0baVec3 unk28; /* model position */
    Fx0baVec3 unk34; /* model scale */
    SVECTOR unk40; /* model rotation */
    Fx0baVec3 unk48; /* summon position */
    Fx0baVec3 unk54; /* summon scale */
    SVECTOR unk60; /* summon rotation */
    SVECTOR unk68; /* model rotation */
    Fx0baVec3 unk70; /* model scale */
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    s16 unk94;
    s16 unk96;
    s16 unk98;
    s16 unk9A;
    u8 pad9C[4];
} Fx0baWork80; /* 0xA0 */
extern BtlModel D_801EC600[];
s32 fx_0ba_op80(s32 mode, Fx0baWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
