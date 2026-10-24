#include "fxmod/fx_023.h"

s32 fx_023_op80(s32 mode, Fx023Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        D_801E7C30 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk14);
        ex->btlfx_getUnitHomePos(0, (s16 *)&work->unk1C);
        work->unk34 = -1;
        work->unk36 = -1;
        work->unk38 = 0;
        work->unk1C.vx = work->unk14.vx;
        work->unk1C.vz = work->unk14.vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 5:
            work->unk34 = 0x1E;
            work->unk24.vx = work->unk1C.vx;
            work->unk24.vy = work->unk1C.vy;
            work->unk24.vz = work->unk1C.vz;
            work->unk2C.vx = work->unk1C.vx;
            work->unk2C.vy = work->unk1C.vy;
            work->unk2C.vz = work->unk1C.vz;
            work->unk2C.vy = -1000;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk24);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk14);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 40:
            work->unk36 = 3;
            work->unk38 = 0;
            work->unk2C.vy = work->unk1C.vy;
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk14);
            break;
        }
        if (work->unk34 >= 0) {
            ex->btlfx_getUnitPos(0, (s16 *)&a);
            a.vy = func_801E7B90(work->unk1C.vy, -1000, 0x1E - work->unk34, 0x1E);
            ex->btlfx_setUnitPos(0, (s16 *)&a);
            if (--work->unk34 == 0) {
                work->unk38 = 1;
            }
        }
        if (work->unk38 != 0) {
            ex->btlfx_getUnitPos(0, (s16 *)&b);
            b.vy = -1000;
            ex->btlfx_setUnitPos(0, (s16 *)&b);
        }
        if (work->unk36 >= 0) {
            ex->btlfx_getUnitPos(0, (s16 *)&c);
            ex->btlfx_getUnitHomePos(0, (s16 *)&work->unk1C);
            work->unk1C.vx = c.vx;
            work->unk1C.vz = c.vz;
            c.vy = func_801E7B90(-1000, work->unk1C.vy, 3 - work->unk36, 3);
            ex->btlfx_setUnitPos(0, (s16 *)&c);
            if (--work->unk36 == 0) {
                ex->btlfx_setUnitPos(0, (s16 *)&work->unk14);
                ex->btlfx_getUnitHomePos(0, (s16 *)&work->unk1C);
                work->unk1C.vx = work->unk14.vx;
                work->unk1C.vz = work->unk14.vz;
                work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk14);
                work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk14);
                if (work->unk10 != NULL) {
                    work->unk10->unk14 = (s32)D_801E7C40;
                    work->unk10->unk12 = 0x3C;
                    work->unk10->unkC = 0x18;
                }
            }
        }
        if (frame >= 5 && work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk2C = (s32)&work->unk2C;
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

void func_801E7B4C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7C30)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7B90(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_023/nonmatchings/fx_023", fx_023_headerPtr);
