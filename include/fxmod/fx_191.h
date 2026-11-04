#ifndef FX_191_H
#define FX_191_H

/* Effect module fx_191 (dir 13 file 0x191): Lancer. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_191 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LANCER_GOLD_TRAIL   0x2 /* emitter: screen trail, orange-gold streak strip (byte-identical to entry 0x9) */
#define FX_LANCER_PATH         0x4 /* table: position path (fxm_posPathInit) */
#define FX_LANCER_PATH_2       0x5 /* table: position path (fxm_posPathInit) */
#define FX_LANCER_GOLD_TRAIL_2 0x9 /* emitter: screen trail, orange-gold streak strip (byte-identical to entry 0x2) */
#define FX_LANCER_RED_SPARKS   0xF /* emitter: white / pink gouraud rings + red spark dots */
#define FX_LANCER_ORANGE_RINGS 0x10 /* emitter: orange gouraud rings, 18 frames */
#define FX_LANCER_GREEN_RINGS  0x11 /* emitter: mint-green gouraud rings, 18 frames */
#define FX_LANCER_CYAN_SPARKS  0x12 /* emitter: white / pale-green gouraud rings + cyan spark dots */

typedef struct {
    s16 x, y, z;
} Fx191Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx191Point *keys4;
} Fx191Path;

typedef struct {
    Pool6CEntry *emit0[4];
    SVECTOR pos10; /* caster position */
    SVECTOR pos18; /* target position */
    SVECTOR rot20; /* angles */
} Fx191Work80; /* 0x28 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* target body position */
    u8 pad20[0x30 - 0x20];
    SVECTOR pos30; /* from, to, from, to */
    SVECTOR pos38;
    SVECTOR pos40;
    SVECTOR pos48;
    SVECTOR target50; /* current */
    SVECTOR target58;
} Fx191Work81; /* 0x60 */

extern u16 fx_191_emitFileIds[4]; /* files */
extern SVECTOR fx_191_op80Offsets[4];
extern s32 (*fx_191_op80ParticleMotions[4])();
extern Unk800F78A4 *fx_191_env;
extern Fx191Path fx_191_posPath0;
extern Fx191Path fx_191_posPath1;
extern SVECTOR fx_191_op80Origin;
extern SVECTOR fx_191_op80Targets[4];
extern SVECTOR fx_191_op81PathOrigin;
extern SVECTOR fx_191_emitPos0;
extern SVECTOR fx_191_emitPos1;
extern s32 fx_191_op80Trails[4][0x60];
extern s32 fx_191_op81Trails0[];
extern s32 fx_191_op81Trails1[];
extern s32 fx_191_op80Frame; /* op80 frame */
extern s32 fx_191_op81Frame; /* op81 frame */

s32 fx_191_op80ParticleMotion0(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_191_op80ParticleMotion1(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_191_op80ParticleMotion2(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
s32 fx_191_op80ParticleMotion3(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fxm_lerpParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *base, SVECTOR *to, s32 frame, s32 frames);
static s32 fx_191_op81ParticleMotion0(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fx_191_op81ParticleMotion1(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fxm_followPathParticle0(Pool30Entry *pt, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames);
static s32 fxm_followPathParticle1(Pool30Entry *pt, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames);
static void fxm_posPathInit(Fx191Path *path, u16 *data);
static s32 fxm_posPathGet(Fx191Path *path, s32 t, SVECTOR *out);

#endif
