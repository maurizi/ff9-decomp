#ifndef FX_063_H
#define FX_063_H

/* Effect module fx_063 (dir 13 file 0x63). */
#include "btlfx_mod.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 unk6; /* colour */
} Fx063Pt;

/* line strip passed to btlfx_drawTrail: a quadratic curve from the head back to the start point */
typedef struct {
    u8 unk0; /* count - 1 */
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Fx063Pt unk4[11];
    SVECTOR unk5C[9]; /* jitter */
    s32 unkA4[9]; /* jitter speed x */
    s32 unkC8[9]; /* jitter speed z */
    s32 unkEC; /* angle */
    s32 unkF0; /* angle */
    s32 unkF4; /* angle speed */
    s32 unkF8; /* angle speed */
    s32 unkFC; /* colour */
    s32 unk100; /* timer */
} Fx063Strip; /* 0x104 */

/* per unit: bobbing */
typedef struct {
    s32 unk0; /* timer */
    s32 unk4; /* phase */
    s32 unk8; /* phase speed */
    SVECTOR unkC; /* home position */
    SVECTOR unk14; /* position */
} Fx063Unit; /* 0x1C */

typedef struct {
    s32 unk0; /* model */
    SVECTOR unk4; /* rotation */
    VECTOR unkC; /* scale */
} Fx063Model; /* 0x1C */

typedef struct {
    s32 unk0[16]; /* timers */
    Fx063Model unk40[3];
    VECTOR unk94; /* position */
    SVECTOR unkA4; /* eye */
    s32 unkAC; /* model */
    SVECTOR unkB0; /* rotation */
    VECTOR unkB8; /* position */
    VECTOR unkC8; /* scale */
    s32 padD8;
} Fx063Work; /* 0xDC */

extern Unk800F78A4 *D_801E8710;
extern BtlModel D_801E8720[];
extern u32 D_801E8A40[];
extern Fx063Strip D_801E8E40[10];
extern Fx063Unit D_801E9870[];

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
