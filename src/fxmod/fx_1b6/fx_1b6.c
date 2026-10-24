#include "fxmod/fx_1b6.h"

void func_801E7704(Fx1b6Split *s, SVECTOR (*faces)[3]) {
    Fx1b6Piece *p;
    s32 i;

    D_801E9744 = s;
    D_801E9748 = s->unk44;
    p = s->unk44;
    s->unk0 = 0;
    s->unk2 = 0;
    s->unk24 = faces;
    s->unk20 = faces;
    for (i = 0; i < 0x708; i++) {
        p->unk18 = -1;
        p++;
    }
}

s32 func_801E774C(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E85B4);
    Fx1b6Split *s = D_801E9744;
    SVECTOR (*f)[3] = s->unk24;
    s32 n;
    s32 i;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)f);
    k = s->unk0;
    s->unk28[k] = f;
    s->unk4[k] = n;
    s->unk24 = f + n;
    s->unk2 += n;
    s->unk0++;
    for (i = 0; i < n; i++) {
        (*f)[0].pad = 0;
        f++;
    }
    return s->unk2;
}

Fx1b6Piece *func_801E7830(void) {
    Fx1b6Piece *p = D_801E9748;
    s32 i;

    for (i = 0; i < 0x708; i++, p++) {
        if (p->unk18 < 0) {
            return p;
        }
    }
    return NULL;
}

void func_801E7868(void) {
    Fx1b6Split *s = D_801E9744;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E85B4);
    s32 t = D_801E974C;
    SVECTOR (*f)[3] = s->unk20;
    SVECTOR d;
    Fx1b6Piece *p;
    s32 j;
    s32 i;

    j = 0;
    i = 0;
    for (; i < s->unk2; i++, f++, j++) {
        if (t < (*f)[1].vz && (*f)[0].pad == 0) {
            p = func_801E7830();
            if (p != NULL) {
                (*f)[0].pad = 1;
                p->unk0 = (((*f)[0].vx + (*f)[1].vx + (*f)[2].vx) / 3) << 12;
                p->unk4 = (((*f)[0].vy + (*f)[1].vy + (*f)[2].vy) / 3) << 12;
                p->unk8 = (((*f)[0].vz + (*f)[1].vz + (*f)[2].vz) / 3) << 12;
                p->unk18 = 0;
                p->unk1A = j;
                p->unkC.pad = 0x1000;
                p->unk14 = ex->btlfx_randCentered(0x100);
                p->unk16 = ex->btlfx_randCentered(0x200);
                d.vx = p->unk0 >> 12;
                d.vy = p->unk4 >> 12;
                d.vz = p->unk8 >> 12;
                p->unkC.vx = (d.vx >> 2) + ex->btlfx_randCentered(0x10);
                p->unkC.vy = (d.vy >> 5) + ex->btlfx_randCentered(0x10);
                p->unkC.vz = ex->btlfx_randCentered(0x10) + 0x80;
            }
        }
    }
}

