#ifndef FX_0EB_H
#define FX_0EB_H

/* Effect module fx_0eb (dir 13 file 0x0EB). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* target position */
    SVECTOR unk1C; /* target home position */
    SVECTOR unk24;
    SVECTOR unk2C;
    s16 unk34; /* lift countdown */
    s16 unk36; /* drop countdown */
    s16 unk38; /* hold up */
} Fx0ebWork;

extern Unk800F78A4 *fx_0eb_env;
extern s32 D_801E7C34[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
