#ifndef FX_0DC_H
#define FX_0DC_H

/* Effect module fx_0dc (dir 13 file 0x0DC). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* time */
    SVECTOR unk8; /* caster bone position */
} Fx0dcWork84;

extern Unk800F78A4 *D_801E9354;

s32 fx_0dc_op84(s32 mode, Fx0dcWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

/* propagated */

extern s16 D_801E92F0[];
void func_801E7718(s16 *a, s16 *b, s32 i, s16 *out);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0dcVec3;

/* one shard, a pool item */
typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* flying */
    s32 unk4; /* index */
    s32 unk8; /* frame */
    Fx0dcVec3 unkC; /* position << 4 */
    SVECTOR unk18; /* velocity */
    SVECTOR unk20; /* rotation */
    s16 unk28; /* rotation speed */
    s16 unk2A;
    s32 unk2C; /* models */
    s32 unk30;
    Pool6CEntry *unk34;
} Fx0dcItem; /* 0x38 */

typedef struct {
    u8 pad0[0x20];
    s32 unk20[22]; /* models */
    s16 unk78;
} Fx0dcWork;

extern SVECTOR D_801E9288[];
s32 func_801E7788(s32 mode, Fx0dcItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx0dcWork *work);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* models */
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20[14]; /* shard models */
    SVECTOR unk58; /* target position */
    SVECTOR unk60;
    SVECTOR unk68;
    SVECTOR unk70;
    s16 unk78;
    s16 unk7A; /* shards spawned */
    s32 unk7C;
    s32 unk80;
    Pool6CEntry *unk84;
} Fx0dcWork80; /* 0x88 */

extern s32 D_801E92B8[14];
extern BtlModel D_801E9364[];

#endif
