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

s32 fx_0b3_op83(s32 mode, Fx0b3Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    Fx0b3Vec3 scale;
    Fx0b3Vec3 pos;
    SVECTOR rot;
    SVECTOR sc;
    Fx0b3Vec3 pos2;
    Fx0b3Vec3 scale2;
    SVECTOR rot2;
    SVECTOR b;
    Pool6CEntry *e;
    s32 frame;
    s32 c;
    s32 w;
    s32 k;
    s32 i;
    s32 t;
    s32 c2;
    s32 x;
    s32 y;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work83);
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801ED5E0 = env;
        work->unk78[0] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8B), 1);
        work->unk78[1] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8A), 1);
        work->unk78[2] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8C), 1);
        work->unk78[3] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8D), 1);
        work->unk44.vx = 0x200;
        work->unk44.vy = 0x200;
        work->unk44.vz = 0x200;
        work->unk38.vy = 0x1C;
        work->unk38.vz = -0x37;
        work->unk50.vx = 0;
        work->unk50.vy = 0;
        work->unk50.vz = 0;
        work->unk38.vx = 0;
        work->unk70.vx = 0;
        work->unk70.vy = 0;
        work->unk70.vz = 0;
        work->unk64.vx = 0x2800;
        work->unk64.vy = 0x2800;
        work->unk64.vz = 0x2800;
        work->unk58.vx = 0;
        work->unk58.vy = 0;
        work->unk58.vz = 0;
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            ex->Hi_DrawEffModel(&work->unk70, (VECTOR *)&work->unk58, (s32)&work->unk64, D_801EE270);
            if (frame == 0) {
                ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->unk4)->unk54->unk184, ((Fx0b3Summon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->unk4)->unk60, 6);
            } else if (frame == 0x42) {
                ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->unk4)->unk54->unk188, ((Fx0b3Summon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->unk4)->unk60, 0);
            }
            func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), frame, &a);
            work->unk38.vx = a.vx;
            work->unk38.vy = a.vy;
            work->unk38.vz = a.vz;
            ex->Hi_DrawSummonModel(&work->unk50, (VECTOR *)&work->unk38, (s32)&work->unk44,
                                   ((Fx0b3Summon *)env->unk4)->unk60, 1);
            if ((u32)(frame - 0xF) < 0x10) {
                t = ex->math_cos(((frame - 0xF) << 10) >> 4);
                ex->Hi_ModifySummonModelRGB(((Fx0b3Summon *)env->unk4)->unk60, t >> 6, t >> 5, t >> 5);
            }
            if (frame < 0x28) {
                if (frame >= 0x1D) {
                    c = 0x1000 - ((frame - 0x1D) << 12) / 10;
                } else {
                    c = 0x1000;
                }
                if (frame < 0x19) {
                    k = 0;
                    w = (frame << 12) / 24;
                } else {
                    k = 1;
                    w = ((frame - 0x19) << 12) / 45;
                }
                for (i = 0; i < 2; i++) {
                    func_801E77DC((u8 *)ex->btlfx_getFileData(entry->unk8, D_801EB5CC[i]), frame, &a, &rot, &sc);
                    pos.vx = a.vx;
                    pos.vy = a.vy;
                    pos.vz = a.vz;
                    scale.vx = sc.vx;
                    scale.vy = sc.vy;
                    scale.vz = sc.vz;
                    ex->Hi_DrawMorphEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk78[k], work->unk78[k + 1],
                                             0x1000 - w);
                    if (c != 0x1000) {
                        ex->Hi_ModifyEffModelRGB(work->unk78[k], (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
                    }
                }
            }
            if (frame < 0x14) {
                if (frame >= 0xA) {
                    c2 = 0x1000 - ((frame - 0xA) << 12) / 10;
                } else {
                    c2 = 0x1000;
                }
                pos2.vx = 0;
                pos2.vy = 0;
                pos2.vz = -0x2F;
                rot2.vx = 0;
                rot2.vy = 0;
                rot2.vz = 0;
                func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 0x2D), frame, &b);
                scale2.vx = b.vx;
                scale2.vy = b.vy;
                scale2.vz = b.vz;
                ex->Hi_DrawEffModel(&rot2, (VECTOR *)&pos2, (s32)&scale2, work->unk78[3]);
                if (c2 != 0x1000) {
                    ex->Hi_ModifyEffModelRGB(work->unk78[3], (c2 >> 5) - 0x80, (c2 >> 5) - 0x80, (c2 >> 5) - 0x80);
                }
            }
            if (frame == 9) {
                work->unk18.vx = 0;
                work->unk18.vy = -0x1DB;
                work->unk18.vz = 0;
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk18);
                if (e != NULL) {
                    e->unk22 = 0x40;
                }
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk18);
                if (e != NULL) {
                    e->unk22 = 0x40;
                }
            }
            if (frame == 0x19) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk18);
                if (e != NULL) {
                    e->unk22 = 0x40;
                }
            }
            if (frame == 0x2C) {
                work->unk20.vx = 0;
                work->unk20.vy = -0x258;
                work->unk20.vz = 0;
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2F), (s32)&work->unk20);
                if (e != NULL) {
                    e->unk14 = (s32)D_801EB660;
                    e->unk12 = 0xF0;
                    e->unkC = 0x1C;
                    e->unk22 = 0x40;
                }
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x30), (s32)&work->unk18);
                if (e != NULL) {
                    e->unk22 = 0x40;
                }
            }
            ex->func_800E6624(frame * 8, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
            ex->func_800E6624(frame * 14, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x77) {
                work->unk0 = 1;
                return 1;
            }
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", func_801E8DF4);

s32 fx_0b3_op84(s32 mode, Fx0b3Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR rot;
    Fx0b3Vec3 scale;
    Fx0b3Vec3 pos;
    SVECTOR rot2;
    Fx0b3Vec3 scale2;
    Fx0b3Vec3 pos2;
    SVECTOR rot3;
    Fx0b3Vec3 scale3;
    Pool6CEntry *e;
    s32 frame;
    s32 t;
    s32 c;
    s32 i;
    s32 u;
    s32 x;
    s32 y;
    s32 z;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work84);
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->unk54.vx = 0x200;
        work->unk54.vy = 0x200;
        work->unk54.vz = 0x200;
        work->unk48.vx = 0;
        work->unk48.vy = 0;
        work->unk48.vz = 0;
        D_801ED5E0 = env;
        work->unk88 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8E), 1);
        work->unk8C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8F), 1);
        work->unk80.vx = 0;
        work->unk80.vy = 0;
        work->unk80.vz = 0;
        work->unk74.vx = 0x2800;
        work->unk74.vy = 0x2800;
        work->unk74.vz = 0x2800;
        work->unk68.vx = 0;
        work->unk68.vy = 0;
        work->unk68.vz = 0;
        ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->unk4)->unk54->unk18C, ((Fx0b3Summon *)env->unk4)->unk60);
        ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->unk4)->unk60, 0x1A);
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            ex->Hi_DrawEffModel(&work->unk80, (VECTOR *)&work->unk68, (s32)&work->unk74, D_801EE270);
            func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), frame, &a);
            work->unk48.vx = a.vx;
            work->unk48.vy = a.vy;
            work->unk48.vz = a.vz;
            work->unk30.vx = a.vx;
            work->unk30.vy = a.vy;
            work->unk30.vz = a.vz;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54,
                                   ((Fx0b3Summon *)env->unk4)->unk60, 1);
            if ((u32)(frame - 0x4B) < 5) {
                func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 0x3F), frame, &a);
                rot.vx = 0;
                rot.vy = 0;
                rot.vz = 0;
                pos.vx = 0;
                pos.vy = -0x5C3;
                pos.vz = 0x61A8;
                scale.vx = a.vx;
                scale.vz = a.vz;
                scale.vy = a.vy = a.vy / 3;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk8C);
            }
            if ((u32)(frame - 0x4F) < 0x10) {
                func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), frame, &a);
                pos2.vz = 0x61A8;
                rot2.vx = 0;
                rot2.vy = 0;
                rot2.vz = 0;
                pos2.vx = 0;
                pos2.vy = 0;
                scale2.vx = a.vx;
                scale2.vy = a.vy;
                scale2.vz = a.vz;
                if (frame < 0x5F) {
                    c = ex->math_cos((frame - 0x4F) << 6);
                } else {
                    c = 0x1000;
                }
                ex->Hi_DrawEffModel(&rot2, (VECTOR *)&pos2, (s32)&scale2, work->unk88);
                y = c >> 5;
                y -= 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk88, y, y, y);
            }
            if (frame >= 0x4E) {
                t = frame - 0x4E;
                u = (t << 12) / 26;
                a.vz = 0x6590 - (((0x1000 - ex->math_cos(u >> 2)) * 0x8278) >> 12);
                a.vy = -0x200;
                scale3.vz = 0x2000;
                scale3.vx = 0x2000;
                a.vx = 0;
                rot3.vz = 0;
                rot3.vy = 0;
                rot3.vx = 0;
                if (frame < 0x53) {
                    scale3.vy = (ex->math_sin(t * 0x5E8 / 5) >> 2) + 0x200;
                } else {
                    scale3.vy = ((ex->math_sin((((frame - 0x53) << 12) / 21) >> 2) * 3) >> 2) + 0x400;
                }
                func_801E8DF4(&a, &rot3, &scale3, 0x80, 0x17, 0x3D07, 0, 0x100, 0x400, 0x2BC, u * 3 / 2,
                              0x100, 0x400, u >> 1, 0x80);
                env->unk28 = 0x80;
                for (i = 0; i < 20; i++) {
                    BTLFX_COPY_SVEC(&a, &D_801EE2A0[i]);
                    ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->unk8, 0x26), &a, NULL, ((i & 1) << 12) + 0x5000,
                                      frame + i * 5, 1, 0x1000);
                    a.vy = -0x100;
                    ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24), &a, NULL, 0x7000, frame + i * 3, 1,
                                      0xC00);
                }
                env->unk28 = 0;
            }
            func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 0x27), frame, &work->unk28);
            work->unk28.vy = -0x100;
            if (frame == 0) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&work->unk28);
                if (e != NULL) {
                    e->unk22 = 0x100;
                }
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x31), (s32)&work->unk30);
                if (e != NULL) {
                    e->unk14 = (s32)D_801ED0A0;
                    e->unk12 = 0x30;
                    e->unkC = 0x1C;
                    e->unk22 = 0x100;
                }
            }
            ex->func_800E6624(frame * 16, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
            ex->func_800E6624(frame * 12, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x68) {
                work->unk0 = 1;
                ex->func_800E3C58();
                return 1;
            }
        }
        work->unk4++;
    }
    return 0;
}

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

