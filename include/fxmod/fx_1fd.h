#ifndef FX_1FD_H
#define FX_1FD_H

/* Effect module fx_1fd (dir 13 file 0x1fd): Fenrir (2nd) (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1fdPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1fdPoint *unk4;
} Fx1fdPath;

typedef struct {
    Pool6CEntry *emit0[5];
    SVECTOR unk14[5];
} Fx1fdWork80; /* 0x3C */

extern u16 D_801E9DF8[5]; /* files */
extern BtlModel fx_1fd_effModels[];
extern Fx1fdPath fx_1fd_posPath[5];
extern s32 fx_1fd_op80Trails[5][0x78];
extern SVECTOR fx_1fd_emitPos[5];

typedef struct {
    Pool6CEntry *emit0[8];
    Pool6CEntry *emit20[8];
    SVECTOR unk40[8]; /* unit positions */
    SVECTOR unk80[8];
    s16 unkC0[8]; /* landed */
} Fx1fdWork84; /* 0xD0 */

typedef struct {
    SVECTOR pos0; /* unit position */
    SVECTOR rot8; /* unit rotation */
    s16 unk10; /* distance from the centre */
    s16 unk12; /* angle around it */
} Fx1fdOrbit; /* 0x14 */
extern s32 fx_1fd_op84Trails[8][0x90];

static void fxm_posPathInit(Fx1fdPath *path, u16 *data);
static s32 fxm_posPathGet(Fx1fdPath *path, s32 t, SVECTOR *out);

/* copied */
extern Unk800F78A4 *fx_1fd_env;
/* end copied */

/* propagated */

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx1fdKey; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx1fdKey *unk4;
} Fx1fdPath_0b4;
static s32 fxm_rotPosPathGet(Fx1fdPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);
static u16 *fxm_offsetTableEntry(u8 *base, u16 i);
static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b);

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx1fdTrack; /* 0x10 */
extern Fx1fdTrack D_801ED73C[];
extern s32 D_801EDB3C[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);

typedef struct {
    DVECTOR unk0; /* screen xy of one edge end */
    DVECTOR unk4; /* screen xy of the other */
    s32 unk8; /* ot z */
} Fx1fdEdge;
extern Fx1fdEdge D_801EB80C[];
void func_801E7AF8(void *track, s32 arg1, s32 arg2, SVECTOR *scale, s32 w, s32 rotY);
extern Fx1fdOrbit D_801EBADC[];
s32 fx_1fd_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
void func_801E7704(void *trackA, void *trackB, s32 arg2, s32 arg3, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist);
void func_801E7E18(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1);

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    Fx1fdPath unk0;
    Fx1fdPath unk8;
    SVECTOR unk10; /* scale */
    s16 unk18;
    s16 unk1A; /* length */
    s16 unk1C; /* brightness */
    s16 unk1E; /* width */
    s16 unk20;
    s16 unk22;
    u8 pad24[0xC];
} Fx1fdRibbon; /* 0x30 */

typedef struct {
    Fx1fdPath unk0;
    SVECTOR unk8; /* scale */
    s16 unk10;
    s16 unk12; /* length */
    s16 unk14; /* brightness */
    s16 unk16; /* width */
    s16 unk18;
} Fx1fdRibbon1;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR rotC; /* model rotations */
    SVECTOR rot14;
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    s32 unk3C; /* model brightness */
    SVECTOR pos40; /* effect positions */
    SVECTOR pos48;
    SVECTOR unk50; /* caster position */
    s32 unk58; /* fade counters */
    s32 unk5C;
    s32 unk60;
    s16 unk64;
    s16 unk66;
    u16 model68; /* models */
    u16 model6A;
} Fx1fdWork82; /* 0x6C */

extern Fx1fdRibbon D_801EB75C;
extern Fx1fdRibbon D_801EB78C;
extern Fx1fdRibbon D_801EB7BC;
extern Fx1fdRibbon1 D_801EB7EC;

#endif
