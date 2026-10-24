#include "fxmod/fx_1a4.h"

s32 fx_1a4_op80(s32 mode, Fx1a4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR w;
    s32 frame;
    s32 d;
    s32 e;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x44;
    } else if (mode == 1) {
        D_801E8168 = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk14);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk14);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        work->unk3C = -1;
        work->unk3E = -1;
        work->unk40 = -1;
        work->unk38 = env->targets->unk25[1] == 0;
    } else {
        frame = entry->unk0;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk1C);
        switch (frame) {
        case 0x1E:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&D_801E83D0);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&D_801E83D8);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            break;
        case 0x49:
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk1C);
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk1C);
            break;
        case 0x4C:
            if (work->unk38 != 0) {
                work->unk3E = -1;
                work->unk3C = 0x60;
                ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk24);
                work->unk24.vy = 0;
            }
            break;
        case 0x6A:
            work->unk40 = 0x20;
            break;
        }
        if (work->unk3C >= 0) {
            work->unk3C--;
            e = 0x60 - work->unk3C;
            v.vx = work->unk24.vx;
            v.vy = work->unk24.vy + e * e;
            v.vz = work->unk24.vz;
            ex->btlfx_setUnitPos(0x10, (s16 *)&v);
        }
        if (work->unk3E >= 0) {
            work->unk3E--;
            d = 0x10 - work->unk3E;
            w.vx = work->unk2C.vx;
            w.vy = work->unk2C.vy - (d * 0x50 - d * 5 * d);
            w.vz = work->unk2C.vz;
            ex->btlfx_setUnitPos(0x10, (s16 *)&w);
        }
        if (work->unk40 >= 0) {
            work->unk34 = func_801E80C8(0, -0x80, 0x20 - work->unk40, 0x20);
            c = work->unk34 + 0x80;
            if (c < 0x46) {
                ex->btlfx_fadeUnitById(0x10, 0x46);
            } else {
                ex->btlfx_fadeUnitById(0x10, c);
            }
            work->unk40--;
        }
        if (frame >= 0x1E) {
            if (work->unk4 != NULL && work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk4, (s32)&work->unk1C);
            if (work->unk8 != NULL && work->unk8->unk30 == -1) {
                work->unk8 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk8, (s32)&work->unk1C);
        }
        return frame >= 0x96;
    }
    return 0;
}

s32 fx_1a4_op81(s32 mode, Fx1a4Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x5C;
    } else if (mode == 1) {
        D_801E8168 = env;
        if (env->targets->unk25[1] != 0) {
            return 1;
        }
        ex->btlfx_initEffModels(D_801E8178, 3);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk44);
        work->unk8.vx = 0x800;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
        work->unk10.vx = 0x800;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk18.vx = 0x800;
        work->unk18.vy = 0;
        work->unk18.vz = 0;
        work->unk30.vx = 0;
        work->unk30.vy = 0;
        work->unk30.vz = 0;
        work->unk20.vx = work->unk44.vx;
        work->unk20.vy = work->unk44.vy;
        work->unk20.vz = work->unk44.vz;
        work->unk54 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 7), 1);
        work->unk56 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xE), 1);
        work->unk58 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xF), 1);
        work->unk52 = -1;
        work->unk4E = 8;
        work->unk4C = 0;
        work->unk50 = 0x10;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk44);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk44);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            break;
        case 0x2D:
            work->unk52 = 0x15;
            break;
        }
        if (frame >= 0 && frame < 0x42) {
            ex->Hi_DrawEffModel(&work->unk8, &work->unk20, (s32)&work->unk30, work->unk54);
            ex->Hi_DrawEffModel(&work->unk10, &work->unk20, (s32)&work->unk30, work->unk56);
            ex->Hi_DrawEffModel(&work->unk18, &work->unk20, (s32)&work->unk30, work->unk58);
            if (work->unk52 >= 0) {
                work->unk40 = func_801E80C8(0, -0x80, 0x15 - work->unk52, 0x15);
                ex->Hi_ModifyEffModelRGB(work->unk54, work->unk40, work->unk40, work->unk40);
                ex->Hi_ModifyEffModelRGB(work->unk56, work->unk40, work->unk40, work->unk40);
                ex->Hi_ModifyEffModelRGB(work->unk58, work->unk40, work->unk40, work->unk40);
                work->unk52--;
            }
            work->unk8.vy += 0x80;
            work->unk10.vy += 0x80;
            work->unk18.vy += 0x40;
            if (frame >= 0xC) {
                if (work->unk30.vx >= 0x40) {
                    work->unk30.vx -= 0x40;
                } else {
                    work->unk30.vx = 0;
                }
                if (work->unk30.vz >= 0x40) {
                    work->unk30.vz -= 0x40;
                } else {
                    work->unk30.vz = 0;
                }
            } else {
                work->unk30.vx += 0x800;
                work->unk30.vy += 0x400;
                work->unk30.vz += 0x800;
                work->unk30.vx = work->unk30.vx > 0xCCC ? 0xCCC : work->unk30.vx;
                work->unk30.vy = work->unk30.vy > 0x1000 ? 0x1000 : work->unk30.vy;
                work->unk30.vz = work->unk30.vz > 0xCCC ? 0xCCC : work->unk30.vz;
            }
            ex->btlfx_drawScrollMove(work->unk4E * 0x64 - work->unk4C * work->unk4E, 0x1E0, 0x180, 0x20, 0x80, 0x1E0, 0x100);
            ex->btlfx_drawScrollMove(work->unk50 * 0x64 - work->unk4C * work->unk50, 0x200, 0x180, 0x20, 0x80, 0x200, 0x100);
            work->unk4C++;
        }
        if (frame >= 0x64) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E8084(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8168)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E80C8(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_1a4/nonmatchings/fx_1a4", fx_1a4_headerPtr);
