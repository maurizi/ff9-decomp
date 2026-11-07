#ifndef FX_195_H
#define FX_195_H

/* Effect module fx_195 (dir 13 file 0x195): Bahamut (short); Mega Flare. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_195 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BAHAMUT_PINK_FLAT_RING 0x2 /* rings: white / salmon gouraud rings, 14 frames */
#define FX_BAHAMUT_BACKDROP       0x7 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_BAHAMUT_VIOLET_RINGS   0x8 /* rings: white-cyan-violet-indigo gouraud rings, 24 frames */
#define FX_BAHAMUT_HALO_RINGS     0x9 /* rings: white-crimson-blue gouraud rings, 16 frames */
#define FX_BAHAMUT_BLUE_RINGS     0xA /* rings: navy-to-blue gouraud rings, 12 frames */
#define FX_BAHAMUT_EMBERS         0xB /* emitter: spiral of tiny orange sprites */
#define FX_BAHAMUT_BLUE_SPARKS    0xC /* emitter: tiny blue-violet spark sprites falling */
#define FX_BAHAMUT_ORANGE_STREAKS 0xD /* emitter: two fans, thin orange-white rising streaks (byte-identical to fx_0e3_1 entry 0xE, same render) */
#define FX_BAHAMUT_BLUE_SPARKS_2  0xE /* emitter: tiny blue-violet spark sprites falling */
#define FX_BAHAMUT_RED_GLOWS      0xF /* emitter: spiral of red glowing orbs */
#define FX_BAHAMUT_EMBERS_2       0x10 /* emitter: spiral of tiny orange sprites */
#define FX_BAHAMUT_FIRE_CYLINDER  0x11 /* 'so' model: open cone cylinder, fire texture */
#define FX_BAHAMUT_FIRE_RING      0x13 /* 'so' model: flared ring, fire texture */
#define FX_BAHAMUT_SMOKE_RING     0x14 /* 'so' model: flared ring, grey-violet smoke texture */
#define FX_BAHAMUT_FLAME_STRIP 0x15 /* 'so' model: gouraud plane, red-to-yellow glow bands */
#define FX_BAHAMUT_SMOKE_ARC      0x16 /* 'so' model: curved half-tube, grey smoke texture */
#define FX_BAHAMUT_VIOLET_FAN     0x17 /* emitter: fan with a violet lightning pattern, 13 frames */
#define FX_BAHAMUT_PURPLE_BOLT    0x18 /* trail: violet energy strip (fxm_drawOffsetTrail0) */
#define FX_BAHAMUT_PURPLE_BOLT_2  0x19 /* trail: violet energy strip (fxm_drawOffsetTrail1) */
#define FX_BAHAMUT_RED_STRIP 0x1A /* 'so' model: gouraud plane, red beam line */
#define FX_BAHAMUT_BLUE_SLIVER 0x1C /* 'so' model: curved gouraud blade, white-blue */

/* fx_195 choreography (deep pass). The short Mega Flare is the long form's last two op80 states (fx_0e3_1 IMPACT and
 * EXPLODE, same state values and names) after a short hold; the names are the long form's, the values this module's. */
