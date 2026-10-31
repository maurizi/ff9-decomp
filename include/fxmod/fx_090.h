#ifndef FX_090_H
#define FX_090_H

/* Effect module fx_090 (dir 13 file 0x90): Curaga, all targets (party; enemy Curaga of Hilgigars and others). A
 * morphing model (Hi_DrawMorphEffModel) and texture frames copied by MoveImage. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4;
    s32 unk8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
    SVECTOR rot24; /* model rotation */
    VECTOR pos2C; /* model position */
    VECTOR scale3C; /* model scale */
    s32 bright4C; /* model brightness */
    SVECTOR pos50; /* position: env->targets->centerX..Z */
    SVECTOR pos58; /* pos50 raised by 0x320 */
    SVECTOR pos60; /* pos50 raised by 0x4B0 */
    s32 fadeIn68; /* fade-in countdown */
    s32 fadeOut6C; /* fade-out countdown */
    u16 model70; /* models */
    u16 model72;
} Fx090Work; /* 0x74 */

extern s16 fx_090_texAnimSrc[][2];
extern Unk800F78A4 *fx_090_env;
extern BtlModelBuf fx_090_effModels[];
extern s32 fx_090_trails[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
