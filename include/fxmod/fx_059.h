#ifndef FX_059_H
#define FX_059_H

/* Effect module fx_059 (dir 13 file 0x59: Haste, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* height */
    s32 unk8; /* angle */
    s32 unkC; /* index into D_801E7D38 / D_801E7D50 */
    s32 unk10; /* angle step */
    Unk800DA1B8 *unk14; /* particle pool */
    s32 unk18;
} Fx059Work80;

typedef struct {
    s32 unk0;
    SVECTOR unk4;
} Fx059Work81;

typedef struct {
    s32 unk0; /* frame */
    s32 unk4;
    s16 unk8; /* file slot */
} Fx059Entry;

typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
} Fx059Rot;

extern s16 D_801E7D38[];
extern s16 D_801E7D50[];
extern Unk800F78A4 *D_801E7DB0;

s32 func_801E7704(s32 mode, SVECTOR *pos, Unk800F78A4 *env, Fx059Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx059Work80 *work);

#endif
