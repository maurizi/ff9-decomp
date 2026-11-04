#ifndef FX_0E1_1_H
#define FX_0E1_1_H

/* Effect module fx_0e1_1 (dir 13 file 0x0E1): Rebirth Flame (Phoenix's second attack), load group 1. Screen tints,
 * a sprite line and a layer texture, then emitters under a bobbing camera point with a ripple grid
 * (btlfx_drawRippleGrid). Evidence: [data] the party ability / item tables and enemy sequences that play this file
 * id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0e1_1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_REBIRTH_FLAME_TEX_EMITTER 0x1 /* emitter: textured elements + tan-white rings; the textured element still reads garbled texels (texture not in the file) (look pending) */
#define FX_REBIRTH_FLAME_RED_RINGS_2 0x2 /* emitter: white-grey-violet-red gouraud rings (byte-identical to fx_0e1_0 entry 0x6) */
#define FX_REBIRTH_FLAME_BACKDROP    0x3 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_REBIRTH_FLAME_RED_SPARKS  0x4 /* emitter: spiral of tiny red spark sprites */
#define FX_REBIRTH_FLAME_LENS_FLARES 0x5 /* sprite line: lens-flare rings and glows */
#define FX_REBIRTH_FLAME_STAR_FLARES 0x6 /* emitter: orange star flares + glowing dots */
#define FX_REBIRTH_FLAME_ORANGE_STREAKS 0x7 /* emitter: two fans, thin orange-white rising streaks */

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
