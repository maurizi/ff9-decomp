#include "fxmod/fx_1ab.h"

INCLUDE_RODATA("asm/us/fx_1ab/nonmatchings/fx_1ab", fx_1ab_headerPtr);

s32 fx_1ab_op80(s32 mode, Fx1abWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x88;
    } else if (mode == 1) {
        D_801E7D80 = env;
        ex->btlfx_initEffModels(D_801E7D90, 2);
        work->unk70.vx = env->targets->centerX;
        work->unk70.vy = env->targets->centerY;
        work->unk70.vz = env->targets->centerZ;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        work->unk14.vx = work->unk70.vx;
        work->unk14.vy = work->unk70.vy;
        work->unk14.vz = work->unk70.vz;
        work->unk24.vx = 0x400;
        work->unk24.vy = 0x800;
        work->unk24.vz = 0x400;
        work->unk14.vy = 0x4B0;
        work->unk38.vx = 0;
        work->unk38.vy = 0;
        work->unk38.vz = 0;
        work->unk40.vx = work->unk70.vx;
        work->unk40.vy = work->unk70.vy;
        work->unk40.vz = work->unk70.vz;
        work->unk50.vx = 0;
        work->unk50.vy = 0;
        work->unk50.vz = 0;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->unk84 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 2), 1);
        work->unk86 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 1);
        work->unk80 = -1;
        work->unk82 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk70);
            break;
        case 8:
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk70);
            break;
        case 32:
            work->unk78.vx = work->unk70.vx;
            work->unk78.vy = work->unk70.vy;
            work->unk78.vz = work->unk70.vz;
            work->unk78.vy = -0x258;
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk78);
            break;
        case 15:
            work->unk80 = 3;
            break;
        case 47:
            work->unk82 = 0x10;
            break;
        }
        if (frame >= 0xF && frame < 0x3F) {
            t = frame - 0xF;
            ex->Hi_SetEffModelSlice(work->unk84, work->unk14.vy);
            ex->Hi_DrawSliceEffModel(&work->unkC, &work->unk14, (s32)&work->unk24, work->unk84);
            ex->Hi_DrawEffModel(&work->unk38, &work->unk40, (s32)&work->unk50, work->unk86);
            if (work->unk80 >= 0 || work->unk82 >= 0) {
                if (work->unk80 >= 0) {
                    work->unk34 = func_801E7CE0(-0x80, 0, 3 - work->unk80, 3);
                    work->unk80--;
                }
                if (work->unk82 >= 0) {
                    work->unk34 = func_801E7CE0(0, -0x80, 0x10 - work->unk82, 0x10);
                    work->unk82--;
                }
                ex->Hi_ModifyEffModelRGB(work->unk84, work->unk34, work->unk34, work->unk34);
                ex->Hi_ModifyEffModelRGB(work->unk86, work->unk34, work->unk34, work->unk34);
            }
            if (t < 8) {
                work->unk14.vy -= 0x145;
            } else {
                if (work->unk24.vy > 0x400) {
                    work->unk24.vy -= 0x40;
                }
                work->unk14.vy += 0x14;
            }
            work->unk50.vx += 0x40;
            work->unk50.vz += 0x40;
            if (t >= 8) {
                work->unk50.vy -= 8;
            } else {
                work->unk50.vy += 0xAA;
            }
            work->unk50.vy = work->unk50.vy > 0x400 ? 0x400 : work->unk50.vy;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 5), frame);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 7), frame);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 6), frame);
        if (frame >= 0x64) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E7C9C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7D80)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7CE0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
