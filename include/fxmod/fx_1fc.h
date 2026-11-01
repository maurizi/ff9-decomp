#ifndef FX_1FC_H
#define FX_1FC_H

/* Effect module fx_1fc (dir 13 file 0x1fc): Fenrir (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1fcKey6;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1fcKey6 *keys4;
} Fx1fcTrack6;

typedef struct {
    s16 x, y, z;
} Fx1fcPoint;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1fcPoint *keys4;
} Fx1fcPath;

extern BtlModelBuf fx_1fc_effModels[];
extern u32 fx_1fc_bgOt[];
extern SVECTOR fx_1fc_unitPos[8]; /* unit positions */
extern VECTOR fx_1fc_unitScale[8]; /* unit scales */

typedef struct {
    Pool6CEntry *emit0[8];
    SVECTOR pos20[8]; /* unit positions */
    SVECTOR emitPos60[8];
    s16 doneA0[8]; /* landed */
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
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx1fcKey; /* 0xC */

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* keys */
    Fx1fcKey *keys4;
} Fx1fcPath_0b4;
static s32 fxm_rotPosPathGet(Fx1fcPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);
static s32 fxm_offsetTableEntry(u8 *base, u16 i);

typedef struct {
    u16 unk0; /* not read by the module */
    u16 count2; /* count */
    s16 *keys4;
} Fx1fcKeys;
static s32 fxm_valuePathGet(Fx1fcKeys *keys, s32 i, s32 *out);
static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b);

typedef struct {
    s16 *keys0;
    s16 startFrame4; /* start frame */
    s16 timer6; /* frames left, -1 = free */
    s16 channel8; /* channel */
    u16 slotsA; /* destination slots, 4 bits each */
    u16 indexC;
    u16 countE;
} Fx1fcTrack; /* 0x10 */
extern Fx1fcTrack fx_1fc_camKeys[];
#define FX_1FC_KEY_FREE (-1) /* timer6: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
extern s32 fx_1fc_camChanFrames[];
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
    Fx1fcVec3 pos10; /* slice position */
    s32 pad1C;
    Fx1fcVec3 scale20; /* slice scale */
    s32 pad2C;
    SVECTOR rot30;
    Fx1fcVec3 pos38;
    s32 pad44;
    Fx1fcVec3 scale48;
    s32 pad54;
    SVECTOR pos58[8]; /* unit positions */
    VECTOR scale98[8]; /* unit scales */
    SVECTOR pos118;
    SVECTOR pos120;
    SVECTOR rot128;
    s16 timer130; /* fade counters */
    s16 timer132;
    s16 active134;
    u16 model136[9]; /* models */
} Fx1fcWork83; /* 0x148 */

extern u16 fx_1fc_modelFileIds[3];
extern s16 fx_1fc_texPages[];
extern s16 fx_1fc_texUv[];
extern u8 fx_1fc_effModelTable[];
extern Unk800E8F50 fx_1fc_sliceModelData;
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

s32 fx_1fc_op83(s32 mode, Fx1fcWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
