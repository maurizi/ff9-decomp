#include "fxmod/fx_114.h"

s32 func_801E777C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E784C(pt, pos, &D_801EF268[0], frame, n);
}

s32 func_801E77B0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E784C(pt, pos, &D_801EF268[1], frame, n);
}

s32 func_801E77E4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E784C(pt, pos, &D_801EF268[2], frame, n);
}

s32 func_801E7818(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E784C(pt, pos, &D_801EF268[3], frame, n);
}

s32 func_801E784C(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n) {
    pos->vx = src->vx;
    pos->vy = src->vy;
    pos->vz = src->vz;
    pt->unk0.vx = pos->vx << 12;
    pt->unk0.vy = pos->vy << 12;
    pt->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

void func_801E78AC(Fx114Path *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
    SVECTOR v;
    SVECTOR *p;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;

    cnt = (size - 4) / 8;
    p = (SVECTOR *)(buf + 4);
    buf[0] = buf[2] = cnt - 1;
    buf[1] = 0;
    if (n < cnt) {
        for (i = 0; i < cnt - n; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        n = cnt;
    }
    for (i = 0; i < n; i++) {
        k = i + 1;
        j = n - k + start;
        if (clip != 0 && j >= keys->unk2) {
            break;
        }
        func_801EA6F4(keys, j, &v);
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = from + (to - from) / keys->unk2 * (n - i);
        p++;
    }
}

s32 fx_114_op80(s32 mode, Fx114Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v40;
    SVECTOR v48;
    SVECTOR v50;
    SVECTOR v58;
    SVECTOR r;
    VECTOR s;
    CVECTOR col;
    s32 frame;
    s32 i;
    s32 q;
    s32 k;
    s32 c;
    s32 t;
    DR_STP *dp;
    s32 n;
    s32 q1;
    s32 k1;
    s32 t1;
    s32 q2;
    s32 k2;
    s32 k3;
    s32 k4;

    if (mode == 0) {
        *arg4 = 0x198;
    } else if (mode == 1) {
        D_801EB768 = env;
        ex->btlfx_initEffModels(D_801EB778, 0x20);
        work->unk20.vx = 0;
        work->unk20.vy = 0x800;
        work->unk20.vz = 0;
        work->unk28.vx = 0;
        work->unk28.vy = 0;
        work->unk28.vz = -0x4B0;
        work->unk38.vx = 0x1800;
        work->unk38.vy = 0x1800;
        work->unk38.vz = 0x1800;
        work->unk4C.pad = 1;
        work->unk4C.vx = 0x400;
        work->unk4C.vy = 0;
        work->unk4C.vz = 0;
        work->unk54.vx = 0;
        work->unk54.vy = 0;
        work->unk54.vz = -0x4B0;
        work->unk5C = 0x1000;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->unk68.vx = 0;
        work->unk68.vy = 0;
        work->unk68.vz = 0;
        work->unk78.vx = 0x1000;
        work->unk78.vy = 0x1000;
        work->unk78.vz = 0x1000;
        func_801EA880(&D_801ED0C8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x95));
        work->unk178 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x80), 1);
        work->unk88.vx = 0;
        work->unk88.vy = 0;
        work->unk88.vz = 0;
        work->unk90.vx = 0;
        work->unk90.vy = 0;
        work->unk90.vz = -0x4B0;
        work->unkA0.vx = 0x1000;
        work->unkA0.vy = 0x1000;
        work->unkA0.vz = 0x1000;
        func_801EA808(&D_801ED0E8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x84));
        work->unk17A = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x85));
        work->unkB4.vx = 0;
        work->unkB4.vy = 0;
        work->unkB4.vz = 0;
        work->unkBC.vx = 0;
        work->unkBC.vy = 0x50;
        work->unkBC.vz = -0x4B0;
        work->unkCC.vx = 0x1000;
        work->unkCC.vy = 0x1000;
        work->unkCC.vz = 0x1000;
        func_801EA808(&D_801ED0D0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x86));
        work->unk17C = REG_TEX_LIST(ex)((void *)ex->btlfx_getFileData(entry->unk8, 0x81), D_801EB6A4, D_801EB6BC);
        work->unkE0.vx = 0;
        work->unkE0.vy = 0;
        work->unkE0.vz = 0;
        work->unkE8.vx = 0;
        work->unkE8.vy = -0xC8;
        work->unkE8.vz = -0x4B0;
        work->unkF8.vx = 0x1000;
        work->unkF8.vy = 0x1000;
        work->unkF8.vz = 0x1000;
        func_801EA808(&D_801ED0D8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x87));
        work->unk17E = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x82), 0x9A, 0x3DC0, 0, 0);
        work->unk10C.vx = 0;
        work->unk10C.vy = 0;
        work->unk10C.vz = 0;
        work->unk114.vx = 0;
        work->unk114.vy = -0xC8;
        work->unk114.vz = -0x4B0;
        work->unk124.vx = 0x1000;
        work->unk124.vy = 0x1000;
        work->unk124.vz = 0x1000;
        func_801EA808(&D_801ED0E0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x88));
        work->unk180 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x83), 0x117, 0, 0, 0);
        work->unk15C = 0;
        work->unk15D = 0;
        work->unk15E = 0;
        work->unk16C = 0;
        work->unk16E = -1;
        work->unk170 = -1;
        work->unk172 = -1;
        work->unk174 = -1;
        work->unk176 = -1;
    } else {
    frame = entry->unk0;
    switch (frame) {
    case 0:
        work->unk160 = 0;
        work->unk164 = 1;
        work->unk168 = 0x10;
        func_801EA6DC(&D_801ED090, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x96));
        func_801EA6DC(&D_801ED098, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x8A));
        func_801EA6DC(&D_801ED0C0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x94));
        break;
    case 0x2F:
        work->unk28.vy = 0;
        work->unk168 = 0;
        func_801EA65C((Fx114Track12 *)&D_801ED0B0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x90));
        func_801EA65C((Fx114Track12 *)&D_801ED0B8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x8F));
        work->unkBC.vy = 0x50;
        work->unkBC.vx = 0;
        work->unkBC.vz = -0x320;
        break;
    case 0x30:
        work->unk160 = 1;
        work->unk164 = 0;
        ex->Hi_SetSummonMotion(((Fx114Summon80 *)env->unk4)->unk54->unk180[work->unk160],
                               ((Fx114Summon80 *)env->unk4)->unk60);
        break;
    case 0x61:
        work->unk168 = 0;
        func_801EA65C((Fx114Track12 *)&D_801ED0B0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x91));
        func_801EA65C((Fx114Track12 *)&D_801ED0B8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x8D));
        break;
    case 0x62:
        work->unk160 = 2;
        work->unk164 = 0;
        ex->Hi_SetSummonMotion(((Fx114Summon80 *)env->unk4)->unk54->unk180[work->unk160],
                               ((Fx114Summon80 *)env->unk4)->unk60);
        break;
    case 0x7A:
        work->unk168 = 0;
        func_801EA65C((Fx114Track12 *)&D_801ED0B0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x92));
        func_801EA65C((Fx114Track12 *)&D_801ED0B8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x8E));
        break;
    case 0x7B:
        work->unk160 = 3;
        work->unk164 = 0;
        ex->Hi_SetSummonMotion(((Fx114Summon80 *)env->unk4)->unk54->unk180[work->unk160],
                               ((Fx114Summon80 *)env->unk4)->unk60);
        break;
    case 0xA7:
        work->unk168 = 0x10;
        func_801EA6DC(&D_801ED0A0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x2B));
        func_801EA6DC(&D_801ED0A8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x2C));
        break;
    }
    switch (frame) {
    case 0:
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&D_801EF288);
        break;
    case 0x28:
        work->unk13C.vx = 0;
        work->unk13C.vy = 0;
        work->unk13C.vz = -0x4B0;
        work->unk144.vx = 0;
        work->unk144.vy = 0;
        work->unk144.vz = -0x4B0;
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk13C);
        work->unk172 = 6;
        break;
    case 0x36:
        work->unk174 = 0xD;
        work->unk176 = 0xD;
        break;
    case 0x2A:
        work->unk16E = 6;
        break;
    case 0x30:
        work->unk170 = 6;
        break;
    case 0x7B:
        work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&D_801EF298[0]);
        work->unk14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1B), (s32)&D_801EF298[1]);
        work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1A), (s32)&D_801EF298[2]);
        work->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1C), (s32)&D_801EF298[3]);
        if (work->unk10 != NULL) {
            work->unk10->unk14 = (s32)D_801EE318;
            work->unk10->unk12 = 1;
            work->unk10->unkC = 0xD0;
        }
        if (work->unk14 != NULL) {
            work->unk14->unk14 = (s32)D_801EE3E8;
            work->unk14->unk12 = 1;
            work->unk14->unkC = 0xD0;
        }
        if (work->unk18 != NULL) {
            work->unk18->unk14 = (s32)D_801EE4B8;
            work->unk18->unk12 = 1;
            work->unk18->unkC = 0xD0;
        }
        if (work->unk1C != NULL) {
            work->unk1C->unk14 = (s32)D_801EE588;
            work->unk1C->unk12 = 1;
            work->unk1C->unkC = 0xD0;
        }
        if (work->unk10 != NULL) {
            ((EffMotion *)work->unk10->unk0)->unk8 = func_801E777C;
        }
        if (work->unk14 != NULL) {
            ((EffMotion *)work->unk14->unk0)->unk8 = func_801E77B0;
        }
        if (work->unk18 != NULL) {
            ((EffMotion *)work->unk18->unk0)->unk8 = func_801E77E4;
        }
        if (work->unk1C != NULL) {
            ((EffMotion *)work->unk1C->unk0)->unk8 = func_801E7818;
        }
        break;
    case 0x2C:
        ex->btlfx_stopEmitter(work->unk0);
        work->unk14C.vx = 0;
        work->unk14C.vy = 0;
        work->unk14C.vz = -0x4B0;
        work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk14C);
        if (work->unk8 != NULL) {
            work->unk8->unk14 = (s32)D_801ED5C8;
            work->unk8->unk12 = 0x55;
            work->unk8->unkC = 0x28;
        }
        break;
    case 0xA8:
        ex->btlfx_stopEmitter(work->unk10);
        ex->btlfx_stopEmitter(work->unk14);
        ex->btlfx_stopEmitter(work->unk18);
        ex->btlfx_stopEmitter(work->unk1C);
        break;
    }
    switch (frame) {
    case 0x2C:
        ex->Hi_FreeEffModel(work->unk178);
        break;
    case 0x2E:
        ex->Hi_FreeEffModel(work->unk17A);
        break;
    case 0x43:
        ex->Hi_FreeEffModel(work->unk17C);
        break;
    }
    if (frame >= 0 && frame < 0x2F) {
        func_801EA6F4(&D_801ED090, work->unk168, &v40);
        env->camera->points[0].vx = 0;
        env->camera->points[0].vy = v40.vy;
        env->camera->points[0].vz = -0x4B0;
        func_801EA6F4(&D_801ED098, work->unk168, &v40);
        env->camera->points[1].vx = v40.vx;
        env->camera->points[1].vy = v40.vy;
        env->camera->points[1].vz = v40.vz;
    }
    if (frame >= 0x2F && frame < 0xA8) {
        func_801EA670(&D_801ED0B0, work->unk168, &v48);
        env->camera->points[0].vx = work->unk28.vx + v48.vx * 3 / 2;
        env->camera->points[0].vy = work->unk28.vy + v48.vy * 3 / 2;
        env->camera->points[0].vz = work->unk28.vz + v48.vz * 3 / 2;
        func_801EA670(&D_801ED0B8, work->unk168, &v48);
        env->camera->points[1].vx = work->unk28.vx + v48.vx * 3 / 2;
        env->camera->points[1].vy = work->unk28.vy + v48.vy * 3 / 2;
        env->camera->points[1].vz = work->unk28.vz + v48.vz * 3 / 2;
    }
    if (frame >= 0xA7 && frame < 0xDE) {
        func_801EA6F4(&D_801ED0A0, work->unk168, &v50);
        env->camera->points[0].vx = v50.vx;
        env->camera->points[0].vy = v50.vy;
        env->camera->points[0].vz = v50.vz;
        func_801EA6F4(&D_801ED0A8, work->unk168, &v50);
        env->camera->points[1].vx = v50.vx;
        env->camera->points[1].vy = v50.vy;
        env->camera->points[1].vz = v50.vz;
    }
    if (frame >= 0 && frame < 0xA8) {
        func_801EA6F4(&D_801ED0C0, frame + 0x10, &v58);
        work->unk28.vy = v58.vy;
        if (work->unk160 != 0) {
            ex->Hi_DrawSummonModel(&work->unk20, (VECTOR *)&work->unk28, (s32)&work->unk38,
                                   ((Fx114Summon80 *)env->unk4)->unk60, work->unk164);
            if (work->unk160 > 0) {
                k1 = frame / 16 % 8;
                q1 = frame % 16;
                t1 = q1 << 9;
                if (q1 >= 9) {
                    t1 = 0x1000 - ((q1 - 8) << 9);
                }
                func_801EABB4(&work->unk54, &work->unk4C, 0x11A, 0, 0xC0, 2, k1, 0x100, 0x10, 0x10, work->unk5C, t1, 1,
                              D_801EEA68, 0);
            }
        }
    }
    if (work->unk16E >= 0) {
        func_801EA914(0, 0xFF, 6 - work->unk16E, 6, 1, 0);
        work->unk16E--;
    }
    if (work->unk170 >= 0) {
        func_801EA914(0xFF, 0, 6 - work->unk170, 6, 1, 0);
        work->unk170--;
    }
    if (frame >= 0x30 && frame < 0xF3) {
        work->unk15C = 0x40;
        work->unk15D = 0x20;
        work->unk15E = 0;
        if (frame < 0xDE) {
            if ((frame / 16) & 1) {
                work->unk15C -= (frame % 16) * 2;
                work->unk15D -= frame % 16;
            } else {
                work->unk15C += (frame % 16) * 2 - 0x1E;
                work->unk15D += frame % 16 - 0xF;
            }
        }
        if (frame < 0xA8) {
            work->unk48 = 0x20;
            ex->Hi_ModifySummonModelRGB(((Fx114Summon80 *)env->unk4)->unk60, 0x20, 0x20, 0x20);
        }
        if (frame >= 0xE3) {
            k3 = frame - 0xE3;
            work->unk15C -= work->unk15C * k3 / 16;
            work->unk15D -= work->unk15D * k3 / 16;
        }
        c = 0x40;
        if (frame >= 0xE3) {
            c = 0x40 - ((frame - 0xE3) << 6) / 16;
        }
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, c, c, c);
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->unk15C, work->unk15D, work->unk15E);
    }
    if (frame >= 0 && frame < 0x2C) {
        work->unk68.vx = work->unk28.vx;
        work->unk68.vy = work->unk28.vy;
        work->unk68.vz = work->unk28.vz;
        func_801EA898(&D_801ED0C8, frame + 0x10, (VECTOR *)&work->unk78);
        ex->Hi_DrawEffModel(&work->unk60, (VECTOR *)&work->unk68, (s32)&work->unk78, work->unk178);
        ex->Hi_ModifyEffModelAbr(work->unk178, 1);
        work->unk134.vx = work->unk68.vx;
        work->unk134.vy = work->unk68.vy;
        work->unk134.vz = work->unk68.vz;
        if (work->unk0 != NULL && work->unk0->unk30 == -1) {
            work->unk0 = NULL;
        }
        ex->btlfx_moveEmitterTo((s32)work->unk0, (s32)&work->unk134);
        work->unk60.vy += 0x40;
    }
    if (frame >= 0x28 && frame < 0x2E) {
        func_801EA81C(&D_801ED0E8, frame, (VECTOR *)&work->unkA0);
        ex->Hi_DrawEffModel(&work->unk88, (VECTOR *)&work->unk90, (s32)&work->unkA0, work->unk17A);
        ex->Hi_ModifyEffModelAbr(work->unk17A, 1);
        if (work->unk172 >= 0) {
            work->unkB0 = func_801EA604(0, -0xFF, 6 - work->unk172, 6);
            ex->Hi_ModifyEffModelRGB(work->unk17A, work->unkB0, work->unkB0, work->unkB0);
            work->unk172--;
        }
    }
    if (frame >= 0x29 && frame < 0x43) {
        n = frame - 0x29;
        if (work->unkDC >= -0x7F) {
            func_801EA81C(&D_801ED0D0, frame + 0x10, (VECTOR *)&work->unkCC);
            ex->Hi_DrawEffModel(&work->unkB4, (VECTOR *)&work->unkBC, (s32)&work->unkCC, work->unk17C);
            ex->Hi_ModifyEffModelAbr(work->unk17C, 1);
            if (work->unk174 >= 0) {
                work->unkDC = func_801EA604(0, -0x80, 0xD - work->unk174, 0xD);
                ex->Hi_ModifyEffModelRGB(work->unk17C, work->unkDC, work->unkDC, work->unkDC);
                work->unk174--;
            }
        }
        if (work->unk108 >= -0x7F) {
            func_801EA81C(&D_801ED0D8, frame + 0x10, (VECTOR *)&work->unkF8);
            func_801EA81C(&D_801ED0E0, frame + 0x10, (VECTOR *)&work->unk124);
            ex->Hi_DrawEffModel(&work->unkE0, (VECTOR *)&work->unkE8, (s32)&work->unkF8, work->unk17E);
            ex->Hi_DrawEffModel(&work->unk10C, (VECTOR *)&work->unk114, (s32)&work->unk124, work->unk180);
            ex->Hi_ModifyEffModelAbr(work->unk17E, 1);
            ex->Hi_ModifyEffModelAbr(work->unk180, 1);
            if (work->unk176 >= 0) {
                work->unk108 = func_801EA604(0, -0x80, 0xD - work->unk176, 0xD);
                ex->Hi_ModifyEffModelRGB(work->unk17E, work->unk108, work->unk108, work->unk108);
                ex->Hi_ModifyEffModelRGB(work->unk180, work->unk108, work->unk108, work->unk108);
                work->unk176--;
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 6), n);
        ex->btlfx_drawScrollMove(0x1180 - work->unk16C * 16, 0x280, 0x100, 0x40, 0x80, 0x240, 0x100);
    }
    if (frame >= 0x7B && frame < 0xB7) {
        for (i = 0; i < 4; i++) {
            ex->Hi_GetSummonBonePos(((Fx114Summon80 *)env->unk4)->unk60, D_801EB71C[i], (u16 *)&D_801EF268[i]);
        }
    }
    if (frame >= 0xA2 && frame < 0xA8) {
        k4 = frame - 0xA2;
        dp = (DR_STP *)env->prim;
        env->prim = (u8 *)(dp + 1);
        setlen(dp, 2);
        dp->code[0] = 0xE6000001;
        dp->code[1] = 0;
        addPrim(env->ot + 1, dp);
        s.vz = s.vy = s.vx = (k4 << 7) + 0x1000;
        col.r = col.g = col.b = 0x3C;
        r.vx = 0;
        r.vy = 0;
        r.vz = 0;
        ex->btlfx_drawScreenGrid(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &r, &s, &col, 1, 0);
        env->camera->projDist = 0x208 - k4 * k4 * 6;
    }
    work->unk168++;
    work->unk16C++;
    if (frame >= 0x118) {
        ex->btlfx_clearEffModels();
        return 1;
    }
    }
    return 0;
}

