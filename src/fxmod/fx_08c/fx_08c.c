#include "fxmod/fx_08c.h"

s32 fx_08c_op80(s32 mode, Fx08cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    Pool6CEntry *p;
    s32 frame;
    s32 a;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk10.vx = env->targets->centerX;
        work->unk10.vy = env->targets->centerY;
        work->unk10.vz = env->targets->centerZ;
        work->unk8.vx = env->targets->centerX;
        work->unk8.vy = env->targets->centerY;
        work->unk8.vz = env->targets->centerZ;
        work->unk10.vy -= 0x100;
        work->unk18[0] = work->unk8.vx;
        work->unk18[1] = work->unk8.vy;
        work->unk18[2] = work->unk8.vz;
        D_801E7AA8 = env;
        ex->btlfx_initEffModels(D_801E7AB8, 2);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 3), 0x138, 0xFFFF, 0, 0x80);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk8);
        if (p != NULL) {
            p->unk22 = 0x80;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk8);
        if (p != NULL) {
            p->unk22 = 0x80;
        }
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            a = (frame << 7) >> 1;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            c = ex->math_sin(a);
            scale.vx = ((a + 0x2800) * 4) / 3;
            scale.vz = scale.vx;
            scale.vy = ((frame << 9) + 0x2000) / 3;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)work->unk18, (s32)&scale, work->unk24);
            ex->Hi_ModifyEffModelAbr(work->unk24, 1);
            ex->Hi_ModifyEffModelRGB(work->unk24, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            scale.vy >>= 1;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)work->unk18, (s32)&scale, work->unk24);
            ex->Hi_ModifyEffModelAbr(work->unk24, 1);
            ex->Hi_ModifyEffModelRGB(work->unk24, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            if (frame >= 0x20) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 1), work->unk4);
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_08c/nonmatchings/fx_08c", fx_08c_headerPtr);
