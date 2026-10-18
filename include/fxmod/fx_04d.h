#ifndef FX_04D_H
#define FX_04D_H

/* Effect module fx_04d (dir 13 file 0x4D). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx04dPos;

/* pool item: a model that grows, then splits into three pieces that fly apart */
typedef struct {
    s32 unk0; /* index into D_801E8578 */
    s32 unk4; /* state */
    s32 unk8; /* frame */
    Fx04dPos unkC[3];
    SVECTOR unk30; /* rotation once split */
    s16 unk38; /* rotation speed */
    s16 unk3A;
    s32 unk3C; /* size */
    Pool6CEntry *unk40;
} Fx04dItem; /* 0x44 */

typedef struct {
    s32 unk0;
    s32 unk4; /* models */
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    SVECTOR unk18;
    SVECTOR unk20;
    s16 unk28; /* split now */
    s16 unk2A;
    s32 unk2C;
    s32 unk30;
} Fx04dWork;

extern SVECTOR D_801E8578[];
extern Unk800F78A4 *D_801E8610;
extern BtlModel D_801E8620[];

s32 func_801E7704(s32 mode, Fx04dItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx04dWork *work);

#endif
