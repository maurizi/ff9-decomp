#ifndef FX_1A0_H
#define FX_1A0_H

/* Effect module fx_1a0 (dir 13 file 0x1a0): enemy Flame Slash (Maliris). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLAME_SLASH_RED_RINGS   0x1 /* emitter: red gouraud rings, 16 frames + dark red speck sprites */
#define FX_FLAME_SLASH_FIRE_BURST  0x3 /* emitter: fire explosion sprites + red spark sprites */
#define FX_FLAME_SLASH_FLAME_FLASH 0x4 /* emitter: white-hot flash turning red and fading + grey smoke */
#define FX_FLAME_SLASH_FLAME_RINGS 0x13 /* emitter: rings textured with a dark flame wall + red gouraud rings + red specks */

/* fx_1a0 choreography (docs/fxdata.md, "Choreography literals"): op80's fire bursts on the caster's blade bones,
 * op81's path-following flames (one per path index, trail buffers), the ops' lengths. */
#define FX_FLAME_SLASH_BURSTS           6    /* op80: FIRE_BURST emitters, one per entry of fx_1a0_op80Bones */
#define FX_FLAME_SLASH_BURSTS_FOLLOW_FRAMES 0xC /* op80: they follow their bones (btlfx_moveEmitterTo) while frame < this */
#define FX_FLAME_SLASH_BURSTS_FRAMES    0x14 /* op80's length (the op ends) */
#define FX_FLAME_SLASH_PATHS            6    /* op81: path indices (args[0]): file paths, emitters, particle motions, trails */
#define FX_FLAME_SLASH_PATH_TRAIL_SLOTS  1    /* op81: each path emitter's trail buffer fx_1a0_op81Trails[i]: slots of */
#define FX_FLAME_SLASH_PATH_TRAIL_STRIDE 0x70 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_FLAME_SLASH_PATH_FRAMES      0x14 /* op81's length (the op ends) */
#define FX_FLAME_SLASH_RINGS_FRAMES     0x14 /* op82 (FLAME_RINGS at the target): its length */

typedef struct {
    s16 x, y, z;
} Fx1a0Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1a0Point *keys4;
} Fx1a0Path;

typedef struct {
    u16 bone0; /* bone */
    u16 dist2; /* distance */
} Fx1a0Bone;

typedef struct {
    Pool6CEntry *emit0[FX_FLAME_SLASH_BURSTS];
    SVECTOR pos18[FX_FLAME_SLASH_BURSTS];
} Fx1a0Work80; /* 0x48 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
    s32 index14; /* index */
    s32 flag18;
} Fx1a0Work81; /* 0x1C */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx1a0Work82; /* 0xC */

extern Fx1a0Bone fx_1a0_op80Bones[FX_FLAME_SLASH_BURSTS];
extern s16 fx_1a0_op81EmitFileIds[FX_FLAME_SLASH_PATHS];
extern s16 fx_1a0_pathFileIds[FX_FLAME_SLASH_PATHS];
extern s32 (*fx_1a0_op81ParticleMotions[FX_FLAME_SLASH_PATHS])();
extern Unk800F78A4 *fx_1a0_env;
extern s32 fx_1a0_op81Trails[FX_FLAME_SLASH_PATHS][FX_FLAME_SLASH_PATH_TRAIL_SLOTS * FX_FLAME_SLASH_PATH_TRAIL_STRIDE / 4];
extern Fx1a0Path fx_1a0_posPaths[FX_FLAME_SLASH_PATHS];
extern s32 fx_1a0_pathFrames[FX_FLAME_SLASH_PATHS]; /* frames */
extern SVECTOR fx_1a0_pathOrigin;
extern SVECTOR fx_1a0_emitPos[FX_FLAME_SLASH_BURSTS];
extern SVECTOR fx_1a0_emitPos1[FX_FLAME_SLASH_PATHS];

s32 fx_1a0_op81ParticleMotion0(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1a0_op81ParticleMotion1(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1a0_op81ParticleMotion2(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1a0_op81ParticleMotion3(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1a0_op81ParticleMotion4(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1a0_op81ParticleMotion5(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_followPathParticle(Pool30Entry *pt, SVECTOR *pos, Fx1a0Path *path, s32 t, s32 frame, s32 frames);
static void fxm_posPathInit(Fx1a0Path *path, u16 *data);
static s32 fxm_posPathGet(Fx1a0Path *path, s32 t, SVECTOR *out);

#endif
