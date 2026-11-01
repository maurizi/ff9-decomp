#include "fxmod/fx_0dc.h"

void fx_0dc_shardPoint(s16 *a, s16 *b, s32 i, s16 *out) {
    BTLFX_EXPORTS(fx_0dc_env)->btlfx_pointAhead(a, b, -((fx_0dc_shardDists[i] * 0x455) >> 9), out);
}

static s32 fx_0dc_op80PoolTask(s32 mode, Fx0dcItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx0dcWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *p;
    Pool6CEntry *e;
    Fx0dcItem *q;
    s32 t;
    s32 c;
    s32 k;
    s32 n;
    s32 d;

    if (mode == 1) {
        it->state0 = 0;
        it->frame8 = 0;
    } else if (it->flying2 == 0) {
        SVECTOR rot;
        VECTOR scale;
        SVECTOR v38;
        SVECTOR v40;
        SVECTOR v48;
        SVECTOR v50;
        SVECTOR v58;

        p = &fx_0dc_shardAngles[it->shard4];
        switch (it->state0) {
        case 0:
            if (it->frame8 == 0) {
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x25), (s32)&it->pos18);
                it->emit34 = e;
                if (e != NULL) {
                    e->otZ = 0x80;
                }
            }
            scale.vx = 0;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->emit34 != NULL) {
                it->emit34->rot.vx = rot.vx;
                it->emit34->rot.vy = rot.vy;
                it->emit34->rot.vz = rot.vz;
            }
            if (++it->frame8 > 0) {
                it->state0 = 1;
                it->frame8 = 0;
            }
            break;
        case 1:
            {
                s32 a = it->frame8 << 10;

                scale.vx = scale.vy = (a * 0x8AA) >> 12;
                scale.vz = (ex->math_sin(a >> 2) * 0x8AA) >> 12;
            }
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->emit34 != NULL) {
                it->emit34->rot.vx = rot.vx;
                it->emit34->rot.vy = rot.vy;
                it->emit34->rot.vz = rot.vz;
            }
            if (++it->frame8 >= 4) {
                it->state0 = 2;
                it->frame8 = 0;
            }
            break;
        case 2:
            scale.vx = scale.vy = scale.vz = 0x8AA;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->emit34 != NULL) {
                it->emit34->rot.vx = rot.vx;
                it->emit34->rot.vy = rot.vy;
                it->emit34->rot.vz = rot.vz;
            }
            if (frame < 12) {
                rot.pad = 1;
                v38.vx = it->posC.vx;
                v38.vy = it->posC.vy;
                v38.vz = it->posC.vz;
                {
                    s32 a = (frame << 10) / 12;
                    s32 b;

                    ex->btlfx_pointAhead((s16 *)&v38, (s16 *)&rot, 0x2F6, (s16 *)&v40);
                    d = ex->btlfx_getFileData(entry->file, 0x24);
                    b = (ex->math_sin(a) * 0x1154) >> 12;
                    ex->btlfx_drawRings((u8 *)d, &v40, &rot, b, 0, 1, ex->math_cos(a) * 2 / 3);
                }
            }
            n = 6;
            if (work->split78 != 0) {
                it->frame8 = 0;
                if (it->shard4 < 3) {
                    n = 10;
                }
                BTLFX_COPY_SVEC(&v50, &it->pos18);
                for (k = 0; k < 4; k++, n++) {
                    q = it;
                    if (k != 0) {
                        q = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    }
                    if (q != NULL) {
                        q->flying2 = 1;
                        q->state0 = n;
                        q->shard4 = k;
                        q->frame8 = 0;
                        q->model2C = work->model20[n];
                        fx_0dc_shardPoint((s16 *)&v50, (s16 *)p, n, (s16 *)&v48);
                        BTLFX_COPY_SVEC(&q->rot20, p);
                        q->posC.vx = v48.vx << 4;
                        q->posC.vy = v48.vy << 4;
                        q->posC.vz = v48.vz << 4;
                        q->spinX28 = ex->btlfx_randCentered(0x84);
                        q->spinY2A = ex->btlfx_randCentered(0x84);
                        d = 0x80; /* through a variable, so the 0x80 isn't folded into k * 0x29 */
                        t = -(((ex->btlfx_rand() & 0xF) + d - k * 0x29) * 16);
                        BTLFX_COPY_SVEC(&v58, p);
                        v58.vx += 0x80;
                        ex->btlfx_rotateOffset(&v58, t, (s32)&q->pos18);
                        q->pos18.vx += ex->btlfx_randCentered(0x200);
                        q->pos18.vy += ex->btlfx_randCentered(0x200);
                        q->pos18.vz += ex->btlfx_randCentered(0x200);
                    }
                }
            }
            break;
        }
        if (scale.vx != 0) {
            scale.vx *= 2;
            scale.vy *= 2;
            scale.vz = scale.vz * 5 / 2;
            ex->Hi_SetEffModelOffset(it->model2C, 2);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->posC, (s32)&scale, it->model2C);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->posC, (s32)&scale, it->model30);
            return 0;
        }
    } else {
        VECTOR pos;
        VECTOR scale;

        t = ((it->frame8 << 12) / 22) >> 2;
        c = ex->math_cos(t);
        it->posC.vx += it->pos18.vx;
        it->posC.vy += it->pos18.vy;
        it->posC.vz += it->pos18.vz;
        it->pos18.vy += 0x40;
        it->rot20.vx += it->spinX28;
        it->rot20.vy += it->spinY2A;
        c = (c * 3) >> 1;
        if (it->state0 < 6) {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x19FE) >> 12;
            scale.vz = (ex->math_cos(t) * 0x1154) >> 12;
        } else {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x1154) >> 12;
            scale.vz = (ex->math_cos(t) * 0x15A9) >> 12;
        }
        pos.vx = it->posC.vx >> 4;
        pos.vy = it->posC.vy >> 4;
        pos.vz = it->posC.vz >> 4;
        ex->Hi_DrawEffModel(&it->rot20, &pos, (s32)&scale, it->model2C);
        ex->Hi_ModifyEffModelAbr(it->model2C, 1);
        ex->Hi_ModifyEffModelRGB(it->model2C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (++it->frame8 >= 22) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0dc/nonmatchings/fx_0dc", fx_0dc_headerPtr);

s32 fx_0dc_op80(s32 mode, Fx0dcWork80 *w, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR rot2;
    Fx0dcItem *it;
    Fx0dcItem *q; /* separate from it: one shard pointer per state (register allocation) */
    Pool6CEntry *e;
    SVECTOR *p;
    s32 frame;
    s32 k;
    s32 c;
    s32 g;
    s32 t;
    s32 i;
    s32 j;
    s32 tp; /* the 0x9A tpage, held in a register across the calls */
    s32 r; /* spawn radius */

    if (mode == 0) {
        init->workSize = 0x88;
        init->elemSize = 0x38;
        init->elemFn = (s32)fx_0dc_op80PoolTask;
        init->poolCount = 0x20;
    } else if (mode == 1) {
        fx_0dc_env = env;
        ex->btlfx_initEffModels(fx_0dc_effModels, 0x19);
        tp = 0x9A;
        w->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x14), tp, 0x3DC0,
                                                  0x80, 0);
        w->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x12), tp, 0x3DC0,
                                                  0x80, 0);
        w->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x13), tp, 0x3DC0,
                                                   0x80, 0);
        w->model14 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x11), 0xBA, 0x3D40,
                                                   0, 0x80);
        w->model18 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xF), 0xBA, 0x3D40,
                                                   0, 0x80);
        w->model1C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x10), 0xBA, 0x3D40,
                                                   0, 0x80);
        for (i = 0; i < 14; i++) {
            w->model20[i] = (s16)ex->Hi_RegisterTexEffModel(
                (BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_0dc_shardModelBlocks[i]), tp, 0x3DC0, 0x80, 0);
        }
        w->state0 = 2;
        w->frame4 = 0;
        w->spawnCount7A = 0;
        w->split78 = 0;
        w->scrollFrame7C = 0;
        w->scroll80 = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&w->pos58);
        BTLFX_COPY_SVEC(&w->pos60, &w->pos58);
        w->pos60.vy -= 0x22A;
        BTLFX_COPY_SVEC(&w->pos70, &w->pos58);
        w->pos70.vy -= 0x80;
        BTLFX_COPY_SVEC(&w->pos68, &w->pos58);
        w->pos68.vy -= 0x5C6;
        e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x26), (s32)&w->pos58);
        if (e != NULL) {
            e->otZ = 0x80;
        }
        e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x27), (s32)&w->pos68);
        if (e != NULL) {
            e->otZ = 0x80;
        }
        e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2A), (s32)&w->pos58);
        if (e != NULL) {
            e->otZ = 0x80;
        }
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x80);
    } else {
        frame = entry->frame;
        pos.vx = w->pos58.vx;
        pos.vy = w->pos58.vy;
        pos.vz = w->pos58.vz;
        scale.vx = scale.vy = 0x1DFE;
        scale.vz = 0x1354;
        if (w->frame4 < 3) {
            t = w->frame4 << 8;
            scale.vx = scale.vy = (t * scale.vx) >> 10;
            scale.vz = (scale.vz * ex->math_sin(t)) >> 12;
        }
        k = 0;
        switch (w->state0) {
        case 2:
            k = 0x1000;
            c = 0x2000 - (frame << 10);
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model8);
            ex->Hi_ModifyEffModelRGB(w->model8, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model14);
            if (frame < 6) {
                c = 0xFF - frame * 0xFF / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 2, c * 2 / 3, c);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 4) {
                w->state0 = 3;
                entry->frame = -1;
            }
            break;
        case 3:
            k = 0x1000;
            if (frame & 1) {
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    it->flying2 = 0;
                    it->shard4 = w->spawnCount7A;
                    p = &fx_0dc_shardAngles[w->spawnCount7A];
                    if (w->spawnCount7A >= 3) {
                        it->model2C = w->modelC;
                        it->model30 = w->model18;
                    } else {
                        it->model2C = w->model10;
                        it->model30 = w->model1C;
                    }
                    it->posC.vx = w->pos58.vx;
                    it->posC.vy = w->pos58.vy;
                    it->posC.vz = w->pos58.vz;
                    r = 200;
                    it->posC.vx += (ex->math_sin(p->vy + 0x800) * r) >> 12;
                    it->posC.vz += (ex->math_cos(p->vy + 0x800) * r) >> 12;
                    it->pos18.vx = it->posC.vx;
                    it->pos18.vy = it->posC.vy;
                    it->pos18.vz = it->posC.vz;
                    w->spawnCount7A++;
                }
            }
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model14);
            if (w->spawnCount7A >= 6) {
                w->state0 = 4;
                entry->frame = -1;
                w->emit84 = e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1C), (s32)&w->pos58);
                if (e != NULL) {
                    e->otZ = 0x5C;
                }
            }
            break;
        case 4:
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model14);
            if (frame >= 8) {
                w->state0 = 5;
                entry->frame = -1;
            }
            break;
        case 5:
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model14);
            rot.pad = 1;
            if (frame >= 10) {
                w->state0 = 6;
                w->split78 = 1;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                ex->btlfx_stopEmitter(w->emit84);
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x20), (s32)&w->pos58);
                if (e != NULL) {
                    e->otZ = 0x60;
                }
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&w->pos58);
                if (e != NULL) {
                    e->otZ = 0x60;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1D), (s32)&w->pos60);
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&w->pos60);
                if (e != NULL) {
                    e->otZ = 0x80;
                }
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&w->pos60);
                if (e != NULL) {
                    e->otZ = 0x80;
                }
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2B), (s32)&w->pos60);
                if (e != NULL) {
                    e->otZ = 0x80;
                }
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = -0x400;
                for (j = 0; j < 6; j++) {
                    q = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (q != NULL) {
                        q->flying2 = 1;
                        q->state0 = j;
                        q->shard4 = j;
                        q->frame8 = 0;
                        q->model2C = w->model20[j];
                        fx_0dc_shardPoint((s16 *)&w->pos58, (s16 *)&rot, j, (s16 *)&rot2);
                        BTLFX_COPY_SVEC(&q->rot20, &rot);
                        q->posC.vx = rot2.vx * 16;
                        q->posC.vy = rot2.vy * 16;
                        q->posC.vz = rot2.vz * 16;
                        q->spinX28 = ex->btlfx_randCentered(0x5C);
                        q->spinY2A = ex->btlfx_randCentered(0x80);
                        ex->btlfx_rotateOffset(&rot, -(((ex->btlfx_rand() & 0xF) + (0x5C - j * 8)) * 16), (s32)&q->pos18);
                        q->pos18.vx += ex->btlfx_randCentered(0x3E0);
                        q->pos18.vz += ex->btlfx_randCentered(0x3E0);
                    }
                }
            }
            break;
        case 6:
            rot.vx = -0x400;
            rot.vz = -0x400;
            rot.vy = 0;
            if (frame < 18) {
                k = ex->math_cos((frame << 10) / 18);
            }
            if (frame == 8) {
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&w->pos60);
                if (e != NULL) {
                    e->otZ = 0x80;
                }
            }
            if (frame < 8) {
                g = 0xFF - ((frame * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, g >> 2, g * 2 / 3, g);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 24) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (w->state0 >= 2 && w->state0 < 6) {
            w->scrollFrame7C++;
            if (w->scrollFrame7C < 11) {
                w->scroll80 += 11 - (w->scrollFrame7C * 10) / 11;
            } else {
                w->scroll80++;
            }
            ex->btlfx_drawScrollMove(w->scroll80, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        }
        if (k != 0) {
            rot2.vx = 0x400;
            rot2.vy = 0;
            rot2.vz = w->frame4 * 41;
            rot2.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x23), &w->pos58, &rot2, (w->frame4 << 6) + 0x15A9,
                              0, 1, k);
            rot2.vz = w->frame4 * 24 + 0x400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x23), &w->pos58, &rot2, (w->frame4 << 6) + 0x15A9,
                              0, 1, k);
            env->otZ = 0x127;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1B), &w->pos58, NULL, (w->frame4 << 5) + 0x15A9,
                              0, 1, k);
            env->otZ = 0;
        }
        w->frame4++;
    }
    return 0;
}

