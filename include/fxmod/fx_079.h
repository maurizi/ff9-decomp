#ifndef FX_079_H
#define FX_079_H

/* Effect module fx_079 (dir 13 file 0x79). */
#include "btlfx_mod.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 unk6; /* colour */
} Fx079Pt;

/* line strip passed to btlfx_drawTrail */
typedef struct {
    u8 unk0; /* count - 1 */
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Fx079Pt unk4[11];
    s32 unk5C; /* end offset */
    s32 unk60;
    s32 unk64;
    s32 pad68;
    s32 unk6C[11]; /* y offset */
    s32 unk98[11];
    s32 unkC4[11]; /* y speed */
    s32 unkF0[11];
    s32 unk11C; /* timer */
} Fx079Strip; /* 0x120 */

typedef struct {
    s32 unk0; /* model */
    SVECTOR unk4; /* rotation */
    VECTOR unkC; /* scale */
} Fx079Model; /* 0x1C */

typedef struct {
    s32 unk0[16]; /* timers */
    Fx079Model unk40[3];
    VECTOR unk94; /* position */
} Fx079Work; /* 0xA4 */

extern Unk800F78A4 *D_801E86F4;
extern BtlModel D_801E8704[];
extern SVECTOR D_801E895C;
extern SVECTOR D_801E8964;
extern SVECTOR D_801E896C;
extern VECTOR D_801E8974;
extern SVECTOR D_801E8984;
extern s32 D_801E898C;
extern Pool6CEntry *D_801E8990;
extern SVECTOR D_801E8994;
extern SVECTOR D_801E899C;
extern Fx079Strip D_801E89A4[5];

s32 func_801E77B8(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
