#include "fxmod/fx_194.h"

void func_801E7704(SVECTOR *v) {
    s32 y = v->vy;
    s32 x = v->vx;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7DFC);
    s32 nx = x + ((ex->math_cos(D_801E7E04 + y * 0x18) * D_801E7E00) >> 13);
    s32 ny = y + ((ex->math_sin(D_801E7E04 + x * 0x18) * D_801E7E00) >> 12);

    v->vx = nx;
    v->vy = ny;
}

void func_801E77DC(s32 frame, s32 amp, SVECTOR *v) {
    s32 d;

    if (frame == 2) {
        d = (amp * 16) >> 7;
        v->vx -= d;
        v->vz -= d;
        v->vy -= d;
    }
    if (frame >= 0x13 && frame < 0x19) {
        SVECTOR *t = &D_801E7D78[frame - 0x13];
        v->vx += (t->vx * amp) >> 7;
        v->vy += (t->vy * amp) >> 7;
        v->vz += (t->vz * amp) >> 7;
    }
}

s32 fx_194_op80(s32 mode, Fx194Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR v;
    Pool6CEntry *p;
    s32 frame;
    s32 c;
    s32 t;
    s32 state;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7DFC = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk4);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = 0x200;
        }
        ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unkC);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = 0x200;
        }
        work->unk14.vx = work->unk4.vx;
        work->unk14.vy = work->unk4.vy;
        work->unk14.vz = work->unk4.vz;
        env->camera->unk0[3].vx = work->unk14.vx;
        env->camera->unk0[3].vy = work->unk14.vy;
        env->camera->unk0[3].vz = work->unk14.vz;
    } else {
        frame = entry->unk0;
        state = work->unk0;
        switch (state) {
        case 0:
            if (frame >= 0x13 && frame < 0x19) {
                s32 k = D_801E7DA8[frame - 0x13] * 0xFF / 100;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, k, k, k);
            }
            v.vx = work->unk14.vx;
            v.vy = work->unk14.vy;
            v.vz = work->unk14.vz;
            func_801E77DC(frame, 0x300, &v);
            env->camera->unk0[3].vx = v.vx;
            env->camera->unk0[3].vy = v.vy;
            env->camera->unk0[3].vz = v.vz;
            if (frame >= 0x18) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = frame << 7;
            if (frame < 4) {
                s32 k = 0xFF - ((frame * 0xFF) >> 2);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, k, k, k);
            }
            t >>= 2;
            D_801E7E00 = (ex->math_cos(t) * 0xC) >> 12;
            D_801E7E04 = frame << 6;
            col.r = col.g = col.b = (ex->math_cos(t) * 0x64) >> 12;
            scale.vx = 0x1080;
            scale.vy = 0x1200;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->func_800E6764(3, 0xA0, 0x73, 0x20, 0x17, 0xA, 0xA, 5, 5, &rot, &scale, &col, state,
                              func_801E7704);
            if (frame >= 0x20) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_194_op81(s32 mode, Fx194Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7DFC = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk4);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = 0x200;
        }
        env->camera->unk0[3].vx = work->unk4.vx;
        env->camera->unk0[3].vy = work->unk4.vy;
        env->camera->unk0[3].vz = work->unk4.vz;
    } else {
        s32 frame = entry->unk0;
        if (work->unk0 == 0 && frame >= 0x20) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_194/nonmatchings/fx_194", fx_194_headerPtr);
