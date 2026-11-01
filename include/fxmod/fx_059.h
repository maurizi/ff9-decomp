#ifndef FX_059_H
#define FX_059_H

/* Effect module fx_059 (dir 13 file 0x59: Haste, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

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
