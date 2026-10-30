#include "fxmod/fx_128.h"

s32 func_801E7704(Fx128Particle *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    BTLFX_EXPORTS(D_801E89D4)->btlfx_lerpVec((s16 *)&D_801E8F9C, (s16 *)&D_801E8FA4, (frame << 12) / 12, (s16 *)pos);
    out->unk0.vx = pos->vx << 12;
    out->unk0.vy = pos->vy << 12;
    out->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 func_801E77C8(Fx128Particle *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    BTLFX_EXPORTS(D_801E89D4)->btlfx_lerpVec((s16 *)&D_801E8F9C, (s16 *)((u8 *)D_801E8FB4 + (out->unk1F - 1) * 8),
                                             (frame << 12) / 12, (s16 *)pos);
    out->unk0.vx = pos->vx << 12;
    out->unk0.vy = pos->vy << 12;
    out->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_128_op80(s32 mode, Fx128Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E89D4 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->unk8);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (work->unk0 != NULL) {
            if (work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk10.vx;
                work->unk0->unk5C.vy = work->unk10.vy;
                work->unk0->unk5C.vz = work->unk10.vz;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->state == -1) {
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

s32 fx_128_op81(s32 mode, Fx128Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR ang;
    SVECTOR p1;
    SVECTOR p2;
    MATRIX m;
    VECTOR lv;
    SVECTOR v;
    s32 s;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    SVECTOR *p;

    if (mode == 0) {
        *arg4 = 0xB0;
    } else if (mode == 1) {
        D_801E89D4 = env;
        ex->btlfx_initEffModels(D_801E89E4, 7);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk50);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk58);
        if (env->targets->targetCount == mode) {
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk60);
            ex->btlfx_getUnitPos(0, (s16 *)&work->unk68);
        } else {
            work->unk60.vx = env->targets->centerX;
            work->unk60.vy = env->targets->centerY;
            work->unk60.vz = env->targets->centerZ;
            work->unk68.vx = env->targets->centerX;
            work->unk68.vy = env->targets->centerY;
            work->unk68.vz = env->targets->centerZ;
            work->unk68.vy = 0;
        }
        work->unk24.vx = 0;
        work->unk24.vy = 0;
        work->unk24.vz = 0;
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk3C.vx = 0x1000;
        work->unk3C.vy = 0x1000;
        work->unk3C.vz = 0x1000;
        work->unkA0[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xB), 1);
        work->unkA0[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x1F), 1);
        func_801E87F0((Fx128Path *)&D_801E8F5C, (u16 *)ex->btlfx_getFileData(entry->file, 2));
        func_801E889C(&D_801E8F8C, (u16 *)ex->btlfx_getFileData(entry->file, 0x25));
        func_801E8918((Fx128Path *)&D_801E8F94, (u16 *)ex->btlfx_getFileData(entry->file, 3));
        for (j = 0; j < 5; j++) {
            func_801E87F0((Fx128Path *)&D_801E8F64[j], (u16 *)ex->btlfx_getFileData(entry->file, D_801E8980[j]));
            work->unkA0[j + 2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, D_801E8974[j]), 1);
        }
        work->unk98 = -1;
        work->unk9C = -1;
        ex->btlfx_pointAhead((s16 *)&work->unk50, (s16 *)&work->unk58, -0x1DE, (s16 *)&work->unk70);
        work->unk0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk70);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x19:
            work->unk0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x24), (s32)&D_801E9004);
            break;
        case 0x2D:
            work->unk78.vx = work->unk90.vx;
            work->unk78.vy = work->unk90.vy;
            work->unk78.vz = work->unk90.vz;
            work->unk78.vy += 0x50;
            D_801E8F9C.vx = work->unk78.vx;
            D_801E8F9C.vy = work->unk78.vy;
            D_801E8F9C.vz = work->unk78.vz;
            D_801E8FA4.vx = work->unk60.vx;
            D_801E8FA4.vy = work->unk60.vy;
            D_801E8FA4.vz = work->unk60.vz;
            for (i = 0; i < 10; i++) {
                D_801E8FB4[i].vx = work->unk60.vx;
                D_801E8FB4[i].vy = work->unk60.vy;
                D_801E8FB4[i].vz = work->unk60.vz;
                if (ex->btlfx_rand() & 1) {
                    D_801E8FB4[i].vx = D_801E8FB4[i].vx + (0xC8 + ex->btlfx_rand() % 400);
                } else {
                    D_801E8FB4[i].vx = D_801E8FB4[i].vx - (0xC8 + ex->btlfx_rand() % 400);
                }
                if (ex->btlfx_rand() & 1) {
                    D_801E8FB4[i].vy = D_801E8FB4[i].vy + (0xC8 + ex->btlfx_rand() % 400);
                } else {
                    D_801E8FB4[i].vy = D_801E8FB4[i].vy - (0xC8 + ex->btlfx_rand() % 400);
                }
            }
            ex->btlfx_aimAngles((s16 *)&work->unk78, (s16 *)&work->unk60, (s16 *)&work->unk80);
            work->unk0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&D_801E900C);
            work->unk0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x23), (s32)&D_801E900C);
            work->unk0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->unk78);
            if (work->unk0[4] != NULL) {
                work->unk0[4]->unk14 = (s32)D_801E9014;
                work->unk0[4]->unk12 = 9;
                work->unk0[4]->unkC = 0x48;
            }
            if (work->unk0[2] != NULL) {
                *(s32 *)&work->unk0[2]->file->unk8 = (s32)func_801E7704;
            }
            if (work->unk0[3] != NULL) {
                *(s32 *)&work->unk0[3]->file->unk8 = (s32)func_801E77C8;
            }
            break;
        case 0x3C:
            work->unk0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&work->unk60);
            work->unk0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->unk60);
            work->unk0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x20), (s32)&work->unk60);
            work->unk0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x26), (s32)&work->unk68);
            if (work->unk0[7] != NULL) {
                work->unk0[7]->unk14 = (s32)D_801E92A4;
                work->unk0[7]->unk12 = 8;
                work->unk0[7]->unkC = 0x48;
            }
            if (work->unk0[5] != NULL) {
                work->unk0[5]->unk22 = 0x80;
            }
            break;
        case 8:
            work->unk98 = 4;
            break;
        case 0x39:
            work->unk9C = 8;
            break;
        }
        if (frame >= 8 && frame < 0x41) {
            k = frame - 8;
            if (work->unk98 >= 0 || work->unk9C >= 0) {
                if (work->unk98 >= 0) {
                    work->unk4C = func_801E8798(-0x80, 0, 4 - work->unk98, 4);
                    work->unk98--;
                }
                if (work->unk9C >= 0) {
                    work->unk4C = func_801E8798(0, -0x80, 8 - work->unk9C, 8);
                    work->unk9C--;
                }
            }
            func_801E8808(&D_801E8F5C, k, &work->unk24, &p1);
            work->unk2C.vx = p1.vx;
            work->unk2C.vy = p1.vy;
            work->unk2C.vz = p1.vz;
            work->unk2C.vx += work->unk50.vx;
            work->unk2C.vy += work->unk50.vy;
            work->unk2C.vz += work->unk50.vz;
            work->unk88.vx = work->unk2C.vx;
            work->unk88.vy = work->unk2C.vy;
            work->unk88.vz = work->unk2C.vz;
            ex->btlfx_aimAngles((s16 *)&work->unk70, (s16 *)&work->unk60, (s16 *)&ang);
            work->unk24.vy = ang.vy;
            if (env->targets->targetCount == 1 && k >= 0x25 && work->unk24.vx <= ang.vx) {
                work->unk24.vx = ang.vx;
            }
            func_801E8930(&D_801E8F94, k, &s);
            ex->Hi_DrawMorphEffModel(&work->unk24, (VECTOR *)&work->unk2C, (s32)&work->unk3C, work->unkA0[0],
                                     work->unkA0[1], s);
            if (work->unk98 >= 0 || work->unk9C >= 0) {
                ex->Hi_ModifyEffModelRGB(work->unkA0[0], work->unk4C, work->unk4C, work->unk4C);
            }
            for (i = 0; i < 5; i++) {
                func_801E8808(&D_801E8F64[i], k, &work->unk24, &p2);
                work->unk2C.vx = p2.vx;
                work->unk2C.vy = p2.vy;
                work->unk2C.vz = p2.vz;
                work->unk2C.vx += work->unk50.vx;
                work->unk2C.vy += work->unk50.vy;
                work->unk2C.vz += work->unk50.vz;
                ex->Hi_SetEffModelSlice(work->unkA0[i + 2], work->unk2C.vy);
                ex->Hi_DrawSliceEffModel(&work->unk24, (VECTOR *)&work->unk2C, (s32)&work->unk3C, work->unkA0[i + 2]);
                if (work->unk98 >= 0 || work->unk9C >= 0) {
                    ex->Hi_ModifyEffModelRGB(work->unkA0[i + 2], work->unk4C, work->unk4C, work->unk4C);
                }
            }
        }
        if (frame >= 0x2D && work->unk0[4] != NULL) {
            if (work->unk0[4]->state == -1) {
                work->unk0[4] = NULL;
            }
            if (work->unk0[4] != NULL) {
                work->unk0[4]->unk5C.vx = work->unk80.vx;
                work->unk0[4]->unk5C.vy = work->unk80.vy;
                work->unk0[4]->unk5C.vz = work->unk80.vz;
            }
        }
        if (frame >= 0x19) {
            func_801E88B4(&D_801E8F8C, frame - 8, &v);
            rot.vx = 0;
            rot.vy = work->unk24.vy;
            rot.vz = 0;
            ex->math_rotMatrixYXZ(&rot, &m);
            gte_SetRotMatrix(&m);
            gte_ldtr0();
            gte_ldv0(&v);
            gte_rtv0tr();
            gte_stlvnl(&lv);
            work->unk90.vx = lv.vx;
            work->unk90.vy = lv.vy;
            work->unk90.vz = lv.vz;
            work->unk90.vx += work->unk50.vx;
            work->unk90.vy += work->unk50.vy;
            work->unk90.vz += work->unk50.vz;
            if (work->unk0[0] != NULL && work->unk0[0]->state == -1) {
                work->unk0[0] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk0[0], (s32)&work->unk90);
        }
        if (frame >= 0x82) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E8754(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E89D4)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8798(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E87F0(Fx128Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx128Point *)data;
}

s32 func_801E8808(Fx128Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx128Key *k;
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

void func_801E889C(Fx128Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx128Point *)data;
}

s32 func_801E88B4(Fx128Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx128Point *p;

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

void func_801E8918(Fx128Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx128Point *)data;
}

s32 func_801E8930(Fx128Keys *keys, s32 i, s32 *out) {
    s32 r = 1;

    if (i > keys->unk2 - 1) {
        i = keys->unk2 - 1;
    } else {
        r = 0;
    }
    *out = *(s16 *)((u8 *)keys->unk4 + (i << 1));
    return r;
}

INCLUDE_RODATA("asm/us/fx_128/nonmatchings/fx_128", fx_128_headerPtr);
