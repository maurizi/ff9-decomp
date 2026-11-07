#ifndef FX_17D_3_H
#define FX_17D_3_H

/* Effect module fx_17d_3 (dir 13 file 0x17d): Ark, full summon (load group 3). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_17d_3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_SMOKE_TRAIL   0x1 /* emitter: spiralling screen trail with a blue-grey smoke texture, orange embers */
#define FX_ARK_CLOUD_TRAIL   0x2 /* emitter: two spiralling screen trails with a grey cloud texture */
#define FX_ARK_SMOKE_TRAIL_2 0x4 /* emitter: spiralling screen trail with a blue-grey smoke texture, orange embers */
#define FX_ARK_SMOKE_TRAIL_3 0x6 /* emitter: spiralling screen trail with a blue-grey smoke texture, orange embers */
#define FX_ARK_DUST_CLOUD    0x9 /* emitter: grey dust clouds and grey sparks */
#define FX_ARK_SMOKE_CLOUDS  0xE /* emitter: two fans of grey-purple smoke clouds fading out */
#define FX_ARK_WING_PLATE 0x80 /* summon data: 'so' model, V-shaped plate with a grey metal texture */
#define FX_ARK_GREY_CYLINDER 0x81 /* summon data: 'so' model, cylinder with a grey patterned texture */
#define FX_ARK_WING_PLATE_2 0x82 /* summon data: 'so' model, flat plate with a grey metal texture and a dark centre bar */
#define FX_ARK_TEAL_CYLINDER 0x83 /* summon data: 'so' model, cylinder with a dark teal patterned texture */

/* fx_17d_3 choreography (docs/fxdata.md, "Choreography literals"): op80's shots (work->state0; the frame restarts at
 * each; some switch the summon's motion at their end), their lengths and emitter keyframes, the trail buffers and
 * the scrolling textures. */
#define FX_ARK_STATE_CLOUDS   0    /* a white flash, CLOUD_TRAIL behind the Ark */
#define FX_ARK_STATE_CLOUDS_2 1    /* CLOUD_TRAIL again; then summon motion 2 */
#define FX_ARK_CLOUDS_FRAMES  0x18
#define FX_ARK_STATE_VOLLEY   2    /* a SMOKE_TRAIL pair; then motion 4 */
#define FX_ARK_STATE_VOLLEY_2 3    /* a SMOKE_TRAIL_3 pair; then motion 5 */
#define FX_ARK_STATE_VOLLEY_3 4    /* a SMOKE_TRAIL_3 pair at VOLLEY_3_FRAME */
#define FX_ARK_STATE_VOLLEY_4 5    /* SMOKE_TRAIL_2, SMOKE_CLOUDS and a SMOKE_TRAIL_3 */
#define FX_ARK_STATE_VOLLEY_5 6    /* the same; then motion 6 */
#define FX_ARK_VOLLEY_FRAMES  0x15 /* VOLLEY, VOLLEY_2, VOLLEY_4 and VOLLEY_5's length */
#define FX_ARK_VOLLEY_3_FRAMES 0x18 /* also the motion 5 frame it ends on, held through VOLLEY_4 / VOLLEY_5 */
#define FX_ARK_STATE_DUST     7    /* DUST_CLOUD; the op ends */
#define FX_ARK_DUST_FRAMES    0x20
#define FX_ARK_SMOKE_FRAME    0xC  /* VOLLEY / VOLLEY_2: the second trail of the pair starts */
#define FX_ARK_VOLLEY_3_FRAME 6
#define FX_ARK_SMOKE_2_FRAME  0xB  /* VOLLEY_4 / VOLLEY_5: SMOKE_TRAIL_2 and SMOKE_CLOUDS start */
#define FX_ARK_DUST_FRAME     0xA  /* STATE_DUST: DUST_CLOUD starts */
#define FX_ARK_WHITE_RGB      0xFF, 0xFF, 0xFF /* STATE_CLOUDS frame 0: opaque */
#define FX_ARK_MODEL_COUNT    5    /* btlfx_initEffModels slots */
#define FX_ARK_CLOUD_TRAIL_TRAIL_SLOTS  0x10 /* CLOUD_TRAIL's trail buffer: slots of */
#define FX_ARK_CLOUD_TRAIL_TRAIL_STRIDE 0xCC /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_ARK_SMOKE_TRAIL_TRAIL_SLOTS  0x32 /* SMOKE_TRAIL / _2 / _3's */
#define FX_ARK_SMOKE_TRAIL_TRAIL_STRIDE 0x1C
/* scrolling textures (not checked against the models): SCROLL 0x18 texels a frame, SCROLL_2 0x10, SCROLL_3 0x10 */
#define FX_ARK_SCROLL_VRAM_X   0x300
#define FX_ARK_SCROLL_VRAM_Y   0x140
#define FX_ARK_SCROLL_VRAM_W   0x20
#define FX_ARK_SCROLL_VRAM_H   0x80
#define FX_ARK_SCROLL_SRC_X    0x320
#define FX_ARK_SCROLL_SRC_Y    0x140
#define FX_ARK_SCROLL_2_VRAM_X 0x2C0
#define FX_ARK_SCROLL_2_VRAM_Y 0x100
#define FX_ARK_SCROLL_2_VRAM_W 0x40
#define FX_ARK_SCROLL_2_VRAM_H 0x80
#define FX_ARK_SCROLL_2_SRC_X  0x2C0
#define FX_ARK_SCROLL_2_SRC_Y  0x180
#define FX_ARK_SCROLL_3_VRAM_X 0x340
#define FX_ARK_SCROLL_3_VRAM_Y 0x100
#define FX_ARK_SCROLL_3_VRAM_W 0x20
#define FX_ARK_SCROLL_3_VRAM_H 0x80
#define FX_ARK_SCROLL_3_SRC_X  0x360
#define FX_ARK_SCROLL_3_SRC_Y  0x100

