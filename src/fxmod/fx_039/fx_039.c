#include "fxmod/fx_039.h"

s32 fx_039_op80(s32 mode, Fx039Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E7BA0 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk8);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

s32 fx_039_op81(s32 mode, Fx039Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        s32 n = *arg4;

        D_801E7BA0 = env;
        work->unk20 = n;
        if (n >= 3) {
            return 1;
        }
        if (ex->btlfx_isEnemyUnit(0x10)) {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk8);
        } else {
            ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        }
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk10);
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0xA) {
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 9), (s32)&D_801EA0D0[work->unk20]);
            if (work->unk0 != NULL) {
                work->unk0->unk14 = (s32)D_801E7BB0[work->unk20];
                work->unk0->unk12 = 0x12;
                work->unk0->unkC = 0xB0;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0, 0x10);
            }
        }
        if (frame >= 0xA) {
            s32 d = frame - 0xA;

            if (d < 0x10) {
                t = (d << 12) / 15;
                ex->btlfx_lerpVec((s16 *)&work->unk10, (s16 *)&work->unk8, t, (s16 *)&work->unk18);
                if (work->unk20 == 1) {
                    work->unk18.vx -= (ex->math_sin(t >> 1) * 0x4B0) >> 12;
                } else if (work->unk20 == 2) {
                    work->unk18.vx += (ex->math_sin(t >> 1) * 0x4B0) >> 12;
                }
                work->unk18.vy -= (ex->math_sin(t >> 1) * 0x258) >> 12;
                if (work->unk0 != NULL && work->unk0->unk30 == -1) {
                    work->unk0 = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->unk0, (s32)&work->unk18);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_039/nonmatchings/fx_039", fx_039_headerPtr);
