#ifndef FX_05A_H
#define FX_05A_H

/* Effect module fx_05a (dir 13 file 0x5A). */
#include "btlfx_mod.h"

/* fx_05a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MIGHT_BACKDROP    0x7 /* backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) (look pending) */
#define FX_MIGHT_LIGHT_TRAIL 0x8 /* emitter: screen trail with a pale crystal strip + sparks (texture not in the file) */
#define FX_MIGHT_SWIRL       0x9 /* emitter: blue-white swirl particles + fan */
#define FX_MIGHT_STAR        0xA /* model: four-pointed star, plain white texture */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    SVECTOR pos10; /* effect position (raised for big enemies) */
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    s16 rgb40; /* model brightness */
    s16 tint42; /* screen tint */
    u16 model44; /* model */
} Fx05aWork; /* 0x48 */

extern Unk800F78A4 *fx_05a_env;
extern BtlModelBuf fx_05a_effModels[];
extern s32 fx_05a_trails[];

#endif
