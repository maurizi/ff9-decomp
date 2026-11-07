#ifndef FX_0E1_1_H
#define FX_0E1_1_H

/* Effect module fx_0e1_1 (dir 13 file 0x0E1): Rebirth Flame (Phoenix's second attack), load group 1. Screen tints,
 * a sprite line and a layer texture, then emitters under a bobbing camera point with a ripple grid
 * (btlfx_drawRippleGrid). Evidence: [data] the party ability / item tables and enemy sequences that play this file
 * id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0e1_1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_REBIRTH_FLAME_TEX_EMITTER 0x1 /* emitter: textured elements + tan-white rings; the textured element still reads garbled texels (texture not in the file); runtime (docs/runtime-findings.md effect-data pass): fetched during the fire blast (orange-white flames on screen); its ring elements read noise in the run-time page (look pending) */
#define FX_REBIRTH_FLAME_RED_RINGS_2 0x2 /* emitter: white-grey-violet-red gouraud rings (byte-identical to fx_0e1_0 entry 0x6) */
#define FX_REBIRTH_FLAME_BACKDROP    0x3 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_REBIRTH_FLAME_RED_SPARKS  0x4 /* emitter: spiral of tiny red spark sprites */
#define FX_REBIRTH_FLAME_LENS_FLARES 0x5 /* sprite line: lens-flare rings and glows */
#define FX_REBIRTH_FLAME_STAR_FLARES 0x6 /* emitter: orange star flares + glowing dots */
#define FX_REBIRTH_FLAME_ORANGE_STREAKS 0x7 /* emitter: two fans, thin orange-white rising streaks */

/* fx_0e1_1 choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths, keyframes and
 * fades. */
#define FX_REBIRTH_FLAME_STATE_FLARE 0 /* op80: LENS_FLARES, BACKDROP, the screen whitens */
#define FX_REBIRTH_FLAME_STATE_RISE  1 /* the camera rises with RED_SPARKS / STAR_FLARES, ripple grid; ends once they faded */
#define FX_REBIRTH_FLAME_FLARES_FRAMES 0x14 /* STATE_FLARE: the LENS_FLARES sprite line fades over this many frames */
#define FX_REBIRTH_FLAME_WHITE_FRAME   0x24 /* STATE_FLARE: an additive white flash grows from this frame */
#define FX_REBIRTH_FLAME_WHITE_FRAMES  16   /* over this many */
#define FX_REBIRTH_FLAME_FLARE_FRAMES  0x34 /* STATE_FLARE's length */
#define FX_REBIRTH_FLAME_CAM_HEIGHT    0x800 /* STATE_RISE: the camera point starts this far above fx_0e1_1_camPos */
#define FX_REBIRTH_FLAME_STARS_HEIGHT  0x300 /* STAR_FLARES start this far above it */
#define FX_REBIRTH_FLAME_SWAY          0x200 /* STATE_RISE: camera sway amplitude, shrinking by 8 per frame */
#define FX_REBIRTH_FLAME_FADE_FRAME    0x35  /* STATE_RISE: the particles fade out from here once the party reload is done */
#define FX_REBIRTH_FLAME_RISE_FRAMES   0x41  /* STATE_RISE: the camera rises for this many frames */
#define FX_REBIRTH_FLAME_UNWHITE_FRAMES 0x18 /* STATE_RISE: the white flash fades over this many frames */
#define FX_REBIRTH_FLAME_RIPPLE_FRAMES  0x20 /* STATE_RISE: the ripple grid draws for this many frames */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR pos8; /* effect origin */
    s32 unk10; /* unused (runtime watch, round 33) */
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR camPos24; /* camera position */
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    s32 swayAmp34; /* sway amplitude */
    s32 fadeOut38; /* fading out */
    s32 alpha3C; /* particle alpha */
} Fx0e11Work; /* 0x40 */

extern SVECTOR fx_0e1_1_origin;
extern SVECTOR fx_0e1_1_camPos;
extern CVECTOR fx_0e1_1_rippleRgb;
extern Unk800F78A4 *fx_0e1_1_env;

s32 fx_0e1_1_op80(s32 mode, Fx0e11Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */
s32 fx_0e1_1_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
