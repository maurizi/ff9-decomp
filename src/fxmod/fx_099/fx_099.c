#include "fxmod/fx_099.h"

s32 fx_099_op80(s32 mode, Fx099Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    s32 t;
    s32 r;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E7C28 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        work->unk8.vy = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        ex->btlfx_initEffModels(D_801E7C38, 1);
        work->unk1C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 0);
        r = (ex->btlfx_getUnitRadius(0) << 12) / 720;
        if (r < 0x800) {
            r = 0x800;
        }
        if (r > 0x2000) {
            r = 0x2000;
        }
        work->unk18 = r / 2;
        work->unk20.vx = 0;
        work->unk20.vy = 0;
        work->unk20.vz = 0;
        work->unk28[0] = work->unk18;
        work->unk28[1] = work->unk18;
        work->unk28[2] = work->unk18;
        work->unk34[0] = work->unk8.vx;
        work->unk34[1] = work->unk8.vy;
        work->unk34[2] = work->unk8.vz;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            for (i = 0; i < 5; i++) {
                if (frame >= i * 2) {
                    t = ((frame - i * 2) << 12) / 7;
                    if (t > 0x1000) {
                        t = 0x1000;
                    }
                    rot.vx = work->unk20.vx;
                    rot.vy = work->unk20.vy;
                    rot.vz = work->unk20.vz;
                    rot.vy += (i << 12) / 5;
                    rot.vx += 0x200 - (ex->math_sin(t >> 2) >> 3);
                    ex->Hi_SetEffModelSlice(work->unk1C, 0);
                    ex->Hi_DrawSliceEffModel(&rot, (VECTOR *)work->unk34, (s32)work->unk28, work->unk1C);
                }
            }
            if (frame >= 0x17) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            if (frame < 0x10) {
                c = (ex->math_cos((frame << 10) >> 4) >> 5) - 0x80;
                for (i = 0; i < 5; i++) {
                    if (frame >= i * 2) {
                        t = ((frame - i * 2) << 12) / 7;
                        if (t > 0x1000) {
                            t = 0x1000;
                        }
                        rot.vx = work->unk20.vx;
                        rot.vy = work->unk20.vy;
                        rot.vz = work->unk20.vz;
                        rot.vy += (i << 12) / 5;
                        ex->Hi_SetEffModelSlice(work->unk1C, 0);
                        ex->Hi_DrawSliceEffModel(&rot, (VECTOR *)work->unk34, (s32)work->unk28, work->unk1C);
                        ex->Hi_ModifyEffModelAbr(work->unk1C, 1);
                        ex->Hi_ModifyEffModelRGB(work->unk1C, c, c, c);
                    }
                }
            }
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x80);
                }
            }
            if (frame == 8) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x80);
                }
            }
            if (frame >= 0x20) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_099/nonmatchings/fx_099", fx_099_headerPtr);
