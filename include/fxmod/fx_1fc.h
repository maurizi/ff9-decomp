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

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1fcVec3;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* slice rotation */
    Fx1fcVec3 unk10; /* slice position */
    s32 pad1C;
    Fx1fcVec3 unk20; /* slice scale */
    s32 pad2C;
    SVECTOR unk30;
    Fx1fcVec3 unk38;
    s32 pad44;
    Fx1fcVec3 unk48;
    s32 pad54;
    SVECTOR unk58[8]; /* unit positions */
    VECTOR unk98[8]; /* unit scales */
    SVECTOR unk118;
    SVECTOR unk120;
    SVECTOR unk128;
    s16 unk130; /* fade counters */
    s16 unk132;
    s16 unk134;
    u16 unk136[9]; /* models */
} Fx1fcWork83; /* 0x148 */

extern u16 D_801E8CC0[3];
extern s16 D_801E8CC8[];
extern s16 D_801E8CD8[];
extern u8 D_801E8CE8[];
extern Unk800E8F50 D_801E99A4;
extern Fx1fcKeys D_801EDA5C;
extern Fx1fcKeys D_801EDA64;
extern Fx1fcKeys D_801EDA6C;
extern Fx1fcKeys D_801EDA7C[10];
extern Fx1fcKeys D_801EDACC;
extern Fx1fcKeys D_801EDAD4;
void func_801E8754(Fx1fcPath *path, u16 *data);
s32 func_801E876C(Fx1fcPath *path, s32 t, SVECTOR *out);
void func_801E87D0(Fx1fcPath *path, u16 *data);
void func_801E887C(Fx1fcPath *path, u16 *data);
void func_801E88D8(Fx1fcPath *path, u16 *data);
s32 func_801E88F0(Fx1fcTrack6 *track, s32 t, VECTOR *out);
void func_801E8954(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);

s32 fx_1fc_op83(s32 mode, Fx1fcWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
