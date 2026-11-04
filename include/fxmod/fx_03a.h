#ifndef FX_03A_H
#define FX_03A_H

/* Effect module fx_03a (dir 13 file 0x3A). */
#include "btlfx_mod.h"

/* fx_03a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_EARTHQUAKE_ROCK_CHUNK_8   0x1 /* 'so' model: brown rock chunk 8 bounced by the quake (fx_03a_models[8]) */
#define FX_EARTHQUAKE_ROCK_CHUNK_1   0x2 /* 'so' model: brown rock chunk 1 bounced by the quake (fx_03a_models[1]) */
#define FX_EARTHQUAKE_ROCK_CHUNK_2   0x3 /* 'so' model: brown rock chunk 2 bounced by the quake (fx_03a_models[2]) */
#define FX_EARTHQUAKE_ROCK_CHUNK_3   0x4 /* 'so' model: brown rock chunk 3 bounced by the quake (fx_03a_models[3]) */
#define FX_EARTHQUAKE_ROCK_CHUNK_4   0x5 /* 'so' model: brown rock chunk 4 bounced by the quake (fx_03a_models[4]) */
#define FX_EARTHQUAKE_ROCK_CHUNK_5   0x6 /* 'so' model: brown rock chunk 5 bounced by the quake (fx_03a_models[5]) */
#define FX_EARTHQUAKE_ROCK_CHUNK_6   0x7 /* 'so' model: brown rock chunk 6 bounced by the quake (fx_03a_models[6]) */
#define FX_EARTHQUAKE_ROCK_CHUNK_7   0x8 /* 'so' model: brown rock chunk 7 bounced by the quake (fx_03a_models[7]) */
#define FX_EARTHQUAKE_ROCK_CHUNK_0   0x9 /* 'so' model: brown rock chunk 0 bounced by the quake (fx_03a_models[0]) */
#define FX_EARTHQUAKE_ROCK_CHUNK_9   0xE /* 'so' model: brown rock chunk, fx_03a_models[9], registered but not drawn */
#define FX_EARTHQUAKE_RUBBLE         0x10 /* emitter: yellow striped rubble quads */

typedef struct {
    s32 bounce0; /* bounce countdown */
    s32 powerUp4; /* power-up countdown */
    s32 powerDown8; /* power-down countdown */
    s32 gridC; /* blur countdown */
    s32 unk10[4]; /* unused (runtime watch, round 33) */
    SVECTOR pos20;
    u8 pad28[8];
} Fx03aWork; /* 0x30 */

/* bouncing model */
typedef struct {
    s32 model0; /* model */
    VECTOR scale4; /* scale */
    VECTOR pos14; /* position */
    SVECTOR rot24; /* rotation */
    s32 speed2C; /* bounce speed */
    s32 time30; /* bounce time */
} Fx03aModel; /* 0x34 */

extern Unk800F78A4 *fx_03a_env;
extern Fx03aModel fx_03a_models[10];
extern s32 fx_03a_bouncePower; /* bounce power */
extern BtlModelBuf fx_03a_effModels[];
extern u32 fx_03a_bgOt[];

#endif
