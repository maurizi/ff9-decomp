#ifndef FX_0AA_H
#define FX_0AA_H

/* Effect module fx_0aa (dir 13 file 0xAA): enemy Snowstorm (Ash). Emitters, two with particles on straight paths
 * (fxm_lerpParticle). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0aa effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SNOWSTORM_SNOW_BURST    0x1 /* emitter: snow-textured ring + snowflake sprites */
#define FX_SNOWSTORM_SNOW_BURST_2  0x2 /* emitter: snow-textured ring + snowflake sprites */
#define FX_SNOWSTORM_SNOW_STREAM   0x3 /* emitter: snow streak trail + snowflakes */
#define FX_SNOWSTORM_SNOW_STREAM_2 0x4 /* emitter: snow streak trail + snowflakes */
#define FX_SNOWSTORM_ICE_FLAKES    0x5 /* emitter: icy cyan flakes */
#define FX_SNOWSTORM_MIST          0x6 /* emitter: grey-teal mist wisps */
#define FX_SNOWSTORM_FROST_RINGS   0x7 /* emitter: teal gouraud rings + snow-textured rings */
#define FX_SNOWSTORM_FLAKES        0x8 /* emitter: snowflake sprites */
#define FX_SNOWSTORM_SNOW_TRAIL    0x9 /* emitter: snow streak trail spiralling */

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
