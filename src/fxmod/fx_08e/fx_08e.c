#include "fxmod/fx_08e.h"

INCLUDE_RODATA("asm/us/fx_08e/nonmatchings/fx_08e", fx_08e_headerPtr);

s32 fx_08e_op80(s32 mode, Fx08eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT r;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        D_801E7E20 = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unk48);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk58);
        work->unk50.vx = work->unk58.vx;
        work->unk50.vy = work->unk58.vy;
        work->unk50.vz = work->unk58.vz;
        work->unk50.vy -= 0x1C2;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk58);
        work->unk1C.vx = 0;
        work->unk1C.vy = 0;
        work->unk1C.vz = 0;
        work->unk34.vx = 0x800;
        work->unk34.vy = 0x1000;
        work->unk34.vz = 0x800;
        work->unk24.vx = work->unk58.vx;
        work->unk24.vy = work->unk58.vy;
        work->unk24.vz = work->unk58.vz;
        ex->btlfx_initEffModels(D_801E7E30, 2);
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE), 0x1B, 0x3D09, 0, 0);
        work->unk6A = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0x1B, 0x3D09, 0, 0);
        work->unk64 = work->unk60 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 6:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk58);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk58);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E7FC0;
                work->unk4->unk12 = 9;
                work->unk4->unkC = 0xA0;
            }
            break;
        case 22:
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk58);
            work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk58);
            work->unk60 = 0x12;
            break;
        case 28:
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk50);
            break;
        case 42:
            work->unk14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk48);
            break;
        case 36:
            work->unk64 = 0x18;
            break;
        }
        if (work->unk60 >= 0) {
            work->unk44 = func_801E7D44(-0x80, 0, 0x12 - work->unk60, 0x12);
            work->unk60--;
        }
        if (work->unk64 >= 0) {
            work->unk44 = func_801E7D44(0, -0x80, 0x18 - work->unk64, 0x18);
            work->unk64--;
        }
        if (frame >= 0x16 && frame < 0x20) {
            s32 t = ((frame - 0x16) << 12) / 10;
            work->unk34.vy = t / 2;
            ex->Hi_DrawEffModel(&work->unk1C, &work->unk24, (s32)&work->unk34, work->unk68);
            ex->Hi_ModifyEffModelAbr(work->unk68, 1);
            ex->Hi_ModifyEffModelRGB(work->unk68, work->unk44, work->unk44, work->unk44);
        }
        if (frame >= 0x20 && frame < 0x38) {
            ex->Hi_DrawMorphEffModel(&work->unk1C, &work->unk24, (s32)&work->unk34, work->unk68, work->unk6A,
                                     0x1000 - ((frame - 0x20) << 12) / 24);
            ex->Hi_ModifyEffModelAbr(work->unk68, 1);
            ex->Hi_ModifyEffModelRGB(work->unk68, work->unk44, work->unk44, work->unk44);
        }
        if (frame >= 0x16) {
            s16 *p = D_801E7D9C[frame % 15];
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

void func_801E7D00(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7E20)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7D44(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