s32 fx_114_op81(s32 mode, Fx114Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x48;
    } else if (mode == 1) {
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk20.vx = 0x1000;
        work->unk20.vy = 0x1000;
        work->unk20.vz = 0x1000;
        D_801EB768 = env;
        func_801EA808(&D_801ED0E8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x89));
        work->unk46 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x85));
        work->unk44 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 1:
            ex->Hi_GetSummonBonePos(((Fx114Summon *)env->unk4)->unk60, 0x1B, (u16 *)&work->unk34);
            work->unk10.vx = work->unk34.vx;
            work->unk10.vy = work->unk34.vy;
            work->unk10.vz = work->unk34.vz;
            work->unk3C.vx = work->unk34.vx;
            work->unk3C.vy = work->unk34.vy;
            work->unk3C.vz = work->unk34.vz;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk34);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk3C);
            work->unk44 = 0xC;
            break;
        case 0xD:
            ex->Hi_FreeEffModel(work->unk46);
            ex->btlfx_stopEmitter(work->unk0);
            ex->btlfx_stopEmitter(work->unk4);
            break;
        }
        if (frame >= 1 && frame < 0xD) {
            func_801EA81C(&D_801ED0E8, frame - 1, &work->unk20);
            ex->Hi_DrawEffModel(&work->unk8, &work->unk10, (s32)&work->unk20, work->unk46);
            ex->Hi_ModifyEffModelAbr(work->unk46, 1);
            if (work->unk44 >= 0) {
                work->unk30 = c = func_801EA604(0, -0xFF, 0xC - work->unk44, 0xC);
                ex->Hi_ModifyEffModelRGB(work->unk46, c, c, c);
                work->unk44--;
            }
        }
        return frame >= 0x14;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_114/nonmatchings/fx_114", fx_114_headerPtr);

