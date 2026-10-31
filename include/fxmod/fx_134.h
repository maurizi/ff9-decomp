#ifndef FX_134_H
#define FX_134_H

/* Effect module fx_134 (dir 13 file 0x134): Doomsday; enemy Doomsday (Hades, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 unk3;
    SVECTOR pos4[25];
} Fx134Bolt;

extern Unk800F78A4 *fx_134_env;

static void fxm_drawMirrorSprite(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr);
static void fxm_drawSprite3D(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr);
static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx134Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 pad);

/* propagated */
static void fxm_drawTiledStrip(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr, s32 flip);
static void fxm_drawBulgeGrid24(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx134Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* position */
    SVECTOR pos10; /* target */
    SVECTOR pos18; /* camera target */
    u8 pad20[4];
    Fx134Vec3 pos24; /* model position */
    Fx134Vec3 scale30; /* model scale */
    SVECTOR rot3C; /* model rotation */
    Fx134Vec3 scale44; /* scale */
    SVECTOR rot50; /* rotation */
    s32 scroll58; /* scroll */
    s32 model5C; /* models */
    s32 model60;
    s32 model64;
    s32 model68;
    s32 model6C;
    s32 model70;
    s32 model74;
    u8 pad78[4];
    Pool6CEntry *emit7C;
    Pool6CEntry *emit80;
} Fx134Work; /* 0x84 */

typedef struct {
    s16 kind0; /* state */
    s16 phase2; /* phase */
    SVECTOR vel4; /* velocity; pad = size index */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s16 spin1C;
    s16 spin1E;
    Pool6CEntry *emit20;
} Fx134Orb; /* 0x24 */

extern s32 fx_134_orbScales[]; /* orb sizes */
extern s32 fx_134_catchRadius; /* catch radius */
extern s32 fx_134_stopOrbs;

static s32 fx_134_op80PoolTask(s32 mode, Fx134Orb *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx134Work *parent);

extern SVECTOR fx_134_origin; /* caster position */
extern CVECTOR fx_134_rippleColors0[];
extern s16 fx_134_orbSpeeds[4];
extern CVECTOR fx_134_rippleColors1[];
extern CVECTOR fx_134_rippleColors2[2];
extern Fx134Bolt fx_134_bolts[];
extern BtlModel fx_134_effModels[];
extern u8 fx_134_op80Trails[];

#endif
