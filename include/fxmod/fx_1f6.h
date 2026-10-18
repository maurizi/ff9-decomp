#ifndef FX_1F6_H
#define FX_1F6_H

/* Effect module fx_1f6 (dir 13 file 0x1f6). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* model */
    SVECTOR unk4; /* rotation */
    VECTOR unkC; /* position */
    VECTOR unk1C; /* scale */
} Fx1f6Model; /* 0x2C */

typedef struct {
    s32 unk0[8]; /* countdowns */
    Fx1f6Model unk20[3];
    SVECTOR unkA4; /* target position */
    SVECTOR unkAC; /* target body position */
} Fx1f6Work; /* 0xB4 */

extern Unk800F78A4 *D_801E7DD4;
extern BtlModel D_801E7DE4[];
extern s32 D_801E8044[];
extern s32 D_801E92B4[];

#endif
