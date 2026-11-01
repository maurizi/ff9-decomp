#ifndef FX_12D_H
#define FX_12D_H

/* Effect module fx_12d (dir 13 file 0x12D): enemy Death (Antlion). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* unused */
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx12dWork80;

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4; /* unused */
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10;
    s16 unk18; /* unused */
    s16 unk1A; /* unused */
    s16 unk1C; /* write-only (-1 at init) */
} Fx12dWork81;

typedef struct {
    SVECTOR rot0; /* model rotation */
    VECTOR pos8; /* model position */
    VECTOR scale18; /* model scale */
    s32 unk28; /* unused */
    SVECTOR pos2C; /* caster position */
    u16 model34; /* model */
} Fx12dWork82;

extern BtlTexInfo fx_12d_casterTexPages;
extern s16 fx_12d_texInfo[6][2]; /* fx_12d_casterTexPages's entries in reverse order */
extern BtlTexInfo fx_12d_casterUvOffsets;
extern s16 fx_12d_texPos[6][2]; /* fx_12d_casterUvOffsets's entries in reverse order */
extern Unk800F78A4 *fx_12d_env;
extern BtlModelBuf fx_12d_effModels[];
extern SVECTOR fx_12d_casterPos;
extern s32 fx_12d_op81Trails0[];
extern s32 fx_12d_op81Trails1[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
