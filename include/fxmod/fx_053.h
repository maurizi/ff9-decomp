#ifndef FX_053_H
#define FX_053_H

/* Effect module fx_053 (dir 13 file 0x53). */
#include "btlfx_mod.h"

/* fx_053 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WHITE_DRAW_BLUE_SPARKLES   0x1 /* emitter: blue bow-tie sparkle sprites + sparks (texture not in the file) */
#define FX_WHITE_DRAW_GREEN_CRYSTAL   0x2 /* emitter: white ring + green crystal particles */
#define FX_WHITE_DRAW_LIGHT_STREAK    0x4 /* emitter: fan drawn as a thin green to violet horizontal light streak */
#define FX_WHITE_DRAW_SPARKLE_CIRCLE  0x5 /* emitter: circle of gold / white sparkle quads + blue-green specks */
#define FX_WHITE_DRAW_BLUE_SPARKLES_2 0xE /* emitter: blue bow-tie sparkle sprites */
#define FX_WHITE_DRAW_TRAIL           0xF /* emitter: grey trail, blue specks, sparks (texture not in the file) */
#define FX_WHITE_DRAW_WHITE_RINGS     0x13 /* emitter: white gouraud rings */
#define FX_WHITE_DRAW_GREEN_CRYSTAL_2 0x14 /* emitter: green crystal particles */

/* fx_053 choreography (docs/fxdata.md, "Choreography literals"): op80's length; op81's keyframe and length; op82's release and length. */
#define FX_WHITE_DRAW_FRAMES          0x28 /* op80: the task ends at this frame */
#define FX_WHITE_DRAW_SPARKLES_FRAME  0xA  /* op81: FX_WHITE_DRAW_BLUE_SPARKLES_2 start */
#define FX_WHITE_DRAW_TRAIL_FRAMES    0x32 /* op81: the task ends at this frame */
#define FX_WHITE_DRAW_RELEASE_FRAME   4    /* op82: sets BTLFX_SIG_WAIT, releasing the script's wait (the next target's task starts) */
#define FX_WHITE_DRAW_CRYSTAL_FRAMES  0x28 /* op82: the task ends at this frame */
#define FX_WHITE_DRAW_TRAIL_TRAIL_SLOTS  6 /* FX_WHITE_DRAW_TRAIL's trail buffer fx_053_trails: slots of */
#define FX_WHITE_DRAW_TRAIL_TRAIL_STRIDE 0x88 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR rot10; /* caster rotation */
} Fx053Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    u8 pad8[4];
    SVECTOR posC;
} Fx053Work81;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target body position */
    s32 target18; /* target index */
} Fx053Work82;

extern Unk800F78A4 *fx_053_env;
extern s32 fx_053_trails[];

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
