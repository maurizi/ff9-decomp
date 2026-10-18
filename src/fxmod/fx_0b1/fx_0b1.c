#include "fxmod/fx_0b1.h"

void func_801E7704(void) {
    s32 i;
    s32 m;

    D_801E8B90 = 0;
    m = -1;
    for (i = 15; i >= 0; i--) {
        D_801E8B9C[i].unk4 = m;
    }
}

void func_801E7738(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (D_801E8B9C[i].unk4 == -1) {
            D_801E8B9C[i].unk4 = 0;
            D_801E8B9C[i].unk10 = data + 1;
            D_801E8B9C[i].unk0 = start;
            D_801E8B9C[i].unk8 = dst;
            D_801E8B9C[i].unkC = src;
            return;
        }
    }
}

INCLUDE_ASM("asm/us/fx_0b1/nonmatchings/fx_0b1", func_801E7788);

s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8B8C);
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

INCLUDE_ASM("asm/us/fx_0b1/nonmatchings/fx_0b1", fx_0b1_op80);

s32 fx_0b1_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EAA54 = 0;
        D_801E8B8C = env;
        ex->btlfx_initEffModels(D_801EAAEC, 4);
        for (i = 0; i < env->unkC->unk24; i++) {
            D_801EAA5C[i] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x23), 0x177,
                                                       0xFFFF, 0, 0x80);
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801EAACC[i]);
            D_801EAA8C[i].vx = D_801EAACC[i].vx;
            D_801EAA8C[i].vy = D_801EAACC[i].vy;
            D_801EAA8C[i].vz = D_801EAACC[i].vz + 0x100;
            D_801EAACC[i].vz += 0x200;
        }
        D_801EAA6C.vx = 0;
        D_801EAA6C.vy = 0x800;
        D_801EAA6C.vz = 0;
        D_801EAA7C.vx = 0x1800;
        D_801EAA7C.vy = 0x1800;
        D_801EAA7C.vz = 0x1800;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            do {
                D_801EAA54 = 0x20;
            } while (0);
            for (i = 0; i < env->unkC->unk24; i++) {
                ex->func_800E3CEC((s32)ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&D_801EAACC[i]);
                ex->func_800E3CEC((s32)ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&D_801EAACC[i]);
            }
        }
        if (D_801EAA54 != 0) {
            D_801EAA54--;
            do {
                for (i = 0; i < env->unkC->unk24; i++) {
                    ex->Hi_DrawEffModel(&D_801EAA6C, &D_801EAA8C[i], (s32)&D_801EAA7C, D_801EAA5C[i]);
                    if (D_801EAA54 >= 0x19) {
                        c = (0x18 - D_801EAA54) * 0x10;
                        ex->Hi_ModifyEffModelRGB(D_801EAA5C[i], c, c, c);
                    }
                    if (D_801EAA54 < 8) {
                        c = (D_801EAA54 - 8) * 0x10;
                        ex->Hi_ModifyEffModelRGB(D_801EAA5C[i], c, c, c);
                    }
                }
            } while (0);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x22), frame);
        }
        return frame >= 0x20;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b1/nonmatchings/fx_0b1", fx_0b1_headerPtr);
