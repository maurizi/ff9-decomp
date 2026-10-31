#ifndef FX_0BA_H
#define FX_0BA_H

/* Effect module fx_0ba (dir 13 file 0xBA): Ramuh. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[1];
} Fx0baStrip;

typedef struct {
    s32 posX0; /* position, 20.12 */
    s32 posY4;
    s32 posZ8;
    s16 velXC; /* velocity */
    s16 velYE;
    s16 velZ10;
    s16 unk12;
    s16 trail14; /* trail index */
    s16 pointIdx16; /* point index */
    u8 *point18; /* strip point */
} Fx0baPart; /* 0x1C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[17];
} Fx0baTrail; /* 0x8C */

typedef struct {
    u8 pad0[8];
    SVECTOR rot8; /* rotation */
} Fx0baWork85; /* 0x10 */

extern Fx0baTrail fx_0ba_arcTrails[5];

typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    u8 strip10; /* path index */
    u8 kind11; /* state */
    u8 branchAt12;
    u8 branchAt13;
    u8 unk14;
    u8 amp15; /* amplitude */
    u8 pad16[2];
    s32 *jitter18; /* arc profile */
} Fx0baBolt;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[4];
} Fx0baStrip4; /* 0x24 */

extern s32 D_801EC550;
extern SVECTOR fx_0ba_boltSrc;
extern Fx0baTrail fx_0ba_boltTrails[];
extern Fx0baStrip4 fx_0ba_boltBranches[];
extern s32 fx_0ba_summonDone;
extern SVECTOR fx_0ba_summonPos;

typedef struct {
    SVECTOR points0[17];
    s32 unk88;
} Fx0baPath; /* 0x8C */

extern Unk800F78A4 *fx_0ba_env;
extern Fx0baPath fx_0ba_boltPoints[32];
extern s32 fx_0ba_arcTrailNext;

typedef struct {
    s32 *jitter0; /* arc profile table */
    s32 boltCount4; /* spawn counter */
    s32 kind8; /* state */
} Fx0baWork83;

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad[3];
} Fx0baPt; /* 0xC */

extern s32 fx_0ba_boltJitter[];
extern s32 fx_0ba_boltLen;
extern SVECTOR fx_0ba_orbPos;

static void fxm_drawCenterBand(u32 *ot, s32 tpage, s32 w, s32 r, s32 g, s32 b);
static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0baStrip *buf, SVECTOR *from, SVECTOR *to, s32 amp, s32 s, s32 step,
                   s32 a7, s32 a8, s32 pad);
static void fxm_drawJaggedTrail(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *buf, s32 size, s32 *tab, SVECTOR *from,
                   SVECTOR *to, s32 r0, s32 r1, s32 s, s32 step, s32 a12, s32 a13, s32 pad, s32 wave);
static void fxm_drawJaggedTrailBranch(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *path, Fx0baStrip *buf, s32 size, s32 *tab,
                   s32 i, s32 a12, s32 a13, s32 pad);
static s32 fxm_randStripPoint32(SVECTOR *v);
static s32 fx_0ba_op85PoolTask(s32 mode, Fx0baPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
static s32 fx_0ba_op83PoolTask(s32 mode, Fx0baBolt *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
static void fxm_arcTrailReset(void);
static void fxm_arcTrailSpawn(Unk80100C78Entry *entry, SVECTOR *center, s32 r, SVECTOR *from, SVECTOR *to);
s32 fx_0ba_op83(s32 mode, Fx0baWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *motion180;
    BtlUnit9CC *motion184;
    BtlUnit9CC *motion188;
} Fx0baUnit;
typedef struct {
    u8 pad0[0x54];
    Fx0baUnit *unk54;
    u8 pad58[0x60 - 0x58];
    s32 model60; /* summon model */
} Fx0baSummon;
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0baVec3;
typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    u8 pad8[8];
    SVECTOR pos10; /* orb position */
    SVECTOR pos18; /* bone position */
    u8 pad20[8];
    Fx0baVec3 pos28; /* model position */
    Fx0baVec3 scale34; /* model scale */
    SVECTOR rot40; /* model rotation */
    Fx0baVec3 summonPos48; /* summon position */
    Fx0baVec3 summonScale54; /* summon scale */
    SVECTOR rot60; /* summon rotation */
    SVECTOR rot68; /* model rotation */
    Fx0baVec3 scale70; /* model scale */
    s32 model7C;
    s32 model80;
    s32 model84;
    s32 model88;
    s32 model8C;
    s32 model90;
    s16 unk94;
    s16 unk96;
    s16 unk98;
    s16 unk9A;
    u8 pad9C[4];
} Fx0baWork80; /* 0xA0 */
extern BtlModel fx_0ba_effModels0[];
s32 fx_0ba_op80(s32 mode, Fx0baWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[1];
} Fx0baPTrail;

typedef struct {
    s16 x, y, z;
} Fx0baPKey;

typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    Fx0baPKey unk4[1];
} Fx0baPKeys;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[17];
} Fx0baPTrail17; /* 0x8C */

typedef struct {
    s16 x, y, z, rx, ry, rz;
} Fx0baPKey12; /* 0xC */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* base position */
    SVECTOR pos10;
    SVECTOR camAt18;
    u8 pad20[0x28];
    s32 pos48[3]; /* model position */
    s32 scale54[3]; /* model scale */
    SVECTOR rot60; /* model rotation */
    Pool6CEntry *emit68;
    Pool6CEntry *emit6C;
    s32 model70; /* models */
    s32 model74;
    s32 model78;
    s32 model7C;
    s32 model80;
} Fx0baPWork81; /* 0x84 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[13];
} Fx0baPTrail13; /* 0x6C */

#define FX0BAP_W(v, i) (((s32 *)&(v))[i])

extern s16 fx_0ba_bandWidths[];
extern s32 fx_0ba_ribbonFromBlocks[3];
extern s32 fx_0ba_ribbonToBlocks[3];
extern s32 fx_0ba_ribbon13FromBlocks[3];
extern s32 fx_0ba_ribbon13ToBlocks[3];
extern Fx0baPTrail fx_0ba_offsetTrail;
extern BtlModel fx_0ba_effModels1[];
extern Fx0baPTrail17 fx_0ba_ribbons[6];
extern Fx0baPTrail13 fx_0ba_ribbons13[3];

#endif
