#ifndef FX_095_H
#define FX_095_H

/* Effect module fx_095 (dir 13 file 0x95). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* spawned */
    DVECTOR unk8; /* caster screen position */
    s32 unkC; /* release */
} Fx095Work80; /* 0x10 */

typedef struct {
    s32 unk0; /* state */
    s32 unk4;
    u16 unk8[0x3000]; /* captured screen strip */
} Fx095Work81; /* 0x6008 */

typedef struct {
    s32 unk0; /* angle */
    s32 unk4; /* state */
    s32 unk8; /* index */
} Fx095Part; /* 0xC */

typedef struct {
    u8 pad0[8];
    s16 unk8; /* screen x */
    s16 unkA; /* screen y */
    s32 unkC; /* release */
} Fx095Ctl;

extern Unk800F78A4 *D_801E8904;
extern SVECTOR D_801E890C;

s32 func_801E7704(s32 mode, Fx095Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx095Ctl *ctl);
void func_801E7CBC(s32 n, s32 x, s32 y, u16 *buf, RECT *r);
void func_801E7F48(s32 ot, s32 x, s32 y, s32 r, s32 c, RECT *rect);

extern s32 D_801E882C[]; /* file ids */
extern s32 D_801E885C[]; /* file ids */
extern s32 D_801E888C[]; /* file ids */

#endif
