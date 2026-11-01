#include "fxmod/fx_115.h"

s32 fx_115_op80(s32 mode, Fx115Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 t;
    s32 i;
    s32 c;

    if (mode == 0) {
        *args = 0xA4;
    } else if (mode == 1) {
        fx_115_env = env;
        ex->btlfx_initEffModels(fx_115_effModels, 2);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&work->pos10);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos8, (s16 *)&work->rot18);
        work->scale30.vx = 0x100;
        work->scale30.vy = 0x100;
        work->scale30.vz = 0x100;
        work->rot18.vx += 0x800;
        work->pos20.vx = work->pos10.vx;
        work->pos20.vy = work->pos10.vy;
        work->pos20.vz = work->pos10.vz;
        work->modelA0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 5), 1);
        work->modelA2 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 5), 1);
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
        ex->Hi_DrawEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->modelA0);
        for (i = 0; i < 4; i++) {
            if (frame >= i + 1) {
                c = -(i * 20);
                ex->Hi_DrawEffModel(&work->rot40[i], &work->pos60[i], (s32)&work->scale30, work->modelA2);
                ex->Hi_ModifyEffModelAbr(work->modelA2, 3);
                ex->Hi_ModifyEffModelRGB(work->modelA2, c, c, c);
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
        if (frame >= 6) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_115_op81(s32 mode, Fx115Work80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        fx_115_env = env;
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

INCLUDE_RODATA("asm/us/fx_115/nonmatchings/fx_115", fx_115_headerPtr);
