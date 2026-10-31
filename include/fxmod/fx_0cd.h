#ifndef FX_0CD_H
#define FX_0CD_H

/* Effect module fx_0cd (dir 13 file 0xCD): enemy Aera (Gimme Cat, ...). Three growing, spinning models (Fx0cdModel)
 * and particle emitters at the caster. Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR posC; /* position */
    VECTOR scale1C; /* scale */
} Fx0cdModel; /* 0x2C */

typedef struct {
    s32 timer0; /* model C countdown */
    s32 timer4; /* model A countdown */
    s32 timer8; /* model B countdown */
    s32 unkC[5];
    Fx0cdModel model20; /* A */
    Fx0cdModel model4C; /* B */
    Fx0cdModel model78; /* C */
    SVECTOR posA4; /* caster position */
    SVECTOR posAC; /* caster body position */
} Fx0cdWork;

extern Unk800F78A4 *fx_0cd_env;
extern BtlModel fx_0cd_effModels[];
extern s32 fx_0cd_trails[];
extern s32 fx_0cd_trails2[];

#endif
