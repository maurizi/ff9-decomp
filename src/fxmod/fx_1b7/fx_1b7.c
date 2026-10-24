#include "fxmod/fx_1b7.h"

void func_801E7704(Fx1b7Split *s, SVECTOR (*faces)[3]) {
    Fx1b7Piece *p;
    s32 i;

    D_801E87E8 = s;
    D_801E87EC = s->unk4C;
    p = s->unk4C;
    s->unk0 = 0;
    s->unk2 = 0;
    s->unk28 = faces;
    s->unk24 = faces;
    for (i = 0; i < 0x6A4; i++) {
        p->unk18 = -1;
        p++;
    }
}

s32 func_801E774C(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E87E0);
    Fx1b7Split *s = D_801E87E8;
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

Fx1b7Piece *func_801E7830(void) {
    Fx1b7Piece *p = D_801E87EC;
    s32 i;

    for (i = 0; i < 0x6A4; i++, p++) {
        if (p->unk18 < 0) {
            return p;
        }
    }
    return NULL;
}

s32 func_801E7868(void) {
    Fx1b7Split *s = D_801E87E8;
    SVECTOR (*f)[3] = s->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E87E0);
    s32 t = D_801E87F0;
    SVECTOR d;
    Fx1b7Piece *p;
    s32 j;
    s32 i;
    s32 n;

    j = 0;
    i = 0;
    n = 0;
    for (; i < s->unk2; i++, f++, j++) {
        if (t < (*f)[1].vy && (*f)[0].pad == 0) {
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
                p->unkC.vx = (d.vx >> 3) + ex->btlfx_randCentered(0x10);
                n++;
                p->unkC.vy = (d.vy >> 5) + (ex->btlfx_rand() & 0xF);
                p->unkC.vz = (d.vz >> 3) + ex->btlfx_randCentered(0x10);
            }
        }
    }
    return n;
}

void func_801E7ACC(void) {
    SVECTOR dir;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR ang;
    SVECTOR v[3];
    s32 zero[2];
    VECTOR o;
    VECTOR scale;
    MATRIX m;
    Fx1b7Split *s;
    Fx1b7Piece *p;
    BtlfxExports *ex;
    SVECTOR (*f)[3];
    s32 i;

    s = D_801E87E8;
    p = D_801E87EC;
    zero[0] = 0;
    zero[1] = 0;
    ex = BTLFX_EXPORTS(D_801E87E0);
    dir.vx = 0;
    dir.vy = 0;
    dir.vz = -0x2BC;
    scale.vx = scale.vy = scale.vz = 0xF80;
    rot.vz = 0;
    for (i = 0; i < 0x6A4; i++, p++) {
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
            ex->btlfx_aimAngles((s16 *)&out, (s16 *)&dir, (s16 *)&ang);
            ang.vx = 0;
            ang.vy += 0x400;
            ex->func_800CFCA8(&ang, 0xA0, (s32)&rot);
            p->unk0 += rot.vx << 12;
            p->unk4 += rot.vy << 12;
            p->unk8 += rot.vz << 12;
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
            if (p->unk18 >= 0x20) {
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

s32 fx_1b7_op80(s32 mode, Fx1b7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 sp;
    Pool6CEntry *p;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 a;
    s32 c;
    s32 r;
    s32 g;
    s32 b;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x64;
    } else if (mode == 1) {
        D_801E87E0 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&pos);
        pos.vz -= 0x258;
        work->unk8[0] = pos.vx;
        work->unk8[1] = pos.vy;
        work->unk8[2] = pos.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
        ex->btlfx_getUnitScale(0x10, work->unk1C);
        work->unk14.vy += 0x800;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk28);
        work->unk28.vy -= 0x180;
        work->unk30.vx = work->unk28.vx;
        work->unk30.vy = work->unk28.vy;
        work->unk30.vz = work->unk28.vz;
        work->unk30.vy = 0;
        work->unk38.vx = work->unk30.vx;
        work->unk38.vy = work->unk30.vy;
        work->unk38.vz = work->unk30.vz;
        work->unk38.vy = -0x280;
        pos.vy -= 0x80;
        env->camera->points[3].vx = pos.vx;
        env->camera->points[3].vy = pos.vy;
        D_801E87E4 = entry;
        env->camera->points[3].vz = pos.vz;
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk40 = NULL;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 7) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E8E40;
                    p->unk12 = 0x20;
                    p->unkC = 0xAC;
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0xB) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x1D) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x27) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x20) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x10) {
                work->unk40 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk28);
                if (work->unk40 != NULL) {
                    work->unk40->unk22 = 0x140;
                }
            }
            if (work->unk40 != NULL && work->unk40->unk30 == -1) {
                work->unk40 = NULL;
            }
            if (frame == 1 || frame == 0xF || frame == 0x14 || frame == 0x24) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0x40, 0x80, 0x80);
            }
            if (frame == 2 || frame == 0x10 || frame == 0x15 || frame == 0x25) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x64, 0x64, 0x40);
            }
            if (frame == 0x2B) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0xFF, 0xFF, 0xFF);
            }
            if (frame >= 0x2C) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unk40);
                ex->btlfx_fadeUnitById(0x10, 0);
                ex->func_800E87C4(0x6E);
                ex->btlfx_initEffModels(D_801E8800, 8);
                for (i = 0; i < 8; i++) {
                    work->unk44[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E8788[i]), 0);
                }
                buf = ((Fx1b7Summon *)env->unk4)->unk54->unk50 + 0x1B800;
                func_801E7704((Fx1b7Split *)buf, (SVECTOR (*)[3])(buf + 0xDBD8));
                for (i = 0; i < 8; i++) {
                    func_801E774C(work->unk44[i]);
                }
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0xC0, 0xAA, 0x20);
            }
            break;
        case 1:
            t = (frame << 12) / 130;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x60, 0x55, 0x10);
            }
            if ((u32)(frame - 4) < 0x18) {
                c = 0xFF - ((frame - 4) * 0xFF) / 24;
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, c, c, c >> 1);
            }
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk38);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk30);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk30);
                if (p != NULL) {
                    p->unk14 = (s32)D_801EA3C0;
                    p->unk12 = 0x1C;
                    p->unkC = 0xAC;
                    p->unk22 = 0x140;
                }
            }
            if (frame > 0) {
                a = t >> 2;
                r = (ex->math_cos(a) * 2) >> 5;
                g = ex->math_cos(a);
                b = t + 0x1000;
                for (i = 0; i < 8; i++) {
                    ex->Hi_DrawEffModel(&work->unk14, (VECTOR *)work->unk8, (s32)work->unk1C, work->unk44[i]);
                    ex->Hi_ModifyEffModelRGB(work->unk44[i], r - 0x80, ((g * 3 / 2) >> 5) - 0x80, (b >> 5) - 0x80);
                }
                D_801E87F0 = 0x898 - ((t << 12) >> 12);
                FX1B7_SPAD_STACK(&sp);
                func_801E7868();
                func_801E7ACC();
                FX1B7_RESET_STACK(&sp);
            }
            if (frame >= 0x82) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_1b7_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E87E0 = env;
        ex->btlfx_clearUnitStatus(0x10);
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b7/nonmatchings/fx_1b7", fx_1b7_headerPtr);
