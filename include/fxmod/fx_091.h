#ifndef FX_091_H
#define FX_091_H

/* Effect module fx_091 (dir 13 file 0x91): enemy Entice (Lamia). Emitters and a layer texture. Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_091 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ENTICE_BACKDROP    0x1 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_ENTICE_VIOLET_HALO 0x2 /* emitter: violet glow ring + sprites */

typedef struct {
    s32 bgFrame0; /* frame for btlfx_drawLayerTexture */
    Pool6CEntry *emit4;
    SVECTOR pos8; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos10; /* caster body position */
    SVECTOR aim18; /* angles from pos8 to pos10 */
} Fx091Work;

extern Unk800F78A4 *fx_091_env;

#endif
