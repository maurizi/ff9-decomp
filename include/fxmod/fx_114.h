#ifndef FX_114_H
#define FX_114_H

/* Effect module fx_114 (dir 13 file 0x114). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* model rotation */
    VECTOR unk10; /* model position */
    VECTOR unk20; /* model scale */
    s32 unk30; /* model brightness */
    SVECTOR unk34; /* summon bone position */
    SVECTOR unk3C;
    s16 unk44; /* fade-in countdown */
    u16 unk46; /* model */
} Fx114Work81; /* 0x48 */

/* View of Unk800F78A44 up to the word fx_114 reads (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x60];
    s32 unk60; /* summon model */
} Fx114Summon;

extern Unk800F78A4 *D_801EB768;
extern u8 D_801ED0E8[]; /* scale keys */

s32 fx_114_op81(s32 mode, Fx114Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
void func_801EA808(void *keys, s32 data);
void func_801EA81C(void *keys, s32 frame, VECTOR *out);

s32 func_801EA914(s32 r, s32 g, s32 t, s32 n, s32 arg4, s32 arg5);

void func_801EA5C0(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801EA604(s32 from, s32 to, s32 t, s32 n);

#endif
