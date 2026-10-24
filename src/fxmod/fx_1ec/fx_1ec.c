#include "fxmod/fx_1ec.h"

s32 func_801E78E0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8C60(&D_801E9034[(s16)(e->unk1F - 1)], frame, &v);
    v.vy = v.vy * 4 / 5;
    v.vz = v.vz * 3 / 4;
    v.vx += D_801EFB64.vx;
    v.vy += D_801EFB64.vy;
    v.vz += D_801EFB64.vz;
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

    func_801E8C60(&D_801E90C4[(s16)(e->unk1F - 1)], frame, &v);
    v.vx += D_801EFB6C.vx;
    v.vy += D_801EFB6C.vy;
    v.vz += D_801EFB6C.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1ec_op80(s32 mode, Fx1ecWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x64;
    } else if (mode == 1) {
        D_801E8E94 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk24);
        work->unk2C.vx = env->targets->unk38;
        work->unk2C.vy = env->targets->unk3A;
        work->unk2C.vz = env->targets->unk3C;
        work->unk34.vx = work->unk24.vx;
        work->unk34.vy = work->unk24.vy;
        work->unk34.vz = work->unk24.vz;
        work->unk3C.vx = work->unk24.vx;
        work->unk3C.vy = work->unk24.vy;
        work->unk3C.vz = work->unk24.vz;
        D_801EFB64.vx = work->unk24.vx;
        D_801EFB64.vy = work->unk24.vy;
        D_801EFB64.vz = work->unk24.vz;
        work->unk34.vy = 0;
        for (i = 0; i < 0x12; i++) {
            func_801E8C48(&D_801E9034[i], func_801E8AFC((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), i));
        }
        work->unk4C.vx = work->unk2C.vx;
        work->unk4C.vy = work->unk2C.vy;
        work->unk4C.vz = work->unk2C.vz;
        work->unk54.vx = work->unk2C.vx;
        work->unk54.vy = work->unk2C.vy;
        work->unk54.vz = work->unk2C.vz;
        work->unk5C.vx = work->unk2C.vx;
        work->unk5C.vy = work->unk2C.vy;
        work->unk5C.vz = work->unk2C.vz;
        work->unk4C.vy -= 0x2AF8;
        work->unk4C.vz -= 0xFA0;
        work->unk54.vy -= 0xC80;
        work->unk54.vz -= 0xFA0;
        work->unk5C.vy -= 0x2AF8;
        env->camera->unk0[0].vx = work->unk4C.vx;
        env->camera->unk0[0].vy = work->unk4C.vy;
        env->camera->unk0[0].vz = work->unk4C.vz;
        env->camera->unk0[1].vx = work->unk54.vx;
        env->camera->unk0[1].vy = work->unk54.vy;
        env->camera->unk0[1].vz = work->unk54.vz;
        env->camera->unk0[2].vx = work->unk5C.vx;
        env->camera->unk0[2].vy = work->unk5C.vy;
        env->camera->unk0[2].vz = work->unk5C.vz;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x8:
            work->unk0[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk24);
            work->unk0[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk24);
            if (work->unk0[3] != NULL) {
                work->unk0[3]->unk14 = (s32)D_801E91B4;
                work->unk0[3]->unk12 = 0x14;
                work->unk0[3]->unkC = 0x50;
            }
            if (work->unk0[0] != NULL) {
                work->unk0[0]->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            break;
        case 0x12:
            work->unk0[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk24);
            work->unk0[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk34);
            break;
        case 0x30:
            work->unk0[5] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk24);
            work->unk0[6] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk24);
            work->unk0[8] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk34);
            work->unk0[7] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&D_801EFB74);
            if (work->unk0[7] != NULL) {
                work->unk0[7]->unk14 = (s32)D_801E97F4;
                work->unk0[7]->unk12 = 0x12;
                work->unk0[7]->unkC = 0x70;
                if (work->unk0[7] != NULL) {
                    ((EffMotion *)work->unk0[7]->unk0)->unk8 = func_801E78E0;
                }
            }
            if (work->unk0[5] != NULL) {
                work->unk0[5]->unk22 = 0x600;
            }
            if (work->unk0[6] != NULL) {
                work->unk0[6]->unk22 = 0x600;
            }
            if (work->unk0[7] != NULL) {
                work->unk0[7]->unk22 = 0x600;
            }
            break;
        case 0x4E:
            ex->func_800E3D54(work->unk0[7]);
            break;
        case 0x34:
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x26), frame);
        return frame >= 0x64;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_headerPtr);

