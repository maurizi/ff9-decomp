#include "fxmod/fx_1e6.h"

void fx_1e6_buildTrail(Fx1e6Trail *trail, s32 size, s32 start) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1e6_env);
    MATRIX m;
    VECTOR o;
    SVECTOR v;
    SVECTOR *p;
    s32 n;
    s32 i;
    s32 j;

    n = (size - 4) / 8;
    p = trail->pos4;
    trail->lastSlot0 = trail->count2 = n - 1;
    trail->head1 = 0;
    if (start < n) {
        for (i = 0; i < n - start; i++) {
        fxm_posPathGet(&fx_1e6_posPath, 0, &v);
        v.vx += fx_1e6_trailOrigin.vx;
        v.vy += fx_1e6_trailOrigin.vy;
        v.vz += fx_1e6_trailOrigin.vz;
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = 0;
        p++;
        }
    } else {
        start = n;
    }
    for (i = 0; i < start;) {
        j = i + 1;
        fxm_posPathGet(&fx_1e6_posPath, start - j, &v);
        v.vx = v.vx * 6 / 10;
        v.vy = v.vy * 6 / 10;
        v.vz = v.vz * 6 / 10;
        ex->math_rotMatrixYXZ(&fx_1e6_trailRot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(&o);
        o.vx += fx_1e6_trailOrigin.vx;
        o.vy += fx_1e6_trailOrigin.vy;
        o.vz += fx_1e6_trailOrigin.vz;
        p->vx = o.vx;
        p->vy = o.vy;
        p->vz = o.vz;
        p->pad = 0x80;
        p++;
        i = j;
    }
}

s32 fx_1e6_op80(s32 mode, Fx1e6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    u32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        fx_1e6_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        work->pos4.vy -= 0x12C;
        fx_1e6_trailOrigin.vx = work->pos4.vx;
        fx_1e6_trailOrigin.vy = work->pos4.vy;
        fx_1e6_trailOrigin.vz = work->pos4.vz;
        fxm_posPathInit(&fx_1e6_posPath, (u8 *)ex->btlfx_getFileData(entry->file, 0xD));
        work->alphaC = 0x1000;
    } else {
        frame = entry->frame;
        ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->pos4, (s16 *)&fx_1e6_trailRot);
        if (frame < 0x20) {
            if ((s32)frame >= 0x18) {
                work->alphaC -= 0x400;
            }
            fx_1e6_buildTrail(&fx_1e6_op80Trail, 0x10C, frame * 4);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xE), 0, NULL, 0, 0x1000, work->alphaC, 1, (u8 *)&fx_1e6_op80Trail, 0);
        }
        return (s32)frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1e6/nonmatchings/fx_1e6", fx_1e6_headerPtr);

s32 fx_1e6_op81(s32 mode, Fx1e6Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    s32 frame;
    s32 w;
    s32 t;
    s32 k;

    if (mode == 0) {
        *arg4 = 0x68;
    } else if (mode == 1) {
        fx_1e6_env = env;
        ex->btlfx_initEffModels(fx_1e6_effModels, 2);
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos48);
        work->scale34.vx = 0x1000;
        work->scale34.vy = 0x1000;
        work->scale34.vz = 0x1000;
        work->frame58 = 0;
        work->rot1C.vx = 0;
        work->rot1C.vy = 0;
        work->rot1C.vz = 0;
        work->pos24.vx = work->pos48.vx;
        work->pos24.vy = work->pos48.vy;
        work->pos24.vz = work->pos48.vz;
        work->pos24.vy = -0x3CA;
        work->model64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 8), 1);
        work->model66 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 1);
        work->timer5C = -1;
        work->timer60 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 8:
            work->pos50.vx = work->pos48.vx;
            work->pos50.vy = work->pos48.vy;
            work->pos50.vz = work->pos48.vz;
            work->pos50.vy -= 0xFA;
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos50);
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos50);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos50);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos50);
            work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos50);
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos50);
            work->emit0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos50);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->trails = (s32)fx_1e6_op81Trails0;
                work->emit0[0]->trailSlots = 5;
                work->emit0[0]->trailStride = 0x90;
            }
            if (work->emit0[1] != NULL) {
                work->emit0[1]->trails = (s32)fx_1e6_op81Trails1;
                work->emit0[1]->trailSlots = 5;
                work->emit0[1]->trailStride = 0x90;
            }
            if (work->emit0[2] != NULL) {
                work->emit0[2]->trails = (s32)fx_1e6_op81Trails2;
                work->emit0[2]->trailSlots = 5;
                work->emit0[2]->trailStride = 0x90;
            }
            if (work->emit0[3] != NULL) {
                work->emit0[3]->trails = (s32)fx_1e6_op81Trails3;
                work->emit0[3]->trailSlots = 0x10;
                work->emit0[3]->trailStride = 0x18;
            }
            if (work->emit0[4] != NULL) {
                work->emit0[4]->trails = (s32)fx_1e6_op81Trails4;
                work->emit0[4]->trailSlots = 0x10;
                work->emit0[4]->trailStride = 0x18;
            }
            if (work->emit0[5] != NULL) {
                work->emit0[5]->trails = (s32)fx_1e6_op81Trails5;
                work->emit0[5]->trailSlots = 0x10;
                work->emit0[5]->trailStride = 0x18;
            }
            break;
        case 0:
            work->timer5C = 4;
            break;
        case 0x13:
            work->timer60 = 0x10;
            break;
        }
        if ((u32)frame < 0x23) {
            t = frame;
            if (frame >= 9) {
                if (work->frame58 >= 4) {
                    w = 0;
                } else {
                    w = 0x1000 - (work->frame58 << 12) / 3;
                }
                work->frame58++;
            } else {
                w = 0x1000;
            }
            rot.vx = work->rot1C.vx;
            rot.vy = work->rot1C.vy;
            rot.vz = work->rot1C.vz;
            pos.vx = work->pos24.vx;
            pos.vy = work->pos24.vy;
            pos.vz = work->pos24.vz;
            if ((u32)t < 8) {
                pos.vy += frame * 4;
                work->pos24.vy = pos.vy;
                k = t % 8; switch (k) {
                case 0:
                case 4:
                    break;
                case 1:
                case 3:
                    rot.vz = 0x155;
                    pos.vx -= 0x14;
                    break;
                case 2:
                    rot.vz = 0x200;
                    pos.vx -= 0x28;
                    break;
                case 5:
                case 7:
                    rot.vz = -0x155;
                    pos.vx += 0x14;
                    break;
                case 6:
                    rot.vz = -0x200;
                    pos.vx += 0x28;
                    break;
                }
            }
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&work->scale34, work->model64, work->model66, w);
            if (work->timer5C >= 0 || work->timer60 >= 0) {
                if (work->timer5C >= 0) {
                    work->rgb44 = fxm_lerpRatio(-0x80, 0, 4 - work->timer5C, 4);
                    work->timer5C--;
                }
                if (work->timer60 >= 0) {
                    work->rgb44 = fxm_lerpRatio(0, -0x80, 0x10 - work->timer60, 0x10);
                    work->timer60--;
                }
                ex->Hi_ModifyEffModelAbr(work->model64, 1);
                ex->Hi_ModifyEffModelRGB(work->model64, work->rgb44, work->rgb44, work->rgb44);
            }
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1e6_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posPathInit(Fx1e6Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1e6Point *)data;
}

static s32 fxm_posPathGet(Fx1e6Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1e6Point *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->keys4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}
