#include "fxmod/fx_180.h"

s32 func_801E78E0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8F08(&D_801E93F8[(s16)(e->unk1F - 1)], frame, &v);
    v.vy = v.vy * 4 / 5;
    v.vz = v.vz * 3 / 4;
    v.vx += D_801F0828.vx;
    v.vy += D_801F0828.vy;
    v.vz += D_801F0828.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 func_801E79DC(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8F08(&D_801E9488[(s16)(e->unk1F - 1)], frame, &v);
    v.vx += D_801F0830.vx;
    v.vy += D_801F0830.vy;
    v.vz += D_801F0830.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_180_op80(s32 mode, Fx180Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x74;
    } else if (mode == 1) {
        D_801E9258 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk24);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk2C);
        work->unk34.vx = env->targets->centerX;
        work->unk34.vy = env->targets->centerY;
        work->unk34.vz = env->targets->centerZ;
        work->unk44.vx = work->unk24.vx;
        work->unk44.vy = work->unk24.vy;
        work->unk44.vz = work->unk24.vz;
        work->unk4C.vx = work->unk24.vx;
        work->unk4C.vy = work->unk24.vy;
        work->unk4C.vz = work->unk24.vz;
        D_801F0828.vx = work->unk24.vx;
        D_801F0828.vy = work->unk24.vy;
        D_801F0828.vz = work->unk24.vz;
        work->unk44.vy = 0;
        for (i = 0; i < 0x12; i++) {
            func_801E8EF0(&D_801E93F8[i], func_801E8DA4((u8 *)ex->btlfx_getFileData(entry->unk8, 0x22), i));
        }
        work->unk54.vx = work->unk34.vx;
        work->unk54.vy = work->unk34.vy;
        work->unk54.vz = work->unk34.vz;
        work->unk5C.vx = work->unk34.vx;
        work->unk5C.vy = work->unk34.vy;
        work->unk5C.vz = work->unk34.vz;
        work->unk64.vx = work->unk34.vx;
        work->unk64.vy = work->unk34.vy;
        work->unk64.vz = work->unk34.vz;
        work->unk6C.vx = work->unk2C.vx;
        work->unk6C.vy = work->unk2C.vy;
        work->unk6C.vz = work->unk2C.vz;
        work->unk54.vy -= 0x2AF8;
        work->unk54.vz -= 0xFA0;
        work->unk5C.vy -= 0xC80;
        work->unk5C.vz -= 0xFA0;
        work->unk64.vy -= 0x2AF8;
        work->unk6C.vy -= 0x3E8;
        env->camera->points[0].vx = work->unk54.vx;
        env->camera->points[0].vy = work->unk54.vy;
        env->camera->points[0].vz = work->unk54.vz;
        env->camera->points[1].vx = work->unk5C.vx;
        env->camera->points[1].vy = work->unk5C.vy;
        env->camera->points[1].vz = work->unk5C.vz;
        env->camera->points[2].vx = work->unk64.vx;
        env->camera->points[2].vy = work->unk64.vy;
        env->camera->points[2].vz = work->unk64.vz;
        env->camera->points[3].vx = work->unk6C.vx;
        env->camera->points[3].vy = work->unk6C.vy;
        env->camera->points[3].vz = work->unk6C.vz;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x8:
            ex->btlfx_getUnitBoneMatrix(0x10, 0x29, (BtlBone *)&m);
            ex->btlfx_matrixPointAhead(&m, 0, (s32)&work->unk3C);
            work->unk3C.vy += 0x64;
            work->unk3C.vz -= 0xC8;
            work->unk0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&work->unk3C);
            work->unk0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk24);
            if (work->unk0[3] != NULL) {
                work->unk0[3]->unk14 = (s32)D_801E9578;
                work->unk0[3]->unk12 = 0x14;
                work->unk0[3]->unkC = 0x50;
            }
            if (work->unk0[0] != NULL) {
                work->unk0[0]->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            break;
        case 0x12:
            work->unk0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk24);
            work->unk0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk44);
            break;
        case 0x30:
            work->unk0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk24);
            work->unk0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk24);
            work->unk0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk44);
            work->unk0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&D_801F0838);
            if (work->unk0[7] != NULL) {
                work->unk0[7]->unk14 = (s32)D_801E9BB8;
                work->unk0[7]->unk12 = 0x12;
                work->unk0[7]->unkC = 0x70;
            }
            if (work->unk0[7] != NULL) {
                ((EffMotion *)work->unk0[7]->unk0)->unk8 = func_801E78E0;
            }
            if (work->unk0[7] != NULL) {
                work->unk0[7]->unk22 = 0x600;
            }
            break;
        case 0x4E:
            ex->btlfx_stopEmitter(work->unk0[7]);
            break;
        case 0x14:
            ex->func_800E83EC();
            break;
        case 0x34:
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x19), frame);
        return frame >= 0x64;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_180/nonmatchings/fx_180", fx_180_headerPtr);

