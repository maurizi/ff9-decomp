#include "fxmod/fx_17a.h"

INCLUDE_ASM("asm/us/fx_17a/nonmatchings/fx_17a", func_801E7718);

s32 func_801E7BC0(s32 mode, Fx17aPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 1) {
        p->unk2 = 0;
        p->unk14.vx = ex->btlfx_rand();
        p->unk14.vy = ex->btlfx_rand();
        p->unk14.vz = 0;
        p->unk1C = ex->btlfx_rand() % 128 + 0x80;
        p->unk1E = 0xC0 - ex->btlfx_rand() % 32;
        p->unk24 = 0;
        p->unk20 = 0;
    } else if (p->unk0 == 4) {
        p->unkC.vx += -0x80 - p->unkA;
        ex->func_800D1680((u8 *)ex->btlfx_getFileData(entry->unk8, 2), &p->unkC, NULL, p->unk4 * 3 >> 1, 0, 1, -1, 0, 0);
        if (frame >= 0x11) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17a/nonmatchings/fx_17a", fx_17a_headerPtr);

INCLUDE_ASM("asm/us/fx_17a/nonmatchings/fx_17a", fx_17a_op80);
