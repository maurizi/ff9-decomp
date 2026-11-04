#ifndef FX_1F3_H
#define FX_1F3_H

/* Effect module fx_1f3 (dir 13 file 0x1f3): enemy Neutron Ring (Dummy/Necron). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_NEUTRON_RING_FIRE_DISC       0x1 /* 'so' model: flat ring disc, orange-teal fire texture */
#define FX_NEUTRON_RING_LIGHTNING_FLASH 0x3 /* emitter: white glow flashes and violet lightning */
#define FX_NEUTRON_RING_LIGHT_STREAKS   0x4 /* emitter: two fans, pale yellow light streaks */
#define FX_NEUTRON_RING_ORBIT_TRAIL     0x5 /* emitter: orbiting 3D trail, white streak texture */
#define FX_NEUTRON_RING_YELLOW_RINGS    0x7 /* emitter: pale yellow and yellow-green gouraud rings */
#define FX_NEUTRON_RING_TEAL_TORUS      0x8 /* 'so' model: teal ring torus */
#define FX_NEUTRON_RING_FIRE_BOWL       0x9 /* 'so' model: bowl, orange-teal fire texture */
#define FX_NEUTRON_RING_COLOR_RINGS     0xB /* emitter: white-lilac, peach-pink and teal-orange gouraud rings */
#define FX_NEUTRON_RING_TEAL_ARC        0xD /* 'so' model: teal arc band */
#define FX_NEUTRON_RING_LIGHT_STREAKS_2 0xE /* emitter: two fans, pale yellow light streaks */
#define FX_NEUTRON_RING_YELLOW_RINGS_2  0xF /* emitter: yellow / pale yellow gouraud rings */
#define FX_NEUTRON_RING_BACKDROP        0x10 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_NEUTRON_RING_VIOLET_TRAIL    0x11 /* trail: violet noise strip */
#define FX_NEUTRON_RING_VIOLET_RINGS    0x12 /* emitter: white-lilac, peach and violet gouraud rings */
#define FX_NEUTRON_RING_GREY_DISC       0x13 /* 'so' model: flat ring disc, grey streak texture */
#define FX_NEUTRON_RING_GREEN_DISC      0x14 /* 'so' model: flat ring disc, green-yellow gouraud */

/* copied */
extern Unk800F78A4 *fx_1f3_env;
/* end copied */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1f3Vec3;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[33];
} Fx1f3Line; /* 0x10C */

typedef struct {
    MATRIX matrix0; /* bone matrix */
    Fx1f3Line trail20;
    s32 timer12C; /* timer */
    s32 bone130; /* bone */
    s32 angle134; /* angle */
    s32 spin138; /* angle speed */
    s32 radius13C; /* radius */
    s32 angleStep140; /* angle step */
} Fx1f3Ring; /* 0x144 */

typedef struct {
    s32 model0; /* model */
    s32 spin4; /* spin */
    SVECTOR rot8; /* rotation */
    Fx1f3Vec3 scale10; /* scale */
    u8 pad1C[4];
    Fx1f3Vec3 pos20; /* position */
    u8 pad2C[4];
} Fx1f3Rock; /* 0x30 */

typedef struct {
    s32 bone0; /* bone */
    s32 radius4; /* radius */
    s32 unk8; /* not read by the module */
} Fx1f3RingDef;

typedef struct {
    s32 timer0[16]; /* timers */
    Pool6CEntry *emit40[7];
    SVECTOR pos5C; /* target position */
    SVECTOR pos64; /* bone position */
    s32 model6C; /* model */
    Fx1f3Vec3 pos70; /* model position */
    u8 pad7C[4];
    Fx1f3Vec3 scale80; /* model scale */
    u8 pad8C[4];
    SVECTOR rot90; /* model rotation */
    s32 model98; /* models */
    s32 model9C;
    SVECTOR rotA0;
    SVECTOR rotA8;
    Fx1f3Vec3 scaleB0;
    u8 padBC[4];
    s32 modelC0;
    s32 modelC4;
    SVECTOR rotC8;
    SVECTOR rotD0;
    Fx1f3Vec3 scaleD8;
    u8 padE4[4];
    s32 modelE8;
    s32 modelEC;
    SVECTOR rotF0;
    Fx1f3Vec3 scaleF8;
    u8 pad104[4];
    SVECTOR pos108; /* camera */
    SVECTOR pos110;
    SVECTOR pos118;
} Fx1f3Work80; /* 0x120 */

extern Fx1f3RingDef fx_1f3_ringDefs[16];
extern BtlModelBuf fx_1f3_effModels[];
extern s32 fx_1f3_op80Trails[];
extern Fx1f3Rock fx_1f3_spinModels[8];
extern Fx1f3Ring fx_1f3_rings[16];
extern Fx1f3Line fx_1f3_ringTrails;
static void fxm_tintScreenRatio(s32 t, s32 n, s32 from, s32 to, s32 abr, s32 ot);
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
