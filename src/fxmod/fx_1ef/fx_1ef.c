#include "fxmod/fx_1ef.h"

INCLUDE_ASM("asm/us/fx_1ef/nonmatchings/fx_1ef", func_801E7704);

INCLUDE_ASM("asm/us/fx_1ef/nonmatchings/fx_1ef", func_801E7738);

INCLUDE_ASM("asm/us/fx_1ef/nonmatchings/fx_1ef", func_801E7788);

s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8B9C);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

INCLUDE_ASM("asm/us/fx_1ef/nonmatchings/fx_1ef", fx_1ef_op80);

s32 fx_1ef_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EAA64[0] = 0;
        D_801EAA64[1] = 0;
        D_801E8B9C = env;
        for (i = 0; i < env->unkC->unk24; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801EAA6C[i]);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&D_801EAA6C[i]);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(i, 0x200);
                p->unk12 = 0x1E;
                p->unk14 = (s32)D_801EAA8C[i];
                p->unkC = 0x9C;
            }
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 16:
            for (i = 0; i < env->unkC->unk24; i++) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&D_801EAA6C[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x200);
                    p->unk12 = 0x1E;
                    p->unk14 = (s32)D_801ECA6C[i];
                    p->unkC = 0x9C;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&D_801EAA6C[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x200);
                }
            }
            break;
        case 24:
            D_801EAA64[0] = 0x10;
            break;
        case 40:
            D_801EAA64[1] = 0x10;
            break;
        }
        if (D_801EAA64[0] != 0) {
            D_801EAA64[0]--;
            for (i = 0; i < env->unkC->unk24; i++) {
                ex->func_800E806C(i, D_801EAA64[0] * 8);
            }
        }
        if (D_801EAA64[1] != 0) {
            D_801EAA64[1]--;
            for (i = 0; i < env->unkC->unk24; i++) {
                ex->func_800E806C(i, 0x80 - D_801EAA64[1] * 8);
            }
        }
        return frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ef/nonmatchings/fx_1ef", fx_1ef_headerPtr);
