#ifndef FX_1B7_H
#define FX_1B7_H

/* Effect module fx_1b7 (dir 13 file 0x1b7): enemy Death (Nova Dragon). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b7 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH_VIOLET_RINGS        0x2 /* emitter: violet / magenta and cream / tan gouraud rings */
#define FX_DEATH_RED_LIGHTNING_TRAIL 0x3 /* emitter: wobbling 3D trail, red lightning strip */
#define FX_DEATH_YELLOW_GLOW         0x4 /* emitter: orbiting fan, yellow-green glow streak, 17 frames */
#define FX_DEATH_ORANGE_RINGS        0x5 /* emitter: pale-yellow / orange gouraud rings */
#define FX_DEATH_FLARE_STREAK        0x6 /* emitter: red-orange lens-flare streak + white-yellow-red gouraud rings */
#define FX_DEATH_STAR_SPECKS         0x7 /* emitter: spiral of faint teal glows + white star dots, 39 frames */
#define FX_DEATH_OLIVE_RINGS         0x8 /* emitter: pale-yellow / olive gouraud rings */
#define FX_DEATH_SMOKE_TRAIL         0x9 /* emitter: spiralling 3D trail, grey smoke strip */
#define FX_DEATH_BUBBLE_RINGS        0xA /* emitter: rings textured with olive bubble spots + pink / pale-cyan gouraud rings */
#define FX_DEATH_BLUE_BEAM           0xB /* emitter: blue cone-beam fan + grey gouraud rings */
#define FX_DEATH_BLUE_STREAK         0xC /* emitter: one fan, thin blue streak */

/* fx_1b7 choreography (docs/fxdata.md, "Choreography literals"): op80's charge (rings, lightning trail, beams,
 * dark / bright flicker pairs, white flash), then the caster swapped for eight models whose faces split off as
 * pieces blown sideways behind a rising sweep line; the states, tints, piece pool and lengths (fx_1b6's names). */
#define FX_DEATH_MODELS        8     /* btlfx_initEffModels slots, split models (fx_1b7_modelFileIds) */
#define FX_DEATH_PIECES        0x6A4 /* flying face pieces (Fx1b7Split pieces4C) */
#define FX_DEATH_PIECE_FRAMES  0x20  /* a piece flies this many frames, then its face is cleared */
#define FX_DEATH_STATE_CHARGE  0     /* work->state0: the charge emitters and flickers */
#define FX_DEATH_STATE_SHATTER 1     /* the models break into pieces */
#define FX_DEATH_LIGHTNING_FRAME 7   /* RED_LIGHTNING_TRAIL (VIOLET_RINGS at frame 0) */
#define FX_DEATH_RED_LIGHTNING_TRAIL_TRAIL_SLOTS  0x20 /* its trail buffer fx_1b7_op80Trails0: slots of */
#define FX_DEATH_RED_LIGHTNING_TRAIL_TRAIL_STRIDE 0xAC /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_DEATH_GLOW_FRAME    0xB   /* YELLOW_GLOW */
#define FX_DEATH_STREAK_FRAME  0x10  /* BLUE_STREAK (stopped when the models take over) */
#define FX_DEATH_RINGS_FRAME   0x1D  /* ORANGE_RINGS */
#define FX_DEATH_BEAM_FRAME    0x20  /* BLUE_BEAM */
#define FX_DEATH_FLARE_FRAME   0x27  /* FLARE_STREAK */
#define FX_DEATH_DARKEN_RGB    0x40, 0x80, 0x80 /* subtractive flicker on frames 1, 0xF, 0x14, 0x24 */
#define FX_DEATH_FLICKER_RGB   0x64, 0x64, 0x40 /* additive flicker on the frame after each */
#define FX_DEATH_WHITE_RGB     0xFF, 0xFF, 0xFF /* additive white on STATE_CHARGE's last frame */
#define FX_DEATH_CHARGE_FRAMES 0x2C  /* STATE_CHARGE's length */
#define FX_DEATH_FLASH_RGB     0xC0, 0xAA, 0x20 /* additive flash as the models replace the caster */
#define FX_DEATH_FLASH_2_RGB   0x60, 0x55, 0x10 /* additive flash on STATE_SHATTER's first frame */
#define FX_DEATH_GLARE_FRAME   4     /* STATE_SHATTER: a yellowish additive glare fades out from this frame */
#define FX_DEATH_GLARE_FRAMES  24    /* over this many frames */
#define FX_DEATH_SMOKE_TRAIL_TRAIL_SLOTS  0x1C /* SMOKE_TRAIL's trail buffer fx_1b7_op80Trails1: slots of */
#define FX_DEATH_SMOKE_TRAIL_TRAIL_STRIDE 0xAC /* this many bytes */
#define FX_DEATH_SHATTER_FRAMES 0x82 /* STATE_SHATTER's length (its ramp; the op ends) */

typedef struct {
    s32 posX0; /* position, 20.12 */
    s32 posY4;
    s32 posZ8;
    SVECTOR velC; /* velocity (vx, vy, vz), unk12: scale */
    s16 rotX14;
    s16 rotY16;
    s16 frame18; /* age, -1 = free */
    s16 face1A; /* face */
} Fx1b7Piece; /* 0x1C */

typedef struct {
    s16 count0; /* model count */
    s16 count2; /* face count */
    s32 counts4[FX_DEATH_MODELS]; /* faces per model */
    SVECTOR (*faces24)[3]; /* faces */
    SVECTOR (*nextFace28)[3]; /* next free face */
    SVECTOR (*modelFaces2C[FX_DEATH_MODELS])[3]; /* faces of each model */
    Fx1b7Piece pieces4C[FX_DEATH_PIECES];
} Fx1b7Split;

extern Fx1b7Piece *fx_1b7_pieces;
extern s32 fx_1b7_sweepLine;

void fx_1b7_initSplit(Fx1b7Split *split, SVECTOR (*faces)[3]);
static s32 fxm_splitModelAdd(s32 model);
Fx1b7Piece *fx_1b7_findFreePiece(void);
s32 fx_1b7_launchPieces(void);

typedef struct {
    u8 pad0[0x50];
    u8 *workBuf50; /* work buffer */
} Fx1b7SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1b7SummonData *summon54; /* g_btlfxLoader.summon */
} Fx1b7Summon;

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only: frame counter */
    s32 pos8[3]; /* caster position */
    SVECTOR rot14; /* caster rotation */
    s32 scale1C[3]; /* caster scale */
    SVECTOR pos28; /* caster body position */
    SVECTOR pos30;
    SVECTOR pos38;
    Pool6CEntry *emit40;
    s32 model44[FX_DEATH_MODELS]; /* split models */
} Fx1b7Work80; /* 0x64 */

extern s16 fx_1b7_modelFileIds[FX_DEATH_MODELS]; /* model files */
extern Unk80100C78Entry *fx_1b7_task;
extern BtlModelBuf fx_1b7_effModels[];
extern s32 fx_1b7_op80Trails0[];
extern s32 fx_1b7_op80Trails1[];

void fx_1b7_updatePieces(void);

#define FX1B7_SPAD_STACK(save) __asm__ volatile("sw $29, 0(%0)\n\tmove $29, %1" : : "r"(save), "r"(0x1F8003F8) : "memory")
#define FX1B7_RESET_STACK(save) __asm__ volatile("lw $29, 0(%0)" : : "r"(save) : "memory")

/* copied */
extern Unk800F78A4 *fx_1b7_env;
extern Fx1b7Split *fx_1b7_split;
/* end copied */

#endif
