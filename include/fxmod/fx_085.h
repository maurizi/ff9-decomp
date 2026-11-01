#ifndef FX_085_H
#define FX_085_H

/* Effect module fx_085 (dir 13 file 0x85): Thundaga, all targets (and one large target). Lightning bolts drawn as
 * jagged strips (fxm_drawJaggedStrip15 / fxm_drawJaggedStrip3, fxm_drawJaggedBranch3, fxm_jaggedStripTask).
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width); as Fx018Strip */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[15];
} Fx085Strip; /* 0x7C */
extern u16 fx_085_strip15Blocks[];
static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 fx_085_strip3Blocks[];
static void fxm_drawJaggedStrip3(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx085Strip *strip, Fx085Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *fx_085_env;
extern SVECTOR fx_085_stripPoints[]; /* strips[0].pts4: the strips start 4 bytes earlier */
#define FX_085_STRIPS ((Fx085Strip *)((u8 *)fx_085_stripPoints - 4))
static s32 fxm_randStripPoint(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[3];
    u8 pad1C[0x1C];
} Fx085Branch; /* 0x38 */

/* one bolt, a pool item; as Fx018Bolt */
typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 strip10; /* bolt index */
    u8 state11; /* state */
    u8 branchAt12; /* branch points */
    u8 branchAt13;
    u8 kind14; /* kind */
    s8 extraWidth15; /* extra width */
    u8 pad16[2];
    s32 *jitter18; /* jitter table */
} Fx085Bolt;

extern Fx085Strip fx_085_strips[];
extern Fx085Branch fx_085_branches[];
extern s32 fx_085_fastBolts; /* faster bolts */

static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx085Strip *strip, Fx085Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
static s32 fxm_jaggedStripTask(s32 mode, Fx085Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

typedef struct {
    SVECTOR pos0; /* target body */
    SVECTOR pos8; /* target */
    s32 *jitter10; /* jitter table */
    s32 boltCount14; /* bolts spawned */
    s32 kind18; /* kind */
} Fx085Work83; /* 0x1C */

extern SVECTOR fx_085_prevBoltEnd; /* previous bolt end */
extern Unk80100C78Entry *fx_085_task;
extern s32 fx_085_jitter[];

typedef struct {
    SVECTOR pos0; /* target position */
    SVECTOR pos8; /* ground position */
    s32 state10; /* state */
    s32 model14; /* model */
    s32 model18; /* model */
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
} Fx085Work80; /* 0x24 */

extern BtlModelBuf fx_085_effModels[];
extern s32 fx_085_ringRadius;
#endif
