#ifndef FX_094_H
#define FX_094_H

/* Effect module fx_094 (dir 13 file 0x94): enemy Delta Attack (Mover). Emitters only. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_094 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DELTA_ATTACK_WHITE_RING   0x1 /* emitter: white gouraud rings */
#define FX_DELTA_ATTACK_VIOLET_RING  0x2 /* emitter: homing violet gouraud rings */
#define FX_DELTA_ATTACK_WHITE_RING_2 0x3 /* emitter: white gouraud rings */
#define FX_DELTA_ATTACK_COLOR_RINGS  0x4 /* emitter: yellow / blue / red / gold gouraud rings */
#define FX_DELTA_ATTACK_RAINBOW_STREAK 0x5 /* emitter: fan drawn as a thin rainbow-coloured horizontal streak */

/* fx_094 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length. */
#define FX_DELTA_ATTACK_WHITE_RING_2_FRAME   0x10 /* FX_DELTA_ATTACK_WHITE_RING_2 starts */
#define FX_DELTA_ATTACK_VIOLET_RING_FRAME    0x18 /* FX_DELTA_ATTACK_VIOLET_RING starts */
#define FX_DELTA_ATTACK_RAINBOW_STREAK_FRAME 0x30 /* FX_DELTA_ATTACK_RAINBOW_STREAK starts */
#define FX_DELTA_ATTACK_WHITE_RING_FRAME     0x40 /* FX_DELTA_ATTACK_WHITE_RING starts */
#define FX_DELTA_ATTACK_FRAMES               0x68 /* op80's length (the op ends) */

typedef struct {
    SVECTOR pos0; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* caster position */
    SVECTOR aim18; /* angles from pos0 to pos8 */
    Pool6CEntry *emit20;
} Fx094Work;

extern Unk800F78A4 *fx_094_env;

#endif
