#ifndef FX_02A_H
#define FX_02A_H

/* Effect module fx_02a (dir 13 file 0x2A). */
#include "btlfx_mod.h"

/* fx_02a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SILENCE_GLYPH      0x1 /* emitter: kana glyph sprite + dotted mouth shape (one element's texture not in the file) */
#define FX_SILENCE_GLYPH_GLOW 0x3 /* emitter: blurred kana glyph + specks */
#define FX_SILENCE_BACKDROP   0x4 /* backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) (look pending) */
#define FX_SILENCE_RUNE_SEAL  0xA /* emitter: grey stone ring carved with runes */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body position */
    s32 unkC; /* unused (runtime watch, round 33) */
} Fx02aWork;

extern Unk800F78A4 *fx_02a_env;

#endif
