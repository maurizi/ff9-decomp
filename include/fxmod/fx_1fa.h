#ifndef FX_1FA_H
#define FX_1FA_H

/* Effect module fx_1fa (dir 13 file 0x1fa): Carbuncle (2) (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1fa effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CARBUNCLE_YELLOW_RED_RINGS 0x2 /* emitter: white-yellow and white-red gouraud rings at each target (byte-identical to fx_1ee entry 0x2) */
#define FX_CARBUNCLE_VIOLET_RINGS     0x13 /* emitter: white / pale blue / violet gouraud rings, 24 frames (byte-identical to fx_1ee entry 0x13) */
#define FX_CARBUNCLE_GREEN_RING       0x14 /* emitter: white-green gouraud ring (byte-identical to fx_1ee entry 0x14) */
#define FX_CARBUNCLE_CLOCK_FACE       0x17 /* emitter: clock face with Roman numerals and a blue ring glow (byte-identical to fx_1ee entry 0x17) */
#define FX_CARBUNCLE_ORBIT_GLOW_2     0x1E /* emitter: orbiting particle + sprite, white speckled arcs (same call site as fx_0b1 entry 0x16, same kind) */

typedef struct {
    s32 delay0; /* delay */
    s32 timer4; /* countdown */
    u8 *sprite8; /* effect data */
    s32 angleC; /* angle */
    s32 radius10; /* radius */
} Fx1faRing; /* 0x14 */

extern Unk800F78A4 *fx_1fa_env;
extern SVECTOR fx_1fa_emitPos0;
extern s32 fx_1fa_timer[]; /* countdown; an array (in-struct for alias analysis) */
extern SVECTOR fx_1fa_emitPos1;
extern SVECTOR fx_1fa_emitPos2[];
extern Fx1faRing fx_1fa_spiralSprites[12][4];

#endif
