#include "fxmod/fx_0f2.h"

s32 fx_0f2_op80(s32 mode, Fx0f2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x5C;
    } else if (mode == 1) {
        fx_0f2_env = env;
        ex->btlfx_initEffModels(fx_0f2_effModels, 1);
        work->pos38.vx = env->targets->centerX;
        work->pos38.vy = env->targets->centerY;
        work->pos38.vz = env->targets->centerZ;
        if (ex->btlfx_isEnemyUnit(0)) {
            work->unk50 = 1;
        } else {
            work->unk50 = 0;
        }
        switch (work->unk50) {
        case 1:
            work->unk34 = 0xC8;
            work->rot8.vx = 0;
            work->rot8.vy = 0;
            work->rot8.vz = 0;
            break;
        case 0:
            work->unk34 = -0xC8;
            work->rot8.vx = 0;
            work->rot8.vy = 0x800;
            work->rot8.vz = 0;
            break;
        }
        work->pos10.vz = 0x1F4;
        work->pos10.vx = 0;
        work->pos10.vy = 0;
        work->scale20.vx = 0xAAA;
        work->scale20.vy = 0xAAA;
        work->scale20.vz = 0xAAA;
        work->model58 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 2), 0x11A, 0, 0, 0x80);
        work->unk40.vx = work->pos10.vx;
        work->unk40.vy = work->pos10.vy;
        work->unk40.vz = work->pos10.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos38);
        work->unk54 = -1;
        work->unk56 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x11:
            work->pos48.vx = work->pos10.vx;
            work->pos48.vy = work->pos10.vy;
            work->pos48.vz = work->pos10.vz;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos48);
            work->unk54 = 3;
            break;
        case 0x15:
            work->unk56 = 0x10;
            break;
        }
        if (frame >= 0x11 && frame < 0x25) {
            t = frame - 0x11;
            ex->Hi_DrawEffModel(&work->rot8, &work->pos10, (s32)&work->scale20, work->model58);
            ex->Hi_ModifyEffModelAbr(work->model58, 1);
            if (work->unk54 >= 0 || work->unk56 >= 0) {
                if (work->unk54 >= 0) {
                    work->unk30 = fxm_lerpRatio(-0x80, 0, 3 - work->unk54, 3);
                    work->unk54--;
                }
                if (work->unk56 >= 0) {
                    work->unk30 = fxm_lerpRatio(0, -0xC0, 0x10 - work->unk56, 0x10);
                    work->unk56--;
                }
                ex->Hi_ModifyEffModelRGB(work->model58, work->unk30, work->unk30, work->unk30);
            }
            work->pos10.vz += work->unk34;
            work->scale20.vz += 0x100;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 3), t);
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0f2_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0f2/nonmatchings/fx_0f2", fx_0f2_headerPtr);
