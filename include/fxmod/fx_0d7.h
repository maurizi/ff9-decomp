#ifndef FX_0D7_H
#define FX_0D7_H

/* Effect module fx_0d7 (dir 13 file 0xD7): Sword Magic Thunder. Lightning bolts drawn as jagged strips
 * (fxm_drawJaggedStrip16 / fxm_drawJaggedStrip4, fxm_drawJaggedBranch4). Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_0d7 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_THUNDER_SMOKE         0x7 /* emitter: yellow / blue smoke puffs after the strike (byte-identical to fx_003 entry 0xa) */
#define FX_SWDMAG_THUNDER_IMPACT_RING   0x8 /* rings: cyan gouraud rings at the strike (byte-identical to fx_003 entry 0xb) */
#define FX_SWDMAG_THUNDER_IMPACT_FLASH  0x9 /* emitter: four-point star flash, white rings, sparks (byte-identical to fx_003 entry 0xc) */
#define FX_SWDMAG_THUNDER_SWORD_GLINT   0xF /* rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat */
#define FX_SWDMAG_THUNDER_SWORD_GLINT_2 0x10 /* rings: white gouraud ring flash at the sword, rotated */

/* fx_0d7 choreography (docs/fxdata.md, "Choreography literals", names as fx_003's Thunder): the bolt pool items'
 * lengths and look, op83's keyframes and length, the strip sizes; op84's sword-flash states. */
#define FX_SWDMAG_THUNDER_BOLT_HEIGHT      0xC00 /* a bolt comes down from this far above its strike point */
#define FX_SWDMAG_THUNDER_FLAT_BOLT_WIDTH  0xAA  /* line width of a flat-strip bolt at its first frame (shrinks to 0) */
#define FX_SWDMAG_THUNDER_BOLT_WIDTH       0xF1  /* line width of a screen-space bolt at its first frame */
#define FX_SWDMAG_THUNDER_BOLT_TAIL_FRAMES 0x10  /* pool item: after a screen-space bolt, wait for SMOKE / the flash */
#define FX_SWDMAG_THUNDER_BOLT_TINT_RGB    0, 7, 0x20 /* additive screen tint (dark blue) as a screen-space bolt strikes */
#define FX_SWDMAG_THUNDER_BOLT_FRAMES      6     /* pool item: a screen-space bolt shows for this many frames */
#define FX_SWDMAG_THUNDER_FLAT_BOLT_FRAMES 4     /* pool item: a flat-strip bolt shows for this many frames (then ends) */
#define FX_SWDMAG_THUNDER_BOLT_REACH       0x640 /* horizontal offset of a bolt's top from its strike point (+-0x80) */
#define FX_SWDMAG_THUNDER_BOLT_COUNT       5     /* op83: bolts, on frames 0 and the four below */
#define FX_SWDMAG_THUNDER_BOLT_2_FRAME     3
#define FX_SWDMAG_THUNDER_BOLT_3_FRAME     5
#define FX_SWDMAG_THUNDER_BOLT_4_FRAME     6
#define FX_SWDMAG_THUNDER_BOLT_5_FRAME     7
#define FX_SWDMAG_THUNDER_SCATTERED_BOLT   3     /* op83: this bolt strikes off the target's centre (+-0x190) */
#define FX_SWDMAG_THUNDER_FRAMES           0xC   /* op83: the task ends at this frame */
#define FX_SWDMAG_THUNDER_STRIP_POINTS  16   /* points of a bolt strip (fx_0d7_strips, fxm_drawJaggedStrip16) */
#define FX_SWDMAG_THUNDER_BRANCH_POINTS 4    /* points of a branch (fxm_drawJaggedStrip4) */
#define FX_SWDMAG_THUNDER_JITTER_ROWS   0x40 /* fx_0d7_jitter rows of three random offsets (along, across, angle) */
#define FX_SWDMAG_THUNDER_BOLT_SLOTS    0xC  /* pool items (strip10 = the bolt count, 1..5) */
#define FX_SWDMAG_THUNDER_FLASH_STATE_IN      0 /* op84: the SWORD_GLINT rings open up at the sword */
#define FX_SWDMAG_THUNDER_FLASH_STATE_FLICKER 1 /* op84: they flicker and grow */
#define FX_SWDMAG_THUNDER_FLASH_STATE_FADE    2 /* op84: the rotated ring turns and they fade */
#define FX_SWDMAG_THUNDER_FLASH_IN_FRAMES      3 /* FX_SWDMAG_THUNDER_FLASH_STATE_IN's length */
#define FX_SWDMAG_THUNDER_FLASH_FLICKER_FRAMES 9 /* FX_SWDMAG_THUNDER_FLASH_STATE_FLICKER's length */
#define FX_SWDMAG_THUNDER_FLASH_FADE_FRAMES    9 /* FX_SWDMAG_THUNDER_FLASH_STATE_FADE's length (the op ends) */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d7Work84;


/* fx_003 copies (asm) */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_SWDMAG_THUNDER_STRIP_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0d7Strip; /* 0x84 */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[8]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
    u8 pad44[4];
} Fx0d7Branch; /* 0x48 */
extern Fx0d7Strip fx_0d7_strips[];
extern Fx0d7Branch fx_0d7_branches[];
typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 strip10; /* bolt index */
    u8 state11; /* state */
    u8 branchAt12; /* branch points */
    u8 branchAt13;
    s16 kind14; /* strip kind: file block, and < FX_0D7_STRIP_FLAT screen-space */
    s32 *jitter18; /* jitter table (fx_0d7_jitter) */
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
} Fx0d7Bolt; /* 0x24 */
#define FX_0D7_STRIP_FLAT 2 /* kind14 from here on: the strips draw flat (btlfx_drawTrail mode 2, else mode 0) */
typedef struct {
    SVECTOR pos0; /* target body position */
    s32 *jitter8; /* fx_0d7_jitter */
    s32 boltCountC; /* bolts spawned */
} Fx0d7Work; /* 0x10 */
extern u16 fx_0d7_strip16Blocks[];
static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx0d7Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
extern u16 fx_0d7_strip4Blocks[];
static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx0d7Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx0d7Strip *strip, Fx0d7Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind);
extern Unk800F78A4 *fx_0d7_env;
extern s32 fx_0d7_jitter[];
static s32 fx_0d7_op83PoolTask(s32 mode, Fx0d7Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
