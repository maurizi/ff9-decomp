#ifndef FX_129_H
#define FX_129_H

/* Effect module fx_129 (dir 13 file 0x129): Pumpkin Head; enemy Pumpkin Head (Abomination/Hedgehog Pie, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_129 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PUMPKIN_HEAD_SMOKE_BURST_2 0x1 /* emitter: pale smoke burst with a dark red glow, later bounces */
#define FX_PUMPKIN_HEAD_SMOKE_BURST   0x3 /* emitter: pale smoke burst with a dark red glow, first bounce */
#define FX_PUMPKIN_HEAD_PUMPKIN       0x7 /* model: round pumpkin head with a bow tie, drawn with the caster's texture list */

/* fx_129 choreography (docs/fxdata.md, "Choreography literals"): the pumpkin's texture list, drop and bounces
 * (fall / rise phases alternate in work->phase34), the target's shake and the op's length. */
#define FX_PUMPKIN_HEAD_PUMPKIN_TPAGE  0xF7   /* PUMPKIN's BtlTexInfo list (Hi_RegisterTexListModel): 8-bit page (0x1C0, 0x100), */
#define FX_PUMPKIN_HEAD_PUMPKIN_CLUT   0x3CC0 /* entry 0: CLUT (0, 0xF3), texel offset (0, V) */
#define FX_PUMPKIN_HEAD_PUMPKIN_V      0x80
#define FX_PUMPKIN_HEAD_PUMPKIN_2_CLUT 0x3C80 /* entry 1: same page, CLUT (0, 0xF2), no offset */
#define FX_PUMPKIN_HEAD_SCALE         0x330   /* PUMPKIN's scale; the landing squash takes the bounce speed off its height */
#define FX_PUMPKIN_HEAD_DROP_HEIGHT   (-0x1B00) /* the pumpkin starts this far above the target */
#define FX_PUMPKIN_HEAD_GRAVITY       0x60    /* height += GRAVITY * t * t / 2 in the fall and rise phases */
#define FX_PUMPKIN_HEAD_BOUNCE_PHASES 9       /* work->phase34: fall, rise, ... (4 bounces, then it rests and fades) */
#define FX_PUMPKIN_HEAD_SQUASH_FRAMES 4       /* work->timer54: the squash after a landing */
#define FX_PUMPKIN_HEAD_SHAKE_FRAME   0xB     /* the target starts to shake (vertical scale wobble) */
#define FX_PUMPKIN_HEAD_SHAKE_FRAMES  0x10    /* work->timer60: for this many frames */
#define FX_PUMPKIN_HEAD_FADE_FRAMES   0x10    /* after the bounces the pumpkin fades out (additive, darker) over this many frames */
#define FX_PUMPKIN_HEAD_FRAMES        0x40    /* the op ends at this frame */

typedef struct {
    s32 vx, vy, vz;
} Fx129Vec3;

typedef struct {
    s32 model0; /* model */
    VECTOR pos4; /* model position */
    SVECTOR rot14; /* model rotation */
    VECTOR scale1C; /* model scale */
    SVECTOR pos2C; /* target body position */
    s32 phase34; /* bounce phase */
    s32 speed38; /* bounce height */
    s32 height3C;
    s32 height40; /* height offset */
    s32 frame44; /* phase time */
    s32 unk48; /* unused */
    s32 fade4C; /* fade time */
    s32 color50; /* brightness */
    s32 timer54; /* squash countdown */
    s32 count58; /* landings */
    s32 unk5C; /* write-only (cleared at init) */
    s32 timer60; /* shake countdown */
    Fx129Vec3 scale64; /* target scale */
    Fx129Vec3 scale70;
} Fx129Work;

extern Unk800F78A4 *fx_129_env;
extern BtlModelBuf fx_129_effModels[];

#endif
