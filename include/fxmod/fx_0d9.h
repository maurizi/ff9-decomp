#ifndef FX_0D9_H
#define FX_0D9_H

/* Effect module fx_0d9 (dir 13 file 0xD9): Sword Magic Thundaga. Lightning bolts drawn as jagged strips
 * (fxm_drawJaggedStrip15 / fxm_drawJaggedStrip3, fxm_drawJaggedBranch3, fxm_jaggedStripTask). Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d9Work84;

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width); as Fx018Strip */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx0d9Strip; /* 0x7C */
extern u16 fx_0d9_strip15Blocks[];
static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 fx_0d9_strip3Blocks[];
static void fxm_drawJaggedStrip3(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx0d9Strip *strip, Fx0d9Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *fx_0d9_env;
extern SVECTOR fx_0d9_stripPoints[]; /* strips[0].unk4: the strips start 4 bytes earlier */
#define FX_0D9_STRIPS ((Fx0d9Strip *)((u8 *)fx_0d9_stripPoints - 4))
static s32 fxm_randStripPoint(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[3];
    u8 pad1C[0x1C];
} Fx0d9Branch; /* 0x38 */

/* one bolt, a pool item; as Fx018Bolt */
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
} Fx0d9Bolt;

extern Fx0d9Strip fx_0d9_strips[];
extern Fx0d9Branch fx_0d9_branches[];
extern s32 fx_0d9_fastBolts; /* faster bolts */

static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx0d9Strip *strip, Fx0d9Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
static s32 fxm_jaggedStripTask(s32 mode, Fx0d9Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

/* propagated */

typedef struct {
    SVECTOR pos0; /* target body */
    SVECTOR pos8; /* target */
    s32 *jitter10; /* jitter table */
    s32 boltCount14; /* bolts spawned */
    s32 kind18; /* kind */
} Fx0d9Work83; /* 0x1C */
extern SVECTOR fx_0d9_prevBoltEnd; /* previous bolt end */
extern Unk80100C78Entry *fx_0d9_task;
extern s32 fx_0d9_jitter[];
s32 fx_0d9_op83(s32 mode, Fx0d9Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR pos8; /* caster position */
    s32 state10; /* state */
    s32 model14; /* model */
    s32 model18; /* model */
    Pool6CEntry *emit1C;
    s32 pad20;
} Fx0d9Work80; /* 0x24 */

extern BtlModel fx_0d9_effModels[];
extern s32 fx_0d9_ringRadius;
#endif