s32 fx_114_op82(s32 mode, Fx114Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    CVECTOR col;
    DR_MODE *dr;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        work->unk4.vx = 0;
        work->unk4.vy = 0;
        work->unk4.vz = 0;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0x320;
        work->unk1C.vx = 0x1000;
        work->unk1C.vy = 0x1000;
        work->unk1C.vz = 0x1000;
        D_801EB768 = env;
        work->unk66 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x2A), 0x117, 0, 0, 0);
        ex->Hi_SetEffModelOffset(work->unk66, 0x6E);
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0x320;
        work->unk44.vx = 0x1000;
        work->unk44.vy = 0x1000;
        work->unk44.vz = 0x1000;
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0x17, 0x3D00, 0x80, 0xE0);
        D_801ED078.unk10 = 0x60;
        D_801ED078.unk8 = 0;
        D_801ED078.unkA = 0;
        D_801ED078.unkC = 0x1000;
        D_801ED078.unkE = 0x1000;
        D_801ED078.unk12 = 0x30;
        work->unk60 = -1;
        work->unk62 = -1;
        work->unk64 = 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x100, 0x80, 0xA0, D_801ED1C8, 0, 0x168, 0xF6E);
            break;
        case 9:
            work->unk58.vx = -3;
            work->unk58.vy = 0;
            work->unk58.vz = 0x320;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk58);
            break;
        case 8:
            work->unk60 = 4;
            break;
        case 2:
            work->unk62 = 6;
            break;
        case 29:
            ex->Hi_FreeEffModel(work->unk66);
            break;
        case 18:
            ex->Hi_FreeEffModel(work->unk68);
            break;
        }
        if ((u32)frame < 0x1D) {
            ex->Hi_DrawEffModel(&work->unk4, &work->unkC, (s32)&work->unk1C, work->unk66);
            ex->Hi_ModifyEffModelRGB(work->unk66, 6, 6, 6);
        }
        if ((u32)(frame - 8) < 10) {
            D_801ED078.unkA = (frame - 8) * 2;
            D_801ED078.unkA = D_801ED078.unkA >= 10 ? 9 : D_801ED078.unkA;
            if (work->unk60 >= 0) {
                D_801ED078.unkC = func_801EA604(0, 0x1000, 4 - work->unk60, 4);
                work->unk60--;
            }
            if (work->unk62 >= 0) {
                D_801ED078.unkC = func_801EA604(0x1000, 0, 6 - work->unk62, 6);
                work->unk62--;
            }
            for (i = 0, buf = D_801EE658[0]; i < 13; i++) {
                func_801EA6DC(&D_801ED078.unk0, (u16 *)func_801EA8FC(ex->btlfx_getFileData(entry->unk8, 0x17), (u16)i));
                func_801E78AC(&D_801ED078.unk0, buf, 0x50, D_801ED078.unk8, D_801ED078.unkA, D_801ED078.unk10, D_801ED078.unk12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, NULL, 0, D_801ED078.unkE, D_801ED078.unkC, 1,
                                    buf, 0);
                buf += 0x50;
            }
        }
        if ((u32)(t = frame - 0xE) < 4) {
            dr = (DR_MODE *)env->prim;
            env->prim = (u8 *)(dr + 1);
            setlen(dr, 2);
            dr->code[0] = 0xE6000001;
            dr->code[1] = 0;
            addPrim(&env->ot[1], dr);
            sc.vz = sc.vy = sc.vx = (t << 7) + 0x1000;
            col.r = col.g = col.b = 0x3C;
            rot.vz = rot.vy = rot.vx = 0;
            ex->btlfx_drawScreenGrid(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &sc, &col, 1, NULL);
            env->camera->projDist = 0x208 - t * t * 4;
        }
        work->unk64++;
        return frame >= 0x1E;
    }
    return 0;
}

