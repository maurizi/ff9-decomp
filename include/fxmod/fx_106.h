#ifndef FX_106_H
#define FX_106_H

/* Effect module fx_106 (dir 13 file 0x106): enemy Mucus (Anemone). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* caster rotation */
    SVECTOR pos10; /* target body position */
    SVECTOR pos18;
    SVECTOR pos20;
} Fx106Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target position */
    SVECTOR pos14;
} Fx106Work81;

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*motion8)(VECTOR *, s32, SVECTOR *, s32, s32, s32);
} Fx106EffHdr;

extern Unk800F78A4 *fx_106_env;
extern s32 fx_106_op80Trails[];
extern s32 fx_106_op81Trails0[];
extern s32 fx_106_op81Trails1[];
extern SVECTOR fx_106_pathStart; /* path start */
extern SVECTOR fx_106_pathEnd; /* path end */
extern SVECTOR fx_106_emitPos;

static s32 fx_106_op80ParticleMotion(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
