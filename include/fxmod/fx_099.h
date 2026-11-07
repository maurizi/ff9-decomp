#ifndef FX_099_H
#define FX_099_H

/* Effect module fx_099 (dir 13 file 0x99): Break. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_099 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BREAK_STONE_CRUST 0x1 /* 'so' model: jagged brown stone pieces (cracked rock texture) */
#define FX_BREAK_CYAN_RINGS 0x2 /* emitter: white-cyan / lilac gouraud rings + fan */
#define FX_BREAK_DUST_CLOUD 0x3 /* emitter: sparkles, brown dust cloud, pale puffs */
#define FX_BREAK_ROCK_RING  0x4 /* emitter: ring of white rock chunks */

/* fx_099 choreography (docs/fxdata.md, "Choreography literals"): op80's states, the stone crust's pieces. */
#define FX_BREAK_STATE_RISE 0 /* FX_BREAK_ROCK_RING, the crust's pieces rise round the target one after another */
#define FX_BREAK_STATE_FADE 1 /* the pieces glow and fade, CYAN_RINGS and DUST_CLOUD burst */
#define FX_BREAK_RISE_FRAMES 0x17 /* FX_BREAK_STATE_RISE's length */
#define FX_BREAK_FADE_FRAMES 0x10 /* FX_BREAK_STATE_FADE: the pieces show (fading) for this many frames */
#define FX_BREAK_DUST_FRAME  8    /* FX_BREAK_STATE_FADE: FX_BREAK_DUST_CLOUD starts */
#define FX_BREAK_FRAMES      0x20 /* FX_BREAK_STATE_FADE's length (the op ends) */
#define FX_BREAK_CRUST_PIECES 5   /* FX_BREAK_STONE_CRUST drawn this many times round the target */
#define FX_BREAK_PIECE_STAGGER 2  /* each piece starts this many frames after the one before */
#define FX_BREAK_PIECE_RISE_FRAMES 7 /* and closes up over this many */
#define FX_BREAK_CRUST_RADIUS 720 /* target radius at crust scale 1 (scale = radius / this, clamped, halved) */
#define FX_BREAK_MODEL_COUNT  1   /* btlfx_initEffModels slots: FX_BREAK_STONE_CRUST */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* caster position on the ground */
    SVECTOR pos10; /* caster body position */
    s32 scale18; /* model scale */
    s32 model1C; /* model */
    SVECTOR rot20; /* model rotation */
    s32 scale28[3]; /* model scale (VECTOR without its pad word) */
    s32 modelPos34[3]; /* model position (VECTOR without its pad word) */
} Fx099Work;

extern Unk800F78A4 *fx_099_env;
extern BtlModelBuf fx_099_effModels[];

#endif
