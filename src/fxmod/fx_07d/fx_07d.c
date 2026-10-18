#include "fxmod/fx_07d.h"

INCLUDE_ASM("asm/us/fx_07d/nonmatchings/fx_07d", func_801E7718);

void func_801E7ECC(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 a3, s32 *a4, s32 *a5, s32 a6, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9FB0);

    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(D_801EA534->unk8, 3), pos, (SVECTOR *)a3, *a4 / 2, frame, 1, *a5, 0,
                      0);
    BTLFX_COPY_SVEC(&D_801EA470[D_801EA530], pos);
    D_801EA530 = (D_801EA530 + 1) % 24;
}

void func_801E7FF4(s32 r, s32 a1, s32 a, s32 a3, s16 *out) {
    BtlfxExports *ex;
    s32 t;
    s32 x;
    s32 y;

    t = ((0x6C - r) << 12) / 108;
    t = (t * D_801EA538) >> 12;
    r = (r * 9) >> 3;
    ex = BTLFX_EXPORTS(D_801E9FB0);
    a += 0x9B;
    a += t;
    x = (ex->math_cos(a) * r) >> 12;
    y = (ex->math_sin(a) * r) >> 12;
    out[0] = x;
    out[1] = y;
}

void func_801E80C4(s16 *a, s16 *b) {
    BTLFX_EXPORTS(D_801E9FB0)->btlfx_aimAngles((s16 *)&D_801E9FB0->unk2C->unk54, a, b);
}

s32 func_801E8108(s32 mode, Fx07dOrb *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx07dWork80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 c;
    s32 s;
    s32 m;
    s32 a;
    s32 a2;

    if (mode == 1) {
        p->unk0 = 0;
        p->unk14 = ex->btlfx_rand();
        p->unk16 = (ex->btlfx_rand() & 7) + 8;
    } else {
        a2 = (frame << 12) / 12;
        p->unk14 += p->unk16;
        func_801E80C4((s16 *)&p->unk4, (s16 *)&rot);
        rot.vz = p->unk14;
        pos.vx = p->unk4.vx;
        pos.vy = p->unk4.vy;
        pos.vz = p->unk4.vz;
        m = ex->math_sin(a2 >> 2);
        m *= 3;
        s = p->unk2;
        scale.vx = scale.vy = scale.vz = ((s * m) >> 13) + (s >> 1);
        if (frame < 3) {
            c = ((frame + 1) << 12) / 3;
        } else {
            c = ex->math_cos(((frame - 3) << 10) / 9);
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
        s = c >> 5;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk28, s, s, s);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk2C);
        s = c * 2;
        s -= 0x1000;
        s >>= 5;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk2C, s, s, s);
        if (frame < 8) {
            p->unkC.vz += 0xFF;
            scale.vx = scale.vy = scale.vx / 5;
            scale.vz = scale.vx / 7;
            ex->Hi_DrawEffModel(&p->unkC, &pos, (s32)&scale, work->unk20);
            a = frame << 10;
            a >>= 3;
            ex->Hi_ModifyEffModelRGB(work->unk20, (ex->math_cos(a) >> 5) - 0x80, (ex->math_cos(a) >> 5) - 0x80,
                                     (ex->math_cos(a) >> 5) - 0x80);
        }
        env->unk28 = 0x200;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &p->unk4, NULL, (p->unk2 * 3) >> 1, 0, 1, c >> 1,
                          0, 0);
        env->unk28 = 0;
        if (frame >= 12) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_07d/nonmatchings/fx_07d", fx_07d_headerPtr);

