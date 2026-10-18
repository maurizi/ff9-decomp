#include "fxmod/fx_047.h"

s32 fx_047_op80(s32 mode, Fx047Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E796C = env;
        ((s32 *)&work->unkC)[0] = ((s32 *)&env->unkC->unk38)[0];
        t = ((s32 *)&env->unkC->unk38)[1];
        work->unkC.vy = 0;
        ((s32 *)&work->unkC)[1] = t;
        ((s32 *)&work->unk4)[0] = ((s32 *)&work->unkC)[0];
        ((s32 *)&work->unk4)[1] = ((s32 *)&work->unkC)[1];
        work->unk4.vy -= 0x12C;
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = 0x80;
        }
        work->unk14 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unkC);
        if (work->unk14 != NULL) {
            work->unk14->unk22 = 0x80;
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            if (frame == 5) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame == 0x11) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = 0x80;
                    p->unk14 = (s32)D_801E797C;
                    p->unk12 = 0x1D;
                    p->unkC = 0xAC;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame >= 0x32) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_047/nonmatchings/fx_047", fx_047_headerPtr);
