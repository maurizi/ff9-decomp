#ifndef FX_0B3_H
#define FX_0B3_H

/* Effect module fx_0b3 (dir 13 file 0xB3): Leviathan, full summon. Scrolling water texture: btlfx_drawScrollMove /
 * fxm_drawScrollMoveX rotate the rows of the 32x128 source at (0x1E0, 0x100) and (0x1E0, 0x180) into (0x1C0, 0x100)
 * / (0x1C0, 0x180) every frame (runtime: VRAM dumps during the summon; not a screen capture). Evidence: btlfx_start
 * and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0b3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LEVIATHAN_WATER_SPLASH       0x2 /* emitter: blue water-splash sprites and droplets */
#define FX_LEVIATHAN_SUMMON_KEYS        0x3 /* table: key track (position, rotation, scale) of the summoned body (fxm_keyGetVec3) */
#define FX_LEVIATHAN_FAN_EMITTER        0x4 /* emitter: orbiting fan element (not rendered) (look pending) */
#define FX_LEVIATHAN_WATER_DROPS        0x5 /* emitter: water droplet sprites */
#define FX_LEVIATHAN_WATER_GLINT        0x6 /* emitter: particles, the render shows the whole water texture page (look pending) */
#define FX_LEVIATHAN_BLUE_RING          0x7 /* emitter: blue gouraud ring + water-page particle */
#define FX_LEVIATHAN_SO_MODEL           0x8 /* 'so' model registered by op80 (look pending) */
#define FX_LEVIATHAN_MORPH_POS_KEYS     0xB /* table: key track, position of the morph models (op80) */
#define FX_LEVIATHAN_MODEL_SCALE_KEYS_3 0xC /* table: key track, scale of an effect model (op84) */
#define FX_LEVIATHAN_SUMMON_POS_KEYS    0x13 /* table: key track, position of the summon model (op83) */
#define FX_LEVIATHAN_SUMMON_POS_KEYS_2  0x15 /* table: key track, position of the summon model (op84) */
#define FX_LEVIATHAN_SUMMON_POS_KEYS_3  0x16 /* table: key track, position of the summon model (op85) */
#define FX_LEVIATHAN_ORBIT_PARTICLE     0x1E /* emitter: orbiting particle, the render shows the whole water texture page (look pending) */
#define FX_LEVIATHAN_WATER_SPLASH_2     0x1F /* emitter: blue water-splash sprites and droplets */
#define FX_LEVIATHAN_MORPH_KEYS         0x20 /* table: key track, position and scale of the morph models (op81) */
#define FX_LEVIATHAN_SPRAY              0x23 /* emitter: blue water spray clouds */
#define FX_LEVIATHAN_FOAM               0x24 /* animquads: blue-white foam, drawn along the wave edge */
#define FX_LEVIATHAN_WATER_DROPS_2      0x25 /* emitter: water droplet sprites */
#define FX_LEVIATHAN_FOAM_2             0x26 /* animquads: blue-white foam cloud */
#define FX_LEVIATHAN_ORBIT_POS_KEYS_2   0x27 /* table: key track, position of the orbit particle emitter (op84) */
#define FX_LEVIATHAN_MODEL_SCALE_KEYS   0x2D /* table: key track, scale of an effect model (op83) */
#define FX_LEVIATHAN_ORBIT_POS_KEYS     0x2E /* table: key track, position of the orbit particle emitter (op81) */
#define FX_LEVIATHAN_WATER_STREAK       0x2F /* emitter: blue water streak (screen trail) */
#define FX_LEVIATHAN_RAINBOW_RING       0x30 /* emitter: blue / green / yellow / red gouraud ring */
#define FX_LEVIATHAN_WATER_STREAK_2     0x31 /* emitter: blue water streak (3D trail) and spray */
#define FX_LEVIATHAN_MODEL_SCALE_KEYS_2 0x3F /* table: key track, scale of an effect model (op84) */
#define FX_LEVIATHAN_SUMMON_MODEL_6     0x86 /* summon data entry 6: 'so' model (look pending) */
#define FX_LEVIATHAN_SUMMON_MODEL_7     0x87 /* summon data entry 7: 'so' model (look pending) */
#define FX_LEVIATHAN_SUMMON_MODEL_8     0x88 /* summon data entry 8: 'so' model (look pending) */
#define FX_LEVIATHAN_SUMMON_MODEL_9     0x89 /* summon data entry 9: 'so' model drawn as the sea (fx_0b3_seaModel) (look pending) */
#define FX_LEVIATHAN_SUMMON_MODEL_10    0x8A /* summon data entry 10: 'so' model (look pending) */
#define FX_LEVIATHAN_SUMMON_MODEL_11    0x8B /* summon data entry 11: 'so' model (look pending) */
#define FX_LEVIATHAN_SUMMON_MODEL_12    0x8C /* summon data entry 12: 'so' model (look pending) */
#define FX_LEVIATHAN_SUMMON_MODEL_13    0x8D /* summon data entry 13: 'so' model (look pending) */
#define FX_LEVIATHAN_SUMMON_MODEL_14    0x8E /* summon data entry 14: 'so' model (look pending) */
#define FX_LEVIATHAN_SUMMON_MODEL_15    0x8F /* summon data entry 15: 'so' model (look pending) */

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

static void fxm_drawWaveWall(SVECTOR *pos, SVECTOR *rot, Fx0b3Vec3 *scale, s32 bright, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9,
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


static s32 fxm_sinHalfNeg(s32 ang);
static s32 fxm_nextRingPoint(SVECTOR *out);
typedef struct {
    s16 pad0; /* always 0 in the effect file blocks (runtime, round 33) */
    s16 count2; /* count */
    s16 keys4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx0b3Track;
static void fxm_keyGetVec(Fx0b3Track *track, s32 t, SVECTOR *out);
static void fxm_keyGetVec2(Fx0b3Track *track, s32 t, SVECTOR *out0, SVECTOR *out1);
static void fxm_keyGetVec3(Fx0b3Track *track, s32 t, SVECTOR *out0, SVECTOR *out1, SVECTOR *out2);
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
static void fxm_drawWaterSheet(SVECTOR *a, SVECTOR *b, VECTOR *c, s32 bright, s32 brightShade, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10,
                   s32 a11, s32 a12, s32 a13, s32 a14);
s32 fx_0b3_op85(s32 mode, Fx0b3Work85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
