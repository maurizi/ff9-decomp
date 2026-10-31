#ifndef FX_005_H
#define FX_005_H

/* Effect module fx_005 (dir 13 file 0x05): Thundara, one target (party and enemy). Lightning bolts drawn as jagged
 * strips (fxm_drawJaggedStrip16 / fxm_drawJaggedStrip4, fxm_drawJaggedBranch4). Evidence: [data] the party ability
 * / item tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md,
 * "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width) */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[16];
} Fx005Strip; /* 0x84 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[8];
    u8 pad44[4];
} Fx005Branch; /* 0x48 */

/* one bolt, a pool item */
typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 strip10;
    u8 state11; /* state */
    u8 branchAt12;
    u8 branchAt13;
    u8 kind14; /* kind */
    u8 width15;
    s16 pad16;
    s32 *jitter18; /* jitter table */
    Pool6CEntry *emit1C;
} Fx005Bolt;

typedef struct {
    SVECTOR pos0; /* target body */
    SVECTOR pos8; /* target */
    s32 *jitter10; /* jitter table */
    s32 boltCount14; /* bolts spawned */
} Fx005Work; /* 0x18 */

extern s16 fx_005_boltSchedule[];
extern Fx005Strip fx_005_strips[];
extern Fx005Branch fx_005_branches[];
extern s32 fx_005_jitter[];
extern u16 fx_005_strip16Blocks[];
extern u16 fx_005_strip4Blocks[];
extern Unk800F78A4 *fx_005_env;
extern Unk80100C78Entry *fx_005_task;

static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx005Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx005Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx005Strip *strip, Fx005Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
static s32 fx_005_op83PoolTask(s32 mode, Fx005Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
static void fx_005_op83ElemDraw(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *col, s32 arg6, s32 t, s32 n);

#endif
