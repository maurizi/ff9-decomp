#ifndef FX_1A0_H
#define FX_1A0_H

/* Effect module fx_1a0 (dir 13 file 0x1a0): enemy Flame Slash (Maliris). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

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
    Pool6CEntry *emit0[6];
    SVECTOR pos18[6];
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
} Fx1a0Work82;

extern Fx1a0Bone fx_1a0_op80Bones[6];
extern s16 fx_1a0_op81EmitFileIds[6];
extern s16 fx_1a0_pathFileIds[6];
extern s32 (*fx_1a0_op81ParticleMotions[6])();
extern Unk800F78A4 *fx_1a0_env;
extern s32 fx_1a0_op81Trails[6][0x1C];
extern Fx1a0Path fx_1a0_posPaths[6];
extern s32 fx_1a0_pathFrames[6]; /* frames */
extern SVECTOR fx_1a0_pathOrigin;
extern SVECTOR fx_1a0_emitPos[6];
extern SVECTOR fx_1a0_emitPos1[6];

s32 fx_1a0_op81ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a0_op81ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a0_op81ParticleMotion2(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a0_op81ParticleMotion3(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a0_op81ParticleMotion4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_1a0_op81ParticleMotion5(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fxm_followPathParticle(Pool30Entry *e, SVECTOR *pos, Fx1a0Path *path, s32 t, s32 frame, s32 frames);
static void fxm_posPathInit(Fx1a0Path *path, u16 *data);
static s32 fxm_posPathGet(Fx1a0Path *path, s32 t, SVECTOR *out);

#endif
