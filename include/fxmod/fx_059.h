#ifndef FX_059_H
#define FX_059_H

/* Effect module fx_059 (dir 13 file 0x59: Haste, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_059 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_HASTE_YELLOW_RED_RINGS 0x2 /* emitter: white-yellow and white-red gouraud rings */
#define FX_HASTE_VIOLET_RINGS     0x13 /* emitter: white-cyan-violet gouraud rings */
#define FX_HASTE_ORANGE_RINGS     0x14 /* emitter: white-grey-violet-orange gouraud rings */
#define FX_HASTE_CLOCK_FACE       0x18 /* emitter: clock face with Roman numerals + blue glow */

/* fx_059 choreography (docs/fxdata.md, "Choreography literals"): op80's spiral (states, radius, trail pool); op81's and op82's lengths. */
#define FX_HASTE_STATE_SPIRAL   0    /* op80: the head particle spirals into the screen, dropping a trail point each frame, */
#define FX_HASTE_SPIRAL_FRAMES  6    /* for this many frames, */
#define FX_HASTE_SPIRAL_RADIUS  0xA0 /* its radius shrinking from this to 0 */
#define FX_HASTE_STATE_END      1    /* op80: the trail fades, the op ends after END_FRAMES */
#define FX_HASTE_END_FRAMES     7
#define FX_HASTE_TRAIL_POINTS   12   /* the op's own pool of trail points (btlfx_poolInit count, SVECTOR items) */
#define FX_HASTE_TRAIL_FRAMES   8    /* a trail point is drawn (fading) for this many frames */
#define FX_HASTE_RINGS_FRAMES   0x20 /* op81: the task ends at this frame */
#define FX_HASTE_VIOLET_FRAMES  0x18 /* op82: the task ends at this frame */

typedef struct {
    s32 state0; /* state */
    s32 depthZ4; /* spiral head's distance into the screen in the view-rotation frame, grows 0xC8 per frame */
    s32 angle8; /* angle */
    s32 kindC; /* index into fx_059_headBlocks / fx_059_tailBlocks */
    s32 angleStep10; /* angle step */
    Unk800DA1B8 *pool14; /* particle pool */
    s32 viewRot18;
} Fx059Work80;

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4;
} Fx059Work81;

typedef struct {
    s32 rotXY0; /* rotation x and y as one word, both 0 (btlfx_pushViewRotation reads it as an SVECTOR) */
    s16 rotZ4;
    s16 unk6; /* SVECTOR pad, unused */
} Fx059Rot;

extern s16 fx_059_headBlocks[];
extern s16 fx_059_tailBlocks[];
extern Unk800F78A4 *fx_059_env;

static s32 fx_059_op80PoolTask(s32 mode, SVECTOR *pos, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx059Work80 *work);

#endif
