#ifndef FX_041_H
#define FX_041_H

/* Effect module fx_041 (dir 13 file 0x41). */
#include "btlfx_mod.h"

/* fx_041 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WHIRL_SLASH_WIND_RINGS  0x1 /* emitter: white-cyan-green and blue-white gouraud rings, teal glints, sparks */
#define FX_WHIRL_SLASH_DUST_CLOUD  0x2 /* emitter: brown dust cloud + green glints, sparks */
#define FX_WHIRL_SLASH_DUST_PUFF   0x3 /* emitter: beige dust puff + sparks */
#define FX_WHIRL_SLASH_SLASH_TRAIL 0x4 /* emitter: trail with a blue-white slash streak */
#define FX_WHIRL_SLASH_CYAN_RINGS  0x5 /* emitter: white-cyan-teal gouraud rings */
#define FX_WHIRL_SLASH_SMOKE       0x6 /* emitter: grey smoke puffs */
#define FX_WHIRL_SLASH_DUST_PUFF_2 0x7 /* emitter: beige dust puff + sparks */
#define FX_WHIRL_SLASH_PATH        0x8 /* point list: the slash path (fxm_posPathInit) */
#define FX_WHIRL_SLASH_GLINTS      0xF /* emitter: fan + yellow glints, sparks */
#define FX_WHIRL_SLASH_GOLD_RINGS  0x10 /* emitter: white-yellow-brown and white-red gouraud rings */

typedef struct {
    s16 x, y, z;
} Fx041Point;

/* point list read from the effect file */
typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx041Point *points4;
} Fx041Path;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* target body position */
    SVECTOR pos20; /* target position */
    SVECTOR pos28; /* caster position */
} Fx041Work80; /* 0x30 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target position */
    SVECTOR pos14;
} Fx041Work81; /* 0x1C */

extern Unk800F78A4 *fx_041_env;
extern Fx041Path fx_041_path;
extern SVECTOR fx_041_targetPos; /* target position */
extern SVECTOR fx_041_lastPathPos; /* last path point */
extern SVECTOR fx_041_emitPos;
extern s32 fx_041_trails[];

static s32 fx_041_op80ParticleMotion(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static void fxm_posPathInit(Fx041Path *path, u16 *data);
static s32 fxm_posPathGet(Fx041Path *path, s32 t, SVECTOR *out);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHitMotionIfActive(s32 unit);
static void fxm_startHeavyHitMotionIfActive(s32 unit);

#endif
