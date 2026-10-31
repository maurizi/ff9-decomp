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
        fx_0b0_env = env;
        BTLFX_COPY_SVEC(&work->posC, &env->targets->centerX);
        BTLFX_COPY_SVEC(&work->pos14, &work->posC);
        work->posC.vy -= 0xF0;
        BTLFX_COPY_SVEC(&work->pos2C, &work->posC);
        BTLFX_COPY_SVEC(&work->pos34, &work->posC);
        work->pos2C.vx -= 0x300;
        work->pos34.vx += 0x300;
        ex->btlfx_getUnitBonePos(0x10, D_801E7C8C[0], (u16 *)&work->pos1C);
        ex->btlfx_getUnitBonePos(0x10, D_801E7C8C[1], (u16 *)&work->pos24);
        work->emit4 = work->emit8 = NULL;
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            if (frame < 0x10) {
                t = ex->math_cos(frame << 7);
                t = (0x1000 - t) >> 1;
            } else {
                t = 0x1000;
            }
            ex->btlfx_getUnitBonePos(0x10, D_801E7C8C[0], (u16 *)&work->pos1C);
            ex->btlfx_getUnitBonePos(0x10, D_801E7C8C[1], (u16 *)&work->pos24);
            ex->btlfx_lerpVec((s16 *)&work->pos1C, (s16 *)&work->pos2C, t, (s16 *)&a);
            ex->btlfx_lerpVec((s16 *)&work->pos24, (s16 *)&work->pos34, t, (s16 *)&b);
            if (frame == 0) {
                BTLFX_COPY_SVEC(&work->pos3C, &a);
                work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos3C);
                if (work->emit4 != NULL) {
                    work->emit4->trails = (s32)D_801E7CE8;
                    work->emit4->trailSlots = 0x96;
                    work->emit4->trailStride = 0x14;
                }
                if (work->emit4 != NULL) {
                    work->emit4->otZ = 0x18;
                }
                BTLFX_COPY_SVEC(&work->pos44, &b);
                work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos44);
                if (work->emit8 != NULL) {
                    work->emit8->trails = (s32)D_801E88A8;
                    work->emit8->trailSlots = 0x96;
                    work->emit8->trailStride = 0x14;
                }
                if (work->emit8 != NULL) {
                    work->emit8->otZ = 0x18;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos1C);
                if (p != NULL) {
                    p->trails = (s32)D_801E9B28;
                    p->trailSlots = 0x12;
                    p->trailStride = 0x14;
                    p->otZ = 0x18;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos24);
                if (p != NULL) {
                    p->trails = (s32)D_801E9C98;
                    p->trailSlots = 0x12;
                    p->trailStride = 0x14;
                    p->otZ = 0x18;
                }
            } else {
                if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&a);
                if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
                    work->emit8 = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit8, (s32)&b);
            }
            if (frame == 0xE) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->posC);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos14);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->posC);
                if (p != NULL) {
                    p->trails = (s32)D_801E9468;
                    p->trailSlots = 0x10;
                    p->trailStride = 0x6C;
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos2C);
                if (p != NULL) {
                    p->trails = (s32)D_801E9E08;
                    p->trailSlots = 0x12;
                    p->trailStride = 0x14;
                    p->otZ = 0x18;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos34);
                if (p != NULL) {
                    p->trails = (s32)D_801E9F78;
                    p->trailSlots = 0x12;
                    p->trailStride = 0x14;
                    p->otZ = 0x18;
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
