#ifndef FX_138_H
#define FX_138_H

/* Effect module fx_138 (dir 13 file 0x138). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* key count */
    SVECTOR *unk4; /* keys */
} Fx138Path;

typedef struct {
    Pool6CEntry *unk0[6];
    Pool6CEntry *unk18;
    Pool6CEntry *unk1C;
    SVECTOR unk20; /* caster body position */
    SVECTOR unk28;
    u16 unk30; /* frame, up to 5 */
    u16 unk32; /* scale */
} Fx138Work81; /* 0x34 */

extern u16 D_801E8488[6]; /* trail files */
extern s32 D_801E84A0[6];
extern Unk800F78A4 *D_801E8500;
extern u8 D_801E8860[];
extern SVECTOR D_801E8E70[6];
extern Fx138Path D_801E8830[6];
extern SVECTOR D_801E8E60; /* path origin */
extern SVECTOR D_801E8E68; /* path rotation */

s32 func_801E7704(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E7728(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E774C(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E7770(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E7794(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E77B8(SVECTOR *out, s32 arg1, s32 i);
void func_801E77DC(SVECTOR *out, Fx138Path *path, s32 i);
s32 func_801E81F0(Fx138Path *path, s32 i, SVECTOR *out);
s32 fx_138_op81(s32 mode, Fx138Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
