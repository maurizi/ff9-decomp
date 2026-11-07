#ifndef FX_1F9_H
#define FX_1F9_H

/* Effect module fx_1f9 (dir 13 file 0x1f9): Carbuncle (3) (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CARBUNCLE_BLUE_RINGS   0x1 /* emitter: pale blue gouraud rings at each target (op 0x81) (byte-identical to fx_1ed entry 0x1) */
#define FX_CARBUNCLE_ORBIT_GLOW_2 0x3 /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) */

/* fx_1f9 choreography (docs/fxdata.md, "Choreography literals"): the short form's op80 (one orbit glow) and
 * op81's blue rings, which is fx_1ed's op81 (same names, own values where they differ). */
#define FX_CARBUNCLE_GLOW_FRAMES    0x40 /* op80: ORBIT_GLOW_2 plays in front of the camera; the task ends at this frame */
#define FX_CARBUNCLE_RINGS_FRAMES      0x30 /* op81: the task (BLUE_RINGS at each target) ends after this many frames */

extern Unk800F78A4 *fx_1f9_env;
extern SVECTOR fx_1f9_emitPos0;
extern SVECTOR fx_1f9_emitPos1[];

#endif
