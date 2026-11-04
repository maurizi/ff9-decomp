#ifndef FX_188_H
#define FX_188_H

/* Effect module fx_188 (dir 13 file 0x188): LV3 Def-less. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_188 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LV3_DEF_LESS_PARTICLE       0x3 /* particles: one frame, renders black (drawn per pool item); runtime (docs/runtime-findings.md effect-data pass): still black with the run-time page (look pending) */
#define FX_LV3_DEF_LESS_VIOLET_ORBS    0x4 /* emitter: violet glowing orb sprites, 12 frames (per pool item) */
#define FX_LV3_DEF_LESS_VIOLET_ORB     0xE /* particles: violet glowing orb (per pool item) */
#define FX_LV3_DEF_LESS_BACKDROP       0xF /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_LV3_DEF_LESS_SMOKE_RINGS    0x10 /* emitter: rings textured with orange smoke */
#define FX_LV3_DEF_LESS_SMOKE_RINGS_2  0x11 /* emitter: rings textured with orange smoke */
#define FX_LV3_DEF_LESS_BLUE_RINGS     0x12 /* emitter: white-blue and white-green gouraud rings */
#define FX_LV3_DEF_LESS_ORANGE_TRAILS  0x13 /* emitter: orbiting 3D trails, orange glowing streak */
#define FX_LV3_DEF_LESS_RING           0x14 /* rings: one textured ring (texture not resolved); runtime (docs/runtime-findings.md effect-data pass): noise with the run-time page (look pending) */
#define FX_LV3_DEF_LESS_CRYSTAL_SHARDS 0x15 /* emitter: orbiting pink / blue / orange crystal shards, 16 frames */

typedef struct {
    s32 state0; /* state */
    s32 speed4; /* speed */
    s32 turn8; /* turn rate */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* direction */
    Pool6CEntry *emit1C;
    SVECTOR target20; /* target */
} Fx188Elem; /* 0x28 */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* target */
    SVECTOR posC; /* caster body position */
    SVECTOR rot14; /* caster rotation */
    s32 count1C; /* spawned */
} Fx188Work; /* 0x20 */

extern Unk800F78A4 *fx_188_env;
extern s32 fx_188_op80Trails[];
extern s32 fx_188_particleScale; /* scale */

static s32 fx_188_op80PoolTask(s32 mode, Fx188Elem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
