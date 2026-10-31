#ifndef FX_0B3_H
#define FX_0B3_H

/* Effect module fx_0b3 (dir 13 file 0xB3). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0[8]; /* enemy positions */
    s32 unk40; /* unit count */
} Fx0b3Work86;

extern s32 D_801EB5D4;
extern SVECTOR D_801EE340[16];

typedef struct {
    u16 unk0; /* file id, track 0 */
    u16 unk2; /* file id, track 1 */
    u16 unk4; /* end frame */
    u16 unk6;
} Fx0b3Track87;

extern Fx0b3Track87 D_801EB5E8[];

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0b3Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[8];
    SVECTOR pos10; /* morph model position */
    u8 pad18[0x28 - 0x18];
    Fx0b3Vec3 unk28; /* morph model position */
    Fx0b3Vec3 unk34; /* morph model scale */
    SVECTOR rot40; /* morph model rotation */
    Fx0b3Vec3 unk48; /* model position */
    Fx0b3Vec3 unk54; /* model scale */
    SVECTOR rot60; /* model rotation */
    Pool6CEntry *emit68;
    s32 model6C; /* model */
    s32 unk70; /* morph frame */
    s32 unk74; /* morph weight */
} Fx0b3Work80; /* 0x78 */

extern BtlModel fx_0b3_effModels[];

typedef struct {
    u8 pad0[0x184];
    BtlUnit9CC *motion184;
    BtlUnit9CC *motion188;
    BtlUnit9CC *motion18C;
    BtlUnit9CC *motion190;
} Fx0b3Unit;

typedef struct {
    u8 pad0[0x54];
    Fx0b3Unit *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx0b3Summon;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[8];
    SVECTOR pos10; /* morph model position */
    u8 pad18[0x28 - 0x18];
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
    Fx0b3Vec3 unk40; /* position */
    Fx0b3Vec3 unk4C; /* scale */
    SVECTOR rot58; /* rotation */
    Fx0b3Vec3 unk60;
    Fx0b3Vec3 unk6C;
    SVECTOR rot78;
    Pool6CEntry *emit80;
    Pool6CEntry *emit84;
    s32 model88[4]; /* models */
    s32 unk98; /* morph frame */
    s32 unk9C; /* morph weight */
} Fx0b3Work81; /* 0xA0 */

extern s32 D_801EE270; /* model */
extern s32 D_801EB5CC[2]; /* file ids */
extern u8 D_801EB660[];
extern u8 D_801ED0A0[];
extern SVECTOR D_801EE2A0[20];

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[0x28 - 0x8];
    SVECTOR pos28;
    SVECTOR pos30;
    u8 pad38[0x48 - 0x38];
    Fx0b3Vec3 unk48; /* position */
    Fx0b3Vec3 unk54; /* scale */
    SVECTOR rot60; /* rotation */
    Fx0b3Vec3 unk68;
    Fx0b3Vec3 unk74;
    SVECTOR rot80;
    s32 model88; /* model */
    s32 model8C; /* model */
} Fx0b3Work84; /* 0x90 */

void func_801E8DF4(SVECTOR *pos, SVECTOR *rot, Fx0b3Vec3 *scale, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9,
                   s32 a10, s32 a11, s32 a12, s32 a13, s32 a14);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[0x18 - 0x8];
    SVECTOR pos18;
    SVECTOR pos20;
    u8 pad28[0x38 - 0x28];
    Fx0b3Vec3 unk38; /* position */
    Fx0b3Vec3 unk44; /* scale */
    SVECTOR rot50; /* rotation */
    Fx0b3Vec3 unk58;
    Fx0b3Vec3 unk64;
    SVECTOR rot70;
    s32 model78[4]; /* models */
} Fx0b3Work83; /* 0x88 */
extern s32 D_801EB5AC[4]; /* start frames */
extern s32 D_801EB5BC[4]; /* file ids */

extern u16 D_801EB5A0[6]; /* morph model file ids */
extern s32 D_801EE280[6]; /* morph models */


s32 func_801E9DB8(s32 a);
static s32 fxm_nextRingPoint(SVECTOR *v);
typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    s16 unk4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx0b3Track;
static void fxm_keyGetVec(Fx0b3Track *track, s32 t, SVECTOR *out);
static void fxm_keyGetVec2(Fx0b3Track *track, s32 t, SVECTOR *a, SVECTOR *b);
static void fxm_keyGetVec3(Fx0b3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c);
static s32 fxm_keyGetValue(Fx0b3Track *track, s32 t);
extern Unk800F78A4 *fx_0b3_env;
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[8];
    SVECTOR pos10; /* midpoint */
    SVECTOR unk18;
    u8 pad20[8];
    Fx0b3Vec3 unk28; /* summon position */
    Fx0b3Vec3 unk34; /* summon scale */
    SVECTOR rot40; /* summon rotation */
    u8 pad48[0x68 - 0x48];
    Pool6CEntry *emit68;
} Fx0b3Work85; /* 0x6C */

extern CVECTOR D_801EB5D8;
void func_801E9DFC(SVECTOR *a, SVECTOR *b, VECTOR *c, s32 r, s32 k, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10,
                   s32 a11, s32 a12, s32 a13, s32 a14);
s32 fx_0b3_op85(s32 mode, Fx0b3Work85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
