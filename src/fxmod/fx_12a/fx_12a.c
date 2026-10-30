#include "fxmod/fx_12a.h"

s32 fx_12a_op80(s32 mode, Fx12aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x60;
    } else if (mode == 1) {
        D_801E7EFC = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk20);
        ex->btlfx_getUnitArgBonePos(0x10, 1, (u16 *)&work->unk28);
        work->unk30.vx = work->unk20.vx;
        work->unk30.vy = work->unk20.vy;
        work->unk30.vz = work->unk20.vz;
        work->unk38.vx = work->unk28.vx;
        work->unk38.vy = work->unk28.vy;
        work->unk38.vz = work->unk28.vz;
        work->unk20.vy += 0x64;
        work->unk28.vy += 0x64;
        work->unk20.vz -= 0x64;
        work->unk28.vz -= 0x64;
        work->unk40.vx = env->targets->centerX;
        work->unk40.vy = env->targets->centerY;
        work->unk40.vz = env->targets->centerZ;
        work->unk48.vx = env->targets->centerX;
        work->unk48.vy = env->targets->centerY;
        work->unk48.vz = env->targets->centerZ;
        work->unk40.vx = work->unk20.vx;
        work->unk48.vx = work->unk28.vx;
        work->unk40.vy -= 0x190;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk20);
        work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk20);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk28);
        work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk28);
        if (work->unk8 != NULL) {
            work->unk8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->unkC != NULL) {
            work->unkC->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&work->unk20, (s16 *)&work->unk40, (s16 *)&work->unk50);
        ex->btlfx_aimAngles((s16 *)&work->unk28, (s16 *)&work->unk48, (s16 *)&work->unk58);
    } else {
        frame = entry->frame;
        if (frame == 6) {
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk30);
            work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk20);
            work->unk14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk38);
            work->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk28);
            if (work->unk18 != NULL) {
                work->unk18->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->unk1C != NULL) {
                work->unk1C->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
        }
        if (frame >= 6) {
            if (work->unk10 != NULL) {
                if (work->unk10->state == -1) {
                    work->unk10 = NULL;
                }
                if (work->unk10 != NULL) {
                    work->unk10->rot.vx = work->unk50.vx;
                    work->unk10->rot.vy = work->unk50.vy;
                    work->unk10->rot.vz = work->unk50.vz;
                }
            }
            if (work->unk18 != NULL) {
                if (work->unk18->state == -1) {
                    work->unk18 = NULL;
                }
                if (work->unk18 != NULL) {
                    work->unk18->rot.vx = work->unk50.vx;
                    work->unk18->rot.vy = work->unk50.vy;
                    work->unk18->rot.vz = work->unk50.vz;
                }
            }
            if (work->unk14 != NULL) {
                if (work->unk14->state == -1) {
                    work->unk14 = NULL;
                }
                if (work->unk14 != NULL) {
                    work->unk14->rot.vx = work->unk58.vx;
                    work->unk14->rot.vy = work->unk58.vy;
                    work->unk14->rot.vz = work->unk58.vz;
                }
            }
            if (work->unk1C != NULL) {
                if (work->unk1C->state == -1) {
                    work->unk1C = NULL;
                }
                if (work->unk1C != NULL) {
                    work->unk1C->rot.vx = work->unk58.vx;
                    work->unk1C->rot.vy = work->unk58.vy;
                    work->unk1C->rot.vz = work->unk58.vz;
                }
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_12a_op81(s32 mode, Fx12aWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk4.vx = env->targets->centerX;
        work->unk4.vy = env->targets->centerY;
        work->unk4.vz = env->targets->centerZ;
        D_801E7EFC = env;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk4);
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_12a_op82(s32 mode, Fx12aWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 n;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        n = *arg4;
        work->unk10 = n;
        D_801E7EFC = env;
        if (n >= env->targets->targetCount) {
            func_801E7EA0(env, 7, 1);
            return 1;
        }
        func_801E7EA0(env, 7, 0);
        ex->btlfx_getUnitPos(work->unk10, (s16 *)&work->unk8);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->otZ = ex->btlfx_addRadiusSixth(work->unk10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 4) {
            func_801E7EA0(env, 7, 1);
        }
        return frame >= 0x28;
    }
    return 0;
}

void func_801E7EA0(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->state + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_12a/nonmatchings/fx_12a", fx_12a_headerPtr);
