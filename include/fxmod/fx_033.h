#ifndef FX_033_H
#define FX_033_H

/* Effect module fx_033 (dir 13 file 0x33). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* knockback countdown */
    SVECTOR pos4;
    SVECTOR unkC; /* target body position */
    SVECTOR unk14; /* target home position */
    SVECTOR unk1C; /* target position while pushed back */
    SVECTOR unk24; /* angles from unk4 to unkC */
    Pool6CEntry *emit2C;
} Fx033Work; /* 0x30 */

extern Unk800F78A4 *fx_033_env;
extern s32 D_801E7AF4[];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
