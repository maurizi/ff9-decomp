#ifndef FX_0FC_H
#define FX_0FC_H

/* Effect module fx_0fc (dir 13 file 0x0FC): an enemy ability (its name text is <FAD5><FC>= in the effect labels).
 * Puts the target back at its home position and fades it. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_0fc_env;

#endif
