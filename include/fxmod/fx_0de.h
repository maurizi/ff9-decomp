#ifndef FX_0DE_H
#define FX_0DE_H

/* Effect module fx_0de (dir 13 file 0xDE): Sword Magic Water. A morphing model at the caster, and the targets
 * thrown up and dropped (fx_0de_op81). Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0de effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_WATER_DROPLETS      0x1 /* emitter: water blobs, blue glow and glints */
#define FX_SWDMAG_WATER_WATER_BALL_1  0x2 /* model: water funnel rising, morph stage 1 */
#define FX_SWDMAG_WATER_WATER_BALL_2  0x3 /* model: water vase shape, morph stage 2 */
#define FX_SWDMAG_WATER_WATER_BALL_3  0x4 /* model: water bulb closing, morph stage 3 */
#define FX_SWDMAG_WATER_WATER_BALL_0  0x5 /* model: flat water pool (water texture), morph stage 0 */
#define FX_SWDMAG_WATER_SPRAY_FUNNEL  0x6 /* model: swirling funnel, mist texture, morph end of the spray */
#define FX_SWDMAG_WATER_SPRAY_COLUMN  0x7 /* model: open cylinder, mist texture, morph start of the spray */
#define FX_SWDMAG_WATER_WATER_SURFACE 0x8 /* model: flat disc, water texture */
#define FX_SWDMAG_WATER_BLUE_RING     0x9 /* emitter: white-blue gouraud rings */
#define FX_SWDMAG_WATER_WATER_BALL    0xD /* model: closed water sphere, morph end */
#define FX_SWDMAG_WATER_SWORD_GLINT   0xE /* rings: white 6-segment rings flashed at the sword (op84, first draw) */
#define FX_SWDMAG_WATER_SWORD_FLASH   0xF /* rings: white 8-segment rings flashed rotating at the sword (op84) */

typedef struct {
    SVECTOR homePos0[8]; /* unit home positions */
    s32 unitCount40; /* unit count */
    s32 jumpHeight44; /* jump height */
    s32 height48[8]; /* unit heights */
} Fx0deWork81; /* 0x68 */

extern Unk800F78A4 *fx_0de_env;
extern s32 fx_0de_jumpScale; /* jump scale */

s32 fx_0de_op81(s32 mode, Fx0deWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0deWork84;
s32 fx_0de_op84(s32 mode, Fx0deWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

typedef struct {
    s32 hold0; /* dead early-out flag, cleared at init and never set (see Fx046Work.hold0) */
    SVECTOR pos4; /* caster body position */
    SVECTOR posC; /* caster position */
    s32 model14[8]; /* models */
    s32 scale34; /* scale from the caster's size */
} Fx0deWork80; /* 0x38 */
typedef struct {
    u16 srcX0;
    u16 srcY2;
} Fx0deFlash;
extern Fx0deFlash fx_0de_scrollSrc[];
extern BtlModelBuf fx_0de_effModels[];

#endif
