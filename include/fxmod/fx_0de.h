#ifndef FX_0DE_H
#define FX_0DE_H

/* Effect module fx_0de (dir 13 file 0xDE): Sword Magic Water. A morphing model at the caster, and the targets
 * thrown up and dropped (fx_0de_op81). Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

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
    s32 stop0; /* dead early-out flag, cleared at init and never set (see Fx046Work.stop0) */
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
