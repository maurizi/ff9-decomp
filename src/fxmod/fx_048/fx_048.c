#include "fxmod/fx_048.h"

INCLUDE_ASM("asm/us/fx_048/nonmatchings/fx_048", func_801E7704);

s32 fx_048_op80(s32 mode, Fx048Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos2;
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot2;
    SVECTOR r3;
    VECTOR pos3;
    VECTOR sc2;
    SVECTOR p3;
    Pool6CEntry *p;
    s32 w;
    s32 frame;
    s32 bri;
    s32 scl;
    s32 t;
    s32 u;
    s32 x;
    s32 i;
    s32 cnt;
    s32 ang;
    s32 q;
    s32 r;
    s32 s;
    s32 sy;
    s32 g;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        work->unk0.vx = env->unkC->unk38;
        D_801E8F3C = env;
        work->unk0.vy = env->unkC->unk3A;
        work->unk0.vz = env->unkC->unk3C;
        work->unk0.vy -= 0x200;
        BTLFX_COPY_SVEC(&work->unk8, &work->unk0);
        work->unk8.vy = 0;
        ex->btlfx_initEffModels(D_801E8F4C, 6);
        work->unk14[0] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xC), 1);
        work->unk14[1] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 9), 1);
        work->unk14[2] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xB), 1);
        work->unk14[3] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 2), 1);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0x37, 0x3D06, 0x80, 0x80);
        work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE), 0x37, 0x3D06, 0x80, 0x80);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk8);
        if (p != NULL) {
            p->unk14 = (s32)D_801E93FC;
            p->unk12 = 0x12;
            p->unkC = 0x24;
            p->unk22 = 0x20;
        }
        work->unk2C = NULL;
        work->unk30 = NULL;
        work->unk34 = 0;
        work->unk10 = 0;
    } else {
        bri = 0;
        scl = 0;
        frame = entry->unk0;
        switch (work->unk10) {
        case 0:
            t = (frame << 12) / 24;
            bri = t / 3;
            scl = 0x1800;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            rot.vx = 0x400;
            env->unk28 = 0x200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &work->unk8, NULL, 0x1000, 0, 1, t);
            env->unk28 = 0;
            if (frame >= 0x18) {
                work->unk10 = 1;
                entry->unk0 = -1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            break;
        case 1:
            t = frame << 9;
            ex->btlfx_drawScreenTint((u32 *)env->unk20 + 0x800, 1, (t * 70) >> 12, 0, 0);
            if (frame == 4) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            bri = ex->math_cos(t >> 2);
            env->unk28 = 0x200;
            scl = (t * 2) / 3 + 0x555;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &work->unk8, NULL, 0x1000, 0, 1,
                              ex->math_cos(t >> 2));
            env->unk28 = 0;
            if (frame >= 8) {
                work->unk10 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 46;
            if (frame < 4) {
                x = ex->math_cos(frame << 8) * 0xFF;
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 1, 1, x >> 12, x >> 13, x >> 15);
            }
            if (frame < 0x20) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 0xFFF, 0xFF,
                                         ((ex->math_cos(frame << 5) / 2 + 0x800) * (ex->btlfx_rand() % 50 + 7)) >> 12, 0,
                                         0);
            }
            {
            s32 z;
            s32 f29;
            s32 lo;
            s32 hi;
            s32 ext;
            s32 c8;
            s32 cc;
            s32 d0;
            s32 d4;
            s32 d8;
            s32 dc;
            s32 b;
            s32 e4;

            z = frame << 4;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.pad = 1;
            rot.vz = z;
            f29 = frame < 0x1D;
            if (f29) {
                g = (frame << 12) / 29;
                w = g >> 2;
                bri = ex->math_cos(w);
                pos2.vx = work->unk8.vx;
                pos2.vy = work->unk8.vy;
                scl = g + 0x1000;
                pos2.vz = work->unk8.vz;
                pos2.vy -= 0x400;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &pos2, &rot, ex->math_sin(w) * 2 + 0x1800,
                                  0, 1, ex->math_cos(w));
            }
            env->unk28 = 0x200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &work->unk8, NULL,
                              ex->math_cos(t >> 2) / 2 * 3 + 0x2000, 0, 1, ex->math_cos(t >> 2));
            env->unk28 = 0;
            if (f29) {
                ex->func_800E6624(z, 0x1E0, 0x180, 0x20, 0x80, 0x1E0, 0x100);
                s = ((frame << 12) / 29) >> 2;
                sc.vx = sc.vz = ex->math_sin(s) / 4 + 0x80;
                sc.vy = ex->math_sin(s) * 3 / 2 + 0x1000;
                rot2.vx = 0;
                rot2.vy = frame * 24;
                rot2.vz = 0;
                pos.vx = work->unk8.vx;
                pos.vy = work->unk8.vy;
                pos.vz = work->unk8.vz;
                sc.vx = sc.vz = ex->math_sin(s) / 2 + 0x200;
                sc.vy = (ex->math_cos(s) * 3 / 2 + 0x1000) * 3 / 2;
                ex->Hi_DrawEffModel(&rot2, &pos, (s32)&sc, work->unk24);
                ex->Hi_ModifyEffModelRGB(work->unk24, (ex->math_cos(s) >> 5) - 0x80, (ex->math_cos(s) >> 5) - 0x80,
                                         (ex->math_cos(s) >> 5) - 0x80);
            }
            if (frame == 2) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 0xE) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 3) {
                p = work->unk2C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                p = work->unk30 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            r3.vx = 0x400;
            r3.vy = 0;
            r3.vz = 0;
            if (work->unk2C != NULL) {
                if (work->unk2C->unk30 == -1) {
                    work->unk2C = NULL;
                }
                if (work->unk2C != NULL) {
                    work->unk2C->unk5C.vx = r3.vx;
                    work->unk2C->unk5C.vy = r3.vy;
                    work->unk2C->unk5C.vz = r3.vz;
                }
            }
            if (work->unk30 != NULL) {
                if (work->unk30->unk30 == -1) {
                    work->unk30 = NULL;
                }
                if (work->unk30 != NULL) {
                    work->unk30->unk5C.vx = r3.vx;
                    work->unk30->unk5C.vy = r3.vy;
                    work->unk30->unk5C.vz = r3.vz;
                }
            }
            if (frame < 0x24) {
                u = (frame << 12) / 36;
                ang = -frame << 7;
                b = -frame * 0x19B;
                r3.vx = 0x800;
                ext = 0;
                r3.vy = frame * 24;
                r3.vz = 0;
                q = (u << 7) >> 12;
                d8 = q + 0x80;
                d4 = q + 0x100;
                if (frame < 0x10) {
                    lo = 0x4E2;
                    hi = (frame * 0x4E2) / 16;
                    e4 = 0xB9;
                } else {
                    s = ((frame - 0x10) << 10) / 20;
                    lo = ex->math_sin(s) / 4 + 0x4E2;
                    hi = ((ex->math_sin(s) * 3) >> 2) + 0x4E2;
                    ext = s / 3;
                    e4 = (s * 2) / 3 + 0xB9;
                }
                s = u >> 2;
                c8 = (u >> 5) + 0xE00;
                d0 = frame * 6 + 8;
                dc = ex->math_cos(s) / 2;
                if (frame < 6) {
                    cc = (ex->math_sin((frame << 10) / 6) * 0xB86) >> 12;
                } else {
                    cc = 0xA28 - s;
                }
                cnt = 0;
                for (i = 0; i < 0x12; i++) {
                    u = (i << 12) / 18;
                    pos3.vx = work->unk8.vx;
                    pos3.vy = work->unk8.vy;
                    pos3.vz = work->unk8.vz;
                    pos3.vy -= 0x200 + ((u * cc) >> 12);
                    ang += d8;
                    r = (d4 * u) >> 12;
                    pos3.vx += (ex->math_sin(ang) * r) >> 12;
                    pos3.vz += (ex->math_cos(ang) * r) >> 12;
                    sy = u >> 2;
                    b += 0x180;
                    s = ex->btlfx_lerp(lo, hi, 0x1000 - ex->math_cos(sy)) + ((ex->math_sin(b) * e4) >> 12);
                    sc2.vy = c8;
                    sc2.vx = sc2.vz = s;
                    if (u > 0x800) {
                        s = u - 0x800;
                        s >>= 1;
                        sc2.vx += ((0x1000 - ex->math_cos(s)) * ext) >> 12;
                        sc2.vz += ((0x1000 - ex->math_cos(s)) * ext) >> 12;
                    }
                    r3.vy += d0;
                    ex->Hi_DrawEffModel(&r3, &pos3, (s32)&sc2, work->unk14[i & 3]);
                    ex->Hi_ModifyEffModelRGB(work->unk14[i & 3], ((dc * ex->math_cos(sy)) >> 17) - 0x80, ((dc * (0x1000 - u)) >> 17) - 0x80, ((dc * (0x1000 - u)) >> 17) - 0x80);
                    if (!(ex->btlfx_rand() & 3) && cnt < 3) {
                        env->unk28 = 0x100;
                        p3.vx = pos3.vx;
                        p3.vy = pos3.vy;
                        p3.vz = pos3.vz;
                        p3.vx += ex->btlfx_randCentered(0x140);
                        p3.vy += ex->btlfx_randCentered(0x100);
                        p3.vz += ex->btlfx_randCentered(0x140);
                        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &p3, NULL, 0x7000, 0, 1, dc);
                        cnt++;
                        env->unk28 = 0;
                    }
                }
            }
            }
            if (frame >= 0x2E) {
                work->unk10 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (bri != 0) {
            rot.vx = 0x400;
            rot.vz = frame << 8;
            rot.pad = 1;
            rot.vy = 0;
            func_801E7704(&work->unk8, &rot, 0x117, 0, 0x30, 2, work->unk34 & 7, 0x100, 0x10, 0x10, (scl * 2) / 3, bri, 1,
                          D_801E968C, 0);
        }
        work->unk34++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_048/nonmatchings/fx_048", fx_048_headerPtr);
