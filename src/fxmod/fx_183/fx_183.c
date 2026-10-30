#include "fxmod/fx_183.h"

s32 fx_183_op80(s32 mode, Fx183Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E82E8 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->unk8);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        s32 frame = entry->frame;

        if (work->unk0 != NULL) {
            if (work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->rot.vx = work->unk10.vx;
                work->unk0->rot.vy = work->unk10.vy;
                work->unk0->rot.vz = work->unk10.vz;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->state == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->rot.vx = work->unk10.vx;
                work->unk4->rot.vy = work->unk10.vy;
                work->unk4->rot.vz = work->unk10.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_183_op81(s32 mode, Fx183Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x4C;
    } else if (mode == 1) {
        D_801E82E8 = env;
        ex->btlfx_initEffModels(D_801E82F8, 7);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk8);
        work->unk18.vx = work->unk8.vx;
        work->unk18.vy = work->unk8.vy;
        work->unk18.vz = work->unk8.vz;
        work->unk18.vy = -0x7D0;
        work->unk10.vx = env->targets->centerX;
        work->unk10.vy = env->targets->centerY;
        work->unk10.vz = env->targets->centerZ;
        ex->btlfx_aimAngles((s16 *)&work->unk18, (s16 *)&work->unk10, (s16 *)&work->unk20);
        work->unk20.vx = ex->ratan2(work->unk10.vy - work->unk18.vy, work->unk18.vz - work->unk10.vz);
        work->unk38.vx = 0x1000;
        work->unk38.vy = 0x1000;
        work->unk38.vz = 0x1000;
        work->unk48 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x1D), 1);
    } else {
        frame = entry->frame;
        if (frame == 4) {
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->unk10);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->unk10);
            if (work->unk4 != NULL) {
                work->unk4->otZ = 0x80;
            }
        }
        if (frame < 5) {
            ex->btlfx_lerpVec((s16 *)&work->unk18, (s16 *)&work->unk10, frame << 10, (s16 *)&v);
            work->unk28.vx = v.vx;
            work->unk28.vy = v.vy;
            work->unk28.vz = v.vz;
            ex->Hi_SetEffModelSlice(work->unk48, work->unk28.vy);
            ex->Hi_DrawSliceEffModel(&work->unk20, &work->unk28, (s32)&work->unk38, work->unk48);
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_183_op82(s32 mode, Fx183Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    SVECTOR v;
    s32 frame;
    s32 i;
    s32 j;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x5C;
    } else if (mode == 1) {
        work->unk40.vx = env->targets->centerX;
        work->unk40.vy = env->targets->centerY;
        D_801E82E8 = env;
        work->unk40.vz = env->targets->centerZ;
        work->unk4.vx = 0;
        work->unk4.vy = 0;
        work->unk4.vz = 0;
        work->unk1C.vx = 0x1000;
        work->unk1C.vy = 0x1000;
        work->unk1C.vz = 0x1000;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        work->unk2C.vx = 0x1800;
        work->unk2C.vy = 0x1800;
        work->unk2C.vz = 0x1800;
        for (j = 0; j < 6; j++) {
            func_801E81DC(&D_801E8878[j], (u16 *)ex->btlfx_getFileData(entry->file, D_801E8294[j]));
            work->unk50[j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, D_801E8288[j]), 1);
        }
        work->unk48 = -1;
        work->unk4C = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0xF:
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->unk40);
            if (work->unk0 != NULL) {
                work->unk0->otZ = 0x80;
            }
            break;
        case 0:
            work->unk48 = 4;
            break;
        case 0x12:
            work->unk4C = 8;
            break;
        }
        if (frame >= 0 && frame < 0x1A) {
            t = frame;
            if (work->unk48 >= 0 || work->unk4C >= 0) {
                if (work->unk48 >= 0) {
                    work->unk3C = func_801E8184(-0x80, 0, 4 - work->unk48, 4);
                    work->unk48--;
                }
                if (work->unk4C >= 0) {
                    work->unk3C = func_801E8184(0, -0x80, 8 - work->unk4C, 8);
                    work->unk4C--;
                }
            }
            for (i = 0; i < 6; i++) {
                func_801E81F4(&D_801E8878[i], t, &work->unk4, &v);
                work->unkC.vx = v.vx;
                work->unkC.vy = v.vy;
                work->unkC.vz = v.vz;
                work->unkC.vx += work->unk40.vx;
                work->unkC.vy += work->unk40.vy;
                work->unkC.vz += work->unk40.vz;
                if (i == 0) {
                    sc.vx = work->unk1C.vx;
                    sc.vy = work->unk1C.vy;
                    sc.vz = work->unk1C.vz;
                } else {
                    sc.vx = work->unk2C.vx;
                    sc.vy = work->unk2C.vy;
                    sc.vz = work->unk2C.vz;
                }
                ex->Hi_SetEffModelSlice(work->unk50[i], work->unkC.vy);
                ex->Hi_DrawSliceEffModel(&work->unk4, &work->unkC, (s32)&sc, work->unk50[i]);
                if (work->unk48 >= 0 || work->unk4C >= 0) {
                    ex->Hi_ModifyEffModelRGB(work->unk50[i], work->unk3C, work->unk3C, work->unk3C);
                }
            }
        }
        if (frame >= 0x50) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_183_op83(s32 mode, Fx183Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        s32 unit = *arg4;
        work->unk14 = unit;
        D_801E82E8 = env;
        if (unit >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitBodyPos(unit, 0, (s16 *)&work->unkC);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->unkC);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->unkC);
        work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E88A8[work->unk14];
            work->unk0->unk12 = 0xC;
            work->unk0->unkC = 0x50;
        }
        if (work->unk0 != NULL) {
            work->unk0->otZ = 0x80;
        }
        if (work->unk4 != NULL) {
            work->unk4->otZ = 0x80;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

void func_801E8140(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E82E8)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8184(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E81DC(Fx183Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx183Key *)data;
}

s32 func_801E81F4(Fx183Path *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx183Key *k;
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

INCLUDE_RODATA("asm/us/fx_183/nonmatchings/fx_183", fx_183_headerPtr);
