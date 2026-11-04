#ifndef FX_138_H
#define FX_138_H

/* Effect module fx_138 (dir 13 file 0x138): Lucky Seven. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_138 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LUCKY_SEVEN_FIRE_BURST    0x1 /* emitter: white-blue gouraud rings, fire bursts and white / pink glows */
#define FX_LUCKY_SEVEN_SPRITE_LINE   0xE /* sprite line: small coloured sprites, drawn frames 0-0xC; the render is almost empty (look pending) */
#define FX_LUCKY_SEVEN_SLOT_STRIPS 0x1B /* model: 15 polys (three slanted strips), tpage 0x9A not in the file (magenta in the render), drawn with the three reels (look pending) */
#define FX_LUCKY_SEVEN_RAINBOW_TRAIL 0x2A /* emitter: orbiting rainbow trail with glow sprites and rainbow rings */

typedef struct {
    s16 x, y, z;
} Fx138Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* key count */
    Fx138Point *keys4; /* keys */
} Fx138Path;

typedef struct {
    Pool6CEntry *emit0[6];
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR pos20; /* caster body position */
    SVECTOR pos28;
    u16 frame30; /* frame, up to 5 */
    u16 fade32; /* scale */
} Fx138Work81; /* 0x34 */

typedef struct {
    u8 pad0[0x24];
    VECTOR pos24; /* model position */
    VECTOR unk34; /* write-only: model scale */
    u8 pad44[0x48 - 0x44];
    SVECTOR rot48; /* model rotation */
    VECTOR pos50; /* model position */
    VECTOR scale60; /* model scale */
    SVECTOR pos70; /* caster position */
    SVECTOR rot78; /* caster rotation */
    u8 pad80[0x88 - 0x80];
    s16 unk88; /* unk88..unk8E: only initialised, never read (runtime watch, round 33); unk8A is the fourth value of the fx_138_randSets row */
    s16 unk8A; /* write-only */
    s16 unk8C; /* write-only (-1) */
    s16 unk8E; /* write-only (-1, then 0x1A) */
    s16 active90; /* model shown */
    s16 timer92; /* model countdown */
    s16 frame94; /* frame */
    u16 model96[3]; /* ring models */
    u16 model9C; /* model */
} Fx138Work80; /* 0xA0 */

typedef struct {
    SVECTOR rot0; /* rotation */
    s16 unk8; /* write-only (0x200) */
    s16 unkA; /* unused */
    s16 unkC; /* unused */
    u16 unkE; /* write-only */
    u8 pad10[0x18 - 0x10];
} Fx138Ring; /* 0x18 */

extern Fx138Ring fx_138_reels[3]; /* set up from a fx_138_randSets row, never read (runtime watch, round 33): slot reels that were never drawn? */
extern u16 fx_138_randSets[6][4];
extern u16 fx_138_modelFileIds[3];
extern u16 fx_138_pathFileIds[6];
extern BtlModelBuf fx_138_effModels[];
extern u16 fx_138_emitFileIds[6]; /* trail files */
extern s32 fx_138_op81SpawnPoints[6];
extern Unk800F78A4 *fx_138_env;
extern u8 fx_138_op81Trails[];
extern SVECTOR fx_138_emitPos[6];
extern Fx138Path fx_138_posPaths[6];
extern SVECTOR fx_138_pathOrigin; /* path origin */
extern SVECTOR fx_138_pathRot; /* path rotation */

s32 fx_138_op81SpawnPoint0(SVECTOR *out, s32 dir, s32 i);
s32 fx_138_op81SpawnPoint1(SVECTOR *out, s32 dir, s32 i);
s32 fx_138_op81SpawnPoint2(SVECTOR *out, s32 dir, s32 i);
s32 fx_138_op81SpawnPoint3(SVECTOR *out, s32 dir, s32 i);
s32 fx_138_op81SpawnPoint4(SVECTOR *out, s32 dir, s32 i);
s32 fx_138_op81SpawnPoint5(SVECTOR *out, s32 dir, s32 i);
void fx_138_getPathPoint(SVECTOR *out, Fx138Path *path, s32 key);
static s32 fxm_posPathGet(Fx138Path *path, s32 i, SVECTOR *out);
s32 fx_138_op80(s32 mode, Fx138Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
s32 fx_138_op81(s32 mode, Fx138Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

static void fxm_posPathInit(Fx138Path *path, u16 *data);
static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

#endif
