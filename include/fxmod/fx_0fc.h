#ifndef FX_0FC_H
#define FX_0FC_H

/* Effect module fx_0fc (dir 13 file 0x0FC): enemy retreat (slides the target back and fades it out; ability name not
 * confirmed). Used by encounters 220 and 238 only (its name text is <FAD5><FC>= in the effect labels). Runtime (round
 * 33, played on the Malboro): fx_0fc_op80 moves target 0 back along +z 0xC0 per frame and fades it out at frame 0x12. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_0fc_env;

#endif
