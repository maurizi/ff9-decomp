#ifndef FX_09B_H
#define FX_09B_H

/* Effect module fx_09b (dir 13 file 0x9B): enemy Lava Gun (Grenade/Vepal). Five shots, each a model and an
 * emitter whose particles are pinned to the shot (fxm_pinParticleN). Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_09b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LAVA_GUN_FIRE_FLASH 0x1 /* emitter: red rings, fiery glow, embers */
#define FX_LAVA_GUN_EXPLOSION  0x3 /* emitter: fire explosion + embers */
#define FX_LAVA_GUN_FIRE_RING  0x4 /* emitter: ring with a fiery texture */
#define FX_LAVA_GUN_RED_RING   0x5 /* emitter: red gouraud rings */
#define FX_LAVA_GUN_LAVA_BALL  0x6 /* 'so' model: rounded blob with a red-orange molten texture */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR rot14; /* model rotation */
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    SVECTOR pos3C; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos44; /* pos64 moved -0xC8 along the angles from pos3C */
    SVECTOR pos4C; /* pos64 on the ground */
    SVECTOR pos54; /* caster body position */
    SVECTOR fromPos5C; /* copy of pos3C */
    SVECTOR pos64; /* pos54 + random */
    s32 instance6C; /* instance (0..4) */
    u16 model70; /* model */
} Fx09bWork;
#define FX_09B_SHOTS 5 /* instances, one per shot */
#define FX_09B_LAST_SHOT (FX_09B_SHOTS - 1) /* no hit motion; clears the models when done */

extern s32 fx_09b_shotBlocks[5]; /* particle files */
extern s32 (*fx_09b_pinParticles[5])(); /* motion callbacks */
extern BtlModelBuf fx_09b_effModels[];
extern s32 fx_09b_trails[5][0x10A];
extern SVECTOR fx_09b_shotPos[5]; /* particle positions */
extern SVECTOR fx_09b_emitStartPos[5];

s32 fxm_pinParticle0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fxm_pinParticle1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fxm_pinParticle2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fxm_pinParticle3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fxm_pinParticle4(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHitMotionIfActive(s32 unit);
extern Unk800F78A4 *fx_09b_env;
static void fxm_startUnitMotion(s32 unit, s32 motion);
static void fxm_startHeavyHitMotionIfActive(s32 unit);

#endif
