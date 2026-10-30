#include "fxmod/fx_03a.h"

s32 fx_03a_op80(s32 mode, Fx03aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 t;
    s32 y;
    Pool6CEntry *p;
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        D_801E7DA8 = env;
        ex->btlfx_initEffModels(D_801E7FC8, 0xA);
        for (i = 7; i >= 0; i--) {
            ((s32 *)work)[i] = 0;
        }
        work->unk20.vx = 0;
        work->unk20.vy = 0;
        work->unk20.vz = -0x550;
        D_801E7DB8[0].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 0xFF);
        D_801E7DB8[1].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 2), 0xFF);
        D_801E7DB8[2].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 3), 0xFF);
        D_801E7DB8[3].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 4), 0xFF);
        D_801E7DB8[4].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 5), 0xFF);
        D_801E7DB8[5].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 6), 0xFF);
        D_801E7DB8[6].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 0xFF);
        D_801E7DB8[7].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 8), 0xFF);
        D_801E7DB8[8].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 1), 0xFF);
        D_801E7DB8[9].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xE), 0xFF);
        for (i = 0; i < 9; i++) {
            setVector(&D_801E7DB8[i].unk24, 0, 0, 0);
            setVector(&D_801E7DB8[i].unk4, 0xF00, 0, 0xF00);
            setVector(&D_801E7DB8[i].unk14, 0, 0, -0x550);
            D_801E7DB8[i].unk2C = ex->btlfx_rand() & 0x3F;
            D_801E7DB8[i].unk30 = -(ex->btlfx_rand() & 7);
        }
        D_801E7FC0 = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x100, 0x100, 0x100, D_801E8798, 0, -0x550, -0x1000);
            break;
        case 1:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->unk20);
            if (p != NULL) {
                p->otZ = 0x100;
            }
            work->unkC = 0x40;
            work->unk0 = 0x40;
            work->unk4 = 0x10;
            break;
        case 33:
            work->unk8 = 0x20;
            break;
        }
        if (work->unk4 != 0) {
            work->unk4--;
            D_801E7FC0 += 0x3C;
        }
        if (work->unk8 != 0) {
            work->unk8--;
            D_801E7FC0 -= 0x1E;
        }
        if (work->unk0 != 0) {
            work->unk0--;
            for (i = 0; i < 9; i++) {
                t = ++D_801E7DB8[i].unk30;
                if (t >= 0) {
                    y = D_801E7DB8[i].unk2C * t - ((t * (t << 8)) >> 1);
                    if (y < 0) {
                        y = 0;
                        D_801E7DB8[i].unk2C = ex->btlfx_rand() % (D_801E7FC0 + 1);
                        if (work->unk0 < 8) {
                            D_801E7DB8[i].unk30 = -100;
                        } else {
                            D_801E7DB8[i].unk30 = 0;
                        }
                    }
                    D_801E7DB8[i].unk4.vy = y;
                    ex->Hi_DrawEffModel(&D_801E7DB8[i].unk24, &D_801E7DB8[i].unk14, (s32)&D_801E7DB8[i].unk4,
                                        D_801E7DB8[i].unk0);
                }
            }
        }
        if (work->unkC != 0) {
            work->unkC--;
            /* the assignment inside the sum keeps 0x1000 in one register for all three fields */
            scale.vy = work->unkC * 2 + (scale.vx = 0x1000);
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            c = D_801E7FC0 / 15;
            col.b = c;
            col.g = c;
            col.r = c;
            ex->btlfx_drawScreenGrid(5, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, NULL);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 4), 2, c, c, c);
        }
        return frame >= 0x58;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_03a/nonmatchings/fx_03a", fx_03a_headerPtr);