void func_801E7AAC(void) {
    Fx1b6Split *s;
    Fx1b6Piece *p;
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

    s = D_801E9744;
    p = D_801E9748;
    zero[0] = 0;
    zero[1] = 0;
    ex = BTLFX_EXPORTS(D_801E85B4);
    scale.vx = scale.vy = scale.vz = 0xE80;
    rot.vz = 0;
    for (i = 0; i < 0x708; i++, p++) {
        if (p->unk18 >= 0) {
            f = &s->unk20[p->unk1A];
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
            ex->btlfx_stepModuleParticle((Pool6CEntry *)p, 0xB, (SVECTOR *)zero, &out);
            *(s32 *)&rot.vx = *(s32 *)&p->unk14;
            ex->func_800316D8(&rot, &m);
            ex->ScaleMatrix(&m, &scale);
            m.t[0] = p->unk0 >> 12;
            m.t[1] = p->unk4 >> 12;
            m.t[2] = p->unk8 >> 12;
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
            if (p->unk18 >= 0x3C) {
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
}

s32 fx_1b6_op80(s32 mode, Fx1b6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 sp;
    Pool6CEntry *p;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 c;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x64;
    } else if (mode == 1) {
        D_801E85B4 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&pos);
        pos.vy -= 0x110;
        work->unk8[0] = pos.vx;
        work->unk8[1] = pos.vy;
        work->unk8[2] = pos.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
        ex->btlfx_getUnitScale(0x10, work->unk1C);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk44);
        work->unk44.vy -= 0x397;
        BTLFX_COPY_SVEC(&work->unk4C, &work->unk44);
        work->unk4C.vz += 0x12C;
        work->unk0 = 0;
        work->unk4 = 0;
        BTLFX_COPY_SVEC(&work->unk54, &work->unk4C);
        work->unk14.vy += 0x800;
        D_801E85B8 = entry;
        work->unk5C = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk44);
        if (p != NULL) {
            p->unk22 = 0x140;
        }
        work->unk60 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk44);
        if (p != NULL) {
            p->unk22 = 0x140;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk44);
        if (p != NULL) {
            p->unk22 = 0x140;
        }
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 24;
            if (work->unk5C != NULL) {
                if (work->unk5C->unk30 == -1) {
                    work->unk5C = NULL;
                }
                if (work->unk5C != NULL) {
                    work->unk5C->unk20 = 0x1000 - t;
                }
            }
            if (work->unk60 != NULL) {
                if (work->unk60->unk30 == -1) {
                    work->unk60 = NULL;
                }
                if (work->unk60 != NULL) {
                    work->unk60->unk20 = t;
                }
            }
            if (frame == 1 || frame == 0xF) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x40, 0x40, 0x40);
            }
            if (frame == 0x17) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0xFF, 0xFF, 0xFF);
            }
            if (frame >= 0x18) {
                ex->btlfx_initParticleSystem();
                ex->btlfx_fadeUnitById(0x10, 0);
                ex->func_800E87C4(0x64);
                work->unk0 = 1;
                entry->frame = -1;
                ex->btlfx_initEffModels(D_801E9754, 7);
                for (i = 0; i < 7; i++) {
                    work->unk28[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, D_801E855C[i]), 0);
                }
                buf = ((Fx1b6Summon *)env->loader)->unk54->unk50 + 0x19000;
                func_801E7704((Fx1b6Split *)buf, (SVECTOR (*)[3])(buf + 0xC5A8));
                for (i = 0; i < 7; i++) {
                    func_801E774C(work->unk28[i]);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk54);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk44);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk4C);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E85C4;
                    p->unk12 = 0x20;
                    p->unkC = 0x8C;
                    p->unk22 = 0x140;
                }
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x80, 0x80, 0x80);
            }
            break;
        case 1:
            t = (frame << 12) / 60;
            if (frame < 8) {
                c = (ex->math_cos((frame << 10) >> 3) << 7) >> 12;
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, c, c, c);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x20, 0x40, 0xFF);
            }
            if (frame > 0) {
                for (i = 0; i < 7; i++) {
                    ex->Hi_DrawEffModel(&work->unk14, (VECTOR *)work->unk8, (s32)work->unk1C, work->unk28[i]);
                }
                if (frame < 0x31) {
                    D_801E974C = 0x514 - ((t * 0x960) >> 12);
                } else {
                    D_801E974C = -0x1000;
                }
                FX1B6_SPAD_STACK(&sp);
                func_801E7868();
                func_801E7AAC();
                FX1B6_RESET_STACK(&sp);
            }
            if (frame >= 0x3C) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_1b6_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E85B4 = env;
        ex->btlfx_clearUnitStatus(0x10);
    } else {
        return 1;
    }
    return 0;
}

s32 fx_1b6_op87(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E85B4 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&pos);
        pos.vy += 0x180;
        env->camera->points[3].vx = pos.vx;
        env->camera->points[3].vy = pos.vy;
        env->camera->points[3].vz = pos.vz;
        return 1;
    } else {
        return 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b6/nonmatchings/fx_1b6", fx_1b6_headerPtr);
