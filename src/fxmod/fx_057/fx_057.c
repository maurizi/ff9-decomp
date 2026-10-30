#include "fxmod/fx_057.h"

void func_801E7704(Pool30Entry *pt, Pool6CEntry *e, SVECTOR *pos, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 t,
                   s32 n) {
    Unk800F78A4 *env = D_801E8EA4;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Unk80100C78Entry *entry = D_801EA97C;
    u8 *buf = (u8 *)ex->func_800E0134(pt, e);
    s32 s;
    s32 d;
    SVECTOR v;

    if (t == 0) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), (s32)pos, NULL, -1, 0, 0, 0, buf, 0);
    }
    if (t < n - 4) {
        s = 0x1000;
    } else {
        s = 0x1000 - (((t - (n - 4)) << 12) >> 2);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), (s32)pos, (s32 *)e->origin, t,
                      0x1C00 - (t << 12) / n, s, 1, buf, 1);
    if (pt->unk1F == 1) {
        if (D_801EA704 != NULL) {
            ex->btlfx_moveEmitterTo((s32)D_801EA704, (s32)pos);
        }
        BTLFX_COPY_SVEC(&D_801EA6FC, pos);
        if (t < 0x1D) {
            env->camera->points[3].vx = pos->vx;
            env->camera->points[3].vy = pos->vy;
            env->camera->points[3].vz = pos->vz;
        } else {
            d = t - 0x1D;
            if (d < 0x20) {
                ex->btlfx_lerpVec((s16 *)pos, (s16 *)&D_801EA6F4, 0x1000 - ex->math_cos(d << 5), (s16 *)&v);
            } else {
                BTLFX_COPY_SVEC(&v, &D_801EA6F4);
            }
            env->camera->points[3].vx = v.vx;
            env->camera->points[3].vy = v.vy;
            env->camera->points[3].vz = v.vz;
        }
    }
}

