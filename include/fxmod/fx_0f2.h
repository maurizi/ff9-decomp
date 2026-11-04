#ifndef FX_0F2_H
#define FX_0F2_H

/* Effect module fx_0f2 (dir 13 file 0x0F2): Tidal Flame. A textured model sweeping across the field over a backdrop
 * (btlfx_drawBackdrop). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TIDAL_FLAME_DOME        0x2 /* model: hemisphere, texture page 0x11A not in the file (magenta in the render) (look pending) */
#define FX_TIDAL_FLAME_BACKDROP    0x3 /* table: background layers for btlfx_drawBackdrop */
#define FX_TIDAL_FLAME_RUNE_BURST  0x8 /* emitter: white rune glyphs with cyan-blue rings, embers, blue light streak */
#define FX_TIDAL_FLAME_FLAME_CLOUD 0x9 /* emitter: embers + orange flame cloud */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* model rotation */
    VECTOR pos10; /* model position */
    VECTOR scale20; /* model scale */
    s32 rgb30; /* model brightness */
    s32 speed34; /* z speed */
    SVECTOR pos38;
    SVECTOR startPos40;
    SVECTOR pos48;
    s32 isEnemy50; /* enemy side */
    s16 fadeInTimer54; /* fade-in countdown */
    s16 fadeOutTimer56; /* fade-out countdown */
    u16 model58; /* model */
} Fx0f2Work;

extern Unk800F78A4 *fx_0f2_env;
extern BtlModelBuf fx_0f2_effModels[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