s32 fx_0dc_op84(s32 mode, Fx0dcWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 k;
    s32 t;
    s32 d;
    u16 r;

    if (mode == 0) {
        *args = 0x10;
    } else {
        if (mode == 1) {
            fx_0dc_env = env;
            work->state0 = 0;
            work->frame4 = 0;
        } else {
            r = c = b = a = 0;
            frame = entry->frame;
            ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
            switch (work->state0) {
            case 0:
                r = 0x200;
                a = (frame << 12) / 3;
                b = ex->math_sin(a >> 2) >> 1;
                c = ((0x1000 - a) >> 1) + 0x800;
                if (frame >= 3) {
                    work->state0 = 1;
                    entry->frame = -1;
                }
                break;
            case 1:
                k = (frame & 1) << 8;
                c = k + 0x1000;
                d = (frame << 12) / 9;
                a = (k + 0x800) + (d >> 1);
                r = (d >> 3) + 0x200;
                b = c;
                if (frame >= 9) {
                    work->state0 = 2;
                    entry->frame = -1;
                }
                break;
            case 2:
                d = (frame << 12) / 9;
                t = d >> 2;
                r = 0x400 - t;
                a = ex->math_cos(t);
                b = ex->math_sin(t) + 0x1000;
                c = ex->math_sin(t) * 2 + 0x1000;
                if (frame >= 9) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = r;
            rot.pad = 0;
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x29), &work->pos8, NULL, b / 3, 0, 1, a / 2);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x28), &work->pos8, &rot, c, 0, 1, a * 2 / 3);
            env->otZ = 0;
            work->frame4++;
        }
    }
    return 0;
}
