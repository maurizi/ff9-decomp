#ifndef FX_112_H
#define FX_112_H

/* Effect module fx_112 (dir 13 file 0x112): Eat (Magic Vice / Mimic, enemy). Captures the screen around the target
 * into VRAM_BTLFX_SCRATCH (fxm_captureScreen). Evidence: [data] the party ability / item tables and enemy sequences
 * that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_112 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_EAT_SPHERE       0x1 /* model: sphere, white texture */
#define FX_EAT_ORBIT_TRAIL  0x2 /* emitter: orbiting screen trail; the strip is still mostly dark in the render; runtime (docs/runtime-findings.md effect-data pass 2): its run-time texture is a brown sphere with highlights; nothing identifiable on screen (look pending) */
#define FX_EAT_SMOKE        0x3 /* emitter: grey smoke clouds, 24 animated frames */
#define FX_EAT_GOLD_CUTLERY 0x4 /* emitter: golden fork, spoon, cup and spinning gold coins */

/* fx_112 choreography (docs/fxdata.md, "Choreography literals"): op80's five states (the target is grabbed into
 * the sphere, swallowed, chewed, then spat back), their keys and the sphere's texture. */
#define FX_EAT_STATE_GRAB      0    /* op80 state: the target stretches, vanishes in SMOKE, the sphere wobbles in the caster's mouth */
#define FX_EAT_STATE_SWALLOW   1    /* the sphere shrinks into the caster */
#define FX_EAT_STATE_CHEW      2    /* ORBIT_TRAILs start one after another at the caster's bone 4 */
#define FX_EAT_STATE_PAUSE     3    /* wait, then GOLD_CUTLERY */
#define FX_EAT_STATE_SPIT      4    /* the target flies back to its place, growing (op80 ends) */
#define FX_EAT_CAPTURE_FRAME   3    /* GRAB: the screen rect round the target is copied to the effect scratch area */
#define FX_EAT_CAPTURE_X       0xA0 /* rect centre on the display */
#define FX_EAT_CAPTURE_Y       0x8C
#define FX_EAT_CAPTURE_W       0x80 /* rect size */
#define FX_EAT_CAPTURE_H       0xC0
#define FX_EAT_VANISH_FRAME    4    /* the target fades out and SMOKE starts; the sphere draws after it */
#define FX_EAT_GRAB_FRAMES     0x20 /* GRAB lasts this many frames */
#define FX_EAT_SWALLOW_FRAMES  9    /* SWALLOW (the (frame << 12) / N path) */
#define FX_EAT_TRAIL_INTERVAL  5    /* CHEW: an ORBIT_TRAIL every this many frames, into the next fx_112_op80Trails slot */
#define FX_EAT_ORBIT_TRAIL_SLOTS  0x14 /* its trail buffer: slots */
#define FX_EAT_ORBIT_TRAIL_STRIDE 0x24 /* bytes per slot */
#define FX_EAT_CHEW_FRAMES     0x13 /* CHEW lasts this many frames */
#define FX_EAT_PAUSE_FRAMES    0x10 /* PAUSE */
#define FX_EAT_SPIT_FRAMES     12   /* SPIT: the target's flight back (the (frame << 12) / N ramps) */
#define FX_EAT_SPIT_END_FRAME  0x10 /* op80 ends */
#define FX_EAT_SPHERE_TPAGE    0x137  /* Hi_RegisterTexEffModel texture: 15-bit page (0x1C0, 0x100), abr 1 */
#define FX_EAT_SPHERE_CLUT     0xFFFF /* no CLUT */
#define FX_EAT_SPHERE_U        0      /* texture origin (u, v) in the page */
#define FX_EAT_SPHERE_V        0

typedef struct {
    s32 vx, vy, vz;
} Fx112Vec3;

typedef struct {
    u32 state0; /* state */
    SVECTOR pos4; /* caster position */
    SVECTOR rotC; /* caster rotation */
    Fx112Vec3 scale14; /* caster scale */
    s32 model20; /* model */
    u8 pad24[0x38 - 0x24];
    SVECTOR pos38; /* caster body position */
    SVECTOR pos40;
    SVECTOR pos48; /* bone position */
    Pool6CEntry *emit50;
} Fx112Work; /* 0x54 */

extern Unk800F78A4 *fx_112_env;
extern BtlModelBuf fx_112_effModels[];
extern u8 fx_112_op80Trails[][FX_EAT_ORBIT_TRAIL_SLOTS * FX_EAT_ORBIT_TRAIL_STRIDE];

static void fxm_captureScreen(s32 x, s32 y, s32 w, s32 h, s32 dx, s32 dy);
s32 fx_112_op80(s32 mode, Fx112Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* fx_112_op80 locals: each state reuses the same stack area (the original frame overlaps them). */
typedef union {
    struct {
        VECTOR scale0;
        SVECTOR rot;
        SVECTOR a;
        SVECTOR b;
        VECTOR scale;
        VECTOR pos;
    } s0;
    struct {
        SVECTOR rot;
        SVECTOR v;
        SVECTOR pos;
        SVECTOR bone;
        VECTOR scale;
        VECTOR tr;
    } s1;
    struct {
        SVECTOR rot;
        SVECTOR v;
        BtlBone m;
        VECTOR scale;
        SVECTOR pad;
        SVECTOR r;
    } s4;
} Fx112Locals;

#endif
