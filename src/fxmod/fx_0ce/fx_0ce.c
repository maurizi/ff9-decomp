#include "fxmod/fx_0ce.h"

s32 fx_0ce_op80(s32 mode, Fx0ceWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 mpos[3];
    s32 scale[3];
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 s;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        fx_0ce_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->unk8.vy -= 0x80;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos18);
        ex->btlfx_aimAngles((s16 *)&work->pos18, (s16 *)&work->unk8, (s16 *)&work->unk24);
        ex->btlfx_initEffModels(fx_0ce_effModels, 1);
        work->model20 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 1);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos18);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x10);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos18);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x10);
        }
        work->emit2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos18);
        if (work->emit2C != NULL) {
            work->emit2C->otZ = ex->btlfx_addRadiusSixth(0x10, 0x10);
        }
        if (work->emit2C != NULL) {
            work->emit2C->trails = (s32)D_801E7E00;
            work->emit2C->trailSlots = 0x15;
            work->emit2C->trailStride = 0x34;
        }
        work->unk0 = work->unk4 = 0;
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            if (frame >= 6) {
                work->unk0 = 1;
                entry->frame = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos10);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0x10, 0x10);
                }
            }
            break;
        case 1:
            mpos[0] = work->unk8.vx;
            mpos[1] = work->unk8.vy;
            mpos[2] = work->unk8.vz;
            rot.vx = rot.vz = 0;
            rot.vy = frame * 0x84;
            t = (frame << 12) / 20;
            if (t < 0x400) {
                s = ex->math_sin(t);
            } else {
                s = ex->math_sin((t - 0x400) / 3) * 3 / 2 + 0x1000;
                work->unk8.vy -= 0x18;
            }
            scale[0] = scale[2] = 0x1A00;
            scale[1] = s;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)mpos, (s32)scale, work->model20);
            ex->Hi_ModifyEffModelAbr(work->model20, 2);
            ex->Hi_ModifyEffModelRGB(work->model20, (ex->math_sin(t >> 1) / 3 >> 5) - 0x80, (ex->math_sin(t >> 1) / 3 >> 5) - 0x80,
                                     (ex->math_sin(t >> 1) / 3 >> 5) - 0x80);
            rot.vy = -frame * 0x80;
            rot.vz = 0;
            rot.vx = 0;
            scale[0] = scale[2] = 0x2400;
            scale[1] = s;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)mpos, (s32)scale, work->model20);
            ex->Hi_ModifyEffModelAbr(work->model20, 1);
            ex->Hi_ModifyEffModelRGB(work->model20, (ex->math_sin(t >> 1) / 2 >> 5) - 0x80, (ex->math_sin(t >> 1) / 2 >> 5) - 0x80,
                                     (ex->math_sin(t >> 1) / 2 >> 5) - 0x80);
            if (frame >= 0x14) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (work->emit2C != NULL) {
            if (work->emit2C->state == BTLFX_EMIT_DONE) {
                work->emit2C = NULL;
            }
            if (work->emit2C != NULL) {
                work->emit2C->rot.vx = work->unk24.vx;
                work->emit2C->rot.vy = work->unk24.vy;
                work->emit2C->rot.vz = work->unk24.vz;
            }
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ce/nonmatchings/fx_0ce", fx_0ce_headerPtr);
