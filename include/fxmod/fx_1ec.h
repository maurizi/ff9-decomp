#ifndef FX_1EC_H
#define FX_1EC_H

/* Effect module fx_1ec (dir 13 file 0x1ec). */
#include "btlfx_mod.h"

/* copied */
typedef struct {
    s16 unk0;
    s16 unk2; /* key count */
    u8 *unk4; /* keys */
} Fx1ecPath;

extern Unk800F78A4 *D_801E8E94;
extern Fx1ecPath D_801E9034[0x12];
s32 func_801E8C60(Fx1ecPath *path, s32 t, SVECTOR *out);
extern SVECTOR D_801EFB64;
extern Fx1ecPath D_801E90C4[0x1E];
extern SVECTOR D_801EFB6C;
/* end copied */

#endif
