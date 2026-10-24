#include "fxmod/fx_125.h"

s32 fx_125_op80(s32 mode, Fx125Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    Pool6CEntry *q;
    s32 frame;
    s32 x;
    s32 x2;
    s32 tp1;
    s32 tp2;
    s32 s;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7D78 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        work->unkC.vx = work->unk4.vx;
        work->unkC.vy = work->unk4.vy;
        work->unkC.vz = work->unk4.vz;
        work->unk1C.vx = work->unk4.vx;
        work->unk1C.vy = work->unk4.vy;
        work->unk1C.vz = work->unk4.vz;
        work->unkC.vy -= 0x100;
        work->unk1C.vy -= 0x400;
        ex->btlfx_initEffModels(D_801E7D88, 3);
        work->unk24 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xD), 0xB3,
                                                      0x3D40, 0, 0);
        x = env->dispEnv->disp.x;
        if (x < 0x100) {
            x2 = x + 0x140;
        } else {
            x2 = x - 0x140;
        }
        tp1 = getTPage(2, 3, x, env->dispEnv->disp.y);
        tp2 = getTPage(2, 3, x2, env->dispEnv->disp.y);
        work->unk28[0] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xE), tp1,
                                                         0xFFFF, 0, 0);
        work->unk28[1] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xE), tp2,
                                                         0xFFFF, 0, 0);
        q = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk4);
        if (q != NULL) {
            q->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
        if (frame >= 0x14) {
            pos.vx = work->unk1C.vx;
            pos.vy = work->unk1C.vy;
            pos.vz = work->unk1C.vz;
            rot.vy = frame << 5;
            rot.vx = 0;
            rot.vz = 0;
            if (frame >= 0x3C) {
                s = (((frame - 0x3C) << 12) / 10);
                s = ex->math_cos(s >> 2) / 3;
            } else {
                s = 0x555;
            }
            scale.vz = s;
            scale.vy = s;
            scale.vx = s;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
            ex->Hi_ModifyEffModelAbr(work->unk24, 1);
            ex->Hi_ModifyEffModelRGB(work->unk24, -0x56, -0x56, -0x56);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28[frame & 1]);
            ex->Hi_ModifyEffModelAbr(work->unk28[frame & 1], 0xFF);
            ex->Hi_ModifyEffModelRGB(work->unk28[frame & 1], 0x40, 0x40, 0x40);
        }
        if (frame == 0x10) {
            Pool6CEntry *p;

            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk4);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x46);
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        if (frame == 0x13) {
            Pool6CEntry *p;

            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk4);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x20);
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk4);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        if (frame == 0x1A) {
            Pool6CEntry *p;

            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk14);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        if (frame == 0x28) {
            Pool6CEntry *p;

            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk14);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x32);
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk14);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x32);
            }
        }
        if (frame >= 0x46) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_125/nonmatchings/fx_125", fx_125_headerPtr);
