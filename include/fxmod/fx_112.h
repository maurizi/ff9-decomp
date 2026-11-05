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
} Fx112Work;

extern Unk800F78A4 *fx_112_env;
extern BtlModelBuf fx_112_effModels[];
extern u8 fx_112_op80Trails[][0x2D0];

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
