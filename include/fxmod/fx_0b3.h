#ifndef FX_0B3_H
#define FX_0B3_H

/* Effect module fx_0b3 (dir 13 file 0xB3): Leviathan, full summon. Scrolling water texture: btlfx_drawScrollMove /
 * fxm_drawScrollMoveX rotate the rows of the 32x128 source at (0x1E0, 0x100) and (0x1E0, 0x180) into (0x1C0, 0x100)
 * / (0x1C0, 0x180) every frame (runtime: VRAM dumps during the summon; not a screen capture). Evidence: btlfx_start
 * and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR emitPos0[8]; /* enemy positions */
    s32 unitCount40; /* unit count */
} Fx0b3Work86;

extern s32 fx_0b3_edgePtIdx;
extern SVECTOR fx_0b3_edgePts[16];

typedef struct {
    u16 point0Keys0; /* file id, track 0 */
    u16 point1Keys2; /* file id, track 1 */
    u16 endFrame4; /* end frame */
    u16 unk6; /* not read by the module */
} Fx0b3Track87;

extern Fx0b3Track87 fx_0b3_camShots[];

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0b3Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    u8 pad8[8];
    SVECTOR pos10; /* morph model position */
    u8 pad18[0x28 - 0x18];
    Fx0b3Vec3 morphPos28; /* morph model position */
    Fx0b3Vec3 morphScale34; /* morph model scale */
    SVECTOR rot40; /* morph model rotation */
    Fx0b3Vec3 modelPos48; /* model position */
    Fx0b3Vec3 modelScale54; /* model scale */
    SVECTOR rot60; /* model rotation */
    Pool6CEntry *emit68;
    s32 model6C; /* model */
    s32 morphFrame70; /* morph frame */
    s32 morphWeight74; /* morph weight */
} Fx0b3Work80; /* 0x78 */

extern BtlModelBuf fx_0b3_effModels[];

typedef struct {
    u8 pad0[0x184];
    BtlMotion *motion184;
    BtlMotion *motion188;
    BtlMotion *motion18C;
    BtlMotion *motion190;
} Fx0b3Unit;

typedef struct {
    u8 pad0[0x54];
    Fx0b3Unit *summon54; /* g_btlfxLoader.summon */
    u8 pad58[0x60 - 0x58];
    s32 model60; /* summon model */
} Fx0b3Summon;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    u8 pad8[8];
    SVECTOR pos10; /* morph model position */
    u8 pad18[0x28 - 0x18];
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
    Fx0b3Vec3 pos40; /* position */
    Fx0b3Vec3 scale4C; /* scale */
    SVECTOR rot58; /* rotation */
    Fx0b3Vec3 modelPos60;
    Fx0b3Vec3 modelScale6C;
    SVECTOR rot78;
    Pool6CEntry *emit80;
    Pool6CEntry *emit84;
    s32 model88[4]; /* models */
    s32 morphFrame98; /* morph frame */
    s32 morphWeight9C; /* morph weight */
} Fx0b3Work81; /* 0xA0 */

extern s32 fx_0b3_seaModel; /* model */
extern s32 fx_0b3_morphKeyBlocks[2]; /* file ids */
extern u8 fx_0b3_trails[];
extern u8 fx_0b3_trails2[];
extern SVECTOR fx_0b3_crestPts[20];

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    u8 pad8[0x28 - 0x8];
    SVECTOR pos28;
    SVECTOR pos30;
    u8 pad38[0x48 - 0x38];
    Fx0b3Vec3 summonPos48; /* position */
    Fx0b3Vec3 summonScale54; /* scale */
    SVECTOR rot60; /* rotation */
    Fx0b3Vec3 modelPos68;
    Fx0b3Vec3 modelScale74;
    SVECTOR rot80;
    s32 model88; /* model */
    s32 model8C; /* model */
} Fx0b3Work84; /* 0x90 */

static void fxm_drawWaveWall(SVECTOR *pos, SVECTOR *rot, Fx0b3Vec3 *scale, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9,
                   s32 a10, s32 a11, s32 a12, s32 a13, s32 a14);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    u8 pad8[0x18 - 0x8];
    SVECTOR pos18;
    SVECTOR pos20;
    u8 pad28[0x38 - 0x28];
    Fx0b3Vec3 summonPos38; /* position */
    Fx0b3Vec3 summonScale44; /* scale */
    SVECTOR rot50; /* rotation */
    Fx0b3Vec3 modelPos58;
    Fx0b3Vec3 modelScale64;
    SVECTOR rot70;
    s32 model78[4]; /* models */
} Fx0b3Work83; /* 0x88 */
extern s32 fx_0b3_partStart[4]; /* start frames */
extern s32 fx_0b3_partKeyBlocks[4]; /* file ids */

extern u16 fx_0b3_morphBlocks[6]; /* morph model file ids */
extern s32 fx_0b3_morphModels[6]; /* morph models */


static s32 fxm_sinHalfNeg(s32 a);
static s32 fxm_nextRingPoint(SVECTOR *v);
typedef struct {
    s16 pad0; /* always 0 in the effect file blocks (runtime, round 33) */
    s16 count2; /* count */
    s16 keys4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx0b3Track;
static void fxm_keyGetVec(Fx0b3Track *track, s32 t, SVECTOR *out);
static void fxm_keyGetVec2(Fx0b3Track *track, s32 t, SVECTOR *a, SVECTOR *b);
static void fxm_keyGetVec3(Fx0b3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c);
static s32 fxm_keyGetValue(Fx0b3Track *track, s32 t);
extern Unk800F78A4 *fx_0b3_env;
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    u8 pad8[8];
    SVECTOR pos10; /* midpoint */
    SVECTOR camPos18;
    u8 pad20[8];
    Fx0b3Vec3 summonPos28; /* summon position */
    Fx0b3Vec3 summonScale34; /* summon scale */
    SVECTOR rot40; /* summon rotation */
    u8 pad48[0x68 - 0x48];
    Pool6CEntry *emit68;
} Fx0b3Work85; /* 0x6C */

extern CVECTOR fx_0b3_rippleColor;
static void fxm_drawWaterSheet(SVECTOR *a, SVECTOR *b, VECTOR *c, s32 r, s32 k, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10,
                   s32 a11, s32 a12, s32 a13, s32 a14);
s32 fx_0b3_op85(s32 mode, Fx0b3Work85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
