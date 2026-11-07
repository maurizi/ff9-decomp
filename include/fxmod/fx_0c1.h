#ifndef FX_0C1_H
#define FX_0C1_H

/* Effect module fx_0c1 (dir 13 file 0xC1): No Mercy. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0c1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_NO_MERCY_BLUE_RING    0x7 /* emitter: white / dark blue / cyan gouraud ring, fans */
#define FX_NO_MERCY_DUST_PLUME   0xB /* emitter: rising tan dust column and blue smoke */
#define FX_NO_MERCY_FLASH_BURST  0xC /* emitter: white star flash with a lens flare, grey-blue smoke, sparkles */
#define FX_NO_MERCY_IMPACT_BURST 0xD /* emitter: white / blue / orange rings, white glow, grey-blue smoke */

/* fx_0c1 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, length and the flash path. */
#define FX_NO_MERCY_BONE         0x17 /* unit 0x10's bone FX_NO_MERCY_BLUE_RING and the flash path start from */
#define FX_NO_MERCY_DASH_FRAME   0x24 /* op80: FX_NO_MERCY_DUST_PLUME and FX_NO_MERCY_FLASH_BURST start */
#define FX_NO_MERCY_FLASH_PATH_FRAMES 9 /* FX_NO_MERCY_FLASH_BURST's particles move from the bone to the caster in this many frames */
#define FX_NO_MERCY_DUST_REACH   0x12C /* op80: FX_NO_MERCY_DUST_PLUME starts this far in front of unit 0x10 */
#define FX_NO_MERCY_IMPACT_FRAME 0x2D /* op80: FX_NO_MERCY_IMPACT_BURST starts at the caster */
#define FX_NO_MERCY_FRAMES       0x46 /* op80: the task ends at this frame */

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
