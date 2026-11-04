#ifndef FX_07E_H
#define FX_07E_H

/* Effect module fx_07e (dir 13 file 0x7E): Slow. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_07e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SLOW_GLINT      0x1 /* particles: white glint, the rest of the frame still outside the file and the common pages (look pending) */
#define FX_SLOW_GREEN_RING 0x2 /* rings: white-green gouraud rings */
#define FX_SLOW_BLUE_WAVE  0x3 /* particles: blue chevron wave frames */
#define FX_SLOW_PARTICLES  0x5 /* particles: 8 frames, still render all black (look pending) */
#define FX_SLOW_BACKDROP   0xA /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_SLOW_CRESCENT   0xC /* model: one quad textured with a blue-white crescent */

typedef struct {
    s32 state0; /* state */
    s32 spawnCount4; /* spawned */
    s32 frame8; /* frame */
    SVECTOR posC; /* caster body position */
    SVECTOR ringRot14; /* ring rotation */
    s32 radius1C; /* radius */
    s32 scale20; /* scale */
    s32 shrinkDone24; /* shrink done */
    s32 model28; /* model */
} Fx07eWork80; /* 0x2C */

typedef struct {
    s32 angle0; /* angle offset */
    s32 state4; /* state */
    s32 index8; /* index */
} Fx07ePart; /* 0xC */

extern Unk800F78A4 *fx_07e_env;
extern BtlModelBuf fx_07e_effModels[];
extern s32 fx_07e_twist;

static s32 fx_07e_op80PoolTask(s32 mode, Fx07ePart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool,
                  Fx07eWork80 *work);
static void fx_07e_op81GridVertex(s32 radius, s32 unused1, s32 ang, s32 unused3, s16 *out);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 otZ8; /* blend */
    SVECTOR posC; /* caster body position */
} Fx07eWork81; /* 0x14 */

s32 fx_07e_op81(s32 mode, Fx07eWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
