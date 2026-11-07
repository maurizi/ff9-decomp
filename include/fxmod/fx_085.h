#ifndef FX_085_H
#define FX_085_H

/* Effect module fx_085 (dir 13 file 0x85): Thundaga, all targets (and one large target). Lightning bolts drawn as
 * jagged strips (fxm_drawJaggedStrip15 / fxm_drawJaggedStrip3, fxm_drawJaggedBranch3, fxm_jaggedStripTask).
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_085 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THUNDAGA_IMPACT_FLASH   0x1 /* emitter: four-point star flash (byte-identical to fx_018 entry 0x25) */
#define FX_THUNDAGA_VIOLET_RING    0x2 /* rings: white-violet gouraud rings (same call site as fx_018 entry 0x1e) */
#define FX_THUNDAGA_GLOW_DOME      0x6 /* model: hemisphere with an orange-to-white streak texture (byte-identical to fx_018 entry 0x11) */
#define FX_THUNDAGA_SPARKLE_SWARM  0x8 /* emitter: swarms of blue, lilac and teal twinkle dots, the cyan sparkles around the strike (byte-identical to fx_018 entry 0x26); runtime (docs/runtime-findings.md effect-data pass) */
#define FX_THUNDAGA_SPARK_DOTS     0xE /* emitter: white spark dots (btlfx_emitDots) (byte-identical to fx_018 entry 0x27) */
#define FX_THUNDAGA_DISC           0xF /* model: flat octagon, 15-bit texture page 0x119 holds the file's 4-bit pages, so it samples something drawn there at run time (byte-identical to fx_018 entry 0x10); runtime (docs/runtime-findings.md effect-data pass): same entry as fx_018 0x10, whose page holds 4-bit lightning textures at run time; ablating it there changes nothing visible (look pending) */
#define FX_THUNDAGA_LIGHTNING_BOLT 0x10 /* particles: wide blue-white lightning bolt (byte-identical to fx_018 entry 0x17) */
#define FX_THUNDAGA_THIN_BOLT      0x11 /* particles: thin cyan lightning bolt (byte-identical to fx_018 entry 0x20) */
#define FX_THUNDAGA_LIGHTNING_ARCS 0x15 /* emitter: four fans drawn as cyan and blue zig-zag lightning arcs (byte-identical to fx_018 entry 0x21) */
#define FX_THUNDAGA_SPARK_BALL     0x18 /* particles: golden plasma ball, 8 frames (byte-identical to fx_018 entry 0x1b) */
#define FX_THUNDAGA_VIOLET_GLOW    0x1B /* emitter: soft violet-magenta glow puffs, the violet light around the thin bolts (byte-identical to fx_018 entry 0x7); runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_THUNDAGA_VIOLET_GLOW_2  0x1C /* emitter: the same violet-magenta glow puffs, started later (byte-identical to fx_018 entry 0xa); runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_THUNDAGA_LIGHTNING_ARCS_2 0x22 /* emitter: four fans drawn as cyan and blue zig-zag lightning arcs (byte-identical to fx_018 entry 0x22) */
#define FX_THUNDAGA_BLUE_RING      0x25 /* rings: white-blue gouraud rings (same call site as fx_018 entry 0x13) */
#define FX_THUNDAGA_WHITE_RING     0x26 /* rings: white gouraud rings drawn flat (same call site as fx_018 entry 0x15) */

/* fx_085 choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths, keyframes and screen
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
#define FX_THUNDAGA_JITTER_ROWS        0x40  /* fx_085_jitter rows of three random offsets (along, across, angle) */
#define FX_THUNDAGA_BOLT_SLOTS         0x10  /* pool items and strips; strip10 = bolt count % slots */
#define FX_THUNDAGA_BODY_HEIGHT        0x200 /* pos0 (strike flash, bolt bases) is this far above the targets' centre */

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width); as Fx018Strip */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_THUNDAGA_STRIP_POINTS];
} Fx085Strip; /* 0x7C */
extern u16 fx_085_strip15Blocks[];
static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
extern u16 fx_085_strip3Blocks[];
static void fxm_drawJaggedStrip3(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx085Strip *strip, Fx085Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind);
extern Unk800F78A4 *fx_085_env;
extern SVECTOR fx_085_stripPoints[]; /* strips[0].pts4: the strips start 4 bytes earlier */
#define FX_085_STRIPS ((Fx085Strip *)((u8 *)fx_085_stripPoints - 4))
static s32 fxm_randStripPoint(SVECTOR *out, SVECTOR *dir, s32 frame, s32 n);

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
    u8 kind14; /* strip kind: file block, and < FX_085_STRIP_FLAT screen-space */
    s8 extraWidth15; /* extra width */
    u8 pad16[2];
    s32 *jitter18; /* jitter table */
    u8 pad1C[8]; /* unused: op83 asks for 0x24-byte items */
} Fx085Bolt; /* 0x24 */
#define FX_085_STRIP_FLAT 2 /* kind14 from here on: the strips draw flat (btlfx_drawTrail mode 2, else mode 0) */

extern Fx085Strip fx_085_strips[];
extern Fx085Branch fx_085_branches[];
extern s32 fx_085_fastBolts; /* faster bolts */

static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx085Strip *strip, Fx085Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
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
