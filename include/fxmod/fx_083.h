#ifndef FX_083_H
#define FX_083_H

/* Effect module fx_083 (dir 13 file 0x83): Scan (party and enemy). Four orbiting ring models, pillar models and
 * scrolling texture frames (btlfx_drawScrollMove). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx083Vec3;

typedef struct {
    s32 timers0[10]; /* countdowns */
    s32 model28[4]; /* ring models */
    SVECTOR ringRot38[4]; /* ring rotations */
    VECTOR ringPos58[4]; /* ring positions */
    Fx083Vec3 ringScale98; /* ring scale */
    s32 unkA4;
    s32 ringOffsetA8; /* ring radius offset */
    s32 ringHeightAC; /* ring height */
    s32 modelB0[5]; /* pillar models */
    s32 pillarOnC4[5]; /* pillar active */
    s32 pillarFadeD8[5]; /* pillar fade */
    VECTOR pillarPosEC[5]; /* pillar positions */
    s32 fade13C; /* fade */
    SVECTOR rot140; /* pillar rotation */
    Fx083Vec3 pillarScale148; /* pillar scale */
    s32 unk154;
    SVECTOR center158; /* center */
    SVECTOR pos160; /* caster position */
    s32 radius168; /* radius */
    s32 height16C; /* height */
    Pool6CEntry *emit170[4];
    SVECTOR emitStartPos180[4];
    SVECTOR emitPos1A0[4];
    s32 orbit1C0; /* orbit angle */
} Fx083Work; /* 0x1C4 */

typedef struct {
    s32 x0;
    s32 y4;
} Fx083Pair;

extern Fx083Pair fx_083_texAnimSrc1[];
extern Fx083Pair fx_083_texAnimSrc2[];
extern Unk800F78A4 *fx_083_env;
extern BtlModelBuf fx_083_effModels[];

#endif
