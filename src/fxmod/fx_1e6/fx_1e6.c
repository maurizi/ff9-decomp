#include "fxmod/fx_1e6.h"

void func_801E7724(Fx1e6Trail *trail, s32 size, s32 start) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8348);
    MATRIX m;
    VECTOR o;
    SVECTOR v;
    SVECTOR *p;
    s32 n;
    s32 i;
    s32 j;

    n = (size - 4) / 8;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
    if (start < n) {
        for (i = 0; i < n - start; i++) {
        func_801E829C(&D_801E84E8, 0, &v);
        v.vx += D_801E92F8.vx;
        v.vy += D_801E92F8.vy;
        v.vz += D_801E92F8.vz;
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
        func_801E829C(&D_801E84E8, start - j, &v);
        v.vx = v.vx * 6 / 10;
        v.vy = v.vy * 6 / 10;
        v.vz = v.vz * 6 / 10;
        ex->func_800316D8(&D_801E9300, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(&o);
        o.vx += D_801E92F8.vx;
        o.vy += D_801E92F8.vy;
        o.vz += D_801E92F8.vz;
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
        D_801E8348 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        work->unk4.vy -= 0x12C;
        D_801E92F8.vx = work->unk4.vx;
        D_801E92F8.vy = work->unk4.vy;
        D_801E92F8.vz = work->unk4.vz;
        func_801E8284(&D_801E84E8, (u8 *)ex->btlfx_getFileData(entry->file, 0xD));
        work->unkC = 0x1000;
    } else {
        frame = entry->frame;
        ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->unk4, (s16 *)&D_801E9300);
        if (frame < 0x20) {
            if ((s32)frame >= 0x18) {
                work->unkC -= 0x400;
            }
            func_801E7724(&D_801E91E8, 0x10C, frame * 4);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xE), 0, NULL, 0, 0x1000, work->unkC, 1, (u8 *)&D_801E91E8, 0);
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
        D_801E8348 = env;
        ex->btlfx_initEffModels(D_801E8358, 2);
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unk48);
        work->unk34.vx = 0x1000;
        work->unk34.vy = 0x1000;
        work->unk34.vz = 0x1000;
        work->unk58 = 0;
        work->unk1C.vx = 0;
        work->unk1C.vy = 0;
        work->unk1C.vz = 0;
        work->unk24.vx = work->unk48.vx;
        work->unk24.vy = work->unk48.vy;
        work->unk24.vz = work->unk48.vz;
        work->unk24.vy = -0x3CA;
        work->unk64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 8), 1);
        work->unk66 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 1);
        work->unk5C = -1;
        work->unk60 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 8:
            work->unk50.vx = work->unk48.vx;
            work->unk50.vy = work->unk48.vy;
            work->unk50.vz = work->unk48.vz;
            work->unk50.vy -= 0xFA;
            work->unk0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk50);
            work->unk0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk50);
            work->unk0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk50);
            work->unk0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk50);
            work->unk0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk50);
            work->unk0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk50);
            work->unk0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk50);
            if (work->unk0[0] != NULL) {
                work->unk0[0]->unk14 = (s32)D_801E84F8;
                work->unk0[0]->unk12 = 5;
                work->unk0[0]->unkC = 0x90;
            }
            if (work->unk0[1] != NULL) {
                work->unk0[1]->unk14 = (s32)D_801E87C8;
                work->unk0[1]->unk12 = 5;
                work->unk0[1]->unkC = 0x90;
            }
            if (work->unk0[2] != NULL) {
                work->unk0[2]->unk14 = (s32)D_801E8A98;
                work->unk0[2]->unk12 = 5;
                work->unk0[2]->unkC = 0x90;
            }
            if (work->unk0[3] != NULL) {
                work->unk0[3]->unk14 = (s32)D_801E8D68;
                work->unk0[3]->unk12 = 0x10;
                work->unk0[3]->unkC = 0x18;
            }
            if (work->unk0[4] != NULL) {
                work->unk0[4]->unk14 = (s32)D_801E8EE8;
                work->unk0[4]->unk12 = 0x10;
                work->unk0[4]->unkC = 0x18;
            }
            if (work->unk0[5] != NULL) {
                work->unk0[5]->unk14 = (s32)D_801E9068;
                work->unk0[5]->unk12 = 0x10;
                work->unk0[5]->unkC = 0x18;
            }
            break;
        case 0:
            work->unk5C = 4;
            break;
        case 0x13:
            work->unk60 = 0x10;
            break;
        }
        if ((u32)frame < 0x23) {
            t = frame;
            if (frame >= 9) {
                if (work->unk58 >= 4) {
                    w = 0;
                } else {
                    w = 0x1000 - (work->unk58 << 12) / 3;
                }
                work->unk58++;
            } else {
                w = 0x1000;
            }
            rot.vx = work->unk1C.vx;
            rot.vy = work->unk1C.vy;
            rot.vz = work->unk1C.vz;
            pos.vx = work->unk24.vx;
            pos.vy = work->unk24.vy;
            pos.vz = work->unk24.vz;
            if ((u32)t < 8) {
                pos.vy += frame * 4;
                work->unk24.vy = pos.vy;
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
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&work->unk34, work->unk64, work->unk66, w);
            if (work->unk5C >= 0 || work->unk60 >= 0) {
                if (work->unk5C >= 0) {
                    work->unk44 = func_801E822C(-0x80, 0, 4 - work->unk5C, 4);
                    work->unk5C--;
                }
                if (work->unk60 >= 0) {
                    work->unk44 = func_801E822C(0, -0x80, 0x10 - work->unk60, 0x10);
                    work->unk60--;
                }
                ex->Hi_ModifyEffModelAbr(work->unk64, 1);
                ex->Hi_ModifyEffModelRGB(work->unk64, work->unk44, work->unk44, work->unk44);
            }
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E81E8(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8348)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E822C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8284(Fx1e6Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1e6Point *)data;
}

s32 func_801E829C(Fx1e6Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1e6Point *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}
