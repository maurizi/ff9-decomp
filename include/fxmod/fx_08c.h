#ifndef FX_08C_H
#define FX_08C_H

/* Effect module fx_08c (dir 13 file 0x8C): Berserk, all targets (party; enemy Berserk of Armstrong). One texture
 * model drawn twice, and a layer texture. Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_08c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BERSERK_FLAME_TEXTURE 0x1 /* layer table: render-to-texture (btlfx_drawLayerTexture): 5 layers into VRAM (0x200,0x180) 64x128, rising flames (byte-identical to fx_00e entry 0x3, played there); runtime (docs/runtime-findings.md effect-data pass) */
#define FX_BERSERK_FUNNEL     0x3 /* model: open funnel, white texture */
#define FX_BERSERK_FIRE_RING  0x8 /* emitter: red-orange-yellow gouraud rings */
#define FX_BERSERK_STAR_FLASH 0xB /* emitter: white and red star flashes + violet ring */

/* fx_08c choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_BERSERK_STATE_FUNNEL 0    /* op80's only state: the funnel grows twice (full and half height) */
#define FX_BERSERK_FRAMES       0x20 /* FX_BERSERK_STATE_FUNNEL's length (the op ends) */
#define FX_BERSERK_MODEL_COUNT  2    /* btlfx_initEffModels slots (only FX_BERSERK_FUNNEL is registered) */
/* FX_BERSERK_FUNNEL samples the rect FX_BERSERK_FLAME_TEXTURE renders each frame: (0x200, 0x100) + v 0x80 */
#define FX_BERSERK_FUNNEL_TPAGE 0x138  /* 15-bit page (0x200, 0x100), abr 1 */
#define FX_BERSERK_FUNNEL_CLUT  0xFFFF /* none (15-bit page) */
#define FX_BERSERK_FUNNEL_V     0x80

typedef struct {
    s32 state0; /* state */
    s32 bgFrame4; /* frame for btlfx_drawLayerTexture */
    SVECTOR pos8; /* position: env->targets->centerX..Z */
    SVECTOR unk10; /* write-only: pos8 raised by 0x100 */
    s32 modelPos18[3]; /* model position (VECTOR without its pad word) */
    s32 model24; /* model */
} Fx08cWork;

extern Unk800F78A4 *fx_08c_env;
extern BtlModelBuf fx_08c_effModels[];

#endif
