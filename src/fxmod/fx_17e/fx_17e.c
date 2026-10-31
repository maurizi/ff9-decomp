#include "fxmod/fx_17e.h"

s32 fx_17e_op80(s32 mode, Fx17eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    SVECTOR v;
    s32 frame;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x54;
    } else if (mode == 1) {
        fx_17e_env = env;
        for (i = 3; i >= 0; i--) {
            work->unk40[i] = 0;
        }
        ex->btlfx_initEffModels(fx_17e_effModels, 1);
        work->rot28.vx = 0;
        work->rot28.vy = 0;
        work->rot28.vz = 0;
        work->scale8.vx = 0x1000;
        work->scale8.vy = 0x1000;
        work->scale8.vz = 0x1000;
        work->model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 0xFF);
        work->unk4 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos38);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 1:
            work->unk40[3] = 0x1A;
            work->unk50 = 8;
            work->unk40[0] = 8;
            break;
        case 10:
            work->unk4 = 0x80;
            work->unk40[1] = 0xE;
            break;
        case 24:
            work->unk40[2] = 3;
            break;
        case 26:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos38);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (work->unk40[0] != 0) {
            work->unk40[0]--;
            ex->btlfx_getUnitBonePos(0x10, 5, (u16 *)&work->pos30);
            work->pos18.vx = work->pos30.vx;
            work->pos18.vy = work->pos30.vy - 0x64;
            work->pos18.vz = work->pos30.vz;
        }
        if (work->unk40[1] != 0) {
            work->unk40[1]--;
            ex->btlfx_getUnitBonePos(0x10, 0xC, (u16 *)&work->pos30);
            work->pos18.vx = work->pos30.vx;
            work->pos18.vy = work->pos30.vy;
            work->pos18.vz = work->pos30.vz;
        }
        if (work->unk40[2] != 0) {
            work->unk40[2]--;
            ex->btlfx_lerpVec((s16 *)&work->pos30, (s16 *)&work->pos38, (3 - work->unk40[2]) * 0x555, (s16 *)&v);
            work->pos18.vx = v.vx;
            work->pos18.vy = v.vy;
            work->pos18.vz = v.vz;
        }
        if (work->unk50 != 0) {
            work->unk50--;
            work->unk4 = 0x80 - work->unk50 * 0x10;
        }
        if (work->unk40[3] != 0) {
            work->unk40[3]--;
            ex->Hi_DrawEffModel(&work->rot28, &work->pos18, (s32)&work->scale8, work->model0);
            if (work->unk4 != 0x80) {
                c = work->unk4 - 0x80;
                ex->Hi_ModifyEffModelAbr(work->model0, 1);
                ex->Hi_ModifyEffModelRGB(work->model0, c, c, c);
            }
        }
        return frame >= 0x42;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17e/nonmatchings/fx_17e", fx_17e_headerPtr);
