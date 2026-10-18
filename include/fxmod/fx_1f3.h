#ifndef FX_1F3_H
#define FX_1F3_H

/* Effect module fx_1f3 (dir 13 file 0x1f3). */
#include "btlfx_mod.h"

/* copied */
extern Unk800F78A4 *D_801E8DB0;
/* end copied */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1f3Vec3;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[33];
} Fx1f3Line; /* 0x10C */

typedef struct {
    MATRIX unk0; /* bone matrix */
    Fx1f3Line unk20;
    s32 unk12C; /* timer */
    s32 unk130; /* bone */
    s32 unk134; /* angle */
    s32 unk138; /* angle speed */
    s32 unk13C; /* radius */
    s32 unk140; /* angle step */
} Fx1f3Ring; /* 0x144 */

typedef struct {
    s32 unk0; /* model */
    s32 unk4; /* spin */
    SVECTOR unk8; /* rotation */
    Fx1f3Vec3 unk10; /* scale */
    u8 pad1C[4];
    Fx1f3Vec3 unk20; /* position */
    u8 pad2C[4];
} Fx1f3Rock; /* 0x30 */

typedef struct {
    s32 unk0; /* bone */
    s32 unk4; /* radius */
    s32 unk8;
} Fx1f3RingDef;

typedef struct {
    s32 unk0[16]; /* timers */
    Pool6CEntry *unk40[7];
    SVECTOR unk5C; /* target position */
    SVECTOR unk64; /* bone position */
    s32 unk6C; /* model */
    Fx1f3Vec3 unk70; /* model position */
    u8 pad7C[4];
    Fx1f3Vec3 unk80; /* model scale */
    u8 pad8C[4];
    SVECTOR unk90; /* model rotation */
    s32 unk98; /* models */
    s32 unk9C;
    SVECTOR unkA0;
    SVECTOR unkA8;
    Fx1f3Vec3 unkB0;
    u8 padBC[4];
    s32 unkC0;
    s32 unkC4;
    SVECTOR unkC8;
    SVECTOR unkD0;
    Fx1f3Vec3 unkD8;
    u8 padE4[4];
    s32 unkE8;
    s32 unkEC;
    SVECTOR unkF0;
    Fx1f3Vec3 unkF8;
    u8 pad104[4];
    SVECTOR unk108; /* camera */
    SVECTOR unk110;
    SVECTOR unk118;
} Fx1f3Work80; /* 0x120 */

extern Fx1f3RingDef D_801E8CA8[16];
extern BtlModel D_801E8DC0[];
extern s32 D_801EA6C0[];
extern Fx1f3Rock D_801EC370[8];
extern Fx1f3Ring D_801EC4F0[16];
extern Fx1f3Line D_801EC510;
void func_801E7704(s32 t, s32 n, s32 from, s32 to, s32 arg4, s32 ot);
s32 func_801E7794(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
