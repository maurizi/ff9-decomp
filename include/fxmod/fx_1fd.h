#ifndef FX_1FD_H
#define FX_1FD_H

/* Effect module fx_1fd (dir 13 file 0x1fd): Fenrir (2nd) (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1fd effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FENRIR_RIBBON_PATHS_A   0x3 /* table: position paths of ribbon 0 (byte-identical to fx_0e2 entry 0x9) */
#define FX_FENRIR_RIBBON_PATHS_B   0x4 /* table: position paths of ribbon 1 (byte-identical to fx_0e2 entry 0xC) */
#define FX_FENRIR_WIND_RING 0x5 /* 'so' model (op84): flat ring with a green wind texture (byte-identical to fx_0e2 entry 0x11) */
#define FX_FENRIR_WIND_RING_2 0x6 /* 'so' model (op84): flat ring with a green wind texture (byte-identical to fx_0e2 entry 0x1D) */
#define FX_FENRIR_GREEN_RINGS_2    0x8 /* emitter: white-green / pale green gouraud rings (byte-identical to fx_0e2 entry 0x10) */
#define FX_FENRIR_CAM_TRACK_1      0xE /* table: camera key track, channel 1 (fxm_camChanAdd) */
#define FX_FENRIR_CAM_TRACK_2      0xF /* table: camera key track, channel 2 (fxm_camChanAdd) */
#define FX_FENRIR_EMIT_PATHS       0x10 /* table: five position paths of the op 0x80 trail emitters (fx_1fd_posPath) */
#define FX_FENRIR_GREEN_RINGS      0x11 /* emitter: pale green gouraud rings, 64 frames (byte-identical to fx_0e2 entry 0x4) */
#define FX_FENRIR_RIBBON_PATHS_C   0x12 /* table: position paths of ribbons 2/3 (fxm_offsetTableEntry, fxm_posPathInit) (byte-identical to fx_0e2 entry 0x1) */
#define FX_FENRIR_GREEN_WIND_TRAIL 0x13 /* emitter: spiralling green wind-streak trail + grey smoke (byte-identical to fx_0e2 entry 0xE) */
#define FX_FENRIR_SAND_RINGS       0x14 /* emitter: brown-salmon gouraud rings + spark sprites (texture not in the file) (byte-identical to fx_0e2 entry 0xD) */
#define FX_FENRIR_RIBBON_PATHS_D   0x15 /* table: position paths of ribbons 2/3 (other branch) (byte-identical to fx_0e2 entry 0xF) */
#define FX_FENRIR_CYAN_GLINTS      0x16 /* emitter: spiral of tiny cyan-white glint sprites, the glints round the green whirlwind (byte-identical to fx_0e2 entry 0x12); runtime (docs/runtime-findings.md effect-data pass 2, medium) */

/* fx_1fd choreography (docs/fxdata.md, "Choreography literals"; Fenrir's second form (fx_0e2) names where the code
 * matches; partial: the ops' lengths, work sizes, trail buffers, counts, primitive words and the scrolled rects; the
 * ribbons' path keys in op82 stay numeric). */
#define FX_FENRIR_MODEL_COUNT 0x20 /* btlfx_initEffModels slots */
#define FX_FENRIR_EDGES       60   /* fx_1fd_edges: the screen-edge strip's points */
#define FX_FENRIR_HEADS       5    /* op80's five wind heads (fx_1fd_emitFileIds / posPath / emitPos / op80Trails) */
#define FX_FENRIR_HEAD_TRAIL_SLOTS         6    /* the heads' trail buffers fx_1fd_op80Trails[slot]: slots of */
#define FX_FENRIR_HEAD_TRAIL_STRIDE        0x50 /* this many bytes */
#define FX_FENRIR_HEADS_FRAMES 0x78 /* op80: the heads follow their paths, the camera its tracks; the task ends at this frame */
#define FX_FENRIR_RIBBONS_FRAMES 0x78 /* op82 (the four edge-quad ribbons, the scrolled wind textures): the task ends here */
#define FX_FENRIR_RIBBON_FRAMES  60  /* op82: ribbons 0 / 1 grow one point a frame up to this many (and end at this frame) */
#define FX_FENRIR_LIFT_FRAMES 0x40 /* op83: the units orbit and rise 120 a frame until this frame */
#define FX_FENRIR_ORBIT_FRAMES 0x46 /* op83: the task ends at this frame */
#define FX_FENRIR_UNIT_SLOTS  8    /* op84: per-unit emitters, positions and landed flags (Fx1fdWork84) */
#define FX_FENRIR_GREEN_WIND_TRAIL_TRAIL_SLOTS  0xC  /* op84: FX_FENRIR_GREEN_WIND_TRAIL's trail buffer per unit (fx_1fd_op84Trails): slots of */
#define FX_FENRIR_GREEN_WIND_TRAIL_TRAIL_STRIDE 0x30 /* this many bytes */
#define FX_FENRIR_LAND_FRAMES 0x3C /* op84 (the units come home): the task ends at this frame */
#define FX_FENRIR_CAM_KEY_SLOTS 64 /* fx_1fd_camKeys: camera key tracks (fxm_camChanAdd / fxm_camChanUpdate) */
#define FX_FENRIR_CAM_CHANNELS  4  /* fx_1fd_camChanFrames: frame counters per channel set */
#define FX_FENRIR_WIND_VRAM_Y   0x180 /* op82: fxm_drawScrollMoveX scrolls two 8-bit wind textures (rows 0x80 .. 0xBF of */
#define FX_FENRIR_WIND_VRAM_W   0x80  /*   the 0x97 and 0x99 pages, the ribbons' fxm_drawEdgeQuads textures) */
#define FX_FENRIR_WIND_VRAM_H   0x40
#define FX_FENRIR_WIND_VRAM_X   0x1C0 /*   page 0x97 (0x1C0, 0x100), from (0x1C0, 0x1C0) */
#define FX_FENRIR_WIND_2_VRAM_X 0x240 /*   page 0x99 (0x240, 0x100), from (0x240, 0x100) */

