#ifndef FX_1FC_H
#define FX_1FC_H

/* Effect module fx_1fc (dir 13 file 0x1fc). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1fcKey6;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1fcKey6 *unk4;
} Fx1fcTrack6;

typedef struct {
    s16 x, y, z;
} Fx1fcPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1fcPoint *unk4;
} Fx1fcPath;

extern BtlModel fx_1fc_effModels[];
extern u32 D_801EDADC[];
extern SVECTOR D_801EDEDC[8]; /* unit positions */
extern VECTOR D_801EDF1C[8]; /* unit scales */

typedef struct {
    Pool6CEntry *emit0[8];
    SVECTOR unk20[8]; /* unit positions */
    SVECTOR unk60[8];
    s16 unkA0[8]; /* landed */
} Fx1fcWork84; /* 0xB0 */


static inline void fx1fc_copy(SVECTOR *d, SVECTOR *s) {
    d->vx = s->vx;
    d->vy = s->vy;
    d->vz = s->vz;
}

/* copied */
extern Unk800F78A4 *fx_1fc_env;
/* end copied */

/* propagated */

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx1fcKey; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx1fcKey *unk4;
} Fx1fcPath_0b4;
static s32 fxm_rotPosPathGet(Fx1fcPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);
static s32 fxm_offsetTableEntry(u8 *base, u16 i);

typedef struct {
    u16 unk0;
    u16 unk2; /* count */
    s16 *unk4;
} Fx1fcKeys;
static s32 fxm_valuePathGet(Fx1fcKeys *keys, s32 i, s32 *out);
static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b);

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx1fcTrack; /* 0x10 */
extern Fx1fcTrack D_801EDF9C[];
extern s32 D_801EE39C[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1fcVec3;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* slice rotation */
    Fx1fcVec3 unk10; /* slice position */
    s32 pad1C;
    Fx1fcVec3 unk20; /* slice scale */
    s32 pad2C;
    SVECTOR rot30;
    Fx1fcVec3 unk38;
    s32 pad44;
    Fx1fcVec3 unk48;
    s32 pad54;
    SVECTOR unk58[8]; /* unit positions */
    VECTOR unk98[8]; /* unit scales */
    SVECTOR pos118;
    SVECTOR pos120;
    SVECTOR unk128;
    s16 unk130; /* fade counters */
    s16 unk132;
    s16 unk134;
    u16 model136[9]; /* models */
} Fx1fcWork83; /* 0x148 */

extern u16 D_801E8CC0[3];
extern s16 D_801E8CC8[];
extern s16 D_801E8CD8[];
extern u8 D_801E8CE8[];
extern Unk800E8F50 D_801E99A4;
extern Fx1fcKeys fx_1fc_rotPosPath0;
extern Fx1fcKeys fx_1fc_rotPosPath1;
extern Fx1fcKeys fx_1fc_valuePath;
extern Fx1fcKeys fx_1fc_rotPosPath2[10];
extern Fx1fcKeys fx_1fc_posPath;
extern Fx1fcKeys fx_1fc_vecPath;
static void fxm_posPathInit(Fx1fcPath *path, u16 *data);
static s32 fxm_posPathGet(Fx1fcPath *path, s32 t, SVECTOR *out);
static void fxm_rotPosPathInit(Fx1fcPath *path, u16 *data);
static void fxm_valuePathInit(Fx1fcPath *path, u16 *data);
static void fxm_vecPathInit(Fx1fcPath *path, u16 *data);
static s32 fxm_vecPathGet(Fx1fcTrack6 *track, s32 t, VECTOR *out);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);

s32 fx_1fc_op83(s32 mode, Fx1fcWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
