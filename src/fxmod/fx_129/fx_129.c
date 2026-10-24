#include "fxmod/fx_129.h"

s32 fx_129_op80(s32 mode, Fx129Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s16 tex[4];
    s16 clut[4];
    s32 frame;
    s32 t;
    s32 v;

    if (mode == 0) {
        *arg4 = 0x7C;
    } else if (mode == 1) {
        D_801E7C04 = env;
        ex->btlfx_initEffModels(D_801E7C14, 1);
        tex[0] = tex[2] = 0xF7;
        tex[1] = 0x3CC0;
        tex[3] = 0x3C80;
        clut[0] = clut[2] = 0;
        clut[1] = 0x80;
        clut[3] = 0;
        /* passes three arguments; the fourth parameter keeps whatever is in $a3 */
        work->unk0 = (u16)((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, 7), tex, clut);
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unk2C);
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk14);
        work->unk14.vy += 0x800;
        env->camera->points[0].vx = work->unk4.vx = work->unk2C.vx;
        env->camera->points[0].vz = work->unk4.vz = work->unk2C.vz;
        env->camera->points[0].vy = work->unk4.vy = work->unk2C.vy;
        work->unk1C.vz = 0x330;
        work->unk1C.vy = 0x330;
        work->unk1C.vx = 0x330;
        work->unk34 = 0;
        work->unk38 = 0;
        work->unk3C = -0x1B00;
        work->unk44 = 0;
        work->unk4C = 0;
        work->unk54 = 0;
        work->unk58 = 0;
        work->unk5C = 0;
        work->unk60 = 0;
        ex->btlfx_getUnitScale(0, (VECTOR *)&work->unk64);
        work->unk70.vx = work->unk64.vx;
        work->unk70.vz = work->unk64.vz;
    } else {
        frame = entry->frame;
        if (work->unk34 < 9) {
            t = ++work->unk44;
            switch (work->unk34 & 1) {
            case 0:
                work->unk40 = work->unk3C + (((t * 0x60) * t) >> 1);
                if (work->unk40 >= 0) {
                    work->unk38 = work->unk44 * 0x180 / 10;
                    work->unk44 = 0;
                    work->unk40 = 0;
                    work->unk54 = 4;
                    work->unk34++;
                    work->unk1C.vy = 0x330 - work->unk38;
                    switch (work->unk58) {
                    case 0:
                        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk2C);
                        break;
                    case 1:
                    case 2:
                        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk2C);
                        break;
                    }
                    work->unk58++;
                }
                break;
            case 1:
                v = -(work->unk38 * t) + (((t * 0x60) * t) >> 1);
                if (v < work->unk40) {
                    work->unk40 = v;
                } else {
                    work->unk3C = work->unk40;
                    work->unk44 = 0;
                    work->unk34++;
                }
                break;
            }
            if (work->unk54 != 0) {
                work->unk54--;
                v = (ex->math_cos(work->unk54 << 9) * (work->unk38 >> 2)) >> 12;
                work->unk1C.vy = 0x330 - ((v >= 0) ? v : -v);
            }
        }
        if (frame == 0xB) {
            work->unk60 = 0x10;
        }
        if (work->unk60 != 0) {
            work->unk60--;
            if (work->unk60 != 0) {
                work->unk70.vy = work->unk64.vy + ((ex->math_cos((work->unk60 << 10) | 0x200) * (work->unk60 << 5)) >> 12);
                if (work->unk70.vy < 0) {
                    work->unk70.vy = 0;
                }
            } else {
                work->unk70.vy = work->unk64.vy;
            }
            ex->btlfx_setUnitScale(0, (s32 *)&work->unk70);
        }
        env->camera->points[0].vy = work->unk4.vy = work->unk2C.vy + work->unk40;
        ex->Hi_DrawEffModel(&work->unk14, &work->unk4, (s32)&work->unk1C, work->unk0);
        if (work->unk34 >= 9) {
            if (work->unk4C < 0x10) {
                work->unk4C++;
            }
            work->unk50 = -(work->unk4C * 16);
            ex->Hi_ModifyEffModelAbr(work->unk0, 1);
            ex->Hi_ModifyEffModelRGB(work->unk0, work->unk50, work->unk50, work->unk50);
        }
        return frame >= 0x40;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_129/nonmatchings/fx_129", fx_129_headerPtr);
