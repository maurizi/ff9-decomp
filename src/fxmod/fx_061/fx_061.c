#include "fxmod/fx_061.h"

s32 fx_061_op80(s32 mode, Fx061Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7914 = env;
        ((s32 *)&work->unk4)[0] = ((s32 *)&env->targets->unk38)[0];
        t = ((s32 *)&env->targets->unk38)[1];
        work->unk4.vy -= 0x180;
        ((s32 *)&work->unk4)[1] = t;
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = 0x20;
            p->unk14 = (s32)D_801E7EF4;
            p->unk12 = 0x13;
            p->unkC = 0x6C;
        }
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = 0x20;
            p->unk14 = (s32)D_801E7924;
            p->unk12 = 0xC;
            p->unkC = 0x7C;
        }
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = 0x20;
        }
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = 0x20;
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0 && frame >= 0x31) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_061/nonmatchings/fx_061", fx_061_headerPtr);
