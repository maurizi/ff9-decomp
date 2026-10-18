#ifndef FX_083_H
#define FX_083_H

/* Effect module fx_083 (dir 13 file 0x083). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx083Vec3;

typedef struct {
    s32 unk0[10]; /* countdowns */
    s32 unk28[4]; /* ring models */
    SVECTOR unk38[4]; /* ring rotations */
    VECTOR unk58[4]; /* ring positions */
    Fx083Vec3 unk98; /* ring scale */
    s32 unkA4;
    s32 unkA8; /* ring radius offset */
    s32 unkAC; /* ring height */
    s32 unkB0[5]; /* pillar models */
    s32 unkC4[5]; /* pillar active */
    s32 unkD8[5]; /* pillar fade */
    VECTOR unkEC[5]; /* pillar positions */
    s32 unk13C; /* fade */
    SVECTOR unk140; /* pillar rotation */
    Fx083Vec3 unk148; /* pillar scale */
    s32 unk154;
    SVECTOR unk158; /* center */
    SVECTOR unk160; /* caster position */
    s32 unk168; /* radius */
    s32 unk16C; /* height */
    Pool6CEntry *unk170[4];
    SVECTOR unk180[4];
    SVECTOR unk1A0[4];
    s32 unk1C0; /* orbit angle */
} Fx083Work; /* 0x1C4 */

typedef struct {
    s32 unk0;
    s32 unk4;
} Fx083Pair;

extern Fx083Pair D_801E8328[];
extern Fx083Pair D_801E8348[];
extern Unk800F78A4 *D_801E83B0;
extern BtlModel D_801E83C0[];

#endif
