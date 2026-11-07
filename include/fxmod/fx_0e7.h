#ifndef FX_0E7_H
#define FX_0E7_H

/* Effect module fx_0e7 (dir 13 file 0x0E7): Thievery. Two texture-list models dropped and tilted in front of the
 * target, a sprite line at the caster. Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0e7 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THIEVERY_CHEST_LID    0x1 /* model: treasure chest lid (Hi_RegisterTexListModel; shape only) */
#define FX_THIEVERY_CHEST_BOX    0x2 /* model: treasure chest box (shape only) */
#define FX_THIEVERY_LENS_FLARES  0x3 /* sprite line: lens-flare circles and rings */
#define FX_THIEVERY_GOLD_GLITTER 0x4 /* emitter: gold glitter + small glows */
#define FX_THIEVERY_WHITE_PUFF   0x5 /* emitter: white smoke puff */
#define FX_THIEVERY_GOLD_RINGS   0xC /* emitter: fans + white-yellow-orange gouraud rings */
#define FX_THIEVERY_GOLD_BURST   0xD /* emitter: golden puffs + yellow-orange / cyan-blue rings */

/* fx_0e7 choreography (docs/fxdata.md, "Choreography literals"): op80's lens flares; op81's chest drop, lid tilt,
 * keyframes and fade. */
#define FX_THIEVERY_FLARES_HEIGHT     0x9C4 /* op80: the LENS_FLARES sprite line hangs at this height over the target */
#define FX_THIEVERY_FLARES_FRAME      0x23  /* op80: the sprite line draws from this frame */
#define FX_THIEVERY_FLARES_END_FRAME  0x30  /* to this one */
#define FX_THIEVERY_FLARES_FADE_FRAME 5     /* and fades from this frame of it on */
#define FX_THIEVERY_FLARES_ANIM_FRAMES 6    /* sprite-line frames (lineFrame14 stops on the last) */
#define FX_THIEVERY_FLARES_FRAMES     0x32  /* op80's length (the op ends) */
#define FX_THIEVERY_DISTANCE    0x190 /* op81: the chest lands this far beyond the target's radius, in front of it */
#define FX_THIEVERY_DROP_HEIGHT 0xBB8 /* op81: the chest falls from this height */
#define FX_THIEVERY_REST_HEIGHT 0x96  /* to this one */
#define FX_THIEVERY_DROP_FRAMES 6     /* over this many frames from frame 0 (dropTimer70) */
#define FX_THIEVERY_PUFF_FRAME  4     /* op81: FX_THIEVERY_WHITE_PUFF on the ground */
#define FX_THIEVERY_TILT_FRAME  8     /* op81: the box stands tilted until this frame (GOLD_RINGS start), then */
#define FX_THIEVERY_TILT_FRAMES 8     /* turns upright over this many (tiltTimer74) */
#define FX_THIEVERY_RINGS_HEIGHT 0xC8 /* GOLD_RINGS start at this height over the chest */
#define FX_THIEVERY_FADE_FRAME  0x10  /* op81: the chest fades from this frame */
#define FX_THIEVERY_FADE_FRAMES 0x10  /* over this many (fadeTimer6C) */
#define FX_THIEVERY_DRAW_FRAMES 0x20  /* op81: the chest draws while frame < this */
#define FX_THIEVERY_BURST_FRAME 0x32  /* op81: FX_THIEVERY_GOLD_BURST at the target */
#define FX_THIEVERY_FRAMES      0x5A  /* op81's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target body position */
    SVECTOR linePosC;
    u16 lineFrame14;
    u16 lineAlpha16;
} Fx0e7Work80; /* 0x18 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR rotC; /* model rotations */
    SVECTOR rot14;
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    s32 rgb3C; /* model brightness */
    SVECTOR pos40; /* target position */
    SVECTOR rot48; /* target rotation */
    SVECTOR pos50;
    SVECTOR pos58;
    SVECTOR pos60;
    s32 blend68; /* when >= 0, both models switch to blend mode 1; set to -1 in mode 1 and never changed here (runtime check: not a countdown) */
    s32 fadeTimer6C; /* fade countdown */
    s32 dropTimer70; /* drop countdown */
    s32 tiltTimer74; /* tilt countdown */
    u16 model78; /* models */
    u16 model7A;
} Fx0e7Work81; /* 0x7C */

extern s16 fx_0e7_texPages[4];
extern s16 fx_0e7_texPos[4];
extern Unk800F78A4 *fx_0e7_env;
extern BtlModelBuf fx_0e7_effModels[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
