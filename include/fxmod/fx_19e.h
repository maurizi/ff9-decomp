#ifndef FX_19E_H
#define FX_19E_H

/* Effect module fx_19e (dir 13 file 0x19e). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx19eWork;

typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* angles */
    SVECTOR unk10; /* destination */
    SVECTOR unk18; /* target position */
    SVECTOR unk20;
    u8 pad28[0x30 - 0x28];
} Fx19eWork81;

extern Unk800F78A4 *fx_19e_env;


/* propagated */
static s32 fxm_turnUnitBy(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta);
static s32 fxm_turnUnitToward(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang);
static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);

#endif
