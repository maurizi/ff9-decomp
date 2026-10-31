#include "fxmod/fx_0ef.h"

s32 fx_0ef_op80(s32 mode, Fx0efWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0efSpawn *o;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x68;
    } else if (mode == 1) {
        fx_0ef_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos38);
    } else {
        o = fx_0ef_spawns;
        frame = entry->frame;
        for (i = 0; i < 5; i++, o++) {
            if (o->frame2 == frame) {
                (work->emitPos40 + i)->vx = work->pos38.vx;
                (work->emitPos40 + i)->vy = work->pos38.vy;
                (work->emitPos40 + i)->vz = work->pos38.vz;
                (work->emitPos40 + i)->vx += o->offset4[0];
                (work->emitPos40 + i)->vy += o->offset4[1];
                (work->emitPos40 + i)->vz += o->offset4[2];
                work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, o->block0), (s32)&work->emitPos40[i]);
                work->emit14[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->emitPos40[i]);
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
                if (work->emit14[i] != NULL) {
                    work->emit14[i]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
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
        fx_0ef_env = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos8);
        work->pos8.vy += 0x64;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

static void fxm_posPathInit(Fx0efPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx0efPoint *)data;
}

static s32 fxm_posPathGet(Fx0efPath *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx0efPoint *p;

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

INCLUDE_RODATA("asm/us/fx_0ef/nonmatchings/fx_0ef", fx_0ef_headerPtr);
