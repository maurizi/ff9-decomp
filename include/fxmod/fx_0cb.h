#ifndef FX_0CB_H
#define FX_0CB_H

/* Effect module fx_0cb (dir 13 file 0xCB). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* scale */
    SVECTOR unk4; /* position */
    SVECTOR unkC; /* rotation */
    s16 unk14; /* spin angle */
    s16 unk16; /* spin speed */
} Fx0cbPart; /* 0x18 */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0cbVec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* priority */
    SVECTOR unkC; /* caster body position */
    Fx0cbVec3 unk14;
    s32 unk20; /* model */
    s32 unk24; /* model */
    s32 unk28;
} Fx0cbWork; /* 0x2C */

extern BtlModel D_801E8680[];

extern Unk800F78A4 *D_801E8670;

void func_801E7EB8(s16 *a, s16 *b);
s32 func_801E7EFC(s32 mode, Fx0cbPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx0cbWork *work);

/* propagated */
void func_801E7704(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
