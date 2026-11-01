#ifndef FX_0F4_H
#define FX_0F4_H

/* Effect module fx_0f4 (dir 13 file 0x0F4): Shift Break. Two growing textured models over backdrops
 * (btlfx_drawBackdrop). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR rot14; /* model rotation */
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    s32 rgb3C; /* model brightness */
    SVECTOR rot40; /* model 2 rotation */
    VECTOR scale48; /* model 2 scale */
    SVECTOR pos58; /* centre */
    SVECTOR startPos60;
    s16 fadeInTimer68; /* fade-in countdown */
    s16 fadeOutTimer6A; /* fade-out countdown */
    u16 model6C; /* model */
    u16 model6E; /* model 2 */
} Fx0f4Work; /* 0x70 */

extern Unk800F78A4 *fx_0f4_env;
extern BtlModelBuf fx_0f4_effModels[];

s32 fx_0f4_op80(s32 mode, Fx0f4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