s32 fx_1ec_op81(s32 mode, Fx1ecWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x54;
    } else if (mode == 1) {
        work->unk2C.vx = env->targets->unk38;
        work->unk2C.vy = env->targets->unk3A;
        work->unk2C.vz = env->targets->unk3C;
        D_801E8E94 = env;
        work->unk34.vx = work->unk2C.vx; work->unk34.vy = work->unk2C.vy; work->unk34.vz = work->unk2C.vz;
        work->unk3C.vx = work->unk2C.vx; work->unk3C.vy = work->unk2C.vy; work->unk3C.vz = work->unk2C.vz;
        work->unk44.vx = work->unk2C.vx; work->unk44.vy = work->unk2C.vy; work->unk44.vz = work->unk2C.vz;
        work->unk34.vy -= 0xC80;
        work->unk34.vz -= 0xFA0;
        work->unk3C.vy -= 0x2AF8;
        work->unk3C.vz -= 0xFA0;
        work->unk44.vy -= 0x2AF8;
        D_801EFB6C.vx = work->unk3C.vx;
        D_801EFB6C.vy = work->unk3C.vy;
        D_801EFB6C.vz = work->unk3C.vz;
        for (i = 0; i < 0x1E; i++) {
            func_801E8C48(&D_801E90C4[i], func_801E8AFC((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), i));
        }
    } else {
    frame = entry->unk0;
    switch (frame) {
    case 0x0:
        work->unk0[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x25), (s32)&work->unk34);
        if (work->unk0[0] != NULL) {
            work->unk0[0]->unk14 = (s32)D_801E9FD4;
            work->unk0[0]->unk12 = 0x17;
            work->unk0[0]->unkC = 0x90;
        }
        break;
    case 0x14:
        work->unk0[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk3C);
        break;
    case 0x18:
        work->unk0[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk3C);
        if (work->unk0[2] != NULL) {
            work->unk0[2]->unk14 = (s32)D_801EB924;
            work->unk0[2]->unk12 = 0x20;
            work->unk0[2]->unkC = 0x50;
        }
        break;
    case 0x1C:
        work->unk0[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk3C);
        break;
    case 0x30:
        work->unk0[4] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk3C);
        work->unk0[5] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk3C);
        work->unk0[7] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk3C);
        work->unk0[8] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk3C);
        break;
    case 0x33:
        ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        break;
    case 0x38:
        work->unk0[9] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&D_801EFB7C);
        if (work->unk0[9] != NULL) {
            work->unk0[9]->unk14 = (s32)D_801EC324;
            work->unk0[9]->unk12 = 0x2A;
            work->unk0[9]->unkC = 0x90;
            if (work->unk0[9] != NULL) {
                ((EffMotion *)work->unk0[9]->unk0)->unk8 = func_801E79DC;
            }
        }
        break;
    case 0x45:
        work->unk4C.vx = work->unk44.vx;
        work->unk4C.vy = work->unk44.vy;
        work->unk4C.vz = work->unk44.vz;
        work->unk4C.vy = -0xFA0;
        work->unk0[10] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk4C);
        if (work->unk0[10] != NULL) {
            work->unk0[10]->unk14 = (s32)D_801EACC4;
            work->unk0[10]->unk12 = 0x16;
            work->unk0[10]->unkC = 0x90;
        }
        break;
    case 0x4B:
        ex->func_800E3D54(work->unk0[9]);
        break;
    }
    ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x26), frame);
    return frame >= 0x64;
    }
    return 0;
}

