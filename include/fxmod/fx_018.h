#ifndef FX_018_H
#define FX_018_H

/* Effect module fx_018 (dir 13 file 0x18): Thundaga, one target (party and enemy). Lightning bolts drawn as jagged
 * strips (fxm_drawJaggedStrip15 / fxm_drawJaggedStrip3, fxm_drawJaggedBranch3, fxm_jaggedStripTask). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width) */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx018Strip; /* 0x7C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[3];
    u8 pad1C[0x1C];
} Fx018Branch; /* 0x38 */

/* one bolt, a pool item */
typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 strip10; /* bolt index */
    u8 state11; /* state */
    u8 branchAt12; /* branch points */
    u8 branchAt13;
    u8 kind14; /* kind */
    s8 width15; /* extra width */
    u8 pad16[2];
    s32 *jitter18; /* jitter table */
} Fx018Bolt;

typedef struct {
    SVECTOR pos0; /* target body */
    SVECTOR pos8; /* target */
    s32 state10; /* state */
    s32 model14; /* models */
    s32 model18;
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
} Fx018Work80; /* 0x24 */

typedef struct {
    SVECTOR pos0; /* target body */
    SVECTOR pos8; /* target */
    s32 *jitter10; /* jitter table */
    s32 boltCount14; /* bolts spawned */
    s32 kind18; /* kind */
} Fx018Work83; /* 0x1C */

extern BtlModel fx_018_effModels[];
extern SVECTOR fx_018_prevBoltEnd; /* previous bolt end */
extern Unk80100C78Entry *fx_018_task;
extern s32 fx_018_jitter[];
extern s32 fx_018_ringRadius; /* ring radius */
extern u16 fx_018_strip15Blocks[];
extern u16 fx_018_strip3Blocks[];
extern Unk800F78A4 *fx_018_env;
extern Fx018Strip fx_018_strips[];
extern Fx018Branch fx_018_branches[];
extern s32 fx_018_fastBolts; /* faster bolts */

static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
static void fxm_drawJaggedStrip3(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx018Strip *strip, Fx018Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
static s32 fxm_jaggedStripTask(s32 mode, Fx018Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
static s32 fxm_randStripPoint(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

#endif
