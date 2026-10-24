#include "fxmod/fx_030.h"

s32 fx_030_op80(s32 mode, Fx030Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        D_801E7A80 = env;
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk0);
        work->unk8.vx = env->targets->centerX;
        work->unk8.vz = env->targets->centerZ + 0x1F4;
        work->unk8.vy = 0;
        ex->btlfx_aimAngles((s16 *)&work->unk0, (s16 *)&work->unk8, (s16 *)&work->unk10);
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk0, (s16 *)&work->unk18);
        work->unk20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk0);
        work->unk24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk0);
        work->unk28 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk0);
    } else {
        frame = entry->unk0;
        if (work->unk20 != NULL && work->unk20->unk30 == -1) {
            work->unk20 = NULL;
        }
        if (work->unk24 != NULL && work->unk24->unk30 == -1) {
            work->unk24 = NULL;
        }
        if (work->unk28 != NULL && work->unk28->unk30 == -1) {
            work->unk28 = NULL;
        }
        if (work->unk20 != NULL) {
            work->unk20->unk5C.vx = work->unk10.vx;
            work->unk20->unk5C.vy = work->unk10.vy;
            work->unk20->unk5C.vz = work->unk10.vz;
        }
        if (work->unk24 != NULL) {
            work->unk24->unk5C.vx = work->unk18.vx;
            work->unk24->unk5C.vy = work->unk18.vy;
            work->unk24->unk5C.vz = work->unk18.vz;
        }
        if (work->unk28 != NULL) {
            work->unk28->unk5C.vx = work->unk10.vx;
            work->unk28->unk5C.vy = work->unk10.vy;
            work->unk28->unk5C.vz = work->unk10.vz;
        }
        if (frame == 6) {
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&work->unk2C[i]);
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk2C[i]);
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_030/nonmatchings/fx_030", fx_030_headerPtr);
