#ifndef FX_003_H
#define FX_003_H

/* Effect module fx_003 (dir 13 file 0x03): Thunder, one target (party and enemy). Lightning bolts drawn as jagged
 * strips (fxm_drawJaggedStrip16 / fxm_drawJaggedStrip4, fxm_drawJaggedBranch4). Evidence: [data] the party ability
 * / item tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md,
 * "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width) */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[16]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx003Strip; /* 0x84 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[8]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
    u8 pad44[4];
} Fx003Branch; /* 0x48 */

/* one bolt, a pool item */
typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 strip10; /* bolt index */
    u8 state11; /* state */
    u8 branchAt12; /* branch points */
    u8 branchAt13;
    s16 kind14; /* kind */
    s32 *jitter18; /* jitter table (fx_003_jitter) */
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
} Fx003Bolt; /* 0x24 */

typedef struct {
    SVECTOR pos0; /* target body position */
    s32 *jitter8; /* fx_003_jitter */
    s32 boltCountC; /* bolts spawned */
} Fx003Work; /* 0x10 */

extern u16 fx_003_strip16Blocks[];
extern u16 fx_003_strip4Blocks[];
extern Unk800F78A4 *fx_003_env;
extern Fx003Strip fx_003_strips[];
extern Fx003Branch fx_003_branches[];
extern s32 fx_003_jitter[];

static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx003Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx003Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx003Strip *strip, Fx003Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
static s32 fx_003_op83PoolTask(s32 mode, Fx003Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
