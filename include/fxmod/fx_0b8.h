#ifndef FX_0B8_H
#define FX_0B8_H

/* Effect module fx_0b8 (dir 13 file 0xB8): Atomos, full summon. Screen bands (fx_0b8_drawScreenBand), a gradient
 * plane, jagged bolts (fx_0b8_bolts), and the targets lifted and spun (fx_0b8_units). Evidence: btlfx_start and
 * btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    u8 u;
    u8 v;
} Fx0b8UV;

extern SVECTOR fx_0b8_particleFrom[32];

void fx_0b8_drawScreenBand(s32 ot, s32 y, s32 h, s32 x, s32 c0, s32 c1, s32 tpage, s32 clut, s32 abr);
void fx_0b8_drawScreenGradient(s32 ot);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot);
static s32 fx_0b8_op80ParticleMotion(Pool30Entry *pt, s32 a1, SVECTOR *pos, s32 a3, s32 frame, s32 n);
extern Unk800F78A4 *fx_0b8_env;
extern SVECTOR fx_0b8_planeColors[]; /* colour table */
void fx_0b8_drawGradientPlane(SVECTOR *pos, SVECTOR *rot, s32 n, s32 tpage, s32 clut, s32 abr);
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

typedef struct {
    u8 flags0[0x28]; /* per-step flags and countdowns, by index (cleared at init) */
    s32 model28; /* model */
    SVECTOR rot2C; /* rotation */
    VECTOR scale34; /* scale */
    VECTOR pos44; /* position */
    s32 model54; /* models */
    s32 model58;
    SVECTOR rot5C;
    SVECTOR rot64;
    VECTOR pos6C;
    VECTOR pos7C;
    VECTOR scale8C;
    VECTOR pos9C;
    VECTOR scaleAC;
    SVECTOR rotBC;
    SVECTOR posC4;
    s32 modelCC; /* models */
    s32 modelD0;
    s32 modelD4;
    VECTOR scaleD8;
    s32 modelE8; /* models */
    s32 modelEC;
    s32 modelF0;
    s32 modelF4;
    s32 modelF8;
    VECTOR posFC;
    VECTOR scale10C;
    SVECTOR rot11C;
    s32 scale124;
    s32 model128; /* models */
    s32 model12C;
    s32 model130;
    s32 model134;
    s32 model138;
    VECTOR pos13C;
    VECTOR scale14C;
    SVECTOR rot15C;
    SVECTOR pos164;
    s32 model16C; /* models */
    s32 model170;
    SVECTOR rot174;
    s32 model17C;
    s32 model180;
    Pool6CEntry *emit184;
    SVECTOR pos188;
    Pool6CEntry *emit190;
    SVECTOR pos194;
    Pool6CEntry *emit19C;
    SVECTOR pos1A0;
    SVECTOR pos1A8;
    Pool6CEntry *emit1B0;
    s32 unk1B4; /* unused */
    Pool6CEntry *emit1B8;
    SVECTOR pos1BC;
    SVECTOR unk1C4; /* write-only (0, -0x2500, -0xE00) */
    SVECTOR pos1CC;
    SVECTOR pos1D4;
    SVECTOR pos1DC;
    s32 scroll1E4;
    s32 scrollSpeed1E8;
} Fx0b8Work80; /* 0x1EC */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[9]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
    s32 x4C[9]; /* x */
    s32 y70[9]; /* y */
    s32 dx94[9]; /* x speed */
    s32 dyB8[9]; /* y speed */
    s32 timerDC; /* timer */
} Fx0b8Bolt; /* 0xE0 */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0b8Vec3;

typedef struct {
    SVECTOR rot0; /* rotation */
    Fx0b8Vec3 scale8; /* scale */
    SVECTOR pos14; /* home position */
    s16 pitch1C;
    s16 yaw1E;
    s16 unk20; /* write-only (cleared at init) */
    s16 pad22;
    SVECTOR rot24; /* rotation */
    s16 spinX2C; /* rotation speed */
    s16 spinY2E;
    s16 spinZ30;
    s16 pad32;
} Fx0b8Unit; /* 0x34 */

typedef struct {
    s32 y0; /* y */
    s32 h4; /* h */
    s32 x8; /* x */
    s32 colorC; /* colours */
    s32 color10;
    s32 dx14; /* x speed */
    s32 dy18; /* y speed */
    s32 unk1C; /* unused */
} Fx0b8Band; /* 0x20 */

typedef struct {
    u8 pad0[0x60];
    s32 model60; /* summon model */
} Fx0b8Summon;

#define SUMMON(env) ((Fx0b8Summon *)(env)->loader)

extern Fx0b8Band fx_0b8_bands[4];
extern BtlModelBuf fx_0b8_effModels[];
extern s32 fx_0b8_unusedScroll;
extern Fx0b8Bolt fx_0b8_bolts[8];
extern Fx0b8Unit fx_0b8_units[];
extern s32 fx_0b8_trails[];
extern s32 fx_0b8_gridShift;

#endif
