#ifndef FX_1B6_H
#define FX_1B6_H

/* Effect module fx_1b6 (dir 13 file 0x1b6): enemy Death (Silver Dragon). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b6 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH_SPIRAL_TRAIL  0x2 /* emitter: spiralling 3D trail, strip mostly black with a faint white streak; runtime (docs/runtime-findings.md effect-data pass 2): started as the screen goes white; its run-time strip is a white streak (look pending) */
#define FX_DEATH_YELLOW_RINGS  0x3 /* emitter: cream and yellow gouraud rings */
#define FX_DEATH_RED_LIGHTNING 0x4 /* emitter: orbiting red lightning fan + yellow-to-brown gouraud rings */
#define FX_DEATH_BUBBLE_RINGS  0x5 /* emitter: rings textured with white bubble spots + pale-yellow gouraud rings */
#define FX_DEATH_STAR_SPECKS   0x7 /* emitter: cone of faint teal glows + white star dots, 40 frames */
#define FX_DEATH_CYAN_RINGS    0x8 /* emitter: cyan gouraud rings */

/* fx_1b6 choreography (docs/fxdata.md, "Choreography literals"): op80's charge (ring cross-fade, flickers, white
 * flash), then the caster swapped for seven models whose faces split off as flying pieces behind a sweep line; the
 * states, tints, piece pool and lengths. */
#define FX_DEATH_MODELS        7     /* btlfx_initEffModels slots, split models (fx_1b6_modelFileIds) */
#define FX_DEATH_PIECES        0x708 /* flying face pieces (Fx1b6Split pieces44) */
#define FX_DEATH_PIECE_FRAMES  0x3C  /* a piece flies this many frames, then its face is cleared */
#define FX_DEATH_STATE_CHARGE  0     /* work->state0: CYAN_RINGS fade into YELLOW_RINGS, flickers */
#define FX_DEATH_STATE_SHATTER 1     /* the models break into pieces */
#define FX_DEATH_CHARGE_FRAMES 0x18  /* STATE_CHARGE's length (and the rings' cross-fade) */
#define FX_DEATH_FLICKER_FRAME   1   /* additive grey flicker */
#define FX_DEATH_FLICKER_2_FRAME 0xF
#define FX_DEATH_FLICKER_RGB   0x40, 0x40, 0x40
#define FX_DEATH_WHITE_RGB     0xFF, 0xFF, 0xFF /* additive white on STATE_CHARGE's last frame */
#define FX_DEATH_FLASH_RGB     0x80, 0x80, 0x80 /* additive flash as the models replace the caster */
#define FX_DEATH_BLUE_RGB      0x20, 0x40, 0xFF /* additive blue flash on STATE_SHATTER's first frame */
#define FX_DEATH_WHITE_FADE_FRAMES 8 /* STATE_SHATTER: an additive white fades out over this many frames */
#define FX_DEATH_SPIRAL_TRAIL_TRAIL_SLOTS  0x20 /* SPIRAL_TRAIL's trail buffer fx_1b6_op80Trails: slots of */
#define FX_DEATH_SPIRAL_TRAIL_TRAIL_STRIDE 0x8C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_DEATH_SWEEP_FRAMES  0x31  /* the sweep line runs down the models while frame < this, then frees all faces */
#define FX_DEATH_SHATTER_FRAMES 0x3C /* STATE_SHATTER's length (the op ends) */

typedef struct {
    s32 posX0; /* position, 20.12 */
    s32 posY4;
    s32 posZ8;
    SVECTOR velC; /* velocity (vx, vy, vz), unk12: scale */
    s16 rotX14;
    s16 rotY16;
    s16 frame18; /* age, -1 = free */
    s16 face1A; /* face */
} Fx1b6Piece; /* 0x1C */

typedef struct {
    s16 count0; /* model count */
    s16 count2; /* face count */
    s32 counts4[FX_DEATH_MODELS]; /* faces per model */
    SVECTOR (*faces20)[3]; /* faces */
    SVECTOR (*nextFace24)[3]; /* next free face */
    SVECTOR (*modelFaces28[FX_DEATH_MODELS])[3]; /* faces of each model */
    Fx1b6Piece pieces44[FX_DEATH_PIECES];
} Fx1b6Split;

extern Unk800F78A4 *fx_1b6_env;
extern Fx1b6Split *fx_1b6_split;
extern Fx1b6Piece *fx_1b6_pieces;
extern s32 fx_1b6_sweepLine;

void fx_1b6_initSplit(Fx1b6Split *split, SVECTOR (*faces)[3]);
static s32 fxm_splitModelAdd(s32 model);
Fx1b6Piece *fx_1b6_findFreePiece(void);
void fx_1b6_launchPieces(void);
void fx_1b6_updatePieces(void);

typedef struct {
    u8 pad0[0x50];
    u8 *workBuf50; /* work buffer */
} Fx1b6SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1b6SummonData *summon54; /* g_btlfxLoader.summon */
} Fx1b6Summon;

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only: frame counter */
    s32 pos8[3]; /* caster position */
    SVECTOR rot14; /* caster rotation */
    s32 scale1C[3]; /* caster scale */
    s32 model28[FX_DEATH_MODELS]; /* split models */
    SVECTOR pos44; /* caster body position */
    SVECTOR pos4C;
    SVECTOR pos54;
    Pool6CEntry *emit5C;
    Pool6CEntry *emit60;
} Fx1b6Work80; /* 0x64 */

extern s16 fx_1b6_modelFileIds[FX_DEATH_MODELS]; /* model files */
extern Unk80100C78Entry *fx_1b6_task;
extern s32 fx_1b6_op80Trails[];
extern BtlModelBuf fx_1b6_effModels[];

#define FX1B6_SPAD_STACK(save) __asm__ volatile("sw $29, 0(%0)\n\tmove $29, %1" : : "r"(save), "r"(0x1F8003F8) : "memory")
#define FX1B6_RESET_STACK(save) __asm__ volatile("lw $29, 0(%0)" : : "r"(save) : "memory")

/* copied */
extern Unk800F78A4 *fx_1b6_env;
/* end copied */

#endif
