#ifndef FX_134_H
#define FX_134_H

/* Effect module fx_134 (dir 13 file 0x134). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[25];
} Fx134Bolt;

extern Unk800F78A4 *D_801EC8E4;

static void fxm_drawMirrorSprite(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr);
static void fxm_drawSprite3D(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr);
void func_801E8460(Unk80100C78Entry *entry, Fx134Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
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
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* position */
    SVECTOR unk10; /* target */
    SVECTOR unk18; /* camera target */
    u8 pad20[4];
    Fx134Vec3 unk24; /* model position */
    Fx134Vec3 unk30; /* model scale */
    SVECTOR unk3C; /* model rotation */
    Fx134Vec3 unk44; /* scale */
    SVECTOR unk50; /* rotation */
    s32 unk58; /* scroll */
    s32 unk5C; /* models */
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    u8 pad78[4];
    Pool6CEntry *unk7C;
    Pool6CEntry *unk80;
} Fx134Work; /* 0x84 */

typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* phase */
    SVECTOR unk4; /* velocity; pad = size index */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C;
    s16 unk1E;
    Pool6CEntry *unk20;
} Fx134Orb; /* 0x24 */

extern s32 D_801EC868[]; /* orb sizes */
extern s32 D_801EF97C; /* catch radius */
extern s32 D_801EF980;

static s32 fx_134_op80PoolTask(s32 mode, Fx134Orb *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx134Work *parent);

extern SVECTOR D_801EC860; /* caster position */
extern CVECTOR D_801EC878[];
extern s16 D_801EC880[4];
extern CVECTOR D_801EC888[];
extern CVECTOR D_801EC894[2];
extern Fx134Bolt D_801EC8F4[];
extern BtlModel D_801ECB64[];
extern u8 D_801ED1A4[];

#endif
