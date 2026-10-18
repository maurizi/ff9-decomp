#ifndef FX_006_H
#define FX_006_H

/* Effect module fx_006 (dir 13 file 0x06). */
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
} Fx006Work; /* 0x28 */

extern Unk800F78A4 *D_801E7F6C;
extern BtlModel D_801E7F7C[];
extern s32 D_801E829C[];

#endif
