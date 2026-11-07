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
#define FX_CARBUNCLE_BACKDROP     0xB /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) (byte-identical to fx_0b1 entry 0x22) */
#define FX_CARBUNCLE_STAR_CRYSTAL 0xC /* model: four-point star crystal (texture 0x177), one per target (byte-identical to fx_0b1 entry 0x23) */

/* fx_1f8 choreography (docs/fxdata.md, "Choreography literals"): the short form's op80 (one orbit glow) and
 * op81's star crystals, which is fx_0b1's op81 (same names, own values where they differ). */
#define FX_CARBUNCLE_GLOW_FRAMES    0x40 /* op80: ORBIT_GLOW_2 plays in front of the camera; the task ends at this frame */
#define FX_CARBUNCLE_MODEL_COUNT    4    /* op81: btlfx_initEffModels slots: a STAR_CRYSTAL per target (fx_1f8_models) */
#define FX_CARBUNCLE_STAR_CRYSTAL_TPAGE 0x177  /* FX_CARBUNCLE_STAR_CRYSTAL's texture (Hi_RegisterTexEffModel): 15-bit page (0x1C0, 0x100) */
#define FX_CARBUNCLE_STAR_CRYSTAL_CLUT  0xFFFF /* (no CLUT) */
#define FX_CARBUNCLE_STAR_CRYSTAL_U     0
#define FX_CARBUNCLE_STAR_CRYSTAL_V     0x80
#define FX_CARBUNCLE_CRYSTAL_FRAMES      0x20 /* op81: the crystals show (fx_1f8_timer) for this many frames from frame 0, */
#define FX_CARBUNCLE_CRYSTAL_FADE_FRAMES 8    /* fading in over the first and out over the last this many */
#define FX_CARBUNCLE_OP81_FRAMES    0x30 /* op81: the task ends at this frame */

extern Unk800F78A4 *fx_1f8_env;
extern SVECTOR fx_1f8_emitPos0;
extern s32 fx_1f8_timer[1]; /* countdown */
extern s32 fx_1f8_models[FX_CARBUNCLE_MODEL_COUNT]; /* models */
extern SVECTOR fx_1f8_modelRot; /* model rotation */
extern VECTOR fx_1f8_modelScale; /* model scale */
extern VECTOR fx_1f8_modelPos[FX_CARBUNCLE_MODEL_COUNT]; /* model positions */
extern SVECTOR fx_1f8_emitPos1[FX_CARBUNCLE_MODEL_COUNT]; /* unit body positions */
extern BtlModelBuf fx_1f8_effModels[];

#endif