/* fx_17d_3 choreography, deep pass (docs/fxchoreo_deep_c.md): scroll speeds, model scales, the summon motion
 * holds inside the shots, the camera keys */
#define FX_ARK_SCROLL_SPEED    0x18 /* texels a frame (btlfx_drawScrollMove's offset = frame * SPEED) */
#define FX_ARK_SCROLL_2_SPEED  0x10
#define FX_ARK_SCROLL_3_SPEED  0x10 /* (counted on work->frame4, not the shot frame) */
#define FX_ARK_SUMMON_SCALE    0x1800 /* the Ark summon model's scale (scale54) */
#define FX_ARK_CYLINDER_SCALE  0x800  /* TEAL_CYLINDER's scale (scale3C); GREY_CYLINDER is drawn at a fraction of it */
#define FX_ARK_WING_PLATE_SCALE_XZ 0x555 /* CLOUDS / CLOUDS_2: the WING_PLATE_2 -> WING_PLATE morph's scale */
#define FX_ARK_WING_PLATE_SCALE_Y  0xAAA
#define FX_ARK_GRID_SCALE      0x1040 /* CLOUDS / CLOUDS_2: the fading screen grid's scale */
#define FX_ARK_VOLLEY_MOTION_FRAME 10 /* VOLLEY / VOLLEY_2: the summon motion is held at frame 0 until here, */
#define FX_ARK_VOLLEY_MOTION_END   20 /* VOLLEY: and frozen from here (at motion frame END - MOTION_FRAME) */
#define FX_ARK_VOLLEY_2_MOTION_END 17 /* VOLLEY_2: the same */
#define FX_ARK_CAMERA_DIP_FRAMES   18 /* VOLLEY_2: camera point 3 sinks over these frames (a cosine quarter) */
#define FX_ARK_DUST_MOTION_END     0x17 /* STATE_DUST: motion 6 is frozen at this frame from this frame */
#define FX_ARK_DUST_BONE       4    /* STATE_DUST (and VOLLEY_5's end): the summon bone camera point 3 and DUST_CLOUD follow */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180[8]; /* summon motions */
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx17d3Summon;

typedef struct {
    u32 state0; /* state */
    s32 frame4; /* counter */
    u8 pad8[8];
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR pos20; /* camera */
    s32 pos28[3]; /* model position */
    SVECTOR rot34; /* model rotation */
    s32 scale3C[3]; /* model scale */
    s32 pos48[3]; /* summon position */
    s32 scale54[3]; /* summon scale */
    SVECTOR unk60; /* write-only (cleared at init) */
    s32 model68; /* models */
    s32 model6C;
    s32 model70;
    s32 model74;
    Pool6CEntry *emit78;
    Pool6CEntry *emit7C;
} Fx17d3Work80; /* 0x80 */

extern Unk800F78A4 *fx_17d_3_env;
extern BtlModelBuf fx_17d_3_effModels[];
extern u8 fx_17d_3_bones0[];
extern u8 fx_17d_3_bones1[];
extern u8 fx_17d_3_bones2[];
extern u8 fx_17d_3_bones3[];
extern u8 fx_17d_3_op80Trails0[];
extern u8 fx_17d_3_op80Trails1[];
extern u8 fx_17d_3_op80Trails2[];

#define SUMMON(env) ((Fx17d3Summon *)(env)->loader)

#endif
