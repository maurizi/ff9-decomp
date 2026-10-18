#include "fxmod/fx_1a6.h"

INCLUDE_ASM("asm/us/fx_1a6/nonmatchings/fx_1a6", func_801E7704);

INCLUDE_ASM("asm/us/fx_1a6/nonmatchings/fx_1a6", func_801E7744);

INCLUDE_ASM("asm/us/fx_1a6/nonmatchings/fx_1a6", func_801E7784);

s32 func_801E77C4(Pool30Entry *e, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8298);
    s32 t = frame << 9;

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)pos);
    e->unk0.vx = pos->vx << 12;
    e->unk0.vy = pos->vy << 12;
    e->unk0.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1a6_op80(s32 mode, Fx1a6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x4C;
    } else if (mode == 1) {
        s32 n = *arg4;
        D_801E8298 = env;
        work->unk48 = n;
        if (n >= 3) {
            return 1;
        }
        ex->func_800E9930(2, entry->unk8, -1);
        ex->func_800E9930(4, entry->unk8, -1);
        ex->func_800E9930(7, entry->unk8, -1);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk18);
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk20);
        if (ex->btlfx_rand() & 1) {
            work->unk18.vx += 0x32 + (s32)ex->btlfx_rand % 200;
        } else {
            work->unk18.vx -= 0x32 + (s32)ex->btlfx_rand % 200;
        }
        if (ex->btlfx_rand() & 1) {
            work->unk18.vy += 0x32 + (s32)ex->btlfx_rand % 200;
        } else {
            work->unk18.vy -= 0x32 + (s32)ex->btlfx_rand % 200;
        }
        ex->btlfx_aimAngles((s16 *)&work->unk20, (s16 *)&work->unk18, (s16 *)&work->unk38);
        ex->btlfx_aimAngles((s16 *)&work->unk18, (s16 *)&work->unk20, (s16 *)&work->unk40);
        ex->func_800CFD78((s16 *)&work->unk20, (s16 *)&work->unk38, 0x12C, (s16 *)&work->unk28);
        ex->func_800CFD78((s16 *)&work->unk20, (s16 *)&work->unk38, 0xC8, (s16 *)&D_801EDD28[work->unk48]);
        D_801EDD48[work->unk48].vx = work->unk18.vx;
        D_801EDD48[work->unk48].vy = work->unk18.vy;
        D_801EDD48[work->unk48].vz = work->unk18.vz;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk28);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801E823C[work->unk48]),
                                       (s32)&D_801EDD68[work->unk48]);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E82A8[work->unk48];
            work->unk0->unk12 = 0xF;
            work->unk0->unkC = 0x18;
        }
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E86E8[work->unk48];
            work->unk4->unk12 = 0x1D;
            work->unk4->unkC = 0x40;
        }
        if (work->unk4 != NULL) {
            ((EffMotion *)work->unk4->unk0)->unk8 = D_801E8244[work->unk48];
        }
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x20);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 6:
            if (work->unk48 != 2) {
                func_801E81A4(0);
            }
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk18);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk18);
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E9CA8[work->unk48];
                work->unk8->unk12 = 0x56;
                work->unk8->unkC = 0x40;
            }
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 8:
            ex->func_800E9930(8, entry->unk8, -1);
            ex->func_800E9930(0xD, entry->unk8, -1);
            ex->func_800E9930(0xE, entry->unk8, -1);
            break;
        }
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk40.vx;
                work->unk0->unk5C.vy = work->unk40.vy;
                work->unk0->unk5C.vz = work->unk40.vz;
            }
        }
        if (frame >= 6) {
            if (work->unkC != NULL) {
                if (work->unkC->unk30 == -1) {
                    work->unkC = NULL;
                }
                if (work->unkC != NULL) {
                    work->unkC->unk5C.vx = work->unk38.vx;
                    work->unkC->unk5C.vy = work->unk38.vy;
                    work->unkC->unk5C.vz = work->unk38.vz;
                }
            }
            if (frame >= 6 && work->unk48 != 2 && func_801E8120(0) != 0) {
                func_801E80B4(0, 0);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_1a6_op81(s32 mode, Fx1a6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E8298 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

void func_801E80B4(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8298);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

s32 func_801E8120(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8298);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

void func_801E81A4(s32 unit) {
    if (BTLFX_EXPORTS(D_801E8298)->btlfx_isUnitActive(unit)) {
        func_801E80B4(unit, 2);
    }
}

void func_801E81F0(s32 unit) {
    if (BTLFX_EXPORTS(D_801E8298)->btlfx_isUnitActive(unit)) {
        func_801E80B4(unit, 3);
    }
}

INCLUDE_RODATA("asm/us/fx_1a6/nonmatchings/fx_1a6", fx_1a6_headerPtr);
