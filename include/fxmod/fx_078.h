#ifndef FX_078_H
#define FX_078_H

/* Effect module fx_078 (dir 13 file 0x78): Bio, all targets (party; enemy Bio of Meltigemini). Ten texture models
 * drawn in turn along a rotation+position track (fxm_rotPosTrackGet), plus emitters. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_078 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BIO_GREEN_RING    0x2 /* emitter: rings textured with teal-green swirls */
#define FX_BIO_VEC_TRACK     0xC /* table: vector track (fxm_vecTrackInit) */
#define FX_BIO_ROT_POS_TRACK 0xD /* table: rotation + position track (fxm_rotPosTrackInit) */
#define FX_BIO_RED_BUBBLES   0x14 /* emitter: red veined bubbles with a crimson glow */
#define FX_BIO_POS_TRACK     0x15 /* table: position track (fxm_posTrackInit) */
#define FX_BIO_BUBBLE_TRAIL  0x16 /* emitter: screen trail of glowing green bubbles around the burst; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_BIO_GREEN_CLOUD   0x17 /* emitter: murky green cloud + grey glow */

/* keyframe tracks read from the effect file: count, then the keys (as in fx_05d) */
typedef struct {
    s16 posX0, posY2, posZ4; /* position */
    s16 rotX6, rotY8, rotZA; /* rotation */
} Fx078Key12;

typedef struct {
    s16 x, y, z;
} Fx078Key6;

typedef struct {
    u16 count0; /* count */
    Fx078Key12 *keys4;
} Fx078Track12;

typedef struct {
    u16 count0; /* count */
    Fx078Key6 *keys4;
} Fx078Track6;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target position */
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    s32 unk40; /* unused */
    SVECTOR emitPos44; /* trail position */
    u16 model4C[10]; /* models */
} Fx078Work; /* 0x60 */

extern u8 fx_078_modelBlocks[10];
extern Unk800F78A4 *fx_078_env;
extern BtlModelBuf fx_078_effModels[];
extern s32 fx_078_trails[];
extern Fx078Track12 fx_078_rotPosKeys;
extern Fx078Track6 fx_078_scaleKeys;
extern Fx078Track6 fx_078_emitPosKeys;
extern SVECTOR fx_078_emitStartPos;

static void fxm_rotPosTrackInit(Fx078Track12 *track, u16 *data);
static s32 fxm_rotPosTrackGet(Fx078Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);
static void fxm_posTrackInit(Fx078Track6 *track, u16 *data);
static s32 fxm_posTrackGet(Fx078Track6 *track, s32 t, SVECTOR *out);
static void fxm_vecTrackInit(Fx078Track6 *track, u16 *data);
static s32 fxm_vecTrackGet(Fx078Track6 *track, s32 t, VECTOR *out);

#endif
