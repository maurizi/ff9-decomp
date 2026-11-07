#ifndef FX_0E3_0_H
#define FX_0E3_0_H

/* Effect module fx_0e3_0 (dir 13 file 0x0E3): Bahamut, full summon (load group 0). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0e3_0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BAHAMUT_WING_TRAIL      0x1 /* trail: white streak strip drawn from Bahamut's bones */
#define FX_BAHAMUT_PURPLE_BOLT     0x2 /* trail: purple energy strip (fxm_drawOffsetTrail1) */
#define FX_BAHAMUT_TEAL_PUFF       0x3 /* particles: a dark teal cloud puff; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_BAHAMUT_BLUE_RINGS      0x5 /* emitter: dark-to-light blue gouraud rings */
#define FX_BAHAMUT_TERRAIN   0x7 /* 'so' model, flat square grid drawn with a fading core colour: grid textured with aerial green mountain terrain, the ground seen from above; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_BAHAMUT_WHITE_BOLT      0x8 /* trail: a thin white lightning bolt over a white cloud; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_BAHAMUT_FIRE_SPARKS     0xA /* emitter: orange flame glows + spark sprites */
#define FX_BAHAMUT_CLOUD_RING 0xB /* 'so' model, flat ring of tiles drawn with a fading colour: flat ring with a band of teal-blue cloud; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_BAHAMUT_WHITE_RINGS     0xC /* rings: grey-white gouraud rings */
#define FX_BAHAMUT_SMOKE_STRIP  0x82 /* summon data, 'so' model, the pillar (pillarRgb): wavy strip of grey-white streaky smoke; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_BAHAMUT_BLUE_SPARKS     0x83 /* summon data: emitter, blue-white spark sprites */
#define FX_BAHAMUT_CLOUD_SPHERE    0x85 /* summon data: 'so' model, sphere with a blue-white cloud texture, drawn fading at the end */
#define FX_BAHAMUT_SMOKE_ARC       0x86 /* summon data: 'so' model, curved band with a grey smoke texture, stacked nine times as a cone */

/* fx_0e3_0 choreography (docs/fxdata.md, "Choreography literals"; partial: the sizes, buffers, strip shapes, ribbon
 * texture and primitive words; op80's state machine is still numeric). */
#define FX_BAHAMUT_PUFF_SLOTS     0xC  /* op80 pool items (Fx0e30Puff) */
#define FX_BAHAMUT_MODEL_COUNT    0x10 /* btlfx_initEffModels slots */
#define FX_BAHAMUT_ARC_POINTS     13   /* points of a WHITE_BOLT arc (fxm_drawOffsetTrail0, Fx0e30Arc), */
#define FX_BAHAMUT_ARC_WIDTH      0x19 /* their width */
#define FX_BAHAMUT_BOLT_POINTS    49   /* points of a PURPLE_BOLT strip (fxm_drawOffsetTrail1, Fx0e30Bolt), */
#define FX_BAHAMUT_BOLT_WIDTH     0x33 /* their width */
#define FX_BAHAMUT_SUMMON_BONES   92   /* the summon model's bones: random arc ends (Hi_GetSummonBonePos(rand() % this)) */
#define FX_BAHAMUT_RIBBONS        4    /* fx_0e3_0_ribbons / ribbon bone pairs */
#define FX_BAHAMUT_RIBBON_SEGMENTS 15  /* a ribbon's history steps */
#define FX_BAHAMUT_RIBBON_TPAGE   0x9D   /* fxm_drawTexRibbonTrail texture: 8-bit page (0x340, 0x100), */
#define FX_BAHAMUT_RIBBON_CLUT    0x3D40 /* CLUT at (0x0, 0xF5), */
#define FX_BAHAMUT_RIBBON_U       0      /* texels from (u, v) */
#define FX_BAHAMUT_RIBBON_V       0x80

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_BAHAMUT_BOLT_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0e30Bolt;

typedef struct {
    SVECTOR pos0; /* position */
    s32 phase8; /* phase */
    s16 sizeC; /* size */
    s16 speedE; /* speed */
} Fx0e30Puff; /* 0x10 */

typedef struct {
    SVECTOR a0; /* pad: point valid */
    SVECTOR b8;
} Fx0e30Seg; /* 0x10: one step of the ribbon history */

extern Unk800F78A4 *fx_0e3_0_env;
extern s32 fx_0e3_0_alongJitter;
extern s32 fx_0e3_0_ampJitter;
extern s32 fx_0e3_0_angleJitter;

static void fxm_drawTexRibbonTrail(SVECTOR *posA, SVECTOR *posB, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 bright,
                   Fx0e30Seg *buf, s32 abr, s32 phase);
static void fxm_drawOffsetTrail0(Unk80100C78Entry *entry, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade);
static void fxm_drawOffsetTrail1(Unk80100C78Entry *entry, s32 anchor, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang,
                   s32 dang, s32 frame, s32 fade);
static s32 fx_0e3_0_op80PoolTask(s32 mode, Fx0e30Puff *p, Unk800F78A4 *env, Unk80100C78Entry *entry);

typedef struct {
    s32 vx, vy, vz;
} Fx0e30Vec3;

typedef struct {
    u32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR basePos8; /* summon position */
    Fx0e30Vec3 summonPos10; /* summon model position */
    Fx0e30Vec3 summonScale1C; /* summon model scale */
    SVECTOR rot28; /* summon model rotation */
    SVECTOR pos30; /* camera */
    SVECTOR pos38; /* bone position */
    SVECTOR pos40; /* bone position */
    s32 unk48; /* unused (runtime watch, round 33) */
    Pool6CEntry *emit4C;
    s32 trailReset50; /* bolts need a reset */
    s32 model54; /* models */
    s32 model58;
    s32 model5C;
    s32 model60;
    s32 model64;
    SVECTOR unk68; /* unused (runtime watch, round 33) */
    Fx0e30Vec3 unk70; /* unused (runtime watch, round 33) */
    Fx0e30Vec3 unk7C; /* unused (runtime watch, round 33) */
    SVECTOR rot88;
    Fx0e30Vec3 pos90;
    Fx0e30Vec3 scale9C;
} Fx0e30Work80; /* 0xA8 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
        BtlMotion *motion184;
        BtlMotion *motion188;
        BtlMotion *motion18C;
        BtlMotion *motion190;
        BtlMotion *motion194;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx0e30Summon;

typedef struct {
    s16 edgeA0; /* a edge: summon bone (entries 0-3) or its distance ahead (entries 4-7) */
    s16 edgeB2; /* b edge: summon bone (entries 0-3) or its distance ahead (entries 4-7) */
} Fx0e30BonePair;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_BAHAMUT_ARC_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0e30Arc; /* 0x6C */

extern SVECTOR fx_0e3_0_origin;
extern s16 fx_0e3_0_trailBones[];
extern Fx0e30BonePair fx_0e3_0_ribbonBones[8];
extern Fx0e30Seg fx_0e3_0_ribbons[FX_BAHAMUT_RIBBONS][FX_BAHAMUT_RIBBON_SEGMENTS];
extern u8 fx_0e3_0_strips[4][0x12C];
extern Fx0e30Arc fx_0e3_0_arcs[];
extern Fx0e30Bolt fx_0e3_0_bolts[];
extern BtlModelBuf fx_0e3_0_effModels[];

#define SUMMON(env) ((Fx0e30Summon *)(env)->loader)

#endif
