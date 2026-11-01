#ifndef FX_0E9_H
#define FX_0E9_H

/* Effect module fx_0e9 (dir 13 file 0x0E9): Annoy. Particles that leave the caster and circle the target on a
 * shrinking, wobbling orbit (fx_0e9_op80ParticleMotion). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster position */
    SVECTOR rot10; /* caster rotation */
    SVECTOR pos18; /* target position */
} Fx0e9Work;

typedef struct {
    s16 radius0; /* start radius */
    s16 radius2; /* radius */
    s16 dir4; /* direction */
    s16 angle6; /* angle */
    s16 wobble8; /* wobble phase */
    s16 timerA; /* countdown */
} Fx0e9Orbit;

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*motion8)(VECTOR *, s32, SVECTOR *, s32, s32, s32);
} Fx0e9EffHdr;

extern Unk800F78A4 *fx_0e9_env;
extern Fx0e9Orbit fx_0e9_orbit;
extern SVECTOR fx_0e9_startPos;
extern SVECTOR fx_0e9_orbitStart;
extern SVECTOR fx_0e9_orbitCenter;
extern SVECTOR fx_0e9_emitPos;

static s32 fx_0e9_op80ParticleMotion(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
