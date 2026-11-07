#ifndef FX_1F2_H
#define FX_1F2_H

/* Effect module fx_1f2 (dir 13 file 0x1f2): enemy Grand Cross (Dummy/Necron). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_GRAND_CROSS_ASTEROID         0x1 /* 'so' model: dark cratered rock */
#define FX_GRAND_CROSS_TWINKLE          0x3 /* emitter: tiny twinkling dot sprites */
#define FX_GRAND_CROSS_TWINKLE_2        0x4 /* emitter: tiny twinkling dot sprites */
#define FX_GRAND_CROSS_COLOR_RINGS      0x5 /* emitter: white-lilac, teal-orange and orange-magenta gouraud rings */
#define FX_GRAND_CROSS_WHITE_TRAIL      0x6 /* trail: white streak strip */
#define FX_GRAND_CROSS_SMOKE_PANEL      0x7 /* 'so' model: curved panel, grey smoke texture */
#define FX_GRAND_CROSS_NEBULA_PANEL     0x9 /* 'so' model: curved panel, violet nebula texture */
#define FX_GRAND_CROSS_LILAC_RINGS      0xA /* emitter: white-lilac and cyan gouraud rings, magenta smoke */
#define FX_GRAND_CROSS_PALE_STREAKS     0xB /* emitter: two fans, pale light streaks */
#define FX_GRAND_CROSS_WHITE_TRAIL_2    0xC /* trail: white streak strip */
#define FX_GRAND_CROSS_PINK_RINGS       0xD /* emitter: pink-to-teal gouraud rings + smoke */
#define FX_GRAND_CROSS_BLUE_RINGS       0xE /* emitter: blue-to-teal gouraud rings + smoke */
#define FX_GRAND_CROSS_ASTEROID_2       0xF /* 'so' model: dark cratered rock */
#define FX_GRAND_CROSS_BEIGE_RINGS      0x10 /* emitter: white-grey and beige gouraud rings */
#define FX_GRAND_CROSS_SMOKE_RINGS      0x11 /* emitter: two rings with a grey smoke texture */
#define FX_GRAND_CROSS_ICE_PLANET       0x80 /* summon data: 'so' model, pale blue planet sphere */
#define FX_GRAND_CROSS_BANDED_PLANET    0x81 /* summon data: 'so' model, banded brown gas-giant sphere */
#define FX_GRAND_CROSS_BANDED_PLANET_2  0x82 /* summon data: 'so' model, banded brown gas-giant sphere */
#define FX_GRAND_CROSS_ORANGE_PLANET    0x83 /* summon data: 'so' model, swirled orange planet sphere */
#define FX_GRAND_CROSS_DARK_SPHERE      0x84 /* summon data: 'so' model, dark speckled sphere */
#define FX_GRAND_CROSS_VORTEX_CONE      0x85 /* summon data: 'so' model, dark cone with an orange vortex */
#define FX_GRAND_CROSS_CLOUD_DOME       0x86 /* summon data: 'so' model, grey cloud dome */
#define FX_GRAND_CROSS_EARTH_GLOBE      0x87 /* summon data: 'so' model, blue-white earth-like globe */
#define FX_GRAND_CROSS_CLOUD_DISC       0x88 /* summon data: 'so' model, dark blue cloud disc */
#define FX_GRAND_CROSS_GALAXY_RING      0x89 /* summon data: 'so' model, band ring with a starry nebula texture */
#define FX_GRAND_CROSS_RED_PLANET       0x8A /* summon data: 'so' model, red planet sphere */
#define FX_GRAND_CROSS_GREY_MOON        0x8B /* summon data: 'so' model, cratered grey moon sphere */
#define FX_GRAND_CROSS_VORTEX_HOURGLASS 0x8C /* summon data: 'so' model, purple-orange hourglass vortex */
#define FX_GRAND_CROSS_BLUE_SPIKE       0x8D /* summon data: 'so' model, gouraud blue spike */
#define FX_GRAND_CROSS_BLUE_HALO        0x8E /* summon data: 'so' model, gouraud blue ring halo */

/* copied */
/* fx_1f2 choreography (docs/fxdata.md, "Choreography literals"; partial: the work size and counts, the white trails,
 * the timeline keys that start emitters or a phase, the task length; the model countdowns' keys and fades stay
 * numeric). */
