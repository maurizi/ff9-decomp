#include "fxmod/fx_036.h"

s32 func_801E7704(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7C28);

    out->vx = D_801E7BCC[D_801E7BDC];
    out->vy = D_801E7C30.vy - ex->btlfx_rand() % 400;
    out->vz = D_801E7C30.vz;
    if (ex->btlfx_rand() & 1) {
        out->vz += ex->btlfx_rand() % 600;
    } else {
        out->vz -= ex->btlfx_rand() % 600;
    }
    out->pad = 1;
    D_801E7BDC++;
    return 0;
}

s32 fx_036_op80(s32 mode, Fx036Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x2C;
    } else if (mode == 1) {
        D_801E7C30.vx = env->targets->centerX;
        D_801E7C30.vy = env->targets->centerY;
        D_801E7C28 = env;
        D_801E7C30.vz = env->targets->centerZ;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 1:
            ex->func_800DC818(0x10, 1, (u16 *)&work->unk1C);
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk1C);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            break;
        case 5:
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk1C);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk1C);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk1C);
            break;
        case 9:
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk24);
            if (work->unk10 != NULL) {
                work->unk10->unk24->unk0 = -1;
                ((EffTailSpawn *)work->unk10->unk24)->unk3C = func_801E7704;
            }
            work->unk14 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&env->targets->centerX);
            work->unk18 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&env->targets->centerX);
            if (work->unk10 != NULL) {
                work->unk10->unk22 = 0x100;
            }
            break;
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_036_op81(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 2;
    } else if (mode == 1) {
        D_801E7C28 = env;
        *work = 0x80;
    } else {
        ex->btlfx_fadeUnitById(0x10, *work);
        *work -= 8;
        return *work < 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_036/nonmatchings/fx_036", fx_036_headerPtr);
