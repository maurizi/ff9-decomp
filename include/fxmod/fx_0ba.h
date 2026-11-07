#ifndef FX_0BA_H
#define FX_0BA_H

/* Effect module fx_0ba (dir 13 file 0xBA): Ramuh. Jagged bolts (fxm_drawJaggedTrail, fxm_drawJaggedTrailBranch),
 * ribbons and arc trails (fxm_arcTrailSpawn) around Ramuh and his orb. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ba effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_RAMUH_STAFF              0x4 /* 'so' model: gold gouraud staff with a hooked head (Ramuh's staff) */
#define FX_RAMUH_LIGHTNING_RIBBON   0x5 /* trail: ribbon of blue-white electric glow drawn between key points */
#define FX_RAMUH_LENS_FLASH_KEYS    0x7 /* table: per-frame size keys of the lens flash (FX_RAMUH_LENS_FLASH) */
#define FX_RAMUH_BLUE_GLOW_RINGS    0xA /* emitter: blue glow quad and fading light-blue gouraud rings */
#define FX_RAMUH_LIGHTNING_RIBBON_2 0xD /* trail: second blue-white electric ribbon drawn between key points */
#define FX_RAMUH_BOLT_SPARK_BURST   0x14 /* emitter: flashing white gouraud rings and blue spark sprites; at run time (fx_19f's byte-identical entry) small blue lightning-bolt glyphs in a glow, the cyan flash and bolt sparks round the strike; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_RAMUH_LENS_FLASH         0x15 /* particles: white-violet lens flash */
#define FX_RAMUH_LIGHTNING_BOLT     0x16 /* trail: 8-frame blue-white lightning bolt */
#define FX_RAMUH_THIN_ROD           0x18 /* 'so' model: thin dark rod with a red tip */
#define FX_RAMUH_MOTION_KEYS_2      0x19 /* table: per-frame position / rotation keys (op81, second phase) */
#define FX_RAMUH_MOTION_KEYS        0x1A /* table: per-frame position / rotation keys (op81, first phase) */
#define FX_RAMUH_BLUE_RING          0x1B /* rings: blue gouraud rings */
#define FX_RAMUH_CYAN_RING          0x1D /* rings: white / cyan / blue gouraud rings */
#define FX_RAMUH_ORANGE_LIGHTNING   0x1E /* trail: 8-frame orange lightning bolt (offset trail) */
#define FX_RAMUH_ELECTRIC_BOWL      0x1F /* 'so' model: shallow bowl with blue electric flames around a white glow */
#define FX_RAMUH_CLOUD_PLANE        0x20 /* 'so' model: flat square plane with a dark blue storm-cloud texture */
#define FX_RAMUH_GREY_RING          0x21 /* rings: grey gouraud rings */
#define FX_RAMUH_BOLT_POINTS        0x23 /* table: ten points around the summon, picked at random (op83) */
#define FX_RAMUH_BOLT_POINTS_2      0x24 /* table: second set of ten points around the summon (op83) */
#define FX_RAMUH_GREEN_DOME         0x26 /* 'so' model: teal-green gouraud hemisphere */
#define FX_RAMUH_WHITE_RING         0x27 /* rings: white gouraud ring */
#define FX_RAMUH_SPARK_RING         0x29 /* rings: pale cyan gouraud rings (pool items) */
#define FX_RAMUH_BOLT_SPARKS        0x2C /* emitter: one particle; not fetched in a full Ramuh (a branch that cast did not take); fx_19f's byte-identical entry is a blue lightning-bolt glyph sprite at run time, the small bolt sparks around the pillar; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_RAMUH_CAMERA_KEYS        0x2E /* table: per-frame camera point 0 (op82) */
#define FX_RAMUH_CAMERA_KEYS_2      0x2F /* table: per-frame camera point 1 (op82) */
#define FX_RAMUH_POOL_LIGHTNING     0x32 /* trail: 8-frame blue-white lightning of the op85 pool items */
#define FX_RAMUH_CLOUD_RING         0x80 /* summon data entry 0: 'so' model, ring mound with the storm-cloud texture */
#define FX_RAMUH_CLOUD_DISC         0x81 /* summon data entry 1: 'so' model, flat disc with the storm-cloud texture */
#define FX_RAMUH_CLOUD_PLANE_2      0x82 /* summon data entry 2: 'so' model, flat square plane with the storm-cloud texture */
#define FX_RAMUH_CAPPED_COLUMN      0x83 /* summon data entry 3: 'so' model, dark blue column with a mushroom-shaped cap (its texture reads as a "dummy" placeholder) */
#define FX_RAMUH_STORM_CLOUDS       0x84 /* summon data entry 4: emitter, orbiting animated blue cloud puffs */
#define FX_RAMUH_SUMMON_KEYS_5      0x85 /* summon data entry 5: per-frame key table (op80) */
#define FX_RAMUH_BLUE_FLASH         0x86 /* summon data entry 6: emitter, orbiting fan of a horizontal blue light flash */

