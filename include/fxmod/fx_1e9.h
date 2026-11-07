#ifndef FX_1E9_H
#define FX_1E9_H

/* Effect module fx_1e9 (dir 13 file 0x1e9): Trance command (not in Trance). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1e9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TRANCE_BLUE_RING      0x1 /* rings: navy-white gouraud rings */
#define FX_TRANCE_AURA_CYLINDER  0x3 /* model: open cylinder, blue-green cloud texture (byte-identical to fx_101 entry 0x3) */
#define FX_TRANCE_THIN_STREAKS 0x5 /* emitter: two fans, thin orange and blue light streaks (byte-identical to fx_101 entry 0x5) */
#define FX_TRANCE_VIOLET_TRAIL   0x8 /* trail: violet sparkle strip, 12 frames */
#define FX_TRANCE_LILAC_RINGS    0x9 /* rings: white / lilac / blue gouraud rings (byte-identical to fx_101 entry 0x9) */
#define FX_TRANCE_WHITE_RINGS    0xA /* rings: white-blue gouraud rings */
#define FX_TRANCE_PINK_STREAKS   0xF /* emitter: two orbiting fans, pink-white light streaks */
#define FX_TRANCE_VIOLET_STREAKS 0x10 /* emitter: two orbiting fans, violet light streaks */

/* fx_1e9 choreography (docs/fxdata.md, "Choreography literals"): op80's three states (the rings and aura cylinder
 * charge, pulse until the script signals, release), the aura's texture and scrolled rect, op82's bolt pool. The names and
 * most values of fx_101 (Trance); shorter bolts here, and no motion end key. */
#define FX_TRANCE_STATE_CHARGE    0     /* op80 state: rings and aura grow in */
#define FX_TRANCE_STATE_HOLD      1     /* they pulse until the battle state's signal byte is set (on a pulse boundary) */
#define FX_TRANCE_STATE_RELEASE   2     /* the streak emitters burst, rings fade (op80 ends) */
#define FX_TRANCE_CHARGE_FRAMES   0x10  /* CHARGE lasts this many frames; THIN_STREAKS start and the caster fades */
#define FX_TRANCE_PULSE_FRAMES    10    /* HOLD's pulse period */
#define FX_TRANCE_GLOW_FRAMES     0x10  /* RELEASE: the aura cylinder fades over this many frames */
#define FX_TRANCE_MOTION_FRAME    3     /* RELEASE: the idle motion of unit 0 resets on this frame */
#define FX_TRANCE_RELEASE_FRAMES  0x2F  /* RELEASE lasts this many frames (the (frame << 12) / N fade); the bolts stop */
#define FX_TRANCE_RING_SCALE      0x10FF /* WHITE_RINGS / LILAC_RINGS full size */
#define FX_TRANCE_AURA_WIDTH      0x174 /* the aura cylinder's x / z scale */
#define FX_TRANCE_AURA_CYLINDER_TPAGE 0xB7   /* Hi_RegisterTexEffModel texture: 8-bit page (0x1C0, 0x100), abr 1 */
#define FX_TRANCE_AURA_CYLINDER_CLUT  0x3D80 /* CLUT at (0x0, 0xF6) */
#define FX_TRANCE_AURA_CYLINDER_U     0      /* texture origin (u, v) in the page */
#define FX_TRANCE_AURA_CYLINDER_V     0
#define FX_TRANCE_AURA_VRAM_X     0x1C0 /* btlfx_drawScrollMove rect: u 0 .. 0x3F, v 0x20 .. 0x5F of the aura's 8-bit page */
#define FX_TRANCE_AURA_VRAM_Y     0x120
#define FX_TRANCE_AURA_VRAM_W     0x20
#define FX_TRANCE_AURA_VRAM_H     0x40
#define FX_TRANCE_AURA_SRC_X      0x1E0 /* from this source */
#define FX_TRANCE_AURA_SRC_Y      0x120
#define FX_TRANCE_BOLT_COUNT      8     /* op82: fx_1e9_bolts, a ring of bolt buffers, one new bolt per frame */
#define FX_TRANCE_BOLT_POINTS     9     /* points per bolt, each a spark in the pool */
#define FX_TRANCE_BOLT_REACH      0x80  /* bolt length from the caster's body */
#define FX_TRANCE_SPARK_FRAMES    8     /* a spark (and its bolt's VIOLET_TRAIL) lives this many frames (the << 9 fade) */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14; /* caster body position */
    s32 frame1C; /* loop counter */
    s32 unk20; /* write-only (cleared) */
    s32 model24; /* model */
} Fx1e9Work80; /* 0x28 */

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR angle8; /* angles */
} Fx1e9Work82; /* 0x10 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pos4[FX_TRANCE_BOLT_POINTS];
} Fx1e9Trail; /* 0x4C */

typedef struct {
    s32 posX0; /* position, 20.12 */
    s32 posY4;
    s32 posZ8;
    s16 velXC; /* velocity */
    s16 velYE;
    s16 velZ10;
    s16 drag12; /* velocity factor per frame (btlfx_stepModuleParticle), 0x1000 = none */
    u16 index14; /* trail */
    s16 index16; /* trail point index */
    SVECTOR *pos18; /* trail point */
} Fx1e9Spark; /* 0x1C */

typedef struct {
    u8 pad0[0x10];
    s16 *pool10; /* pool */
} Fx1e9Inst;

extern s32 fx_1e9_stopBolts;
extern Unk800F78A4 *fx_1e9_env;
extern BtlModelBuf fx_1e9_effModels[];
extern Fx1e9Trail fx_1e9_bolts[FX_TRANCE_BOLT_COUNT];
extern s32 fx_1e9_nextBolt;

static s32 fx_1e9_op82PoolTask(s32 mode, Fx1e9Spark *spark, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void fx_1e9_resetBolts(void);
void fx_1e9_spawnBolt(Fx1e9Inst *inst, SVECTOR *origin, s32 dist, SVECTOR *from, SVECTOR *to);

#define TRAIL(e) ((Fx1e9Trail *)((e)->pos18 - 2))

#endif
