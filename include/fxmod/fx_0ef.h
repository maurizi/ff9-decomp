#ifndef FX_0EF_H
#define FX_0EF_H

/* Effect module fx_0ef (dir 13 file 0x0EF): enemy Red Clipper (Stilva). Five timed emitter spawns around the target
 * body (fx_0ef_spawns). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0[5];
    Pool6CEntry *emit14[5];
    u8 pad28[0x38 - 0x28];
    SVECTOR pos38;
    SVECTOR emitPos40[5];
} Fx0efWork80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
} Fx0efWork81;

typedef struct {
    u16 block0; /* file */
    u16 frame2; /* frame */
    s16 offset4[3]; /* offset */
    s16 padA;
} Fx0efSpawn;

extern Fx0efSpawn fx_0ef_spawns[5];
extern Unk800F78A4 *fx_0ef_env;

typedef struct {
    s16 x, y, z;
} Fx0efPoint;

typedef struct {
    s16 unk0;
    u16 count2; /* count */
    Fx0efPoint *keys4;
} Fx0efPath;
static void fxm_posPathInit(Fx0efPath *path, u16 *data);
static s32 fxm_posPathGet(Fx0efPath *path, s32 t, SVECTOR *out);

#endif