/* fx_0ba choreography (docs/fxdata.md, "Choreography literals"): op80's states (the summon and the storm clouds)
 * and op81's (the bolt), their lengths, keyframes and screen tints, the pool sizes, the VRAM rects the scrolling
 * cloud textures fill (checked against the models' tpage), GPU words. */
#define FX_RAMUH_MODEL_COUNT    6    /* op80: btlfx_initEffModels slots */
#define FX_RAMUH_SUMMON_STATE_APPEAR 0 /* op80: the screen brightens, the clouds scroll */
#define FX_RAMUH_SUMMON_STATE_STORM  1 /* op80: the summon; CLOUD_DISC / CLOUD_RING morph, BLUE_FLASH, STORM_CLOUDS */
#define FX_RAMUH_SUMMON_STATE_COLUMN 2 /* op80: CLOUD_PLANE_2 and CAPPED_COLUMN */
#define FX_RAMUH_APPEAR_FADE_FRAMES 6  /* FX_RAMUH_SUMMON_STATE_APPEAR: brightens over this many frames */
#define FX_RAMUH_APPEAR_FRAMES  0x20   /* FX_RAMUH_SUMMON_STATE_APPEAR's length (the summon motion starts) */
#define FX_RAMUH_STORM_1_FRAME  0xC    /* FX_RAMUH_SUMMON_STATE_STORM keyframes */
#define FX_RAMUH_STORM_2_FRAME  0x18
#define FX_RAMUH_STORM_MOTION_FRAME 0x3B /* the summon's second motion */
#define FX_RAMUH_STORM_CLOUD_FRAME  0x3C /* the cloud morph models, BLUE_FLASH and STORM_CLOUDS start */
#define FX_RAMUH_STORM_FRAMES   0x5F   /* FX_RAMUH_SUMMON_STATE_STORM's length */
#define FX_RAMUH_COLUMN_FRAME   0x14   /* FX_RAMUH_SUMMON_STATE_COLUMN: CAPPED_COLUMN shows from here */
#define FX_RAMUH_COLUMN_FRAMES  0x1E   /* FX_RAMUH_SUMMON_STATE_COLUMN's length */
#define FX_RAMUH_BOLT_MODEL_COUNT 5    /* op81: btlfx_initEffModels slots */
#define FX_RAMUH_STATE_GATHER 0 /* op81: Ramuh's motion keys, GREY_RING, the glow */
#define FX_RAMUH_STATE_CHARGE 1 /* op81: subtractive white flash, then a blue one */
#define FX_RAMUH_STATE_STRIKE 2 /* op81: the bolt: sparks, rings, ribbons and a run of screen tints */
#define FX_RAMUH_STATE_TAIL   3 /* op81: the emitters fade and stop */
#define FX_RAMUH_GLOW_FRAME     7    /* FX_RAMUH_STATE_GATHER: the glow pulses from here */
#define FX_RAMUH_GATHER_FRAMES  15   /* FX_RAMUH_STATE_GATHER's length */
#define FX_RAMUH_CHARGE_FRAMES  9    /* FX_RAMUH_STATE_CHARGE's length; at its end a subtractive tint (leaves blue) */
#define FX_RAMUH_CHARGE_END_RGB 0xFF, 0xFF, 0
#define FX_RAMUH_STRIKE_FRAMES  51   /* FX_RAMUH_STATE_STRIKE's length (BOLT_SPARKS start at its end) */
#define FX_RAMUH_SPARK_BURST_FRAME 14 /* FX_RAMUH_STATE_STRIKE: FX_RAMUH_BOLT_SPARK_BURST starts */
#define FX_RAMUH_FLASH_FRAME    16   /*   additive blue flash fading over */
#define FX_RAMUH_FLASH_FRAMES   6    /*   this many frames (yellow tint on its first) */
#define FX_RAMUH_FLASH_RGB      0x80, 0x80, 0
#define FX_RAMUH_GLOW_RINGS_FRAME 20 /*   FX_RAMUH_BLUE_GLOW_RINGS start */
#define FX_RAMUH_DARKEN_FRAME   22   /*   one frame of subtractive tint, */
#define FX_RAMUH_DARKEN_RGB     0x80, 0x80, 0x20
#define FX_RAMUH_YELLOW_FRAME   23   /*   then additive yellow fading over */
#define FX_RAMUH_YELLOW_FRAMES  3    /*   this many frames */
#define FX_RAMUH_BLUE_FRAME     29   /*   one frame of additive blue */
#define FX_RAMUH_BLUE_RGB       0x20, 0x64, 0xFF
#define FX_RAMUH_DARKEN_2_FRAME 33   /*   one frame of subtractive tint, */
#define FX_RAMUH_DARKEN_2_RGB   0x40, 0x40, 0
#define FX_RAMUH_GREY_FRAME     34   /*   one of additive grey */
#define FX_RAMUH_GREY_RGB       0x40, 0x40, 0x40
#define FX_RAMUH_SKY_FRAME      41   /*   additive sky-blue fade-in over */
#define FX_RAMUH_SKY_FRAMES     10   /*   this many frames */
#define FX_RAMUH_TAIL_FRAMES    20   /* FX_RAMUH_STATE_TAIL's length (the op ends); on its first frame a cyan tint */
#define FX_RAMUH_TAIL_RGB       0x20, 0xFF, 0xFF
#define FX_RAMUH_BOLT_SLOTS     0x20 /* op83: pool items (Fx0baBolt) */
#define FX_RAMUH_PART_SLOTS     0x55 /* op85: pool items (Fx0baPart) */
/* Scrolling textures: FX_RAMUH_CLOUD_PLANE / CLOUD_RING / CLOUD_DISC's storm-cloud page (tpage 0x99 = 8-bit page
 * (0x240, 0x100)): btlfx_drawScrollMove copies each half's top rows down to v 0x70, scrolled; CAPPED_COLUMN (tpage
 * 0x15 = 4-bit page (0x140, 0x100)) gets its right half from its left one. */
