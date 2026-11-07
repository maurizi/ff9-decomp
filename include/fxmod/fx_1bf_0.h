#ifndef FX_1BF_0_H
#define FX_1BF_0_H

/* Effect module fx_1bf_0 (dir 13 file 0x1bf): Ark (short); Eternal Darkness (load group 0). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1bf_0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_BEAM_LINE      0x5 /* table: trail data for fxm_drawTrailLine, vertical light line at the strand tips and ring centre */
#define FX_ARK_RED_FLASH_RING 0xA /* rings: red-white flat flash ring */
#define FX_ARK_RED_RING       0x1C /* rings: white-pink-red gouraud rings (byte-identical to fx_17d_4 entry 0x3) */
#define FX_ARK_STRAND_POINTS  0x1D /* table: offset table of the 18 strand point lists per layer (drawStrands) */

/* copied */
/* fx_1bf_0 choreography (docs/fxdata.md, "Choreography literals"): op80's two states, the last two of the long form's
 * (fx_17d_4: same names; the short form waits in BEAM_2 before the ring moves), and the keys inside BLAST. */
#define FX_ARK_STATE_BEAM_2  5    /* (the first state) after RING_FRAME, RED_RING and the BEAM_LINE orbit in to the centre */
#define FX_ARK_RING_FRAME    0x26
#define FX_ARK_BEAM_2_FRAMES 0x3E /*   (the orbit's (frame - RING_FRAME) << 12 / N ramp spans the rest) */
#define FX_ARK_STATE_BLAST   6    /* RED_FLASH_RING, the strands and particles (the op ends) */
#define FX_ARK_FLASH_RING_FRAMES 0x10 /*   the flat flash ring grows and fades over this many frames */
#define FX_ARK_STRANDS_FRAMES    0x1E /*   the strands grow one point a frame until this frame; then the outer flash ring, the
                                        *   pattern particles and a red tint come in */
#define FX_ARK_STRANDS_FADE_FRAME 0x26 /*   the strands widen and fade until the end */
#define FX_ARK_BLAST_FRAMES  0x33
#define FX_ARK_MODEL_COUNT   5    /* btlfx_initEffModels slots */

extern SVECTOR fx_1bf_0_particleOffsets[3];
extern s16 fx_1bf_0_particleFileIds[16];
extern Unk800F78A4 *fx_1bf_0_env;
/* end copied */

/* propagated */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[2];
} Fx1bf0Line;

extern Fx1bf0Line fx_1bf_0_lineTrail;
static void fxm_drawTrailLine(u8 *file, SVECTOR *from, SVECTOR *to, s32 frame, s32 fade, s32 abr, s32 lineWidth);

extern s16 fx_1bf_0_strandParams[18 * 4]; /* per strand: file, start, stride, width */
extern Fx1bf0Line *fx_1bf_0_strandTrails[18];
void fx_1bf_0_drawStrands(Unk80100C78Entry *entry, SVECTOR *from, s32 reach, s32 lineWidth, s32 alpha, s32 fade, s32 flag);
extern s16 fx_1bf_0_pointFileIds[];

typedef struct {
    s16 x, y, z;
} Fx1bf0Pt;
void fxm_drawPatternParticles(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot);

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only: frame counter */
    SVECTOR pos8;
    SVECTOR unk10; /* write-only (copy of pos8) */
    u8 pad18[0x28 - 0x18];
} Fx1bf0Work; /* 0x28 */

extern SVECTOR fx_1bf_0_origin;
extern BtlModelBuf fx_1bf_0_effModels[];
extern u8 fx_1bf_0_strandTrailTail[]; /* spill area of the strand trail buffers (fx_1bf_0_strandTrails) */
s32 fx_1bf_0_op80(s32 mode, Fx1bf0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
