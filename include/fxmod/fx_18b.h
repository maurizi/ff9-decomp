#ifndef FX_18B_H
#define FX_18B_H

/* Effect module fx_18b (dir 13 file 0x18b): Gizamaluke's Silent Voice. Captures the screen around the target into
 * VRAM_BTLFX_SCRATCH and draws it back. Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_18b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SILENT_VOICE_SWIRLS 0x1 /* emitter: violet and blue spiral swirl particles + white-blue-violet gouraud rings */

/* fx_18b choreography (docs/fxdata.md, "Choreography literals"): the rippling radial screen grid (a copy of the
 * screen through the scratch VRAM rect) swelling and ebbing, the swirls' keyframe; the states and lengths. */
#define FX_SILENT_VOICE_STATE_SWELL 0    /* work->state0: the ripple grows (wave amplitude up, phase turning one way) */
#define FX_SILENT_VOICE_STATE_EBB   1    /* it dies down (phase turning back); the op ends after it */
#define FX_SILENT_VOICE_SWELL_FRAMES 0x28 /* STATE_SWELL's length (and its amplitude ramp) */
#define FX_SILENT_VOICE_EBB_FRAMES  0x28 /* STATE_EBB's length (and its amplitude ramp) */
#define FX_SILENT_VOICE_SWIRLS_FRAME 1   /* STATE_EBB: SWIRLS at the target */
#define FX_SILENT_VOICE_GRID_W      0x100 /* the screen copy's rect at (VRAM_BTLFX_SCRATCH_X, _Y) */
#define FX_SILENT_VOICE_GRID_H      0x100

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* counter */
    s32 unk8; /* write-only (btlfx_addRadiusSixth(0, 0x20)) */
    SVECTOR posC; /* target body position */
    SVECTOR pos14; /* caster body position */
} Fx18bWork; /* 0x1C */

extern Unk800F78A4 *fx_18b_env;
extern s32 fx_18b_wavePhase; /* wave phase */
extern s32 fx_18b_waveAmp; /* wave amplitude */

static void fx_18b_op80GridVertex(s32 radius, s32 ring, s32 ang, s32 spoke, s16 *out);

/* propagated */
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
