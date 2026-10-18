#include "fxmod/fx_17d_4.h"

void func_801E7720(u8 *file, SVECTOR *a, SVECTOR *b, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9890);
    SVECTOR *v;

    D_801E987C.unk0 = D_801E987C.unk2 = 1;
    D_801E987C.unk1 = 0;
    v = D_801E987C.unk4;
    v[0].vx = a->vx;
    v[0].vy = a->vy;
    v[0].vz = a->vz;
    v[0].pad = arg6;
    v[1].vx = b->vx;
    v[1].vy = b->vy;
    v[1].vz = b->vz;
    v[1].pad = arg6;
    ex->btlfx_drawTrail(file, 0, NULL, arg3, 0x1000, arg4, arg5, (u8 *)&D_801E987C, 0);
}

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
