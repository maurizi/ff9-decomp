#ifndef FX_191_H
#define FX_191_H

/* Effect module fx_191 (dir 13 file 0x191): Lancer. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_191 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LANCER_GOLD_TRAIL   0x2 /* emitter: screen trail, orange-gold streak strip (byte-identical to entry 0x9) */
#define FX_LANCER_PATH         0x4 /* table: position path (fxm_posPathInit) */
#define FX_LANCER_PATH_2       0x5 /* table: position path (fxm_posPathInit) */
#define FX_LANCER_GOLD_TRAIL_2 0x9 /* emitter: screen trail, orange-gold streak strip (byte-identical to entry 0x2) */
#define FX_LANCER_RED_SPARKS   0xF /* emitter: white / pink gouraud rings + red spark dots */
#define FX_LANCER_ORANGE_RINGS 0x10 /* emitter: orange gouraud rings, 18 frames */
#define FX_LANCER_GREEN_RINGS  0x11 /* emitter: mint-green gouraud rings, 18 frames */
#define FX_LANCER_CYAN_SPARKS  0x12 /* emitter: white / pale-green gouraud rings + cyan spark dots */

/* fx_191 choreography (docs/fxdata.md, "Choreography literals"): op80's four streaks from the caster to the target,
 * op81's two path-following gold trails and the spark sweeps over the target, trail buffers and lengths. */
#define FX_LANCER_STREAKS        4    /* op80: streak emitters (fx_191_emitFileIds), each with its own motion and trail */
#define FX_LANCER_STREAK_TRAIL_SLOTS  4    /* op80: each streak's trail buffer fx_191_op80Trails[i]: slots of */
#define FX_LANCER_STREAK_TRAIL_STRIDE 0x60 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_LANCER_STREAK_FRAMES  12   /* op80: the streaks reach the target in this many frames */
#define FX_LANCER_STREAKS_FRAMES 0x28 /* op80's length (the op ends) */
#define FX_LANCER_GOLD_TRAIL_TRAIL_SLOTS  2    /* op81: GOLD_TRAIL's and GOLD_TRAIL_2's trail buffers: slots of */
#define FX_LANCER_GOLD_TRAIL_TRAIL_STRIDE 0x70 /* this many bytes */
#define FX_LANCER_CYAN_SPARKS_FRAME 3 /* op81: CYAN_SPARKS and GREEN_RINGS start; the sparks sweep across the target over 4 frames */
#define FX_LANCER_GOLD_TRAIL_2_FRAME 5 /* op81: GOLD_TRAIL_2 starts along PATH_2 (GOLD_TRAIL along PATH at init) */
#define FX_LANCER_RED_SPARKS_FRAME  8 /* op81: RED_SPARKS and ORANGE_RINGS start; the sparks sweep back over 4 frames */
#define FX_LANCER_FRAMES         0x28 /* op81's length (the op ends) */

typedef struct {
    s16 x, y, z;
} Fx191Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx191Point *keys4;
} Fx191Path;

typedef struct {
    Pool6CEntry *emit0[FX_LANCER_STREAKS];
    SVECTOR pos10; /* caster position */
    SVECTOR pos18; /* target position */
    SVECTOR rot20; /* angles */
} Fx191Work80; /* 0x28 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* target body position */
    u8 pad20[0x30 - 0x20];
    SVECTOR pos30; /* from, to, from, to */
    SVECTOR pos38;
    SVECTOR pos40;
    SVECTOR pos48;
    SVECTOR target50; /* current */
    SVECTOR target58;
} Fx191Work81; /* 0x60 */

extern u16 fx_191_emitFileIds[FX_LANCER_STREAKS]; /* files */
extern SVECTOR fx_191_op80Offsets[FX_LANCER_STREAKS];
extern s32 (*fx_191_op80ParticleMotions[FX_LANCER_STREAKS])();
extern Unk800F78A4 *fx_191_env;
extern Fx191Path fx_191_posPath0;
extern Fx191Path fx_191_posPath1;
extern SVECTOR fx_191_op80Origin;
extern SVECTOR fx_191_op80Targets[FX_LANCER_STREAKS];
extern SVECTOR fx_191_op81PathOrigin;
extern SVECTOR fx_191_emitPos0;
extern SVECTOR fx_191_emitPos1;
extern s32 fx_191_op80Trails[FX_LANCER_STREAKS][FX_LANCER_STREAK_TRAIL_SLOTS * FX_LANCER_STREAK_TRAIL_STRIDE / 4];
extern s32 fx_191_op81Trails0[];
extern s32 fx_191_op81Trails1[];
extern s32 fx_191_op80Frame; /* op80 frame */
extern s32 fx_191_op81Frame; /* op81 frame */

s32 fx_191_op80ParticleMotion0(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_191_op80ParticleMotion1(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_191_op80ParticleMotion2(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_191_op80ParticleMotion3(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fxm_lerpParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *base, SVECTOR *to, s32 frame, s32 frames);
static s32 fx_191_op81ParticleMotion0(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fx_191_op81ParticleMotion1(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fxm_followPathParticle0(Pool30Entry *pt, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames);
static s32 fxm_followPathParticle1(Pool30Entry *pt, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames);
static void fxm_posPathInit(Fx191Path *path, u16 *data);
static s32 fxm_posPathGet(Fx191Path *path, s32 t, SVECTOR *out);

#endif
