#ifndef FX_07E_H
#define FX_07E_H

/* Effect module fx_07e (dir 13 file 0x7E). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* spawned */
    s32 unk8; /* frame */
    SVECTOR unkC; /* caster body position */
    SVECTOR unk14; /* ring rotation */
    s32 unk1C; /* radius */
    s32 unk20; /* scale */
    s32 unk24; /* shrink done */
    s32 unk28; /* model */
} Fx07eWork80; /* 0x2C */

typedef struct {
    s32 unk0; /* angle offset */
    s32 unk4; /* state */
    s32 unk8; /* index */
} Fx07ePart; /* 0xC */

extern Unk800F78A4 *D_801E8AB8;
extern BtlModel D_801E8AC8[];
extern s32 D_801E8B90;

s32 func_801E7EB8(s32 mode, Fx07ePart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx07eWork80 *work);
void func_801E8498(s32 r, s32 a1, s32 a, s32 a3, s16 *out);

#endif