typedef struct {
    s16 x, y, z;
} Fx1fdPoint;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1fdPoint *keys4;
} Fx1fdPath;

typedef struct {
    Pool6CEntry *emit0[FX_FENRIR_HEADS];
    SVECTOR pos14[FX_FENRIR_HEADS];
} Fx1fdWork80; /* 0x3C */

extern u16 fx_1fd_emitFileIds[FX_FENRIR_HEADS]; /* files */
extern BtlModelBuf fx_1fd_effModels[];
extern Fx1fdPath fx_1fd_posPath[FX_FENRIR_HEADS];
extern s32 fx_1fd_op80Trails[FX_FENRIR_HEADS][0x78];
extern SVECTOR fx_1fd_emitPos[FX_FENRIR_HEADS];

typedef struct {
    Pool6CEntry *emit0[FX_FENRIR_UNIT_SLOTS];
    Pool6CEntry *emit20[FX_FENRIR_UNIT_SLOTS];
    SVECTOR pos40[FX_FENRIR_UNIT_SLOTS]; /* unit positions */
    SVECTOR emitPos80[FX_FENRIR_UNIT_SLOTS];
    s16 doneC0[FX_FENRIR_UNIT_SLOTS]; /* landed */
} Fx1fdWork84; /* 0xD0 */

typedef struct {
    SVECTOR pos0; /* unit position */
    SVECTOR rot8; /* unit rotation */
    s16 radius10; /* distance from the centre */
    s16 angle12; /* angle around it */
} Fx1fdOrbit; /* 0x14 */
extern s32 fx_1fd_op84Trails[FX_FENRIR_UNIT_SLOTS][0x90];

static void fxm_posPathInit(Fx1fdPath *path, u16 *data);
static s32 fxm_posPathGet(Fx1fdPath *path, s32 t, SVECTOR *out);

/* copied */
extern Unk800F78A4 *fx_1fd_env;
/* end copied */

/* propagated */

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx1fdKey; /* 0xC */

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* keys */
    Fx1fdKey *keys4;
} Fx1fdPath_0b4;
static s32 fxm_rotPosPathGet(Fx1fdPath_0b4 *path, s32 t, SVECTOR *rot, SVECTOR *pos);
static u16 *fxm_offsetTableEntry(u8 *base, u16 idx);
static s32 fxm_moveUnitHome(s32 unit, s32 t, s32 n, SVECTOR *start);

typedef struct {
    s16 *keys0;
    s16 startFrame4; /* start frame */
    s16 timer6; /* frames left, -1 = free */
    s16 channel8; /* channel */
    u16 slotsA; /* destination slots, 4 bits each */
    u16 indexC;
    u16 countE;
} Fx1fdTrack; /* 0x10 */
extern Fx1fdTrack fx_1fd_camKeys[];
#define FX_1FD_KEY_FREE (-1) /* timer6: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
extern s32 fx_1fd_camChanFrames[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);

typedef struct {
    DVECTOR sxy0; /* screen xy of one edge end */
    DVECTOR sxy4; /* screen xy of the other */
    s32 otz8; /* ot z */
} Fx1fdEdge;
extern Fx1fdEdge fx_1fd_ribbonEdges[];
void fxm_projectEdges(void *track, s32 unused, s32 unusedLength, SVECTOR *scale, s32 width, s32 rotY);
extern Fx1fdOrbit fx_1fd_unitOrbits[];
s32 fx_1fd_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
void fxm_projectEdgesBlend(void *trackA, void *trackB, s32 unused, s32 unusedLength, SVECTOR *scale, s32 width, s32 t, s32 rotY, s32 twist);
void fxm_drawEdgeQuads(s32 n, s32 bright, s32 tpage, s32 clut, u16 v0, u16 v1);

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    Fx1fdPath path0;
    Fx1fdPath path8;
    SVECTOR scale10; /* scale */
    s16 unk18; /* passed to fxm_projectEdgesBlend, which ignores that argument (cleared at init) */
    s16 count1A; /* length */
    s16 bright1C; /* brightness */
    s16 width1E; /* width */
    s16 blend20;
    s16 angle22;
    u8 pad24[0xC];
} Fx1fdRibbon; /* 0x30 */

typedef struct {
    Fx1fdPath path0;
    SVECTOR scale8; /* scale */
    s16 unk10; /* passed to fxm_projectEdges, which ignores that argument (cleared at init) */
    s16 count12; /* length */
    s16 bright14; /* brightness */
    s16 width16; /* width */
    s16 angle18;
} Fx1fdRibbon1;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR rotC; /* model rotations */
    SVECTOR rot14;
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    s32 tint3C; /* model brightness */
    SVECTOR pos40; /* effect positions */
    SVECTOR pos48;
    SVECTOR unk50; /* write-only: target centre raised by 0x3E8 */
    s32 timer58; /* fade counters */
    s32 timer5C;
    s32 timer60;
    s16 frame64;
    s16 speed66;
    u16 model68; /* models */
    u16 model6A;
} Fx1fdWork82; /* 0x6C */

extern Fx1fdRibbon fx_1fd_ribbon0;
extern Fx1fdRibbon fx_1fd_ribbon1;
extern Fx1fdRibbon fx_1fd_ribbon2;
extern Fx1fdRibbon1 fx_1fd_ribbon3;

#endif
