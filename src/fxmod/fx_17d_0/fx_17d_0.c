#include "fxmod/fx_17d_0.h"

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", func_801E7704);

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", func_801E7894);

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", func_801E79E8);

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", func_801E7D18);

s32 func_801E7FE0(s32 mode, Fx17d0Part *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 1) {
        p->unk0 = 0;
    } else {
        p->unk8.vz += p->unk2 + 0x186;
        env->unk28 = -0x80;
        if (frame == 0) {
            ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&p->unk8, 0, -1, 0x800, 0x1000, 1,
                              D_801EBC90[p->unk4], 0);
        }
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&p->unk8, 0, 0, 0x800, 0x1000, 1,
                          D_801EBC90[p->unk4], 0);
        env->unk28 = 0;
        if (frame >= 0x10) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", fx_17d_0_op80);

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", fx_17d_0_op82);

INCLUDE_RODATA("asm/us/fx_17d_0/nonmatchings/fx_17d_0", fx_17d_0_headerPtr);
