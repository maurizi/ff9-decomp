#include "fxmod/fx_17d_4.h"

INCLUDE_ASM("asm/us/fx_17d_4/nonmatchings/fx_17d_4", func_801E7720);

INCLUDE_ASM("asm/us/fx_17d_4/nonmatchings/fx_17d_4", func_801E77DC);

INCLUDE_ASM("asm/us/fx_17d_4/nonmatchings/fx_17d_4", func_801E7B8C);

void func_801E7EB4(Unk80100C78Entry *entry, SVECTOR *from, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    SVECTOR *d = D_801E981C;
    s32 i = 0;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9890);

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    do {
        pos.vx = from->vx + d->vx * 2;
        pos.vy = from->vy + d->vy * 2;
        pos.vz = from->vz + d->vz * 2;
        ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
        rot.vz = -ang.vy;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E97FC[i + 8]), &pos, &rot, 0x2AF8, 0, 1, ot, 0, 0);
        d++;
        i++;
    } while (i < 3);
}

INCLUDE_RODATA("asm/us/fx_17d_4/nonmatchings/fx_17d_4", fx_17d_4_headerPtr);

INCLUDE_ASM("asm/us/fx_17d_4/nonmatchings/fx_17d_4", fx_17d_4_op80);
