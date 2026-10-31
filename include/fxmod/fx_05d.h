#ifndef FX_05D_H
#define FX_05D_H

/* Effect module fx_05d (dir 13 file 0x5D). */
#include "btlfx_mod.h"

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
