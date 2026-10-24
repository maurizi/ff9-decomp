#include "fxmod/fx_0f6.h"

s32 fx_0f6_op80(s32 mode, Fx0f6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    Fx0f6Layer *p;
    Fx0f6Layer *q;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->unk8.vx = env->targets->centerX;
        work->unk8.vy = env->targets->centerY;
        work->unk8.vz = env->targets->centerZ;
        work->unk10.vx = env->targets->centerX;
        work->unk10.vy = env->targets->centerY;
        work->unk10.vz = env->targets->centerZ;
        D_801E79FC = env;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = 0x100;
        }
    } else {
        frame = entry->unk0;
        q = D_801E7984;
        p = D_801E7984;
        for (i = 0; i < 4; i++, p++, q++) {
            if (p->unk2 == 0 && frame >= p->unk4 && frame < p->unk6) {
                ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, q->unk0), &work->unk10, p->unk8,
                                  p->unkA);
                p->unkA -= 0x1000 / (p->unk6 - p->unk4);
                if (frame == p->unk6 - 1) {
                    p->unk2 = 1;
                }
                p->unk8++;
            }
        }
        if (frame == 0x14) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk8);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = 0x100;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f6/nonmatchings/fx_0f6", fx_0f6_headerPtr);
