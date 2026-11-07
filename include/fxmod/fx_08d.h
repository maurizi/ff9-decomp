#ifndef FX_08D_H
#define FX_08D_H

/* Effect module fx_08d (dir 13 file 0x8D): Cura, one target (party; item Hi-Potion; enemy Cura of Beatrix).
 * Emitters and a btlfx_drawSpriteLine. Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_08d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CURA_STAR_TRAIL     0x1 /* emitter: blue star-sparkle trail + sprites */
#define FX_CURA_STAR_FLARES    0x7 /* sprite line: blue-white star flares */
#define FX_CURA_CRESCENT_BURST 0x8 /* emitter: ring + yellow, white and green crescent sprites */
#define FX_CURA_CYAN_GLOW      0xA /* emitter: fan of a round cyan-white glow, the cyan glow streaks round the target; runtime (docs/runtime-findings.md effect-data pass) */

/* fx_08d choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length. */
#define FX_CURA_CYAN_GLOW_FRAME          0x12 /* FX_CURA_CYAN_GLOW starts */
#define FX_CURA_CRESCENT_FRAME           0x14 /* FX_CURA_CRESCENT_BURST starts */
#define FX_CURA_STAR_FLARES_FRAME        0x25 /* FX_CURA_STAR_FLARES shows from this frame */
#define FX_CURA_STAR_FLARES_FRAMES       0xC  /* for this many frames */
#define FX_CURA_STAR_FLARES_FADE_IN_FRAMES 6  /* fading in over the first of them, then out */
#define FX_CURA_STAR_FLARES_ANIM_FRAMES  6    /* sprite-line frames (frame1C stops on the last) */
#define FX_CURA_FRAMES                   0x32 /* op80's length (the op ends) */
#define FX_CURA_STAR_TRAIL_TRAIL_SLOTS   3    /* FX_CURA_STAR_TRAIL's trail buffer fx_08d_trails: slots of */
#define FX_CURA_STAR_TRAIL_TRAIL_STRIDE  0xA0 /* this many bytes */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster body position */
    SVECTOR pos14; /* caster position */
    u16 frame1C;
    u16 alpha1E;
} Fx08dWork;

extern Unk800F78A4 *fx_08d_env;
extern s32 fx_08d_trails[];

#endif
