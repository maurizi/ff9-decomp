#include "fxmod/fx_092.h"

s32 fx_092_op80(s32 mode, Fx092Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x48;
    } else if (mode == 1) {
        D_801E7A1C = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk18);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk20);
        work->unk30.vx = work->unk20.vx;
        work->unk30.vy = work->unk20.vy;
        work->unk30.vz = work->unk20.vz;
        work->unk38.vx = work->unk20.vx;
        work->unk38.vy = work->unk20.vy;
        work->unk38.vz = work->unk20.vz;
        work->unk30.vy -= 0x1C2;
        work->unk38.vy -= 0xC8;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk28);
        ex->btlfx_pointAhead((s16 *)&work->unk28, (s16 *)&work->unk18, 0xC8, (s16 *)&work->unk28);
        env->camera->points[0].vx = work->unk28.vx;
        env->camera->points[0].vy = work->unk28.vy;
        env->camera->points[0].vz = work->unk28.vz;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk28);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x20);
        }
    } else {
        frame = entry->unk0;
        ex->btlfx_aimAngles(&env->camera->eye.vx, (s16 *)&work->unk28, (s16 *)&work->unk40);
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk40.vx;
                work->unk0->unk5C.vy = work->unk40.vy;
                work->unk0->unk5C.vz = work->unk40.vz;
            }
        }
        if (frame == 0xA) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk30);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk20);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk38);
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_092/nonmatchings/fx_092", fx_092_headerPtr);
