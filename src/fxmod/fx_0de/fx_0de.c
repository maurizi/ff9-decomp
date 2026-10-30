#include "fxmod/fx_0de.h"

s32 fx_0de_op80(s32 mode, Fx0deWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    SVECTOR rot;
    VECTOR pos;
    VECTOR sc;
    s32 frame;
    s32 t;
    s32 v;
    s32 m;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        D_801E8698 = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, &work->unk4.vx);
        ex->btlfx_getUnitPos(0, &work->unkC.vx);
        ex->btlfx_initEffModels(D_801E86A8, 8);
        work->unk14[0] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 5), 0x39, 0x3D00, 0, 0x80);
        work->unk14[1] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 2), 0x39, 0x3D00, 0, 0x80);
        work->unk14[2] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 3), 0x39, 0x3D00, 0, 0x80);
        work->unk14[3] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 4), 0x39, 0x3D00, 0, 0x80);
        work->unk14[4] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xD), 0x39, 0x3D00, 0, 0x80);
        work->unk14[5] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 7), 0x3B, 0x3D01, 0, 0x80);
        work->unk14[6] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 6), 0x3B, 0x3D01, 0, 0x80);
        work->unk14[7] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 8), 0x39, 0x3D00, 0, 0x80);
        v = (ex->btlfx_getUnitRadius(0) << 12) / 720;
        if (v < 0x400) {
            v = 0x6A4;
        }
        if (v > 0x1800) {
            v = 0x1800;
        }
        D_801E8CE8 = v;
        work->unk34 = v;
    } else {
        frame = entry->frame;
        if (work->unk0 != 0) {
            return 0;
        }
        if (frame < 0x18) {
            t = (frame << 12) / 24;
            pos.vx = work->unkC.vx;
            pos.vy = work->unkC.vy;
            pos.vz = work->unkC.vz;
            v = (((ex->math_sin(t >> 2) / 10) + 0x199) * work->unk34) >> 12;
            rot.vz = 0;
            rot.vx = 0;
            m = (0x1000 - t) << 2;
            rot.vy = -frame << 5;
            sc.vy = (v * 3) >> 2;
            sc.vz = v;
            sc.vx = v;
            if (m > 0x3000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk14[0], work->unk14[1], m - 0x3000);
            } else if (m > 0x2000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk14[1], work->unk14[2], m - 0x2000);
            } else if (m > 0x1000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk14[2], work->unk14[3], m - 0x1000);
            } else {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk14[3], work->unk14[4], m);
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk14[7]);
            ex->Hi_ModifyEffModelRGB(work->unk14[7], (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80);
            /* Dead test, removed after reload: it keeps m live (and call-crossing) through this branch, so
             * global-alloc gives it s2 (as in fx_05f op80). */
            if (m != 0) {
                v = 0;
            }
        } else {
            m = ((frame - 0x18) << 12) / 26;
            c = m >> 2;
            pos.vx = work->unkC.vx;
            pos.vy = work->unkC.vy;
            pos.vz = work->unkC.vz;
            sc.vx = sc.vz = (work->unk34 * 818) >> 12;
            sc.vy = (sc.vx * 3) >> 2;
            rot.vz = 0;
            rot.vx = 0;
            rot.vy = -frame << 5;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk14[4]);
            ex->Hi_ModifyEffModelRGB(work->unk14[4], (ex->math_cos(c) >> 5) - 0x80, (ex->math_cos(c) >> 5) - 0x80, (ex->math_cos(c) >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk14[7]);
            ex->Hi_ModifyEffModelRGB(work->unk14[7], (ex->math_cos(c) >> 5) - 0x80, (ex->math_cos(c) >> 5) - 0x80, (ex->math_cos(c) >> 5) - 0x80);
        }
        ex->btlfx_drawScrollMove(frame, 0x240, 0x180, 0x10, 0x40, D_801E85F0[frame % 23 + 1].unk0, D_801E85F0[frame % 23 + 1].unk2);
        if (frame >= 0x18) {
            t = ((frame - 0x18) << 12) / 26;
            pos.vx = work->unkC.vx;
            pos.vy = work->unkC.vy;
            pos.vz = work->unkC.vz;
            sc.vx = sc.vz = (work->unk34 << 11) >> 12;
            sc.vy = ((ex->math_sin(t >> 2) / 4 + 0x2AA) * work->unk34) >> 12;
            rot.vz = 0;
            rot.vx = 0;
            rot.vy = -frame << 6;
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk14[5], work->unk14[6], 0x1000 - t);
            /* Scheduling barrier (as in fx_05f op80). */
            do {
            } while (0);
            t >>= 1;
            ex->Hi_ModifyEffModelRGB(work->unk14[5], (ex->math_sin(t) >> 5) - 0x80, (ex->math_sin(t) >> 5) - 0x80, (ex->math_sin(t) >> 5) - 0x80);
        }
        if (frame == 0x10) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
            if (p != NULL) {
                p->unk22 = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame == 1) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unk4);
            if (p != NULL) {
                p->unk22 = ex->btlfx_addRadiusSixth(0, 0x40);
            }
        }
        if (frame >= 0x32) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_0de_op81(s32 mode, Fx0deWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 i;
    s32 a;

    if (mode == 0) {
        *arg4 = 0x68;
    } else if (mode == 1) {
        D_801E8698 = env;
        work->unk40 = env->targets->targetCount;
        for (i = 0; i < work->unk40; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk0[i]);
            work->unk44 = ((-0x190 - (ex->btlfx_rand() & 0x7F)) * D_801E8CE8) >> 12;
            ex->btlfx_getUnitBonePos(i, 0, (u16 *)&v);
            work->unk48[i] = -v.vy;
        }
    } else {
        frame = entry->frame;
        if (frame < 0x20) {
            a = frame << 7;
            for (i = 0; i < work->unk40; i++) {
                BTLFX_COPY_SVEC(&v, &work->unk0[i]);
                v.vy += (work->unk44 * ex->math_sin(a >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
        } else {
            a = (frame - 0x20) << 10;
            for (i = 0; i < work->unk40; i++) {
                BTLFX_COPY_SVEC(&v, &work->unk0[i]);
                v.vy += (work->unk48[i] * a) >> 12;
                v.vy += (work->unk44 * ex->math_cos(a >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
        }
        if (frame >= 0x24) {
            for (i = 0; i < work->unk40; i++) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&v);
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
            return 1;
        }
    }
    return 0;
}

s32 fx_0de_op84(s32 mode, Fx0deWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 u;
    s32 k;
    s32 v;
    u16 ry;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E8698 = env;
        work->unk0 = 0;
        work->unk4 = 0;
    } else {
        a = 0;
        b = 0;
        c = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->unk8);
        ry = 0;
        switch (work->unk0) {
        case 0:
            ry = 0x200;
            a = (frame << 12) / 3;
            u = ex->math_sin(a >> 2);
            b = u >> 1;
            c = ((0x1000 - a) >> 1) + 0x800;
            if (frame >= 3) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            u = (frame << 12) / 9;
            k = (frame & 1) << 8;
            c = k + 0x1000;
            v = (u >> 1) + 0x800;
            a = k + v;
            ry = (u >> 3) + 0x200;
            b = c;
            if (frame >= 9) {
                work->unk0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            u = (frame << 12) / 9;
            t = u >> 2;
            ry = 0x400 - t;
            a = ex->math_cos(t);
            b = ex->math_sin(t) + 0x1000;
            c = ex->math_sin(t) * 3 / 2 + 0x1000;
            if (frame >= 9) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->otZ = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xE), &work->unk8, NULL, b / 3, 0, 1, a / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xF), &work->unk8, &rot, c / 2, 0, 1, a * 2 / 3);
        env->otZ = 0;
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0de/nonmatchings/fx_0de", fx_0de_headerPtr);
