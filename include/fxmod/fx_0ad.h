#ifndef FX_0AD_H
#define FX_0AD_H

/* Effect module fx_0ad (dir 13 file 0xAD). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    Pool6CEntry *emit4;
    SVECTOR pos8; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos10; /* caster body position */
    SVECTOR unk18; /* angles from unk8 to unk10 */
} Fx0adWork;

extern Unk800F78A4 *fx_0ad_env;
extern s32 D_801E793C[];

#endif
