#include "fxmod/fx_0fb_1.h"

INCLUDE_ASM("asm/us/fx_0fb_1/nonmatchings/fx_0fb_1", func_801EC738);

INCLUDE_RODATA("asm/us/fx_0fb_1/nonmatchings/fx_0fb_1", fx_0fb_1_headerPtr);

INCLUDE_ASM("asm/us/fx_0fb_1/nonmatchings/fx_0fb_1", func_801ECBE0);

INCLUDE_ASM("asm/us/fx_0fb_1/nonmatchings/fx_0fb_1", fx_0fb_1_op80);

INCLUDE_ASM("asm/us/fx_0fb_1/nonmatchings/fx_0fb_1", fx_0fb_1_op81);

s32 fx_0fb_1_op82(s32 mode, Fx0fb1Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR rot;
    s32 frame;
    s32 n;
    s32 k;
    u8 *f;
    BtlUnitList *units;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        units = env->unkC;
        n = *arg4;
        D_801F0D2C = env;
        work->unk24 = units->unk24;
        work->unk0 = 0;
        if (n >= work->unk24) {
            return 1;
        }
        work->unk2C = n;
        ex->btlfx_getUnitHomePos(n, (s16 *)&work->unk4);
        ex->btlfx_getUnitRot(work->unk2C, (s16 *)&work->unkC);
        BTLFX_COPY_SVEC(&work->unk1C, &work->unkC);
        work->unk14 = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 16);
        work->unk16 = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 16);
        work->unk18 = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 16);
        work->unk28 = -0x1000 - (ex->btlfx_rand() & 0x3FF);
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            k = frame << 8;
            BTLFX_COPY_SVEC(&v, &work->unk4);
            v.vy += (work->unk28 * ex->math_cos(k >> 2)) >> 12;
            ex->btlfx_setUnitPos(work->unk2C, (s16 *)&v);
            work->unk1C.vx += work->unk14;
            work->unk1C.vy += work->unk16;
            work->unk1C.vz += work->unk18;
            ex->btlfx_setUnitRot(work->unk2C, (s16 *)&work->unk1C);
            if (frame >= 0x10) {
                ex->btlfx_setUnitRot(work->unk2C, (s16 *)&work->unkC);
                ex->btlfx_getUnitHomePos(work->unk2C, (s16 *)&v);
                ex->btlfx_setUnitPos(work->unk2C, (s16 *)&v);
                work->unk0 = 1;
                entry->unk0 = -1;
            }
        } else {
            k = (frame << 12) / 7;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            f = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x12);
            ex->func_800D23D8(f, &work->unk4, &rot, 0x1000, frame, 1, ex->math_cos(k >> 2));
            if (frame >= 7) {
                return 1;
            }
        }
    }
    return 0;
}