s32 fx_180_op81(s32 mode, Fx180Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x54;
    } else if (mode == 1) {
        work->unk2C.vx = env->targets->centerX;
        work->unk2C.vy = env->targets->centerY;
        work->unk2C.vz = env->targets->centerZ;
        D_801E9258 = env;
        work->unk34.vx = work->unk2C.vx; work->unk34.vy = work->unk2C.vy; work->unk34.vz = work->unk2C.vz;
        work->unk3C.vx = work->unk2C.vx; work->unk3C.vy = work->unk2C.vy; work->unk3C.vz = work->unk2C.vz;
        work->unk44.vx = work->unk2C.vx; work->unk44.vy = work->unk2C.vy; work->unk44.vz = work->unk2C.vz;
        work->unk34.vy -= 0xC80;
        work->unk34.vz -= 0xFA0;
        work->unk3C.vy -= 0x2AF8;
        work->unk3C.vz -= 0xFA0;
        work->unk44.vy -= 0x2AF8;
        D_801F0830.vx = work->unk3C.vx;
        D_801F0830.vy = work->unk3C.vy;
        D_801F0830.vz = work->unk3C.vz;
        for (i = 0; i < 0x1E; i++) {
            func_801E8EF0(&D_801E9488[i], func_801E8DA4((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), i));
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x0:
            work->unk0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk34);
            if (work->unk0[0] != NULL) {
                work->unk0[0]->unk14 = (s32)D_801EA398;
                work->unk0[0]->unk12 = 0x17;
                work->unk0[0]->unkC = 0x90;
            }
            break;
        case 0x14:
            work->unk0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk3C);
            break;
        case 0x18:
            work->unk0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk3C);
            if (work->unk0[2] != NULL) {
                work->unk0[2]->unk14 = (s32)D_801EBCE8;
                work->unk0[2]->unk12 = 0x20;
                work->unk0[2]->unkC = 0x50;
            }
            break;
        case 0x1C:
            work->unk0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk3C);
            break;
        case 0x30:
            work->unk0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk3C);
            work->unk0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk3C);
            work->unk0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk3C);
            work->unk0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk3C);
            break;
        case 0x33:
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
            break;
        case 0x38:
            work->unk0[9] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&D_801F0840);
            if (work->unk0[9] != NULL) {
                work->unk0[9]->unk14 = (s32)D_801EC6E8;
                work->unk0[9]->unk12 = 0x2A;
                work->unk0[9]->unkC = 0x90;
            }
            if (work->unk0[9] != NULL) {
                ((EffMotion *)work->unk0[9]->unk0)->unk8 = func_801E79DC;
            }
            break;
        case 0x45:
            work->unk4C.vx = work->unk44.vx;
            work->unk4C.vy = work->unk44.vy;
            work->unk4C.vz = work->unk44.vz;
            work->unk4C.vy = -0xFA0;
            work->unk0[10] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk4C);
            if (work->unk0[10] != NULL) {
                work->unk0[10]->unk14 = (s32)D_801EB088;
                work->unk0[10]->unk12 = 0x16;
                work->unk0[10]->unkC = 0x90;
            }
            break;
        case 0x4B:
            ex->btlfx_stopEmitter(work->unk0[9]);
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x19), frame);
        return frame >= 0x64;
    }
    return 0;
}

