#include "fxmod/fx_00e.h"

s32 fx_00e_op80(s32 mode, Fx00eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    SVECTOR rot;
    VECTOR scale;
    s32 a;
    s32 s;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        fx_00e_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->pos18[0] = work->pos8.vx;
        work->pos18[1] = work->pos8.vy;
        work->pos18[2] = work->pos8.vz;
        ex->btlfx_initEffModels(fx_00e_effModels, 2);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 4), 0x138, 0xFFFF, 0, 0x80);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos8);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos8);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            a = (frame << 7) >> 1;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            c = ex->math_sin(a);
            s = (a * 2 + 0x1800) * 7 / 4;
            scale.vx = s;
            scale.vy = (frame << 8) + 0x1000;
            scale.vz = s;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)work->pos18, (s32)&scale, work->model24);
            ex->Hi_ModifyEffModelAbr(work->model24, 1);
            ex->Hi_ModifyEffModelRGB(work->model24, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            scale.vy >>= 1;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)work->pos18, (s32)&scale, work->model24);
            ex->Hi_ModifyEffModelAbr(work->model24, 1);
            ex->Hi_ModifyEffModelRGB(work->model24, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            if (frame >= 0x20) {
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 3), work->frame4);
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_00e/nonmatchings/fx_00e", fx_00e_headerPtr);
