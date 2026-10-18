#include "fxmod/fx_0b3.h"

void func_801E7704(Fx0b3Track *track, s32 t, SVECTOR *out) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    track = (Fx0b3Track *)&track->unk4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

void func_801E7754(Fx0b3Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 6];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
}

void func_801E77DC(Fx0b3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 9];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
    if (c != NULL) {
        c->vx = k[6];
        c->vy = k[7];
        c->vz = k[8];
    }
}

s32 func_801E7890(Fx0b3Track *track, s32 t) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    return (track->unk4 + t)[0];
}

void func_801E78BC(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801ED5E0;
    u32 *p = (u32 *)env->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->unk24 = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
}

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_op80);

s32 fx_0b3_op81(s32 mode, Fx0b3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    SVECTOR d;
    SVECTOR c;
    SVECTOR rot;
    Fx0b3Vec3 scale;
    Fx0b3Vec3 pos;
    SVECTOR sc;
    Pool6CEntry *e;
    s32 frame;
    s32 i;
    s32 n;
    s32 t;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work81);
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801ED5E0 = env;
        D_801EE270 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x89), 1);
        work->unk88[0] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x86), 1);
        work->unk88[1] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x87), 0);
        work->unk88[2] = work->unk88[3] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x88), 1);
        work->unk58.vy = 0x800;
        work->unk58.vx = 0;
        work->unk58.vz = 0;
        work->unk4C.vx = 0x200;
        work->unk4C.vy = 0x200;
        work->unk4C.vz = 0x200;
        work->unk40.vx = 0;
        work->unk40.vy = 0;
        work->unk40.vz = 0;
        ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->unk4)->unk54->unk190, ((Fx0b3Summon *)env->unk4)->unk60);
        ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->unk4)->unk60, 0x28);
        work->unk78.vx = 0;
        work->unk78.vy = 0;
        work->unk78.vz = 0;
        work->unk6C.vx = 0x2800;
        work->unk6C.vy = 0x2800;
        work->unk6C.vz = 0x2800;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->unk80 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk10);
        if (work->unk80 != NULL) {
            work->unk80->unk22 = 0x80;
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            ex->Hi_DrawEffModel(&work->unk78, (VECTOR *)&work->unk60, (s32)&work->unk6C, D_801EE270);
            if (frame >= 0x2B) {
                func_801E77DC((u8 *)ex->btlfx_getFileData(entry->unk8, 3), frame, &c, &work->unk58, &d);
                work->unk40.vx = c.vx;
                work->unk40.vy = c.vy;
                work->unk40.vz = c.vz;
                work->unk4C.vx = d.vx;
                work->unk4C.vy = d.vy;
                work->unk4C.vz = d.vz;
                ex->Hi_DrawSummonModel(&work->unk58, (VECTOR *)&work->unk40, (s32)&work->unk4C,
                                       ((Fx0b3Summon *)env->unk4)->unk60, 1);
            }
            if (frame < 8) {
                t = 0xFF - frame * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, t, t, t);
            }
            if (frame == 0) {
                work->unk9C = 0x1000;
                work->unk98 = 0;
            }
            func_801E7754((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), frame, &a, &b);
            work->unk40.vx = a.vx;
            work->unk40.vy = a.vy;
            work->unk40.vz = a.vz;
            BTLFX_COPY_SVEC(&work->unk10, &a);
            work->unk4C.vx = b.vx;
            work->unk4C.vy = b.vy;
            work->unk4C.vz = b.vz;
            work->unk9C -= 0x800;
            if (work->unk9C <= 0) {
                work->unk9C = 0x1000;
                if (++work->unk98 >= 6) {
                    work->unk98 = 0;
                }
            }
            n = work->unk98 + 1;
            if (n >= 6) {
                n = 0;
            }
            ex->Hi_DrawMorphEffModel(&work->unk58, (VECTOR *)&work->unk40, (s32)&work->unk4C, D_801EE280[work->unk98],
                                     D_801EE280[n], work->unk9C);
            rot.vx = 0;
            rot.vy = 0x800;
            rot.vz = 0;
            if (work->unk80 != NULL) {
                if (work->unk80->unk30 == -1) {
                    work->unk80 = NULL;
                }
                if (work->unk80 != NULL) {
                    work->unk80->unk5C.vx = rot.vx;
                    work->unk80->unk5C.vy = rot.vy;
                    work->unk80->unk5C.vz = rot.vz;
                }
            }
            if (frame >= 0x25) {
                for (i = 0; i < 4; i++) {
                    if (frame >= D_801EB5AC[i]) {
                        rot.vz = 0;
                        rot.vy = 0;
                        rot.vx = 0;
                        func_801E7754((u8 *)ex->btlfx_getFileData(entry->unk8, D_801EB5BC[i]), frame, &a, &sc);
                        pos.vx = a.vx;
                        pos.vy = a.vy;
                        pos.vz = a.vz;
                        scale.vx = sc.vx;
                        scale.vy = sc.vy;
                        scale.vz = sc.vz;
                        ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk88[i]);
                    }
                }
                ex->func_800E6624(frame * 14, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            }
            if (frame == 0x21) {
                work->unk28.vx = 0xD5C;
                work->unk28.vy = 0;
                work->unk28.vz = 0x3797;
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk28);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            if (frame == 0x26) {
                work->unk30.vy = -0x3000;
                work->unk30.vx = 0;
                work->unk30.vz = 0x483;
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk30);
                if (e != NULL) {
                    e->unk22 = 0x200;
                }
            }
            func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 0x2E), frame, &work->unk38);
            work->unk38.vy = -0x100;
            if (frame == 0) {
                work->unk84 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&work->unk38);
                if (work->unk84 != NULL) {
                    work->unk84->unk22 = 0x100;
                }
            }
            ex->func_800E6624(frame * 16, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
            if (frame >= 0x36) {
                ex->func_800E3D54(work->unk80);
                ex->func_800E3D54(work->unk84);
                return 1;
            }
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_op83);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", func_801E8DF4);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_op84);

