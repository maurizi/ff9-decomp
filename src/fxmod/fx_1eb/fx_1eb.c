#include "fxmod/fx_1eb.h"

s32 fx_1eb_op80(s32 mode, Fx1ebWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E8268 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk8);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        s32 frame = entry->unk0;

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

s32 fx_1eb_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E8268 = env;
    } else {
        return entry->unk0 >= 0xA;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1eb/nonmatchings/fx_1eb", fx_1eb_headerPtr);

s32 fx_1eb_op82(s32 mode, Fx1ebWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 m;
    s32 frame;
    s32 i;
    s32 j;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x60;
    } else if (mode == 1) {
        D_801E8268 = env;
        ex->btlfx_initEffModels(D_801E8278, 6);
        work->unk44.vx = env->targets->centerX;
        work->unk44.vy = env->targets->centerY;
        work->unk44.vz = env->targets->centerZ;
        work->unk18.vx = 0;
        work->unk18.vy = 0;
        work->unk18.vz = 0;
        work->unk20.vx = 0;
        work->unk20.vy = 0;
        work->unk20.vz = 0;
        work->unk30.vx = 0xAAA;
        work->unk30.vy = 0xAAA;
        work->unk30.vz = 0xAAA;
        func_801E81B4(&D_801E8740, (u16 *)ex->btlfx_getFileData(entry->unk8, 7));
        for (j = 0; j < 3; j++) {
            func_801E8108(&D_801E8728[j], (u16 *)ex->btlfx_getFileData(entry->unk8, D_801E8218[j]));
            work->unk52[j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E8210[j]), 1);
        }
        work->unk58 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 6), 1);
        work->unk4C = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk50 = 0x1000;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unk44);
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&work->unk44);
            break;
        case 30:
            work->unk4C = 0xA;
            break;
        case 10:
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk44);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk44);
            break;
        case 20:
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk44);
            break;
        case 22:
            work->unk14 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk44);
            if (work->unk14 != NULL) {
                work->unk14->unk14 = (s32)D_801E8748;
                work->unk14->unk12 = 6;
                work->unk14->unkC = 0x60;
            }
            break;
        }
        if (frame >= 0 && frame < 0x28) {
            t = frame;
            if (work->unk4C >= 0) {
                work->unk40 = func_801E80B0(0, -0x80, 0xA - work->unk4C, 0xA);
                work->unk4C--;
            }
            for (i = 0; i < 3; i++) {
                func_801E8120(&D_801E8728[i], t, &work->unk18, &v);
                work->unk20.vx = v.vx;
                work->unk20.vy = v.vy;
                work->unk20.vz = v.vz;
                work->unk20.vx += work->unk44.vx;
                work->unk20.vy += work->unk44.vy;
                work->unk20.vz += work->unk44.vz;
                if (i == 0 && t >= 0x16) {
                    func_801E81CC(&D_801E8740, t - 0x16, &m);
                    ex->Hi_DrawMorphEffModel(&work->unk18, &work->unk20, (s32)&work->unk30, work->unk52[i],
                                             work->unk58, m);
                } else {
                    ex->Hi_SetEffModelSlice(work->unk52[i], work->unk20.vy);
                    ex->Hi_DrawSliceEffModel(&work->unk18, &work->unk20, (s32)&work->unk30, work->unk52[i]);
                }
                if (work->unk4C >= 0) {
                    ex->Hi_ModifyEffModelAbr(work->unk52[i], 1);
                    ex->Hi_ModifyEffModelRGB(work->unk52[i], work->unk40, work->unk40, work->unk40);
                } else {
                    ex->Hi_ModifyEffModelAbr(work->unk52[i], 0);
                }
            }
        }
        if (frame >= 0x18) {
            work->unk50 -= 0x200;
            if (work->unk0 != NULL) {
                work->unk0->unk20 = work->unk50;
            }
            if (work->unk50 == 0) {
                ex->func_800E3D54(work->unk0);
            }
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_1eb_op83(s32 mode, Fx1ebWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        s32 unit = *arg4;
        work->unkC = unit;
        D_801E8268 = env;
        if (unit >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitPos(unit, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(work->unkC, 0x80);
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

void func_801E806C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8268)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E80B0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8108(Fx1ebKeyPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1ebKey *)data;
}

s32 func_801E8120(Fx1ebKeyPath *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx1ebKey *k;
    s32 r = 1;

    if (i > path->unk2 - 1) {
        i = path->unk2 - 1;
    } else {
        r = 0;
    }
    k = &path->unk4[i];
    if (rot != NULL) {
        rot->vx = k->unk6[0];
        rot->vy = k->unk6[1];
        rot->vz = k->unk6[2];
    }
    if (pos != NULL) {
        pos->vx = k->unk0[0];
        pos->vy = k->unk0[1];
        pos->vz = k->unk0[2];
    }
    return r;
}

void func_801E81B4(Fx1ebPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1ebPoint *)data;
}

s32 func_801E81CC(Fx1ebPath *keys, s32 i, s32 *out) {
    s32 r = 1;

    if (i > keys->unk2 - 1) {
        i = keys->unk2 - 1;
    } else {
        r = 0;
    }
    *out = *(s16 *)((u8 *)keys->unk4 + (i << 1));
    return r;
}
