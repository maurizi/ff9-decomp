#ifndef FX_196_H
#define FX_196_H

/* Effect module fx_196 (dir 13 file 0x196): Leviathan (short); Tsunami. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0; /* not read by the module */
    s16 count2; /* count */
    s16 keys4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx196Track;

typedef struct {
    u16 fileId0;
    u16 fileId2;
    u16 duration4;
    u16 unk6; /* not read by the module */
} Fx196CamPath;

typedef struct {
    SVECTOR pos0[8]; /* unit positions */
    s32 count40; /* unit count */
} Fx196Work86; /* 0x44 */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    u8 pad8[0x10 - 0x8];
    SVECTOR pos10; /* position */
    SVECTOR camPos18; /* camera */
    u8 pad20[0x48 - 0x20];
    Pool6CEntry *emit48;
} Fx196Work85; /* 0x4C */

extern CVECTOR fx_196_rippleColors[];

extern Fx196CamPath fx_196_camPaths[];
extern Unk800F78A4 *fx_196_env;

static void fxm_keyGetVec(Fx196Track *track, s32 t, SVECTOR *out);
void fx_196_drawWaveMesh(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 col2, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 amp, s32 ph, s32 step, s32 bulge, s32 ang, s32 dang);

/* copied */
extern Unk800F78A4 *fx_196_env;
/* end copied */

/* propagated */
s32 fxm_sinHalfNeg(s32 ang);

extern s32 fx_196_nextWavePoint;
extern SVECTOR fx_196_wavePoints[];
static s32 fxm_nextRingPoint(SVECTOR *out);

#endif
