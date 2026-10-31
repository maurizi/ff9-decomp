#ifndef FX_0E2_H
#define FX_0E2_H

/* Effect module fx_0e2 (dir 13 file 0x0E2): Fenrir (2nd), drawn with the summon model. Strips along key tracks (btlfx_drawTrail),
 * projected ribbons (fxm_projectEdges, fxm_drawEdgeQuads), tower and piece models and camera key tracks
 * (fxm_camChanUpdate). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[1]; /* (size - 4) / 8 points */
} Fx0e2Trail;

typedef struct {
    DVECTOR xy0; /* screen xy of one edge end */
    DVECTOR xy4; /* screen xy of the other */
    s32 otz8; /* ot z */
} Fx0e2Edge;

typedef struct {
    SVECTOR pos0; /* unit position */
    SVECTOR rot8; /* unit rotation */
    s16 dist10; /* distance from the centre */
    s16 angle12; /* angle around it */
} Fx0e2Orbit;

extern Unk800F78A4 *fx_0e2_env;
extern Fx0e2Orbit fx_0e2_orbits[];
extern u8 fx_0e2_op84Trails[][0x240]; /* per unit */

typedef struct {
    Pool6CEntry *emit0[8];
    Pool6CEntry *emit20[8];
    SVECTOR unitPos40[8]; /* unit positions */
    SVECTOR emitPos80[8]; /* effect positions */
    s16 landedC0[8]; /* unit landed */
} Fx0e2Work84; /* 0xD0 */
extern Fx0e2Edge fx_0e2_edges[60];
extern SVECTOR fx_0e2_stripOrigin; /* trail origin */
extern SVECTOR fx_0e2_headPos; /* trail head position */
extern SVECTOR fx_0e2_headRot; /* trail head rotation */
extern SVECTOR fx_0e2_aimPos; /* trail aim target */

void fx_0e2_stripFromTrackBlend(void *trackA, void *trackB, s32 t, Fx0e2Trail *trail, s32 size, s32 pad);
void fx_0e2_stripFromTrack(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 pad);
void fx_0e2_stripFromPosPath(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad);
void fx_0e2_stripFromTrackBlendRot(void *trackA, void *trackB, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad, s32 t,
                   s32 rotY);
void fxm_projectEdgesBlend(void *trackA, void *trackB, s32 arg2, s32 arg3, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist);
void fxm_projectEdges(void *track, s32 arg1, s32 arg2, SVECTOR *scale, s32 w, s32 rotY);
void fxm_drawEdgeQuads(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1);
typedef struct {
    Pool6CEntry *emit0[5];
    SVECTOR rot14; /* tower rotation */
    VECTOR pos1C; /* tower position */
    VECTOR scale2C; /* tower scale */
    SVECTOR rot3C; /* piece rotation */
    VECTOR pos44; /* piece position */
    VECTOR scale54; /* piece scale */
    SVECTOR blockRot64[4]; /* block rotations */
    VECTOR blockPos84[4]; /* block positions */
    VECTOR blockScaleC4[4]; /* block scales */
    SVECTOR emitPos104[4]; /* effect positions */
    u16 model124[12]; /* models: 3 tower stages, 5 pieces, 4 blocks */
} Fx0e2Work81; /* 0x13C */

extern u16 fx_0e2_towerBlocks[3];
extern u16 fx_0e2_pieceBlocks[5];
extern u32 fx_0e2_bgBuf[];
extern SVECTOR fx_0e2_blockPos[4];

s32 fx_0e2_op81(s32 mode, Fx0e2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_0e2_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_0e2_op84(s32 mode, Fx0e2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

typedef struct {
    s16 x, y, z;
} Fx0e2Point;

typedef struct {
    s16 unk0;
    u16 count2; /* count */
    Fx0e2Point *keys4;
} Fx0e2Path;
static void fxm_posPathInit(Fx0e2Path *path, u16 *data);
extern Fx0e2Path fx_0e2_pieceTracks[5]; /* piece tracks */
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *motion180[8]; /* summon motions */
    } *unk54;
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx0e2Summon;

typedef s32 (*Fx0e2RegTexList)(BtlUnit9C8 *, BtlTexInfo *, s16 *);

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8[5];
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
    Pool6CEntry *emit28[5];
    SVECTOR rot3C; /* summon rotation */
    VECTOR pos44; /* summon position */
    VECTOR pos54; /* model position */
    VECTOR scale64; /* scale */
    s32 summonRgb74;
    s32 rgb78;
    SVECTOR rot7C;
    VECTOR pos84;
    VECTOR scale94;
    VECTOR unkA4;
    s32 rgbB4;
    SVECTOR rotB8;
    VECTOR posC0;
    VECTOR scaleD0;
    SVECTOR rotE0;
    VECTOR posE8;
    VECTOR scaleF8;
    u8 pad108[8];
    SVECTOR emitPos110;
    SVECTOR emitPos118;
    SVECTOR emitPos120;
    s32 unk128;
    s32 unk12C;
    s32 unk130;
    s32 unk134;
    s32 morphStep138;
    s32 morphStep13C;
    s32 fadeTimer140;
    s32 fadeInTimer144;
    s32 fadeOutTimer148;
    s16 scrollStep14C;
    s16 scrollSpeed14E;
    u16 model150[14]; /* models */
} Fx0e2Work80; /* 0x16C */

