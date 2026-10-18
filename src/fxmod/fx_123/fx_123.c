#include "fxmod/fx_123.h"

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E77B4);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E77E8);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E781C);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E7850);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E7884);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E78B8);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E78EC);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E7920);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E7954);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E7988);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E79BC);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E79F0);

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E7A24);

INCLUDE_RODATA("asm/us/fx_123/nonmatchings/fx_123", fx_123_headerPtr);

s32 fx_123_op80(s32 mode, Fx123Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone m;
    Fx123Bone *b;
    Pool6CEntry *p;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x8C;
    } else if (mode == 1) {
        D_801E90CC = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk4C);
        work->unk54.vx = work->unk4C.vx;
        work->unk54.vy = work->unk4C.vy;
        work->unk54.vz = work->unk4C.vz;
        work->unk54.vy -= 0xC8;
        work->unk54.vz -= 0x190;
    } else {
    frame = entry->unk0;
    switch (frame) {
    case 0:
        for (i = 0; i < 6; i++) {
            work->unk0[i] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&D_801E9BFC[i]);
            if (p != NULL) {
                p->unk22 = 0x20;
            }
        }
        break;
    case 20:
        for (i = 0; i < 6; i++) {
            work->unk18[i] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&D_801E9BFC[i]);
            if (p != NULL) {
                p->unk22 = 0x20;
            }
        }
        break;
    case 30:
        for (i = 0; i < 6; i++) {
            work->unk30[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&D_801E9C2C[i]);
        }
        break;
    case 43:
        work->unk48 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk54);
        break;
    case 38:
        for (i = 0; i < 6; i++) {
            ex->func_800E3D54(work->unk18[i]);
        }
        break;
    }
    if (frame >= 0 && frame < 0x12) {
        b = D_801E8F64;
        for (i = 0; i < 6; i++, b++) {
            ex->func_800DC8AC(0x10, b->unk0, &m);
            ex->func_800CFE88((MATRIX *)&m, b->unk2, (s32)&work->unk5C[i]);
            if (work->unk0[i] != NULL && work->unk0[i]->unk30 == -1) {
                work->unk0[i] = NULL;
            }
            ex->func_800E3D20((s32)work->unk0[i], (s32)&work->unk5C[i]);
        }
    }
    if (frame >= 0x14 && frame < 0x26) {
        b = D_801E8F64;
        for (i = 0; i < 6; i++, b++) {
            ex->func_800DC8AC(0x10, b->unk0, &m);
            ex->func_800CFE88((MATRIX *)&m, b->unk2, (s32)&work->unk5C[i]);
            if (work->unk18[i] != NULL && work->unk18[i]->unk30 == -1) {
                work->unk18[i] = NULL;
            }
            ex->func_800E3D20((s32)work->unk18[i], (s32)&work->unk5C[i]);
        }
    }
    if (frame >= 0x1E && frame < 0x37) {
        b = D_801E8F64;
        for (i = 0; i < 6; i++, b++) {
            ex->func_800DC8AC(0x10, b->unk0, &m);
            ex->func_800CFE88((MATRIX *)&m, b->unk2, (s32)&work->unk5C[i]);
            if (work->unk30[i] != NULL && work->unk30[i]->unk30 == -1) {
                work->unk30[i] = NULL;
            }
            ex->func_800E3D20((s32)work->unk30[i], (s32)&work->unk5C[i]);
        }
    }
    return frame >= 0x50;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", fx_123_op81);

s32 fx_123_op82(s32 mode, Fx123Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        work->unk14.vx = env->unkC->unk38;
        work->unk14.vy = env->unkC->unk3A;
        D_801E90CC = env;
        work->unk14.vz = env->unkC->unk3C;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk14);
            work->unk0[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk14);
            work->unk0[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x20), (s32)&work->unk14);
            break;
        case 0xC:
            work->unk0[4] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk14);
            work->unk0[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk14);
            break;
        }
        return frame >= 0x1E;
    }
    return 0;
}

s32 fx_123_op83(s32 mode, Fx123Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E90CC = env;
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
            func_801E8ECC(0, 0xFF, 0x10 - work->unk0, 0x10, 1, 0);
            work->unk0--;
        } else if (work->unk2 >= 0) {
            func_801E8ECC(0xFF, 0, 0x10 - work->unk2, 0x10, 1, 0);
            work->unk2--;
        } else {
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x20;
    }
    return 0;
}

void func_801E8D84(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E90CC)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8DC8(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8E20(Fx123Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx123Point *)data;
}

INCLUDE_ASM("asm/us/fx_123/nonmatchings/fx_123", func_801E8E38);

void func_801E8ECC(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E90CC)->btlfx_drawScreenTint((u32 *)(D_801E90CC->unk20 + ot), arg4, c, c, c);
}
