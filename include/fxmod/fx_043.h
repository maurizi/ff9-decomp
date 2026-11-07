#ifndef FX_043_H
#define FX_043_H

/* Effect module fx_043 (dir 13 file 0x43). */
#include "btlfx_mod.h"

/* fx_043 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_INK_BLOTS      0x1 /* emitter: flat trail + spark dots, the black ink blots splattered on the target; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_INK_BLOTS_2    0x2 /* emitter: screen trail + spark dots, another set of the black ink blots on the target; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_INK_GREY_RINGS 0x3 /* emitter: grey gouraud rings */

/* fx_043 choreography (docs/fxdata.md, "Choreography literals"): op80's and op81's lengths and the blots' positions. */
#define FX_INK_FRAMES       0x1E /* op80: the task ends at this frame (FX_INK_BLOTS_2 flies to the target over 4 frames, frame << 10) */
#define FX_INK_BLOT_HEIGHT  0x64 /* the blots' positions: this far above the target body (op80's flight end, op81's pos18) */
#define FX_INK_BLOTS_REACH  0xC8 /* op81: FX_INK_BLOTS start this far from the target body towards the caster */
#define FX_INK_SPLAT_FRAMES 0x28 /* op81: the task ends at this frame */
#define FX_INK_BLOTS_2_TRAIL_SLOTS  0xB /* FX_INK_BLOTS_2's trail buffer fx_043_trails: slots of */
#define FX_INK_BLOTS_2_TRAIL_STRIDE 0x20 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_INK_BLOTS_TRAIL_SLOTS  0x34 /* FX_INK_BLOTS's trail buffer fx_043_trails2: slots of */
#define FX_INK_BLOTS_TRAIL_STRIDE 0x20 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    u8 pad4[8];
    SVECTOR posC;
    SVECTOR pos14; /* above the target body */
} Fx043Work80; /* 0x1C */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position */
    SVECTOR pos10; /* target position */
    SVECTOR pos18;
    SVECTOR aim20; /* angles from pos8 to fx_043_particleFrom */
} Fx043Work81; /* 0x28 */

extern Unk800F78A4 *fx_043_env;
extern SVECTOR fx_043_particleFrom; /* particle start */
extern SVECTOR fx_043_particleTo; /* particle end */
extern SVECTOR fx_043_emitPos;
extern s32 fx_043_trails[];
extern s32 fx_043_trails2[];

static s32 fx_043_op80ParticleMotion(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
