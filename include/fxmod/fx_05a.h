#ifndef FX_05A_H
#define FX_05A_H

/* Effect module fx_05a (dir 13 file 0x5A). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    SVECTOR pos10; /* effect position (raised for big enemies) */
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    s16 rgb40; /* model brightness */
    s16 tint42; /* screen tint */
    u16 model44; /* model */
} Fx05aWork; /* 0x48 */

extern Unk800F78A4 *fx_05a_env;
extern BtlModelBuf fx_05a_effModels[];
extern s32 fx_05a_trails[];

#endif
