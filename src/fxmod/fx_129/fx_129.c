#include "fxmod/fx_129.h"

s32 fx_129_op80(s32 mode, Fx129Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s16 tex[4];
    s16 clut[4];
    s32 frame;
    s32 t;
    s32 bounce;

    if (mode == 0) {
        *args = 0x7C;
    } else if (mode == 1) {
        fx_129_env = env;
        ex->btlfx_initEffModels(fx_129_effModels, 1);
        tex[0] = tex[2] = 0xF7;
        tex[1] = 0x3CC0;
        tex[3] = 0x3C80;
        clut[0] = clut[2] = 0;
        clut[1] = 0x80;
        clut[3] = 0;
        /* passes three arguments; the fourth parameter keeps whatever is in $a3 */
        work->model0 = (u16)((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, FX_PUMPKIN_HEAD_PUMPKIN), tex, clut);
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos2C);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot14);
        work->rot14.vy += 0x800;
        env->camera->points[0].vx = work->pos4.vx = work->pos2C.vx;
        env->camera->points[0].vz = work->pos4.vz = work->pos2C.vz;
        env->camera->points[0].vy = work->pos4.vy = work->pos2C.vy;
        work->scale1C.vz = 0x330;
        work->scale1C.vy = 0x330;
        work->scale1C.vx = 0x330;
        work->phase34 = 0;
        work->speed38 = 0;
        work->height3C = -0x1B00;
        work->frame44 = 0;
        work->fade4C = 0;
        work->timer54 = 0;
        work->count58 = 0;
        work->unk5C = 0;
        work->timer60 = 0;
        ex->btlfx_getUnitScale(0, (VECTOR *)&work->scale64);
        work->scale70.vx = work->scale64.vx;
        work->scale70.vz = work->scale64.vz;
    } else {
        frame = entry->frame;
        if (work->phase34 < 9) {
            t = ++work->frame44;
            switch (work->phase34 & 1) {
            case 0:
                work->height40 = work->height3C + (((t * 0x60) * t) >> 1);
                if (work->height40 >= 0) {
                    work->speed38 = work->frame44 * 0x180 / 10;
                    work->frame44 = 0;
                    work->height40 = 0;
                    work->timer54 = 4;
                    work->phase34++;
                    work->scale1C.vy = 0x330 - work->speed38;
                    switch (work->count58) {
                    case 0:
                        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PUMPKIN_HEAD_SMOKE_BURST), (s32)&work->pos2C);
                        break;
                    case 1:
                    case 2:
                        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PUMPKIN_HEAD_SMOKE_BURST_2), (s32)&work->pos2C);
                        break;
                    }
                    work->count58++;
                }
                break;
            case 1:
                bounce = -(work->speed38 * t) + (((t * 0x60) * t) >> 1);
                if (bounce < work->height40) {
                    work->height40 = bounce;
                } else {
                    work->height3C = work->height40;
                    work->frame44 = 0;
                    work->phase34++;
                }
                break;
            }
            if (work->timer54 != 0) {
                work->timer54--;
                bounce = (ex->math_cos(work->timer54 << 9) * (work->speed38 >> 2)) >> 12;
                work->scale1C.vy = 0x330 - ((bounce >= 0) ? bounce : -bounce);
            }
        }
        if (frame == 0xB) {
            work->timer60 = 0x10;
        }
        if (work->timer60 != 0) {
            work->timer60--;
            if (work->timer60 != 0) {
                work->scale70.vy = work->scale64.vy + ((ex->math_cos((work->timer60 << 10) | 0x200) * (work->timer60 << 5)) >> 12);
                if (work->scale70.vy < 0) {
                    work->scale70.vy = 0;
                }
            } else {
                work->scale70.vy = work->scale64.vy;
            }
            ex->btlfx_setUnitScale(0, (s32 *)&work->scale70);
        }
        env->camera->points[0].vy = work->pos4.vy = work->pos2C.vy + work->height40;
        ex->Hi_DrawEffModel(&work->rot14, &work->pos4, (s32)&work->scale1C, work->model0);
        if (work->phase34 >= 9) {
            if (work->fade4C < 0x10) {
                work->fade4C++;
            }
            work->color50 = -(work->fade4C * 16);
            ex->Hi_ModifyEffModelAbr(work->model0, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model0, work->color50, work->color50, work->color50);
        }
        return frame >= 0x40;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_129/nonmatchings/fx_129", fx_129_headerPtr);