s32 fx_114_op83(s32 mode, Fx114Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR w;
    s32 frame;
    s32 i;
    s32 j;

    if (mode == 0) {
        *arg4 = 0x150;
    } else if (mode == 1) {
        work->unkC.vx = 0x400;
        work->unk14.vy = -0x78;
        work->unk14.vz = 0x320;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        work->unkC.pad = 1;
        work->unk14.vx = 0;
        work->unk1C = 0x1800;
        work->unk20.vx = 0;
        work->unk20.vy = 0;
        work->unk20.vz = 0;
        work->unk28.vx = 0;
        work->unk28.vy = 0;
        work->unk28.vz = 0x320;
        work->unk38.vx = 0x1000;
        work->unk38.vy = 0x1000;
        work->unk38.vz = 0x1000;
        D_801EB768 = env;
        work->unk118[0] = REG_TEX_LIST(ex)((void *)ex->btlfx_getFileData(entry->unk8, 0x12), D_801EB6D4, D_801EB6E0);
        ex->Hi_SetEffModelOffset(work->unk118[0], 0x6E);
        work->unk48.vx = 0;
        work->unk48.vy = 0;
        work->unk48.vz = 0;
        work->unk50.vx = 0;
        work->unk50.vy = 0;
        work->unk50.vz = 0x320;
        work->unk60.vx = 0x1000;
        work->unk60.vy = 0x1000;
        work->unk60.vz = 0x1000;
        for (j = 0; j < 23; j++) {
            u8 *tex;
            s16 *uv;
            s32 a;
            s32 b;

            if (D_801EB68C[j] != 0) {
                tex = D_801EB6F4;
                uv = D_801EB704;
            } else {
                tex = D_801EB6EC;
                uv = D_801EB6FC;
            }
            a = func_801EA8FC(ex->btlfx_getFileData(entry->unk8, 0x28), j & 0xFFFF);
            b = func_801EA8FC(ex->btlfx_getFileData(entry->unk8, 0x29), j & 0xFFFF);
            work->unk118[j + 1] = REG_TEX_LIST(ex)((void *)a, tex, uv);
            ex->Hi_SetEffModelOffset(work->unk118[j + 1], 0x6E);
            func_801EA758(&D_801ED0F8[j], (u16 *)b);
        }
        work->unk70.vx = 0;
        work->unk70.vy = 0;
        work->unk70.vz = 0;
        work->unk78.vx = 0;
        work->unk78.vy = 0;
        work->unk78.vz = 0x320;
        work->unk88.vx = 0x1000;
        work->unk88.vy = 0x1000;
        work->unk88.vz = 0x1000;
        func_801EA808(&D_801ED0E8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x1D));
        work->unk148 = REG_TEX_LIST(ex)((void *)ex->btlfx_getFileData(entry->unk8, 0x1E), D_801EB70C, D_801EB714);
        work->unk9C.vx = 0;
        work->unk9C.vy = 0;
        work->unk9C.vz = 0;
        work->unkA4.vx = 0;
        work->unkA4.vy = 0;
        work->unkA4.vz = 0x320;
        work->unkB4.vx = 0x1000;
        work->unkB4.vy = 0x1000;
        work->unkB4.vz = 0x1000;
        work->unkCC.vx = 0;
        work->unkCC.vy = 0;
        work->unkCC.vz = 0;
        work->unkD4.vx = 0;
        work->unkD4.vy = 0;
        work->unkD4.vz = 0x320;
        work->unkE4.vx = 0x1000;
        work->unkE4.vy = 0x1000;
        work->unkE4.vz = 0x1000;
        work->unk14A = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x27), 0x9A, 0x3DC0, 0, 0);
        work->unk14C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x22), 0x9A, 0x3DC0, 0, 0);
        func_801EA65C(&D_801ED1B0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x23));
        func_801EA808(&D_801ED1B8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x24));
        func_801EA808(&D_801ED1C0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x25));
        work->unk10C = 0;
        work->unk104.vy = work->unk14.vy;
        work->unk10E = -1;
        work->unk110 = -1;
        work->unk112 = -1;
        work->unk114 = -1;
        work->unk116 = -1;
        work->unk104.vx = work->unk14.vx;
        work->unk104.vz = work->unk14.vz;
        work->unk104.vy -= 0xF0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unkF4.vx = work->unk28.vx;
            work->unkF4.vy = work->unk28.vy;
            work->unkF4.vz = work->unk28.vz;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x26), (s32)&work->unkF4);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unkF4);
            break;
        case 3:
            work->unkD4.vx = work->unk28.vx;
            work->unkD4.vy = work->unk28.vy;
            work->unkD4.vz = work->unk28.vz;
            work->unkFC.vx = work->unk28.vx;
            work->unkFC.vy = work->unk28.vy;
            work->unkFC.vz = work->unk28.vz;
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unkFC);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = 0x100;
            }
            break;
        case 4:
            work->unk110 = 3;
            work->unk114 = 6;
            break;
        case 0x15:
            work->unk112 = 0x10;
            work->unk116 = 0x10;
            work->unk10E = 0x10;
            break;
        }
        if (frame >= 6 && frame < 0x14) {
            Fx114Rock *r;
            s32 t;

            ex->Hi_DrawEffModel(&work->unk20, (VECTOR *)&work->unk28, (s32)&work->unk38, work->unk118[0]);
            ex->Hi_ModifyEffModelRGB(work->unk118[0], 6, 6, 6);
            for (i = 0, r = D_801EB2F4; i < 23; i++, r++) {
                t = frame - 2;
                if (r->unk2 != 0) {
                    t = 0;
                }
                if (t == r->unk0) {
                    v.vx = r->unk18.vx;
                    v.vy = r->unk18.vy;
                    v.vz = r->unk18.vz - 0x190;
                    ex->btlfx_aimAngles((s16 *)&work->unk50, (s16 *)&v, (s16 *)&r->unk8);
                    r->unk4 = ex->btlfx_distance(&v, (SVECTOR *)&work->unk50) / 30;
                    r->unk6 = 0xC8;
                    r->unk6 += ex->btlfx_rand() % 100;
                    r->unk4 += ex->btlfx_rand() % 50;
                    r->unk6 /= 2;
                    r->unk4 /= 2;
                    r->unk8.vx &= 0xFFF;
                    r->unk8.vy &= 0xFFF;
                    r->unk8.vz &= 0xFFF;
                    if (r->unk8.vy < 0x800) {
                        r->unk1 |= 1;
                    } else {
                        r->unk1 |= 2;
                    }
                    if (r->unk8.vy < 0x800) {
                        r->unk1 |= 4;
                    } else {
                        r->unk1 |= 8;
                    }
                }
                if (t >= r->unk0) {
                    v.vx = r->unk18.vx;
                    v.vy = r->unk18.vy;
                    v.vz = r->unk18.vz;
                    ex->btlfx_pointAhead((s16 *)&v, (s16 *)&r->unk8, r->unk4, (s16 *)&v);
                    r->unk18.vx = v.vx;
                    r->unk18.vy = v.vy;
                    r->unk18.vz = v.vz;
                    r->unk18.vy -= r->unk6;
                    if (t < r->unk0 + 2) {
                        if (r->unk1 & 1) {
                            r->unk10.vx += 0x40;
                        } else {
                            r->unk10.vx -= 0x40;
                        }
                        if (r->unk1 & 4) {
                            r->unk10.vz += 0x40;
                        } else {
                            r->unk10.vz -= 0x40;
                        }
                    }
                    work->unk48.vx = r->unk10.vx;
                    work->unk48.vy = r->unk10.vy;
                    work->unk48.vz = r->unk10.vz;
                    r->unk6 -= 0x14;
                    if (r->unk6 >= 4) {
                        r->unk6 -= t * 5;
                    }
                    if (r->unk18.vy >= 0x32) {
                        r->unk2 = 1;
                        continue;
                    }
                } else {
                    func_801EA76C(&D_801ED0F8[i], t, &work->unk48, &v);
                    v.vx = -v.vx;
                    work->unk50.vx = 0;
                    work->unk50.vy = 0;
                    work->unk50.vz = 0x320;
                    work->unk48.vz = -work->unk48.vz;
                    work->unk50.vx += v.vx;
                    work->unk50.vy += v.vy;
                    work->unk50.vz += v.vz;
                    r->unk18.vx = work->unk50.vx;
                    r->unk18.vy = work->unk50.vy;
                    r->unk18.vz = work->unk50.vz;
                    work->unk50.vx = 0;
                    work->unk50.vy = 0;
                    work->unk50.vz = 0x320;
                }
                ex->Hi_DrawEffModel(&work->unk48, (VECTOR *)&r->unk18, (s32)&work->unk60, work->unk118[i + 1]);
                ex->Hi_ModifyEffModelRGB(work->unk118[i + 1], 6, 6, 6);
            }
        }
        if (frame >= 4 && frame < 0x25) {
            func_801EA81C(&D_801ED0E8, frame - 4, (VECTOR *)&work->unk88);
            ex->Hi_DrawEffModel(&work->unk70, (VECTOR *)&work->unk78, (s32)&work->unk88, work->unk148);
            ex->Hi_ModifyEffModelAbr(work->unk148, 1);
            if (work->unk10E >= 0) {
                work->unk98 = func_801EA604(0, -0x80, 0x10 - work->unk10E, 0x10);
                ex->Hi_ModifyEffModelRGB(work->unk148, work->unk98, work->unk98, work->unk98);
                work->unk10E--;
            }
            func_801EA81C(&D_801ED1B8, frame - 4, (VECTOR *)&work->unkB4);
            ex->Hi_DrawEffModel(&work->unk9C, (VECTOR *)&work->unkA4, (s32)&work->unkB4, work->unk14A);
            ex->Hi_ModifyEffModelAbr(work->unk14A, 1);
            if (work->unk110 >= 0 || work->unk112 >= 0) {
                if (work->unk110 >= 0) {
                    work->unkC4 = func_801EA604(-0x80, 0, 3 - work->unk110, 3);
                    work->unk110--;
                }
                if (work->unk112 >= 0) {
                    work->unkC4 = func_801EA604(0, -0x80, 0x10 - work->unk112, 0x10);
                    work->unk112--;
                }
                ex->Hi_ModifyEffModelRGB(work->unk14A, work->unkC4, work->unkC4, work->unkC4);
            }
            work->unk9C.vy += 0x40;
            if (frame >= 4 && frame < 0x25) {
                work->unkD4.vx = 0;
                work->unkD4.vy = 0;
                work->unkD4.vz = 0x320;
                func_801EA670((Fx114Track6 *)&D_801ED1B0, frame - 4, &w);
                work->unkD4.vx += w.vx;
                work->unkD4.vy += w.vy;
                work->unkD4.vz += w.vz;
                func_801EA81C(&D_801ED1C0, frame - 4, (VECTOR *)&work->unkE4);
                ex->Hi_DrawEffModel(&work->unkCC, (VECTOR *)&work->unkD4, (s32)&work->unkE4, work->unk14C);
                ex->Hi_ModifyEffModelAbr(work->unk14C, 1);
                if (work->unk114 >= 0 || work->unk116 >= 0) {
                    if (work->unk114 >= 0) {
                        work->unkC8 = func_801EA604(-0x80, 0, 6 - work->unk114, 6);
                        work->unk114--;
                    }
                    if (work->unk116 >= 0) {
                        work->unkC8 = func_801EA604(0, -0x80, 0x10 - work->unk116, 0x10);
                        work->unk116--;
                    }
                    ex->Hi_ModifyEffModelRGB(work->unk14C, work->unkC8, work->unkC8, work->unkC8);
                }
            }
        }
        ex->btlfx_drawScrollMove(0x3C0 - work->unk10C * 16, 0x280, 0x100, 0x40, 0x80, 0x240, 0x100);
        work->unk10C++;
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_114_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 2;
    } else if (mode == 1) {
        D_801EB768 = env;
        *work = -1;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            *work = 0x10;
        }
        if (*work >= 0) {
            func_801EA914(0xFF, 0, 0x10 - *work, 0x10, 1, 0);
            (*work)--;
        }
        return frame >= 0x10;
    }
    return 0;
}

