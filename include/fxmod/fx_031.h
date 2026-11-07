#ifndef FX_031_H
#define FX_031_H

/* Effect module fx_031 (dir 13 file 0x31). */
#include "btlfx_mod.h"

/* fx_031 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_GRADUAL_PETRIFY_GOLD_RINGS   0x1 /* emitter: white-cyan and white-gold gouraud rings */
#define FX_GRADUAL_PETRIFY_GOLD_RINGS_2 0x2 /* emitter: white-gold gouraud rings */
#define FX_GRADUAL_PETRIFY_STONE_DUST   0x3 /* emitter: grey dust clouds + grey streaks */
#define FX_GRADUAL_PETRIFY_STONE_DUST_2 0x9 /* emitter: grey dust clouds */

/* fx_031 choreography (docs/fxdata.md, "Choreography literals"): op80's flash, keyframe and length. */
#define FX_GRADUAL_PETRIFY_FLASH_FRAMES 8    /* op80: additive white flash fading over this many frames; then the stone dust and GOLD_RINGS_2 start */
#define FX_GRADUAL_PETRIFY_FRAMES       0x28 /* op80: the task ends at this frame */
#define FX_GRADUAL_PETRIFY_DUST_2_DROP  0x80 /* FX_GRADUAL_PETRIFY_STONE_DUST_2's position (pos8): this far below the target body position */

typedef struct {
    SVECTOR pos0; /* target body position */
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR pos20;
    SVECTOR pos28;
    Pool6CEntry *emit30;
    Pool6CEntry *emit34;
} Fx031Work; /* 0x38 */

extern Unk800F78A4 *fx_031_env;

#endif
