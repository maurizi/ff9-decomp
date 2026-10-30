#include "fxmod/fx_1b0.h"

void func_801E7704(Fx1b0Split *s, SVECTOR (*faces)[3]) {
    Fx1b0Piece *p;
    s32 i;

    D_801EBF3C = s;
    D_801EBF40 = s->unk4C;
    p = s->unk4C;
    s->unk0 = 0;
    s->unk2 = 0;
    s->unk28 = faces;
    s->unk24 = faces;
    for (i = 0; i < 0x7BC; i++) {
        p->unk18 = -1;
        p++;
    }
}

s32 func_801E774C(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EBF34);
    Fx1b0Split *s = D_801EBF3C;
    SVECTOR (*f)[3] = s->unk28;
    s32 n;
    s32 i;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)f);
    k = s->unk0;
    s->unk2C[k] = f;
    s->unk4[k] = n;
    s->unk28 = f + n;
    s->unk2 += n;
    s->unk0++;
    for (i = 0; i < n; i++) {
        (*f)[0].pad = 0;
        f++;
    }
    return s->unk2;
}

Fx1b0Piece *func_801E7830(void) {
    Fx1b0Piece *p = D_801EBF40;
    s32 i;

    for (i = 0; i < 0x7BC; i++, p++) {
        if (p->unk18 < 0) {
            return p;
        }
    }
    return NULL;
}

s32 func_801E7868(SVECTOR *center) {
    Fx1b0Split *s = D_801EBF3C;
    SVECTOR (*f)[3] = s->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EBF34);
    SVECTOR d;
    Fx1b0Piece *p;
    s32 j;
    s32 i;
    s32 n;

    j = 0;
    i = 0;
    n = 0;
    for (; i < s->unk2; i++, f++, j++) {
        p = func_801E7830();
        if (p != NULL) {
            (*f)[0].pad = 1;
            p->unk0 = (((*f)[0].vx + (*f)[1].vx + (*f)[2].vx) / 3) << 12;
            p->unk4 = (((*f)[0].vy + (*f)[1].vy + (*f)[2].vy) / 3) << 12;
            p->unk8 = (((*f)[0].vz + (*f)[1].vz + (*f)[2].vz) / 3) << 12;
            p->unk18 = 0;
            p->unk1A = j;
            p->unkC.pad = 0x1000;
            p->unk14 = ex->btlfx_randCentered(0x40);
            p->unk16 = ex->btlfx_randCentered(0x80);
            d.vx = (p->unk0 >> 12) - center->vx;
            d.vy = (p->unk4 >> 12) - center->vy;
            d.vz = (p->unk8 >> 12) - center->vz;
            p->unkC.vx = d.vx / 10 + ex->btlfx_randCentered(0x10);
            p->unkC.vy = d.vy / 10 + ex->btlfx_randCentered(0x10);
            p->unkC.vz = d.vz / 10 + ex->btlfx_randCentered(0x10);
            n++;
        }
    }
    return n;
}

void func_801E7B04(void) {
    Fx1b0Split *s;
    Fx1b0Piece *p;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR v[3];
    s32 zero[2];
    VECTOR o;
    VECTOR scale;
    MATRIX m;
    SVECTOR (*f)[3];
    s32 i;

    s = D_801EBF3C;
    p = D_801EBF40;
    zero[0] = 0;
    zero[1] = 0;
    ex = BTLFX_EXPORTS(D_801EBF34);
    scale.vx = scale.vy = 0x1000;
    scale.vz = 0xF00;
    rot.vz = 0;
    for (i = 0; i < 0x7BC; i++, p++) {
        if (p->unk18 >= 0) {
            f = &s->unk24[p->unk1A];
            c.vx = p->unk0 >> 12;
            c.vy = p->unk4 >> 12;
            c.vz = p->unk8 >> 12;
            v[0].vx = (*f)[0].vx - c.vx;
            v[0].vy = (*f)[0].vy - c.vy;
            v[0].vz = (*f)[0].vz - c.vz;
            v[1].vx = (*f)[1].vx - c.vx;
            v[1].vy = (*f)[1].vy - c.vy;
            v[1].vz = (*f)[1].vz - c.vz;
            v[2].vx = (*f)[2].vx - c.vx;
            v[2].vy = (*f)[2].vy - c.vy;
            v[2].vz = (*f)[2].vz - c.vz;
            ex->btlfx_stepModuleParticle((Pool6CEntry *)p, 0, (SVECTOR *)zero, &out);
            if (p->unk4 >= 0) {
                *(s32 *)&rot.vx = *(s32 *)&p->unk14;
                ex->math_rotMatrixYXZ(&rot, &m);
                m.t[0] = p->unk0 >> 12;
                m.t[1] = p->unk4 >> 12;
                m.t[2] = p->unk8 >> 12;
                ex->ScaleMatrix(&m, &scale);
                ex->SetRotMatrix(&m);
                ex->SetTransMatrix(&m);
                gte_ldv0(&v[0]);
                gte_rtv0tr();
                gte_stlvnl(&o);
                gte_ldv0(&v[1]);
                gte_rtv0tr();
                (*f)[0].vx = o.vx;
                (*f)[0].vy = o.vy;
                (*f)[0].vz = o.vz;
                gte_stlvnl(&o);
                gte_ldv0(&v[2]);
                gte_rtv0tr();
                (*f)[1].vx = o.vx;
                (*f)[1].vy = o.vy;
                (*f)[1].vz = o.vz;
                gte_stlvnl(&o);
                (*f)[2].vx = o.vx;
                (*f)[2].vy = o.vy;
                (*f)[2].vz = o.vz;
                p->unk18++;
                if (p->unk18 < 0x80) {
                    continue;
                }
            }
            p->unk18 = -1;
            (*f)[2].vz = 0;
            *(s32 *)&(*f)[2].vx = 0;
            (*f)[1].vz = 0;
            *(s32 *)&(*f)[1].vx = 0;
            (*f)[0].vz = 0;
            *(s32 *)&(*f)[0].vx = 0;
        }
    }
}