void func_801EA5C0(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801EB768)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801EA604(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801EA65C(Fx114Track12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx114Key12 *)data;
}

s32 func_801EA670(Fx114Track6 *track, s32 t, SVECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx114Key6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = -k->x;
    out->vy = k->y;
    out->vz = -k->z;
    return done;
}

void func_801EA6DC(Fx114Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx114Point *)data;
}

s32 func_801EA6F4(Fx114Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx114Point *p;

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

void func_801EA758(Fx114Track12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx114Key12 *)data;
}

s32 func_801EA76C(Fx114Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx114Key12 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    if (rot != NULL) {
        rot->vx = k->unk6;
        rot->vy = k->unk8;
        rot->vz = k->unkA;
    }
    if (pos != NULL) {
        pos->vx = -k->unk0;
        pos->vy = k->unk2;
        pos->vz = -k->unk4;
    }
    return done;
}

void func_801EA808(Fx114Track6 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx114Key6 *)data;
}

s32 func_801EA81C(Fx114Track6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx114Key6 *k;

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

void func_801EA880(Fx114Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx114Point *)data;
}

s32 func_801EA898(Fx114Path *path, s32 t, VECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx114Point *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &path->unk4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

s32 func_801EA8FC(s32 base, s32 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

void func_801EA914(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801EB768)->btlfx_drawScreenTint((u32 *)(D_801EB768->ot + ot), arg4, c, c, c);
}

void func_801EA9AC(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801EF2B8[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801EF6B8[i] = 0;
    }
}

void func_801EA9F8(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (D_801EF2B8[i].unk6 == -1) {
            D_801EF2B8[i].unk6 = 0;
            D_801EF2B8[i].unk0 = data;
            D_801EF2B8[i].unk4 = start;
            D_801EF2B8[i].unkA = dst;
            D_801EF2B8[i].unkC = 0;
            D_801EF2B8[i].unkE = c;
            D_801EF2B8[i].unk8 = ch;
            return;
        }
    }
}

