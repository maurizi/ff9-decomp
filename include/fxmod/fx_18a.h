#ifndef FX_18A_H
#define FX_18A_H

/* Effect module fx_18a (dir 13 file 0x18a): enemy Transform (Kuja/Trance Kuja). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_18a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TRANSFORM_AURA_CYLINDER  0x1 /* model: open cone ring, magenta-violet cloud texture (byte-identical to fx_101 FX_TRANCE_AURA_CYLINDER) */
#define FX_TRANSFORM_VIOLET_RINGS   0x3 /* rings: white-lilac-violet gouraud rings */
#define FX_TRANSFORM_VIOLET_RINGS_2 0x4 /* rings: white-violet gouraud rings */
#define FX_TRANSFORM_VIOLET_TRAIL   0x6 /* trail: violet speckled strip, 12 frames (per pool item) */
#define FX_TRANSFORM_BLUE_RING      0x9 /* rings: navy-blue gouraud ring */
#define FX_TRANSFORM_FAN_STREAK     0xA /* emitter: fan elements, a faint thin white-red streak in the render (look pending) */
#define FX_TRANSFORM_SPRITE_SPIRAL  0xC /* emitter: spiral of sprites + sub-sprites, renders black (look pending) */
#define FX_TRANSFORM_PINK_STREAKS   0xE /* emitter: orbiting fans, pink-white light streaks */
#define FX_TRANSFORM_RED_STREAKS    0xF /* emitter: orbiting fans, red light streaks with pink orbs */
#define FX_TRANSFORM_VIOLET_RING    0x12 /* rings: violet gouraud ring */

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR rot8; /* angles */
} Fx18aWork82; /* 0x10 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[15];
} Fx18aTrail; /* 0x7C */

typedef struct {
    s32 pos0; /* position, 20.12 */
    s32 pos4;
    s32 pos8;
    s16 velC; /* velocity */
    s16 velE;
    s16 vel10;
    s16 drag12; /* velocity factor per frame (btlfx_stepModuleParticle), 0x1000 = none */
    u16 slot14; /* trail */
    s16 index16; /* trail point index */
    SVECTOR *point18; /* trail point */
} Fx18aSpark; /* 0x1C */

typedef struct {
    s32 frame0; /* frame */
    u8 pad4[0xC];
    s16 *pool10; /* pool */
} Fx18aInst;

extern Unk800F78A4 *fx_18a_env;
extern Fx18aTrail fx_18a_trails[12];
extern s32 fx_18a_nextTrail;

static s32 fx_18a_op82PoolTask(s32 mode, Fx18aSpark *spark, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void fx_18a_resetTrails(void);
void fx_18a_spawnTrail(Fx18aInst *inst, SVECTOR *origin, s32 reach, SVECTOR *from, SVECTOR *to);

#define TRAIL(e) ((Fx18aTrail *)((e)->point18 - 2))

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14; /* caster body position */
    s32 frame1C; /* charge counter */
    s32 unk20; /* write-only (cleared at init) */
    s32 model24; /* model */
} Fx18aWork80; /* 0x28 */

extern s32 fx_18a_op80Done; /* set at the end of op80, nothing reads it (runtime watch, round 33) */
extern BtlModelBuf fx_18a_effModels[];

#endif
