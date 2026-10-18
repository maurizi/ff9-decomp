#ifndef FX_19F_H
#define FX_19F_H

/* Effect module fx_19f (dir 13 file 0x19f). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[25];
} Fx19fTrail; /* 0xCC */

extern Unk800F78A4 *D_801EAA48;

typedef struct {
    s16 x, y, z;
} Fx19fKey;

typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    Fx19fKey unk4[1];
} Fx19fKeys;
typedef struct {
    SVECTOR unk0[17]; /* points of a 17-point trail (its 4-byte header precedes them) */
    u8 pad88[4];
} Fx19fTrailPts; /* 0x8C */

extern Fx19fTrailPts D_801EB3BC[32];

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[17];
} Fx19fTrail17; /* 0x8C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[4];
} Fx19fTrail4; /* 0x24 */

typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    u8 unk10; /* trail index */
    u8 unk11; /* kind */
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 *unk18; /* noise */
} Fx19fBolt;

extern Fx19fTrail17 D_801EB3B8[];
extern Fx19fTrail4 D_801EC538[];
extern SVECTOR D_801EA9AC;

void func_801E9968(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);
void func_801E9C08(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *path, Fx19fTrail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad);

/* copied */
void func_801E9968(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);
/* end copied */

#endif
