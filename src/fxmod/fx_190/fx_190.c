#include "fxmod/fx_190.h"

s32 fx_190_op80(s32 mode, Fx190Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    SVECTOR *v;

    if (mode == 0) {
        *arg4 = 0x74;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7AF8 = env;
        ex->btlfx_getUnitBonePos(0x10, 0x1C, (u16 *)&work->unk44);
        ex->btlfx_getUnitBonePos(0x10, 0x2C, (u16 *)&work->unk4C);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk34);
        work->unk3C.vx = env->targets->centerX;
        work->unk3C.vz = env->targets->centerZ;
        work->unk3C.vy = 0;
        ex->btlfx_aimAngles((s16 *)&work->unk34, (s16 *)&work->unk3C, (s16 *)&work->unk4);
        ex->btlfx_aimAngles((s16 *)&work->unk44, (s16 *)&work->unk3C, (s16 *)&work->unkC);
        work->unk14[0].vy = 0x80;
        work->unk14[1].vy = -0x80;
        work->unk14[2].vy = 0xC0;
        work->unk14[3].vy = -0xC0;
        work->unk14[0].vx = 0x80;
        work->unk14[1].vx = 0x40;
        work->unk14[2].vx = 0x80;
        work->unk14[3].vx = 0x40;
        work->unkC.vy = work->unk4.vy;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
        case 0x12:
        case 0x24:
        case 0x36:
            work->unk54[work->unk0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk44);
            work->unk64[work->unk0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4C);
            work->unk14[work->unk0].vy += work->unkC.vy;
            work->unk14[work->unk0].vx += work->unkC.vx;
            work->unk14[work->unk0].vz += work->unkC.vz;
            work->unk0++;
            break;
        }
        if (frame == 0xF) {
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk3C);
        }
        for (i = 0; i < work->unk0; i++) {
            if (work->unk54[i] != NULL && work->unk54[i]->unk30 == -1) {
                work->unk54[i] = NULL;
            }
            if (work->unk64[i] != NULL && work->unk64[i]->unk30 == -1) {
                work->unk64[i] = NULL;
            }
            if (work->unk54[i] != NULL) {
                v = (SVECTOR *)((u8 *)work + i * 8 + 0x14);
                work->unk54[i]->unk5C.vx = v->vx;
                work->unk54[i]->unk5C.vy = v->vy;
                work->unk54[i]->unk5C.vz = v->vz;
            }
            if (work->unk64[i] != NULL) {
                v = (SVECTOR *)((u8 *)work + i * 8 + 0x14);
                work->unk64[i]->unk5C.vx = v->vx;
                work->unk64[i]->unk5C.vy = v->vy;
                work->unk64[i]->unk5C.vz = v->vz;
            }
        }
        return frame >= 0x60;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_190/nonmatchings/fx_190", fx_190_headerPtr);
