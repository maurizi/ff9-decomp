#include "fxmod/fx_12f.h"

s32 fx_12f_op80(s32 mode, Fx12fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x44;
    } else if (mode == 1) {
        D_801E7A54 = env;
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk8);
        work->unk18.vx = env->targets->centerX;
        work->unk18.vz = env->targets->centerZ;
        work->unk18.vy = 0;
        work->unk40 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk18, (s16 *)&work->unk0);
    } else {
        frame = entry->unk0;
        if (work->unk40 != NULL) {
            if (work->unk40->unk30 == -1) {
                work->unk40 = NULL;
            }
        }
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk10);
        ex->btlfx_moveEmitterTo((s32)work->unk40, (s32)&work->unk10);
        if (work->unk40 != NULL) {
            work->unk40->unk5C.vx = work->unk0.vx;
            work->unk40->unk5C.vy = work->unk0.vy;
            work->unk40->unk5C.vz = work->unk0.vz;
        }
        if (frame == 0x10) {
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk18);
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitPos(i, (s16 *)&work->unk20[i]);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk20[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x80);
                }
            }
        }
        if (frame == 0x14) {
            for (i = 0; i < env->targets->targetCount; i++) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk20[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x80);
                }
            }
        }
        return frame >= 0x40;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_12f/nonmatchings/fx_12f", fx_12f_headerPtr);
