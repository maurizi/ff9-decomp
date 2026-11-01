#ifndef FX_0E0_H
#define FX_0E0_H

/* Effect module fx_0e0 (dir 13 file 0xE0): Sword Magic Doomsday. Objects (pool items) pulled into a centre point,
 * offset trails (fxm_drawOffsetTrail) and a ripple grid. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* time */
    SVECTOR pos8; /* caster bone position */
} Fx0e0Work84;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[25]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0e0Bolt;

extern Unk800F78A4 *fx_0e0_env;

static void fxm_drawMirrorSprite(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 abr);
static void fxm_drawTiledStrip(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 abr, s32 flip);
static void fxm_drawSprite3D(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr);
static void fxm_drawBulgeGrid24(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr);
static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0e0Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth);
s32 fx_0e0_op84(s32 mode, Fx0e0Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

typedef struct {
    s16 state0; /* state */
    s16 phase2; /* sub-state */
    SVECTOR vel4; /* velocity (vx, vy, vz); pad = model scale index */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s16 spinX1C; /* rotation speed */
    s16 spinY1E;
    Pool6CEntry *emit20; /* particle */
} Fx0e0Obj;

typedef struct {
    u8 pad0[8];
    SVECTOR pos8; /* target */
    u8 pad10[0x5C - 0x10];
    s32 model5C; /* model */
    u8 pad60[0x70 - 0x60];
    s32 model70; /* model */
} Fx0e0Inst;

extern s32 fx_0e0_objScales[];
/* Dead: both 0 in the module data, read only by the pool items' state 0/1 branch, and fx_0e0_op80 spawns its pool
 * items with state0 = 3 (no reads over a whole Doomsday Sword, runtime round 33). */
extern s32 fx_0e0_captureRadius;
extern s32 fx_0e0_endEarly;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0e0Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* position */
    SVECTOR pos10; /* target */
    SVECTOR camAt18; /* camera target */
    u8 pad20[4];
    Fx0e0Vec3 pos24; /* model position */
    Fx0e0Vec3 scale30; /* model scale */
    SVECTOR rot3C; /* model rotation */
    Fx0e0Vec3 scale44; /* scale */
    SVECTOR rot50; /* rotation */
    u8 pad58[4];
    s32 model5C; /* models */
    s32 model60;
    s32 model64;
    s32 model68;
    s32 model6C;
    s32 model70;
    s32 model74;
    u8 pad78[0xC];
} Fx0e0Work80; /* 0x84 */

typedef struct {
    s16 state0; /* state */
    s16 unk2; /* unused */
    s16 velX4;
    s16 velY6;
    s16 velZ8;
    s16 unkA; /* write-only */
    s16 posXC;
    s16 posYE;
    s16 posZ10;
    s16 unk12; /* unused */
    s16 rotX14;
    s16 rotY16;
    s16 rotZ18;
} Fx0e0Spark;

typedef union {
    VECTOR v;
    struct {
        CVECTOR col;
        s32 pad;
        SVECTOR rot;
    } s;
} Fx0e0Tmp;

extern SVECTOR fx_0e0_centerPos;
extern CVECTOR fx_0e0_rippleColors[];
extern Fx0e0Bolt fx_0e0_trailStrips[];
extern BtlModelBuf fx_0e0_effModels[];

#endif
