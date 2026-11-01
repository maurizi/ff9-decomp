#ifndef FX_049_H
#define FX_049_H

/* Effect module fx_049 (dir 13 file 0x49): Thundara, all targets (and one large target). Lightning bolts drawn as
 * jagged strips (fxm_drawJaggedStrip16 / fxm_drawJaggedStrip4, fxm_drawJaggedBranch4). Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width) */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[16]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
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
} Fx049Bolt;
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
static void fx_049_op83ElemDraw(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *col, s32 blend, s32 t, s32 n);

#endif
