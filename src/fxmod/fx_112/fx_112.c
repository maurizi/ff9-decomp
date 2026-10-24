#include "fxmod/fx_112.h"

void func_801E7718(s32 x, s32 y, s32 w, s32 h, s32 dx, s32 dy) {
    Unk800F78A4 *env = D_801E8050;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT r;

    r.x = env->dispEnv->disp.x + x - (w >> 1);
    r.y = env->dispEnv->disp.y + y - (h >> 1);
    r.w = w;
    r.h = h;
    ex->MoveImage(&r, dx, dy);
}

INCLUDE_RODATA("asm/us/fx_112/nonmatchings/fx_112", fx_112_headerPtr);

s32 fx_112_op80(s32 mode, Fx112Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 k;
    s32 t;
    s32 s;
    Pool6CEntry *p;
    Pool6CEntry *q;
    Fx112Locals u;

    if (mode == 0) {
        *arg4 = 0x54;
    } else if (mode == 1) {
        D_801E8050 = env;
        ex->btlfx_initEffModels(D_801E8060, 1);
        work->unk20 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 1), 0x137, 0xFFFF, 0, 0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        ex->btlfx_getUnitRot(0, (s16 *)&work->unkC);
        ex->btlfx_getUnitScale(0, (VECTOR *)&work->unk14);
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unk38);
        work->unk38.vy += 0x80;
        env->camera->points[3].vx = work->unk38.vx;
        env->camera->points[3].vy = work->unk38.vy;
        env->camera->points[3].vz = work->unk38.vz;
        work->unk0 = 0;
    } else {
    frame = entry->frame;
    switch (work->unk0) {
    case 0:
        if (frame == 0) {
            u.s0.scale0.vx = u.s0.scale0.vz = work->unk14.vx;
            u.s0.scale0.vy = work->unk14.vx + 0x1CC;
            ex->btlfx_setUnitScale(0, (s32 *)&u.s0.scale0);
        }
        if (frame == 3) {
            func_801E7718(0xA0, 0x8C, 0x80, 0xC0, 0x1C0, 0x100);
        }
        if (frame == 4) {
            ex->btlfx_fadeUnitById(0, 0);
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk40);
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk40);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        } else if (frame >= 5) {
            t = ((frame - 4) << 12) / 28;
            ex->btlfx_getUnitBonePos(0x10, 9, (u16 *)&u.s0.a);
            ex->btlfx_getUnitBonePos(0x10, 0x1C, (u16 *)&u.s0.b);
            ex->btlfx_aimAngles((s16 *)&u.s0.a, (s16 *)&u.s0.b, (s16 *)&u.s0.rot);
            u.s0.rot.vy += 0x800;
            u.s0.pos.vx = (u.s0.a.vx + u.s0.b.vx) >> 1;
            u.s0.pos.vy = (u.s0.a.vy + u.s0.b.vy) >> 1;
            u.s0.pos.vz = (u.s0.a.vz + u.s0.b.vz) >> 1;
            u.s0.scale.vx = ((ex->math_sin(t * 5 + 0x400) / 3 + 0x1400) * (0x1000 - (t >> 1))) >> 12;
            u.s0.scale.vy = ((ex->math_sin(t * 5) / 3 + 0x1400) * (0x1000 - (t >> 1))) >> 12;
            u.s0.scale.vz = ((ex->math_cos(t * 6) / 3 + 0x1400) * (0x1000 - (t >> 1))) >> 12;
            ex->Hi_DrawEffModel(&u.s0.rot, &u.s0.pos, (s32)&u.s0.scale, work->unk20);
        }
        if (frame >= 0x20) {
            work->unk0 = 1;
            entry->frame = -1;
        }
        break;
    case 1:
        t = (frame << 12) / 9;
        ex->btlfx_getUnitPos(0x10, (s16 *)&u.s1.pos);
        ex->btlfx_getUnitBonePos(0x10, 0x1C, (u16 *)&u.s1.bone);
        ex->btlfx_lerpVec((s16 *)&u.s1.bone, (s16 *)&u.s1.pos, t, (s16 *)&u.s1.v);
        s = ex->math_sin((t * 0x992) >> 12);
        u.s1.rot.vx = -frame << 8;
        u.s1.rot.vy = u.s1.rot.vz = frame * 6;
        u.s1.scale.vx = u.s1.scale.vy = u.s1.scale.vz = ((0x1000 - t) * 0x1400) >> 13;
        u.s1.v.vy -= s >> 5;
        u.s1.tr.vx = u.s1.v.vx;
        u.s1.tr.vy = u.s1.v.vy;
        u.s1.tr.vz = u.s1.v.vz;
        ex->Hi_DrawEffModel(&u.s1.rot, &u.s1.tr, (s32)&u.s1.scale, work->unk20);
        if (frame >= 9) {
            work->unk0 = 2;
            entry->frame = -1;
        }
        break;
    case 2:
        ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&work->unk48);
        k = frame / 5;
        if (frame == k * 5) {
            q = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk48);
            if (q != NULL) {
                q->unk14 = (s32)D_801E8130[k];
                q->unk12 = 0x14;
                q->unkC = 0x24;
                q->unk22 = 0x80;
            }
        }
        if (frame >= 0x13) {
            work->unk0 = 3;
            entry->frame = -1;
        }
        break;
    case 3:
        if (frame >= 0x10) {
            work->unk0 = 4;
            entry->frame = -1;
            work->unk50 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk48);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        break;
    case 4:
        ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&work->unk48);
        ex->btlfx_getUnitBoneMatrix(0x10, 4, &u.s4.m);
        ex->btlfx_matrixAngles((MATRIX *)&u.s4.m, (s16 *)&u.s4.rot);
        if (work->unk50 != NULL) {
            work->unk50->unk5C.vx = u.s4.rot.vx;
            work->unk50->unk5C.vy = u.s4.rot.vy;
            work->unk50->unk5C.vz = u.s4.rot.vz;
        }
        if (frame < 0xD) {
            if (frame == 1) {
                ex->btlfx_fadeUnitById(0, 0x80);
            }
            ex->btlfx_lerpVec((s16 *)&work->unk48, (s16 *)&work->unk4, (frame << 12) / 12, (s16 *)&u.s4.v);
            u.s4.v.vy -= ex->math_sin((frame << 12) / 12 >> 1) >> 3;
            ex->btlfx_setUnitPos(0, (s16 *)&u.s4.v);
            u.s4.scale.vx = u.s4.scale.vy = u.s4.scale.vz =
                work->unk14.vx / 2 + ((work->unk14.vx / 2 * ((frame << 12) / 12)) >> 12);
            ex->btlfx_setUnitScale(0, (s32 *)&u.s4.scale);
            u.s4.r.vx = work->unkC.vx + ((0x1000 - (frame << 12) / 12) >> 1);
            u.s4.r.vy = work->unkC.vy - (0x1000 - (frame << 12) / 12) / 3;
            u.s4.r.vz = work->unkC.vz;
            ex->btlfx_setUnitRot(0, (s16 *)&u.s4.r);
        }
        if (frame >= 0x10) {
            return 1;
        }
        break;
    }
    }
    return 0;
}
