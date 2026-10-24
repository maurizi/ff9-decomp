#include "fxmod/fx_130.h"

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7DF4);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

s32 fx_130_op80(s32 mode, Fx130Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 *q;
    s32 frame;
    s32 r;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E7DF4 = env;
        for (i = 7, q = &work->unk0[7]; i >= 0; i--) {
            *q-- = 0;
        }
        ex->btlfx_initEffModels(D_801E7E04, 4);
        work->unk20.vx = env->targets->unk38;
        work->unk20.vz = env->targets->unk3C;
        work->unk20.vy = 0;
        for (i = 0; i < 4; i++) {
            D_801E8124[i].unk18 = -1;
            D_801E8124[i].unk1C = -1;
        }
        for (i = 0; i < env->targets->unk24; i++) {
            ex->btlfx_getUnitPos(i, &D_801E8124[i].unk0.vx);
            ex->btlfx_getUnitRot(i, &D_801E8124[i].unk8.vx);
            ex->btlfx_getUnitRot(i, &D_801E8124[i].unk10.vx);
            D_801E8124[i].unk18 = i * 6;
            D_801E8124[i].unk1C = 0x20;
            D_801E8124[i].unk20 = i * 6;
            D_801E8124[i].unk24 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 1);
            setVector(&D_801E8124[i].unk30, D_801E8124[i].unk0.vx, D_801E8124[i].unk0.vy, D_801E8124[i].unk0.vz);
            setVector(&D_801E8124[i].unk28, 0, 0, 0);
            setVector(&D_801E8124[i].unk40, 0x1000, 0x1000, 0x1000);
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk20);
            if (p != NULL) {
                p->unk22 = -0x100;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk20);
            if (p != NULL) {
                p->unk22 = -0x100;
            }
            break;
        case 8:
            work->unk0[0] = 0x38;
            work->unk0[1] = 0x20;
            break;
        case 0x2A:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk20);
            if (p != NULL) {
                p->unk22 = -0x100;
            }
            break;
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            for (i = 0; i < 4; i++) {
                if (D_801E8124[i].unk18 == 0) {
                    if (D_801E8124[i].unk1C != 0) {
                        D_801E8124[i].unk1C--;
                        ex->Hi_DrawEffModel(&D_801E8124[i].unk28, &D_801E8124[i].unk30, (s32)&D_801E8124[i].unk40, D_801E8124[i].unk24);
                        D_801E8124[i].unk28.vy += 0x80;
                        if (D_801E8124[i].unk1C > 0x10) {
                            D_801E8124[i].unk8.vy = (D_801E8124[i].unk10.vy + func_801E7704(1, 0x10, D_801E8124[i].unk1C - 0x10, 0, 0x2000)) & 0xFFF;
                        } else {
                            D_801E8124[i].unk8.vy = (D_801E8124[i].unk10.vy + func_801E7704(2, 0x10, D_801E8124[i].unk1C, 0x2000, 0x4000)) & 0xFFF;
                        }
                        if (D_801E8124[i].unk1C == 0) {
                            D_801E8124[i].unk8.vy = D_801E8124[i].unk10.vy;
                        }
                        ex->btlfx_setUnitRot(i, &D_801E8124[i].unk8.vx);
                        if (D_801E8124[i].unk1C > 0x18) {
                            D_801E8124[i].unk40.vx = D_801E8124[i].unk40.vz = ex->btlfx_lerpRatio(0x1000, 0, D_801E8124[i].unk1C - 0x18, 8);
                            r = -((D_801E8124[i].unk1C - 0x18) << 5);
                            ex->Hi_ModifyEffModelRGB(D_801E8124[i].unk24, r, r, r);
                        }
                        if (D_801E8124[i].unk1C < 8) {
                            D_801E8124[i].unk40.vx = D_801E8124[i].unk40.vz = ex->btlfx_lerpRatio(0, 0x1000, D_801E8124[i].unk1C, 8);
                            r = -((8 - D_801E8124[i].unk1C) << 5);
                            ex->Hi_ModifyEffModelRGB(D_801E8124[i].unk24, r, r, r);
                        }
                    }
                } else {
                    D_801E8124[i].unk18--;
                }
            }
        }
        if (work->unk0[1] != 0) {
            work->unk0[1]--;
            for (i = 0; i < 4; i++) {
                if (D_801E8124[i].unk20 == 0) {
                    ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&D_801E8124[i]);
                }
                D_801E8124[i].unk20--;
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_130/nonmatchings/fx_130", fx_130_headerPtr);
