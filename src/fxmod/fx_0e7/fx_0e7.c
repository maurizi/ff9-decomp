#include "fxmod/fx_0e7.h"

s32 fx_0e7_op80(s32 mode, Fx0e7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u16 lineFrame;

    if (mode == 0) {
        *args = sizeof(Fx0e7Work80);
    } else if (mode == 1) {
        fx_0e7_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 1, (s16 *)&work->pos4);
        work->linePosC.vx = work->pos4.vx;
        work->linePosC.vy = work->pos4.vy;
        work->linePosC.vz = work->pos4.vz;
        work->linePosC.vy = -FX_THIEVERY_FLARES_HEIGHT;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THIEVERY_GOLD_GLITTER), (s32)&work->pos4);
        work->lineAlpha16 = 0x1000;
        work->lineFrame14 = 0;
    } else {
        frame = entry->frame;
        if (frame >= FX_THIEVERY_FLARES_FRAME && frame < FX_THIEVERY_FLARES_END_FRAME) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_THIEVERY_LENS_FLARES), &work->linePosC, work->lineFrame14, work->lineAlpha16);
            if (frame - FX_THIEVERY_FLARES_FRAME >= FX_THIEVERY_FLARES_FADE_FRAME) {
                work->lineAlpha16 -= 0x200;
            }
            work->lineFrame14++;
            lineFrame = work->lineFrame14;
            if (lineFrame >= FX_THIEVERY_FLARES_ANIM_FRAMES) {
                lineFrame = FX_THIEVERY_FLARES_ANIM_FRAMES - 1;
            }
            work->lineFrame14 = lineFrame;
        }
        return frame >= FX_THIEVERY_FLARES_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e7/nonmatchings/fx_0e7", fx_0e7_headerPtr);

s32 fx_0e7_op81(s32 mode, Fx0e7Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0e7Work81);
    } else if (mode == 1) {
        fx_0e7_env = env;
        ex->btlfx_initEffModels(fx_0e7_effModels, 2);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos40);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot48);
        ex->btlfx_pointAhead((s16 *)&work->pos40, (s16 *)&work->rot48, ex->btlfx_getUnitRadius(0) + FX_THIEVERY_DISTANCE, (s16 *)&pos);
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
        work->model78 = ((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, FX_THIEVERY_CHEST_BOX), fx_0e7_texPages,
                                                               fx_0e7_texPos);
        work->model7A = ((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, FX_THIEVERY_CHEST_LID), fx_0e7_texPages,
                                                               fx_0e7_texPos);
        work->blend68 = -1;
        work->fadeTimer6C = -1;
        work->dropTimer70 = -1;
        work->tiltTimer74 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->dropTimer70 = FX_THIEVERY_DROP_FRAMES;
            break;
        case FX_THIEVERY_PUFF_FRAME:
            work->pos50.vx = work->pos1C.vx;
            work->pos50.vy = work->pos1C.vy;
            work->pos50.vz = work->pos1C.vz;
            work->pos50.vy = 0;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THIEVERY_WHITE_PUFF), (s32)&work->pos50);
            break;
        case FX_THIEVERY_TILT_FRAME:
            work->tiltTimer74 = FX_THIEVERY_TILT_FRAMES;
            work->pos58.vx = work->pos1C.vx;
            work->pos58.vy = work->pos1C.vy;
            work->pos58.vz = work->pos1C.vz;
            work->pos58.vy = -FX_THIEVERY_RINGS_HEIGHT;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THIEVERY_GOLD_RINGS), (s32)&work->pos58);
            if (work->emit4 != NULL) {
                work->emit4->otZ = 0x80;
            }
            break;
        case FX_THIEVERY_BURST_FRAME:
            work->pos60.vx = work->pos40.vx;
            work->pos60.vy = work->pos40.vy;
            work->pos60.vz = work->pos40.vz;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THIEVERY_GOLD_BURST), (s32)&work->pos60);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_THIEVERY_FADE_FRAME:
            work->fadeTimer6C = FX_THIEVERY_FADE_FRAMES;
            break;
        }
        if (frame >= 0 && frame < FX_THIEVERY_DRAW_FRAMES) {
            if (frame < 0) {
                work->pos1C.vy = -FX_THIEVERY_DROP_HEIGHT;
            } else if (work->dropTimer70 >= 0) {
                work->pos1C.vy = fxm_lerpRatio(-FX_THIEVERY_DROP_HEIGHT, -FX_THIEVERY_REST_HEIGHT, FX_THIEVERY_DROP_FRAMES - work->dropTimer70, FX_THIEVERY_DROP_FRAMES);
                work->dropTimer70--;
            } else {
                work->pos1C.vy = -FX_THIEVERY_REST_HEIGHT;
            }
            if (frame < FX_THIEVERY_TILT_FRAME) {
                work->rotC.vx = 0x400;
            } else if (work->tiltTimer74 >= 0) {
                work->rotC.vx = fxm_lerpRatio(0x400, 0, FX_THIEVERY_TILT_FRAMES - work->tiltTimer74, FX_THIEVERY_TILT_FRAMES);
                work->tiltTimer74--;
            } else {
                work->rotC.vx = 0;
            }
            ex->Hi_DrawEffModel(&work->rotC, &work->pos1C, (s32)&work->scale2C, work->model78);
            ex->Hi_DrawEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, work->model7A);
            env->camera->points[0].vx = work->pos1C.vx;
            env->camera->points[0].vy = work->pos1C.vy;
            env->camera->points[0].vz = work->pos1C.vz;
            if (work->blend68 >= 0 || work->fadeTimer6C >= 0) {
                if (work->fadeTimer6C >= 0) {
                    work->rgb3C = fxm_lerpRatio(0, -0x80, FX_THIEVERY_FADE_FRAMES - work->fadeTimer6C, FX_THIEVERY_FADE_FRAMES);
                    work->fadeTimer6C--;
                }
                ex->Hi_ModifyEffModelAbr(work->model7A, BTLFX_ABR_ADD);
                ex->Hi_ModifyEffModelAbr(work->model78, BTLFX_ABR_ADD);
                ex->Hi_ModifyEffModelRGB(work->model7A, work->rgb3C, work->rgb3C, work->rgb3C);
                ex->Hi_ModifyEffModelRGB(work->model78, work->rgb3C, work->rgb3C, work->rgb3C);
            }
        }
        if (frame >= FX_THIEVERY_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0e7_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