s32 fx_07d_op80(s32 mode, Fx07dWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot0;
    SVECTOR sxy0;
    Pool6CEntry *p;
    Fx07dOrb *part;
    s32 frame;
    s32 t;
    s32 s;
    s32 c;
    s32 k;
    s32 q;
    s32 w;
    s32 a;
    s32 b;

    if (mode == 0) {
        init->unk0 = 0x40;
        init->unk8 = 0x18;
        init->unk14 = (s32)func_801E8108;
        init->unk4 = 6;
    } else if (mode == 1) {
        D_801E9FB0 = env;
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EA534 = entry;
        work->unk8 = ex->func_800E7CC4(0, 0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk14 = work->unkC.vx;
        work->unk18 = work->unkC.vy;
        work->unk1C = work->unkC.vz;
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unkC);
        if (p != NULL) {
            p->unk22 = work->unk8 + 0x46;
        }
        D_801EA530 = 0;
        work->unk34 = 0x3E8;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            D_801EA530 = 0;
            rot0.vy = 0;
            rot0.vx = 0;
            rot0.vz = 0;
            rot0.pad = 0;
            t = (frame << 12) / 50;
            s = ex->math_sin(t >> 2);
            env->unk28 = work->unk8 + 0x46;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &work->unkC, &rot0, t / 3 + 0x400, 0, 1,
                              s);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot0,
                              ex->math_cos(t >> 2) * 5 / 2 + 0x800, 0, 3, s);
            env->unk28 = 0;
            if (frame == 0x27 || frame == 0x2B) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x40, 0x40, 0x40);
            }
            D_801EA538 = t >> 1;
            q = (t * 0x64) >> 12;
            gte_SetRotMatrix(env->unk14);
            gte_SetTransMatrix(env->unk14);
            gte_ldv0(&work->unkC);
            gte_rtps();
            gte_stsxy(&sxy0);
            if (sxy0.vx < 0x78) {
                sxy0.vx = 0x78;
            }
            if (sxy0.vx >= 0xC9) {
                sxy0.vx = 0xC8;
            }
            if (sxy0.vy < 0x69) {
                sxy0.vy = 0x69;
            }
            if (sxy0.vy >= 0x74) {
                sxy0.vy = 0x73;
            }
            func_801E7718(1, sxy0.vx, sxy0.vy, 0x10, 7, 0x11, q, 0, NULL,
                          (void (*)(s32, s32, s32, s32, DVECTOR *))func_801E7FF4);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x15), work->unk4);
            if (frame == 14) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = work->unk8 + 0x46;
                    p->unk18->unk24 = (s32)func_801E7ECC;
                }
            }
            if (frame == 0x31) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0, 0, 0x80);
            }
            if (frame >= 0x32) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0xFF, 0xFF);
                ex->btlfx_initEffModels(D_801E9FC0, 6);
                work->unk20 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0x139,
                                                         0xFFFF, 0, 0x80);
                work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x16), 0x38,
                                                         0x3E83, 0, 0);
                work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), 0x13C,
                                                         0xFFFF, 0x10, 0x80);
                work->unk2C = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF));
            }
                break;
        case 1:
            switch (frame) {
            default: {
                SVECTOR sxy;
                SVECTOR rot;

                t = (frame << 12) / 10;
                if (frame < 6) {
                    s32 c = 0xFF - frame * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c, c >> 2);
                }
                D_801EA538 = 0x800;
                w = (t * 0x58) >> 12;
                gte_SetRotMatrix(env->unk14);
                gte_SetTransMatrix(env->unk14);
                gte_ldv0(&work->unkC);
                gte_rtps();
                gte_stsxy(&sxy);
                if (sxy.vx < 0x78) {
                    sxy.vx = 0x78;
                }
                if (sxy.vx >= 0xC9) {
                    sxy.vx = 0xC8;
                }
                if (sxy.vy < 0x69) {
                    sxy.vy = 0x69;
                }
                if (sxy.vy >= 0x74) {
                    sxy.vy = 0x73;
                }
                func_801E7718(1, sxy.vx, sxy.vy, 0x10, 7, 0x11, w, 0, NULL,
                              (void (*)(s32, s32, s32, s32, DVECTOR *))func_801E7FF4);
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                s = ex->math_sin(t >> 2);
                env->unk28 = work->unk8 + 0x46;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unkC, &rot, 0x1400 - t, 0, 1, s);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, (0x1000 - t) * 6, 0, 1,
                                  s >> 1);
                rot.vx = 0x200;
                rot.vy = 0x200;
                rot.vz = 0;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, t * 4, 0, 1, s);
                rot.vx = 0x200;
                rot.vy = 0x600;
                rot.vz = 0;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, t * 4, 0, 1, s);
                env->unk28 = 0;
                if (frame >= 10) {
                    work->unk0 = 2;
                    entry->unk0 = -1;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0x80, 0x80);
                    work->unk3C = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unkC);
                    if (p != NULL) {
                        p->unk22 = work->unk8 + 0x46;
                    }
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unkC);
                    if (p != NULL) {
                        p->unk22 = work->unk8 + 0x46;
                    }
                    work->unk38 = (ex->btlfx_rand() % 3 - 1) * 0x9B;
                }
                break;
            }
            }
            break;
        case 2:
            switch (frame) {
            default: {
                VECTOR scale;
                VECTOR pos;
                SVECTOR rot2;
                SVECTOR rot;
                SVECTOR rot3;

                t = (frame << 12) / 14;
                if (frame < 4) {
                    s32 c = 0xFF - frame * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c >> 2, c >> 2);
                }
                ex->func_800E6624(work->unk34, 0x200, 0x100, 0x20, 0x80, 0x220, 0x100);
                work->unk34 += 8;
                pos.vx = work->unkC.vx;
                pos.vy = work->unkC.vy;
                pos.vz = work->unkC.vz;
                pos.vx += 0x200;
                scale.vz = 0x400;
                scale.vy = 0x400;
                scale.vx = 0x400;
                func_801E80C4(&work->unkC.vx, &rot2.vx);
                rot2.vx = work->unk38 + (s16)(rot2.vx + 0x400);
                rot2.vz = 0;
                ex->Hi_DrawEffModel(&rot2, &pos, (s32)&scale, work->unk24);
                if (frame == 10) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x64, 0x40, 0);
                }
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                s = ex->math_sin(t >> 2);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x17), work->unk4);
                env->unk28 = work->unk8 + 0x46;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unkC, &rot, 0x1400 - t, 0, 1, s);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), &work->unkC, &rot, t * 2 + 0x400, 0, 1, s);
                env->unk28 = 0;
                if (frame >= 14) {
                    work->unk0 = 3;
                    entry->unk0 = -1;
                }
                break;
            }
            }
            break;
        case 3:
            switch (frame) {
            default: {
                VECTOR scale;
                VECTOR pos;
                SVECTOR rot2;
                SVECTOR rot;
                SVECTOR rot3;

                t = frame << 9;
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x80, 0x80, 0x80);
                }
                ex->func_800E6624(work->unk34, 0x200, 0x100, 0x20, 0x80, 0x220, 0x100);
                work->unk34 -= 14;
                pos.vx = work->unkC.vx;
                pos.vy = work->unkC.vy;
                pos.vz = work->unkC.vz;
                pos.vx += 0x200;
                scale.vz = 0x400;
                scale.vy = 0x400;
                scale.vx = 0x400;
                func_801E80C4(&work->unkC.vx, &rot2.vx);
                rot2.vx = work->unk38 + (s16)(rot2.vx + 0x400);
                rot2.vz = 0;
                ex->Hi_DrawEffModel(&rot2, &pos, (s32)&scale, work->unk24);
                rot3.vy = 0;
                rot3.vx = 0;
                rot3.vz = 0;
                rot3.pad = 0;
                s = ex->math_sin(t >> 2);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x17), work->unk4);
                env->unk28 = work->unk8 + 0x46;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unkC, &rot3, (frame << 10) | 0x200, 0,
                                  1, s);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), &work->unkC, &rot3, (frame << 11) + 0x2400,
                                  0, 1, (s >> 1) + 0x800);
                env->unk28 = 0;
                if (frame >= 8) {
                    work->unk0 = 4;
                    entry->unk0 = -1;
                    ex->func_800E3D54(work->unk3C);
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xAA, 0xAA, 0xAA);
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1A), (s32)&work->unkC);
                    if (p != NULL) {
                        p->unk22 = work->unk8 + 0x46;
                    }
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1B), (s32)&work->unkC);
                    if (p != NULL) {
                        p->unk22 = work->unk8 + 0x46;
                    }
                }
                break;
            }
            }
            break;
        case 4:
            switch (frame) {
            default: {
                SVECTOR rot;
                SVECTOR rot2;

                t = (frame << 12) / 50;
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0, 0xFF, 0xFF);
                } else if (frame < 7) {
                    s32 c = 0xFF - (frame - 1) * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c, c >> 1);
                }
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x11), work->unk4);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x15), work->unk4);
                if (frame < 0x20 && !(frame & 1)) {
                    b = frame << 7;
                    a = (b >> 3) + 0x200;
                    k = 0x1800 - b * 3 / 4;
                    part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (part != NULL) {
                        ex->btlfx_randRotation(&rot);
                        if (frame != 0) {
                            ex->func_800CFCA8(&rot, a, (s32)&rot2);
                        } else {
                            rot2.vz = 0;
                            rot2.vy = 0;
                            rot2.vx = 0;
                        }
                        part->unk4.vx = work->unkC.vx + rot2.vx;
                        part->unk4.vz = work->unkC.vz + rot2.vy;
                        part->unk4.vy = work->unkC.vy + rot2.vz;
                        part->unk2 = k;
                        ex->btlfx_randRotation(&part->unkC);
                    }
                }
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                s = ex->math_cos(t >> 2);
                env->unk28 = work->unk8 + 0x46;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &work->unkC, &rot,
                                  ex->math_cos(t >> 2) / 2 + 0x400, 0, 1, s >> 1);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x17), work->unk4);
                rot.vx = 0x200;
                rot.vy = 0x590;
                rot.vz = 0x80;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x19), &work->unkC, &rot, c = t * 3 + 0x800, 0, 1,
                                  s);
                rot.vx = 0x600;
                rot.vy = 0x990;
                rot.vz = 0x80;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x19), &work->unkC, &rot, c, 0, 1, s);
                env->unk28 = 0;
                if (frame >= 50) {
                    ex->btlfx_clearEffModels();
                    return 1;
                }
                break;
            }
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

