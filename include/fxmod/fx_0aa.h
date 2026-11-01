#ifndef FX_0AA_H
#define FX_0AA_H

/* Effect module fx_0aa (dir 13 file 0xAA): enemy Snowstorm (Ash). Emitters, two with particles on straight paths
 * (fxm_lerpParticle). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* btlfx_getUnitBonePos(0x10, 0x12) */
    SVECTOR pos18; /* btlfx_getUnitBonePos(0x10, 0x2B) */
} Fx0aaWork80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* position: env->targets->centerX..Z */
} Fx0aaWork81;

extern Unk800F78A4 *fx_0aa_env;
extern s32 fx_0aa_trails[];
extern s32 fx_0aa_trails2[];
extern s32 fx_0aa_trails3[];
extern SVECTOR fx_0aa_pathFrom0; /* particle paths: starts */
extern SVECTOR fx_0aa_pathFrom1;
extern SVECTOR fx_0aa_pathTo0; /* ends */
extern SVECTOR fx_0aa_pathTo1;
extern SVECTOR fx_0aa_emitStartPos0;
extern SVECTOR fx_0aa_emitStartPos1;

static s32 fx_0aa_op80ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_0aa_op80ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_lerpParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 n);

#endif
