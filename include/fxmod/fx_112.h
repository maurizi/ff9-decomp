#ifndef FX_112_H
#define FX_112_H

/* Effect module fx_112 (dir 13 file 0x112). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx, vy, vz;
} Fx112Vec3;

typedef struct {
    u32 unk0; /* state */
    SVECTOR unk4; /* caster position */
    SVECTOR unkC; /* caster rotation */
    Fx112Vec3 unk14; /* caster scale */
    s32 unk20; /* model */
    u8 pad24[0x38 - 0x24];
    SVECTOR unk38; /* caster body position */
    SVECTOR unk40;
    SVECTOR unk48; /* bone position */
    Pool6CEntry *unk50;
} Fx112Work;

extern Unk800F78A4 *D_801E8050;
extern BtlModel D_801E8060[];
extern u8 D_801E8130[][0x2D0];

void func_801E7718(s32 x, s32 y, s32 w, s32 h, s32 dx, s32 dy);
s32 fx_112_op80(s32 mode, Fx112Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

/* fx_112_op80 locals: each state reuses the same stack area (the original frame overlaps them). */
typedef union {
    struct {
        VECTOR scale0;
        SVECTOR rot;
        SVECTOR a;
        SVECTOR b;
        VECTOR scale;
        VECTOR pos;
    } s0;
    struct {
        SVECTOR rot;
        SVECTOR v;
        SVECTOR pos;
        SVECTOR bone;
        VECTOR scale;
        VECTOR tr;
    } s1;
    struct {
        SVECTOR rot;
        SVECTOR v;
        BtlBone m;
        VECTOR scale;
        SVECTOR pad;
        SVECTOR r;
    } s4;
} Fx112Locals;

#endif
