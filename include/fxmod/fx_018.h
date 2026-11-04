#ifndef FX_018_H
#define FX_018_H

/* Effect module fx_018 (dir 13 file 0x18): Thundaga, one target (party and enemy). Lightning bolts drawn as jagged
 * strips (fxm_drawJaggedStrip15 / fxm_drawJaggedStrip3, fxm_drawJaggedBranch3, fxm_jaggedStripTask). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_018 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THUNDAGA_CYL_EMITTER    0x7 /* emitter: cylinder particles, render all transparent (look pending) */
#define FX_THUNDAGA_CYL_EMITTER_2  0xA /* emitter: cylinder particles, render all transparent (look pending) */
#define FX_THUNDAGA_DISC           0x10 /* model: flat octagon, 15-bit texture page 0x119 (look pending) */
#define FX_THUNDAGA_GLOW_DOME      0x11 /* model: hemisphere with an orange-to-white streak texture */
#define FX_THUNDAGA_BLUE_RING      0x13 /* rings: white-blue gouraud rings */
#define FX_THUNDAGA_WHITE_RING     0x15 /* rings: white gouraud rings drawn flat */
#define FX_THUNDAGA_LIGHTNING_BOLT 0x17 /* particles: wide blue-white lightning bolt */
#define FX_THUNDAGA_SPARK_BALL     0x1B /* particles: golden plasma ball, 8 frames */
#define FX_THUNDAGA_VIOLET_RING    0x1E /* rings: white-violet gouraud rings */
#define FX_THUNDAGA_THIN_BOLT      0x20 /* particles: thin cyan lightning bolt */
#define FX_THUNDAGA_FAN_EMITTER    0x21 /* emitter: four fan elements (not rendered) (look pending) */
#define FX_THUNDAGA_FAN_EMITTER_2  0x22 /* emitter: four fan elements (not rendered) (look pending) */
#define FX_THUNDAGA_IMPACT_FLASH   0x25 /* emitter: four-point star flash */
#define FX_THUNDAGA_CONE_EMITTER   0x26 /* emitter: cone of sprites, render all transparent (look pending) */
#define FX_THUNDAGA_SPARKS         0x27 /* emitter: spark sprites (texture not in the file) (look pending) */

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width) */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[15]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx018Strip; /* 0x7C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[3]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
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
    u8 kind14; /* strip kind: file block, and < FX_018_STRIP_FLAT screen-space */
    s8 width15; /* extra width */
    u8 pad16[2];
    s32 *jitter18; /* jitter table */
} Fx018Bolt;
#define FX_018_STRIP_FLAT 2 /* kind14 from here on: the strips draw flat (btlfx_drawTrail mode 2, else mode 0) */

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
    s32 kind18; /* kind: the three bolt stages 0, 1, 2 of one cast (op 0x83 byte a in the file 0x18 script) */
} Fx018Work83; /* 0x1C */

extern BtlModelBuf fx_018_effModels[];
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

static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedStrip3(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx018Strip *strip, Fx018Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind);
static s32 fxm_jaggedStripTask(s32 mode, Fx018Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
static s32 fxm_randStripPoint(SVECTOR *out, SVECTOR *dir, s32 frame, s32 n);

#endif
