#include "fxmod/fx_0b0.h"

s32 fx_0b0_op80(s32 mode, Fx0b0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x4C;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7CD8 = env;
        BTLFX_COPY_SVEC(&work->unkC, &env->unkC->unk38);
        BTLFX_COPY_SVEC(&work->unk14, &work->unkC);
        work->unkC.vy -= 0xF0;
        BTLFX_COPY_SVEC(&work->unk2C, &work->unkC);
        BTLFX_COPY_SVEC(&work->unk34, &work->unkC);
        work->unk2C.vx -= 0x300;
        work->unk34.vx += 0x300;
        ex->func_800DC818(0x10, D_801E7C8C[0], (u16 *)&work->unk1C);
        ex->func_800DC818(0x10, D_801E7C8C[1], (u16 *)&work->unk24);
        work->unk4 = work->unk8 = NULL;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (frame < 0x10) {
                t = ex->math_cos(frame << 7);
                t = (0x1000 - t) >> 1;
            } else {
                t = 0x1000;
            }
            ex->func_800DC818(0x10, D_801E7C8C[0], (u16 *)&work->unk1C);
            ex->func_800DC818(0x10, D_801E7C8C[1], (u16 *)&work->unk24);
            ex->btlfx_lerpVec((s16 *)&work->unk1C, (s16 *)&work->unk2C, t, (s16 *)&a);
            ex->btlfx_lerpVec((s16 *)&work->unk24, (s16 *)&work->unk34, t, (s16 *)&b);
            if (frame == 0) {
                BTLFX_COPY_SVEC(&work->unk3C, &a);
                work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk3C);
                if (work->unk4 != NULL) {
                    work->unk4->unk14 = (s32)D_801E7CE8;
                    work->unk4->unk12 = 0x96;
                    work->unk4->unkC = 0x14;
                }
                if (work->unk4 != NULL) {
                    work->unk4->unk22 = 0x18;
                }
                BTLFX_COPY_SVEC(&work->unk44, &b);
                work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk44);
                if (work->unk8 != NULL) {
                    work->unk8->unk14 = (s32)D_801E88A8;
                    work->unk8->unk12 = 0x96;
                    work->unk8->unkC = 0x14;
                }
                if (work->unk8 != NULL) {
                    work->unk8->unk22 = 0x18;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E9B28;
                    p->unk12 = 0x12;
                    p->unkC = 0x14;
                    p->unk22 = 0x18;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk24);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E9C98;
                    p->unk12 = 0x12;
                    p->unkC = 0x14;
                    p->unk22 = 0x18;
                }
            } else {
                if (work->unk4 != NULL && work->unk4->unk30 == -1) {
                    work->unk4 = NULL;
                }
                ex->func_800E3D20((s32)work->unk4, (s32)&a);
                if (work->unk8 != NULL && work->unk8->unk30 == -1) {
                    work->unk8 = NULL;
                }
                ex->func_800E3D20((s32)work->unk8, (s32)&b);
            }
            if (frame == 0xE) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk14);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E9468;
                    p->unk12 = 0x10;
                    p->unkC = 0x6C;
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk2C);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E9E08;
                    p->unk12 = 0x12;
                    p->unkC = 0x14;
                    p->unk22 = 0x18;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk34);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E9F78;
                    p->unk12 = 0x12;
                    p->unkC = 0x14;
                    p->unk22 = 0x18;
                }
            }
            if (frame >= 0x37) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b0/nonmatchings/fx_0b0", fx_0b0_headerPtr);
