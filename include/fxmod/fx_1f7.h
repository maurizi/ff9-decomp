#ifndef FX_1F7_H
#define FX_1F7_H

/* Effect module fx_1f7 (dir 13 file 0x1f7). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[33];
    s32 unk10C; /* passes left */
    s32 unk110; /* window start */
    s32 unk114; /* delay */
} Fx1f7Trail; /* 0x118 */

typedef struct {
    s32 unk0; /* point */
    s32 unk4;
    s32 unk8; /* repeats left */
    s32 unkC; /* frame */
    s32 unk10; /* delay */
} Fx1f7Spark; /* 0x14 */

typedef struct {
    s32 unk0; /* trails on */
    s32 unk4; /* sparks on */
    s32 unk8; /* flash */
    SVECTOR unkC; /* caster body position */
} Fx1f7Work; /* 0x14 */

extern Unk800F78A4 *D_801E80C8;
extern s32 D_801E7EB0[6][17]; /* bones */
extern s32 D_801E8048[2][7]; /* bones */
extern SVECTOR D_801E80D8[64];
extern SVECTOR D_801E82D8[48];
extern Fx1f7Trail D_801E8458[6];
extern Fx1f7Spark D_801E8AE8[12];

#endif
