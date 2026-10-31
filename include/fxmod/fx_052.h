#ifndef FX_052_H
#define FX_052_H

/* Effect module fx_052 (dir 13 file 0x52). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* model rotation */
    SVECTOR unk14; /* second model rotation */
    VECTOR unk1C; /* model position */
    VECTOR unk2C; /* model scale */
    s32 unk3C; /* model brightness */
    SVECTOR unk40; /* target body position */
    SVECTOR unk48; /* target position */
    s16 unk50; /* fade-in countdown */
    u16 unk52; /* models */
    u16 unk54;
    u16 unk56;
} Fx052Work80; /* 0x58 */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* target body position */
} Fx052Work81; /* 0x14 */

typedef struct {
    s16 unk0; /* fade-in countdown */
    s16 unk2; /* fade-out countdown */
} Fx052Work82;

extern Unk800F78A4 *fx_052_env;
extern BtlModel fx_052_effModels[];
extern s32 D_801E8218[];
extern s32 D_801E8FD8[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);

#endif
