#include "fxmod/fx_1f6.h"

s32 fx_1f6_op80(s32 mode, Fx1f6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0xB4;
    } else if (mode == 1) {
        D_801E7DD4 = env;
        for (i = 7; i >= 0; i--) {
            work->unk0[i] = 0;
        }
        ex->btlfx_initEffModels(D_801E7DE4, 3);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unkA4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkAC);
        work->unk20[0].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 6), 1);
        work->unk20[1].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 3), 1);
        work->unk20[2].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 1);
        work->unk20[2].unk4.vx = 0;
        work->unk20[2].unk4.vy = 0;
        work->unk20[2].unk4.vz = 0;
        work->unk20[2].unkC.vx = work->unkA4.vx;
        work->unk20[2].unkC.vy = work->unkA4.vy;
        work->unk20[2].unkC.vz = work->unkA4.vz;
        work->unk20[2].unk1C.vx = 0x1000;
        work->unk20[2].unk1C.vy = 0x1000;
        work->unk20[2].unk1C.vz = 0x1000;
        work->unk20[1].unk4.vx = 0;
        work->unk20[1].unk4.vy = 0;
        work->unk20[1].unk4.vz = 0;
        work->unk20[1].unkC.vx = work->unkAC.vx;
        work->unk20[1].unkC.vy = work->unkAC.vy;
        work->unk20[1].unkC.vz = work->unkAC.vz;
        work->unk20[1].unk1C.vx = 0x1000;
        work->unk20[1].unk1C.vy = 0x1000;
        work->unk20[1].unk1C.vz = 0x1000;
        work->unk20[0].unk4.vx = 0;
        work->unk20[0].unk4.vy = 0;
        work->unk20[0].unk4.vz = 0;
        work->unk20[0].unkC.vx = work->unkA4.vx;
        work->unk20[0].unkC.vy = work->unkA4.vy;
        work->unk20[0].unkC.vz = work->unkA4.vz;
        work->unk20[0].unk1C.vx = 0x1000;
        work->unk20[0].unk1C.vy = 0x1000;
        work->unk20[0].unk1C.vz = 0x1000;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x0:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unkA4);
            if (p != NULL) {
                p->unk14 = (s32)D_801E8044;
                p->unk12 = 0x26;
                p->unkC = 0x7C;
            }
            break;
        case 0x8:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unkA4);
            if (p != NULL) {
                p->unk14 = (s32)D_801E92B4;
                p->unk12 = 0x26;
                p->unkC = 0x7C;
            }
            break;
        case 0x10:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unkA4);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, -0x200);
            }
            work->unk0[0] = 0x20;
            break;
        case 0x28:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unkA4);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, -0x200);
            }
            work->unk0[1] = 0x30;
            break;
        case 0x38:
            work->unk0[2] = 0x18;
            break;
        case 0x50:
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unkA4);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unkA4);
            break;
        case 0x58:
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unkA4);
            break;
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            work->unk20[2].unk4.vy += 0x80;
            work->unk20[2].unk1C.vx = work->unk20[2].unk1C.vz = ex->btlfx_lerpRatio(0x2000, 0x1000, work->unk0[0], 0x20);
            work->unk20[2].unk1C.vy = ex->btlfx_lerpRatio(0, 0x4000, work->unk0[0], 0x20);
            ex->Hi_DrawEffModel(&work->unk20[2].unk4, &work->unk20[2].unkC, (s32)&work->unk20[2].unk1C,
                                work->unk20[2].unk0);
            if (work->unk0[0] >= 0x19) {
                c = -((work->unk0[0] - 0x18) << 5);
                ex->Hi_ModifyEffModelRGB(work->unk20[2].unk0, c, c, c);
            }
            if (work->unk0[0] < 8) {
                c = (work->unk0[0] << 5) - 0x100;
                ex->Hi_ModifyEffModelRGB(work->unk20[2].unk0, c, c, c);
            }
        }
        if (work->unk0[1] != 0) {
            work->unk0[1]--;
            work->unk20[0].unk4.vy += 0x80;
            work->unk20[0].unk1C.vx = work->unk20[0].unk1C.vz = ex->btlfx_lerpRatio(0, 0x2000, work->unk0[1], 0x30);
            ex->Hi_DrawEffModel(&work->unk20[0].unk4, &work->unk20[0].unkC, (s32)&work->unk20[0].unk1C,
                                work->unk20[0].unk0);
            if (work->unk0[1] >= 0x29) {
                c = -((work->unk0[1] - 0x28) << 5);
                ex->Hi_ModifyEffModelRGB(work->unk20[0].unk0, c, c, c);
            }
            if (work->unk0[1] < 8) {
                c = work->unk0[1] * 0x10 - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk20[0].unk0, c, c, c);
            }
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            work->unk20[1].unk4.vy += 0x80;
            work->unk20[1].unk1C.vx = work->unk20[1].unk1C.vz = ex->btlfx_lerpRatio(0x4000, 0, work->unk0[2], 0x18);
            ex->Hi_DrawEffModel(&work->unk20[1].unk4, &work->unk20[1].unkC, (s32)&work->unk20[1].unk1C,
                                work->unk20[1].unk0);
            c = -0x40;
            if (work->unk0[2] >= 0x11) {
                c = -((work->unk0[2] - 0x10) * 8) - 0x40;
            }
            if (work->unk0[2] < 8) {
                c = work->unk0[2] * 8 - 0x80;
            }
            ex->Hi_ModifyEffModelRGB(work->unk20[1].unk0, c, c, c);
        }
        return frame >= 0x80;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f6/nonmatchings/fx_1f6", fx_1f6_headerPtr);
