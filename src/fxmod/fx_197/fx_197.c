#include "fxmod/fx_197.h"

void func_801E7704(Fx197Track *track, s32 t, SVECTOR *out) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    track = (Fx197Track *)&track->unk4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

void func_801E7754(Fx197Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
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

void func_801E77DC(Fx197Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
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

s32 func_801E7890(Fx197Track *track, s32 t) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    return (track->unk4 + t)[0];
}

void func_801E78BC(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801E9660;
    u32 *p = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->prim = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->ot + 0xFFF, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->ot + 0xFFF, p);
    }
}

void func_801E7A5C(s32 i, s32 n, Fx197Shatter *buf, Fx197Tri *tris) {
    Fx197Shatter *h;

    D_801EAF80[i] = buf;
    D_801EAFA0[i] = (Fx197Shard *)(buf + 1);
    h = D_801EAF80[i];
    h->unk0 = n;
    h->unk2 = 0;
    h->unkC = tris;
    h->unk8 = tris;
}

s32 func_801E7AA0(s32 i, s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9660);
    Fx197Shatter *h = D_801EAF80[i];
    Fx197Tri *t = h->unkC;
    Fx197Shard *p;
    s32 n;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)t);
    h->unk10 = t;
    h->unk4 = n;
    h->unkC = t + n;
    h->unk2 += n;
    for (k = 0; k < n; k++, t++) {
        t->unk0.pad = 0;
    }
    p = D_801EAFA0[i];
    for (k = 0; k < n; k++, p++) {
        p->unk18 = -1;
    }
    return h->unk2;
}

Fx197Shard *func_801E7BBC(s32 i) {
    Fx197Shatter *h = D_801EAF80[i];
    Fx197Shard *p = D_801EAFA0[i];
    s32 k;

    for (k = 0; k < h->unk2; k++, p++) {
        if (p->unk18 < 0) {
            return p;
        }
    }
    return NULL;
}

s32 func_801E7C1C(s32 i) {
    s32 idx = 0;
    s32 k = 0;
    Fx197Shatter *h = D_801EAF80[i];
    Fx197Tri *t = h->unk8;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9660);
    s32 n = 0;
    s32 y = D_801EAFBC;
    Fx197Shard *p;
    SVECTOR v;

    for (; k < h->unk2; k++, t++, idx++) {
        if (y < t->unk8.vy && t->unk0.pad == 0) {
            p = func_801E7BBC(i);
            if (p != NULL) {
                t->unk0.pad = 1;
                p->unk0 = ((t->unk0.vx + t->unk8.vx + t->unk10.vx) / 3) << 12;
                p->unk4 = ((t->unk0.vy + t->unk8.vy + t->unk10.vy) / 3) << 12;
                p->unk8 = ((t->unk0.vz + t->unk8.vz + t->unk10.vz) / 3) << 12;
                p->unk18 = 0;
                p->unk1A = idx;
                p->unk12 = 0x1000;
                p->unk14 = ex->btlfx_randCentered(0x100);
                p->unk16 = ex->btlfx_randCentered(0x200);
                v.vx = p->unk0 >> 12;
                v.vy = p->unk4 >> 12;
                v.vz = p->unk8 >> 12;
                p->unkC = (v.vx >> 5) + ex->btlfx_randCentered(0x18);
                n++;
                p->unkE = (v.vy >> 5) + (ex->btlfx_rand() & 0x1F);
                p->unk10 = (v.vz >> 5) + ex->btlfx_randCentered(0x18);
            }
        }
    }
    return n;
}

