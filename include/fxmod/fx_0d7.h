#ifndef FX_0D7_H
#define FX_0D7_H

/* Effect module fx_0d7 (dir 13 file 0xD7): Sword Magic Thunder. Lightning bolts drawn as jagged strips
 * (fxm_drawJaggedStrip16 / fxm_drawJaggedStrip4, fxm_drawJaggedBranch4). Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d7Work84;


/* fx_003 copies (asm) */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 unk3;
    SVECTOR unk4[16];
} Fx0d7Strip; /* 0x84 */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 unk3;
    SVECTOR unk4[8];
    u8 pad44[4];
} Fx0d7Branch; /* 0x48 */
extern Fx0d7Strip fx_0d7_strips[];
extern Fx0d7Branch fx_0d7_branches[];
typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 strip10; /* bolt index */
    u8 state11; /* state */
    u8 branchAt12; /* branch points */
    u8 branchAt13;
    s16 kind14; /* kind */
    s32 *jitter18; /* jitter table (fx_0d7_jitter) */
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
} Fx0d7Bolt; /* 0x24 */
typedef struct {
    SVECTOR pos0; /* target body position */
    s32 *jitter8; /* fx_0d7_jitter */
    s32 boltCountC; /* bolts spawned */
} Fx0d7Work; /* 0x10 */
extern u16 fx_0d7_strip16Blocks[];
static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx0d7Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 fx_0d7_strip4Blocks[];
static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx0d7Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx0d7Strip *strip, Fx0d7Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *fx_0d7_env;
extern s32 fx_0d7_jitter[];
static s32 fx_0d7_op83PoolTask(s32 mode, Fx0d7Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
