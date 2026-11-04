#ifndef FX_0EA_H
#define FX_0EA_H

/* Effect module fx_0ea (dir 13 file 0x0EA): Sacrifice. Particles arcing from the caster to each target
 * (fxm_arcParticle), a sprite line at the caster. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ea effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SACRIFICE_RED_PETALS  0x1 /* emitter: spiral of red petals + red glitter */
#define FX_SACRIFICE_LIGHT_FLASH 0x2 /* emitter: lilac / blue-white rings + white light streaks */
#define FX_SACRIFICE_FAN_EMITTER 0x5 /* emitter: fans + faint sprites with cyan-white rings (look pending) */
#define FX_SACRIFICE_LENS_FLARES 0x9 /* sprite line: lens-flare circles and rings */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster position */
    SVECTOR linePos10;
    u16 lineFrame18;
    u16 lineAlpha1A;
} Fx0eaWork80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* target body position */
    SVECTOR pos18; /* target position */
    s32 target20; /* target index */
} Fx0eaWork81;

typedef s32 (*Fx0eaPathFn)(VECTOR *, s32, SVECTOR *, s32, s32, s32);

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    Fx0eaPathFn motion8;
} Fx0eaEffHdr;

extern u16 fx_0ea_emitBlocks[4];
extern Fx0eaPathFn fx_0ea_op81ParticleMotions[4];
extern Unk800F78A4 *fx_0ea_env;
extern SVECTOR fx_0ea_arcFrom; /* path start */
extern SVECTOR fx_0ea_arcTo[4]; /* path ends */
extern SVECTOR fx_0ea_emitPos;

static s32 fxm_arcParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *to, s32 frame, s32 n);
static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

/* propagated */
s32 fx_0ea_op81ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_0ea_op81ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_0ea_op81ParticleMotion2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_0ea_op81ParticleMotion3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
