#ifndef FX_0C1_H
#define FX_0C1_H

/* Effect module fx_0c1 (dir 13 file 0xC1): No Mercy. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* unit 0x10 position */
    SVECTOR rot18; /* unit 0x10 rotation */
    SVECTOR pos20; /* caster body position */
    SVECTOR pos28; /* unit 0x10 bone 0x17 position */
    SVECTOR pos30; /* pos10 moved 0x12C along rot18 */
    SVECTOR pos38; /* btlfx_getUnitBonePos(0x10, 0x17) */
    SVECTOR rot40; /* copy of rot48 */
    SVECTOR rot48; /* angles from pos38 to pos20 */
} Fx0c1Work;

extern Unk800F78A4 *fx_0c1_env;
extern SVECTOR fx_0c1_pathFrom; /* particle path start */
extern SVECTOR fx_0c1_pathTo; /* particle path end */
extern SVECTOR fx_0c1_emitPos;

static s32 fx_0c1_op80ParticleMotion(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