s32 fx_180_op82(s32 mode, Fx180Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 w;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        D_801E9258 = env;
        ex->btlfx_initEffModels(D_801E9268, 2);
        work->unk4C.vx = env->targets->centerX;
        work->unk4C.vy = env->targets->centerY;
        work->unk4C.vz = env->targets->centerZ;
        work->unk20.vx = 0;
        work->unk20.vy = 0;
        work->unk20.vz = 0;
        work->unk28.vx = work->unk4C.vx;
        work->unk28.vy = 0;
        work->unk28.vz = work->unk4C.vz;
        work->unk38.vx = 0x4000;
        work->unk38.vy = 0x4000;
        work->unk38.vz = 0x4000;
        work->unk68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x17), 1);
        work->unk6A = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 1));
        work->unk64 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk54.vx = work->unk4C.vx;
            work->unk54.vy = work->unk4C.vy;
            work->unk54.vz = work->unk4C.vz;
            work->unk54.vy = -0x157C;
            work->unk0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk54);
            break;
        case 4:
            work->unk0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk4C);
            break;
        case 16:
            work->unk5C.vx = work->unk4C.vx;
            work->unk5C.vy = work->unk4C.vy;
            work->unk5C.vz = work->unk4C.vz;
            work->unk5C.vy = -0x1F40;
            work->unk0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk5C);
            work->unk0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk4C);
            break;
        case 30:
            work->unk0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unk4C);
            work->unk0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x20), (s32)&work->unk4C);
            work->unk0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&work->unk4C);
            if (work->unk0[4] != NULL) {
                work->unk0[4]->unk14 = (s32)D_801EDE88;
                work->unk0[4]->unk12 = 0x26;
                work->unk0[4]->unkC = 0x90;
            }
            if (work->unk0[5] != NULL) {
                work->unk0[5]->unk14 = (s32)D_801EF3E8;
                work->unk0[5]->unk12 = 0x14;
                work->unk0[5]->unkC = 0x90;
            }
            break;
        case 32:
            for (i = 0; i < env->targets->targetCount; i++) {
                func_801E90A8(i);
            }
            break;
        case 42:
            work->unk64 = 8;
            break;
        }
        t = frame - 0x1E;
        if ((u32)t < 0x14) {
            if (t < 0) {
                w = 0x1000;
            } else if (t >= 0x14) {
                w = 0;
            } else {
                w = 0x1000 - (t << 12) / 20;
            }
            ex->Hi_DrawMorphEffModel(&work->unk20, &work->unk28, (s32)&work->unk38, work->unk68, work->unk6A, w);
            if (work->unk64 >= 0) {
                work->unk48 = func_801E8E00(0, -0xFF, 8 - work->unk64, 8);
                work->unk64--;
                ex->Hi_ModifyEffModelRGB(work->unk68, work->unk48, work->unk48, work->unk48);
            }
        }
        if (frame >= 0x20) {
            for (i = 0; i < env->targets->targetCount; i++) {
                if (func_801E8FD8(i) != 0) {
                    func_801E8F6C(i, 4);
                    ex->btlfx_holdMotion(i);
                }
            }
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_180_op83(s32 mode, Fx180Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        D_801E9258 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        work->unk38 = -1;
        work->unk3A = 0;
        work->unk30.vx = work->unk8.vx;
        work->unk30.vy = work->unk8.vy;
        work->unk30.vz = work->unk8.vz;
        work->unk30.vy += 0x190;
        work->unk30.vz -= 0x3E8;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 3:
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&work->unk30);
            if (work->unk0 != NULL) {
                work->unk0->unk14 = (s32)D_801EFF28;
                work->unk0->unk12 = 0x10;
                work->unk0->unkC = 0x90;
            }
            break;
        case 0x11:
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk10);
            work->unk18.vx = work->unk10.vx;
            work->unk18.vy = work->unk10.vy;
            work->unk18.vz = work->unk10.vz + 0x1770;
            break;
        case 0x25:
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk20);
            break;
        case 0x14:
            work->unk38 = 0x1E;
            break;
        }
        if (frame >= 0x11 && frame < 0x25) {
            func_801E90F4(0x10, &work->unk18, frame - 0x11, 0x14, 0, &work->unk28);
        }
        if (frame >= 0x25) {
            work->unk20.vy += 0xC8;
            ex->btlfx_setUnitPos(0x10, (s16 *)&work->unk20);
        }
        if (work->unk38 >= 0) {
            func_801E8E58(0, 0xFF, 0x1E - work->unk38, 0x1E, 1, 0);
            work->unk38--;
            if (work->unk38 == -1) {
                work->unk3A = 1;
                ex->func_800E83EC();
            }
        } else if (work->unk3A != 0) {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x50;
    }
    return 0;
}

u16 *func_801E8DA4(u8 *base, u16 i) {
    return (u16 *)((s32)base + ((s32 *)base)[(u16)i + 1]);
}

void func_801E8DBC(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E9258)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8E00(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8E58(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E9258)->btlfx_drawScreenTint((u32 *)(D_801E9258->ot + ot), arg4, c, c, c);
}

void func_801E8EF0(Fx180Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx180Point *)data;
}

s32 func_801E8F08(Fx180Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx180Point *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

void func_801E8F6C(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9258);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

s32 func_801E8FD8(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9258);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

void func_801E905C(s32 unit) {
    if (BTLFX_EXPORTS(D_801E9258)->btlfx_isUnitActive(unit)) {
        func_801E8F6C(unit, 2);
    }
}

void func_801E90A8(s32 unit) {
    if (BTLFX_EXPORTS(D_801E9258)->btlfx_isUnitActive(unit)) {
        func_801E8F6C(unit, 3);
    }
}

s32 func_801E90F4(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9258);
    SVECTOR pos;
    s32 t;

    if (frame == 0) {
        ex->btlfx_getUnitPos(unit, (s16 *)start);
    }
    t = (frame << 12) / n;
    func_801E8DBC(start, dst, t, &pos);
    if (height) {
        pos.vy -= (ex->math_sin(t >> 1) * height) >> 12;
    }
    ex->btlfx_setUnitPos(unit, (s16 *)&pos);
    return frame >= n;
}
