#ifndef FX_188_H
#define FX_188_H

/* Effect module fx_188 (dir 13 file 0x188). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* speed */
    s32 unk8; /* turn rate */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* direction */
    Pool6CEntry *unk1C;
    SVECTOR unk20; /* target */
} Fx188Elem; /* 0x28 */

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* target */
    SVECTOR unkC; /* caster body position */
    SVECTOR unk14; /* caster rotation */
    s32 unk1C; /* spawned */
} Fx188Work; /* 0x20 */

extern Unk800F78A4 *D_801E7E88;
extern s32 D_801E7E98[];
extern s32 D_801E86CC; /* scale */

s32 func_801E7704(s32 mode, Fx188Elem *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
