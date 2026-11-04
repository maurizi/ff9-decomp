#ifndef FX_05D_H
#define FX_05D_H

/* Effect module fx_05d (dir 13 file 0x5D). */
#include "btlfx_mod.h"

/* fx_05d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BIO_PATH          0x1 /* track: rotation / position keys (fxm_rotPosTrackInit) */
#define FX_BIO_SCALE_TRACK   0x2 /* track: vector keys (fxm_vecTrackInit), paired with FX_BIO_PATH */
#define FX_BIO_SCALE_TRACK_2 0xF /* track: vector keys (fxm_vecTrackInit), paired with FX_BIO_PATH_2 */
#define FX_BIO_PATH_2        0x10 /* track: rotation / position keys (fxm_rotPosTrackInit) */
#define FX_BIO_BUBBLE_TRAIL  0x11 /* emitter: screen trail of glowing green bubbles streaming around the spikes; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_BIO_TEAL_RING     0x12 /* emitter: ring with a teal glow texture */
#define FX_BIO_TOXIC_CLOUD   0x13 /* emitter: green toxic cloud + grey glow */
#define FX_BIO_RED_ORB       0x14 /* emitter: red-veined orbs + crimson glow */

/* keyframe tracks read from the effect file: count, then the keys */
typedef struct {
    s16 px0, py2, pz4; /* position */
    s16 rx6, ry8, rzA; /* rotation */
} Fx05dKey12;

typedef struct {
    s16 x, y, z;
} Fx05dKey6;

typedef struct {
    u16 count0; /* count */
    Fx05dKey12 *keys4;
} Fx05dTrack12;

typedef struct {
    u16 count0; /* count */
    Fx05dKey6 *keys4;
} Fx05dTrack6;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target position */
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    s32 unk40; /* unused (runtime watch, round 33) */
    SVECTOR pos44; /* model position, for the trail */
    SVECTOR pos4C;
    s32 morphFrame54; /* frames into the current morph */
    s32 unk58; /* unused (runtime watch, round 33) */
    u16 model5C[6]; /* morph models */
} Fx05dWork; /* 0x68 */

extern u8 fx_05d_morphBlocks[6];
extern Unk800F78A4 *fx_05d_env;
extern BtlModelBuf fx_05d_effModels[];
extern s32 fx_05d_trails[];
extern SVECTOR fx_05d_emitPos;
extern Fx05dTrack12 fx_05d_rotPosKeys;
extern Fx05dTrack12 fx_05d_rotPosKeys2;
extern Fx05dTrack6 fx_05d_scaleKeys;
extern Fx05dTrack6 fx_05d_scaleKeys2;

static void fxm_rotPosTrackInit(Fx05dTrack12 *track, u16 *data);
static s32 fxm_rotPosTrackGet(Fx05dTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);
static void fxm_vecTrackInit(Fx05dTrack6 *track, u16 *data);
static s32 fxm_vecTrackGet(Fx05dTrack6 *track, s32 t, VECTOR *out);

#endif
