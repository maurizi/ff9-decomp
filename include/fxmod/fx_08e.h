#ifndef FX_08E_H
#define FX_08E_H

/* Effect module fx_08e (dir 13 file 0x8E): Curaga, one target (party; enemy Curaga of Dummy/Necron). A morphing
 * model (Hi_DrawMorphEffModel) and texture frames copied by MoveImage. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    SVECTOR rot1C; /* model rotation */
    VECTOR pos24; /* model position */
    VECTOR scale34; /* model scale */
    s32 bright44; /* model brightness */
    SVECTOR pos48; /* target body position */
    SVECTOR pos50;
    SVECTOR pos58; /* caster position */
    s32 fadeIn60; /* fade-in countdown */
    s32 fadeOut64; /* fade-out countdown */
    u16 model68; /* models */
    u16 model6A;
} Fx08eWork; /* 0x6C */

extern Unk800F78A4 *fx_08e_env;
extern s16 fx_08e_texAnimSrc[][2];
extern BtlModel fx_08e_effModels[];
extern s32 fx_08e_trails[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
