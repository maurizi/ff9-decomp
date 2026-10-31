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
    s32 a1;
    s32 a2;
    s32 a3;
    s32 a4;
    s32 c1;
    s32 c2;
    s32 c3;
    s32 frame;
    s32 c;
    s32 a;
    s32 r;
    s32 i;
    s32 n;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        fx_018_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        ex->btlfx_initEffModels(fx_018_effModels, 2);
        work->model14 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x11), 0xB8, 0x3E40, 0, 0x80);
        work->model18 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x10), 0x119, 0xFFFF, 0, 0x80);
        work->state10 = 0;
    } else {
        frame = entry->frame;
        switch (work->state10) {
        case 0:
            if (frame == 0) {
                work->emit1C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos8);
                if (work->emit1C != NULL) {
                    work->emit1C->otZ = 0x46;
                }
            }
            if (frame == 5) {
                work->emit20 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos8);
                if (work->emit20 != NULL) {
                    work->emit20->otZ = 0x46;
                }
            }
            if (frame == 28) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x40, 0);
            }
            if (frame == 29) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0x80);
            }
            if (frame >= 29) {
                work->state10 = 1;
                entry->frame = -1;
                ex->btlfx_stopEmitter(work->emit1C);
                ex->btlfx_stopEmitter(work->emit20);
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->pos8);
                if (p != NULL) {
                    p->otZ = 0x20;
                }
            }
            break;
        case 1:
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x20, 0x40, 0xFF);
            }
            if (frame == 1) {
                env->otZ = 0x200;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x20), &work->pos8, NULL, 0x2800, 0, 1, 0x1000,
                                  0, 0);
                env->otZ = 0;
            }
            env->otZ = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->pos8, NULL, 0x800, 0, 1, -1);
            env->otZ = 0;
            if (frame == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0xFF);
            }
            if (frame >= 2) {
                work->state10 = 2;
                entry->frame = -1;
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->pos8);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                work->emit1C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x26), (s32)&work->pos8);
                if (work->emit1C != NULL) {
                    work->emit1C->otZ = 0x80;
                }
            }
            break;
        case 2:
            a3 = (frame << 12) / 50;
            if (frame < 6) {
                c1 = ex->math_cos((frame << 10) / 6) * 0xFF;
                c2 = c1 >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c1 >> 14, c1 >> 13, c2);
            }
            if (frame < 20) {
                c2 = ex->math_cos((frame << 10) / 20);
                env->otZ = 0x200;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x17), &work->pos8, NULL, 0x2800, 0, 1, c2, 0, 0);
                if (frame < 10) {
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x20), &work->pos8, NULL, 0x2800, 0, 1,
                                      0x1000 - (frame << 12) / 10, 0, 0);
                }
                env->otZ = 0;
            }
            if (frame < 24) {
                env->otZ = 0x200;
                a1 = (frame << 12) / 24;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->pos8, NULL,
                                  ex->math_cos(a1 >> 2) * 3 / 2 + 0x1000, 0, 1, ex->math_cos(a1 >> 2));
                env->otZ = 0;
            }
            d.vx = 0x400;
            d.vz = 0;
            d.vy = 0;
            if (work->emit1C != NULL) {
                if (work->emit1C->state == BTLFX_EMIT_DONE) {
                    work->emit1C = NULL;
                }
                if (work->emit1C != NULL) {
                    work->emit1C->rot.vx = d.vx;
                    work->emit1C->rot.vy = d.vy;
                    work->emit1C->rot.vz = d.vz;
                }
            }
            if (frame < 16) {
                d.vx = 0x400;
                d.vz = 0;
                d.vy = 0;
                d.pad = 1;
                a2 = frame << 8;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x15), &work->pos8, &d,
                                  ex->math_sin(a2 >> 2) * 4, 0, 1, 0x1000 - a2);
            }
            d2.vx = 0x400;
            d2.vz = 0;
            d2.vy = 0;
            d2.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x13), &work->pos8, &d2, ex->math_sin(a3 >> 2) * 4, 0, 1,
                              ex->math_cos(a3 >> 2));
            if (frame == 1) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x27), (s32)&work->pos8);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
            }
            if (frame < 24) {
                pos.vx = work->pos8.vx;
                pos.vy = work->pos8.vy;
                pos.vz = work->pos8.vz;
                a4 = (frame << 12) / 24 >> 2;
                rot.vx = 0;
                rot.vy = frame * 0x18;
                rot.vz = 0;
                r = ((ex->math_sin(a4) * 0x5DC) >> 12) + 0x140;
                scale.vz = r;
                scale.vy = r;
                scale.vx = r;
                fx_018_ringRadius = (r * 0x708) >> 12;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model14);
                ex->Hi_ModifyEffModelAbr(work->model14, 1);
                n = frame;
                i = 0;
                ex->Hi_ModifyEffModelRGB(work->model14, (ex->math_cos(a4) * 2 / 3 >> 5) - 0x80, (ex->math_cos(a4) * 2 / 3 >> 5) - 0x80,
                                         (ex->math_cos(a4) * 2 / 3 >> 5) - 0x80);
                env->otZ = 0x20;
                c3 = ex->math_cos(a4);
                do {
                    BTLFX_COPY_SVEC(&v, &work->pos8);
                    v.vx += (ex->math_cos(frame * 0x18 + (i << 8)) * fx_018_ringRadius) >> 12;
                    v.vz += (ex->math_sin(frame * 0x18 + (i << 8)) * fx_018_ringRadius) >> 12;
                    {
                        void *fd_ = ex->btlfx_getFileData(entry->file, 0x1B);
                        s32 r0_ = ex->btlfx_randCentered(5); /* the PS1 draws them in this order */
                        s32 r1_ = ex->btlfx_randCentered(5);
                        ex->btlfx_drawParticles((u8 *)fd_, &v, NULL, 0x1800 - ((i & 1) << 12),
                                      n & 7, 1, c3, r0_, r1_);
                    }
                    i++;
                    n += 3;
                } while (i < 16);
                env->otZ = 0;
            }
            if (frame >= 50) {
                work->state10 = 2;
                return 1;
            }
            break;
        }
    }
    return 0;
}

