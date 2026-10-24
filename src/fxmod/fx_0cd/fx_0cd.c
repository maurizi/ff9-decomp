#include "fxmod/fx_0cd.h"

s32 fx_0cd_op80(s32 mode, Fx0cdWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    s32 *q;
    s32 s;
    s32 c;

    if (mode == 0) {
        *arg4 = 0xB4;
    } else if (mode == 1) {
        D_801E7DD4 = env;
        for (i = 7, q = &((s32 *)work)[7]; i >= 0; i--, q--) {
            *q = 0;
        }
        ex->btlfx_initEffModels(D_801E7DE4, 3);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unkA4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkAC);
        work->unk20.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD), 1);
        work->unk4C.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 3), 1);
        work->unk78.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 1), 1);
        work->unk78.unk4.vx = 0;
        work->unk78.unk4.vy = 0;
        work->unk78.unk4.vz = 0;
        work->unk78.unk1C.vx = 0x1000;
        work->unk78.unk1C.vy = 0x1000;
        work->unk78.unk1C.vz = 0x1000;
        work->unk4C.unk4.vx = 0;
        work->unk4C.unk4.vy = 0;
        work->unk4C.unk4.vz = 0;
        work->unk4C.unk1C.vx = 0x1000;
        work->unk4C.unk1C.vy = 0x1000;
        work->unk4C.unk1C.vz = 0x1000;
        work->unk20.unk4.vx = 0;
        work->unk20.unk4.vy = 0;
        work->unk20.unk4.vz = 0;
        work->unk78.unkC.vx = work->unkA4.vx;
        work->unk78.unkC.vy = work->unkA4.vy;
        work->unk78.unkC.vz = work->unkA4.vz;
        work->unk4C.unkC.vx = work->unkAC.vx;
        work->unk4C.unkC.vy = work->unkAC.vy;
        work->unk4C.unkC.vz = work->unkAC.vz;
        work->unk20.unkC.vx = work->unkA4.vx;
        work->unk20.unkC.vy = work->unkA4.vy;
        work->unk20.unkC.vz = work->unkA4.vz;
        work->unk20.unk1C.vx = 0x1000;
        work->unk20.unk1C.vy = 0x1000;
        work->unk20.unk1C.vz = 0x1000;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x0:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unkA4);
            if (p != NULL) {
                p->unk14 = (s32)D_801E8044;
                p->unk12 = 0x26;
                p->unkC = 0x7C;
            }
            break;
        case 0x8:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unkA4);
            if (p != NULL) {
                p->unk14 = (s32)D_801E92B4;
                p->unk12 = 0x26;
                p->unkC = 0x7C;
            }
            break;
        case 0x10:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unkA4);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, -0x200);
            }
            work->unk0 = 0x20;
            break;
        case 0x28:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unkA4);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, -0x200);
            }
            work->unk4 = 0x30;
            break;
        case 0x38:
            work->unk8 = 0x18;
            break;
        case 0x50:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->unkA4);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unkA4);
            break;
        case 0x58:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unkA4);
            break;
        }
        if (work->unk0 != 0) {
            work->unk0--;
            work->unk78.unk4.vy += 0x80;
            work->unk78.unk1C.vx = work->unk78.unk1C.vz = ex->btlfx_lerpRatio(0x2000, 0x1000, work->unk0, 0x20);
            work->unk78.unk1C.vy = ex->btlfx_lerpRatio(0, 0x4000, work->unk0, 0x20);
            ex->Hi_DrawEffModel(&work->unk78.unk4, &work->unk78.unkC, (s32)&work->unk78.unk1C, work->unk78.unk0);
            if (work->unk0 > 0x18) {
                c = -((work->unk0 - 0x18) << 5);
                ex->Hi_ModifyEffModelRGB(work->unk78.unk0, c, c, c);
            }
            if (work->unk0 < 8) {
                c = (work->unk0 << 5) - 0x100;
                ex->Hi_ModifyEffModelRGB(work->unk78.unk0, c, c, c);
            }
        }
        if (work->unk4 != 0) {
            work->unk4--;
            work->unk20.unk4.vy += 0x80;
            work->unk20.unk1C.vx = work->unk20.unk1C.vz = ex->btlfx_lerpRatio(0, 0x2000, work->unk4, 0x30);
            ex->Hi_DrawEffModel(&work->unk20.unk4, &work->unk20.unkC, (s32)&work->unk20.unk1C, work->unk20.unk0);
            if (work->unk4 > 0x28) {
                c = -((work->unk4 - 0x28) << 5);
                ex->Hi_ModifyEffModelRGB(work->unk20.unk0, c, c, c);
            }
            if (work->unk4 < 8) {
                c = work->unk4 * 16 - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk20.unk0, c, c, c);
            }
        }
        if (work->unk8 != 0) {
            work->unk8--;
            work->unk4C.unk4.vy += 0x80;
            work->unk4C.unk1C.vx = work->unk4C.unk1C.vz = ex->btlfx_lerpRatio(0x4000, 0, work->unk8, 0x18);
            ex->Hi_DrawEffModel(&work->unk4C.unk4, &work->unk4C.unkC, (s32)&work->unk4C.unk1C, work->unk4C.unk0);
            c = -0x40;
            if (work->unk8 > 0x10) {
                c = -((work->unk8 - 0x10) * 8) - 0x40;
            }
            if (work->unk8 < 8) {
                c = work->unk8 * 8 - 0x80;
            }
            ex->Hi_ModifyEffModelRGB(work->unk4C.unk0, c, c, c);
        }
        return frame >= 0x80;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0cd/nonmatchings/fx_0cd", fx_0cd_headerPtr);
