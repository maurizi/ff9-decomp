#include "fxmod/fx_187.h"

s32 fx_187_op80(s32 mode, Fx187Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x64;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7E04 = env;
        BTLFX_COPY_SVEC(&work->unk14, &env->targets->centerX);
        BTLFX_COPY_SVEC(&work->unk1C, &work->unk14);
        work->unk14.vy -= 0xF0;
        BTLFX_COPY_SVEC(&work->unk34, &work->unk14);
        BTLFX_COPY_SVEC(&work->unk3C, &work->unk14);
        work->unk34.vx -= 0x300;
        work->unk3C.vx += 0x300;
        ex->btlfx_getUnitBonePos(0x10, D_801E7DB8[0], (u16 *)&work->unk24);
        ex->btlfx_getUnitBonePos(0x10, D_801E7DB8[1], (u16 *)&work->unk2C);
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBonePos(0x10, D_801E7DB8[0], (u16 *)&work->unk24);
        ex->btlfx_getUnitBonePos(0x10, D_801E7DB8[1], (u16 *)&work->unk2C);
        switch (work->unk0) {
        case 0:
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk24);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk2C);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame >= 0x10) {
                work->unk0 = 1;
                entry->frame = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk24);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E7E14;
                    p->unk12 = 0x12;
                    p->unkC = 0x14;
                    p->unk22 = 0x46;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk24);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk2C);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E7F7C;
                    p->unk12 = 0x12;
                    p->unkC = 0x14;
                    p->unk22 = 0x46;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk2C);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            break;
        case 1:
            if (frame < 0x10) {
                t = (0x1000 - ex->math_cos(frame << 7)) >> 1;
            } else {
                t = 0x1000;
            }
            ex->btlfx_lerpVec((s16 *)&work->unk24, (s16 *)&work->unk34, t, (s16 *)&a);
            ex->btlfx_lerpVec((s16 *)&work->unk2C, (s16 *)&work->unk3C, t, (s16 *)&b);
            if (frame == 0) {
                BTLFX_COPY_SVEC(&work->unk44, &a);
                work->unk4[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk44);
                if (work->unk4[0] != NULL) {
                    work->unk4[0]->unk14 = (s32)D_801E80E4;
                    work->unk4[0]->unk12 = 0x20;
                    work->unk4[0]->unkC = 0x14;
                }
                if (work->unk4[0] != NULL) {
                    work->unk4[0]->unk22 = 0x18;
                }
                BTLFX_COPY_SVEC(&work->unk4C, &a);
                work->unk4[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unk4C);
                if (work->unk4[1] != NULL) {
                    work->unk4[1]->unk22 = 0x18;
                }
                BTLFX_COPY_SVEC(&work->unk54, &b);
                work->unkC[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk54);
                if (work->unkC[0] != NULL) {
                    work->unkC[0]->unk14 = (s32)D_801E8364;
                    work->unkC[0]->unk12 = 0x20;
                    work->unkC[0]->unkC = 0x14;
                }
                if (work->unkC[0] != NULL) {
                    work->unkC[0]->unk22 = 0x18;
                }
                BTLFX_COPY_SVEC(&work->unk5C, &b);
                work->unkC[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->unk5C);
                if (work->unkC[1] != NULL) {
                    work->unkC[1]->unk22 = 0x18;
                }
            } else {
                for (i = 0; i < 2; i++) {
                    if (work->unk4[i] != NULL && work->unk4[i]->unk30 == -1) {
                        work->unk4[i] = NULL;
                    }
                    ex->btlfx_moveEmitterTo((s32)work->unk4[i], (s32)&a);
                    if (work->unkC[i] != NULL && work->unkC[i]->unk30 == -1) {
                        work->unkC[i] = NULL;
                    }
                    ex->btlfx_moveEmitterTo((s32)work->unkC[i], (s32)&b);
                }
            }
            if (frame == 0xE) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk1C);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E85E4;
                    p->unk12 = 7;
                    p->unkC = 0x6C;
                    p->unk22 = 0x80;
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

INCLUDE_RODATA("asm/us/fx_187/nonmatchings/fx_187", fx_187_headerPtr);
