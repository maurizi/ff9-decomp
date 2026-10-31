#ifndef FX_0C9_H
#define FX_0C9_H

/* Effect module fx_0c9 (dir 13 file 0xC9): Jewel. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster position */
    SVECTOR pos10; /* caster body position */
} Fx0c9Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* unit 0x10 body position, raised by 0x50 */
    SVECTOR pos14; /* caster position */
    SVECTOR pos1C; /* moves from pos14 to posC */
    SVECTOR rot24; /* model rotation */
    VECTOR pos2C; /* model position */
    VECTOR scale3C; /* model scale */
    SVECTOR pos4C;
    u16 model54; /* model */
} Fx0c9Work81;

extern Unk800F78A4 *fx_0c9_env;
extern BtlModel fx_0c9_effModels[];
extern SVECTOR fx_0c9_particlePos; /* particle position */
extern SVECTOR fx_0c9_emitPos;
extern s32 fx_0c9_trails[];

static s32 fx_0c9_op81ParticleMotion(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

#endif
