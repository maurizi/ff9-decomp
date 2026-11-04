#ifndef FX_020_H
#define FX_020_H

/* Effect module fx_020 (dir 13 file 0x20). */
#include "btlfx_mod.h"

/* fx_020 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_REGEN_GLINTS      0x2 /* emitter: fan + blue glint, teal-white ring, blue / green star glints */
#define FX_REGEN_LIGHT_TRAIL 0x8 /* emitter: white-blue light trail + faint sparkle sprites */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
} Fx020Work;

extern SVECTOR fx_020_emitRot;
extern Unk800F78A4 *fx_020_env;
extern s32 fx_020_trails[];

#endif
