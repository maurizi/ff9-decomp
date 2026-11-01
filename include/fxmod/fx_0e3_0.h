#ifndef FX_0E3_0_H
#define FX_0E3_0_H

/* Effect module fx_0e3_0 (dir 13 file 0x0E3): Bahamut, full summon (load group 0). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[49]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0e30Bolt;

typedef struct {
    SVECTOR pos0; /* position */
    s32 phase8; /* phase */
    s16 sizeC; /* size */
    s16 speedE; /* speed */
} Fx0e30Puff;

typedef struct {
    SVECTOR a0; /* pad: point valid */
    SVECTOR b8;
} Fx0e30Seg; /* 0x10: one step of the ribbon history */

extern Unk800F78A4 *fx_0e3_0_env;
extern s32 fx_0e3_0_alongJitter;
extern s32 fx_0e3_0_ampJitter;
extern s32 fx_0e3_0_angleJitter;

static void fxm_drawTexRibbonTrail(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0e30Seg *buf, s32 abr, s32 phase);
static void fxm_drawOffsetTrail0(Unk80100C78Entry *entry, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9);
static void fxm_drawOffsetTrail1(Unk80100C78Entry *entry, s32 arg1, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang,
                   s32 dang, s32 arg8, s32 arg9);
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
    s16 unk0;
    s16 unk2;
} Fx0e30BonePair;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[13]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0e30Arc; /* 0x6C */

extern SVECTOR fx_0e3_0_origin;
extern s16 fx_0e3_0_trailBones[];
extern Fx0e30BonePair fx_0e3_0_ribbonBones[8];
extern Fx0e30Seg fx_0e3_0_ribbons[4][15];
extern u8 fx_0e3_0_strips[4][0x12C];
extern Fx0e30Arc fx_0e3_0_arcs[];
extern Fx0e30Bolt fx_0e3_0_bolts[];
extern BtlModelBuf fx_0e3_0_effModels[];

#define SUMMON(env) ((Fx0e30Summon *)(env)->loader)

#endif
