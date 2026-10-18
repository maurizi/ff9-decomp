#include "fxmod/fx_03d.h"

s32 fx_03d_op80(s32 mode, Fx03dWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E7E60 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk8);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk10.vx;
                work->unk0->unk5C.vy = work->unk10.vy;
                work->unk0->unk5C.vz = work->unk10.vz;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk10.vx;
                work->unk4->unk5C.vy = work->unk10.vy;
                work->unk4->unk5C.vz = work->unk10.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_03d_op81(s32 mode, Fx03dWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk18.vx = 0;
        work->unk18.vy = 0;
        work->unk18.vz = 0;
        D_801E7E60 = env;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk18);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E8000;
            work->unk0->unk12 = 9;
            work->unk0->unkC = 0x88;
        }
    } else {
        frame = entry->unk0;
        if (frame == 0x10) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk18);
        }
        if (frame == 0x28) {
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk18);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk18);
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_03d_op82(s32 mode, Fx03dWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 s;

    if (mode == 0) {
        *arg4 = 0x44;
    } else if (mode == 1) {
        D_801E7E60 = env;
        ex->btlfx_initEffModels(D_801E7E70, 2);
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
        work->unk0.vx = 0;
        work->unk0.vy = 0;
        work->unk0.vz = 0;
        work->unk18.vx = 0x1000;
        work->unk18.vy = 0x1000;
        work->unk18.vz = 0x1000;
        work->unk28.vx = 0x1000;
        work->unk28.vy = 0x1000;
        work->unk28.vz = 0x1000;
        work->unk40 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x17), 1);
        work->unk42 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x16), 1);
        work->unk3C = -1;
        work->unk3E = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk3C = 0xF;
            break;
        case 0x19:
            work->unk3E = 0xF;
            break;
        }
        if (frame >= 0 && frame < 0x28) {
            if (frame >= 0 && frame < 0xF) {
                s = func_801E7DC0(0, 0x1800, frame, 0xF);
                work->unk18.vx = s;
                work->unk18.vy = s;
                work->unk18.vz = s;
                work->unk28.vx = s;
                work->unk28.vy = s;
                work->unk28.vz = s;
            }
            if (frame >= 0xF && frame < 0x28) {
                s = func_801E7DC0(0x1800, 0x2000, frame - 0xF, 0x19);
                work->unk18.vx = s;
                work->unk18.vz = s;
                s = func_801E7DC0(0x1800, 0x2800, frame - 0xF, 0x19);
                work->unk28.vx = s;
                work->unk28.vz = s;
            }
            ex->Hi_DrawEffModel(&work->unk0, &work->unk8, (s32)&work->unk18, work->unk40);
            ex->Hi_DrawEffModel(&work->unk0, &work->unk8, (s32)&work->unk28, work->unk42);
            if (work->unk3C >= 0 || work->unk3E >= 0) {
                if (work->unk3C >= 0) {
                    work->unk38 = func_801E7DC0(-0x80, 0, 0xF - work->unk3C, 0xF);
                    work->unk3C--;
                }
                if (work->unk3E >= 0) {
                    work->unk38 = func_801E7DC0(0, -0x80, 0xF - work->unk3E, 0xF);
                    work->unk3E--;
                }
                ex->Hi_ModifyEffModelRGB(work->unk40, work->unk38, work->unk38, work->unk38);
                ex->Hi_ModifyEffModelRGB(work->unk42, work->unk38, work->unk38, work->unk38);
            }
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E7D7C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7E60)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7DC0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_03d/nonmatchings/fx_03d", fx_03d_headerPtr);
