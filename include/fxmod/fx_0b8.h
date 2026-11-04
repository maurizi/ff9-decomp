#ifndef FX_0B8_H
#define FX_0B8_H

/* Effect module fx_0b8 (dir 13 file 0xB8): Atomos, full summon. Screen bands (fx_0b8_drawScreenBand), a gradient
 * plane, jagged bolts (fx_0b8_bolts), and the targets lifted and spun (fx_0b8_units). Evidence: btlfx_start and
 * btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0b8 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ATOMOS_SPRITES         0x2 /* emitter: two sprite elements; the render is still black (texels blank in the file's upload, filled at run time?) (look pending) */
#define FX_ATOMOS_VORTEX_STREAK   0x3 /* trail: violet vortex streak */
#define FX_ATOMOS_ORANGE_STREAKS  0x7 /* emitter: two fans, thin orange light streaks */
#define FX_ATOMOS_SPRITES_2       0x8 /* emitter: two spiralling sprite elements; the render is still black (texels blank in the file's upload, filled at run time?) (look pending) */
#define FX_ATOMOS_VIOLET_RINGS    0x9 /* emitter: violet streak and violet / dark blue gouraud rings */
#define FX_ATOMOS_PINK_CLOUD      0xA /* emitter: violet-blue and white-red rings, pink cloud particles */
#define FX_ATOMOS_FAINT_GLOW      0xB /* emitter: spiralling glow particles; the render is almost black (a faint white glow) (look pending) */
#define FX_ATOMOS_ROCK_DEBRIS     0xC /* emitter: grey-blue rock debris particles */
#define FX_ATOMOS_BLAST_RINGS     0xD /* emitter: white / yellow / orange / brown and purple gouraud rings */
#define FX_ATOMOS_VORTEX_SPARKLES 0xE /* emitter: orbiting violet vortex trails and gold sparkles */
#define FX_ATOMOS_UNIT_RING       0xF /* emitter: white-grey gouraud rings at each unit */
#define FX_ATOMOS_SPHERE          0x10 /* model: sphere, texture 0x17C (not in the file) */
#define FX_ATOMOS_RING_FUNNEL     0x11 /* model: gouraud ring funnel (flat torus) */
#define FX_ATOMOS_CLOUD_FUNNEL    0x12 /* model: ring funnel with a grey cloud texture */
#define FX_ATOMOS_ORANGE_RING     0x13 /* model: ring with an orange cloud texture */
#define FX_ATOMOS_BACKDROP        0x1F /* table: background layers for btlfx_drawBackdrop */
#define FX_ATOMOS_VIOLET_RING     0x20 /* emitter: white-violet and white-blue gouraud rings */
#define FX_ATOMOS_PURPLE_RINGS    0x21 /* emitter: white / magenta / purple and pale yellow / grey gouraud rings */
#define FX_ATOMOS_SPRITES_3       0x22 /* emitter: two sprite elements; the render is still black (texels blank in the file's upload, filled at run time?) (look pending) */
#define FX_ATOMOS_GREY_RING       0x80 /* summon data entry 0: 'so' model, thin white-grey gouraud ring */
#define FX_ATOMOS_CRATER_MOUND    0x81 /* summon data entry 1: 'so' model, grey gouraud crater mound */
#define FX_ATOMOS_WHITE_PILLAR    0x82 /* summon data entry 2: 'so' model, white octagonal gouraud pillar */
#define FX_ATOMOS_GROUND_CHUNK    0x83 /* summon data entry 3: 'so' model, chunk of rocky ground (earth cylinder with grass top) */
#define FX_ATOMOS_PINE_TREES      0x84 /* summon data entry 4: 'so' model, row of pine-tree cards */
#define FX_ATOMOS_PURPLE_DOME     0x85 /* summon data entry 5: textured model, purple dome with a cyan star pattern */
#define FX_ATOMOS_GOLD_RING       0x86 /* summon data entry 6: 'so' model, gold-brown gouraud ring */
#define FX_ATOMOS_PINE_TREES_2    0x87 /* summon data entry 7: 'so' model, row of pine-tree cards */
#define FX_ATOMOS_LAVA_CRATER     0x88 /* summon data entry 8: 'so' model, crater mound with a fire / lava texture */
#define FX_ATOMOS_CLOUD_DISC      0x89 /* summon data entry 9: textured model, disc with a dark cloud texture */
#define FX_ATOMOS_RUBBLE_BAND     0x8A /* summon data entry 10: 'so' model, ring band with a grey rubble / smoke texture */
#define FX_ATOMOS_WHITE_DROP      0x8B /* summon data entry 11: textured model, thin white teardrop (needle) */
#define FX_ATOMOS_WHITE_DROP_2    0x8C /* summon data entry 12: textured model, white teardrop */
#define FX_ATOMOS_WHITE_DROP_3    0x8D /* summon data entry 13: textured model, slim white teardrop */
#define FX_ATOMOS_WHITE_DROP_4    0x8E /* summon data entry 14: textured model, wide white teardrop */

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
extern s32 fx_0b8_scrollInitWord;
extern Fx0b8Bolt fx_0b8_bolts[8];
extern Fx0b8Unit fx_0b8_units[];
extern s32 fx_0b8_trails[];
extern s32 fx_0b8_gridShift;

#endif
