#ifndef FX_127_H
#define FX_127_H

/* Effect module fx_127 (dir 13 file 0x127): enemy Wave (Garland). Evidence: the static sweep of the ability, item
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
    u8 pad1C[0x24 - 0x1C];
    SVECTOR pos24; /* caster position */
    SVECTOR rot2C; /* caster rotation */
    SVECTOR pos34; /* target body position */
    SVECTOR pos3C; /* target position */
    SVECTOR pos44; /* caster bone position */
    SVECTOR pos4C;
    SVECTOR rot54;
    SVECTOR rot5C;
    SVECTOR pos64;
    SVECTOR pos6C;
    SVECTOR rot74;
    SVECTOR rot7C;
} Fx127Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    s32 unk8; /* unused */
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target body position */
    SVECTOR rot18;
    SVECTOR rot20;
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
} Fx127Work81;

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*motionFn8)(VECTOR *, s32, SVECTOR *, s32, s32, s32); /* particle motion callback, installed by the module */
} Fx127EffHdr;

extern SVECTOR fx_127_aimRot;
extern SVECTOR fx_127_aimPoint;
extern SVECTOR fx_127_emitPos0;
extern SVECTOR fx_127_emitPos1;
extern SVECTOR fx_127_emitPos2;
extern SVECTOR fx_127_emitPos3;
extern s32 fx_127_op80Trails0[];
extern s32 fx_127_op80Trails1[];
extern Unk800F78A4 *fx_127_env;
extern SVECTOR fx_127_lerpFrom; /* path start */
extern SVECTOR fx_127_lerpTo; /* path end */
extern SVECTOR fx_127_emitterPos;
extern Pool6CEntry *fx_127_emitter;

static s32 fx_127_op80ParticleMotion0(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_127_op80ParticleMotion1(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_lerpParticle(VECTOR *out, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 len, s32 frame, s32 n);

#endif
