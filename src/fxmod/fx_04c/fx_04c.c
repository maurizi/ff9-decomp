#include "fxmod/fx_04c.h"

s32 fx_04c_op80(s32 mode, Fx04cWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7CB4 = env;
        ((s32 *)&work->unk4)[0] = ((s32 *)&env->unkC->unk38)[0];
        t = ((s32 *)&env->unkC->unk38)[1];
        work->unk4.vy = 0;
        ((s32 *)&work->unk4)[1] = t;
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = -0x12C;
        }
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = -0x258;
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            ex->func_800D3030((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 5), frame);
            if (frame >= 0x3C) {
                return 1;
            }
        }
    }
    return 0;
}

s32 fx_04c_op81(s32 mode, Fx04cWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        D_801E7CB4 = env;
        work->unk34 = env->unkC->unk24;
        ((u8 *)env->unk0)[0x12] = 0;
        for (i = 0; i < work->unk34; i++) {
            ex->btlfx_getUnitPos(i, (s16 *)&work->unk4[i]);
            work->unk24[i] = NULL;
        }
        work->unk0 = 0;
        work->unk38 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            work->unk24[work->unk38] =
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk4[work->unk38]);
            if (work->unk24[work->unk38] != NULL) {
                work->unk24[work->unk38]->unk22 = ex->func_800E7CC4(work->unk38, 0x40);
            }
            work->unk0 = 1;
            entry->unk0 = -1;
            break;
        case 1:
            if (frame > 0) {
                if (++work->unk38 < work->unk34) {
                    work->unk0 = 0;
                    entry->unk0 = -1;
                } else {
                    work->unk0 = 2;
                    entry->unk0 = -1;
                }
            }
            break;
        case 2:
            if (frame >= 0xA) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_04c_op82(s32 mode, Fx04cWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        D_801E7CB4 = env;
        work->unk34 = env->unkC->unk24;
        ((u8 *)env->unk0)[0x12] = 0;
        for (i = 0; i < work->unk34; i++) {
            ex->btlfx_getUnitPos(i, (s16 *)&work->unk4[i]);
            work->unk24[i] = NULL;
        }
        work->unk0 = 0;
        work->unk38 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            work->unk24[work->unk38] =
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk4[work->unk38]);
            if (work->unk24[work->unk38] != NULL) {
                work->unk24[work->unk38]->unk22 = ex->func_800E7CC4(work->unk38, 0x40);
            }
            work->unk0 = 1;
            entry->unk0 = -1;
            break;
        case 1:
            if (frame > 0) {
                if (++work->unk38 < work->unk34) {
                    work->unk0 = 0;
                    entry->unk0 = -1;
                } else {
                    work->unk0 = 2;
                    ((u8 *)env->unk0)[0x12] = 1;
                    entry->unk0 = -1;
                }
            }
            break;
        case 2:
            if (frame >= 0xA) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04c/nonmatchings/fx_04c", fx_04c_headerPtr);
