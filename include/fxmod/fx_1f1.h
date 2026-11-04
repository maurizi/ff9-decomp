#ifndef FX_1F1_H
#define FX_1F1_H

/* Effect module fx_1f1 (dir 13 file 0x1f1): enemy Death3 (Dummy/Necron). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH3_RUNE_CYLINDER   0x2 /* 'so' model: cylinder of blue glowing runes */
#define FX_DEATH3_RUNE_CYLINDER_2 0x4 /* 'so' model: cylinder of blue glowing runes (other texture row) */
#define FX_DEATH3_MAGIC_CIRCLE    0x5 /* emitter: blue glowing magic circle with a star */

typedef struct {
    s32 delay0; /* delay */
    s32 timer4; /* countdown */
    s32 height8; /* saved height */
    SVECTOR posC; /* unit position */
    s32 unk14; /* write-only (cleared) */
    s32 model18; /* models */
    s32 model1C;
    SVECTOR rot20; /* model rotations */
    SVECTOR rot28;
    VECTOR scale30; /* model scales */
    VECTOR scale40;
    VECTOR pos50; /* model position */
} Fx1f1Unit; /* 0x60 */

/* the same records seen from their model part at 0x20 */
typedef struct {
    SVECTOR rot0[2]; /* model rotations */
    VECTOR scale10[2]; /* model scales */
    VECTOR pos30; /* model position */
    u8 pad40[0x20];
} Fx1f1Model; /* 0x60 */

extern s32 fx_1f1_unitDelays[];
extern Unk800F78A4 *fx_1f1_env;
extern Fx1f1Unit fx_1f1_units[];
extern Fx1f1Model fx_1f1_unitModels[];
extern BtlModelBuf fx_1f1_effModels[];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
