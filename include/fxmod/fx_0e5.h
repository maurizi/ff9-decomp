#ifndef FX_0E5_H
#define FX_0E5_H

/* Effect module fx_0e5 (dir 13 file 0x0E5): Soul Blade. Emitters aimed from the caster at the target and a sprite
 * line at the target (btlfx_drawSpriteLine). Evidence: [data] the party ability / item tables and enemy sequences
 * that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0e5 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SOUL_BLADE_LIGHT_FLASH  0x6 /* emitter: lilac / blue-white rings + white light streak */
#define FX_SOUL_BLADE_SPIRIT_WISPS 0x8 /* emitter: blue rings, white sparkles, golden smoke wisps */
#define FX_SOUL_BLADE_LENS_FLARES  0x9 /* sprite line: lens-flare circles and rings */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR rot20;
    SVECTOR aimRot28;
    SVECTOR pos30;
    SVECTOR linePos38;
    u16 lineFrame40;
    u16 lineAlpha42;
} Fx0e5Work;

extern Unk800F78A4 *fx_0e5_env;

#endif
