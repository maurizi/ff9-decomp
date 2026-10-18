#ifndef FX_1EC_H
#define FX_1EC_H

/* Effect module fx_1ec (dir 13 file 0x1ec). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1ecPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1ecPoint *unk4;
} Fx1ecPath;

typedef struct {
    Pool6CEntry *unk0[9];
    SVECTOR unk24; /* caster body position */
    SVECTOR unk2C; /* target position */
    SVECTOR unk34;
    SVECTOR unk3C;
    SVECTOR unk44;
    SVECTOR unk4C; /* camera targets */
    SVECTOR unk54;
    SVECTOR unk5C;
} Fx1ecWork80; /* 0x64 */

typedef struct {
    Pool6CEntry *unk0[11];
    SVECTOR unk2C; /* target position */
    SVECTOR unk34;
    SVECTOR unk3C;
    SVECTOR unk44;
    SVECTOR unk4C;
} Fx1ecWork81; /* 0x54 */

typedef struct {
    Pool6CEntry *unk0[8];
    SVECTOR unk20; /* model rotation */
    VECTOR unk28; /* model position */
    VECTOR unk38; /* model scale */
    s32 unk48; /* brightness */
    SVECTOR unk4C; /* target position */
    SVECTOR unk54;
    SVECTOR unk5C;
    s32 unk64; /* fade countdown */
    s16 unk68; /* screen fade countdown */
    s16 unk6A; /* screen flash */
    u16 unk6C; /* models */
    u16 unk6E;
} Fx1ecWork82; /* 0x70 */

extern BtlModel D_801E8EA4[];
extern s32 D_801E91B4[];
extern s32 D_801E97F4[];
extern s32 D_801E9FD4[];
extern s32 D_801EACC4[];
extern s32 D_801EB924[];
extern s32 D_801EC324[];
extern s32 D_801EDAC4[];
extern s32 D_801EF024[];
extern SVECTOR D_801EFB74;
extern SVECTOR D_801EFB7C;

s32 func_801E78E0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E79DC(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E8B58(s32 from, s32 to, s32 t, s32 n);
void func_801E8BB0(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
void func_801E8C48(Fx1ecPath *path, u16 *data);
s32 func_801E8D30(s32 unit);
void func_801E8E00(s32 unit);

/* copied */
extern Unk800F78A4 *D_801E8E94;
extern Fx1ecPath D_801E9034[0x12];
s32 func_801E8C60(Fx1ecPath *path, s32 t, SVECTOR *out);
extern SVECTOR D_801EFB64;
extern Fx1ecPath D_801E90C4[0x1E];
extern SVECTOR D_801EFB6C;
void func_801E8CC4(s32 unit, s32 motion);
/* end copied */

/* propagated */
u16 *func_801E8AFC(u8 *base, u16 i);

#endif
