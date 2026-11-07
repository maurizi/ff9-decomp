#ifndef FX_018_H
#define FX_018_H

/* Effect module fx_018 (dir 13 file 0x18): Thundaga, one target (party and enemy). Lightning bolts drawn as jagged
 * strips (fxm_drawJaggedStrip15 / fxm_drawJaggedStrip3, fxm_drawJaggedBranch3, fxm_jaggedStripTask). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_018 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THUNDAGA_VIOLET_GLOW    0x7 /* emitter: soft violet-magenta glow puffs, the violet light around the thin bolts; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_THUNDAGA_VIOLET_GLOW_2  0xA /* emitter: the same violet-magenta glow puffs, started later; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_THUNDAGA_DISC           0x10 /* model: flat octagon, 15-bit texture page 0x119 holds the file's 4-bit pages, so it samples something drawn there at run time; runtime (docs/runtime-findings.md effect-data pass): its page (0x240,0x100) holds 4-bit lightning textures at run time, not a screen copy; ablating the model changes nothing visible (look pending) */
#define FX_THUNDAGA_GLOW_DOME      0x11 /* model: hemisphere with an orange-to-white streak texture */
#define FX_THUNDAGA_BLUE_RING      0x13 /* rings: white-blue gouraud rings */
#define FX_THUNDAGA_WHITE_RING     0x15 /* rings: white gouraud rings drawn flat */
#define FX_THUNDAGA_LIGHTNING_BOLT 0x17 /* particles: wide blue-white lightning bolt */
#define FX_THUNDAGA_SPARK_BALL     0x1B /* particles: golden plasma ball, 8 frames */
#define FX_THUNDAGA_VIOLET_RING    0x1E /* rings: white-violet gouraud rings */
#define FX_THUNDAGA_THIN_BOLT      0x20 /* particles: thin cyan lightning bolt */
#define FX_THUNDAGA_LIGHTNING_ARCS 0x21 /* emitter: four fans drawn as cyan and blue zig-zag lightning arcs */
#define FX_THUNDAGA_LIGHTNING_ARCS_2 0x22 /* emitter: four fans drawn as cyan and blue zig-zag lightning arcs */
#define FX_THUNDAGA_IMPACT_FLASH   0x25 /* emitter: four-point star flash */
#define FX_THUNDAGA_SPARKLE_SWARM  0x26 /* emitter: swarms of blue, lilac and teal twinkle dots, the cyan sparkles around the strike; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_THUNDAGA_SPARK_DOTS     0x27 /* emitter: white spark dots (btlfx_emitDots) */

/* fx_018 choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths, keyframes and screen
 * tints; op83's bolt kinds and lengths; the bolt pool items' lengths and widths; the strip sizes. */
