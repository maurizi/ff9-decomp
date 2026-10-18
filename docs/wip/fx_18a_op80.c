/* Draft of fx_18a fx_18a_op80. Status: 191 aligned mismatches, 43 ignoring registers; equivalent by construction.
 * Variable roles were brute-forced (build/w/roles2.py). The original spills t (0x88, cases 0-2) and c (0x8C) and keeps
 * frame in fp; using one t for cases 0-2 reproduces that allocation (work s7, frame fp) but spills case 2's
 * t + 0x1400 (the original keeps it in s5) and grows the frame by 8 (277 rows), so this version gives case 1 its own
 * variable. Also open: case 0 stores the particle straight to work->unk8 (no copy to s0). */
#include "fxmod/fx_18a.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    Pool6CEntry *unk8;
    SVECTOR unkC; /* caster position */
    SVECTOR unk14; /* caster body position */
    s32 unk1C; /* charge counter */
    s32 unk20;
    s32 unk24; /* model */
} Fx18aWork80; /* 0x28 */

extern s32 D_801E89D8;
extern BtlModel D_801E8A34[];

s32 fx_18a_op80(s32 mode, Fx18aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    VECTOR pos;
    CVECTOR col;
    SVECTOR ang;
    VECTOR scale;
    SVECTOR rot2;
    s32 t;
    s32 c;
    s32 state;
    s32 n1;
    s32 n2;
    s32 frame;
    s32 a;
    s32 f;
    s32 b;
    s32 g;
    s32 x1;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E8A24 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk14);
        work->unk20 = 0;
        work->unk4 = 0;
        D_801E89D8 = 0;
        ex->btlfx_initEffModels(D_801E8A34, 1);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 1), 0xB7, 0x3D80, 0, 0);
    } else {
        c = 0;
        state = work->unk0;
        frame = entry->unk0;
        switch (state) {
        case 0:
            t = frame << 8;
            n2 = t >> 2;
            n1 = (ex->math_sin(n2) * 0x1400) >> 12;
            env->unk28 = 0x100;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unk14, NULL, n1, 0, 1, t);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk14, NULL, 0x1000, 0, 1, t);
            env->unk28 = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, &rot, n1, 0, 1, t);
            sc.vx = sc.vz = 0x200;
            sc.vy = ex->math_sin(n2) / 5;
            c = t;
            if (frame >= 0x10) {
                work->unk0 = 1;
                entry->unk0 = -1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk14);
                work->unk8 = p;
                if (p != NULL) {
                    work->unk8->unk22 = ex->func_800E7CC4(0x10, 0x20);
                }
                work->unk1C = 0;
            }
            break;
        case 1:
            x1 = ((++work->unk1C % 10) << 12) / 10;
            a = (ex->math_sin(x1) >> 4) + 0x1400;
            env->unk28 = 0x100;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unk14, NULL, a, 0, 1, 0x1000);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk14, NULL, 0x1000, 0, 1, 0x1000);
            env->unk28 = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, &rot, a, 0, 1, 0x1000);
            sc.vx = sc.vz = 0x200;
            sc.vy = (ex->math_sin(x1) >> 7) + 0x333;
            c = 0x1000;
            if (work->unk8 != NULL && work->unk8->unk30 == -1) {
                work->unk8 = NULL;
            }
            if (frame >= 0xB && x1 == 0) {
                work->unk0 = 2;
                entry->unk0 = -1;
                env->unk2C->unk52 = 0;
                ex->func_800E806C(0x10, -1);
                ex->func_800E806C(0, -2);
                work->unkC.vz += 0x64;
                ex->btlfx_setUnitPos(0, (s16 *)&work->unkC);
                ex->func_800E3D54(work->unk8);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0x10, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0x10, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0x10, 0x20);
                }
            }
            break;
        case 2:
            ang.vx = 0;
            ang.vy = 0;
            ang.vz = 0;
            t = (frame << 12) / 180;
            col.r = col.g = col.b = (((0x1000 - t) << 6) >> 12) - 0x74;
            if (frame < 0x8C) {
                b = ex->math_cos(t >> 2) >> 3;
            } else {
                b = ex->math_cos(((frame - 0x8C) << 10) / 40) >> 2;
            }
            scale.vx = scale.vy = scale.vz = b + 0x1000;
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &ang, &scale, &col, 0, 0);
            if (frame >= 0x8C) {
                rot2.vx = 0x400;
                rot2.pad = 1;
                rot2.vz = 0;
                rot2.vy = 0;
                env->unk28 = 0x20;
                g = ((frame - 0x8C) << 12) / 160 >> 2;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk14, &rot2,
                                  ex->math_sin(g) * 5, 0, 1, ex->math_cos(g));
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk14, &rot2,
                                  ex->math_sin(g) * 2, 0, 1, ex->math_cos(g));
                rot2.vx += 0x200;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk14, &rot2,
                                  ex->math_sin(g) * 3, 0, 1, ex->math_cos(g));
                rot2.vx -= 0x400;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk14, &rot2,
                                  ex->math_sin(g) * 3, 0, 1, ex->math_cos(g));
                env->unk28 = 0;
            }
            f = 0x1000 - t;
            env->unk28 = 0x100;
            x1 = t + 0x1400;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unk14, NULL, x1, 0, 1, f);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk14, NULL, 0x1000, 0, 1, f);
            if (frame < 6) {
                g = (frame << 10) / 6;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk14, NULL,
                                  ex->math_sin(g) * 2, 0, 1, ex->math_cos(g));
            }
            env->unk28 = 0;
            if (frame == 0x1E) {
                ex->func_800E83EC();
            }
            if (frame < 4) {
                g = ex->math_cos(frame << 8) * 0xFF;
                f = g >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, f, g >> 14, g >> 13);
            }
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, &rot, x1, 0, 1, f);
            if (frame < 0x16) {
                g = (frame << 12) / 88 >> 2;
                c = ex->math_cos(g);
                sc.vx = sc.vz = (ex->math_sin(g) >> 2) + 0x174;
                sc.vy = ex->math_cos(g) / 4;
            }
            if (frame >= 0xB4) {
                D_801E89D8 = 1;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (c != 0) {
            pos.vx = work->unkC.vx;
            pos.vy = work->unkC.vy;
            rot.vx = 0;
            pos.vz = work->unkC.vz;
            rot.vz = 0;
            rot.vy = work->unk4 << 5;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk24);
            ex->Hi_ModifyEffModelRGB(work->unk24, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            sc.vy >>= 1;
            sc.vx = sc.vz = sc.vx * 3 / 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk24);
            ex->Hi_ModifyEffModelRGB(work->unk24, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        }
        ex->func_800E6624(work->unk4 * 4, 0x1C0, 0x120, 0x20, 0x40, 0x1E0, 0x120);
        work->unk4++;
    }
    return 0;
}
