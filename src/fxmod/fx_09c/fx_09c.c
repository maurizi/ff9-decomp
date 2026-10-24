#include "fxmod/fx_09c.h"

s32 fx_09c_op80(s32 mode, Fx09cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    BtlBone bone;
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        work->unk20 = NULL;
        work->unk3C = NULL;
        D_801E7BB8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk18);
        work->unk0.vx = work->unk10.vx + ex->btlfx_randRange(-0x100, 0x100);
        work->unk0.vy = work->unk10.vy + ex->btlfx_randRange(-0x100, 0x100);
        work->unk0.vz = work->unk10.vz + ex->btlfx_randRange(-0x100, 0x100);
        work->unk8.vx = work->unk10.vx + ex->btlfx_randRange(-0x100, 0x100);
        work->unk8.vy = work->unk10.vy + ex->btlfx_randRange(-0x100, 0x100);
        work->unk8.vz = work->unk10.vz + ex->btlfx_randRange(-0x100, 0x100);
    } else {
        frame = entry->unk0;
        if (frame == 4) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (frame == 0xA) {
            ex->btlfx_getUnitBoneMatrix(0x10, 0x11, &bone);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone, 0x800, (s32)&a);
            ex->btlfx_getUnitBonePos(0x10, 0x11, (u16 *)&b);
            ex->btlfx_aimAngles((s16 *)&a, (s16 *)&b, (s16 *)&work->unk34);
            work->unk34.vy -= 0xB4;
            ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&a);
            work->unk2C.vx = a.vx - ((ex->math_sin(work->unk34.vy) * 0x1E0) >> 12);
            work->unk2C.vz = a.vz - ((ex->math_cos(work->unk34.vy) * 0x1E0) >> 12);
            work->unk2C.vy = a.vy - ((ex->math_sin(work->unk34.vx) * 0x1E0) >> 12);
            work->unk20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk2C);
            work->unk3C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk2C);
        }
        if (frame == 0x12) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk18);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (work->unk20 != NULL) {
            if (work->unk20->unk30 == -1) {
                work->unk20 = NULL;
            }
            if (work->unk20 != NULL) {
                work->unk20->unk5C.vx = work->unk34.vx;
                work->unk20->unk5C.vy = work->unk34.vy;
                work->unk20->unk5C.vz = work->unk34.vz;
            }
        }
        if (work->unk3C != NULL) {
            if (work->unk3C->unk30 == -1) {
                work->unk3C = NULL;
            }
            if (work->unk3C != NULL) {
                work->unk3C->unk5C.vx = work->unk34.vx;
                work->unk3C->unk5C.vy = work->unk34.vy;
                work->unk3C->unk5C.vz = work->unk34.vz;
            }
        }
        return frame >= 0x2D;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_09c/nonmatchings/fx_09c", fx_09c_headerPtr);