#define FX_RAMUH_CLOUD_VRAM_X   0x240
#define FX_RAMUH_CLOUD_2_VRAM_X 0x280
#define FX_RAMUH_CLOUD_VRAM_Y   0x170
#define FX_RAMUH_CLOUD_VRAM_W   0x38
#define FX_RAMUH_CLOUD_VRAM_H   0x70
#define FX_RAMUH_CLOUD_SRC_Y    0x100
#define FX_RAMUH_COLUMN_VRAM_X  0x160
#define FX_RAMUH_COLUMN_VRAM_Y  0x100
#define FX_RAMUH_COLUMN_VRAM_W  0x20
#define FX_RAMUH_COLUMN_VRAM_H  0x80
#define FX_RAMUH_COLUMN_SRC_X   0x140
#define FX_RAMUH_COLUMN_SRC_Y   0x100

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[1]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0baStrip;

typedef struct {
    s32 posX0; /* position, 20.12 */
    s32 posY4;
    s32 posZ8;
    s16 velXC; /* velocity */
    s16 velYE;
    s16 velZ10;
    s16 unk12; /* write-only (0x1080 +- 0x20) */
    s16 trail14; /* trail index */
    s16 pointIdx16; /* point index */
    u8 *point18; /* strip point */
} Fx0baPart; /* 0x1C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[17]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0baTrail; /* 0x8C */

typedef struct {
    u8 pad0[8];
    SVECTOR rot8; /* rotation */
} Fx0baWork85; /* 0x10 */

extern Fx0baTrail fx_0ba_arcTrails[5];

typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 strip10; /* path index */
    u8 kind11; /* state */
    u8 branchAt12;
    u8 branchAt13;
    u8 unk14; /* write-only (cleared) */
    u8 amp15; /* amplitude */
    u8 pad16[2];
    s32 *jitter18; /* arc profile */
} Fx0baBolt;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[4]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0baStrip4; /* 0x24 */

extern s32 fx_0ba_preOrb; /* 1 from the data until the orb appears (fx_0ba_op80 frame 0x3C clears it): bolts spawn every
                           * frame and draw impact rings while set */
extern SVECTOR fx_0ba_boltSrc;
extern Fx0baTrail fx_0ba_boltTrails[];
extern Fx0baStrip4 fx_0ba_boltBranches[];
extern s32 fx_0ba_summonDone;
extern SVECTOR fx_0ba_summonPos;

typedef struct {
    SVECTOR points0[17];
    s32 unk88; /* unused */
} Fx0baPath; /* 0x8C */

extern Unk800F78A4 *fx_0ba_env;
extern Fx0baPath fx_0ba_boltPoints[32];
extern s32 fx_0ba_arcTrailNext;

typedef struct {
    s32 *jitter0; /* arc profile table */
    s32 boltCount4; /* spawn counter */
    s32 kind8; /* state */
} Fx0baWork83;

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad[3];
} Fx0baPt; /* 0xC */