void func_801E9834(Unk80100C78Entry *entry, Fx07dStrip *buf, s32 *tab, SVECTOR *from, SVECTOR *to, s32 r0, s32 r1, s32 s,
                   s32 step, s32 a9, s32 a10, s32 pad) {
    Unk800F78A4 *env = D_801E9FB0;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR ang;
    s32 i;
    s32 t;
    s32 h;
    SVECTOR *v;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    v = buf->unk4;
    buf->unk0 = buf->unk2 = 14;
    buf->unk1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
        t += *tab++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *tab++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(r0, r1, t);
        ex->func_800D044C(&pos, (s32)v, 0, (XY32 *)&ang, h, s);
        v->pad = pad;
        s += step + *tab++;
        v++;
    }
    env->unk28 = 0x200;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 8), 0, NULL, a9, 0x1000, a10 * 2 / 3, 2, (u8 *)buf, 0);
    env->unk28 = 0;
}

s32 func_801E9A40(s32 mode, Fx07dArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    if (mode != 1) {
        func_801E9834(entry, &D_801EA540[arc->unk10], arc->unk14, &arc->unk0, &arc->unk8, 0, 0x200, frame * 0x9B, 0x186,
                      2, 0x1000, ((0x1000 - (frame << 12) / 5) << 6) >> 12);
        if (frame >= 5) {
            return 1;
        }
    }
    return 0;
}

