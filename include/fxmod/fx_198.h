#ifndef FX_198_H
#define FX_198_H

/* Effect module fx_198 (dir 13 file 0x198). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    s32 unk8; /* scale */
    SVECTOR unkC; /* caster position */
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s32 unk1C; /* model */
    s32 unk20;
} Fx198Work; /* 0x24 */

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} Fx198Key;

extern s16 D_801E7DE8[0x11]; /* file ids */
extern Unk800F78A4 *D_801E7E54;
extern BtlModel D_801E7E64[];

#endif
