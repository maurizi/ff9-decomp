#ifndef FX_1FD_H
#define FX_1FD_H

/* Effect module fx_1fd (dir 13 file 0x1fd): Fenrir (2nd) (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1fdPoint;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1fdPoint *keys4;
} Fx1fdPath;

typedef struct {
    Pool6CEntry *emit0[5];
    SVECTOR pos14[5];
} Fx1fdWork80; /* 0x3C */

extern u16 fx_1fd_emitFileIds[5]; /* files */
extern BtlModelBuf fx_1fd_effModels[];
extern Fx1fdPath fx_1fd_posPath[5];
extern s32 fx_1fd_op80Trails[5][0x78];
extern SVECTOR fx_1fd_emitPos[5];

typedef struct {
    Pool6CEntry *emit0[8];
    Pool6CEntry *emit20[8];
    SVECTOR pos40[8]; /* unit positions */
    SVECTOR emitPos80[8];
    s16 doneC0[8]; /* landed */
} Fx1fdWork84; /* 0xD0 */

typedef struct {
    SVECTOR pos0; /* unit position */
    SVECTOR rot8; /* unit rotation */
    s16 radius10; /* distance from the centre */
    s16 angle12; /* angle around it */
} Fx1fdOrbit; /* 0x14 */
extern s32 fx_1fd_op84Trails[8][0x90];

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
static s32 fxm_rotPosPathGet(Fx1fdPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);
static u16 *fxm_offsetTableEntry(u8 *base, u16 i);
static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b);

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
void fxm_projectEdges(void *track, s32 unused, s32 unusedLength, SVECTOR *scale, s32 w, s32 rotY);
extern Fx1fdOrbit fx_1fd_unitOrbits[];
s32 fx_1fd_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
void fxm_projectEdgesBlend(void *trackA, void *trackB, s32 unused, s32 unusedLength, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist);
void fxm_drawEdgeQuads(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1);

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
