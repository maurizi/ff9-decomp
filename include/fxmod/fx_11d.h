#ifndef FX_11D_H
#define FX_11D_H

/* Effect module fx_11d (dir 13 file 0x11D): enemy Absorb (Garnet/Prison Cage). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    SVECTOR pos1C; /* caster position */
    SVECTOR pos24;
    SVECTOR pos2C;
} Fx11dWork;

typedef struct {
    u16 bone0; /* bone from */
    u16 bone2; /* bone to */
    u16 blend4; /* blend */
} Fx11dPathKey;

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*motion8)(VECTOR *, s32, SVECTOR *, s32, s32, s32);
} Fx11dEffHdr;

extern Fx11dPathKey fx_11d_boneKeys0[4];
extern Fx11dPathKey fx_11d_boneKeys1[4];
extern Unk800F78A4 *fx_11d_env;
extern s32 fx_11d_op80Trails0[];
extern s32 fx_11d_op80Trails1[];
extern s32 fx_11d_op80Trails2[];
extern SVECTOR fx_11d_emitPos0;
extern SVECTOR fx_11d_emitPos1;

static s32 fx_11d_op80ParticleMotion0(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_11d_op80ParticleMotion1(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_boneLerpParticle(VECTOR *out, SVECTOR *pos, Fx11dPathKey *key, s32 frame, s32 n);

#endif
