#include "fxmod/fx_12c.h"

INCLUDE_RODATA("asm/us/fx_12c/nonmatchings/fx_12c", fx_12c_headerPtr);

s32 fx_12c_op80(s32 mode, Fx12cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    CVECTOR col;
    SVECTOR rot;
    VECTOR sc;
    s32 *q;
    s32 frame;
    s32 t;
    s32 y;
    s32 c;
    s32 n;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        D_801E7EF8 = env;
        ex->btlfx_initEffModels(D_801E8118, 10);
        for (i = 7, q = &work->unk0[7]; i >= 0; i--) {
            *q-- = 0;
        }
        work->unk20.vx = 0;
        work->unk20.vy = 0;
        work->unk20.vz = -0x550;
        D_801E7F08[0].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 9), 0xFF);
        D_801E7F08[1].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 2), 0xFF);
        D_801E7F08[2].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 3), 0xFF);
        D_801E7F08[3].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 4), 0xFF);
        D_801E7F08[4].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 5), 0xFF);
        D_801E7F08[5].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 6), 0xFF);
        D_801E7F08[6].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 7), 0xFF);
        D_801E7F08[7].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 8), 0xFF);
        D_801E7F08[8].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 0xFF);
        D_801E7F08[9].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xE), 0xFF);
        for (i = 0; i < 9; i++) {
            setVector(&D_801E7F08[i].unk24, 0, 0, 0);
            setVector(&D_801E7F08[i].unk4, 0xF00, 0, 0xF00);
            setVector(&D_801E7F08[i].unk14, 0, 0, -0x550);
            D_801E7F08[i].unk2C = ex->btlfx_rand() & 0x3F;
            D_801E7F08[i].unk30 = -(ex->btlfx_rand() & 7);
        }
        D_801E8110 = 0;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)env->camera);
        env->camera->points[0].vy = 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x100, 0x100, 0x100, D_801E88E8, 0, -0x550, -0x1000);
            break;
        case 1:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk28);
            work->unk28.vy = 0;
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk28);
            if (p != NULL) {
                p->unk22 = -0x100;
            }
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk28);
            break;
        case 9:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk20);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            work->unk0[3] = 0x40;
            work->unk0[1] = 0x10;
            break;
        case 17:
            work->unk0[0] = 0x38;
            break;
        case 41:
            work->unk0[2] = 0x20;
            break;
        }
        if (work->unk0[1] != 0) {
            work->unk0[1]--;
            D_801E8110 += 0x3C;
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            D_801E8110 -= 0x1E;
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            for (i = 0; i < 9; i++) {
                if ((t = ++D_801E7F08[i].unk30) >= 0) {
                    y = D_801E7F08[i].unk2C * t - ((t * (t << 8)) >> 1);
                    if (y < 0) {
                        y = 0;
                        D_801E7F08[i].unk2C = ex->btlfx_rand() % (D_801E8110 + 1);
                        if (work->unk0[0] < 8) {
                            D_801E7F08[i].unk30 = -100;
                        } else {
                            D_801E7F08[i].unk30 = 0;
                        }
                    }
                    D_801E7F08[i].unk4.vy = y;
                    ex->Hi_DrawEffModel(&D_801E7F08[i].unk24, &D_801E7F08[i].unk14, (s32)&D_801E7F08[i].unk4,
                                        D_801E7F08[i].unk0);
                }
            }
        }
        if (work->unk0[3] != 0) {
            n = --work->unk0[3];
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vx = 0x1000;
            sc.vy = (n << 1) + 0x1000;
            sc.vz = 0x1000;
            c = D_801E8110 / 15;
            col.r = col.g = col.b = c;
            ex->btlfx_drawScreenGrid(5, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &sc, &col, 1, NULL);
            ex->btlfx_drawScreenTint((u32 *)&env->ot[4], 2, c, c, c);
        }
        return frame >= 0x80;
    }
    return 0;
}
