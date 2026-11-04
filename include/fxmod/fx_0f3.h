#ifndef FX_0F3_H
#define FX_0F3_H

/* Effect module fx_0f3 (dir 13 file 0x0F3): Scoop Art. A moving point drawn as a strip (btlfx_drawTrail) with an
 * emitter and screen tints, three path variants. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SCOOP_ART_BLUE_RINGS   0x2 /* emitter: white-blue gouraud rings + rings textured with blue swirl marks */
#define FX_SCOOP_ART_PINK_TRAIL   0x3 /* trail: red-pink glowing strip drawn along the moving point */
#define FX_SCOOP_ART_SCREEN_TRAIL 0x8 /* emitter: screen trail + sprite, texture on a placeholder page (the render reads garbled tan texels) (look pending) */

typedef struct {
    s32 state0; /* state */
    s32 variant4; /* variant (0..2) */
    SVECTOR pos8; /* target body position */
    SVECTOR headPos10;
    SVECTOR pos18;
    s32 unk20; /* unused (runtime watch, round 33) */
    Pool6CEntry *emit24;
} Fx0f3Work;

extern Unk800F78A4 *fx_0f3_env;
extern u8 fx_0f3_strips[][0xEC];
extern u8 fx_0f3_op80Trails[][0x2B0];

#endif
