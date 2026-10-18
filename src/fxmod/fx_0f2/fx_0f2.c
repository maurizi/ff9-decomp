#include "fxmod/fx_0f2.h"

s32 fx_0f2_op80(s32 mode, Fx0f2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x5C;
    } else if (mode == 1) {
        D_801E7B80 = env;
        ex->btlfx_initEffModels(D_801E7B90, 1);
        work->unk38.vx = env->unkC->unk38;
        work->unk38.vy = env->unkC->unk3A;
        work->unk38.vz = env->unkC->unk3C;
        if (ex->btlfx_isEnemyUnit(0)) {
            work->unk50 = 1;
        } else {
            work->unk50 = 0;
        }
        switch (work->unk50) {
        case 1:
            work->unk34 = 0xC8;
            work->unk8.vx = 0;
            work->unk8.vy = 0;
            work->unk8.vz = 0;
            break;
        case 0:
            work->unk34 = -0xC8;
            work->unk8.vx = 0;
            work->unk8.vy = 0x800;
            work->unk8.vz = 0;
            break;
        }
        work->unk10.vz = 0x1F4;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk20.vx = 0xAAA;
        work->unk20.vy = 0xAAA;
        work->unk20.vz = 0xAAA;
        work->unk58 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0x11A, 0, 0, 0x80);
        work->unk40.vx = work->unk10.vx;
        work->unk40.vy = work->unk10.vy;
        work->unk40.vz = work->unk10.vz;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk38);
        work->unk54 = -1;
        work->unk56 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x11:
            work->unk48.vx = work->unk10.vx;
            work->unk48.vy = work->unk10.vy;
            work->unk48.vz = work->unk10.vz;
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk48);
            work->unk54 = 3;
            break;
        case 0x15:
            work->unk56 = 0x10;
            break;
        }
        if (frame >= 0x11 && frame < 0x25) {
            t = frame - 0x11;
            ex->Hi_DrawEffModel(&work->unk8, &work->unk10, (s32)&work->unk20, work->unk58);
            ex->Hi_ModifyEffModelAbr(work->unk58, 1);
            if (work->unk54 >= 0 || work->unk56 >= 0) {
                if (work->unk54 >= 0) {
                    work->unk30 = func_801E7AE0(-0x80, 0, 3 - work->unk54, 3);
                    work->unk54--;
                }
                if (work->unk56 >= 0) {
                    work->unk30 = func_801E7AE0(0, -0xC0, 0x10 - work->unk56, 0x10);
                    work->unk56--;
                }
                ex->Hi_ModifyEffModelRGB(work->unk58, work->unk30, work->unk30, work->unk30);
            }
            work->unk10.vz += work->unk34;
            work->unk20.vz += 0x100;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 3), t);
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E7A9C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7B80)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7AE0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0f2/nonmatchings/fx_0f2", fx_0f2_headerPtr);
