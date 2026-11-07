#ifndef FX_049_H
#define FX_049_H

/* Effect module fx_049 (dir 13 file 0x49): Thundara, all targets (and one large target). Lightning bolts drawn as
 * jagged strips (fxm_drawJaggedStrip16 / fxm_drawJaggedStrip4, fxm_drawJaggedBranch4). Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_049 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THUNDARA_IMPACT_FLASH 0x2 /* emitter: four-point star flash, white rings, sparks */
#define FX_THUNDARA_IMPACT_RING  0x6 /* emitter: white-cyan gouraud rings */
#define FX_THUNDARA_ORB_EMITTER  0x15 /* emitter: helix whose callback element draws the orb (0x16, 0x1C-0x1F) */
#define FX_THUNDARA_SPARK_DOTS   0x18 /* emitter: white spark dots (btlfx_emitDots) */
#define FX_THUNDARA_ORB_FADE     0x19 /* particles: blue energy sphere fading to orange */
#define FX_THUNDARA_ORB          0x1A /* particles: bright blue energy sphere */
#define FX_THUNDARA_STAR_GLINTS  0x1B /* sprites: three 16x16 star-glint sprites in the run-time page (0x1C0,0x100), twinkling orange to blue around the orbs; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_THUNDARA_ORB_DOTS     0x1C /* sprites: white dots scattering around the orb */
#define FX_THUNDARA_LENS_FLARES  0x1F /* particles: orange lens-flare dots around the orb */

/* fx_049 choreography (docs/fxdata.md, "Choreography literals"): the bolt pool items' lengths and look, op83's
 * keyframes and length, and the strip sizes. */
#define FX_THUNDARA_BOLT_FRAMES      8    /* pool item: a screen-space bolt shows for this many frames */
#define FX_THUNDARA_FLAT_BOLT_FRAMES 6    /* pool item: a flat-strip bolt shows for this many frames */
#define FX_THUNDARA_BOLT_TAIL_FRAMES 6    /* pool item: then waits this long for FX_THUNDARA_SPARK_DOTS */
#define FX_THUNDARA_BOLT_WIDTH       0xFB /* line width of a screen-space bolt at its first frame (+ width15) */
#define FX_THUNDARA_FLAT_BOLT_WIDTH  0xAA /* line width of a flat-strip bolt at its first frame */
#define FX_THUNDARA_BOLT_TINT_RGB    0, 0xC, 0x46 /* additive screen tint (blue) with every other bolt's impact flash */
#define FX_THUNDARA_BOLT_HEIGHT      0xC00 /* a bolt comes down from this far above its strike point */
#define FX_THUNDARA_BOLT_REACH       0x640 /* horizontal offset of a strike bolt's top (+-0x80; scattered bolts 3/8) */
#define FX_THUNDARA_SCHEDULE_FRAMES  0x10 /* op83: frames covered by fx_049_boltSchedule */
#define FX_THUNDARA_DARKEN_FRAME     0xF  /* op83: one frame of subtractive grey screen tint */
#define FX_THUNDARA_DARKEN_RGB       0x40, 0x40, 0x40
#define FX_THUNDARA_FLASH_FRAME      0x10 /* op83: additive blue-white screen flash, fading over */
#define FX_THUNDARA_FLASH_FRAMES     3    /* this many frames */
#define FX_THUNDARA_ORB_FRAME        0x14 /* op83: FX_THUNDARA_IMPACT_RING and FX_THUNDARA_ORB_EMITTER start */
#define FX_THUNDARA_FRAMES           0x30 /* op83: the task ends at this frame */
#define FX_THUNDARA_TARGET_BOLTS_FRAME 0x22 /* op83: from here one bolt per frame from each target up to the orb */
#define FX_THUNDARA_STRIP_POINTS  16   /* points of a bolt strip (fx_049_strips, fxm_drawJaggedStrip16) */
#define FX_THUNDARA_BRANCH_POINTS 4    /* points of a branch (fxm_drawJaggedStrip4) */
#define FX_THUNDARA_JITTER_ROWS   0x40 /* fx_049_jitter rows of three random offsets (along, across, angle) */
#define FX_THUNDARA_BOLT_SLOTS    0xC  /* pool items and fx_049_strips / fx_049_branches entries; strip10 = bolt count % slots */

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width) */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_THUNDARA_STRIP_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx049Strip; /* 0x84 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[8]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
    u8 pad44[4];
} Fx049Branch; /* 0x48 */

/* one bolt, a pool item */
typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 strip10;
    u8 state11; /* state */
    u8 branchAt12;
    u8 branchAt13;
    u8 kind14; /* strip kind: file block, and < FX_049_STRIP_FLAT screen-space */
    u8 width15;
    s16 pad16;
    s32 *jitter18; /* jitter table */
    Pool6CEntry *emit1C;
    u8 pad20[4]; /* unused: op83 asks for 0x24-byte items (fx_003's bolt has emit20 here) */
} Fx049Bolt; /* 0x24 */
#define FX_049_STRIP_FLAT 2 /* kind14 from here on: the strips draw flat (btlfx_drawTrail mode 2, else mode 0) */
#define FX_049_WIDTH_DEFAULT 0xFF /* width15 of the late bolts: drawn as -0x20, no screen tint */

typedef struct {
    SVECTOR pos0; /* target body */
    s32 *jitter8; /* jitter table */
    s32 boltCountC; /* bolts spawned */
} Fx049Work; /* 0x10 */

extern s16 fx_049_boltSchedule[]; /* per frame: 0 none or an FX_049_BOLT_* */
#define FX_049_BOLT_FLAT 1 /* scattered vertical bolt, flat strip */
#define FX_049_BOLT_STRIKE 2 /* from the target point, slanted, longer, screen-space strip */
#define FX_049_BOLT_PAIR 3 /* two short bolts */
extern s32 fx_049_jitter[];
extern Fx049Strip fx_049_strips[];
extern Fx049Branch fx_049_branches[];
extern u16 fx_049_strip16Blocks[];
extern u16 fx_049_strip4Blocks[];
extern Unk800F78A4 *fx_049_env;
extern Unk80100C78Entry *fx_049_task;

static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx049Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx049Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx049Strip *strip, Fx049Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind);
static s32 fx_049_op83PoolTask(s32 mode, Fx049Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
static void fx_049_op83ElemDraw(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *alpha, s32 blend, s32 frame, s32 n);

#endif