s32 func_801E7E48(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    s32 k = e->unk1F - 1;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EBF34);
    Fx1b0Orbit *o = &D_801EC724[k];
    SVECTOR v;

    if (frame == 0) {
        o->unk0.vx = k << 9;
        o->unk0.vy = k * 0x140;
        o->unk0.vz = 0;
        o->unk8 = ex->btlfx_randCentered(0x20) + 0x40;
        o->unkA = ex->btlfx_randCentered(0x20) + 0x190;
    }
    ex->btlfx_pointAhead((s16 *)&D_801EC714, (s16 *)&o->unk0, (ex->math_sin(((frame << 12) / frames) >> 2) * 0xBB8) >> 12,
                      (s16 *)&v);
    o->unk0.vx += o->unk8;
    o->unk0.vy += o->unkA;
    o->unk8 = (o->unk8 * 0x11) >> 4;
    o->unkA = (o->unkA * 7) >> 3;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    e->pos.vx = v.vx << 12;
    e->pos.vy = v.vy << 12;
    e->pos.vz = v.vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

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
        env->camera->points[3].vx = v.vx;
        env->camera->points[3].vy = v.vy;
        D_801EBF38 = entry;
        env->camera->points[3].vz = v.vz;
        work->unk0 = 0;
        work->unk4 = 0;
    } else {
        a = 0;
        b = 0;
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            env->otZ = 0xC8;
            t = (frame << 12) / 90;
            if (frame < 0x3D) {
                v.vz = 0;
                v.vy = 0;
                v.vx = 0;
                v.pad = 1;
                v.vx = ex->math_cos(t >> 2);
                for (i = 0; i < 4; i++) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x11), &work->unk28, &v, 0x4000, 0, 1,
                                      ex->math_sin(t >> 1));
                    v.vx += 0x200;
                }
                v.vx = 0;
                v.vy = ex->math_cos(t >> 2);
                for (i = 0; i < 4; i++) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x11), &work->unk28, &v, 0x4000, 0, 1,
                                      ex->math_sin(t >> 1));
                    v.vy += 0x200;
                }
            }
            env->otZ = 0;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0x40, 0x40, 0x55);
            }
            if (frame == 1) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x14) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x15) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x16) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x40, 0x40, 0x55);
            }
            if ((u32)(frame - 0x20) < 8) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0xC8 - (frame - 0x20) * 0x19, 0xC8 - (frame - 0x20) * 0x19, 0xC8 - (frame - 0x20) * 0x19);
            }
            if (frame == 0x2A || frame == 0x30) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x2B || frame == 0x31) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame >= 0x4A) {
                a = ((frame - 0x4A) * 0xFF) / 16;
                b = a * 2 + a;
            }
            if (frame == 0x3C || frame == 0x46 || frame == 0x50) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0, 0x80, 0xFF);
            }
            if (frame == 0x3D || frame == 0x47 || frame == 0x51) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame >= 0x46 && (ex->btlfx_rand() & 3) == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x40, 0x40, 0x20);
            }
            if (frame >= 0x3C) {
                ex->btlfx_sampleGradient(D_801E8CAC, &work->unk38, frame - 0x3C);
            }
            if (frame == 0x3B) {
                ex->btlfx_fadeUnitById(0x10, 0);
                ex->btlfx_moveSummonToHeap(0x64);
                ex->btlfx_initEffModels(D_801EBF44, 0xA);
                for (j = 0; j < 8; j++) {
                    work->unk3C[j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, D_801E8CEC[j]), 0);
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
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x28) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x31) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk28);
                if (p != NULL) {
                    ((EffMotion *)p->file)->unk8 = func_801E7E48;
                    p->unk14 = (s32)D_801E8D44;
                    p->unk12 = 0x20;
                    p->unkC = 0xAC;
                    p->unk22 = 0x140;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x3C || frame == 0x46) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x59) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0xFF, 0xFF, 0xFF);
            }
            if (frame >= 0x5A) {
                work->unk0 = 1;
                entry->frame = -1;
                buf = ((Fx1b0Summon *)env->loader)->unk54->unk50 + 0x19000;
                func_801E7704((Fx1b0Split *)buf, (SVECTOR (*)[3])(buf + 0xDB88));
                for (i = 0; i < 8; i++) {
                    func_801E774C(work->unk3C[i]);
                }
                ex->btlfx_initParticleSystem();
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
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0, 0xFF, 0xFF);
            }
            if (frame < 8) {
                c = 0xFF - (frame * 0xFF) / 8;
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, c, c, c >> 1);
            }
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk14 = (s32)D_801EA2C4;
                    p->unk12 = 0x18;
                    p->unkC = 0x8C;
                    p->unk22 = 0x140;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 1) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk28);
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
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &w, &scale, (CVECTOR *)&v, 1, 0);
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_1b0_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EBF34 = env;
        ex->btlfx_clearUnitStatus(0x10);
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b0/nonmatchings/fx_1b0", fx_1b0_headerPtr);
