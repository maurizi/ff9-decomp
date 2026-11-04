#ifndef FX_1BF_1_H
#define FX_1BF_1_H

/* Effect module fx_1bf_1 (dir 13 file 0x1bf): Ark (short); Eternal Darkness (load group 1). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1bf_1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_LIGHT_STREAKS 0x1 /* emitter: two fans, thin pale orange light streaks (byte-identical to fx_17d_7 entry 0x2) */
#define FX_ARK_ROCK_DEBRIS_2   0x3 /* emitter: grey-blue rock chunks (byte-identical to fx_17d_7 entry 0x4) */
#define FX_ARK_PALE_RING       0x5 /* emitter: white-cream-violet and white-peach-mauve gouraud rings (byte-identical to fx_17d_7 entry 0x6) */
#define FX_ARK_VIOLET_RING     0x6 /* emitter: white-cream-violet and white-purple-blue gouraud rings (byte-identical to fx_17d_7 entry 0x7) */
#define FX_ARK_GOLD_GLOW       0x9 /* emitter: golden glow flash (byte-identical to fx_17d_7 entry 0xA) */
#define FX_ARK_PEACH_RING      0xC /* rings: white-peach-mauve gouraud rings (byte-identical to fx_17d_6 entry 0xB) */
#define FX_ARK_ROCK_DEBRIS     0xE /* emitter: grey-blue rock chunks (byte-identical to fx_17d_7 entry 0x1) */
#define FX_ARK_ORANGE_RING     0xF /* emitter: two fans (not rendered) + white-orange gouraud rings (byte-identical to fx_17d_7 entry 0xD) */
#define FX_ARK_ORANGE_BEAM 0x86 /* summon data: 'so' model, glowing red-orange beam bar */
#define FX_ARK_TEAL_RING 0x88 /* summon data: 'so' model, dark green rippled crater ring */
#define FX_ARK_FLAME_TEX_RING 0x89 /* summon data: 'so' model, flat ring disc with an orange flame texture */
#define FX_ARK_SMOKE_BOWL      0x8A /* summon data: 'so' model, bowl with a dark cloud texture */
#define FX_ARK_GREY_TORUS      0x8C /* summon data: 'so' model, grey gouraud torus */
#define FX_ARK_EMBER_RING 0x8D /* summon data: 'so' model, torus with a brown rock texture */
#define FX_ARK_GOLD_RING 0x8E /* summon data: 'so' model, gold gouraud torus */
#define FX_ARK_BLUE_CLOUD_RING 0x8F /* summon data: 'so' model, bowl with a twisted blue cloud texture */
#define FX_ARK_ROCK            0x90 /* summon data: 'so' model, rock chunk */
#define FX_ARK_FIRE_CONE       0x91 /* summon data: 'so' model, open cone with a dark flame texture */
#define FX_ARK_ROCK_2          0x92 /* summon data: 'so' model, rock chunk */
#define FX_ARK_ROCK_3          0x93 /* summon data: 'so' model, rock chunk */
#define FX_ARK_FIRE_ARC        0x94 /* summon data: 'so' model, curved band with an orange flame texture */

typedef struct {
    s16 state0; /* state */
    s16 phase2; /* sub-state */
    s16 vel4; /* velocity */
    s16 vel6;
    s16 vel8;
    s16 scaleA; /* scale */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s16 spin1C; /* spin */
    s16 spin1E;
    s16 index20; /* model index */
    s16 offset22;
} Fx1bf1Rock;

typedef struct {
    u8 pad0[0x84];
    s32 models84[1]; /* models */
} Fx1bf1RockInst;

typedef struct {
    s32 frame0; /* frame */
    u8 pad4[0xC];
    s16 *pool10; /* pool */
} Fx1bf1Inst;

typedef struct {
    s32 *noise0; /* noise */
    s32 count4; /* spawn count */
    s32 kind8; /* kind */
} Fx1bf1Work83;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[1];
} Fx1bf1Trail;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[17];
} Fx1bf1Trail17; /* 0x8C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[5];
} Fx1bf1Trail5; /* 0x2C */

typedef struct {
    SVECTOR pos0; /* to */
    SVECTOR pos8; /* from */
    u8 index10; /* trail index */
    u8 kind11; /* kind */
    u8 index12;
    u8 unk13; /* write-only (rand % 15) */
    u8 angle14; /* angle */
    u8 width15;
    u8 pad16[2];
    s32 *noise18; /* noise */
} Fx1bf1Bolt;

extern Fx1bf1Trail17 fx_1bf_1_boltTrails[];
extern Fx1bf1Trail5 fx_1bf_1_branchTrails[];

static void fxm_drawArcTrailBranch(Unk80100C78Entry *entry, s32 file, s32 mode, Fx1bf1Trail *path, Fx1bf1Trail *trail, s32 size, s32 *noise,
                   s32 branchAt, s32 frame, s32 fade, s32 lineWidth);

/* copied */
extern Unk800F78A4 *fx_1bf_1_env;
extern s32 fx_1bf_1_ringRadius; /* radius */
static void fxm_drawArcTrail(Unk80100C78Entry *entry, s32 file, s32 mode, Fx1bf1Trail *trail, s32 size, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 amp0, s32 amp1, s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 dip);
static s32 fx_1bf_1_op83PoolTask(s32 mode, Fx1bf1Bolt *bolt, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
extern s32 fx_1bf_1_noise[64][3]; /* noise */
extern SVECTOR fx_1bf_1_boltPos0;
extern SVECTOR fx_1bf_1_boltPos1;
extern s32 fx_1bf_1_releaseZ;
/* end copied */

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only: frame counter */
    SVECTOR pos8; /* position */
    SVECTOR pos10;
    SVECTOR pos18; /* camera */
    u8 pad20[8];
    s32 pos28[3]; /* model position */
    SVECTOR rot34;
    SVECTOR rot3C;
    s32 scale44[3]; /* model scale */
    s32 pos50[3];
    SVECTOR rot5C;
    Pool6CEntry *emit64;
    Pool6CEntry *emit68;
    Pool6CEntry *emit6C;
    s32 model70; /* models */
    s32 model74;
    s32 model78;
    s32 model7C;
    s32 model80;
    s32 model84;
    s32 model88;
    s32 model8C;
    s32 model90;
    s32 model94;
    s32 model98;
    s32 model9C;
    s32 modelA0;
} Fx1bf1Work80; /* 0xA4 */

extern BtlModelBuf fx_1bf_1_effModels[];
extern SVECTOR fx_1bf_1_origin;
extern s16 fx_1bf_1_spawnOffsets[];
static s32 fxm_randRingPoint(SVECTOR *out, SVECTOR *rot);
static s32 fx_1bf_1_op80PoolTask(s32 mode, Fx1bf1Rock *rock, Unk800F78A4 *env, s32 entry, s32 frame, s32 pool, Fx1bf1RockInst *inst);

#endif