s32 fx_057_op80(s32 mode, Fx057Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR ang;
    VECTOR sv;
    SVECTOR unused; 
    Pool6CEntry *p;
    s32 frame;
    s32 alpha;
    s32 scale;
    s32 t;
    s32 c;
    s32 k;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        D_801E8EA4 = env;
        D_801EA708 = 0;
        D_801EA97C = entry;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk0 = 0;
        work->unk4 = 0;
        BTLFX_COPY_SVEC(&D_801EA6F4, &work->unk10);
        work->unk18.vx = work->unk10.vx;
        work->unk18.vy = work->unk10.vy;
        work->unk18.vz = work->unk10.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801EA714);
        work->unk2C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk10);
        if (work->unk2C != NULL) {
            work->unk2C->unk22 = 0x18;
        }
        if (work->unk2C != NULL) {
            work->unk2C->unk14 = (s32)D_801E8EB4;
            work->unk2C->unk12 = 0x10;
            work->unk2C->unkC = 0x184;
        }
        if (work->unk2C != NULL) {
            work->unk2C->elems->unk24 = (void (*)())func_801E7704;
        }
        D_801EA704 = work->unk30 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&work->unk24);
        if (work->unk30 != NULL) {
            work->unk30->unk22 = 0x18;
        }
        work->unk34 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk24);
        if (work->unk34 != NULL) {
            work->unk34->unk22 = 0x10;
        }
        ex->btlfx_initEffModels(D_801EA724, 3);
        work->unk8 =
            ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x19), 0x137, 0xFFFF, 0, 0x80);
        work->unkC = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x28));
    } else {
        alpha = 0;
        frame = entry->frame;
        scale = 0;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 22;
            if (work->unk2C != NULL && work->unk2C->state == -1) {
                work->unk2C = NULL;
            }
            if (work->unk30 != NULL && work->unk30->state == -1) {
                work->unk30 = NULL;
            }
            if (work->unk34 != NULL && work->unk34->state == -1) {
                work->unk34 = NULL;
            }
            alpha = 0x1000;
            if (work->unk2C != NULL) {
                work->unk2C->unk20 = D_801EA708;
            }
            if (work->unk30 != NULL) {
                work->unk30->unk20 = alpha;
            }
            if (work->unk34 != NULL) {
                work->unk34->unk20 = alpha;
            }
            scale = (((0x1000 - t) * 3) >> 1) + 0x1000;
            if (frame < 4) {
                c = 0xFF - frame * 0xFF / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 1, c, c);
            }
            if (frame >= 22) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 20;
            alpha = 0x1000 - t;
            scale = 0x1000;
            D_801EA708 = t;
            if (work->unk2C != NULL && work->unk2C->state == -1) {
                work->unk2C = NULL;
            }
            if (work->unk30 != NULL && work->unk30->state == -1) {
                work->unk30 = NULL;
            }
            if (work->unk34 != NULL && work->unk34->state == -1) {
                work->unk34 = NULL;
            }
            if (work->unk2C != NULL) {
                work->unk2C->unk20 = D_801EA708;
            }
            if (work->unk30 != NULL) {
                work->unk30->unk20 = alpha;
            }
            if (work->unk34 != NULL) {
                work->unk34->unk20 = alpha;
            }
            if (frame >= 20) {
                work->unk0 = 2;
                entry->frame = -1;
                ex->btlfx_stopEmitter(work->unk30);
                ex->btlfx_stopEmitter(work->unk34);
                D_801EA704 = NULL;
            }
            break;
        case 2:
            D_801EA708 = 0x1000;
            if (work->unk2C != NULL && work->unk2C->state == -1) {
                work->unk2C = NULL;
            }
            if (work->unk2C != NULL) {
                work->unk2C->unk20 = D_801EA708;
            }
            if (frame == 19) {
                D_801EA70C = p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk10);
                if (D_801EA70C != NULL) {
                    D_801EA70C->unk22 = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x80, 0xFF);
            }
            if (frame >= 22) {
                ex->btlfx_stopEmitter(work->unk2C);
                return 1;
            }
            break;
        }
        if (alpha != 0) {
            rot.pad = 0;
            rot.vy = 0;
            rot.vx = 0;
            rot.vz = -work->unk4 * 20;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xF), &D_801EA6FC, &rot, scale, 0, 1, alpha);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x11), &D_801EA6FC, &rot, 0xE00, 0, 1, alpha);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x22), &D_801EA6FC, &rot, 0x1000, 0, 1, alpha);
        }
        if (D_801EA708 != 0) {
            sv.vx = 0x1000;
            sv.vy = 0x1000;
            sv.vz = 0x1000;
            ang.vx = 0;
            ang.vy = -0x400;
            ang.vz = 0;
            ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->unk18, (s32)&sv, work->unk8);
            ex->Hi_ModifyEffModelAbr(work->unk8, 0xFF);
            ex->Hi_ModifyEffModelRGB(work->unk8, (D_801EA708 >> 5) - 0x80, (D_801EA708 >> 5) - 0x80,
                                     (D_801EA708 >> 5) - 0x80);
            ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->unk18, (s32)&sv, work->unkC);
            ex->Hi_ModifyEffModelAbr(work->unkC, 0xFF);
            ex->Hi_ModifyEffModelRGB(work->unkC, (D_801EA708 >> 5) - 0x80, (D_801EA708 >> 5) - 0x80,
                                     (D_801EA708 >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x18), work->unk4);
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->unk10, NULL, 0x1600, 0, 1, D_801EA708);
            env->otZ = 0;
            ang.pad = 1;
            ang.vx = -0x200;
            ang.vy = work->unk4 * 0x18 + 0x580;
            ang.vz = 0;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xF), &work->unk10, &ang, 0x16000, 0, 1,
                              D_801EA708);
            k = ex->math_cos(((work->unk4 - 22) << 10) / 42);
            ang.vx = -0x180;
            ang.vy = work->unk4 * 32 + 0x180;
            ang.vz = work->unk4 * 0x18;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->unk10, &ang, k * 4, 0, 1,
                              D_801EA708);
            ang.vx = 0x200;
            ang.vy = 0x200 - work->unk4 * 32;
            ang.vz = -work->unk4 * 0x10;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->unk10, &ang, (k * 8 + k * 4) >> 1, 0, 1,
                              D_801EA708);
            ang.vx = 0x100;
            ang.vy = -0x180 - work->unk4 * 0x18;
            ang.vz = -work->unk4 * 0x10;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->unk10, &ang, (k * 16 + k * 4) >> 1, 0, 1,
                              D_801EA708);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x1F), work->unk4);
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 3), work->unk4);
        work->unk4++;
    }
    return 0;
}

