#ifndef FX_03E_H
#define FX_03E_H

/* Effect module fx_03e (dir 13 file 0x3E). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* models */
    s32 unk8;
    s32 unkC;
    s32 unk10;
    SVECTOR unk14; /* target body position */
    SVECTOR unk1C; /* target position */
    s32 unk24; /* frame */
} Fx03eWork; /* 0x28 */

extern Unk800F78A4 *D_801E7F70;
extern BtlModel D_801E7F80[];
extern s32 D_801E82A0[];

#endif
