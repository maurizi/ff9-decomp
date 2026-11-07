#ifndef FX_0F2_H
#define FX_0F2_H

/* Effect module fx_0f2 (dir 13 file 0x0F2): Tidal Flame. A textured model sweeping across the field with a layer texture
 * (btlfx_drawLayerTexture). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TIDAL_FLAME_FIRE_DOME   0x2 /* model: hemisphere; page 0x11A holds flames at run time: the fire dome behind the target; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_TIDAL_FLAME_BACKDROP    0x3 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_TIDAL_FLAME_RUNE_BURST  0x8 /* emitter: white rune glyphs with cyan-blue rings, embers, blue light streak */
#define FX_TIDAL_FLAME_FLAME_CLOUD 0x9 /* emitter: embers + orange flame cloud */

/* fx_0f2 choreography (docs/fxdata.md, "Choreography literals"): op80's sweep of the fire dome, its fades, the
 * model texture and the op's length. */
#define FX_TIDAL_FLAME_SPEED           0xC8 /* the dome sweeps along z this fast (towards the party when an enemy casts, else away) */
#define FX_TIDAL_FLAME_SWEEP_FRAME     0x11 /* FLAME_CLOUD starts, the dome and BACKDROP draw from this frame */
#define FX_TIDAL_FLAME_SWEEP_FRAMES    0x14 /* for this many frames */
#define FX_TIDAL_FLAME_FADE_IN_FRAMES  3    /* the dome fades in over this many frames (fadeInTimer54) */
#define FX_TIDAL_FLAME_FADE_OUT_FRAME  0x15 /* and out from this frame */
#define FX_TIDAL_FLAME_FADE_OUT_FRAMES 0x10 /* over this many (fadeOutTimer56) */
#define FX_TIDAL_FLAME_FRAMES          0x3C /* op80's length (the op ends) */
#define FX_TIDAL_FLAME_FIRE_DOME_TPAGE 0x11A /* Hi_RegisterTexEffModel texture: 15-bit page (0x280, 0x100), abr 0, flames at run time (runtime pass 2) */
#define FX_TIDAL_FLAME_FIRE_DOME_CLUT  0x0 /* CLUT at (0x0, 0x0) */
#define FX_TIDAL_FLAME_FIRE_DOME_U     0x0 /* texture origin (u, v) in the page */
#define FX_TIDAL_FLAME_FIRE_DOME_V     0x80

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* model rotation */
    VECTOR pos10; /* model position */
    VECTOR scale20; /* model scale */
    s32 rgb30; /* model brightness */
    s32 speed34; /* z speed */
    SVECTOR pos38;
    SVECTOR startPos40;
    SVECTOR pos48;
    s32 isEnemy50; /* enemy side */
    s16 fadeInTimer54; /* fade-in countdown */
    s16 fadeOutTimer56; /* fade-out countdown */
    u16 model58; /* model */
} Fx0f2Work; /* 0x5C */

extern Unk800F78A4 *fx_0f2_env;
extern BtlModelBuf fx_0f2_effModels[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
