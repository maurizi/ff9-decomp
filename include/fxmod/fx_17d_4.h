#ifndef FX_17D_4_H
#define FX_17D_4_H

/* Effect module fx_17d_4 (dir 13 file 0x17d): Ark, full summon (load group 4). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_17d_4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_GREEN_TRAIL    0x1 /* emitter: orbiting 3D trails with a green-blue texture */
#define FX_ARK_RED_RING       0x3 /* rings: white-pink-red gouraud rings */
#define FX_ARK_RED_FLASH_RING 0x5 /* rings: red-white flat flash ring */
#define FX_ARK_MAGENTA_RING   0x17 /* emitter: grey noise rings and white-magenta-purple gouraud rings */
#define FX_ARK_SMOKE_TORUS    0x1D /* 'so' model: flat torus with a grey smoke texture (type 10 data below the group's slot) */
#define FX_ARK_SMOKE_CYLINDER 0x1F /* 'so' model: open cylinder with a grey smoke texture (type 10 data below the group's slot) */
#define FX_ARK_BEAM_LINE      0x20 /* table: trail data for fxm_drawTrailLine, vertical light line at the strand tips and ring centre (same use as fx_1bf_0's FX_ARK_BEAM_LINE) */
#define FX_ARK_FLAME_BOWL     0x21 /* 'so' model: shallow bowl with an orange-white fire texture (byte-identical to fx_1bf_0 entry 0x6) */
#define FX_ARK_STREAK_BOWL 0x84 /* summon data: 'so' model, bowl ring with a grey streak texture */
#define FX_ARK_SWIRL_DOME 0x86 /* summon data: 'so' model, dome with a grey swirl texture */
#define FX_ARK_STRAND_POINTS 0x8A /* summon data: offset table of the strand point lists per layer, read by fx_17d_4_drawStrands (byte-identical to fx_1bf_0 entry 0x1D) */

/* fx_17d_4 choreography (docs/fxdata.md, "Choreography literals"): op80's states (work->state0; the frame restarts at
 * each, named by what they draw) and their lengths, the trail buffer and the scrolling texture. */
#define FX_ARK_STATE_CHARGE  0    /* SMOKE_CYLINDER and FLAME_BOWL; then the summon's next motion */
#define FX_ARK_CHARGE_FRAMES 0x14
#define FX_ARK_STATE_HOLD    1    /* SMOKE_CYLINDER; then GREEN_TRAIL */
#define FX_ARK_HOLD_FRAMES   0xA
#define FX_ARK_STATE_SWIRL   2    /* SMOKE_CYLINDER and SWIRL_DOME, a growing white flash; then the next motion */
#define FX_ARK_SWIRL_FRAMES  0x14
#define FX_ARK_STATE_BOWL    3    /* STREAK_BOWL, FLAME_BOWL, SMOKE_TORUS, SWIRL_DOME; the flash fades over its first 0xC frames */
#define FX_ARK_BOWL_FRAMES   0x40
#define FX_ARK_STATE_BEAM    4    /* MAGENTA_RING, RED_RING and the BEAM_LINE come down */
#define FX_ARK_BEAM_FRAMES   0x14
#define FX_ARK_STATE_BEAM_2  5    /* the ring and beam move */
#define FX_ARK_BEAM_2_FRAMES 0x18
#define FX_ARK_STATE_BLAST   6    /* RED_FLASH_RING, the strands and particles (the op ends) */
#define FX_ARK_BLAST_FRAMES  0x33
#define FX_ARK_MODEL_COUNT   5    /* btlfx_initEffModels slots */
#define FX_ARK_GREEN_TRAIL_TRAIL_SLOTS  0x30 /* GREEN_TRAIL's trail buffer: slots of */
#define FX_ARK_GREEN_TRAIL_TRAIL_STRIDE 0x8C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
/* scrolling texture (fx_17d_2's SCROLL rect; not checked against the models) */
#define FX_ARK_SCROLL_VRAM_X 0x180
#define FX_ARK_SCROLL_VRAM_Y 0x100
#define FX_ARK_SCROLL_VRAM_W 0x40
#define FX_ARK_SCROLL_VRAM_H 0x80
#define FX_ARK_SCROLL_SRC_X  0x180
#define FX_ARK_SCROLL_SRC_Y  0x180

extern Unk800F78A4 *fx_17d_4_env;
extern s16 fx_17d_4_particleFileIds[16];
extern SVECTOR fx_17d_4_particleOffsets[3];
extern s16 fx_17d_4_pointFileIds[3];

typedef struct {
    s16 x, y, z;
} Fx17d4Pt;

/* propagated */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[2];
} Fx17d4Line;

extern Fx17d4Line fx_17d_4_lineTrail;
static void fxm_drawTrailLine(u8 *file, SVECTOR *from, SVECTOR *to, s32 frame, s32 fade, s32 abr, s32 lineWidth);

extern s16 fx_17d_4_strandInfo[18 * 4]; /* per strand: file, start, stride, width */
extern Fx17d4Line *fx_17d_4_strandTrails[18];
void fx_17d_4_drawStrands(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 w, s32 a, s32 fade, s32 flag);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d4Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* home position */
    SVECTOR pos10; /* particle position */
    u8 pad18[8];
    SVECTOR pos20; /* camera target */
    Fx17d4Vec3 pos28; /* model position */
    SVECTOR rot34; /* model rotation */
    Fx17d4Vec3 scale3C; /* model scale */
    Fx17d4Vec3 pos48; /* summon position */
    Fx17d4Vec3 scale54; /* summon scale */
    SVECTOR rot60; /* summon rotation */
    s32 model68; /* models */
    s32 model6C;
    s32 model70;
    s32 model74;
    s32 model78;
    u8 pad7C[8];
} Fx17d4Work80; /* 0x84 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x19C];
        BtlMotion *motion19C;
        BtlMotion *motion1A0;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx17d4Summon;

#define SUMMON(env) ((Fx17d4Summon *)(env)->loader)

extern SVECTOR fx_17d_4_centerPos;
extern s32 fx_17d_4_op80Trails[];
extern s8 fx_17d_4_strandTrailTail; /* spill area of the strand trail buffers (fx_17d_4_strandTrails) */
extern BtlModelBuf fx_17d_4_effModels[];
void fxm_drawPatternParticles(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot);
void fxm_drawOffsetParticles3(Unk80100C78Entry *entry, SVECTOR *from, s32 ot);

#endif
