#ifndef FX_0EF_H
#define FX_0EF_H

/* Effect module fx_0ef (dir 13 file 0x0EF): enemy Red Clipper (Stilva). Five timed emitter spawns around the target
 * body (fx_0ef_spawns). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ef effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_RED_CLIPPER_RED_SLASH  0x8 /* emitter: red slash texture + yellow rings + sparks (texture not in the file) */
#define FX_RED_CLIPPER_PINK_SMOKE 0xE /* emitter: pink smoke puffs */
#define FX_RED_CLIPPER_RED_SMOKE  0xF /* emitter: brown-red smoke puffs */

/* fx_0ef choreography (docs/fxdata.md, "Choreography literals"): op80's timed spawns, op81's slash and the op
 * lengths. */
#define FX_RED_CLIPPER_SPAWNS       5    /* op80: fx_0ef_spawns entries (emitter, frame, offset from the target body), emit0 / emit14 / emitPos40 slots */
#define FX_RED_CLIPPER_SLASH_HEIGHT 0x64 /* op81: RED_SLASH / RED_SMOKE start this far below the target's body point */
#define FX_RED_CLIPPER_FRAMES       0x28 /* op80's and op81's length (the ops end) */

typedef struct {
    Pool6CEntry *emit0[FX_RED_CLIPPER_SPAWNS];
    Pool6CEntry *emit14[FX_RED_CLIPPER_SPAWNS];
    u8 pad28[0x38 - 0x28];
    SVECTOR pos38;
    SVECTOR emitPos40[FX_RED_CLIPPER_SPAWNS];
} Fx0efWork80; /* 0x68 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
} Fx0efWork81; /* 0x10 */

typedef struct {
    u16 block0; /* file */
    u16 frame2; /* frame */
    s16 offset4[3]; /* offset */
    s16 padA;
} Fx0efSpawn;

extern Fx0efSpawn fx_0ef_spawns[FX_RED_CLIPPER_SPAWNS];
extern Unk800F78A4 *fx_0ef_env;

typedef struct {
    s16 x, y, z;
} Fx0efPoint;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx0efPoint *keys4;
} Fx0efPath;
static void fxm_posPathInit(Fx0efPath *path, u16 *data);
static s32 fxm_posPathGet(Fx0efPath *path, s32 t, SVECTOR *out);

#endif
