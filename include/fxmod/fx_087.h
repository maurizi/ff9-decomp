#ifndef FX_087_H
#define FX_087_H

/* Effect module fx_087 (dir 13 file 0x87): Spare Change. Emitters only. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* unit 0x10 body position */
    SVECTOR pos14; /* caster body position */
    SVECTOR pos1C; /* btlfx_getUnitBonePos(0x10, 0x17) */
    u8 pad24[0x2C - 0x24];
    SVECTOR aim2C; /* angles from posC to pos14 */
} Fx087Work;

extern Unk800F78A4 *fx_087_env;
extern s32 fx_087_trails[];

#endif
