/* Draft of fx_1b0 fx_1b0_op80. Status: 17 aligned mismatches, two tint blocks only: the original loads the
 * drawScreenTint argument a1 = 1 before computing c, and its x2 / >> 1 then use a1 (sllv/srav); c also lands in a2
 * directly. Registers elsewhere match (the unk3C-init loop needs its own counter j). */
#include "fxmod/fx_1b0.h"

s32 fx_1b0_op80(s32 mode, Fx1b0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR w;
    VECTOR scale;
    Pool6CEntry *p;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 a;
    s32 b;
    s32 c;
    s32 i;
    s32 j;

    if (mode == 0) {
        *arg4 = 0x5C;
    } else if (mode == 1) {
        D_801EBF34 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&v);
        work->unk8[0] = v.vx;
        work->unk8[1] = v.vy;
        work->unk8[2] = v.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
        ex->btlfx_getUnitScale(0x10, work->unk1C);
        work->unk14.vy += 0x800;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk28);
        D_801EC714.vx = work->unk28.vx;
        D_801EC714.vy = work->unk28.vy;
        D_801EC714.vz = work->unk28.vz;
        work->unk30.vx = work->unk28.vx;
        work->unk30.vy = work->unk28.vy;
        work->unk30.vz = work->unk28.vz;
        work->unk30.vy = 0;
        env->unk2C->unk0[3].vx = v.vx;
        env->unk2C->unk0[3].vy = v.vy;
        D_801EBF38 = entry;
        env->unk2C->unk0[3].vz = v.vz;
        work->unk0 = 0;
        work->unk4 = 0;
    } else {
        a = 0;
        b = 0;
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            env->unk28 = 0xC8;
            t = (frame << 12) / 90;
            if (frame < 0x3D) {
                v.vz = 0;
                v.vy = 0;
                v.vx = 0;
                v.pad = 1;
                v.vx = ex->math_cos(t >> 2);
                for (i = 0; i < 4; i++) {
                    ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), &work->unk28, &v, 0x4000, 0, 1,
                                      ex->math_sin(t >> 1));
                    v.vx += 0x200;
                }
                v.vx = 0;
                v.vy = ex->math_cos(t >> 2);
                for (i = 0; i < 4; i++) {
                    ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), &work->unk28, &v, 0x4000, 0, 1,
                                      ex->math_sin(t >> 1));
                    v.vy += 0x200;
                }
            }
            env->unk28 = 0;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 2, 0x40, 0x40, 0x55);
            }
            if (frame == 1) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x14) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 2, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x15) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x16) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0x40, 0x40, 0x55);
            }
            if ((u32)(frame - 0x20) < 8) {
                c = 0xC8 - (frame - 0x20) * 0x19;
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, c, c, c);
            }
            if (frame == 0x2A || frame == 0x30) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 2, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x2B || frame == 0x31) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame >= 0x4A) {
                a = ((frame - 0x4A) * 0xFF) / 16;
                b = a * 2 + a;
            }
            if (frame == 0x3C || frame == 0x46 || frame == 0x50) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 2, 0, 0x80, 0xFF);
            }
            if (frame == 0x3D || frame == 0x47 || frame == 0x51) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame >= 0x46 && (ex->btlfx_rand() & 3) == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0x40, 0x40, 0x20);
            }
            if (frame >= 0x3C) {
                ex->func_800E651C(D_801E8CAC, &work->unk38, frame - 0x3C);
            }
            if (frame == 0x3B) {
                ex->func_800E806C(0x10, 0);
                ex->func_800E87C4(0x64);
                ex->btlfx_initEffModels(D_801EBF44, 0xA);
                for (j = 0; j < 8; j++) {
                    work->unk3C[j] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E8CEC[j]), 0);
                }
            }
            if (frame >= 0x3C) {
                work->unk14.vy += t >> 2;
                for (i = 0; i < 8; i++) {
                    ex->Hi_DrawEffModel(&work->unk14, (VECTOR *)work->unk8, (s32)work->unk1C, work->unk3C[i]);
                    ex->Hi_ModifyEffModelRGB(work->unk3C[i], work->unk38.r - 0x80, work->unk38.g - 0x80, work->unk38.b);
                }
            }
            if (frame == 0x10) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x28) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x31) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk28);
                if (p != NULL) {
                    ((EffMotion *)p->unk0)->unk8 = func_801E7E48;
                    p->unk14 = (s32)D_801E8D44;
                    p->unk12 = 0x20;
                    p->unkC = 0xAC;
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x3C || frame == 0x46) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x59) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0xFF, 0xFF, 0xFF);
            }
            if (frame >= 0x5A) {
                work->unk0 = 1;
                entry->unk0 = -1;
                buf = ((Fx1b0Summon *)env->unk4)->unk54->unk50 + 0x19000;
                func_801E7704((Fx1b0Split *)buf, (SVECTOR (*)[3])(buf + 0xDB88));
                for (i = 0; i < 8; i++) {
                    func_801E774C(work->unk3C[i]);
                }
                ex->func_800E3C58();
            }
            break;
        case 1:
            w.vx = 0;
            w.vy = 0;
            w.vz = 0;
            if (frame == 0) {
                func_801E7868(&w);
            }
            work->unk14.vy += 0x18;
            for (i = 0; i < 8; i++) {
                ex->Hi_DrawEffModel(&work->unk14, (VECTOR *)work->unk8, (s32)work->unk1C, work->unk3C[i]);
            }
            func_801E7B04();
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 2, 0, 0xFF, 0xFF);
            }
            if (frame < 8) {
                c = 0xFF - (frame * 0xFF) / 8;
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, c, c, c >> 1);
            }
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk14 = (s32)D_801EA2C4;
                    p->unk12 = 0x18;
                    p->unkC = 0x8C;
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 1) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk14 = (s32)D_801EAFE4;
                    p->unk12 = 0x1C;
                    p->unkC = 0x8C;
                    p->unk22 = 0x140;
                }
            }
            if (frame >= 0x80) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (a != 0) {
            ((CVECTOR *)&v)->r = ((CVECTOR *)&v)->g = ((CVECTOR *)&v)->b = a;
            w.vx = 0;
            w.vy = 0;
            w.vz = 0;
            scale.vx = scale.vy = scale.vz = b + 0x1000;
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &w, &scale, (CVECTOR *)&v, 1, 0);
        }
        work->unk4++;
    }
    return 0;
}
