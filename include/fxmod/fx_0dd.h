#ifndef FX_0DD_H
#define FX_0DD_H

/* Effect module fx_0dd (dir 13 file 0x0DD). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* time */
    SVECTOR unk8; /* caster bone position */
} Fx0ddWork84;

extern Unk800F78A4 *D_801E83B0;

typedef struct {
    s16 unk0, unk2, unk4; /* position */
    s16 unk6, unk8, unkA; /* rotation */
} Fx0ddKey12;

typedef struct {
    u16 unk0; /* count */
    Fx0ddKey12 *unk4;
} Fx0ddTrack12;
typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10; /* caster position */
    SVECTOR unk18; /* model rotation */
    VECTOR unk20; /* model position */
    VECTOR unk30; /* model scale */
    u8 pad40[0x44 - 0x40];
    SVECTOR unk44; /* model position (16 bit) */
    SVECTOR unk4C; /* effect position */
    s32 unk54; /* morph time */
    s32 unk58;
    u16 unk5C[6]; /* models */
} Fx0ddWork80; /* 0x68 */

void func_801E8238(Fx0ddTrack12 *track, u16 *data);
s32 func_801E824C(Fx0ddTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);

typedef struct {
    s16 x, y, z;
} Fx0ddKey6;

typedef struct {
    u16 unk0; /* count */
    Fx0ddKey6 *unk4;
} Fx0ddTrack6;
void func_801E82E8(Fx0ddTrack6 *track, u16 *data);
s32 func_801E82FC(Fx0ddTrack6 *track, s32 t, VECTOR *out);

extern u8 D_801E8360[6];
extern BtlModel D_801E83C0[];
extern s32 D_801E8870[];
extern Fx0ddTrack12 D_801E90B8;
extern Fx0ddTrack12 D_801E90C0;
extern Fx0ddTrack6 D_801E90C8;
extern Fx0ddTrack6 D_801E90D0;
extern SVECTOR D_801E90D8;

s32 fx_0dd_op80(s32 mode, Fx0ddWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
