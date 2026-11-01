#ifndef FX_1AD_H
#define FX_1AD_H

/* Effect module fx_1ad (dir 13 file 0x1ad): enemy 6 Legs (Gigan Octopus). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 done0; /* done */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* bone position */
    SVECTOR pos10; /* target body position */
    u8 pad18[0x20 - 0x18];
    Pool6CEntry *emit20;
    s32 model24; /* model */
} Fx1adWork;

extern u16 fx_1ad_boneChains[6][9]; /* per row: count, bones */
extern Unk800F78A4 *fx_1ad_env;
extern BtlModelBuf fx_1ad_effModels[];
extern s32 fx_1ad_op80Trails0[];
extern s32 fx_1ad_op80Trails1[];

static s32 fx_1ad_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);

#endif
