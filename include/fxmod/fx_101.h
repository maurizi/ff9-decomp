#ifndef FX_101_H
#define FX_101_H

/* Effect module fx_101 (dir 13 file 0x101): Trance transformation. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_101 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TRANCE_BLUE_RING      0x1 /* rings: navy-white gouraud rings */
#define FX_TRANCE_AURA_CYLINDER  0x3 /* model: open cylinder, blue-green cloud texture */
#define FX_TRANCE_THIN_STREAKS   0x5 /* emitter: two fans, thin orange and blue streaks */
#define FX_TRANCE_VIOLET_TRAIL   0x8 /* trail: violet sparkle strip, 12 frames */
#define FX_TRANCE_LILAC_RINGS    0x9 /* rings: white / lilac / blue gouraud rings */
#define FX_TRANCE_WHITE_RINGS    0xA /* rings: white-blue gouraud rings */
#define FX_TRANCE_SPARKLE_SPIRAL 0xC /* emitter: small blue-gold sparkles rising in a spiral (first element renders black) */
#define FX_TRANCE_PINK_STREAKS   0xF /* emitter: two orbiting fans, white-pink light streaks */
#define FX_TRANCE_VIOLET_STREAKS 0x10 /* emitter: two orbiting fans, violet-blue light streaks */

/* fx_101 choreography (docs/fxdata.md, "Choreography literals"): op80's three states (the rings and aura cylinder
 * charge, pulse until the script signals, release), the aura's texture and scrolled rect, op82's bolt pool. */
#define FX_TRANCE_STATE_CHARGE    0     /* op80 state: rings and aura grow in */
#define FX_TRANCE_STATE_HOLD      1     /* they pulse until the battle state's signal byte is set (on a pulse boundary) */
#define FX_TRANCE_STATE_RELEASE   2     /* the streak emitters burst, rings fade (op80 ends) */
#define FX_TRANCE_CHARGE_FRAMES   0x10  /* CHARGE lasts this many frames; THIN_STREAKS start and the caster fades */
#define FX_TRANCE_PULSE_FRAMES    10    /* HOLD's pulse period */
#define FX_TRANCE_GLOW_FRAMES     0x10  /* RELEASE: the aura cylinder fades over this many frames */
#define FX_TRANCE_MOTION_FRAME    3     /* RELEASE: the caster's idle motion resets on this frame */
#define FX_TRANCE_MOTION_END_FRAME 0x12 /* and back from this one */
#define FX_TRANCE_RELEASE_FRAMES  47    /* RELEASE lasts this many frames (the (frame << 12) / N fade); the bolts stop */
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
#define FX_TRANCE_BOLT_COUNT      8     /* op82: fx_101_bolts, a ring of bolt buffers, one new bolt per frame */
#define FX_TRANCE_BOLT_POINTS     12    /* points per bolt, each a spark in the pool */
#define FX_TRANCE_BOLT_REACH      0x9B  /* bolt length from the caster's body */
#define FX_TRANCE_SPARK_FRAMES    8     /* a spark (and its bolt's VIOLET_TRAIL) lives this many frames (the << 9 fade) */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pos4[FX_TRANCE_BOLT_POINTS];
} Fx101Bolt; /* 0x64 */

typedef struct {
    s32 posX0; /* position, 20.12 */
    s32 posY4;
    s32 posZ8;
    s16 velXC; /* velocity */
    s16 velYE;
    s16 velZ10;
    s16 drag12; /* velocity factor per frame (btlfx_stepModuleParticle), 0x1000 = none */
    s16 index14; /* bolt */
    s16 index16; /* point index from the tip */
    SVECTOR *pos18; /* bolt point */
} Fx101Spark; /* 0x1C */

typedef struct {
    s32 workSize0; /* work size */
    s32 poolCount4; /* count */
    s32 elemSize8; /* entry size */
    u8 padC[0x14 - 0xC];
    void *elemFn14; /* entry callback */
} Fx101PoolCfg;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14; /* caster body position */
    s32 frame1C; /* loop counter */
    s32 flag20; /* camera shake on */
    s32 model24; /* model */
} Fx101Work80; /* 0x28 */

/* View of Unk800F78A40 up to the byte fx_101 reads (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x10];
    u8 signal10;
} Fx101BattleState;

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR angle8; /* angles */
} Fx101Work82; /* 0x10 */

extern s32 fx_101_stopBolts; /* bolts stop */
extern BtlModelBuf fx_101_effModels[];
extern Unk800F78A4 *fx_101_env;
extern Fx101Bolt fx_101_bolts[FX_TRANCE_BOLT_COUNT];
extern s32 fx_101_nextBolt; /* next bolt */

s32 fx_101_op80(s32 mode, Fx101Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
s32 fx_101_op82PoolTask(s32 mode, Fx101Spark *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void fx_101_resetBolts(void);
void fx_101_spawnBolt(Unk80100C78Entry *entry, SVECTOR *origin, s32 ang, SVECTOR *from, SVECTOR *to);
s32 fx_101_op82(s32 mode, Fx101Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, Fx101PoolCfg *cfg);

#endif
