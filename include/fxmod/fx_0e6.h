#ifndef FX_0E6_H
#define FX_0E6_H

/* Effect module fx_0e6 (dir 13 file 0x0E6): enemy Stone (Mu). A model and a 25-point strip (btlfx_drawTrail) flying
 * from the caster to the target. Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0e6 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_STONE_ROCK_SHELL 0x2 /* model: ball with a rock texture */
#define FX_STONE_RED_RINGS  0x8 /* emitter: white-pink-red gouraud rings + spark sprites (texture not in the file) */
#define FX_STONE_ROCK_STRIP 0x9 /* trail: strip with a rock texture */

/* fx_0e6 choreography (docs/fxdata.md, "Choreography literals"): op80's flight, the rock strip, the model texture
 * and the op's length. */
#define FX_STONE_FLIGHT_FRAMES 4    /* FX_STONE_ROCK_SHELL flies caster -> target over this many frames (frame << 10 ramp); then RED_RINGS start */
#define FX_STONE_STRIP_POINTS  25   /* points of the ROCK_STRIP from the caster to the rock (fx_0e6_strip) */
#define FX_STONE_STRIP_WIDTH   0x20 /* their width */
#define FX_STONE_STRIP_FRAMES  8    /* the strip draws, fading, for this many frames */
#define FX_STONE_FRAMES        0x1E /* op80's length (the op ends) */
#define FX_STONE_ROCK_SHELL_TPAGE 0xF7 /* Hi_RegisterTexEffModel texture: 8-bit page (0x1C0, 0x100), abr 3, rock texture */
#define FX_STONE_ROCK_SHELL_CLUT  0x3D00 /* CLUT at (0x0, 0xF4) */
#define FX_STONE_ROCK_SHELL_U     0x0 /* texture origin (u, v) in the page */
#define FX_STONE_ROCK_SHELL_V     0x0

typedef struct {
    s32 model0; /* model */
    VECTOR scale4; /* scale */
    VECTOR pos14; /* position */
    SVECTOR rot24; /* rotation */
    SVECTOR pos2C;
    SVECTOR pos34;
} Fx0e6Work; /* 0x3C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_STONE_STRIP_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0e6Line;

extern Unk800F78A4 *fx_0e6_env;
extern BtlModelBuf fx_0e6_effModels[];
extern Fx0e6Line fx_0e6_strip;

#endif
