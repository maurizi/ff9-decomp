#ifndef FX_1AD_H
#define FX_1AD_H

/* Effect module fx_1ad (dir 13 file 0x1ad). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* done */
    s32 unk4; /* frame counter */
    SVECTOR pos8; /* bone position */
    SVECTOR pos10; /* target body position */
    u8 pad18[0x20 - 0x18];
    Pool6CEntry *emit20;
    s32 model24; /* model */
} Fx1adWork;

extern u16 D_801E7CD8[6][9]; /* per row: count, bones */
extern Unk800F78A4 *fx_1ad_env;
extern BtlModel fx_1ad_effModels[];
extern s32 D_801E7E6C[];
extern s32 D_801E8F4C[];

static s32 fx_1ad_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);

#endif