s32 func_801E9DB8(s32 a) {
    s32 v = BTLFX_EXPORTS(D_801ED5E0)->math_sin(a);

    if (v < 0) {
        v >>= 1;
    }
    return v;
}

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", func_801E9DFC);

s32 func_801EA6C8(SVECTOR *v) {
    s32 i = D_801EB5D4;

    v->pad = 1;
    v->vx = D_801EE340[i].vx;
    v->vy = D_801EE340[i].vy;
    v->vz = D_801EE340[i].vz;
    i++;
    if (i >= 16) {
        i = 0;
    }
    D_801EB5D4 = i;
    return 0;
}

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_op85);

s32 fx_0b3_op86(s32 mode, Fx0b3Work86 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *e;
    s32 i;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work86);
    } else if (mode == 1) {
        D_801ED5E0 = env;
        work->unk40 = env->unkC->unk24;
        for (i = 0; i < work->unk40; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk0[i]);
                work->unk0[i].vz -= ex->btlfx_getUnitRadius(i);
                work->unk0[i].vy -= 0x200;
                e = ex->func_800E3CEC((s32)ex->btlfx_getFileData(entry->unk8, 0x23), (s32)&work->unk0[i]);
                if (e != NULL) {
                    e->unk22 = ex->func_800E7CC4(i, 0x200);
                }
            }
        }
    } else if (entry->unk0 >= 0x2E) {
        return 1;
    }
    return 0;
}

s32 fx_0b3_op87(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        t = *arg4;
        D_801ED5E0 = env;
        *work = t;
        if (*work >= 10) {
            v.vx = env->unkC->unk38;
            v.vy = env->unkC->unk3A;
            v.vz = env->unkC->unk3C;
            v.vy -= 0x2BC;
            env->unk2C->unk0[2].vx = v.vx;
            env->unk2C->unk0[2].vy = v.vy;
            env->unk2C->unk0[2].vz = v.vz;
            return 1;
        }
    } else {
        frame = entry->unk0;
        func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, D_801EB5E8[*work].unk2), frame, &v);
        env->unk2C->unk0[1].vx = v.vx;
        env->unk2C->unk0[1].vy = v.vy;
        env->unk2C->unk0[1].vz = v.vz;
        func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, D_801EB5E8[*work].unk0), frame, &v);
        env->unk2C->unk0[0].vx = v.vx;
        env->unk2C->unk0[0].vy = v.vy;
        env->unk2C->unk0[0].vz = v.vz;
        if (frame >= D_801EB5E8[*work].unk4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_headerPtr);
