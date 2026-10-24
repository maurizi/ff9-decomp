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
        D_801E7D20 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        work->unk8.vy -= 0x80;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk18);
        ex->btlfx_aimAngles((s16 *)&work->unk18, (s16 *)&work->unk8, (s16 *)&work->unk24);
        ex->btlfx_initEffModels(D_801E7D30, 1);
        work->unk20 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 1);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk18);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0x10, 0x10);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk18);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0x10, 0x10);
        }
        work->unk2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk18);
        if (work->unk2C != NULL) {
            work->unk2C->unk22 = ex->func_800E7CC4(0x10, 0x10);
        }
        if (work->unk2C != NULL) {
            work->unk2C->unk14 = (s32)D_801E7E00;
            work->unk2C->unk12 = 0x15;
            work->unk2C->unkC = 0x34;
        }
        work->unk0 = work->unk4 = 0;
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            if (frame >= 6) {
                work->unk0 = 1;
                entry->frame = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0x10, 0x10);
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
            ex->Hi_DrawEffModel(&rot, (VECTOR *)mpos, (s32)scale, work->unk20);
            ex->Hi_ModifyEffModelAbr(work->unk20, 2);
            ex->Hi_ModifyEffModelRGB(work->unk20, (ex->math_sin(t >> 1) / 3 >> 5) - 0x80, (ex->math_sin(t >> 1) / 3 >> 5) - 0x80,
                                     (ex->math_sin(t >> 1) / 3 >> 5) - 0x80);
            rot.vy = -frame * 0x80;
            rot.vz = 0;
            rot.vx = 0;
            scale[0] = scale[2] = 0x2400;
            scale[1] = s;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)mpos, (s32)scale, work->unk20);
            ex->Hi_ModifyEffModelAbr(work->unk20, 1);
            ex->Hi_ModifyEffModelRGB(work->unk20, (ex->math_sin(t >> 1) / 2 >> 5) - 0x80, (ex->math_sin(t >> 1) / 2 >> 5) - 0x80,
                                     (ex->math_sin(t >> 1) / 2 >> 5) - 0x80);
            if (frame >= 0x14) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (work->unk2C != NULL) {
            if (work->unk2C->unk30 == -1) {
                work->unk2C = NULL;
            }
            if (work->unk2C != NULL) {
                work->unk2C->unk5C.vx = work->unk24.vx;
                work->unk2C->unk5C.vy = work->unk24.vy;
                work->unk2C->unk5C.vz = work->unk24.vz;
            }
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ce/nonmatchings/fx_0ce", fx_0ce_headerPtr);
