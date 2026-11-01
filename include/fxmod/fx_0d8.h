#ifndef FX_0D8_H
#define FX_0D8_H

/* Effect module fx_0d8 (dir 13 file 0xD8): Sword Magic Thundara. Lightning bolts drawn as jagged strips
 * (fxm_drawJaggedStrip16 / fxm_drawJaggedStrip4, fxm_drawJaggedBranch4). Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d8Work84;

extern Unk80100C78Entry *fx_0d8_task;

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
    u8 strip10; /* index */
    u8 state11; /* state */
    u8 branchAt12;
    u8 branchAt13;
    u8 kind14; /* strip kind: file block, and < FX_0D8_STRIP_FLAT screen-space */
    u8 width15;
    s16 unk16; /* unused */
    s32 *jitter18;
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
} Fx0d8Part; /* pool entry data */
#define FX_0D8_STRIP_FLAT 2 /* kind14 from here on: the strips draw flat (btlfx_drawTrail mode 2, else mode 0) */

/* fx_003 copies (asm) */
static s32 fx_0d8_op83PoolTask(s32 mode, Fx0d8Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

static void fx_0d8_op83ElemDraw(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *arg5, s32 arg6,
                   s32 frame, s32 n);
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[16]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0d8Strip; /* 0x84 */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[8]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
    u8 pad44[4];
} Fx0d8Branch; /* 0x48 */
extern Fx0d8Strip fx_0d8_strips[];
extern Fx0d8Branch fx_0d8_branches[];
extern u16 fx_0d8_strip16Blocks[];
static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx0d8Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 fx_0d8_strip4Blocks[];
extern Unk800F78A4 *fx_0d8_env;
static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx0d8Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx0d8Strip *strip, Fx0d8Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR pos8; /* caster position */
    s32 *jitter10; /* jitter table (fx_0d8_jitter) */
    s32 boltCount14; /* bolts spawned */
} Fx0d8Work83; /* 0x18 */

extern s16 fx_0d8_spawnKinds[16]; /* bolt kind per frame */
extern s32 fx_0d8_jitter[];
s32 fx_0d8_op83(s32 mode, Fx0d8Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
