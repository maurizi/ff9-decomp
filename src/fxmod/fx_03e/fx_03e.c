#include "fxmod/fx_03e.h"

s32 fx_03e_op80(s32 mode, Fx03eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 s;
    s32 t;
    Pool6CEntry *p;
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E7F70 = env;
        ex->btlfx_initEffModels(D_801E7F80, 4);
        work->unk8 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0xBA, 0x3DC0, 0, 0);
        work->unkC = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 1), 0xBA, 0x3DC0, 0, 0);
        work->unk10 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x19), 0xBA, 0x3DC0, 0, 0);
        work->unk4 =
            (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0xB8, 0x3D40, 0x80, 0x80);
        work->unk0 = 0;
        work->unk24 = 0;
        work->unk14.vx = env->targets->centerX;
        work->unk14.vy = env->targets->centerY;
        work->unk14.vz = env->targets->centerZ;
        work->unk1C.vx = work->unk14.vx;
        work->unk1C.vy = work->unk14.vy;
        work->unk1C.vz = work->unk14.vz;
        work->unk14.vy -= 0x19F;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E82A0;
                    p->unk12 = 0xA;
                    p->unkC = 0x5C;
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame >= 8) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = frame << 10;
            scale.vx = scale.vy = scale.vz = ((0x1000 - ex->math_cos(t >> 2)) * 0xE00) >> 12;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos.vx = work->unk14.vx;
            pos.vy = work->unk14.vy;
            pos.vz = work->unk14.vz;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk8);
            ex->Hi_ModifyEffModelAbr(work->unk8, 0);
            ex->Hi_ModifyEffModelRGB(work->unk8, 0, 0, 0);
            if (frame >= 4) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            scale.vz = 0xE00;
            scale.vy = 0xE00;
            scale.vx = 0xE00;
            rot.vx = 0;
            rot.vy = frame << 5;
            rot.vz = 0;
            pos.vx = work->unk14.vx;
            pos.vy = work->unk14.vy;
            pos.vz = work->unk14.vz;
            t = frame << 8;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk8);
            ex->Hi_ModifyEffModelAbr(work->unk8, 0);
            ex->Hi_ModifyEffModelRGB(work->unk8, 0, 0, 0);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk4);
            ex->Hi_ModifyEffModelAbr(work->unk4, 1);
            ex->Hi_ModifyEffModelRGB(work->unk4, (ex->math_sin(t >> 1) >> 5) - 0x80, (ex->math_sin(t >> 1) >> 5) - 0x80,
                                     (ex->math_sin(t >> 1) >> 5) - 0x80);
            ex->btlfx_drawScrollMove(((0x1000 - ex->math_cos(t >> 1)) >> 9) + frame * 5, 0x240, 0x180, 0x40, 0x80, 0x240, 0x100);
            if (frame >= 0x10) {
                work->unk0 = 3;
                entry->unk0 = -1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk1C);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x20, 0x46);
            }
            break;
        case 3:
            scale.vz = 0xE00;
            scale.vy = 0xE00;
            scale.vx = 0xE00;
            rot.vx = 0;
            rot.vy = 0x200;
            rot.vz = 0;
            pos.vx = work->unk14.vx;
            pos.vy = work->unk14.vy;
            pos.vz = work->unk14.vz;
            if (frame < 2) {
                s = 0x2000 - (frame << 11);
            } else {
                s = 0x1000 - ((frame - 2) << 12) / 14;
            }
            if (s > 0x1000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->unk8, work->unkC, s - 0x1000);
                ex->Hi_ModifyEffModelRGB(work->unk8, -0x40, -0x40, -0x40);
            } else {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->unkC, work->unk10, s);
                ex->Hi_ModifyEffModelRGB(work->unkC, ((s - 0x800) >> 5) - 0x80, ((s - 0x800) >> 5) - 0x80,
                                         ((s - 0x800) >> 5) - 0x80);
            }
            if (frame == 1) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame >= 0xC) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk24++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_03e/nonmatchings/fx_03e", fx_03e_headerPtr);
