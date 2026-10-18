#include "fxmod/fx_090.h"

s32 fx_090_op80(s32 mode, Fx090Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT r;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x74;
    } else if (mode == 1) {
        work->unk50.vx = env->unkC->unk38;
        work->unk50.vy = env->unkC->unk3A;
        work->unk50.vz = env->unkC->unk3C;
        work->unk58.vx = env->unkC->unk38;
        work->unk58.vy = env->unkC->unk3A;
        work->unk58.vz = env->unkC->unk3C;
        work->unk60.vx = env->unkC->unk38;
        work->unk60.vy = env->unkC->unk3A;
        work->unk60.vz = env->unkC->unk3C;
        work->unk58.vy -= 0x320;
        work->unk60.vy -= 0x4B0;
        D_801E7E38 = env;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk50);
        work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk50);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7FD8;
            work->unk0->unk12 = 0xC;
            work->unk0->unkC = 0xA0;
        }
        if (work->unkC != NULL) {
            work->unkC->unk22 = 0x180;
        }
        work->unk24.vx = 0;
        work->unk24.vy = 0;
        work->unk24.vz = 0;
        work->unk2C.vx = env->unkC->unk38;
        work->unk2C.vy = env->unkC->unk3A;
        work->unk2C.vz = env->unkC->unk3C;
        work->unk3C.vx = 0x1000;
        work->unk3C.vy = 0x1000;
        work->unk3C.vz = 0x1000;
        ex->btlfx_initEffModels(D_801E7E48, 2);
        work->unk70 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0x1B, 0x3D09, 0, 0);
        work->unk72 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0x1B, 0x3D09, 0, 0);
        work->unk6C = work->unk68 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 18:
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk50);
            work->unk20 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk50);
            if (work->unk10 != NULL) {
                work->unk10->unk22 = 0x180;
            }
            work->unk68 = 0x12;
            break;
        case 22:
            work->unk14 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk58);
            work->unk18 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk58);
            break;
        case 39:
            work->unk1C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk58);
            break;
        case 28:
            work->unk6C = 0x1A;
            break;
        }
        if (work->unk68 >= 0) {
            work->unk4C = func_801E7D5C(-0x80, 0, 0x12 - work->unk68, 0x12);
            work->unk68--;
        }
        if (work->unk6C >= 0) {
            work->unk4C = func_801E7D5C(0, -0x80, 0x1A - work->unk6C, 0x1A);
            work->unk6C--;
        }
        if (frame >= 0x12 && frame < 0x18) {
            work->unk3C.vy = ((frame - 0x12) << 12) / 6;
            ex->Hi_DrawEffModel(&work->unk24, &work->unk2C, (s32)&work->unk3C, work->unk70);
            ex->Hi_ModifyEffModelAbr(work->unk70, 1);
            ex->Hi_ModifyEffModelRGB(work->unk70, work->unk4C, work->unk4C, work->unk4C);
        }
        if (frame >= 0x18 && frame < 0x32) {
            ex->Hi_DrawMorphEffModel(&work->unk24, &work->unk2C, (s32)&work->unk3C, work->unk70, work->unk72,
                                     0x1000 - ((frame - 0x18) << 12) / 26);
            ex->Hi_ModifyEffModelAbr(work->unk70, 1);
            ex->Hi_ModifyEffModelRGB(work->unk70, work->unk4C, work->unk4C, work->unk4C);
        }
        if (frame >= 0x12) {
            s16 *p = D_801E7DB4[frame % 15];
            r.x = p[0];
            r.y = p[1];
            r.w = 0x20;
            r.h = 0x80;
            ex->MoveImage(&r, 0x2C0, 0x100);
        }
        if (frame >= 0x46) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E7D18(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7E38)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7D5C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_090/nonmatchings/fx_090", fx_090_headerPtr);
