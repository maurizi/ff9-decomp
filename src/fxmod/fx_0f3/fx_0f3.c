#include "fxmod/fx_0f3.h"

s32 fx_0f3_op80(s32 mode, Fx0f3Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Pool6CEntry *p;
    s32 state;
    s32 frame;
    s32 t;
    s32 d;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk4 = *arg4;
        work->unk0 = 0;
        D_801E7CA0 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        work->unk24 = NULL;
    } else {
        state = work->unk0;
        frame = entry->frame;
        switch (state) {
        case 0:
            t = (frame << 12) / 12;
            BTLFX_COPY_SVEC(&v, &work->unk8);
            switch (work->unk4) {
            case 0:
                v.vy += (t - 0x800) * 2;
                break;
            case 1:
                d = t - 0x800;
                d *= 2;
                v.vx -= d;
                v.vz += d;
                v.vy -= (0x1000 - ex->math_sin(t >> 1)) * 2 / 3;
                break;
            case 2:
                d = t - 0x800;
                d *= 2;
                v.vx += d;
                v.vz += d;
                v.vy -= (0x1000 - ex->math_sin(t >> 1)) * 2 / 3;
                break;
            }
            BTLFX_COPY_SVEC(&work->unk10, &v);
            if (frame == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk10, 0, -1, 0, 0, 1,
                                  D_801E7CB0[work->unk4], 0);
            }
            env->otZ = 0x40;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk10, 0, 0, 0x5000, 0x1000, 1,
                              D_801E7CB0[work->unk4], 0);
            env->otZ = 0;
            if (frame == 0) {
                work->unk18.vx = work->unk10.vx;
                work->unk18.vy = work->unk10.vy;
                work->unk18.vz = work->unk10.vz;
                work->unk24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk18);
                if (work->unk24 != NULL) {
                    work->unk24->unk14 = (s32)D_801E7F80[work->unk4];
                    work->unk24->unk12 = 4;
                    work->unk24->unkC = 0xAC;
                    if (work->unk24 != NULL) {
                        work->unk24->unk22 = 0x40;
                    }
                }
            }
            if (work->unk24 != NULL && work->unk24->state == -1) {
                work->unk24 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk24, (s32)&work->unk10);
            if (frame == 6 && work->unk4 == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 1, 2, 0x40, 0x40, 0x40);
            }
            if (frame == 7) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                ex->btlfx_drawScreenTint((u32 *)env->ot + 1, 1, 0x5D, 0x5D, 0x5D);
            }
            if (frame >= 0xC) {
                work->unk0 = 1;
                entry->frame = -1;
                ex->btlfx_stopEmitter(work->unk24);
            }
            return 0;
        case 1:
            t = (frame << 12) / 29;
            env->otZ = 0x40;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk10, 0, 0, 0x5000,
                              ex->math_cos(t >> 2), 1, D_801E7CB0[work->unk4], 0);
            env->otZ = 0;
            if (frame < 0x1D) {
                break;
            }
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f3/nonmatchings/fx_0f3", fx_0f3_headerPtr);
