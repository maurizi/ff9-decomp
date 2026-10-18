#ifndef FX_1E6_H
#define FX_1E6_H

/* Effect module fx_1e6 (dir 13 file 0x1e6). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* key count */
    SVECTOR *unk4; /* keys (6 bytes each) */
} Fx1e6Path;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[1];
} Fx1e6Trail;

typedef struct {
    s32 unk0;
    SVECTOR unk4; /* caster position */
    u16 unkC; /* fade */
    s16 unkE;
} Fx1e6Work80; /* 0x10 */

typedef struct {
    Pool6CEntry *unk0[7];
    SVECTOR unk1C; /* model rotation */
    VECTOR unk24; /* model position */
    VECTOR unk34; /* model scale */
    s32 unk44; /* brightness */
    SVECTOR unk48; /* caster body position */
    SVECTOR unk50;
    s32 unk58; /* morph counter */
    s32 unk5C; /* fade-in countdown */
    s32 unk60; /* fade-out countdown */
    u16 unk64; /* models */
    u16 unk66;
} Fx1e6Work81; /* 0x68 */

extern Unk800F78A4 *D_801E8348;
extern BtlModel D_801E8358[];
extern Fx1e6Path D_801E84E8;
extern s32 D_801E84F8[];
extern s32 D_801E87C8[];
extern s32 D_801E8A98[];
extern s32 D_801E8D68[];
extern s32 D_801E8EE8[];
extern s32 D_801E9068[];
extern Fx1e6Trail D_801E91E8;
extern SVECTOR D_801E92F8; /* origin */
extern SVECTOR D_801E9300; /* rotation */

void func_801E7724(Fx1e6Trail *trail, s32 size, s32 start);
s32 func_801E822C(s32 from, s32 to, s32 t, s32 n);
void func_801E8284(Fx1e6Path *path, u8 *data);
s32 func_801E829C(Fx1e6Path *path, s32 i, SVECTOR *out);

/* copied */
extern Unk800F78A4 *D_801E8348;
/* end copied */

#endif
