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

/* fx_0aa choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_SNOWSTORM_STREAM_FRAME 4    /* op80: SNOW_BURST, _2 and the snow streams from the unit's two bones start */
#define FX_SNOWSTORM_BLOW_FRAMES  0x1E /* op80's length */
#define FX_SNOWSTORM_MIST_FRAME   4    /* op81: FX_SNOWSTORM_MIST and FX_SNOWSTORM_SNOW_TRAIL start (FROST_RINGS at 0) */
#define FX_SNOWSTORM_FLAKES_FRAME 6    /* op81: FX_SNOWSTORM_ICE_FLAKES and FX_SNOWSTORM_FLAKES start */
#define FX_SNOWSTORM_FRAMES       0x28 /* op81's length (the op ends) */
#define FX_SNOWSTORM_SNOW_STREAM_TRAIL_SLOTS  0xC   /* SNOW_STREAM and _2's trail buffers fx_0aa_trails, _trails2: slots of */
#define FX_SNOWSTORM_SNOW_STREAM_TRAIL_STRIDE 0x70  /* this many bytes */
#define FX_SNOWSTORM_SNOW_TRAIL_TRAIL_SLOTS   8     /* FX_SNOWSTORM_SNOW_TRAIL's trail buffer fx_0aa_trails3: slots of */
#define FX_SNOWSTORM_SNOW_TRAIL_TRAIL_STRIDE  0x110 /* this many bytes */

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
