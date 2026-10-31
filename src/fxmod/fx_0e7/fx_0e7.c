#include "fxmod/fx_0e7.h"

s32 fx_0e7_op80(s32 mode, Fx0e7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u16 n;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_0e7_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 1, (s16 *)&work->pos4);
        work->unkC.vx = work->pos4.vx;
        work->unkC.vy = work->pos4.vy;
        work->unkC.vz = work->pos4.vz;
        work->unkC.vy = -0x9C4;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos4);
        work->unk16 = 0x1000;
        work->unk14 = 0;
    } else {
        frame = entry->frame;
        if (frame >= 0x23 && frame < 0x30) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 3), &work->unkC, work->unk14, work->unk16);
            if (frame - 0x23 >= 5) {
                work->unk16 -= 0x200;
            }
            work->unk14++;
            n = work->unk14;
            if (n >= 6) {
                n = 5;
            }
            work->unk14 = n;
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e7/nonmatchings/fx_0e7", fx_0e7_headerPtr);

s32 fx_0e7_op81(s32 mode, Fx0e7Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x7C;
    } else if (mode == 1) {
        fx_0e7_env = env;
        ex->btlfx_initEffModels(fx_0e7_effModels, 2);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk40);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot48);
        ex->btlfx_pointAhead((s16 *)&work->unk40, (s16 *)&work->rot48, ex->btlfx_getUnitRadius(0) + 0x190, (s16 *)&pos);
        work->rotC.vx = 0;
        work->rotC.vz = 0;
        work->rot14.vx = 0;
        work->rot14.vz = 0;
        work->rotC.vy = work->rot48.vy;
        work->rot14.vy = work->rot48.vy;
        work->pos1C.vx = pos.vx;
        work->pos1C.vy = pos.vy;
        work->pos1C.vz = pos.vz;
        work->scale2C.vx = 0x2000;
        work->scale2C.vy = 0x2000;
        work->scale2C.vz = 0x2000;
        /* passes three arguments; the fourth parameter keeps whatever is in $a3 */
        work->unk78 = ((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, 2), D_801E7EFC,
                                                               D_801E7F04);
        work->unk7A = ((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, 1), D_801E7EFC,
                                                               D_801E7F04);
        work->unk68 = -1;
        work->unk6C = -1;
        work->unk70 = -1;
        work->unk74 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->unk70 = 6;
            break;
        case 4:
            work->pos50.vx = work->pos1C.vx;
            work->pos50.vy = work->pos1C.vy;
            work->pos50.vz = work->pos1C.vz;
            work->pos50.vy = 0;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos50);
            break;
        case 8:
            work->unk74 = 8;
            work->pos58.vx = work->pos1C.vx;
            work->pos58.vy = work->pos1C.vy;
            work->pos58.vz = work->pos1C.vz;
            work->pos58.vy = -0xC8;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos58);
            if (work->emit4 != NULL) {
                work->emit4->otZ = 0x80;
            }
            break;
        case 0x32:
            work->pos60.vx = work->unk40.vx;
            work->pos60.vy = work->unk40.vy;
            work->pos60.vz = work->unk40.vz;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos60);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 0x10:
            work->unk6C = 0x10;
            break;
        }
        if (frame >= 0 && frame < 0x20) {
            if (frame < 0) {
                work->pos1C.vy = -0xBB8;
            } else if (work->unk70 >= 0) {
                work->pos1C.vy = fxm_lerpRatio(-0xBB8, -0x96, 6 - work->unk70, 6);
                work->unk70--;
            } else {
                work->pos1C.vy = -0x96;
            }
            if (frame < 8) {
                work->rotC.vx = 0x400;
            } else if (work->unk74 >= 0) {
                work->rotC.vx = fxm_lerpRatio(0x400, 0, 8 - work->unk74, 8);
                work->unk74--;
            } else {
                work->rotC.vx = 0;
            }
            ex->Hi_DrawEffModel(&work->rotC, &work->pos1C, (s32)&work->scale2C, work->unk78);
            ex->Hi_DrawEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, work->unk7A);
            env->camera->points[0].vx = work->pos1C.vx;
            env->camera->points[0].vy = work->pos1C.vy;
            env->camera->points[0].vz = work->pos1C.vz;
            if (work->unk68 >= 0 || work->unk6C >= 0) {
                if (work->unk6C >= 0) {
                    work->unk3C = fxm_lerpRatio(0, -0x80, 0x10 - work->unk6C, 0x10);
                    work->unk6C--;
                }
                ex->Hi_ModifyEffModelAbr(work->unk7A, 1);
                ex->Hi_ModifyEffModelAbr(work->unk78, 1);
                ex->Hi_ModifyEffModelRGB(work->unk7A, work->unk3C, work->unk3C, work->unk3C);
                ex->Hi_ModifyEffModelRGB(work->unk78, work->unk3C, work->unk3C, work->unk3C);
            }
        }
        if (frame >= 0x5A) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0e7_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
