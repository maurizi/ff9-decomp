#ifndef FX_0C9_H
#define FX_0C9_H

/* Effect module fx_0c9 (dir 13 file 0xC9): Jewel. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0c9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_JEWEL_GEM          0x1 /* model: faceted gem (polyhedron, white texture) */
#define FX_JEWEL_GEM_SPARKLES 0x2 /* emitter: stone / crystal glint particles and star sparkles (+ sprites, texture not in the file) */
#define FX_JEWEL_COMET_STREAKS 0x3 /* emitter: two fans, white-violet comet streaks */
#define FX_JEWEL_LIGHT_BURST  0x4 /* emitter: white / lilac / cyan rings and violet light streaks */
#define FX_JEWEL_ORE_GLINT    0x5 /* emitter: green ore glint, lilac / cyan ring, star sparkles */
#define FX_JEWEL_BACKDROP     0x6 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_JEWEL_GLOW_RING    0x10 /* emitter: white-cyan gouraud ring, fans */

/* fx_0c9 choreography (docs/fxdata.md, "Choreography literals"): the ops' keyframes and lengths, the gem's flight,
 * size and texture, the trail buffer. */
#define FX_JEWEL_FRAMES          0x28 /* op80 (GEM_SPARKLES, COMET_STREAKS): the task ends at this frame */
#define FX_JEWEL_GEM_FRAME       0xA  /* op81: FX_JEWEL_GEM, FX_JEWEL_ORE_GLINT and FX_JEWEL_BACKDROP start (elapsed = frame - this) */
#define FX_JEWEL_GLOW_RING_FRAME 0x1A /* op81: FX_JEWEL_GLOW_RING starts where the gem is */
#define FX_JEWEL_FLIGHT_FRAMES   0x11 /* op81: the gem (and ORE_GLINT's particles) arc from the caster to unit 0x10 this long */
#define FX_JEWEL_GEM_SCALE       0x1B /* op81: the gem's start scale, */
#define FX_JEWEL_GROW_FRAMES     0xC  /* growing by 1 a frame this long, */
#define FX_JEWEL_SHRINK_FRAME    0x2E /* shrinking by 8 a frame from this elapsed frame on */
#define FX_JEWEL_GEM_FRAMES      0x50 /* op81: the task ends at this frame */
#define FX_JEWEL_GEM_TPAGE       0x117 /* FX_JEWEL_GEM's texture (Hi_RegisterTexEffModel): 15-bit page (0x1C0, 0x100) */
#define FX_JEWEL_GEM_CLUT        0     /* (no CLUT) */
#define FX_JEWEL_GEM_U           0
#define FX_JEWEL_GEM_V           0xC0
#define FX_JEWEL_ORE_GLINT_TRAIL_SLOTS  0x12 /* FX_JEWEL_ORE_GLINT's trail buffer fx_0c9_trails: slots of */
#define FX_JEWEL_ORE_GLINT_TRAIL_STRIDE 0xB0 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster position */
    SVECTOR pos10; /* caster body position */
} Fx0c9Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* unit 0x10 body position, raised by 0x50 */
    SVECTOR pos14; /* caster position */
    SVECTOR pos1C; /* moves from pos14 to posC */
    SVECTOR rot24; /* model rotation */
    VECTOR pos2C; /* model position */
    VECTOR scale3C; /* model scale */
    SVECTOR pos4C;
    u16 model54; /* model */
} Fx0c9Work81;

extern Unk800F78A4 *fx_0c9_env;
extern BtlModelBuf fx_0c9_effModels[];
extern SVECTOR fx_0c9_particlePos; /* particle position */
extern SVECTOR fx_0c9_emitPos;
extern s32 fx_0c9_trails[];

static s32 fx_0c9_op81ParticleMotion(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
