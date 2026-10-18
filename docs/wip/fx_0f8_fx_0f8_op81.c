/* Draft of fx_0f8_op81 (module fx_0f8). Status: 141/186 aligned instructions with ccprobe; register allocation only.
 * Declarations it needs that are not yet in include/fxmod/fx_0f8.h are repeated below. */
#include "fxmod/fx_0f8.h"

s32 fx_0f8_op81(s32 mode, Fx0f8Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 i;
    Fx0f8Spawn *t;

    if (mode == 0) {
        *arg4 = 0x50;
    } else if (mode == 1) {
        work->unk28.vx = env->unkC->unk38;
        work->unk28.vy = env->unkC->unk3A;
        work->unk28.vz = env->unkC->unk3C;
        D_801E7BD0 = env;
        for (i = 0; i < 4; i++) {
            (work->unk30 + i)->vx = work->unk28.vx;
            (work->unk30 + i)->vy = work->unk28.vy;
            (work->unk30 + i)->vz = work->unk28.vz;
            D_801E7B78[i].unk2 = i * 4;
            rot.vx = 0;
            rot.vy = i << 10;
            rot.vz = 0;
            ex->func_800CFD78((s16 *)&work->unk28, (s16 *)&rot, ex->btlfx_rand() % 400 + 0x578, (s16 *)&work->unk30[i]);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0xF) {
            work->unk20 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk28);
            work->unk24 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk28);
        }
        for (i = 0; i < 4; i++) {
            if (frame == D_801E7B78[i].unk2) {
                work->unk0[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801E7B78[i].unk0), (s32)&work->unk30[i]);
                work->unk10[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk30[i]);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 1), frame);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 2), frame);
        return frame >= 0x2D;
    }
    return 0;
}
