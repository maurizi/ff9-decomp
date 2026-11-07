#ifndef FX_02C_H
#define FX_02C_H

/* Effect module fx_02c (dir 13 file 0x2C). */
#include "btlfx_mod.h"

/* fx_02c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BAD_BREATH_GAS_CLOUD 0x7 /* emitter: yellow-grey and purple gas clouds + green / pink glows */
#define FX_BAD_BREATH_GAS_BURST 0x8 /* emitter: yellow-grey and purple gas clouds, glows, green gas burst */

/* fx_02c choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe, length and aim point. */
#define FX_BAD_BREATH_GAS_BURST_FRAME 8    /* op80: FX_BAD_BREATH_GAS_BURST starts at pos10 */
#define FX_BAD_BREATH_FRAMES          0x46 /* op80: the task ends at this frame */
#define FX_BAD_BREATH_AIM_HEIGHT      0xC8 /* pos10, where the gas cloud is aimed and the burst starts: this far above the targets' centre */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10;
    SVECTOR aim18; /* angles from pos8 to pos10 */
} Fx02cWork;

extern Unk800F78A4 *fx_02c_env;

#endif
