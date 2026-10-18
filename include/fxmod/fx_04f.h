#ifndef FX_04F_H
#define FX_04F_H

/* Effect module fx_04f (dir 13 file 0x4F: Reflect, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* models */
    s32 unk8;
    s32 unkC;
    s32 unk10;
    SVECTOR unk14; /* in front of the target */
    SVECTOR unk1C;
    SVECTOR unk24;
    s32 unk2C[3]; /* model position (a VECTOR without its pad word) */
    SVECTOR unk38; /* model rotation (target facing) */
    s32 unk40; /* target radius */
    Pool6CEntry *unk44;
    s32 unk48; /* frame */
    s32 unk4C; /* model scale x */
    s32 unk50; /* model scale z */
} Fx04fWork; /* 0x54 */

extern Unk800F78A4 *D_801E7F18;
extern BtlModel D_801E7F28[];

s32 func_801E7704(s32 mode, s32 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx04fWork *work);

#endif