typedef struct {
    Fx0e2Path pathA0;
    Fx0e2Path pathB8;
    s16 unk10;
    s16 fade12;
    s16 scale14;
    s16 width16;
} Fx0e2Ribbon80;

typedef struct {
    Fx0e2Path path0;
    s16 base8;
    s16 countA;
    s16 unkC;
    s16 unkE;
    s16 width10;
} Fx0e2Path80;

typedef struct {
    s32 pos0[2][4]; /* positions */
    s32 start20[3]; /* start position */
    u8 pad2C[4];
    s16 vel30[3]; /* velocity */
    s16 pad36;
} Fx0e2Drop; /* 0x38 */

typedef struct {
    u8 data[0x98];
} Fx0e2Trail98;

typedef struct {
    u8 data[0x1E0];
} Fx0e2Tex1E0;

static void fxm_rotPosPathInit(Fx0e2Path *path, u16 *data);
static s32 fxm_posPathGet(Fx0e2Path *path, s32 t, SVECTOR *out);
extern Fx0e2Drop fx_0e2_drops[3];
extern u16 fx_0e2_dropBlocks[3];
extern u16 fx_0e2_stripTrackBlocks[4];
extern u16 fx_0e2_emitBlocks0[5];
extern u16 fx_0e2_emitBlocks1[5];
extern BtlTexInfo fx_0e2_texPages[];
extern s16 fx_0e2_texPos[];
extern BtlModel fx_0e2_effModels[];
extern Fx0e2Ribbon80 fx_0e2_stripBlend;
extern Fx0e2Path fx_0e2_stripBlendPathB;
extern s16 fx_0e2_stripBlendFade;
extern Fx0e2Path80 fx_0e2_stripPath0;
extern Fx0e2Path80 fx_0e2_stripPath1;
extern Fx0e2Trail98 fx_0e2_strips[5];
extern Fx0e2Tex1E0 fx_0e2_op80Trails0[5];
extern u8 fx_0e2_op80Trails1[];

typedef struct {
    Fx0e2Path pathA0;
    Fx0e2Path pathB8;
    SVECTOR scale10; /* scale */
    s16 unk18;
    s16 length1A; /* length */
    s16 bright1C; /* brightness */
    s16 width1E; /* width */
    s16 blend20;
    s16 rotY22;
    u8 pad24[0xC];
} Fx0e2Ribbon; /* 0x30 */

typedef struct {
    Fx0e2Path path0;
    SVECTOR scale8; /* scale */
    s16 unk10;
    s16 length12; /* length */
    s16 bright14; /* brightness */
    s16 width16; /* width */
    s16 rotY18;
} Fx0e2Ribbon1;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR rotC; /* model rotations */
    SVECTOR rot14;
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    s32 rgb3C; /* model brightness */
    SVECTOR pos40; /* effect positions */
    SVECTOR pos48;
    SVECTOR center50; /* target centre, 0x3E8 up */
    s32 fadeTimer58; /* fade counters */
    s32 fadeTimer5C;
    s32 fadeTimer60;
    s16 scrollStep64;
    s16 scrollSpeed66;
    u16 model68; /* models */
    u16 model6A;
} Fx0e2Work82; /* 0x6C */

extern Fx0e2Ribbon fx_0e2_ribbon0;
extern Fx0e2Ribbon fx_0e2_ribbon1;
extern Fx0e2Ribbon fx_0e2_ribbon2;
extern Fx0e2Ribbon1 fx_0e2_ribbon3;
extern SVECTOR fx_0e2_stripHeads[5];
extern SVECTOR fx_0e2_emitPos0;
extern SVECTOR fx_0e2_emitPos1;
extern SVECTOR fx_0e2_emitPos2;
extern SVECTOR fx_0e2_emitPos3[5];
extern SVECTOR fx_0e2_emitPos4[5];

/* propagated */

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx0e2Key; /* 0xC */

typedef struct {
    s16 unk0;
    u16 count2; /* keys */
    Fx0e2Key *keys4;
} Fx0e2Path_0b4;
static s32 fxm_rotPosPathGet(Fx0e2Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);
static s32 fxm_offsetTableEntry(u8 *base, u16 i);
static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b);

typedef struct {
    s16 *data0;
    s16 start4; /* start frame */
    s16 left6; /* frames left, -1 = free */
    s16 chan8; /* channel */
    u16 dstA; /* destination slots, 4 bits each */
    u16 stepC;
    u16 stepsE;
} Fx0e2Track; /* 0x10 */
extern Fx0e2Track fx_0e2_camTracks[];
extern s32 fx_0e2_camChanFrame[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);

#endif
