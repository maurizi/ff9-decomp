#ifndef FX_19F_H
#define FX_19F_H

/* Effect module fx_19f (dir 13 file 0x19f): Judgement Bolt; Ramuh (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_19f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_RAMUH_STAFF 0x5 /* 'so' model: gold gouraud staff with a hooked head (Ramuh's staff) (byte-identical to fx_0ba entry 0x4) */
#define FX_RAMUH_LIGHTNING_RIBBON   0x6 /* trail: ribbon drawn between key points (texture garbled in the render; byte-identical to fx_0ba entry 0x5) (look pending) */
#define FX_RAMUH_LENS_FLASH_KEYS    0x8 /* table: per-frame size keys of the lens flash (FX_RAMUH_LENS_FLASH; byte-identical to fx_0ba entry 0x7) */
#define FX_RAMUH_BLUE_GLOW_RINGS    0xC /* emitter: blue glow quad and fading light-blue gouraud rings (byte-identical to fx_0ba entry 0xA) */
#define FX_RAMUH_LIGHTNING_RIBBON_2 0xF /* trail: second ribbon drawn between key points (texture garbled in the render; byte-identical to fx_0ba entry 0xD) (look pending) */
#define FX_RAMUH_SPARK_BURST        0x16 /* emitter: white gouraud rings + sprites (texture garbled in the render; byte-identical to fx_0ba entry 0x14) (look pending) */
#define FX_RAMUH_LENS_FLASH         0x17 /* particles: white-violet lens flash (byte-identical to fx_0ba entry 0x15) */
#define FX_RAMUH_LIGHTNING_BOLT     0x18 /* trail: 8-frame trail (texture garbled in the render; byte-identical to fx_0ba entry 0x16) (look pending) */
#define FX_RAMUH_THIN_ROD 0x1A /* 'so' model: thin dark rod with a red tip (byte-identical to fx_0ba entry 0x18) */
#define FX_RAMUH_MOTION_KEYS_2      0x1B /* table: per-frame position / rotation keys (op81, second phase; byte-identical to fx_0ba entry 0x19) */
#define FX_RAMUH_MOTION_KEYS        0x1C /* table: per-frame position / rotation keys (op81, first phase; byte-identical to fx_0ba entry 0x1A) */
#define FX_RAMUH_BLUE_RING          0x1D /* rings: blue gouraud rings (byte-identical to fx_0ba entry 0x1B) */
#define FX_RAMUH_CYAN_RING          0x1F /* rings: white / cyan / blue gouraud rings (byte-identical to fx_0ba entry 0x1D) */
#define FX_RAMUH_ORANGE_LIGHTNING 0x20 /* trail: 8-frame orange lightning bolt (offset trail) (byte-identical to fx_0ba entry 0x1E, same render) */
#define FX_RAMUH_ELECTRIC_BOWL 0x21 /* 'so' model: shallow bowl with blue electric flames around a white glow (byte-identical to fx_0ba entry 0x1F) */
#define FX_RAMUH_DOME_COLUMN        0x22 /* 'so' model: dome-capped column (its texture pages hold a 'dummy' placeholder) (look pending) */
#define FX_RAMUH_GREY_RING          0x23 /* rings: grey gouraud ring */
#define FX_RAMUH_GREEN_DOME 0x27 /* 'so' model: teal-green gouraud hemisphere (byte-identical to fx_0ba entry 0x26) */
#define FX_RAMUH_SPARK_PARTICLE     0x2A /* emitter: one particle (texture garbled in the render; byte-identical to fx_0ba entry 0x2C) (look pending) */
#define FX_RAMUH_CAMERA_KEYS        0x2B /* table: per-frame camera point 0 (op82; byte-identical to fx_0ba entry 0x2E) */
#define FX_RAMUH_CAMERA_KEYS_2      0x2C /* table: per-frame camera point 1 (op82; byte-identical to fx_0ba entry 0x2F) */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[1];
} Fx19fTrail;