void func_801E7E94(s32 i) {
    SVECTOR a;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR d0;
    SVECTOR d1;
    SVECTOR d2;
    SVECTOR zero;
    VECTOR out;
    VECTOR sv;
    MATRIX m;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9660);
    Fx197Shard *p;
    Fx197Shatter *h;
    Fx197Tri *t;
    s32 k;

    sv.vx = sv.vy = sv.vz = 0xF9C;
    p = D_801EAFA0[i];
    h = D_801EAF80[i];
    rot.vz = 0;
    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    for (k = 0; k < h->unk2; k++, p++) {
        if (p->unk18 >= 0) {
            t = &h->unk8[p->unk1A];
            c.vx = p->unk0 >> 12;
            c.vy = p->unk4 >> 12;
            c.vz = p->unk8 >> 12;
            d0.vx = t->unk0.vx - c.vx;
            d0.vy = t->unk0.vy - c.vy;
            d0.vz = t->unk0.vz - c.vz;
            d1.vx = t->unk8.vx - c.vx;
            d1.vy = t->unk8.vy - c.vy;
            d1.vz = t->unk8.vz - c.vz;
            d2.vx = t->unk10.vx - c.vx;
            d2.vy = t->unk10.vy - c.vy;
            d2.vz = t->unk10.vz - c.vz;
            ex->btlfx_stepModuleParticle((Pool6CEntry *)p, 0, &zero, &a);
            *(s32 *)&rot = *(s32 *)&p->unk14;
            ex->func_800316D8(&rot, &m);
            ex->ScaleMatrix(&m, &sv);
            m.t[0] = p->unk0 >> 12;
            m.t[1] = p->unk4 >> 12;
            m.t[2] = p->unk8 >> 12;
            ex->SetRotMatrix(&m);
            ex->SetTransMatrix(&m);
            gte_ldv0(&d0);
            gte_rtv0tr();
            gte_stlvnl(&out);
            gte_ldv0(&d1);
            gte_rtv0tr();
            t->unk0.vx = out.vx;
            t->unk0.vy = out.vy;
            t->unk0.vz = out.vz;
            gte_stlvnl(&out);
            gte_ldv0(&d2);
            gte_rtv0tr();
            t->unk8.vx = out.vx;
            t->unk8.vy = out.vy;
            t->unk8.vz = out.vz;
            gte_stlvnl(&out);
            t->unk10.vx = out.vx;
            t->unk10.vy = out.vy;
            t->unk10.vz = out.vz;
            p->unk18++;
            if (p->unk18 >= h->unk0) {
                p->unk18 = -1;
                t->unk10.vz = 0;
                *(s32 *)&t->unk10 = 0;
                t->unk8.vz = 0;
                *(s32 *)&t->unk8 = 0;
                t->unk0.vz = 0;
                *(s32 *)&t->unk0 = 0;
            }
        }
    }
}

