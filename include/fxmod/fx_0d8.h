#ifndef FX_0D8_H
#define FX_0D8_H

/* Effect module fx_0d8 (dir 13 file 0xD8): Sword Magic Thundara. Lightning bolts drawn as jagged strips
 * (fxm_drawJaggedStrip16 / fxm_drawJaggedStrip4, fxm_drawJaggedBranch4). Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_0d8 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_THUNDARA_IMPACT_FLASH  0x2 /* emitter: four-point star flash, white rings, sparks (byte-identical to fx_005 entry 0x17) */
#define FX_SWDMAG_THUNDARA_ORB_EMITTER   0x14 /* emitter: helix whose callback element draws the orb (0x19-0x1D here) (byte-identical to fx_005 entry 0x1a) */
#define FX_SWDMAG_THUNDARA_IMPACT_RING   0x17 /* emitter: white-cyan gouraud rings (byte-identical to fx_005 entry 0x19) */
#define FX_SWDMAG_THUNDARA_SPARKS        0x18 /* emitter: spark sprites (texture not in the file) (byte-identical to fx_005 entry 0x7) (look pending) */
#define FX_SWDMAG_THUNDARA_ORB_FADE      0x19 /* particles: blue energy sphere fading to orange (FX_THUNDARA_ORB_FADE's draw slot in fx_005) */
#define FX_SWDMAG_THUNDARA_ORB           0x1A /* particles: bright blue energy sphere (byte-identical to fx_005 entry 0x1d) */
#define FX_SWDMAG_THUNDARA_ORB_SPARKS    0x1B /* sprites: sparks around the orb (texture not in the file) (byte-identical to fx_005 entry 0x1e) (look pending) */
#define FX_SWDMAG_THUNDARA_ORB_SPARKS_2  0x1C /* sprites: sparks around the orb (texture not in the file) (byte-identical to fx_005 entry 0x1f) (look pending) */
#define FX_SWDMAG_THUNDARA_LENS_FLARES   0x1D /* particles: orange lens-flare dots around the orb (byte-identical to fx_005 entry 0x16) */
#define FX_SWDMAG_THUNDARA_SWORD_GLINT   0x1E /* rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat */
#define FX_SWDMAG_THUNDARA_SWORD_GLINT_2 0x1F /* rings: white gouraud ring flash at the sword, rotated */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d8Work84;

extern Unk80100C78Entry *fx_0d8_task;

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
    u8 strip10; /* index */
    u8 state11; /* state */
    u8 branchAt12;
    u8 branchAt13;
    u8 kind14; /* strip kind: file block, and < FX_0D8_STRIP_FLAT screen-space */
    u8 width15;
    s16 unk16; /* unused */
    s32 *jitter18;
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
} Fx0d8Part; /* pool entry data */
#define FX_0D8_STRIP_FLAT 2 /* kind14 from here on: the strips draw flat (btlfx_drawTrail mode 2, else mode 0) */

/* fx_003 copies (asm) */
static s32 fx_0d8_op83PoolTask(s32 mode, Fx0d8Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

static void fx_0d8_op83ElemDraw(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *alpha, s32 blend,
                   s32 frame, s32 n);
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[16]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0d8Strip; /* 0x84 */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[8]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
    u8 pad44[4];
} Fx0d8Branch; /* 0x48 */
extern Fx0d8Strip fx_0d8_strips[];
extern Fx0d8Branch fx_0d8_branches[];
extern u16 fx_0d8_strip16Blocks[];
static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx0d8Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
extern u16 fx_0d8_strip4Blocks[];
extern Unk800F78A4 *fx_0d8_env;
static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx0d8Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind);
static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx0d8Strip *strip, Fx0d8Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind);
typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR pos8; /* caster position */
    s32 *jitter10; /* jitter table (fx_0d8_jitter) */
    s32 boltCount14; /* bolts spawned */
} Fx0d8Work83; /* 0x18 */

extern s16 fx_0d8_spawnKinds[16]; /* bolt kind per frame */
extern s32 fx_0d8_jitter[];
s32 fx_0d8_op83(s32 mode, Fx0d8Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
