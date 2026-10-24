#include "fxmod/fx_1f8.h"

s32 fx_1f8_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E7B6C.vx = 0;
        D_801E7B6C.vy = 0;
        D_801E7B6C.vz = -0x618;
        D_801E7B64 = env;
        ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&D_801E7B6C);
    } else {
        return entry->unk0 >= 0x40;
    }
    return 0;
}

s32 fx_1f8_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E7B74[0] = 0;
        D_801E7B64 = env;
        ex->btlfx_initEffModels(D_801E7C14, 4);
        for (i = 0; i < env->targets->targetCount; i++) {
            D_801E7B84[i] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0x177,
                                                       0xFFFF, 0, 0x80);
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801E7BF4[i]);
            D_801E7BB4[i].vx = D_801E7BF4[i].vx;
            D_801E7BB4[i].vy = D_801E7BF4[i].vy;
            D_801E7BB4[i].vz = D_801E7BF4[i].vz + 0x100;
            D_801E7BF4[i].vz += 0x200;
        }
        D_801E7B94.vx = 0;
        D_801E7B94.vy = 0x800;
        D_801E7B94.vz = 0;
        D_801E7BA4.vx = 0x1800;
        D_801E7BA4.vy = 0x1800;
        D_801E7BA4.vz = 0x1800;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            D_801E7B74[0] = 0x20;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&D_801E7BF4[i]);
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&D_801E7BF4[i]);
            }
        }
        if (D_801E7B74[0] != 0) {
            D_801E7B74[0]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->Hi_DrawEffModel(&D_801E7B94, &D_801E7BB4[i], (s32)&D_801E7BA4, D_801E7B84[i]);
                if (D_801E7B74[0] >= 0x19) {
                    c = (0x18 - D_801E7B74[0]) * 0x10;
                    ex->Hi_ModifyEffModelRGB(D_801E7B84[i], c, c, c);
                }
                if (D_801E7B74[0] < 8) {
                    c = (D_801E7B74[0] - 8) * 0x10;
                    ex->Hi_ModifyEffModelRGB(D_801E7B84[i], c, c, c);
                }
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xB), frame);
        }
        return frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f8/nonmatchings/fx_1f8", fx_1f8_headerPtr);
