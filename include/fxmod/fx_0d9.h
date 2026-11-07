#ifndef FX_0D9_H
#define FX_0D9_H

/* Effect module fx_0d9 (dir 13 file 0xD9): Sword Magic Thundaga. Lightning bolts drawn as jagged strips
 * (fxm_drawJaggedStrip15 / fxm_drawJaggedStrip3, fxm_drawJaggedBranch3, fxm_jaggedStripTask). Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0d9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_THUNDAGA_DISC           0xD /* model: flat octagon on 15-bit texture page 0x119, which the file does not fill (drawn at run time) (byte-identical to fx_018 entry 0x10); runtime (docs/runtime-findings.md effect-data pass): as Thundaga's disc (fx_018 0x10) its page holds 4-bit lightning textures at run time, read as 15-bit noise (look pending) */
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
#define FX_SWDMAG_THUNDAGA_SPARKLE_SWARM  0x26 /* emitter: Thundaga's swarms of blue, lilac and teal twinkle dots (byte-identical to fx_018 entry 0x26); runtime (docs/runtime-findings.md effect-data pass) */
#define FX_SWDMAG_THUNDAGA_SPARK_DOTS 0x27 /* emitter: white and yellow dot sparks (byte-identical to fx_018 entry 0x27) */
#define FX_SWDMAG_THUNDAGA_SWORD_GLINT    0x28 /* rings: white 6-segment rings flashed at the sword (op84, first draw) */
#define FX_SWDMAG_THUNDAGA_SWORD_FLASH    0x29 /* rings: white 8-segment rings flashed rotating at the sword (op84) */

/* fx_0d9 choreography (docs/fxdata.md, "Choreography literals"; as fx_018 Thundaga): op80's states, their lengths,
 * keyframes and screen tints; op83's bolt kinds and lengths; the bolt pool items' lengths and widths; the strip
 * sizes; op84's sword flash. */
