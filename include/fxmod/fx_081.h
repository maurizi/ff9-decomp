#ifndef FX_081_H
#define FX_081_H

/* Effect module fx_081 (dir 13 file 0x81). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E8ED8;
extern SVECTOR D_801EA728;
extern SVECTOR D_801EA730;
extern Pool6CEntry *D_801EA738;
extern Unk80100C78Entry *D_801EA9B0;
extern s32 D_801EA73C; /* glow level */
extern Pool6CEntry *D_801EA740;
extern SVECTOR D_801EA748; /* caster body position */
extern BtlModel D_801EA758[];
extern u8 D_801E8EE8[];

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx081Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* model */
    s32 unkC; /* model */
    SVECTOR unk10;
    Fx081Vec3 unk18;
    SVECTOR unk24;
    Pool6CEntry *unk2C;
    Pool6CEntry *unk30;
    Pool6CEntry *unk34;
} Fx081Work80; /* 0x38 */

void func_801E7704(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 arg3, s32 *arg4, s32 arg5, s32 arg6, s32 frame,
                   s32 n);

#endif
