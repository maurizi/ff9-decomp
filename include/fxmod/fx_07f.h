#ifndef FX_07F_H
#define FX_07F_H

/* Effect module fx_07f (dir 13 file 0x7F): Silence. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_07f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SILENCE_RING_DISC  0x1 /* model: flat annulus (UV box reads a placeholder page) (look pending) */
#define FX_SILENCE_BACKDROP   0x2 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_SILENCE_KANJI      0x3 /* emitter: the kanji "silence" glyph, glowing orange to violet */
#define FX_SILENCE_KANJI_DOME 0x5 /* model: dome textured with the "silence" glyph */
#define FX_SILENCE_PINK_PUFF  0x9 /* emitter: pink-white puff */
#define FX_SILENCE_PINK_RING  0xC /* emitter: white-pink gouraud rings */

typedef struct {
    s32 state0; /* state */
    s32 bgFrame4; /* frame for btlfx_drawBackdrop */
    SVECTOR pos8; /* position: env->targets->centerX */
    SVECTOR pos10; /* pos8 raised by 0x180 */
    s32 modelPos18[3]; /* model position (VECTOR without its pad word) */
    SVECTOR rot24; /* model rotation */
    s32 model2C; /* model */
    s32 model30; /* model */
} Fx07fWork;

extern Unk800F78A4 *fx_07f_env;
extern BtlModelBuf fx_07f_effModels[];

#endif
