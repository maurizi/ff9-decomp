#ifndef FX_187_H
#define FX_187_H

/* Effect module fx_187 (dir 13 file 0x187): enemy Fire Blades (Soulcage). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_187 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FIRE_BLADES_FLAME_BURST   0x1 /* emitter: flame burst + sprites (texture not in the file) */
#define FX_FIRE_BLADES_FLAME_BURST_2 0x2 /* emitter: flame burst + sprites (texture not in the file) */
#define FX_FIRE_BLADES_FIRE_BLADE    0x3 /* emitter: spiralling yellow blade screen trail with red flame particles */
#define FX_FIRE_BLADES_FIRE_BLADE_2  0x4 /* emitter: spiralling yellow blade screen trail with red flame particles */
#define FX_FIRE_BLADES_SPARKS        0x5 /* emitter: red dot sparks */
#define FX_FIRE_BLADES_SPARKS_2      0x6 /* emitter: red dot sparks */
#define FX_FIRE_BLADES_FIRE_BLADE_3  0x7 /* emitter: helix of yellow blade screen trail with red flame particles */
#define FX_FIRE_BLADES_FIRE_BLADE_4  0x8 /* emitter: helix of yellow blade screen trail with red flame particles */
#define FX_FIRE_BLADES_SPARKS_3      0x9 /* emitter: helix of red dot sparks */
#define FX_FIRE_BLADES_SPARKS_4      0xA /* emitter: helix of red dot sparks */
#define FX_FIRE_BLADES_FLAME_BURST_3 0xB /* emitter: orbiting flame burst + sprites (texture not in the file) */
#define FX_FIRE_BLADES_RED_RING      0xC /* emitter: dark red gouraud rings */
#define FX_FIRE_BLADES_FLAME_TRAIL   0x13 /* emitter: spiralling 3D trail with an orange flame texture */

/* fx_187 choreography (docs/fxdata.md, "Choreography literals"): op80's states, keyframes and lengths, and the
 * trail buffers. */
#define FX_FIRE_BLADES_STATE_IGNITE 0    /* work->state0: FLAME_BURSTs at the caster's two hand bones (fx_187_bones), */
#define FX_FIRE_BLADES_IGNITE_FRAMES 0x10 /* for this many frames; then the FIRE_BLADEs and SPARKS start there */
#define FX_FIRE_BLADES_STATE_SWING  1    /* the blades (FIRE_BLADE_3 / _4, SPARKS_3 / _4) swing out to both sides of the targets */
#define FX_FIRE_BLADES_SWING_EASE_FRAMES 0x10 /* over this many frames (cosine ease) */
#define FX_FIRE_BLADES_BURST_FRAME  0xE  /* STATE_SWING: FLAME_BURST_3, RED_RING and FLAME_TRAIL at the targets */
#define FX_FIRE_BLADES_SWING_FRAMES 0x37 /* STATE_SWING's length (the op ends) */
#define FX_FIRE_BLADES_FIRE_BLADE_TRAIL_SLOTS    0x12 /* FIRE_BLADE / _2's trail buffers (fx_187_op80Trails0 / 1): slots of */
#define FX_FIRE_BLADES_FIRE_BLADE_TRAIL_STRIDE   0x14 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_FIRE_BLADES_FIRE_BLADE_3_TRAIL_SLOTS  0x20 /* FIRE_BLADE_3 / _4's (fx_187_op80Trails2 / 3) */
#define FX_FIRE_BLADES_FIRE_BLADE_3_TRAIL_STRIDE 0x14
#define FX_FIRE_BLADES_FLAME_TRAIL_TRAIL_SLOTS   7    /* FLAME_TRAIL's (fx_187_op80Trails4) */
#define FX_FIRE_BLADES_FLAME_TRAIL_TRAIL_STRIDE  0x6C

typedef struct {
    s32 state0; /* state */
    Pool6CEntry *emit4[2];
    Pool6CEntry *emitC[2];
    SVECTOR pos14; /* target position */
    SVECTOR pos1C;
    SVECTOR pos24; /* bone positions */
    SVECTOR pos2C;
    SVECTOR pos34;
    SVECTOR pos3C;
    SVECTOR pos44;
    SVECTOR pos4C;
    SVECTOR pos54;
    SVECTOR pos5C;
} Fx187Work; /* 0x64 */

extern u16 fx_187_bones[2]; /* bones */
extern Unk800F78A4 *fx_187_env;
extern s32 fx_187_op80Trails0[];
extern s32 fx_187_op80Trails1[];
extern s32 fx_187_op80Trails2[];
extern s32 fx_187_op80Trails3[];
extern s32 fx_187_op80Trails4[];

#endif