void func_801EAA54(s32 ch) {
    Unk800F78A4 *env = D_801EB768;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (D_801EF2B8[i].unk8 == ch) {
            if (D_801EF2B8[i].unk6 == 0 && D_801EF2B8[i].unk4 == D_801EF6B8[ch]) {
                D_801EF2B8[i].unk6 = *D_801EF2B8[i].unk0;
                D_801EF2B8[i].unk0++;
            }
            if (D_801EF2B8[i].unk6 > 0) {
                D_801EF2B8[i].unk6--;
                for (k = 0; k < 3; k++) {
                    d = ((D_801EF2B8[i].unkA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->camera)[d].vx = D_801EF2B8[i].unk0[0];
                        ((SVECTOR *)env->camera)[d].vy = D_801EF2B8[i].unk0[1];
                        ((SVECTOR *)env->camera)[d].vz = D_801EF2B8[i].unk0[2];
                        if (D_801EF2B8[i].unkC < D_801EF2B8[i].unkE) {
                            D_801EF2B8[i].unk0 += 3;
                            D_801EF2B8[i].unkC++;
                        }
                    }
                }
            }
        }
    }
    D_801EF6B8[ch]++;
}

void func_801EABB4(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = D_801EB768;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR v[4];
    CVECTOR c1;
    CVECTOR c3;
    CVECTOR c0;
    CVECTOR c2;
    MATRIX m;
    s32 z;
    s32 flag;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *p;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 hh;
    s32 x0;
    s32 col;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        hh = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * hh);
        tpage = ex->btlfx_allocDrMode(0, abr);
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(pos);
        if (rot == NULL) {
            rot = &zero;
        }
        gte_rtv0tr_b();
        ex->func_800316D8(rot, &m);
        if (scale >= 0) {
            sv.vx = sv.vy = sv.vz = scale;
            ex->ScaleMatrix(&m, &sv);
        }
        gte_stlvnl(m.t);
        gte_rtps();
        code = 0x38;
        if (abr != 0xFF) {
            code = 0x3A;
        }
        gte_stszotz(&z);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        x0 = -(w * size) >> 1;
        h = hh;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = x0;
            p = row;
            *(s32 *)&c0 = 0;
            *(s32 *)&c2 = 0;
            for (i = 0; i < w; i++, prim++, px += size, p++) {
                v[0].vx = v[2].vx = px;
                v[0].vy = v[1].vy = py;
                v[1].vx = v[3].vx = px + size;
                v[2].vy = v[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&c1 = 0;
                    *(s32 *)&c3 = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&c1 = 0;
                    } else {
                        col = p[0];
                        c1.r = ((col & 0x1F) * (bright << 2)) >> 12;
                        c1.g = (((col >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c1.b = (((col >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&c3 = 0;
                    } else {
                        col = p[w - 1];
                        c3.r = ((col & 0x1F) * (bright << 2)) >> 12;
                        c3.g = (((col >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c3.b = (((col >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&v[0], &v[1], &v[2]);
                c0.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&c0;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&c1;
                *(s32 *)&prim->r2 = *(s32 *)&c2;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&flag);
                gte_ldv0xy(&v[3]);
                *(s32 *)&prim->r3 = *(s32 *)&c3;
                gte_rtps_bm();
                *(s32 *)&c0 = *(s32 *)&c1;
                *(s32 *)&c2 = *(s32 *)&c3;
                gte_stsxy(&prim->x3);
                if (flag != 0) {
                    ex->btlfx_addPrim(0x8000000, z, prim);
                }
            }
        }
        ex->btlfx_addPrim(0x1000000, z, tpage);
    }
    w--;
    h--;
    x0 = (tp & 0xF) * 64 + x + (idx % div) * w;
    py = (tp & 0x10) * 16 + y + (idx / div) * h;
    rect.x = x0;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

void func_801EB154(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801EB768;
    u32 *p = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->prim = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->ot + 0xFFF, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->ot + 0xFFF, p);
    }
}
