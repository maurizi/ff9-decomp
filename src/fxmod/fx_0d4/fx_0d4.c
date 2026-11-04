#include "fxmod/fx_0d4.h"

s32 fx_0d4_op80(s32 mode, Fx0d4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_0d4_env = env;
        work->state0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRE_FIREBALL), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRE_HEAT_RING), (s32)&work->pos10);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame == 2) {
                work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRE_FLAME_BURST), (s32)&work->pos8);
                if (work->emit4 != NULL) {
                    work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0x3C) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_0d4_op84(s32 mode, Fx0d4Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
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
        *args = 0x10;
    } else if (mode == 1) {
        fx_0d4_env = env;
        work->state0 = 0;
        work->frame4 = 0;
    } else {
        alpha = 0;
        scale0 = 0;
        scale1 = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
        ry = 0;
        switch (work->state0) {
        case 0:
            ry = 0x200;
            alpha = (frame << 12) / 3;
            t = ex->math_sin(alpha >> 2);
            scale0 = t >> 1;
            scale1 = ((0x1000 - alpha) >> 1) + 0x800;
            if (frame >= 3) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 9;
            flicker = (frame & 1) << 8;
            scale1 = flicker + 0x1000;
            grow = (t >> 1) + 0x800;
            alpha = flicker + grow;
            ry = (t >> 3) + 0x200;
            scale0 = scale1;
            if (frame >= 9) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 9;
            ang = t >> 2;
            ry = 0x400 - ang;
            alpha = ex->math_cos(ang);
            scale0 = ex->math_sin(ang) + 0x1000;
            scale1 = ex->math_sin(ang) * 3 / 2 + 0x1000;
            if (frame >= 9) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->otZ = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRE_SWORD_GLINT), &work->pos8, NULL, scale0 / 3, 0, 1, alpha / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRE_SWORD_GLINT_2), &work->pos8, &rot, scale1 / 2, 0, 1, alpha * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d4/nonmatchings/fx_0d4", fx_0d4_headerPtr);
