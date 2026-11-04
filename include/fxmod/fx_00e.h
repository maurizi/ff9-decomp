#ifndef FX_00E_H
#define FX_00E_H

/* Effect module fx_00e (dir 13 file 0x0E: Berserk, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_00e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BERSERK_BACKDROP   0x3 /* backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) (look pending) */
#define FX_BERSERK_CONE       0x4 /* model: open cone (frustum), plain white texture */
#define FX_BERSERK_FIRE_RINGS 0xA /* emitter: dark red-orange-yellow gouraud rings */
#define FX_BERSERK_RED_FLASH  0xB /* emitter: white star flash, red star flash, violet circle */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    s32 frame4; /* background frame */
    SVECTOR pos8; /* target position */
    SVECTOR pos10; /* target body position */
    s32 pos18[3]; /* model position (a VECTOR without its pad word) */
    s32 model24; /* model */
} Fx00eWork; /* 0x28 */

extern Unk800F78A4 *fx_00e_env;
extern BtlModelBuf fx_00e_effModels[];

#endif