extern s32 fx_0ba_boltJitter[];
extern s32 fx_0ba_boltLen;
extern SVECTOR fx_0ba_orbPos;

static void fxm_drawCenterBand(u32 *ot, s32 tpage, s32 width, s32 r, s32 g, s32 b);
static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0baStrip *trail, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang, s32 dang,
                   s32 frame, s32 fade, s32 lineWidth);
static void fxm_drawJaggedTrail(Unk80100C78Entry *entry, s32 file, s32 mode, Fx0baStrip *trail, s32 size, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 amp0, s32 amp1, s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 dip);
static void fxm_drawJaggedTrailBranch(Unk80100C78Entry *entry, s32 file, s32 mode, Fx0baStrip *path, Fx0baStrip *trail, s32 size, s32 *noise,
                   s32 branchAt, s32 frame, s32 fade, s32 lineWidth);
static s32 fxm_randStripPoint32(SVECTOR *out);
static s32 fx_0ba_op85PoolTask(s32 mode, Fx0baPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
static s32 fx_0ba_op83PoolTask(s32 mode, Fx0baBolt *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
static void fxm_arcTrailReset(void);
static void fxm_arcTrailSpawn(Unk80100C78Entry *entry, SVECTOR *center, s32 radius, SVECTOR *from, SVECTOR *to);
s32 fx_0ba_op83(s32 mode, Fx0baWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

typedef struct {
    u8 pad0[0x180];
    BtlMotion *motion180;
    BtlMotion *motion184;
    BtlMotion *motion188;
} Fx0baUnit;
typedef struct {
    u8 pad0[0x54];
    Fx0baUnit *summon54; /* g_btlfxLoader.summon */
    u8 pad58[0x60 - 0x58];
    s32 model60; /* summon model */
} Fx0baSummon;
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0baVec3;
typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    u8 pad8[8];
    SVECTOR pos10; /* orb position */
    SVECTOR pos18; /* bone position */
    u8 pad20[8];
    Fx0baVec3 pos28; /* model position */
    Fx0baVec3 scale34; /* model scale */
    SVECTOR rot40; /* model rotation */
    Fx0baVec3 summonPos48; /* summon position */
    Fx0baVec3 summonScale54; /* summon scale */
    SVECTOR rot60; /* summon rotation */
    SVECTOR rot68; /* model rotation */
    Fx0baVec3 scale70; /* model scale */
    s32 model7C;
    s32 model80;
    s32 model84;
    s32 model88;
    s32 model8C;
    s32 model90;
    s16 unk94; /* write-only (cleared at init) */
    s16 unk96; /* write-only (cleared at init) */
    s16 unk98; /* write-only (cleared at init) */
    s16 unk9A; /* write-only (cleared at init) */
    u8 pad9C[4];
} Fx0baWork80; /* 0xA0 */
extern BtlModelBuf fx_0ba_effModels0[];
s32 fx_0ba_op80(s32 mode, Fx0baWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[1]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0baPTrail;

typedef struct {
    s16 x, y, z;
} Fx0baPKey;

typedef struct {
    s16 unk0; /* read only through a pointer moved onto a key (then the key's x) */
    s16 unk2; /* count, not read by the module */
    Fx0baPKey unk4[1]; /* not read as a member: the keys are reached by byte offsets */
} Fx0baPKeys;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[17]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0baPTrail17; /* 0x8C */

typedef struct {
    s16 x, y, z, rx, ry, rz;
} Fx0baPKey12; /* 0xC */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* base position */
    SVECTOR pos10;
    SVECTOR camAt18;
    u8 pad20[0x28];
    s32 pos48[3]; /* model position */
    s32 scale54[3]; /* model scale */
    SVECTOR rot60; /* model rotation */
    Pool6CEntry *emit68;
    Pool6CEntry *emit6C;
    s32 model70; /* models */
    s32 model74;
    s32 model78;
    s32 model7C;
    s32 model80;
} Fx0baPWork81; /* 0x84 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[13]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0baPTrail13; /* 0x6C */

#define FX0BAP_W(v, i) (((s32 *)&(v))[i])

extern s16 fx_0ba_bandWidths[];
extern s32 fx_0ba_ribbonFromBlocks[3];
extern s32 fx_0ba_ribbonToBlocks[3];
extern s32 fx_0ba_ribbon13FromBlocks[3];
extern s32 fx_0ba_ribbon13ToBlocks[3];
extern Fx0baPTrail fx_0ba_offsetTrail;
extern BtlModelBuf fx_0ba_effModels1[];
extern Fx0baPTrail17 fx_0ba_ribbons[6];
extern Fx0baPTrail13 fx_0ba_ribbons13[3];

#endif
