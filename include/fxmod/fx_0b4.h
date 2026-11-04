#ifndef FX_0B4_H
#define FX_0B4_H

/* Effect module fx_0b4 (dir 13 file 0xB4): enemy Aero (Griffin and others). Six emitters whose particles follow
 * rotation+position paths (fxm_followPathParticle). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0b4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AERO_GREEN_RING   0xE /* emitter: orbiting green gouraud rings */
#define FX_AERO_WIND_RING    0xF /* emitter: orbiting pale green wind-ring particles */
#define FX_AERO_GREEN_BURST  0x10 /* emitter: green gust puffs fading to blue */
#define FX_AERO_SPRITE_SWIRL 0x11 /* emitter: spiralling sprites, render all black (look pending) */
#define FX_AERO_WIND_TRAIL   0x19 /* emitter: white 3D wind trail spiralling */

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx0b4Key; /* 0xC */

typedef struct {
    s16 pad0; /* always 0 in the effect file blocks (runtime, round 33) */
    u16 count2; /* keys */
    Fx0b4Key *keys4;
} Fx0b4Path;

typedef struct {
    u16 start0; /* start frame */
    u16 count2; /* frames since the start */
} Fx0b4Timer;

typedef struct {
    Pool6CEntry *emit0[6];
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
    Pool6CEntry *emit28;
    SVECTOR pos2C; /* caster body position */
    SVECTOR pos34; /* caster position */
} Fx0b4Work;

extern Fx0b4Timer fx_0b4_timers[6];
extern u16 fx_0b4_emitBlocks[6]; /* particle files */
extern u16 fx_0b4_pathBlocks[6]; /* path files */
extern SVECTOR fx_0b4_emitRot;
extern s32 (*fx_0b4_op80ParticleMotions[6])(); /* motion callbacks */
extern Unk800F78A4 *fx_0b4_env;
extern SVECTOR fx_0b4_pathOrigin; /* path origin */
extern SVECTOR fx_0b4_emitStartPos[6];
extern Fx0b4Path fx_0b4_paths[6];
extern s32 fx_0b4_trails[6][0x360];
extern s32 fx_0b4_trails2[];

s32 fx_0b4_op80ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_0b4_op80ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_0b4_op80ParticleMotion2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_0b4_op80ParticleMotion3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_0b4_op80ParticleMotion4(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_0b4_op80ParticleMotion5(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_followPathParticle(Pool30Entry *pt, SVECTOR *pos, Fx0b4Timer *timer, Fx0b4Path *path, s32 frame, s32 n);
static s32 fxm_rotPosPathGet(Fx0b4Path *path, s32 t, SVECTOR *rot, SVECTOR *pos);
typedef struct {
    s16 x, y, z;
} Fx0b4Point;
static void fxm_rotPosPathInit(Fx0b4Path *path, u16 *data);

#endif
