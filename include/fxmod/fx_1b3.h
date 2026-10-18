#ifndef FX_1B3_H
#define FX_1B3_H

/* Effect module fx_1b3 (dir 13 file 0x1b3). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0; /* velocity */
    s16 unk2;
    s16 unk4;
    s16 unk6; /* frames left */
    s16 unk8; /* spin */
    s16 unkA;
    s16 unkC;
    s16 unkE; /* scale step */
    s16 unk10; /* center */
    s16 unk12;
    s16 unk14;
    s16 unk16; /* scale */
} Fx1b3Piece; /* 0x18 */

typedef struct {
    s32 unk0; /* model */
    s32 pad4;
    s32 unk8[8]; /* primitive counts */
    SVECTOR *unk28[8]; /* vertices */
    Fx1b3Piece *unk48[8]; /* pieces */
} Fx1b3Split;

extern u8 *D_801EB0C0;

typedef struct {
    s32 unk0; /* mode */
    s16 unk4; /* target */
    s16 unk6;
    s16 unk8;
    s16 padA;
    s32 unkC; /* radius */
} Fx1b3Blast;

extern Fx1b3Blast D_801EB0D4;

/* copied */
extern Unk800F78A4 *D_801EB074;
/* end copied */

#endif