extern Unk800F78A4 *fx_19f_env;

typedef struct {
    s16 x, y, z;
} Fx19fKey;

typedef struct {
    s16 unk0; /* read only through a pointer moved onto a key (then the key's x) */
    s16 unk2; /* count; through a pointer moved onto a key: the key's y */
    Fx19fKey unk4[1]; /* not read as a member: the keys are reached by byte offsets */
} Fx19fKeys;
typedef struct {
    SVECTOR points0[17]; /* points of a 17-point trail (its 4-byte header precedes them) */
    u8 pad88[4];
} Fx19fTrailPts; /* 0x8C */

extern Fx19fTrailPts fx_19f_boltTrailPoints[32];

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[17];
} Fx19fTrail17; /* 0x8C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[4];
} Fx19fTrail4; /* 0x24 */

typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 index10; /* trail index */
    u8 kind11; /* kind */
    u8 index12;
    u8 unk13; /* write-only (rand % 15) */
    u8 unk14; /* write-only (cleared) */
    u8 alpha15; /* brightness at t = 0 (fxm_drawJaggedTrail alpha, fades with t) */
    u8 pad16[2];
    s32 *noise18; /* noise */
} Fx19fBolt;

extern Fx19fTrail17 fx_19f_boltTrails[];
extern Fx19fTrail4 fx_19f_branchTrails[];
extern SVECTOR fx_19f_boltCenter;
extern s32 fx_19f_boltLength;
extern s32 fx_19f_noise[64][3]; /* noise */

typedef struct {
    s32 *noise0; /* noise */
    s32 count4; /* spawn count */
    s32 kind8; /* kind */
} Fx19fWork83;

typedef struct {
    s32 frame0; /* frame */
    u8 pad4[0xC];
    s16 *pool10; /* pool */
} Fx19fInst;

static s32 fx_19f_op83PoolTask(s32 mode, Fx19fBolt *bolt, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

static void fxm_drawJaggedTrail(Unk80100C78Entry *entry, s32 file, s32 mode, Fx19fTrail *trail, s32 size, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 amp0, s32 amp1, s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 dip);
static void fxm_drawJaggedTrailBranch(Unk80100C78Entry *entry, s32 file, s32 mode, Fx19fTrail *path, Fx19fTrail *trail, s32 size, s32 *noise,
                   s32 branchAt, s32 frame, s32 fade, s32 lineWidth);

/* word i of an SVECTOR, for copies done as two words */
#define FX19F_W(v, i) (((s32 *)&(v))[i])

typedef struct {
    s16 x, y, z, rx, ry, rz;
} Fx19fKey12; /* 0xC */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* base position */
    SVECTOR pos10;
    SVECTOR camPoint18; /* camera point 3: pos8 raised by 0x1EA */
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
} Fx19fWork81; /* 0x84 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[13];
} Fx19fTrail13; /* 0x6C */

extern s16 fx_19f_bandWidths[];
extern s32 fx_19f_stripFromFileIds0[3];
extern s32 fx_19f_stripToFileIds0[3];
extern s32 fx_19f_stripFromFileIds1[3];
extern s32 fx_19f_stripToFileIds1[3];
extern Fx19fTrail fx_19f_offsetTrail;
extern BtlModelBuf fx_19f_effModels[];
extern Fx19fTrail17 fx_19f_stripTrails0[6];
extern Fx19fTrail13 fx_19f_stripTrails1[3];
static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx19fTrail *trail, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang, s32 dang,
                   s32 frame, s32 fade, s32 lineWidth);

/* copied */
static void fxm_drawJaggedTrail(Unk80100C78Entry *entry, s32 file, s32 mode, Fx19fTrail *trail, s32 size, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 amp0, s32 amp1, s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 dip);
/* end copied */

/* propagated */
static void fxm_drawCenterBand(u32 *ot, s32 tpage, s32 width, s32 r, s32 g, s32 b);

#endif
