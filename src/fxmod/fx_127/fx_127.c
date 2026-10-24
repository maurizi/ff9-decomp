#include "fxmod/fx_127.h"

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E82F0);
    s32 ret = func_801E7818(out, pos, &D_801E82F8, &D_801E8300, 5, frame, n);

    D_801E8308.vx = pos->vx;
    D_801E8308.vy = 0;
    D_801E8308.vz = pos->vz;
    if (D_801E8340 != NULL) {
        if (D_801E8340->unk30 == -1) {
            D_801E8340 = NULL;
        }
    }
    ex->btlfx_moveEmitterTo((s32)D_801E8340, (s32)&D_801E8308);
    return ret;
}

s32 func_801E77D0(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7818(out, pos, &D_801E82F8, &D_801E8300, 5, frame, n);
}

s32 func_801E7818(VECTOR *out, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 len, s32 frame, s32 n) {
    BTLFX_EXPORTS(D_801E82F0)->btlfx_lerpVec((s16 *)from, (s16 *)to, (frame << 12) / len, (s16 *)pos);
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_127_op80(s32 mode, Fx127Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x84;
    } else if (mode == 1) {
        D_801E82F0 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk24);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk2C);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk34);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk3C);
    } else {
        frame = entry->unk0;
        ex->func_800DC8AC(0x10, 0x3C, (BtlBone *)&m1);
        ex->btlfx_matrixPointAhead(&m1, 0, (s32)&work->unk44);
        ex->btlfx_aimAngles((s16 *)&work->unk44, (s16 *)&work->unk34, (s16 *)&work->unk74);
        switch (frame) {
        case 0xA:
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&D_801E8320);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&D_801E8320);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&D_801E8320);
            work->unk4C.vx = work->unk44.vx;
            work->unk4C.vy = work->unk44.vy;
            work->unk4C.vz = work->unk44.vz;
            work->unk4C.vy = 0;
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk4C);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x20);
            }
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            break;
        case 0x22:
            ex->btlfx_aimAngles((s16 *)&work->unk34, (s16 *)&work->unk44, (s16 *)&work->unk54);
            work->unk5C.vx = work->unk54.vx;
            work->unk5C.vy = work->unk54.vy;
            work->unk5C.vz = work->unk54.vz;
            work->unk5C.vy += 0x800;
            D_801E8310.vx = work->unk5C.vx;
            D_801E8310.vy = work->unk5C.vy;
            D_801E8310.vz = work->unk5C.vz;
            ex->func_800DC8AC(0x10, 0x3C, (BtlBone *)&m2);
            ex->btlfx_matrixPointAhead(&m2, 0, (s32)&work->unk64);
            ex->func_800DC8AC(0x10, 0x3C, (BtlBone *)&m3);
            ex->btlfx_matrixPointAhead(&m3, 0, (s32)&D_801E82F8);
            D_801E8300.vx = work->unk34.vx;
            D_801E8300.vy = work->unk34.vy;
            D_801E8300.vz = work->unk34.vz;
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&D_801E8328);
            work->unk14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&D_801E8330);
            D_801E8340 = work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&D_801E8338);
            if (work->unk10 != NULL) {
                work->unk10->unk14 = (s32)D_801E8350;
                work->unk10->unk12 = 0x112;
                work->unk10->unkC = 0x18;
            }
            if (work->unk14 != NULL) {
                work->unk14->unk14 = (s32)D_801E9D00;
                work->unk14->unk12 = 0x28;
                work->unk14->unkC = 0x40;
            }
            if (work->unk10 != NULL) {
                ((Fx127EffHdr *)work->unk10->unk0)->unk8 = func_801E7704;
            }
            if (work->unk14 != NULL) {
                ((Fx127EffHdr *)work->unk14->unk0)->unk8 = func_801E77D0;
            }
            if (work->unk10 != NULL) {
                work->unk10->unk22 = ex->func_800E7CC4(0, 0x20);
            }
            if (work->unk14 != NULL) {
                work->unk14->unk22 = ex->func_800E7CC4(0, 0x20);
            }
            break;
        case 0x27:
            ex->btlfx_pointAhead((s16 *)&work->unk34, (s16 *)&work->unk5C, -0xC8, (s16 *)&work->unk6C);
            D_801E8318.vx = work->unk6C.vx;
            D_801E8318.vy = work->unk6C.vy;
            D_801E8318.vz = work->unk6C.vz;
            break;
        }
        if (frame >= 0xA && frame < 0xE) {
            if (work->unk0 != NULL && work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk0, (s32)&work->unk44);
            if (work->unk4 != NULL && work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk4, (s32)&work->unk44);
            if (work->unk8 != NULL && work->unk8->unk30 == -1) {
                work->unk8 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk8, (s32)&work->unk44);
            ex->btlfx_aimAngles((s16 *)&work->unk44, (s16 *)&work->unk34, (s16 *)&work->unk74);
            work->unk7C.vx = work->unk74.vx;
            work->unk7C.vy = work->unk74.vy;
            work->unk7C.vz = work->unk74.vz;
            work->unk7C.vy += 0x800;
        }
        if (frame >= 0xA) {
            if (work->unk0 != NULL) {
                if (work->unk0->unk30 == -1) {
                    work->unk0 = NULL;
                }
                if (work->unk0 != NULL) {
                    work->unk0->unk5C.vx = work->unk74.vx;
                    work->unk0->unk5C.vy = work->unk74.vy;
                    work->unk0->unk5C.vz = work->unk74.vz;
                }
            }
            if (work->unk4 != NULL) {
                if (work->unk4->unk30 == -1) {
                    work->unk4 = NULL;
                }
                if (work->unk4 != NULL) {
                    work->unk4->unk5C.vx = work->unk74.vx;
                    work->unk4->unk5C.vy = work->unk74.vy;
                    work->unk4->unk5C.vz = work->unk74.vz;
                }
            }
            if (work->unk8 != NULL) {
                if (work->unk8->unk30 == -1) {
                    work->unk8 = NULL;
                }
                if (work->unk8 != NULL) {
                    work->unk8->unk5C.vx = work->unk7C.vx;
                    work->unk8->unk5C.vy = work->unk7C.vy;
                    work->unk8->unk5C.vz = work->unk7C.vz;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_127_op81(s32 mode, Fx127Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        D_801E82F0 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        work->unk18.vx = D_801E8310.vx;
        work->unk18.vy = D_801E8310.vy;
        work->unk18.vz = D_801E8310.vz;
        work->unk20.vx = D_801E8310.vx;
        work->unk20.vy = D_801E8310.vy;
        work->unk20.vz = D_801E8310.vz;
        work->unk30.vx = work->unk10.vx;
        work->unk30.vy = work->unk10.vy;
        work->unk30.vz = work->unk10.vz;
        work->unk20.vy += 0x800;
        work->unk28.vx = D_801E8318.vx;
        work->unk28.vy = D_801E8318.vy;
        work->unk28.vz = D_801E8318.vz;
        work->unk38.vx = work->unk10.vx;
        work->unk38.vy = work->unk10.vy;
        work->unk38.vz = work->unk10.vz;
        work->unk38.vy = 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk30);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk38);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 4:
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk38);
            break;
        }
        if (frame >= 0) {
            if (work->unk0 != NULL) {
                if (work->unk0->unk30 == -1) {
                    work->unk0 = NULL;
                }
                if (work->unk0 != NULL) {
                    work->unk0->unk5C.vx = work->unk18.vx;
                    work->unk0->unk5C.vy = work->unk18.vy;
                    work->unk0->unk5C.vz = work->unk18.vz;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_127/nonmatchings/fx_127", fx_127_headerPtr);
