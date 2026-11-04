#ifndef FX_0C9_H
#define FX_0C9_H

/* Effect module fx_0c9 (dir 13 file 0xC9): Jewel. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0c9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_JEWEL_GEM          0x1 /* model: faceted gem (polyhedron, white texture) */
#define FX_JEWEL_GEM_SPARKLES 0x2 /* emitter: stone / crystal glint particles and star sparkles (+ sprites, texture not in the file) */
#define FX_JEWEL_COMET_STREAKS 0x3 /* emitter: two fans, white-violet comet streaks */
#define FX_JEWEL_LIGHT_BURST  0x4 /* emitter: white / lilac / cyan rings and violet light streaks */
#define FX_JEWEL_ORE_GLINT    0x5 /* emitter: green ore glint, lilac / cyan ring, star sparkles */
#define FX_JEWEL_BACKDROP     0x6 /* table: background layers for btlfx_drawBackdrop */
#define FX_JEWEL_GLOW_RING    0x10 /* emitter: white-cyan gouraud ring, fans */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster position */
    SVECTOR pos10; /* caster body position */
} Fx0c9Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* unit 0x10 body position, raised by 0x50 */
    SVECTOR pos14; /* caster position */
    SVECTOR pos1C; /* moves from pos14 to posC */
    SVECTOR rot24; /* model rotation */
    VECTOR pos2C; /* model position */
    VECTOR scale3C; /* model scale */
    SVECTOR pos4C;
    u16 model54; /* model */
} Fx0c9Work81;

extern Unk800F78A4 *fx_0c9_env;
extern BtlModelBuf fx_0c9_effModels[];
extern SVECTOR fx_0c9_particlePos; /* particle position */
extern SVECTOR fx_0c9_emitPos;
extern s32 fx_0c9_trails[];

static s32 fx_0c9_op81ParticleMotion(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
