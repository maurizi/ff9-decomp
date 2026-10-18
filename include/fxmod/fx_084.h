#ifndef FX_084_H
#define FX_084_H

/* Effect module fx_084 (dir 13 file 0x84). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* caster body position */
    SVECTOR unkC; /* unit 0x10 body position */
    SVECTOR unk14; /* angles from unkC to unk4 */
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
} Fx084Work;

extern Unk800F78A4 *D_801E7B90;
extern SVECTOR D_801E7B98; /* particle path start */
extern SVECTOR D_801E7BA0; /* particle path end */
extern Unk80100C78Entry *D_801E7BA8;

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame);

#endif
