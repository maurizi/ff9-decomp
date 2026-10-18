#ifndef FX_1FC_H
#define FX_1FC_H

/* Effect module fx_1fc (dir 13 file 0x1fc). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1fcKey6;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1fcKey6 *unk4;
} Fx1fcTrack6;

typedef struct {
    s16 x, y, z;
} Fx1fcPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1fcPoint *unk4;
} Fx1fcPath;

extern BtlModel D_801EC15C[];
extern u32 D_801EDADC[];
extern SVECTOR D_801EDEDC[8]; /* unit positions */
extern VECTOR D_801EDF1C[8]; /* unit scales */

typedef struct {
    Pool6CEntry *unk0[8];
    SVECTOR unk20[8]; /* unit positions */
    SVECTOR unk60[8];
    s16 unkA0[8]; /* landed */
} Fx1fcWork84; /* 0xB0 */


static inline void fx1fc_copy(SVECTOR *d, SVECTOR *s) {
    d->vx = s->vx;
    d->vy = s->vy;
    d->vz = s->vz;
}

/* copied */
extern Unk800F78A4 *D_801EC14C;
/* end copied */

/* propagated */

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx1fcKey; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx1fcKey *unk4;
} Fx1fcPath_0b4;
s32 func_801E87E8(Fx1fcPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);
s32 func_801E86A0(u8 *base, u16 i);

typedef struct {
    u16 unk0;
    u16 unk2; /* count */
    s16 *unk4;
} Fx1fcKeys;
s32 func_801E8894(Fx1fcKeys *keys, s32 i, s32 *out);
s32 func_801E89EC(s32 a0, s32 t, s32 n, SVECTOR *b);

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx1fcTrack; /* 0x10 */
extern Fx1fcTrack D_801EDF9C[];
extern s32 D_801EE39C[];
void func_801E8AB8(void);
void func_801E8B04(s16 start, u16 dst, s16 ch, s16 *data);
void func_801E8B60(s32 ch);

#endif
