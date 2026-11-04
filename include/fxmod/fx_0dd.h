#ifndef FX_0DD_H
#define FX_0DD_H

/* Effect module fx_0dd (dir 13 file 0xDD): Sword Magic Bio. Two models morphing along key tracks
 * (Hi_DrawMorphEffModel, fxm_rotPosTrackGet). Evidence: [data] the party ability / item tables and enemy sequences
 * that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0dd effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_BIO_PATH 0x1 /* table: rotation/position keys of the morphing bubble (fxm_rotPosTrackInit) */
#define FX_SWDMAG_BIO_SCALE_TRACK 0x2 /* table: scale keys of the morphing bubble (fxm_vecTrackInit) */
#define FX_SWDMAG_BIO_SCALE_TRACK_2 0xF /* table: scale keys of the second bubble pass (fxm_vecTrackInit) */
#define FX_SWDMAG_BIO_PATH_2 0x10 /* table: rotation/position keys of the second bubble pass */
#define FX_SWDMAG_BIO_BUBBLE_TRAIL   0x11 /* emitter: Bio's screen trail of glowing green bubbles; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_SWDMAG_BIO_TEAL_RING 0x12 /* emitter: teal-green wisp rings */
#define FX_SWDMAG_BIO_TOXIC_CLOUD 0x13 /* emitter: green gas cloud + grey haze */
#define FX_SWDMAG_BIO_RED_ORB 0x14 /* emitter: red cell-mesh balls with a crimson glow */
#define FX_SWDMAG_BIO_SWORD_GLINT    0x15 /* rings: white 6-segment rings flashed at the sword (op84, first draw) */
#define FX_SWDMAG_BIO_SWORD_FLASH    0x16 /* rings: white 8-segment rings flashed rotating at the sword (op84) */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* time */
    SVECTOR pos8; /* caster bone position */
} Fx0ddWork84;

extern Unk800F78A4 *fx_0dd_env;

typedef struct {
    s16 posX0, posY2, posZ4; /* position */
    s16 rotX6, rotY8, rotZA; /* rotation */
} Fx0ddKey12;

typedef struct {
    u16 count0; /* count */
    Fx0ddKey12 *keys4;
} Fx0ddTrack12;
typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* caster position */
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    u8 pad40[0x44 - 0x40];
    SVECTOR pos44; /* model position (16 bit) */
    SVECTOR pos4C; /* effect position */
    s32 morphTime54; /* morph time */
    s32 unk58; /* write-only (-1 at init) */
    u16 model5C[6]; /* models */
} Fx0ddWork80; /* 0x68 */

static void fxm_rotPosTrackInit(Fx0ddTrack12 *track, u16 *data);
static s32 fxm_rotPosTrackGet(Fx0ddTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);

typedef struct {
    s16 x, y, z;
} Fx0ddKey6;

typedef struct {
    u16 count0; /* count */
    Fx0ddKey6 *keys4;
} Fx0ddTrack6;
static void fxm_vecTrackInit(Fx0ddTrack6 *track, u16 *data);
static s32 fxm_vecTrackGet(Fx0ddTrack6 *track, s32 t, VECTOR *out);

extern u8 fx_0dd_morphBlocks[6];
extern BtlModelBuf fx_0dd_effModels[];
extern s32 fx_0dd_trails[];
extern Fx0ddTrack12 fx_0dd_rotPosKeys;
extern Fx0ddTrack12 fx_0dd_rotPosKeys2;
extern Fx0ddTrack6 fx_0dd_scaleKeys;
extern Fx0ddTrack6 fx_0dd_scaleKeys2;
extern SVECTOR fx_0dd_emitPos;

s32 fx_0dd_op80(s32 mode, Fx0ddWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