#define FX_SWDMAG_THUNDAGA_STATE_STRIKE 1 /* op80 (starts here, LIGHTNING_ARCS from init): a blue flash, THIN_BOLT, VIOLET_RING */
#define FX_SWDMAG_THUNDAGA_STATE_BLAST  2 /* op80: the bolts, rings, FX_SWDMAG_THUNDAGA_GLOW_DOME and the spark balls */
#define FX_SWDMAG_THUNDAGA_STRIKE_TINT_RGB   0x20, 0x40, 0xFF /* STRIKE frame 0: additive screen tint (blue) */
#define FX_SWDMAG_THUNDAGA_STRIKE_DARKEN_RGB 0x80, 0x80, 0xFF /* STRIKE's last frame: subtractive screen tint */
#define FX_SWDMAG_THUNDAGA_STRIKE_FRAMES      2  /* STRIKE's length; at its end LIGHTNING_ARCS_2 and SPARKLE_SWARM start */
#define FX_SWDMAG_THUNDAGA_BLAST_FRAMES       50 /* BLAST's length (the op ends); FX_SWDMAG_THUNDAGA_BLUE_RING spreads over it */
#define FX_SWDMAG_THUNDAGA_FLASH_FRAMES       6  /* BLAST: additive blue-white screen flash */
#define FX_SWDMAG_THUNDAGA_LIGHTNING_BOLT_FRAMES 20 /* BLAST: FX_SWDMAG_THUNDAGA_LIGHTNING_BOLT fades over this many frames */
#define FX_SWDMAG_THUNDAGA_THIN_BOLT_FRAMES   10 /* BLAST: FX_SWDMAG_THUNDAGA_THIN_BOLT fades over this many frames */
#define FX_SWDMAG_THUNDAGA_VIOLET_RING_FRAMES 24 /* BLAST: FX_SWDMAG_THUNDAGA_VIOLET_RING shrinks and fades */
#define FX_SWDMAG_THUNDAGA_WHITE_RING_FRAMES  16 /* BLAST: FX_SWDMAG_THUNDAGA_WHITE_RING spreads flat and fades */
#define FX_SWDMAG_THUNDAGA_GLOW_DOME_FRAMES   24 /* BLAST: FX_SWDMAG_THUNDAGA_GLOW_DOME and the spark balls around it */
#define FX_SWDMAG_THUNDAGA_SPARK_BALLS        16 /* BLAST: FX_SWDMAG_THUNDAGA_SPARK_BALL particles circling the dome */
#define FX_SWDMAG_THUNDAGA_SPARK_BALL_FRAME_MASK 7 /* the spark ball's 8 animation frames */
#define FX_SWDMAG_THUNDAGA_MODEL_COUNT        2  /* btlfx_initEffModels slots: GLOW_DOME, DISC */
#define FX_SWDMAG_THUNDAGA_GLOW_DOME_TPAGE 0xB8 /* Hi_RegisterTexEffModel texture: 8-bit page (0x200, 0x100), abr 1 */
#define FX_SWDMAG_THUNDAGA_GLOW_DOME_CLUT  0x3E40 /* CLUT at (0x0, 0xF9) */
#define FX_SWDMAG_THUNDAGA_GLOW_DOME_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_THUNDAGA_GLOW_DOME_V     0x80
#define FX_SWDMAG_THUNDAGA_DISC_TPAGE 0x119 /* Hi_RegisterTexEffModel texture: 15-bit page (0x240, 0x100), abr 0, run-time lightning textures (runtime pass) */
#define FX_SWDMAG_THUNDAGA_DISC_CLUT  0xFFFF /* no CLUT (15-bit page) */
#define FX_SWDMAG_THUNDAGA_DISC_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_THUNDAGA_DISC_V     0x80
#define FX_SWDMAG_THUNDAGA_BOLTS_CHARGE 0 /* op83 kind (its init arg): a flat bolt every other frame around the target */
#define FX_SWDMAG_THUNDAGA_BOLTS_BURST  1 /* op83 kind: a ring of bolts, then bolts reaching further out, screen flashes */
#define FX_SWDMAG_THUNDAGA_BOLTS_CHAIN  2 /* op83 kind: a chain of bolts zig-zagging around the target, plus spokes */
#define FX_SWDMAG_THUNDAGA_CHARGE_BOLTS_FRAMES 29 /* BOLTS_CHARGE's length */
#define FX_SWDMAG_THUNDAGA_BURST_RING_BOLTS    6  /* BOLTS_BURST frame 0: bolts in the ring */
#define FX_SWDMAG_THUNDAGA_BURST_OUTER_FRAME   6  /* BOLTS_BURST: two outer bolts per frame from here */
#define FX_SWDMAG_THUNDAGA_BURST_OUTER_END     16 /* up to this frame */
#define FX_SWDMAG_THUNDAGA_BURST_FLASH_FRAME_1 8  /* BOLTS_BURST: additive BURST_TINT on these three frames */
#define FX_SWDMAG_THUNDAGA_BURST_FLASH_FRAME_2 10
#define FX_SWDMAG_THUNDAGA_BURST_FLASH_FRAME_3 13
#define FX_SWDMAG_THUNDAGA_BURST_DARKEN_FRAME  9  /* BOLTS_BURST: subtractive BURST_DARKEN on this one */
#define FX_SWDMAG_THUNDAGA_BURST_TINT_RGB      0x10, 0x40, 0x80 /* additive (blue) */
#define FX_SWDMAG_THUNDAGA_BURST_DARKEN_RGB    0xFF, 0xFF, 0    /* subtractive (the screen turns blue) */
#define FX_SWDMAG_THUNDAGA_BURST_BOLTS_FRAMES  20 /* BOLTS_BURST's length */
#define FX_SWDMAG_THUNDAGA_CHAIN_BOLTS_FRAMES  31 /* BOLTS_CHAIN's length (one chain link per frame, growing reach) */
#define FX_SWDMAG_THUNDAGA_CHAIN_SPOKES_FRAMES 19 /* BOLTS_CHAIN: a flat spoke bolt per frame until this frame */
#define FX_SWDMAG_THUNDAGA_CHAIN_TINT_RGB      0x10, 0x20, 0x46 /* BOLTS_CHAIN: additive (blue), on random odd frames */
#define FX_SWDMAG_THUNDAGA_BOLT_HEIGHT         0xC00 /* a bolt comes down from this far above its strike point */
#define FX_SWDMAG_THUNDAGA_BOLT_FRAMES         8  /* pool item: a screen-space bolt shows for this many frames */
#define FX_SWDMAG_THUNDAGA_FAST_BOLT_FRAMES    6  /* (BOLTS_CHAIN: fx_0d9_fastBolts) */
#define FX_SWDMAG_THUNDAGA_FLAT_BOLT_FRAMES    6  /* pool item: a flat-strip bolt shows for this many frames */
#define FX_SWDMAG_THUNDAGA_FAST_FLAT_BOLT_FRAMES 4
#define FX_SWDMAG_THUNDAGA_BOLT_WIDTH          0x97 /* line width of a screen-space bolt at its first frame (+ 2 * width15) */
#define FX_SWDMAG_THUNDAGA_FLAT_BOLT_WIDTH     0xAA /* line width of a flat-strip bolt at its first frame */
#define FX_SWDMAG_THUNDAGA_STRIP_POINTS        15 /* points of a bolt strip (fxm_drawJaggedStrip15) */
#define FX_SWDMAG_THUNDAGA_BRANCH_POINTS       3  /* points of a branch (fxm_drawJaggedStrip3) */
#define FX_SWDMAG_THUNDAGA_JITTER_ROWS         0x40 /* fx_0d9_jitter rows of three random offsets (along, across, angle) */
#define FX_SWDMAG_THUNDAGA_BOLT_SLOTS          0x10 /* pool items and strips; strip10 = bolt count % slots */
#define FX_SWDMAG_THUNDAGA_SWORD_STATE_APPEAR  0 /* op84: SWORD_GLINT / SWORD_FLASH grow in at the weapon */
#define FX_SWDMAG_THUNDAGA_SWORD_STATE_FLICKER 1 /* op84: they flicker and turn */
#define FX_SWDMAG_THUNDAGA_SWORD_STATE_FADE    2 /* op84: they spread and fade (the op ends) */
#define FX_SWDMAG_THUNDAGA_SWORD_APPEAR_FRAMES  3 /* SWORD_STATE_APPEAR's length */
#define FX_SWDMAG_THUNDAGA_SWORD_FLICKER_FRAMES 9 /* SWORD_STATE_FLICKER's length */
#define FX_SWDMAG_THUNDAGA_SWORD_FADE_FRAMES    9 /* SWORD_STATE_FADE's length */

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
    SVECTOR pts4[FX_SWDMAG_THUNDAGA_STRIP_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
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
    SVECTOR pts4[FX_SWDMAG_THUNDAGA_BRANCH_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
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
    u8 pad1C[8]; /* unused: op83 asks for 0x24-byte items */
} Fx0d9Bolt; /* 0x24 */
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
