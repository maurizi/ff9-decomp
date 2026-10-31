#ifndef FX_084_H
#define FX_084_H

/* Effect module fx_084 (dir 13 file 0x84): Drain (party and enemy). Particles on an arc between the two units
 * (fx_084_op80ParticleMotion). Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster body position */
    SVECTOR posC; /* unit 0x10 body position */
    SVECTOR aim14; /* angles from posC to pos4 */
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
} Fx084Work;

extern Unk800F78A4 *fx_084_env;
extern SVECTOR fx_084_pathFrom; /* particle path start */
extern SVECTOR fx_084_pathTo; /* particle path end */
extern Unk80100C78Entry *fx_084_task;

static s32 fx_084_op80ParticleMotion(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame);

#endif
