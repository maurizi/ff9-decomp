#include "fxmod/fx_1b2.h"

s32 fx_1b2_op80(s32 mode, Fx1b2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x70;
    } else if (mode == 1) {
        work->unk18.vx = 0;
        work->unk18.vy = 0;
        work->unk18.vz = -0x44C;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk28.vx = 0x1000;
        work->unk28.vy = 0x1000;
        work->unk28.vz = 0x1000;
        work->unk54.vx = work->unk18.vx;
        work->unk54.vy = work->unk18.vy;
        work->unk54.vz = work->unk18.vz;
        D_801E84EC = env;
        func_801E8208(&D_801E8754, (u8 *)ex->btlfx_getFileData(entry->unk8, 1));
        work->unk6C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0x117, 0, 0, 0);
        work->unk3C.vx = -0x200;
        work->unk3C.vy = 0;
        work->unk3C.vz = 0;
        work->unk64 = 0;
        work->unk68 = 0;
        work->unk5C = 1;
        work->unk60 = -1;
        work->unk4 = NULL;
        work->unk8 = NULL;
    } else {
        frame = entry->unk0;
        if (!(frame & 0xF)) {
            work->unk5C = 0xB;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk54);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk54);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = 0x200;
            }
            if (work->unkC != NULL) {
                work->unkC->unk22 = 0x180;
            }
            for (i = 0; i < env->targets->unk24; i++) {
                if (work->unk64 != 2) {
                    func_801E840C(i);
                }
            }
            work->unk64++;
            if (!(frame & 0xF)) {
                ex->func_800DC818(0x10, 0xC, (u16 *)&work->unk44);
                ex->func_800DC818(0x10, 0x29, (u16 *)&work->unk4C);
                work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk44);
                work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk4C);
                if (work->unk4 != NULL) {
                    work->unk4->unk14 = (s32)D_801E8B5C[work->unk68];
                    work->unk4->unk12 = 0x3C;
                    work->unk4->unkC = 0x28;
                }
                if (work->unk8 != NULL) {
                    work->unk8->unk14 = (s32)D_801EA77C[work->unk68];
                    work->unk8->unk12 = 0x3C;
                    work->unk8->unkC = 0x28;
                }
                work->unk68++;
            }
        }
        if (work->unk5C >= 0) {
            t = 0xB - work->unk5C;
            if (t == 8) {
                work->unk60 = 4;
            }
            if (work->unk60 >= 0) {
                work->unk38 = func_801E81B0(0, -0x80, 4 - work->unk60, 4);
                work->unk60--;
            }
            func_801E8220(&D_801E8754, t, &work->unk28);
            ex->Hi_DrawEffModel(&work->unk10, &work->unk18, (s32)&work->unk28, work->unk6C);
            ex->Hi_ModifyEffModelAbr(work->unk6C, 1);
            if (work->unk60 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->unk6C, work->unk38, work->unk38, work->unk38);
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 3), t);
            for (i = 0; i < env->targets->unk24; i++) {
                if (func_801E8388(i) != 0) {
                    func_801E831C(i, 0);
                }
            }
            work->unk5C--;
        }
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk3C.vx;
                work->unk4->unk5C.vy = work->unk3C.vy;
                work->unk4->unk5C.vz = work->unk3C.vz;
            }
        }
        if (work->unk8 != NULL) {
            if (work->unk8->unk30 == -1) {
                work->unk8 = NULL;
            }
            if (work->unk8 != NULL) {
                work->unk8->unk5C.vx = work->unk3C.vx;
                work->unk8->unk5C.vy = work->unk3C.vy;
                work->unk8->unk5C.vz = work->unk3C.vz;
            }
        }
        return frame >= 0x2C;
    }
    return 0;
}

s32 fx_1b2_op81(s32 mode, Fx1b2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E84EC = env;
        ex->btlfx_initEffModels(D_801E84FC, 3);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 6:
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unkC);
            break;
        case 9:
            ex->func_800DC818(0x10, 0xC, (u16 *)&work->unk14);
            ex->func_800DC818(0x10, 0x29, (u16 *)&work->unk1C);
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk14);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk1C);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801EC39C;
                work->unk4->unk12 = 0xA;
                work->unk4->unkC = 0x80;
            }
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801EC89C;
                work->unk8->unk12 = 0xA;
                work->unk8->unkC = 0x80;
            }
            break;
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_1b2_op82(s32 mode, Fx1b2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        work->unk8.vz = -0x44C;
        work->unk0.vx = 0;
        work->unk0.vy = 0;
        work->unk0.vz = 0;
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk18.vx = 0x1000;
        work->unk18.vy = 0x1000;
        work->unk18.vz = 0x1000;
        D_801E84EC = env;
        work->unk34 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0x119, 0, 0, 0);
        work->unk2C = -1;
        work->unk30 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk2C = 8;
            break;
        case 0x28:
            work->unk30 = 0xA;
            break;
        }
        if (frame >= 0 && frame < 0x32) {
            if (work->unk2C >= 0) {
                work->unk28 = func_801E81B0(-0x80, 0, 8 - work->unk2C, 8);
                work->unk2C--;
            }
            if (work->unk30 >= 0) {
                work->unk28 = func_801E81B0(0, -0x80, 0xA - work->unk30, 0xA);
                work->unk30--;
            }
            ex->Hi_DrawEffModel(&work->unk0, &work->unk8, (s32)&work->unk18, work->unk34);
            ex->Hi_ModifyEffModelAbr(work->unk34, 1);
            if (work->unk2C > 0 || work->unk30 >= 0) {
                c = work->unk28 + 6;
                ex->Hi_ModifyEffModelRGB(work->unk34, c, c, c);
            } else {
                ex->Hi_ModifyEffModelRGB(work->unk34, 6, 6, 6);
            }
        }
        if (frame >= 0x50) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_1b2_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E84EC = env;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            ex->func_800E6E1C(0x240, 0x100, 0x80, 0x80, D_801E875C, 0, -0x3E8, 0x1770);
        }
        return frame >= 5;
    }
    return 0;
}

void func_801E816C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E84EC)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E81B0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8208(Fx1b2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1b2Point *)data;
}

s32 func_801E8220(Fx1b2Track6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk2 - 1;
    s32 done = 1;
    Fx1b2Key6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

void func_801E8284(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E84EC)->btlfx_drawScreenTint((u32 *)(D_801E84EC->ot + ot), arg4, c, c, c);
}

void func_801E831C(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E84EC);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

s32 func_801E8388(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E84EC);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

void func_801E840C(s32 unit) {
    if (BTLFX_EXPORTS(D_801E84EC)->btlfx_isUnitActive(unit)) {
        func_801E831C(unit, 2);
    }
}

void func_801E8458(s32 unit) {
    if (BTLFX_EXPORTS(D_801E84EC)->btlfx_isUnitActive(unit)) {
        func_801E831C(unit, 3);
    }
}

INCLUDE_RODATA("asm/us/fx_1b2/nonmatchings/fx_1b2", fx_1b2_headerPtr);
