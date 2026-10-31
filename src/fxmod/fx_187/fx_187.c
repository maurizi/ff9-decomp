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
        fx_187_env = env;
        BTLFX_COPY_SVEC(&work->pos14, &env->targets->centerX);
        BTLFX_COPY_SVEC(&work->pos1C, &work->pos14);
        work->pos14.vy -= 0xF0;
        BTLFX_COPY_SVEC(&work->unk34, &work->pos14);
        BTLFX_COPY_SVEC(&work->unk3C, &work->pos14);
        work->unk34.vx -= 0x300;
        work->unk3C.vx += 0x300;
        ex->btlfx_getUnitBonePos(0x10, D_801E7DB8[0], (u16 *)&work->pos24);
        ex->btlfx_getUnitBonePos(0x10, D_801E7DB8[1], (u16 *)&work->pos2C);
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBonePos(0x10, D_801E7DB8[0], (u16 *)&work->pos24);
        ex->btlfx_getUnitBonePos(0x10, D_801E7DB8[1], (u16 *)&work->pos2C);
        switch (work->unk0) {
        case 0:
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos24);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos2C);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
            }
            if (frame >= 0x10) {
                work->unk0 = 1;
                entry->frame = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos24);
                if (p != NULL) {
                    p->trails = (s32)fx_187_op80Trails0;
                    p->trailSlots = 0x12;
                    p->trailStride = 0x14;
                    p->otZ = 0x46;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos24);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos2C);
                if (p != NULL) {
                    p->trails = (s32)fx_187_op80Trails1;
                    p->trailSlots = 0x12;
                    p->trailStride = 0x14;
                    p->otZ = 0x46;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos2C);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
            }
            break;
        case 1:
            if (frame < 0x10) {
                t = (0x1000 - ex->math_cos(frame << 7)) >> 1;
            } else {
                t = 0x1000;
            }
            ex->btlfx_lerpVec((s16 *)&work->pos24, (s16 *)&work->unk34, t, (s16 *)&a);
            ex->btlfx_lerpVec((s16 *)&work->pos2C, (s16 *)&work->unk3C, t, (s16 *)&b);
            if (frame == 0) {
                BTLFX_COPY_SVEC(&work->pos44, &a);
                work->emit4[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos44);
                if (work->emit4[0] != NULL) {
                    work->emit4[0]->trails = (s32)fx_187_op80Trails2;
                    work->emit4[0]->trailSlots = 0x20;
                    work->emit4[0]->trailStride = 0x14;
                }
                if (work->emit4[0] != NULL) {
                    work->emit4[0]->otZ = 0x18;
                }
                BTLFX_COPY_SVEC(&work->pos4C, &a);
                work->emit4[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos4C);
                if (work->emit4[1] != NULL) {
                    work->emit4[1]->otZ = 0x18;
                }
                BTLFX_COPY_SVEC(&work->pos54, &b);
                work->emitC[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos54);
                if (work->emitC[0] != NULL) {
                    work->emitC[0]->trails = (s32)fx_187_op80Trails3;
                    work->emitC[0]->trailSlots = 0x20;
                    work->emitC[0]->trailStride = 0x14;
                }
                if (work->emitC[0] != NULL) {
                    work->emitC[0]->otZ = 0x18;
                }
                BTLFX_COPY_SVEC(&work->pos5C, &b);
                work->emitC[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos5C);
                if (work->emitC[1] != NULL) {
                    work->emitC[1]->otZ = 0x18;
                }
            } else {
                for (i = 0; i < 2; i++) {
                    if (work->emit4[i] != NULL && work->emit4[i]->state == BTLFX_EMIT_DONE) {
                        work->emit4[i] = NULL;
                    }
                    ex->btlfx_moveEmitterTo((s32)work->emit4[i], (s32)&a);
                    if (work->emitC[i] != NULL && work->emitC[i]->state == BTLFX_EMIT_DONE) {
                        work->emitC[i] = NULL;
                    }
                    ex->btlfx_moveEmitterTo((s32)work->emitC[i], (s32)&b);
                }
            }
            if (frame == 0xE) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos14);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos1C);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos14);
                if (p != NULL) {
                    p->trails = (s32)fx_187_op80Trails4;
                    p->trailSlots = 7;
                    p->trailStride = 0x6C;
                    p->otZ = 0x80;
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
