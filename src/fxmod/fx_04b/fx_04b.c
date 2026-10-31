#include "fxmod/fx_04b.h"

s32 fx_04b_op80(s32 mode, Fx04bWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        fx_04b_env = env;
        work->pos8.vz = env->targets->centerZ;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 10:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos8);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)D_801E7BA4;
                work->emit0->trailSlots = 0x82;
                work->emit0->trailStride = 0x18;
            }
            break;
        case 40:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk10);
            D_801E7B9C.vx = work->unk10.vx;
            D_801E7B9C.vy = work->unk10.vy;
            D_801E7B9C.vz = work->unk10.vz;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos18);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)D_801E87D4;
                work->emit4->trailSlots = 0xA0;
                work->emit4->trailStride = 0x18;
            }
            ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->pos8, (s16 *)&work->unk20);
            break;
        }
        if (frame >= 10) {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos18);
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->target = (s32)&work->pos18;
                }
            }
        }
        if (frame >= 40) {
            if (work->emit4 != NULL) {
                if (work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                if (work->emit4 != NULL) {
                    work->emit4->rot.vx = work->unk20.vx;
                    work->emit4->rot.vy = work->unk20.vy;
                    work->emit4->rot.vz = work->unk20.vz;
                }
            }
        }
        return frame >= 100;
    }
    return 0;
}

s32 fx_04b_op81(s32 mode, Fx04bWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        s32 n = *arg4;

        work->unk14 = n;
        fx_04b_env = env;
        if (n >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitPos(n, (s16 *)&work->pos4);
        work->pos4.vy -= 0xC8;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->unk14, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&D_801E7B9C, (s16 *)&work->pos4, (s16 *)&work->unkC);
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->unkC.vx;
                work->emit0->rot.vy = work->unkC.vy;
                work->emit0->rot.vz = work->unkC.vz;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04b/nonmatchings/fx_04b", fx_04b_headerPtr);
