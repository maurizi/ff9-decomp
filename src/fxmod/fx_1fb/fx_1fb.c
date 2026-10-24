#include "fxmod/fx_1fb.h"

s32 fx_1fb_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E7B7C.vx = 0;
        D_801E7B7C.vy = 0;
        D_801E7B7C.vz = -0x618;
        D_801E7B74 = env;
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&D_801E7B7C);
    } else {
        return entry->frame >= 0x40;
    }
    return 0;
}

s32 fx_1fb_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E7B84[0] = 0;
        D_801E7B84[1] = 0;
        D_801E7B74 = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801E7B94[i]);
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&D_801E7B94[i]);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(i, 0x200);
                p->unk12 = 0x1E;
                p->unk14 = (s32)D_801E7BB4[i];
                p->unkC = 0x9C;
            }
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 16:
            for (i = 0; i < env->targets->targetCount; i++) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&D_801E7B94[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x200);
                    p->unk12 = 0x1E;
                    p->unk14 = (s32)D_801E9B94[i];
                    p->unkC = 0x9C;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&D_801E7B94[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x200);
                }
            }
            break;
        case 24:
            D_801E7B84[0] = 0x10;
            break;
        case 40:
            D_801E7B84[1] = 0x10;
            break;
        }
        if (D_801E7B84[0] != 0) {
            D_801E7B84[0]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_fadeUnitById(i, D_801E7B84[0] * 8);
            }
        }
        if (D_801E7B84[1] != 0) {
            D_801E7B84[1]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_fadeUnitById(i, 0x80 - D_801E7B84[1] * 8);
            }
        }
        return frame >= 0x38;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1fb/nonmatchings/fx_1fb", fx_1fb_headerPtr);