#define FX_BAHAMUT_MODEL_COUNT      7    /* btlfx_initEffModels slots (7 registered) */
#define FX_BAHAMUT_ARC_POINTS       13   /* points of the (uncalled) fx_195_buildOffsetTrail arc, */
#define FX_BAHAMUT_ARC_WIDTH        0x1A /* their width */
#define FX_BAHAMUT_BOLT_POINTS      57   /* points of a PURPLE_BOLT strip (Fx195Trail57, fxm_drawOffsetTrail0) */
#define FX_BAHAMUT_LIGHTNING_POINTS 13   /* points of a PURPLE_BOLT_2 lightning (Fx195Trail, fxm_drawOffsetTrail1) */
#define FX_BAHAMUT_STATE_HOLD       0    /* op80 state: the camera target held above the target point */
#define FX_BAHAMUT_STATE_IMPACT     4    /* flash, quake, FIRE_CYLINDER burst, BLUE_SPARKS (fx_0e3_1's IMPACT) */
#define FX_BAHAMUT_STATE_EXPLODE    5    /* the explosion: smoke core, rising cylinders, SMOKE_RING column, FIRE_RING pillars, bolts (op80 ends) */
#define FX_BAHAMUT_HOLD_FRAMES      2
#define FX_BAHAMUT_IMPACT_FRAMES    14
#define FX_BAHAMUT_IMPACT_FLASH_FRAME 4  /* IMPACT: the blue tint fades before it; flash, BLUE_SLIVER blown up, the FIRE_CYLINDER burst from here */
#define FX_BAHAMUT_BURST_FRAMES     10   /* IMPACT: the burst and BLUE_RINGS run this long */
#define FX_BAHAMUT_QUAKE_FRAME      5    /* IMPACT: the camera quake */
#define FX_BAHAMUT_QUAKE_FRAMES     7
#define FX_BAHAMUT_BEAM_BOLTS       3    /* IMPACT: PURPLE_BOLTs around the burst */
#define FX_BAHAMUT_EXPLODE_FRAMES   77
#define FX_BAHAMUT_WHITE_FRAME      2    /* EXPLODE: darken before it, a white flash on it, */
#define FX_BAHAMUT_WHITE_FADE_FRAMES 16  /* fading over this many frames after */
#define FX_BAHAMUT_RED_TINT_1_FRAME 0x14 /* EXPLODE: three additive red flashes */
#define FX_BAHAMUT_RED_TINT_1_RGB   0xD7, 0x64, 0x64
#define FX_BAHAMUT_RED_TINT_2_FRAME 0x1D
#define FX_BAHAMUT_RED_TINT_2_RGB   0x97, 0x4D, 0x4D
#define FX_BAHAMUT_RED_TINT_3_FRAME 0x20
#define FX_BAHAMUT_RED_TINT_3_RGB   0x80, 0x40, 0x40
#define FX_BAHAMUT_CORE_FRAMES      0x49 /* EXPLODE: SMOKE_ARC and FLAME_STRIP (the core) drawn until this frame */
#define FX_BAHAMUT_CORE_IN_FRAMES   7    /* the core shrinks in over these frames, then grows */
#define FX_BAHAMUT_CORE_FLASH_FRAME 5    /* EXPLODE: a blue flash over CORE_FLASH_FRAMES */
#define FX_BAHAMUT_CORE_FLASH_FRAMES 7
#define FX_BAHAMUT_AFTERGLOW_FRAME  0x3B /* EXPLODE: RED_STRIP fades from here to the end */
#define FX_BAHAMUT_COLUMN_FRAMES    0x41 /* EXPLODE: the SMOKE_RING column drawn until this frame */
#define FX_BAHAMUT_PILLARS          6    /* EXPLODE: FIRE_RING pillars, */
#define FX_BAHAMUT_PILLAR_STEP      7    /* one every this many frames, */
#define FX_BAHAMUT_PILLAR_LIFE      0x20 /* each this long, */
#define FX_BAHAMUT_PILLAR_RISE_FRAMES 4  /* rising over these frames */
#define FX_BAHAMUT_DOME_FLASH_FRAMES 0x10 /* EXPLODE: FIRE_CYLINDER flash (the long form draws FIRE_DOME here) */
#define FX_BAHAMUT_SOUND_TAIL_FRAME 0x3C /* EXPLODE: the two looping sounds are replaced by their tails */
#define FX_BAHAMUT_COLUMN_BOLTS     2    /* EXPLODE: PURPLE_BOLTs up the column, opposite each other, */
#define FX_BAHAMUT_THIRD_BOLT_FRAMES 0x26 /* and the third until this frame */
#define FX_BAHAMUT_LIGHTNINGS       5    /* EXPLODE: PURPLE_BOLT_2 lightnings around the core */
#define FX_BAHAMUT_EMBERS_FRAME     0x31 /* EXPLODE: EMBERS_2 start */
#define FX_BAHAMUT_CYL_TPAGE        0x18   /* fxm_drawTexCylinder texture: 4-bit page (0x200, 0x100), abr 0 */
#define FX_BAHAMUT_CYL_CLUT         0x3E81 /* CLUT at (0x10, 0xFA) */
#define FX_BAHAMUT_CYL_U            0
#define FX_BAHAMUT_CYL_V            0xC0
#define FX_BAHAMUT_CYL_ROWS         3    /* fxm_drawTexCylinder: quad rings */
#define FX_BAHAMUT_CYL_COLS         16   /* quads per ring: gouraud at the first and last quad, flat between */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[FX_BAHAMUT_LIGHTNING_POINTS];
} Fx195Trail; /* 0x6C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[FX_BAHAMUT_BOLT_POINTS];
} Fx195Trail57;

extern Unk800F78A4 *fx_195_env;

/* propagated */
void fxm_rollTowardEye(VECTOR *pos, SVECTOR *rot);

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx195Rgbc; /* primitive colour word */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx195Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* counter */
    SVECTOR unk8; /* write-only (copy of fx_195_basePos) */
    SVECTOR pos10; /* position */
    SVECTOR pos18;
    SVECTOR camPos20; /* camera */
    u8 pad28[0x30 - 0x28];
    s16 sound30;
    s16 sound32;
    u8 pad34[0x44 - 0x34];
    s32 unk44; /* write-only (cleared at init) */
    u8 pad48[0x54 - 0x48];
    Pool6CEntry *emit54;
    Pool6CEntry *emit58;
    s32 model5C; /* model */
    Fx195Vec3 pos60; /* model position */
    Fx195Vec3 scale6C; /* model scale */
    SVECTOR rot78; /* model rotation */
    s32 model80; /* models */
    s32 model84;
    s32 model88;
    s32 model8C;
    s32 model90;
    s32 model94;
} Fx195Work80; /* 0x98 */

extern BtlModelBuf fx_195_effModels[];
extern SVECTOR fx_195_basePos;
extern Fx195Trail fx_195_shortTrails[];
extern Fx195Trail57 fx_195_longTrails[];
extern Fx195Trail57 fx_195_longTrail2;
static void fxm_drawOffsetTrail0(Unk80100C78Entry *entry, Fx195Trail57 *trail, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth);
static void fxm_drawOffsetTrail1(Unk80100C78Entry *entry, Fx195Trail *trail, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth);

s32 fx_195_op80(s32 mode, Fx195Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