static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_018_env);
    SVECTOR v;
    SVECTOR ang;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 s;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    p = strip->unk4;
    strip->count2 = 0xE;
    strip->lastSlot0 = 0xE;
    strip->head1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
        t += *tbl++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        s = *tbl++;
        s = (ex->math_sin(t >> 1) * s) >> 12;
        s += ex->btlfx_lerp(from1, to1, t);
        ex->btlfx_offsetPoint(&v, (s32)p, 0, (XY32 *)&ang, s, width);
        p->pad = arg11;
        width += dwidth + *tbl++;
        p++;
    }
    if (kind < 2) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_018_strip15Blocks[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_018_strip15Blocks[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedStrip3(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_018_env);
    SVECTOR v;
    SVECTOR ang;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 s;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    p = strip->unk4;
    strip->count2 = 2;
    strip->lastSlot0 = 2;
    strip->head1 = 0;
    for (i = 0; i < 3; i++) {
        t = 0x1000 - (i << 11);
        t += *tbl++ / 3;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        s = *tbl++ / 2;
        t >>= 1;
        s = (ex->math_sin(t) * s) >> 12;
        s += (ex->math_sin(t) * amp) >> 12;
        ex->btlfx_offsetPoint(&v, (s32)p, 0, (XY32 *)&ang, s, width);
        p->pad = arg10;
        width += dwidth + *tbl++ / 2;
        p++;
    }
    if (kind < 2) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_018_strip3Blocks[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_018_strip3Blocks[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx018Strip *strip, Fx018Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *p = strip->unk4;
    SVECTOR v;

    p += i;
    ex = BTLFX_EXPORTS(fx_018_env);
    ex->btlfx_lerpVec((s16 *)p, (s16 *)(p + 1), 0x4000, (s16 *)&v);
    fxm_drawJaggedStrip3(entry, out, tbl, p, &v, 0x80, ex->btlfx_rand(), 0x33, 0, arg5, arg6, kind);
}

static s32 fxm_jaggedStripTask(s32 mode, Fx018Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx018Strip *strip;
    Fx018Strip *br;
    s32 n;
    s32 c;
    s32 s;
    s32 t;

    if (mode == 1) {
        it->state11 = 0;
    } else if (it->state11 == 0) {
        if (it->kind14 < 2) {
            n = 8;
            if (fx_018_fastBolts != 0) {
                n = 6;
            }
            t = (frame << 12) / n;
            c = ex->math_cos(t >> 2);
            strip = &fx_018_strips[it->strip10];
            br = (Fx018Strip *)&fx_018_branches[it->strip10];
            fxm_drawJaggedStrip15(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame * 0x97, 0x186, frame, c,
                          ((0x1000 - t) * (it->width15 * 2 + 0x97)) >> 12, it->kind14);
            fxm_drawJaggedBranch3(entry, strip, br, it->jitter18, it->branchAt12, c, ((0x1000 - t) * (it->width15 * 2 + 0x97)) >> 13,
                          it->kind14);
            fxm_drawJaggedBranch3(entry, strip, br, it->jitter18 + 3, it->branchAt13, c,
                          ((0x1000 - t) * (it->width15 * 2 + 0x97)) >> 13, it->kind14);
            if (frame >= n) {
                it->state11 = 1;
                return 1;
            }
        } else {
            n = 6;
            if (fx_018_fastBolts != 0) {
                n = 4;
            }
            t = (frame << 12) / n;
            c = ex->math_cos(t >> 2);
            strip = &fx_018_strips[it->strip10];
            br = (Fx018Strip *)&fx_018_branches[it->strip10];
            fxm_drawJaggedStrip15(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame << 6, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, it->kind14);
            fxm_drawJaggedBranch3(entry, strip, br, it->jitter18, it->branchAt12, c, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch3(entry, strip, br, it->jitter18 + 3, it->branchAt13, c, (0x1000 - t) >> 6, it->kind14);
            if (frame >= n) {
                it->state11 = 1;
                return 1;
            }
        }
    }
    return 0;
}

static s32 fxm_randStripPoint(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_018_env);
    SVECTOR *p = fx_018_strips[ex->btlfx_rand() % 16].unk4;

    p += ex->btlfx_rand() % 15;

    out->vx = p->vx;
    out->vy = p->vy;
    out->vz = p->vz;
    out->pad = 1;
    return 0;
}

s32 fx_018_op83(s32 mode, Fx018Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    Fx018Bolt *it0;
    s32 r0;
    s32 ang0;
    Fx018Bolt *it1;
    s32 r1;
    s32 ang1;
    Fx018Bolt *it2;
    s32 r2;
    s32 ang2;
    Fx018Bolt *it3;
    Fx018Bolt *it4;
    s32 r4;
    Fx018Bolt *it5;
    Pool6CEntry *p;
    s32 *q;
    s32 frame;
    s32 kind;
    s32 rnd2;
    s32 rnd3;
    s32 rnd5;
    s32 base2;
    s32 base3;
    s32 i;
    s32 j;
    s32 s;

    if (mode == 0) {
        init->workSize = 0x1C;
        init->elemSize = 0x24;
        init->elemFn = (s32)fxm_jaggedStripTask;
        init->poolCount = 0x10;
    } else if (mode == 1) {
        fx_018_env = env;
        kind = init->workSize;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        i = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        q = work->jitter10 = fx_018_jitter;
        for (; i < 0x40; i++) {
            *q++ = ex->btlfx_randCentered(0x100);
            *q++ = ex->btlfx_randCentered(0x200);
            *q++ = ex->btlfx_randCentered(0xF5);
        }
        work->kind18 = kind;
        fx_018_task = entry;
        work->boltCount14 = 0;
    } else {
        frame = entry->frame;
        switch (work->kind18) {
        case 0:
            fx_018_fastBolts = 0;
            if (frame < 29) {
                if (frame & 1) {
                    work->boltCount14++;
                    BTLFX_COPY_SVEC(&a, &work->pos0);
                    a.vy = 0;
                    it0 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it0 != NULL) {
                        BTLFX_COPY_SVEC(&it0->pos0, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        b.vy -= 0xC00;
                        r0 = ex->btlfx_randCentered(0x100) + 0x4B0;
                        ang0 = (work->boltCount14 << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                        b.vx += (ex->math_cos(ang0) * r0) >> 12;
                        b.vz += (ex->math_sin(ang0) * r0) >> 12;
                        BTLFX_COPY_SVEC(&it0->pos8, &b);
                        it0->strip10 = work->boltCount14 % 16;
                        it0->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                        it0->branchAt12 = ex->btlfx_rand() % 13;
                        it0->branchAt13 = ex->btlfx_rand() % 13;
                        it0->width15 = 0;
                        it0->kind14 = 2;
                    }
                }
            }
            if (frame >= 29) {
                return 1;
            }
            break;
        case 1:
            fx_018_fastBolts = 0;
            if (frame == 0) {
                for (j = 0; j < 6; j++) {
                    work->boltCount14++;
                    BTLFX_COPY_SVEC(&a, &work->pos0);
                    a.vy = 0;
                    it1 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it1 != NULL) {
                        BTLFX_COPY_SVEC(&it1->pos0, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        b.vy -= 0xC00;
                        r1 = ex->btlfx_randCentered(0x80) + 0x3E8;
                        ang1 = (work->boltCount14 << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                        b.vx += (ex->math_cos(ang1) * r1) >> 12;
                        b.vz += (ex->math_sin(ang1) * r1) >> 12;
                        BTLFX_COPY_SVEC(&it1->pos8, &b);
                        it1->strip10 = work->boltCount14 % 16;
                        it1->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                        it1->branchAt12 = ex->btlfx_rand() % 13;
                        it1->branchAt13 = ex->btlfx_rand() % 13;
                        it1->width15 = -12;
                        it1->kind14 = ex->btlfx_rand() & 1;
                    }
                }
            }
            if (frame >= 6 && frame < 16) {
                work->boltCount14++;
                BTLFX_COPY_SVEC(&c, &work->pos0);
                c.vy = 0;
                it2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it2 != NULL) {
                    BTLFX_COPY_SVEC(&it2->pos0, &c);
                    BTLFX_COPY_SVEC(&d, &c);
                    d.vy -= 0xC00;
                    rnd2 = ex->btlfx_randCentered(0x100);
                    base2 = (frame - 6) << 8;
                    rnd2 += 0x5DC;
                    r2 = base2 + rnd2;
                    ang2 = ((work->boltCount14 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    d.vx += (ex->math_cos(ang2) * r2) >> 12;
                    d.vz += (ex->math_sin(ang2) * r2) >> 12;
                    BTLFX_COPY_SVEC(&it2->pos8, &d);
                    it2->strip10 = work->boltCount14 % 16;
                    it2->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                    it2->branchAt12 = ex->btlfx_rand() % 13;
                    it2->branchAt13 = ex->btlfx_rand() % 13;
                    it2->width15 = 7;
                    it2->kind14 = ex->btlfx_rand() & 1;
                }
                work->boltCount14++;
                BTLFX_COPY_SVEC(&c, &work->pos0);
                c.vy = 0;
                it3 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it3 != NULL) {
                    BTLFX_COPY_SVEC(&it3->pos0, &c);
                    BTLFX_COPY_SVEC(&d, &c);
                    d.vy -= 0xC00;
                    rnd3 = ex->btlfx_randCentered(0x100);
                    base3 = (frame - 6) << 8;
                    rnd3 += 0x7D0;
                    r2 = base3 + rnd3;
                    ang2 = ((work->boltCount14 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    d.vx += (ex->math_cos(ang2) * r2) >> 12;
                    d.vz += (ex->math_sin(ang2) * r2) >> 12;
                    BTLFX_COPY_SVEC(&it3->pos8, &d);
                    it3->strip10 = work->boltCount14 % 16;
                    it3->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                    it3->branchAt12 = ex->btlfx_rand() % 13;
                    it3->branchAt13 = ex->btlfx_rand() % 13;
                    it3->width15 = 0;
                    it3->kind14 = 2;
                }
            }
            if (frame == 8 || frame == 10 || frame == 13) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x10, 0x40, 0x80);
            }
            if (frame == 9) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
            }
            if (frame >= 20) {
                return 1;
            }
            break;
        case 2:
            fx_018_fastBolts = 1;
            if (frame == 0) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x25), (s32)&work->pos0);
                if (p != NULL) {
                    p->tail->shape = -1;
                    p->tail->s.module.spawnFn = fxm_randStripPoint;
                    p->otZ = 0x20;
                }
            }
            if (frame < 31) {
                if (frame == 0) {
                    BTLFX_COPY_SVEC(&fx_018_prevBoltEnd, &work->pos0);
                }
                s = frame * 250 / 31 + 0x333;
                work->boltCount14++;
                BTLFX_COPY_SVEC(&a, &work->pos0);
                a.vy = 0;
                c.vz = 0;
                BTLFX_COPY_SVEC(&b, &a);
                r4 = s + ex->btlfx_randCentered(0x46);
                c.vy = ex->btlfx_rand();
                c.vx = (ex->btlfx_rand() & 0xFF) + 0x80;
                ex->btlfx_pointAhead((s16 *)&b, (s16 *)&c, r4, (s16 *)&b);
                work->boltCount14++;
                BTLFX_COPY_SVEC(&a, &work->pos0);
                a.vy = 0;
                it4 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it4 != NULL) {
                    BTLFX_COPY_SVEC(&it4->pos0, &b);
                    BTLFX_COPY_SVEC(&it4->pos8, &fx_018_prevBoltEnd);
                    it4->strip10 = work->boltCount14 % 16;
                    it4->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                    it4->branchAt12 = ex->btlfx_rand() % 13;
                    it4->branchAt13 = ex->btlfx_rand() % 13;
                    it4->width15 = -(frame * 50 / 31);
                    it4->kind14 = ex->btlfx_rand() & 1;
                }
                BTLFX_COPY_SVEC(&fx_018_prevBoltEnd, &b);
                if (frame < 19) {
                    work->boltCount14++;
                    BTLFX_COPY_SVEC(&a, &work->pos0);
                    a.vy = 0;
                    c.vz = 0;
                    it5 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it5 != NULL) {
                        BTLFX_COPY_SVEC(&it5->pos8, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        rnd5 = ex->btlfx_randCentered(0x46) + 0x400;
                        r4 = s + rnd5;
                        c.vy = ex->btlfx_rand();
                        c.vx = (ex->btlfx_rand() & 0xFF) + 0x80;
                        ex->btlfx_pointAhead((s16 *)&b, (s16 *)&c, r4, (s16 *)&b);
                        BTLFX_COPY_SVEC(&it5->pos0, &b);
                        it5->strip10 = work->boltCount14 % 16;
                        it5->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                        it5->branchAt12 = ex->btlfx_rand() % 13;
                        it5->branchAt13 = ex->btlfx_rand() % 13;
                        it5->width15 = 0;
                        it5->kind14 = 2;
                    }
                }
            }
            if (frame & 1) {
                if (ex->btlfx_rand() & 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x10, 0x20, 0x46);
                }
            }
            if (frame >= 31) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_018/nonmatchings/fx_018", fx_018_headerPtr);
