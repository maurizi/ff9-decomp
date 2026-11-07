#ifndef FX_058_H
#define FX_058_H

/* Effect module fx_058 (dir 13 file 0x58). */
#include "btlfx_mod.h"

/* fx_058 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MATRA_MAGIC_MISSILE_TRAIL 0x2 /* trail: blue streak behind each homing missile */
#define FX_MATRA_MAGIC_BLAST         0x8 /* particles: violet-white radial blast */
#define FX_MATRA_MAGIC_SMOKE         0x9 /* particles: pink-violet smoke puff */
#define FX_MATRA_MAGIC_STAR_GLOW     0xA /* 'so' model: crossed quads with a violet-white glow texture (a star-shaped flare), registered in op80 */

/* fx_058 choreography (docs/fxdata.md, "Choreography literals"): the missile pool, the fly / blast states, homing limits and hit tint. */
#define FX_MATRA_MAGIC_MISSILES      0x20  /* homing missiles (op80 poolCount), one spawned per frame, each with its fx_058_trailBufs entry */
#define FX_MATRA_MAGIC_MODEL_COUNT   1     /* btlfx_initEffModels slots: STAR_GLOW */
#define FX_MATRA_MAGIC_STATE_FLY     0     /* missile: accelerates and turns toward the target, drawn as STAR_GLOW with its trail, */
#define FX_MATRA_MAGIC_FLY_FRAMES    0x11  /* until it comes within HIT_RADIUS of the target or this many frames pass */
#define FX_MATRA_MAGIC_HIT_RADIUS    0x200
#define FX_MATRA_MAGIC_MAX_SPEED     0x300 /* the missiles' speed cap (they gain 0x20 per frame) */
#define FX_MATRA_MAGIC_HIT_TINT_RGB  0x20, 0x10, 0 /* additive screen tint (orange) as a missile hits, on odd frames */
#define FX_MATRA_MAGIC_STATE_BLAST   1     /* missile: smoke rises and the blast flares at its point, */
#define FX_MATRA_MAGIC_BLAST_FRAMES  0x10  /* FX_MATRA_MAGIC_BLAST is drawn this many frames, */
#define FX_MATRA_MAGIC_SMOKE_FRAMES  0x18  /* the smoke (and the item) this many */

/* homing particle, one pool item */
typedef struct {
    s32 state0; /* state */
    s32 speed4; /* speed */
    s32 turn8; /* turn rate */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* direction */
    u8 *trail1C; /* trail buffer (fx_058_trailBufs) */
    SVECTOR target20; /* target; pad = model z scale */
} Fx058Item; /* 0x28 */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body position */
    SVECTOR posC; /* caster body position */
    SVECTOR rot14; /* caster rotation */
    s32 count1C; /* particles spawned */
    s32 frame20; /* frame */
    s32 model24; /* model */
} Fx058Work; /* 0x28 */

extern Unk800F78A4 *fx_058_env;
extern BtlModelBuf fx_058_effModels[];
extern u8 fx_058_trailBufs[][0x54]; /* per missile */

static s32 fx_058_op80PoolTask(s32 mode, Fx058Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx058Work *work);

#endif
