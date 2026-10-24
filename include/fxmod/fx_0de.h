#ifndef FX_0DE_H
#define FX_0DE_H

/* Effect module fx_0de (dir 13 file 0x0DE). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0[8]; /* unit home positions */
    s32 unk40; /* unit count */
    s32 unk44; /* jump height */
    s32 unk48[8]; /* unit heights */
} Fx0deWork81; /* 0x68 */

extern Unk800F78A4 *D_801E8698;
extern s32 D_801E8CE8; /* jump scale */

s32 fx_0de_op81(s32 mode, Fx0deWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

/* propagated */

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0deWork84;
s32 fx_0de_op84(s32 mode, Fx0deWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    s32 unk0; /* done */
    SVECTOR unk4; /* caster body position */
    SVECTOR unkC; /* caster position */
    s32 unk14[8]; /* models */
    s32 unk34; /* scale from the caster's size */
} Fx0deWork80; /* 0x38 */
typedef struct {
    u16 unk0;
    u16 unk2;
} Fx0deFlash;
extern Fx0deFlash D_801E85F0[];
extern BtlModel D_801E86A8[];

#endif
