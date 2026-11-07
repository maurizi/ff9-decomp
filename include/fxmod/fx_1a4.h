#ifndef FX_1A4_H
#define FX_1A4_H

/* Effect module fx_1a4 (dir 13 file 0x1a4): enemy Death0 (Deathguise). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH0_ROCK_DEBRIS    0x1 /* emitter: brown rock chips + orange spark dots */
#define FX_DEATH0_PINK_RINGS     0x2 /* emitter: spiral of white-pink-magenta gouraud rings + tan dust cloud */
#define FX_DEATH0_DUST_BURST     0x3 /* emitter: tan dust clouds + grey smoke */
#define FX_DEATH0_FIRE_DEBRIS    0x4 /* emitter: brown rock chips + spark dots + red fire flames */
#define FX_DEATH0_COLOUR_STREAKS 0x5 /* emitter: fans of thin blue / orange / green streaks with violet and teal smoke + pale-blue rings */
#define FX_DEATH0_FLAME_COLUMN   0x7 /* 'so' model: tall cylinder, flame texture */
#define FX_DEATH0_DUST_SPIRAL    0xC /* emitter: spiral of tan dust clouds + grey smoke */
#define FX_DEATH0_DUST_SPIRAL_2  0xD /* emitter: spiral of tan dust clouds + grey smoke */
#define FX_DEATH0_FLAME_COIL     0xE /* 'so' model: helix coil, flame texture */
#define FX_DEATH0_FLAME_COIL_2   0xF /* 'so' model: helix coil, flame texture */

/* fx_1a4 choreography (docs/fxdata.md, "Choreography literals"): op80's streaks and debris at the caster's argument
 * bone, the caster sinking into the ground and fading; op81's flame column and coils (scrolled flame textures)
 * growing over the caster and fading; countdowns, VRAM rects, keyframes and lengths. */
#define FX_DEATH0_BURST_FRAME  0x1E  /* op80: DUST_BURST and FIRE_DEBRIS, following the argument bone from then on */
#define FX_DEATH0_DEBRIS_FRAME 0x49  /* op80: ROCK_DEBRIS and PINK_RINGS */
#define FX_DEATH0_SINK_FRAME   0x4C  /* op80: the caster sinks (when fxArgs[1] == 0) */
#define FX_DEATH0_SINK_FRAMES  0x60  /* over this many frames (timer3C, drop = frame^2) */
#define FX_DEATH0_HOP_FRAMES   0x10  /* timer3E's hop length (never started: it is only ever reset to -1) */
#define FX_DEATH0_FADE_FRAME   0x6A  /* op80: the caster fades */
#define FX_DEATH0_FADE_FRAMES  0x20  /* over this many frames (timer40) */
#define FX_DEATH0_FADE_MIN     0x46  /* down to this unit alpha */
#define FX_DEATH0_FRAMES       0x96  /* op80's length (the op ends) */
#define FX_DEATH0_MODELS       3     /* op81: btlfx_initEffModels slots (FLAME_COLUMN, FLAME_COIL, FLAME_COIL_2) */
#define FX_DEATH0_GROW_FRAMES  0xC   /* op81: the models widen and rise while frame < this, then narrow */
#define FX_DEATH0_FLAMES_FADE_FRAME  0x2D /* op81: the models fade */
#define FX_DEATH0_FLAMES_FADE_FRAMES 0x15 /* over this many frames (timer52), then stop drawing */
#define FX_DEATH0_COLUMN_VRAM_X 0x1E0 /* btlfx_drawScrollMove: FLAME_COLUMN's 4-bit texture (tpage 0x17, u 0x80, v 0x80) */
#define FX_DEATH0_COLUMN_VRAM_Y 0x180
#define FX_DEATH0_COLUMN_SRC_X  0x1E0 /* scrolled from the rect above it */
#define FX_DEATH0_COLUMN_SRC_Y  0x100
#define FX_DEATH0_COIL_VRAM_X   0x200 /* FLAME_COIL / _2's texture (tpage 0x18, u 0, v 0x80) */
#define FX_DEATH0_COIL_VRAM_Y   0x180
#define FX_DEATH0_COIL_SRC_X    0x200
#define FX_DEATH0_COIL_SRC_Y    0x100
#define FX_DEATH0_FLAME_VRAM_W  0x20  /* both rects' size */
#define FX_DEATH0_FLAME_VRAM_H  0x80
#define FX_DEATH0_FLAMES_FRAMES 0x64  /* op81's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24; /* caster position */
    SVECTOR pos2C;
    s32 fade34; /* brightness */
    s32 flag38; /* flag */
    s16 timer3C; /* countdowns */
    s16 timer3E;
    s16 timer40;
} Fx1a4Work80; /* 0x44 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* model rotations */
    SVECTOR rot10;
    SVECTOR rot18;
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    s32 color40; /* brightness */
    SVECTOR pos44; /* caster body position */
    s16 frame4C; /* counter */
    s16 speed4E;
    s16 speed50;
    s16 timer52; /* fade countdown */
    u16 model54; /* models */
    u16 model56;
    u16 model58;
} Fx1a4Work81; /* 0x5C */

extern Unk800F78A4 *fx_1a4_env;
extern BtlModelBuf fx_1a4_effModels[];
extern SVECTOR fx_1a4_emitPos0;
extern SVECTOR fx_1a4_emitPos1;

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

/* copied */
extern Unk800F78A4 *fx_1a4_env;
/* end copied */

#endif