s32 fx_057_op81(s32 mode, Fx057Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR d;
    SVECTOR ang;
    SVECTOR out;
    SVECTOR v;
    SVECTOR out2;
    Pool6CEntry *p;
    s32 t4;
    s32 frame;
    s32 t;
    s32 a;
    s32 s;
    s32 i;
    s32 j;
    s32 r;
    s32 step;
    s32 cs;
    s32 q;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x2C;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E8EA4 = env;
        work->unk8 = ex->func_800E7CC4(0, 0);
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->unkC);
        if (p != NULL) {
            p->unk22 = work->unk8 + 0x40;
        }
    } else {
    frame = entry->frame;
    switch (work->unk0) {
    case 0:
        d.vx = 0;
        d.vy = 0;
        d.vz = 0x400;
        d.pad = 0;
        t = (frame << 12) / 7;
        s = ex->math_sin(t >> 2);
        env->otZ = work->unk8 + 0x40;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x23), &work->unkC, NULL, 0x13000, frame, 1, s);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x22), &work->unkC, NULL, (t >> 1) + 0x3000, 0, 1,
                          s);
        env->otZ = 0;
        ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 0x17), &work->unkC, work->unk4, ex->math_sin(t >> 2));
        if (frame >= 7) {
            work->unk0 = 1;
            entry->frame = -1;
            work->unk28 =
                ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x27), 0xB9, 0x3D80, 0x80, 0);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0xFF);
        }
        break;
    case 1:
        d.vx = 0;
        d.vy = 0;
        d.vz = 0x400;
        d.pad = 0;
        env->otZ = work->unk8 + 0x40;
        t = (frame << 12) / 7;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x10), &work->unkC, &d, 0x3000, frame, 1, 0x1000);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x23), &work->unkC, NULL, t + 0x13000, frame, 1,
                          0x1000);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x22), &work->unkC, NULL, (t >> 1) + 0x3800, 0, 1,
                          0x1000);
        env->otZ = 0;
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x80);
        ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 0x17), &work->unkC, work->unk4, 0x1000);
        if (frame >= 7) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
            work->unk0 = 2;
            entry->frame = -1;
            BTLFX_COPY_SVEC(&work->unkC, &D_801EA714);
            work->unk1C.vx = work->unkC.vx;
            work->unk1C.vy = work->unkC.vy;
            work->unk1C.vz = work->unkC.vz;
            BTLFX_COPY_SVEC(&work->unk14, &work->unkC);
            work->unk14.vy -= 0xC00;
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = work->unk8 + 0x40;
            }
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = work->unk8 + 0x40;
            }
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->unkC);
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x26), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = work->unk8 + 0x80;
            }
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2A), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = work->unk8 + 0x46;
            }
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x29), (s32)&work->unk14);
            if (p != NULL) {
                p->unk22 = 0x46;
            }
        }
        break;
    case 2:
        t = frame << 7;
        env->otZ = 0;
        if (frame < 6) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, (0xFF - frame * 0xFF / 6) >> 1,
                                     0xFF - frame * 0xFF / 6, 0xFF - frame * 0xFF / 6);
        }
        if (frame == 7) {
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2B), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = 0x46;
            }
        }
        cs = ex->math_cos(t >> 2);
        r = 0x80;
        i = 0;
        step = ((t >> 7) + 4) * 3;
        ang.vx = work->unk4 * 128;
        ang.vy = work->unk4 * 32;
        q = (t / 40 + 10) * 2;
        do {
            ex->btlfx_pointAhead((s16 *)&work->unkC, (s16 *)&ang, r, (s16 *)&out);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0xB), &out, NULL, 0x1000, (frame + 7) & 0xF, 1,
                              (cs * ex->math_sin((i << 11) >> 7)) >> 12, 0, 0);
            i++;
            r += step;
            ang.vx += q * 2;
            ang.vy += q;
        } while (i < 0x2E);
        c = ex->math_cos(t >> 2);
        t4 = ex->math_sin(t >> 2) * 0x92E / 4096 + 0x200;
        v.vx = 0;
        v.vy = 0;
        v.vz = 0;
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                v.vx = work->unk4 * 0x18 + 0x200;
            } else {
                v.vx = -0x200 - work->unk4 * 0x18;
            }
            a = work->unk4 * 0x29 + (j << 7);
            for (i = 0; i < 0x10; i++, a += 0x100) {
                ex->func_800D044C(&work->unkC, (s32)&out2, 0, (XY32 *)&v, t4, a);
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x1D), &out2, NULL, 0x1800, frame & 0xF, 1,
                                  c, 0, 0);
            }
        }
        if (frame >= 0x20) {
            work->unk0 = 3;
            entry->frame = -1;
        }
        break;
    case 3:
        if (frame >= 0xF) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_057/nonmatchings/fx_057", fx_057_headerPtr);
