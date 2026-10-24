#include "fxmod/fx_046.h"

s32 fx_046_op80(s32 mode, Fx046Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7890 = env;
        ((s32 *)&work->unk4)[0] = ((s32 *)&env->targets->centerX)[0];
        t = ((s32 *)&env->targets->centerX)[1];
        work->unk4.vy = 0;
        ((s32 *)&work->unk4)[1] = t;
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0 && frame >= 0x3C) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_046/nonmatchings/fx_046", fx_046_headerPtr);
