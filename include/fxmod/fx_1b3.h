#ifndef FX_1B3_H
#define FX_1B3_H

/* Effect module fx_1b3 (dir 13 file 0x1b3): enemy Death1 (Dummy/Necron). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH1_DARK_NEBULA        0x2 /* emitter: dark violet nebula particles + pale-yellow light-streak fans */
#define FX_DEATH1_LIGHTNING_TRAILS   0x4 /* emitter: orbiting 3D trails with violet lightning strips + dark violet nebula particles */
#define FX_DEATH1_TEAL_RINGS         0x5 /* emitter: white / grey / violet and teal / orange gouraud rings */
#define FX_DEATH1_GOLD_RAYS          0x6 /* emitter: two fan elements, the pale-gold light rays fanning out (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently); runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_DEATH1_YELLOW_STREAKS     0x7 /* emitter: pale-yellow light-streak fans + white-violet and cream-crimson gouraud rings */
#define FX_DEATH1_EXPLOSIONS         0x8 /* emitter: fire explosion quads, three elements, + speck sprites */
#define FX_DEATH1_ROCKS              0x9 /* emitter: grey rock particles + magenta patterned particles */
#define FX_DEATH1_BACKDROP           0xA /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_DEATH1_TAN_STREAKS        0xB /* emitter: tan light-streak fans + white-violet gouraud rings with a silver nebula texture */
#define FX_DEATH1_GREEN_SMOKE        0xC /* emitter: teal grid quads + pale-green smoke puffs + speck sprites */
#define FX_DEATH1_ROCK_DEBRIS        0xD /* emitter: grey rock quads, three elements */
#define FX_DEATH1_ROCK_DEBRIS_2      0xE /* emitter: orbiting grey rock quads */
#define FX_DEATH1_TEAL_RINGS_2       0x10 /* emitter: white / grey / violet and teal / orange gouraud rings */
#define FX_DEATH1_RIPPLE_RINGS       0x11 /* emitter: rings textured with orange and grey ripple patterns */
#define FX_DEATH1_LIGHTNING_TRAILS_2 0x12 /* emitter: orbiting 3D trails with violet lightning strips + white / navy / cream gouraud rings */
#define FX_DEATH1_ORANGE_RINGS       0x13 /* emitter: white-orange-plum gouraud rings, 40 frames */
#define FX_DEATH1_TAN_STREAKS_2      0x14 /* emitter: two tan light-streak fans */
#define FX_DEATH1_LIGHTNING          0x15 /* trail: violet lightning strip */
#define FX_DEATH1_SCREEN_TRAIL       0x39 /* emitter: orbiting screen trails (strip texture not resolved) + speck sprites; runtime (docs/runtime-findings.md effect-data pass 2): not fetched within 1000 frames of a forced cast (Zidane as the caster) (look pending) */
#define FX_DEATH1_SCREEN_TRAIL_2     0x3A /* emitter: orbiting screen trails (strip texture not resolved) + speck sprites; runtime (docs/runtime-findings.md effect-data pass 2): not fetched within 1000 frames of a forced cast (Zidane as the caster) (look pending) */
#define FX_DEATH1_SCRATCH_BUF        0x93 /* summon data entry 0x13 (0x93 - 0x80): its address is the base of the module's trail and split-piece work buffers */

typedef struct {
    s16 velX0; /* velocity */
    s16 velY2;
    s16 velZ4;
    s16 timer6; /* frames left */
    s16 rot8; /* spin */
    s16 unkA; /* write-only (random +-0x80) */
    s16 unkC; /* write-only (random +-0x80) */
    s16 scaleStepE; /* scale step */
    s16 posX10; /* center */
    s16 posY12;
    s16 posZ14;
    s16 scale16; /* scale */
} Fx1b3Piece; /* 0x18 */

typedef struct {
    s32 model0; /* model */
    s32 timer4; /* frames left */
    s32 count8[8]; /* primitive counts */
    SVECTOR *verts28[8]; /* vertices */
    Fx1b3Piece *pieces48[8]; /* pieces */
} Fx1b3Split;

extern u8 *fx_1b3_splitBufPtr;

typedef struct {
    s32 mode0; /* mode */
    s16 posX4; /* target */
    s16 posY6;
    s16 posZ8;
    s16 padA;
    s32 radiusC; /* radius */
    s32 angle10; /* sweep angle */
    s32 spin14; /* sweep step */
} Fx1b3Blast;

extern Fx1b3Blast fx_1b3_splitTrigger;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[9]; /* drawn points */
    s32 timer4C; /* frames left */
    SVECTOR pos50[9]; /* points */
    SVECTOR vel98[9]; /* point velocities */
} Fx1b3Trail; /* 0xE0 */

typedef struct {
    s32 timers0[32]; /* timers: 0 tint, 1 flash, 2 trails, 3 trail kind, 4 sweep, 5 sweep split, 6..0xD */
    Pool6CEntry *emit80[32];
    SVECTOR pos100; /* body position */
    SVECTOR pos108; /* unit position */
    SVECTOR pos110;
    SVECTOR pos118;
    SVECTOR pos120;
    SVECTOR rot128;
    SVECTOR rot130;
    SVECTOR rot138;
    SVECTOR rot140;
    SVECTOR rot148;
    SVECTOR rot150;
    s32 model158; /* model */
    SVECTOR rot15C;
    SVECTOR rot164;
    VECTOR pos16C;
    VECTOR scale17C;
    SVECTOR rot18C;
    SVECTOR rot194;
    VECTOR pos19C;
    VECTOR pos1AC;
    VECTOR scale1BC;
    SVECTOR rot1CC;
    VECTOR pos1D4;
    VECTOR scale1E4;
    SVECTOR pos1F4;
    SVECTOR rot1FC;
    SVECTOR rot204;
    VECTOR pos20C;
    VECTOR pos21C;
    VECTOR scale22C;
    SVECTOR rot23C;
    VECTOR pos244;
    VECTOR scale254;
} Fx1b3Work80; /* 0x264 */

extern s32 fx_1b3_trailBones0[];
extern s32 fx_1b3_trailBones1[];
extern s32 fx_1b3_trailBones2[];
extern BtlTexInfo fx_1b3_casterTexInfo;
extern BtlTexInfo fx_1b3_casterTexPos;
extern u8 *fx_1b3_splitBuf;
extern Fx1b3Trail *fx_1b3_trails;
extern s32 fx_1b3_op80Trails0;
extern s32 fx_1b3_op80Trails1;
extern Fx1b3Split fx_1b3_splits0[9];
extern Fx1b3Split fx_1b3_splits1[2];
extern Fx1b3Split fx_1b3_splits2;
extern Fx1b3Split fx_1b3_splits3[2];
extern Fx1b3Split fx_1b3_splits4[3];
extern SVECTOR fx_1b3_emitPos0;
extern BtlModelBuf fx_1b3_effModels[];

void fx_1b3_splitModel(Fx1b3Split *sp);
void fx_1b3_updateSplitPieces(Fx1b3Split *sp);

/* copied */
extern Unk800F78A4 *fx_1b3_env;
/* end copied */

#endif
