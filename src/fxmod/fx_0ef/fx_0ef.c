#include "fxmod/fx_0ef.h"

s32 fx_0ef_op80(s32 mode, Fx0efWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0efSpawn *o;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x68;
    } else if (mode == 1) {
        D_801E7B4C = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk38);
    } else {
        o = D_801E7AC8;
        frame = entry->frame;
        for (i = 0; i < 5; i++, o++) {
            if (o->unk2 == frame) {
                (work->unk40 + i)->vx = work->unk38.vx;
                (work->unk40 + i)->vy = work->unk38.vy;
                (work->unk40 + i)->vz = work->unk38.vz;
                (work->unk40 + i)->vx += o->unk4[0];
                (work->unk40 + i)->vy += o->unk4[1];
                (work->unk40 + i)->vz += o->unk4[2];
                work->unk0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, o->unk0), (s32)&work->unk40[i]);
                work->unk14[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->unk40[i]);
                if (work->unk0[i] != NULL) {
                    work->unk0[i]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
                if (work->unk14[i] != NULL) {
                    work->unk14[i]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_0ef_op81(s32 mode, Fx0efWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E7B4C = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unk8);
        work->unk8.vy += 0x64;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk8);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->unk8);
        if (work->unk4 != NULL) {
            work->unk4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

void func_801E7A4C(Fx0efPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0efPoint *)data;
}

s32 func_801E7A64(Fx0efPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx0efPoint *p;

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

INCLUDE_RODATA("asm/us/fx_0ef/nonmatchings/fx_0ef", fx_0ef_headerPtr);
