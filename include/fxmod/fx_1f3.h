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
/* fx_1f3 choreography (docs/fxdata.md, "Choreography literals"): op80's timeline, its countdown slots (work->timer0
 * indices) with the lengths written into them, the violet trail rings, and the VRAM rects of the scrolled textures. */
#define FX_NEUTRON_RING_TIMERS      16   /* work->timer0[]: countdowns (cleared at init): */
#define FX_NEUTRON_RING_T_DISC      0    /*   the FIRE_DISC model, facing the camera, fades in over its first 8 frames */
#define FX_NEUTRON_RING_T_DISC_SHRINK 1  /*   the disc shrinks away (eased 0x3000 -> 0) */
#define FX_NEUTRON_RING_T_FLASH     2    /*   an additive white screen tint (fxm_tintScreenRatio 0xFF / 0) */
#define FX_NEUTRON_RING_T_WHITE     3    /*   another (0 / 0xFF) */
#define FX_NEUTRON_RING_T_ARCS      4    /*   the TEAL_ARC spin models slow down and fade (the last 0x40 frames) */
#define FX_NEUTRON_RING_T_BOWLS     5    /*   the two FIRE_BOWLs grow, fading over the last 0x10 frames */
#define FX_NEUTRON_RING_T_TORI      6    /*   the two TEAL_TORUSes grow, the same */
#define FX_NEUTRON_RING_T_RINGS     7    /*   the violet trail rings spawn at the caster's bones */
#define FX_NEUTRON_RING_T_DISCS     8    /*   GREY_DISC and GREEN_DISC grow, the same */
#define FX_NEUTRON_RING_T_CAM_DOWN  9    /*   camera point eased from pos108 to the caster's home (pos118) */
#define FX_NEUTRON_RING_T_CAM_UP    10   /*   and back */
#define FX_NEUTRON_RING_MODEL_SLOTS 0x20 /* btlfx_initEffModels slots */
#define FX_NEUTRON_RING_RINGS       16   /* VIOLET_TRAIL rings (fx_1f3_rings, fx_1f3_ringDefs: bone and radius), two sets of 8 */
#define FX_NEUTRON_RING_RING_SET    8    /*   one ring of each set spawns per frame while T_RINGS is above RING_FRAMES */
#define FX_NEUTRON_RING_RING_FRAMES 8    /*   a ring lives this many frames (timer12C), spinning down over the last 4 */
#define FX_NEUTRON_RING_RING_POINTS 0x21 /*   points of its circle strip */
#define FX_NEUTRON_RING_ARCS        8    /* TEAL_ARC spin models (fx_1f3_spinModels) */
#define FX_NEUTRON_RING_RINGS_FRAMES 0x4A /* from frame 0: T_RINGS; T_CAM_DOWN for CAM_FRAMES */
#define FX_NEUTRON_RING_CAM_FRAMES  0x20
#define FX_NEUTRON_RING_STREAKS_FRAME 0x22 /* LIGHT_STREAKS_2 start at the caster's bone 0x2E */
#define FX_NEUTRON_RING_YELLOW_RINGS_2_FRAME 0x2A /* YELLOW_RINGS_2 start there */
#define FX_NEUTRON_RING_YELLOW_RINGS_FRAME 0x52 /* YELLOW_RINGS start at the target centre (pos5C) */
#define FX_NEUTRON_RING_ORBIT_TRAIL_FRAME 0x62 /* ORBIT_TRAIL and LIGHT_STREAKS start */
#define FX_NEUTRON_RING_ORBIT_TRAIL_TRAIL_SLOTS  0x24 /* ORBIT_TRAIL's trail buffer (fx_1f3_op80Trails): slots of */
#define FX_NEUTRON_RING_ORBIT_TRAIL_TRAIL_STRIDE 0xCC /*   this many bytes */
#define FX_NEUTRON_RING_DISC_FRAME  0x72 /* T_DISC starts, */
#define FX_NEUTRON_RING_DISC_FRAMES 0x80 /*   this long (cut short at STOP_FRAME) */
#define FX_NEUTRON_RING_DISC_FADE_FRAMES 8 /* the disc fades in over this many frames */
#define FX_NEUTRON_RING_LIGHTNING_FRAME 0x7E /* LIGHTNING_FLASH starts */
#define FX_NEUTRON_RING_VIOLET_RINGS_FRAME 0x92 /* LIGHTNING_FLASH stops, VIOLET_RINGS start, T_DISC_SHRINK */
#define FX_NEUTRON_RING_DISC_SHRINK_FRAMES 0x10
#define FX_NEUTRON_RING_FLASH_FRAME 0x9A /* T_FLASH */
#define FX_NEUTRON_RING_FLASH_FRAMES 4
#define FX_NEUTRON_RING_STOP_FRAME  0x9D /* the disc goes, the emitters stop */
#define FX_NEUTRON_RING_BURST_FRAME 0x9E /* COLOR_RINGS start; T_WHITE, T_BOWLS, T_TORI, T_DISCS, T_ARCS */
#define FX_NEUTRON_RING_WHITE_FRAMES 8
#define FX_NEUTRON_RING_GROW_FRAMES 0x20 /*   (the bowls', tori's and discs' length) */
#define FX_NEUTRON_RING_GROW_FADE_FRAMES 0x10 /* they fade over their last this many frames */
#define FX_NEUTRON_RING_ARCS_FRAMES 0x60
#define FX_NEUTRON_RING_ARCS_FADE_FRAMES 0x40
#define FX_NEUTRON_RING_CAM_UP_FRAME 0xBE /* T_CAM_UP (CAM_FRAMES) */
#define FX_NEUTRON_RING_FRAMES      0xFE /* the task ends at this frame */
#define FX_NEUTRON_RING_FIRE_VRAM_X 0x240 /* FIRE_DISC's and FIRE_BOWL's texture (tpage 0x99: 8-bit page (0x240, 0x100)): */
#define FX_NEUTRON_RING_FIRE_VRAM_Y 0x100 /*   btlfx_drawScrollMove scrolls it 8 texels a frame from the strip below */
#define FX_NEUTRON_RING_FIRE_VRAM_W 0x40
#define FX_NEUTRON_RING_FIRE_VRAM_H 0x80
#define FX_NEUTRON_RING_FIRE_SRC_Y  0x180
#define FX_NEUTRON_RING_GREY_DISC_VRAM_X 0x200 /* GREY_DISC's (tpage 0x97 at u 0x80: x 0x200), same rows, from y 0x180 */
#define FX_NEUTRON_RING_TORUS_VRAM_X 0x2C0 /* TEAL_TORUS's (tpage 0x1B: 4-bit page (0x2C0, 0x100)), from x 0x2E0 */
#define FX_NEUTRON_RING_TORUS_VRAM_W 0x20
#define FX_NEUTRON_RING_TORUS_SRC_X  0x2E0

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
    s32 timer0[FX_NEUTRON_RING_TIMERS]; /* timers */
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

extern Fx1f3RingDef fx_1f3_ringDefs[FX_NEUTRON_RING_RINGS];
extern BtlModelBuf fx_1f3_effModels[];
extern s32 fx_1f3_op80Trails[];
extern Fx1f3Rock fx_1f3_spinModels[FX_NEUTRON_RING_ARCS];
extern Fx1f3Ring fx_1f3_rings[FX_NEUTRON_RING_RINGS];
extern Fx1f3Line fx_1f3_ringTrails;
static void fxm_tintScreenRatio(s32 t, s32 n, s32 from, s32 to, s32 abr, s32 ot);
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
