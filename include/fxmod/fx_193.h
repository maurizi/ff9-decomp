#ifndef FX_193_H
#define FX_193_H

/* Effect module fx_193 (dir 13 file 0x193): enemy Counter Horn (Antlion). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_193 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_COUNTER_HORN_RED_TRAIL     0x1 /* emitter: 3D trail, red-white flame streak (byte-identical to entry 0xF) */
#define FX_COUNTER_HORN_FLAME_RINGS   0x2 /* emitter: rings textured with red-pink flame streaks */
#define FX_COUNTER_HORN_FLAME_RINGS_2 0x3 /* emitter: rings textured with red-pink flame streaks */
#define FX_COUNTER_HORN_DUST_CLOUD    0x4 /* emitter: peach dust cloud bursting and fading, 12 frames */
#define FX_COUNTER_HORN_DARK_RING     0xE /* emitter: dark red textured ring */
#define FX_COUNTER_HORN_RED_TRAIL_2   0xF /* emitter: 3D trail, red-white flame streak (byte-identical to entry 0x1) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position */
} Fx193Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target body position */
    SVECTOR pos18; /* target position */
} Fx193Work81;

extern Unk800F78A4 *fx_193_env;
extern SVECTOR fx_193_op80From0;
extern SVECTOR fx_193_op80To0;
extern SVECTOR fx_193_op80From1;
extern SVECTOR fx_193_op80To1;
extern SVECTOR fx_193_emitPos;
extern s32 fx_193_op80Trails0[];
extern s32 fx_193_op80Trails1[];

static s32 fx_193_op80ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fx_193_op80ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fxm_wobbleParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 amp, s32 frame, s32 frames);

#endif
