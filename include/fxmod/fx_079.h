#ifndef FX_079_H
#define FX_079_H

/* Effect module fx_079 (dir 13 file 0x79): Demi Shock. Three models over the target, a lift of the target and 5
 * jagged btlfx_drawTrail strips from a bone of unit 0x10. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

/* fx_079 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEMI_SHOCK_WISP_BURST 0x1 /* emitter: white gouraud rings + pink-violet smoke wisps */
#define FX_DEMI_SHOCK_FLASH_RING 0x2 /* emitter: white gouraud ring */
#define FX_DEMI_SHOCK_BOWL_2     0x6 /* model: second gouraud hemisphere bowl */
#define FX_DEMI_SHOCK_TRAIL      0x9 /* trail: strip texture still outside the file and the common pages (magenta) (look pending) */
#define FX_DEMI_SHOCK_BOWL       0xA /* model: gouraud hemisphere bowl */
#define FX_DEMI_SHOCK_SPHERE     0xB /* model: sphere, texture still outside the file and the common pages (magenta) (look pending) */
#define FX_DEMI_SHOCK_BACKDROP   0xC /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 halfWidth6; /* half-width of the ribbon at this point (scales the edge normal) */
} Fx079Pt;

/* line strip passed to btlfx_drawTrail */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    Fx079Pt pts4[11];
    s32 endX5C; /* end offset */
    s32 endY60;
    s32 endZ64;
    s32 pad68;
    s32 offsetY6C[11]; /* y offset */
    s32 unk98[11]; /* unused */
    s32 speedYC4[11]; /* y speed */
    s32 unkF0[11]; /* unused */
    s32 timer11C; /* timer */
} Fx079Strip; /* 0x120 */

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR scaleC; /* scale */
} Fx079Model; /* 0x1C */

typedef struct {
    s32 timers0[16]; /* timers */
    Fx079Model models40[3];
    VECTOR pos94; /* position */
} Fx079Work; /* 0xA4 */

extern Unk800F78A4 *fx_079_env;
extern BtlModelBuf fx_079_effModels[];
extern SVECTOR fx_079_bonePos;
extern SVECTOR fx_079_bodyPos;
extern SVECTOR fx_079_homePos;
extern VECTOR fx_079_unitScale;
extern SVECTOR fx_079_curBodyPos;
extern s32 fx_079_unitHeight;
extern Pool6CEntry *fx_079_emitter;
extern SVECTOR fx_079_emitStartPos;
extern SVECTOR fx_079_emitPos;
extern Fx079Strip fx_079_strips[5];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
