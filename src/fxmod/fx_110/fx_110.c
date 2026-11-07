#include "fxmod/fx_110.h"

s32 fx_110_op80(s32 mode, Fx110Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 t;
    s32 i;
    s32 rgb;

    if (mode == 0) {
        *args = sizeof(Fx110Work80);
    } else if (mode == 1) {
        fx_110_env = env;
        ex->btlfx_initEffModels(fx_110_effModels, FX_THROW_MODEL_SLOTS);
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
        work->modelA0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_THROW_DAGGER), 1);
        work->modelA2 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_THROW_DAGGER_AFTERIMAGE), FX_THROW_AFTERIMAGE_TPAGE, FX_THROW_AFTERIMAGE_CLUT, FX_THROW_AFTERIMAGE_U, FX_THROW_AFTERIMAGE_V);
    } else {
        frame = entry->frame;
        if (frame < FX_THROW_PATH_FRAMES) {
            t = frame << 9;
            ex->btlfx_lerpVec((s16 *)&work->pos0, (s16 *)&work->pos8, t, (s16 *)&pos);
            work->pos20.vx = pos.vx;
            work->pos20.vy = pos.vy;
            work->pos20.vz = pos.vz;
            ex->math_sin(t >> 1);
        }
        ex->Hi_DrawEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->modelA0);
        for (i = 0; i < FX_THROW_AFTERIMAGES; i++) {
            if (frame >= i + 1) {
                rgb = -(i * FX_THROW_AFTERIMAGE_DIM);
                ex->Hi_DrawEffModel(&work->rot40[i], &work->pos60[i], (s32)&work->scale30, work->modelA2);
                ex->Hi_ModifyEffModelAbr(work->modelA2, BTLFX_ABR_QUARTER);
                ex->Hi_ModifyEffModelRGB(work->modelA2, rgb, rgb, rgb);
            }
        }
        work->rot40[3].vx = work->rot40[2].vx;
        work->rot40[3].vy = work->rot40[2].vy;
        work->rot40[3].vz = work->rot40[2].vz;
        work->pos60[3].vx = work->pos60[2].vx;
        work->pos60[3].vy = work->pos60[2].vy;
        work->pos60[3].vz = work->pos60[2].vz;
        work->rot40[2].vx = work->rot40[1].vx;
        work->rot40[2].vy = work->rot40[1].vy;
        work->rot40[2].vz = work->rot40[1].vz;
        work->pos60[2].vx = work->pos60[1].vx;
        work->pos60[2].vy = work->pos60[1].vy;
        work->pos60[2].vz = work->pos60[1].vz;
        work->rot40[1].vx = work->rot40[0].vx;
        work->rot40[1].vy = work->rot40[0].vy;
        work->rot40[1].vz = work->rot40[0].vz;
        work->pos60[1].vx = work->pos60[0].vx;
        work->pos60[1].vy = work->pos60[0].vy;
        work->pos60[1].vz = work->pos60[0].vz;
        work->rot40[0].vx = work->rot18.vx;
        work->rot40[0].vy = work->rot18.vy;
        work->rot40[0].vz = work->rot18.vz;
        work->pos60[0].vx = work->pos20.vx;
        work->pos60[0].vy = work->pos20.vy;
        work->pos60[0].vz = work->pos20.vz;
        if (frame >= FX_THROW_FLIGHT_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_110_op81(s32 mode, Fx110Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx110Work81);
    } else if (mode == 1) {
        fx_110_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THROW_IMPACT), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= FX_THROW_IMPACT_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_110/nonmatchings/fx_110", fx_110_headerPtr);