#define FX_GRAND_CROSS_TIMER_WORDS 0x20 /* op80 init clears the work's first 0x20 words (the countdowns) */
#define FX_GRAND_CROSS_MODEL_SLOTS 0x20 /* btlfx_initEffModels slots */
#define FX_GRAND_CROSS_TRAILS      0x10 /* WHITE_TRAIL strips (fx_1f2_trails; fx_1f2_unitTrails in the second phase) */
#define FX_GRAND_CROSS_TRAIL_POINTS 9   /*   points per strip */
#define FX_GRAND_CROSS_ORBITS      4    /* orbiting model triples (fx_1f2_orbitModels) */
#define FX_GRAND_CROSS_RINGS_FRAME 0x2D /* PINK_RINGS, BLUE_RINGS, LILAC_RINGS start (PALE_STREAKS at frame 0) */
#define FX_GRAND_CROSS_TRAILS_FRAMES 0x5F /* the trails swirl (timer4C) from frame 0 until this frame, then restart */
#define FX_GRAND_CROSS_TWINKLE_FRAME 0x9F /* TWINKLE and TWINKLE_2 start */
#define FX_GRAND_CROSS_COLOR_RINGS_FRAME 0xFF /* COLOR_RINGS start */
#define FX_GRAND_CROSS_BEIGE_RINGS_FRAME 0x11F /* BEIGE_RINGS and SMOKE_RINGS start */
#define FX_GRAND_CROSS_PHASE_2_FRAME 0x14F /* the rings move up, the first models stop and the cross model (timer50) starts */
#define FX_GRAND_CROSS_UNIT_TRAILS_FRAME 0x18F /* the trails restart at the units (fx_1f2_unitTrails) */
#define FX_GRAND_CROSS_FRAMES      0x1BF /* the task ends at this frame */

extern Unk800F78A4 *fx_1f2_env;
/* end copied */

typedef struct {
    s32 active0; /* models drawn */
    s32 timer4; /* flash model timer */
    s32 timer8; /* twin model timer */
    s32 timerC; /* tint timers */
    s32 timer10;
    s32 timer14;
    s32 timer18;
    s32 unk1C; /* unused */
    s32 timer20; /* screen ripple timer */
    s32 timer24;
    s32 timer28;
    s32 spin2C; /* spin speed */
    s32 unk30; /* unused */
    s32 timer34; /* sparks timer */
    s32 halfWidth38; /* ribbon half-width, stored in the trail points' pad */
    s32 timer3C;
    s32 done40;
    s32 timer44;
    s32 done48;
    s32 timer4C;
    s32 timer50;
    s32 timer54;
    s32 timer58;
    s32 pad5C[(0x80 - 0x5C) / 4];
    Pool6CEntry *emit80;
    Pool6CEntry *emit84;
    Pool6CEntry *emit88;
    Pool6CEntry *emit8C;
    s32 pad90[(0x100 - 0x90) / 4];
    SVECTOR pos100;
    SVECTOR pos108;
    SVECTOR bonePos110[2]; /* bone 5 and bone 9 positions */
    SVECTOR pos120; /* midpoint */
} Fx1f2Work; /* 0x128 */

typedef struct {
    s32 model0; /* model */
    s32 radius4; /* radius */
    SVECTOR rot8; /* rotation */
    VECTOR pos10; /* position */
    VECTOR scale20; /* scale */
} Fx1f2Model; /* 0x30 */

typedef struct {
    s32 model0; /* models */
    s32 model4;
    s32 model8;
    s32 radiusC; /* radius */
    SVECTOR rot10; /* rotation */
    VECTOR pos18; /* position */
    VECTOR scale28; /* scale */
} Fx1f2Ring; /* 0x38 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[FX_GRAND_CROSS_TRAIL_POINTS];
    s32 timer4C; /* timer */
    SVECTOR pos50[16]; /* points (pad = next) */
    SVECTOR velD0[16]; /* velocities */
} Fx1f2Trail; /* 0x150 */

extern Fx1f2Model fx_1f2_model0;
extern Fx1f2Model fx_1f2_model1;
extern Fx1f2Model fx_1f2_model2;
extern Fx1f2Ring fx_1f2_orbitModels[FX_GRAND_CROSS_ORBITS];
extern Fx1f2Model fx_1f2_model3;
extern Fx1f2Model fx_1f2_model4[2];
extern Fx1f2Model fx_1f2_model5;
extern Fx1f2Model fx_1f2_model6[2];
extern Fx1f2Model fx_1f2_model7;
extern Fx1f2Model fx_1f2_model8[4];
extern SVECTOR fx_1f2_sparkRot;
extern Fx1f2Model fx_1f2_model9;
extern VECTOR fx_1f2_model9Pos;
extern Fx1f2Model fx_1f2_model10;
extern SVECTOR fx_1f2_emitPos0; /* target */
extern SVECTOR fx_1f2_faceAngles; /* angles to the target */
extern SVECTOR fx_1f2_emitPos1;
extern SVECTOR fx_1f2_unitPos[4]; /* unit positions */
extern Fx1f2Trail fx_1f2_trails[FX_GRAND_CROSS_TRAILS];
extern Fx1f2Trail fx_1f2_unitTrails[FX_GRAND_CROSS_TRAILS]; /* 0x1500 bytes up to fx_1f2_effModels, cleared as 16; only entries below
                                         * targetCount are drawn */
extern BtlModelBuf fx_1f2_effModels[];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