s32 fx_07d_op83(s32 mode, Fx07dWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Fx07dArc *arc;
    Fx07dArc *arc2;
    s32 frame;
    s32 i;
    s32 t;
    s32 *p;
    s32 *tab;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx07dWork83);
        ((EffInit *)arg4)->unk8 = sizeof(Fx07dArc);
        ((EffInit *)arg4)->unk14 = (s32)func_801E9A40;
        ((EffInit *)arg4)->unk4 = 7;
    } else if (mode == 1) {
        D_801E9FB0 = env;
        t = *arg4;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        work->unk8 = D_801EA8B0[0];
        p = work->unk8;
        for (i = 0; i < 64; i++) {
            *p++ = ex->btlfx_randCentered(0x100);
            *p++ = ex->btlfx_randCentered(0x180);
            *p++ = ex->btlfx_randCentered(0xC0);
        }
        work->unk10 = t;
        work->unkC = 0;
    } else {
        frame = entry->unk0;
        if (work->unk10 == 0) {
            if (frame >= 0x29) {
                return 1;
            }
            work->unkC++;
            arc = ex->btlfx_poolSpawn((s16 *)entry->unk10);
            if (arc != NULL) {
                ((s32 *)&arc->unk0)[0] = ((s32 *)&D_801EA470[ex->btlfx_rand() % 24])[0];
                ((s32 *)&arc->unk0)[1] = ((s32 *)&D_801EA470[ex->btlfx_rand() % 24])[1];
                ((s32 *)&arc->unk8)[0] = ((s32 *)&D_801EA470[ex->btlfx_rand() % 24])[0];
                ((s32 *)&arc->unk8)[1] = ((s32 *)&D_801EA470[ex->btlfx_rand() % 24])[1];
                arc->unk14 = work->unk8 + (ex->btlfx_rand() & 0x1F);
                arc->unk10 = work->unkC % 7;
            }
        } else {
            if (frame < 0x10) {
                t = ex->btlfx_randCentered(0x300);
                work->unkC++;
                t += frame * 0x334;
                arc2 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (arc2 != NULL) {
                    ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&v);
                    BTLFX_COPY_SVEC(&arc2->unk0, &v);
                    v.vz += ex->math_cos(t) / 2;
                    v.vy += ex->math_sin(t) / 2;
                    BTLFX_COPY_SVEC(&arc2->unk8, &v);
                    arc2->unk14 = work->unk8 + (ex->btlfx_rand() & 0x1F);
                    arc2->unk10 = work->unkC % 7;
                }
            }
            if (frame >= 0x10) {
                return 1;
            }
        }
    }
    return 0;
}
