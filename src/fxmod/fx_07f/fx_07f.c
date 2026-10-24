#include "fxmod/fx_07f.h"

s32 fx_07f_op80(s32 mode, Fx07fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    Pool6CEntry *p;
    s32 frame;
    s32 s;
    s32 t;
    s32 sxz;
    s32 sy;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x34;
    } else if (mode == 1) {
        D_801E7B4C = env;
        BTLFX_COPY_SVEC(&work->unk8, &env->targets->centerX);
        BTLFX_COPY_SVEC(&work->unk10, &work->unk8);
        work->unk18[0] = work->unk8.vx;
        work->unk18[1] = work->unk8.vy;
        work->unk18[2] = work->unk8.vz;
        work->unk24.vx = 0;
        work->unk24.vy = 0;
        work->unk24.vz = 0;
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk10.vy -= 0x180;
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unk8);
        if (p != NULL) {
            p->unk22 = 0x20;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk8);
        if (p != NULL) {
            p->unk22 = -0x20;
        }
        ex->btlfx_initEffModels(D_801E7B5C, 2);
        work->unk2C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 5), 0xB7, 0x3D40, 0, 0);
        work->unk30 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 1), 0x139, 0xFFFF, 0x40, 0x80);
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 41;
            s = ex->math_sin(t >> 1);
            sy = ex->math_sin(t >> 2) * 3;
            sxz = t + 0x1800;
            scale.vx = sxz;
            scale.vy = sy;
            scale.vz = sxz;
            if (frame < 0xC) {
                work->unk24.vy = ex->math_sin((frame << 10) / 12) >> 2;
            } else {
                work->unk24.vy = 0x400;
            }
            ex->Hi_DrawEffModel(&work->unk24, (VECTOR *)work->unk18, (s32)&scale, work->unk2C);
            ex->Hi_ModifyEffModelAbr(work->unk2C, 1);
            c = (s >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk2C, c, c, c);
            scale.vx = sxz;
            scale.vy = sy;
            scale.vz = sxz;
            work->unk24.vy = frame * 0x18;
            ex->Hi_DrawEffModel(&work->unk24, (VECTOR *)work->unk18, (s32)&scale, work->unk30);
            ex->Hi_ModifyEffModelAbr(work->unk30, 1);
            ex->Hi_ModifyEffModelRGB(work->unk30, c, c, c);
            if (frame == 0xC) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x46;
                }
            }
            if (frame >= 0x29) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 2), work->unk4);
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_07f/nonmatchings/fx_07f", fx_07f_headerPtr);
