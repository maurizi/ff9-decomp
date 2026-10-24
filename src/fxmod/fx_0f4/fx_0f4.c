#include "fxmod/fx_0f4.h"

s32 fx_0f4_op80(s32 mode, Fx0f4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x70;
    } else if (mode == 1) {
        D_801E7DC0 = env;
        ex->btlfx_initEffModels(D_801E7DD0, 2);
        work->unk58.vx = env->targets->centerX;
        work->unk58.vy = env->targets->centerY;
        work->unk58.vz = env->targets->centerZ;
        work->unk58.vx = 0;
        work->unk14.vx = 0;
        work->unk14.vy = 0;
        work->unk14.vz = 0;
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk40.vx = 0;
        work->unk40.vz = 0;
        work->unk48.vx = 0;
        work->unk48.vy = 0;
        work->unk48.vz = 0;
        work->unk1C.vy = work->unk58.vy;
        work->unk40.vy = 0x200;
        work->unk1C.vx = work->unk58.vx;
        work->unk1C.vz = work->unk58.vz;
        work->unk6C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xA), 0x11A, 0, 0, 0x80);
        work->unk6E = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xB), 0x11A, 0, 0, 0x80);
        work->unk60.vx = work->unk1C.vx;
        work->unk60.vy = work->unk1C.vy;
        work->unk60.vz = work->unk1C.vz;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk58);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk58);
        work->unk68 = -1;
        work->unk6A = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0xF:
            p = work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk58);
            if (p != NULL) {
                p->unk22 = -0x100;
            }
            break;
        case 0x32:
            break;
        case 0x36:
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->unk58);
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk58);
            if (work->unkC != NULL) {
                work->unkC->unk22 = 0x80;
            }
            if (work->unk10 != NULL) {
                work->unk10->unk22 = 0x100;
            }
            break;
        case 0x52:
            work->unk6A = 0x10;
            break;
        }
        if ((u32)(frame - 0x32) < 0x30) {
            t = frame - 0x32;
            if (t >= 4) {
                ex->Hi_DrawEffModel(&work->unk14, &work->unk1C, (s32)&work->unk2C, work->unk6C);
                ex->Hi_ModifyEffModelAbr(work->unk6C, 1);
            }
            ex->Hi_DrawEffModel(&work->unk40, &work->unk1C, (s32)&work->unk48, work->unk6E);
            ex->Hi_ModifyEffModelAbr(work->unk6E, 1);
            if (work->unk68 >= 0 || work->unk6A >= 0) {
                if (work->unk68 >= 0) {
                    work->unk3C = func_801E7D20(-0x80, 0, 3 - work->unk68, 3);
                    work->unk68--;
                }
                if (work->unk6A >= 0) {
                    work->unk3C = func_801E7D20(0, -0x100, 0x10 - work->unk6A, 0x10);
                    work->unk6A--;
                }
                if (t >= 4) {
                    ex->Hi_ModifyEffModelRGB(work->unk6C, work->unk3C, work->unk3C, work->unk3C);
                }
                ex->Hi_ModifyEffModelRGB(work->unk6E, work->unk3C, work->unk3C, work->unk3C);
            }
            if (t >= 4) {
                work->unk2C.vx += 0x40;
                work->unk2C.vz += 0x40;
                if (t >= 0x10) {
                    work->unk2C.vy -= 8;
                } else {
                    work->unk2C.vy += 0x40;
                }
            }
            work->unk48.vx += 0x40;
            work->unk48.vz += 0x40;
            if (t >= 0xC) {
                work->unk48.vy -= 8;
            } else {
                work->unk48.vy += 0x55;
            }
            work->unk2C.vx = work->unk2C.vx > 0x666 ? 0x666 : work->unk2C.vx;
            work->unk2C.vy = work->unk2C.vy > 0xCCC ? 0xCCC : work->unk2C.vy;
            work->unk2C.vz = work->unk2C.vz > 0x666 ? 0x666 : work->unk2C.vz;
            work->unk48.vx = work->unk48.vx > 0x666 ? 0x666 : work->unk48.vx;
            work->unk48.vy = work->unk48.vy > 0xCCC ? 0xCCC : work->unk48.vy;
            work->unk48.vz = work->unk48.vz > 0x666 ? 0x666 : work->unk48.vz;
        }
        if (frame >= 0x32) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 8), frame);
        } else {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xE), frame);
        }
        if (frame >= 0x78) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E7CDC(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7DC0)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7D20(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0f4/nonmatchings/fx_0f4", fx_0f4_headerPtr);
