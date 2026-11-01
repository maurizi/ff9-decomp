#ifndef FX_042_H
#define FX_042_H

/* Effect module fx_042 (dir 13 file 0x42): Thunder, all targets (and one large target). Lightning bolts drawn as
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
} Fx042Strip; /* 0x84 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[8]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
    u8 pad44[4];
} Fx042Branch; /* 0x48 */

/* one bolt, a pool item */
typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 strip10; /* bolt index */
    u8 state11; /* state */
    u8 branchAt12; /* branch points */
    u8 branchAt13;
    s16 kind14; /* strip kind: file block, and < FX_042_STRIP_FLAT screen-space */
    s32 *jitter18; /* jitter table */
    Pool6CEntry *emit1C;
} Fx042Bolt;
#define FX_042_STRIP_FLAT 2 /* kind14 from here on: the strips draw flat (btlfx_drawTrail mode 2, else mode 0) */

typedef struct {
    SVECTOR pos0; /* origin */
    s32 *jitter8; /* jitter table */
    s32 boltCountC; /* bolts spawned */
} Fx042Work; /* 0x10 */

extern s32 fx_042_jitter[];
extern u16 fx_042_strip16Blocks[];
extern u16 fx_042_strip4Blocks[];
extern Unk800F78A4 *fx_042_env;
extern Fx042Strip fx_042_strips[];
extern Fx042Branch fx_042_branches[];

static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx042Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx042Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx042Strip *strip, Fx042Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind);
static s32 fx_042_op83PoolTask(s32 mode, Fx042Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
