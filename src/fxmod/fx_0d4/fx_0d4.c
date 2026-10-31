#include "fxmod/fx_0d4.h"

s32 fx_0d4_op80(s32 mode, Fx0d4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_0d4_env = env;
        work->state0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos10);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame == 2) {
                work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos8);
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

s32 fx_0d4_op84(s32 mode, Fx0d4Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 u;
    s32 k;
    s32 v;
    u16 ry;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        fx_0d4_env = env;
        work->state0 = 0;
        work->frame4 = 0;
    } else {
        a = 0;
        b = 0;
        c = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
        ry = 0;
        switch (work->state0) {
        case 0:
            ry = 0x200;
            a = (frame << 12) / 3;
            u = ex->math_sin(a >> 2);
            b = u >> 1;
            c = ((0x1000 - a) >> 1) + 0x800;
            if (frame >= 3) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            u = (frame << 12) / 9;
            k = (frame & 1) << 8;
            c = k + 0x1000;
            v = (u >> 1) + 0x800;
            a = k + v;
            ry = (u >> 3) + 0x200;
            b = c;
            if (frame >= 9) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            u = (frame << 12) / 9;
            t = u >> 2;
            ry = 0x400 - t;
            a = ex->math_cos(t);
            b = ex->math_sin(t) + 0x1000;
            c = ex->math_sin(t) * 3 / 2 + 0x1000;
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
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xB), &work->pos8, NULL, b / 3, 0, 1, a / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xC), &work->pos8, &rot, c / 2, 0, 1, a * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d4/nonmatchings/fx_0d4", fx_0d4_headerPtr);
