#include "fxmod/fx_0a5.h"

s32 fx_0a5_op80(s32 mode, Fx0a5Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 u;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        D_801E7A2C = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk8);
        work->unk18.vx = env->targets->centerX;
        work->unk18.vy = env->targets->centerY;
        work->unk18.vz = env->targets->centerZ;
        work->unk20.vx = env->targets->centerX;
        work->unk20.vy = env->targets->centerY;
        work->unk20.vz = env->targets->centerZ;
        work->unk18.vx = 0x5DC;
        work->unk20.vx = -0x5DC;
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk8);
        ex->btlfx_aimAngles((s16 *)&work->unk8, &env->targets->centerX, (s16 *)&work->unk10);
    } else {
        frame = entry->frame;
        if (frame == 0x19) {
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk18);
            if (work->unk0 != NULL) {
                work->unk0->unk14 = (s32)D_801E7A3C;
                work->unk0->unk12 = 0x80;
                work->unk0->unkC = 0x20;
            }
            if (work->unk0 != NULL) {
                work->unk0->otZ = 0x80;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->state == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->rot.vx = work->unk10.vx;
                work->unk4->rot.vy = work->unk10.vy;
                work->unk4->rot.vz = work->unk10.vz;
            }
        }
        if (frame >= 0x19) {
            t = (frame << 12) / 30;
            u = 0x1000;
            if (t <= 0x1000) {
                u = t;
            }
            ex->btlfx_lerpVec((s16 *)&work->unk18, (s16 *)&work->unk20, u, (s16 *)&work->unk28);
            if (work->unk0 != NULL) {
                if (work->unk0->state == -1) {
                    work->unk0 = NULL;
                }
                if (work->unk0 != NULL) {
                    work->unk0->unk2C = (s32)&work->unk28;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a5/nonmatchings/fx_0a5", fx_0a5_headerPtr);
