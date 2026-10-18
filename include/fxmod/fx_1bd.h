#ifndef FX_1BD_H
#define FX_1BD_H

/* Effect module fx_1bd (dir 13 file 0x1bd). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1bdPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1bdPoint *unk4;
} Fx1bdPath;

typedef struct {
    s16 x, y, z;
} Fx1bdKey6;

typedef struct {
    u16 unk0; /* count */
    Fx1bdKey6 *unk4;
} Fx1bdTrack6;

typedef struct {
    s16 unk0, unk2, unk4; /* position */
    s16 unk6, unk8, unkA; /* rotation */
} Fx1bdKey12;

typedef struct {
    u16 unk0; /* count */
    Fx1bdKey12 *unk4;
} Fx1bdTrack12;

extern BtlModel D_801E9E64[];


typedef struct {
    Fx1bdPath unk0;
    s16 unk8;
    s16 unkA;
    s16 unkC; /* alpha */
    s16 unkE;
    s16 unk10;
    s16 unk12;
} Fx1bdLine;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* rotation */
    VECTOR unkC; /* position */
    VECTOR unk1C; /* scale */
    SVECTOR unk2C;
    VECTOR unk34;
    VECTOR unk44;
    u8 pad54[4];
    SVECTOR unk58;
    s16 unk60; /* fade-in countdown */
    s16 unk62; /* fade-out countdown */
    u16 unk64; /* frame */
    u16 unk66; /* model */
    u16 unk68; /* model */
} Fx1bdWork82; /* 0x6C */

extern Fx1bdLine D_801EB764;
extern u8 D_801EBC54[13][0x50]; /* trails */
extern u32 D_801EB854[];

s32 func_801E8DA0(s32 from, s32 to, s32 t, s32 n);
void func_801E8E78(Fx1bdPath *path, u16 *data);

/* copied */
extern Unk800F78A4 *D_801E9E54;
/* end copied */

/* propagated */

void func_801E77C0(Fx1bdPath *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip);
u16 *func_801E901C(u16 *base, u16 i);

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx1bdTrack; /* 0x10 */
extern Fx1bdTrack D_801EC064[];
extern s32 D_801EC464[];
void func_801E90CC(void);
void func_801E9118(s16 start, u16 dst, s16 ch, s16 *data);
void func_801E9174(s32 ch);

#endif