s32 fx_0b3_op85(s32 mode, Fx0b3Work85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR q;
    SVECTOR a;
    SVECTOR b;
    VECTOR c;
    SVECTOR p1;
    SVECTOR p2;
    VECTOR scale;
    Pool6CEntry *p;
    SVECTOR a2;
    SVECTOR b2;
    VECTOR c2;
    s32 frame;
    s32 t;
    s32 f;
    s32 d;
    s32 m;
    s32 s;
    s32 s_6;
    s32 s_5;
    s32 u;
    s32 u_7;
    s32 k;
    s32 o;
    s32 k_10;
    s32 r;
    s32 g;
    s32 i;
    s32 j;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work85);
    } else if (mode == 1) {
        work->unk40.vy = 0x800;
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk40.vx = 0;
        work->unk40.vz = 0;
        work->unk34.vx = 0x1000;
        work->unk34.vy = 0x1000;
        work->unk34.vz = 0x1000;
        work->unk28.vx = 0;
        work->unk28.vy = 0;
        work->unk28.vz = 0;
        D_801ED5E0 = env;
        ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->unk4)->unk54->unk18C, ((Fx0b3Summon *)env->unk4)->unk60);
        ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->unk4)->unk60, 0x5A);
        return 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 81;
            if (frame == 0x12) {
                work->unk68 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x25), (s32)&work->unk10);
                if (work->unk68 != NULL) {
                    work->unk68->unk24->unk0 = -1;
                    ((EffTailSpawn *)work->unk68->unk24)->unk3C = (s32 (*)(SVECTOR *, SVECTOR *, s32, s32))func_801EA6C8;
                    if (work->unk68 != NULL) {
                        work->unk68->unk22 = 0x200;
                    }
                }
            }
            if (frame >= 9) {
                if (frame < 0x3D) {
                    f = ((frame - 9) << 12) / 52;
                } else {
                    f = 0x1000;
                }
                if (frame < 0x14) {
                    a.vx = 0;
                    m = 1;
                } else {
                    a.vx = 0x1000 - (t >> 1);
                    m = 2;
                }
                s_5 = ex->math_sin(t >> 2);
                u_7 = f >> 2;
                a.vy = (s16)(((0x1000 - s_5) >> 2) - 0x400) - ((ex->math_cos(u_7) * 0x44C) >> 12);
                a.vz = ((0x1000 - ex->math_cos(u_7)) << 2) - 0x4000;
                b.vx = 0x400;
                b.vy = 0x400;
                b.vz = 0;
                c.vy = 0x1000;
                c.vx = 0x1000;
                c.vz = ex->math_cos(u_7);
                if (frame < 0x10) {
                    d = ex->math_sin((frame << 10) >> 4) + 0x800;
                } else {
                    d = 0x1800;
                }
                k = 0xC8;
                if ((u32)(frame - 0x28) < 0x20) {
                    k = 0xC8 - (((frame - 0x28) * 0x96) >> 5);
                } else if (frame >= 0x48) {
                    k = 0x32;
                }
                if (frame >= 0x3D) {
                    r = 0x80 - (((frame - 0x3D) << 5) / 20);
                } else {
                    r = 0x80;
                }
                if (frame < 0x29) {
                    env->unk28 = 0;
                } else {
                    env->unk28 = -0x80;
                }
                func_801E9DFC(&a, &b, &c, r, k, 0x17, 0x3D07, 0, 0x80, 0x7D0, f * m, -0x140, d,
                              ex->math_cos(f >> 2) + 0x400, 0x100);
                g = (t * 3) >> 3;
                env->unk28 = 0x7C;
                for (i = 0; i < 18; i++) {
                    if (D_801EE340[i].pad != 0) {
                        r = (i & 1) << 12; /* r reused: a global pseudo, so r + 0x3800 is not tied to it */
                        ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24), &D_801EE340[i], NULL,
                                                g + (s16)(r + 0x3800), frame + i * 2, 1, 0x1000);
                    }
                }
                env->unk28 = 0x37;
                if (frame >= 0x1D) {
                    for (i = 0, j = frame; i < 7; j += 2, i++) {
                        if (D_801EE340[i].pad != 0) {
                            ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24), &D_801EE340[i], NULL,
                                                    0x2400, j + 4, 1, 0x1000);
                        }
                    }
                }
                env->unk28 = 0;
            }
            if (frame < 0x10) {
                func_801E7704((Fx0b3Track *)ex->btlfx_getFileData(entry->unk8, 0x16), frame, &q);
                work->unk28.vx = q.vx;
                work->unk28.vy = q.vy;
                work->unk28.vz = q.vz;
                ex->Hi_DrawSummonModel(&work->unk40, (VECTOR *)&work->unk28, (s32)&work->unk34,
                                       ((Fx0b3Summon *)env->unk4)->unk60, 1);
            }
            if (frame == 0x32) {
                p1.vx = env->unk2C->unk0[0].vx;
                p1.vy = env->unk2C->unk0[0].vy;
                p1.vz = env->unk2C->unk0[0].vz;
                p2.vx = env->unk2C->unk0[1].vx;
                p2.vy = env->unk2C->unk0[1].vy;
                p2.vz = env->unk2C->unk0[1].vz;
                work->unk10.vx = (p1.vx + p2.vx) / 2;
                work->unk10.vy = (p1.vy + p2.vy) / 2;
                work->unk10.vz = (p1.vz + p2.vz) / 2;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame >= 0x2B) {
                s_6 = ((frame - 0x2B) << 10) / 38;
                ex->math_cos(s_6);
                ex->math_cos(s_6);
                ex->func_800D0B18((s32)(env->unk20 + 2), -0x20, 0x10, 3, 0x50, 1, &D_801EB5D8, 0x18, frame << 8, 0x186,
                                  0x100);
                if (frame == 0x2B) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 0, 0x20, 0xAA, 0xFF);
                }
                if (frame == 0x2D || frame == 0x32) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 0, 0, 0x64, 0xFF);
                }
            }
            if (frame >= 0x2B) {
                k_10 = frame - 0x2B;
                p2.vx = 0;
                p2.vy = 0;
                p2.vz = 0;
                scale.vz = 0x1000;
                ((CVECTOR *)&p1)->r = ((CVECTOR *)&p1)->g = ((CVECTOR *)&p1)->b = -0x10 - (k_10 * 0x8D) / 38;
                scale.vy = scale.vx = 0x1400 - (k_10 << 10) / 38;
                ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &p2, &scale, (CVECTOR *)&p1, 0, NULL);
            }
            func_801E78BC(frame * 3, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x51) {
                work->unk0 = 1;
                work->unk18.vx = 0;
                work->unk18.vy = -0x64;
                work->unk18.vz = 0;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
                ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&a2);
                if (a2.vy < -0x800) {
                    work->unk18.vz = 0x300;
                    work->unk18.vy -= 0x1F4;
                }
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 24;
            u = t >> 2;
            work->unk18.vy -= (ex->math_cos(u) * 0x18) >> 12;
            env->unk2C->unk0[3].vx = work->unk18.vx;
            env->unk2C->unk0[3].vy = work->unk18.vy;
            env->unk2C->unk0[3].vz = work->unk18.vz;
            a2.vx = 0;
            a2.vy = -0x200 - (ex->math_cos(u) >> 3);
            b2.vx = 0x400;
            b2.vy = 0x400;
            a2.vz = 0;
            b2.vz = 0;
            c2.vy = 0x1000;
            c2.vx = 0x1000;
            c2.vz = 0;
            s = ex->math_cos(u) >> 5;
            func_801E9DFC(&a2, &b2, &c2, s, s, 0x17, 0x3D07, 0, 0x80, (ex->math_cos(u) >> 3) + 0x300, u, 0x100, 0, 0,
                          0);
            func_801E78BC(frame * 3, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x18) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

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
