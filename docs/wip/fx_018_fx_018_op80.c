/* Status: compiles; 389 aligned mismatches vs original, mostly register allocation (orig keeps ex in s6, constant 1
 * in s4, frame in s7). Logic follows m2c: state switch on work->unk10 (0: effects at frames 0/5/28/29, 1:
 * shockwave, 2: ring of 16 particles with radius D_801E9E80). */
#include "fxmod/fx_018.h"

s32 fx_018_op80(s32 mode, Fx018Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR d;
    SVECTOR d2;
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR v;
    Pool6CEntry *p;
    s32 frame;
    s32 c;
    s32 a;
    s32 r;
    s32 i;
    s32 n;
    s32 ang;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E9CE8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        ex->btlfx_initEffModels(D_801E9CF0, 2);
        work->unk14 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0xB8, 0x3E40, 0, 0x80);
        work->unk18 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0x119, 0xFFFF, 0, 0x80);
        work->unk10 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk10) {
        case 0:
            if (frame == 0) {
                work->unk1C = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk8);
                if (work->unk1C != NULL) {
                    work->unk1C->unk22 = 0x46;
                }
            }
            if (frame == 5) {
                work->unk20 = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk8);
                if (work->unk20 != NULL) {
                    work->unk20->unk22 = 0x46;
                }
            }
            if (frame == 28) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x40, 0);
            }
            if (frame == 29) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x80, 0x80);
            }
            if (frame >= 29) {
                work->unk10 = 1;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unk1C);
                ex->func_800E3D54(work->unk20);
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            break;
        case 1:
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x20, 0x40, 0xFF);
            }
            if (frame == 1) {
                env->unk28 = 0x200;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &work->unk8, NULL, 0x2800, 0, 1, 0x1000,
                                  0, 0);
                env->unk28 = 0;
            }
            env->unk28 = 0x200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), &work->unk8, NULL, 0x800, 0, 1, -1);
            env->unk28 = 0;
            if (frame == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x80, 0xFF);
            }
            if (frame >= 2) {
                work->unk10 = 2;
                entry->unk0 = -1;
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                work->unk1C = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x26), (s32)&work->unk8);
                if (work->unk1C != NULL) {
                    work->unk1C->unk22 = 0x80;
                }
            }
            break;
        case 2:
            if (frame < 6) {
                c = ex->math_cos((frame << 10) / 6) * 0xFF;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c >> 14, c >> 13, c >> 12);
            }
            if (frame < 20) {
                c = ex->math_cos((frame << 10) / 20);
                env->unk28 = 0x200;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x17), &work->unk8, NULL, 0x2800, 0, 1, c, 0, 0);
                if (frame < 10) {
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &work->unk8, NULL, 0x2800, 0, 1,
                                      0x1000 - (frame << 12) / 10, 0, 0);
                }
                env->unk28 = 0;
            }
            if (frame < 24) {
                env->unk28 = 0x200;
                a = (frame << 12) / 96;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), &work->unk8, NULL,
                                  ex->math_cos(a) * 3 / 2 + 0x1000, 0, 1, ex->math_cos(a));
                env->unk28 = 0;
            }
            d.vx = 0x400;
            d.vz = 0;
            d.vy = 0;
            if (work->unk1C != NULL) {
                if (work->unk1C->unk30 == -1) {
                    work->unk1C = NULL;
                }
                if (work->unk1C != NULL) {
                    work->unk1C->unk5C.vx = d.vx;
                    work->unk1C->unk5C.vy = d.vy;
                    work->unk1C->unk5C.vz = d.vz;
                }
            }
            if (frame < 16) {
                d.vx = 0x400;
                d.vz = 0;
                d.vy = 0;
                d.pad = 1;
                a = frame << 8;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), &work->unk8, &d,
                                  ex->math_sin(a >> 2) * 4, 0, 1, 0x1000 - a);
            }
            d2.vx = 0x400;
            d2.vz = 0;
            d2.vy = 0;
            d2.pad = 1;
            a = (frame << 12) / 200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &work->unk8, &d2, ex->math_sin(a) * 4, 0, 1,
                              ex->math_cos(a));
            if (frame == 1) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x27), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame < 24) {
                pos.vx = work->unk8.vx;
                ang = frame * 0x18;
                pos.vy = work->unk8.vy;
                rot.vx = 0;
                rot.vy = ang;
                rot.vz = 0;
                pos.vz = work->unk8.vz;
                a = (frame << 12) / 96;
                r = ((ex->math_sin(a) * 0x5DC) >> 12) + 0x140;
                scale.vz = r;
                scale.vy = r;
                scale.vx = r;
                D_801E9E80 = (r * 0x708) >> 12;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk14);
                ex->Hi_ModifyEffModelAbr(work->unk14, 1);
                n = frame;
                i = 0;
                ex->Hi_ModifyEffModelRGB(work->unk14, ex->math_cos(a) * 2 / 96 - 0x80, ex->math_cos(a) * 2 / 96 - 0x80,
                                         ex->math_cos(a) * 2 / 96 - 0x80);
                env->unk28 = 0x20;
                c = ex->math_cos(a);
                do {
                    v.vz = work->unk8.vz;
                    v.vx = work->unk8.vx;
                    v.vx += (ex->math_cos(ang) * D_801E9E80) >> 12;
                    v.vz += (ex->math_sin(ang) * D_801E9E80) >> 12;
                    ang += 0x100;
                    n += 3;
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &v, NULL, 0x1800 - ((i & 1) << 12),
                                      n & 7, 1, c, ex->btlfx_randCentered(5), ex->btlfx_randCentered(5));
                    i++;
                } while (i < 16);
                env->unk28 = 0;
            }
            if (frame >= 50) {
                work->unk10 = 2;
                return 1;
            }
            break;
        }
    }
    return 0;
}
