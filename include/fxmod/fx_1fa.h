#ifndef FX_1FA_H
#define FX_1FA_H

/* Effect module fx_1fa (dir 13 file 0x1fa). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* delay */
    s32 unk4; /* countdown */
    u8 *unk8; /* effect data */
    s32 unkC; /* angle */
    s32 unk10; /* radius */
} Fx1faRing; /* 0x14 */

extern Unk800F78A4 *D_801E80DC;
extern SVECTOR D_801E80E4;
extern s32 D_801E80EC[]; /* countdown; an array (in-struct for alias analysis) */
extern SVECTOR D_801E80F4;
extern SVECTOR D_801E80FC[];
extern Fx1faRing D_801E811C[12][4];

#endif
