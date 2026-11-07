#ifndef FX_12D_H
#define FX_12D_H

/* Effect module fx_12d (dir 13 file 0x12D): enemy Death (Antlion). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_12d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH_SAND_RING       0x1 /* emitter: ring with a sand texture */
#define FX_DEATH_SAND_DUST       0x2 /* emitter: pale sand-dust sprites */
#define FX_DEATH_SAND_CLOUD      0x3 /* emitter: spiralling pale dust clouds */
#define FX_DEATH_SAND_SWIRL      0x4 /* emitter: spiralling 3D trail with a grey sand texture */
#define FX_DEATH_PINK_GLOW_TRAIL 0xB /* emitter: spiralling 3D trail of pink glows */
#define FX_DEATH_ANTLION         0xE /* model: the Antlion (1142 polys), drawn with the caster's texture pages while the unit is hidden */

/* fx_12d choreography (docs/fxdata.md, "Choreography literals"): the ops' keyframes and lengths, the trail
 * buffers, the Antlion model's texture list (the caster's six pages, reversed) and its sinking. */
#define FX_DEATH_DUST_FRAME     4    /* op80: SAND_DUST starts (SAND_RING at init) */
#define FX_DEATH_RING_FRAMES    0x28 /* op80's length */
#define FX_DEATH_PINK_GLOW_TRAIL_TRAIL_SLOTS  8    /* PINK_GLOW_TRAIL's trail buffer fx_12d_op81Trails1: slots of */
#define FX_DEATH_PINK_GLOW_TRAIL_TRAIL_STRIDE 0x60 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_DEATH_SWIRL_FRAME    8    /* op81: SAND_CLOUD and SAND_SWIRL start */
#define FX_DEATH_SAND_SWIRL_TRAIL_SLOTS  0xC  /* SAND_SWIRL's trail buffer fx_12d_op81Trails0: slots of */
#define FX_DEATH_SAND_SWIRL_TRAIL_STRIDE 0x70 /* this many bytes */
#define FX_DEATH_SWIRL_FRAMES   0x3C /* op81's length */
#define FX_DEATH_ANTLION_TEX_PAGES  6    /* op82: the caster's texture pages / uv offsets copied, reversed, into ANTLION's list */
#define FX_DEATH_ANTLION_HOLD_FRAMES 0xC /* op82: ANTLION (sliced at its height) stays this many frames, then sinks 0xF a frame */
#define FX_DEATH_ANTLION_FRAMES     0x50 /* op82's length (the models are cleared) */

typedef struct {
    s32 unk0; /* unused */
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx12dWork80;

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4; /* unused */
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10;
    s16 unk18; /* unused */
    s16 unk1A; /* unused */
    s16 unk1C; /* write-only (-1 at init) */
} Fx12dWork81;

typedef struct {
    SVECTOR rot0; /* model rotation */
    VECTOR pos8; /* model position */
    VECTOR scale18; /* model scale */
    s32 unk28; /* unused */
    SVECTOR pos2C; /* caster position */
    u16 model34; /* model */
} Fx12dWork82;

extern BtlTexInfo fx_12d_casterTexPages;
extern s16 fx_12d_texInfo[FX_DEATH_ANTLION_TEX_PAGES][2]; /* fx_12d_casterTexPages's entries in reverse order */
extern BtlTexInfo fx_12d_casterUvOffsets;
extern s16 fx_12d_texPos[FX_DEATH_ANTLION_TEX_PAGES][2]; /* fx_12d_casterUvOffsets's entries in reverse order */
extern Unk800F78A4 *fx_12d_env;
extern BtlModelBuf fx_12d_effModels[];
extern SVECTOR fx_12d_casterPos;
extern s32 fx_12d_op81Trails0[];
extern s32 fx_12d_op81Trails1[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
