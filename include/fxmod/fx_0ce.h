#ifndef FX_0CE_H
#define FX_0CE_H

/* Effect module fx_0ce (dir 13 file 0xCE): enemy Screech, Shriek (Gimme Cat, ...). Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ce effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SCREECH_TRAIL       0x1 /* emitter: 3D trail; the render still reads garbled pink stripes (texture not resolved) (look pending) */
#define FX_SCREECH_PURPLE_MIST 0x2 /* emitter: dark purple mist rings and brown-pink gouraud rings */
#define FX_SCREECH_BLADE_RING  0x7 /* 'so' model: ring of pale green curved blades */
#define FX_SCREECH_DARK_RINGS  0xB /* emitter: textured rings; the render is almost black (faint dark red) (look pending) */
#define FX_SCREECH_LILAC_RING  0xC /* emitter: lilac glow ring particle */

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
