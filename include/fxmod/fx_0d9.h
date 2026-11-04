#ifndef FX_0D9_H
#define FX_0D9_H

/* Effect module fx_0d9 (dir 13 file 0xD9): Sword Magic Thundaga. Lightning bolts drawn as jagged strips
 * (fxm_drawJaggedStrip15 / fxm_drawJaggedStrip3, fxm_drawJaggedBranch3, fxm_jaggedStripTask). Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0d9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_THUNDAGA_DISC           0xD /* model: flat octagon on 15-bit texture page 0x119, which the file does not fill (drawn at run time) (byte-identical to fx_018 entry 0x10) (look pending) */
#define FX_SWDMAG_THUNDAGA_GLOW_DOME      0xE /* model: hemisphere with an orange-to-white streak texture (byte-identical to fx_018 entry 0x11) */
#define FX_SWDMAG_THUNDAGA_BLUE_RING      0x10 /* rings: white-blue gouraud rings */
#define FX_SWDMAG_THUNDAGA_WHITE_RING     0x12 /* rings: white gouraud rings drawn flat */
#define FX_SWDMAG_THUNDAGA_LIGHTNING_BOLT 0x14 /* particles: wide blue-white lightning bolt (byte-identical to fx_018 entry 0x17) */
#define FX_SWDMAG_THUNDAGA_SPARK_BALL     0x18 /* particles: golden plasma ball, 8 frames (byte-identical to fx_018 entry 0x1B) */
#define FX_SWDMAG_THUNDAGA_VIOLET_RING    0x1E /* rings: white-violet gouraud rings */
#define FX_SWDMAG_THUNDAGA_THIN_BOLT      0x20 /* particles: thin cyan lightning bolt (byte-identical to fx_018 entry 0x20) */
#define FX_SWDMAG_THUNDAGA_LIGHTNING_ARCS 0x21 /* emitter: four fans of cyan / blue zigzag lightning arcs (byte-identical to fx_018 entry 0x21) */
#define FX_SWDMAG_THUNDAGA_LIGHTNING_ARCS_2 0x22 /* emitter: four fans of cyan / blue zigzag lightning arcs (byte-identical to fx_018 entry 0x22) */
#define FX_SWDMAG_THUNDAGA_IMPACT_FLASH   0x25 /* emitter: four-point star flash (byte-identical to fx_018 entry 0x25) */
#define FX_SWDMAG_THUNDAGA_CONE_EMITTER   0x26 /* emitter: cone of sprites; the render is still all transparent (byte-identical to fx_018 entry 0x26) (look pending) */
#define FX_SWDMAG_THUNDAGA_SPARK_DOTS 0x27 /* emitter: white and yellow dot sparks (byte-identical to fx_018 entry 0x27) */
#define FX_SWDMAG_THUNDAGA_SWORD_GLINT    0x28 /* rings: white 6-segment rings flashed at the sword (op84, first draw) */
#define FX_SWDMAG_THUNDAGA_SWORD_FLASH    0x29 /* rings: white 8-segment rings flashed rotating at the sword (op84) */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d9Work84;

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width); as Fx018Strip */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[15]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0d9Strip; /* 0x7C */
extern u16 fx_0d9_strip15Blocks[];
static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
extern u16 fx_0d9_strip3Blocks[];
static void fxm_drawJaggedStrip3(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx0d9Strip *strip, Fx0d9Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind);
extern Unk800F78A4 *fx_0d9_env;
extern SVECTOR fx_0d9_stripPoints[]; /* strips[0].pts4: the strips start 4 bytes earlier */
#define FX_0D9_STRIPS ((Fx0d9Strip *)((u8 *)fx_0d9_stripPoints - 4))
static s32 fxm_randStripPoint(SVECTOR *out, SVECTOR *dir, s32 frame, s32 n);

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[3]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
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
    u8 kind14; /* strip kind: file block, and < FX_0D9_STRIP_FLAT screen-space */
    s8 width15; /* extra width */
    u8 pad16[2];
    s32 *jitter18; /* jitter table */
} Fx0d9Bolt;
#define FX_0D9_STRIP_FLAT 2 /* kind14 from here on: the strips draw flat (btlfx_drawTrail mode 2, else mode 0) */

extern Fx0d9Strip fx_0d9_strips[];
extern Fx0d9Branch fx_0d9_branches[];
extern s32 fx_0d9_fastBolts; /* faster bolts */

static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx0d9Strip *strip, Fx0d9Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
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

extern BtlModelBuf fx_0d9_effModels[];
extern s32 fx_0d9_ringRadius;
#endif