#define FX_THUNDAGA_STATE_CHARGE 0 /* op80: FX_THUNDAGA_VIOLET_GLOW(_2) gather at the target */
#define FX_THUNDAGA_STATE_STRIKE 1 /* op80: FX_THUNDAGA_LIGHTNING_ARCS, a blue flash and FX_THUNDAGA_THIN_BOLT */
#define FX_THUNDAGA_STATE_BLAST  2 /* op80: the bolts, rings, FX_THUNDAGA_GLOW_DOME and the spark balls */
#define FX_THUNDAGA_VIOLET_GLOW_2_FRAME 5    /* CHARGE: FX_THUNDAGA_VIOLET_GLOW_2 starts */
#define FX_THUNDAGA_CHARGE_FRAMES      29    /* CHARGE's length; one frame before its end a subtractive DARKEN tint, at the end DARKEN_2 */
#define FX_THUNDAGA_DARKEN_RGB         0x80, 0x40, 0    /* subtractive screen tint (the screen turns blue) */
#define FX_THUNDAGA_DARKEN_2_RGB       0x80, 0x80, 0x80 /* subtractive screen tint (grey: the screen darkens) */
#define FX_THUNDAGA_STRIKE_TINT_RGB    0x20, 0x40, 0xFF /* STRIKE frame 0: additive screen tint (blue) */
#define FX_THUNDAGA_STRIKE_DARKEN_RGB  0x80, 0x80, 0xFF /* STRIKE's last frame: subtractive screen tint */
#define FX_THUNDAGA_STRIKE_FRAMES      2     /* STRIKE's length; at its end LIGHTNING_ARCS_2 and SPARKLE_SWARM start */
#define FX_THUNDAGA_BLAST_FRAMES       50    /* BLAST's length (the op ends); FX_THUNDAGA_BLUE_RING spreads over it */
#define FX_THUNDAGA_FLASH_FRAMES       6     /* BLAST: additive blue-white screen flash */
#define FX_THUNDAGA_LIGHTNING_BOLT_FRAMES 20 /* BLAST: FX_THUNDAGA_LIGHTNING_BOLT fades over this many frames */
#define FX_THUNDAGA_THIN_BOLT_FRAMES   10    /* BLAST: FX_THUNDAGA_THIN_BOLT fades over this many frames */
#define FX_THUNDAGA_VIOLET_RING_FRAMES 24    /* BLAST: FX_THUNDAGA_VIOLET_RING shrinks and fades */
#define FX_THUNDAGA_WHITE_RING_FRAMES  16    /* BLAST: FX_THUNDAGA_WHITE_RING spreads flat and fades */
#define FX_THUNDAGA_GLOW_DOME_FRAMES   24    /* BLAST: FX_THUNDAGA_GLOW_DOME and the spark balls around it */
#define FX_THUNDAGA_SPARK_BALLS        16    /* BLAST: FX_THUNDAGA_SPARK_BALL particles circling the dome */
#define FX_THUNDAGA_SPARK_BALL_FRAME_MASK 7  /* the spark ball's 8 animation frames */
#define FX_THUNDAGA_MODEL_COUNT        2     /* btlfx_initEffModels slots: GLOW_DOME, DISC */
#define FX_THUNDAGA_BOLTS_CHARGE 0 /* op83 kind (its init arg): a flat bolt every other frame around the target */
#define FX_THUNDAGA_BOLTS_BURST  1 /* op83 kind: a ring of bolts, then bolts reaching further out, screen flashes */
#define FX_THUNDAGA_BOLTS_CHAIN  2 /* op83 kind: a chain of bolts zig-zagging around the target, plus spokes */
#define FX_THUNDAGA_CHARGE_BOLTS_FRAMES 29   /* BOLTS_CHARGE's length */
#define FX_THUNDAGA_BURST_RING_BOLTS   6     /* BOLTS_BURST frame 0: bolts in the ring */
#define FX_THUNDAGA_BURST_OUTER_FRAME  6     /* BOLTS_BURST: two outer bolts per frame from here */
#define FX_THUNDAGA_BURST_OUTER_END    16    /* up to this frame */
#define FX_THUNDAGA_BURST_FLASH_FRAME_1 8    /* BOLTS_BURST: additive BURST_TINT on these three frames */
#define FX_THUNDAGA_BURST_FLASH_FRAME_2 10
#define FX_THUNDAGA_BURST_FLASH_FRAME_3 13
#define FX_THUNDAGA_BURST_DARKEN_FRAME 9     /* BOLTS_BURST: subtractive BURST_DARKEN on this one */
#define FX_THUNDAGA_BURST_TINT_RGB     0x10, 0x40, 0x80 /* additive (blue) */
#define FX_THUNDAGA_BURST_DARKEN_RGB   0xFF, 0xFF, 0    /* subtractive (the screen turns blue) */
#define FX_THUNDAGA_BURST_BOLTS_FRAMES 20    /* BOLTS_BURST's length */
#define FX_THUNDAGA_CHAIN_BOLTS_FRAMES 31    /* BOLTS_CHAIN's length (one chain link per frame, growing reach) */
#define FX_THUNDAGA_CHAIN_SPOKES_FRAMES 19   /* BOLTS_CHAIN: a flat spoke bolt per frame until this frame */
#define FX_THUNDAGA_CHAIN_TINT_RGB     0x10, 0x20, 0x46 /* BOLTS_CHAIN: additive (blue), on random odd frames */
#define FX_THUNDAGA_BOLT_HEIGHT        0xC00 /* a bolt comes down from this far above its strike point */
#define FX_THUNDAGA_BOLT_FRAMES        8     /* pool item: a screen-space bolt shows for this many frames */
#define FX_THUNDAGA_FAST_BOLT_FRAMES   6     /* (BOLTS_CHAIN: fx_018_fastBolts) */
#define FX_THUNDAGA_FLAT_BOLT_FRAMES   6     /* pool item: a flat-strip bolt shows for this many frames */
#define FX_THUNDAGA_FAST_FLAT_BOLT_FRAMES 4
#define FX_THUNDAGA_BOLT_WIDTH         0x97  /* line width of a screen-space bolt at its first frame (+ 2 * width15) */
#define FX_THUNDAGA_FLAT_BOLT_WIDTH    0xAA  /* line width of a flat-strip bolt at its first frame */
#define FX_THUNDAGA_STRIP_POINTS       15    /* points of a bolt strip (fxm_drawJaggedStrip15) */
#define FX_THUNDAGA_BRANCH_POINTS      3     /* points of a branch (fxm_drawJaggedStrip3) */
#define FX_THUNDAGA_JITTER_ROWS        0x40  /* fx_018_jitter rows of three random offsets (along, across, angle) */
#define FX_THUNDAGA_BOLT_SLOTS         0x10  /* pool items and strips; strip10 = bolt count % slots */

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width) */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_THUNDAGA_STRIP_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
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
    u8 pad1C[8]; /* unused: op83 asks for 0x24-byte items */
} Fx018Bolt; /* 0x24 */
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
