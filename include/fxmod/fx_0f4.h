#ifndef FX_0F4_H
#define FX_0F4_H

/* Effect module fx_0f4 (dir 13 file 0x0F4): Shift Break. Two growing textured models with layer textures
 * (btlfx_drawLayerTexture). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SHIFT_BREAK_WHITE_RINGS  0x2 /* emitter: white gouraud rings in a cone + a textured ring (texture not resolved) */
#define FX_SHIFT_BREAK_RAINBOW_HALO 0x3 /* emitter: orbiting particles, dark disc with a rainbow rim */
#define FX_SHIFT_BREAK_VIOLET_HALO  0x4 /* emitter: orbiting white swirl marks + blue-violet glow ring */
#define FX_SHIFT_BREAK_BACKDROP     0x8 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_SHIFT_BREAK_DOME         0xA /* model: stepped dome, white texture */
#define FX_SHIFT_BREAK_DOME_2       0xB /* model: stepped dome, white texture (byte-identical to entry 0xA) */
#define FX_SHIFT_BREAK_WHITE_GLOW   0xC /* emitter: textured rings in a cone, the big soft white-blue haze over the target (the ring UVs still read noise in the run-time page); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_SHIFT_BREAK_LIGHTNING    0xD /* emitter: white-cyan gouraud rings + blue lightning streak particles */
#define FX_SHIFT_BREAK_BACKDROP_2   0xE /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */

/* fx_0f4 choreography (docs/fxdata.md, "Choreography literals"): op80's keys, the two domes' rise and sink, their
 * fades and texture. */
#define FX_SHIFT_BREAK_MODEL_SLOTS        2    /* btlfx_initEffModels slots: DOME, DOME_2 */
#define FX_SHIFT_BREAK_VIOLET_HALO_FRAME  0xF  /* VIOLET_HALO starts */
#define FX_SHIFT_BREAK_DOME_FRAME         0x32 /* the domes draw from this frame, and BACKDROP replaces BACKDROP_2 */
#define FX_SHIFT_BREAK_DOME_FRAMES        0x30 /* for this many frames */
#define FX_SHIFT_BREAK_LIGHTNING_FRAME    0x36 /* LIGHTNING and WHITE_GLOW start */
#define FX_SHIFT_BREAK_FADE_OUT_FRAME     0x52 /* the domes fade out from this frame */
#define FX_SHIFT_BREAK_FADE_OUT_FRAMES    0x10 /* over this many (fadeOutTimer6A) */
#define FX_SHIFT_BREAK_FADE_IN_FRAMES     3    /* fade-in length of fadeInTimer68 (never started here) */
#define FX_SHIFT_BREAK_DOME_DELAY         4    /* DOME draws and grows this many frames after DOME_2 */
#define FX_SHIFT_BREAK_DOME_RISE_FRAMES   0x10 /* DOME rises for this many frames of the dome phase, then sinks */
#define FX_SHIFT_BREAK_DOME_2_RISE_FRAMES 0xC  /* DOME_2 likewise */
#define FX_SHIFT_BREAK_DOME_MAX_WIDTH     0x666 /* the domes' x / z scale stops here */
#define FX_SHIFT_BREAK_DOME_MAX_HEIGHT    0xCCC /* their y scale */
#define FX_SHIFT_BREAK_FRAMES             0x78 /* op80's length (the op ends) */
#define FX_SHIFT_BREAK_DOME_TPAGE         0x11A /* Hi_RegisterTexEffModel texture of DOME and DOME_2: 15-bit page (0x280, 0x100), abr 0 */
#define FX_SHIFT_BREAK_DOME_CLUT          0x0  /* CLUT at (0x0, 0x0) */
#define FX_SHIFT_BREAK_DOME_U             0x0  /* texture origin (u, v) in the page */
#define FX_SHIFT_BREAK_DOME_V             0x80

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR rot14; /* model rotation */
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    s32 rgb3C; /* model brightness */
    SVECTOR rot40; /* model 2 rotation */
    VECTOR scale48; /* model 2 scale */
    SVECTOR pos58; /* centre */
    SVECTOR startPos60;
    s16 fadeInTimer68; /* fade-in countdown */
    s16 fadeOutTimer6A; /* fade-out countdown */
    u16 model6C; /* model */
    u16 model6E; /* model 2 */
} Fx0f4Work; /* 0x70 */

extern Unk800F78A4 *fx_0f4_env;
extern BtlModelBuf fx_0f4_effModels[];

s32 fx_0f4_op80(s32 mode, Fx0f4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