s32 func_801E81EC(s32 mode, Fx197Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx197Work81 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR p;
    SVECTOR unused; /* allocated but never referenced in the original */
    s32 m;
    s32 m2;
    s32 c;
    s32 t;
    s32 on;
    Pool6CEntry *q;

    if (mode == 1) {
        it->unk0 = 0;
        it->unk8 = 0;
    } else if (it->unk2 < 6) {
        switch (it->unk0) {
        case 0:
            func_801E7704((Fx197Track *)ex->btlfx_getFileData(entry->unk8, D_801E91D4[it->unk2]), frame, &p);
            p.vx += work->unk8.vx;
            p.vy += work->unk8.vy;
            p.vz += work->unk8.vz;
            pos.vx = p.vx;
            pos.vy = p.vy;
            pos.vz = p.vz;
            scale.vx = 0x2000;
            scale.vy = 0x2000;
            scale.vz = 0x3000;
            rot.vx = -0x200;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, D_801EAF70);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, D_801EAF74);
            if (frame >= 3) {
                it->unk0 = 1;
                ((s16 *)it)[-1] = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x20, 0x46);
                it->unkC.vx = D_801E91EC[it->unk2].vx;
                it->unkC.vy = D_801E91EC[it->unk2].vy;
                it->unkC.vz = D_801E91EC[it->unk2].vz;
                it->unkC.vx += work->unk8.vx;
                it->unkC.vy += work->unk8.vy;
                it->unkC.vz += work->unk8.vz;
                it->unkC.vy = 0;
                q = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&it->unkC);
                if (q != NULL) {
                    q->unk22 = 0x80;
                }
                break;
            }
            return 0;
        case 1:
            if (frame + (it->unk2 - 5) <= 0) {
                m = work->unk18[it->unk2][0];
                m2 = -1;
            } else {
                if (frame < 0x10) {
                    m = work->unk18[it->unk2][D_801E921C[frame]];
                } else {
                    m = work->unk18[it->unk2][2];
                }
                if (frame >= 0x10) {
                    m2 = work->unk6C[it->unk2];
                } else {
                    m2 = -1;
                }
            }
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            if (m2 >= 0 && D_801E91C4 != 0) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, m2);
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, m);
            if (it->unk2 < 5) {
                on = D_801E91C8;
            } else {
                on = D_801E91D0;
            }
            if (on != 0) {
                it->unk0 = 2;
                ((s16 *)it)[-1] = -1;
                func_801E7A5C(it->unk2, 0x14, (Fx197Shatter *)(env->unk30 + D_801E9194[it->unk2].unk0),
                              (Fx197Tri *)(env->unk30 + D_801E9194[it->unk2].unk4));
                func_801E7AA0(it->unk2, work->unk18[it->unk2][2]);
                return 0;
            }
            break;
        case 2:
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk18[it->unk2][2]);
            if (frame >= 0xE) {
                t = 0x1000 - ((frame - 0xE) << 12) / 6;
                ex->Hi_ModifyEffModelAbr(work->unk18[it->unk2][2], 1);
                c = (t / 2 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk18[it->unk2][2], c, c, c);
            } else {
                ex->Hi_ModifyEffModelAbr(work->unk18[it->unk2][2], 1);
                c = -0x40;
                ex->Hi_ModifyEffModelRGB(work->unk18[it->unk2][2], c, c, c);
            }
            func_801E7C1C(it->unk2);
            func_801E7E94(it->unk2);
            if (frame < 0x14) {
                break;
            }
            return 1;
        default:
            return 0;
        }
    } else {
        switch (it->unk0) {
        case 0:
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk6C[6]);
            if (frame < 8) {
                s32 c0 = ((frame << 12) >> 9) - 0x80;

                ex->Hi_ModifyEffModelRGB(work->unk6C[6], c0, c0, c0);
            }
            if (D_801E91CC != 0) {
                it->unk0 = 1;
                ((s16 *)it)[-1] = -1;
                func_801E7A5C(it->unk2, 0x57, &D_801EAFC0, D_801EC5C0);
                func_801E7AA0(it->unk2, work->unk6C[6]);
                return 0;
            }
            break;
        case 1:
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk6C[6]);
            if (frame >= 0x37) {
                s32 c1 = ((0x1000 - (((frame - 0x37) << 12) >> 5)) >> 5) - 0x80;

                ex->Hi_ModifyEffModelRGB(work->unk6C[6], c1, c1, c1);
            }
            func_801E7C1C(6);
            func_801E7E94(6);
            if (frame >= 0x57) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_197_op81(s32 mode, Fx197Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx197Item81 *it;
    Fx197Item81 *it2;
    Pool6CEntry *p;
    s32 frame;
    s32 state;
    s32 i;
    s32 j;
    s32 c;

    if (mode == 0) {
        init->unk0 = 0x88;
        init->unk8 = 0x14;
        init->unk14 = (s32)func_801E81EC;
        init->unk4 = 7;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E9660 = env;
        ex->btlfx_initEffModels(D_801E9670, 0x20);
        D_801EAF74 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xD8), 0);
        D_801EAF70 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xD9), 1);
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 3; j++) {
                work->unk18[i][j] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E9130[i][j]), 0);
            }
        }
        for (i = 0; i < 7; i++) {
            work->unk6C[i] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E9178[i]), 1);
        }
        work->unk8.vx = D_801E9128.vx;
        work->unk8.vy = D_801E9128.vy;
        work->unk8.vz = D_801E9128.vz;
        work->unk10.vx = D_801E9128.vx;
        work->unk10.vy = D_801E9128.vy;
        work->unk10.vz = D_801E9128.vz;
    } else {
        state = work->unk0;
        frame = entry->unk0;
        switch (state) {
        case 0:
            if (frame < 10 && (frame & 1)) {
                it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it != NULL) {
                    it->unk2 = D_801E925C[frame >> 1];
                }
            }
            if (frame == 5) {
                it2 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it2 != NULL) {
                    it2->unk2 = 6;
                }
            }
            if (frame == 4) {
                it2 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it2 != NULL) {
                    it2->unk2 = 5;
                }
            }
            if (frame >= 16) {
                work->unk0 = 1;
                entry->unk0 = -1;
                D_801EAFBC = 0x400;
            }
            break;
        case 1:
            if (frame == 20) {
                D_801E91C8 = 1;
                D_801E91CC = 1;
            } else {
                D_801EAFBC = 0x400 - (frame << 7);
            }
            if (frame == 24) {
                D_801E91D0 = 1;
            }
            if (frame == 20) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E92D0;
                    p->unk12 = 0xC;
                    p->unkC = 0x4C;
                    p->unk22 = 0x40;
                }
            }
            if (frame == 18) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame == 32) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame == 44) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1A), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame == 19) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0x80);
            }
            if (frame == 20) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x80, 0xFF);
                D_801E91C4 = 0;
            }
            if (frame >= 20 && frame < 36) {
                c = 0x80 - (((frame - 20) << 7) >> 4);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, c >> 2, c);
            }
            ex->func_800E6624(frame * 2, 0x2C0, 0x180, 0x40, 0x80, 0x2C0, 0x100);
            if (frame >= 126) {
                return 1;
            }
            break;
        }
        ex->func_800E6624(frame, 0x280, 0x100, 0x40, 0x80, 0x280, 0x180);
        work->unk4++;
    }
    return 0;
}

s32 fx_197_op87(s32 mode, Fx197Work87 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        work->unk0 = *arg4 - 3;
        D_801E9660 = env;
    } else {
        frame = entry->unk0;
        func_801E7704((Fx197Track *)ex->btlfx_getFileData(entry->unk8, D_801E9270[work->unk0].unk2), frame,
                      &work->unk4);
        env->camera->unk0[1].vx = work->unk4.vx;
        env->camera->unk0[1].vy = work->unk4.vy;
        env->camera->unk0[1].vz = work->unk4.vz;
        func_801E7704((Fx197Track *)ex->btlfx_getFileData(entry->unk8, D_801E9270[work->unk0].unk0), frame,
                      &work->unkC);
        env->camera->unk0[0].vx = work->unkC.vx;
        env->camera->unk0[0].vy = work->unkC.vy;
        env->camera->unk0[0].vz = work->unkC.vz;
        if (frame >= D_801E9270[work->unk0].unk4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_197/nonmatchings/fx_197", fx_197_headerPtr);
