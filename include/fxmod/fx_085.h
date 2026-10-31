#ifndef FX_085_H
#define FX_085_H

/* Effect module fx_085 (dir 13 file 0x85): Thundaga, all targets (and one large target). Lightning bolts drawn as
 * jagged strips (fxm_drawJaggedStrip15 / fxm_drawJaggedStrip3, fxm_drawJaggedBranch3, fxm_jaggedStripTask).
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width); as Fx018Strip */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx085Strip; /* 0x7C */
extern u16 D_801E9D94[];
static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 D_801E9D9C[];
static void fxm_drawJaggedStrip3(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx085Strip *strip, Fx085Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *fx_085_env;
extern SVECTOR D_801E9FA0[]; /* strips[0].unk4: the strips start 4 bytes earlier */
#define FX_085_STRIPS ((Fx085Strip *)((u8 *)D_801E9FA0 - 4))
static s32 fxm_randStripPoint(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[3];
    u8 pad1C[0x1C];
} Fx085Branch; /* 0x38 */

/* one bolt, a pool item; as Fx018Bolt */
typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    u8 unk10; /* bolt index */
    u8 unk11; /* state */
    u8 unk12; /* branch points */
    u8 unk13;
    u8 unk14; /* kind */
    s8 unk15; /* extra width */
    u8 pad16[2];
    s32 *unk18; /* jitter table */
} Fx085Bolt;

extern Fx085Strip D_801E9F9C[];
extern Fx085Branch D_801EA75C[];
extern s32 D_801EAADC; /* faster bolts */

static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx085Strip *strip, Fx085Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
static s32 fxm_jaggedStripTask(s32 mode, Fx085Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

typedef struct {
    SVECTOR pos0; /* target body */
    SVECTOR unk8; /* target */
    s32 *unk10; /* jitter table */
    s32 unk14; /* bolts spawned */
    s32 unk18; /* kind */
} Fx085Work83; /* 0x1C */

extern SVECTOR D_801E9DEC; /* previous bolt end */
extern Unk80100C78Entry *fx_085_task;
extern s32 D_801EAAEC[];

typedef struct {
    SVECTOR unk0; /* target position */
    SVECTOR pos8; /* ground position */
    s32 unk10; /* state */
    s32 model14; /* model */
    s32 model18; /* model */
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
} Fx085Work80; /* 0x24 */

extern BtlModel fx_085_effModels[];
extern s32 D_801E9F8C;
#endif
