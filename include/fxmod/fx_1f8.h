#ifndef FX_1F8_H
#define FX_1F8_H

/* Effect module fx_1f8 (dir 13 file 0x1f8): Carbuncle (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f8 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CARBUNCLE_ORBIT_GLOW_2 0x2 /* emitter: orbiting particle + sprite, orange speckled arcs (same call site as fx_0b1 entry 0x16, same kind) */
#define FX_CARBUNCLE_COLOR_RINGS  0x5 /* emitter: pink / green / pink gouraud rings at each target body (byte-identical to fx_0b1 entry 0x18) */
#define FX_CARBUNCLE_PINK_RING    0xA /* emitter: pink gouraud ring and swirl-ring particles at each target body (byte-identical to fx_0b1 entry 0x21) */
#define FX_CARBUNCLE_BACKDROP     0xB /* table: background layers for btlfx_drawBackdrop (byte-identical to fx_0b1 entry 0x22) */
#define FX_CARBUNCLE_STAR_CRYSTAL 0xC /* model: four-point star crystal (texture 0x177), one per target (byte-identical to fx_0b1 entry 0x23) */

extern Unk800F78A4 *fx_1f8_env;
extern SVECTOR fx_1f8_emitPos0;
extern s32 fx_1f8_timer[1]; /* countdown */
extern s32 fx_1f8_models[4]; /* models */
extern SVECTOR fx_1f8_modelRot; /* model rotation */
extern VECTOR fx_1f8_modelScale; /* model scale */
extern VECTOR fx_1f8_modelPos[4]; /* model positions */
extern SVECTOR fx_1f8_emitPos1[4]; /* unit body positions */
extern BtlModelBuf fx_1f8_effModels[];

#endif
