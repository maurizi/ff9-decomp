#ifndef FX_0CE_H
#define FX_0CE_H

/* Effect module fx_0ce (dir 13 file 0xCE): enemy Screech, Shriek (Gimme Cat, ...). Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ce effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SCREECH_TRAIL       0x1 /* emitter: 3D trail; the render still reads garbled pink stripes (texture not resolved); runtime (docs/runtime-findings.md effect-data pass 2): ablating it changes only scattered pink squiggle pixels (max 1234 px) (look pending) */
#define FX_SCREECH_PURPLE_MIST 0x2 /* emitter: dark purple mist rings and brown-pink gouraud rings */
#define FX_SCREECH_BLADE_RING  0x7 /* 'so' model: ring of pale green curved blades */
#define FX_SCREECH_SOUND_RINGS 0xB /* emitter: textured rings, the pale expanding sound-wave rings over the target; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_SCREECH_LILAC_RING  0xC /* emitter: lilac glow ring particle */

/* fx_0ce choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths and the trail buffer. */
#define FX_SCREECH_STATE_WAVES  0 /* PURPLE_MIST, SOUND_RINGS and TRAIL run from unit 0x10's bone */
#define FX_SCREECH_STATE_BLADES 1 /* FX_SCREECH_BLADE_RING drawn twice (subtractive and additive), spinning up */
#define FX_SCREECH_WAVES_FRAMES  6    /* FX_SCREECH_STATE_WAVES' length; then FX_SCREECH_LILAC_RING starts at the caster */
#define FX_SCREECH_BLADES_FRAMES 0x14 /* FX_SCREECH_STATE_BLADES' length (the op ends) */
#define FX_SCREECH_TRAIL_TRAIL_SLOTS  0x15 /* FX_SCREECH_TRAIL's trail buffer fx_0ce_trails: slots of */
#define FX_SCREECH_TRAIL_TRAIL_STRIDE 0x34 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* caster body position, raised by 0x80 */
    SVECTOR pos10; /* caster position */
    SVECTOR pos18; /* btlfx_getUnitArgBonePos(0x10, 0) */
    s32 model20; /* model */
    SVECTOR rot24; /* angles from pos18 to pos8 */
    Pool6CEntry *emit2C;
} Fx0ceWork;

extern Unk800F78A4 *fx_0ce_env;
extern BtlModelBuf fx_0ce_effModels[];
extern s32 fx_0ce_trails[];

#endif