s32 fx_1ec_op82(s32 mode, Fx1ecWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 w;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x70;
    } else if (mode == 1) {
        D_801E8E94 = env;
        ex->btlfx_initEffModels(D_801E8EA4, 2);
        work->unk4C.vx = env->targets->unk38;
        work->unk4C.vy = env->targets->unk3A;
        work->unk4C.vz = env->targets->unk3C;
        work->unk20.vx = 0;
        work->unk20.vy = 0;
        work->unk20.vz = 0;
        work->unk28.vx = work->unk4C.vx;
        work->unk28.vy = 0;
        work->unk28.vz = work->unk4C.vz;
        work->unk38.vx = 0x4000;
        work->unk38.vy = 0x4000;
        work->unk38.vz = 0x4000;
        work->unk6C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xD), 1);
        work->unk6E = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE));
        work->unk64 = -1;
        work->unk68 = -1;
        work->unk6A = 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk54.vx = work->unk4C.vx;
            work->unk54.vy = work->unk4C.vy;
            work->unk54.vz = work->unk4C.vz;
            work->unk54.vy = -0x157C;
            work->unk0[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk54);
            if (work->unk0[0] != NULL) {
                work->unk0[0]->unk22 = 0x500;
            }
            break;
        case 4:
            work->unk0[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&work->unk4C);
            if (work->unk0[1] != NULL) {
                work->unk0[1]->unk22 = 0x500;
            }
            break;
        case 16:
            work->unk5C.vx = work->unk4C.vx;
            work->unk5C.vy = work->unk4C.vy;
            work->unk5C.vz = work->unk4C.vz;
            work->unk5C.vy = -0x1F40;
            work->unk0[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x24), (s32)&work->unk5C);
            work->unk0[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&work->unk4C);
            if (work->unk0[2] != NULL) {
                work->unk0[2]->unk22 = 0x500;
            }
            break;
        case 30:
            work->unk0[4] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x23), (s32)&work->unk4C);
            work->unk0[5] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x20), (s32)&work->unk4C);
            work->unk0[7] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x27), (s32)&work->unk4C);
            if (work->unk0[4] != NULL) {
                work->unk0[4]->unk14 = (s32)D_801EDAC4;
                work->unk0[4]->unk12 = 0x26;
                work->unk0[4]->unkC = 0x90;
            }
            if (work->unk0[5] != NULL) {
                work->unk0[5]->unk14 = (s32)D_801EF024;
                work->unk0[5]->unk12 = 0x14;
                work->unk0[5]->unkC = 0x90;
            }
            if (work->unk0[7] != NULL) {
                work->unk0[7]->unk22 = 0x200;
            }
            break;
        case 32:
            work->unk68 = 0x3C;
            for (i = 0; i < env->targets->unk24; i++) {
                func_801E8E00(i);
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
            ex->Hi_DrawMorphEffModel(&work->unk20, &work->unk28, (s32)&work->unk38, work->unk6C, work->unk6E, w);
            if (work->unk64 >= 0) {
                work->unk48 = func_801E8B58(0, -0xFF, 8 - work->unk64, 8);
                work->unk64--;
                ex->Hi_ModifyEffModelRGB(work->unk6C, work->unk48, work->unk48, work->unk48);
            }
        }
        if (work->unk68 >= 0) {
            func_801E8BB0(0, 0xFF, 0x3C - work->unk68, 0x3C, 1, 0);
            work->unk68--;
            if (work->unk68 == -1) {
                work->unk6A = 1;
                ex->func_800E83EC();
            }
        } else if (work->unk6A != 0) {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        }
        if (frame >= 0x20) {
            for (i = 0; i < env->targets->unk24; i++) {
                if (func_801E8D30(i) != 0) {
                    func_801E8CC4(i, 4);
                    ex->func_800D89E8(i);
                }
            }
        }
        if (frame >= 0x78) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

u16 *func_801E8AFC(u8 *base, u16 i) {
    return (u16 *)((s32)base + ((s32 *)base)[(u16)i + 1]);
}

void func_801E8B14(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8E94)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8B58(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8BB0(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E8E94)->btlfx_drawScreenTint((u32 *)(D_801E8E94->ot + ot), arg4, c, c, c);
}

void func_801E8C48(Fx1ecPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1ecPoint *)data;
}

s32 func_801E8C60(Fx1ecPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1ecPoint *p;

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

void func_801E8CC4(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8E94);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

s32 func_801E8D30(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8E94);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

void func_801E8DB4(s32 unit) {
    if (BTLFX_EXPORTS(D_801E8E94)->btlfx_isUnitActive(unit)) {
        func_801E8CC4(unit, 2);
    }
}

void func_801E8E00(s32 unit) {
    if (BTLFX_EXPORTS(D_801E8E94)->btlfx_isUnitActive(unit)) {
        func_801E8CC4(unit, 3);
    }
}
