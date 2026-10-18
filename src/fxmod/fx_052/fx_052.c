#include "fxmod/fx_052.h"

s32 fx_052_op80(s32 mode, Fx052Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x58;
    } else if (mode == 1) {
        D_801E7FA8 = env;
        ex->btlfx_initEffModels(D_801E7FB8, 3);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk40);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk48);
        work->unkC.vz = -0x200;
        work->unk14.vz = 0x200;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unk14.vx = 0;
        work->unk14.vy = 0;
        work->unk2C.vx = 0x2000;
        work->unk2C.vy = 0x2000;
        work->unk2C.vz = 0x2000;
        work->unk1C.vx = work->unk40.vx;
        work->unk1C.vy = work->unk40.vy;
        work->unk1C.vz = work->unk40.vz;
        work->unk52 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 7), 1);
        work->unk54 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xB), 1);
        work->unk56 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE));
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk48);
        work->unk50 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x1E:
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk48);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E8218;
                work->unk4->unk12 = 0x14;
                work->unk4->unkC = 0xB0;
            }
            break;
        case 0x37:
            work->unk50 = 0x18;
            break;
        case 0x50:
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk48);
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E8FD8;
                work->unk8->unk12 = 0x14;
                work->unk8->unkC = 0xB0;
            }
            break;
        }
        t = frame - 0x37;
        if (t >= 0 && t < 0x79) {
            if (t >= 0x23) {
                if (work->unk2C.vx > 0x400) {
                    work->unk2C.vx -= 0x100;
                    work->unk2C.vy -= 0x100;
                    work->unk2C.vz -= 0x100;
                }
                ex->Hi_DrawEffModel(&work->unk14, &work->unk1C, (s32)&work->unk2C, work->unk54);
                work->unk14.vy += 0x10;
            }
            ex->Hi_DrawEffModel(&work->unkC, &work->unk1C, (s32)&work->unk2C, work->unk56);
            ex->Hi_DrawEffModel(&work->unkC, &work->unk1C, (s32)&work->unk2C, work->unk52);
            work->unkC.vy -= 0x10;
            if (work->unk50 >= 0) {
                work->unk3C = func_801E7E70(-0x80, 0, 0x18 - work->unk50, 0x18);
                work->unk50--;
                ex->Hi_ModifyEffModelRGB(work->unk52, work->unk3C, work->unk3C, work->unk3C);
                ex->Hi_ModifyEffModelRGB(work->unk56, work->unk3C, work->unk3C, work->unk3C);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xC), frame);
        if (frame >= 0xC8) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_052_op81(s32 mode, Fx052Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E7FA8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 15:
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unkC);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 40:
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unkC);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        }
        return frame >= 0x5A;
    }
    return 0;
}

s32 fx_052_op82(s32 mode, Fx052Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E7FA8 = env;
        work->unk2 = -1;
        work->unk0 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0 = 0x10;
            break;
        case 20:
            work->unk2 = 0x10;
            break;
        }
        if (work->unk0 >= 0) {
            func_801E7EC8(0, 0xFF, 0x10 - work->unk0, 0x10, 1, 0);
            work->unk0--;
        } else if (work->unk2 >= 0) {
            func_801E7EC8(0xFF, 0, 0x10 - work->unk2, 0x10, 1, 0);
            work->unk2--;
        } else {
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x20;
    }
    return 0;
}

void func_801E7E2C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7FA8)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7E70(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E7EC8(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E7FA8)->btlfx_drawScreenTint((u32 *)(D_801E7FA8->unk20 + ot), arg4, c, c, c);
}

INCLUDE_RODATA("asm/us/fx_052/nonmatchings/fx_052", fx_052_headerPtr);
