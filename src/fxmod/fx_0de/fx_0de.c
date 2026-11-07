#include "fxmod/fx_0de.h"

s32 fx_0de_op80(s32 mode, Fx0deWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    SVECTOR rot;
    VECTOR pos;
    VECTOR sc;
    s32 frame;
    s32 t;
    s32 size;
    s32 morphT;
    s32 fadeAng;

    if (mode == 0) {
        *args = sizeof(Fx0deWork80);
    } else if (mode == 1) {
        fx_0de_env = env;
        work->hold0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, &work->pos4.vx);
        ex->btlfx_getUnitPos(0, &work->posC.vx);
        ex->btlfx_initEffModels(fx_0de_effModels, FX_SWDMAG_WATER_MODEL_COUNT);
        work->model14[0] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_GLOBE_0), FX_SWDMAG_WATER_GLOBE_0_TPAGE, FX_SWDMAG_WATER_GLOBE_0_CLUT, FX_SWDMAG_WATER_GLOBE_0_U, FX_SWDMAG_WATER_GLOBE_0_V);
        work->model14[1] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_GLOBE_1), FX_SWDMAG_WATER_GLOBE_0_TPAGE, FX_SWDMAG_WATER_GLOBE_0_CLUT, FX_SWDMAG_WATER_GLOBE_0_U, FX_SWDMAG_WATER_GLOBE_0_V);
        work->model14[2] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_GLOBE_2), FX_SWDMAG_WATER_GLOBE_0_TPAGE, FX_SWDMAG_WATER_GLOBE_0_CLUT, FX_SWDMAG_WATER_GLOBE_0_U, FX_SWDMAG_WATER_GLOBE_0_V);
        work->model14[3] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_GLOBE_3), FX_SWDMAG_WATER_GLOBE_0_TPAGE, FX_SWDMAG_WATER_GLOBE_0_CLUT, FX_SWDMAG_WATER_GLOBE_0_U, FX_SWDMAG_WATER_GLOBE_0_V);
        work->model14[4] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_GLOBE), FX_SWDMAG_WATER_GLOBE_0_TPAGE, FX_SWDMAG_WATER_GLOBE_0_CLUT, FX_SWDMAG_WATER_GLOBE_0_U, FX_SWDMAG_WATER_GLOBE_0_V);
        work->model14[5] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_SPLASH_COLUMN), FX_SWDMAG_WATER_SPLASH_COLUMN_TPAGE, FX_SWDMAG_WATER_SPLASH_COLUMN_CLUT, FX_SWDMAG_WATER_SPLASH_COLUMN_U, FX_SWDMAG_WATER_SPLASH_COLUMN_V);
        work->model14[6] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_SPLASH_FUNNEL), FX_SWDMAG_WATER_SPLASH_COLUMN_TPAGE, FX_SWDMAG_WATER_SPLASH_COLUMN_CLUT,
                                                      FX_SWDMAG_WATER_SPLASH_COLUMN_U, FX_SWDMAG_WATER_SPLASH_COLUMN_V);
        work->model14[7] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_BASE_DISC), FX_SWDMAG_WATER_GLOBE_0_TPAGE, FX_SWDMAG_WATER_GLOBE_0_CLUT, FX_SWDMAG_WATER_GLOBE_0_U, FX_SWDMAG_WATER_GLOBE_0_V);
        size = (ex->btlfx_getUnitRadius(0) << 12) / 720;
        if (size < 0x400) {
            size = 0x6A4;
        }
        if (size > 0x1800) {
            size = 0x1800;
        }
        fx_0de_jumpScale = size;
        work->scale34 = size;
    } else {
        frame = entry->frame;
        if (work->hold0 != 0) {
            return 0;
        }
        if (frame < FX_SWDMAG_WATER_GLOBE_FRAMES) {
            t = (frame << 12) / FX_SWDMAG_WATER_GLOBE_FRAMES;
            pos.vx = work->posC.vx;
            pos.vy = work->posC.vy;
            pos.vz = work->posC.vz;
            size = (((ex->math_sin(t >> 2) / 10) + 0x199) * work->scale34) >> 12;
            rot.vz = 0;
            rot.vx = 0;
            morphT = (0x1000 - t) << 2;
            rot.vy = -frame << 5;
            sc.vy = (size * 3) >> 2;
            sc.vz = size;
            sc.vx = size;
            if (morphT > 0x3000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model14[0], work->model14[1], morphT - 0x3000);
            } else if (morphT > 0x2000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model14[1], work->model14[2], morphT - 0x2000);
            } else if (morphT > 0x1000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model14[2], work->model14[3], morphT - 0x1000);
            } else {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model14[3], work->model14[4], morphT);
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14[7]);
            ex->Hi_ModifyEffModelRGB(work->model14[7], (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80);
            /* Dead test, removed after reload: it keeps morphT live (and call-crossing) through this branch, so
             * global-alloc gives it s2 (as in fx_05f op80). */
            if (morphT != 0) {
                size = 0;
            }
        } else {
            morphT = ((frame - FX_SWDMAG_WATER_GLOBE_FRAMES) << 12) / FX_SWDMAG_WATER_SPRAY_FRAMES;
            fadeAng = morphT >> 2;
            pos.vx = work->posC.vx;
            pos.vy = work->posC.vy;
            pos.vz = work->posC.vz;
            sc.vx = sc.vz = (work->scale34 * 818) >> 12;
            sc.vy = (sc.vx * 3) >> 2;
            rot.vz = 0;
            rot.vx = 0;
            rot.vy = -frame << 5;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14[4]);
            ex->Hi_ModifyEffModelRGB(work->model14[4], (ex->math_cos(fadeAng) >> 5) - 0x80, (ex->math_cos(fadeAng) >> 5) - 0x80, (ex->math_cos(fadeAng) >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14[7]);
            ex->Hi_ModifyEffModelRGB(work->model14[7], (ex->math_cos(fadeAng) >> 5) - 0x80, (ex->math_cos(fadeAng) >> 5) - 0x80, (ex->math_cos(fadeAng) >> 5) - 0x80);
        }
        ex->btlfx_drawScrollMove(frame, FX_SWDMAG_WATER_WATER_VRAM_X, FX_SWDMAG_WATER_WATER_VRAM_Y, FX_SWDMAG_WATER_WATER_VRAM_W, FX_SWDMAG_WATER_WATER_VRAM_H,
                                 fx_0de_scrollSrc[frame % FX_SWDMAG_WATER_SCROLL_FRAMES + 1].srcX0,
                                 fx_0de_scrollSrc[frame % FX_SWDMAG_WATER_SCROLL_FRAMES + 1].srcY2);
        if (frame >= FX_SWDMAG_WATER_GLOBE_FRAMES) {
            t = ((frame - FX_SWDMAG_WATER_GLOBE_FRAMES) << 12) / FX_SWDMAG_WATER_SPRAY_FRAMES;
            pos.vx = work->posC.vx;
            pos.vy = work->posC.vy;
            pos.vz = work->posC.vz;
            sc.vx = sc.vz = (work->scale34 << 11) >> 12;
            sc.vy = ((ex->math_sin(t >> 2) / 4 + 0x2AA) * work->scale34) >> 12;
            rot.vz = 0;
            rot.vx = 0;
            rot.vy = -frame << 6;
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model14[5], work->model14[6], 0x1000 - t);
            /* Scheduling barrier (as in fx_05f op80). */
            do {
            } while (0);
            t >>= 1;
            ex->Hi_ModifyEffModelRGB(work->model14[5], (ex->math_sin(t) >> 5) - 0x80, (ex->math_sin(t) >> 5) - 0x80, (ex->math_sin(t) >> 5) - 0x80);
        }
        if (frame == FX_SWDMAG_WATER_SPLASH_DROPS_FRAME) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_SPLASH_DROPS), (s32)&work->pos4);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame == 1) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_BLUE_RING), (s32)&work->pos4);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
            }
        }
        if (frame >= FX_SWDMAG_WATER_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_0de_op81(s32 mode, Fx0deWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 i;
    s32 phase;

    if (mode == 0) {
        *args = sizeof(Fx0deWork81);
    } else if (mode == 1) {
        fx_0de_env = env;
        work->unitCount40 = env->targets->targetCount;
        for (i = 0; i < work->unitCount40; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->homePos0[i]);
            work->jumpHeight44 = ((-0x190 - (ex->btlfx_rand() & 0x7F)) * fx_0de_jumpScale) >> 12;
            ex->btlfx_getUnitBonePos(i, 0, (u16 *)&pos);
            work->height48[i] = -pos.vy;
        }
    } else {
        frame = entry->frame;
        if (frame < FX_SWDMAG_WATER_LIFT_FRAMES) {
            phase = frame << 7;
            for (i = 0; i < work->unitCount40; i++) {
                BTLFX_COPY_SVEC(&pos, &work->homePos0[i]);
                pos.vy += (work->jumpHeight44 * ex->math_sin(phase >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&pos);
            }
        } else {
            phase = (frame - FX_SWDMAG_WATER_LIFT_FRAMES) << 10;
            for (i = 0; i < work->unitCount40; i++) {
                BTLFX_COPY_SVEC(&pos, &work->homePos0[i]);
                pos.vy += (work->height48[i] * phase) >> 12;
                pos.vy += (work->jumpHeight44 * ex->math_cos(phase >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&pos);
            }
        }
        if (frame >= FX_SWDMAG_WATER_LIFT_FRAMES + FX_SWDMAG_WATER_DROP_FRAMES) {
            for (i = 0; i < work->unitCount40; i++) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&pos);
                ex->btlfx_setUnitPos(i, (s16 *)&pos);
            }
            return 1;
        }
    }
    return 0;
}

