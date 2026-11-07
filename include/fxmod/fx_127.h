#ifndef FX_127_H
#define FX_127_H

/* Effect module fx_127 (dir 13 file 0x127): enemy Wave (Garland). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_127 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WAVE_WHITE_RINGS     0x1 /* emitter: white gouraud rings in a tube, 28 frames */
#define FX_WAVE_WHITE_RINGS_2   0x2 /* emitter: white gouraud rings in a cylinder, 28 frames */
#define FX_WAVE_WATER_STREAK    0x3 /* emitter: fan along an axis, a thin white-blue water streak */
#define FX_WAVE_WATER_STREAK_2  0x4 /* emitter: fan along an axis, a thin white-blue water streak */
#define FX_WAVE_STRIPED_RINGS   0x5 /* emitter: white and dark striped gouraud rings */
#define FX_WAVE_STRIPED_RINGS_2 0x6 /* emitter: white and dark striped gouraud rings, homing */
#define FX_WAVE_WATER_TRAIL     0x7 /* emitter: screen trail, grey streak strip (part of the texture not in the file) */
#define FX_WAVE_WHITE_RINGS_3   0xA /* emitter: white gouraud rings, 38 frames + a fan element */
#define FX_WAVE_WHITE_RING      0xD /* emitter: white gouraud ring */
#define FX_WAVE_WHITE_RINGS_4   0x12 /* emitter: white gouraud rings, 38 frames */

/* fx_127 choreography (docs/fxdata.md, "Choreography literals"): op80's keys (charge at the caster's bone 0x3C,
 * release of the wave along a path to the target), the trail buffers, op81's keys. */
#define FX_WAVE_PATH_FRAMES          5     /* the particle callbacks move STRIPED_RINGS / WATER_TRAIL to the target over this many frames */
#define FX_WAVE_CHARGE_FRAME         0xA   /* WHITE_RINGS, WATER_STREAK(_2) and WHITE_RINGS_2 start; they turn towards the target from here */
#define FX_WAVE_FOLLOW_FRAMES        4     /* and follow the caster's bone for this many frames */
#define FX_WAVE_RELEASE_FRAME        0x22  /* STRIPED_RINGS, WATER_TRAIL and STRIPED_RINGS_2 start (path set from the bone to the target) */
#define FX_WAVE_STRIPED_TRAIL_SLOTS  0x112 /* STRIPED_RINGS' trail buffer (fx_127_op80Trails0): slots */
#define FX_WAVE_STRIPED_TRAIL_STRIDE 0x18  /* bytes per slot */
#define FX_WAVE_WATER_TRAIL_SLOTS    0x28  /* WATER_TRAIL's (fx_127_op80Trails1): slots */
#define FX_WAVE_WATER_TRAIL_STRIDE   0x40  /* bytes per slot */
#define FX_WAVE_AIM_FRAME            0x27  /* the point behind the target for op81 is set */
#define FX_WAVE_CAST_FRAMES          0x50  /* op80's length */
#define FX_WAVE_WHITE_RING_FRAME     4     /* op81: WHITE_RING starts (WHITE_RINGS_3 / _4 on frame 0) */
#define FX_WAVE_HIT_FRAMES           0x50  /* op81's length */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    u8 pad1C[0x24 - 0x1C];
    SVECTOR pos24; /* caster position */
    SVECTOR rot2C; /* caster rotation */
    SVECTOR pos34; /* target body position */
    SVECTOR pos3C; /* target position */
    SVECTOR pos44; /* caster bone position */
    SVECTOR pos4C;
    SVECTOR rot54;
    SVECTOR rot5C;
    SVECTOR pos64;
    SVECTOR pos6C;
    SVECTOR rot74;
    SVECTOR rot7C;
} Fx127Work80; /* 0x84 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    s32 unk8; /* unused */
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target body position */
    SVECTOR rot18;
    SVECTOR rot20;
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
} Fx127Work81; /* 0x40 */

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*motionFn8)(VECTOR *, s32, SVECTOR *, s32, s32, s32); /* particle motion callback, installed by the module */
} Fx127EffHdr;

extern SVECTOR fx_127_aimRot;
extern SVECTOR fx_127_aimPoint;
extern SVECTOR fx_127_emitPos0;
extern SVECTOR fx_127_emitPos1;
extern SVECTOR fx_127_emitPos2;
extern SVECTOR fx_127_emitPos3;
extern s32 fx_127_op80Trails0[];
extern s32 fx_127_op80Trails1[];
extern Unk800F78A4 *fx_127_env;
extern SVECTOR fx_127_lerpFrom; /* path start */
extern SVECTOR fx_127_lerpTo; /* path end */
extern SVECTOR fx_127_emitterPos;
extern Pool6CEntry *fx_127_emitter;

static s32 fx_127_op80ParticleMotion0(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_127_op80ParticleMotion1(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_lerpParticle(VECTOR *out, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 len, s32 frame, s32 n);

#endif
