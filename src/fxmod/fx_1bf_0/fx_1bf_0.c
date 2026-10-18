#include "fxmod/fx_1bf_0.h"

INCLUDE_ASM("asm/us/fx_1bf_0/nonmatchings/fx_1bf_0", func_801E7704);

INCLUDE_ASM("asm/us/fx_1bf_0/nonmatchings/fx_1bf_0", func_801E77C0);

INCLUDE_ASM("asm/us/fx_1bf_0/nonmatchings/fx_1bf_0", func_801E7B70);

void func_801E7E98(Unk80100C78Entry *entry, SVECTOR *from, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    SVECTOR *d = D_801E87BC;
    s32 i = 0;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8830);

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    do {
        pos.vx = from->vx + d->vx * 2;
        pos.vy = from->vy + d->vy * 2;
        pos.vz = from->vz + d->vz * 2;
        ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
        rot.vz = -ang.vy;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E879C[i + 8]), &pos, &rot, 0x2AF8, 0, 1, ot, 0, 0);
        d++;
        i++;
    } while (i < 3);
}

INCLUDE_ASM("asm/us/fx_1bf_0/nonmatchings/fx_1bf_0", fx_1bf_0_op80);

INCLUDE_RODATA("asm/us/fx_1bf_0/nonmatchings/fx_1bf_0", fx_1bf_0_headerPtr);
