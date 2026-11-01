#include "fxmod/fx_10c.h"

s32 fx_10c_op80(s32 mode, Fx10cWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 t;
    s32 i;
    s32 rgb;

    if (mode == 0) {
        *args = 0x74;
    } else if (mode == 1) {
        fx_10c_env = env;
        ex->btlfx_initEffModels(fx_10c_effModels, 2);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&work->pos10);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos8, (s16 *)&work->rot18);
        work->scale30.vx = 0x1000;
        work->scale30.vy = 0x1000;
        work->scale30.vz = 0x1000;
        work->rot18.vx += 0x800;
        work->pos20.vx = work->pos10.vx;
        work->pos20.vy = work->pos10.vy;
        work->pos20.vz = work->pos10.vz;
        work->model70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 5), 1);
        work->model72 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xC), 0x97, 0x3D40, 0, 0);
    } else {
        frame = entry->frame;
        if (frame < 8) {
            t = (frame << 12) / 6;
            ex->btlfx_lerpVec((s16 *)&work->pos0, (s16 *)&work->pos8, t, (s16 *)&pos);
            work->pos20.vx = pos.vx;
            work->pos20.vy = pos.vy;
            work->pos20.vz = pos.vz;
            ex->math_sin(t >> 1);
        }
        ex->Hi_DrawEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model70);
        for (i = 0; i < 2; i++) {
            if (frame >= i + 1) {
                rgb = -(i * 16);
                ex->Hi_DrawEffModel(&work->rot40[i], &work->pos50[i], (s32)&work->scale30, work->model72);
                ex->Hi_ModifyEffModelAbr(work->model72, 3);
                ex->Hi_ModifyEffModelRGB(work->model72, rgb, rgb, rgb);
            }
        }
        work->rot40[1].vx = work->rot40[0].vx;
        work->rot40[1].vy = work->rot40[0].vy;
        work->rot40[1].vz = work->rot40[0].vz;
        work->pos50[1].vx = work->pos50[0].vx;
        work->pos50[1].vy = work->pos50[0].vy;
        work->pos50[1].vz = work->pos50[0].vz;
        work->rot40[0].vx = work->rot18.vx;
        work->rot40[0].vy = work->rot18.vy;
        work->rot40[0].vz = work->rot18.vz;
        work->pos50[0].vx = work->pos20.vx;
        work->pos50[0].vy = work->pos20.vy;
        work->pos50[0].vz = work->pos20.vz;
        if (frame >= 6) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_10c_op81(s32 mode, Fx10cWork80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        fx_10c_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_10c/nonmatchings/fx_10c", fx_10c_headerPtr);