s32 fx_0de_op84(s32 mode, Fx0deWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 alpha;
    s32 scale0;
    s32 scale1;
    s32 ang;
    s32 t;
    s32 flicker;
    s32 grow;
    u16 ry;

    if (mode == 0) {
        *args = sizeof(Fx0deWork84);
    } else if (mode == 1) {
        fx_0de_env = env;
        work->state0 = FX_SWDMAG_WATER_SWORD_STATE_APPEAR;
        work->frame4 = 0;
    } else {
        alpha = 0;
        scale0 = 0;
        scale1 = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
        ry = 0;
        switch (work->state0) {
        case FX_SWDMAG_WATER_SWORD_STATE_APPEAR:
            ry = 0x200;
            alpha = (frame << 12) / FX_SWDMAG_WATER_SWORD_APPEAR_FRAMES;
            t = ex->math_sin(alpha >> 2);
            scale0 = t >> 1;
            scale1 = ((0x1000 - alpha) >> 1) + 0x800;
            if (frame >= FX_SWDMAG_WATER_SWORD_APPEAR_FRAMES) {
                work->state0 = FX_SWDMAG_WATER_SWORD_STATE_FLICKER;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_WATER_SWORD_STATE_FLICKER:
            t = (frame << 12) / FX_SWDMAG_WATER_SWORD_FLICKER_FRAMES;
            flicker = (frame & 1) << 8;
            scale1 = flicker + 0x1000;
            grow = (t >> 1) + 0x800;
            alpha = flicker + grow;
            ry = (t >> 3) + 0x200;
            scale0 = scale1;
            if (frame >= FX_SWDMAG_WATER_SWORD_FLICKER_FRAMES) {
                work->state0 = FX_SWDMAG_WATER_SWORD_STATE_FADE;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_WATER_SWORD_STATE_FADE:
            t = (frame << 12) / FX_SWDMAG_WATER_SWORD_FADE_FRAMES;
            ang = t >> 2;
            ry = 0x400 - ang;
            alpha = ex->math_cos(ang);
            scale0 = ex->math_sin(ang) + 0x1000;
            scale1 = ex->math_sin(ang) * 3 / 2 + 0x1000;
            if (frame >= FX_SWDMAG_WATER_SWORD_FADE_FRAMES) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->otZ = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_SWORD_GLINT), &work->pos8, NULL, scale0 / 3, 0, BTLFX_ABR_ADD, alpha / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_WATER_SWORD_FLASH), &work->pos8, &rot, scale1 / 2, 0, BTLFX_ABR_ADD, alpha * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0de/nonmatchings/fx_0de", fx_0de_headerPtr);
