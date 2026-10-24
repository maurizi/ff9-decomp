#include "fxmod/fx_001.h"

INCLUDE_RODATA("asm/us/fx_001/nonmatchings/fx_001", fx_001_headerPtr);

s32 fx_001_op80(s32 mode, Fx001Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT r;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x70;
    } else if (mode == 1) {
        D_801E7E20 = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unk4C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk5C);
        work->unk54.vx = work->unk5C.vx;
        work->unk54.vy = work->unk5C.vy;
        work->unk54.vz = work->unk5C.vz;
        work->unk54.vy -= 0x1C2;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk5C);
        work->unk20.vx = 0;
        work->unk20.vy = 0;
        work->unk20.vz = 0;
        work->unk38.vx = 0x800;
        work->unk38.vy = 0x1000;
        work->unk38.vz = 0x800;
        work->unk28.vx = work->unk5C.vx;
        work->unk28.vy = work->unk5C.vy;
        work->unk28.vz = work->unk5C.vz;
        ex->btlfx_initEffModels(D_801E7E30, 2);
        work->unk6C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE), 0x1B, 0x3D09, 0, 0);
        work->unk6E = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0x1B, 0x3D09, 0, 0);
        work->unk68 = work->unk64 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 6:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk5C);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk5C);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E7FC0;
                work->unk4->unk12 = 9;
                work->unk4->unkC = 0xA0;
            }
            break;
        case 22:
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk5C);
            work->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk5C);
            work->unk64 = 0x12;
            break;
        case 28:
            work->unk14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk54);
            break;
        case 42:
            work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk4C);
            break;
        case 36:
            work->unk68 = 0x18;
            break;
        }
        if (work->unk64 >= 0) {
            work->unk48 = func_801E7D44(-0x80, 0, 0x12 - work->unk64, 0x12);
            work->unk64--;
        }
        if (work->unk68 >= 0) {
            work->unk48 = func_801E7D44(0, -0x80, 0x18 - work->unk68, 0x18);
            work->unk68--;
        }
        if (frame >= 0x16 && frame < 0x20) {
            s32 t = ((frame - 0x16) << 12) / 10;
            work->unk38.vy = t / 2;
            ex->Hi_DrawEffModel(&work->unk20, &work->unk28, (s32)&work->unk38, work->unk6C);
            ex->Hi_ModifyEffModelAbr(work->unk6C, 1);
            ex->Hi_ModifyEffModelRGB(work->unk6C, work->unk48, work->unk48, work->unk48);
        }
        if (frame >= 0x20 && frame < 0x38) {
            ex->Hi_DrawMorphEffModel(&work->unk20, &work->unk28, (s32)&work->unk38, work->unk6C, work->unk6E,
                                     0x1000 - ((frame - 0x20) << 12) / 24);
            ex->Hi_ModifyEffModelAbr(work->unk6C, 1);
            ex->Hi_ModifyEffModelRGB(work->unk6C, work->unk48, work->unk48, work->unk48);
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
